#include "mif_storage.h"

#include "vtr_assert.h"

namespace openfpga {

MifStorage::segment_range MifStorage::segments() const {
  return vtr::make_range(segment_ids_.begin(), segment_ids_.end());
}

size_t MifStorage::num_segments() const { return segment_ids_.size(); }

MifStorage::memory_line_range MifStorage::segment_memory_lines(
  const MifSegmentId& segment_id) const {
  VTR_ASSERT(valid_segment_id(segment_id));
  const std::vector<MifMemoryLineId>& line_ids =
    segment_memory_line_ids_[segment_id];
  return vtr::make_range(line_ids.begin(), line_ids.end());
}

int MifStorage::data_width(const MifSegmentId& segment_id) const {
  VTR_ASSERT(valid_segment_id(segment_id));
  return segment_data_width_[segment_id];
}

const BasicPort& MifStorage::addr_range(const MifSegmentId& segment_id) const {
  VTR_ASSERT(valid_segment_id(segment_id));
  return segment_addr_range_[segment_id];
}

const std::string& MifStorage::physical_pb(
  const MifSegmentId& segment_id) const {
  VTR_ASSERT(valid_segment_id(segment_id));
  return segment_physical_pb_[segment_id];
}

uint64_t MifStorage::memory_line_address(
  const MifMemoryLineId& memory_line_id) const {
  VTR_ASSERT(valid_memory_line_id(memory_line_id));
  return memory_line_addresses_[memory_line_id];
}

uint64_t MifStorage::memory_line_data(
  const MifMemoryLineId& memory_line_id) const {
  VTR_ASSERT(valid_memory_line_id(memory_line_id));
  return memory_line_data_[memory_line_id];
}

bool MifStorage::empty() const { return segment_ids_.empty(); }

void MifStorage::remove_last_segment_if_empty() {
  if (segment_ids_.empty()) {
    return;
  }
  const MifSegmentId last_segment_id = segment_ids_.back();
  if (!segment_memory_line_ids_[last_segment_id].empty()) {
    return;
  }
  segment_ids_.pop_back();
  segment_data_width_.pop_back();
  segment_addr_range_.pop_back();
  segment_physical_pb_.pop_back();
  segment_memory_line_ids_.pop_back();
}

void MifStorage::clear() {
  segment_ids_.clear();
  segment_data_width_.clear();
  segment_addr_range_.clear();
  segment_physical_pb_.clear();
  segment_memory_line_ids_.clear();
  memory_line_ids_.clear();
  memory_line_addresses_.clear();
  memory_line_data_.clear();
}

MifSegmentId MifStorage::create_segment() {
  MifSegmentId segment_id(segment_ids_.size());
  segment_ids_.push_back(segment_id);
  segment_data_width_.push_back(-1);
  segment_addr_range_.emplace_back();
  segment_physical_pb_.push_back(std::string());
  segment_memory_line_ids_.emplace_back();
  return segment_id;
}

void MifStorage::set_segment_data_width(const MifSegmentId& segment_id,
                                        int width) {
  VTR_ASSERT(valid_segment_id(segment_id));
  segment_data_width_[segment_id] = width;
}

void MifStorage::set_segment_addr_range(const MifSegmentId& segment_id,
                                        const BasicPort& addr_range) {
  VTR_ASSERT(valid_segment_id(segment_id));
  VTR_ASSERT(addr_range.is_valid());
  segment_addr_range_[segment_id] = addr_range;
}

void MifStorage::set_segment_physical_pb(const MifSegmentId& segment_id,
                                         const std::string& physical_pb) {
  VTR_ASSERT(valid_segment_id(segment_id));
  segment_physical_pb_[segment_id] = physical_pb;
}

MifMemoryLineId MifStorage::create_memory_line(const MifSegmentId& segment_id,
                                               uint64_t address,
                                               uint64_t data) {
  VTR_ASSERT(valid_segment_id(segment_id));
  MifMemoryLineId memory_line_id(memory_line_ids_.size());
  memory_line_ids_.push_back(memory_line_id);
  memory_line_addresses_.push_back(address);
  memory_line_data_.push_back(data);
  segment_memory_line_ids_[segment_id].push_back(memory_line_id);
  return memory_line_id;
}

bool MifStorage::valid_segment_id(const MifSegmentId& segment_id) const {
  return (size_t(segment_id) < segment_ids_.size()) &&
         (segment_id == segment_ids_[segment_id]);
}

bool MifStorage::valid_memory_line_id(
  const MifMemoryLineId& memory_line_id) const {
  return (size_t(memory_line_id) < memory_line_ids_.size()) &&
         (memory_line_id == memory_line_ids_[memory_line_id]);
}

} /* namespace openfpga */
