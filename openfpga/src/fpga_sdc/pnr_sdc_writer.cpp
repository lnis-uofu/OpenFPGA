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

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"
#include "openfpga_digest.h"

#include "mux_utils.h"

#include "openfpga_naming.h"

#include "sdc_writer_naming.h"
#include "sdc_writer_utils.h"
#include "sdc_memory_utils.h"
#include "pnr_sdc_routing_writer.h"
#include "pnr_sdc_grid_writer.h"
#include "pnr_sdc_writer.h"

/* begin namespace openfpga */
namespace openfpga {

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

  /* Start time count */
  std::string timer_message = std::string("Generating SDC for constraining clocks for P&R flow '") + sdc_fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Clock contraints for PnR"));

  /* Get clock port from the global port */
  for (const CircuitPortId& clock_port : global_ports) {
    if (CIRCUIT_MODEL_PORT_CLOCK != circuit_lib.port_type(clock_port)) {
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
    if (CIRCUIT_MODEL_PORT_CLOCK == circuit_lib.port_type(global_port)) {
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

  /* Start time count */
  std::string timer_message = std::string("Generating SDC to disable configurable memory outputs for P&R flow '") + sdc_fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Disable configurable memory outputs for PnR"));

  /* Go recursively in the module manager, starting from the top-level module: instance id of the top-level module is 0 by default */
  rec_print_pnr_sdc_disable_configurable_memory_module_output(fp, module_manager, top_module, 
                                                              format_dir_path(module_manager.module_name(top_module)));

  /* Close file handler */
  fp.close();
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

  /* Start time count */
  std::string timer_message = std::string("Generating SDC to disable routing multiplexer outputs for P&R flow '") + sdc_fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Disable routing multiplexer outputs for PnR"));

  /* Iterate over the MUX modules */
  for (const MuxId& mux_id : mux_lib.muxes()) {
    const CircuitModelId& mux_model = mux_lib.mux_circuit_model(mux_id);
    
    /* Skip LUTs, we only care about multiplexers here */
    if (CIRCUIT_MODEL_MUX != circuit_lib.model_type(mux_model)) {
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
                                                                const DeviceRRGSB& device_rr_gsb) {
  /* Create the file name for Verilog netlist */
  std::string sdc_fname(sdc_dir + std::string(SDC_DISABLE_SB_OUTPUTS_FILE_NAME));

  /* Start time count */
  std::string timer_message = std::string("Generating SDC to disable switch block outputs for P&R flow '") + sdc_fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Disable Switch Block outputs for PnR"));

  /* Get the range of SB array */
  vtr::Point<size_t> sb_range = device_rr_gsb.get_gsb_range();
  /* Go for each SB */
  for (size_t ix = 0; ix < sb_range.x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.y(); ++iy) {
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);

      if (false == rr_gsb.is_sb_exist()) {
        continue;
      }

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
                                                                const DeviceRRGSB& device_rr_gsb) {
  /* Create the file name for Verilog netlist */
  std::string sdc_fname(sdc_dir + std::string(SDC_DISABLE_SB_OUTPUTS_FILE_NAME));

  /* Start time count */
  std::string timer_message = std::string("Generating SDC to disable switch block outputs for P&R flow '") + sdc_fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Disable Switch Block outputs for PnR"));

  /* Build unique switch block modules */
  for (size_t isb = 0; isb < device_rr_gsb.get_num_sb_unique_module(); ++isb) {
    const RRGSB& rr_gsb = device_rr_gsb.get_sb_unique_module(isb);
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
}

/********************************************************************
 * Top-level function to print a number of SDC files in different purpose
 * This function will generate files upon the options provided by users
 * 1. Design constraints for CLBs
 * 2. Design constraints for Switch Blocks
 * 3. Design constraints for Connection Blocks 
 * 4. Design constraints for breaking the combinational loops in FPGA fabric
 *******************************************************************/
void print_pnr_sdc(const PnrSdcOption& sdc_options,
                   const float& critical_path_delay,
                   const DeviceContext& device_ctx,
                   const VprDeviceAnnotation& device_annotation,
                   const DeviceRRGSB& device_rr_gsb,
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
                                                                 device_rr_gsb);
    } else {
      VTR_ASSERT_SAFE (false == compact_routing_hierarchy);
      print_pnr_sdc_flatten_routing_disable_switch_block_outputs(sdc_options.sdc_dir(),
                                                                 module_manager,
                                                                 device_rr_gsb);
    }
  }

  /* Output routing constraints for Switch Blocks */
  if (true == sdc_options.constrain_sb()) {
    if (true == compact_routing_hierarchy) {
      print_pnr_sdc_compact_routing_constrain_sb_timing(sdc_options.sdc_dir(),
                                                        module_manager,
                                                        device_ctx.rr_graph,
                                                        device_rr_gsb);
    } else {
	  VTR_ASSERT_SAFE (false == compact_routing_hierarchy);
      print_pnr_sdc_flatten_routing_constrain_sb_timing(sdc_options.sdc_dir(),
                                                        module_manager,
                                                        device_ctx.rr_graph,
                                                        device_rr_gsb);
    }
  }

  /* Output routing constraints for Connection Blocks */
  if (true == sdc_options.constrain_cb()) {
    if (true == compact_routing_hierarchy) {
      print_pnr_sdc_compact_routing_constrain_cb_timing(sdc_options.sdc_dir(),
                                                        module_manager,
                                                        device_ctx.rr_graph,
                                                        device_rr_gsb);
    } else {
	  VTR_ASSERT_SAFE (false == compact_routing_hierarchy);
      print_pnr_sdc_flatten_routing_constrain_cb_timing(sdc_options.sdc_dir(),
                                                        module_manager, 
                                                        device_ctx.rr_graph,
                                                        device_rr_gsb);
    }
  }

  /* Output Timing constraints for Programmable blocks */
  if (true == sdc_options.constrain_grid()) {
    print_pnr_sdc_constrain_grid_timing(sdc_options.sdc_dir(),
                                        device_ctx,
                                        device_annotation,
                                        module_manager);
  }
}

} /* end namespace openfpga */
