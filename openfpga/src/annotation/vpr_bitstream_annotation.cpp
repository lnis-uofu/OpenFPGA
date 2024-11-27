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
VprBitstreamAnnotation::e_bitstream_source_type
VprBitstreamAnnotation::pb_type_bitstream_source(t_pb_type* pb_type) const {
  auto result = bitstream_sources_.find(pb_type);
  if (result != bitstream_sources_.end()) {
    return result->second;
  }

  /* Not found, return an invalid type*/
  return NUM_BITSTREAM_SOURCE_TYPES;
}

std::string VprBitstreamAnnotation::pb_type_bitstream_content(
  t_pb_type* pb_type) const {
  auto result = bitstream_contents_.find(pb_type);
  if (result != bitstream_contents_.end()) {
    return result->second;
  }

  /* Not found, return an invalid type */
  return std::string();
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

size_t VprBitstreamAnnotation::pb_type_bitstream_offset(
  t_pb_type* pb_type) const {
  auto result = bitstream_offsets_.find(pb_type);
  if (result != bitstream_offsets_.end()) {
    return result->second;
  }

  /* Not found, return an zero offset */
  return 0;
}

VprBitstreamAnnotation::e_bitstream_source_type
VprBitstreamAnnotation::pb_type_mode_select_bitstream_source(
  t_pb_type* pb_type) const {
  auto result = mode_select_bitstream_sources_.find(pb_type);
  if (result != mode_select_bitstream_sources_.end()) {
    return result->second;
  }

  /* Not found, return an invalid type*/
  return NUM_BITSTREAM_SOURCE_TYPES;
}

std::string VprBitstreamAnnotation::pb_type_mode_select_bitstream_content(
  t_pb_type* pb_type) const {
  auto result = mode_select_bitstream_contents_.find(pb_type);
  if (result != mode_select_bitstream_contents_.end()) {
    return result->second;
  }

  /* Not found, return an invalid type */
  return std::string();
}

size_t VprBitstreamAnnotation::pb_type_mode_select_bitstream_offset(
  t_pb_type* pb_type) const {
  auto result = mode_select_bitstream_offsets_.find(pb_type);
  if (result != mode_select_bitstream_offsets_.end()) {
    return result->second;
  }

  /* Not found, return an zero offset */
  return 0;
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
void VprBitstreamAnnotation::set_pb_type_bitstream_source(
  t_pb_type* pb_type, const e_bitstream_source_type& bitstream_source) {
  bitstream_sources_[pb_type] = bitstream_source;
}

void VprBitstreamAnnotation::set_pb_type_bitstream_content(
  t_pb_type* pb_type, const std::string& bitstream_content) {
  bitstream_contents_[pb_type] = bitstream_content;
}

void VprBitstreamAnnotation::set_pb_type_bitstream_offset(
  t_pb_type* pb_type, const size_t& offset) {
  bitstream_offsets_[pb_type] = offset;
}

void VprBitstreamAnnotation::set_pb_type_mode_select_bitstream_source(
  t_pb_type* pb_type, const e_bitstream_source_type& bitstream_source) {
  mode_select_bitstream_sources_[pb_type] = bitstream_source;
}

void VprBitstreamAnnotation::set_pb_type_mode_select_bitstream_content(
  t_pb_type* pb_type, const std::string& bitstream_content) {
  mode_select_bitstream_contents_[pb_type] = bitstream_content;
}

void VprBitstreamAnnotation::set_pb_type_mode_select_bitstream_offset(
  t_pb_type* pb_type, const size_t& offset) {
  mode_select_bitstream_offsets_[pb_type] = offset;
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
