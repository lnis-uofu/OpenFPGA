/********************************************************************
 * This file includes functions that print SDC (Synopsys Design Constraint)
 * files in physical design tools, i.e., Place & Route (PnR) tools
 * The SDC files are used to constrain the physical design for each routing
 *modules in FPGA fabric, such as Switch Blocks (SBs) and Connection Blocks
 *(CBs)
 *
 * Note that this is different from the SDC to constrain VPR Place&Route
 * engine! These SDCs are designed for PnR to generate FPGA layouts!!!
 *******************************************************************/
#include <ctime>
#include <fstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "build_routing_module_utils.h"
#include "mux_utils.h"
#include "openfpga_digest.h"
#include "openfpga_naming.h"
#include "openfpga_port.h"
#include "openfpga_rr_graph_utils.h"
#include "openfpga_scale.h"
#include "openfpga_side_manager.h"
#include "pnr_sdc_routing_writer.h"
#include "sdc_writer_naming.h"
#include "sdc_writer_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Find the timing constraints between the inputs and outputs of a routing
 * multiplexer in a Switch Block
 *******************************************************************/
static float find_pnr_sdc_switch_tmax(const t_rr_switch_inf& switch_inf) {
  return switch_inf.R * switch_inf.Cout + switch_inf.Tdel;
}

/********************************************************************
 * Set timing constraints between the inputs and outputs of a routing
 * multiplexer in a Switch Block
 *******************************************************************/
static void print_pnr_sdc_constrain_sb_mux_timing(
  std::fstream& fp, const float& time_unit, const bool& hierarchical,
  const std::string& module_path, const ModuleManager& module_manager,
  const ModuleId& sb_module, const VprDeviceAnnotation& device_annotation,
  const DeviceGrid& grids, const RRGraphView& rr_graph, const RRGSB& rr_gsb,
  const e_side& output_node_side, const RRNodeId& output_rr_node,
  const bool& constrain_zero_delay_paths) {
  /* Validate file stream */
  valid_file_stream(fp);

  VTR_ASSERT((CHANX == rr_graph.node_type(output_rr_node)) ||
             (CHANY == rr_graph.node_type(output_rr_node)));

  /* Find the module port corresponding to the output rr_node */
  ModulePinInfo module_output_port = find_switch_block_module_chan_port(
    module_manager, sb_module, rr_graph, rr_gsb, output_node_side,
    output_rr_node, OUT_PORT);

  /* Find the module port corresponding to the fan-in rr_nodes of the output
   * rr_node */
  std::vector<ModulePinInfo> module_input_ports =
    find_switch_block_module_input_ports(
      module_manager, sb_module, grids, device_annotation, rr_graph, rr_gsb,
      get_rr_graph_configurable_driver_nodes(rr_graph, output_rr_node));

  /* Find timing constraints for each path (edge) */
  std::map<ModulePinInfo, float> switch_delays;
  size_t edge_counter = 0;
  for (const RREdgeId& edge :
       rr_graph.node_configurable_in_edges(output_rr_node)) {
    /* Get the switch delay */
    const RRSwitchId& driver_switch = rr_graph.edge_switch(edge);
    switch_delays[module_input_ports[edge_counter]] =
      find_pnr_sdc_switch_tmax(rr_graph.rr_switch_inf(driver_switch));
    edge_counter++;
  }

  /* Find the starting points */
  for (const ModulePinInfo& module_input_port : module_input_ports) {
    /* If we have a zero-delay path to contrain, we will skip unless users want
     * so */
    if ((false == constrain_zero_delay_paths) &&
        (0. == switch_delays[module_input_port])) {
      continue;
    }

    BasicPort src_port(
      module_manager.module_port(sb_module, module_input_port.first).get_name(),
      module_input_port.second, module_input_port.second);

    BasicPort sink_port(
      module_manager.module_port(sb_module, module_output_port.first)
        .get_name(),
      module_output_port.second, module_output_port.second);

    /* Constrain a path */
    if (false == hierarchical) {
      print_pnr_sdc_constrain_max_delay(
        fp, module_path, generate_sdc_port(src_port), module_path,
        generate_sdc_port(sink_port),
        switch_delays[module_input_port] / time_unit);

    } else {
      VTR_ASSERT_SAFE(true == hierarchical);
      print_pnr_sdc_constrain_max_delay(
        fp, std::string(), generate_sdc_port(src_port), std::string(),
        generate_sdc_port(sink_port),
        switch_delays[module_input_port] / time_unit);
    }
  }
}

/********************************************************************
 * Set timing constraints between the inputs and outputs of SBs,
 * which are connected by routing multiplexers with the given delays
 * specified in architectural XML file
 *
 * To enable block by block timing constraining, we generate the SDC
 * file for each unique SB module
 *******************************************************************/
static void print_pnr_sdc_constrain_sb_timing(
  const PnrSdcOption& options, const std::string& module_path,
  const ModuleManager& module_manager,
  const VprDeviceAnnotation& device_annotation, const DeviceGrid& grids,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb) {
  std::string sdc_dir = options.sdc_dir();
  float time_unit = options.time_unit();
  bool hierarchical = options.hierarchical();
  bool include_time_stamp = options.time_stamp();
  bool constrain_zero_delay_paths = options.constrain_zero_delay_paths();

  /* Create the file name for Verilog netlist */
  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  std::string sdc_fname(sdc_dir +
                        generate_switch_block_module_name(gsb_coordinate) +
                        std::string(SDC_FILE_NAME_POSTFIX));

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  /* Validate file stream */
  check_file_stream(sdc_fname.c_str(), fp);

  std::string sb_module_name =
    generate_switch_block_module_name(gsb_coordinate);
  ModuleId sb_module = module_manager.find_module(sb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

  /* Generate the descriptions*/
  print_sdc_file_header(fp,
                        std::string("Constrain timing of Switch Block " +
                                    sb_module_name + " for PnR"),
                        include_time_stamp);

  /* Print time unit for the SDC file */
  print_sdc_timescale(fp, time_unit_to_string(time_unit));

  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    SideManager side_manager(side);
    for (size_t itrack = 0;
         itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      const RRNodeId& chan_rr_node =
        rr_gsb.get_chan_node(side_manager.get_side(), itrack);
      /* We only care the output port and it should indicate a SB mux */
      if (OUT_PORT !=
          rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        continue;
      }
      /* Constrain thru wires */
      if (false != rr_gsb.is_sb_node_passing_wire(
                     rr_graph, side_manager.get_side(), itrack)) {
        continue;
      }
      /* This is a MUX, constrain all the paths from an input to an output */
      print_pnr_sdc_constrain_sb_mux_timing(
        fp, time_unit, hierarchical, module_path, module_manager, sb_module,
        device_annotation, grids, rr_graph, rr_gsb, side_manager.get_side(),
        chan_rr_node, constrain_zero_delay_paths);
    }
  }

  /* Close file handler */
  fp.close();
}

/********************************************************************
 * Print SDC timing constraints for Switch blocks
 * This function is designed for flatten routing hierarchy
 *******************************************************************/
void print_pnr_sdc_flatten_routing_constrain_sb_timing(
  const PnrSdcOption& options, const ModuleManager& module_manager,
  const ModuleId& top_module, const VprDeviceAnnotation& device_annotation,
  const DeviceGrid& grids, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb) {
  /* Start time count */
  vtr::ScopedStartFinishTimer timer(
    "Write SDC for constrain Switch Block timing for P&R flow");

  std::string root_path = module_manager.module_name(top_module);

  /* Get the range of SB array */
  vtr::Point<size_t> sb_range = device_rr_gsb.get_gsb_range();
  /* Go for each SB */
  for (size_t ix = 0; ix < sb_range.x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.y(); ++iy) {
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
      if (false == rr_gsb.is_sb_exist(rr_graph)) {
        continue;
      }

      vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
      std::string sb_instance_name =
        generate_switch_block_module_name(gsb_coordinate);

      ModuleId sb_module = module_manager.find_module(sb_instance_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

      std::string module_path = format_dir_path(root_path) + sb_instance_name;

      print_pnr_sdc_constrain_sb_timing(options, module_path, module_manager,
                                        device_annotation, grids, rr_graph,
                                        rr_gsb);
    }
  }
}

/********************************************************************
 * Print SDC timing constraints for Switch blocks
 * This function is designed for compact routing hierarchy
 *******************************************************************/
void print_pnr_sdc_compact_routing_constrain_sb_timing(
  const PnrSdcOption& options, const ModuleManager& module_manager,
  const ModuleId& top_module, const VprDeviceAnnotation& device_annotation,
  const DeviceGrid& grids, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb) {
  /* Start time count */
  vtr::ScopedStartFinishTimer timer(
    "Write SDC for constrain Switch Block timing for P&R flow");

  std::string root_path = module_manager.module_name(top_module);

  for (size_t isb = 0; isb < device_rr_gsb.get_num_sb_unique_module(); ++isb) {
    const RRGSB& rr_gsb = device_rr_gsb.get_sb_unique_module(isb);
    if (false == rr_gsb.is_sb_exist(rr_graph)) {
      continue;
    }

    /* Find all the sb instance under this module
     * Create a regular expression to include these instance names
     */
    vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
    std::string sb_module_name =
      generate_switch_block_module_name(gsb_coordinate);

    ModuleId sb_module = module_manager.find_module(sb_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

    std::string module_path = format_dir_path(root_path) + sb_module_name;

    print_pnr_sdc_constrain_sb_timing(options, module_path, module_manager,
                                      device_annotation, grids, rr_graph,
                                      rr_gsb);
  }
}

/********************************************************************
 * Set timing constraints between the inputs and outputs of a routing
 * multiplexer in a Connection Block
 *******************************************************************/
static void print_pnr_sdc_constrain_cb_mux_timing(
  std::fstream& fp, const float& time_unit, const bool& hierarchical,
  const std::string& module_path, const ModuleManager& module_manager,
  const ModuleId& cb_module, const VprDeviceAnnotation& device_annotation,
  const DeviceGrid& grids, const RRGraphView& rr_graph, const RRGSB& rr_gsb,
  const t_rr_type& cb_type, const RRNodeId& output_rr_node,
  const bool& constrain_zero_delay_paths) {
  /* Validate file stream */
  valid_file_stream(fp);

  VTR_ASSERT(IPIN == rr_graph.node_type(output_rr_node));

  /* We have OPINs since we may have direct connections:
   * These connections should be handled by other functions in the
   * compact_netlist.c So we just return here for OPINs
   */
  std::vector<RRNodeId> input_rr_nodes =
    get_rr_graph_configurable_driver_nodes(rr_graph, output_rr_node);

  if (0 == input_rr_nodes.size()) {
    return;
  }

  /* Xifan Tang: VPR considers delayless switch to be configurable
   * As a result, the direct connection is considered to be configurable...
   * Here, I simply kick out OPINs in CB connection because they should be built
   * in the top mopdule.
   *
   * Note: this MUST BE reconsidered if we do have OPIN connected to IPINs
   * through a programmable multiplexer!!!
   */
  if (true == is_ipin_direct_connected_opin(rr_graph, output_rr_node)) {
    return;
  }

  /* Find the module port corresponding to the output rr_node */
  ModulePortId module_output_port = find_connection_block_module_ipin_port(
    module_manager, cb_module, grids, device_annotation, rr_graph, rr_gsb,
    output_rr_node);

  /* Find the module port corresponding to the fan-in rr_nodes of the output
   * rr_node */
  std::vector<ModulePinInfo> module_input_ports =
    find_connection_block_module_input_ports(module_manager, cb_module, grids,
                                             device_annotation, rr_graph,
                                             rr_gsb, cb_type, input_rr_nodes);

  /* Find timing constraints for each path (edge) */
  std::map<ModulePinInfo, float> switch_delays;
  size_t edge_counter = 0;
  for (const RREdgeId& edge :
       rr_graph.node_configurable_in_edges(output_rr_node)) {
    /* Get the switch delay */
    const RRSwitchId& driver_switch = rr_graph.edge_switch(edge);
    switch_delays[module_input_ports[edge_counter]] =
      find_pnr_sdc_switch_tmax(rr_graph.rr_switch_inf(driver_switch));
    edge_counter++;
  }

  /* Find the starting points */
  for (const ModulePinInfo& module_input_port : module_input_ports) {
    /* If we have a zero-delay path to contrain, we will skip unless users want
     * so */
    if ((false == constrain_zero_delay_paths) &&
        (0. == switch_delays[module_input_port])) {
      continue;
    }

    BasicPort input_port(
      module_manager.module_port(cb_module, module_input_port.first).get_name(),
      module_input_port.second, module_input_port.second);
    BasicPort output_port =
      module_manager.module_port(cb_module, module_output_port);

    /* Constrain a path */
    if (true == hierarchical) {
      print_pnr_sdc_constrain_max_delay(
        fp, std::string(), generate_sdc_port(input_port), std::string(),
        generate_sdc_port(output_port),
        switch_delays[module_input_port] / time_unit);

    } else {
      VTR_ASSERT_SAFE(false == hierarchical);
      print_pnr_sdc_constrain_max_delay(
        fp, std::string(module_path), generate_sdc_port(input_port),
        std::string(module_path), generate_sdc_port(output_port),
        switch_delays[module_input_port] / time_unit);
    }
  }
}

/********************************************************************
 * Print SDC timing constraints for a Connection block
 * This function is designed for compact routing hierarchy
 *******************************************************************/
static void print_pnr_sdc_constrain_cb_timing(
  const PnrSdcOption& options, const std::string& module_path,
  const ModuleManager& module_manager,
  const VprDeviceAnnotation& device_annotation, const DeviceGrid& grids,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const t_rr_type& cb_type) {
  std::string sdc_dir = options.sdc_dir();
  float time_unit = options.time_unit();
  bool include_time_stamp = options.time_stamp();
  bool hierarchical = options.hierarchical();
  bool constrain_zero_delay_paths = options.constrain_zero_delay_paths();

  /* Create the netlist */
  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_cb_x(cb_type),
                                    rr_gsb.get_cb_y(cb_type));

  /* Find the module name and create a SDC file for it */
  std::string sdc_fname(
    sdc_dir + generate_connection_block_module_name(cb_type, gsb_coordinate) +
    std::string(SDC_FILE_NAME_POSTFIX));

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  /* Validate file stream */
  check_file_stream(sdc_fname.c_str(), fp);

  std::string cb_module_name =
    generate_connection_block_module_name(cb_type, gsb_coordinate);
  ModuleId cb_module = module_manager.find_module(cb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(cb_module));

  /* Generate the descriptions*/
  print_sdc_file_header(fp,
                        std::string("Constrain timing of Connection Block " +
                                    cb_module_name + " for PnR"),
                        include_time_stamp);

  /* Print time unit for the SDC file */
  print_sdc_timescale(fp, time_unit_to_string(time_unit));

  /* Contrain each routing track inside the connection block */
  for (size_t itrack = 0; itrack < rr_gsb.get_cb_chan_width(cb_type);
       ++itrack) {
    /* Create a port description for the input */
    std::string input_port_name =
      generate_cb_module_track_port_name(cb_type, IN_PORT, 0 == itrack % 2);
    ModulePortId input_port_id =
      module_manager.find_module_port(cb_module, input_port_name);
    BasicPort input_port(
      module_manager.module_port(cb_module, input_port_id).get_name(),
      itrack / 2, itrack / 2);

    /* Create a port description for the output */
    std::string output_port_name =
      generate_cb_module_track_port_name(cb_type, OUT_PORT, 0 == itrack % 2);
    ModulePortId output_port_id =
      module_manager.find_module_port(cb_module, output_port_name);
    BasicPort output_port(
      module_manager.module_port(cb_module, output_port_id).get_name(),
      itrack / 2, itrack / 2);

    /* Connection block routing segment ids for each track */
    RRSegmentId segment_id =
      rr_gsb.get_chan_node_segment(rr_gsb.get_cb_chan_side(cb_type), itrack);

    /* Computing the delay of the routing segment
     * Here we just assume a simple 1-level RC delay model
     * TODO: Should consider multi-level RC delay models
     *       where the number of levels are defined by users
     */
    float routing_segment_delay = rr_graph.rr_segments(segment_id).Rmetal *
                                  rr_graph.rr_segments(segment_id).Cmetal;

    /* If we have a zero-delay path to contrain, we will skip unless users want
     * so */
    if ((false == constrain_zero_delay_paths) &&
        (0. == routing_segment_delay)) {
      continue;
    }

    /* Constrain a path with routing segment delay */
    if (true == hierarchical) {
      print_pnr_sdc_constrain_max_delay(
        fp, std::string(), generate_sdc_port(input_port), std::string(),
        generate_sdc_port(output_port), routing_segment_delay / time_unit);

    } else {
      VTR_ASSERT_SAFE(false == hierarchical);
      print_pnr_sdc_constrain_max_delay(
        fp, std::string(module_path), generate_sdc_port(input_port),
        std::string(module_path), generate_sdc_port(output_port),
        routing_segment_delay / time_unit);
    }
  }

  /* Contrain each multiplexers inside the connection block */
  std::vector<enum e_side> cb_sides = rr_gsb.get_cb_ipin_sides(cb_type);

  for (size_t side = 0; side < cb_sides.size(); ++side) {
    enum e_side cb_ipin_side = cb_sides[side];
    SideManager side_manager(cb_ipin_side);
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side);
         ++inode) {
      const RRNodeId& ipin_rr_node = rr_gsb.get_ipin_node(cb_ipin_side, inode);
      print_pnr_sdc_constrain_cb_mux_timing(
        fp, time_unit, hierarchical, module_path, module_manager, cb_module,
        device_annotation, grids, rr_graph, rr_gsb, cb_type, ipin_rr_node,
        constrain_zero_delay_paths);
    }
  }

  /* Close file handler */
  fp.close();
}

/********************************************************************
 * Iterate over all the connection blocks in a device
 * and print SDC file for each of them
 *******************************************************************/
static void print_pnr_sdc_flatten_routing_constrain_cb_timing(
  const PnrSdcOption& options, const ModuleManager& module_manager,
  const ModuleId& top_module, const VprDeviceAnnotation& device_annotation,
  const DeviceGrid& grids, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb, const t_rr_type& cb_type) {
  /* Build unique X-direction connection block modules */
  vtr::Point<size_t> cb_range = device_rr_gsb.get_gsb_range();

  std::string root_path = module_manager.module_name(top_module);

  for (size_t ix = 0; ix < cb_range.x(); ++ix) {
    for (size_t iy = 0; iy < cb_range.y(); ++iy) {
      /* Check if the connection block exists in the device!
       * Some of them do NOT exist due to heterogeneous blocks (height > 1)
       * We will skip those modules
       */
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
      if (false == rr_gsb.is_cb_exist(cb_type)) {
        continue;
      }

      /* Find all the cb instance under this module
       * Create a regular expression to include these instance names
       */
      vtr::Point<size_t> gsb_coordinate(rr_gsb.get_cb_x(cb_type),
                                        rr_gsb.get_cb_y(cb_type));
      std::string cb_instance_name =
        generate_connection_block_module_name(cb_type, gsb_coordinate);
      ModuleId cb_module = module_manager.find_module(cb_instance_name);
      VTR_ASSERT(true == module_manager.valid_module_id(cb_module));

      std::string module_path = format_dir_path(root_path) + cb_instance_name;

      print_pnr_sdc_constrain_cb_timing(options, module_path, module_manager,
                                        device_annotation, grids, rr_graph,
                                        rr_gsb, cb_type);
    }
  }
}

/********************************************************************
 * Iterate over all the connection blocks in a device
 * and print SDC file for each of them
 *******************************************************************/
void print_pnr_sdc_flatten_routing_constrain_cb_timing(
  const PnrSdcOption& options, const ModuleManager& module_manager,
  const ModuleId& top_module, const VprDeviceAnnotation& device_annotation,
  const DeviceGrid& grids, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb) {
  /* Start time count */
  vtr::ScopedStartFinishTimer timer(
    "Write SDC for constrain Connection Block timing for P&R flow");

  print_pnr_sdc_flatten_routing_constrain_cb_timing(
    options, module_manager, top_module, device_annotation, grids, rr_graph,
    device_rr_gsb, CHANX);

  print_pnr_sdc_flatten_routing_constrain_cb_timing(
    options, module_manager, top_module, device_annotation, grids, rr_graph,
    device_rr_gsb, CHANY);
}

/********************************************************************
 * Print SDC timing constraints for Connection blocks
 * This function is designed for compact routing hierarchy
 *******************************************************************/
void print_pnr_sdc_compact_routing_constrain_cb_timing(
  const PnrSdcOption& options, const ModuleManager& module_manager,
  const ModuleId& top_module, const VprDeviceAnnotation& device_annotation,
  const DeviceGrid& grids, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb) {
  /* Start time count */
  vtr::ScopedStartFinishTimer timer(
    "Write SDC for constrain Connection Block timing for P&R flow");

  std::string root_path = module_manager.module_name(top_module);

  /* Print SDC for unique X-direction connection block modules */
  for (size_t icb = 0; icb < device_rr_gsb.get_num_cb_unique_module(CHANX);
       ++icb) {
    const RRGSB& unique_mirror = device_rr_gsb.get_cb_unique_module(CHANX, icb);

    /* Find all the cb instance under this module
     * Create a regular expression to include these instance names
     */
    vtr::Point<size_t> gsb_coordinate(unique_mirror.get_cb_x(CHANX),
                                      unique_mirror.get_cb_y(CHANX));
    std::string cb_module_name =
      generate_connection_block_module_name(CHANX, gsb_coordinate);
    ModuleId cb_module = module_manager.find_module(cb_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(cb_module));

    std::string module_path = format_dir_path(root_path) + cb_module_name;

    print_pnr_sdc_constrain_cb_timing(options, module_path, module_manager,
                                      device_annotation, grids, rr_graph,
                                      unique_mirror, CHANX);
  }

  /* Print SDC for unique Y-direction connection block modules */
  for (size_t icb = 0; icb < device_rr_gsb.get_num_cb_unique_module(CHANY);
       ++icb) {
    const RRGSB& unique_mirror = device_rr_gsb.get_cb_unique_module(CHANY, icb);

    /* Find all the cb instance under this module
     * Create a regular expression to include these instance names
     */
    vtr::Point<size_t> gsb_coordinate(unique_mirror.get_cb_x(CHANY),
                                      unique_mirror.get_cb_y(CHANY));
    std::string cb_module_name =
      generate_connection_block_module_name(CHANY, gsb_coordinate);
    ModuleId cb_module = module_manager.find_module(cb_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(cb_module));

    std::string module_path = format_dir_path(root_path) + cb_module_name;

    print_pnr_sdc_constrain_cb_timing(options, module_path, module_manager,
                                      device_annotation, grids, rr_graph,
                                      unique_mirror, CHANY);
  }
}

} /* end namespace openfpga */
