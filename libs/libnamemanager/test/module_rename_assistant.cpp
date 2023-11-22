/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from readarchopenfpga */
#include "command_echo.h"
#include "command_exit_codes.h"
#include "command_parser.h"
#include "read_xml_module_name_map.h"
#include "write_xml_module_name_map.h"

/** @brief Initialize the options from command-line inputs and organize in the
 * format that is ready for parsing */
static std::vector<std::string> format_argv(const std::string& cmd_name,
                                            int argc, const char** argv) {
  std::vector<std::string> cmd_opts;
  cmd_opts.push_back(cmd_name);
  for (int iarg = 1; iarg < argc; ++iarg) {
    cmd_opts.push_back(std::string(argv[iarg]));
  }
  return cmd_opts;
}

/** @brief Convert module renaming rules from fabric A (ref -> renamed) to
 * fabric B (given the ref only) Here is an example Fabric A reference names:
 *   <module_name default="tile_1__1_" given="tile_4_"/>
 * Fabric A renamed:
 *   <module_name default="tile_1__1_" given="tile_big"/>
 * Fabric B reference names:
 *   <module_name default="tile_2__2_" given="tile_4_"/>
 * We want a renamed version for fabric B is
 *   <module_name default="tile_2__2_" given="tile_big"/>
 */
static int rename_module_names_for_fabricB_from_fabricA(
  const openfpga::ModuleNameMap& refA_module_names,
  const openfpga::ModuleNameMap& renamedA_module_names,
  const openfpga::ModuleNameMap& refB_module_names,
  openfpga::ModuleNameMap& renamedB_module_names, const bool& verbose) {
  /* Ensure a clear start */
  renamedB_module_names.clear();
  for (std::string ref_tag : refA_module_names.tags()) {
    std::string ref_given = refA_module_names.name(ref_tag);
    if (!renamedA_module_names.name_exist(ref_tag)) {
      VTR_LOG_ERROR(
        "Fail to find given name for default '%s' in the hand-crafted module "
        "names of fabric A!\n",
        ref_tag.c_str());
      return openfpga::CMD_EXEC_FATAL_ERROR;
    }
    std::string renamed_given = renamedA_module_names.name(ref_tag);
    /* Now find the same given name in refB */
    if (!refB_module_names.tag_exist(ref_given)) {
      VTR_LOG_ERROR(
        "Fail to find default name for the given name '%s' in the reference "
        "module names of fabric B!\n",
        ref_given.c_str());
      return openfpga::CMD_EXEC_FATAL_ERROR;
    }
    std::string refB_tag = refB_module_names.tag(ref_given);
    /* Add the new pair to the renamed modules for fabric B */
    renamedB_module_names.set_tag_to_name_pair(refB_tag, renamed_given);
    VTR_LOGV(verbose,
             "Successfully pair default name '%s' to given '%s' for fabric B\n",
             refB_tag.c_str(), renamed_given.c_str());
  }
  return openfpga::CMD_EXEC_FATAL_ERROR;
}

int main(int argc, const char** argv) {
  /* Create a new command and Initialize the options available in the user
   * interface */
  openfpga::Command cmd("module_rename_assistant");
  openfpga::CommandOptionId opt_refA =
    cmd.add_option("reference_fabricA_names", true,
                   "Specify the reference module name file for fabric A");
  cmd.set_option_require_value(opt_refA, openfpga::OPT_STRING);
  openfpga::CommandOptionId opt_renamedA =
    cmd.add_option("renamed_fabricA_names", true,
                   "Specify the hand-crafted module name file for fabric A");
  cmd.set_option_require_value(opt_renamedA, openfpga::OPT_STRING);
  openfpga::CommandOptionId opt_refB =
    cmd.add_option("reference_fabricB_names", true,
                   "Specify the reference module name file for fabric B");
  cmd.set_option_require_value(opt_refB, openfpga::OPT_STRING);
  openfpga::CommandOptionId opt_renamedB = cmd.add_option(
    "output", true,
    "Specify the renamed module name file for fabric B to be outputted");
  cmd.set_option_require_value(opt_renamedB, openfpga::OPT_STRING);
  openfpga::CommandOptionId opt_no_time_stamp = cmd.add_option(
    "no_time_stamp", false, "Include time stamps in output file");
  openfpga::CommandOptionId opt_verbose =
    cmd.add_option("verbose", false, "Show verbose outputs");
  openfpga::CommandOptionId opt_help =
    cmd.add_option("help", false, "Show help desk");

  /* Parse the option, to avoid issues, we use the command name to replace the
   * argv[0] */
  std::vector<std::string> cmd_opts = format_argv(cmd.name(), argc, argv);

  openfpga::CommandContext cmd_ctx(cmd);
  if (false == parse_command(cmd_opts, cmd, cmd_ctx) ||
      cmd_ctx.option_enable(cmd, opt_help)) {
    /* Echo the command */
    print_command_options(cmd);
    return openfpga::CMD_EXEC_FATAL_ERROR;
  } else {
    /* Let user to confirm selected options */
    print_command_context(cmd, cmd_ctx);
  }

  int status = 0;

  VTR_LOG(
    "Read the reference module names for fabric A from an XML file: %s.\n",
    cmd_ctx.option_value(cmd, opt_refA).c_str());
  openfpga::ModuleNameMap refA_module_names;
  status = openfpga::read_xml_module_name_map(
    cmd_ctx.option_value(cmd, opt_refA).c_str(), refA_module_names);
  if (status != openfpga::CMD_EXEC_SUCCESS) {
    return status;
  }

  VTR_LOG(
    "Read the reference module names for fabric B from an XML file: %s.\n",
    cmd_ctx.option_value(cmd, opt_refB).c_str());
  openfpga::ModuleNameMap refB_module_names;
  status = openfpga::read_xml_module_name_map(
    cmd_ctx.option_value(cmd, opt_refB).c_str(), refB_module_names);
  if (status != openfpga::CMD_EXEC_SUCCESS) {
    return status;
  }

  VTR_LOG("Read the renamed module names for fabric A from an XML file: %s.\n",
          cmd_ctx.option_value(cmd, opt_renamedA).c_str());
  openfpga::ModuleNameMap renamedA_module_names;
  status = openfpga::read_xml_module_name_map(
    cmd_ctx.option_value(cmd, opt_renamedA).c_str(), renamedA_module_names);
  if (status != openfpga::CMD_EXEC_SUCCESS) {
    return status;
  }

  /* Now apply name mapping from fabric A to fabric B */
  openfpga::ModuleNameMap renamedB_module_names;
  status = rename_module_names_for_fabricB_from_fabricA(
    refA_module_names, renamedA_module_names, refB_module_names,
    renamedB_module_names, cmd_ctx.option_enable(cmd, opt_verbose));

  VTR_LOG("Write the renamed module names for fabric B to an XML file: %s.\n",
          cmd_ctx.option_value(cmd, opt_renamedB).c_str());
  return openfpga::write_xml_module_name_map(
    cmd_ctx.option_value(cmd, opt_renamedB).c_str(), renamedB_module_names,
    !cmd_ctx.option_enable(cmd, opt_no_time_stamp),
    cmd_ctx.option_enable(cmd, opt_verbose));
}
