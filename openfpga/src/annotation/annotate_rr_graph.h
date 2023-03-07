#ifndef ANNOTATE_RR_GRAPH_H
#define ANNOTATE_RR_GRAPH_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "device_rr_gsb.h"
#include "openfpga_context.h"
#include "vpr_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void annotate_device_rr_gsb(const DeviceContext& vpr_device_ctx,
                            DeviceRRGSB& device_rr_gsb,
                            const bool& include_clock,
                            const bool& verbose_output);

void sort_device_rr_gsb_chan_node_in_edges(const RRGraphView& rr_graph,
                                           DeviceRRGSB& device_rr_gsb,
                                           const bool& verbose_output);

void sort_device_rr_gsb_ipin_node_in_edges(const RRGraphView& rr_graph,
                                           DeviceRRGSB& device_rr_gsb,
                                           const bool& verbose_output);

void annotate_rr_graph_circuit_models(
  const DeviceContext& vpr_device_ctx, const Arch& openfpga_arch,
  VprDeviceAnnotation& vpr_device_annotation, const bool& verbose_output);

} /* end namespace openfpga */

#endif
