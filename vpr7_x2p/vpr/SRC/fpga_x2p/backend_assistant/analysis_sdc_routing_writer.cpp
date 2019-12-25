/********************************************************************
 * This file includes functions that are used to output a SDC file
 * that constrain routing modules of a FPGA fabric (P&Red netlist) 
 * using a benchmark 
 *******************************************************************/
#include <map>

#include "vtr_assert.h"
#include "device_port.h"

#include "fpga_x2p_reserved_words.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_types.h"

#include "sdc_writer_utils.h"
#include "analysis_sdc_writer_utils.h"
#include "analysis_sdc_routing_writer.h"

#include "globals.h"

/********************************************************************
 * This function will disable 
 * 1. all the unused port (unmapped by a benchmark) of a connection block
 * 2. all the unused inputs (unmapped by a benchmark) of routing multiplexers 
 *    in a connection block
 *******************************************************************/
static 
void print_analysis_sdc_disable_cb_unused_resources(std::fstream& fp, 
                                                    const std::vector<std::vector<t_grid_tile>>& grids,
                                                    const ModuleManager& module_manager, 
                                                    const DeviceRRGSB& L_device_rr_gsb,
                                                    const RRGSB& rr_gsb, 
                                                    const t_rr_type& cb_type,
                                                    const bool& compact_routing_hierarchy) {
  /* Validate file stream */
  check_file_handler(fp);

  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));

  std::string cb_instance_name = generate_connection_block_module_name(cb_type, gsb_coordinate);

  /* If we use the compact routing hierarchy, we need to find the module name !*/
  vtr::Point<size_t> cb_coordinate(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
  if (true == compact_routing_hierarchy) {
    DeviceCoordinator cb_coord(rr_gsb.get_x(), rr_gsb.get_y());
    /* Note: use GSB coordinate when inquire for unique modules!!! */
    const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(cb_type, cb_coord);
    cb_coordinate.set_x(unique_mirror.get_cb_x(cb_type)); 
    cb_coordinate.set_y(unique_mirror.get_cb_y(cb_type)); 
  }

  std::string cb_module_name = generate_connection_block_module_name(cb_type, cb_coordinate);

  ModuleId cb_module = module_manager.find_module(cb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(cb_module));

  /* Print comments */
  fp << "##################################################" << std::endl; 
  fp << "# Disable timing for Connection block " << cb_module_name << std::endl;
  fp << "##################################################" << std::endl; 

  /* Disable all the input port (routing tracks), which are not used by benchmark */
  for (size_t itrack = 0; itrack < rr_gsb.get_cb_chan_width(cb_type); ++itrack) {
    t_rr_node* chan_node = rr_gsb.get_chan_node(rr_gsb.get_cb_chan_side(cb_type), itrack);
    /* Check if this node is used by benchmark  */
    if (false == is_rr_node_to_be_disable_for_analysis(chan_node)) {
      continue;
    }

    /* Disable both input of the routing track if it is not used! */
    vtr::Point<size_t> port_coord(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
    if (true == compact_routing_hierarchy) {
      /* Note: use GSB coordinate when inquire for unique modules!!! */
      DeviceCoordinator cb_coord(rr_gsb.get_x(), rr_gsb.get_y());
      const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(cb_type, cb_coord);
      port_coord.set_x(unique_mirror.get_cb_x(cb_type));
      port_coord.set_y(unique_mirror.get_cb_y(cb_type));
    }
    std::string port_name = generate_cb_module_track_port_name(cb_type,
                                                               itrack,  
                                                               IN_PORT);

    /* Ensure we have this port in the module! */
    ModulePortId module_port = module_manager.find_module_port(cb_module, port_name);
    VTR_ASSERT(true == module_manager.valid_module_port_id(cb_module, module_port));

    fp << "set_disable_timing ";
    fp << cb_instance_name << "/";
    fp << generate_sdc_port(module_manager.module_port(cb_module, module_port));
    fp << std::endl;
  }

  /* Disable all the output port (routing tracks), which are not used by benchmark */
  for (size_t itrack = 0; itrack < rr_gsb.get_cb_chan_width(cb_type); ++itrack) {
    t_rr_node* chan_node = rr_gsb.get_chan_node(rr_gsb.get_cb_chan_side(cb_type), itrack);
    /* Check if this node is used by benchmark  */
    if (false == is_rr_node_to_be_disable_for_analysis(chan_node)) {
      continue;
    }

    /* Disable both input of the routing track if it is not used! */
    vtr::Point<size_t> port_coord(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
    if (true == compact_routing_hierarchy) {
      /* Note: use GSB coordinate when inquire for unique modules!!! */
      DeviceCoordinator cb_coord(rr_gsb.get_x(), rr_gsb.get_y());
      const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(cb_type, cb_coord);
      port_coord.set_x(unique_mirror.get_cb_x(cb_type));
      port_coord.set_y(unique_mirror.get_cb_y(cb_type));
    }
    std::string port_name = generate_cb_module_track_port_name(cb_type,
                                                               itrack,  
                                                               OUT_PORT);

    /* Ensure we have this port in the module! */
    ModulePortId module_port = module_manager.find_module_port(cb_module, port_name);
    VTR_ASSERT(true == module_manager.valid_module_port_id(cb_module, module_port));

    fp << "set_disable_timing ";
    fp << cb_instance_name << "/";
    fp << generate_sdc_port(module_manager.module_port(cb_module, module_port));
    fp << std::endl;
  }

  /* Build a map between mux_instance name and net_num */
  std::map<std::string, int> mux_instance_to_net_map;

  /* Disable all the output port (grid input pins), which are not used by benchmark */
  std::vector<enum e_side> cb_sides = rr_gsb.get_cb_ipin_sides(cb_type);

  for (size_t side = 0; side < cb_sides.size(); ++side) {
    enum e_side cb_ipin_side = cb_sides[side];
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side); ++inode) {
      t_rr_node* ipin_node = rr_gsb.get_ipin_node(cb_ipin_side, inode);

      /* Find the MUX instance that drives the IPIN! */
      std::string mux_instance_name = generate_cb_mux_instance_name(CONNECTION_BLOCK_MUX_INSTANCE_PREFIX, rr_gsb.get_ipin_node_grid_side(cb_ipin_side, inode), inode, std::string(""));
      mux_instance_to_net_map[mux_instance_name] = ipin_node->vpack_net_num;  

      if (false == is_rr_node_to_be_disable_for_analysis(ipin_node)) {
        continue;
      }
      if (0 == ipin_node->fan_in) {
        continue;
      }

      vtr::Point<size_t> port_coord(ipin_node->xlow, ipin_node->ylow);
      std::string port_name = generate_cb_module_grid_port_name(cb_ipin_side,
                                                                ipin_node->ptc_num); 

      /* Find the port in unique mirror! */
      if (true == compact_routing_hierarchy) {
        /* Note: use GSB coordinate when inquire for unique modules!!! */
        DeviceCoordinator cb_coord(rr_gsb.get_x(), rr_gsb.get_y());
        const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(cb_type, cb_coord);
        t_rr_node* unique_mirror_ipin_node = unique_mirror.get_ipin_node(cb_ipin_side, inode);
        port_coord.set_x(unique_mirror_ipin_node->xlow);
        port_coord.set_y(unique_mirror_ipin_node->ylow);
        port_name = generate_cb_module_grid_port_name(cb_ipin_side, 
                                                      unique_mirror_ipin_node->ptc_num); 
      }

      /* Ensure we have this port in the module! */
      ModulePortId module_port = module_manager.find_module_port(cb_module, port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(cb_module, module_port));

      fp << "set_disable_timing ";
      fp << cb_instance_name << "/";
      fp << generate_sdc_port(module_manager.module_port(cb_module, module_port));
      fp << std::endl;
    }
  }

  /* Disable all the unused inputs of routing multiplexers, which are not used by benchmark 
   * Here, we start from each input of the Connection Blocks, and traverse forward to the sink 
   * port of the module net whose source is the input
   * We will find the instance name which is the parent of the sink port, and search the 
   * net id through the instance_name_to_net_map 
   * The the net id does not match the net id of this input, we will disable the sink port!
   *
   *                   cb_module
   *                 +-----------------------
   *                 |           MUX instance A
   *                 |          +-----------
   *   input_port--->|--+---x-->| sink port (disable!)
   *                 |  |       +----------
   *                 |  |        MUX instance B
   *                 |  |       +----------
   *                 |  +------>| sink port (do not disable!)
   */ 
  for (size_t itrack = 0; itrack < rr_gsb.get_cb_chan_width(cb_type); ++itrack) {
    t_rr_node* chan_node = rr_gsb.get_chan_node(rr_gsb.get_cb_chan_side(cb_type), itrack);

    /* Disable both input of the routing track if it is not used! */
    vtr::Point<size_t> port_coord(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
    if (true == compact_routing_hierarchy) {
      /* Note: use GSB coordinate when inquire for unique modules!!! */
      DeviceCoordinator cb_coord(rr_gsb.get_x(), rr_gsb.get_y());
      const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(cb_type, cb_coord);
      port_coord.set_x(unique_mirror.get_cb_x(cb_type));
      port_coord.set_y(unique_mirror.get_cb_y(cb_type));
    }
    std::string port_name = generate_cb_module_track_port_name(cb_type,
                                                               itrack,  
                                                               OUT_PORT);

    /* Ensure we have this port in the module! */
    ModulePortId module_port = module_manager.find_module_port(cb_module, port_name);
    VTR_ASSERT(true == module_manager.valid_module_port_id(cb_module, module_port));

    disable_analysis_module_input_port_net_sinks(fp, 
                                                 module_manager, cb_module,
                                                 cb_instance_name,
                                                 module_port,
                                                 chan_node,
                                                 mux_instance_to_net_map);
  }
}

/********************************************************************
 * Iterate over all the connection blocks in a device
 * and disable unused ports for each of them 
 *******************************************************************/
static 
void print_analysis_sdc_disable_unused_cb_ports(std::fstream& fp,
                                                const std::vector<std::vector<t_grid_tile>>& grids,
                                                const ModuleManager& module_manager, 
                                                const DeviceRRGSB& L_device_rr_gsb,
                                                const t_rr_type& cb_type,
                                                const bool& compact_routing_hierarchy) {
  /* Build unique X-direction connection block modules */
  DeviceCoordinator cb_range = L_device_rr_gsb.get_gsb_range();

  for (size_t ix = 0; ix < cb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < cb_range.get_y(); ++iy) {
      /* Check if the connection block exists in the device!
       * Some of them do NOT exist due to heterogeneous blocks (height > 1) 
       * We will skip those modules
       */
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      if (false == rr_gsb.is_cb_exist(cb_type)) {
        continue;
      }

      print_analysis_sdc_disable_cb_unused_resources(fp, grids, 
                                                     module_manager, 
                                                     L_device_rr_gsb, 
                                                     rr_gsb, 
                                                     cb_type,
                                                     compact_routing_hierarchy);
    }
  }
}

/********************************************************************
 * Iterate over all the connection blocks in a device
 * and disable unused ports for each of them 
 *******************************************************************/
void print_analysis_sdc_disable_unused_cbs(std::fstream& fp,
                                           const std::vector<std::vector<t_grid_tile>>& grids,
                                           const ModuleManager& module_manager, 
                                           const DeviceRRGSB& L_device_rr_gsb,
                                           const bool& compact_routing_hierarchy) {

  print_analysis_sdc_disable_unused_cb_ports(fp, grids, module_manager, 
                                             L_device_rr_gsb,
                                             CHANX, compact_routing_hierarchy);

  print_analysis_sdc_disable_unused_cb_ports(fp, grids, module_manager, 
                                             L_device_rr_gsb,
                                             CHANY, compact_routing_hierarchy);
}

/********************************************************************
 * This function will disable 
 * 1. all the unused port (unmapped by a benchmark) of a switch block
 * 2. all the unused inputs (unmapped by a benchmark) of routing multiplexers 
 *    in a switch block
 *******************************************************************/
static 
void print_analysis_sdc_disable_sb_unused_resources(std::fstream& fp, 
                                                    const std::vector<std::vector<t_grid_tile>>& grids,
                                                    const ModuleManager& module_manager, 
                                                    const DeviceRRGSB& L_device_rr_gsb,
                                                    const RRGSB& rr_gsb, 
                                                    const bool& compact_routing_hierarchy) {
  /* Validate file stream */
  check_file_handler(fp);

  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());

  std::string sb_instance_name = generate_switch_block_module_name(gsb_coordinate);

  /* If we use the compact routing hierarchy, we need to find the module name !*/
  vtr::Point<size_t> sb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  if (true == compact_routing_hierarchy) {
    DeviceCoordinator sb_coord(rr_gsb.get_x(), rr_gsb.get_y());
    /* Note: use GSB coordinate when inquire for unique modules!!! */
    const RRGSB& unique_mirror = L_device_rr_gsb.get_sb_unique_module(sb_coord);
    sb_coordinate.set_x(unique_mirror.get_sb_x()); 
    sb_coordinate.set_y(unique_mirror.get_sb_y()); 
  }

  std::string sb_module_name = generate_switch_block_module_name(sb_coordinate);

  ModuleId sb_module = module_manager.find_module(sb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

  /* Print comments */
  fp << "##################################################" << std::endl; 
  fp << "# Disable timing for Switch block " << sb_module_name << std::endl;
  fp << "##################################################" << std::endl; 

  /* Build a map between mux_instance name and net_num */
  std::map<std::string, int> mux_instance_to_net_map;

  /* Disable all the input/output port (routing tracks), which are not used by benchmark */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    DeviceCoordinator port_coordinate = rr_gsb.get_side_block_coordinator(side_manager.get_side()); 

    for (size_t itrack = 0; itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      t_rr_node* chan_node = rr_gsb.get_chan_node(side_manager.get_side(), itrack);

      vtr::Point<size_t> port_coord(port_coordinate.get_x(), port_coordinate.get_y());
      std::string port_name = generate_sb_module_track_port_name(rr_gsb.get_chan_node(side_manager.get_side(), itrack)->type,
                                                                 side_manager.get_side(), itrack,  
                                                                 rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack));

      if (true == compact_routing_hierarchy) {
        /* Note: use GSB coordinate when inquire for unique modules!!! */
        DeviceCoordinator sb_coord(rr_gsb.get_x(), rr_gsb.get_y());
        const RRGSB& unique_mirror = L_device_rr_gsb.get_sb_unique_module(sb_coord);
        DeviceCoordinator unique_port_coordinate = unique_mirror.get_side_block_coordinator(side_manager.get_side()); 
        port_coord.set_x(unique_port_coordinate.get_x());
        port_coord.set_y(unique_port_coordinate.get_y());
        port_name = generate_sb_module_track_port_name(unique_mirror.get_chan_node(side_manager.get_side(), itrack)->type,
                                                       side_manager.get_side(), itrack,  
                                                       unique_mirror.get_chan_node_direction(side_manager.get_side(), itrack));
      }

      /* Ensure we have this port in the module! */
      ModulePortId module_port = module_manager.find_module_port(sb_module, port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(sb_module, module_port));

      /* Cache the net name for routing tracks which are outputs of the switch block */
      if (OUT_PORT == rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        /* Generate the name of mux instance related to this output node */
        std::string mux_instance_name = generate_sb_memory_instance_name(SWITCH_BLOCK_MUX_INSTANCE_PREFIX, side_manager.get_side(), itrack, std::string(""));
        mux_instance_to_net_map[mux_instance_name] = chan_node->vpack_net_num;
      }

      /* Check if this node is used by benchmark  */
      if (false == is_rr_node_to_be_disable_for_analysis(chan_node)) {
        continue;
      }

      fp << "set_disable_timing ";
      fp << sb_instance_name << "/";
      fp << generate_sdc_port(module_manager.module_port(sb_module, module_port));
      fp << std::endl;
    }
  }

  /* Disable all the input port (grid output pins), which are not used by benchmark */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);

    for (size_t inode = 0; inode < rr_gsb.get_num_opin_nodes(side_manager.get_side()); ++inode) {
      t_rr_node* opin_node = rr_gsb.get_opin_node(side_manager.get_side(), inode);
      vtr::Point<size_t> port_coord(rr_gsb.get_opin_node(side_manager.get_side(), inode)->xlow,
                                    rr_gsb.get_opin_node(side_manager.get_side(), inode)->ylow);

      std::string port_name = generate_sb_module_grid_port_name(side_manager.get_side(), 
                                                                rr_gsb.get_opin_node_grid_side(side_manager.get_side(), inode),
                                                                opin_node->ptc_num); 

      if (true == compact_routing_hierarchy) {
        /* Note: use GSB coordinate when inquire for unique modules!!! */
        DeviceCoordinator sb_coord(rr_gsb.get_x(), rr_gsb.get_y());
        const RRGSB& unique_mirror = L_device_rr_gsb.get_sb_unique_module(sb_coord);
        port_coord.set_x(unique_mirror.get_opin_node(side_manager.get_side(), inode)->xlow);
        port_coord.set_y(unique_mirror.get_opin_node(side_manager.get_side(), inode)->ylow);

        port_name = generate_sb_module_grid_port_name(side_manager.get_side(),
                                                      unique_mirror.get_opin_node_grid_side(side_manager.get_side(), inode),
                                                      unique_mirror.get_opin_node(side_manager.get_side(), inode)->ptc_num); 
      }


      /* Ensure we have this port in the module! */
      ModulePortId module_port = module_manager.find_module_port(sb_module, port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(sb_module, module_port));

      /* Check if this node is used by benchmark  */
      if (false == is_rr_node_to_be_disable_for_analysis(opin_node)) {
        continue;
      }

      fp << "set_disable_timing ";
      fp << sb_instance_name << "/";
      fp << generate_sdc_port(module_manager.module_port(sb_module, module_port));
      fp << std::endl;
    }
  }

  /* Disable all the unused inputs of routing multiplexers, which are not used by benchmark 
   * Here, we start from each input of the Switch Blocks, and traverse forward to the sink 
   * port of the module net whose source is the input
   * We will find the instance name which is the parent of the sink port, and search the 
   * net id through the instance_name_to_net_map 
   * The the net id does not match the net id of this input, we will disable the sink port!
   *
   *                   sb_module
   *                 +-----------------------
   *                 |           MUX instance A
   *                 |          +-----------
   *   input_port--->|--+---x-->| sink port (disable! net_id = Y) 
   *   (net_id = X)  |  |       +----------
   *                 |  |        MUX instance B
   *                 |  |       +----------
   *                 |  +------>| sink port (do not disable! net_id = X)
   *
   * Because the input ports of a SB module come from 
   * 1. Grid output pins
   * 2. routing tracks
   * We will walk through these ports and do conditionally disable_timing
   */ 

  /* Iterate over input ports coming from grid output pins */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);

    for (size_t inode = 0; inode < rr_gsb.get_num_opin_nodes(side_manager.get_side()); ++inode) {
      t_rr_node* opin_node = rr_gsb.get_opin_node(side_manager.get_side(), inode);
      vtr::Point<size_t> port_coord(rr_gsb.get_opin_node(side_manager.get_side(), inode)->xlow,
                                    rr_gsb.get_opin_node(side_manager.get_side(), inode)->ylow);

      std::string port_name = generate_sb_module_grid_port_name(side_manager.get_side(),
                                                                rr_gsb.get_opin_node_grid_side(side_manager.get_side(), inode),
                                                                opin_node->ptc_num); 

      if (true == compact_routing_hierarchy) {
        /* Note: use GSB coordinate when inquire for unique modules!!! */
        DeviceCoordinator sb_coord(rr_gsb.get_x(), rr_gsb.get_y());
        const RRGSB& unique_mirror = L_device_rr_gsb.get_sb_unique_module(sb_coord);
        port_coord.set_x(unique_mirror.get_opin_node(side_manager.get_side(), inode)->xlow);
        port_coord.set_y(unique_mirror.get_opin_node(side_manager.get_side(), inode)->ylow);

        port_name = generate_sb_module_grid_port_name(side_manager.get_side(),
                                                      unique_mirror.get_opin_node_grid_side(side_manager.get_side(), inode),
                                                      unique_mirror.get_opin_node(side_manager.get_side(), inode)->ptc_num); 
      }


      /* Ensure we have this port in the module! */
      ModulePortId module_port = module_manager.find_module_port(sb_module, port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(sb_module, module_port));

      disable_analysis_module_input_port_net_sinks(fp, module_manager,
                                                   sb_module,
                                                   sb_instance_name,
                                                   module_port,
                                                   opin_node,
                                                   mux_instance_to_net_map);
    }
  }

  /* Iterate over input ports coming from routing tracks */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    DeviceCoordinator port_coordinate = rr_gsb.get_side_block_coordinator(side_manager.get_side()); 

    for (size_t itrack = 0; itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      /* Skip output ports, they have already been disabled or not */
      if (OUT_PORT == rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        continue;
      }

      t_rr_node* chan_node = rr_gsb.get_chan_node(side_manager.get_side(), itrack);

      vtr::Point<size_t> port_coord(port_coordinate.get_x(), port_coordinate.get_y());
      std::string port_name = generate_sb_module_track_port_name(rr_gsb.get_chan_node(side_manager.get_side(), itrack)->type,
                                                                 side_manager.get_side(), itrack,  
                                                                 rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack));

      if (true == compact_routing_hierarchy) {
        /* Note: use GSB coordinate when inquire for unique modules!!! */
        DeviceCoordinator sb_coord(rr_gsb.get_x(), rr_gsb.get_y());
        const RRGSB& unique_mirror = L_device_rr_gsb.get_sb_unique_module(sb_coord);
        DeviceCoordinator unique_port_coordinate = unique_mirror.get_side_block_coordinator(side_manager.get_side()); 
        port_coord.set_x(unique_port_coordinate.get_x());
        port_coord.set_y(unique_port_coordinate.get_y());

        port_name = generate_sb_module_track_port_name(unique_mirror.get_chan_node(side_manager.get_side(), itrack)->type,
                                                       side_manager.get_side(), itrack,  
                                                       unique_mirror.get_chan_node_direction(side_manager.get_side(), itrack));
      }


      /* Ensure we have this port in the module! */
      ModulePortId module_port = module_manager.find_module_port(sb_module, port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(sb_module, module_port));

      disable_analysis_module_input_port_net_sinks(fp, module_manager,
                                                   sb_module,
                                                   sb_instance_name,
                                                   module_port,
                                                   chan_node,
                                                   mux_instance_to_net_map);
    }
  }
}


/********************************************************************
 * Iterate over all the connection blocks in a device
 * and disable unused ports for each of them 
 *******************************************************************/
void print_analysis_sdc_disable_unused_sbs(std::fstream& fp,
                                           const std::vector<std::vector<t_grid_tile>>& grids,
                                           const ModuleManager& module_manager, 
                                           const DeviceRRGSB& L_device_rr_gsb,
                                           const bool& compact_routing_hierarchy) {

  /* Build unique X-direction connection block modules */
  DeviceCoordinator sb_range = L_device_rr_gsb.get_gsb_range();

  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      /* Check if the connection block exists in the device!
       * Some of them do NOT exist due to heterogeneous blocks (height > 1) 
       * We will skip those modules
       */
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);

      print_analysis_sdc_disable_sb_unused_resources(fp, grids, 
                                                     module_manager, 
                                                     L_device_rr_gsb, 
                                                     rr_gsb, 
                                                     compact_routing_hierarchy);
    }
  }
}

