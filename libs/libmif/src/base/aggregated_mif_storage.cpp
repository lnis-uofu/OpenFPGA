#include "aggregated_mif_storage.h"

#include "vtr_assert.h"

namespace openfpga {

bool AggregatedMifStorage::empty() const { return storage_.empty(); }

void AggregatedMifStorage::clear() { storage_.clear(); }

const MifStorage& AggregatedMifStorage::storage() const { return storage_; }

AggregatedMifStorage::segment_range AggregatedMifStorage::segments() const {
  return storage_.segments();
}

size_t AggregatedMifStorage::num_segments() const {
  return storage_.num_segments();
}

AggregatedMifStorage::memory_line_range
AggregatedMifStorage::segment_memory_lines(
  const MifSegmentId& segment_id) const {
  return storage_.segment_memory_lines(segment_id);
}

int AggregatedMifStorage::addr_width(const MifSegmentId& segment_id) const {
  return storage_.addr_width(segment_id);
}

int AggregatedMifStorage::data_width(const MifSegmentId& segment_id) const {
  return storage_.data_width(segment_id);
}

const std::string& AggregatedMifStorage::physical_pb(
  const MifSegmentId& segment_id) const {
  return storage_.physical_pb(segment_id);
}

uint64_t AggregatedMifStorage::memory_line_address(
  const MifMemoryLineId& memory_line_id) const {
  return storage_.memory_line_address(memory_line_id);
}

uint64_t AggregatedMifStorage::memory_line_data(
  const MifMemoryLineId& memory_line_id) const {
  return storage_.memory_line_data(memory_line_id);
}

MifSegmentId AggregatedMifStorage::create_segment() {
  return storage_.create_segment();
}

void AggregatedMifStorage::set_segment_addr_width(
  const MifSegmentId& segment_id, int width) {
  storage_.set_segment_addr_width(segment_id, width);
}

void AggregatedMifStorage::set_segment_data_width(
  const MifSegmentId& segment_id, int width) {
  storage_.set_segment_data_width(segment_id, width);
}

void AggregatedMifStorage::set_segment_physical_pb(
  const MifSegmentId& segment_id, const std::string& physical_pb) {
  storage_.set_segment_physical_pb(segment_id, physical_pb);
}

MifMemoryLineId AggregatedMifStorage::create_memory_line(
  const MifSegmentId& segment_id, uint64_t address, uint64_t data) {
  return storage_.create_memory_line(segment_id, address, data);
}

} /* namespace openfpga */
