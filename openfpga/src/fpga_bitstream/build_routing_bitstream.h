/********************************************************************
 * Header file for build_routing_bitstream.cpp
 *******************************************************************/
#ifndef BUILD_ROUTING_BITSTREAM_H
#define BUILD_ROUTING_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "bitstream_manager.h"
#include "vpr_context.h"
#include "module_manager.h"
#include "circuit_library.h"
#include "mux_library.h"
#include "device_rr_gsb.h"
#include "vpr_device_annotation.h"
#include "vpr_routing_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void build_routing_bitstream(BitstreamManager& bitstream_manager,
                             const ConfigBlockId& top_configurable_block,
                             const ModuleManager& module_manager,
                             const CircuitLibrary& circuit_lib,
                             const MuxLibrary& mux_lib,
                             const AtomContext& atom_ctx,
                             const VprDeviceAnnotation& device_annotation,
                             const VprRoutingAnnotation& routing_annotation,
                             const RRGraph& rr_graph,
                             const DeviceRRGSB& device_rr_gsb,
                             const bool& compact_routing_hierarchy);

} /* end namespace openfpga */

#endif
