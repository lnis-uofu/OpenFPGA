#ifndef SDC_OPTION_H
#define SDC_OPTION_H

/********************************************************************
 * A data structure to include all the options for the SDC generator
 ********************************************************************/

#include <string>

class SdcOption {
  public: /* Public Constructors */
    SdcOption(const std::string& sdc_dir);
  public: /* Public accessors */
    std::string sdc_dir() const;
    bool generate_sdc() const;
    bool generate_sdc_pnr() const;
    bool generate_sdc_analysis() const;
    bool constrain_global_port() const;
    bool constrain_grid() const;
    bool constrain_sb() const;
    bool constrain_cb() const;
    bool break_loop() const;
  public: /* Public mutators */
    void set_sdc_dir(const std::string& sdc_dir);
    void set_generate_sdc_pnr(const bool& generate_sdc_pnr);
    void set_generate_sdc_analysis(const bool& generate_sdc_analysis);
    void set_constrain_global_port(const bool& constrain_global_port);
    void set_constrain_grid(const bool& constrain_grid);
    void set_constrain_sb(const bool& constrain_sb);
    void set_constrain_cb(const bool& constrain_cb);
    void set_break_loop(const bool& break_loop);
  private: /* Internal data */
    std::string sdc_dir_;
    bool generate_sdc_pnr_; 
    bool constrain_global_port_; 
    bool constrain_grid_; 
    bool constrain_sb_;
    bool constrain_cb_;
    bool break_loop_;
    bool generate_sdc_analysis_; 
};

#endif
