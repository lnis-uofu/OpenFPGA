#ifndef VERILOG_TESTBENCH_UTILS_H
#define VERILOG_TESTBENCH_UTILS_H

/* Include header files which are used in the function declaration */
#include <fstream>
#include <string>
#include <vector>
#include "module_manager.h"
#include "vpr_types.h"

void print_verilog_testbench_fpga_instance(std::fstream& fp,
                                           const ModuleManager& module_manager,
                                           const ModuleId& top_module,
                                           const std::string& top_instance_name);

void print_verilog_testbench_benchmark_instance(std::fstream& fp,
                                                const std::string& module_name,
                                                const std::string& instance_name,
                                                const std::string& module_input_port_postfix,
                                                const std::string& module_output_port_postfix,
                                                const std::string& output_port_postfix,
                                                const std::vector<t_logical_block>& L_logical_blocks,
                                                const bool& use_explicit_port_map);

void print_verilog_testbench_connect_fpga_ios(std::fstream& fp,
                                              const ModuleManager& module_manager,
                                              const ModuleId& top_module,
                                              const std::vector<t_logical_block>& L_logical_blocks,
                                              const vtr::Point<size_t>& device_size,
                                              const std::vector<std::vector<t_grid_tile>>& L_grids, 
                                              const std::vector<t_block>& L_blocks,
                                              const std::string& io_port_name_postfix,
                                              const size_t& unused_io_value);

void print_verilog_timeout_and_vcd(std::fstream& fp,
                                   const std::string& icarus_preprocessing_flag,
                                   const std::string& module_name,
                                   const std::string& vcd_fname,
                                   const std::string& simulation_start_counter_name,
                                   const std::string& error_counter_name,
                                   const int& simulation_time);

BasicPort generate_verilog_testbench_clock_port(const std::vector<std::string>& clock_port_names,
                                                const std::string& default_clock_name);

void print_verilog_testbench_check(std::fstream& fp,
                                   const std::string& autochecked_preprocessing_flag,
                                   const std::string& simulation_start_counter_name,
                                   const std::string& benchmark_port_postfix,
                                   const std::string& fpga_port_postfix,
                                   const std::string& check_flag_port_postfix,
                                   const std::string& error_counter_name,
                                   const std::vector<t_logical_block>& L_logical_blocks,
                                   const std::vector<std::string>& clock_port_names,
                                   const std::string& default_clock_name);

#endif
