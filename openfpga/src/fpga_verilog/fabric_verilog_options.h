#ifndef FABRIC_VERILOG_OPTIONS_H
#define FABRIC_VERILOG_OPTIONS_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <string>
#include "verilog_port_types.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Options for Fabric Verilog generator
 *******************************************************************/
class FabricVerilogOption {
  public: /* Public constructor */
    /* Set default options */
    FabricVerilogOption();
  public: /* Public accessors */
    std::string output_directory() const;
    bool include_timing() const;
    bool explicit_port_mapping() const;
    bool compress_routing() const;
    e_verilog_default_net_type default_net_type() const;
    bool print_user_defined_template() const;
    bool verbose_output() const;
  public: /* Public mutators */
    void set_output_directory(const std::string& output_dir);
    void set_include_timing(const bool& enabled);
    void set_explicit_port_mapping(const bool& enabled);
    void set_compress_routing(const bool& enabled);
    void set_print_user_defined_template(const bool& enabled);
    void set_default_net_type(const std::string& default_net_type);
    void set_verbose_output(const bool& enabled);
  private: /* Internal Data */
    std::string output_directory_;
    bool include_timing_;
    bool explicit_port_mapping_;
    bool compress_routing_;
    bool print_user_defined_template_;
    e_verilog_default_net_type default_net_type_;
    bool verbose_output_;
};

} /* End namespace openfpga*/

#endif
