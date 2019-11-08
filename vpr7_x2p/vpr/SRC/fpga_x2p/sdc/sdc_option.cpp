/********************************************************************
 * Member functions for a data structure which includes all the options for the SDC generator
 ********************************************************************/
#include "sdc_option.h"

/********************************************************************
 * Public Constructors
 ********************************************************************/
SdcOption::SdcOption(const std::string& sdc_dir) {
  sdc_dir_ = sdc_dir;
  constrain_global_port_ = true;
  constrain_grid_ = true;
  constrain_sb_ = true;
  constrain_cb_ = true;
  break_loop_ = true;
}


/********************************************************************
 * Public accessors
 ********************************************************************/
std::string SdcOption::sdc_dir() const {
  return sdc_dir_;
}

bool SdcOption::generate_sdc() const {
  return generate_sdc_pnr_ && generate_sdc_analysis_;
}

bool SdcOption::generate_sdc_pnr() const {
  return generate_sdc_pnr_;
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

bool SdcOption::break_loop() const {
  return break_loop_;
}

/********************************************************************
 * Public mutators
 ********************************************************************/
void SdcOption::set_sdc_dir(const std::string& sdc_dir) {
  sdc_dir_ = sdc_dir;
}

void SdcOption::set_generate_sdc_pnr(const bool& generate_sdc_pnr) {
  generate_sdc_pnr_ = generate_sdc_pnr;
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

void SdcOption::set_break_loop(const bool& break_loop) {
  break_loop_ = break_loop;
}

