#ifndef VERILOG_PRECONFIG_TOP_MODULE_H
#define VERILOG_PRECONFIG_TOP_MODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include <string>
#include "circuit_library.h" 
#include "vpr_context.h" 
#include "module_manager.h" 
#include "bitstream_manager.h" 
#include "io_location_map.h" 

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_preconfig_top_module(const ModuleManager& module_manager,
                                        const BitstreamManager& bitstream_manager,
                                        const CircuitLibrary& circuit_lib,
                                        const std::vector<CircuitPortId>& global_ports,
                                        const AtomContext& atom_ctx,
                                        const PlacementContext& place_ctx,
                                        const IoLocationMap& io_location_map,
                                        const std::string& circuit_name,
                                        const std::string& verilog_fname,
                                        const std::string& verilog_dir);

} /* end namespace openfpga */

#endif
