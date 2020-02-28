/*********************************************************************
 * This function includes the writer for generating exchangeable
 * information, in order to interface different simulators   
 ********************************************************************/
#include <cmath>
#include <ctime>
#include <map>
#define MINI_CASE_SENSITIVE
#include "ini.h"

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_time.h"

#include "simulation_utils.h"

#include "verilog_constants.h"
#include "simulation_info_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Top-level function to write an ini file which contains exchangeable
 * information, in order to interface different Verilog simulators
 ********************************************************************/
void print_verilog_simulation_info(const std::string& ini_fname,
                                   const std::string& circuit_name,
                                   const std::string& src_dir,
                                   const size_t& num_program_clock_cycles,
                                   const int& num_operating_clock_cycles,
                                   const float& prog_clock_freq,
                                   const float& op_clock_freq) {

  std::string timer_message = std::string("Write exchangeable file containing simulation information '") + ini_fname + std::string("'");

  /* Start time count */
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Use default name if user does not provide one */
  VTR_ASSERT(true != ini_fname.empty());

  mINI::INIStructure ini;
  // std::map<char, int> units_map;
  // units_map['s']=1;  // units_map['ms']=1E-3;  // units_map['us']=1E-6;
  // units_map['ns']=1E-9;  // units_map['ps']=1E-12;  // units_map['fs']=1E-15;

  /* Compute simulation time period */
  float simulation_time_period = find_simulation_time_period(1E-3,
                                                             num_program_clock_cycles,
                                                             1. / prog_clock_freq,
                                                             num_operating_clock_cycles,
                                                             1. / op_clock_freq);
  ini["SIMULATION_DECK"]["PROJECTNAME "] = "ModelSimProject";
  ini["SIMULATION_DECK"]["BENCHMARK "] = circuit_name;
  ini["SIMULATION_DECK"]["TOP_TB"] = circuit_name + std::string(FORMAL_RANDOM_TOP_TESTBENCH_POSTFIX);
  ini["SIMULATION_DECK"]["SIMTIME "] = std::to_string(simulation_time_period);
  ini["SIMULATION_DECK"]["UNIT "] = "ms";
  ini["SIMULATION_DECK"]["VERILOG_PATH "] = std::string(src_dir);
  ini["SIMULATION_DECK"]["VERILOG_FILE1"] = std::string(DEFINES_VERILOG_FILE_NAME);
  ini["SIMULATION_DECK"]["VERILOG_FILE2"] = std::string(circuit_name + std::string(TOP_INCLUDE_NETLIST_FILE_NAME_POSTFIX));

  mINI::INIFile file(ini_fname);
  file.generate(ini, true);
}

} /* end namespace openfpga */
