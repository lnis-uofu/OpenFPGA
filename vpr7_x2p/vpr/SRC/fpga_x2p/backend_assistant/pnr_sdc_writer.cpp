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

#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"

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
 * Break combinational loops in FPGA fabric, which mainly come from:
 * 1. Configurable memory cells. 
 *    To handle this, we disable the outputs of memory cells
 * 2. Loops of multiplexers.
 *    To handle this, we disable the outputs of routing multiplexers
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
 * Top-level function to print a number of SDC files in different purpose
 * This function will generate files upon the options provided by users
 * 1. Design constraints for CLBs
 * 2. Design constraints for Switch Blocks
 * 3. Design constraints for Connection Blocks 
 * 4. Design constraints for breaking the combinational loops in FPGA fabric
 *******************************************************************/
void print_pnr_sdc(const SdcOption& sdc_options,
                   const float& critical_path_delay,
                   const CircuitLibrary& circuit_lib,
                   const ModuleManager& module_manager,
                   const std::vector<CircuitPortId>& global_ports) {
  
  /* Part 1. Constrain global ports */
  if (true == sdc_options.constrain_global_port()) {
    print_pnr_sdc_global_ports(sdc_options.sdc_dir(), critical_path_delay, circuit_lib, global_ports);
  }

  /* Part 2. Output Design Constraints to disable outputs of memory cells */
  if (true == sdc_options.constrain_configurable_memory_outputs()) {
    std::string top_module_name = generate_fpga_top_module_name();
    ModuleId top_module = module_manager.find_module(top_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(top_module));
    print_pnr_sdc_constrain_configurable_memory_outputs(sdc_options.sdc_dir(), module_manager, top_module); 
  } 

  /* 2. Break loops from Multiplexer Output */
  /*
  if (TRUE == sdc_opts.break_loops_mux) {
    verilog_generate_sdc_break_loop_mux(fp, num_switch, switches, spice, routing_arch); 
  }
   */

  /* TODO: 3. Break loops from any SB output */
  /*
  if (TRUE == sdc_opts.compact_routing_hierarchy) {
    verilog_generate_sdc_break_loop_sb(fp, LL_device_rr_gsb);
  } else {
    verilog_generate_sdc_break_loop_sb(fp, LL_nx, LL_ny);
  }
  */
}
