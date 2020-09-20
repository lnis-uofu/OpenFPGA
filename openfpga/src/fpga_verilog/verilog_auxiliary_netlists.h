#ifndef VERILOG_AUXILIARY_NETLISTS_H
#define VERILOG_AUXILIARY_NETLISTS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "circuit_library.h"
#include "fabric_verilog_options.h"
#include "netlist_manager.h"
#include "verilog_testbench_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_fabric_include_netlist(const NetlistManager& netlist_manager,
                                          const std::string& src_dir,
                                          const CircuitLibrary& circuit_lib);

void print_verilog_testbench_include_netlists(const std::string& src_dir,
                                              const std::string& circuit_name,
                                              const std::string& reference_benchmark_file);

void print_verilog_preprocessing_flags_netlist(const std::string& src_dir,
                                               const FabricVerilogOption& fabric_verilog_opts);

void print_verilog_simulation_preprocessing_flags(const std::string& src_dir,
                                                  const VerilogTestbenchOption& verilog_testbench_opts);

} /* end namespace openfpga */

#endif 
