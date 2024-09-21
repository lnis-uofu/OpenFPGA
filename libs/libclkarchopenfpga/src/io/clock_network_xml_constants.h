#ifndef CLOCK_NETWORK_XML_CONSTANTS_H
#define CLOCK_NETWORK_XML_CONSTANTS_H

/* Constants required by XML parser */

constexpr const char* XML_CLOCK_NETWORK_ROOT_NAME = "clock_networks";
constexpr const char* XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_SEGMENT =
  "default_segment";
constexpr const char* XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_TAP_SWITCH =
  "default_tap_switch";
constexpr const char* XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_DRIVER_SWITCH =
  "default_driver_switch";
constexpr const char* XML_CLOCK_TREE_NODE_NAME = "clock_network";
constexpr const char* XML_CLOCK_TREE_ATTRIBUTE_NAME = "name";
constexpr const char* XML_CLOCK_TREE_ATTRIBUTE_GLOBAL_PORT = "global_port";
constexpr const char* XML_CLOCK_SPINE_NODE_NAME = "spine";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_NAME = "name";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_START_X = "start_x";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_START_Y = "start_y";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_END_X = "end_x";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_END_Y = "end_y";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_TYPE = "type";
constexpr const char* XML_CLOCK_SPINE_ATTRIBUTE_DIRECTION = "direction";
constexpr const char* XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_NODE_NAME =
  "intermediate_driver";
constexpr const char* XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_TAP_NODE_NAME = "tap";
constexpr const char* XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_X = "x";
constexpr const char* XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_Y = "y";
constexpr const char* XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_FROM_PIN =
  "from_pin";
constexpr const char* XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_TO_PIN =
  "to_pin";
constexpr const char* XML_CLOCK_SPINE_SWITCH_POINT_NODE_NAME = "switch_point";
constexpr const char* XML_CLOCK_SPINE_SWITCH_POINT_INTERNAL_DRIVER_NODE_NAME =
  "internal_driver";
constexpr const char*
  XML_CLOCK_SPINE_SWITCH_POINT_INTERNAL_DRIVER_ATTRIBUTE_FROM_PIN = "from_pin";
constexpr const char*
  XML_CLOCK_SPINE_SWITCH_POINT_INTERNAL_DRIVER_ATTRIBUTE_TO_PIN = "to_pin";
constexpr const char* XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_TAP = "tap";
constexpr const char* XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_X = "x";
constexpr const char* XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_Y = "y";
constexpr const char* XML_CLOCK_TREE_TAPS_NODE_NAME = "taps";
constexpr const char* XML_CLOCK_TREE_TAP_ALL_NODE_NAME = "all";
constexpr const char* XML_CLOCK_TREE_TAP_REGION_NODE_NAME = "region";
constexpr const char* XML_CLOCK_TREE_TAP_SINGLE_NODE_NAME = "single";
constexpr const char* XML_CLOCK_TREE_TAP_ATTRIBUTE_FROM_PIN = "from_pin";
constexpr const char* XML_CLOCK_TREE_TAP_ATTRIBUTE_TO_PIN = "to_pin";
constexpr const char* XML_CLOCK_TREE_TAP_ATTRIBUTE_X = "x";
constexpr const char* XML_CLOCK_TREE_TAP_ATTRIBUTE_Y = "y";
constexpr const char* XML_CLOCK_TREE_TAP_ATTRIBUTE_STARTX = "start_x";
constexpr const char* XML_CLOCK_TREE_TAP_ATTRIBUTE_STARTY = "start_y";
constexpr const char* XML_CLOCK_TREE_TAP_ATTRIBUTE_ENDX = "end_x";
constexpr const char* XML_CLOCK_TREE_TAP_ATTRIBUTE_ENDY = "end_y";
constexpr const char* XML_CLOCK_TREE_TAP_ATTRIBUTE_REPEATX = "repeat_x";
constexpr const char* XML_CLOCK_TREE_TAP_ATTRIBUTE_REPEATY = "repeat_y";

#endif
