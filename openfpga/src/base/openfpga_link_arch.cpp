/********************************************************************
 * This file includes functions to read an OpenFPGA architecture file
 * which are built on the libarchopenfpga library
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "vpr_pb_type_annotation.h"
#include "pb_type_utils.h"
#include "openfpga_link_arch.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function will traverse pb_type graph from its top to find
 * a pb_type with a given name as well as its hierarchy 
 *******************************************************************/
static 
t_pb_type* try_find_pb_type_with_given_path(t_pb_type* top_pb_type, 
                                            const std::vector<std::string>& target_pb_type_names, 
                                            const std::vector<std::string>& target_pb_mode_names) {
  /* Ensure that number of parent names and modes matches */
  VTR_ASSERT_SAFE(target_pb_type_names.size() == target_pb_mode_names.size() + 1);

  t_pb_type* cur_pb_type = top_pb_type;

  /* We start from the first element of the parent names and parent modes.
   * If the pb_type does not match in name, we fail 
   * If we cannot find a mode match the name, we fail 
   */
  for (size_t i = 0; i < target_pb_type_names.size() - 1; ++i) {
    /* If this level does not match, search fail */
    if (target_pb_type_names[i] != std::string(cur_pb_type->name)) {
      return nullptr;
    }
    /* Find if the mode matches */
    t_mode* cur_mode = find_pb_type_mode(cur_pb_type, target_pb_mode_names[i].c_str()); 
    if (nullptr == cur_mode) {
      return nullptr;
    }
    /* Go to the next level of pb_type */
    cur_pb_type = find_mode_child_pb_type(cur_mode, target_pb_type_names[i + 1].c_str());
    if (nullptr == cur_pb_type) {
      return nullptr;
    }
    /* If this is already the last pb_type in the list, this is what we want */
    if (i == target_pb_type_names.size() - 1) {
      return cur_pb_type;
    }
  }

  /* Reach here, it means we find nothing */
  return nullptr;
}

/********************************************************************
 * This function will recursively traverse pb_type graph to ensure
 * 1. there is only a physical mode under each pb_type
 * 2. physical mode appears only when its parent is a physical mode.
 *******************************************************************/
static 
void rec_check_pb_type_physical_mode(t_pb_type* cur_pb_type) {
  /* We do not check any primitive pb_type */
  if (true == is_primitive_pb_type(cur_pb_type)) {
    return;
  }

  /* For non-primitive pb_type: we should iterate over each mode 
   * Ensure there is only one physical mode
   */

  /* Traverse all the modes for identifying idle mode */
  for (int imode = 0; cur_pb_type->num_modes; ++imode) {
    /* Check each pb_type_child */
    for (int ichild = 0; ichild < cur_pb_type->modes[imode].num_pb_type_children; ++ichild) { 
      rec_check_pb_type_physical_mode(&(cur_pb_type->modes[imode].pb_type_children[ichild]));
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
void build_vpr_physical_pb_type_annotation(const DeviceContext& vpr_device_ctx, 
                                           const Arch& openfpga_arch,
                                           VprPbTypeAnnotation& vpr_pb_type_annotation) {
  /* Walk through the pb_type annotation stored in the openfpga arch */
  for (const PbTypeAnnotation& pb_type_annotation : openfpga_arch.pb_type_annotations) {
    /* Since our target is to annotate the physical mode name, 
     * we can skip those has not physical mode defined
     */
    if (true == pb_type_annotation.physical_mode_name().empty()) {
      continue;
    } 

    /* Identify if the pb_type is operating or physical,
     * For operating pb_type, get the full name of operating pb_type    
     * For physical pb_type, get the full name of physical pb_type    
     */
    std::vector<std::string> target_pb_type_names;
    std::vector<std::string> target_pb_mode_names;

    if (true == pb_type_annotation.is_operating_pb_type()) {
      target_pb_type_names = pb_type_annotation.operating_parent_pb_type_names();
      target_pb_type_names.push_back(pb_type_annotation.operating_pb_type_name());
      target_pb_mode_names = pb_type_annotation.operating_parent_mode_names();
    } 

    if (true == pb_type_annotation.is_physical_pb_type()) {
      target_pb_type_names = pb_type_annotation.physical_parent_pb_type_names();
      target_pb_type_names.push_back(pb_type_annotation.physical_pb_type_name());
      target_pb_mode_names = pb_type_annotation.physical_parent_mode_names();
    } 

    /* We must have at least one pb_type in the list */
    VTR_ASSERT_SAFE(0 < target_pb_type_names.size());

    /* Pb type information are located at the logic_block_types in the device context of VPR
     * We iterate over the vectors and find the pb_type matches the parent_pb_type_name
     */
    for (const t_logical_block_type& lb_type : vpr_device_ctx.logical_block_types) {
      /* By pass nullptr for pb_type head */
      if (nullptr == lb_type.pb_type) {
        continue;
      }
      /* Check the name of the top-level pb_type, if it does not match, we can bypass */
      if (target_pb_type_names[0] != std::string(lb_type.pb_type->name)) {
        continue;
      }
      /* Match the name in the top-level, we go further to search the pb_type in the graph */
      t_pb_type* target_pb_type = try_find_pb_type_with_given_path(lb_type.pb_type, target_pb_type_names, 
                                                                   target_pb_mode_names);
      if (nullptr == target_pb_type) {
        /* Not found, error out! */
        VTR_LOG_ERROR("Unable to find the pb_type '%s' in VPR architecture definition!\n",
                      target_pb_type_names.back().c_str());
      }

      /* Found, we update the annotation by assigning the physical mode */
      t_mode* physical_mode = find_pb_type_mode(target_pb_type, pb_type_annotation.physical_mode_name().c_str());
      vpr_pb_type_annotation.add_pb_type_physical_mode(target_pb_type, physical_mode);
      
      /* Give a message */
      VTR_LOG("Annotate pb_type '%s' with physical mode '%s'\n",
              target_pb_type->name, physical_mode->name);
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
  build_vpr_physical_pb_type_annotation(g_vpr_ctx.device(), openfpga_context.arch(),
                                        openfpga_context.mutable_vpr_pb_type_annotation());

  /* Annotate idle pb_type in the VPR pb_type graph */

  /* Link physical pb_type to circuit model */

  /* Link routing architecture to circuit model */
} 

} /* end namespace openfpga */
