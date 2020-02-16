#ifndef VERILOG_OPTIONS_H
#define VERILOG_OPTIONS_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <string>

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * FlowManager aims to resolve the dependency between OpenFPGA functional
 * code blocks
 * It can provide flags for downstream modules about if the data structures
 * they require have already been constructed
 *
 *******************************************************************/
class FabricVerilogOption {
  public: /* Public accessors */
    std::string output_directory() const;
    bool support_icarus_simulator() const;
    bool include_timing() const;
    bool include_signal_init() const;
    bool explicit_port_mapping() const;
    bool compress_routing() const;
    bool verbose_output() const;
  public: /* Public mutators */
    void set_output_directory(const std::string& output_dir);
    void set_support_icarus_simulator(const bool& enabled);
    void set_include_timing(const bool& enabled);
    void set_include_signal_init(const bool& enabled);
    void set_explicit_port_mapping(const bool& enabled);
    void set_compress_routing(const bool& enabled);
    void set_verbose_output(const bool& enabled);
  private: /* Internal Data */
    std::string output_directory_;
    bool support_icarus_simulator_;
    bool include_signal_init_;
    bool include_timing_;
    bool explicit_port_mapping_;
    bool compress_routing_;
    bool verbose_output_;
};

} /* End namespace openfpga*/

#endif
