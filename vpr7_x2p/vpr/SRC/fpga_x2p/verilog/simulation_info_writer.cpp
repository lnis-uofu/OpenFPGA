/***********************************/
/*  Synthesizable Verilog Dumping  */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <map>
#define MINI_CASE_SENSITIVE
#include "ini.h"

/* Include vpr structs*/
#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph.h"
#include "vpr_utils.h"
#include "path_delay.h"
#include "stats.h"

/* Include FPGA-SPICE utils */
#include "linkedlist.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_globals.h"

/* Include verilog utils */
#include "verilog_global.h"
#include "verilog_utils.h"
#include "simulation_info_writer.h"

// Infile  Function
static float get_verilog_modelsim_simulation_time_period(const float &time_unit,
                                                         const int &num_prog_clock_cycles,
                                                         const float &prog_clock_period,
                                                         const int &num_op_clock_cycles,
                                                         const float &op_clock_period)
{
  float total_time_period = 0.;

  /* Take into account the prog_reset and reset cycles */
  total_time_period = (num_prog_clock_cycles + 2) * prog_clock_period + num_op_clock_cycles * op_clock_period;
  total_time_period = total_time_period / time_unit;

  return total_time_period;
}

/***** Top-level function *****/
void print_verilog_simulation_info(const int &num_operating_clock_cycles,
                                   const std::string &verilog_dir_formatted,
                                   const std::string &chomped_circuit_name,
                                   const std::string &src_dir_path,
                                   const size_t &num_program_clock_cycles,
                                   const float &prog_clock_freq,
                                   const float &op_clock_freq)
{
  mINI::INIStructure ini;
  // std::map<char, int> units_map;
  // units_map['s']=1;  // units_map['ms']=1E-3;  // units_map['us']=1E-6;
  // units_map['ns']=1E-9;  // units_map['ps']=1E-12;  // units_map['fs']=1E-15;

  /* Compute simulation time period */
  float simulation_time_period = get_verilog_modelsim_simulation_time_period(1E-3,
                                                                             num_program_clock_cycles,
                                                                             1. / prog_clock_freq,
                                                                             num_operating_clock_cycles,
                                                                             1. / op_clock_freq);

  ini["SIMULATION_DECK"]["PROJECTNAME "] = "ModelSimProject";
  ini["SIMULATION_DECK"]["BENCHMARK "] = chomped_circuit_name;
  ini["SIMULATION_DECK"]["TOP_TB"] = chomped_circuit_name + std::string("_top_formal_verification_random_tb");
  ini["SIMULATION_DECK"]["SIMTIME "] = std::to_string(simulation_time_period);
  ini["SIMULATION_DECK"]["UNIT "] = "ms";
  ini["SIMULATION_DECK"]["VERILOG_PATH "] = std::string(src_dir_path);
  ini["SIMULATION_DECK"]["VERILOG_FILE1"] = std::string(defines_verilog_file_name);
  ini["SIMULATION_DECK"]["VERILOG_FILE2"] = std::string(chomped_circuit_name + "_include_netlists.v");

  mINI::INIFile file("SimulationDeckInfo.ini");
  file.generate(ini, true);
}