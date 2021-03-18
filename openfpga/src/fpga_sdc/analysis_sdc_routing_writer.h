#ifndef ANALYSIS_SDC_ROUTING_WRITER_H
#define ANALYSIS_SDC_ROUTING_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <vector>
#include "vpr_context.h"
#include "module_manager.h"
#include "device_grid.h"
#include "device_rr_gsb.h"
#include "vpr_routing_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_analysis_sdc_disable_unused_cbs(std::fstream& fp,
                                           const AtomContext& atom_ctx, 
                                           const ModuleManager& module_manager, 
                                           const VprDeviceAnnotation& device_annotation, 
                                           const DeviceGrid& grids, 
                                           const RRGraph& rr_graph, 
                                           const VprRoutingAnnotation& routing_annotation, 
                                           const DeviceRRGSB& device_rr_gsb,
                                           const bool& compact_routing_hierarchy);

void print_analysis_sdc_disable_unused_sbs(std::fstream& fp,
                                           const AtomContext& atom_ctx, 
                                           const ModuleManager& module_manager, 
                                           const VprDeviceAnnotation& device_annotation, 
                                           const DeviceGrid& grids, 
                                           const RRGraph& rr_graph, 
                                           const VprRoutingAnnotation& routing_annotation, 
                                           const DeviceRRGSB& device_rr_gsb,
                                           const bool& compact_routing_hierarchy);

} /* end namespace openfpga */

#endif
