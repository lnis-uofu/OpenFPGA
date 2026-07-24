#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "mif_storage_fwd.h"
#include "openfpga_port.h"
#include "vtr_vector.h"

/********************************************************************
 * In-memory MIF storage used for both:
 *   - logical init.hex data from read_mif()
 *   - aggregated preload data from aggregate_mif()
 * Distinguish stages by variable naming / comments at call sites.
 *
 * Logical segment typically has:
 *   - memory lines (required)
 *   - optional addr_range from init.hex depth comment
 *
 * Aggregated segment typically has:
 *   - memory lines
 *   - physical_pb, data_width, addr_range (for .mem header)
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
  int data_width(const MifSegmentId& segment_id) const;
  /* Invalid BasicPort when no address range was set. */
  const BasicPort& addr_range(const MifSegmentId& segment_id) const;
  const std::string& physical_pb(const MifSegmentId& segment_id) const;
  uint64_t memory_line_address(const MifMemoryLineId& memory_line_id) const;
  uint64_t memory_line_data(const MifMemoryLineId& memory_line_id) const;
  bool empty() const;

 public: /* Mutators */
  void clear();
  void remove_last_segment_if_empty();
  MifSegmentId create_segment();
  void set_segment_data_width(const MifSegmentId& segment_id, int width);
  void set_segment_addr_range(const MifSegmentId& segment_id,
                              const BasicPort& addr_range);
  void set_segment_physical_pb(const MifSegmentId& segment_id,
                               const std::string& physical_pb);
  MifMemoryLineId create_memory_line(const MifSegmentId& segment_id,
                                     uint64_t address, uint64_t data);

 public: /* Validators */
  bool valid_segment_id(const MifSegmentId& segment_id) const;
  bool valid_memory_line_id(const MifMemoryLineId& memory_line_id) const;

 private: /* Internal data */
  vtr::vector<MifSegmentId, MifSegmentId> segment_ids_;
  vtr::vector<MifSegmentId, int> segment_data_width_;
  vtr::vector<MifSegmentId, BasicPort> segment_addr_range_;
  vtr::vector<MifSegmentId, std::string> segment_physical_pb_;
  vtr::vector<MifSegmentId, std::vector<MifMemoryLineId>>
    segment_memory_line_ids_;

  vtr::vector<MifMemoryLineId, MifMemoryLineId> memory_line_ids_;
  vtr::vector<MifMemoryLineId, uint64_t> memory_line_addresses_;
  vtr::vector<MifMemoryLineId, uint64_t> memory_line_data_;
};

} /* namespace openfpga */
