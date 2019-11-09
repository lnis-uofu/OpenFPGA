/********************************************************************
 * This file includes functions that print SDC (Synopsys Design Constraint) 
 * files in physical design tools, i.e., Place & Route (PnR) tools
 * The SDC files are used to constrain the physical design for each module
 * in FPGA fabric, such as Configurable Logic Blocks (CLBs), 
 * Heterogeneous blocks, Switch Blocks (SBs) and Connection Blocks (CBs)
 *
 * Note that this is different from the SDC to constrain VPR Place&Route
 * engine! These SDCs are designed for PnR to generate FPGA layouts!!!
 *******************************************************************/
#include <ctime>
#include <fstream>
#include <iomanip>

#include "vtr_assert.h"
#include "device_port.h"

#include "util.h"
#include "mux_utils.h"

#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"

#include "build_routing_module_utils.h"

#include "sdc_writer_naming.h"
#include "sdc_writer_utils.h"
#include "pnr_sdc_writer.h"

/********************************************************************
 * Local variables
 *******************************************************************/
constexpr float SDC_FIXED_PROG_CLOCK_PERIOD = 100;
constexpr float SDC_FIXED_CLOCK_PERIOD = 10;

/********************************************************************
 * Print a SDC file to constrain the global ports of FPGA fabric
 * in particular clock ports
 * 
 * For programming clock, we give a fixed period, while for operating
 * clock, we constrain with critical path delay 
 *******************************************************************/
static 
void print_pnr_sdc_global_ports(const std::string& sdc_dir, 
                                const float& critical_path_delay,
                                const CircuitLibrary& circuit_lib,
                                const std::vector<CircuitPortId>& global_ports) {

  /* Create the file name for Verilog netlist */
  std::string sdc_fname(sdc_dir + std::string(SDC_GLOBAL_PORTS_FILE_NAME));

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for constraining clocks for P&R flow: %s ...",
             sdc_fname.c_str());

  /* Start time count */
  clock_t t_start = clock();

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Clock contraints for PnR"));

  /* Get clock port from the global port */
  for (const CircuitPortId& clock_port : global_ports) {
    if (SPICE_MODEL_PORT_CLOCK != circuit_lib.port_type(clock_port)) {
      continue;
    }
    /* Reach here, it means a clock port and we need print constraints */
    float clock_period = critical_path_delay; 

    /* For programming clock, we give a fixed period */
    if (true == circuit_lib.port_is_prog(clock_port)) {
      clock_period = SDC_FIXED_PROG_CLOCK_PERIOD;
      /* Print comments */
      fp << "##################################################" << std::endl; 
      fp << "# Create programmable clock                       " << std::endl;
      fp << "##################################################" << std::endl; 
    } else {
      /* Print comments */
      fp << "##################################################" << std::endl; 
      fp << "# Create clock                                    " << std::endl;
      fp << "##################################################" << std::endl; 
    }

    for (const size_t& pin : circuit_lib.pins(clock_port)) {
      BasicPort port_to_constrain(circuit_lib.port_prefix(clock_port), pin, pin);

      fp << "create_clock ";
      fp << generate_sdc_port(port_to_constrain) << "-period ";
      fp << std::setprecision(10) << clock_period;
      fp << " -waveform {0 ";
      fp << std::setprecision(10) << clock_period / 2;
      fp << "}" << std::endl;

      fp << std::endl;
    }
  }

  /* For non-clock port from the global port: give a fixed period */
  for (const CircuitPortId& global_port : global_ports) {
    if (SPICE_MODEL_PORT_CLOCK == circuit_lib.port_type(global_port)) {
      continue;
    }

    /* Print comments */
    fp << "##################################################" << std::endl; 
    fp << "# Constrain other global ports                    " << std::endl;
    fp << "##################################################" << std::endl; 

    /* Reach here, it means a non-clock global port and we need print constraints */
    float clock_period = SDC_FIXED_CLOCK_PERIOD; 
    for (const size_t& pin : circuit_lib.pins(global_port)) {
      BasicPort port_to_constrain(circuit_lib.port_prefix(global_port), pin, pin);
      fp << "create_clock ";
      fp << generate_sdc_port(port_to_constrain) << "-period ";
      fp << std::setprecision(10) << clock_period;
      fp << " -waveform {0 ";
      fp << std::setprecision(10) << clock_period / 2;
      fp << "} ";
      fp << "[list [get_ports { " << generate_sdc_port(port_to_constrain) << "}]]" << std::endl;

      fp << "set_drive 0 " << generate_sdc_port(port_to_constrain) << std::endl;
     
      fp << std::endl;
    }
  }

  /* Close file handler */
  fp.close();

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}

/********************************************************************
 * Print SDC commands to disable outputs of all the configurable memory modules
 * in a given module 
 * This function will be executed in a recursive way, 
 * using a Depth-First Search (DFS) strategy
 * It will iterate over all the configurable children under each module
 * and print a SDC command to disable its outputs
 *******************************************************************/
static 
void rec_print_pnr_sdc_disable_configurable_memory_module_output(std::fstream& fp, 
                                                                 const ModuleManager& module_manager, 
                                                                 const ModuleId& parent_module,
                                                                 const std::string& parent_module_path) {

  /* For each configurable child, we will go one level down in priority */
  for (size_t child_index = 0; child_index < module_manager.configurable_children(parent_module).size(); ++child_index) {
    std::string child_module_path = parent_module_path;
    ModuleId child_module_id = module_manager.configurable_children(parent_module)[child_index];
    size_t child_instance_id = module_manager.configurable_child_instances(parent_module)[child_index];
    if (true == module_manager.instance_name(parent_module, child_module_id, child_instance_id).empty()) {
      /* Give a default name <module_name>_<instance_id>_ */
      child_module_path += module_manager.module_name(child_module_id); 
      child_module_path += "_";
      child_module_path += std::to_string(child_instance_id);
      child_module_path += "_";
    } else {
      child_module_path += module_manager.instance_name(parent_module, child_module_id, child_instance_id);
    }
    child_module_path = format_dir_path(child_module_path);

    rec_print_pnr_sdc_disable_configurable_memory_module_output(fp, module_manager, 
                                                                child_module_id, 
                                                                child_module_path);
  }

  /* If there is no configurable children any more, this is a leaf module, print a SDC command for disable timing */
  if (0 < module_manager.configurable_children(parent_module).size()) {
    return;
  }

  /* Validate file stream */
  check_file_handler(fp);

  /* Disable timing for each output port of this module */
  for (const BasicPort& output_port : module_manager.module_ports_by_type(parent_module, ModuleManager::MODULE_OUTPUT_PORT)) {
    for (const size_t& pin : output_port.pins()) {
      BasicPort output_pin(output_port.get_name(), pin, pin);
      fp << "set_disable_timing ";
      fp << parent_module_path << generate_sdc_port(output_pin);
      fp << std::endl;
    }
  }
}

/********************************************************************
 * Break combinational loops in FPGA fabric, which mainly come from
 * configurable memory cells. 
 * To handle this, we disable the outputs of memory cells
 *******************************************************************/
static 
void print_pnr_sdc_constrain_configurable_memory_outputs(const std::string& sdc_dir,
                                                         const ModuleManager& module_manager,
                                                         const ModuleId& top_module) {

  /* Create the file name for Verilog netlist */
  std::string sdc_fname(sdc_dir + std::string(SDC_DISABLE_CONFIG_MEM_OUTPUTS_FILE_NAME));

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for disable configurable memory outputs for P&R flow: %s ...",
             sdc_fname.c_str());

  /* Start time count */
  clock_t t_start = clock();

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Disable configurable memory outputs for PnR"));

  /* Go recursively in the module manager, starting from the top-level module: instance id of the top-level module is 0 by default */
  rec_print_pnr_sdc_disable_configurable_memory_module_output(fp, module_manager, top_module, 
                                                              format_dir_path(module_manager.module_name(top_module)));

  /* Close file handler */
  fp.close();

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}

/********************************************************************
 * Break combinational loops in FPGA fabric, which mainly come from 
 * loops of multiplexers.
 * To handle this, we disable the timing at outputs of routing multiplexers
 *******************************************************************/
static 
void print_sdc_disable_routing_multiplexer_outputs(const std::string& sdc_dir,
                                                   const MuxLibrary& mux_lib,
                                                   const CircuitLibrary& circuit_lib,
                                                   const ModuleManager& module_manager) {
  /* Create the file name for Verilog netlist */
  std::string sdc_fname(sdc_dir + std::string(SDC_DISABLE_MUX_OUTPUTS_FILE_NAME));

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for disable routing multiplexer outputs for P&R flow: %s ...",
             sdc_fname.c_str());

  /* Start time count */
  clock_t t_start = clock();

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Disable routing multiplexer outputs for PnR"));

  /* Iterate over the MUX modules */
  for (const MuxId& mux_id : mux_lib.muxes()) {
    const CircuitModelId& mux_model = mux_lib.mux_circuit_model(mux_id);
    
    /* Skip LUTs, we only care about multiplexers here */
    if (SPICE_MODEL_MUX != circuit_lib.model_type(mux_model)) {
      continue;
    }

    const MuxGraph& mux_graph = mux_lib.mux_graph(mux_id);
    std::string mux_module_name = generate_mux_subckt_name(circuit_lib, mux_model, 
                                                           find_mux_num_datapath_inputs(circuit_lib, mux_model, mux_graph.num_inputs()), 
                                                           std::string(""));
    /* Find the module name in module manager */
    ModuleId mux_module = module_manager.find_module(mux_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(mux_module));
     
    /* Disable the timing for the output ports */ 
    for (const BasicPort& output_port : module_manager.module_ports_by_type(mux_module, ModuleManager::MODULE_OUTPUT_PORT)) {
      fp << "set_disable_timing [get_pins -filter \"name =~ " << output_port.get_name() << "*\" ";
      fp << "-of [get_cells -hier -filter \"ref_lib_cell_name == " << mux_module_name << "\"]]" << std::endl;
      fp << std::endl;
    }
  }

  /* Close file handler */
  fp.close();

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}

/********************************************************************
 * Break combinational loops in FPGA fabric, which mainly come from 
 * loops of multiplexers.
 * To handle this, we disable the timing at outputs of Switch blocks
 * This function is designed for flatten routing hierarchy
 *******************************************************************/
static 
void print_pnr_sdc_flatten_routing_disable_switch_block_outputs(const std::string& sdc_dir,
                                                                const ModuleManager& module_manager,
                                                                const DeviceRRGSB& L_device_rr_gsb) {
  /* Create the file name for Verilog netlist */
  std::string sdc_fname(sdc_dir + std::string(SDC_DISABLE_SB_OUTPUTS_FILE_NAME));

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for disable Switch Block outputs for P&R flow: %s ...",
             sdc_fname.c_str());

  /* Start time count */
  clock_t t_start = clock();

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Disable Switch Block outputs for PnR"));

  /* Get the range of SB array */
  DeviceCoordinator sb_range = L_device_rr_gsb.get_gsb_range();
  /* Go for each SB */
  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
      std::string sb_instance_name = generate_switch_block_module_name(gsb_coordinate); 

      ModuleId sb_module = module_manager.find_module(sb_instance_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

      /* Disable the outputs of the module */
      for (const BasicPort& output_port : module_manager.module_ports_by_type(sb_module, ModuleManager::MODULE_OUTPUT_PORT)) {
        fp << "set_disable_timing " << sb_instance_name << "/" << output_port.get_name() << std::endl;
        fp << std::endl;
      }
    }
  }
  
  /* Close file handler */
  fp.close();

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}

/********************************************************************
 * Break combinational loops in FPGA fabric, which mainly come from 
 * loops of multiplexers.
 * To handle this, we disable the timing at outputs of Switch blocks
 * This function is designed for compact routing hierarchy
 *******************************************************************/
static 
void print_pnr_sdc_compact_routing_disable_switch_block_outputs(const std::string& sdc_dir,
                                                                const ModuleManager& module_manager,
                                                                const ModuleId& top_module,
                                                                const DeviceRRGSB& L_device_rr_gsb) {
  /* Create the file name for Verilog netlist */
  std::string sdc_fname(sdc_dir + std::string(SDC_DISABLE_SB_OUTPUTS_FILE_NAME));

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for disable Switch Block outputs for P&R flow: %s ...",
             sdc_fname.c_str());

  /* Start time count */
  clock_t t_start = clock();

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Disable Switch Block outputs for PnR"));

  /* Build unique switch block modules */
  for (size_t isb = 0; isb < L_device_rr_gsb.get_num_sb_unique_module(); ++isb) {
    const RRGSB& rr_gsb = L_device_rr_gsb.get_sb_unique_module(isb);
    vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
    std::string sb_module_name = generate_switch_block_module_name(gsb_coordinate); 

    ModuleId sb_module = module_manager.find_module(sb_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

    /* Find all the instances in the top-level module */
    for (const size_t& instance_id : module_manager.child_module_instances(top_module, sb_module)) {
      std::string sb_instance_name = module_manager.instance_name(top_module, sb_module, instance_id);
      /* Disable the outputs of the module */
      for (const BasicPort& output_port : module_manager.module_ports_by_type(sb_module, ModuleManager::MODULE_OUTPUT_PORT)) {
        fp << "set_disable_timing " << sb_instance_name << "/" << output_port.get_name() << std::endl;
        fp << std::endl;
      }
    }
  }
  
  /* Close file handler */
  fp.close();

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}

/********************************************************************
 * Find the timing constraints between the inputs and outputs of a routing
 * multiplexer in a Switch Block
 *******************************************************************/
static 
float find_pnr_sdc_switch_tmax(const t_switch_inf& switch_inf) {
  return switch_inf.R * switch_inf.Cout + switch_inf.Tdel;
}

/********************************************************************
 * Set timing constraints between the inputs and outputs of a routing
 * multiplexer in a Switch Block
 *******************************************************************/
static 
void print_pnr_sdc_constrain_sb_mux_timing(std::fstream& fp,
                                           const ModuleManager& module_manager,
                                           const ModuleId& sb_module, 
                                           const RRGSB& rr_gsb,
                                           const std::vector<std::vector<t_grid_tile>>& grids,
                                           const std::vector<t_switch_inf>& switches,
                                           const e_side& output_node_side,
                                           t_rr_node* output_rr_node) {
  /* Validate file stream */
  check_file_handler(fp);

  VTR_ASSERT(  ( CHANX == output_rr_node->type )
            || ( CHANY == output_rr_node->type ));

  /* Find the module port corresponding to the output rr_node */
  ModulePortId module_output_port = find_switch_block_module_chan_port(module_manager, 
                                                                       sb_module,
                                                                       rr_gsb, 
                                                                       output_node_side,
                                                                       output_rr_node,
                                                                       OUT_PORT);

  /* Find the module port corresponding to the fan-in rr_nodes of the output rr_node */
  std::vector<t_rr_node*> input_rr_nodes;
  for (int iedge = 0; iedge < output_rr_node->num_drive_rr_nodes; iedge++) {
    input_rr_nodes.push_back(output_rr_node->drive_rr_nodes[iedge]);
  }
     
  std::vector<ModulePortId> module_input_ports = find_switch_block_module_input_ports(module_manager,
                                                                                      sb_module, 
                                                                                      rr_gsb, 
                                                                                      grids,
                                                                                      input_rr_nodes);

  /* Find timing constraints for each path (edge) */
  std::map<ModulePortId, float> switch_delays;
  for (int iedge = 0; iedge < output_rr_node->num_drive_rr_nodes; iedge++) {
    /* Get the switch delay */
    int switch_id = output_rr_node->drive_switches[iedge];
    switch_delays[module_input_ports[iedge]] = find_pnr_sdc_switch_tmax(switches[switch_id]);
  }

  /* Find the starting points */
  for (const ModulePortId& module_input_port : module_input_ports) {
    /* Constrain a path */
    print_pnr_sdc_constrain_module_port2port_timing(fp,
                                                    module_manager, sb_module,
                                                    module_input_port, module_output_port,
                                                    switch_delays[module_input_port]);
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
static 
void print_pnr_sdc_constrain_sb_timing(const std::string& sdc_dir,
                                       const ModuleManager& module_manager,
                                       const std::vector<std::vector<t_grid_tile>>& grids,
                                       const std::vector<t_switch_inf>& switches,
                                       const RRGSB& rr_gsb) {

  /* Create the file name for Verilog netlist */
  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  std::string sdc_fname(sdc_dir + generate_switch_block_module_name(gsb_coordinate) + std::string(SDC_FILE_NAME_POSTFIX));

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  /* Validate file stream */
  check_file_handler(fp);

  std::string sb_module_name = generate_switch_block_module_name(gsb_coordinate);
  ModuleId sb_module = module_manager.find_module(sb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Constrain timing of Switch Block " + sb_module_name + " for PnR"));

  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    for (size_t itrack = 0; itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      t_rr_node* chan_rr_node = rr_gsb.get_chan_node(side_manager.get_side(), itrack);
      /* We only care the output port and it should indicate a SB mux */
      if (OUT_PORT != rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) { 
        continue; 
      }
      /* Constrain thru wires */
      if (false != rr_gsb.is_sb_node_passing_wire(side_manager.get_side(), itrack)) {
        continue;
      } 
      /* This is a MUX, constrain all the paths from an input to an output */
      print_pnr_sdc_constrain_sb_mux_timing(fp,
                                            module_manager, sb_module, 
                                            rr_gsb,
                                            grids, switches,
                                            side_manager.get_side(),
                                            chan_rr_node);
    }
  }

  /* Close file handler */
  fp.close();
}

/********************************************************************
 * Print SDC timing constraints for Switch blocks
 * This function is designed for flatten routing hierarchy
 *******************************************************************/
static 
void print_pnr_sdc_flatten_routing_constrain_sb_timing(const std::string& sdc_dir,
                                                       const ModuleManager& module_manager,
                                                       const std::vector<std::vector<t_grid_tile>>& grids,
                                                       const std::vector<t_switch_inf>& switches,
                                                       const DeviceRRGSB& L_device_rr_gsb) {
  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for constrain Switch Block timing for P&R flow...");

  /* Start time count */
  clock_t t_start = clock();

  /* Get the range of SB array */
  DeviceCoordinator sb_range = L_device_rr_gsb.get_gsb_range();
  /* Go for each SB */
  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      print_pnr_sdc_constrain_sb_timing(sdc_dir,
                                        module_manager,
                                        grids, switches,
                                        rr_gsb);
    }
  }
  
  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}

/********************************************************************
 * Print SDC timing constraints for Switch blocks
 * This function is designed for compact routing hierarchy
 *******************************************************************/
static 
void print_pnr_sdc_compact_routing_constrain_sb_timing(const std::string& sdc_dir,
                                                       const ModuleManager& module_manager,
                                                       const std::vector<std::vector<t_grid_tile>>& grids,
                                                       const std::vector<t_switch_inf>& switches,
                                                       const DeviceRRGSB& L_device_rr_gsb) {
  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for constrain Switch Block timing for P&R flow...");

  /* Start time count */
  clock_t t_start = clock();

  for (size_t isb = 0; isb < L_device_rr_gsb.get_num_sb_unique_module(); ++isb) {
    const RRGSB& rr_gsb = L_device_rr_gsb.get_sb_unique_module(isb);
    print_pnr_sdc_constrain_sb_timing(sdc_dir,
                                      module_manager,
                                      grids, switches,
                                      rr_gsb);
  }
  
  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}

/********************************************************************
 * Set timing constraints between the inputs and outputs of a routing
 * multiplexer in a Connection Block
 *******************************************************************/
static 
void print_pnr_sdc_constrain_cb_mux_timing(std::fstream& fp,
                                           const ModuleManager& module_manager,
                                           const ModuleId& cb_module, 
                                           const RRGSB& rr_gsb,
                                           const t_rr_type& cb_type,
                                           const std::vector<std::vector<t_grid_tile>>& grids,
                                           const std::vector<t_switch_inf>& switches,
                                           t_rr_node* output_rr_node) {
  /* Validate file stream */
  check_file_handler(fp);

  VTR_ASSERT(IPIN == output_rr_node->type);

  /* Find the module port corresponding to the output rr_node */
  ModulePortId module_output_port = find_connection_block_module_ipin_port(module_manager, 
                                                                           cb_module,
                                                                           rr_gsb, 
                                                                           grids, output_rr_node);

  /* Find the module port corresponding to the fan-in rr_nodes of the output rr_node */
  std::vector<t_rr_node*> input_rr_nodes;
  for (int iedge = 0; iedge < output_rr_node->num_drive_rr_nodes; iedge++) {
    input_rr_nodes.push_back(output_rr_node->drive_rr_nodes[iedge]);
  }
     
  std::vector<ModulePortId> module_input_ports = find_connection_block_module_input_ports(module_manager,
                                                                                          cb_module, 
                                                                                          rr_gsb, 
                                                                                          cb_type,
                                                                                          input_rr_nodes);

  /* Find timing constraints for each path (edge) */
  std::map<ModulePortId, float> switch_delays;
  for (int iedge = 0; iedge < output_rr_node->num_drive_rr_nodes; iedge++) {
    /* Get the switch delay */
    int switch_id = output_rr_node->drive_switches[iedge];
    switch_delays[module_input_ports[iedge]] = find_pnr_sdc_switch_tmax(switches[switch_id]);
  }

  /* Find the starting points */
  for (const ModulePortId& module_input_port : module_input_ports) {
    /* Constrain a path */
    print_pnr_sdc_constrain_module_port2port_timing(fp,
                                                    module_manager, cb_module,
                                                    module_input_port, module_output_port,
                                                    switch_delays[module_input_port]);
  }
}


/********************************************************************
 * Print SDC timing constraints for a Connection block 
 * This function is designed for compact routing hierarchy
 *******************************************************************/
static 
void print_pnr_sdc_constrain_cb_timing(const std::string& sdc_dir,
                                       const ModuleManager& module_manager,
                                       const RRGSB& rr_gsb, 
                                       const t_rr_type& cb_type,
                                       const std::vector<std::vector<t_grid_tile>>& grids,
                                       const std::vector<t_switch_inf>& switches) {
  /* Create the netlist */
  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));

  /* Find the module name and create a SDC file for it */
  std::string sdc_fname(sdc_dir + generate_connection_block_module_name(cb_type, gsb_coordinate) + std::string(SDC_FILE_NAME_POSTFIX));

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  /* Validate file stream */
  check_file_handler(fp);

  std::string cb_module_name = generate_connection_block_module_name(cb_type, gsb_coordinate); 
  ModuleId cb_module = module_manager.find_module(cb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(cb_module));

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Constrain timing of Connection Block " + cb_module_name + " for PnR"));

  std::vector<enum e_side> cb_sides = rr_gsb.get_cb_ipin_sides(cb_type);

  for (size_t side = 0; side < cb_sides.size(); ++side) {
    enum e_side cb_ipin_side = cb_sides[side];
    Side side_manager(cb_ipin_side);
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side); ++inode) {
      t_rr_node* ipin_rr_node = rr_gsb.get_ipin_node(cb_ipin_side, inode);
      print_pnr_sdc_constrain_cb_mux_timing(fp,
                                            module_manager, cb_module, 
                                            rr_gsb, cb_type,
                                            grids, switches,
                                            ipin_rr_node);
    }
  }

  /* Close file handler */
  fp.close();
}

/********************************************************************
 * Iterate over all the connection blocks in a device
 * and print SDC file for each of them 
 *******************************************************************/
static 
void print_pnr_sdc_flatten_routing_constrain_cb_timing(const std::string& sdc_dir,
                                                       const ModuleManager& module_manager, 
                                                       const DeviceRRGSB& L_device_rr_gsb,
                                                       const std::vector<std::vector<t_grid_tile>>& grids,
                                                       const std::vector<t_switch_inf>& switches,
                                                       const t_rr_type& cb_type) {
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
      print_pnr_sdc_constrain_cb_timing(sdc_dir,
                                        module_manager,
                                        rr_gsb, 
                                        cb_type,
                                        grids, switches);

    }
  }
}

/********************************************************************
 * Iterate over all the connection blocks in a device
 * and print SDC file for each of them 
 *******************************************************************/
static 
void print_pnr_sdc_flatten_routing_constrain_cb_timing(const std::string& sdc_dir,
                                                       const ModuleManager& module_manager, 
                                                       const DeviceRRGSB& L_device_rr_gsb,
                                                       const std::vector<std::vector<t_grid_tile>>& grids,
                                                       const std::vector<t_switch_inf>& switches) {
  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for constrain Connection Block timing for P&R flow...");

  /* Start time count */
  clock_t t_start = clock();

  print_pnr_sdc_flatten_routing_constrain_cb_timing(sdc_dir, module_manager, 
                                                    L_device_rr_gsb,
                                                    grids,
                                                    switches,
                                                    CHANX);

  print_pnr_sdc_flatten_routing_constrain_cb_timing(sdc_dir, module_manager, 
                                                    L_device_rr_gsb,
                                                    grids,
                                                    switches,
                                                    CHANY);

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}

/********************************************************************
 * Print SDC timing constraints for Connection blocks
 * This function is designed for compact routing hierarchy
 *******************************************************************/
static 
void print_pnr_sdc_compact_routing_constrain_cb_timing(const std::string& sdc_dir,
                                                       const ModuleManager& module_manager,
                                                       const std::vector<std::vector<t_grid_tile>>& grids,
                                                       const std::vector<t_switch_inf>& switches,
                                                       const DeviceRRGSB& L_device_rr_gsb) {
  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for constrain Connection Block timing for P&R flow...");

  /* Start time count */
  clock_t t_start = clock();

  /* Print SDC for unique X-direction connection block modules */
  for (size_t icb = 0; icb < L_device_rr_gsb.get_num_cb_unique_module(CHANX); ++icb) {
    const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(CHANX, icb);
    print_pnr_sdc_constrain_cb_timing(sdc_dir,
                                      module_manager,
                                      unique_mirror, 
                                      CHANX,
                                      grids, switches);
  }

  /* Print SDC for unique Y-direction connection block modules */
  for (size_t icb = 0; icb < L_device_rr_gsb.get_num_cb_unique_module(CHANY); ++icb) {
    const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(CHANY, icb);
    print_pnr_sdc_constrain_cb_timing(sdc_dir,
                                      module_manager,
                                      unique_mirror, 
                                      CHANY,
                                      grids, switches);
  }

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}

/********************************************************************
 * Top-level function to print a number of SDC files in different purpose
 * This function will generate files upon the options provided by users
 * 1. Design constraints for CLBs
 * 2. Design constraints for Switch Blocks
 * 3. Design constraints for Connection Blocks 
 * 4. Design constraints for breaking the combinational loops in FPGA fabric
 *******************************************************************/
void print_pnr_sdc(const SdcOption& sdc_options,
                   const float& critical_path_delay,
                   const std::vector<std::vector<t_grid_tile>>& grids,
                   const std::vector<t_switch_inf>& switches,
                   const DeviceRRGSB& L_device_rr_gsb,
                   const ModuleManager& module_manager,
                   const MuxLibrary& mux_lib,
                   const CircuitLibrary& circuit_lib,
                   const std::vector<CircuitPortId>& global_ports,
                   const bool& compact_routing_hierarchy) {
  
  /* Constrain global ports */
  if (true == sdc_options.constrain_global_port()) {
    print_pnr_sdc_global_ports(sdc_options.sdc_dir(), critical_path_delay, circuit_lib, global_ports);
  }

  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Output Design Constraints to disable outputs of memory cells */
  if (true == sdc_options.constrain_configurable_memory_outputs()) {
    print_pnr_sdc_constrain_configurable_memory_outputs(sdc_options.sdc_dir(), module_manager, top_module); 
  } 

  /* Break loops from Multiplexer Output */
  if (true == sdc_options.constrain_routing_multiplexer_outputs()) {
    print_sdc_disable_routing_multiplexer_outputs(sdc_options.sdc_dir(),
                                                  mux_lib, circuit_lib,
                                                  module_manager);
  }

  /* Break loops from any SB output */
  if (true == sdc_options.constrain_switch_block_outputs()) {
    if (true == compact_routing_hierarchy) {
      print_pnr_sdc_compact_routing_disable_switch_block_outputs(sdc_options.sdc_dir(),
                                                                 module_manager, top_module,
                                                                 L_device_rr_gsb);
    } else {
      VTR_ASSERT_SAFE (false == compact_routing_hierarchy);
      print_pnr_sdc_flatten_routing_disable_switch_block_outputs(sdc_options.sdc_dir(),
                                                                module_manager,
                                                                L_device_rr_gsb);
    }
  }

  /* Output routing constraints for Switch Blocks */
  if (true == sdc_options.constrain_sb()) {
    if (true == compact_routing_hierarchy) {
      print_pnr_sdc_compact_routing_constrain_sb_timing(sdc_options.sdc_dir(),
                                                        module_manager,
                                                        grids, switches,
                                                        L_device_rr_gsb);
    } else {
	  VTR_ASSERT_SAFE (false == compact_routing_hierarchy);
      print_pnr_sdc_flatten_routing_constrain_sb_timing(sdc_options.sdc_dir(),
                                                        module_manager,
                                                        grids, switches,
                                                        L_device_rr_gsb);
    }
  }

  /* Output routing constraints for Connection Blocks */
  if (true == sdc_options.constrain_cb()) {
    if (true == compact_routing_hierarchy) {
      print_pnr_sdc_compact_routing_constrain_cb_timing(sdc_options.sdc_dir(),
                                                        module_manager,
                                                        grids,
                                                        switches,
                                                        L_device_rr_gsb);
    } else {
	  VTR_ASSERT_SAFE (false == compact_routing_hierarchy);
      print_pnr_sdc_flatten_routing_constrain_cb_timing(sdc_options.sdc_dir(),
                                                        module_manager, 
                                                        L_device_rr_gsb,
                                                        grids,
                                                        switches);
    }
  }

  /* TODO: Output routing constraints for Programmable blocks */
  /*
  if (true == sdc_options.constrain_grid()) {
    verilog_generate_sdc_constrain_pb_types(cur_sram_orgz_info,
                                            sdc_dir);
  }
  */
}
