/********************************************************************
 * This file includes functions that are used to generate a Verilog 
 * testbench for the top-level module (FPGA fabric), in purpose of
 * running formal verification with random input vectors 
 *******************************************************************/
#include <fstream>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iomanip>

/* Include external library headers*/
#include "device_port.h"
#include "vtr_assert.h"
#include "util.h"

/* Include VPR headers*/

/* Include FPGA-X2P headers*/
#include "simulation_utils.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_benchmark_utils.h"

/* Include FPGA Verilog headers*/
#include "verilog_global.h"
#include "verilog_writer_utils.h"
#include "verilog_testbench_utils.h"
#include "verilog_formal_random_top_testbench.h"

/********************************************************************
 * Local variables used only in this file
 *******************************************************************/
constexpr char* FORMAL_RANDOM_TOP_TESTBENCH_POSTFIX = "_top_formal_verification_random_tb";
constexpr char* FPGA_PORT_POSTFIX = "_gfpga";
constexpr char* BENCHMARK_PORT_POSTFIX = "_bench";
constexpr char* CHECKFLAG_PORT_POSTFIX = "_flag";
constexpr char* DEFAULT_CLOCK_NAME = "clk";
constexpr char* BENCHMARK_INSTANCE_NAME = "REF_DUT";
constexpr char* FPGA_INSTANCE_NAME = "FPGA_DUT";
constexpr char* ERROR_COUNTER = "nb_error";
constexpr char* FORMAL_TB_SIM_START_PORT_NAME = "sim_start";
constexpr int MAGIC_NUMBER_FOR_SIMULATION_TIME = 200;

/********************************************************************
 * Generate the clock port name to be used in this testbench
 *
 * Restrictions:
 * Assume this is a single clock benchmark
 *******************************************************************/
static 
BasicPort generate_verilog_top_clock_port(const std::vector<std::string>& clock_port_names) {
  if (0 == clock_port_names.size()) {
    return BasicPort(std::string(DEFAULT_CLOCK_NAME), 1); 
  }

  VTR_ASSERT(1 == clock_port_names.size());
  return BasicPort(clock_port_names[0], 1); 
}

/********************************************************************
 * Print the module ports for the Verilog testbench 
 * using random vectors 
 * This function generates 
 * 1. the input ports to drive both input benchmark module and FPGA fabric module
 * 2. the output ports for input benchmark module
 * 3. the output ports for FPGA fabric module
 * 4. the error checking ports 
 *******************************************************************/
static 
void print_verilog_top_random_testbench_ports(std::fstream& fp,
                                              const std::string& circuit_name,
                                              const std::vector<std::string>& clock_port_names,
                                              const std::vector<t_logical_block>& L_logical_blocks) {
  /* Validate the file stream */
  check_file_handler(fp);
 
  /* Print the declaration for the module */
  fp << "module " << circuit_name << FORMAL_RANDOM_TOP_TESTBENCH_POSTFIX << ";" << std::endl;

  /* Instantiate register for inputs stimulis */
  print_verilog_comment(fp, std::string("----- Shared inputs -------"));
  for (const t_logical_block& lb : L_logical_blocks) {
    /* We care only those logic blocks which are input I/Os */
    if (VPACK_INPAD != lb.type) { 
      continue;
    }
   
    /* Each logical block assumes a single-width port */
    BasicPort input_port(std::string(lb.name), 1); 
    fp << "\t" << generate_verilog_port(VERILOG_PORT_REG, input_port) << ";" << std::endl;
  }

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Create a clock port if the benchmark does not have one! 
   * The clock is used for counting and synchronizing input stimulus 
   */
  if (0 == clock_port_names.size()) {
    BasicPort clock_port = generate_verilog_top_clock_port(clock_port_names);
    print_verilog_comment(fp, std::string("----- Default clock port is added here since benchmark does not contain one -------"));
    fp << "\t" << generate_verilog_port(VERILOG_PORT_REG, clock_port) << ";" << std::endl;
  }

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Instantiate wires for FPGA fabric outputs */
  print_verilog_comment(fp, std::string("----- FPGA fabric outputs -------"));

  for (const t_logical_block& lb : L_logical_blocks) {
    /* We care only those logic blocks which are input I/Os */
    if (VPACK_OUTPAD != lb.type) { 
      continue;
    }

    /* Each logical block assumes a single-width port */
    BasicPort output_port(std::string(std::string(lb.name) + std::string(FPGA_PORT_POSTFIX)), 1); 
    fp << "\t" << generate_verilog_port(VERILOG_PORT_WIRE, output_port) << ";" << std::endl;
  }

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Benchmark is instanciated conditionally: only when a preprocessing flag is enable */
  print_verilog_preprocessing_flag(fp, std::string(autochecked_simulation_flag)); 

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Instantiate wire for benchmark output */
  print_verilog_comment(fp, std::string("----- Benchmark outputs -------"));
  for (const t_logical_block& lb : L_logical_blocks) {
    /* We care only those logic blocks which are input I/Os */
    if (VPACK_OUTPAD != lb.type) { 
      continue;
    }

    /* Each logical block assumes a single-width port */
    BasicPort output_port(std::string(std::string(lb.name) + std::string(BENCHMARK_PORT_POSTFIX)), 1); 
    fp << "\t" << generate_verilog_port(VERILOG_PORT_WIRE, output_port) << ";" << std::endl;
  }

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Instantiate register for output comparison */
  print_verilog_comment(fp, std::string("----- Output vectors checking flags -------"));
  for (const t_logical_block& lb : L_logical_blocks) {
    /* We care only those logic blocks which are input I/Os */
    if (VPACK_OUTPAD != lb.type) { 
      continue;
    }

    /* Each logical block assumes a single-width port */
    BasicPort output_port(std::string(std::string(lb.name) + std::string(CHECKFLAG_PORT_POSTFIX)), 1); 
    fp << "\t" << generate_verilog_port(VERILOG_PORT_REG, output_port) << ";" << std::endl;
  }

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Condition ends for the benchmark instanciation */
  print_verilog_endif(fp);

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Instantiate an integer to count the number of error 
   * and determine if the simulation succeed or failed 
   */
  print_verilog_comment(fp, std::string("----- Error counter -------"));
  fp << "\tinteger " << ERROR_COUNTER << "= 0;" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Instanciate the input benchmark module
 *******************************************************************/
static
void print_verilog_top_random_testbench_benchmark_instance(std::fstream& fp, 
                                                           const std::string& reference_verilog_top_name,
                                                           const std::vector<t_logical_block>& L_logical_blocks) {
  /* Validate the file stream */
  check_file_handler(fp);

  /* Benchmark is instanciated conditionally: only when a preprocessing flag is enable */
  print_verilog_preprocessing_flag(fp, std::string(autochecked_simulation_flag)); 

  print_verilog_comment(fp, std::string("----- Reference Benchmark Instanication -------"));

  /* Do NOT use explicit port mapping here: 
   * VPR added a prefix of "out_" to the output ports of input benchmark
   */
  print_verilog_testbench_benchmark_instance(fp, reference_verilog_top_name,
                                             std::string(BENCHMARK_INSTANCE_NAME),
                                             std::string(),
                                             std::string(),
                                             std::string(BENCHMARK_PORT_POSTFIX), L_logical_blocks,
                                             false);

  print_verilog_comment(fp, std::string("----- End reference Benchmark Instanication -------"));

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Condition ends for the benchmark instanciation */
  print_verilog_endif(fp);

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Print Verilog codes to set up a timeout for the simulation 
 * and dump the waveform to VCD files
 *
 * Note that: these codes are tuned for Icarus simulator!!!
 *******************************************************************/
static
void print_verilog_timeout_and_vcd(std::fstream& fp,
                                   const std::string& circuit_name,
                                   const int& simulation_time) {
  /* Validate the file stream */
  check_file_handler(fp);

  /* The following verilog codes are tuned for Icarus */
  print_verilog_preprocessing_flag(fp, std::string(icarus_simulator_flag)); 

  print_verilog_comment(fp, std::string("----- Begin Icarus requirement -------"));

  fp << "\tinitial begin" << std::endl;
  fp << "\t\t$dumpfile(\"" << circuit_name << "_formal.vcd\");" << std::endl;
  fp << "\t\t$dumpvars(1, " << circuit_name << FORMAL_RANDOM_TOP_TESTBENCH_POSTFIX << ");" << std::endl;
  fp << "\tend" << std::endl;

  /* Condition ends for the Icarus requirement */
  print_verilog_endif(fp);

  print_verilog_comment(fp, std::string("----- END Icarus requirement -------"));

  /* Add an empty line as splitter */
  fp << std::endl;

  BasicPort sim_start_port(std::string(FORMAL_TB_SIM_START_PORT_NAME), 1);

  fp << "initial begin" << std::endl;
  fp << "\t" << generate_verilog_port(VERILOG_PORT_CONKT, sim_start_port) << " <= 1'b1;" << std::endl;
  fp << "\t$timeformat(-9, 2, \"ns\", 20);" << std::endl;
  fp << "\t$display(\"Simulation start\");" << std::endl;
  print_verilog_comment(fp, std::string("----- Can be changed by the user for his/her need -------"));
  fp << "\t#" << simulation_time << std::endl;
  fp << "\tif(" << ERROR_COUNTER << " == 0) begin" << std::endl;
  fp << "\t\t$display(\"Simulation Succeed\");" << std::endl;
  fp << "\tend else begin" << std::endl;
  fp << "\t\t$display(\"Simulation Failed with " << std::string("%d") << " error(s)\", " << ERROR_COUNTER << ");" << std::endl;
  fp << "\tend" << std::endl;
  fp << "\t$finish;" << std::endl;
  fp << "end" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Print Verilog codes to check the equivalence of output vectors 
 *
 * Restriction: this function only supports single clock benchmarks!
 *******************************************************************/
static
void print_verilog_top_random_testbench_check(std::fstream& fp,
                                              const std::vector<t_logical_block>& L_logical_blocks,
                                              const std::vector<std::string>& clock_port_names) {

  /* Validate the file stream */
  check_file_handler(fp);

  /* Add output autocheck conditionally: only when a preprocessing flag is enable */
  print_verilog_preprocessing_flag(fp, std::string(autochecked_simulation_flag)); 

  print_verilog_comment(fp, std::string("----- Begin checking output vectors -------"));

  BasicPort clock_port = generate_verilog_top_clock_port(clock_port_names);

  print_verilog_comment(fp, std::string("----- Skip the first falling edge of clock, it is for initialization -------"));

  BasicPort sim_start_port(std::string(FORMAL_TB_SIM_START_PORT_NAME), 1);

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
     fp << "\t\t\tif(!(" << std::string(lb.name) << std::string(FPGA_PORT_POSTFIX);
     fp << " === " << std::string(lb.name) << std::string(BENCHMARK_PORT_POSTFIX);
     fp << ") && !(" << std::string(lb.name) << std::string(BENCHMARK_PORT_POSTFIX);
     fp << " === 1'bx)) begin" << std::endl;
     fp << "\t\t\t\t" << std::string(lb.name) << std::string(CHECKFLAG_PORT_POSTFIX) << " <= 1'b1;" << std::endl;
     fp << "\t\t\tend else begin" << std::endl;
     fp << "\t\t\t\t" << std::string(lb.name) << std::string(CHECKFLAG_PORT_POSTFIX) << "<= 1'b0;" << std::endl;
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

    fp << "\talways@(posedge " << std::string(lb.name) << std::string(CHECKFLAG_PORT_POSTFIX) << ") begin" << std::endl;
    fp << "\t\tif(" << std::string(lb.name) << std::string(CHECKFLAG_PORT_POSTFIX) << ") begin" << std::endl;
    fp << "\t\t\t" << ERROR_COUNTER << " = " << ERROR_COUNTER << " + 1;" << std::endl;
    fp << "\t\t\t$display(\"Mismatch on " << std::string(lb.name) << std::string(FPGA_PORT_POSTFIX) << " at time = " << std::string("%t") << "\", $realtime);" << std::endl;
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
 * Instanciate the FPGA fabric module
 *******************************************************************/
static
void print_verilog_random_testbench_fpga_instance(std::fstream& fp,
                                                  const std::string& circuit_name,
                                                  const std::vector<t_logical_block>& L_logical_blocks) {
  /* Validate the file stream */
  check_file_handler(fp);

  print_verilog_comment(fp, std::string("----- FPGA fabric instanciation -------"));

  /* Always use explicit port mapping */
  print_verilog_testbench_benchmark_instance(fp, std::string(circuit_name + std::string(formal_verification_top_postfix)),
                                             std::string(FPGA_INSTANCE_NAME),
                                             std::string(formal_verification_top_module_port_postfix),
                                             std::string(formal_verification_top_module_port_postfix),
                                             std::string(FPGA_PORT_POSTFIX), L_logical_blocks,
                                             true);

  print_verilog_comment(fp, std::string("----- End FPGA Fabric Instanication -------"));

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Generate random stimulus for the input ports
 *******************************************************************/
static
void print_verilog_top_random_stimuli(std::fstream& fp,
                                      const t_spice_params& simulation_parameters,
                                      const std::vector<t_logical_block>& L_logical_blocks,
                                      const std::vector<std::string>& clock_port_names) {
  /* Validate the file stream */
  check_file_handler(fp);

  print_verilog_comment(fp, std::string("----- Initialization -------"));

  fp << "\tinitial begin" << std::endl;
  /* Create clock stimuli */
  BasicPort clock_port = generate_verilog_top_clock_port(clock_port_names);
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
    BasicPort output_port(std::string(std::string(lb.name) + std::string(CHECKFLAG_PORT_POSTFIX)), 1); 
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

/*********************************************************************
 * Top-level function in this file:
 * Create a Verilog testbench using random input vectors 
 * The testbench consists of two modules, i.e., Design Under Test (DUT)
 * 1. top-level module of FPGA fabric
 * 2. top-level module of users' benchmark, 
 *    i.e., the input benchmark of VPR flow
 *                                    +----------+
 *                                    |   FPGA   |       +------------+
 *                             +----->|  Fabric  |------>|            |
 *                             |      |          |       |            |
 *                             |      +----------+       |            |
 *                             |                         | Output     | 
 *   random_input_vectors -----+                         | Vector     |---->Functional correct?
 *                             |                         | Comparator |    
 *                             |      +-----------+      |            |
 *                             |      |  Input    |      |            |
 *                             +----->| Benchmark |----->|            |
 *                                    +-----------+      +------------+
 *
 * Same input vectors are given to drive both DUTs.
 * The output vectors of the DUTs are compared to verify if they
 * have the same functionality.
 * A flag will be raised to indicate the result 
 ********************************************************************/
void print_verilog_random_top_testbench(const std::string& circuit_name,
                                        const std::string& verilog_fname,
                                        const std::string& verilog_dir,
                                        const std::vector<t_logical_block>& L_logical_blocks,
                                        const t_syn_verilog_opts& fpga_verilog_opts,
                                        const t_spice_params& simulation_parameters) {
  vpr_printf(TIO_MESSAGE_INFO, 
             "Writing Random Testbench for FPGA Top-level Verilog netlist for  %s...", 
             circuit_name.c_str());

  /* Start time count */
  clock_t t_start = clock();

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_handler(fp);

  /* Generate a brief description on the Verilog file*/
  std::string title = std::string("FPGA Verilog Testbench for Formal Top-level netlist of Design: ") + circuit_name;
  print_verilog_file_header(fp, title); 

  /* Print preprocessing flags and external netlists */
  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  print_verilog_include_netlist(fp, std::string(verilog_dir + std::string(defines_verilog_simulation_file_name)));  

  print_verilog_include_netlist(fp, std::string(fpga_verilog_opts.reference_verilog_benchmark_file));

  /* Preparation: find all the clock ports */
  std::vector<std::string> clock_port_names = find_benchmark_clock_port_name(L_logical_blocks);

  /* Start of testbench */
  print_verilog_top_random_testbench_ports(fp, circuit_name, clock_port_names, L_logical_blocks);

  /* Call defined top-level module */
  print_verilog_random_testbench_fpga_instance(fp, circuit_name, L_logical_blocks);

  /* Call defined benchmark */
  print_verilog_top_random_testbench_benchmark_instance(fp, circuit_name, L_logical_blocks);

  /* Add stimuli for reset, set, clock and iopad signals */
  print_verilog_top_random_stimuli(fp, simulation_parameters, L_logical_blocks, clock_port_names);

  print_verilog_top_random_testbench_check(fp, L_logical_blocks, clock_port_names);

  int simulation_time = find_operating_phase_simulation_time(MAGIC_NUMBER_FOR_SIMULATION_TIME,
                                                             simulation_parameters.meas_params.sim_num_clock_cycle,
                                                             1./simulation_parameters.stimulate_params.op_clock_freq,
                                                             verilog_sim_timescale);

  /* Add Icarus requirement */
  print_verilog_timeout_and_vcd(fp, circuit_name, simulation_time);

  /* Testbench ends*/
  print_verilog_module_end(fp, std::string(circuit_name) + std::string(FORMAL_RANDOM_TOP_TESTBENCH_POSTFIX));

  /* Close the file stream */
  fp.close();

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}

