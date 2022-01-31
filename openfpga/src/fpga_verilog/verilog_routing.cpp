/*********************************************************************
 * This file includes functions that are used for 
 * Verilog generation of FPGA routing architecture (global routing) 
 *********************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

/* Include FPGA-Verilog header files*/
#include "openfpga_naming.h"
#include "verilog_constants.h"
#include "verilog_writer_utils.h"
#include "verilog_module_writer.h"
#include "verilog_routing.h"

/* begin namespace openfpga */
namespace openfpga {

/******************************************************************** 
 * Print the sub-circuit of a connection Box (Type: [CHANX|CHANY])
 * Actually it is very similiar to switch box but
 * the difference is connection boxes connect Grid INPUT Pins to channels
 * NOTE: direct connection between CLBs should NOT be included inside this
 *       module! They should be added in the top-level module as their connection
 *       is not limited to adjacent CLBs!!!
 *
 * Location of a X- and Y-direction Connection Block in FPGA fabric
 *               +------------+       +-------------+ 
 *               |            |------>|             |
 *               |     CLB    |<------| Y-direction |
 *               |            | ...   |  Connection |
 *               |            |------>|    Block    |
 *               +------------+       +-------------+
 *                 |  ^ ... |            | ^ ... |
 *                 v  |     v            v |     v
 *           +-------------------+    +-------------+
 *       --->|                   |--->|             |
 *       <---|     X-direction   |<---|    Switch   |                           
 *        ...|  Connection block |... |    Block    |
 *       --->|                   |--->|             |
 *           +-------------------+    +-------------+
 *
 *  Internal structure:
 *  This is an example of a X-direction connection block  
 *  Note that middle output ports are shorted wire from inputs of routing tracks, 
 *  which are also the inputs of routing multiplexer of the connection block 
 *
 *                      CLB Input Pins
 *                         (IPINs)
 *                       ^   ^     ^
 *                       |   | ... |
 *              +--------------------------+
 *              |       ^    ^     ^       |
 *              |       |    | ... |       |   
 *              |  +--------------------+  | 
 *              |  |       routing      |  |
 *              |  |    multiplexers    |  |
 *              |  +--------------------+  | 
 *              |      middle outputs      |
 *              |    of routing channel    |
 *              |    ^ ^ ^ ^     ^ ^ ^ ^   |
 *              |    | | | | ... | | | |   |
 *     in[0] -->|------------------------->|---> out[0]
 *    out[1] <--|<-------------------------|<--- in[1]
 *              |           ...            |
 *   in[W-2] -->|------------------------->|---> out[W-2]
 *  out[W-1] <--|<-------------------------|<--- in[W-1]
 *              +--------------------------+
 *
 *  W: routing channel width
 *              
 ********************************************************************/
static 
void print_verilog_routing_connection_box_unique_module(NetlistManager& netlist_manager,
                                                        const ModuleManager& module_manager, 
                                                        const std::string& subckt_dir, 
                                                        const std::string& subckt_dir_name, 
                                                        const RRGSB& rr_gsb,
                                                        const t_rr_type& cb_type,
                                                        const FabricVerilogOption& options) {
  /* Create the netlist */
  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
  std::string verilog_fname(generate_connection_block_netlist_name(cb_type, gsb_coordinate, std::string(VERILOG_NETLIST_FILE_POSTFIX)));
  std::string verilog_fpath(subckt_dir + verilog_fname);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fpath, std::fstream::out | std::fstream::trunc);

  check_file_stream(verilog_fpath.c_str(), fp);

  print_verilog_file_header(fp,
                            std::string("Verilog modules for Unique Connection Blocks[" + std::to_string(rr_gsb.get_cb_x(cb_type)) + "]["+ std::to_string(rr_gsb.get_cb_y(cb_type)) + "]"),
                            options.time_stamp()); 

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId cb_module = module_manager.find_module(generate_connection_block_module_name(cb_type, gsb_coordinate)); 
  VTR_ASSERT(true == module_manager.valid_module_id(cb_module));

  /* Write the verilog module */
  write_verilog_module_to_file(fp,
                               module_manager, cb_module,
                               options.explicit_port_mapping(),
                               options.default_net_type());
 
  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Close file handler */
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = NetlistId::INVALID();
  if (options.use_relative_path()) {
    nlist_id = netlist_manager.add_netlist(subckt_dir_name + verilog_fname);
  } else {
    nlist_id = netlist_manager.add_netlist(verilog_fpath);
  }
  VTR_ASSERT(nlist_id);
  netlist_manager.set_netlist_type(nlist_id, NetlistManager::ROUTING_MODULE_NETLIST);
}

/*********************************************************************
 * Generate the Verilog module for a Switch Box.
 * A Switch Box module consists of following ports:
 * 1. Channel Y [x][y] inputs 
 * 2. Channel X [x+1][y] inputs
 * 3. Channel Y [x][y-1] outputs
 * 4. Channel X [x][y] outputs
 * 5. Grid[x][y+1] Right side outputs pins
 * 6. Grid[x+1][y+1] Left side output pins
 * 7. Grid[x+1][y+1] Bottom side output pins
 * 8. Grid[x+1][y] Top side output pins
 * 9. Grid[x+1][y] Left side output pins
 * 10. Grid[x][y] Right side output pins
 * 11. Grid[x][y] Top side output pins
 * 12. Grid[x][y+1] Bottom side output pins
 *
 * Location of a Switch Box in FPGA fabric:
 *
 *    --------------          --------------
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |  [x][y+1]  | [x][y+1] | [x+1][y+1] |
 *    |            |          |            |
 *    --------------          --------------
 *                  ----------
 *       ChanX      | Switch |     ChanX 
 *       [x][y]     |   Box  |    [x+1][y]
 *                  | [x][y] |
 *                  ----------
 *    --------------          --------------
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |   [x][y]   |  [x][y]  |  [x+1][y]  |
 *    |            |          |            |
 *    --------------          --------------
 *
 * Switch Block pin location map
 *
 *                       Grid[x][y+1]   ChanY[x][y+1]  Grid[x+1][y+1] 
 *                        right_pins  inputs/outputs     left_pins
 *                            |             ^                |
 *                            |             |                |
 *                            v             v                v
 *                    +-----------------------------------------------+
 *                    |                                               |
 *    Grid[x][y+1]    |                                               |    Grid[x+1][y+1]
 *    bottom_pins---->|                                               |<---- bottom_pins
 *                    |                                               |
 * ChanX[x][y]        |              Switch Box [x][y]                |     ChanX[x+1][y]
 * inputs/outputs<--->|                                               |<---> inputs/outputs
 *                    |                                               |
 *    Grid[x][y+1]    |                                               |    Grid[x+1][y+1]
 *       top_pins---->|                                               |<---- top_pins
 *                    |                                               |
 *                    +-----------------------------------------------+
 *                            ^             ^                ^
 *                            |             |                |
 *                            |             v                |
 *                       Grid[x][y]     ChanY[x][y]      Grid[x+1][y] 
 *                       right_pins    inputs/outputs      left_pins
 *
 *
 ********************************************************************/
static 
void print_verilog_routing_switch_box_unique_module(NetlistManager& netlist_manager,
                                                    const ModuleManager& module_manager, 
                                                    const std::string& subckt_dir, 
                                                    const std::string& subckt_dir_name, 
                                                    const RRGSB& rr_gsb,
                                                    const FabricVerilogOption& options) {
  /* Create the netlist */
  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  std::string verilog_fname(generate_routing_block_netlist_name(SB_VERILOG_FILE_NAME_PREFIX, gsb_coordinate, std::string(VERILOG_NETLIST_FILE_POSTFIX)));
  std::string verilog_fpath(subckt_dir + verilog_fname);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fpath, std::fstream::out | std::fstream::trunc);

  check_file_stream(verilog_fpath.c_str(), fp);

  print_verilog_file_header(fp,
                            std::string("Verilog modules for Unique Switch Blocks[" + std::to_string(rr_gsb.get_sb_x()) + "]["+ std::to_string(rr_gsb.get_sb_y()) + "]"),
                            options.time_stamp()); 

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId sb_module = module_manager.find_module(generate_switch_block_module_name(gsb_coordinate)); 
  VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

  /* Write the verilog module */
  write_verilog_module_to_file(fp,
                               module_manager,
                               sb_module,
                               options.explicit_port_mapping(),
                               options.default_net_type());
 
  /* Close file handler */
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = NetlistId::INVALID();
  if (options.use_relative_path()) {
    nlist_id = netlist_manager.add_netlist(subckt_dir_name + verilog_fname);
  } else {
    nlist_id = netlist_manager.add_netlist(verilog_fpath);
  }
  VTR_ASSERT(nlist_id);
  netlist_manager.set_netlist_type(nlist_id, NetlistManager::ROUTING_MODULE_NETLIST);
}

/********************************************************************
 * Iterate over all the connection blocks in a device
 * and build a module for each of them 
 *******************************************************************/
static 
void print_verilog_flatten_connection_block_modules(NetlistManager& netlist_manager,
                                                    const ModuleManager& module_manager, 
                                                    const DeviceRRGSB& device_rr_gsb,
                                                    const std::string& subckt_dir,
                                                    const std::string& subckt_dir_name,
                                                    const t_rr_type& cb_type,
                                                    const FabricVerilogOption& options) {
  /* Build unique X-direction connection block modules */
  vtr::Point<size_t> cb_range = device_rr_gsb.get_gsb_range();

  for (size_t ix = 0; ix < cb_range.x(); ++ix) {
    for (size_t iy = 0; iy < cb_range.y(); ++iy) {
      /* Check if the connection block exists in the device!
       * Some of them do NOT exist due to heterogeneous blocks (height > 1) 
       * We will skip those modules
       */
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
      if (true != rr_gsb.is_cb_exist(cb_type)) {
        continue;
      }
      print_verilog_routing_connection_box_unique_module(netlist_manager, 
                                                         module_manager,
                                                         subckt_dir, 
                                                         subckt_dir_name, 
                                                         rr_gsb, cb_type,  
                                                         options);
    }
  }
}

/********************************************************************
 * A top-level function of this file
 * Print all the modules for global routing architecture of a FPGA fabric
 * in Verilog format  in a flatten way:
 *   Each connection block and switch block will be generated as a unique module
 * Covering:
 * 1. Connection blocks
 * 2. Switch blocks
 *******************************************************************/
void print_verilog_flatten_routing_modules(NetlistManager& netlist_manager,
                                           const ModuleManager& module_manager,
                                           const DeviceRRGSB& device_rr_gsb,
                                           const std::string& subckt_dir,
                                           const std::string& subckt_dir_name,
                                           const FabricVerilogOption& options) {
  /* Create a vector to contain all the Verilog netlist names that have been generated in this function */
  std::vector<std::string> netlist_names;

  vtr::Point<size_t> sb_range = device_rr_gsb.get_gsb_range();

  /* Build unique switch block modules */
  for (size_t ix = 0; ix < sb_range.x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.y(); ++iy) {
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
      if (true != rr_gsb.is_sb_exist()) {
        continue;
      }
      print_verilog_routing_switch_box_unique_module(netlist_manager,
                                                     module_manager, 
                                                     subckt_dir, 
                                                     subckt_dir_name, 
                                                     rr_gsb, 
                                                     options);
    }
  }

  print_verilog_flatten_connection_block_modules(netlist_manager,
                                                 module_manager,
                                                 device_rr_gsb,
                                                 subckt_dir,
                                                 subckt_dir_name,
                                                 CHANX,
                                                 options);

  print_verilog_flatten_connection_block_modules(netlist_manager,
                                                 module_manager,
                                                 device_rr_gsb,
                                                 subckt_dir,
                                                 subckt_dir_name,
                                                 CHANY,
                                                 options);
}


/********************************************************************
 * A top-level function of this file
 * Print all the unique modules for global routing architecture of a FPGA fabric
 * in Verilog format, including: 
 * 1. Connection blocks
 * 2. Switch blocks
 *
 * Note: this function SHOULD be called only when 
 * the option compact_routing_hierarchy is turned on!!!
 *******************************************************************/
void print_verilog_unique_routing_modules(NetlistManager& netlist_manager,
                                          const ModuleManager& module_manager,
                                          const DeviceRRGSB& device_rr_gsb,
                                          const std::string& subckt_dir,
                                          const std::string& subckt_dir_name,
                                          const FabricVerilogOption& options) {
  /* Create a vector to contain all the Verilog netlist names that have been generated in this function */
  std::vector<std::string> netlist_names;

  /* Build unique switch block modules */
  for (size_t isb = 0; isb < device_rr_gsb.get_num_sb_unique_module(); ++isb) {
    const RRGSB& unique_mirror = device_rr_gsb.get_sb_unique_module(isb);
    print_verilog_routing_switch_box_unique_module(netlist_manager,
                                                   module_manager,
                                                   subckt_dir, 
                                                   subckt_dir_name, 
                                                   unique_mirror, 
                                                   options);
  }

  /* Build unique X-direction connection block modules */
  for (size_t icb = 0; icb < device_rr_gsb.get_num_cb_unique_module(CHANX); ++icb) {
    const RRGSB& unique_mirror = device_rr_gsb.get_cb_unique_module(CHANX, icb);

    print_verilog_routing_connection_box_unique_module(netlist_manager,
                                                       module_manager,
                                                       subckt_dir, 
                                                       subckt_dir_name, 
                                                       unique_mirror, CHANX,  
                                                       options);
  }

  /* Build unique X-direction connection block modules */
  for (size_t icb = 0; icb < device_rr_gsb.get_num_cb_unique_module(CHANY); ++icb) {
    const RRGSB& unique_mirror = device_rr_gsb.get_cb_unique_module(CHANY, icb);

    print_verilog_routing_connection_box_unique_module(netlist_manager, 
                                                       module_manager,
                                                       subckt_dir, 
                                                       subckt_dir_name, 
                                                       unique_mirror, CHANY,  
                                                       options);
  }

  VTR_LOG("\n");
}

} /* end namespace openfpga */
