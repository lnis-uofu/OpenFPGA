#ifndef VERILOG_TESTBENCH_UTILS_H
#define VERILOG_TESTBENCH_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <string>
#include <vector>
#include "module_manager.h"
#include "circuit_library.h"
#include "vpr_context.h"
#include "io_location_map.h"
#include "vpr_netlist_annotation.h"
#include "fabric_global_port_info.h"
#include "pin_constraints.h"
#include "simulation_setting.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

constexpr char* VPR_BENCHMARK_OUT_PORT_PREFIX = "out:";
constexpr char* OPENFPGA_BENCHMARK_OUT_PORT_PREFIX = "out_";

void print_verilog_testbench_fpga_instance(std::fstream& fp,
                                           const ModuleManager& module_manager,
                                           const ModuleId& top_module,
                                           const std::string& top_instance_name,
                                           const bool& explicit_port_mapping);

void print_verilog_testbench_benchmark_instance(std::fstream& fp,
                                                const std::string& module_name,
                                                const std::string& instance_name,
                                                const std::string& module_input_port_postfix,
                                                const std::string& module_output_port_postfix,
                                                const std::vector<std::string>& output_port_prefix_to_remove,
                                                const std::string& output_port_postfix,
                                                const AtomContext& atom_ctx,
                                                const VprNetlistAnnotation& netlist_annotation,
                                                const bool& use_explicit_port_map);

void print_verilog_testbench_connect_fpga_ios(std::fstream& fp,
                                              const ModuleManager& module_manager,
                                              const ModuleId& top_module,
                                              const AtomContext& atom_ctx,
                                              const PlacementContext& place_ctx,
                                              const IoLocationMap& io_location_map,
                                              const VprNetlistAnnotation& netlist_annotation,
                                              const std::string& io_input_port_name_postfix,
                                              const std::string& io_output_port_name_postfix,
                                              const size_t& unused_io_value);

void print_verilog_timeout_and_vcd(std::fstream& fp,
                                   const std::string& module_name,
                                   const std::string& vcd_fname,
                                   const std::string& simulation_start_counter_name,
                                   const std::string& error_counter_name,
                                   const float& simulation_time);

std::vector<BasicPort> generate_verilog_testbench_clock_port(const std::vector<std::string>& clock_port_names,
                                                             const std::string& default_clock_name);

void print_verilog_testbench_check(std::fstream& fp,
                                   const std::string& autochecked_preprocessing_flag,
                                   const std::string& simulation_start_counter_name,
                                   const std::string& benchmark_port_postfix,
                                   const std::string& fpga_port_postfix,
                                   const std::string& check_flag_port_postfix,
                                   const std::string& error_counter_name,
                                   const AtomContext& atom_ctx,
                                   const VprNetlistAnnotation& netlist_annotation,
                                   const std::vector<std::string>& clock_port_names,
                                   const std::string& default_clock_name);

void print_verilog_testbench_clock_stimuli(std::fstream& fp,
                                           const PinConstraints& pin_constraints,
                                           const SimulationSetting& simulation_parameters,
                                           const std::vector<BasicPort>& clock_ports);

void print_verilog_testbench_random_stimuli(std::fstream& fp,
                                            const AtomContext& atom_ctx,
                                            const VprNetlistAnnotation& netlist_annotation,
                                            const ModuleManager& module_manager,
                                            const FabricGlobalPortInfo& global_ports,
                                            const PinConstraints& pin_constraints,
                                            const std::vector<std::string>& clock_port_names,
                                            const std::string& check_flag_port_postfix,
                                            const std::vector<BasicPort>& clock_ports);

void print_verilog_testbench_shared_ports(std::fstream& fp,
                                          const AtomContext& atom_ctx,
                                          const VprNetlistAnnotation& netlist_annotation,
                                          const std::vector<std::string>& clock_port_names,
                                          const std::string& benchmark_output_port_postfix,
                                          const std::string& fpga_output_port_postfix,
                                          const std::string& check_flag_port_postfix,
                                          const std::string& autocheck_preprocessing_flag);

void print_verilog_testbench_signal_initialization(std::fstream& fp,
                                                   const std::string& top_instance_name,
                                                   const CircuitLibrary& circuit_lib,
                                                   const ModuleManager& module_manager,
                                                   const ModuleId& top_module);

} /* end namespace openfpga */

#endif
