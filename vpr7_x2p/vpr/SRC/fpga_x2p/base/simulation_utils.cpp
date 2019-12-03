/********************************************************************
 * This file include most utilized functions in generating simulations
 * Note: function placed here MUST be generic enough for both SPICE
 * and Verilog simulations!
 *******************************************************************/
#include <cmath>

#include "simulation_utils.h"

/********************************************************************
 * Compute the time period for the simulation
 *******************************************************************/
int find_operating_phase_simulation_time(const int& factor,
                                         const int& num_op_clock_cycles,
                                         const float& op_clock_period,
                                         const float& timescale) {
  /* Take into account the prog_reset and reset cycles 
   * 1e9 is to change the unit to ns rather than second 
   */
  return (factor * num_op_clock_cycles * op_clock_period) / timescale; 
}

/********************************************************************
 * Find the the full time period of a simulation, including
 * both the programming time and operating time
 * This is a generic function that can be used to generate simulation
 * time period for SPICE/Verilog simulators
 *******************************************************************/
float find_simulation_time_period(const float &time_unit,
                                  const int &num_prog_clock_cycles,
                                  const float &prog_clock_period,
                                  const int &num_op_clock_cycles,
                                  const float &op_clock_period) {
  float total_time_period = 0.;

  /* Take into account the prog_reset and reset cycles */
  total_time_period = (num_prog_clock_cycles + 2) * prog_clock_period + num_op_clock_cycles * op_clock_period;
  total_time_period = total_time_period / time_unit;

  return total_time_period;
}

