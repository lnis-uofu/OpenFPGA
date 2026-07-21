#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "mif_storage_fwd.h"
#include "vtr_vector.h"

/********************************************************************
 * In-memory logical MIF data read from init.hex via read_mif.
 *******************************************************************/
namespace openfpga {

class MifStorage {
 public: /* Types */
  typedef vtr::vector<MifSegmentId, MifSegmentId>::const_iterator
    segment_iterator;
  typedef vtr::Range<segment_iterator> segment_range;
  typedef vtr::vector<MifMemoryLineId, MifMemoryLineId>::const_iterator
    memory_line_iterator;
  typedef vtr::Range<memory_line_iterator> memory_line_range;

 public: /* Constructors */
  MifStorage() = default;

 public: /* Accessors: aggregates */
  segment_range segments() const;
  memory_line_range segment_memory_lines(const MifSegmentId& segment_id) const;
  size_t num_segments() const;

 public: /* Accessors */
  int addr_width(const MifSegmentId& segment_id) const;
  int data_width(const MifSegmentId& segment_id) const;
  bool has_addr_range(const MifSegmentId& segment_id) const;
  uint64_t min_addr(const MifSegmentId& segment_id) const;
  uint64_t max_addr(const MifSegmentId& segment_id) const;
  bool has_source_file(const MifSegmentId& segment_id) const;
  const std::string& segment_source_file(const MifSegmentId& segment_id) const;
  const std::string& physical_pb(const MifSegmentId& segment_id) const;
  uint64_t memory_line_address(const MifMemoryLineId& memory_line_id) const;
  uint64_t memory_line_data(const MifMemoryLineId& memory_line_id) const;
  bool empty() const;

 public: /* Mutators */
  void clear();
  void remove_last_segment_if_empty();
  MifSegmentId create_segment();
  void set_segment_addr_width(const MifSegmentId& segment_id, int width);
  void set_segment_data_width(const MifSegmentId& segment_id, int width);
  void set_segment_addr_range(const MifSegmentId& segment_id, uint64_t min_addr,
                              uint64_t max_addr);
  void set_segment_source_file(const MifSegmentId& segment_id,
                               const std::string& file_path);
  void set_segment_physical_pb(const MifSegmentId& segment_id,
                               const std::string& physical_pb);
  MifMemoryLineId create_memory_line(const MifSegmentId& segment_id,
                                     uint64_t address, uint64_t data);

 public: /* Validators */
  bool valid_segment_id(const MifSegmentId& segment_id) const;
  bool valid_memory_line_id(const MifMemoryLineId& memory_line_id) const;

 private: /* Internal data */
  vtr::vector<MifSegmentId, MifSegmentId> segment_ids_;
  vtr::vector<MifSegmentId, int> segment_addr_width_;
  vtr::vector<MifSegmentId, int> segment_data_width_;
  vtr::vector<MifSegmentId, bool> segment_has_addr_range_;
  vtr::vector<MifSegmentId, uint64_t> segment_min_addr_;
  vtr::vector<MifSegmentId, uint64_t> segment_max_addr_;
  vtr::vector<MifSegmentId, std::string> segment_source_file_;
  vtr::vector<MifSegmentId, std::string> segment_physical_pb_;
  vtr::vector<MifSegmentId, std::vector<MifMemoryLineId>>
    segment_memory_line_ids_;

  vtr::vector<MifMemoryLineId, MifMemoryLineId> memory_line_ids_;
  vtr::vector<MifMemoryLineId, uint64_t> memory_line_addresses_;
  vtr::vector<MifMemoryLineId, uint64_t> memory_line_data_;
};

} /* namespace openfpga */
