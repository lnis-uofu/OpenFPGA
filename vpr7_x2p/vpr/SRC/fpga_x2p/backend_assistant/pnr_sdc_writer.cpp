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
  std::string sdc_fname(sdc_dir + std::string(SDC_CLOCK_FILE_NAME));

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
                   const std::vector<CircuitPortId>& global_ports) {
  
  /* Part 1. Constrain global ports */
  if (true == sdc_options.constrain_global_port()) {
    print_pnr_sdc_global_ports(sdc_options.sdc_dir(), critical_path_delay, circuit_lib, global_ports);
  }
}
