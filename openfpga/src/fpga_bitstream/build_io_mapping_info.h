#ifndef BUILD_IO_MAPPING_INFO_H
#define BUILD_IO_MAPPING_INFO_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "module_manager.h"
#include "vpr_context.h"
#include "io_location_map.h"
#include "io_map.h"
#include "vpr_netlist_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

IoMap build_fpga_io_mapping_info(const ModuleManager& module_manager,
                                 const ModuleId& top_module,
                                 const AtomContext& atom_ctx,
                                 const PlacementContext& place_ctx,
                                 const IoLocationMap& io_location_map,
                                 const VprNetlistAnnotation& netlist_annotation,
                                 const std::string& io_input_port_name_postfix,
                                 const std::string& io_output_port_name_postfix,
                                 const std::vector<std::string>& output_port_prefix_to_remove);

} /* end namespace openfpga */

#endif
