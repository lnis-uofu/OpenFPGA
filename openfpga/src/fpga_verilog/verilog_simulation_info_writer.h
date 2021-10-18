#ifndef VERILOG_SIMULATION_INFO_WRITER_H
#define VERILOG_SIMULATION_INFO_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "module_manager.h"
#include "config_protocol.h"
#include "vpr_context.h"
#include "io_location_map.h"
#include "verilog_testbench_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_simulation_info(const std::string& ini_fname,
                                   const VerilogTestbenchOption& options,
                                   const std::string& circuit_name,
                                   const std::string& src_dir,
                                   const AtomContext& atom_ctx,
                                   const PlacementContext& place_ctx,
                                   const IoLocationMap& io_location_map,
                                   const ModuleManager& module_manager,
                                   const e_config_protocol_type& config_protocol_type,
                                   const size_t& num_program_clock_cycles,
                                   const int& num_operating_clock_cycles,
                                   const float& prog_clock_freq,
                                   const float& op_clock_freq);

} /* end namespace openfpga */

#endif
