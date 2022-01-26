#ifndef SDC_WRITER_UTILS_H
#define SDC_WRITER_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <string>
#include "openfpga_port.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_sdc_file_header(std::fstream& fp,
                           const std::string& usage,
                           const bool& include_time_stamp);

void print_sdc_timescale(std::fstream& fp,
                         const std::string& timescale);

std::string generate_sdc_port(const BasicPort& port);

void print_pnr_sdc_constrain_max_delay(std::fstream& fp,
                                       const std::string& src_instance_name,
                                       const std::string& src_port_name,
                                       const std::string& des_instance_name,
                                       const std::string& des_port_name,
                                       const float& delay);

void print_pnr_sdc_regexp_constrain_max_delay(std::fstream& fp,
                                              const std::string& src_instance_name,
                                              const std::string& src_port_name,
                                              const std::string& des_instance_name,
                                              const std::string& des_port_name,
                                              const float& delay);

void print_pnr_sdc_constrain_min_delay(std::fstream& fp,
                                       const std::string& src_instance_name,
                                       const std::string& src_port_name,
                                       const std::string& des_instance_name,
                                       const std::string& des_port_name,
                                       const float& delay);

void print_pnr_sdc_constrain_module_port2port_timing(std::fstream& fp,
                                                     const ModuleManager& module_manager,
                                                     const ModuleId& input_parent_module_id, 
                                                     const ModulePortId& module_input_port_id, 
                                                     const ModuleId& output_parent_module_id, 
                                                     const ModulePortId& module_output_port_id, 
                                                     const float& tmax);

void print_pnr_sdc_constrain_port2port_timing(std::fstream& fp,
                                              const ModuleManager& module_manager,
                                              const ModuleId& input_parent_module_id, 
                                              const ModulePortId& module_input_port_id, 
                                              const ModuleId& output_parent_module_id, 
                                              const ModulePortId& module_output_port_id, 
                                              const float& tmax);

void print_sdc_disable_port_timing(std::fstream& fp,
                                   const BasicPort& port);

void print_sdc_set_port_input_delay(std::fstream& fp,
                                    const BasicPort& port,
                                    const BasicPort& clock_port,
                                    const float& delay);

void print_sdc_set_port_output_delay(std::fstream& fp,
                                     const BasicPort& port,
                                     const BasicPort& clock_port,
                                     const float& delay);

int rec_print_sdc_disable_timing_for_module_ports(std::fstream& fp, 
                                                  const bool& flatten_names,
                                                  const ModuleManager& module_manager, 
                                                  const ModuleId& parent_module,
                                                  const ModuleId& module_to_disable,
                                                  const std::string& parent_module_path,
                                                  const std::string& disable_port_name);

} /* end namespace openfpga */

#endif
