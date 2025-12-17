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

std::string VprBitstreamAnnotation::pb_type_pcf_mode_bits_to_string(
  t_pb_type* pb_type) const {
  std::string mode_bits_str;
  for (const char& bit : pb_type_pcf_mode_bits(pb_type)) {
    mode_bits_str += bit;
  }
  return mode_bits_str;
}

std::vector<char> VprBitstreamAnnotation::pb_type_pcf_mode_bits(
  t_pb_type* pb_type) const {
  /* Ensure that the pb_type is in the list */
  std::map<t_pb_type*, std::vector<char>>::const_iterator it =
    pb_type_pcf_mode_bits_.find(pb_type);
  if (it == pb_type_pcf_mode_bits_.end()) {
    /* Return an empty vector */
    return std::vector<char>();
  }
  return pb_type_pcf_mode_bits_.at(pb_type);
}

void VprBitstreamAnnotation::add_pb_type_pcf_mode_bits(
  t_pb_type* pb_type, const std::vector<char>& mode_bits, const bool& verbose) {
  /* Warn any override attempt */
  std::map<t_pb_type*, std::vector<char>>::const_iterator it =
    pb_type_pcf_mode_bits_.find(pb_type);
  if (it != pb_type_pcf_mode_bits_.end()) {
    VTR_LOGV_WARN(verbose,
                  "Override the pcf mode bits mapping for pb_type '%s'!\n",
                  pb_type->name);
  }

  pb_type_pcf_mode_bits_[pb_type] = mode_bits;
}

void VprBitstreamAnnotation::add_pb_type_pcf_pins(t_pb_type* pb_type,
                                                  const BasicPort& pcf_pin,
                                                  const bool& verbose) {
  /* Warn any override attempt */
  std::map<t_pb_type*, BasicPort>::const_iterator it =
    pb_type_pcf_pins_.find(pb_type);
  if (it != pb_type_pcf_pins_.end()) {
    VTR_LOGV_WARN(verbose, "Override the pcf pin mapping for pb_type '%s'!\n",
                  pcf_pin.get_name().c_str());
  }

  pb_type_pcf_pins_[pb_type] = pcf_pin;
}
std::map<t_pb_type*, BasicPort> VprBitstreamAnnotation::pb_type_pcf_pins()
  const {
  return pb_type_pcf_pins_;
}

void VprBitstreamAnnotation::add_pcf_coord_pb_type(
  const std::array<size_t, 3>& coord, t_pb_type* pb_type) {
  pcf_coord_pb_type_[coord] = pb_type;
}

t_pb_type* VprBitstreamAnnotation::pcf_coord_pb_type(
  const std::array<size_t, 3>& coord) const {
  auto it = pcf_coord_pb_type_.find(coord);
  if (it == pcf_coord_pb_type_.end()) {
    return nullptr;
  }
  VTR_LOG("\n PCF mode bits specified for location: <%d, %d, %d> \n", coord[0],
          coord[1], coord[2]);
  return it->second;
}

} /* End namespace openfpga*/
