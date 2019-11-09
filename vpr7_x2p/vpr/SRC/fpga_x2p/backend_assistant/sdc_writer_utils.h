#ifndef SDC_WRITER_UTILS_H
#define SDC_WRITER_UTILS_H

#include <fstream>
#include <string>
#include "device_port.h"
#include "module_manager.h"

void print_sdc_file_header(std::fstream& fp,
                           const std::string& usage);

std::string generate_sdc_port(const BasicPort& port);

void print_pnr_sdc_constrain_module_port2port_timing(std::fstream& fp,
                                                     const ModuleManager& module_manager,
                                                     const ModuleId& module_id, 
                                                     const ModulePortId& module_input_port_id, 
                                                     const ModulePortId& module_output_port_id, 
                                                     const float& tmax);

#endif
