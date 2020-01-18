#ifndef SIMULATION_SETTING_H
#define SIMULATION_SETTING_H

/********************************************************************
 * This file include the declaration of simulation settings
 * which are used by OpenFPGA 
 *******************************************************************/
#include <string>
#include <array>

#include "vtr_geometry.h" 

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
constexpr std::array<const char*, NUM_SIM_ACCURACY_TYPES> SIM_ACCUARCY_TYPE_STRING = {{"frac", "abs"}};

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
  public:  /* Constructors */
    SimulationSetting();
  public: /* Public Accessors */
    float operating_clock_freqency() const;
    float programming_clock_freqency() const;
    bool auto_select_num_clock_cycles() const;
    size_t num_clock_cycles() const;
    size_t operating_clock_frequency_slack() const;
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
  public: /* Public Validators */
    bool valid_signal_threshold(const float& threshold) const;
  private: /* Internal data */
    /* Operating clock frequency: the clock frequency to be applied to users' implemetation on FPGA 
     *                            This will be stored in the x() part of vtr::Point
     * Programming clock frequency: the clock frequency to be applied to configuration protocol of FPGA 
     *                            This will be stored in the y() part of vtr::Point
     */
    vtr::Point<float> clock_frequencies_;

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
    std::array<float, 2> slew_upper_thresholds_;
    std::array<float, 2> slew_lower_thresholds_;

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
    std::array<float, 2> delay_input_thresholds_;
    std::array<float, 2> delay_output_thresholds_;

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
    std::array<e_sim_accuracy_type, 2> clock_slew_types_;
    std::array<float, 2> clock_slews_;
    std::array<e_sim_accuracy_type, 2> input_slew_types_;
    std::array<float, 2> input_slews_;
};

#endif
