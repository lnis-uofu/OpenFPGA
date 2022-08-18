#ifndef SIMULATION_SETTING_H
#define SIMULATION_SETTING_H

/********************************************************************
 * This file include the declaration of simulation settings
 * which are used by OpenFPGA 
 *******************************************************************/
#include <string>
#include <array>
#include <map>

#include "vtr_vector.h" 
#include "vtr_geometry.h" 

#include "openfpga_port.h" 

#include "simulation_setting_fwd.h" 

/********************************************************************
 * Types of signal type in measurement and stimuli
 *******************************************************************/
enum e_sim_signal_type {
  SIM_SIGNAL_RISE, 
  SIM_SIGNAL_FALL,
  NUM_SIM_SIGNAL_TYPES
};
/* Strings correspond to each delay type */
constexpr std::array<const char*, NUM_SIM_SIGNAL_TYPES> SIM_SIGNAL_TYPE_STRING = {{"rise", "fall"}};

/********************************************************************
 * Types of simulation accuracy type
 * 1. Fraction to the operating clock frequency
 * 2. Absolute value 
 *******************************************************************/
enum e_sim_accuracy_type {
  SIM_ACCURACY_FRAC,
  SIM_ACCURACY_ABS,
  NUM_SIM_ACCURACY_TYPES
};
/* Strings correspond to each accuracy type */
constexpr std::array<const char*, NUM_SIM_ACCURACY_TYPES> SIM_ACCURACY_TYPE_STRING = {{"frac", "abs"}};

/* namespace openfpga begins */
namespace openfpga {

/********************************************************************
 * A data structure to describe simulation settings
 *
 * Typical usage:
 * --------------
 *   // Create an empty technology library
 *   SimulationSetting sim_setting;
 *   // call your builder for sim_setting
 *
 *******************************************************************/
class SimulationSetting {
  public: /* Types */
    typedef vtr::vector<SimulationClockId, SimulationClockId>::const_iterator simulation_clock_iterator;
    /* Create range */
    typedef vtr::Range<simulation_clock_iterator> simulation_clock_range;
  public:  /* Constructors */
    SimulationSetting();
  public: /* Accessors: aggregates */
    simulation_clock_range clocks() const;
    std::vector<SimulationClockId> operating_clocks() const;
    std::vector<SimulationClockId> programming_clocks() const;
    std::vector<SimulationClockId> programming_shift_register_clocks() const;
  public: /* Public Accessors */
    float default_operating_clock_frequency() const;
    float programming_clock_frequency() const;
    size_t num_simulation_clock_cycles() const;
    std::string clock_name(const SimulationClockId& clock_id) const;
    BasicPort clock_port(const SimulationClockId& clock_id) const;
    float clock_frequency(const SimulationClockId& clock_id) const;
    bool clock_is_programming(const SimulationClockId& clock_id) const;
    bool clock_is_shift_register(const SimulationClockId& clock_id) const;
    bool auto_select_num_clock_cycles() const;
    size_t num_clock_cycles() const;
    float operating_clock_frequency_slack() const;
    float simulation_temperature() const;
    bool verbose_output() const;
    bool capacitance_output() const;
    e_sim_accuracy_type simulation_accuracy_type() const;
    float simulation_accuracy() const;
    bool fast_simulation() const;
    bool run_monte_carlo_simulation() const;
    size_t monte_carlo_simulation_points() const;
    float measure_slew_upper_threshold(const e_sim_signal_type& signal_type) const;
    float measure_slew_lower_threshold(const e_sim_signal_type& signal_type) const;
    float measure_delay_input_threshold(const e_sim_signal_type& signal_type) const;
    float measure_delay_output_threshold(const e_sim_signal_type& signal_type) const;
    e_sim_accuracy_type stimuli_clock_slew_type(const e_sim_signal_type& signal_type) const;
    float stimuli_clock_slew(const e_sim_signal_type& signal_type) const;
    e_sim_accuracy_type stimuli_input_slew_type(const e_sim_signal_type& signal_type) const;
    float stimuli_input_slew(const e_sim_signal_type& signal_type) const;
  public: /* Public Mutators */
    void set_default_operating_clock_frequency(const float& clock_freq);
    void set_programming_clock_frequency(const float& clock_freq);
    /* Add a new simulation clock with 
     * - a given name
     * - a given port description
     * - a default zero frequency which can be overwritten by 
     *   the operating_clock_frequency()
     */
    SimulationClockId create_clock(const std::string& name);
    void set_clock_port(const SimulationClockId& clock_id,
                        const BasicPort& port);
    void set_clock_frequency(const SimulationClockId& clock_id,
                             const float& frequency);
    void set_clock_is_programming(const SimulationClockId& clock_id,
                                  const float& is_prog);
    void set_clock_is_shift_register(const SimulationClockId& clock_id,
                                     const float& is_sr);
    void set_num_clock_cycles(const size_t& num_clk_cycles);
    void set_operating_clock_frequency_slack(const float& op_clk_freq_slack);
    void set_simulation_temperature(const float& sim_temp);
    void set_verbose_output(const bool& verbose_output);
    void set_capacitance_output(const bool& cap_output);
    void set_simulation_accuracy_type(const e_sim_accuracy_type& type);
    void set_simulation_accuracy(const float& accuracy);
    void set_fast_simulation(const bool& fast_sim);
    void set_monte_carlo_simulation_points(const size_t& num_mc_points);
    void set_measure_slew_upper_threshold(const e_sim_signal_type& signal_type,
                                          const float& upper_thres);
    void set_measure_slew_lower_threshold(const e_sim_signal_type& signal_type,
                                          const float& lower_thres);
    void set_measure_delay_input_threshold(const e_sim_signal_type& signal_type,
                                           const float& input_thres);
    void set_measure_delay_output_threshold(const e_sim_signal_type& signal_type,
                                            const float& output_thres);
    void set_stimuli_clock_slew_type(const e_sim_signal_type& signal_type,
                                     const e_sim_accuracy_type& slew_type);
    void set_stimuli_clock_slew(const e_sim_signal_type& signal_type,
                                const float& clock_slew);
    void set_stimuli_input_slew_type(const e_sim_signal_type& signal_type,
                                     const e_sim_accuracy_type& slew_type);
    void set_stimuli_input_slew(const e_sim_signal_type& signal_type,
                                const float& input_slew);
  public: /* Public Validators */
    bool valid_signal_threshold(const float& threshold) const;
    bool valid_clock_id(const SimulationClockId& clock_id) const;
    /** @brief Validate if a given clock is constrained or not */
    bool constrained_clock(const SimulationClockId& clock_id) const;
  private: /* Internal data */
    /* Operating clock frequency: the default clock frequency to be applied to users' implemetation on FPGA 
     *                            This will be stored in the x() part of vtr::Point
     * Programming clock frequency: the clock frequency to be applied to configuration protocol of FPGA 
     *                            This will be stored in the y() part of vtr::Point
     */
    vtr::Point<float> default_clock_frequencies_;

    /* Multiple simulation clocks with detailed information
     * Each clock has 
     * - a unique id
     * - a unique name
     * - a unique port definition which is supposed 
     *   to match the clock port definition in OpenFPGA documentation
     * - a frequency which is only applicable to this clock name
     */
    vtr::vector<SimulationClockId, SimulationClockId> clock_ids_;
    vtr::vector<SimulationClockId, std::string> clock_names_;
    vtr::vector<SimulationClockId, BasicPort> clock_ports_;
    vtr::vector<SimulationClockId, float> clock_frequencies_;
    vtr::vector<SimulationClockId, bool> clock_is_programming_;
    vtr::vector<SimulationClockId, bool> clock_is_shift_register_;

    /* Fast name-to-id lookup */
    std::map<std::string, SimulationClockId> clock_name2ids_;

    /* Number of clock cycles to be used in simulation
     * If the value is 0, the clock cycles can be automatically 
     * inferred from the signal activities of users' implementation
     */
    size_t num_clock_cycles_;
   
    /* Slack or margin to be added to clock frequency 
     * if the operating clock frequency is automatically 
     * detemined by VPR's routing results
     */
    float operating_clock_frequency_slack_;

    /* Operating temperature to be use in simulation */
    float simulation_temperature_;

    /* Options support by simulators 
     * verbose_output:     This is an option to turn on verbose output in simulators
     *                     Simulation runtime can be slow when this option is on
     *                     for large FPGA fabrics!
     * capacitance_output: Show capacitance of each nodes in the log file
     *                     This is an option provided by SPICE simulators 
     *                     Simulation runtime can be slow when this option is on
     *                     for large FPGA fabrics!
     * accuracy_type:      type of accuracy to be used in simulation 
     *                     See the definition in enumeration e_sim_accuracy_type
     *                     Simulation runtime can be slow when a high accuracy is enable
     *                     for large FPGA fabrics!
     * accuracy:           the absolute value of accuracy to be used in simulation.
     *                     If fractional accuarcy is specified, the value will be determined by
     *                     the maximum operating frequency after VPR routing finished
     *                     If absolute accuracy is specified, the value will be given by users
     */
    bool verbose_output_;
    bool capacitance_output_;
    e_sim_accuracy_type simulation_accuracy_type_;
    float simulation_accuracy_;

    /* Enable fast simulation
     * Note: this may impact the accuracy of simulation results
     */
    bool fast_simulation_;

    /* Number of simulation points to be used in Monte Carlo simulation
     * If a zero is given, monte carlo simulation will not be applied
     * The larger number of simulation points is used, the slower runtime will be
     */
    size_t monte_carlo_simulation_points_;

    /* The thresholds (in percentage) to be used in the measuring signal slews
     * Thresholds related to rising edge will be stored in the first element 
     * Thresholds related to falling edge will be stored in the second element
     */
    std::array<float, NUM_SIM_SIGNAL_TYPES> slew_upper_thresholds_;
    std::array<float, NUM_SIM_SIGNAL_TYPES> slew_lower_thresholds_;

    /* The thresholds (in percentage) to be used in the measuring signal delays
     * Thresholds related to rising edge will be stored in the first element
     * Thresholds related to falling edge will be stored in the second element
     *
     * An example of delay measurement in rising edge 
     * from 50% of input signal to 50% of output signal
     * (delay_input_threshold=0.5; delay_output_threshold=0.5)
     *
     *   Input signal
     *
     *         50% of full swing of input signal 
     *            ^ +--------------------------
     *            |/
     *            +----------------+
     *           /        |        |
     *   -------+         |        |
     *                    v        |
     *                 rise delay  |
     *                             |
     *  Output signal              | 
     *                             | +--------
     *                             |/
     *                             +
     *                            /|
     *  -------------------------+ |
     *                             v
     *                50% of full swing of output signal 
     */
    std::array<float, NUM_SIM_SIGNAL_TYPES> delay_input_thresholds_;
    std::array<float, NUM_SIM_SIGNAL_TYPES> delay_output_thresholds_;

    /* Stimulus to be given to each type of port.
     * We support two types of ports:
     * 1. clock ports
     * 2. regular input ports  
     *
     * Slew time related to rising edge will be stored in the first element
     * Slew time related to falling edge will be stored in the second element
     *
     * accuracy_type:   type of accuracy to be used in simulation 
     *                  Fractional accuracy will be determined by the clock frequency 
     *                  to be defined by user in the clock_setting
     */
    std::array<e_sim_accuracy_type, NUM_SIM_ACCURACY_TYPES> clock_slew_types_;
    std::array<float, NUM_SIM_ACCURACY_TYPES> clock_slews_;
    std::array<e_sim_accuracy_type, NUM_SIM_ACCURACY_TYPES> input_slew_types_;
    std::array<float, NUM_SIM_ACCURACY_TYPES> input_slews_;
};

} /* namespace openfpga ends */

#endif
