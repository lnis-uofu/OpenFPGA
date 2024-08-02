/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from fabric key */
#include "check_fabric_key.h"
#include "command_echo.h"
#include "command_exit_codes.h"
#include "command_parser.h"
#include "read_xml_fabric_key.h"
#include "write_xml_fabric_key.h"

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

/** @brief Checks to be done:
 * - Each alias of reference key can be found in the input key
 */
static int check_input_and_ref_key_alias_match(
  const openfpga::FabricKey& input_key, const openfpga::FabricKey& ref_key,
  const bool& verbose) {
  size_t num_errors = 0;
  size_t num_ref_keys_checked = 0;
  VTR_LOG(
    "Checking key alias matching between reference key and input keys...\n");
  for (openfpga::FabricKeyId key_id : ref_key.keys()) {
    /* Note that this is slow. May consider to build a map first */
    std::string curr_alias = ref_key.key_alias(key_id);
    std::vector<openfpga::FabricKeyId> input_found_keys =
      input_key.find_key_by_alias(curr_alias);
    float progress = static_cast<float>(num_ref_keys_checked) /
                     static_cast<float>(ref_key.num_keys()) * 100.0;
    VTR_LOGV(verbose, "[%lu%] Checking key alias '%s'\r", size_t(progress),
             curr_alias.c_str());
    if (input_found_keys.empty()) {
      VTR_LOG_ERROR(
        "Invalid alias '%s' in the reference key (id='%lu'), which does not "
        "exist in the input key!\n",
        curr_alias.c_str(), size_t(key_id));
      num_errors++;
    }
    if (input_found_keys.size() > 1) {
      VTR_LOG_ERROR(
        "Invalid alias '%s' in the input key (id='%lu'), which have been "
        "found %lu times!\n",
        curr_alias.c_str(), size_t(key_id), input_found_keys.size());
      num_errors++;
    }
    num_ref_keys_checked++;
  }
  VTR_LOG(
    "Checking key alias matching between reference key and input keys... %s\n",
    num_errors ? "[Fail]" : "[Pass]");
  /* If failed, provide a detailed diff on the key alias */
  if (num_errors) {
    size_t num_input_keys_checked = 0;
    for (openfpga::FabricKeyId key_id : input_key.keys()) {
      /* Note that this is slow. May consider to build a map first */
      std::string curr_alias = input_key.key_alias(key_id);
      std::vector<openfpga::FabricKeyId> ref_found_keys =
        ref_key.find_key_by_alias(curr_alias);
      float progress = static_cast<float>(num_input_keys_checked) /
                       static_cast<float>(input_key.num_keys()) * 100.0;
      VTR_LOGV(verbose, "[%lu%] Checking key alias '%s'\r", size_t(progress),
               curr_alias.c_str());
      if (ref_found_keys.empty()) {
        VTR_LOG_ERROR(
          "Invalid alias '%s' in the input key (id='%lu'), which does not "
          "exist in the reference key!\n",
          curr_alias.c_str(), size_t(key_id));
        num_errors++;
      }
      if (ref_found_keys.size() > 1) {
        VTR_LOG_ERROR(
          "Invalid alias '%s' in the reference key (id='%lu'), which have been "
          "found %lu times!\n",
          curr_alias.c_str(), size_t(key_id), ref_found_keys.size());
        num_errors++;
      }
      num_input_keys_checked++;
    }
  }
  return num_errors ? openfpga::CMD_EXEC_FATAL_ERROR
                    : openfpga::CMD_EXEC_SUCCESS;
}

/** @brief Checks to be done:
 * - Number of configuration regions match
 * - Number of keys match
 */
static int check_input_key(const openfpga::FabricKey& input_key,
                           const openfpga::FabricKey& ref_key,
                           const bool& verbose) {
  if (ref_key.num_regions() != input_key.num_regions()) {
    VTR_LOG_ERROR(
      "Different number of configuration regions between reference key "
      "(='%lu') and input key ('=%lu')!\n",
      ref_key.num_regions(), input_key.num_regions());
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }
  if (ref_key.num_keys() != input_key.num_keys()) {
    VTR_LOG_ERROR(
      "Different number of keys between reference key (='%lu') and input key "
      "('=%lu')!\n",
      ref_key.num_keys(), input_key.num_keys());
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }
  size_t num_errors = 0;
  size_t curr_num_err = 0;
  VTR_LOG("Checking key alias in reference key...\n");
  curr_num_err = openfpga::check_fabric_key_alias(ref_key, verbose);
  VTR_LOG("Checking key alias in reference key... %s\n",
          curr_num_err ? "[Fail]" : "[Pass]");
  VTR_LOG("Checking key names and values in reference key...\n");
  curr_num_err = openfpga::check_fabric_key_names_and_values(ref_key, verbose);
  num_errors += curr_num_err;
  VTR_LOG("Checking key names and valus in reference key... %s\n",
          curr_num_err ? "[Fail]" : "[Pass]");
  VTR_LOG("Checking key alias in input key...\n");
  curr_num_err = openfpga::check_fabric_key_alias(input_key, verbose);
  num_errors += curr_num_err;
  VTR_LOG("Checking key alias in input key... %s\n",
          curr_num_err ? "[Fail]" : "[Pass]");
  num_errors +=
    check_input_and_ref_key_alias_match(input_key, ref_key, verbose);
  return num_errors ? openfpga::CMD_EXEC_FATAL_ERROR
                    : openfpga::CMD_EXEC_SUCCESS;
}

/** @brief Checks to be done:
 * - Each alias of input key can be found in the reference key
 * - Update input key with pair of name and value which matches the alias from
 * the reference key
 */
static int update_input_key(openfpga::FabricKey& input_key,
                            const openfpga::FabricKey& ref_key,
                            const bool& verbose) {
  size_t num_errors = 0;
  size_t num_keys_checked = 0;
  float progress = 0.;
  VTR_LOG("Pairing key alias between reference key and input keys...\n");
  for (openfpga::FabricKeyId key_id : input_key.keys()) {
    /* Note that this is slow. May consider to build a map first */
    std::string curr_alias = input_key.key_alias(key_id);
    std::vector<openfpga::FabricKeyId> ref_found_keys =
      ref_key.find_key_by_alias(curr_alias);
    progress = static_cast<float>(num_keys_checked) /
               static_cast<float>(input_key.num_keys()) * 100.0;
    VTR_LOGV(verbose, "[%lu%] Pairing key alias '%s'\r", size_t(progress),
             curr_alias.c_str());
    if (ref_found_keys.empty()) {
      VTR_LOG_ERROR(
        "Invalid alias '%s' in the input key (id='%lu'), which does not "
        "exist in the reference key!\n",
        curr_alias.c_str(), size_t(key_id));
      num_errors++;
    }
    if (ref_found_keys.size() > 1) {
      VTR_LOG_ERROR(
        "Invalid alias '%s' in the reference key (id='%lu'), which have been "
        "found %lu times!\n",
        curr_alias.c_str(), size_t(key_id));
      num_errors++;
    }
    /* Now we have a key, get the name and value, and update input key */
    input_key.set_key_name(key_id, ref_key.key_name(ref_found_keys[0]));
    input_key.set_key_value(key_id, ref_key.key_value(ref_found_keys[0]));
    VTR_LOGV(verbose, "[%lu%] Pairing key alias '%s' -> ('%s', %lu)\r",
             size_t(progress), curr_alias.c_str(),
             input_key.key_name(key_id).c_str(), input_key.key_value(key_id));
    num_keys_checked++;
  }
  return num_errors ? openfpga::CMD_EXEC_FATAL_ERROR
                    : openfpga::CMD_EXEC_SUCCESS;
}

/** @brief Checks to be done:
 * - Number of configuration regions match
 * - Number of keys match
 * - Each alias can be found in the reference key
 */
static int check_and_update_input_key(openfpga::FabricKey& input_key,
                                      const openfpga::FabricKey& ref_key,
                                      const bool& verbose) {
  int status = openfpga::CMD_EXEC_SUCCESS;
  status = check_input_key(input_key, ref_key, verbose);
  if (status != openfpga::CMD_EXEC_SUCCESS) {
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }
  return update_input_key(input_key, ref_key, verbose);
}

int main(int argc, const char** argv) {
  /* Create a new command and Initialize the options available in the user
   * interface */
  openfpga::Command cmd("fabric_key_assistant");
  openfpga::CommandOptionId opt_ref =
    cmd.add_option("reference", true, "Specify the reference fabric key file");
  cmd.set_option_require_value(opt_ref, openfpga::OPT_STRING);
  openfpga::CommandOptionId opt_input =
    cmd.add_option("input", true, "Specify the hand-crafted fabric key file");
  cmd.set_option_require_value(opt_input, openfpga::OPT_STRING);
  openfpga::CommandOptionId opt_output = cmd.add_option(
    "output", true, "Specify the final fabric key file to be outputted");
  cmd.set_option_require_value(opt_output, openfpga::OPT_STRING);
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

  /* Parse the fabric key from an XML file */
  VTR_LOG("Read the reference fabric key from an XML file: %s.\n",
          cmd_ctx.option_value(cmd, opt_ref).c_str());
  openfpga::FabricKey ref_key =
    openfpga::read_xml_fabric_key(cmd_ctx.option_value(cmd, opt_ref).c_str());

  VTR_LOG("Read the hand-crafted fabric key from an XML file: %s.\n",
          cmd_ctx.option_value(cmd, opt_input).c_str());
  openfpga::FabricKey input_key =
    openfpga::read_xml_fabric_key(cmd_ctx.option_value(cmd, opt_input).c_str());

  /* Check the input key */
  if (check_and_update_input_key(input_key, ref_key,
                                 cmd_ctx.option_enable(cmd, opt_verbose))) {
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  VTR_LOG("Write the final fabric key to an XML file: %s.\n",
          cmd_ctx.option_value(cmd, opt_output).c_str());
  return openfpga::write_xml_fabric_key(
    cmd_ctx.option_value(cmd, opt_output).c_str(), input_key);
}
