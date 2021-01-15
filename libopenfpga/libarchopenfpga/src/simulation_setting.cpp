#include "vtr_assert.h"

#include "simulation_setting.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Member functions for class SimulationSetting
 ***********************************************************************/

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
SimulationSetting::simulation_clock_range SimulationSetting::clocks() const {
  return vtr::make_range(clock_ids_.begin(), clock_ids_.end());
}

/************************************************************************
 * Constructors
 ***********************************************************************/
SimulationSetting::SimulationSetting() {
  return;
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
float SimulationSetting::default_operating_clock_frequency() const {
  return default_clock_frequencies_.x();
}

float SimulationSetting::programming_clock_frequency() const {
  return default_clock_frequencies_.y();
}

size_t SimulationSetting::num_simulation_clock_cycles() const {
  return clock_ids_.size();
}

std::string SimulationSetting::clock_name(const SimulationClockId& clock_id) const {
  VTR_ASSERT(valid_clock_id(clock_id));
  return clock_names_[clock_id];
}

BasicPort SimulationSetting::clock_port(const SimulationClockId& clock_id) const {
  VTR_ASSERT(valid_clock_id(clock_id));
  return clock_ports_[clock_id];
}

float SimulationSetting::clock_frequency(const SimulationClockId& clock_id) const {
  VTR_ASSERT(valid_clock_id(clock_id));
  return clock_frequencies_[clock_id];
}

bool SimulationSetting::auto_select_num_clock_cycles() const {
  return 0 == num_clock_cycles_;
}

size_t SimulationSetting::num_clock_cycles() const {
  return num_clock_cycles_;
}

float SimulationSetting::operating_clock_frequency_slack() const {
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
  return clock_slews_[signal_type];
}

e_sim_accuracy_type SimulationSetting::stimuli_input_slew_type(const e_sim_signal_type& signal_type) const {
  return input_slew_types_[signal_type];
}

float SimulationSetting::stimuli_input_slew(const e_sim_signal_type& signal_type) const {
  return input_slews_[signal_type];
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void SimulationSetting::set_default_operating_clock_frequency(const float& clock_freq) {
  default_clock_frequencies_.set_x(clock_freq);
}

void SimulationSetting::set_programming_clock_frequency(const float& clock_freq) {
  default_clock_frequencies_.set_y(clock_freq);
}

SimulationClockId SimulationSetting::create_clock(const std::string& name) {
  /* Ensure a unique name for the clock definition */
  std::map<std::string, SimulationClockId>::iterator it = clock_name2ids_.find(name);
  if (it != clock_name2ids_.end()) {
    return SimulationClockId::INVALID();
  }

  /* This is a legal name. we can create a new id */
  SimulationClockId clock_id = SimulationClockId(clock_ids_.size());
  clock_ids_.push_back(clock_id);
  clock_names_.push_back(name);
  clock_ports_.emplace_back();
  clock_frequencies_.push_back(0.);

  /* Register in the name-to-id map */
  clock_name2ids_[name] = clock_id;

  return clock_id;
}

void SimulationSetting::set_clock_port(const SimulationClockId& clock_id,
                                       const BasicPort& port) {
  VTR_ASSERT(valid_clock_id(clock_id));
  clock_ports_[clock_id] = port;
}

void SimulationSetting::set_clock_frequency(const SimulationClockId& clock_id,
                                            const float& frequency) {
  VTR_ASSERT(valid_clock_id(clock_id));
  clock_frequencies_[clock_id] = frequency;
}

void SimulationSetting::set_num_clock_cycles(const size_t& num_clk_cycles) {
  num_clock_cycles_ = num_clk_cycles;
}

void SimulationSetting::set_operating_clock_frequency_slack(const float& op_clk_freq_slack) {
  operating_clock_frequency_slack_ = op_clk_freq_slack;
}

void SimulationSetting::set_simulation_temperature(const float& sim_temp) {
  simulation_temperature_ = sim_temp;
}

void SimulationSetting::set_verbose_output(const bool& verbose_output) {
  verbose_output_ = verbose_output;
}

void SimulationSetting::set_capacitance_output(const bool& cap_output) {
  capacitance_output_ = cap_output;
}

void SimulationSetting::set_simulation_accuracy_type(const e_sim_accuracy_type& type) {
  VTR_ASSERT(NUM_SIM_ACCURACY_TYPES != type);
  simulation_accuracy_type_ = type;
}

void SimulationSetting::set_simulation_accuracy(const float& accuracy) {
  simulation_accuracy_ = accuracy;
}

void SimulationSetting::set_fast_simulation(const bool& fast_sim) {
  fast_simulation_ = fast_sim;
}

void SimulationSetting::set_monte_carlo_simulation_points(const size_t& num_mc_points) {
  monte_carlo_simulation_points_ = num_mc_points;
}

void SimulationSetting::set_measure_slew_upper_threshold(const e_sim_signal_type& signal_type,
                                                         const float& upper_thres) {
  VTR_ASSERT(NUM_SIM_SIGNAL_TYPES != signal_type);
  VTR_ASSERT (true == valid_signal_threshold(upper_thres));
  slew_upper_thresholds_[signal_type] = upper_thres;
}

void SimulationSetting::set_measure_slew_lower_threshold(const e_sim_signal_type& signal_type,
                                                         const float& lower_thres) {
  VTR_ASSERT(NUM_SIM_SIGNAL_TYPES != signal_type);
  VTR_ASSERT (true == valid_signal_threshold(lower_thres));
  slew_lower_thresholds_[signal_type] = lower_thres;
}

void SimulationSetting::set_measure_delay_input_threshold(const e_sim_signal_type& signal_type,
                                                          const float& input_thres) {
  VTR_ASSERT(NUM_SIM_SIGNAL_TYPES != signal_type);
  VTR_ASSERT (true == valid_signal_threshold(input_thres));
  delay_input_thresholds_[signal_type] = input_thres;
}

void SimulationSetting::set_measure_delay_output_threshold(const e_sim_signal_type& signal_type,
                                                           const float& output_thres) {
  VTR_ASSERT(NUM_SIM_SIGNAL_TYPES != signal_type);
  VTR_ASSERT (true == valid_signal_threshold(output_thres));
  delay_output_thresholds_[signal_type] = output_thres;
}

void SimulationSetting::set_stimuli_clock_slew_type(const e_sim_signal_type& signal_type,
                                                    const e_sim_accuracy_type& slew_type) {
  VTR_ASSERT(NUM_SIM_SIGNAL_TYPES != signal_type);
  clock_slew_types_[signal_type] = slew_type;
}

void SimulationSetting::set_stimuli_clock_slew(const e_sim_signal_type& signal_type,
                                               const float& clock_slew) {
  clock_slews_[signal_type] = clock_slew;
}

void SimulationSetting::set_stimuli_input_slew_type(const e_sim_signal_type& signal_type,
                                                    const e_sim_accuracy_type& input_type) {
  VTR_ASSERT(NUM_SIM_SIGNAL_TYPES != signal_type);
  input_slew_types_[signal_type] = input_type;
}

void SimulationSetting::set_stimuli_input_slew(const e_sim_signal_type& signal_type,
                                               const float& input_slew) {
  input_slews_[signal_type] = input_slew;
}

/************************************************************************
 * Public Validators
 ***********************************************************************/
bool SimulationSetting::valid_signal_threshold(const float& threshold) const {
  return (0. < threshold) && (threshold < 1);
}

bool SimulationSetting::valid_clock_id(const SimulationClockId& clock_id) const {
  return ( size_t(clock_id) < clock_ids_.size() ) && ( clock_id == clock_ids_[clock_id] ); 
}


} /* namespace openfpga ends */
