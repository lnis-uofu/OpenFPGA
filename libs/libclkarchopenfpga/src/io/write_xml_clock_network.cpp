/********************************************************************
 * This file includes functions that outputs a clock network object to XML
 *format
 *******************************************************************/
/* Headers from system goes first */
#include <algorithm>
#include <string>

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpga util library */
#include "openfpga_digest.h"

/* Headers from arch openfpga library */
#include "pugixml.hpp"

/* Headers from pin constraint library */
#include "clock_network_xml_constants.h"
#include "write_xml_clock_network.h"

namespace openfpga {  // Begin namespace openfpga

static int write_xml_clock_tree_taps(pugi::xml_node& root_node,
                                     const ClockNetwork& clk_ntwk,
                                     const ClockTreeId& tree_id) {
  pugi::xml_node taps_node =
    root_node.append_child(XML_CLOCK_TREE_TAPS_NODE_NAME);

  /* Depends on the type */
  for (ClockTapId tap_id : clk_ntwk.tree_taps(tree_id)) {
    switch (clk_ntwk.tap_type(tap_id)) {
      case ClockNetwork::e_tap_type::ALL: {
        pugi::xml_node tap_node =
          taps_node.append_child(XML_CLOCK_TREE_TAP_ALL_NODE_NAME);
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_FROM_PIN) =
          clk_ntwk.tap_from_port(tap_id).to_verilog_string().c_str();
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_TO_PIN) =
          clk_ntwk.tap_to_port(tap_id).c_str();
        break;
      }
      case ClockNetwork::e_tap_type::SINGLE: {
        pugi::xml_node tap_node =
          taps_node.append_child(XML_CLOCK_TREE_TAP_SINGLE_NODE_NAME);
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_FROM_PIN) =
          clk_ntwk.tap_from_port(tap_id).to_verilog_string().c_str();
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_TO_PIN) =
          clk_ntwk.tap_to_port(tap_id).c_str();
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_X) =
          std::to_string(clk_ntwk.tap_x(tap_id)).c_str();
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_Y) =
          std::to_string(clk_ntwk.tap_y(tap_id)).c_str();
        break;
      }
      case ClockNetwork::e_tap_type::REGION: {
        pugi::xml_node tap_node =
          taps_node.append_child(XML_CLOCK_TREE_TAP_REGION_NODE_NAME);
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_FROM_PIN) =
          clk_ntwk.tap_from_port(tap_id).to_verilog_string().c_str();
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_TO_PIN) =
          clk_ntwk.tap_to_port(tap_id).c_str();
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_STARTX) =
          std::to_string(clk_ntwk.tap_bounding_box(tap_id).xmin()).c_str();
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_STARTY) =
          std::to_string(clk_ntwk.tap_bounding_box(tap_id).ymin()).c_str();
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_ENDX) =
          std::to_string(clk_ntwk.tap_bounding_box(tap_id).xmax()).c_str();
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_ENDY) =
          std::to_string(clk_ntwk.tap_bounding_box(tap_id).ymax()).c_str();
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_REPEATX) =
          std::to_string(clk_ntwk.tap_step_x(tap_id)).c_str();
        tap_node.append_attribute(XML_CLOCK_TREE_TAP_ATTRIBUTE_REPEATY) =
          std::to_string(clk_ntwk.tap_step_y(tap_id)).c_str();
        break;
      }
      default: {
        VTR_LOG_ERROR("Invalid type of tap point!\n");
        return 1;
      }
    }
  }

  return 0;
}

static int write_xml_clock_spine_switch_point(
  pugi::xml_node& root_node, const ClockNetwork& clk_ntwk,
  const ClockSpineId& spine_id, const ClockSwitchPointId& switch_point_id) {
  pugi::xml_node sw_node =
    root_node.append_child(XML_CLOCK_SPINE_SWITCH_POINT_NODE_NAME);

  sw_node.append_attribute(XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_TAP) =
    clk_ntwk
      .spine_name(clk_ntwk.spine_switch_point_tap(spine_id, switch_point_id))
      .c_str();
  vtr::Point<int> coord =
    clk_ntwk.spine_switch_point(spine_id, switch_point_id);
  sw_node.append_attribute(XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_X) =
    std::to_string(coord.x()).c_str();
  sw_node.append_attribute(XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_Y) =
    std::to_string(coord.y()).c_str();

  /* Optional: internal drivers */
  if (!clk_ntwk.spine_switch_point_internal_drivers(spine_id, switch_point_id)
         .empty()) {
    for (ClockInternalDriverId int_driver_id :
         clk_ntwk.spine_switch_point_internal_drivers(spine_id,
                                                      switch_point_id)) {
      pugi::xml_node int_driver_node = sw_node.append_child(
        XML_CLOCK_SPINE_SWITCH_POINT_INTERNAL_DRIVER_NODE_NAME);
      int_driver_node.append_attribute(
        XML_CLOCK_SPINE_SWITCH_POINT_INTERNAL_DRIVER_ATTRIBUTE_FROM_PIN) =
        clk_ntwk.internal_driver_from_pin(int_driver_id).c_str();
      int_driver_node.append_attribute(
        XML_CLOCK_SPINE_SWITCH_POINT_INTERNAL_DRIVER_ATTRIBUTE_TO_PIN) =
        clk_ntwk.internal_driver_to_pin(int_driver_id)
          .to_verilog_string()
          .c_str();
    }
  }

  return 0;
}

static int write_xml_clock_spine_intermediate_drivers(
  pugi::xml_node& root_node, const ClockNetwork& clk_ntwk,
  const ClockSpineId& spine_id, const vtr::Point<int>& coord) {
  std::vector<ClockInternalDriverId> int_drivers =
    clk_ntwk.spine_intermediate_drivers(spine_id, coord);
  if (int_drivers.empty()) {
    return 0;
  }

  pugi::xml_node int_driver_node =
    root_node.append_child(XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_NODE_NAME);

  int_driver_node.append_attribute(
    XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_X) =
    std::to_string(coord.x()).c_str();
  int_driver_node.append_attribute(
    XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_Y) =
    std::to_string(coord.y()).c_str();

  for (ClockInternalDriverId int_driver_id : int_drivers) {
    pugi::xml_node tap_node = int_driver_node.append_child(
      XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_TAP_NODE_NAME);
    tap_node.append_attribute(
      XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_FROM_PIN) =
      clk_ntwk.internal_driver_from_pin(int_driver_id).c_str();
    tap_node.append_attribute(
      XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_TO_PIN) =
      clk_ntwk.internal_driver_to_pin(int_driver_id)
        .to_verilog_string()
        .c_str();
  }

  return 0;
}

static int write_xml_clock_spine(pugi::xml_node& root_node,
                                 const ClockNetwork& clk_ntwk,
                                 const ClockSpineId& spine_id) {
  pugi::xml_node spine_node = root_node.append_child(XML_CLOCK_SPINE_NODE_NAME);
  spine_node.append_attribute(XML_CLOCK_SPINE_ATTRIBUTE_NAME) =
    clk_ntwk.spine_name(spine_id).c_str();
  vtr::Point<int> start_coord = clk_ntwk.spine_start_point(spine_id);
  spine_node.append_attribute(XML_CLOCK_SPINE_ATTRIBUTE_START_X) =
    std::to_string(start_coord.x()).c_str();
  spine_node.append_attribute(XML_CLOCK_SPINE_ATTRIBUTE_START_Y) =
    std::to_string(start_coord.y()).c_str();
  vtr::Point<int> end_coord = clk_ntwk.spine_end_point(spine_id);
  spine_node.append_attribute(XML_CLOCK_SPINE_ATTRIBUTE_END_X) =
    std::to_string(end_coord.x()).c_str();
  spine_node.append_attribute(XML_CLOCK_SPINE_ATTRIBUTE_END_Y) =
    std::to_string(end_coord.y()).c_str();
  if (clk_ntwk.is_vague_coordinate(spine_id)) {
    spine_node.append_attribute(XML_CLOCK_SPINE_ATTRIBUTE_TYPE) =
      rr_node_typename[clk_ntwk.spine_track_type(spine_id)];
    spine_node.append_attribute(XML_CLOCK_SPINE_ATTRIBUTE_DIRECTION) =
      DIRECTION_STRING[size_t(clk_ntwk.spine_direction(spine_id))];
  }

  for (const vtr::Point<int>& coord : clk_ntwk.spine_coordinates(spine_id)) {
    write_xml_clock_spine_intermediate_drivers(spine_node, clk_ntwk, spine_id,
                                               coord);
  }

  for (const ClockSwitchPointId& switch_point_id :
       clk_ntwk.spine_switch_points(spine_id)) {
    write_xml_clock_spine_switch_point(spine_node, clk_ntwk, spine_id,
                                       switch_point_id);
  }

  return 0;
}

/********************************************************************
 * A writer to output a clock tree to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 *******************************************************************/
static int write_xml_clock_tree(pugi::xml_node& root_node,
                                const ClockNetwork& clk_ntwk,
                                const ClockTreeId& tree_id) {
  pugi::xml_node tree_node = root_node.append_child(XML_CLOCK_TREE_NODE_NAME);
  if (false == clk_ntwk.valid_tree_id(tree_id)) {
    return 1;
  }

  tree_node.append_attribute(XML_CLOCK_TREE_ATTRIBUTE_NAME) =
    clk_ntwk.tree_name(tree_id).c_str();
  tree_node.append_attribute(XML_CLOCK_TREE_ATTRIBUTE_GLOBAL_PORT) =
    clk_ntwk.tree_global_port(tree_id).to_verilog_string().c_str();

  /* Output all the pins under this bus */
  for (const ClockSpineId& spine_id : clk_ntwk.spines(tree_id)) {
    write_xml_clock_spine(tree_node, clk_ntwk, spine_id);
  }

  write_xml_clock_tree_taps(tree_node, clk_ntwk, tree_id);

  return 0;
}

/********************************************************************
 * A writer to output a bus group object to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 *******************************************************************/
int write_xml_clock_network(const char* fname, const ClockNetwork& clk_ntwk) {
  vtr::ScopedStartFinishTimer timer("Write Clock Network");

  pugi::xml_document out_xml;

  /* Write the root node */
  pugi::xml_node root_node = out_xml.append_child(XML_CLOCK_NETWORK_ROOT_NAME);
  root_node.append_child(pugi::node_comment)
    .set_value(
      "This file is automatically generated!!! Do not modify by hand!");

  root_node.append_attribute(XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_SEGMENT) =
    clk_ntwk.default_segment_name().c_str();
  root_node.append_attribute(XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_TAP_SWITCH) =
    clk_ntwk.default_tap_switch_name().c_str();
  root_node.append_attribute(
    XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_DRIVER_SWITCH) =
    clk_ntwk.default_driver_switch_name().c_str();

  int err_code = 0;

  /* Write each bus */
  for (const ClockTreeId& tree_id : clk_ntwk.trees()) {
    /* Write bus */
    err_code = write_xml_clock_tree(root_node, clk_ntwk, tree_id);
    if (0 != err_code) {
      return err_code;
    }
  }

  out_xml.save_file(fname);

  return err_code;
}

}  // End of namespace openfpga
