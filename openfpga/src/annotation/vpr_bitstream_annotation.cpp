/************************************************************************
 * Member functions for class VprBitstreamAnnotation
 ***********************************************************************/
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vpr_bitstream_annotation.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/
VprBitstreamAnnotation::VprBitstreamAnnotation() {
  return;
}

/************************************************************************
 * Public accessors
 ***********************************************************************/
VprBitstreamAnnotation::e_bitstream_source_type VprBitstreamAnnotation::pb_type_bitstream_source(t_pb_type* pb_type) const {
  auto result = bitstream_sources_.find(pb_type);
  if (result != bitstream_sources_.end()) {
    return result->second;
  }

  /* Not found, return an invalid type*/
  return NUM_BITSTREAM_SOURCE_TYPES;
}

std::string VprBitstreamAnnotation::pb_type_bitstream_content(t_pb_type* pb_type) const {
  auto result = bitstream_contents_.find(pb_type);
  if (result != bitstream_contents_.end()) {
    return result->second;
  }

  /* Not found, return an invalid type */
  return std::string();
}

size_t VprBitstreamAnnotation::pb_type_bitstream_offset(t_pb_type* pb_type) const {
  auto result = bitstream_offsets_.find(pb_type);
  if (result != bitstream_offsets_.end()) {
    return result->second;
  }

  /* Not found, return an zero offset */
  return 0;
}

VprBitstreamAnnotation::e_bitstream_source_type VprBitstreamAnnotation::pb_type_mode_select_bitstream_source(t_pb_type* pb_type) const {
  auto result = mode_select_bitstream_sources_.find(pb_type);
  if (result != mode_select_bitstream_sources_.end()) {
    return result->second;
  }

  /* Not found, return an invalid type*/
  return NUM_BITSTREAM_SOURCE_TYPES;
}

std::string VprBitstreamAnnotation::pb_type_mode_select_bitstream_content(t_pb_type* pb_type) const {
  auto result = mode_select_bitstream_contents_.find(pb_type);
  if (result != mode_select_bitstream_contents_.end()) {
    return result->second;
  }

  /* Not found, return an invalid type */
  return std::string();
}

size_t VprBitstreamAnnotation::pb_type_mode_select_bitstream_offset(t_pb_type* pb_type) const {
  auto result = mode_select_bitstream_offsets_.find(pb_type);
  if (result != mode_select_bitstream_offsets_.end()) {
    return result->second;
  }

  /* Not found, return an zero offset */
  return 0;
}

/************************************************************************
 * Public mutators
 ***********************************************************************/
void VprBitstreamAnnotation::set_pb_type_bitstream_source(t_pb_type* pb_type,
                                                          const e_bitstream_source_type& bitstream_source) {
  bitstream_sources_[pb_type] = bitstream_source; 
}

void VprBitstreamAnnotation::set_pb_type_bitstream_content(t_pb_type* pb_type,
                                                           const std::string& bitstream_content) {
  bitstream_contents_[pb_type] = bitstream_content; 
}

void VprBitstreamAnnotation::set_pb_type_bitstream_offset(t_pb_type* pb_type,
                                                          const size_t& offset) {
  bitstream_offsets_[pb_type] = offset; 
}

void VprBitstreamAnnotation::set_pb_type_mode_select_bitstream_source(t_pb_type* pb_type,
                                                                      const e_bitstream_source_type& bitstream_source) {
  mode_select_bitstream_sources_[pb_type] = bitstream_source; 
}

void VprBitstreamAnnotation::set_pb_type_mode_select_bitstream_content(t_pb_type* pb_type,
                                                                       const std::string& bitstream_content) {
  mode_select_bitstream_contents_[pb_type] = bitstream_content; 
}

void VprBitstreamAnnotation::set_pb_type_mode_select_bitstream_offset(t_pb_type* pb_type,
                                                                      const size_t& offset) {
  mode_select_bitstream_offsets_[pb_type] = offset; 
}

} /* End namespace openfpga*/
