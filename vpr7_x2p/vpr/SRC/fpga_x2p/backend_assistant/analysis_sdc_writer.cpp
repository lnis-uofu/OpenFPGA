/********************************************************************
 * This file includes functions that are used to output a SDC file
 * that constrain a FPGA fabric (P&Red netlist) using a benchmark 
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
#include "fpga_x2p_benchmark_utils.h"

#include "sdc_writer_naming.h"
#include "sdc_writer_utils.h"
#include "sdc_memory_utils.h"

#include "analysis_sdc_routing_writer.h"
#include "analysis_sdc_writer.h"

/********************************************************************
 * Generate SDC constaints for inputs and outputs
 * We consider the top module in formal verification purpose here
 * which is easier 
 *******************************************************************/
static 
void print_analysis_sdc_io_delays(std::fstream& fp,
                                  const std::vector<t_logical_block>& L_logical_blocks,
                                  const vtr::Point<size_t>& device_size,
                                  const std::vector<std::vector<t_grid_tile>>& L_grids, 
                                  const std::vector<t_block>& L_blocks,
                                  const ModuleManager& module_manager,
                                  const ModuleId& top_module,
                                  const CircuitLibrary& circuit_lib,
                                  const std::vector<CircuitPortId>& global_ports,
                                  const float& critical_path_delay) {
  /* Validate the file stream */
  check_file_handler(fp);

  /* Print comments */
  fp << "##################################################" << std::endl; 
  fp << "# Create clock                                    " << std::endl;
  fp << "##################################################" << std::endl; 

  /* Get clock port from the global port */
  std::vector<BasicPort> operating_clock_ports;
  for (const CircuitPortId& clock_port : global_ports) {
    if (SPICE_MODEL_PORT_CLOCK != circuit_lib.port_type(clock_port)) {
      continue;
    }
    /* We only constrain operating clock here! */
    if (true == circuit_lib.port_is_prog(clock_port)) {
      continue;
    }
   
    /* Find the module port and Update the operating port list */
    ModulePortId module_port = module_manager.find_module_port(top_module, circuit_lib.port_prefix(clock_port)); 
    operating_clock_ports.push_back(module_manager.module_port(top_module, module_port));
  }

  for (const BasicPort& operating_clock_port : operating_clock_ports) {
    /* Reach here, it means a clock port and we need print constraints */
    fp << "create_clock ";
    fp << generate_sdc_port(operating_clock_port);
    fp << " -period " << std::setprecision(10) << critical_path_delay;
    fp << " -waveform {0 " << std::setprecision(10) << critical_path_delay / 2 << "}";
    fp << std::endl;

    /* Add an empty line as a splitter */
    fp << std::endl;
  }

  /* There should be only one operating clock!
   * TODO: this should be changed when developing multi-clock support!!!
   */
  VTR_ASSERT(1 == operating_clock_ports.size());

  /* In this function, we support only 1 type of I/Os */
  VTR_ASSERT(1 == module_manager.module_ports_by_type(top_module, ModuleManager::MODULE_GPIO_PORT).size());
  BasicPort module_io_port = module_manager.module_ports_by_type(top_module, ModuleManager::MODULE_GPIO_PORT)[0];

  /* Keep tracking which I/Os have been used */
  std::vector<bool> io_used(module_io_port.get_width(), false);

  /* Print comments */
  fp << "##################################################" << std::endl; 
  fp << "# Create input and output delays for used I/Os    " << std::endl;
  fp << "##################################################" << std::endl; 

  for (const t_logical_block& io_lb : L_logical_blocks) {
    /* We only care I/O logical blocks !*/
    if ( (VPACK_INPAD != io_lb.type) && (VPACK_OUTPAD != io_lb.type) ) {
      continue;
    }

    /* clock net or constant generator should be disabled in timing analysis */
    if (TRUE == io_lb.is_clock) {
      continue;
    }

    /* Find the index of the mapped GPIO in top-level FPGA fabric */
    size_t io_index = find_benchmark_io_index(io_lb, device_size, L_grids, L_blocks);

    /* Ensure that IO index is in range */
    BasicPort module_mapped_io_port = module_io_port; 
    /* Set the port pin index */ 
    VTR_ASSERT(io_index < module_mapped_io_port.get_width());
    module_mapped_io_port.set_width(io_index, io_index);

    /* For input I/O, we set an input delay constraint correlated to the operating clock
     * For output I/O, we set an output delay constraint correlated to the operating clock
     */
    if (VPACK_INPAD == io_lb.type) {
      print_sdc_set_port_input_delay(fp, module_mapped_io_port,
                                     operating_clock_ports[0], critical_path_delay);
    } else {
      VTR_ASSERT(VPACK_OUTPAD == io_lb.type);
      print_sdc_set_port_output_delay(fp, module_mapped_io_port,
                                      operating_clock_ports[0], critical_path_delay);
    }

    /* Mark this I/O has been used/wired */
    io_used[io_index] = true;
  }

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Print comments */
  fp << "##################################################" << std::endl; 
  fp << "# Disable timing for unused I/Os    " << std::endl;
  fp << "##################################################" << std::endl; 

  /* Wire the unused iopads to a constant */
  for (size_t io_index = 0; io_index < io_used.size(); ++io_index) {
    /* Bypass used iopads */
    if (true == io_used[io_index]) {
      continue;
    }

    /* Wire to a contant */
    BasicPort module_unused_io_port = module_io_port; 
    /* Set the port pin index */ 
    module_unused_io_port.set_width(io_index, io_index);
    print_sdc_disable_port_timing(fp, module_unused_io_port);
  }

  /* Add an empty line as a splitter */
  fp << std::endl;
}

/********************************************************************
 * Disable the timing for all the global port except the operating clock ports
 *******************************************************************/
static 
void print_analysis_sdc_disable_global_ports(std::fstream& fp,
                                             const ModuleManager& module_manager,
                                             const ModuleId& top_module,
                                             const CircuitLibrary& circuit_lib,
                                             const std::vector<CircuitPortId>& global_ports) {
  /* Validate file stream */
  check_file_handler(fp);

  /* Print comments */
  fp << "##################################################" << std::endl; 
  fp << "# Disable timing for global ports                 " << std::endl;
  fp << "##################################################" << std::endl; 

  for (const CircuitPortId& global_port : global_ports) {
    /* Skip operating clock here! */
    if ( (SPICE_MODEL_PORT_CLOCK == circuit_lib.port_type(global_port)) 
      && (false == circuit_lib.port_is_prog(global_port)) ) {
      continue;
    }

    ModulePortId module_port = module_manager.find_module_port(top_module, circuit_lib.port_prefix(global_port)); 
    BasicPort port_to_disable = module_manager.module_port(top_module, module_port);

    print_sdc_disable_port_timing(fp, port_to_disable);
  }
}

/********************************************************************
 * Top-level function outputs a SDC file
 * that constrain a FPGA fabric (P&Red netlist) using a benchmark 
 *******************************************************************/
void print_analysis_sdc(const std::string& sdc_dir,
                        const float& critical_path_delay,
                        const DeviceRRGSB& L_device_rr_gsb, 
                        const std::vector<t_logical_block>& L_logical_blocks,
                        const vtr::Point<size_t>& device_size,
                        const std::vector<std::vector<t_grid_tile>>& L_grids, 
                        const std::vector<t_block>& L_blocks,
                        const ModuleManager& module_manager,
                        const CircuitLibrary& circuit_lib,
                        const std::vector<CircuitPortId>& global_ports,
                        const bool& compact_routing_hierarchy) {
  /* Create the file name for Verilog netlist */
  std::string sdc_fname(sdc_dir + std::string(SDC_ANALYSIS_FILE_NAME));

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for Timing/Power analysis on the mapped FPGA: %s ...",
             sdc_fname.c_str());

  /* Start time count */
  clock_t t_start = clock();

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  /* Validate file stream */
  check_file_handler(fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Constrain for Timing/Power analysis on the mapped FPGA"));

  /* Find the top_module */
  ModuleId top_module = module_manager.find_module(generate_fpga_top_module_name());
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Create clock and set I/O ports with input/output delays */
  print_analysis_sdc_io_delays(fp,
                               L_logical_blocks, device_size, L_grids, L_blocks,
                               module_manager, top_module, 
                               circuit_lib, global_ports,
                               critical_path_delay);

  /* Disable the timing for global ports */
  print_analysis_sdc_disable_global_ports(fp,
                                          module_manager, top_module,
                                          circuit_lib, global_ports);

  /* Disable the timing for configuration cells */ 
  rec_print_pnr_sdc_disable_configurable_memory_module_output(fp, 
                                                              module_manager, top_module, 
                                                              format_dir_path(module_manager.module_name(top_module)));


  /* TODO: Disable timing for unused routing resources in connection blocks */
  print_analysis_sdc_disable_unused_cbs(fp, L_grids, 
                                        module_manager,
                                        L_device_rr_gsb, 
                                        compact_routing_hierarchy);

  /* TODO: Disable timing for unused routing resources in switch blocks */
  /*
  if (TRUE == compact_routing_hierarchy) {
    verilog_generate_sdc_disable_unused_sbs(fp); 
    verilog_generate_sdc_disable_unused_sbs_muxs(fp);
  } else {
    verilog_generate_sdc_disable_unused_sbs(fp, LL_nx, LL_ny); 
    verilog_generate_sdc_disable_unused_sbs_muxs(fp, LL_nx, LL_ny);
  }
   */

  /* TODO: Disable timing for unused routing resources in grids (programmable blocks) */
  /*
  verilog_generate_sdc_disable_unused_grids(fp, LL_nx, LL_ny, LL_grid, LL_block);
  verilog_generate_sdc_disable_unused_grids_muxs(fp, LL_nx, LL_ny, LL_grid, LL_block);
   */

  /* Close file handler */
  fp.close();

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}
