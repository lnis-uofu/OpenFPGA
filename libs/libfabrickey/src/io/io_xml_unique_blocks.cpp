#include <string>
/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "io_xml_unique_blocks.h"
#include "openfpga_digest.h"
#include "read_xml_util.h"
#include "write_xml_utils.h"

namespace openfpga {
vtr::Point<size_t> read_xml_unique_instance_info(
  pugi::xml_node& xml_instance_info, const pugiutil::loc_data& loc_data) {
  int instance_x = get_attribute(xml_instance_info, "x", loc_data).as_int();
  int instance_y = get_attribute(xml_instance_info, "y", loc_data).as_int();
  vtr::Point<size_t> instance_coordinate(instance_x, instance_y);
  return instance_coordinate;
}

int write_xml_block(
  std::map<int, vtr::Point<size_t>>& id_unique_block_map,
  std::map<int, std::vector<vtr::Point<size_t>>>& id_instance_map,
  std::fstream& fp, std::string type) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  for (const auto& pair : id_unique_block_map) {
    openfpga::write_tab_to_file(fp, 1);
    fp << "<block";
    write_xml_attribute(fp, "type", type.c_str());
    write_xml_attribute(fp, "x", pair.second.x());
    write_xml_attribute(fp, "y", pair.second.y());

    fp << ">"
       << "\n";

    for (const auto& instance_info : id_instance_map[pair.first]) {
      if (instance_info.x() == pair.second.x() &&
          instance_info.y() == pair.second.y()) {
        ;
      } else {
        openfpga::write_tab_to_file(fp, 2);
        fp << "<instance";
        write_xml_attribute(fp, "x", instance_info.x());
        write_xml_attribute(fp, "y", instance_info.y());

        fp << "/>"
           << "\n";
      }
    }
    openfpga::write_tab_to_file(fp, 1);
    fp << "</block>"
       << "\n";
  }

  return CMD_EXEC_SUCCESS;
}
}  // namespace openfpga
