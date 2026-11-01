#include "query_commands.h"

#include <functional>
#include <string>

#include "command_exit_codes.h"
#include "query_design.h"
#include "query_timing.h"
#include "staged_flow.h"
#include "vpr_run.h"
#include "vtr_log.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

using Query = std::function<bool(const vpr::VprRun&, std::string&)>;

/* Make the answer of a query, and give it: print it, and hand it to the
 * caller of the command */
int answer(const OpenfpgaContext& openfpga_ctx, const OpenFpgaStage& stage,
           const char* command, const Query& query) {
  if (!stage_ready(openfpga_ctx.session(), stage, command)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  const vpr::VprRun* run = vpr::current_vpr_run();
  if (nullptr == run) {
    VTR_LOG_ERROR(
      "Cannot run '%s': there is no data of VPR. Run the flow first, e.g., "
      "with the commands that read the inputs and run the stages\n",
      command);
    return CMD_EXEC_FATAL_ERROR;
  }
  std::string json;
  if (!query(*run, json)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  openfpga_ctx.set_query_result(json);
  VTR_LOG("%s\n", json.c_str());
  return CMD_EXEC_SUCCESS;
}

/* The value of an option that is a whole number. Return false and log if it
 * is not one */
bool whole_number(const char* option, const std::string& text, int& number) {
  try {
    size_t used = 0;
    number = std::stoi(text, &used);
    if (used == text.size()) {
      return true;
    }
  } catch (const std::exception&) {
  }
  VTR_LOG_ERROR("The value of --%s must be a whole number, not '%s'\n", option,
                text.c_str());
  return false;
}

/* The value of an option that is a number of items, from 1 to the largest
 * size_t. The default is used if the option is not given */
bool count_option(const Command& cmd, const CommandContext& cmd_context,
                  const char* name, const size_t default_value, size_t& count) {
  const CommandOptionId option = cmd.option(name);
  if (!cmd_context.option_enable(cmd, option)) {
    count = default_value;
    return true;
  }
  int number = 0;
  if (!whole_number(name, cmd_context.option_value(cmd, option), number)) {
    return false;
  }
  if (number < 1) {
    VTR_LOG_ERROR("The value of --%s must be at least 1, not %d\n", name,
                  number);
    return false;
  }
  count = static_cast<size_t>(number);
  return true;
}

std::string string_option(const Command& cmd, const CommandContext& cmd_context,
                          const char* name) {
  const CommandOptionId option = cmd.option(name);
  return cmd_context.option_enable(cmd, option)
           ? cmd_context.option_value(cmd, option)
           : std::string();
}

int timing_summary_command(const OpenfpgaContext& openfpga_ctx, const Command&,
                           const CommandContext&) {
  return answer(openfpga_ctx, OpenFpgaStage::ROUTED, "timing_summary",
                [](const vpr::VprRun& run, std::string& json) {
                  return timing_summary_json(run, json);
                });
}

int critical_paths_command(const OpenfpgaContext& openfpga_ctx,
                           const Command& cmd,
                           const CommandContext& cmd_context) {
  size_t count = 0;
  if (!count_option(cmd, cmd_context, "count", 5, count)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  return answer(openfpga_ctx, OpenFpgaStage::ROUTED, "critical_paths",
                [count](const vpr::VprRun& run, std::string& json) {
                  return critical_paths_json(run, count, json);
                });
}

int utilization_command(const OpenfpgaContext& openfpga_ctx, const Command&,
                        const CommandContext&) {
  return answer(openfpga_ctx, OpenFpgaStage::PACKED, "utilization",
                [](const vpr::VprRun&, std::string& json) {
                  return utilization_json(json);
                });
}

int placement_command(const OpenfpgaContext& openfpga_ctx, const Command& cmd,
                      const CommandContext& cmd_context) {
  size_t limit = 0;
  if (!count_option(cmd, cmd_context, "limit", QUERY_LIST_DEFAULT_LIMIT,
                    limit)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  const std::string type = string_option(cmd, cmd_context, "type");
  const std::string prefix = string_option(cmd, cmd_context, "prefix");
  return answer(openfpga_ctx, OpenFpgaStage::PLACED, "placement",
                [&](const vpr::VprRun&, std::string& json) {
                  return placement_json(type, prefix, limit, json);
                });
}

int net_route_command(const OpenfpgaContext& openfpga_ctx, const Command& cmd,
                      const CommandContext& cmd_context) {
  const std::string net = string_option(cmd, cmd_context, "net");
  return answer(openfpga_ctx, OpenFpgaStage::ROUTED, "net_route",
                [&](const vpr::VprRun&, std::string& json) {
                  return net_route_json(net, json);
                });
}

int congestion_command(const OpenfpgaContext& openfpga_ctx, const Command& cmd,
                       const CommandContext& cmd_context) {
  size_t region_size = 1;
  if (!count_option(cmd, cmd_context, "region_size", 1, region_size)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  return answer(openfpga_ctx, OpenFpgaStage::ROUTED, "congestion",
                [region_size](const vpr::VprRun&, std::string& json) {
                  return congestion_json(static_cast<int>(region_size), json);
                });
}

int report_cluster_command(const OpenfpgaContext& openfpga_ctx,
                           const Command& cmd,
                           const CommandContext& cmd_context) {
  size_t limit = 0;
  if (!count_option(cmd, cmd_context, "limit", QUERY_LIST_DEFAULT_LIMIT,
                    limit)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  const std::string query = string_option(cmd, cmd_context, "query");
  const bool with_stats = cmd_context.option_enable(cmd, cmd.option("stats"));
  return answer(openfpga_ctx, OpenFpgaStage::PACKED, "report_cluster",
                [&](const vpr::VprRun&, std::string& json) {
                  return cluster_report_json(query, with_stats, limit, json);
                });
}

/* An option that takes a value, which is optional unless required is set */
void add_option(Command& cmd, const char* name, const char* short_name,
                const bool required, const e_option_value_type type,
                const char* help) {
  CommandOptionId option = cmd.add_option(name, required, help);
  if (nullptr != short_name) {
    cmd.set_option_short_name(option, short_name);
  }
  cmd.set_option_require_value(option, type);
}

/* A command that does not change the data: its function is const */
void add_query_command(openfpga::Shell<OpenfpgaContext>& shell,
                       const ShellCommandClassId& cmd_class, const Command& cmd,
                       const char* description,
                       int (*exec)(const OpenfpgaContext&, const Command&,
                                   const CommandContext&)) {
  ShellCommandId id = shell.add_command(cmd, description);
  shell.set_command_class(id, cmd_class);
  shell.set_command_const_execute_function(id, exec);
}

}  // End anonymous namespace

void add_query_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  ShellCommandClassId cmd_class = shell.add_command_class("Queries");

  add_query_command(
    shell, cmd_class, Command("timing_summary"),
    "Answer with the timing of the routed design: slacks, critical path "
    "delay and Fmax, and the critical path of each pair of clocks",
    timing_summary_command);

  Command critical_paths("critical_paths");
  add_option(critical_paths, "count", "n", false, OPT_INT,
             "Number of paths, from 1 to 100. Default: 5");
  add_query_command(shell, cmd_class, critical_paths,
                    "Answer with the worst setup paths of the routed design, "
                    "with the delay of each point",
                    critical_paths_command);

  add_query_command(shell, cmd_class, Command("utilization"),
                    "Answer with the use of the blocks of the device by the "
                    "packed design",
                    utilization_command);

  Command placement("placement");
  add_option(placement, "type", "t", false, OPT_STRING,
             "Only the blocks of this type, e.g., clb");
  add_option(placement, "prefix", "p", false, OPT_STRING,
             "Only the blocks whose name starts with this text");
  add_option(placement, "limit", "l", false, OPT_INT,
             "Most blocks to list. Default: 1000");
  add_query_command(shell, cmd_class, placement,
                    "Answer with the place of the blocks of the placed design",
                    placement_command);

  Command net_route("net_route");
  add_option(net_route, "net", "n", true, OPT_STRING, "Name of the net");
  add_query_command(shell, cmd_class, net_route,
                    "Answer with the routing tree of a net, as the nodes of "
                    "the routing resource graph",
                    net_route_command);

  Command congestion("congestion");
  add_option(congestion, "region_size", "r", false, OPT_INT,
             "Aggregate the channels in regions of this many tiles on a "
             "side. Default: 1");
  add_query_command(shell, cmd_class, congestion,
                    "Answer with the use of the routing channels of the "
                    "routed design",
                    congestion_command);

  Command report_cluster("report_cluster");
  add_option(report_cluster, "query", "q", false, OPT_STRING,
             "Only the clusters whose name matches this shell-style "
             "pattern, e.g., 'n1*'. Default: all");
  report_cluster.add_option("stats", false,
                            "Also answer with the statistics of all the "
                            "clusters");
  add_option(report_cluster, "limit", "l", false, OPT_INT,
             "Most clusters to list. Default: 1000");
  add_query_command(shell, cmd_class, report_cluster,
                    "Answer with the clusters of the packed design",
                    report_cluster_command);
}

} /* End namespace openfpga */
