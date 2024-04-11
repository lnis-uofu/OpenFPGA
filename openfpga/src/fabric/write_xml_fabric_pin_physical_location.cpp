/***************************************************************************************
 * Output internal structure of module graph to XML format
 ***************************************************************************************/
/* Headers from system goes first */
#include <algorithm>
#include <chrono>
#include <ctime>
#include <regex>
#include <string>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "command_exit_codes.h"
#include "openfpga_digest.h"
#include "openfpga_side_manager.h"

/* Headers from arch openfpga library */
#include "fabric_pin_physical_location_xml_constants.h"
#include "write_xml_fabric_pin_physical_location.h"
#include "write_xml_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function write header information to a pin location file
 *******************************************************************/
static void write_xml_fabric_pin_physical_location_file_head(
  std::fstream& fp, const bool& include_time_stamp) {
  valid_file_stream(fp);

  fp << "<!--" << std::endl;
  fp << "\t- Fabric Pin Physical Location" << std::endl;
  fp << "\t- Author: Xifan TANG" << std::endl;
  fp << "\t- Organization: RapidFlex" << std::endl;

  if (include_time_stamp) {
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    fp << "\t- Date: " << std::ctime(&end_time);
  }

  fp << "-->" << std::endl;
  fp << std::endl;
}

/********************************************************************
 * This function write header information to a pin location file
 *******************************************************************/
static int write_xml_fabric_module_pin_phy_loc(
  std::fstream& fp, const ModuleManager& module_manager,
  const ModuleId& curr_module, const bool& show_invalid_side,
  const bool& verbose) {
  valid_file_stream(fp);

  /* If show invalid side is off, we should check if there is any valid side. If
   * there are not any, skip this module */
  bool skip_curr_module = true;
  for (ModulePortId curr_port_id : module_manager.module_ports(curr_module)) {
    SideManager side_mgr(module_manager.port_side(curr_module, curr_port_id));
    if (side_mgr.validate()) {
      skip_curr_module = false;
      break;
    }
  }

  if (!show_invalid_side && skip_curr_module) {
    VTR_LOGV(verbose, "Skip module '%s' as it contains no valid sides\n",
             module_manager.module_name(curr_module).c_str());
    return CMD_EXEC_SUCCESS;
  }
  /* Print a head */
  write_tab_to_file(fp, 1);
  fp << "<" << XML_MODULE_NODE_NAME;
  write_xml_attribute(fp, XML_MODULE_ATTRIBUTE_NAME,
                      module_manager.module_name(curr_module).c_str());
  fp << ">"
     << "\n";

  size_t cnt = 0;
  for (ModulePortId curr_port_id : module_manager.module_ports(curr_module)) {
    BasicPort curr_port = module_manager.module_port(curr_module, curr_port_id);
    SideManager side_mgr(module_manager.port_side(curr_module, curr_port_id));
    if (!side_mgr.validate() && !show_invalid_side) {
      continue;
    }
    for (int curr_pin_id : curr_port.pins()) {
      BasicPort curr_pin(curr_port.get_name(), curr_pin_id, curr_pin_id);
      std::string curr_port_str = generate_xml_port_name(curr_pin);
      write_tab_to_file(fp, 2);
      fp << "<" << XML_MODULE_PINLOC_NODE_NAME;
      write_xml_attribute(fp, XML_MODULE_PINLOC_ATTRIBUTE_PIN,
                          curr_port_str.c_str());
      write_xml_attribute(fp, XML_MODULE_PINLOC_ATTRIBUTE_SIDE,
                          side_mgr.c_str());
      fp << "/>";
      fp << std::endl;
    }
    cnt++;
  }
  VTR_LOGV(verbose, "Output %lu ports with physical sides for module '%s'\n",
           cnt, module_manager.module_name(curr_module).c_str());

  /* Print a tail */
  write_tab_to_file(fp, 1);
  fp << "</" << XML_MODULE_NODE_NAME;
  fp << ">"
     << "\n";

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Top-level function
 *******************************************************************/
int write_xml_fabric_pin_physical_location(const char* fname,
                                           const std::string& module_name,
                                           const ModuleManager& module_manager,
                                           const bool& show_invalid_side,
                                           const bool& include_time_stamp,
                                           const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Write fabric pin physical location");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  write_xml_fabric_pin_physical_location_file_head(fp, include_time_stamp);

  /* Write the root node */
  fp << "<" << XML_PINLOC_ROOT_NAME;
  fp << ">"
     << "\n";

  /* If module name is not specified, walk through all the modules and write
   * physical pin location when any is specified */
  short cnt = 0;
  /* Use regular expression to capture the module whose name matches the pattern
   */
  for (ModuleId curr_module : module_manager.modules()) {
    std::string curr_module_name = module_manager.module_name(curr_module);
    std::string pattern = module_name;
    std::regex star_replace("\\*");
    std::regex questionmark_replace("\\?");
    std::string wildcard_pattern =
      std::regex_replace(std::regex_replace(pattern, star_replace, ".*"),
                         questionmark_replace, ".");
    std::regex wildcard_regex(wildcard_pattern);
    if (!std::regex_match(curr_module_name, wildcard_regex)) {
      continue;
    }
    VTR_LOGV(verbose, "Outputted pin physical location of module '%s'.\n",
             curr_module_name.c_str());
    /* Write the pin physical location for this module */
    int err_code = write_xml_fabric_module_pin_phy_loc(
      fp, module_manager, curr_module, show_invalid_side, verbose);
    if (err_code != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
    cnt++;
  }

  /* Finish writing the root node */
  fp << "</" << XML_PINLOC_ROOT_NAME << ">"
     << "\n";

  /* Close the file stream */
  fp.close();

  /* If there is no match, error out! */
  if (cnt == 0) {
    VTR_LOG_ERROR(
      "Invalid regular expression for module name '%s' which does not match "
      "any in current fabric!\n",
      module_name.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  VTR_LOGV(verbose, "Outputted %lu modules with pin physical location.\n", cnt);

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
