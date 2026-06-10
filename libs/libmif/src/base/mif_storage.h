#ifndef MIF_STORAGE_H
#define MIF_STORAGE_H

#include <cstdint>
#include <vector>

#include "mif_storage_fwd.h"
#include "vtr_vector.h"

/********************************************************************
 * In-memory MIF data aggregated from one or more read_mif calls.
 *******************************************************************/
namespace openfpga {

class MifStorage {
 public: /* Types */
  typedef vtr::vector<MifSegmentId, MifSegmentId>::const_iterator segment_iterator;
  typedef vtr::Range<segment_iterator> segment_range;
  typedef vtr::vector<MifMemoryLineId, MifMemoryLineId>::const_iterator
    memory_line_iterator;
  typedef vtr::Range<memory_line_iterator> memory_line_range;

 public: /* Constructors */
  MifStorage() = default;

 public: /* Accessors: aggregates */
  segment_range segments() const;
  memory_line_range segment_memory_lines(const MifSegmentId& segment_id) const;

 public: /* Accessors */
  bool has_xy(const MifSegmentId& segment_id) const;
  int coord_x(const MifSegmentId& segment_id) const;
  int coord_y(const MifSegmentId& segment_id) const;
  int addr_width(const MifSegmentId& segment_id) const;
  int data_width(const MifSegmentId& segment_id) const;
  bool has_ram_id(const MifSegmentId& segment_id) const;
  int ram_id(const MifSegmentId& segment_id) const;
  int id_width(const MifSegmentId& segment_id) const;
  uint64_t memory_line_address(const MifMemoryLineId& memory_line_id) const;
  uint64_t memory_line_data(const MifMemoryLineId& memory_line_id) const;
  bool empty() const;

 public: /* Mutators */
  void clear();
  void remove_last_segment_if_empty();
  MifSegmentId create_segment();
  void set_segment_coord_x(const MifSegmentId& segment_id, int x);
  void set_segment_coord_y(const MifSegmentId& segment_id, int y);
  void set_segment_addr_width(const MifSegmentId& segment_id, int width);
  void set_segment_data_width(const MifSegmentId& segment_id, int width);
  void set_segment_ram_id(const MifSegmentId& segment_id, int ram_id);
  void set_segment_id_width(const MifSegmentId& segment_id, int width);
  void reset_segment(const MifSegmentId& segment_id);
  MifMemoryLineId create_memory_line(const MifSegmentId& segment_id,
                                     uint64_t address, uint64_t data);

 public: /* Validators */
  bool valid_segment_id(const MifSegmentId& segment_id) const;
  bool valid_memory_line_id(const MifMemoryLineId& memory_line_id) const;

 private: /* Internal data */
  vtr::vector<MifSegmentId, MifSegmentId> segment_ids_;
  vtr::vector<MifSegmentId, bool> segment_has_xy_;
  vtr::vector<MifSegmentId, int> segment_coord_x_;
  vtr::vector<MifSegmentId, int> segment_coord_y_;
  vtr::vector<MifSegmentId, int> segment_addr_width_;
  vtr::vector<MifSegmentId, int> segment_data_width_;
  vtr::vector<MifSegmentId, bool> segment_has_ram_id_;
  vtr::vector<MifSegmentId, int> segment_ram_id_;
  vtr::vector<MifSegmentId, int> segment_id_width_;
  vtr::vector<MifSegmentId, std::vector<MifMemoryLineId>>
    segment_memory_line_ids_;

  vtr::vector<MifMemoryLineId, MifMemoryLineId> memory_line_ids_;
  vtr::vector<MifMemoryLineId, uint64_t> memory_line_addresses_;
  vtr::vector<MifMemoryLineId, uint64_t> memory_line_data_;
};

} /* namespace openfpga */

#endif
