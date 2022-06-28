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
    bool flatten_names() const;
    bool hierarchical() const;
    float time_unit() const;
    bool output_hierarchy() const;
    bool generate_sdc_pnr() const;
    bool constrain_global_port() const;
    bool constrain_non_clock_global_port() const;
    bool constrain_grid() const;
    bool constrain_sb() const;
    bool constrain_cb() const;
    bool constrain_configurable_memory_outputs() const;
    bool constrain_routing_multiplexer_outputs() const;
    bool constrain_switch_block_outputs() const;
    bool constrain_zero_delay_paths() const;
    bool time_stamp() const;
  public: /* Public mutators */
    void set_sdc_dir(const std::string& sdc_dir);
    void set_flatten_names(const bool& flatten_names);
    void set_hierarchical(const bool& hierarchical);
    void set_time_unit(const float& time_unit);
    void set_output_hierarchy(const bool& output_hierarchy);
    void set_generate_sdc_pnr(const bool& generate_sdc_pnr);
    void set_constrain_global_port(const bool& constrain_global_port);
    void set_constrain_non_clock_global_port(const bool& constrain_non_clock_global_port);
    void set_constrain_grid(const bool& constrain_grid);
    void set_constrain_sb(const bool& constrain_sb);
    void set_constrain_cb(const bool& constrain_cb);
    void set_constrain_configurable_memory_outputs(const bool& constrain_config_mem_outputs);
    void set_constrain_routing_multiplexer_outputs(const bool& constrain_routing_mux_outputs);
    void set_constrain_switch_block_outputs(const bool& constrain_sb_outputs);
    void set_constrain_zero_delay_paths(const bool& constrain_zero_delay_paths);
    void set_time_stamp(const bool& enable);
  private: /* Internal data */
    std::string sdc_dir_;
    bool flatten_names_;
    bool hierarchical_;
    float time_unit_;
    bool output_hierarchy_;
    bool constrain_global_port_; 
    bool constrain_non_clock_global_port_; 
    bool constrain_grid_; 
    bool constrain_sb_;
    bool constrain_cb_;
    bool constrain_configurable_memory_outputs_;
    bool constrain_routing_multiplexer_outputs_;
    bool constrain_switch_block_outputs_;
    bool constrain_zero_delay_paths_;
    bool time_stamp_;
};

} /* end namespace openfpga */

#endif
