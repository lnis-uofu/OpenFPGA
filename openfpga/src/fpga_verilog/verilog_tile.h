#ifndef VERILOG_TILE_H
#define VERILOG_TILE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

#include "fabric_tile.h"
#include "fabric_verilog_options.h"
#include "module_manager.h"
#include "module_name_map.h"
#include "netlist_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_verilog_tiles(NetlistManager& netlist_manager,
                        const ModuleManager& module_manager,
                        const ModuleNameMap& module_name_map,
                        const std::string& verilog_dir,
                        const FabricTile& fabric_tile,
                        const std::string& subckt_dir_name,
                        const FabricVerilogOption& options);

} /* end namespace openfpga */

#endif
