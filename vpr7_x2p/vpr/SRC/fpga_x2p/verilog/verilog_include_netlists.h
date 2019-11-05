#ifndef VERILOG_INCLUDE_NETLISTS_H
#define VERILOG_INCLUDE_NETLISTS_H

#include <string>
#include "circuit_library.h"

void print_include_netlists(const std::string& src_dir,
                            const std::string& circuit_name,
                            const std::string& reference_benchmark_file,
                            const CircuitLibrary& circuit_lib);

void write_include_netlists (char* src_dir_formatted,
                            char* chomped_circuit_name, 
							t_spice spice);

#endif 
