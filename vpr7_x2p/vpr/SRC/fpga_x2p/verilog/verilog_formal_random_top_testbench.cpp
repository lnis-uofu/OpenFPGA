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
    BasicPort clock_port = generate_verilog_testbench_clock_port(clock_port_names, std::string(DEFAULT_CLOCK_NAME));
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
  print_verilog_testbench_random_stimuli(fp, simulation_parameters, L_logical_blocks, 
                                         std::string(CHECKFLAG_PORT_POSTFIX),
                                         clock_port_names, std::string(DEFAULT_CLOCK_NAME));

  print_verilog_testbench_check(fp, 
                                std::string(autochecked_simulation_flag),
                                std::string(FORMAL_TB_SIM_START_PORT_NAME),
                                std::string(BENCHMARK_PORT_POSTFIX),
                                std::string(FPGA_PORT_POSTFIX),
                                std::string(CHECKFLAG_PORT_POSTFIX),
                                std::string(ERROR_COUNTER),
                                L_logical_blocks, clock_port_names, std::string(DEFAULT_CLOCK_NAME));

  int simulation_time = find_operating_phase_simulation_time(MAGIC_NUMBER_FOR_SIMULATION_TIME,
                                                             simulation_parameters.meas_params.sim_num_clock_cycle,
                                                             1./simulation_parameters.stimulate_params.op_clock_freq,
                                                             verilog_sim_timescale);

  /* Add Icarus requirement */
  print_verilog_timeout_and_vcd(fp, 
                                std::string(icarus_simulator_flag),
                                std::string(circuit_name + std::string(FORMAL_RANDOM_TOP_TESTBENCH_POSTFIX)),
                                std::string(circuit_name + std::string("_formal.vcd")), 
                                std::string(FORMAL_TB_SIM_START_PORT_NAME),
                                std::string(ERROR_COUNTER),
                                simulation_time);

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

