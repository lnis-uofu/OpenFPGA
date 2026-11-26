/********************************************************************
 * The commands of synthesis (see synthesis_commands.h)
 *******************************************************************/
#include "synthesis_commands.h"

#include <fstream>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

#include "command_exit_codes.h"
#include "design_hierarchy.h"
#include "design_synthesis.h"
#include "device_package.h"
#include "nlohmann/json.hpp"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

namespace fs = std::filesystem;
using json = nlohmann::ordered_json;

namespace {

const int SYNTHESIS_SCHEMA_VERSION = 1;
const int DESIGN_HIERARCHY_SCHEMA_VERSION = 1;

/* The words of a list given as "a,b,c" */
std::vector<std::string> list_of(const std::string& text) {
  std::vector<std::string> words;
  std::stringstream stream(text);
  std::string word;
  while (std::getline(stream, word, ',')) {
    word.erase(0, word.find_first_not_of(" \t"));
    word.erase(word.find_last_not_of(" \t") + 1);
    if (!word.empty()) {
      words.push_back(word);
    }
  }
  return words;
}

std::string value_of(const Command& cmd, const CommandContext& context,
                     const char* name) {
  const CommandOptionId opt = cmd.option(name);
  return context.option_enable(cmd, opt) ? context.option_value(cmd, opt)
                                         : std::string();
}

bool switch_of(const Command& cmd, const CommandContext& context,
               const char* name) {
  return context.option_enable(cmd, cmd.option(name));
}

std::string whole(const fs::path& path) {
  std::error_code error;
  return fs::absolute(path, error).lexically_normal().string();
}

/* The device package of the session, read again from its directory, or none
 */
bool session_package(const OpenfpgaContext& ctx, DevicePackage& package) {
  const std::string data = ctx.session().device_package();
  if (data.empty()) {
    return false;
  }
  package = load_device_package(
    json::parse(data)["directory"].get<std::string>(), false);
  return true;
}

/* Run a script with the command yosys of the shell */
int run_yosys(Shell<OpenfpgaContext>* shell, OpenfpgaContext& ctx,
              const fs::path& script_file) {
  const ShellCommandId yosys = shell->command("yosys");
  if (ShellCommandId::INVALID() != yosys && shell->command_removed(yosys)) {
    VTR_LOG_ERROR("The command 'yosys' was removed: Yosys cannot run\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (ShellCommandId::INVALID() == yosys) {
    VTR_LOG_ERROR(
      "This build of OpenFPGA has no Yosys (OPENFPGA_WITH_YOSYS): Yosys "
      "cannot run\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  const std::string run = "yosys --file \"" + script_file.string() + "\"";
  return shell->execute_command(run.c_str(), ctx);
}

/* Write the script of a plan, run Yosys on it unless no_run, and read the
 * netlist as the circuit when the architectures are read, unless no_read.
 * Fills the answer */
int run_plan(Shell<OpenfpgaContext>* shell, OpenfpgaContext& ctx,
             const SynthesisPlan& plan, const bool no_run, const bool no_read,
             json& answer) {
  std::error_code error;
  fs::create_directories(plan.script_file.parent_path(), error);
  /* One script in the folder: not one for each top module */
  for (const auto& entry :
       fs::directory_iterator(plan.script_file.parent_path(), error)) {
    if (".ys" == entry.path().extension() &&
        entry.path().filename() != plan.script_file.filename()) {
      fs::remove(entry.path(), error);
    }
  }
  {
    std::ofstream out(plan.script_file);
    out << plan.script;
  }
  fs::remove(plan.netlist, error);
  answer["top"] = plan.top;
  answer["netlist"] = whole(plan.netlist);
  answer["script"] = whole(plan.script_file);
  answer["device_package"] = plan.package.empty() ? json() : json(plan.package);
  answer["lut_size"] = plan.lut_size;
  json files = json::array();
  for (const fs::path& file : plan.files) {
    files.push_back(whole(file));
  }
  answer["files"] = files;
  answer["constraints"] = plan.constraints;
  answer["synthesized"] = !no_run;
  answer["circuit_read"] = false;
  if (no_run) {
    VTR_LOG("Wrote the script '%s' (not run)\n", plan.script_file.c_str());
    return CMD_EXEC_SUCCESS;
  }
  if (CMD_EXEC_SUCCESS != run_yosys(shell, ctx, plan.script_file)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  if (!fs::is_regular_file(plan.netlist, error)) {
    VTR_LOG_ERROR("Yosys wrote no netlist '%s'\n", plan.netlist.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  const OpenFpgaSession& session = ctx.session();
  if (!no_read && session.has_input(OpenFpgaInput::VPR_ARCH) &&
      session.has_input(OpenFpgaInput::OPENFPGA_ARCH)) {
    const std::string read =
      "read_circuit --file \"" + plan.netlist.string() + "\"";
    if (CMD_EXEC_SUCCESS != shell->execute_command(read.c_str(), ctx)) {
      return CMD_EXEC_FATAL_ERROR;
    }
    answer["circuit_read"] = true;
  }
  return CMD_EXEC_SUCCESS;
}

/* What the constraints of the session (ODC) give to a synthesis: its Yosys
 * commands, the top module and the defines (docs/spec_odc.md, R-22) */
void apply_constraints(const OpenFpgaSession& session,
                       SynthesisRequest& request) {
  const SessionConstraints& constraints = session.constraints();
  request.constraints = constraints.synthesis_script;
  if (!constraints.synthesis_top.empty() &&
      constraints.synthesis_top != request.top) {
    VTR_LOG("The top module is '%s', as set_synth_option -top of '%s' says\n",
            constraints.synthesis_top.c_str(), constraints.source.c_str());
    request.top = constraints.synthesis_top;
  }
  for (const auto& define : constraints.synthesis_defines) {
    bool replaced = false;
    for (auto& existing : request.defines) {
      if (existing.first == define.first) {
        existing.second = define.second;
        replaced = true;
      }
    }
    if (!replaced) {
      request.defines.push_back(define);
    }
  }
}

/* Read the ODC file of a design with the command read_odc (R-47), which a
 * plugin gives */
int read_design_odc(Shell<OpenfpgaContext>* shell, OpenfpgaContext& ctx,
                    const DesignDirectory& design) {
  if (design.odc.empty()) {
    return CMD_EXEC_SUCCESS;
  }
  const ShellCommandId read_odc = shell->command("read_odc");
  if (ShellCommandId::INVALID() == read_odc ||
      shell->command_removed(read_odc)) {
    VTR_LOG_ERROR(
      "The design '%s' has an ODC file, and this build has no command "
      "'read_odc' (the plugin ui)\n",
      design.name.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  const std::string read = "read_odc --file \"" + design.odc.string() + "\"";
  return shell->execute_command(read.c_str(), ctx);
}

/* Give the answer to the caller, as a query does */
int give(OpenfpgaContext& ctx, const json& data, const int status) {
  if (CMD_EXEC_SUCCESS != status) {
    return status;
  }
  json answer;
  answer["schema_version"] = SYNTHESIS_SCHEMA_VERSION;
  answer["kind"] = "synthesis";
  answer["stage"] = json();
  answer["data"] = data;
  const std::string text = answer.dump();
  ctx.set_query_result(text);
  VTR_LOG("%s\n", text.c_str());
  return status;
}

int synthesize_command(Shell<OpenfpgaContext>* shell, OpenfpgaContext& ctx,
                       const Command& cmd, const CommandContext& context) {
  SynthesisRequest request;
  SynthesisPlan plan;
  try {
    request.top = value_of(cmd, context, "top");
    for (const std::string& file : list_of(value_of(cmd, context, "files"))) {
      request.files.push_back(whole(file));
    }
    const std::string size = value_of(cmd, context, "lut_size");
    if (!size.empty()) {
      request.lut_size = std::stoi(size);
    }
    for (const std::string& dir :
         list_of(value_of(cmd, context, "include_dirs"))) {
      request.include_dirs.push_back(whole(dir));
    }
    for (const std::string& define :
         list_of(value_of(cmd, context, "defines"))) {
      const size_t eq = define.find('=');
      request.defines.emplace_back(
        define.substr(0, eq),
        std::string::npos == eq ? std::string() : define.substr(eq + 1));
    }
    request.read_options = list_of(value_of(cmd, context, "read_options"));
    const std::string folder = value_of(cmd, context, "output_dir");
    request.folder = whole(folder.empty() ? std::string("synth") : folder);
    apply_constraints(ctx.session(), request);
    DevicePackage package;
    const bool has_package = session_package(ctx, package);
    plan = make_synthesis_plan(request, has_package ? &package : nullptr);
  } catch (const SynthesisRefused& error) {
    VTR_LOG_ERROR("Synthesis refused: %s\n", error.what());
    return CMD_EXEC_FATAL_ERROR;
  } catch (const std::exception& error) {
    VTR_LOG_ERROR("Synthesis refused: %s\n", error.what());
    return CMD_EXEC_FATAL_ERROR;
  }
  json answer;
  answer["design"] = json();
  const int status =
    run_plan(shell, ctx, plan, switch_of(cmd, context, "no_run"),
             switch_of(cmd, context, "no_read"), answer);
  return give(ctx, answer, status);
}

int read_design_command(Shell<OpenfpgaContext>* shell, OpenfpgaContext& ctx,
                        const Command& cmd, const CommandContext& context) {
  DesignDirectory design;
  try {
    design = read_design_directory(value_of(cmd, context, "file"),
                                   value_of(cmd, context, "netlist"));
  } catch (const SynthesisRefused& error) {
    VTR_LOG_ERROR("Synthesis refused: %s\n", error.what());
    return CMD_EXEC_FATAL_ERROR;
  }
  json answer;
  answer["design"] = design.name;
  answer["design_folder"] = whole(design.folder);
  answer["top"] = design.top.empty() ? json() : json(design.top);
  answer["sdc"] = design.sdc.empty() ? json() : json(whole(design.sdc));
  answer["odc"] = design.odc.empty() ? json() : json(whole(design.odc));
  const bool check = switch_of(cmd, context, "check");
  const bool no_read = switch_of(cmd, context, "no_read");
  /* The constraints of the design come first: the synthesis takes its part
   * of them */
  if (!check && CMD_EXEC_SUCCESS != read_design_odc(shell, ctx, design)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  /* A design of a netlist: it is read, nothing is synthesized */
  if (!design.netlist.empty()) {
    answer["netlist"] = whole(design.netlist);
    answer["synthesized"] = false;
    answer["circuit_read"] = false;
    if (!check && !no_read) {
      const std::string read =
        "read_circuit --file \"" + design.netlist.string() + "\"";
      if (CMD_EXEC_SUCCESS != shell->execute_command(read.c_str(), ctx)) {
        return CMD_EXEC_FATAL_ERROR;
      }
      answer["circuit_read"] = true;
    }
    return give(ctx, answer, CMD_EXEC_SUCCESS);
  }
  SynthesisRequest request;
  request.top = design.top;
  request.files = design.rtl;
  request.include_dirs = design.include_dirs;
  request.defines = design.defines;
  request.read_options = design.read_options;
  const std::string size = value_of(cmd, context, "lut_size");
  if (!size.empty()) {
    try {
      request.lut_size = std::stoi(size);
    } catch (const std::exception&) {
      VTR_LOG_ERROR("Synthesis refused: lut_size is a number\n");
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  const std::string folder = value_of(cmd, context, "output_dir");
  request.folder = whole(folder.empty() ? std::string("synth") : folder);
  SynthesisPlan plan;
  try {
    apply_constraints(ctx.session(), request);
    DevicePackage package;
    const bool has_package = session_package(ctx, package);
    plan = make_synthesis_plan(request, has_package ? &package : nullptr);
  } catch (const SynthesisRefused& error) {
    VTR_LOG_ERROR("Synthesis refused: %s\n", error.what());
    return CMD_EXEC_FATAL_ERROR;
  }
  if (check) {
    json files = json::array();
    for (const fs::path& file : plan.files) {
      files.push_back(whole(file));
    }
    answer["files"] = files;
    answer["netlist"] = whole(plan.netlist);
    answer["synthesized"] = false;
    answer["circuit_read"] = false;
    return give(ctx, answer, CMD_EXEC_SUCCESS);
  }
  /* The SDC of the design is the one of VPR (spec_design_directory.md,
   * R-23) */
  if (!design.sdc.empty()) {
    if (ctx.session().constraints().timing) {
      VTR_LOG_ERROR(
        "The design '%s' has an SDC, and the ODC '%s' of the session has "
        "timing commands: use clear_odc first\n",
        design.name.c_str(), ctx.session().constraints().source.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    std::string error;
    if (!ctx.mutable_session().set_app_option("filename.sdc_file",
                                              whole(design.sdc), error)) {
      VTR_LOG_ERROR("%s\n", error.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  const int status = run_plan(
    shell, ctx, plan, switch_of(cmd, context, "no_run"), no_read, answer);
  return give(ctx, answer, status);
}

/* The hierarchy of a design (design_hierarchy.h): its RTL elaborated by a
 * Yosys script of its own, which leaves the flow as it is */
int report_design_hierarchy_command(Shell<OpenfpgaContext>* shell,
                                    OpenfpgaContext& ctx, const Command& cmd,
                                    const CommandContext& context) {
  SynthesisRequest request;
  json answer;
  answer["design"] = json();
  size_t limit = 500;
  fs::path script_file;
  fs::path json_file;
  try {
    const std::string limit_text = value_of(cmd, context, "limit");
    if (!limit_text.empty()) {
      const int given = std::stoi(limit_text);
      if (given < 1 || given > 100000) {
        throw SynthesisRefused("limit is a number from 1 to 100000");
      }
      limit = static_cast<size_t>(given);
    }
    const std::string file = value_of(cmd, context, "file");
    if (!file.empty()) {
      if (!value_of(cmd, context, "top").empty() ||
          !value_of(cmd, context, "files").empty()) {
        throw SynthesisRefused("give --file, or --top and --files, not both");
      }
      const DesignDirectory design = read_design_directory(file, "");
      if (design.rtl.empty()) {
        throw SynthesisRefused("The design '" + design.name +
                               "' has no RTL, only a netlist: there is no "
                               "hierarchy to elaborate");
      }
      answer["design"] = design.name;
      request.top = design.top;
      request.files = design.rtl;
      request.include_dirs = design.include_dirs;
      request.defines = design.defines;
      request.read_options = design.read_options;
    } else {
      request.top = value_of(cmd, context, "top");
      if (request.top.empty()) {
        throw SynthesisRefused(
          "give --file (a design directory), or --top "
          "and --files");
      }
      for (const std::string& name : list_of(value_of(cmd, context, "files"))) {
        request.files.push_back(whole(name));
      }
      for (const std::string& dir :
           list_of(value_of(cmd, context, "include_dirs"))) {
        request.include_dirs.push_back(whole(dir));
      }
      for (const std::string& define :
           list_of(value_of(cmd, context, "defines"))) {
        const size_t eq = define.find('=');
        request.defines.emplace_back(
          define.substr(0, eq),
          std::string::npos == eq ? std::string() : define.substr(eq + 1));
      }
      request.read_options = list_of(value_of(cmd, context, "read_options"));
    }
    /* The top and the defines of the ODC of the session, as the synthesis
     * takes them; its Yosys commands are for the synthesis only */
    apply_constraints(ctx.session(), request);
    request.constraints.clear();
    const std::string folder = value_of(cmd, context, "output_dir");
    const fs::path output =
      whole(folder.empty() ? std::string("hierarchy") : folder);
    script_file = output / "hierarchy.ys";
    json_file = output / "hierarchy.json";
    DevicePackage package;
    const bool has_package = session_package(ctx, package);
    const std::string script = make_hierarchy_script(
      request, has_package ? &package : nullptr, json_file);
    std::error_code error;
    fs::create_directories(output, error);
    std::ofstream(script_file) << script;
    fs::remove(json_file, error);
  } catch (const std::exception& error) {
    VTR_LOG_ERROR("The hierarchy is refused: %s\n", error.what());
    return CMD_EXEC_FATAL_ERROR;
  }
  if (CMD_EXEC_SUCCESS != run_yosys(shell, ctx, script_file)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  nlohmann::json yosys;
  try {
    std::ifstream in(json_file);
    if (!in) {
      throw std::runtime_error("Yosys wrote no file '" + json_file.string() +
                               "'");
    }
    yosys = nlohmann::json::parse(in);
    const json tree = hierarchy_of(yosys, request.top, limit);
    for (auto it = tree.begin(); it != tree.end(); ++it) {
      answer[it.key()] = it.value();
    }
  } catch (const std::exception& error) {
    VTR_LOG_ERROR("The hierarchy cannot be read: %s\n", error.what());
    return CMD_EXEC_FATAL_ERROR;
  }
  json files = json::array();
  for (const fs::path& file : request.files) {
    files.push_back(whole(file));
  }
  answer["files"] = files;
  answer["script"] = whole(script_file);
  answer["json"] = whole(json_file);
  json reply;
  reply["schema_version"] = DESIGN_HIERARCHY_SCHEMA_VERSION;
  reply["kind"] = "design_hierarchy";
  reply["stage"] = json();
  reply["data"] = answer;
  const std::string text = reply.dump();
  ctx.set_query_result(text);
  VTR_LOG("%s\n", text.c_str());
  return CMD_EXEC_SUCCESS;
}

void add_text_option(Command& cmd, const char* name, const bool required,
                     const char* help) {
  const CommandOptionId opt = cmd.add_option(name, required, help);
  cmd.set_option_require_value(opt, OPT_STRING);
}

} /* end anonymous namespace */

void add_synthesis_commands(Shell<OpenfpgaContext>& shell) {
  ShellCommandClassId cmd_class = shell.add_command_class("Synthesis");

  Command synth("synthesize");
  add_text_option(synth, "top", true, "The top module");
  add_text_option(synth, "files", false,
                  "The Verilog files (.v, .sv, .vh), separated by commas");
  add_text_option(synth, "lut_size", false,
                  "The size of the LUTs, when the session has no device "
                  "package (2 to 8)");
  add_text_option(synth, "include_dirs", false,
                  "The folders of `include, separated by commas");
  add_text_option(synth, "defines", false,
                  "Defines NAME=value, separated by commas");
  add_text_option(synth, "read_options", false,
                  "Options of read_verilog (-nolatches), separated by commas");
  add_text_option(synth, "output_dir", false,
                  "The folder of the script and the netlist (synth)");
  synth.add_option("no_run", false,
                   "Write the script and answer the plan, without running "
                   "Yosys");
  synth.add_option("no_read", false, "Do not read the netlist as the circuit");
  ShellCommandId synth_id = shell.add_command(
    synth,
    "Synthesize Verilog into a netlist of the LUTs of the device, with Yosys "
    "and the script of the device package of the session (or a generic one), "
    "and read it as the circuit when the architectures are read");
  shell.set_command_class(synth_id, cmd_class);
  shell.set_command_execute_function(synth_id, synthesize_command);

  Command design("read_design");
  add_text_option(design, "file", true, "The design directory");
  add_text_option(design, "lut_size", false,
                  "The size of the LUTs, when the session has no device "
                  "package (2 to 8)");
  add_text_option(design, "netlist", false,
                  "Read the netlist of this label instead of synthesizing");
  add_text_option(design, "output_dir", false,
                  "The folder of the script and the netlist (synth)");
  design.add_option("check", false,
                    "Check the design and answer what would be synthesized, "
                    "without writing anything");
  design.add_option("no_run", false,
                    "Write the script and answer the plan, without running "
                    "Yosys");
  design.add_option("no_read", false, "Do not read the netlist as the circuit");
  ShellCommandId design_id = shell.add_command(
    design,
    "Read a design directory: synthesize its RTL (or read its netlist) as "
    "synthesize does, and give its SDC to VPR");
  shell.set_command_class(design_id, cmd_class);
  shell.set_command_execute_function(design_id, read_design_command);

  Command hierarchy("report_design_hierarchy");
  add_text_option(hierarchy, "file", false, "The design directory");
  add_text_option(hierarchy, "top", false,
                  "The top module (without a design directory)");
  add_text_option(hierarchy, "files", false,
                  "The Verilog files (.v, .sv, .vh), separated by commas");
  add_text_option(hierarchy, "include_dirs", false,
                  "The folders of `include, separated by commas");
  add_text_option(hierarchy, "defines", false,
                  "Defines NAME=value, separated by commas");
  add_text_option(hierarchy, "read_options", false,
                  "Options of read_verilog (-nolatches), separated by commas");
  add_text_option(hierarchy, "output_dir", false,
                  "The folder of the script and of the JSON of Yosys "
                  "(hierarchy)");
  add_text_option(hierarchy, "limit", false,
                  "The instances of the tree at most, breadth first (500)");
  ShellCommandId hierarchy_id = shell.add_command(
    hierarchy,
    "Elaborate the RTL of a design with Yosys, in a script of its own, and "
    "answer the tree of its instances and its modules, as JSON. The flow is "
    "not changed");
  shell.set_command_class(hierarchy_id, cmd_class);
  shell.set_command_execute_function(hierarchy_id,
                                     report_design_hierarchy_command);
}

} /* end namespace openfpga */
