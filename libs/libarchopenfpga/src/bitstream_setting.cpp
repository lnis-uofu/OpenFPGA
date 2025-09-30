#include "bitstream_setting.h"

#include "vtr_assert.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Member functions for class BitstreamSetting
 ***********************************************************************/

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
BitstreamSetting::bitstream_pb_type_setting_range
BitstreamSetting::pb_type_settings() const {
  return vtr::make_range(pb_type_setting_ids_.begin(),
                         pb_type_setting_ids_.end());
}

BitstreamSetting::bitstream_default_mode_setting_range
BitstreamSetting::default_mode_settings() const {
  return vtr::make_range(default_mode_setting_ids_.begin(),
                         default_mode_setting_ids_.end());
}

BitstreamSetting::bitstream_clock_routing_setting_range
BitstreamSetting::clock_routing_settings() const {
  return vtr::make_range(clock_routing_setting_ids_.begin(),
                         clock_routing_setting_ids_.end());
}

BitstreamSetting::bitstream_interconnect_setting_range
BitstreamSetting::interconnect_settings() const {
  return vtr::make_range(interconnect_setting_ids_.begin(),
                         interconnect_setting_ids_.end());
}

BitstreamSetting::overwrite_bitstream_range
BitstreamSetting::overwrite_bitstreams() const {
  return vtr::make_range(overwrite_bitstream_ids_.begin(),
                         overwrite_bitstream_ids_.end());
}

/************************************************************************
 * Constructors
 ***********************************************************************/
BitstreamSetting::BitstreamSetting() { return; }

/************************************************************************
 * Public Accessors
 ***********************************************************************/
std::string BitstreamSetting::pb_type_name(
  const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  return pb_type_names_[pb_type_setting_id];
}

std::vector<std::string> BitstreamSetting::parent_pb_type_names(
  const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  return parent_pb_type_names_[pb_type_setting_id];
}

std::vector<std::string> BitstreamSetting::parent_mode_names(
  const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  return parent_mode_names_[pb_type_setting_id];
}

std::string BitstreamSetting::pb_type_bitstream_source(
  const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  return pb_type_bitstream_sources_[pb_type_setting_id];
}

std::string BitstreamSetting::pb_type_bitstream_content(
  const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  return pb_type_bitstream_contents_[pb_type_setting_id];
}

bool BitstreamSetting::is_mode_select_bitstream(
  const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  return is_mode_select_bitstreams_[pb_type_setting_id];
}

size_t BitstreamSetting::bitstream_offset(
  const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  return bitstream_offsets_[pb_type_setting_id];
}

std::string BitstreamSetting::default_mode_pb_type_name(
  const BitstreamDefaultModeSettingId& default_mode_setting_id) const {
  VTR_ASSERT(true ==
             valid_bitstream_default_mode_setting_id(default_mode_setting_id));
  return default_mode_pb_type_names_[default_mode_setting_id];
}

std::vector<std::string> BitstreamSetting::default_mode_parent_pb_type_names(
  const BitstreamDefaultModeSettingId& default_mode_setting_id) const {
  VTR_ASSERT(true ==
             valid_bitstream_default_mode_setting_id(default_mode_setting_id));
  return default_mode_parent_pb_type_names_[default_mode_setting_id];
}

std::vector<std::string> BitstreamSetting::default_mode_parent_mode_names(
  const BitstreamDefaultModeSettingId& default_mode_setting_id) const {
  VTR_ASSERT(true ==
             valid_bitstream_default_mode_setting_id(default_mode_setting_id));
  return default_mode_parent_mode_names_[default_mode_setting_id];
}

std::vector<char> BitstreamSetting::default_mode_bits(
  const BitstreamDefaultModeSettingId& default_mode_setting_id) const {
  VTR_ASSERT(true ==
             valid_bitstream_default_mode_setting_id(default_mode_setting_id));
  return pb_type_default_mode_bits_[default_mode_setting_id];
}

std::string BitstreamSetting::default_mode_bits_to_string(
  const BitstreamDefaultModeSettingId& default_mode_setting_id) const {
  VTR_ASSERT(true ==
             valid_bitstream_default_mode_setting_id(default_mode_setting_id));
  std::string mode_bits_str;
  for (const char& bit : pb_type_default_mode_bits_[default_mode_setting_id]) {
    mode_bits_str += bit;
  }
  return mode_bits_str;
}

std::string BitstreamSetting::clock_routing_network(
  const BitstreamClockRoutingSettingId& clock_routing_setting_id) const {
  VTR_ASSERT(
    true == valid_bitstream_clock_routing_setting_id(clock_routing_setting_id));
  return clock_routing_network_names_[clock_routing_setting_id];
}

BasicPort BitstreamSetting::clock_routing_pin(
  const BitstreamClockRoutingSettingId& clock_routing_setting_id) const {
  VTR_ASSERT(
    true == valid_bitstream_clock_routing_setting_id(clock_routing_setting_id));
  return clock_routing_pins_[clock_routing_setting_id];
}

std::string BitstreamSetting::interconnect_name(
  const BitstreamInterconnectSettingId& interconnect_setting_id) const {
  VTR_ASSERT(true ==
             valid_bitstream_interconnect_setting_id(interconnect_setting_id));
  return interconnect_names_[interconnect_setting_id];
}

std::vector<std::string> BitstreamSetting::parent_pb_type_names(
  const BitstreamInterconnectSettingId& interconnect_setting_id) const {
  VTR_ASSERT(true ==
             valid_bitstream_interconnect_setting_id(interconnect_setting_id));
  return interconnect_parent_pb_type_names_[interconnect_setting_id];
}

std::vector<std::string> BitstreamSetting::parent_mode_names(
  const BitstreamInterconnectSettingId& interconnect_setting_id) const {
  VTR_ASSERT(true ==
             valid_bitstream_interconnect_setting_id(interconnect_setting_id));
  return interconnect_parent_mode_names_[interconnect_setting_id];
}

std::string BitstreamSetting::default_path(
  const BitstreamInterconnectSettingId& interconnect_setting_id) const {
  VTR_ASSERT(true ==
             valid_bitstream_interconnect_setting_id(interconnect_setting_id));
  return interconnect_default_paths_[interconnect_setting_id];
}

std::vector<NonFabricBitstreamSetting> BitstreamSetting::non_fabric() const {
  return non_fabric_;
}

std::string BitstreamSetting::overwrite_bitstream_path(
  const OverwriteBitstreamId& id) const {
  VTR_ASSERT(true == valid_overwrite_bitstream_id(id));
  return overwrite_bitstream_paths_[id];
}

bool BitstreamSetting::overwrite_bitstream_value(
  const OverwriteBitstreamId& id) const {
  VTR_ASSERT(true == valid_overwrite_bitstream_id(id));
  return overwrite_bitstream_values_[id];
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
BitstreamPbTypeSettingId BitstreamSetting::add_bitstream_pb_type_setting(
  const std::string& pb_type_name,
  const std::vector<std::string>& parent_pb_type_names,
  const std::vector<std::string>& parent_mode_names,
  const std::string& bitstream_source, const std::string& bitstream_content) {
  BitstreamPbTypeSettingId pb_type_setting_id =
    BitstreamPbTypeSettingId(pb_type_setting_ids_.size());
  pb_type_setting_ids_.push_back(pb_type_setting_id);
  pb_type_names_.push_back(pb_type_name);
  parent_pb_type_names_.push_back(parent_pb_type_names);
  parent_mode_names_.push_back(parent_mode_names);
  pb_type_bitstream_sources_.push_back(bitstream_source);
  pb_type_bitstream_contents_.push_back(bitstream_content);
  is_mode_select_bitstreams_.push_back(false);
  bitstream_offsets_.push_back(0);

  return pb_type_setting_id;
}

void BitstreamSetting::set_mode_select_bitstream(
  const BitstreamPbTypeSettingId& pb_type_setting_id,
  const bool& is_mode_select_bitstream) {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  is_mode_select_bitstreams_[pb_type_setting_id] = is_mode_select_bitstream;
}

void BitstreamSetting::set_bitstream_offset(
  const BitstreamPbTypeSettingId& pb_type_setting_id, const size_t& offset) {
  VTR_ASSERT(true == valid_bitstream_pb_type_setting_id(pb_type_setting_id));
  bitstream_offsets_[pb_type_setting_id] = offset;
}

BitstreamDefaultModeSettingId
BitstreamSetting::add_bitstream_default_mode_setting(
  const std::string& pb_type_name,
  const std::vector<std::string>& parent_pb_type_names,
  const std::vector<std::string>& parent_mode_names,
  const std::vector<char>& mode_bits) {
  BitstreamDefaultModeSettingId default_mode_setting_id =
    BitstreamDefaultModeSettingId(default_mode_setting_ids_.size());
  default_mode_setting_ids_.push_back(default_mode_setting_id);
  default_mode_pb_type_names_.push_back(pb_type_name);
  default_mode_parent_pb_type_names_.push_back(parent_pb_type_names);
  default_mode_parent_mode_names_.push_back(parent_mode_names);
  pb_type_default_mode_bits_.push_back(mode_bits);

  return default_mode_setting_id;
}

BitstreamClockRoutingSettingId
BitstreamSetting::add_bitstream_clock_routing_setting(
  const std::string& ntwk_name, const BasicPort& pin) {
  BitstreamClockRoutingSettingId clock_routing_setting_id =
    BitstreamClockRoutingSettingId(clock_routing_setting_ids_.size());
  clock_routing_setting_ids_.push_back(clock_routing_setting_id);
  clock_routing_network_names_.push_back(ntwk_name);
  clock_routing_pins_.push_back(pin);

  return clock_routing_setting_id;
}

BitstreamInterconnectSettingId
BitstreamSetting::add_bitstream_interconnect_setting(
  const std::string& interconnect_name,
  const std::vector<std::string>& parent_pb_type_names,
  const std::vector<std::string>& parent_mode_names,
  const std::string& default_path) {
  BitstreamInterconnectSettingId interc_setting_id =
    BitstreamInterconnectSettingId(interconnect_setting_ids_.size());
  interconnect_setting_ids_.push_back(interc_setting_id);
  interconnect_names_.push_back(interconnect_name);
  interconnect_parent_pb_type_names_.push_back(parent_pb_type_names);
  interconnect_parent_mode_names_.push_back(parent_mode_names);
  interconnect_default_paths_.push_back(default_path);

  return interc_setting_id;
}

void BitstreamSetting::add_non_fabric(const std::string& name,
                                      const std::string& file) {
  VTR_ASSERT(name.size());
  VTR_ASSERT(file.size());
  non_fabric_.push_back(NonFabricBitstreamSetting(name, file));
}

void BitstreamSetting::add_non_fabric_pb(const std::string& pb,
                                         const std::string& content) {
  VTR_ASSERT(non_fabric_.size());
  VTR_ASSERT(content.find(".param ") == 0 || content.find(".attr ") == 0);
  if (content.find(".param ") == 0) {
    VTR_ASSERT(content.size() > 7);
    non_fabric_.back().add_pb(pb, "param", content.substr(7));
  } else {
    VTR_ASSERT(content.size() > 6);
    non_fabric_.back().add_pb(pb, "attr", content.substr(6));
  }
}

OverwriteBitstreamId BitstreamSetting::add_overwrite_bitstream(
  const std::string& path, const bool& value) {
  VTR_ASSERT(path.size());
  VTR_ASSERT(overwrite_bitstream_ids_.size() ==
             overwrite_bitstream_paths_.size());
  VTR_ASSERT(overwrite_bitstream_paths_.size() ==
             overwrite_bitstream_values_.size());
  OverwriteBitstreamId id =
    OverwriteBitstreamId(overwrite_bitstream_ids_.size());
  overwrite_bitstream_ids_.push_back(id);
  overwrite_bitstream_paths_.push_back(path);
  overwrite_bitstream_values_.push_back(value);
  return id;
}

/************************************************************************
 * Public Validators
 ***********************************************************************/
bool BitstreamSetting::valid_bitstream_pb_type_setting_id(
  const BitstreamPbTypeSettingId& pb_type_setting_id) const {
  return (size_t(pb_type_setting_id) < pb_type_setting_ids_.size()) &&
         (pb_type_setting_id == pb_type_setting_ids_[pb_type_setting_id]);
}

bool BitstreamSetting::valid_bitstream_default_mode_setting_id(
  const BitstreamDefaultModeSettingId& default_mode_setting_id) const {
  return (size_t(default_mode_setting_id) < default_mode_setting_ids_.size()) &&
         (default_mode_setting_id ==
          default_mode_setting_ids_[default_mode_setting_id]);
}

bool BitstreamSetting::valid_bitstream_clock_routing_setting_id(
  const BitstreamClockRoutingSettingId& clock_routing_setting_id) const {
  return (size_t(clock_routing_setting_id) <
          clock_routing_setting_ids_.size()) &&
         (clock_routing_setting_id ==
          clock_routing_setting_ids_[clock_routing_setting_id]);
}

bool BitstreamSetting::valid_bitstream_interconnect_setting_id(
  const BitstreamInterconnectSettingId& interconnect_setting_id) const {
  return (size_t(interconnect_setting_id) < interconnect_setting_ids_.size()) &&
         (interconnect_setting_id ==
          interconnect_setting_ids_[interconnect_setting_id]);
}

bool BitstreamSetting::valid_overwrite_bitstream_id(
  const OverwriteBitstreamId& id) const {
  VTR_ASSERT(overwrite_bitstream_ids_.size() ==
             overwrite_bitstream_paths_.size());
  VTR_ASSERT(overwrite_bitstream_paths_.size() ==
             overwrite_bitstream_values_.size());
  return (size_t(id) < overwrite_bitstream_ids_.size()) &&
         (id == overwrite_bitstream_ids_[id]);
}

}  // namespace openfpga
