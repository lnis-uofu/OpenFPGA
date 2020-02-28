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
  generate_sdc_analysis_ = false;
}

/********************************************************************
 * Public accessors
 ********************************************************************/
std::string AnalysisSdcOption::sdc_dir() const {
  return sdc_dir_;
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

void AnalysisSdcOption::set_generate_sdc_analysis(const bool& generate_sdc_analysis) {
  generate_sdc_analysis_ = generate_sdc_analysis;
}

} /* end namespace openfpga */
