#ifndef SIMULATION_INFO_WRITER_H
#define SIMULATION_INFO_WRITER_H

#include <string>

void print_verilog_simulation_info(const int& num_operating_clock_cycles,
                                   const std::string& verilog_dir_formatted,
                                   const std::string& chomped_circuit_name,
                                   const std::string& src_dir_path,
                                   const size_t& num_program_clock_cycles,
                                   const float& prog_clock_freq,
                                   const float& op_clock_freq);
#endif
