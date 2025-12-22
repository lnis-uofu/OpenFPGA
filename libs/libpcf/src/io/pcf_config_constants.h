#pragma once

/* Constants required by XML parser */

constexpr const char* XML_PCF_CONFIG_ROOT_NAME = "pcf_config";
/* Pb-type XML syntax */
constexpr const char* XML_COMMAND_TYPE_NODE_NAME = "command";
constexpr const char* XML_COMMAND_TYPE_ATTRIBUTE_NAME = "name";
constexpr const char* XML_COMMAND_TYPE_ATTRIBUTE_TYPE = "type";

constexpr const char* XML_PB_TYPE_NODE_NAME = "pb_type";
constexpr const char* XML_PB_TYPE_ATTRIBUTE_NAME = "name";
constexpr const char* XML_PB_TYPE_ATTRIBUTE_OFFSET = "offset";

constexpr const char* XML_OPTION_TYPE_NODE_NAME = "option";
constexpr const char* XML_OPTION_ATTRIBUTE_NAME = "name";
constexpr const char* XML_OPTION_ATTRIBUTE_TYPE = "type";
constexpr const char* XML_OPTION_ATTRIBUTE_OFFSET = "offset";

constexpr const char* XML_MODE_TYPE_NODE_NAME = "mode";
constexpr const char* XML_MODE_ATTRIBUTE_NAME = "name";
constexpr const char* XML_MODE_ATTRIBUTE_VALUE = "value";

/* Sanity check constants */
constexpr const char* XML_VALID_NODE_NAMES[] = {
  XML_COMMAND_TYPE_NODE_NAME, XML_PB_TYPE_NODE_NAME, XML_OPTION_TYPE_NODE_NAME,
  XML_MODE_TYPE_NODE_NAME};
