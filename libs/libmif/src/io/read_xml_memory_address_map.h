#pragma once

/********************************************************************
 * Read a memory_address_map.xml into MemoryAddressMap.
 *******************************************************************/
#include <string>

#include "memory_address_map.h"

namespace openfpga {
/* Constants required by memory_address_map XML parser */
constexpr const char* XML_MEMORY_ADDRESS_MAP_ROOT_NAME = "memory_address_map";
constexpr const char* XML_MEMORY_ADDRESS_MAP_MEMORY_NAME = "memory";
constexpr const char* XML_MEMORY_ADDRESS_MAP_ATTRIBUTE_X = "x";
constexpr const char* XML_MEMORY_ADDRESS_MAP_ATTRIBUTE_Y = "y";
constexpr const char* XML_MEMORY_ADDRESS_MAP_ATTRIBUTE_ID = "id";
constexpr const char* XML_MEMORY_ADDRESS_MAP_ATTRIBUTE_ID_WIDTH = "id_width";
constexpr const char* XML_MEMORY_ADDRESS_MAP_ATTRIBUTE_ADDR_WIDTH = "addr_width";
constexpr const char* XML_MEMORY_ADDRESS_MAP_ATTRIBUTE_DATA_WIDTH = "data_width";

int read_xml_memory_address_map(const std::string& file_path,
                                MemoryAddressMap& memory_address_map);

} /* namespace openfpga */
