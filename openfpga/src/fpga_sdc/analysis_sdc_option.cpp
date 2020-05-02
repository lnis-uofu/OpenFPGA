/********************************************************************
 * Member functions for a data structure which includes all the options for the SDC generator
 ********************************************************************/
#include "analysis_sdc_option.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Public Constructors
 ********************************************************************/
AnalysisSdcOption::AnalysisSdcOption(const std::string& sdc_dir) {
  sdc_dir_ = sdc_dir;
  flatten_names_ = false;
  generate_sdc_analysis_ = false;
}

/********************************************************************
 * Public accessors
 ********************************************************************/
std::string AnalysisSdcOption::sdc_dir() const {
  return sdc_dir_;
}

bool AnalysisSdcOption::flatten_names() const {
  return flatten_names_;
}

bool AnalysisSdcOption::generate_sdc_analysis() const {
  return generate_sdc_analysis_;
}

/********************************************************************
 * Public mutators
 ********************************************************************/
void AnalysisSdcOption::set_sdc_dir(const std::string& sdc_dir) {
  sdc_dir_ = sdc_dir;
}

void AnalysisSdcOption::set_flatten_names(const bool& flatten_names) {
  flatten_names_ = flatten_names;
}

void AnalysisSdcOption::set_generate_sdc_analysis(const bool& generate_sdc_analysis) {
  generate_sdc_analysis_ = generate_sdc_analysis;
}

} /* end namespace openfpga */
