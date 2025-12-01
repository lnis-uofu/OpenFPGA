/************************************************************************
 * Member functions for class VprBitstreamAnnotation
 ***********************************************************************/
#include "vpr_bitstream_annotation.h"

#include "mux_bitstream_constants.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/
VprBitstreamAnnotation::VprBitstreamAnnotation() { return; }

/************************************************************************
 * Public accessors
 ***********************************************************************/
std::vector<VprBitstreamAnnotation::BitstreamSourceInfo>
VprBitstreamAnnotation::pb_type_bitstream_sources(t_pb_type* pb_type) const {
  auto result = bitstream_sources_.find(pb_type);
  if (result != bitstream_sources_.end()) {
    return result->second;
  }

  /* Not found, return an empty vector */
  return std::vector<VprBitstreamAnnotation::BitstreamSourceInfo>();
}

std::string VprBitstreamAnnotation::pb_type_default_mode_bits(
  t_pb_type* pb_type) const {
  auto result = default_mode_bits_.find(pb_type);
  if (result != default_mode_bits_.end()) {
    return result->second;
  }

  /* Not found, return an invalid type */
  return std::string();
}

std::vector<VprBitstreamAnnotation::BitstreamSourceInfo>
VprBitstreamAnnotation::pb_type_mode_select_bitstream_sources(
  t_pb_type* pb_type) const {
  auto result = mode_select_bitstream_sources_.find(pb_type);
  if (result != mode_select_bitstream_sources_.end()) {
    return result->second;
  }

  /* Not found, return an empty vector */
  return std::vector<VprBitstreamAnnotation::BitstreamSourceInfo>();
}

size_t VprBitstreamAnnotation::interconnect_default_path_id(
  t_interconnect* interconnect) const {
  auto result = interconnect_default_path_ids_.find(interconnect);
  if (result != interconnect_default_path_ids_.end()) {
    return result->second;
  }

  /* Not found, return an invalid input id */
  return DEFAULT_PATH_ID;
}

ClockTreePinId VprBitstreamAnnotation::clock_tap_routing_pin(
  const ClockTreeId& tree_id) const {
  auto result = clock_tap_routing_pins_.find(tree_id);
  if (result != clock_tap_routing_pins_.end()) {
    return result->second;
  }

  /* Not found, return an invalid input id */
  return ClockTreePinId::INVALID();
}

/************************************************************************
 * Public mutators
 ***********************************************************************/
bool VprBitstreamAnnotation::add_pb_type_bitstream_source(
  t_pb_type* pb_type, const BitstreamSourceInfo& bitstream_source) {
  /* Check if there is any duplication */
  for (const auto& src_info : bitstream_sources_[pb_type]) {
    if (src_info.overlap(bitstream_source)) {
      VTR_LOG_ERROR(
        "The bitstream source (eblif) has a collison: '%s' for pb_type '%s' is "
        "defined more than once!\n",
        src_info.content.c_str(), pb_type->name);
      return false;
    }
  }
  bitstream_sources_[pb_type].push_back(bitstream_source);
  return true;
}

bool VprBitstreamAnnotation::add_pb_type_mode_select_bitstream_source(
  t_pb_type* pb_type, const BitstreamSourceInfo& bitstream_source) {
  /* Check if there is any duplication */
  for (const auto& src_info : mode_select_bitstream_sources_[pb_type]) {
    if (src_info.overlap(bitstream_source)) {
      VTR_LOG_ERROR(
        "The mode-select bitstream source (eblif) has a collison: '%s' for "
        "pb_type '%s' is defined more than once!\n",
        src_info.content.c_str(), pb_type->name);
      return false;
    }
  }
  mode_select_bitstream_sources_[pb_type].push_back(bitstream_source);
  return true;
}

void VprBitstreamAnnotation::set_pb_type_default_mode_bits(
  t_pb_type* pb_type, const std::string& default_mode_bits) {
  default_mode_bits_[pb_type] = default_mode_bits;
}

void VprBitstreamAnnotation::set_interconnect_default_path_id(
  t_interconnect* interconnect, const size_t& default_path_id) {
  interconnect_default_path_ids_[interconnect] = default_path_id;
}

void VprBitstreamAnnotation::set_clock_tap_routing_pin(
  const ClockTreeId& tree_id, const ClockTreePinId& tree_pin_id) {
  auto result = clock_tap_routing_pins_.find(tree_id);
  if (result != clock_tap_routing_pins_.end()) {
    VTR_LOG_WARN(
      "Overwrite the clock tree pin '%lu' for clock tree '%d' tap routing (Was "
      "pin '%lu')\n",
      size_t(tree_pin_id), size_t(tree_id), size_t(result->second));
  }
  clock_tap_routing_pins_[tree_id] = tree_pin_id;
}

} /* End namespace openfpga*/
