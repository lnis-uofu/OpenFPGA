#pragma once

/********************************************************************
 * Read a memory_address_map.xml into MifAddressMap.
 *******************************************************************/
#include <string>

#include "mif_address_map.h"

namespace openfpga {
/* Constants required by memory_address_map XML parser */
constexpr const char* XML_MIF_ADDRESS_MAP_ROOT_NAME = "memory_address_map";
constexpr const char* XML_MIF_ADDRESS_MAP_ENTRY_NAME = "address_map";
constexpr const char* XML_MIF_ADDRESS_MAP_ATTRIBUTE_PB_TYPE = "pb_type";
constexpr const char* XML_MIF_ADDRESS_MAP_ATTRIBUTE_ADDRESS_OFFSET =
  "address_offset";
constexpr const char* XML_MIF_ADDRESS_MAP_ATTRIBUTE_DATA_OFFSET = "data_offset";

int read_xml_mif_address_map(const std::string& file_path,
                             MifAddressMap& mif_address_map);

} /* namespace openfpga */
