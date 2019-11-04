/********************************************************************
 * This file includes most utilized functions that are used to create
 * Verilog testbenches
 *
 * Note: please try to avoid using global variables in this file
 * so that we can make it free to use anywhere
 *******************************************************************/
#include <iomanip>

#include "vtr_assert.h"
#include "device_port.h"

#include "fpga_x2p_utils.h"
#include "fpga_x2p_benchmark_utils.h"

#include "verilog_writer_utils.h"
#include "verilog_testbench_utils.h"

/********************************************************************
 * Print an instance of the FPGA top-level module
 *******************************************************************/
void print_verilog_testbench_fpga_instance(std::fstream& fp,
                                           const ModuleManager& module_manager,
                                           const ModuleId& top_module,
                                           const std::string& top_instance_name) {
  /* Validate the file stream */
  check_file_handler(fp);

  /* Include defined top-level module */
  print_verilog_comment(fp, std::string("----- FPGA top-level module to be capsulated -----"));

  /* Create an empty port-to-port name mapping, because we use default names */
  std::map<std::string, BasicPort> port2port_name_map;

  /* Use explicit port mapping for a clean instanciation */
  print_verilog_module_instance(fp, module_manager, top_module, 
                                top_instance_name, 
                                port2port_name_map, true); 

  /* Add an empty line as a splitter */
  fp << std::endl;
}

/********************************************************************
 * Instanciate the input benchmark module
 *******************************************************************/
void print_verilog_testbench_benchmark_instance(std::fstream& fp,
                                                const std::string& module_name,
                                                const std::string& instance_name,
                                                const std::string& module_input_port_postfix,
                                                const std::string& module_output_port_postfix,
                                                const std::string& output_port_postfix,
                                                const std::vector<t_logical_block>& L_logical_blocks,
                                                const bool& use_explicit_port_map) {
  /* Validate the file stream */
  check_file_handler(fp);

  fp << "\t" << module_name << " " << instance_name << "(" << std::endl;

  size_t port_counter = 0;
  for (const t_logical_block& lb : L_logical_blocks) {
    /* Bypass non-I/O logical blocks ! */
    if ( (VPACK_INPAD != lb.type) && (VPACK_OUTPAD != lb.type) ) {
      continue;
    }
    /* The first port does not need a comma */
    if(0 < port_counter){
      fp << "," << std::endl;
    }
    /* Input port follows the logical block name while output port requires a special postfix */
    if (VPACK_INPAD == lb.type){
      fp << "\t\t";
      if (true == use_explicit_port_map) {
        fp << "." << std::string(lb.name) << module_input_port_postfix << "(";
      }
      fp << std::string(lb.name);
      if (true == use_explicit_port_map) {
        fp << ")";
      }
    } else {
      VTR_ASSERT_SAFE(VPACK_OUTPAD == lb.type);
      fp << "\t\t";
      if (true == use_explicit_port_map) {
        fp << "." << std::string(lb.name) << module_output_port_postfix << "(";
      }
      fp << std::string(lb.name) << output_port_postfix;
      if (true == use_explicit_port_map) {
        fp << ")";
      }
    }
    /* Update the counter */
    port_counter++;
  }
  fp << "\t);" << std::endl;
}

/********************************************************************
 * This function adds stimuli to I/Os of FPGA fabric
 * 1. For mapped I/Os, this function will wire them to the input ports
 *    of the pre-configured FPGA top module
 * 2. For unmapped I/Os, this function will assign a constant value
 *    by default 
 *******************************************************************/
void print_verilog_testbench_connect_fpga_ios(std::fstream& fp,
                                              const ModuleManager& module_manager,
                                              const ModuleId& top_module,
                                              const std::vector<t_logical_block>& L_logical_blocks,
                                              const vtr::Point<size_t>& device_size,
                                              const std::vector<std::vector<t_grid_tile>>& L_grids, 
                                              const std::vector<t_block>& L_blocks,
                                              const std::string& io_port_name_postfix,
                                              const size_t& unused_io_value) {
  /* Validate the file stream */
  check_file_handler(fp);

  /* In this function, we support only 1 type of I/Os */
  VTR_ASSERT(1 == module_manager.module_ports_by_type(top_module, ModuleManager::MODULE_GPIO_PORT).size());
  BasicPort module_io_port = module_manager.module_ports_by_type(top_module, ModuleManager::MODULE_GPIO_PORT)[0];

  /* Keep tracking which I/Os have been used */
  std::vector<bool> io_used(module_io_port.get_width(), false);

  /* See if this I/O should be wired to a benchmark input/output */
  /* Add signals from blif benchmark and short-wire them to FPGA I/O PADs
   * This brings convenience to checking functionality  
   */
  print_verilog_comment(fp, std::string("----- Link BLIF Benchmark I/Os to FPGA I/Os -----"));
  for (const t_logical_block& io_lb : L_logical_blocks) {
    /* We only care I/O logical blocks !*/
    if ( (VPACK_INPAD != io_lb.type) && (VPACK_OUTPAD != io_lb.type) ) {
      continue;
    }

    /* Find the index of the mapped GPIO in top-level FPGA fabric */
    size_t io_index = find_benchmark_io_index(io_lb, device_size, L_grids, L_blocks);

    /* Ensure that IO index is in range */
    BasicPort module_mapped_io_port = module_io_port; 
    /* Set the port pin index */ 
    VTR_ASSERT(io_index < module_mapped_io_port.get_width());
    module_mapped_io_port.set_width(io_index, io_index);

    /* Create the port for benchmark I/O, due to BLIF benchmark, each I/O always has a size of 1 */
    BasicPort benchmark_io_port(std::string(std::string(io_lb.name)+ io_port_name_postfix), 1); 

    print_verilog_comment(fp, std::string("----- Blif Benchmark inout " + std::string(io_lb.name) + " is mapped to FPGA IOPAD " + module_mapped_io_port.get_name() + "[" + std::to_string(io_index) + "] -----"));
    if (VPACK_INPAD == io_lb.type) {
      print_verilog_wire_connection(fp, module_mapped_io_port, benchmark_io_port, false);
    } else {
      VTR_ASSERT(VPACK_OUTPAD == io_lb.type);
      print_verilog_wire_connection(fp, benchmark_io_port, module_mapped_io_port, false);
    }

    /* Mark this I/O has been used/wired */
    io_used[io_index] = true;
  }

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Wire the unused iopads to a constant */
  print_verilog_comment(fp, std::string("----- Wire unused FPGA I/Os to constants -----"));
  for (size_t io_index = 0; io_index < io_used.size(); ++io_index) {
    /* Bypass used iopads */
    if (true == io_used[io_index]) {
      continue;
    }

    /* Wire to a contant */
    BasicPort module_unused_io_port = module_io_port; 
    /* Set the port pin index */ 
    module_unused_io_port.set_width(io_index, io_index);

    std::vector<size_t> default_values(module_unused_io_port.get_width(), unused_io_value);
    print_verilog_wire_constant_values(fp, module_unused_io_port, default_values);
  }

  /* Add an empty line as a splitter */
  fp << std::endl;
}

/********************************************************************
 * Print Verilog codes to set up a timeout for the simulation 
 * and dump the waveform to VCD files
 *
 * Note that: these codes are tuned for Icarus simulator!!!
 *******************************************************************/
void print_verilog_timeout_and_vcd(std::fstream& fp,
                                   const std::string& icarus_preprocessing_flag,
                                   const std::string& module_name,
                                   const std::string& vcd_fname,
                                   const std::string& simulation_start_counter_name,
                                   const std::string& error_counter_name,
                                   const int& simulation_time) {
  /* Validate the file stream */
  check_file_handler(fp);

  /* The following verilog codes are tuned for Icarus */
  print_verilog_preprocessing_flag(fp, icarus_preprocessing_flag); 

  print_verilog_comment(fp, std::string("----- Begin Icarus requirement -------"));

  fp << "\tinitial begin" << std::endl;
  fp << "\t\t$dumpfile(\"" << vcd_fname << "\");" << std::endl;
  fp << "\t\t$dumpvars(1, " << module_name << ");" << std::endl;
  fp << "\tend" << std::endl;

  /* Condition ends for the Icarus requirement */
  print_verilog_endif(fp);

  print_verilog_comment(fp, std::string("----- END Icarus requirement -------"));

  /* Add an empty line as splitter */
  fp << std::endl;

  BasicPort sim_start_port(simulation_start_counter_name, 1);

  fp << "initial begin" << std::endl;
  fp << "\t" << generate_verilog_port(VERILOG_PORT_CONKT, sim_start_port) << " <= 1'b1;" << std::endl;
  fp << "\t$timeformat(-9, 2, \"ns\", 20);" << std::endl;
  fp << "\t$display(\"Simulation start\");" << std::endl;
  print_verilog_comment(fp, std::string("----- Can be changed by the user for his/her need -------"));
  fp << "\t#" << simulation_time << std::endl;
  fp << "\tif(" << error_counter_name << " == 0) begin" << std::endl;
  fp << "\t\t$display(\"Simulation Succeed\");" << std::endl;
  fp << "\tend else begin" << std::endl;
  fp << "\t\t$display(\"Simulation Failed with " << std::string("%d") << " error(s)\", " << error_counter_name << ");" << std::endl;
  fp << "\tend" << std::endl;
  fp << "\t$finish;" << std::endl;
  fp << "end" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Generate the clock port name to be used in this testbench
 *
 * Restrictions:
 * Assume this is a single clock benchmark
 *******************************************************************/
BasicPort generate_verilog_testbench_clock_port(const std::vector<std::string>& clock_port_names,
                                                const std::string& default_clock_name) {
  if (0 == clock_port_names.size()) {
    return BasicPort(default_clock_name, 1); 
  }

  VTR_ASSERT(1 == clock_port_names.size());
  return BasicPort(clock_port_names[0], 1); 
}

/********************************************************************
 * Print Verilog codes to check the equivalence of output vectors 
 *
 * Restriction: this function only supports single clock benchmarks!
 *******************************************************************/
void print_verilog_testbench_check(std::fstream& fp,
                                   const std::string& autochecked_preprocessing_flag,
                                   const std::string& simulation_start_counter_name,
                                   const std::string& benchmark_port_postfix,
                                   const std::string& fpga_port_postfix,
                                   const std::string& check_flag_port_postfix,
                                   const std::string& error_counter_name,
                                   const std::vector<t_logical_block>& L_logical_blocks,
                                   const std::vector<std::string>& clock_port_names,
                                   const std::string& default_clock_name) {

  /* Validate the file stream */
  check_file_handler(fp);

  /* Add output autocheck conditionally: only when a preprocessing flag is enable */
  print_verilog_preprocessing_flag(fp, autochecked_preprocessing_flag); 

  print_verilog_comment(fp, std::string("----- Begin checking output vectors -------"));

  BasicPort clock_port = generate_verilog_testbench_clock_port(clock_port_names, default_clock_name);

  print_verilog_comment(fp, std::string("----- Skip the first falling edge of clock, it is for initialization -------"));

  BasicPort sim_start_port(simulation_start_counter_name, 1);

  fp << "\t" << generate_verilog_port(VERILOG_PORT_REG, sim_start_port) << ";" << std::endl;
  fp << std::endl;

  fp << "\talways@(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, clock_port) << ") begin" << std::endl;
  fp << "\t\tif (1'b1 == " << generate_verilog_port(VERILOG_PORT_CONKT, sim_start_port) << ") begin" << std::endl;
  fp << "\t\t";
  print_verilog_register_connection(fp, sim_start_port, sim_start_port, true);
  fp << "\t\tend else begin" << std::endl;

  for (const t_logical_block& lb : L_logical_blocks) {
    /* Bypass non-I/O logical blocks ! */
    if ( (VPACK_INPAD != lb.type) && (VPACK_OUTPAD != lb.type) ) {
      continue;
    }

    if (VPACK_OUTPAD == lb.type){
     fp << "\t\t\tif(!(" << std::string(lb.name) << fpga_port_postfix;
     fp << " === " << std::string(lb.name) << benchmark_port_postfix;
     fp << ") && !(" << std::string(lb.name) << benchmark_port_postfix;
     fp << " === 1'bx)) begin" << std::endl;
     fp << "\t\t\t\t" << std::string(lb.name) << check_flag_port_postfix << " <= 1'b1;" << std::endl;
     fp << "\t\t\tend else begin" << std::endl;
     fp << "\t\t\t\t" << std::string(lb.name) << check_flag_port_postfix << "<= 1'b0;" << std::endl;
     fp << "\t\t\tend" << std::endl; 
    }
  } 
  fp << "\t\tend" << std::endl;
  fp << "\tend" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;

  for (const t_logical_block& lb : L_logical_blocks) {
    /* Bypass non-I/O logical blocks ! */
    if (VPACK_OUTPAD != lb.type) {
      continue;
    }

    fp << "\talways@(posedge " << std::string(lb.name) << check_flag_port_postfix << ") begin" << std::endl;
    fp << "\t\tif(" << std::string(lb.name) << check_flag_port_postfix << ") begin" << std::endl;
    fp << "\t\t\t" << error_counter_name << " = " << error_counter_name << " + 1;" << std::endl;
    fp << "\t\t\t$display(\"Mismatch on " << std::string(lb.name) << fpga_port_postfix << " at time = " << std::string("%t") << "\", $realtime);" << std::endl;
    fp << "\t\tend" << std::endl;
    fp << "\tend" << std::endl;

    /* Add an empty line as splitter */
    fp << std::endl;
  }

  /* Condition ends */
  print_verilog_endif(fp);

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Generate random stimulus for the input ports
 *******************************************************************/
void print_verilog_testbench_random_stimuli(std::fstream& fp,
                                            const t_spice_params& simulation_parameters,
                                            const std::vector<t_logical_block>& L_logical_blocks,
                                            const std::string& check_flag_port_postfix,
                                            const std::vector<std::string>& clock_port_names,
                                            const std::string& default_clock_name) {
  /* Validate the file stream */
  check_file_handler(fp);

  print_verilog_comment(fp, std::string("----- Initialization -------"));

  fp << "\tinitial begin" << std::endl;
  /* Create clock stimuli */
  BasicPort clock_port = generate_verilog_testbench_clock_port(clock_port_names, default_clock_name);
  fp << "\t\t" << generate_verilog_port(VERILOG_PORT_CONKT, clock_port) << " <= 1'b0;" << std::endl;
  fp << "\t\twhile(1) begin" << std::endl;
  fp << "\t\t\t#" << std::setprecision(2) << ((0.5/simulation_parameters.stimulate_params.op_clock_freq)/verilog_sim_timescale) << std::endl;
  fp << "\t\t\t" << generate_verilog_port(VERILOG_PORT_CONKT, clock_port);
  fp << " <= !";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, clock_port);
  fp << ";" << std::endl;
  fp << "\t\tend" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;

  for (const t_logical_block& lb : L_logical_blocks) {
    /* Bypass non-I/O logical blocks ! */
    if ( (VPACK_INPAD != lb.type) && (VPACK_OUTPAD != lb.type) ) {
      continue;
    }

    /* Clock ports will be initialized later */
    if ( (VPACK_INPAD == lb.type) && (FALSE == lb.is_clock) ) {
      fp << "\t\t" << std::string(lb.name) << " <= 1'b0;" << std::endl;
    }
  }

  /* Add an empty line as splitter */
  fp << std::endl;
  
  /* Set 0 to registers for checking flags */
  for (const t_logical_block& lb : L_logical_blocks) {
    /* We care only those logic blocks which are input I/Os */
    if (VPACK_OUTPAD != lb.type) { 
      continue;
    }

    /* Each logical block assumes a single-width port */
    BasicPort output_port(std::string(std::string(lb.name) + check_flag_port_postfix), 1); 
    fp << "\t\t" << generate_verilog_port(VERILOG_PORT_CONKT, output_port) << " <= 1'b0;" << std::endl;
  }

  fp << "\tend" << std::endl;
  /* Finish initialization */

  /* Add an empty line as splitter */
  fp << std::endl;

  // Not ready yet to determine if input is reset
/*
  fprintf(fp, "//----- Reset Stimulis\n");      
  fprintf(fp, "  initial begin\n");
  fprintf(fp, "    #%.3f\n",(rand() % 10) + 0.001);
  fprintf(fp, "    %s <= !%s;\n", reset_input_name, reset_input_name);
  fprintf(fp, "    #%.3f\n",(rand() % 10) + 0.001);
  fprintf(fp, "    %s <= !%s;\n", reset_input_name, reset_input_name);
  fprintf(fp, "    while(1) begin\n");
  fprintf(fp, "      #%.3f\n", (rand() % 15) + 0.5);
  fprintf(fp, "      %s <= !%s;\n", reset_input_name, reset_input_name);
  fprintf(fp, "      #%.3f\n", (rand() % 10000) + 200);
  fprintf(fp, "      %s <= !%s;\n", reset_input_name, reset_input_name);
  fprintf(fp, "    end\n");
  fprintf(fp, "  end\n\n");  
*/

  print_verilog_comment(fp, std::string("----- Input Stimulus -------"));
  fp << "\talways@(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, clock_port) << ") begin" << std::endl;

  for (const t_logical_block& lb : L_logical_blocks) {
    /* Bypass non-I/O logical blocks ! */
    if ( (VPACK_INPAD != lb.type) && (VPACK_OUTPAD != lb.type) ) {
      continue;
    }

    /* Clock ports will be initialized later */
    if ( (VPACK_INPAD == lb.type) && (FALSE == lb.is_clock) ) {
      fp << "\t\t" << std::string(lb.name) << " <= $random;" << std::endl;
    }
  }

  fp << "\tend" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;
}

