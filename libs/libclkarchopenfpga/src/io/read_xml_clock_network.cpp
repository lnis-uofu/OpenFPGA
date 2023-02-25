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

  clk_ntwk.add_spine_switch_point(spine_id, tap_spine_id,
                                  vtr::Point<int>(tap_x, tap_y));
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

  for (pugi::xml_node xml_switch_point : xml_spine.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_switch_point.name() !=
        std::string(XML_CLOCK_SPINE_SWITCH_POINT_NODE_NAME)) {
      bad_tag(xml_switch_point, loc_data, xml_spine,
              {XML_CLOCK_SPINE_SWITCH_POINT_NODE_NAME});
    }
    read_xml_clock_spine_switch_point(xml_switch_point, loc_data, clk_ntwk,
                                      spine_id);
  }
}

/********************************************************************
 * Parse XML codes of a <clock_network> to an object of ClockNetwork
 *******************************************************************/
static void read_xml_clock_tree(pugi::xml_node& xml_clk_tree,
                                const pugiutil::loc_data& loc_data,
                                ClockNetwork& clk_ntwk) {
  std::string clk_tree_name =
    get_attribute(xml_clk_tree, XML_CLOCK_TREE_ATTRIBUTE_NAME, loc_data)
      .as_string();
  int clk_tree_width =
    get_attribute(xml_clk_tree, XML_CLOCK_TREE_ATTRIBUTE_WIDTH, loc_data)
      .as_int();

  /* Create a new tree in the storage */
  ClockTreeId tree_id = clk_ntwk.create_tree(clk_tree_name, clk_tree_width);

  if (false == clk_ntwk.valid_tree_id(tree_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_clk_tree),
                   "Fail to create a clock tree!\n");
  }

  for (pugi::xml_node xml_spine : xml_clk_tree.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_spine.name() != std::string(XML_CLOCK_SPINE_NODE_NAME)) {
      bad_tag(xml_spine, loc_data, xml_clk_tree, {XML_CLOCK_SPINE_NODE_NAME});
    }
    read_xml_clock_spine(xml_spine, loc_data, clk_ntwk, tree_id);
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

    std::string default_switch_name =
      get_attribute(xml_root, XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_SWITCH,
                    loc_data)
        .as_string();
    clk_ntwk.set_default_switch_name(default_switch_name);

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
