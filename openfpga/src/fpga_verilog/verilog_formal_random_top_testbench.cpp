/********************************************************************
 * This file includes functions that are used to generate a Verilog 
 * testbench for the top-level module (FPGA fabric), in purpose of
 * running formal verification with random input vectors 
 *******************************************************************/
#include <fstream>
#include <cstring>
#include <cmath>
#include <iomanip>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"
#include "openfpga_digest.h"

#include "openfpga_atom_netlist_utils.h"
#include "simulation_utils.h"

#include "verilog_constants.h"
#include "verilog_writer_utils.h"
#include "verilog_testbench_utils.h"
#include "verilog_formal_random_top_testbench.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Local variables used only in this file
 *******************************************************************/
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
                                              const AtomContext& atom_ctx,
                                              const VprNetlistAnnotation& netlist_annotation) {
  /* Validate the file stream */
  valid_file_stream(fp);
 
  /* Print the declaration for the module */
  fp << "module " << circuit_name << FORMAL_RANDOM_TOP_TESTBENCH_POSTFIX << ";" << std::endl;

  /* Create a clock port if the benchmark does not have one! 
   * The clock is used for counting and synchronizing input stimulus 
   */
  BasicPort clock_port = generate_verilog_testbench_clock_port(clock_port_names, std::string(DEFAULT_CLOCK_NAME));
  print_verilog_comment(fp, std::string("----- Default clock port is added here since benchmark does not contain one -------"));
  fp << "\t" << generate_verilog_port(VERILOG_PORT_REG, clock_port) << ";" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;

  print_verilog_testbench_shared_ports(fp, atom_ctx, netlist_annotation,
                                       clock_port_names,
                                       std::string(BENCHMARK_PORT_POSTFIX),
                                       std::string(FPGA_PORT_POSTFIX),
                                       std::string(CHECKFLAG_PORT_POSTFIX),
                                       std::string(AUTOCHECKED_SIMULATION_FLAG));

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
                                                           const AtomContext& atom_ctx,
                                                           const VprNetlistAnnotation& netlist_annotation,
                                                           const bool& explicit_port_mapping) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Benchmark is instanciated conditionally: only when a preprocessing flag is enable */
  print_verilog_preprocessing_flag(fp, std::string(AUTOCHECKED_SIMULATION_FLAG)); 

  print_verilog_comment(fp, std::string("----- Reference Benchmark Instanication -------"));

  /* Do NOT use explicit port mapping here: 
   * VPR added a prefix of "out_" to the output ports of input benchmark
   */
  std::vector<std::string> prefix_to_remove;
  prefix_to_remove.push_back(std::string(VPR_BENCHMARK_OUT_PORT_PREFIX));
  prefix_to_remove.push_back(std::string(OPENFPGA_BENCHMARK_OUT_PORT_PREFIX));
  print_verilog_testbench_benchmark_instance(fp, reference_verilog_top_name,
                                             std::string(BENCHMARK_INSTANCE_NAME),
                                             std::string(),
                                             std::string(),
                                             prefix_to_remove,
                                             std::string(BENCHMARK_PORT_POSTFIX),
                                             atom_ctx, netlist_annotation,
                                             explicit_port_mapping);

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
                                                  const AtomContext& atom_ctx,
                                                  const VprNetlistAnnotation& netlist_annotation,
                                                  const bool& explicit_port_mapping) {
  /* Validate the file stream */
  valid_file_stream(fp);

  print_verilog_comment(fp, std::string("----- FPGA fabric instanciation -------"));

  /* Always use explicit port mapping */
  print_verilog_testbench_benchmark_instance(fp, std::string(circuit_name + std::string(FORMAL_VERIFICATION_TOP_MODULE_POSTFIX)),
                                             std::string(FPGA_INSTANCE_NAME),
                                             std::string(FORMAL_VERIFICATION_TOP_MODULE_PORT_POSTFIX),
                                             std::string(FORMAL_VERIFICATION_TOP_MODULE_PORT_POSTFIX),
                                             std::vector<std::string>(),
                                             std::string(FPGA_PORT_POSTFIX),
                                             atom_ctx, netlist_annotation,
                                             explicit_port_mapping);

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
                                        const AtomContext& atom_ctx,
                                        const VprNetlistAnnotation& netlist_annotation,
                                        const SimulationSetting& simulation_parameters,
                                        const bool& explicit_port_mapping) {
  std::string timer_message = std::string("Write configuration-skip testbench for FPGA top-level Verilog netlist implemented by '") + circuit_name.c_str() + std::string("'");

  /* Start time count */
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(verilog_fname.c_str(), fp);

  /* Generate a brief description on the Verilog file*/
  std::string title = std::string("FPGA Verilog Testbench for Formal Top-level netlist of Design: ") + circuit_name;
  print_verilog_file_header(fp, title); 

  /* Preparation: find all the clock ports */
  std::vector<std::string> clock_port_names = find_atom_netlist_clock_port_names(atom_ctx.nlist, netlist_annotation);

  /* Start of testbench */
  print_verilog_top_random_testbench_ports(fp, circuit_name, clock_port_names, atom_ctx, netlist_annotation);

  /* Call defined top-level module */
  print_verilog_random_testbench_fpga_instance(fp, circuit_name,
                                               atom_ctx, netlist_annotation,
                                               explicit_port_mapping);

  /* Call defined benchmark */
  print_verilog_top_random_testbench_benchmark_instance(fp, circuit_name,
                                                        atom_ctx, netlist_annotation,
                                                        explicit_port_mapping);

  /* Find clock port to be used */
  BasicPort clock_port = generate_verilog_testbench_clock_port(clock_port_names, std::string(DEFAULT_CLOCK_NAME));

  /* Add stimuli for reset, set, clock and iopad signals */
  print_verilog_testbench_clock_stimuli(fp, simulation_parameters, 
                                        clock_port);
  print_verilog_testbench_random_stimuli(fp, atom_ctx,
                                         netlist_annotation, 
                                         clock_port_names, 
                                         std::string(CHECKFLAG_PORT_POSTFIX),
                                         clock_port);

  print_verilog_testbench_check(fp, 
                                std::string(AUTOCHECKED_SIMULATION_FLAG),
                                std::string(FORMAL_TB_SIM_START_PORT_NAME),
                                std::string(BENCHMARK_PORT_POSTFIX),
                                std::string(FPGA_PORT_POSTFIX),
                                std::string(CHECKFLAG_PORT_POSTFIX),
                                std::string(ERROR_COUNTER),
                                atom_ctx,
                                netlist_annotation, 
                                clock_port_names,
                                std::string(DEFAULT_CLOCK_NAME));

  float simulation_time = find_operating_phase_simulation_time(MAGIC_NUMBER_FOR_SIMULATION_TIME,
                                                               simulation_parameters.num_clock_cycles(),
                                                               1./simulation_parameters.operating_clock_frequency(),
                                                               VERILOG_SIM_TIMESCALE);

  /* Add Icarus requirement */
  print_verilog_timeout_and_vcd(fp, 
                                std::string(ICARUS_SIMULATOR_FLAG),
                                std::string(circuit_name + std::string(FORMAL_RANDOM_TOP_TESTBENCH_POSTFIX)),
                                std::string(circuit_name + std::string("_formal.vcd")), 
                                std::string(FORMAL_TB_SIM_START_PORT_NAME),
                                std::string(ERROR_COUNTER),
                                simulation_time);

  /* Testbench ends*/
  print_verilog_module_end(fp, std::string(circuit_name) + std::string(FORMAL_RANDOM_TOP_TESTBENCH_POSTFIX));

  /* Close the file stream */
  fp.close();
}

} /* end namespace openfpga */
