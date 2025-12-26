/******************************************************************************
 * Inspired from https://github.com/genbtc/VerilogPCFparser
 ******************************************************************************/
#include <sstream>

/* Headers from vtrutil library */
#include <cstdint>

#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "command_exit_codes.h"
#include "openfpga_decode.h"
#include "openfpga_digest.h"
#include "pcf_config_constants.h"
#include "pcf_reader.h"
#include "read_xml_util.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Constants
 *************************************************/
constexpr const char COMMENT = '#';

static std::string generate_binary_strings(std::size_t num_bits,
                                           std::uint64_t max_decimal,
                                           bool little_endian,
                                           std::uint64_t decimal_value) {
  if (num_bits == 0 || num_bits > 64) {
    VTR_LOG_ERROR("num_bits must be in range [1, 64]\n");
    exit(1);
  }

  const std::uint64_t max_representable =
    (num_bits == 64) ? std::numeric_limits<std::uint64_t>::max()
                     : ((std::uint64_t(1) << num_bits) - 1);

  if (max_decimal > max_representable) {
    VTR_LOG_ERROR("max_decimal exceeds num_bits capacity\n");
    exit(1);
  }

  if (decimal_value > max_decimal) {
    VTR_LOG_ERROR("User specified decimal value exceeds max_decimal\n");
    exit(1);
  }

  std::vector<char> bits = itobin_charvec(decimal_value, num_bits);

  if (little_endian) {
    std::reverse(bits.begin(), bits.end());
  }

  return std::string(bits.begin(), bits.end());
}

static int read_xml_pcf_command(pugi::xml_node& xml_pcf_command,
                                const pugiutil::loc_data& loc_data,
                                PcfCustomCommand& pcf_custom_command) {
  std::string command_name =
    get_attribute(xml_pcf_command, XML_COMMAND_TYPE_ATTRIBUTE_NAME, loc_data)
      .as_string();

  std::string command_type =
    get_attribute(xml_pcf_command, XML_COMMAND_TYPE_ATTRIBUTE_TYPE, loc_data)
      .as_string();

  int status =
    pcf_custom_command.create_custom_command(command_name, command_type);

  for (pugi::xml_node xml_child : xml_pcf_command.children()) {
    if (xml_child.name() != std::string(XML_OPTION_TYPE_NODE_NAME) &&
        xml_child.name() != std::string(XML_PB_TYPE_NODE_NAME)) {
      bad_tag(xml_child, loc_data, xml_pcf_command,
              {XML_OPTION_TYPE_NODE_NAME, XML_PB_TYPE_NODE_NAME});
      return CMD_EXEC_FATAL_ERROR;
    }
    /*parse option*/
    if (xml_child.name() == std::string(XML_OPTION_TYPE_NODE_NAME)) {
      std::string option_name =
        get_attribute(xml_child, XML_OPTION_ATTRIBUTE_NAME, loc_data)
          .as_string();
      std::string option_type =
        get_attribute(xml_child, XML_OPTION_ATTRIBUTE_TYPE, loc_data)
          .as_string();
      VTR_ASSERT(option_type == OPTION_TYPE_DECIMAL ||
                 option_type == OPTION_TYPE_PIN ||
                 option_type == OPTION_TYPE_MODE);
      /*The case the mode is defined using decimal*/
      if (option_type == OPTION_TYPE_DECIMAL) {
        status = pcf_custom_command.create_custom_option(
          command_name, option_name, OPTION_TYPE_DECIMAL);
        int num_bits =
          get_attribute(xml_child, XML_OPTION_ATTRIBUTE_NUM_BITS, loc_data)
            .as_int();
        std::uint64_t max_decimal = std::stoull(
          get_attribute(xml_child, XML_OPTION_ATTRIBUTE_MAX_DECIMAL, loc_data)
            .as_string());
        bool little_endian =
          get_attribute(xml_child, XML_OPTION_ATTRIBUTE_LITTLE_ENDIAN, loc_data)
            .as_bool();
        int mode_offset =
          get_attribute(xml_child, XML_OPTION_ATTRIBUTE_OFFSET, loc_data)
            .as_int();

        pcf_custom_command.create_custom_decimal_mode(
          command_name, option_name, num_bits, max_decimal, little_endian,
          mode_offset);

      } else if (option_type == "mode") {
        status = pcf_custom_command.create_custom_option(
          command_name, option_name, option_type);
        /*The case the mode is defined explicitly*/
        auto xml_pcf_option_mode =
          get_first_child(xml_child, XML_MODE_TYPE_NODE_NAME, loc_data,
                          pugiutil::ReqOpt::OPTIONAL);
        int mode_offset = -1;
        if (xml_pcf_option_mode) {
          mode_offset =
            get_attribute(xml_child, XML_OPTION_ATTRIBUTE_OFFSET, loc_data)
              .as_int();
        }
        while (xml_pcf_option_mode) {
          std::string mode_name =
            get_attribute(xml_pcf_option_mode, XML_MODE_ATTRIBUTE_NAME,
                          loc_data)
              .as_string();
          std::string mode_value =
            get_attribute(xml_pcf_option_mode, XML_MODE_ATTRIBUTE_VALUE,
                          loc_data)
              .as_string();
          xml_pcf_option_mode = xml_pcf_option_mode.next_sibling();
          status = pcf_custom_command.create_custom_mode(
            command_name, option_name, mode_name, mode_value, mode_offset);
        }
      } else {
        status = pcf_custom_command.create_custom_option(
          command_name, option_name, option_type);
      }
    }
    /*parse pb_type*/
    if (xml_child.name() == std::string(XML_PB_TYPE_NODE_NAME)) {
      std::string pb_type_name =
        get_attribute(xml_child, XML_PB_TYPE_ATTRIBUTE_NAME, loc_data)
          .as_string();

      pcf_custom_command.set_custom_command_pb_type(command_name, pb_type_name);
    }
  }
  bool conflict =
    pcf_custom_command.command_mode_offset_conflict_check(command_name);
  if (conflict) {
    VTR_LOG_ERROR(
      "Please check offset for options of command %s, there shows conflicts! "
      "\n",
      command_name.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * A writer to output a repack pin constraint object to XML format
 *
 * Return 0 if successful
 * Return 1 if there are serious errors when parsing data
 * Return 2 if fail when opening files
 *******************************************************************/
int read_pcf(const char* fname, PcfData& pcf_data,
             const PcfCustomCommand& pcf_custom_command,
             bool reduce_error_to_warning, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Read " + std::string(fname));

  /* Create a file handler */
  std::ifstream fp(fname);
  if (!fp.is_open()) {
    VTR_LOG_ERROR("Fail to open pcf file '%s'!", fname);
    return 2;
  }

  int num_err = 0;

  /* Get line by line */
  std::string line;
  while (std::getline(fp, line)) {
    std::stringstream ss(line);
    while (ss) {
      std::string word;
      /* TODO: Use command parser */
      if (ss >> word) {
        if (word.find("set_io") == 0) {
          std::string net_name;
          std::string pin_name;
          ss >> net_name >> pin_name;
          /* Decode data */
          PcfIoConstraintId io_id = pcf_data.create_io_constraint();
          pcf_data.set_io_net(io_id, net_name);
          pcf_data.set_io_pin(io_id, pin_name);
        } else if (word[0] == COMMENT) {  // if it's a comment
          VTR_LOGV(verbose, "Bypass comment line '%s' !\n", word.c_str());
          break;  // or ignore the full line comment and move on
        } else {
          bool valid_command = false;
          bool valid_option = false;
          valid_command = pcf_custom_command.valid_command(word);
          /*ToDO: can consider use parse_command API to parse the command from
           * pcf */
          if (valid_command) {
            std::string option_name;
            std::string option_value;
            PcfCustomConstraintId constraint_id =
              pcf_data.create_custom_constraint();
            pcf_data.set_custom_constraint_command(constraint_id, word);
            while (ss >> option_name >> option_value) {
              valid_option = false;
              option_name.erase(0, 1); /* omit "-" in front of option */
              valid_option = pcf_custom_command.valid_option(word, option_name);
              if (valid_option) {
                std::string option_type =
                  pcf_custom_command.custom_option_type(word, option_name);

                if (option_type == OPTION_TYPE_PIN) {
                  pcf_data.set_custom_constraint_pin(constraint_id,
                                                     option_value);
                } else if (option_type == OPTION_TYPE_MODE) {
                  std::string mode_value = pcf_custom_command.custom_mode_value(
                    word, option_name, option_value);
                  int mode_offset = pcf_custom_command.custom_mode_offset(
                    word, option_name, option_value);
                  pcf_data.set_custom_constraint_pin_mode(constraint_id,
                                                          mode_value);
                  pcf_data.set_custom_constraint_pin_mode_offset(constraint_id,
                                                                 mode_offset);
                } else if (option_type == OPTION_TYPE_DECIMAL) {
                  int num_bits =
                    pcf_custom_command.custom_decimal_mode_num_bits(
                      word, option_name);
                  std::uint64_t max_decimal =
                    pcf_custom_command.custom_decimal_mode_max_val(word,
                                                                   option_name);
                  bool little_endian =
                    pcf_custom_command.custom_decimal_mode_little_endian(word,
                                                                   option_name);
                  std::string mode_value = generate_binary_strings(
                    num_bits, max_decimal, little_endian,
                    std::stoull(option_value));
                  int mode_offset =
                    pcf_custom_command.custom_decimal_mode_offset(word,
                                                                  option_name);
                  pcf_data.set_custom_constraint_pin_mode(constraint_id,
                                                          mode_value);
                  pcf_data.set_custom_constraint_pin_mode_offset(constraint_id,
                                                                 mode_offset);
                }
              } else {
                break;
              }
            }
            /* set pb_type and offset */
            std::string pb_type =
              pcf_custom_command.custom_command_pb_type(word);
            pcf_data.set_custom_constraint_pb_type(constraint_id, pb_type);
          }
          if (!valid_command || !valid_option) {
            if (reduce_error_to_warning) {
              VTR_LOG_WARN("Bypass unknown command '%s' !\n", word.c_str());
              break;
            } else {
              /* Reach unknown command for OpenFpga, error out */
              VTR_LOG_ERROR("Unknown command '%s'!\n", word.c_str());
              num_err++;
              break;  // and move onto next line. without this, it will accept
                      // more following values on this line
            }
          }
        }
      }
    }
  }

  if (num_err) {
    return 1;
  }
  return 0;
}

int read_pcf_config(const std::string& pcf_config_file,
                    PcfCustomCommand& pcf_custom_command) {
  int status = CMD_EXEC_FATAL_ERROR;

  pugi::xml_node Next;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  loc_data = pugiutil::load_xml(doc, pcf_config_file.c_str());

  /* First node should be <openfpga_architecture> */
  auto xml_pcf_config =
    get_single_child(doc, XML_PCF_CONFIG_ROOT_NAME, loc_data);

  /* Parse circuit_models to circuit library
   * under the node <module_circuit_models>
   */
  for (pugi::xml_node xml_command : xml_pcf_config.children()) {
    if (xml_command.name() != std::string(XML_COMMAND_TYPE_NODE_NAME)) {
      bad_tag(xml_command, loc_data, xml_pcf_config,
              {XML_COMMAND_TYPE_NODE_NAME});
      return CMD_EXEC_FATAL_ERROR;
    }
    status = read_xml_pcf_command(xml_command, loc_data, pcf_custom_command);
    if (status != CMD_EXEC_SUCCESS) {
      VTR_LOG_ERROR("Fail to read command from PCF Config file!\n");
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
