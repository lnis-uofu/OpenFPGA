/********************************************************************
 * This file include most utilized functions to be used in SDC writers 
 *******************************************************************/
#include <chrono>
#include <ctime>
#include <iomanip>

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "sdc_writer_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Write a head (description) in SDC file 
 *******************************************************************/
void print_sdc_file_header(std::fstream& fp,
                           const std::string& usage) {

  valid_file_stream(fp);

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
 * Constrain a path between two ports of a module with a given maximum timing value
 *******************************************************************/
void print_pnr_sdc_constrain_max_delay(std::fstream& fp,
                                       const std::string& src_instance_name,
                                       const std::string& src_port_name,
                                       const std::string& des_instance_name,
                                       const std::string& des_port_name,
                                       const float& delay) {
  /* Validate file stream */
  valid_file_stream(fp);

  fp << "set_max_delay";

  fp << " -from ";
  if (!src_instance_name.empty()) {
    fp << src_instance_name << "/";
  }
  fp << src_port_name;

  fp << " -to ";
 
  if (!des_instance_name.empty()) {
    fp << des_instance_name << "/";
  }
  fp << des_port_name;

  fp << " " << std::setprecision(10) << delay;

  fp << std::endl;
}

/********************************************************************
 * Constrain a path between two ports of a module with a given minimum timing value
 *******************************************************************/
void print_pnr_sdc_constrain_min_delay(std::fstream& fp,
                                       const std::string& src_instance_name,
                                       const std::string& src_port_name,
                                       const std::string& des_instance_name,
                                       const std::string& des_port_name,
                                       const float& delay) {
  /* Validate file stream */
  valid_file_stream(fp);

  fp << "set_min_delay";

  fp << " -from ";
  if (!src_instance_name.empty()) {
    fp << src_instance_name << "/";
  }
  fp << src_port_name;

  fp << " -to ";
 
  if (!des_instance_name.empty()) {
    fp << des_instance_name << "/";
  }
  fp << des_port_name;

  fp << " " << std::setprecision(10) << delay;

  fp << std::endl;
}

/********************************************************************
 * Constrain a path between two ports of a module with a given timing value
 * Note: this function uses set_max_delay !!!
 *******************************************************************/
void print_pnr_sdc_constrain_module_port2port_timing(std::fstream& fp,
                                                     const ModuleManager& module_manager,
                                                     const ModuleId& input_parent_module_id, 
                                                     const ModulePortId& module_input_port_id, 
                                                     const ModuleId& output_parent_module_id, 
                                                     const ModulePortId& module_output_port_id, 
                                                     const float& tmax) {
  print_pnr_sdc_constrain_max_delay(fp,
                                    module_manager.module_name(input_parent_module_id),
                                    generate_sdc_port(module_manager.module_port(input_parent_module_id, module_input_port_id)),
                                    module_manager.module_name(output_parent_module_id),
                                    generate_sdc_port(module_manager.module_port(output_parent_module_id, module_output_port_id)),
                                    tmax);

}

/********************************************************************
 * Constrain a path between two ports of a module with a given timing value
 * This function will NOT output the module name
 * Note: this function uses set_max_delay !!!
 *******************************************************************/
void print_pnr_sdc_constrain_port2port_timing(std::fstream& fp,
                                              const ModuleManager& module_manager,
                                              const ModuleId& input_parent_module_id, 
                                              const ModulePortId& module_input_port_id, 
                                              const ModuleId& output_parent_module_id, 
                                              const ModulePortId& module_output_port_id, 
                                              const float& tmax) {
  print_pnr_sdc_constrain_max_delay(fp,
                                    std::string(),
                                    generate_sdc_port(module_manager.module_port(input_parent_module_id, module_input_port_id)),
                                    std::string(),
                                    generate_sdc_port(module_manager.module_port(output_parent_module_id, module_output_port_id)),
                                    tmax);

}

/********************************************************************
 * Disable timing for a port 
 *******************************************************************/
void print_sdc_disable_port_timing(std::fstream& fp,
                                   const BasicPort& port) {
  /* Validate file stream */
  valid_file_stream(fp);

  fp << "set_disable_timing ";

  fp << generate_sdc_port(port);

  fp << std::endl;
}

/********************************************************************
 * Set the input delay for a port in SDC format 
 * Note that the input delay will be bounded by a clock port
 *******************************************************************/
void print_sdc_set_port_input_delay(std::fstream& fp,
                                    const BasicPort& port,
                                    const BasicPort& clock_port,
                                    const float& delay) {
  /* Validate file stream */
  valid_file_stream(fp);

  fp << "set_input_delay ";

  fp << "-clock ";

  fp << generate_sdc_port(clock_port);

  fp << " -max ";
 
  fp << std::setprecision(10) << delay;

  fp << " ";

  fp << generate_sdc_port(port);

  fp << std::endl;
}

/********************************************************************
 * Set the output delay for a port in SDC format 
 * Note that the output delay will be bounded by a clock port
 *******************************************************************/
void print_sdc_set_port_output_delay(std::fstream& fp,
                                     const BasicPort& port,
                                     const BasicPort& clock_port,
                                     const float& delay) {
  /* Validate file stream */
  valid_file_stream(fp);

  fp << "set_output_delay ";

  fp << "-clock ";

  fp << generate_sdc_port(clock_port);

  fp << " -max ";
 
  fp << std::setprecision(10) << delay;

  fp << " ";

  fp << generate_sdc_port(port);

  fp << std::endl;
}

} /* end namespace openfpga */
