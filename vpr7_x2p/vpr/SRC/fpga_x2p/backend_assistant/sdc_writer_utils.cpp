/********************************************************************
 * This file include most utilized functions to be used in SDC writers 
 *******************************************************************/
#include <chrono>
#include <ctime>
#include <iomanip>

#include "fpga_x2p_utils.h"

#include "sdc_writer_utils.h"

/********************************************************************
 * Write a head (description) in SDC file 
 *******************************************************************/
void print_sdc_file_header(std::fstream& fp,
                           const std::string& usage) {

  check_file_handler(fp);

  auto end = std::chrono::system_clock::now(); 
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);

  fp << "#############################################" << std::endl;
  fp << "#\tSynopsys Design Constraints (SDC)" << std::endl;
  fp << "#\tFor FPGA fabric " << std::endl;
  fp << "#\tDescription: " << usage << std::endl;
  fp << "#\tAuthor: Xifan TANG " << std::endl;
  fp << "#\tOrganization: University of Utah " << std::endl;
  fp << "#\tDate: " << std::ctime(&end_time);
  fp << "#############################################" << std::endl;
  fp << std::endl;
}


/********************************************************************
 * Write a port in SDC format
 *******************************************************************/
std::string generate_sdc_port(const BasicPort& port) {
  std::string sdc_line;

  std::string size_str = "[" + std::to_string(port.get_lsb()) + ":" + std::to_string(port.get_msb()) + "]";

  /* Only connection require a format of <port_name>[<lsb>:<msb>]
   * others require a format of <port_type> [<lsb>:<msb>] <port_name> 
   */
  /* When LSB == MSB, we can use a simplified format <port_type>[<lsb>]*/
  if ( 1 == port.get_width()) {
    size_str = "[" + std::to_string(port.get_lsb()) + "]";
  }
  sdc_line = port.get_name() + size_str;

  return sdc_line;
}

/********************************************************************
 * Constrain a path between two ports of a module with a given timing value
 *******************************************************************/
void print_pnr_sdc_constrain_module_port2port_timing(std::fstream& fp,
                                                     const ModuleManager& module_manager,
                                                     const ModuleId& module_id, 
                                                     const ModulePortId& module_input_port_id, 
                                                     const ModulePortId& module_output_port_id, 
                                                     const float& tmax) {
  /* Validate file stream */
  check_file_handler(fp);

  fp << "set_max_delay";

  fp << " -from ";
  fp << module_manager.module_name(module_id) << "/";
  fp << generate_sdc_port(module_manager.module_port(module_id, module_input_port_id));

  fp << " -to ";
 
  fp << module_manager.module_name(module_id) << "/";
  fp << generate_sdc_port(module_manager.module_port(module_id, module_output_port_id));

  fp << " " << std::setprecision(10) << tmax;

  fp << std::endl;
}
