#ifndef PNR_SDC_ROUTING_WRITER_H
#define PNR_SDC_ROUTING_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "module_manager.h"
#include "device_rr_gsb.h"
#include "rr_graph_obj.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_pnr_sdc_flatten_routing_constrain_sb_timing(const std::string& sdc_dir,
                                                       const bool& hierarchical,
                                                       const ModuleManager& module_manager,
                                                       const ModuleId& top_module,
                                                       const RRGraph& rr_graph,
                                                       const DeviceRRGSB& device_rr_gsb,
                                                       const bool& constrain_zero_delay_paths);

void print_pnr_sdc_compact_routing_constrain_sb_timing(const std::string& sdc_dir,
                                                       const bool& hierarchical,
                                                       const ModuleManager& module_manager,
                                                       const ModuleId& top_module,
                                                       const RRGraph& rr_graph,
                                                       const DeviceRRGSB& device_rr_gsb,
                                                       const bool& constrain_zero_delay_paths);

void print_pnr_sdc_flatten_routing_constrain_cb_timing(const std::string& sdc_dir,
                                                       const ModuleManager& module_manager, 
                                                       const RRGraph& rr_graph,
                                                       const DeviceRRGSB& device_rr_gsb,
                                                       const bool& constrain_zero_delay_paths);

void print_pnr_sdc_compact_routing_constrain_cb_timing(const std::string& sdc_dir,
                                                       const ModuleManager& module_manager,
                                                       const RRGraph& rr_graph,
                                                       const DeviceRRGSB& device_rr_gsb,
                                                       const bool& constrain_zero_delay_paths);

} /* end namespace openfpga */

#endif
