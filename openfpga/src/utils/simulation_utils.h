#ifndef SIMULATION_UTILS_H
#define SIMULATION_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int find_operating_phase_simulation_time(const int& factor,
                                         const int& num_op_clock_cycles,
                                         const float& op_clock_period,
                                         const float& timescale);

float find_simulation_time_period(const float& time_unit,
                                  const int& num_prog_clock_cycles,
                                  const float& prog_clock_period,
                                  const int& num_op_clock_cycles,
                                  const float& op_clock_period);

} /* end namespace openfpga */

#endif
