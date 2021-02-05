/********************************************************************
 * This file includes functions that are used to annotate pb_type 
 * from VPR to OpenFPGA
 *******************************************************************/
#include <cmath>
#include <iterator>

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "pb_type_utils.h"
#include "annotate_bitstream_setting.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Annotate bitstream setting based on VPR device information
 *  - Find the pb_type and link to the bitstream source
 *  - Find the pb_type and link to the bitstream content
 *******************************************************************/
int annotate_bitstream_setting(const BitstreamSetting& bitstream_setting, 
                               const DeviceContext& vpr_device_ctx,
                               VprBitstreamAnnotation& vpr_bitstream_annotation) {

  for (const auto& bitstream_pb_type_setting_id : bitstream_setting.pb_type_settings()) {
    /* Get the full name of pb_type */
    std::vector<std::string> target_pb_type_names;
    std::vector<std::string> target_pb_mode_names;

    target_pb_type_names = bitstream_setting.parent_pb_type_names(bitstream_pb_type_setting_id);
    target_pb_type_names.push_back(bitstream_setting.pb_type_name(bitstream_pb_type_setting_id));
    target_pb_mode_names = bitstream_setting.parent_mode_names(bitstream_pb_type_setting_id);

    /* Pb type information are located at the logic_block_types in the device context of VPR
     * We iterate over the vectors and find the pb_type matches the parent_pb_type_name
     */
    bool link_success = false;

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
        continue;
      }
      /* Found one, build annotation */
      if (std::string("eblif") == bitstream_setting.pb_type_bitstream_source(bitstream_pb_type_setting_id)) {
        vpr_bitstream_annotation.set_pb_type_bitstream_source(target_pb_type, VprBitstreamAnnotation::e_bitstream_source_type::BITSTREAM_SOURCE_EBLIF);
      } else {
        /* Invalid source, error out! */
        VTR_LOG_ERROR("Invalid bitstream source '%s' for pb_type '%s' which is defined in bitstream setting\n",
                      bitstream_setting.pb_type_bitstream_source(bitstream_pb_type_setting_id).c_str(),
                      target_pb_type_names[0].c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      vpr_bitstream_annotation.set_pb_type_bitstream_content(target_pb_type, bitstream_setting.pb_type_bitstream_content(bitstream_pb_type_setting_id));

      link_success = true;
    }

    /* If fail to link bitstream setting to architecture, error out immediately */
    if (false == link_success) {
      VTR_LOG_ERROR("Fail to find a pb_type '%s' which is defined in bitstream setting from VPR architecture description\n",
                    target_pb_type_names[0].c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
