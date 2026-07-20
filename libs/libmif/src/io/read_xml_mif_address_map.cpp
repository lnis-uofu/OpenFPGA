/********************************************************************
 * Parse memory_address_map.xml into MifAddressMap.
 *
 * Expected format:
 *   <memory_address_map>
 *     <address_map pb_type="dpram8x32[0]{dual}.dpram8x16[0]"
 *                  address_offset="0" data_offset="0"/>
 *   </memory_address_map>
 *******************************************************************/
#include <string>

#include "pugixml.hpp"
#include "pugixml_util.hpp"

#include "arch_error.h"
#include "command_exit_codes.h"
#include "read_xml_mif_address_map.h"
#include "read_xml_util.h"
#include "vtr_log.h"
#include "vtr_time.h"

namespace openfpga {

static void read_xml_one_address_map(pugi::xml_node& xml_entry,
                                     const pugiutil::loc_data& loc_data,
                                     MifAddressMap& mif_address_map) {
  const std::string pb_type =
    get_attribute(xml_entry, XML_MIF_ADDRESS_MAP_ATTRIBUTE_PB_TYPE, loc_data)
      .as_string();
  const int address_offset =
    get_attribute(xml_entry, XML_MIF_ADDRESS_MAP_ATTRIBUTE_ADDRESS_OFFSET,
                  loc_data)
      .as_int();
  const int data_offset =
    get_attribute(xml_entry, XML_MIF_ADDRESS_MAP_ATTRIBUTE_DATA_OFFSET,
                  loc_data)
      .as_int();

  if (pb_type.empty()) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_entry),
                   "Invalid empty pb_type!\n");
  }
  if (address_offset < 0 || data_offset < 0) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_entry),
                   "Invalid address_offset/data_offset = (%d, %d)! Expect "
                   "zero or a positive integer!\n",
                   address_offset, data_offset);
  }
  if (mif_address_map.find_by_pb_type(pb_type).is_valid()) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_entry),
                   "Duplicated pb_type '%s' in memory address map!\n",
                   pb_type.c_str());
  }

  mif_address_map.create_address_map(pb_type, address_offset, data_offset);
}

int read_xml_mif_address_map(const std::string& file_path,
                             MifAddressMap& mif_address_map) {
  vtr::ScopedStartFinishTimer timer("Read MIF Address Map");

  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, file_path.c_str());

    pugi::xml_node xml_root =
      get_single_child(doc, XML_MIF_ADDRESS_MAP_ROOT_NAME, loc_data);

    for (pugi::xml_node xml_entry : xml_root.children()) {
      if (xml_entry.name() != std::string(XML_MIF_ADDRESS_MAP_ENTRY_NAME)) {
        bad_tag(xml_entry, loc_data, xml_root,
                {XML_MIF_ADDRESS_MAP_ENTRY_NAME});
      }
      read_xml_one_address_map(xml_entry, loc_data, mif_address_map);
    }
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(file_path.c_str(), e.line(), "%s", e.what());
  }

  if (mif_address_map.empty()) {
    VTR_LOG_ERROR("MIF address map parse: no <%s> entries found in %s\n",
                  XML_MIF_ADDRESS_MAP_ENTRY_NAME, file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
