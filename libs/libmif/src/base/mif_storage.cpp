#include "mif_storage.h"

#include "vtr_assert.h"

namespace openfpga {

MifStorage::segment_range MifStorage::segments() const {
  return vtr::make_range(segment_ids_.begin(), segment_ids_.end());
}

MifStorage::memory_line_range MifStorage::segment_memory_lines(
  const MifSegmentId& segment_id) const {
  VTR_ASSERT(valid_segment_id(segment_id));
  const std::vector<MifMemoryLineId>& line_ids =
    segment_memory_line_ids_[segment_id];
  return vtr::make_range(line_ids.begin(), line_ids.end());
}

bool MifStorage::has_xy(const MifSegmentId& segment_id) const {
  VTR_ASSERT(valid_segment_id(segment_id));
  return segment_has_xy_[segment_id];
}

int MifStorage::coord_x(const MifSegmentId& segment_id) const {
  VTR_ASSERT(valid_segment_id(segment_id));
  return segment_coord_x_[segment_id];
}

int MifStorage::coord_y(const MifSegmentId& segment_id) const {
  VTR_ASSERT(valid_segment_id(segment_id));
  return segment_coord_y_[segment_id];
}

int MifStorage::addr_width(const MifSegmentId& segment_id) const {
  VTR_ASSERT(valid_segment_id(segment_id));
  return segment_addr_width_[segment_id];
}

int MifStorage::data_width(const MifSegmentId& segment_id) const {
  VTR_ASSERT(valid_segment_id(segment_id));
  return segment_data_width_[segment_id];
}

bool MifStorage::has_ram_id(const MifSegmentId& segment_id) const {
  VTR_ASSERT(valid_segment_id(segment_id));
  return segment_has_ram_id_[segment_id];
}

int MifStorage::ram_id(const MifSegmentId& segment_id) const {
  VTR_ASSERT(valid_segment_id(segment_id));
  return segment_ram_id_[segment_id];
}

int MifStorage::id_width(const MifSegmentId& segment_id) const {
  VTR_ASSERT(valid_segment_id(segment_id));
  return segment_id_width_[segment_id];
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

const std::vector<std::string>& MifStorage::source_files() const {
  return source_files_;
}

void MifStorage::add_source_file(const std::string& file_path) {
  source_files_.push_back(file_path);
}

void MifStorage::remove_last_segment_if_empty() {
  if (segment_ids_.empty()) {
    return;
  }
  const MifSegmentId last_segment_id = segment_ids_.back();
  if (!segment_memory_line_ids_[last_segment_id].empty() ||
      segment_has_xy_[last_segment_id] ||
      segment_has_ram_id_[last_segment_id]) {
    return;
  }
  segment_ids_.pop_back();
  segment_has_xy_.pop_back();
  segment_coord_x_.pop_back();
  segment_coord_y_.pop_back();
  segment_addr_width_.pop_back();
  segment_data_width_.pop_back();
  segment_has_ram_id_.pop_back();
  segment_ram_id_.pop_back();
  segment_id_width_.pop_back();
  segment_memory_line_ids_.pop_back();
}

void MifStorage::clear() {
  segment_ids_.clear();
  segment_has_xy_.clear();
  segment_coord_x_.clear();
  segment_coord_y_.clear();
  segment_addr_width_.clear();
  segment_data_width_.clear();
  segment_has_ram_id_.clear();
  segment_ram_id_.clear();
  segment_id_width_.clear();
  segment_memory_line_ids_.clear();
  memory_line_ids_.clear();
  memory_line_addresses_.clear();
  memory_line_data_.clear();
  source_files_.clear();
}

MifSegmentId MifStorage::create_segment() {
  MifSegmentId segment_id(segment_ids_.size());
  segment_ids_.push_back(segment_id);
  segment_has_xy_.push_back(false);
  segment_coord_x_.push_back(-1);
  segment_coord_y_.push_back(-1);
  segment_addr_width_.push_back(-1);
  segment_data_width_.push_back(-1);
  segment_has_ram_id_.push_back(false);
  segment_ram_id_.push_back(-1);
  segment_id_width_.push_back(-1);
  segment_memory_line_ids_.emplace_back();
  return segment_id;
}

void MifStorage::set_segment_coord_x(const MifSegmentId& segment_id, int x) {
  VTR_ASSERT(valid_segment_id(segment_id));
  segment_has_xy_[segment_id] = true;
  segment_coord_x_[segment_id] = x;
}

void MifStorage::set_segment_coord_y(const MifSegmentId& segment_id, int y) {
  VTR_ASSERT(valid_segment_id(segment_id));
  segment_has_xy_[segment_id] = true;
  segment_coord_y_[segment_id] = y;
}

void MifStorage::set_segment_addr_width(const MifSegmentId& segment_id,
                                        int width) {
  VTR_ASSERT(valid_segment_id(segment_id));
  segment_addr_width_[segment_id] = width;
}

void MifStorage::set_segment_data_width(const MifSegmentId& segment_id,
                                        int width) {
  VTR_ASSERT(valid_segment_id(segment_id));
  segment_data_width_[segment_id] = width;
}

void MifStorage::set_segment_ram_id(const MifSegmentId& segment_id,
                                    int ram_id) {
  VTR_ASSERT(valid_segment_id(segment_id));
  segment_has_ram_id_[segment_id] = true;
  segment_ram_id_[segment_id] = ram_id;
}

void MifStorage::set_segment_id_width(const MifSegmentId& segment_id,
                                      int width) {
  VTR_ASSERT(valid_segment_id(segment_id));
  segment_id_width_[segment_id] = width;
}

void MifStorage::reset_segment(const MifSegmentId& segment_id) {
  VTR_ASSERT(valid_segment_id(segment_id));
  segment_has_xy_[segment_id] = false;
  segment_coord_x_[segment_id] = -1;
  segment_coord_y_[segment_id] = -1;
  segment_addr_width_[segment_id] = -1;
  segment_data_width_[segment_id] = -1;
  segment_has_ram_id_[segment_id] = false;
  segment_ram_id_[segment_id] = -1;
  segment_id_width_[segment_id] = -1;
  segment_memory_line_ids_[segment_id].clear();
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
