/********************************************************************
 * Member functions for a data structure which includes all the options for the SDC generator
 ********************************************************************/
#include "pnr_sdc_option.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Public Constructors
 ********************************************************************/
PnrSdcOption::PnrSdcOption(const std::string& sdc_dir) {
  sdc_dir_ = sdc_dir;
  flatten_names_ = false;
  constrain_global_port_ = false;
  constrain_non_clock_global_port_ = false;
  constrain_grid_ = false;
  constrain_sb_ = false;
  constrain_cb_ = false;
  constrain_configurable_memory_outputs_ = false;
  constrain_routing_multiplexer_outputs_ = false;
  constrain_switch_block_outputs_ = false;
  constrain_zero_delay_paths_ = false;
}

/********************************************************************
 * Public accessors
 ********************************************************************/
std::string PnrSdcOption::sdc_dir() const {
  return sdc_dir_;
}

bool PnrSdcOption::flatten_names() const {
  return flatten_names_;
}

bool PnrSdcOption::generate_sdc_pnr() const {
  return constrain_global_port_ 
      || constrain_grid_
      || constrain_sb_
      || constrain_cb_
      || constrain_configurable_memory_outputs_
      || constrain_routing_multiplexer_outputs_
      || constrain_switch_block_outputs_;
}

bool PnrSdcOption::constrain_global_port() const {
  return constrain_global_port_;
}

bool PnrSdcOption::constrain_non_clock_global_port() const {
  return constrain_non_clock_global_port_;
}

bool PnrSdcOption::constrain_grid() const {
  return constrain_grid_;
}

bool PnrSdcOption::constrain_sb() const {
  return constrain_sb_;
}

bool PnrSdcOption::constrain_cb() const {
  return constrain_cb_;
}

bool PnrSdcOption::constrain_configurable_memory_outputs() const {
  return constrain_configurable_memory_outputs_;
}

bool PnrSdcOption::constrain_routing_multiplexer_outputs() const {
  return constrain_routing_multiplexer_outputs_;
}

bool PnrSdcOption::constrain_switch_block_outputs() const {
  return constrain_switch_block_outputs_;
}

bool PnrSdcOption::constrain_zero_delay_paths() const {
  return constrain_zero_delay_paths_;
}

/********************************************************************
 * Public mutators
 ********************************************************************/
void PnrSdcOption::set_sdc_dir(const std::string& sdc_dir) {
  sdc_dir_ = sdc_dir;
}

void PnrSdcOption::set_flatten_names(const bool& flatten_names) {
  flatten_names_ = flatten_names;
}

void PnrSdcOption::set_generate_sdc_pnr(const bool& generate_sdc_pnr) {
  constrain_global_port_ = generate_sdc_pnr;
  constrain_grid_ = generate_sdc_pnr;
  constrain_sb_ = generate_sdc_pnr;
  constrain_cb_ = generate_sdc_pnr;
  constrain_configurable_memory_outputs_ = generate_sdc_pnr;
  constrain_routing_multiplexer_outputs_ = generate_sdc_pnr;
  constrain_switch_block_outputs_ = generate_sdc_pnr;
}

void PnrSdcOption::set_constrain_global_port(const bool& constrain_global_port) {
  constrain_global_port_ = constrain_global_port;
}

void PnrSdcOption::set_constrain_non_clock_global_port(const bool& constrain_non_clock_global_port) {
  constrain_non_clock_global_port_ = constrain_non_clock_global_port;
}

void PnrSdcOption::set_constrain_grid(const bool& constrain_grid) {
  constrain_grid_ = constrain_grid;
}

void PnrSdcOption::set_constrain_sb(const bool& constrain_sb) {
  constrain_sb_ = constrain_sb;
}

void PnrSdcOption::set_constrain_cb(const bool& constrain_cb) {
  constrain_cb_ = constrain_cb;
}

void PnrSdcOption::set_constrain_configurable_memory_outputs(const bool& constrain_config_mem_outputs) {
  constrain_configurable_memory_outputs_ = constrain_config_mem_outputs;
}

void PnrSdcOption::set_constrain_routing_multiplexer_outputs(const bool& constrain_routing_mux_outputs) {
  constrain_routing_multiplexer_outputs_ = constrain_routing_mux_outputs;
}

void PnrSdcOption::set_constrain_switch_block_outputs(const bool& constrain_sb_outputs) {
  constrain_switch_block_outputs_ = constrain_sb_outputs;
}

void PnrSdcOption::set_constrain_zero_delay_paths(const bool& constrain_zero_delay_paths) {
  constrain_zero_delay_paths_ = constrain_zero_delay_paths;
}

} /* end namespace openfpga */
