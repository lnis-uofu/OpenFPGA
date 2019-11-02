/*********************************************************************
 * This function includes the writer for generating exchangeable
 * information, in order to interface different simulators   
 ********************************************************************/
#include <math.h>
#include <time.h>
#include <map>
#define MINI_CASE_SENSITIVE
#include "ini.h"

/* Include vpr structs*/
#include "util.h"

/* Include FPGA-SPICE utils */
#include "simulation_utils.h"

/* Include verilog utils */
#include "verilog_global.h"
#include "simulation_info_writer.h"

/*********************************************************************
 * Top-level function to write an ini file which contains exchangeable
 * information, in order to interface different Verilog simulators
 ********************************************************************/
void print_verilog_simulation_info(const std::string& simulation_ini_filename,
                                   const std::string& parent_dir,
                                   const std::string& verilog_dir,
                                   const std::string& circuit_name,
                                   const std::string& src_dir,
                                   const size_t& num_program_clock_cycles,
                                   const int& num_operating_clock_cycles,
                                   const float& prog_clock_freq,
                                   const float& op_clock_freq) {
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
  ini["SIMULATION_DECK"]["TOP_TB"] = circuit_name + std::string("_top_formal_verification_random_tb");
  ini["SIMULATION_DECK"]["SIMTIME "] = std::to_string(simulation_time_period);
  ini["SIMULATION_DECK"]["UNIT "] = "ms";
  ini["SIMULATION_DECK"]["VERILOG_PATH "] = std::string(src_dir);
  ini["SIMULATION_DECK"]["VERILOG_FILE1"] = std::string(defines_verilog_file_name);
  ini["SIMULATION_DECK"]["VERILOG_FILE2"] = std::string(circuit_name + "_include_netlists.v");

  /* Use default name if user does not provide one */
  std::string ini_fname;
  if (true == simulation_ini_filename.empty()) {
    ini_fname = parent_dir + std::string("SimulationDeckInfo.ini");
  } else {
    ini_fname = simulation_ini_filename;
  }

  mINI::INIFile file(ini_fname);
  file.generate(ini, true);
}
