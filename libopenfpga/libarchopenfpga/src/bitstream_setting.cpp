#include "vtr_assert.h"

#include "bitstream_setting.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Member functions for class BitstreamSetting
 ***********************************************************************/

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
BitstreamSetting::bitstream_pb_type_setting_range BitstreamSetting::pb_type_settings() const {
  return vtr::make_range(pb_type_setting_ids_.begin(), pb_type_setting_ids_.end());
}

/************************************************************************
 * Constructors
 ***********************************************************************/
BitstreamSetting::BitstreamSetting() {
  return;
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
std::string BitstreamSetting::pb_type_name(const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  return pb_type_names_[pb_type_setting_id];
}

std::vector<std::string> BitstreamSetting::parent_pb_type_names(const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  return parent_pb_type_names_[pb_type_setting_id];
}

std::vector<std::string> BitstreamSetting::parent_mode_names(const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  return parent_mode_names_[pb_type_setting_id];
}

std::string BitstreamSetting::pb_type_bitstream_source(const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  return pb_type_bitstream_sources_[pb_type_setting_id];
}

std::string BitstreamSetting::pb_type_bitstream_content(const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  return pb_type_bitstream_contents_[pb_type_setting_id];
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
BitstreamPbTypeSettingId BitstreamSetting::add_bitstream_pb_type_setting(const std::string& pb_type_name,
                                                                         const std::vector<std::string>& parent_pb_type_names,
                                                                         const std::vector<std::string>& parent_mode_names,
                                                                         const std::string& bitstream_source,
                                                                         const std::string& bitstream_content) {
  BitstreamPbTypeSettingId pb_type_setting_id = BitstreamPbTypeSettingId(pb_type_setting_ids_.size());
  pb_type_setting_ids_.push_back(pb_type_setting_id);
  pb_type_names_.push_back(pb_type_name);
  parent_pb_type_names_.push_back(parent_pb_type_names);
  parent_mode_names_.push_back(parent_mode_names);
  pb_type_bitstream_sources_.push_back(bitstream_source);
  pb_type_bitstream_contents_.push_back(bitstream_content);

  return pb_type_setting_id;
}

/************************************************************************
 * Public Validators
 ***********************************************************************/
bool BitstreamSetting::valid_bitstream_pb_type_setting_id(const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  return ( size_t(pb_type_setting_id) < pb_type_setting_ids_.size() ) && ( pb_type_setting_id == pb_type_setting_ids_[pb_type_setting_id] ); 
}

} /* namespace openfpga ends */
