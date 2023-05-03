#ifndef CLOCK_NETWORK_XML_CONSTANTS_H
#define CLOCK_NETWORK_XML_CONSTANTS_H

/* Constants required by XML parser */

constexpr const char* XML_CLOCK_NETWORK_ROOT_NAME = "clock_networks";
constexpr const char* XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_SEGMENT =
  "default_segment";
constexpr const char* XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_SWITCH =
  "default_switch";
constexpr const char* XML_CLOCK_TREE_NODE_NAME = "clock_network";
constexpr const char* XML_CLOCK_TREE_ATTRIBUTE_NAME = "name";
constexpr const char* XML_CLOCK_TREE_ATTRIBUTE_WIDTH = "width";
constexpr const char* XML_CLOCK_SPINE_NODE_NAME = "spine";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_NAME = "name";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_START_X = "start_x";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_START_Y = "start_y";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_END_X = "end_x";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_END_Y = "end_y";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_TYPE = "type";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_DIRECTION = "direction";
constexpr const char* XML_CLOCK_SPINE_SWITCH_POINT_NODE_NAME = "switch_point";
constexpr const char* XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_TAP = "tap";
constexpr const char* XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_X = "x";
constexpr const char* XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_Y = "y";
constexpr const char* XML_CLOCK_TREE_TAPS_NODE_NAME = "taps";
constexpr const char* XML_CLOCK_TREE_TAP_NODE_NAME = "tap";
constexpr const char* XML_CLOCK_TREE_TAP_ATTRIBUTE_TILE_PIN = "tile_pin";

#endif
