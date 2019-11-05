#ifndef VERILOG_AUXILIARY_NETLISTS_H
#define VERILOG_AUXILIARY_NETLISTS_H

#include <string>
#include "circuit_library.h"
#include "vpr_types.h"

void print_include_netlists(const std::string& src_dir,
                            const std::string& circuit_name,
                            const std::string& reference_benchmark_file,
                            const CircuitLibrary& circuit_lib);

void print_verilog_preprocessing_flags_netlist(const std::string& src_dir,
                                               const t_syn_verilog_opts& fpga_verilog_opts);

#endif 
