/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML modeling OpenFPGA architecture to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"

/* Headers from openfpga util library */
#include "openfpga_port_parser.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_util.h"

#include "read_xml_simulation_setting.h"

/********************************************************************
 * Convert string to the enumerate of simulation accuracy type
 *******************************************************************/
static 
e_sim_accuracy_type string_to_sim_accuracy_type(const std::string& type_string) {
  for (size_t itype = 0; itype < NUM_SIM_ACCURACY_TYPES; ++itype) {
    if (std::string(SIM_ACCURACY_TYPE_STRING[itype]) == type_string) {
      return static_cast<e_sim_accuracy_type>(itype);
    }
  }

  return NUM_SIM_ACCURACY_TYPES;
}

/********************************************************************
 * Parse XML codes of a <clock> line under <operating> to an object of simulation setting
 *******************************************************************/
static 
void read_xml_operating_clock_override_setting(pugi::xml_node& xml_clock_override_setting,
                                               const pugiutil::loc_data& loc_data,
                                               openfpga::SimulationSetting& sim_setting) {
  std::string clock_name = get_attribute(xml_clock_override_setting, "name", loc_data).as_string();

  /* Create a new clock override object in the sim_setting object with the given name */
  SimulationClockId clock_id = sim_setting.create_clock(clock_name);
 
  /* Report if the clock creation failed, this is due to a conflicts in naming*/
  if (false == sim_setting.valid_clock_id(clock_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_clock_override_setting),
                   "Fail to create simulation clock '%s', it may share the same name as other simulation clock definition!\n",
                   clock_name.c_str());
  }

  /* Parse port information */
  openfpga::PortParser clock_port_parser(get_attribute(xml_clock_override_setting, "port", loc_data).as_string());
  sim_setting.set_clock_port(clock_id, clock_port_parser.port());

  /* Parse frequency information */
  sim_setting.set_clock_frequency(clock_id, get_attribute(xml_clock_override_setting, "frequency", loc_data).as_float(0.));
}

/********************************************************************
 * Parse XML codes of a <clock> line under <programming> to an object of simulation setting
 *******************************************************************/
static 
void read_xml_programming_clock_override_setting(pugi::xml_node& xml_clock_override_setting,
                                                 const pugiutil::loc_data& loc_data,
                                                 openfpga::SimulationSetting& sim_setting) {
  std::string clock_name = get_attribute(xml_clock_override_setting, "name", loc_data).as_string();

  /* Create a new clock override object in the sim_setting object with the given name */
  SimulationClockId clock_id = sim_setting.create_clock(clock_name);
 
  /* Report if the clock creation failed, this is due to a conflicts in naming*/
  if (false == sim_setting.valid_clock_id(clock_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_clock_override_setting),
                   "Fail to create simulation clock '%s', it may share the same name as other simulation clock definition!\n",
                   clock_name.c_str());
  }

  /* Parse port information */
  openfpga::PortParser clock_port_parser(get_attribute(xml_clock_override_setting, "port", loc_data).as_string());
  sim_setting.set_clock_port(clock_id, clock_port_parser.port());

  /* Parse frequency information */
  std::string clock_freq_str = get_attribute(xml_clock_override_setting, "frequency", loc_data).as_string();
  if (std::string("auto") != clock_freq_str) {
    sim_setting.set_clock_frequency(clock_id, get_attribute(xml_clock_override_setting, "frequency", loc_data).as_float(0.));
  }

  sim_setting.set_clock_is_programming(clock_id, true);

  sim_setting.set_clock_is_shift_register(clock_id, get_attribute(xml_clock_override_setting, "is_shift_register", loc_data).as_bool(false));
}

/********************************************************************
 * Parse XML codes of a <clock_setting> to an object of simulation setting
 *******************************************************************/
static 
void read_xml_clock_setting(pugi::xml_node& xml_clock_setting,
                            const pugiutil::loc_data& loc_data,
                            openfpga::SimulationSetting& sim_setting) {
  /* Parse operating clock setting */
  pugi::xml_node xml_operating_clock_setting = get_single_child(xml_clock_setting, "operating", loc_data);

  sim_setting.set_default_operating_clock_frequency(get_attribute(xml_operating_clock_setting, "frequency", loc_data).as_float(0.));

  /* Parse number of clock cycles to be used in simulation
   * Valid keywords is "auto" or other integer larger than 0
   */
  std::string num_cycles_str = get_attribute(xml_operating_clock_setting, "num_cycles", loc_data).as_string();
  if (std::string("auto") == num_cycles_str) {
    sim_setting.set_num_clock_cycles(0);
  } else if (0 < get_attribute(xml_operating_clock_setting, "num_cycles", loc_data).as_int(0.)) {
    sim_setting.set_num_clock_cycles(get_attribute(xml_operating_clock_setting, "num_cycles", loc_data).as_int(0));
  } else {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_operating_clock_setting),
                   "Invalid <num_cycles> defined under <operating>");
  }

  sim_setting.set_operating_clock_frequency_slack(get_attribute(xml_operating_clock_setting, "slack", loc_data).as_float(0.));

  /* Iterate over multiple operating clock settings and parse one by one */
  for (pugi::xml_node xml_clock : xml_operating_clock_setting.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_clock.name() != std::string("clock")) {
      bad_tag(xml_clock, loc_data, xml_operating_clock_setting, {"clock"});
    }
    read_xml_operating_clock_override_setting(xml_clock, loc_data, sim_setting);
  } 

  /* Parse programming clock setting */
  pugi::xml_node xml_programming_clock_setting = get_single_child(xml_clock_setting, "programming", loc_data);

  sim_setting.set_programming_clock_frequency(get_attribute(xml_programming_clock_setting, "frequency", loc_data).as_float(0.));

  /* Iterate over multiple operating clock settings and parse one by one */
  for (pugi::xml_node xml_clock : xml_programming_clock_setting.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_clock.name() != std::string("clock")) {
      bad_tag(xml_clock, loc_data, xml_programming_clock_setting, {"clock"});
    }
    read_xml_programming_clock_override_setting(xml_clock, loc_data, sim_setting);
  } 
}

/********************************************************************
 * Parse XML codes of a <simulator_option> to an object of simulation setting
 *******************************************************************/
static 
void read_xml_simulator_option(pugi::xml_node& xml_sim_option,
                               const pugiutil::loc_data& loc_data,
                               openfpga::SimulationSetting& sim_setting) {

  pugi::xml_node xml_operating_condition = get_single_child(xml_sim_option, "operating_condition", loc_data);
  sim_setting.set_simulation_temperature(get_attribute(xml_operating_condition, "temperature", loc_data).as_float(0.));

  pugi::xml_node xml_output_log = get_single_child(xml_sim_option, "output_log", loc_data);
  sim_setting.set_verbose_output(get_attribute(xml_output_log, "verbose", loc_data).as_bool(false));
  sim_setting.set_capacitance_output(get_attribute(xml_output_log, "captab", loc_data).as_bool(false));

  pugi::xml_node xml_accuracy = get_single_child(xml_sim_option, "accuracy", loc_data);

  /* Find the type of accuracy */
  const char* type_attr = get_attribute(xml_accuracy, "type", loc_data).value();
  /* Translate the type of design technology to enumerate */
  e_sim_accuracy_type accuracy_type = string_to_sim_accuracy_type(std::string(type_attr));

  if (NUM_SIM_ACCURACY_TYPES == accuracy_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_accuracy),
                   "Invalid 'type' attribute '%s'\n",
                   type_attr);
  }

  sim_setting.set_simulation_accuracy_type(accuracy_type);

  sim_setting.set_simulation_accuracy(get_attribute(xml_accuracy, "value", loc_data).as_float(0.));

  /* Validate the accuracy value */
  if (SIM_ACCURACY_FRAC == sim_setting.simulation_accuracy_type()) {
    if (false == sim_setting.valid_signal_threshold(sim_setting.simulation_accuracy())) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_accuracy),
                     "Invalid 'value' attribute '%f', which should be in the range of (0,1)\n",
                     sim_setting.simulation_accuracy());
    }
  }

  pugi::xml_node xml_runtime = get_single_child(xml_sim_option, "runtime", loc_data);
  sim_setting.set_fast_simulation(get_attribute(xml_runtime, "fast_simulation", loc_data).as_bool(false));
}


/********************************************************************
 * Parse XML codes of a <monte_carlo> to an object of simulation setting
 *******************************************************************/
static 
void read_xml_monte_carlo(pugi::xml_node& xml_mc,
                          const pugiutil::loc_data& loc_data,
                          openfpga::SimulationSetting& sim_setting) {
  sim_setting.set_monte_carlo_simulation_points(get_attribute(xml_mc, "num_simulation_points", loc_data).as_int(0));
}

/********************************************************************
 * Parse XML codes of a <measurement_setting> to an object of simulation setting
 *******************************************************************/
static 
void read_xml_measurement_setting(pugi::xml_node& xml_measurement,
                                  const pugiutil::loc_data& loc_data,
                                  openfpga::SimulationSetting& sim_setting) {
  pugi::xml_node xml_slew = get_single_child(xml_measurement, "slew", loc_data);
  pugi::xml_node xml_slew_rise = get_single_child(xml_slew, "rise", loc_data);
  sim_setting.set_measure_slew_upper_threshold(SIM_SIGNAL_RISE, get_attribute(xml_slew_rise, "upper_thres_pct", loc_data).as_float(0.));
  sim_setting.set_measure_slew_lower_threshold(SIM_SIGNAL_RISE, get_attribute(xml_slew_rise, "lower_thres_pct", loc_data).as_float(0.));

  pugi::xml_node xml_slew_fall = get_single_child(xml_slew, "fall", loc_data);
  sim_setting.set_measure_slew_upper_threshold(SIM_SIGNAL_FALL, get_attribute(xml_slew_fall, "upper_thres_pct", loc_data).as_float(0.));
  sim_setting.set_measure_slew_lower_threshold(SIM_SIGNAL_FALL, get_attribute(xml_slew_fall, "lower_thres_pct", loc_data).as_float(0.));

  pugi::xml_node xml_delay = get_single_child(xml_measurement, "delay", loc_data);
  pugi::xml_node xml_delay_rise = get_single_child(xml_delay, "rise", loc_data);
  sim_setting.set_measure_delay_input_threshold(SIM_SIGNAL_RISE, get_attribute(xml_delay_rise, "input_thres_pct", loc_data).as_float(0.));
  sim_setting.set_measure_delay_output_threshold(SIM_SIGNAL_RISE, get_attribute(xml_delay_rise, "output_thres_pct", loc_data).as_float(0.));

  pugi::xml_node xml_delay_fall = get_single_child(xml_delay, "fall", loc_data);
  sim_setting.set_measure_delay_input_threshold(SIM_SIGNAL_FALL, get_attribute(xml_delay_fall, "input_thres_pct", loc_data).as_float(0.));
  sim_setting.set_measure_delay_output_threshold(SIM_SIGNAL_FALL, get_attribute(xml_delay_fall, "output_thres_pct", loc_data).as_float(0.));
}

/********************************************************************
 * Parse XML codes of a <clock> inside <stimulus> to an object of simulation setting
 *******************************************************************/
static 
void read_xml_stimulus_clock(pugi::xml_node& xml_stimuli_clock,
                             const pugiutil::loc_data& loc_data,
                             openfpga::SimulationSetting& sim_setting,
                             const e_sim_signal_type& signal_type) {
  /* Find the type of accuracy */
  const char* type_attr = get_attribute(xml_stimuli_clock, "slew_type", loc_data).value();
  /* Translate the type of design technology to enumerate */
  e_sim_accuracy_type accuracy_type = string_to_sim_accuracy_type(std::string(type_attr));

  if (NUM_SIM_ACCURACY_TYPES == accuracy_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_stimuli_clock),
                   "Invalid 'type' attribute '%s'\n",
                   type_attr);
  }

  sim_setting.set_stimuli_clock_slew_type(signal_type, accuracy_type);

  sim_setting.set_stimuli_clock_slew(signal_type, get_attribute(xml_stimuli_clock, "slew_time", loc_data).as_float(0.));

  /* Validate the accuracy value */
  if (SIM_ACCURACY_FRAC == sim_setting.stimuli_clock_slew_type(signal_type)) {
    if (false == sim_setting.valid_signal_threshold(sim_setting.stimuli_clock_slew(signal_type))) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_stimuli_clock),
                     "Invalid 'value' attribute '%f', which should be in the range of (0,1)\n",
                     sim_setting.stimuli_clock_slew(signal_type));
    }
  }
}

/********************************************************************
 * Parse XML codes of a <input> inside <stimulus> to an object of simulation setting
 *******************************************************************/
static 
void read_xml_stimulus_input(pugi::xml_node& xml_stimuli_input,
                             const pugiutil::loc_data& loc_data,
                             openfpga::SimulationSetting& sim_setting,
                             const e_sim_signal_type& signal_type) {
  /* Find the type of accuracy */
  const char* type_attr = get_attribute(xml_stimuli_input, "slew_type", loc_data).value();
  /* Translate the type of design technology to enumerate */
  e_sim_accuracy_type accuracy_type = string_to_sim_accuracy_type(std::string(type_attr));

  if (NUM_SIM_ACCURACY_TYPES == accuracy_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_stimuli_input),
                   "Invalid 'type' attribute '%s'\n",
                   type_attr);
  }

  sim_setting.set_stimuli_input_slew_type(signal_type, accuracy_type);

  sim_setting.set_stimuli_input_slew(signal_type, get_attribute(xml_stimuli_input, "slew_time", loc_data).as_float(0.));

  /* Validate the accuracy value */
  if (SIM_ACCURACY_FRAC == sim_setting.stimuli_input_slew_type(signal_type)) {
    if (false == sim_setting.valid_signal_threshold(sim_setting.stimuli_input_slew(signal_type))) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_stimuli_input),
                     "Invalid 'value' attribute '%f', which should be in the range of (0,1)\n",
                     sim_setting.stimuli_input_slew(signal_type));
    }
  }
}

/********************************************************************
 * Parse XML codes of a <stimulus> to an object of simulation setting
 *******************************************************************/
static 
void read_xml_stimulus(pugi::xml_node& xml_stimulus,
                       const pugiutil::loc_data& loc_data,
                       openfpga::SimulationSetting& sim_setting) {
  pugi::xml_node xml_clock = get_single_child(xml_stimulus, "clock", loc_data);
  pugi::xml_node xml_clock_rise = get_single_child(xml_clock, "rise", loc_data);
  read_xml_stimulus_clock(xml_clock_rise, loc_data, sim_setting, SIM_SIGNAL_RISE);

  pugi::xml_node xml_clock_fall = get_single_child(xml_clock, "fall", loc_data);
  read_xml_stimulus_clock(xml_clock_fall, loc_data, sim_setting, SIM_SIGNAL_FALL);

  pugi::xml_node xml_input = get_single_child(xml_stimulus, "input", loc_data);
  pugi::xml_node xml_input_rise = get_single_child(xml_input, "rise", loc_data);
  read_xml_stimulus_input(xml_input_rise, loc_data, sim_setting, SIM_SIGNAL_RISE);

  pugi::xml_node xml_input_fall = get_single_child(xml_input, "fall", loc_data);
  read_xml_stimulus_input(xml_input_fall, loc_data, sim_setting, SIM_SIGNAL_FALL);
}

/********************************************************************
 * Parse XML codes about <openfpga_simulation_setting> to an object of technology library
 *******************************************************************/
openfpga::SimulationSetting read_xml_simulation_setting(pugi::xml_node& Node,
                                                        const pugiutil::loc_data& loc_data) {
  openfpga::SimulationSetting sim_setting;

  /* Parse clock settings */
  pugi::xml_node xml_clock_setting = get_single_child(Node, "clock_setting", loc_data);
  read_xml_clock_setting(xml_clock_setting, loc_data, sim_setting);

  /* Parse simulator options */
  pugi::xml_node xml_simulator_option = get_single_child(Node, "simulator_option", loc_data);
  read_xml_simulator_option(xml_simulator_option, loc_data, sim_setting);

  /* Parse Monte carlo simulation options */
  pugi::xml_node xml_mc = get_single_child(Node, "monte_carlo", loc_data, pugiutil::ReqOpt::OPTIONAL);
  if (xml_mc) {
    read_xml_monte_carlo(xml_mc, loc_data, sim_setting);
  }

  /* Parse measurement settings */
  pugi::xml_node xml_measurement = get_single_child(Node, "measurement_setting", loc_data);
  read_xml_measurement_setting(xml_measurement, loc_data, sim_setting);

  /* Parse stimulus settings */
  pugi::xml_node xml_stimulus = get_single_child(Node, "stimulus", loc_data);
  read_xml_stimulus(xml_stimulus, loc_data, sim_setting);

  return sim_setting;
}
