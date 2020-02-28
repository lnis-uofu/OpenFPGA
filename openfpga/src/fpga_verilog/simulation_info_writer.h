#ifndef SIMULATION_INFO_WRITER_H
#define SIMULATION_INFO_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_simulation_info(const std::string& ini_fname,
                                   const std::string& circuit_name,
                                   const std::string& src_dir,
                                   const size_t& num_program_clock_cycles,
                                   const int& num_operating_clock_cycles,
                                   const float& prog_clock_freq,
                                   const float& op_clock_freq);

} /* end namespace openfpga */

#endif
