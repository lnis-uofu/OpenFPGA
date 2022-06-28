/********************************************************************
 * This file includes functions that print SDC (Synopsys Design Constraint) 
 * files in physical design tools, i.e., Place & Route (PnR) tools
 * The SDC files are used to constrain global ports for each module
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
#include "openfpga_scale.h"

#include "sdc_writer_naming.h"
#include "sdc_writer_utils.h"
#include "pnr_sdc_global_port.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print SDC constraint for a clock port
 * This format is derived from the open-source SDC syntax,
 * which is supposed to be generic 
 *
 * This function is design to the SDC writer for any port
 * wants to be treated as a clock port
 *******************************************************************/
static 
void print_pnr_sdc_clock_port(std::fstream& fp, 
                              const BasicPort& port_to_constrain,
                              const float& clock_period) { 
  valid_file_stream(fp);

  fp << "create_clock";
  fp << " -name " << generate_sdc_port(port_to_constrain);
  fp << " -period " << std::setprecision(10) << clock_period;
  fp << " -waveform {0 " << std::setprecision(10) << clock_period / 2 << "}";
  fp << " [get_ports {" << generate_sdc_port(port_to_constrain) << "}]";
  fp << std::endl;
}        

/********************************************************************
 * Print SDC constraints for the clock ports which are the global ports 
 * of FPGA fabric
 * 
 * For programming clock, we give a fixed period, while for operating
 * clock, we constrain with critical path delay 
 *******************************************************************/
static 
void print_pnr_sdc_global_clock_ports(std::fstream& fp, 
                                      const float& time_unit,
                                      const ModuleManager& module_manager,
                                      const ModuleId& top_module,
                                      const FabricGlobalPortInfo& fabric_global_port_info,
                                      const SimulationSetting& sim_setting) {

  valid_file_stream(fp);

  /* Get clock port from the global port */
  for (const FabricGlobalPortId& global_port : fabric_global_port_info.global_ports()) {
    if (false == fabric_global_port_info.global_port_is_clock(global_port)) {
      continue;
    }
    /* Reach here, it means a clock port and we need print constraints */
    float clock_period = 1./sim_setting.default_operating_clock_frequency();

    /* For programming clock, we give a fixed period */
    if (true == fabric_global_port_info.global_port_is_prog(global_port)) {
      clock_period =  1./sim_setting.programming_clock_frequency();
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

    BasicPort clock_port = module_manager.module_port(top_module, fabric_global_port_info.global_module_port(global_port));
    for (const size_t& pin : clock_port.pins()) {
      BasicPort port_to_constrain(clock_port.get_name(), pin, pin);

      /* Should try to find a port defintion from simulation parameters
       * If found, it means that we need to use special clock name! 
       */
      for (const SimulationClockId& sim_clock : sim_setting.operating_clocks()) { 
        if (port_to_constrain == sim_setting.clock_port(sim_clock)) {
          clock_period = 1./sim_setting.clock_frequency(sim_clock);
        }
      }

      print_pnr_sdc_clock_port(fp, 
                               port_to_constrain,
                               clock_period / time_unit);
    }
  }
}

/********************************************************************
 * Print SDC constraints for the non-clock ports which are the global ports 
 * of FPGA fabric
 * Here, we will the treat the non-clock ports as the clock ports
 * in the CTS
 * Note that, this may be applied to the reset, set and other global
 * signals which do need very balanced delays to each sink
 *******************************************************************/
static 
void print_pnr_sdc_global_non_clock_ports(std::fstream& fp, 
                                          const float& time_unit,
                                          const float& operating_critical_path_delay,
                                          const ModuleManager& module_manager,
                                          const ModuleId& top_module,
                                          const FabricGlobalPortInfo& fabric_global_port_info) {

  valid_file_stream(fp);

  /* For non-clock port from the global port: give a fixed period */
  for (const FabricGlobalPortId& global_port : fabric_global_port_info.global_ports()) {
    if (true == fabric_global_port_info.global_port_is_clock(global_port)) {
      continue;
    }

    /* Print comments */
    fp << "##################################################" << std::endl; 
    fp << "# Constrain other global ports                    " << std::endl;
    fp << "##################################################" << std::endl; 

    /* Reach here, it means a non-clock global port and we need print constraints */
    float clock_period = operating_critical_path_delay; 
    BasicPort non_clock_port = module_manager.module_port(top_module, fabric_global_port_info.global_module_port(global_port));
    for (const size_t& pin : non_clock_port.pins()) {
      BasicPort port_to_constrain(non_clock_port.get_name(), pin, pin);

      print_pnr_sdc_clock_port(fp, 
                               port_to_constrain,
                               clock_period / time_unit);
    }
  }
}

/********************************************************************
 * Print a SDC file to constrain the global ports of FPGA fabric
 * in particular clock ports
 *
 * This ports to appear in this file will be treated in Clock Tree
 * Synthesis (CTS)
 * 
 * For non-clock global ports, we have an option to select if they 
 * should be treated in CTS or not
 * In general, we do not recommend to do this
 *******************************************************************/
void print_pnr_sdc_global_ports(const PnrSdcOption& options,
                                const ModuleManager& module_manager,
                                const ModuleId& top_module,
                                const FabricGlobalPortInfo& global_ports,
                                const SimulationSetting& sim_setting) {

  std::string sdc_dir = options.sdc_dir();
  float time_unit = options.time_unit();
  bool include_time_stamp = options.time_stamp();
  bool constrain_non_clock_port = options.constrain_non_clock_global_port();

  /* Create the file name for Verilog netlist */
  std::string sdc_fname(sdc_dir + std::string(SDC_GLOBAL_PORTS_FILE_NAME));

  /* Start time count */
  std::string timer_message = std::string("Write SDC for constraining clocks for P&R flow '") + sdc_fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp,
                        std::string("Clock contraints for PnR"),
                        include_time_stamp);

  /* Print time unit for the SDC file */
  print_sdc_timescale(fp, time_unit_to_string(time_unit));

  print_pnr_sdc_global_clock_ports(fp, time_unit, 
                                   module_manager, top_module,
                                   global_ports, sim_setting);

  if (true == constrain_non_clock_port) {
    print_pnr_sdc_global_non_clock_ports(fp, time_unit, 
                                         1./sim_setting.default_operating_clock_frequency(),
                                         module_manager, top_module,
                                         global_ports);

  }

  /* Close file handler */
  fp.close();
}

} /* end namespace openfpga */
