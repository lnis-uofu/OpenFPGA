#ifndef IO_NAME_MAP_XML_CONSTANTS_H
#define IO_NAME_MAP_XML_CONSTANTS_H

/* Constants required by XML parser */

constexpr const char* XML_IO_NAME_MAP_ROOT_NAME = "ports";
constexpr const char* XML_IO_NAME_MAP_NODE_NAME = "port";
constexpr const char* XML_IO_NAME_MAP_ATTRIBUTE_TOP_NAME = "top_name";
constexpr const char* XML_IO_NAME_MAP_ATTRIBUTE_CORE_NAME = "core_name";
constexpr const char* XML_IO_NAME_MAP_ATTRIBUTE_IS_DUMMY = "is_dummy";
constexpr const char* XML_IO_NAME_MAP_ATTRIBUTE_DIRECTION = "direction";

constexpr std::array<const char*, 3>
  XML_IO_NAME_MAP_DUMMY_PORT_DIRECTION_STRING = {
    {"input", "output", "inout"}};  // String versions of side orientations

#endif
