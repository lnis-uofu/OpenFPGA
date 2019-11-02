/********************************************************************
 * This file includes functions that are used to create
 * an auto-check top-level testbench for a FPGA fabric
 *******************************************************************/
#include <ctime>
#include <fstream>

#include "vtr_assert.h"

#include "fpga_x2p_utils.h"

#include "verilog_writer_utils.h"

#include "verilog_autocheck_top_testbench.h"

/********************************************************************
 * The top-level function to generate a testbench, in order to verify: 
 * 1. Configuration phase of the FPGA fabric, where the bitstream is 
 *    loaded to the configuration protocol of the FPGA fabric
 * 2. Operating phase of the FPGA fabric, where input stimuli are
 *    fed to the I/Os of the FPGA fabric
 *******************************************************************/
void print_verilog_autocheck_top_testbench(const ModuleManager& module_manager,
                                           const BitstreamManager& bitstream_manager,
                                           const CircuitLibrary& circuit_lib,
                                           const std::vector<CircuitPortId>& global_ports,
                                           const std::vector<t_logical_block>& L_logical_blocks,
                                           const vtr::Point<size_t>& device_size,
                                           const std::vector<std::vector<t_grid_tile>>& L_grids, 
                                           const std::vector<t_block>& L_blocks,
                                           const std::string& circuit_name,
                                           const std::string& verilog_fname,
                                           const std::string& verilog_dir,
                                           const t_syn_verilog_opts& fpga_verilog_opts,
                                           const t_spice_params& simulation_parameters) {
  vpr_printf(TIO_MESSAGE_INFO, 
             "Writing Autocheck Testbench for FPGA Top-level Verilog netlist for %s...", 
             circuit_name.c_str());

  /* Start time count */
  clock_t t_start = clock();

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_handler(fp);

  /* Generate a brief description on the Verilog file*/
  std::string title = std::string("FPGA Verilog Testbench for Top-level netlist of Design: ") + circuit_name;
  print_verilog_file_header(fp, title); 

  /* Print preprocessing flags and external netlists */
  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Start of testbench */
  //dump_verilog_top_auto_testbench_ports(fp, cur_sram_orgz_info, circuit_name, fpga_verilog_opts);

  /* Call defined top-level module */
  //dump_verilog_top_testbench_call_top_module(cur_sram_orgz_info, fp, 
  //                                           circuit_name, is_explicit_mapping);

  /* Call defined benchmark */
  //dump_verilog_top_auto_testbench_call_benchmark(fp, circuit_name);

  /* Add stimuli for reset, set, clock and iopad signals */
  //dump_verilog_top_testbench_stimuli(cur_sram_orgz_info, fp, verilog);

  /* Add output autocheck */
  //dump_verilog_top_auto_testbench_check(fp);

  /* Add Icarus requirement */
  //dump_verilog_timeout_and_vcd(fp, circuit_name , verilog, cur_sram_orgz_info);

  /* Testbench ends*/
  //fprintf(fp, "endmodule\n");

  /* Close the file stream */
  fp.close();

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}
