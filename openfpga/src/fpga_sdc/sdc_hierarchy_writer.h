#ifndef SDC_HIERARCHY_WRITER_H
#define SDC_HIERARCHY_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "openfpga_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_pnr_sdc_routing_sb_hierarchy(const std::string& sdc_dir,
                                        const ModuleManager& module_manager,
                                        const ModuleId& top_module,
                                        const DeviceRRGSB& device_rr_gsb);

void print_pnr_sdc_routing_cb_hierarchy(const std::string& sdc_dir,
                                        const ModuleManager& module_manager,
                                        const ModuleId& top_module,
                                        const t_rr_type& cb_type,
                                        const DeviceRRGSB& device_rr_gsb);

void print_pnr_sdc_grid_hierarchy(const std::string& sdc_dir,
                                  const DeviceContext& device_ctx,
                                  const VprDeviceAnnotation& device_annotation,
                                  const ModuleManager& module_manager,
                                  const ModuleId& top_module);


} /* end namespace openfpga */

#endif
