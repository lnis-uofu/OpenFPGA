/******************************************************************************
 * Inspired from https://github.com/genbtc/VerilogPCFparser
 ******************************************************************************/
#include <sstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
#include "pcf_reader.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Constants
 *************************************************/
constexpr const char COMMENT = '#';

/********************************************************************
 * A writer to output a repack pin constraint object to XML format
 *
 * Return 0 if successful
 * Return 1 if there are serious errors when parsing data
 * Return 2 if fail when opening files
 *******************************************************************/
int read_pcf(const char* fname, PcfData& pcf_data,
             bool reduce_error_to_warning) {
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
          break;  // or ignore the full line comment and move on
        } else {
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

  if (num_err) {
    return 1;
  }
  return 0;
}

int read_pcf_conifg(const std::string& pcf_config_file, PcfData& pcf_data) {
  // int status = openfpga::CMD_EXEC_FATAL_ERROR;

  pugi::xml_node Next;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  loc_data = pugiutil::load_xml(doc, pcf_config_file.c_str());

  /* First node should be <openfpga_architecture> */
  auto xml_pcf_config = get_single_child(doc, "pcf_config", loc_data);

  /* Parse circuit_models to circuit library
   * under the node <module_circuit_models>
   */
  for (pugi::xml_node xml_command : xml_pcf_config.children()) {
    int status = read_xml_pcf_command(xml_command, loc_data, pcf_data);
    if(status != 0){
      VTR_LOG_ERROR("Fail to read command from PCF Config file!\n");
      return 1;
    }
  }
  return 0;
}

int read_xml_pcf_command(pugi::xml_node& xml_pcf_command,
                         const pugiutil::loc_data& loc_data, PcfData& pcf_data) {
  
  std::string command_name =
    get_attribute(xml_pcf_command, "name", loc_data).as_string();

  std::string command_type =
    get_attribute(xml_pcf_command, "type", loc_data).as_string();
  
  PcfCustomCommandId command_id = pcf_data.create_custom_command(command_name, command_type);
  auto xml_pcf_option = get_first_child(xml_pcf_command, "option", loc_data);
  while (xml_pcf_option) {
    std::string option_name =
      get_attribute(xml_pcf_option, "name", loc_data).as_string();
    std::string option_type =
      get_attribute(xml_pcf_option, "type", loc_data).as_string();
    auto xml_pcf_option_mode = get_first_child(xml_pcf_option, "mode", loc_data,
                                               pugiutil::ReqOpt::OPTIONAL);
    PcfCustomCommandOptionId option_id = pcf_data.create_custom_command(command_id， option_name, option_type);
    while (xml_pcf_option_mode) {
      std::string mode_name =
        get_attribute(xml_pcf_option_mode, "name", loc_data).as_string();
      std::string mode_value =
        get_attribute(xml_pcf_option_mode, "value", loc_data).as_string();
      xml_pcf_option_mode = xml_pcf_option_mode.next_sibling();
      PcfCustomCommandModeId mode_id = pcf_data.create_custom_command(option_id, mode_name, mode_value);
    }
    xml_pcf_option = xml_pcf_option.next_sibling();
  }

  return 0;
}

} /* end namespace openfpga */
