/********************************************************************
 * The commands of the device packages (see device_package_commands.h)
 *******************************************************************/
#include "device_package_commands.h"

#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "command_exit_codes.h"
#include "device_package.h"
#include "globals.h"
#include "package_io_map.h"
#include "vpr_option_table.h"
#include "vpr_run.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

namespace {

const int DEVICE_PACKAGE_SCHEMA_VERSION = 1;

/* The answer of report_device_package, around its data */
std::string answer(const std::string& data) {
  return "{\"schema_version\":" +
         std::to_string(DEVICE_PACKAGE_SCHEMA_VERSION) +
         ",\"kind\":\"device_package\",\"stage\":null,\"data\":" + data + "}";
}

/* The flags of VPR, with the names of their application options */
std::vector<std::pair<std::string, std::string>> vpr_flags() {
  std::vector<std::pair<std::string, std::string>> flags;
  for (const VprOptionSpec& spec : vpr_option_specs()) {
    flags.emplace_back(spec.flag, std::string(spec.group) + "." + spec.name);
  }
  return flags;
}

/* A path for a command line of the shell */
std::string quoted(const std::filesystem::path& path) {
  return "\"" + path.string() + "\"";
}

/* Make the session target the device of a package: the options of VPR of
 * its flow, its two architectures (with the layout of its grid) and its
 * channel width. The options are set first, and a value that the flow refuses
 * stops before the architectures are read */
int read_device_package_command(Shell<OpenfpgaContext>* shell,
                                OpenfpgaContext& openfpga_ctx,
                                const Command& cmd,
                                const CommandContext& cmd_context) {
  const std::string file = cmd_context.option_value(cmd, cmd.option("file"));
  DevicePackage package;
  try {
    package = load_device_package(file);
  } catch (const DevicePackageError& error) {
    VTR_LOG_ERROR("%s\n", error.what());
    return CMD_EXEC_FATAL_ERROR;
  }
  for (const auto& io_map : package.files_of("io_map")) {
    try {
      read_package_io_map(io_map);
    } catch (const PackageIoMapError& error) {
      VTR_LOG_ERROR("The device package '%s' cannot be used: %s\n",
                    package.name.c_str(), error.what());
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  const DevicePackageOptions applied =
    device_package_options(package, vpr_flags());
  OpenFpgaSession& session = openfpga_ctx.mutable_session();
  for (const auto& option : applied.set) {
    std::string error;
    if (!session.set_app_option(option.first, option.second.text, error)) {
      VTR_LOG_ERROR(
        "The device package '%s' cannot be used: flow_defaults.vpr_options "
        "sets %s to %s: %s\n",
        package.name.c_str(), option.first.c_str(),
        option.second.literal.c_str(), error.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    VTR_LOG("Set %s to %s (flow_defaults.vpr_options of the package)\n",
            option.first.c_str(), option.second.text.c_str());
  }
  for (const auto& ignored : applied.ignored) {
    VTR_LOG("Not set: %s, as %s\n", ignored.first.c_str(),
            ignored.second.c_str());
  }
  const std::string read_vpr = "read_vpr_arch --file " +
                               quoted(package.vpr_arch) + " --layout " +
                               package.layout;
  if (CMD_EXEC_SUCCESS !=
      shell->execute_command(read_vpr.c_str(), openfpga_ctx)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  const std::string read_openfpga =
    "read_openfpga_arch --file " + quoted(package.openfpga_arch);
  if (CMD_EXEC_SUCCESS !=
      shell->execute_command(read_openfpga.c_str(), openfpga_ctx)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  if (package.channel_width && 0 != *package.channel_width) {
    std::string error;
    if (!session.set_app_option("router.route_chan_width",
                                std::to_string(*package.channel_width),
                                error)) {
      VTR_LOG_ERROR("%s\n", error.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  session.set_device_package(device_package_json(package, &applied));
  VTR_LOG("Device package '%s' %s: layout %s\n", package.name.c_str(),
          package.version.c_str(), package.layout.c_str());
  return CMD_EXEC_SUCCESS;
}

/* The package of the session, or the one in a directory (--file), which is
 * checked and described without being read */
int report_device_package_command(const OpenfpgaContext& openfpga_ctx,
                                  const Command& cmd,
                                  const CommandContext& cmd_context) {
  std::string data;
  const CommandOptionId opt_file = cmd.option("file");
  if (cmd_context.option_enable(cmd, opt_file)) {
    try {
      const bool checksums =
        !cmd_context.option_enable(cmd, cmd.option("no_checksums"));
      data = device_package_json(
        load_device_package(cmd_context.option_value(cmd, opt_file), checksums),
        nullptr);
    } catch (const DevicePackageError& error) {
      VTR_LOG_ERROR("%s\n", error.what());
      return CMD_EXEC_FATAL_ERROR;
    }
  } else {
    data = openfpga_ctx.session().device_package();
    if (data.empty()) {
      VTR_LOG_ERROR(
        "The architectures of the session were not read from a device "
        "package: run 'read_device_package' first, or give --file\n");
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  const std::string json = answer(data);
  openfpga_ctx.set_query_result(json);
  VTR_LOG("%s\n", json.c_str());
  return CMD_EXEC_SUCCESS;
}

/* Whether VPR has made the grid of a device for this session: the data of VPR
 * is the one of the session (and not of another shell of the process) */
bool has_grid(const OpenFpgaSession& session) {
  return session.vpr_generation() == vpr::vpr_run_generation() &&
         0 < g_vpr_ctx.device().grid.width();
}

/* A first I/O map of the device of the session */
int write_io_map_command(const OpenfpgaContext& openfpga_ctx,
                         const Command& cmd,
                         const CommandContext& cmd_context) {
  if (!has_grid(openfpga_ctx.session())) {
    VTR_LOG_ERROR(
      "There is no device yet: an I/O map is written once the device is made "
      "(the inputs are read)\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  const std::string file = cmd_context.option_value(cmd, cmd.option("file"));
  const IoLocationMap& fabric = openfpga_ctx.io_location_map();
  std::ofstream out(file);
  out << write_package_io_map(g_vpr_ctx.device().grid,
                              fabric.empty() ? nullptr : &fabric);
  if (!out) {
    VTR_LOG_ERROR("The I/O map cannot be written to '%s'\n", file.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  VTR_LOG("Wrote the I/O map '%s'%s\n", file.c_str(),
          fabric.empty() ? " (without the names of the fabric, which is not "
                           "built)"
                         : "");
  return CMD_EXEC_SUCCESS;
}

/* Check an I/O map: its rules, and its locations in the grid of the device
 * when there is one */
int check_io_map_command(const OpenfpgaContext& openfpga_ctx,
                         const Command& cmd,
                         const CommandContext& cmd_context) {
  const std::string file = cmd_context.option_value(cmd, cmd.option("file"));
  PackageIoMap map;
  try {
    map = read_package_io_map(file);
  } catch (const PackageIoMapError& error) {
    VTR_LOG_ERROR("%s\n", error.what());
    return CMD_EXEC_FATAL_ERROR;
  }
  if (!has_grid(openfpga_ctx.session())) {
    VTR_LOG(
      "The I/O map '%s' follows its rules (%lu rows); the locations are "
      "checked once the device is made\n",
      file.c_str(), map.rows.size());
    return CMD_EXEC_SUCCESS;
  }
  const std::vector<std::string> problems =
    check_package_io_map_grid(map, g_vpr_ctx.device().grid);
  for (const std::string& problem : problems) {
    VTR_LOG_ERROR("%s\n", problem.c_str());
  }
  if (!problems.empty()) {
    return CMD_EXEC_FATAL_ERROR;
  }
  VTR_LOG("The I/O map '%s' fits the device (%lu rows)\n", file.c_str(),
          map.rows.size());
  return CMD_EXEC_SUCCESS;
}

} /* end anonymous namespace */

void add_device_package_commands(Shell<OpenfpgaContext>& shell) {
  ShellCommandClassId cmd_class = shell.add_command_class("Device packages");

  Command read_cmd("read_device_package");
  CommandOptionId opt_file =
    read_cmd.add_option("file", true, "The directory of the device package");
  read_cmd.set_option_short_name(opt_file, "f");
  read_cmd.set_option_require_value(opt_file, OPT_STRING);
  ShellCommandId read_id = shell.add_command(
    read_cmd,
    "Target the device of a device package: check it, set the options of its "
    "flow and its channel width, and read its VPR architecture (with the "
    "layout of its grid) and its OpenFPGA architecture");
  shell.set_command_class(read_id, cmd_class);
  shell.set_command_execute_function(read_id, read_device_package_command);

  Command report_cmd("report_device_package");
  CommandOptionId opt_report_file = report_cmd.add_option(
    "file", false,
    "The directory of a package to check and describe, without reading it");
  report_cmd.set_option_short_name(opt_report_file, "f");
  report_cmd.set_option_require_value(opt_report_file, OPT_STRING);
  report_cmd.add_option("no_checksums", false,
                        "With --file: do not check the checksums");
  ShellCommandId report_id = shell.add_command(
    report_cmd,
    "Answer, as JSON, the device package of the session (what it set and did "
    "not set), or the one of --file");
  shell.set_command_class(report_id, cmd_class);
  shell.set_command_const_execute_function(report_id,
                                           report_device_package_command);

  Command write_map("write_io_map");
  CommandOptionId opt_write_file =
    write_map.add_option("file", true, "The CSV file to write");
  write_map.set_option_short_name(opt_write_file, "f");
  write_map.set_option_require_value(opt_write_file, OPT_STRING);
  ShellCommandId write_id = shell.add_command(
    write_map,
    "Write a first I/O map of the device (a row for each I/O subtile, with "
    "the names of the fabric once it is built), for a device package");
  shell.set_command_class(write_id, cmd_class);
  shell.set_command_const_execute_function(write_id, write_io_map_command);

  Command check_map("check_io_map");
  CommandOptionId opt_check_file =
    check_map.add_option("file", true, "The CSV file of the I/O map");
  check_map.set_option_short_name(opt_check_file, "f");
  check_map.set_option_require_value(opt_check_file, OPT_STRING);
  ShellCommandId check_id = shell.add_command(
    check_map,
    "Check an I/O map: its rules, and its locations in the grid of the "
    "device once it is made");
  shell.set_command_class(check_id, cmd_class);
  shell.set_command_const_execute_function(check_id, check_io_map_command);
}

} /* end namespace openfpga */
