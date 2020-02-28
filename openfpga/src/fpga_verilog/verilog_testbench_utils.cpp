/********************************************************************
 * This file includes most utilized functions that are used to create
 * Verilog testbenches
 *
 * Note: please try to avoid using global variables in this file
 * so that we can make it free to use anywhere
 *******************************************************************/
#include <iomanip>

/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"
#include "openfpga_digest.h"

#include "verilog_port_types.h"

#include "verilog_constants.h"
#include "verilog_writer_utils.h"
#include "verilog_testbench_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print an instance of the FPGA top-level module
 *******************************************************************/
void print_verilog_testbench_fpga_instance(std::fstream& fp,
                                           const ModuleManager& module_manager,
                                           const ModuleId& top_module,
                                           const std::string& top_instance_name) {
  /* Validate the file stream */
  valid_file_stream(fp);

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
                                                const AtomContext& atom_ctx,
                                                const VprNetlistAnnotation& netlist_annotation,
                                                const bool& use_explicit_port_map) {
  /* Validate the file stream */
  valid_file_stream(fp);

  fp << "\t" << module_name << " " << instance_name << "(" << std::endl;

  size_t port_counter = 0;
  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-I/O atom blocks ! */
    if ( (AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk))
      && (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) ) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    } 

    /* The first port does not need a comma */
    if(0 < port_counter){
      fp << "," << std::endl;
    }
    /* Input port follows the logical block name while output port requires a special postfix */
    if (AtomBlockType::INPAD == atom_ctx.nlist.block_type(atom_blk)) {
      fp << "\t\t";
      if (true == use_explicit_port_map) {
        fp << "." << block_name << module_input_port_postfix << "(";
      }
      fp << block_name;
      if (true == use_explicit_port_map) {
        fp << ")";
      }
    } else {
      VTR_ASSERT_SAFE(AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk));
      fp << "\t\t";
      if (true == use_explicit_port_map) {
        fp << "." << block_name << module_output_port_postfix << "(";
      }
      fp << block_name << output_port_postfix;
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
                                              const AtomContext& atom_ctx,
                                              const PlacementContext& place_ctx,
                                              const IoLocationMap& io_location_map,
                                              const VprNetlistAnnotation& netlist_annotation,
                                              const std::string& io_input_port_name_postfix,
                                              const std::string& io_output_port_name_postfix,
                                              const size_t& unused_io_value) {
  /* Validate the file stream */
  valid_file_stream(fp);

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

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-I/O atom blocks ! */
    if ( (AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk))
      && (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) ) {
      continue;
    }

    /* Find the index of the mapped GPIO in top-level FPGA fabric */
    size_t io_index = io_location_map.io_index(place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.x,
                                               place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.y,
                                               place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.z);

    /* Ensure that IO index is in range */
    BasicPort module_mapped_io_port = module_io_port; 
    /* Set the port pin index */ 
    VTR_ASSERT(io_index < module_mapped_io_port.get_width());
    module_mapped_io_port.set_width(io_index, io_index);

    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    } 

    /* Create the port for benchmark I/O, due to BLIF benchmark, each I/O always has a size of 1 
     * In addition, the input and output ports may have different postfix in naming
     * due to verification context! Here, we give full customization on naming 
     */
    BasicPort benchmark_io_port;
    if (AtomBlockType::INPAD == atom_ctx.nlist.block_type(atom_blk)) {
      benchmark_io_port.set_name(std::string(block_name + io_input_port_name_postfix)); 
      benchmark_io_port.set_width(1);
      print_verilog_comment(fp, std::string("----- Blif Benchmark input " + block_name + " is mapped to FPGA IOPAD " + module_mapped_io_port.get_name() + "[" + std::to_string(io_index) + "] -----"));
      print_verilog_wire_connection(fp, module_mapped_io_port, benchmark_io_port, false);
    } else {
      VTR_ASSERT(AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk));
      benchmark_io_port.set_name(std::string(block_name + io_output_port_name_postfix)); 
      benchmark_io_port.set_width(1);
      print_verilog_comment(fp, std::string("----- Blif Benchmark output " + block_name + " is mapped to FPGA IOPAD " + module_mapped_io_port.get_name() + "[" + std::to_string(io_index) + "] -----"));
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
  valid_file_stream(fp);

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
                                   const AtomContext& atom_ctx,
                                   const VprNetlistAnnotation& netlist_annotation,
                                   const std::vector<std::string>& clock_port_names,
                                   const std::string& default_clock_name) {

  /* Validate the file stream */
  valid_file_stream(fp);

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

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-I/O atom blocks ! */
    if ( (AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk))
      && (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) ) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    } 

    if (AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk)) {
     fp << "\t\t\tif(!(" << block_name << fpga_port_postfix;
     fp << " === " << block_name << benchmark_port_postfix;
     fp << ") && !(" << block_name << benchmark_port_postfix;
     fp << " === 1'bx)) begin" << std::endl;
     fp << "\t\t\t\t" << block_name << check_flag_port_postfix << " <= 1'b1;" << std::endl;
     fp << "\t\t\tend else begin" << std::endl;
     fp << "\t\t\t\t" << block_name << check_flag_port_postfix << "<= 1'b0;" << std::endl;
     fp << "\t\t\tend" << std::endl; 
    }
  } 
  fp << "\t\tend" << std::endl;
  fp << "\tend" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Only care about output atom blocks ! */
    if (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    } 

    fp << "\talways@(posedge " << block_name << check_flag_port_postfix << ") begin" << std::endl;
    fp << "\t\tif(" << block_name << check_flag_port_postfix << ") begin" << std::endl;
    fp << "\t\t\t" << error_counter_name << " = " << error_counter_name << " + 1;" << std::endl;
    fp << "\t\t\t$display(\"Mismatch on " << block_name << fpga_port_postfix << " at time = " << std::string("%t") << "\", $realtime);" << std::endl;
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
 * Generate random stimulus for the clock port
 * This function is designed to drive the clock port of a benchmark module
 * If there is no clock port found, we will give a default clock name
 * In such case, this clock will not be wired to the benchmark module
 * but be only used as a synchronizer in verification
 *******************************************************************/
void print_verilog_testbench_clock_stimuli(std::fstream& fp,
                                           const SimulationSetting& simulation_parameters,
                                           const BasicPort& clock_port) {
  /* Validate the file stream */
  valid_file_stream(fp);

  print_verilog_comment(fp, std::string("----- Clock Initialization -------"));

  fp << "\tinitial begin" << std::endl;
  /* Create clock stimuli */
  fp << "\t\t" << generate_verilog_port(VERILOG_PORT_CONKT, clock_port) << " <= 1'b0;" << std::endl;
  fp << "\t\twhile(1) begin" << std::endl;
  fp << "\t\t\t#" << std::setprecision(10) << ((0.5/simulation_parameters.operating_clock_frequency())/VERILOG_SIM_TIMESCALE) << std::endl;
  fp << "\t\t\t" << generate_verilog_port(VERILOG_PORT_CONKT, clock_port);
  fp << " <= !";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, clock_port);
  fp << ";" << std::endl;
  fp << "\t\tend" << std::endl;

  fp << "\tend" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Generate random stimulus for the input ports (non-clock signals)
 * For clock signals, please use print_verilog_testbench_clock_stimuli
 *******************************************************************/
void print_verilog_testbench_random_stimuli(std::fstream& fp,
                                            const AtomContext& atom_ctx,
                                            const VprNetlistAnnotation& netlist_annotation,
                                            const std::string& check_flag_port_postfix,
                                            const BasicPort& clock_port) {
  /* Validate the file stream */
  valid_file_stream(fp);

  print_verilog_comment(fp, std::string("----- Input Initialization -------"));

  fp << "\tinitial begin" << std::endl;

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-I/O atom blocks ! */
    if ( (AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk))
      && (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) ) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    } 

    /* TODO: find the clock inputs will be initialized later */
    if (AtomBlockType::INPAD == atom_ctx.nlist.block_type(atom_blk)) {
      fp << "\t\t" << block_name << " <= 1'b0;" << std::endl;
    }
  }

  /* Add an empty line as splitter */
  fp << std::endl;
  
  /* Set 0 to registers for checking flags */
  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-I/O atom blocks ! */
    if (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    } 

    /* Each logical block assumes a single-width port */
    BasicPort output_port(std::string(block_name + check_flag_port_postfix), 1); 
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

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-I/O atom blocks ! */
    if ( (AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk))
      && (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) ) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    } 

    /* TODO: find the clock inputs will be initialized later */
    if (AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk)) {
      fp << "\t\t" << block_name << " <= $random;" << std::endl;
    }
  }

  fp << "\tend" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Print Verilog declaration of shared ports appear in testbenches
 * which are 
 * 1. the shared input ports (registers) to drive both 
 *    FPGA fabric and benchmark instance
 * 2. the output ports (wires) for both FPGA fabric and benchmark instance
 * 3. the checking flag ports to evaluate if outputs matches under the
 *    same input vectors
 *******************************************************************/
void print_verilog_testbench_shared_ports(std::fstream& fp,
                                          const AtomContext& atom_ctx,
                                          const VprNetlistAnnotation& netlist_annotation,
                                          const std::vector<std::string>& clock_port_names,
                                          const std::string& benchmark_output_port_postfix,
                                          const std::string& fpga_output_port_postfix,
                                          const std::string& check_flag_port_postfix,
                                          const std::string& autocheck_preprocessing_flag) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Instantiate register for inputs stimulis */
  print_verilog_comment(fp, std::string("----- Shared inputs -------"));
  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* We care only those logic blocks which are input I/Os */
    if (AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk)) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    } 

    /* TODO: Skip clocks because they are handled in another function */
    if (clock_port_names.end() != std::find(clock_port_names.begin(), clock_port_names.end(), block_name)) {
      continue;
    }
   
    /* Each logical block assumes a single-width port */
    BasicPort input_port(block_name, 1); 
    fp << "\t" << generate_verilog_port(VERILOG_PORT_REG, input_port) << ";" << std::endl;
  }

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Instantiate wires for FPGA fabric outputs */
  print_verilog_comment(fp, std::string("----- FPGA fabric outputs -------"));

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* We care only those logic blocks which are output I/Os */
    if (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    } 

    /* Each logical block assumes a single-width port */
    BasicPort output_port(std::string(block_name + fpga_output_port_postfix), 1); 
    fp << "\t" << generate_verilog_port(VERILOG_PORT_WIRE, output_port) << ";" << std::endl;
  }

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Benchmark is instanciated conditionally: only when a preprocessing flag is enable */
  print_verilog_preprocessing_flag(fp, std::string(autocheck_preprocessing_flag)); 

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Instantiate wire for benchmark output */
  print_verilog_comment(fp, std::string("----- Benchmark outputs -------"));
  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* We care only those logic blocks which are output I/Os */
    if (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    } 

    /* Each logical block assumes a single-width port */
    BasicPort output_port(std::string(block_name + benchmark_output_port_postfix), 1); 
    fp << "\t" << generate_verilog_port(VERILOG_PORT_WIRE, output_port) << ";" << std::endl;
  }

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Instantiate register for output comparison */
  print_verilog_comment(fp, std::string("----- Output vectors checking flags -------"));
  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* We care only those logic blocks which are output I/Os */
    if (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    } 

    /* Each logical block assumes a single-width port */
    BasicPort output_port(std::string(block_name + check_flag_port_postfix), 1); 
    fp << "\t" << generate_verilog_port(VERILOG_PORT_REG, output_port) << ";" << std::endl;
  }

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Condition ends for the benchmark instanciation */
  print_verilog_endif(fp);

  /* Add an empty line as splitter */
  fp << std::endl;
}

} /* end namespace openfpga */
