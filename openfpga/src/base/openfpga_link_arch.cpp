/********************************************************************
 * This file includes functions to read an OpenFPGA architecture file
 * which are built on the libarchopenfpga library
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_log.h"

#include "vpr_pb_type_annotation.h"
#include "openfpga_link_arch.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function will recursively traverse pb_type graph to ensure
 * 1. there is only a physical mode under each pb_type
 * 2. physical mode appears only when its parent is a physical mode.
 *******************************************************************/
static 
void rec_check_pb_type_physical_mode(const t_pb_type& cur_pb_type) {
  /* We do not check any primitive pb_type */
  if (true == cur_pb_type.is_primitive()) {
    return;
  }

  /* For non-primitive pb_type: we should iterate over each mode 
   * Ensure there is only one physical mode
   */

  /* Traverse all the modes for identifying idle mode */
  for (int imode = 0; cur_pb_type.num_modes; ++imode) {
    /* Check each pb_type_child */
    for (int ichild = 0; ichild < cur_pb_type.modes[imode].num_pb_type_children; ++ichild) { 
      rec_check_pb_type_physical_mode(cur_pb_type.modes[imode].pb_type_children[ichild]);
    }
  }
}

/********************************************************************
 * This function will 
 * - identify the physical pb_type for each multi-mode pb_type in 
 *   VPR pb_type graph
 * - identify the physical pb_type for operating pb_types in VPR 
 *******************************************************************/
static 
void build_physical_pb_type_vpr_annotation(const DeviceContext& vpr_device_ctx, 
                                           const Arch& openfpga_arch,
                                           VprPbTypeAnnotation& vpr_pb_type_annotation) {
  /* Walk through the pb_type annotation stored in the openfpga arch */
  for (const PbTypeAnnotation& pb_type_annotation : openfpga_arch.pb_type_annotations) {
    /* Pb type information are located at the logic_block_types in the device context of VPR
     * We iterate over the vectors and annotate the pb_type  
     */
    for (const t_logical_block_type& lb_type : vpr_device_ctx.logical_block_types) {
      /* By pass nullptr for pb_type head */
      if (nullptr == lb_type.pb_type) {
        continue;
      }
    }
  } 
}

/********************************************************************
 * Top-level function to link openfpga architecture to VPR, including:
 * - physical pb_type
 * - idle pb_type 
 * - circuit models for pb_type, pb interconnect and routing architecture
 *******************************************************************/
void link_arch(OpenfpgaContext& openfpga_context) {

  /* Annotate physical pb_type in the VPR pb_type graph */
  build_physical_pb_type_vpr_annotation(g_vpr_ctx.device(), openfpga_context.arch(),
                                        openfpga_context.mutable_vpr_pb_type_annotation());

  /* Annotate idle pb_type in the VPR pb_type graph */

  /* Link physical pb_type to circuit model */

  /* Link routing architecture to circuit model */
} 

} /* end namespace openfpga */

