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

/* Headers from openfpgautil library */
#include "openfpga_tokenizer.h"

#include "pb_type_utils.h"
#include "annotate_bitstream_setting.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Annotate bitstream setting based on VPR device information
 *  - Find the pb_type and link to the bitstream source
 *  - Find the pb_type and link to the bitstream content
 *******************************************************************/
static 
int annotate_bitstream_pb_type_setting(const BitstreamSetting& bitstream_setting, 
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
      if (std::string("eblif") != bitstream_setting.pb_type_bitstream_source(bitstream_pb_type_setting_id)) {
        /* Invalid source, error out! */
        VTR_LOG_ERROR("Invalid bitstream source '%s' for pb_type '%s' which is defined in bitstream setting\n",
                      bitstream_setting.pb_type_bitstream_source(bitstream_pb_type_setting_id).c_str(),
                      target_pb_type_names[0].c_str());
        return CMD_EXEC_FATAL_ERROR;
      }

      /* Depending on the bitstream type, annotate through different entrances
       * - For regular bitstream, set bitstream content, flags etc. 
       * - For mode-select bitstream, set mode-select bitstream content, flags etc. 
       */
      if (false == bitstream_setting.is_mode_select_bitstream(bitstream_pb_type_setting_id)) {
        vpr_bitstream_annotation.set_pb_type_bitstream_source(target_pb_type, VprBitstreamAnnotation::e_bitstream_source_type::BITSTREAM_SOURCE_EBLIF);
        vpr_bitstream_annotation.set_pb_type_bitstream_content(target_pb_type, bitstream_setting.pb_type_bitstream_content(bitstream_pb_type_setting_id));
        vpr_bitstream_annotation.set_pb_type_bitstream_offset(target_pb_type, bitstream_setting.bitstream_offset(bitstream_pb_type_setting_id));
      } else {
        VTR_ASSERT_SAFE(false == bitstream_setting.is_mode_select_bitstream(bitstream_pb_type_setting_id));
        vpr_bitstream_annotation.set_pb_type_mode_select_bitstream_source(target_pb_type, VprBitstreamAnnotation::e_bitstream_source_type::BITSTREAM_SOURCE_EBLIF);
        vpr_bitstream_annotation.set_pb_type_mode_select_bitstream_content(target_pb_type, bitstream_setting.pb_type_bitstream_content(bitstream_pb_type_setting_id));
        vpr_bitstream_annotation.set_pb_type_mode_select_bitstream_offset(target_pb_type, bitstream_setting.bitstream_offset(bitstream_pb_type_setting_id));
      }

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

/********************************************************************
 * Annotate bitstream setting based on VPR device information
 *  - Find the interconnect and link to the default path id
 *******************************************************************/
static 
int annotate_bitstream_interconnect_setting(const BitstreamSetting& bitstream_setting, 
                                            const DeviceContext& vpr_device_ctx,
                                            const VprDeviceAnnotation& vpr_device_annotation,
                                            VprBitstreamAnnotation& vpr_bitstream_annotation) {

  for (const auto& bitstream_interc_setting_id : bitstream_setting.interconnect_settings()) {
    /* Get the full name of pb_type */
    std::vector<std::string> target_pb_type_names;
    std::vector<std::string> target_pb_mode_names;

    target_pb_type_names = bitstream_setting.parent_pb_type_names(bitstream_interc_setting_id);
    target_pb_mode_names = bitstream_setting.parent_mode_names(bitstream_interc_setting_id);
    /* Kick out the last mode so that we can use an existing function search the pb_type in graph */
    std::string expected_physical_mode_name = target_pb_mode_names.back();
    target_pb_mode_names.pop_back();

    std::string interconnect_name = bitstream_setting.interconnect_name(bitstream_interc_setting_id);
    std::string expected_input_path = bitstream_setting.default_path(bitstream_interc_setting_id);

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
      t_mode* physical_mode = vpr_device_annotation.physical_mode(target_pb_type);
    
      VTR_ASSERT(nullptr != physical_mode);
      /* Ensure that the annotation is only applicable to physical mode  */ 
      if (std::string(physical_mode->name) != expected_physical_mode_name) {
        VTR_LOG_ERROR("The physical mode '%s' under pb_type '%s' does not match in the bitstream setting '%s'!\n",
                      physical_mode->name, 
                      target_pb_type->name, 
                      expected_physical_mode_name.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
    
      /* Find the interconnect name under the physical mode of a physical pb_type */
      t_interconnect* pb_interc = find_pb_mode_interconnect(physical_mode, interconnect_name.c_str());
    
      if (nullptr == pb_interc) {
        VTR_LOG_ERROR("Unable to find interconnect '%s' under physical mode '%s' of pb_type '%s'!\n",
                      interconnect_name.c_str(), 
                      physical_mode->name, 
                      target_pb_type->name);
        return CMD_EXEC_FATAL_ERROR;
      }

      /* Find the default path and spot the path id from the input string recorded */
      StringToken input_string_tokenizer(std::string(pb_interc->input_string)); 
      std::vector<std::string> input_paths = input_string_tokenizer.split(' ');
      size_t input_path_id = input_paths.size();
      for (size_t ipath = 0; ipath < input_paths.size(); ++ipath) {
        if (expected_input_path == input_paths[ipath]) {
          input_path_id = ipath;
          break;
        }
      }
      /* If the input_path id is invalid, error out! */
      if (input_path_id == input_paths.size()) {
        VTR_LOG_ERROR("Invalid default path '%s' for interconnect '%s' which inputs are '%s'\n",
                      expected_input_path.c_str(),
                      interconnect_name.c_str(),
                      pb_interc->input_string);
        return CMD_EXEC_FATAL_ERROR;
      }

      vpr_bitstream_annotation.set_interconnect_default_path_id(pb_interc, input_path_id);

      link_success = true;
    }

    /* If fail to link bitstream setting to architecture, error out immediately */
    if (false == link_success) {
      VTR_LOG_ERROR("Fail to find an interconnect '%s' with default path '%s', which is defined in bitstream setting from VPR architecture description\n",
                    interconnect_name.c_str(),
                    expected_input_path.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Annotate bitstream setting based on VPR device information
 *  - Fill pb_type -related mapping
 *  - Fill interconnect -related mapping
 *******************************************************************/
int annotate_bitstream_setting(const BitstreamSetting& bitstream_setting, 
                               const DeviceContext& vpr_device_ctx,
                               const VprDeviceAnnotation& vpr_device_annotation,
                               VprBitstreamAnnotation& vpr_bitstream_annotation) {

  int status = CMD_EXEC_SUCCESS;
  
  status = annotate_bitstream_pb_type_setting(bitstream_setting, 
                                              vpr_device_ctx,
                                              vpr_bitstream_annotation);
  if (status == CMD_EXEC_FATAL_ERROR) {
    return status;
  }

  status = annotate_bitstream_interconnect_setting(bitstream_setting, 
                                                   vpr_device_ctx, vpr_device_annotation,
                                                   vpr_bitstream_annotation);

  return status;
}

} /* end namespace openfpga */
