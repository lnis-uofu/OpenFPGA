#pragma once

/* Constants required by XML parser */

constexpr const char* XML_TILE_ANNOTATIONS_NODE_NAME = "tile_annotations";

constexpr const char* XML_TILE_ANNOTATIONS_GLOBAL_PORT_NODE_NAME = "global_port";
constexpr const char* XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_NAME = "name";
constexpr const char* XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_IS_CLOCK = "is_clock";
constexpr const char* XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_IS_SET = "is_set";
constexpr const char* XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_IS_RESET = "is_reset";
constexpr const char* XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_CLOCK_ARCH_TREE_NAME = "clock_arch_tree_name";
constexpr const char* XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_DEFAULT_VAL = "default_val";
constexpr const char* XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_NODE_NAME = "tile";
constexpr const char* XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_ATTR_NAME = "name";
constexpr const char* XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_ATTR_PORT = "port";
constexpr const char* XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_ATTR_X = "x";
constexpr const char* XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_ATTR_Y = "y";

constexpr const char* XML_TILE_ANNOTATIONS_MERGE_SUBTILE_PORTS_NODE_NAME = "merge_subtile_ports";
constexpr const char* XML_TILE_ANNOTATIONS_MERGE_SUBTILE_PORTS_ATTR_TILE = "tile";
constexpr const char* XML_TILE_ANNOTATIONS_MERGE_SUBTILE_PORTS_ATTR_PORT = "port";

constexpr const char* XML_TILE_ANNOTATIONS_PHYSICAL_EQUIVALENT_SITE_NODE_NAME = "physical_equivalent_site";
constexpr const char* XML_TILE_ANNOTATIONS_PHYSICAL_EQUIVALENT_SITE_ATTR_TILE = "tile";
constexpr const char* XML_TILE_ANNOTATIONS_PHYSICAL_EQUIVALENT_SITE_ATTR_SUBTILE = "subtile";
constexpr const char* XML_TILE_ANNOTATIONS_PHYSICAL_EQUIVALENT_SITE_ATTR_SITE = "site";
