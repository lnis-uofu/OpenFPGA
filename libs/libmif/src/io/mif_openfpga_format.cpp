#include "mif_openfpga_format.h"

#include <cstdint>
#include <iomanip>
#include <ostream>

namespace openfpga {

static int hex_digits_for_width(int width_bits) {
  if (width_bits <= 0) {
    return 0;
  }
  return static_cast<int>((static_cast<unsigned>(width_bits) + 3u) / 4u);
}

static void print_hex_with_width(std::ostream& os, uint64_t v, int width_bits) {
  int nd = hex_digits_for_width(width_bits);
  if (nd <= 0) {
    os << "0x" << std::hex << v;
  } else {
    os << "0x" << std::hex << std::setw(nd) << std::setfill('0') << v;
  }
  os << std::dec << std::setfill(' ');
}

void serialize_openfpga_mif(const MifStorage& storage, std::ostream& os) {
  os << "# This is a comment\n";
  os << "# All the address and data in HEX format\n";

  bool first = true;
  for (const MifSegmentId& segment_id : storage.segments()) {
    if (!first) {
      os << "\n";
    }
    first = false;

    if (storage.has_xy(segment_id)) {
      os << "// X " << storage.coord_x(segment_id)
         << " # Coordinates of the RAM block\n";
      os << "// Y " << storage.coord_y(segment_id)
         << " # Coordinates of the RAM block\n";
    }
    if (!storage.has_ram_id(segment_id)) {
      if (storage.addr_width(segment_id) >= 0) {
        os << "// ADDR_WIDTH " << storage.addr_width(segment_id) << "\n";
      }
      if (storage.data_width(segment_id) >= 0) {
        os << "// DATA_WIDTH " << storage.data_width(segment_id) << "\n";
      }
    } else {
      os << "//RAM_ID " << storage.ram_id(segment_id) << "\n";
      if (storage.id_width(segment_id) >= 0) {
        os << "//ID_WIDTH " << storage.id_width(segment_id) << "\n";
      }
      if (storage.addr_width(segment_id) >= 0) {
        os << "// ADDR_WIDTH " << storage.addr_width(segment_id) << "\n";
      }
      if (storage.data_width(segment_id) >= 0) {
        os << "// DATA_WIDTH " << storage.data_width(segment_id) << "\n";
      }
    }
    os << "# Address Data \n";
    for (const MifMemoryLineId& memory_line_id :
         storage.segment_memory_lines(segment_id)) {
      print_hex_with_width(os, storage.memory_line_address(memory_line_id),
                           storage.addr_width(segment_id));
      os << " ";
      print_hex_with_width(os, storage.memory_line_data(memory_line_id),
                           storage.data_width(segment_id));
      os << "\n";
    }
  }
}

} /* namespace openfpga */
