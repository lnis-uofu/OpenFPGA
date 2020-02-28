/********************************************************************
 * Member functions for a data structure which includes all the options for the SDC generator
 ********************************************************************/
#include "sdc_option.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Public Constructors
 ********************************************************************/
SdcOption::SdcOption(const std::string& sdc_dir) {
  sdc_dir_ = sdc_dir;
  constrain_global_port_ = false;
  constrain_grid_ = false;
  constrain_sb_ = false;
  constrain_cb_ = false;
  constrain_configurable_memory_outputs_ = false;
  constrain_routing_multiplexer_outputs_ = false;
  constrain_switch_block_outputs_ = false;
}

/********************************************************************
 * Public accessors
 ********************************************************************/
std::string SdcOption::sdc_dir() const {
  return sdc_dir_;
}

bool SdcOption::generate_sdc() const {
  return generate_sdc_pnr() && generate_sdc_analysis_;
}

bool SdcOption::generate_sdc_pnr() const {
  return constrain_global_port_ 
      || constrain_grid_
      || constrain_sb_
      || constrain_cb_
      || constrain_configurable_memory_outputs_
      || constrain_routing_multiplexer_outputs_
      || constrain_switch_block_outputs_;
}

bool SdcOption::generate_sdc_analysis() const {
  return generate_sdc_analysis_;
}

bool SdcOption::constrain_global_port() const {
  return constrain_global_port_;
}

bool SdcOption::constrain_grid() const {
  return constrain_grid_;
}

bool SdcOption::constrain_sb() const {
  return constrain_sb_;
}

bool SdcOption::constrain_cb() const {
  return constrain_cb_;
}

bool SdcOption::constrain_configurable_memory_outputs() const {
  return constrain_configurable_memory_outputs_;
}

bool SdcOption::constrain_routing_multiplexer_outputs() const {
  return constrain_routing_multiplexer_outputs_;
}

bool SdcOption::constrain_switch_block_outputs() const {
  return constrain_switch_block_outputs_;
}

/********************************************************************
 * Public mutators
 ********************************************************************/
void SdcOption::set_sdc_dir(const std::string& sdc_dir) {
  sdc_dir_ = sdc_dir;
}

void SdcOption::set_generate_sdc_pnr(const bool& generate_sdc_pnr) {
  constrain_global_port_ = generate_sdc_pnr;
  constrain_grid_ = generate_sdc_pnr;
  constrain_sb_ = generate_sdc_pnr;
  constrain_cb_ = generate_sdc_pnr;
  constrain_configurable_memory_outputs_ = generate_sdc_pnr;
  constrain_routing_multiplexer_outputs_ = generate_sdc_pnr;
  constrain_switch_block_outputs_ = generate_sdc_pnr;
}

void SdcOption::set_generate_sdc_analysis(const bool& generate_sdc_analysis) {
  generate_sdc_analysis_ = generate_sdc_analysis;
}

void SdcOption::set_constrain_global_port(const bool& constrain_global_port) {
  constrain_global_port_ = constrain_global_port;
}

void SdcOption::set_constrain_grid(const bool& constrain_grid) {
  constrain_grid_ = constrain_grid;
}

void SdcOption::set_constrain_sb(const bool& constrain_sb) {
  constrain_sb_ = constrain_sb;
}

void SdcOption::set_constrain_cb(const bool& constrain_cb) {
  constrain_cb_ = constrain_cb;
}

void SdcOption::set_constrain_configurable_memory_outputs(const bool& constrain_config_mem_outputs) {
  constrain_configurable_memory_outputs_ = constrain_config_mem_outputs;
}

void SdcOption::set_constrain_routing_multiplexer_outputs(const bool& constrain_routing_mux_outputs) {
  constrain_routing_multiplexer_outputs_ = constrain_routing_mux_outputs;
}

void SdcOption::set_constrain_switch_block_outputs(const bool& constrain_sb_outputs) {
  constrain_switch_block_outputs_ = constrain_sb_outputs;
}

} /* end namespace openfpga */
