#ifndef PNR_SDC_OPTION_H
#define PNR_SDC_OPTION_H

/********************************************************************
 * A data structure to include all the options for the SDC generator
 * in purpose of constraining physical design of FPGA fabric in back-end flow
 ********************************************************************/

#include <string>

/* begin namespace openfpga */
namespace openfpga {

class PnrSdcOption {
  public: /* Public Constructors */
    PnrSdcOption(const std::string& sdc_dir);
  public: /* Public accessors */
    std::string sdc_dir() const;
    bool generate_sdc_pnr() const;
    bool constrain_global_port() const;
    bool constrain_non_clock_global_port() const;
    bool constrain_grid() const;
    bool constrain_sb() const;
    bool constrain_cb() const;
    bool constrain_configurable_memory_outputs() const;
    bool constrain_routing_multiplexer_outputs() const;
    bool constrain_switch_block_outputs() const;
  public: /* Public mutators */
    void set_sdc_dir(const std::string& sdc_dir);
    void set_generate_sdc_pnr(const bool& generate_sdc_pnr);
    void set_constrain_global_port(const bool& constrain_global_port);
    void set_constrain_non_clock_global_port(const bool& constrain_non_clock_global_port);
    void set_constrain_grid(const bool& constrain_grid);
    void set_constrain_sb(const bool& constrain_sb);
    void set_constrain_cb(const bool& constrain_cb);
    void set_constrain_configurable_memory_outputs(const bool& constrain_config_mem_outputs);
    void set_constrain_routing_multiplexer_outputs(const bool& constrain_routing_mux_outputs);
    void set_constrain_switch_block_outputs(const bool& constrain_sb_outputs);
  private: /* Internal data */
    std::string sdc_dir_;
    bool constrain_global_port_; 
    bool constrain_non_clock_global_port_; 
    bool constrain_grid_; 
    bool constrain_sb_;
    bool constrain_cb_;
    bool constrain_configurable_memory_outputs_;
    bool constrain_routing_multiplexer_outputs_;
    bool constrain_switch_block_outputs_;
};

} /* end namespace openfpga */

#endif
