#ifndef ANALYSIS_SDC_OPTION_H
#define ANALYSIS_SDC_OPTION_H

/********************************************************************
 * A data structure to include all the options for the SDC generator
 * in purpose of analyzing users' implementations
 ********************************************************************/

#include <string>

/* begin namespace openfpga */
namespace openfpga {

class AnalysisSdcOption {
  public: /* Public Constructors */
    AnalysisSdcOption(const std::string& sdc_dir);
  public: /* Public accessors */
    std::string sdc_dir() const;
    bool flatten_names() const;
    float time_unit() const;
    bool generate_sdc_analysis() const;
    bool time_stamp() const;
  public: /* Public mutators */
    void set_sdc_dir(const std::string& sdc_dir);
    void set_flatten_names(const bool& flatten_names);
    void set_time_stamp(const bool& time_stamp);
    void set_time_unit(const float& time_unit);
    void set_generate_sdc_analysis(const bool& generate_sdc_analysis);
  private: /* Internal data */
    std::string sdc_dir_;
    bool generate_sdc_analysis_; 
    bool flatten_names_; 
    float time_unit_;
    bool time_stamp_;
};

} /* end namespace openfpga */

#endif
