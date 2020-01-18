#include "vtr_assert.h"

#include "simulation_setting.h"

/************************************************************************
 * Member functions for class SimulationSetting
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
SimulationSetting::SimulationSetting() {
  return;
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
float SimulationSetting::operating_clock_freqency() const {
  return clock_frequencies_.x();
}

float SimulationSetting::programming_clock_freqency() const {
  return clock_frequencies_.y();
}

bool SimulationSetting::auto_select_num_clock_cycles() const {
  return 0 == num_clock_cycles_;
}

size_t SimulationSetting::num_clock_cycles() const {
  return num_clock_cycles_;
}

size_t SimulationSetting::operating_clock_frequency_slack() const {
  return operating_clock_frequency_slack_;
}

float SimulationSetting::simulation_temperature() const {
  return simulation_temperature_;
}

bool SimulationSetting::verbose_output() const {
  return verbose_output_;
}

bool SimulationSetting::capacitance_output() const {
  return capacitance_output_;
}

e_sim_accuracy_type SimulationSetting::simulation_accuracy_type() const {
  return simulation_accuracy_type_;
}

float SimulationSetting::simulation_accuracy() const {
  /* If fractional accuracy is selected, we give a zero accuracy */
  if (SIM_ACCURACY_FRAC == simulation_accuracy_type()) {
    return 0.;
  }
  return simulation_accuracy_;
}

bool SimulationSetting::fast_simulation() const {
  return fast_simulation_;
}

bool SimulationSetting::run_monte_carlo_simulation() const {
  return 0 == monte_carlo_simulation_points_;
}

size_t SimulationSetting::monte_carlo_simulation_points() const {
  return monte_carlo_simulation_points_;
}

float SimulationSetting::measure_slew_upper_threshold(const e_sim_signal_type& signal_type) const {
  VTR_ASSERT (true == valid_signal_threshold(slew_upper_thresholds_[signal_type]));
  return slew_upper_thresholds_[signal_type];
}

float SimulationSetting::measure_slew_lower_threshold(const e_sim_signal_type& signal_type) const {
  VTR_ASSERT (true == valid_signal_threshold(slew_lower_thresholds_[signal_type]));
  return slew_lower_thresholds_[signal_type];
}

float SimulationSetting::measure_delay_input_threshold(const e_sim_signal_type& signal_type) const {
  VTR_ASSERT (true == valid_signal_threshold(delay_input_thresholds_[signal_type]));
  return delay_input_thresholds_[signal_type];
}

float SimulationSetting::measure_delay_output_threshold(const e_sim_signal_type& signal_type) const {
  VTR_ASSERT (true == valid_signal_threshold(delay_output_thresholds_[signal_type]));
  return delay_output_thresholds_[signal_type];
}

e_sim_accuracy_type SimulationSetting::stimuli_clock_slew_type(const e_sim_signal_type& signal_type) const {
  return clock_slew_types_[signal_type];
}

float SimulationSetting::stimuli_clock_slew(const e_sim_signal_type& signal_type) const {
  /* If fractional accuracy is selected, we give a zero accuracy */
  if (SIM_ACCURACY_FRAC == stimuli_clock_slew_type(signal_type)) {
    return 0.;
  }
  return clock_slews_[signal_type];
}

e_sim_accuracy_type SimulationSetting::stimuli_input_slew_type(const e_sim_signal_type& signal_type) const {
  return input_slew_types_[signal_type];
}

float SimulationSetting::stimuli_input_slew(const e_sim_signal_type& signal_type) const {
  /* If fractional accuracy is selected, we give a zero accuracy */
  if (SIM_ACCURACY_FRAC == stimuli_input_slew_type(signal_type)) {
    return 0.;
  }
  return input_slews_[signal_type];
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/


/************************************************************************
 * Public Validators
 ***********************************************************************/
bool SimulationSetting::valid_signal_threshold(const float& threshold) const {
  return (0. < threshold) && (threshold < 1);
}
