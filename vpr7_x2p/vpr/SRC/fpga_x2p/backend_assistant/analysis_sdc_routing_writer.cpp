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
#include "analysis_sdc_routing_writer.h"

#include "globals.h"

/********************************************************************
 * Identify if a node should be disabled during analysis SDC generation
 *******************************************************************/
static 
bool is_rr_node_to_be_disable_for_analysis(t_rr_node* cur_rr_node) {
  /* Conditions to enable timing analysis for a node 
   * 1st condition: it have a valid vpack_net_number 
   * 2nd condition: it is not an parasitic net 
   * 3rd condition: it is not a global net
   */
  if ( (OPEN != cur_rr_node->vpack_net_num) 
    && (FALSE == cur_rr_node->is_parasitic_net)
    && (FALSE == vpack_net[cur_rr_node->vpack_net_num].is_global)
    && (FALSE == vpack_net[cur_rr_node->vpack_net_num].is_const_gen) ){
    return false;
  }
  return true;
}

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
    std::string port_name = generate_routing_track_port_name(cb_type,
                                                             port_coord, itrack,  
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
    std::string port_name = generate_routing_track_port_name(cb_type,
                                                             port_coord, itrack,  
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
      if (false == is_rr_node_to_be_disable_for_analysis(ipin_node)) {
        continue;
      }
      if (0 == ipin_node->fan_in) {
        continue;
      }

      /* Find the MUX instance that drives the IPIN! */
      std::string mux_instance_name = generate_cb_mux_instance_name(CONNECTION_BLOCK_MUX_INSTANCE_PREFIX, rr_gsb.get_ipin_node_grid_side(cb_ipin_side, inode), inode, std::string(""));
      mux_instance_to_net_map[mux_instance_name] = ipin_node->vpack_net_num;  

      vtr::Point<size_t> port_coord(ipin_node->xlow, ipin_node->ylow);
      std::string port_name = generate_grid_side_port_name(grids,
                                                           port_coord,
                                                           rr_gsb.get_ipin_node_grid_side(cb_ipin_side, inode),
                                                           ipin_node->ptc_num); 

      /* Find the port in unique mirror! */
      if (true == compact_routing_hierarchy) {
        /* Note: use GSB coordinate when inquire for unique modules!!! */
        DeviceCoordinator cb_coord(rr_gsb.get_x(), rr_gsb.get_y());
        const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(cb_type, cb_coord);
        t_rr_node* unique_mirror_ipin_node = unique_mirror.get_ipin_node(cb_ipin_side, inode);
        port_coord.set_x(unique_mirror_ipin_node->xlow);
        port_coord.set_y(unique_mirror_ipin_node->ylow);
        port_name = generate_grid_side_port_name(grids,
                                                 port_coord,
                                                 unique_mirror.get_ipin_node_grid_side(cb_ipin_side, inode),
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
    std::string port_name = generate_routing_track_port_name(cb_type,
                                                             port_coord, itrack,  
                                                             OUT_PORT);

    /* Ensure we have this port in the module! */
    ModulePortId module_port = module_manager.find_module_port(cb_module, port_name);
    VTR_ASSERT(true == module_manager.valid_module_port_id(cb_module, module_port));

    /* Find the module net which sources from this port! */
    for (const size_t& pin : module_manager.module_port(cb_module, module_port).pins()) {
      ModuleNetId module_net = module_manager.module_instance_port_net(cb_module, cb_module, 0, module_port, pin); 
      VTR_ASSERT(true == module_manager.valid_module_net_id(cb_module, module_net));

      /* Touch each sink of the net! */
      for (const ModuleNetSinkId& sink_id : module_manager.module_net_sinks(cb_module, module_net)) {
        ModuleId sink_module = module_manager.net_sink_modules(cb_module, module_net)[sink_id]; 
        size_t sink_instance = module_manager.net_sink_instances(cb_module, module_net)[sink_id]; 

        /* Skip when sink module is the cb module, 
         * the output ports of cb modules have been disabled/enabled already! 
         */
        if (sink_module == cb_module) {
          continue;
        }

        std::string sink_instance_name = module_manager.instance_name(cb_module, sink_module, sink_instance);
        bool disable_timing = false;
        /* Check if this node is used by benchmark  */
        if (true == is_rr_node_to_be_disable_for_analysis(chan_node)) {
          /* Disable all the sinks! */
          disable_timing = true;
        } else {
          /* See if the net id matches. If does not match, we should disable! */
          if (chan_node->vpack_net_num != mux_instance_to_net_map[sink_instance_name]) {
            disable_timing = true;
          }
        }

        /* Time to write SDC command to disable timing or not */
        if (false == disable_timing) {
          continue;
        }

        BasicPort sink_port = module_manager.module_port(sink_module, module_manager.net_sink_ports(cb_module, module_net)[sink_id]);
        sink_port.set_width(module_manager.net_sink_pins(cb_module, module_net)[sink_id],
                            module_manager.net_sink_pins(cb_module, module_net)[sink_id]);
        /* Get the input id that is used! Disable the unused inputs! */
        fp << "set_disable_timing ";
        fp << cb_instance_name << "/";
        fp << sink_instance_name << "/";
        fp << generate_sdc_port(sink_port);
        fp << std::endl;
      }
    }
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

