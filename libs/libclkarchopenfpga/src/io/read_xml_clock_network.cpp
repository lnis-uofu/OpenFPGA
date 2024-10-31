/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML of clock network file to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from libopenfpga util library */
#include "openfpga_port_parser.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "clock_network_xml_constants.h"
#include "read_xml_clock_network.h"
#include "read_xml_util.h"

namespace openfpga {  // Begin namespace openfpga

/********************************************************************
 * Parse XML codes of a <all> to an object of ClockNetwork
 *******************************************************************/
static void read_xml_clock_tree_tap_type_all(pugi::xml_node& xml_tap,
                                             const pugiutil::loc_data& loc_data,
                                             ClockNetwork& clk_ntwk,
                                             const ClockTreeId& tree_id) {
  if (!clk_ntwk.valid_tree_id(tree_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_tap),
                   "Invalid id of a clock tree!\n");
  }

  std::string from_pin_name =
    get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_FROM_PIN, loc_data)
      .as_string();
  std::string to_pin_name =
    get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_TO_PIN, loc_data)
      .as_string();
  PortParser from_port_parser(from_pin_name);
  clk_ntwk.add_tree_tap(tree_id, from_port_parser.port(), to_pin_name);
}

/********************************************************************
 * Parse XML codes of a <single> to an object of ClockNetwork
 *******************************************************************/
static void read_xml_clock_tree_tap_type_single(
  pugi::xml_node& xml_tap, const pugiutil::loc_data& loc_data,
  ClockNetwork& clk_ntwk, const ClockTreeId& tree_id) {
  if (!clk_ntwk.valid_tree_id(tree_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_tap),
                   "Invalid id of a clock tree!\n");
  }

  std::string from_pin_name =
    get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_FROM_PIN, loc_data)
      .as_string();
  std::string to_pin_name =
    get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_TO_PIN, loc_data)
      .as_string();
  PortParser from_port_parser(from_pin_name);
  ClockTapId tap_id =
    clk_ntwk.add_tree_tap(tree_id, from_port_parser.port(), to_pin_name);

  /* Single tap only require a coordinate */
  size_t tap_x = get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_X,
                               loc_data, pugiutil::ReqOpt::REQUIRED)
                   .as_int();
  size_t tap_y = get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_Y,
                               loc_data, pugiutil::ReqOpt::REQUIRED)
                   .as_int();
  clk_ntwk.set_tap_bounding_box(tap_id,
                                vtr::Rect<size_t>(tap_x, tap_y, tap_x, tap_y));
}

/********************************************************************
 * Parse XML codes of a <region> to an object of ClockNetwork
 *******************************************************************/
static void read_xml_clock_tree_tap_type_region(
  pugi::xml_node& xml_tap, const pugiutil::loc_data& loc_data,
  ClockNetwork& clk_ntwk, const ClockTreeId& tree_id) {
  if (!clk_ntwk.valid_tree_id(tree_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_tap),
                   "Invalid id of a clock tree!\n");
  }

  std::string from_pin_name =
    get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_FROM_PIN, loc_data)
      .as_string();
  std::string to_pin_name =
    get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_TO_PIN, loc_data)
      .as_string();
  PortParser from_port_parser(from_pin_name);
  ClockTapId tap_id =
    clk_ntwk.add_tree_tap(tree_id, from_port_parser.port(), to_pin_name);

  /* Region require a bounding box */
  size_t tap_start_x =
    get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_STARTX, loc_data,
                  pugiutil::ReqOpt::REQUIRED)
      .as_int();
  size_t tap_start_y =
    get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_STARTY, loc_data,
                  pugiutil::ReqOpt::REQUIRED)
      .as_int();
  size_t tap_end_x = get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_ENDX,
                                   loc_data, pugiutil::ReqOpt::REQUIRED)
                       .as_int();
  size_t tap_end_y = get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_ENDY,
                                   loc_data, pugiutil::ReqOpt::REQUIRED)
                       .as_int();
  clk_ntwk.set_tap_bounding_box(
    tap_id, vtr::Rect<size_t>(tap_start_x, tap_start_y, tap_end_x, tap_end_y));

  /* Default step is all 1 */
  size_t tap_step_x =
    get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_REPEATX, loc_data)
      .as_int(1);
  size_t tap_step_y =
    get_attribute(xml_tap, XML_CLOCK_TREE_TAP_ATTRIBUTE_REPEATY, loc_data)
      .as_int(1);
  clk_ntwk.set_tap_step_x(tap_id, tap_step_x);
  clk_ntwk.set_tap_step_y(tap_id, tap_step_y);
}

static void read_xml_clock_tree_taps(pugi::xml_node& xml_taps,
                                     const pugiutil::loc_data& loc_data,
                                     ClockNetwork& clk_ntwk,
                                     const ClockTreeId& tree_id) {
  for (pugi::xml_node xml_tap : xml_taps.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_tap.name() == std::string(XML_CLOCK_TREE_TAP_ALL_NODE_NAME)) {
      read_xml_clock_tree_tap_type_all(xml_tap, loc_data, clk_ntwk, tree_id);
    } else if (xml_tap.name() ==
               std::string(XML_CLOCK_TREE_TAP_REGION_NODE_NAME)) {
      read_xml_clock_tree_tap_type_region(xml_tap, loc_data, clk_ntwk, tree_id);
    } else if (xml_tap.name() ==
               std::string(XML_CLOCK_TREE_TAP_SINGLE_NODE_NAME)) {
      read_xml_clock_tree_tap_type_single(xml_tap, loc_data, clk_ntwk, tree_id);
    } else {
      bad_tag(
        xml_taps, loc_data, xml_tap,
        {XML_CLOCK_TREE_TAP_ALL_NODE_NAME, XML_CLOCK_TREE_TAP_REGION_NODE_NAME,
         XML_CLOCK_TREE_TAP_SINGLE_NODE_NAME});
    }
  }
}

/********************************************************************
 * Parse XML codes of a <switch_point> to an object of ClockNetwork
 *******************************************************************/
static void read_xml_clock_spine_switch_point_internal_driver(
  pugi::xml_node& xml_int_driver, const pugiutil::loc_data& loc_data,
  ClockNetwork& clk_ntwk, const ClockSpineId& spine_id,
  const ClockSwitchPointId& switch_point_id) {
  if (!clk_ntwk.valid_spine_id(spine_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_int_driver),
                   "Invalid id of a clock spine!\n");
  }

  std::string int_driver_from_port_name =
    get_attribute(
      xml_int_driver,
      XML_CLOCK_SPINE_SWITCH_POINT_INTERNAL_DRIVER_ATTRIBUTE_FROM_PIN, loc_data)
      .as_string();
  std::string int_driver_to_port_name =
    get_attribute(xml_int_driver,
                  XML_CLOCK_SPINE_SWITCH_POINT_INTERNAL_DRIVER_ATTRIBUTE_TO_PIN,
                  loc_data)
      .as_string();
  clk_ntwk.add_spine_switch_point_internal_driver(spine_id, switch_point_id,
                                                  int_driver_from_port_name,
                                                  int_driver_to_port_name);
}

/********************************************************************
 * Parse XML codes of a <tap> to an object of ClockNetwork
 *******************************************************************/
static void read_xml_clock_spine_intermediate_driver_tap(
  pugi::xml_node& xml_int_driver, const pugiutil::loc_data& loc_data,
  ClockNetwork& clk_ntwk, const ClockSpineId& spine_id,
  const vtr::Point<int>& spine_coord) {
  if (!clk_ntwk.valid_spine_id(spine_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_int_driver),
                   "Invalid id of a clock spine!\n");
  }

  std::string int_driver_from_port_name =
    get_attribute(xml_int_driver,
                  XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_FROM_PIN,
                  loc_data)
      .as_string();
  std::string int_driver_to_port_name =
    get_attribute(xml_int_driver,
                  XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_TO_PIN,
                  loc_data)
      .as_string();
  clk_ntwk.add_spine_intermediate_driver(
    spine_id, spine_coord, int_driver_from_port_name, int_driver_to_port_name);
}

/********************************************************************
 * Parse XML codes of a <switch_point> to an object of ClockNetwork
 *******************************************************************/
static void read_xml_clock_spine_switch_point(
  pugi::xml_node& xml_switch_point, const pugiutil::loc_data& loc_data,
  ClockNetwork& clk_ntwk, const ClockSpineId& spine_id) {
  if (!clk_ntwk.valid_spine_id(spine_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_switch_point),
                   "Invalid id of a clock spine!\n");
  }

  std::string tap_spine_name =
    get_attribute(xml_switch_point, XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_TAP,
                  loc_data)
      .as_string();

  /* Try to find an existing spine, if not, create one */
  ClockSpineId tap_spine_id = clk_ntwk.find_spine(tap_spine_name);
  if (!tap_spine_id) {
    tap_spine_id = clk_ntwk.create_spine(tap_spine_name);
  }

  if (false == clk_ntwk.valid_spine_id(tap_spine_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_switch_point),
                   "Fail to create a clock spine!\n");
  }

  int tap_x = get_attribute(xml_switch_point,
                            XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_X, loc_data)
                .as_int();
  int tap_y = get_attribute(xml_switch_point,
                            XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_Y, loc_data)
                .as_int();

  ClockSwitchPointId switch_point_id = clk_ntwk.add_spine_switch_point(
    spine_id, tap_spine_id, vtr::Point<int>(tap_x, tap_y));

  /* Add internal drivers if possible */
  for (pugi::xml_node xml_int_driver : xml_switch_point.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_int_driver.name() ==
        std::string(XML_CLOCK_SPINE_SWITCH_POINT_INTERNAL_DRIVER_NODE_NAME)) {
      read_xml_clock_spine_switch_point_internal_driver(
        xml_int_driver, loc_data, clk_ntwk, spine_id, switch_point_id);
    } else {
      bad_tag(xml_int_driver, loc_data, xml_switch_point,
              {XML_CLOCK_SPINE_SWITCH_POINT_INTERNAL_DRIVER_NODE_NAME});
    }
  }
}

/********************************************************************
 * Parse XML codes of a <driver> to an object of ClockNetwork
 *******************************************************************/
static void read_xml_clock_spine_intermediate_driver(
  pugi::xml_node& xml_driver, const pugiutil::loc_data& loc_data,
  ClockNetwork& clk_ntwk, const ClockSpineId& spine_id) {
  if (!clk_ntwk.valid_spine_id(spine_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_driver),
                   "Invalid id of a clock spine!\n");
  }

  int tap_x =
    get_attribute(xml_driver, XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_X,
                  loc_data)
      .as_int();
  int tap_y =
    get_attribute(xml_driver, XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_Y,
                  loc_data)
      .as_int();

  /* Add internal drivers if possible */
  for (pugi::xml_node xml_int_driver : xml_driver.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_int_driver.name() ==
        std::string(XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_TAP_NODE_NAME)) {
      read_xml_clock_spine_intermediate_driver_tap(
        xml_int_driver, loc_data, clk_ntwk, spine_id,
        vtr::Point<int>(tap_x, tap_y));
    } else {
      bad_tag(xml_int_driver, loc_data, xml_driver,
              {XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_TAP_NODE_NAME});
    }
  }
}

/********************************************************************
 * Convert string to the enumerate of model type
 *******************************************************************/
static t_rr_type string_to_track_type(const std::string& type_string) {
  for (size_t itype = 0; itype < NUM_RR_TYPES; ++itype) {
    if (std::string(rr_node_typename[itype]) == type_string) {
      return static_cast<t_rr_type>(itype);
    }
  }

  /* Reach here, we have an invalid value, error out */
  return NUM_RR_TYPES;
}

/********************************************************************
 * Convert string to the enumerate of model type
 *******************************************************************/
static Direction string_to_direction(const std::string& type_string) {
  for (size_t itype = 0; itype < size_t(Direction::NUM_DIRECTIONS); ++itype) {
    if (std::string(DIRECTION_STRING[itype]) == type_string) {
      return static_cast<Direction>(itype);
    }
  }

  /* Reach here, we have an invalid value, error out */
  return Direction::NUM_DIRECTIONS;
}

/********************************************************************
 * Parse XML codes of a <spine> to an object of ClockNetwork
 *******************************************************************/
static void read_xml_clock_spine(pugi::xml_node& xml_spine,
                                 const pugiutil::loc_data& loc_data,
                                 ClockNetwork& clk_ntwk,
                                 const ClockTreeId& tree_id) {
  if (!clk_ntwk.valid_tree_id(tree_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_spine),
                   "Invalid id of a clock tree!\n");
  }

  std::string clk_spine_name =
    get_attribute(xml_spine, XML_CLOCK_SPINE_ATTRIBUTE_NAME, loc_data)
      .as_string();

  /* Try to find an existing spine, if not, create one */
  ClockSpineId spine_id = clk_ntwk.find_spine(clk_spine_name);
  if (!spine_id) {
    spine_id = clk_ntwk.create_spine(clk_spine_name);
  }

  if (false == clk_ntwk.valid_spine_id(spine_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_spine),
                   "Fail to create a clock spine!\n");
  }

  clk_ntwk.set_spine_parent_tree(spine_id, tree_id);

  int start_x =
    get_attribute(xml_spine, XML_CLOCK_SPINE_ATTRIBUTE_START_X, loc_data)
      .as_int();
  int start_y =
    get_attribute(xml_spine, XML_CLOCK_SPINE_ATTRIBUTE_START_Y, loc_data)
      .as_int();
  clk_ntwk.set_spine_start_point(spine_id, vtr::Point<int>(start_x, start_y));

  int end_x =
    get_attribute(xml_spine, XML_CLOCK_SPINE_ATTRIBUTE_END_X, loc_data)
      .as_int();
  int end_y =
    get_attribute(xml_spine, XML_CLOCK_SPINE_ATTRIBUTE_END_Y, loc_data)
      .as_int();
  clk_ntwk.set_spine_end_point(spine_id, vtr::Point<int>(end_x, end_y));

  if (clk_ntwk.is_vague_coordinate(spine_id)) {
    std::string track_type_name =
      get_attribute(xml_spine, XML_CLOCK_SPINE_ATTRIBUTE_TYPE, loc_data)
        .as_string();
    t_rr_type track_type = string_to_track_type(track_type_name);
    if (CHANX != track_type && CHANY != track_type) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_spine),
                     "Invalid track type! Expect '%s' or '%s'!\n",
                     rr_node_typename[CHANX], rr_node_typename[CHANY]);
    }
    clk_ntwk.set_spine_track_type(spine_id, track_type);

    std::string direction_name =
      get_attribute(xml_spine, XML_CLOCK_SPINE_ATTRIBUTE_DIRECTION, loc_data)
        .as_string();
    Direction direction_type = string_to_direction(direction_name);
    if (Direction::INC != direction_type && Direction::DEC != direction_type) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_spine),
                     "Invalid direction type! Expect '%s' or '%s'!\n",
                     DIRECTION_STRING[size_t(Direction::INC)],
                     DIRECTION_STRING[size_t(Direction::DEC)]);
    }
    clk_ntwk.set_spine_direction(spine_id, direction_type);
  }

  for (pugi::xml_node xml_switch_point : xml_spine.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_switch_point.name() ==
        std::string(XML_CLOCK_SPINE_SWITCH_POINT_NODE_NAME)) {
      read_xml_clock_spine_switch_point(xml_switch_point, loc_data, clk_ntwk,
                                        spine_id);
    } else if (xml_switch_point.name() ==
               std::string(XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_NODE_NAME)) {
      read_xml_clock_spine_intermediate_driver(xml_switch_point, loc_data,
                                               clk_ntwk, spine_id);

    } else {
      bad_tag(xml_switch_point, loc_data, xml_spine,
              {XML_CLOCK_SPINE_SWITCH_POINT_NODE_NAME,
               XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_NODE_NAME});
    }
  }
}

/********************************************************************
 * Parse XML codes of a <clock_network> to an object of ClockNetwork
 *******************************************************************/
static void read_xml_clock_tree(pugi::xml_node& xml_clk_tree,
                                const pugiutil::loc_data& loc_data,
                                ClockNetwork& clk_ntwk) {
  std::string clk_tree_name =
    get_attribute(xml_clk_tree, XML_CLOCK_TREE_ATTRIBUTE_NAME, loc_data,
                  pugiutil::ReqOpt::REQUIRED)
      .as_string();
  std::string clk_global_port_str =
    get_attribute(xml_clk_tree, XML_CLOCK_TREE_ATTRIBUTE_GLOBAL_PORT, loc_data,
                  pugiutil::ReqOpt::REQUIRED)
      .as_string();

  /* Create a new tree in the storage */
  PortParser gport_parser(clk_global_port_str);
  ClockTreeId tree_id =
    clk_ntwk.create_tree(clk_tree_name, gport_parser.port());

  if (false == clk_ntwk.valid_tree_id(tree_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_clk_tree),
                   "Fail to create a clock tree!\n");
  }

  for (pugi::xml_node xml_spine : xml_clk_tree.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_spine.name() == std::string(XML_CLOCK_SPINE_NODE_NAME)) {
      read_xml_clock_spine(xml_spine, loc_data, clk_ntwk, tree_id);
    } else if (xml_spine.name() == std::string(XML_CLOCK_TREE_TAPS_NODE_NAME)) {
      read_xml_clock_tree_taps(xml_spine, loc_data, clk_ntwk, tree_id);
    } else {
      bad_tag(xml_spine, loc_data, xml_clk_tree,
              {XML_CLOCK_SPINE_NODE_NAME, XML_CLOCK_TREE_TAPS_NODE_NAME});
    }
  }
}

/********************************************************************
 * Parse XML codes about <clock_network> to an object of ClockNetwork
 *******************************************************************/
ClockNetwork read_xml_clock_network(const char* fname) {
  vtr::ScopedStartFinishTimer timer("Read clock network");

  ClockNetwork clk_ntwk;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, fname);

    pugi::xml_node xml_root =
      get_single_child(doc, XML_CLOCK_NETWORK_ROOT_NAME, loc_data);

    std::string default_segment_name =
      get_attribute(xml_root, XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_SEGMENT,
                    loc_data)
        .as_string();
    clk_ntwk.set_default_segment_name(default_segment_name);

    std::string default_tap_switch_name =
      get_attribute(xml_root, XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_TAP_SWITCH,
                    loc_data)
        .as_string();
    clk_ntwk.set_default_tap_switch_name(default_tap_switch_name);

    std::string default_driver_switch_name =
      get_attribute(xml_root, XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_DRIVER_SWITCH,
                    loc_data)
        .as_string();
    clk_ntwk.set_default_driver_switch_name(default_driver_switch_name);

    size_t num_trees =
      std::distance(xml_root.children().begin(), xml_root.children().end());

    /* Count the total number of spines */
    size_t num_spines = 0;
    for (pugi::xml_node xml_tree : xml_root.children()) {
      num_spines +=
        std::distance(xml_tree.children().begin(), xml_tree.children().end());
    }

    /* Reserve memory space */
    clk_ntwk.reserve_trees(num_trees);
    clk_ntwk.reserve_spines(num_spines);

    for (pugi::xml_node xml_tree : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_tree.name() != std::string(XML_CLOCK_TREE_NODE_NAME)) {
        bad_tag(xml_tree, loc_data, xml_root, {XML_CLOCK_TREE_NODE_NAME});
      }
      read_xml_clock_tree(xml_tree, loc_data, clk_ntwk);
    }
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(fname, e.line(), "%s", e.what());
  }

  return clk_ntwk;
}

}  // End of namespace openfpga
