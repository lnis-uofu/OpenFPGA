/***************************************************************************************
 * Output internal structure of module graph to XML format
 ***************************************************************************************/
/* Headers from system goes first */
#include <algorithm>
#include <chrono>
#include <ctime>
#include <string>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
#include "openfpga_side_manager.h"
#include "command_exit_codes.h"

/* Headers from arch openfpga library */
#include "write_xml_utils.h"

#include "fabric_pin_physical_location_xml_constants.h"
#include "write_xml_fabric_pin_physical_location.h"

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
  std::fstream& fp, const ModuleManager& module_manager, const ModuleId& curr_module) {
  valid_file_stream(fp);

  /* Print a head */
  write_tab_to_file(fp, 1);
  fp << "<" << XML_MODULE_NODE_NAME;
  write_xml_attribute(fp, XML_MODULE_ATTRIBUTE_NAME, module_manager.module_name(curr_module).c_str());
  fp << ">"
     << "\n";

  for (ModulePortId curr_port_id : module_manager.module_ports(curr_module)) {
    BasicPort curr_port = module_manager.module_port(curr_module, curr_port_id);
    SideManager side_mgr(module_manager.port_side(curr_module, curr_port_id));
    for (int curr_pin_id : curr_port.pins()) {
      BasicPort curr_pin(curr_port.get_name(), curr_pin_id, curr_pin_id); 
      std::string curr_port_str = generate_xml_port_name(curr_pin);
      write_tab_to_file(fp, 2);
      fp << "<" << XML_MODULE_PINLOC_NODE_NAME;
      write_xml_attribute(fp, XML_MODULE_PINLOC_ATTRIBUTE_PIN, curr_port_str.c_str());
      write_xml_attribute(fp, XML_MODULE_PINLOC_ATTRIBUTE_SIDE, side_mgr.c_str());
      fp << "/>";
      fp << std::endl;
    }
  }

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
int write_xml_fabric_pin_physical_location(
  const char* fname, const std::string& module_name,
  const ModuleManager& module_manager,
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

  /* If module name is not specified, walk through all the modules and write physical pin location when any is specified */
  short cnt = 0;
  if (module_name.empty()) {
    for (ModuleId curr_module : module_manager.modules()) {
      int err_code = write_xml_fabric_module_pin_phy_loc(fp, module_manager, curr_module);
      if (err_code != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_FATAL_ERROR;
      }
      cnt++;
    }
  } else {
    /* Check if the module name is valid or not, if not, error out */
    ModuleId curr_module = module_manager.find_module(module_name);
    if (!module_manager.valid_module_id(curr_module)) {
      VTR_LOG_ERROR("Invalid module name '%s' which does not exist in current fabric!\n", module_name.c_str());
      return CMD_EXEC_FATAL_ERROR;
    } 
    /* Write the pin physical location for this module */
    int err_code = write_xml_fabric_module_pin_phy_loc(fp, module_manager, curr_module);
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

  VTR_LOGV(verbose, "Outputted %lu modules with pin physical location.\n", cnt);

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
