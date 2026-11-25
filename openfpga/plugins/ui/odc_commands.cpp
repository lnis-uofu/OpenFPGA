/********************************************************************
 * The commands of ODC (see odc_commands.h).
 *
 * The ODC of a session is a file of the user, read with the default
 * constraints of the device package (R-46). Each time something it depends
 * on is read (the netlist, the architectures, the package), it is resolved
 * again against what is known, and the files of the tools are written in the
 * folder odc/ of the current directory (R-40):
 * - constraints.sdc, given to VPR as filename.sdc_file;
 * - vpr_constraints.xml, given as filename.read_vpr_constraints_file;
 * - pin_constraints.xml, given to the OpenFPGA commands that take
 *   --pin_constraints_file;
 * - synthesis.ys, given to the synthesis at the line ${ODC_SYNTHESIS}.
 * Before VPR makes the device, the checks run, and an error stops the
 * command, with the line of the constraint.
 *******************************************************************/
#include "odc_commands.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "command_exit_codes.h"
#include "nlohmann/json.hpp"
#include "odc.h"
#include "odc_design.h"
#include "staged_flow.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

namespace {

namespace fs = std::filesystem;
using json = nlohmann::ordered_json;

const int CONSTRAINTS_SCHEMA_VERSION = 1;
const char* const SDC_OPTION = "filename.sdc_file";
const char* const VPR_OPTION = "filename.read_vpr_constraints_file";

/* The ODC of a session, kept by the plugin (OpenfpgaContext::extension) */
struct OdcState {
  bool loaded = false;
  std::string file; /* the file of the user, as a whole path */
  bool lenient = false;
  std::string defaults; /* the default constraints of the package, or empty */
  odc::Constraints constraints;
  odc::Resolution resolution;
  OdcDesignData data;
  std::string folder; /* where the files of the tools go */
  /* The files written, by name, with their text */
  std::map<std::string, std::string> written;
};

std::string whole(const std::string& path) {
  std::error_code error;
  return fs::absolute(path, error).lexically_normal().string();
}

std::string option_value(const OpenFpgaSession& session, const char* name) {
  const AppOptionValue* option = session.app_options().find(name);
  return nullptr == option ? std::string() : option->string_value;
}

std::string text_of(const Command& cmd, const CommandContext& context,
                    const char* name) {
  const CommandOptionId opt = cmd.option(name);
  return context.option_enable(cmd, opt) ? context.option_value(cmd, opt)
                                         : std::string();
}

bool uses_vpr_constraints(const odc::Constraints& constraints) {
  for (const odc::Command& command : constraints.commands) {
    if (odc::Kind::FLOORPLAN == command.kind ||
        odc::Kind::ROUTING == command.kind ||
        odc::Consumer::VPR_CONSTRAINTS == command.consumer) {
      return true;
    }
  }
  return false;
}

/* Read the file of the user with the defaults of the package. The errors
 * are logged */
bool load(const std::string& file, const bool lenient,
          const std::string& defaults, odc::Constraints& constraints) {
  odc::ReadOptions options;
  options.lenient = lenient;
  constraints = odc::read_odc_file(file, options);
  if (!defaults.empty()) {
    odc::ReadOptions strict;
    constraints =
      odc::merge_defaults(odc::read_odc_file(defaults, strict), constraints);
  }
  for (const odc::Diagnostic& diagnostic : constraints.diagnostics) {
    if (odc::Diagnostic::ERROR == diagnostic.level) {
      VTR_LOG_ERROR("%s\n", diagnostic.text().c_str());
    } else {
      VTR_LOG_WARN("%s\n", diagnostic.text().c_str());
    }
  }
  return !constraints.has_errors();
}

/* Write a file of the tools if its text changed; remove it when there is
 * nothing for it. Return true if what it holds changed */
bool write_output(OdcState& state, const std::string& name,
                  const std::string& text) {
  const fs::path path = fs::path(state.folder) / name;
  const auto found = state.written.find(name);
  const bool changed =
    state.written.end() == found ? !text.empty() : found->second != text;
  std::error_code error;
  if (text.empty()) {
    fs::remove(path, error);
    state.written.erase(name);
    return changed;
  }
  fs::create_directories(state.folder, error);
  std::ofstream(path) << text;
  state.written[name] = text;
  return changed;
}

/* Give a file to VPR through its option. A file that changed where it is
 * makes the device again */
void give_to_vpr(OpenFpgaSession& session, OdcState& state, const char* option,
                 const std::string& name, const std::string& text) {
  const bool changed = write_output(state, name, text);
  const std::string path = (fs::path(state.folder) / name).string();
  const std::string current = option_value(session, option);
  std::string error;
  if (text.empty()) {
    if (current == path) {
      session.reset_app_option(option);
    }
    return;
  }
  if (current != path) {
    session.set_app_option(option, path, error);
  } else if (changed) {
    flow_input_file_changed(session);
  }
}

/* Resolve the constraints against what is known, and give the files to the
 * tools. With errors, nothing is given: each error is logged, and the reason
 * names the first one */
bool apply(OpenFpgaSession& session, OdcState& state, std::string& reason) {
  state.resolution = odc::resolve(state.constraints, state.data.design());
  size_t errors = 0;
  for (const odc::Diagnostic& diagnostic : state.resolution.diagnostics) {
    if (odc::Diagnostic::ERROR == diagnostic.level) {
      VTR_LOG_ERROR("%s\n", diagnostic.text().c_str());
      if (0 == errors++) {
        reason = diagnostic.text();
      }
    }
  }
  if (0 != errors) {
    reason = "the constraints of '" + state.file + "' do not fit: " + reason +
             (1 < errors ? " (and " + std::to_string(errors - 1) + " more)"
                         : std::string());
    return false;
  }
  give_to_vpr(session, state, SDC_OPTION, "constraints.sdc",
              odc::write_sdc(state.constraints));
  give_to_vpr(session, state, VPR_OPTION, "vpr_constraints.xml",
              odc::write_vpr_constraints(state.resolution));
  write_output(state, "pin_constraints.xml",
               odc::write_pin_constraints(state.resolution));
  const std::string yosys = odc::write_yosys(state.constraints);
  write_output(state, "synthesis.ys", yosys);
  SessionConstraints given;
  given.source = state.file;
  given.synthesis_script = yosys;
  const odc::SynthesisOptions options =
    odc::synthesis_options(state.constraints);
  given.synthesis_top = options.top;
  given.synthesis_defines = options.defines;
  if (state.written.count("pin_constraints.xml")) {
    given.pin_constraints_file =
      (fs::path(state.folder) / "pin_constraints.xml").string();
  }
  given.timing = state.constraints.has(odc::Kind::TIMING);
  given.vpr_constraints = uses_vpr_constraints(state.constraints);
  session.set_constraints(given);
  return true;
}

/* The synthesis part of the constraints, to tell that it changed */
std::string synthesis_part(const SessionConstraints& constraints) {
  std::string text =
    constraints.synthesis_script + "\n" + constraints.synthesis_top;
  for (const auto& define : constraints.synthesis_defines) {
    text += "\n" + define.first + "=" + define.second;
  }
  return text;
}

/* A change of the synthesis constraints drops the circuit (R-44) */
void drop_circuit_if(OpenFpgaSession& session, const std::string& before) {
  if (before != synthesis_part(session.constraints()) &&
      session.has_input(OpenFpgaInput::CIRCUIT)) {
    VTR_LOG(
      "The synthesis constraints changed: the circuit is dropped, synthesize "
      "it again\n");
    session.drop_input(OpenFpgaInput::CIRCUIT);
  }
}

/* Take the ODC away: the options it set, the constraints of the session */
void clear(OpenFpgaSession& session, OdcState& state) {
  for (const auto& [option, name] :
       {std::make_pair(SDC_OPTION, "constraints.sdc"),
        std::make_pair(VPR_OPTION, "vpr_constraints.xml")}) {
    if (option_value(session, option) ==
        (fs::path(state.folder) / name).string()) {
      session.reset_app_option(option);
    }
  }
  session.set_constraints(SessionConstraints());
  state = OdcState();
}

int read_odc_command(OpenfpgaContext& ctx, const Command& cmd,
                     const CommandContext& context) {
  OpenFpgaSession& session = ctx.mutable_session();
  OdcState& state = ctx.extension<OdcState>();
  OdcState next;
  next.loaded = true;
  next.file = whole(text_of(cmd, context, "file"));
  next.lenient = context.option_enable(cmd, cmd.option("lenient"));
  next.folder = whole("odc");
  next.written = state.written;
  next.data = odc_design_data(session);
  next.defaults = next.data.defaults_file;
  if (!load(next.file, next.lenient, next.defaults, next.constraints)) {
    VTR_LOG_ERROR(
      "The ODC '%s' is not read: the session keeps its "
      "constraints\n",
      next.file.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  /* A raw file and the ODC cannot set the same thing (R-45) */
  const auto refuse_raw = [&](const char* option, const std::string& own,
                              const char* what) {
    const std::string current = option_value(session, option);
    if (!current.empty() && current != (fs::path(next.folder) / own).string()) {
      VTR_LOG_ERROR(
        "The ODC '%s' has %s commands, and the session has the file '%s' "
        "(%s): take it away first (reset_app_option --name %s)\n",
        next.file.c_str(), what, current.c_str(), option, option);
      return true;
    }
    return false;
  };
  if ((next.constraints.has(odc::Kind::TIMING) &&
       refuse_raw(SDC_OPTION, "constraints.sdc", "timing")) ||
      (uses_vpr_constraints(next.constraints) &&
       refuse_raw(VPR_OPTION, "vpr_constraints.xml",
                  "floorplanning, routing or pin"))) {
    return CMD_EXEC_FATAL_ERROR;
  }
  const std::string before = synthesis_part(session.constraints());
  std::string reason;
  if (!apply(session, next, reason)) {
    VTR_LOG_ERROR(
      "The ODC '%s' is not read, as %s: the session keeps its "
      "constraints\n",
      next.file.c_str(), reason.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  for (const odc::Diagnostic& diagnostic : next.resolution.diagnostics) {
    VTR_LOG_WARN("%s\n", diagnostic.text().c_str());
  }
  state = next;
  drop_circuit_if(session, before);
  size_t checked = 0;
  for (const bool done : state.resolution.checked) {
    checked += done ? 1 : 0;
  }
  VTR_LOG(
    "Read %zu constraints from '%s' (%zu checked; the others wait for "
    "the netlist or the device)\n",
    state.constraints.commands.size(), state.file.c_str(), checked);
  return CMD_EXEC_SUCCESS;
}

int write_odc_command(const OpenfpgaContext& ctx, const Command& cmd,
                      const CommandContext& context) {
  const OdcState* state = ctx.find_extension<OdcState>();
  if (nullptr == state || !state->loaded) {
    VTR_LOG_ERROR("The session has no ODC: use read_odc first\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  const std::string file = text_of(cmd, context, "file");
  std::ofstream out(file);
  if (!out) {
    VTR_LOG_ERROR("Cannot write the file '%s'\n", file.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  out << odc::write_odc(state->constraints);
  VTR_LOG("Wrote the ODC of the session in '%s'\n", file.c_str());
  return CMD_EXEC_SUCCESS;
}

int clear_odc_command(OpenfpgaContext& ctx, const Command&,
                      const CommandContext&) {
  OdcState& state = ctx.extension<OdcState>();
  if (!state.loaded) {
    VTR_LOG("The session has no ODC\n");
    return CMD_EXEC_SUCCESS;
  }
  OpenFpgaSession& session = ctx.mutable_session();
  const std::string before = synthesis_part(session.constraints());
  const std::string file = state.file;
  clear(session, state);
  drop_circuit_if(session, before);
  VTR_LOG("Took away the ODC '%s'\n", file.c_str());
  return CMD_EXEC_SUCCESS;
}

json diagnostic_json(const odc::Diagnostic& diagnostic) {
  json item;
  item["level"] =
    odc::Diagnostic::ERROR == diagnostic.level ? "error" : "warning";
  item["file"] = diagnostic.file;
  item["line"] = diagnostic.line;
  item["message"] = diagnostic.message;
  return item;
}

json names_json(const std::vector<std::string>& names) {
  json list = json::array();
  for (const std::string& name : names) {
    list.push_back(name);
  }
  return list;
}

int report_constraints_command(const OpenfpgaContext& ctx, const Command& cmd,
                               const CommandContext& context) {
  size_t limit = 50;
  const std::string limit_text = text_of(cmd, context, "limit");
  if (!limit_text.empty()) {
    try {
      limit = static_cast<size_t>(std::stoul(limit_text));
    } catch (const std::exception&) {
      VTR_LOG_ERROR("--limit is a whole number\n");
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  const OdcState* found = ctx.find_extension<OdcState>();
  OdcState state = nullptr == found ? OdcState() : *found;
  json data;
  data["file"] = state.loaded ? json(state.file) : json();
  data["lenient"] = state.lenient;
  data["defaults"] = state.defaults.empty() ? json() : json(state.defaults);
  data["files"] = names_json(state.constraints.files);
  /* The checks run again on what is known now */
  if (state.loaded) {
    state.data = odc_design_data(ctx.session());
    state.resolution = odc::resolve(state.constraints, state.data.design());
  }
  json design;
  design["netlist"] =
    state.data.netlist ? json(state.data.netlist->file) : json();
  design["device"] = nullptr != state.data.device;
  design["io_map"] =
    state.data.io_map_file.empty() ? json() : json(state.data.io_map_file);
  design["problems"] = names_json(state.data.problems);
  data["design"] = design;
  json commands = json::array();
  for (size_t i = 0; i < state.constraints.commands.size(); ++i) {
    const odc::Command& command = state.constraints.commands[i];
    json item;
    item["file"] = command.file;
    item["line"] = command.line;
    item["name"] = command.name;
    item["text"] = command.text();
    item["kind"] = odc::to_string(command.kind);
    item["stage"] = odc::to_string(command.stage);
    item["consumer"] = odc::to_string(command.consumer);
    item["checked"] =
      i < state.resolution.checked.size() && state.resolution.checked[i];
    item["from_defaults"] = command.from_defaults;
    json queries = json::array();
    for (const odc::QueryMatch& match : state.resolution.queries) {
      if (match.command != i) {
        continue;
      }
      json query;
      query["text"] = match.text;
      query["kind"] = match.kind;
      query["checked"] = match.checked;
      std::vector<std::string> some(
        match.matched.begin(),
        match.matched.begin() +
          static_cast<long>(std::min(limit, match.matched.size())));
      query["count"] = match.matched.size();
      query["matched"] = names_json(some);
      query["truncated"] = some.size() < match.matched.size();
      query["nearest"] = names_json(match.nearest);
      queries.push_back(query);
    }
    item["queries"] = queries;
    commands.push_back(item);
  }
  data["commands"] = commands;
  json diagnostics = json::array();
  for (const odc::Diagnostic& diagnostic : state.constraints.diagnostics) {
    diagnostics.push_back(diagnostic_json(diagnostic));
  }
  for (const odc::Diagnostic& diagnostic : state.resolution.diagnostics) {
    diagnostics.push_back(diagnostic_json(diagnostic));
  }
  data["diagnostics"] = diagnostics;
  const odc::TimingCoverage& timing = state.resolution.timing;
  json coverage;
  coverage["checked"] = timing.checked;
  json clocks = json::array();
  for (const odc::Clock& clock : timing.clocks) {
    json item;
    item["name"] = clock.name;
    item["period"] = clock.period;
    item["line"] = state.constraints.commands[clock.command].line;
    item["targets"] = names_json(clock.targets);
    clocks.push_back(item);
  }
  coverage["clocks"] = clocks;
  coverage["unconstrained_clocks"] = names_json(timing.unconstrained_clocks);
  coverage["inputs_without_delay"] = names_json(timing.inputs_without_delay);
  coverage["outputs_without_delay"] = names_json(timing.outputs_without_delay);
  data["timing"] = coverage;
  json outputs;
  for (const char* name : {"constraints.sdc", "vpr_constraints.xml",
                           "pin_constraints.xml", "synthesis.ys"}) {
    const std::string key = fs::path(name).stem().string();
    outputs[key] = state.written.count(name)
                     ? json((fs::path(state.folder) / name).string())
                     : json();
  }
  data["outputs"] = outputs;
  json answer;
  answer["schema_version"] = CONSTRAINTS_SCHEMA_VERSION;
  answer["kind"] = "constraints";
  answer["stage"] = json();
  answer["data"] = data;
  const std::string text = answer.dump();
  ctx.set_query_result(text);
  VTR_LOG("%s\n", text.c_str());
  return CMD_EXEC_SUCCESS;
}

/* Resolve the constraints again before a command that reads a netlist or
 * makes the device, and refuse it on an error of the constraints */
std::string refresh(OpenfpgaContext& ctx, const std::string& netlist_file) {
  OdcState& state = ctx.extension<OdcState>();
  if (!state.loaded) {
    return std::string();
  }
  state.data = odc_design_data(ctx.session(), netlist_file);
  std::string reason;
  if (!apply(ctx.mutable_session(), state, reason)) {
    return reason + "; fix them and read them again, or clear_odc";
  }
  return std::string();
}

void add_hooks(Shell<OpenfpgaContext>& shell) {
  shell.on_before("read_circuit", [](OpenfpgaContext& ctx, const Command& cmd,
                                     const CommandContext& context) {
    return refresh(ctx, whole(text_of(cmd, context, "file")));
  });
  /* Once VPR read the circuit, its blocks are known as VPR names them (the
   * buffers absorbed): the files are made again with them. A change makes the
   * device again; an error stops the next stage */
  shell.on_after("read_circuit", [](OpenfpgaContext& ctx, const Command&,
                                    const CommandContext&, const int status) {
    if (CMD_EXEC_SUCCESS != status) {
      return;
    }
    const std::string reason = refresh(ctx, "");
    if (!reason.empty()) {
      VTR_LOG_ERROR("%s\n", reason.c_str());
      ctx.mutable_session().fail(OpenFpgaStage::DEVICE);
    }
  });
  /* The commands that may make the device */
  for (const char* name :
       {"read_openfpga_arch", "pack", "place", "route", "analysis",
        "read_packing", "read_placement", "read_routing"}) {
    shell.on_before(
      name, [](OpenfpgaContext& ctx, const Command&, const CommandContext&) {
        if (ctx.session().reached(OpenFpgaStage::DEVICE)) {
          return std::string();
        }
        return refresh(ctx, "");
      });
  }
  /* A package brings its I/O map and its default constraints */
  shell.on_after(
    "read_device_package", [](OpenfpgaContext& ctx, const Command&,
                              const CommandContext&, const int status) {
      OdcState& state = ctx.extension<OdcState>();
      if (CMD_EXEC_SUCCESS != status || !state.loaded) {
        return;
      }
      state.data = odc_design_data(ctx.session());
      state.defaults = state.data.defaults_file;
      if (!load(state.file, state.lenient, state.defaults, state.constraints)) {
        return;
      }
      std::string reason;
      if (!apply(ctx.mutable_session(), state, reason)) {
        VTR_LOG_WARN("%s\n", reason.c_str());
      }
    });
  /* The files that the ODC gives are its own (R-45) */
  for (const char* name : {"set_app_option", "reset_app_option"}) {
    shell.on_before(name, [](OpenfpgaContext& ctx, const Command& cmd,
                             const CommandContext& context) {
      const SessionConstraints& constraints = ctx.session().constraints();
      const std::string option = text_of(cmd, context, "name");
      if ((SDC_OPTION == option && constraints.timing) ||
          (VPR_OPTION == option && constraints.vpr_constraints)) {
        return "the option " + option + " is given by the ODC '" +
               constraints.source + "': use clear_odc first";
      }
      return std::string();
    });
  }
}

}  // namespace

void add_odc_commands(Shell<OpenfpgaContext>& shell) {
  ShellCommandClassId cmd_class = shell.add_command_class("Constraints");

  Command read_cmd("read_odc");
  CommandOptionId opt = read_cmd.add_option("file", true, "The ODC file");
  read_cmd.set_option_short_name(opt, "f");
  read_cmd.set_option_require_value(opt, OPT_STRING);
  read_cmd.add_option("lenient", false,
                      "An unknown command is a warning, and is skipped");
  ShellCommandId id = shell.add_command(
    read_cmd,
    "Read the constraints of the design from an ODC file "
    "(docs/spec_odc.md): timing, synthesis, floorplanning, routing and pins. "
    "They replace the ones of the session");
  shell.set_command_class(id, cmd_class);
  shell.set_command_execute_function(id, read_odc_command);

  Command write_cmd("write_odc");
  opt = write_cmd.add_option("file", true, "The file to write");
  write_cmd.set_option_short_name(opt, "f");
  write_cmd.set_option_require_value(opt, OPT_STRING);
  id = shell.add_command(write_cmd,
                         "Write the ODC of the session, one command per line");
  shell.set_command_class(id, cmd_class);
  shell.set_command_const_execute_function(id, write_odc_command);

  Command clear_cmd("clear_odc");
  id = shell.add_command(clear_cmd, "Take the ODC of the session away");
  shell.set_command_class(id, cmd_class);
  shell.set_command_execute_function(id, clear_odc_command);

  Command report_cmd("report_constraints");
  opt = report_cmd.add_option(
    "limit", false, "The most objects listed for a query (default: 50)");
  report_cmd.set_option_require_value(opt, OPT_STRING);
  id = shell.add_command(
    report_cmd,
    "Report the constraints of the session (JSON, kind constraints): each "
    "command with its line, the objects it matched, the errors and warnings, "
    "and the coverage of the timing");
  shell.set_command_class(id, cmd_class);
  shell.set_command_const_execute_function(id, report_constraints_command);

  add_hooks(shell);
}

} /* end namespace openfpga */
