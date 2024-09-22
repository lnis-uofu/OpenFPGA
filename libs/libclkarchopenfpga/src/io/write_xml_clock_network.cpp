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
#include "write_xml_utils.h"

/* Headers from pin constraint library */
#include "clock_network_xml_constants.h"
#include "write_xml_clock_network.h"

namespace openfpga {  // Begin namespace openfpga

static int write_xml_clock_tree_taps(std::fstream& fp,
                                     const ClockNetwork& clk_ntwk,
                                     const ClockTreeId& tree_id) {
  openfpga::write_tab_to_file(fp, 3);
  fp << "<" << XML_CLOCK_TREE_TAPS_NODE_NAME << ">\n";
  /* Depends on the type */
  for (ClockTapId tap_id : clk_ntwk.tree_taps(tree_id)) {
    switch (clk_ntwk.tap_type(tap_id)) {
      case ClockNetwork::e_tap_type::ALL: {
        openfpga::write_tab_to_file(fp, 4);
        fp << "<" << XML_CLOCK_TREE_TAP_ALL_NODE_NAME << "";
        write_xml_attribute(
          fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_FROM_PIN,
          clk_ntwk.tap_from_port(tap_id).to_verilog_string().c_str());
        write_xml_attribute(fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_TO_PIN,
                            clk_ntwk.tap_to_port(tap_id).c_str());
        fp << "/>"
           << "\n";
        break;
      }
      case ClockNetwork::e_tap_type::SINGLE: {
        openfpga::write_tab_to_file(fp, 4);
        fp << "<" << XML_CLOCK_TREE_TAP_SINGLE_NODE_NAME << "";
        write_xml_attribute(
          fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_FROM_PIN,
          clk_ntwk.tap_from_port(tap_id).to_verilog_string().c_str());
        write_xml_attribute(fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_TO_PIN,
                            clk_ntwk.tap_to_port(tap_id).c_str());
        write_xml_attribute(fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_X,
                            clk_ntwk.tap_x(tap_id));
        write_xml_attribute(fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_Y,
                            clk_ntwk.tap_y(tap_id));
        fp << "/>"
           << "\n";
        break;
      }
      case ClockNetwork::e_tap_type::REGION: {
        openfpga::write_tab_to_file(fp, 4);
        fp << "<" << XML_CLOCK_TREE_TAP_REGION_NODE_NAME << "";
        write_xml_attribute(
          fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_FROM_PIN,
          clk_ntwk.tap_from_port(tap_id).to_verilog_string().c_str());
        write_xml_attribute(fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_TO_PIN,
                            clk_ntwk.tap_to_port(tap_id).c_str());
        write_xml_attribute(fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_STARTX,
                            clk_ntwk.tap_bounding_box(tap_id).xmin());
        write_xml_attribute(fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_STARTY,
                            clk_ntwk.tap_bounding_box(tap_id).ymin());
        write_xml_attribute(fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_ENDX,
                            clk_ntwk.tap_bounding_box(tap_id).xmax());
        write_xml_attribute(fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_ENDY,
                            clk_ntwk.tap_bounding_box(tap_id).ymax());
        write_xml_attribute(fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_REPEATX,
                            clk_ntwk.tap_step_x(tap_id));
        write_xml_attribute(fp, XML_CLOCK_TREE_TAP_ATTRIBUTE_REPEATY,
                            clk_ntwk.tap_step_y(tap_id));
        fp << "/>"
           << "\n";
        break;
      }
      default: {
        VTR_LOG_ERROR("Invalid type of tap point!\n");
        return 1;
      }
    }
  }

  openfpga::write_tab_to_file(fp, 3);
  fp << "</" << XML_CLOCK_TREE_TAPS_NODE_NAME << ">\n";

  return 0;
}

static int write_xml_clock_spine_switch_point(
  std::fstream& fp, const ClockNetwork& clk_ntwk, const ClockSpineId& spine_id,
  const ClockSwitchPointId& switch_point_id) {
  openfpga::write_tab_to_file(fp, 3);
  fp << "<" << XML_CLOCK_SPINE_SWITCH_POINT_NODE_NAME << "";

  write_xml_attribute(
    fp, XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_TAP,
    clk_ntwk
      .spine_name(clk_ntwk.spine_switch_point_tap(spine_id, switch_point_id))
      .c_str());
  vtr::Point<int> coord =
    clk_ntwk.spine_switch_point(spine_id, switch_point_id);
  write_xml_attribute(fp, XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_X, coord.x());
  write_xml_attribute(fp, XML_CLOCK_SPINE_SWITCH_POINT_ATTRIBUTE_Y, coord.y());

  /* Optional: internal drivers */
  if (clk_ntwk.spine_switch_point_internal_drivers(spine_id, switch_point_id)
        .empty()) {
    fp << "/>"
       << "\n";
  } else {
    fp << ">"
       << "\n";
    for (ClockInternalDriverId int_driver_id :
         clk_ntwk.spine_switch_point_internal_drivers(spine_id,
                                                      switch_point_id)) {
      openfpga::write_tab_to_file(fp, 4);
      fp << "<" << XML_CLOCK_SPINE_SWITCH_POINT_INTERNAL_DRIVER_NODE_NAME;
      write_xml_attribute(
        fp, XML_CLOCK_SPINE_SWITCH_POINT_INTERNAL_DRIVER_ATTRIBUTE_FROM_PIN,
        clk_ntwk.internal_driver_from_pin(int_driver_id).c_str());
      write_xml_attribute(
        fp, XML_CLOCK_SPINE_SWITCH_POINT_INTERNAL_DRIVER_ATTRIBUTE_TO_PIN,
        clk_ntwk.internal_driver_to_pin(int_driver_id)
          .to_verilog_string()
          .c_str());
      fp << "/>"
         << "\n";
    }
    fp << "</" << XML_CLOCK_SPINE_SWITCH_POINT_NODE_NAME << ">\n";
  }

  return 0;
}

static int write_xml_clock_spine_intermediate_drivers(
  std::fstream& fp, const ClockNetwork& clk_ntwk, const ClockSpineId& spine_id,
  const vtr::Point<int>& coord) {
  std::vector<ClockInternalDriverId> int_drivers =
    clk_ntwk.spine_intermediate_drivers(spine_id, coord);
  if (int_drivers.empty()) {
    return 0;
  }
  openfpga::write_tab_to_file(fp, 3);
  fp << "<" << XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_NODE_NAME << "";

  write_xml_attribute(fp, XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_X,
                      coord.x());
  write_xml_attribute(fp, XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_Y,
                      coord.y());

  for (ClockInternalDriverId int_driver_id : int_drivers) {
    openfpga::write_tab_to_file(fp, 4);
    fp << "<" << XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_TAP_NODE_NAME;
    write_xml_attribute(
      fp, XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_FROM_PIN,
      clk_ntwk.internal_driver_from_pin(int_driver_id).c_str());
    write_xml_attribute(fp,
                        XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_ATTRIBUTE_TO_PIN,
                        clk_ntwk.internal_driver_to_pin(int_driver_id)
                          .to_verilog_string()
                          .c_str());
    fp << "/>"
       << "\n";
  }
  fp << "</" << XML_CLOCK_SPINE_INTERMEDIATE_DRIVER_NODE_NAME << ">\n";

  return 0;
}

static int write_xml_clock_spine(std::fstream& fp, const ClockNetwork& clk_ntwk,
                                 const ClockSpineId& spine_id) {
  openfpga::write_tab_to_file(fp, 2);
  fp << "<" << XML_CLOCK_SPINE_NODE_NAME << "";

  write_xml_attribute(fp, XML_CLOCK_SPINE_ATTRIBUTE_NAME,
                      clk_ntwk.spine_name(spine_id).c_str());
  vtr::Point<int> start_coord = clk_ntwk.spine_start_point(spine_id);
  write_xml_attribute(fp, XML_CLOCK_SPINE_ATTRIBUTE_START_X, start_coord.x());
  write_xml_attribute(fp, XML_CLOCK_SPINE_ATTRIBUTE_START_Y, start_coord.y());
  vtr::Point<int> end_coord = clk_ntwk.spine_end_point(spine_id);
  write_xml_attribute(fp, XML_CLOCK_SPINE_ATTRIBUTE_END_X, end_coord.x());
  write_xml_attribute(fp, XML_CLOCK_SPINE_ATTRIBUTE_END_Y, end_coord.y());
  if (clk_ntwk.is_vague_coordinate(spine_id)) {
    write_xml_attribute(fp, XML_CLOCK_SPINE_ATTRIBUTE_TYPE,
                        rr_node_typename[clk_ntwk.spine_track_type(spine_id)]);
    write_xml_attribute(
      fp, XML_CLOCK_SPINE_ATTRIBUTE_DIRECTION,
      DIRECTION_STRING[size_t(clk_ntwk.spine_direction(spine_id))]);
  }

  fp << ">"
     << "\n";

  for (const vtr::Point<int>& coord : clk_ntwk.spine_coordinates(spine_id)) {
    write_xml_clock_spine_intermediate_drivers(fp, clk_ntwk, spine_id, coord);
  }

  for (const ClockSwitchPointId& switch_point_id :
       clk_ntwk.spine_switch_points(spine_id)) {
    write_xml_clock_spine_switch_point(fp, clk_ntwk, spine_id, switch_point_id);
  }

  openfpga::write_tab_to_file(fp, 2);
  fp << "</" << XML_CLOCK_SPINE_NODE_NAME << "";
  fp << ">"
     << "\n";

  return 0;
}

/********************************************************************
 * A writer to output a clock tree to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 *******************************************************************/
static int write_xml_clock_tree(std::fstream& fp, const ClockNetwork& clk_ntwk,
                                const ClockTreeId& tree_id) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  openfpga::write_tab_to_file(fp, 1);
  fp << "<" << XML_CLOCK_TREE_NODE_NAME << "";

  if (false == clk_ntwk.valid_tree_id(tree_id)) {
    return 1;
  }

  write_xml_attribute(fp, XML_CLOCK_TREE_ATTRIBUTE_NAME,
                      clk_ntwk.tree_name(tree_id).c_str());
  write_xml_attribute(
    fp, XML_CLOCK_TREE_ATTRIBUTE_GLOBAL_PORT,
    clk_ntwk.tree_global_port(tree_id).to_verilog_string().c_str());
  fp << ">"
     << "\n";

  /* Output all the pins under this bus */
  for (const ClockSpineId& spine_id : clk_ntwk.spines(tree_id)) {
    write_xml_clock_spine(fp, clk_ntwk, spine_id);
  }

  write_xml_clock_tree_taps(fp, clk_ntwk, tree_id);

  openfpga::write_tab_to_file(fp, 1);
  fp << "</" << XML_CLOCK_TREE_NODE_NAME << "";
  fp << ">"
     << "\n";

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

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node */
  fp << "<" << XML_CLOCK_NETWORK_ROOT_NAME;
  write_xml_attribute(fp, XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_SEGMENT,
                      clk_ntwk.default_segment_name().c_str());
  write_xml_attribute(fp, XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_TAP_SWITCH,
                      clk_ntwk.default_tap_switch_name().c_str());
  write_xml_attribute(fp, XML_CLOCK_NETWORK_ATTRIBUTE_DEFAULT_DRIVER_SWITCH,
                      clk_ntwk.default_driver_switch_name().c_str());
  fp << ">"
     << "\n";

  int err_code = 0;

  /* Write each bus */
  for (const ClockTreeId& tree_id : clk_ntwk.trees()) {
    /* Write bus */
    err_code = write_xml_clock_tree(fp, clk_ntwk, tree_id);
    if (0 != err_code) {
      return err_code;
    }
  }

  /* Finish writing the root node */
  fp << "</" << XML_CLOCK_NETWORK_ROOT_NAME << ">"
     << "\n";

  /* Close the file stream */
  fp.close();

  return err_code;
}

}  // End of namespace openfpga
