#pragma once

#include "mif_storage.h"

namespace openfpga {

/********************************************************************
 * Aggregated preload MIF produced from logical init.hex segments.
 * Populated by aggregate_mif(); consumed by write_mif().
 *******************************************************************/
class AggregatedMifStorage {
 public:
  using segment_range = MifStorage::segment_range;
  using memory_line_range = MifStorage::memory_line_range;

  bool empty() const;
  void clear();

  const MifStorage& storage() const;

  MifStorage::segment_range segments() const;
  MifStorage::memory_line_range segment_memory_lines(
    const MifSegmentId& segment_id) const;
  size_t num_segments() const;
  int addr_width(const MifSegmentId& segment_id) const;
  int data_width(const MifSegmentId& segment_id) const;
  const std::string& physical_pb(const MifSegmentId& segment_id) const;
  uint64_t memory_line_address(const MifMemoryLineId& memory_line_id) const;
  uint64_t memory_line_data(const MifMemoryLineId& memory_line_id) const;

  MifSegmentId create_segment();
  void set_segment_addr_width(const MifSegmentId& segment_id, int width);
  void set_segment_data_width(const MifSegmentId& segment_id, int width);
  void set_segment_physical_pb(const MifSegmentId& segment_id,
                               const std::string& physical_pb);
  MifMemoryLineId create_memory_line(const MifSegmentId& segment_id,
                                     uint64_t address, uint64_t data);

 private:
  MifStorage storage_;
};

} /* namespace openfpga */
