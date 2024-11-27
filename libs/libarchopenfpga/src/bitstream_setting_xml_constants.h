#ifndef BITSTREAM_SETTING_XML_CONSTANTS_H
#define BITSTREAM_SETTING_XML_CONSTANTS_H

/* Constants required by XML parser */

constexpr const char* XML_BITSTREAM_SETTING_ROOT_NAME =
  "openfpga_bitstream_setting";
/* Pb-type XML syntax */
constexpr const char* XML_PB_TYPE_NODE_NAME = "pb_type";
constexpr const char* XML_PB_TYPE_ATTRIBUTE_NAME = "name";
constexpr const char* XML_PB_TYPE_ATTRIBUTE_SOURCE = "source";
constexpr const char* XML_PB_TYPE_ATTRIBUTE_CONTENT = "content";
constexpr const char* XML_PB_TYPE_ATTRIBUTE_IS_MODE_SELECT_BITSTREAM =
  "is_mode_select_bitstream";
constexpr const char* XML_PB_TYPE_ATTRIBUTE_BITSTREAM_OFFSET =
  "bitstream_offset";

/* Default mode bits XML syntax */
constexpr const char* XML_DEFAULT_MODE_BITS_NODE_NAME = "default_mode_bits";
constexpr const char* XML_DEFAULT_MODE_BITS_ATTRIBUTE_NAME = "name";
constexpr const char* XML_DEFAULT_MODE_BITS_ATTRIBUTE_MODE_BITS = "mode_bits";

/* Clock routing XML syntax */
constexpr const char* XML_CLOCK_ROUTING_NODE_NAME = "clock_routing";
constexpr const char* XML_CLOCK_ROUTING_ATTRIBUTE_NETWORK = "network";
constexpr const char* XML_CLOCK_ROUTING_ATTRIBUTE_PIN = "pin";

/* Interconnect XML syntax */
constexpr const char* XML_INTERCONNECT_NODE_NAME = "interconnect";
constexpr const char* XML_INTERCONNECT_ATTRIBUTE_NAME = "name";
constexpr const char* XML_INTERCONNECT_ATTRIBUTE_DEFAULT_PATH = "default_path";

/* Non fabric XML syntax */
constexpr const char* XML_NON_FABRIC_NODE_NAME = "non_fabric";
constexpr const char* XML_NON_FABRIC_ATTRIBUTE_NAME = "name";
constexpr const char* XML_NON_FABRIC_ATTRIBUTE_FILE = "file";
constexpr const char* XML_NON_FABRIC_PB_NODE_NAME = "pb";
constexpr const char* XML_NON_FABRIC_PB_ATTRIBUTE_NAME = "name";
constexpr const char* XML_NON_FABRIC_PB_ATTRIBUTE_CONTENT = "content";

/* Overwrite bitstream XML syntax */
constexpr const char* XML_OVERWRITE_BITSTREAM_NODE_NAME = "overwrite_bitstream";
constexpr const char* XML_OVERWRITE_BITSTREAM_ATTRIBUTE_BIT = "bit";
constexpr const char* XML_OVERWRITE_BITSTREAM_ATTRIBUTE_PATH = "path";
constexpr const char* XML_OVERWRITE_BITSTREAM_ATTRIBUTE_VALUE = "value";

/* Sanity check constants */
constexpr const char* XML_VALID_NODE_NAMES[] = {
  XML_PB_TYPE_NODE_NAME,      XML_DEFAULT_MODE_BITS_NODE_NAME,
  XML_INTERCONNECT_NODE_NAME, XML_CLOCK_ROUTING_NODE_NAME,
  XML_NON_FABRIC_NODE_NAME,   XML_OVERWRITE_BITSTREAM_NODE_NAME};

#endif
