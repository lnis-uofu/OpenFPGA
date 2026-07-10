/********************************************************************
 * Parse memory_address_map.xml into MemoryAddressMap.
 *
 * Expected format:
 *   <memory_address_map>
 *     <memory x="2" y="1" id="0" addr_width="3" data_width="16"/>
 *   </memory_address_map>
 *******************************************************************/
#include <string>

#include "pugixml.hpp"
#include "pugixml_util.hpp"

#include "arch_error.h"
#include "command_exit_codes.h"
#include "read_xml_memory_address_map.h"
#include "read_xml_util.h"
#include "vtr_log.h"
#include "vtr_time.h"

namespace openfpga {

static void read_xml_one_memory(pugi::xml_node& xml_memory,
                                const pugiutil::loc_data& loc_data,
                                MemoryAddressMap& memory_address_map) {
  const int x =
    get_attribute(xml_memory, XML_MEMORY_ADDRESS_MAP_ATTRIBUTE_X, loc_data)
      .as_int();
  const int y =
    get_attribute(xml_memory, XML_MEMORY_ADDRESS_MAP_ATTRIBUTE_Y, loc_data)
      .as_int();
  const int ram_id =
    get_attribute(xml_memory, XML_MEMORY_ADDRESS_MAP_ATTRIBUTE_ID, loc_data)
      .as_int();
  const int addr_width = get_attribute(
                           xml_memory, XML_MEMORY_ADDRESS_MAP_ATTRIBUTE_ADDR_WIDTH,
                           loc_data)
                           .as_int();
  const int data_width = get_attribute(
                           xml_memory, XML_MEMORY_ADDRESS_MAP_ATTRIBUTE_DATA_WIDTH,
                           loc_data)
                           .as_int();

  if (x < 0 || y < 0) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_memory),
                   "Invalid coordinate (x, y) = (%d, %d)! Expect zero or a "
                   "positive integer!\n",
                   x, y);
  }
  if (ram_id < 0) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_memory),
                   "Invalid id = %d! Expect zero or a positive integer!\n",
                   ram_id);
  }
  if (addr_width <= 0 || data_width <= 0) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_memory),
                   "Invalid addr_width/data_width = (%d, %d)! Expect positive "
                   "integers!\n",
                   addr_width, data_width);
  }

  memory_address_map.create_memory(x, y, ram_id, addr_width, data_width);
}

int read_xml_memory_address_map(const std::string& file_path,
                                MemoryAddressMap& memory_address_map) {
  vtr::ScopedStartFinishTimer timer("Read Memory Address Map");

  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, file_path.c_str());

    pugi::xml_node xml_root =
      get_single_child(doc, XML_MEMORY_ADDRESS_MAP_ROOT_NAME, loc_data);

    for (pugi::xml_node xml_memory : xml_root.children()) {
      if (xml_memory.name() != std::string(XML_MEMORY_ADDRESS_MAP_MEMORY_NAME)) {
        bad_tag(xml_memory, loc_data, xml_root,
                {XML_MEMORY_ADDRESS_MAP_MEMORY_NAME});
      }
      read_xml_one_memory(xml_memory, loc_data, memory_address_map);
    }
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(file_path.c_str(), e.line(), "%s", e.what());
  }

  if (memory_address_map.empty()) {
    VTR_LOG_ERROR("Memory address map parse: no <%s> entries found in %s\n",
                  XML_MEMORY_ADDRESS_MAP_MEMORY_NAME, file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
