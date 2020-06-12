#ifndef VERILOG_TOP_TESTBENCH
#define VERILOG_TOP_TESTBENCH

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "module_manager.h"
#include "bitstream_manager.h"
#include "fabric_bitstream.h"
#include "circuit_library.h"
#include "vpr_context.h"
#include "io_location_map.h"
#include "vpr_netlist_annotation.h"
#include "simulation_setting.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_top_testbench(const ModuleManager& module_manager,
                                 const BitstreamManager& bitstream_manager,
                                 const FabricBitstream& fabric_bitstream,
                                 const e_config_protocol_type& sram_orgz_type,
                                 const CircuitLibrary& circuit_lib,
                                 const std::vector<CircuitPortId>& global_ports,
                                 const AtomContext& atom_ctx,
                                 const PlacementContext& place_ctx,
                                 const IoLocationMap& io_location_map,
                                 const VprNetlistAnnotation& netlist_annotation,
                                 const std::string& circuit_name,
                                 const std::string& verilog_fname,
                                 const SimulationSetting& simulation_parameters,
                                 const bool& fast_configuration,
                                 const bool& explicit_port_mapping);

} /* end namespace openfpga */

#endif
