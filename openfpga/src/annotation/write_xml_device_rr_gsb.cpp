/***************************************************************************************
 * Output internal structure of DeviceRRGSB to XML format
 ***************************************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"
#include "openfpga_gz_xml_writer.h"
#include "command_exit_codes.h"

/* Headers from openfpgautil library */
#include "build_routing_module_utils.h"
#include "openfpga_digest.h"
#include "openfpga_naming.h"
#include "openfpga_rr_graph_utils.h"
#include "rr_graph_in_edges.h"
#include "rr_gsb_edges.h"
#include "side_manager.h"
#include "write_xml_device_rr_gsb.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Calculate the Manhattan distance between two rr_nodes
 ***************************************************************************************/
static int calculate_manhattan_distance(const RRGraphView& rr_graph,
                                        const RRNodeId& node1,
                                        const RRNodeId& node2) {
  uint32_t x1, x2, y1, y2;
  uint32_t offset = 0;

  x1 = rr_graph.node_xlow(node1);
  y1 = rr_graph.node_ylow(node1);
  x2 = rr_graph.node_xlow(node2);
  y2 = rr_graph.node_ylow(node2);

  if (rr_graph.node_type(node1) == e_rr_type::CHANX ||
      rr_graph.node_type(node1) == e_rr_type::CHANY) {
    if (rr_graph.node_direction(node1) == Direction::DEC) {
      // if the node is DEC, the driver is placed at xhigh and yhigh
      x1 = rr_graph.node_xhigh(node1);
      y1 = rr_graph.node_yhigh(node1);
    }
    // On the edges all the drivers are placed on first row or column
    // For tap calculation logical driver location can be found by
    // finding the difference in logical length and physical length of the
    // segment, which is the offset from the driver to the first row/column
    uint32_t physical_length =
      (rr_graph.node_xhigh(node1) - rr_graph.node_xlow(node1)) +
      (rr_graph.node_yhigh(node1) - rr_graph.node_ylow(node1));
    const RRSegmentId& src_segment_id = rr_graph.node_segment(node1);
    /* The logical segment length must be at least the physical length;
     * otherwise the unsigned subtraction below would underflow */
    VTR_ASSERT(rr_graph.rr_segments()[src_segment_id].length >=
               static_cast<int>(physical_length));
    offset = rr_graph.rr_segments()[src_segment_id].length - physical_length;
  }
  if (rr_graph.node_type(node2) == e_rr_type::CHANX ||
      rr_graph.node_type(node2) == e_rr_type::CHANY) {
    if (rr_graph.node_direction(node2) == Direction::DEC) {
      // if the node is DEC, the driver is placed at xhigh and yhigh
      x2 = rr_graph.node_xhigh(node2);
      y2 = rr_graph.node_yhigh(node2);
    }
  }

  uint32_t distance_x = std::abs(static_cast<int>(x1) - static_cast<int>(x2));
  uint32_t distance_y = std::abs(static_cast<int>(y1) - static_cast<int>(y2));
  return distance_x + distance_y + offset;
}

/***************************************************************************************
 * Output the input pin of Programmable Blocks, e.g., CLBs inside a GSB to XML
 * format
 ***************************************************************************************/
static void write_rr_gsb_ipin_connection_to_xml(
  pugi::xml_node& parent, const RRGraphView& rr_graph, const RRGraphInEdges& in_edges,
  const RRGSB& rr_gsb, const RRGSBEdges& gsb_edges, const enum e_side& gsb_side,
  const bool& include_rr_info) {
  SideManager gsb_side_manager(gsb_side);

  for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(gsb_side); ++inode) {
    const RRNodeId& cur_rr_node = rr_gsb.get_ipin_node(gsb_side, inode);
    /* General information of this IPIN */
    pugi::xml_node xml_rr_node = parent.append_child(rr_node_typename[rr_graph.node_type(cur_rr_node)]);
    xml_rr_node.append_attribute("side").set_value(gsb_side_manager.c_str());
    xml_rr_node.append_attribute("index").set_value(static_cast<unsigned long long>(inode));
    if (include_rr_info) {
      xml_rr_node.append_attribute("node_id").set_value(static_cast<unsigned long long>(size_t(cur_rr_node)));
    }
    const std::vector<RREdgeId>& driver_rr_edges =
      gsb_edges.get_ipin_node_in_edges(rr_gsb, in_edges, gsb_side, inode);
    xml_rr_node.append_attribute("mux_size").set_value(static_cast<unsigned long long>(driver_rr_edges.size()));
    /* General information of each driving nodes */
    for (const RREdgeId& edge : driver_rr_edges) {
      RRNodeId driver_node = rr_graph.edge_src_node(edge);
      /* Skip OPINs: they should be in direct connections */
      if (e_rr_type::OPIN == rr_graph.node_type(driver_node)) {
        continue;
      }

      enum e_rr_type driver_node_type = rr_graph.node_type(driver_node);

      int manhattan_distance =
        calculate_manhattan_distance(rr_graph, driver_node, cur_rr_node);

      enum Direction node_direction = rr_graph.node_direction(driver_node);

      enum e_side chan_side;
      if (driver_node_type == e_rr_type::CHANX) {
        chan_side = (node_direction == Direction::INC) ? LEFT : RIGHT;
      } else if (driver_node_type == e_rr_type::CHANY) {
        chan_side = (node_direction == Direction::INC) ? BOTTOM : TOP;
      }

      SideManager chan_side_manager(chan_side);

      int driver_node_index =
        rr_gsb.get_chan_node_index(chan_side, driver_node);
      if (-1 == driver_node_index) {
        /* If node index is not found, it could be connected to the channel
           starting from current grid location, in which case, we try to find
           the node index on the opposite side */
        driver_node_index = rr_gsb.get_chan_node_index(
          chan_side_manager.get_opposite(), driver_node);
      }
      /* We must have a valid node index */
      VTR_ASSERT(-1 != driver_node_index);
      const RRSegmentId& des_segment_id = rr_graph.node_segment(driver_node);

      // Write to a file in the following format:
      // <driver_node type="CHANX" side="TOP" index="0" node_id="0"
      // segment_id="0"/>
      pugi::xml_node xml_driver_node = xml_rr_node.append_child("driver_node");
      xml_driver_node.append_attribute("type").set_value(rr_node_typename[rr_graph.node_type(driver_node)]);
      xml_driver_node.append_attribute("side").set_value(chan_side_manager.c_str());
      if (include_rr_info) {
        xml_driver_node.append_attribute("node_id").set_value(static_cast<unsigned long long>(size_t(driver_node)));
      }
      xml_driver_node.append_attribute("index").set_value(driver_node_index);
      xml_driver_node.append_attribute("segment_id").set_value(static_cast<unsigned long long>(size_t(des_segment_id)));
      xml_driver_node.append_attribute("tap").set_value(manhattan_distance);
    }
  }
}

/***************************************************************************************
 * Output the routing tracks connections inside a GSB to XML format
 ***************************************************************************************/
static void write_rr_gsb_chan_connection_to_xml(
  pugi::xml_node& parent, const DeviceGrid& vpr_device_grid,
  const VprDeviceAnnotation& vpr_device_annotation, const RRGraphView& rr_graph,
  const RRGraphInEdges& in_edges, const RRGSB& rr_gsb,
  const RRGSBEdges& gsb_edges, const enum e_side& gsb_side,
  const bool& include_rr_info) {

  SideManager gsb_side_manager(gsb_side);

  /* Output chan nodes */
  for (size_t inode = 0; inode < rr_gsb.get_chan_width(gsb_side); ++inode) {
    /* We only care OUT_PORT */
    if (OUT_PORT != rr_gsb.get_chan_node_direction(gsb_side, inode)) {
      continue;
    }
    /* Output drivers */
    const RRNodeId& cur_rr_node = rr_gsb.get_chan_node(gsb_side, inode);
    std::vector<RREdgeId> driver_rr_edges =
      gsb_edges.get_chan_node_in_edges(rr_gsb, in_edges, gsb_side, inode);

    /* Output node information: location, index, side */
    const RRSegmentId& src_segment_id =
      rr_gsb.get_chan_node_segment(gsb_side, inode);

    /* Check if this node is directly connected to the node on the opposite side
     */
    if (true == rr_gsb.is_sb_node_passing_wire(rr_graph, gsb_side, inode)) {
      driver_rr_edges.clear();
    }

    e_rr_type cur_node_type = rr_graph.node_type(cur_rr_node);

    pugi::xml_node xml_rr_node = parent.append_child(rr_node_typename[cur_node_type]);
    xml_rr_node.append_attribute("side").set_value(gsb_side_manager.c_str());
    xml_rr_node.append_attribute("index").set_value(static_cast<unsigned long long>(inode));
    if (include_rr_info) {
      xml_rr_node.append_attribute("node_id").set_value(static_cast<unsigned long long>(size_t(cur_rr_node)));
      xml_rr_node.append_attribute("segment_id").set_value(static_cast<unsigned long long>(size_t(src_segment_id)));
      xml_rr_node.append_attribute("segment_name").set_value(rr_graph.rr_segments(src_segment_id).name.c_str());
    }
    xml_rr_node.append_attribute("mux_size").set_value(static_cast<unsigned long long>(driver_rr_edges.size()));
    if (include_rr_info) {
	  xml_rr_node.append_attribute("sb_module_pin_name").set_value(
        generate_sb_module_track_port_name(cur_node_type, gsb_side,
                                           OUT_PORT).c_str());
    }

    /* Direct connection: output the node on the opposite side */
    if (0 == driver_rr_edges.size()) {
      SideManager oppo_side = gsb_side_manager.get_opposite();
      pugi::xml_node xml_driver_node = xml_rr_node.append_child("driver_node");
      xml_driver_node.append_attribute("type").set_value(rr_node_typename[cur_node_type]);
      xml_driver_node.append_attribute("side").set_value(oppo_side.c_str());
      xml_driver_node.append_attribute("index").set_value(rr_graph.node_track_num(cur_rr_node));
      if (include_rr_info) {
        xml_driver_node.append_attribute("node_id").set_value(static_cast<unsigned long long>(size_t(cur_rr_node)));
        xml_driver_node.append_attribute("segment_id").set_value(static_cast<unsigned long long>(size_t(src_segment_id)));
        xml_driver_node.append_attribute("segment_name").set_value(
           rr_graph.rr_segments(src_segment_id).name.c_str());
        xml_driver_node.append_attribute("sb_module_pin_name").set_value(
           generate_sb_module_track_port_name(cur_node_type,
                                              oppo_side.get_side(), IN_PORT).c_str());
      }
    } else {
      for (const RREdgeId& driver_rr_edge : driver_rr_edges) {
        const RRNodeId& driver_rr_node = rr_graph.edge_src_node(driver_rr_edge);
        e_side driver_node_side = NUM_2D_SIDES;
        int driver_node_index = -1;
        rr_gsb.get_node_side_and_index(rr_graph, driver_rr_node, IN_PORT,
                                       driver_node_side, driver_node_index);
        SideManager driver_side(driver_node_side);

        int manhattan_distance =
          calculate_manhattan_distance(rr_graph, driver_rr_node, cur_rr_node);

        if (e_rr_type::OPIN == rr_graph.node_type(driver_rr_node)) {
          SideManager grid_side(
            get_rr_graph_single_node_side(rr_graph, driver_rr_node));

          pugi::xml_node xml_driver_node = xml_rr_node.append_child("driver_node");
          xml_driver_node.append_attribute("type").set_value(rr_node_typename[e_rr_type::OPIN]);
          xml_driver_node.append_attribute("side").set_value(driver_side.c_str());
          xml_driver_node.append_attribute("index").set_value(driver_node_index);
          xml_driver_node.append_attribute("tap").set_value(manhattan_distance);
          if (include_rr_info) {
            xml_driver_node.append_attribute("node_id").set_value(static_cast<unsigned long long>(size_t(driver_rr_node)));
            xml_driver_node.append_attribute("grid_side").set_value(grid_side.c_str());
            xml_driver_node.append_attribute("sb_module_pin_name").set_value(
               generate_sb_module_grid_port_name(
                 gsb_side, grid_side.get_side(), vpr_device_grid,
                 vpr_device_annotation, rr_graph, driver_rr_node).c_str());
          }
        } else {
          const RRSegmentId& des_segment_id =
            rr_graph.node_segment(driver_rr_node);

          pugi::xml_node xml_driver_node = xml_rr_node.append_child("driver_node");
          xml_driver_node.append_attribute("type").set_value(rr_node_typename[rr_graph.node_type(driver_rr_node)]);
          xml_driver_node.append_attribute("side").set_value(driver_side.c_str());
          xml_driver_node.append_attribute("index").set_value(driver_node_index);
          xml_driver_node.append_attribute("tap").set_value(manhattan_distance);
          if (include_rr_info) {
            xml_driver_node.append_attribute("node_id").set_value(static_cast<unsigned long long>(size_t(driver_rr_node)));
            xml_driver_node.append_attribute("segment_id").set_value(static_cast<unsigned long long>(size_t(des_segment_id)));
            xml_driver_node.append_attribute("segment_name").set_value(rr_graph.rr_segments(des_segment_id).name.c_str());
            xml_driver_node.append_attribute("sb_module_pin_name").set_value(
              generate_sb_module_track_port_name(
                rr_graph.node_type(driver_rr_node), driver_side.get_side(),
                IN_PORT).c_str());
          }
        }
      }
    }
  }
}

/***************************************************************************************
 * Output internal structure (only the switch block part) of a RRGSB to XML
 *format
 ***************************************************************************************/
static int write_rr_switch_block_to_xml(
  const std::string fname_prefix, const DeviceGrid& vpr_device_grid,
  const VprDeviceAnnotation& vpr_device_annotation, const RRGraphView& rr_graph,
  const RRGraphInEdges& in_edges, const RRGSB& rr_gsb,
  const RRGSBEdges& gsb_edges, const bool include_ipin_info,
  const RRGSBWriterOption& options) {
  /* Prepare file name */
  std::string fname(fname_prefix);
  vtr::Point<size_t> sb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  std::string curr_sb_name = generate_switch_block_module_name(sb_coordinate);
  fname += curr_sb_name;
  fname += ".xml";

  /* If there is a list of gsb list, we skip those which are not in the list */
  std::vector<std::string> include_gsb_names = options.include_gsb_names();
  if (!include_gsb_names.empty() &&
      include_gsb_names.end() == std::find(include_gsb_names.begin(),
                                           include_gsb_names.end(),
                                           curr_sb_name)) {
    return 0;
  }

  VTR_LOGV(options.verbose_output(),
           "Output internal structure of Switch Block to '%s'\n",
           fname.c_str());

  pugi::xml_document doc;
  /* Output location of the Switch Block */
  pugi::xml_node root = doc.append_child("rr_sb");
  root.append_attribute("x").set_value(static_cast<unsigned long long>(rr_gsb.get_x()));
  root.append_attribute("y").set_value(static_cast<unsigned long long>(rr_gsb.get_y()));
  root.append_attribute("num_sides").set_value(static_cast<unsigned long long>(rr_gsb.get_num_sides()));

  /* Output each side */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    SideManager gsb_side_manager(side);
    enum e_side gsb_side = gsb_side_manager.get_side();

    /* routing-track and related connections */
    write_rr_gsb_chan_connection_to_xml(
      root, vpr_device_grid, vpr_device_annotation, rr_graph, in_edges, rr_gsb,
      gsb_edges, gsb_side, options.include_rr_info());
  }
  if (include_ipin_info) {
    /* Output IPINs and related connections */
    for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
      SideManager gsb_side_manager(side);
      enum e_side gsb_side = gsb_side_manager.get_side();
      write_rr_gsb_ipin_connection_to_xml(root, rr_graph, in_edges, rr_gsb,
                                          gsb_edges, gsb_side,
                                          options.include_rr_info());
    }
  }
  /* Output to xml file */
  bool output_success = false;
  if (options.compress_output()) {
    fname += ".gz";
    GzXmlWriter writer(fname.c_str());
    if (writer.isValid()) {
      doc.save(writer);
      output_success = true;
    }
  } else {
    output_success = doc.save_file(fname.c_str());
  }
  if (output_success) {
    VTR_LOGV(options.verbose_output(), "Succeed to output XML file: %s\n", fname.c_str());
  } else {
    VTR_LOG_ERROR("Failed to output XML file: %s\n", fname.c_str());
  }
  return output_success ? 0 : 1;
}

/***************************************************************************************
 * Output internal structure (only the connection block part) of a RRGSB to XML
 *format
 ***************************************************************************************/
static int write_rr_connection_block_to_xml(const std::string fname_prefix,
                                             const RRGraphView& rr_graph,
                                             const RRGraphInEdges& in_edges,
                                             const RRGSB& rr_gsb,
                                             const RRGSBEdges& gsb_edges,
                                             const e_rr_type& cb_type,
                                             const RRGSBWriterOption& options) {
  /* Prepare file name */
  std::string fname(fname_prefix);
  vtr::Point<size_t> cb_coordinate(rr_gsb.get_cb_x(cb_type),
                                   rr_gsb.get_cb_y(cb_type));
  std::string curr_cb_name =
    generate_connection_block_module_name(cb_type, cb_coordinate);
  fname += curr_cb_name;
  fname += ".xml";

  /* If there is a list of gsb list, we skip those which are not in the list */
  std::vector<std::string> include_gsb_names = options.include_gsb_names();
  if (!include_gsb_names.empty() &&
      include_gsb_names.end() == std::find(include_gsb_names.begin(),
                                           include_gsb_names.end(),
                                           curr_cb_name)) {
    return 0;
  }

  if (options.compress_output()) {
    fname += ".gz";
  }
  VTR_LOGV(options.verbose_output(),
           "Output internal structure of Connection Block to '%s'\n",
           fname.c_str());


  pugi::xml_document doc;
  /* Output location of the Switch Block */
  pugi::xml_node root = doc.append_child("rr_cb");
  root.append_attribute("x").set_value(static_cast<unsigned long long>(rr_gsb.get_x()));
  root.append_attribute("y").set_value(static_cast<unsigned long long>(rr_gsb.get_y()));
  root.append_attribute("num_sides").set_value(static_cast<unsigned long long>(rr_gsb.get_num_sides()));

  /* Output each side */
  for (e_side side : rr_gsb.get_cb_ipin_sides(cb_type)) {
    /* IPIN nodes and related connections */
    write_rr_gsb_ipin_connection_to_xml(root, rr_graph, in_edges, rr_gsb,
                                        gsb_edges, side,
                                        options.include_rr_info());
  }

  /* Output to xml file */
  bool output_success = false;
  if (options.compress_output()) {
    GzXmlWriter writer(fname.c_str());
    if (writer.isValid()) {
        doc.save(writer);
        output_success = true;
    }
  } else {
    output_success = doc.save_file(fname.c_str());
  }
  if (output_success) {
    VTR_LOGV(options.verbose_output(), "Succeed to output XML file: %s\n", fname.c_str());
  } else {
    VTR_LOG_ERROR("Failed to output XML file: %s\n", fname.c_str());
  }
  return output_success ? 0 : 1;
}

/***************************************************************************************
 * Output internal structure (only the switch block part) of all the RRGSBs
 * in a DeviceRRGSB  to XML format
 ***************************************************************************************/
int write_device_rr_gsb_to_xml(
  const DeviceGrid& vpr_device_grid,
  const VprDeviceAnnotation& vpr_device_annotation, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb, const ModuleManager& module_manager,
  const RRGSBWriterOption& options) {
  std::string xml_dir_name = format_dir_path(options.output_directory());

  /* Create directories */
  create_directory(xml_dir_name, true, options.verbose_output());

  vtr::Point<size_t> sb_range = device_rr_gsb.get_gsb_range();

  size_t sb_counter = 0;
  std::map<e_rr_type, size_t> cb_counters = {{e_rr_type::CHANX, 0},
                                             {e_rr_type::CHANY, 0}};
  std::map<e_rr_type, std::string> cb_names = {
    {e_rr_type::CHANX, "X-direction"}, {e_rr_type::CHANY, "Y-direction"}};

  std::vector<std::string> include_gsb_names = options.include_gsb_names();

  RRGraphInEdges in_edges;
  in_edges.init(rr_graph);

  size_t num_err = 0;

  /* For each switch block, an XML file will be outputted */
  if (options.unique_module_only()) {
    /* Only output unique GSB modules */
    VTR_LOG("Only output unique GSB modules to XML\n");
    for (size_t igsb = 0; igsb < device_rr_gsb.get_num_sb_unique_module();
         ++igsb) {
      const RRGSB& rr_gsb = device_rr_gsb.get_sb_unique_module(igsb);
      vtr::Point<size_t> gsb_coord(rr_gsb.get_x(), rr_gsb.get_y());
      const RRGSBEdges& gsb_edges = device_rr_gsb.get_gsb_edges(gsb_coord);
      /* Write CBx, CBy, SB on need */
      if (options.include_sb_content()) {
        num_err += write_rr_switch_block_to_xml(
          xml_dir_name, vpr_device_grid, vpr_device_annotation, rr_graph,
          in_edges, rr_gsb, gsb_edges, module_manager.group_routing(), options);
      }
      sb_counter++;
    }
    if (false == module_manager.group_routing()) {
      for (e_rr_type cb_type : {e_rr_type::CHANX, e_rr_type::CHANY}) {
        for (size_t igsb = 0;
             igsb < device_rr_gsb.get_num_cb_unique_module(cb_type); ++igsb) {
          const RRGSB& rr_gsb =
            device_rr_gsb.get_cb_unique_module(cb_type, igsb);
          vtr::Point<size_t> gsb_coord(rr_gsb.get_x(), rr_gsb.get_y());
          const RRGSBEdges& gsb_edges = device_rr_gsb.get_gsb_edges(gsb_coord);
          if (options.include_cb_content(cb_type)) {
            num_err += write_rr_connection_block_to_xml(xml_dir_name, rr_graph, in_edges,
                                             rr_gsb, gsb_edges, cb_type,
                                             options);
            cb_counters[cb_type]++;
          }
        }
      }
    }
  } else {
    /* Output all GSB instances in the fabric (some instances may share the same
     * module) */
    for (size_t ix = 0; ix < sb_range.x(); ++ix) {
      for (size_t iy = 0; iy < sb_range.y(); ++iy) {
        const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
        const RRGSBEdges& gsb_edges = device_rr_gsb.get_gsb_edges(ix, iy);
        /* Write CBx, CBy, SB on need */
        if (options.include_sb_content()) {
          num_err += write_rr_switch_block_to_xml(xml_dir_name, vpr_device_grid,
                                       vpr_device_annotation, rr_graph,
                                       in_edges, rr_gsb, gsb_edges,
                                       module_manager.group_routing(), options);
          sb_counter++;
        }
        if (false == module_manager.group_routing()) {
          for (e_rr_type cb_type : {e_rr_type::CHANX, e_rr_type::CHANY}) {
            if (options.include_cb_content(cb_type)) {
              num_err += write_rr_connection_block_to_xml(xml_dir_name, rr_graph, in_edges,
                                               rr_gsb, gsb_edges, cb_type,
                                               options);
              cb_counters[cb_type]++;
            }
          }
        }
      }
    }
  }

  VTR_LOG("Output %lu Switch blocks to XML files under directory '%s'\n",
          sb_counter, xml_dir_name.c_str());
  for (e_rr_type cb_type : {e_rr_type::CHANX, e_rr_type::CHANY}) {
    VTR_LOG(
      "Output %lu %s Connection blocks to XML files under directory '%s'\n",
      cb_counters[cb_type], cb_names[cb_type].c_str(), xml_dir_name.c_str());
  }
  return num_err ? CMD_EXEC_FATAL_ERROR : CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
