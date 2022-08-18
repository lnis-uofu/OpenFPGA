/********************************************************************
 * This file includes functions that outputs a simulation setting to XML format
 *******************************************************************/
/* Headers from system goes first */
#include <string>
#include <algorithm>

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "openfpga_digest.h"

/* Headers from readarchopenfpga library */
#include "write_xml_utils.h" 
#include "write_xml_simulation_setting.h"

/********************************************************************
 * A writer to output a clock setting in a simulation setting to XML format
 *******************************************************************/
static 
void write_xml_clock_setting(std::fstream& fp,
                             const char* fname,
                             const openfpga::SimulationSetting& sim_setting) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t" << "<clock_setting>" << "\n";

  fp << "\t\t" << "<operating";
  write_xml_attribute(fp, "frequency", sim_setting.default_operating_clock_frequency());

  if (true == sim_setting.auto_select_num_clock_cycles()) {
    write_xml_attribute(fp, "num_cycles", "auto");
  } else {
    VTR_ASSERT_SAFE(false == sim_setting.auto_select_num_clock_cycles());
    write_xml_attribute(fp, "num_cycles", std::to_string(sim_setting.num_clock_cycles()).c_str());
  }

  write_xml_attribute(fp, "slack", std::to_string(sim_setting.operating_clock_frequency_slack()).c_str());

  fp << ">" << "\n";
  
  /* Output clock information one by one */
  for (const SimulationClockId& clock_id : sim_setting.operating_clocks()) {
    fp << "\t\t\t" << "<clock";
    write_xml_attribute(fp, "name", sim_setting.clock_name(clock_id).c_str());
    write_xml_attribute(fp, "port", generate_xml_port_name(sim_setting.clock_port(clock_id)).c_str());
    write_xml_attribute(fp, "frequency", std::to_string(sim_setting.clock_frequency(clock_id)).c_str());
    fp << ">" << "\n";
  }
  
  fp << "\t\t" << "</operating";
  fp << ">" << "\n";

  fp << "\t\t" << "<programming";
  write_xml_attribute(fp, "frequency", sim_setting.programming_clock_frequency());
  fp << ">" << "\n";

  /* Output clock information one by one */
  for (const SimulationClockId& clock_id : sim_setting.programming_clocks()) {
    fp << "\t\t\t" << "<clock";
    write_xml_attribute(fp, "name", sim_setting.clock_name(clock_id).c_str());
    write_xml_attribute(fp, "port", generate_xml_port_name(sim_setting.clock_port(clock_id)).c_str());
    write_xml_attribute(fp, "frequency", std::to_string(sim_setting.clock_frequency(clock_id)).c_str());
    write_xml_attribute(fp, "is_shift_register", std::to_string(sim_setting.clock_is_shift_register(clock_id)).c_str());
    fp << ">" << "\n";
  }

  fp << "\t\t" << "</programming";
  fp << ">" << "\n";

  fp << "\t" << "</clock_setting>" << "\n";
}

/********************************************************************
 * A writer to output a simulator option in a simulation setting to XML format
 *******************************************************************/
static 
void write_xml_simulator_option(std::fstream& fp,
                                const char* fname,
                                const openfpga::SimulationSetting& sim_setting) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t" << "<simulator_option>" << "\n";

  fp << "\t\t" << "<operating_condition";
  write_xml_attribute(fp, "temperature", sim_setting.simulation_temperature());
  fp << "/>" << "\n";

  fp << "\t\t" << "<output_log";
  write_xml_attribute(fp, "verbose", sim_setting.verbose_output());
  write_xml_attribute(fp, "captab", sim_setting.capacitance_output());
  fp << "/>" << "\n";

  fp << "\t\t" << "<accuracy";
  write_xml_attribute(fp, "type", SIM_ACCURACY_TYPE_STRING[sim_setting.simulation_accuracy_type()]);
  write_xml_attribute(fp, "value", sim_setting.simulation_accuracy());
  fp << "/>" << "\n";

  fp << "\t\t" << "<runtime";
  write_xml_attribute(fp, "fast_simulation", sim_setting.fast_simulation());
  fp << "/>" << "\n";

  fp << "\t" << "</simulator_option>" << "\n";
}

/********************************************************************
 * A writer to output a monte carlo simulation setting
 * in a simulation setting to XML format
 *******************************************************************/
static 
void write_xml_monte_carlo(std::fstream& fp,
                           const char* fname,
                           const openfpga::SimulationSetting& sim_setting) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* This is an optional setting,
   * If defined, we will output the monte carlo simulation
   */
  if (false == sim_setting.run_monte_carlo_simulation()) {
    return;
  }

  fp << "\t\t" << "<monte_carlo";

  write_xml_attribute(fp, "num_simulation_points", std::to_string(sim_setting.monte_carlo_simulation_points()).c_str());
  
  fp << "/>" << "\n";
}

/********************************************************************
 * A writer to output the slew measurement setting in a simulation setting to XML format
 *******************************************************************/
static 
void write_xml_slew_measurement(std::fstream& fp,
                                const char* fname,
                                const openfpga::SimulationSetting& sim_setting,
                                const e_sim_signal_type& signal_type) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t\t" << "<" << SIM_SIGNAL_TYPE_STRING[signal_type];

  write_xml_attribute(fp, "upper_thres_pct", sim_setting.measure_slew_upper_threshold(signal_type));
  write_xml_attribute(fp, "lower_thres_pct", sim_setting.measure_slew_lower_threshold(signal_type));
  
  fp << "/>" << "\n";
}

/********************************************************************
 * A writer to output the delay measurement setting in a simulation setting to XML format
 *******************************************************************/
static 
void write_xml_delay_measurement(std::fstream& fp,
                                const char* fname,
                                const openfpga::SimulationSetting& sim_setting,
                                const e_sim_signal_type& signal_type) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t\t" << "<" << SIM_SIGNAL_TYPE_STRING[signal_type];

  write_xml_attribute(fp, "input_thres_pct", sim_setting.measure_delay_input_threshold(signal_type));
  write_xml_attribute(fp, "output_thres_pct", sim_setting.measure_delay_output_threshold(signal_type));
  
  fp << "/>" << "\n";
}

/********************************************************************
 * A writer to output a measurement setting in a simulation setting to XML format
 *******************************************************************/
static 
void write_xml_measurement(std::fstream& fp,
                           const char* fname,
                           const openfpga::SimulationSetting& sim_setting) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t" << "<measurement_setting>" << "\n";

  fp << "\t\t" << "<slew>" << "\n";
  write_xml_slew_measurement(fp, fname, sim_setting, SIM_SIGNAL_RISE);
  write_xml_slew_measurement(fp, fname, sim_setting, SIM_SIGNAL_FALL);
  fp << "\t\t" << "</slew>" << "\n";

  fp << "\t\t" << "<delay>" << "\n";
  write_xml_delay_measurement(fp, fname, sim_setting, SIM_SIGNAL_RISE);
  write_xml_delay_measurement(fp, fname, sim_setting, SIM_SIGNAL_FALL);
  fp << "\t\t" << "</delay>" << "\n";

  fp << "\t" << "</measurement_setting>" << "\n";
}

/********************************************************************
 * A writer to output a stimulus setting in a simulation setting to XML format
 *******************************************************************/
static 
void write_xml_stimulus(std::fstream& fp,
                        const char* fname,
                        const openfpga::SimulationSetting& sim_setting) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t" << "<stimulus>" << "\n";

  fp << "\t\t" << "<clock>" << "\n";

  fp << "\t\t\t" << "<rise";
  write_xml_attribute(fp, "slew_type", SIM_ACCURACY_TYPE_STRING[sim_setting.stimuli_clock_slew_type(SIM_SIGNAL_RISE)]);
  write_xml_attribute(fp, "slew_time", sim_setting.stimuli_clock_slew(SIM_SIGNAL_RISE));
  fp << "/>" << "\n";

  fp << "\t\t\t" << "<fall";
  write_xml_attribute(fp, "slew_type", SIM_ACCURACY_TYPE_STRING[sim_setting.stimuli_clock_slew_type(SIM_SIGNAL_FALL)]);
  write_xml_attribute(fp, "slew_time", sim_setting.stimuli_clock_slew(SIM_SIGNAL_FALL));
  fp << "/>" << "\n";

  fp << "\t\t" << "</clock>" << "\n";

  fp << "\t\t" << "<input>" << "\n";

  fp << "\t\t\t" << "<rise";
  write_xml_attribute(fp, "slew_type", SIM_ACCURACY_TYPE_STRING[sim_setting.stimuli_input_slew_type(SIM_SIGNAL_RISE)]);
  write_xml_attribute(fp, "slew_time", sim_setting.stimuli_input_slew(SIM_SIGNAL_RISE));
  fp << "/>" << "\n";

  fp << "\t\t\t" << "<fall";
  write_xml_attribute(fp, "slew_type", SIM_ACCURACY_TYPE_STRING[sim_setting.stimuli_input_slew_type(SIM_SIGNAL_FALL)]);
  write_xml_attribute(fp, "slew_time", sim_setting.stimuli_input_slew(SIM_SIGNAL_FALL));
  fp << "/>" << "\n";

  fp << "\t\t" << "</input>" << "\n";

  fp << "\t" << "</stimulus>" << "\n";
}

/********************************************************************
 * A writer to output a simulation setting to XML format
 *******************************************************************/
void write_xml_simulation_setting(std::fstream& fp,
                                  const char* fname,
                                  const openfpga::SimulationSetting& sim_setting) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node <openfpga_simulation_setting>
   */
  fp << "<openfpga_simulation_setting>" << "\n";

  /* Write clock settings */ 
  write_xml_clock_setting(fp, fname, sim_setting);

  /* Write simulator option */ 
  write_xml_simulator_option(fp, fname, sim_setting);

  /* Write monte carlo simulation setting */ 
  write_xml_monte_carlo(fp, fname, sim_setting);

  /* Write measurement setting */ 
  write_xml_measurement(fp, fname, sim_setting);

  /* Write stimuli setting */ 
  write_xml_stimulus(fp, fname, sim_setting);

  /* Write the root node <openfpga_simulation_setting> */
  fp << "</openfpga_simulation_setting>" << "\n";
}
