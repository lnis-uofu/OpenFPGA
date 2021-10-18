/********************************************************************
 * This file include most utilized functions in generating simulations
 * Note: function placed here MUST be generic enough for both SPICE
 * and Verilog simulations!
 *******************************************************************/
#include <cmath>

#include "simulation_utils.h"

/* begin namespace openfpga */
namespace openfpga {


/********************************************************************
 * Compute the time period for the simulation
 *******************************************************************/
float find_operating_phase_simulation_time(const int& num_op_clock_cycles,
                                           const float& op_clock_period,
                                           const float& timescale) {
  /* Take into account the prog_reset and reset cycles 
   * 1e9 is to change the unit to ns rather than second 
   */
  return ((float)num_op_clock_cycles * op_clock_period) / timescale; 
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

  /* Take into account 
   * - the prog_reset
   * - the gap clock cycle between programming and operating phase
   * - 2 reset cycles before operating phase starts
   * This is why the magic number 2 and 2 are added
   */
  total_time_period = ((float)num_prog_clock_cycles + 2) * prog_clock_period 
                    + ((float)num_op_clock_cycles + 2) * op_clock_period;
  total_time_period = total_time_period / time_unit;

  return total_time_period;
}

} /* end namespace openfpga */
