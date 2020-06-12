#ifndef FABRIC_VERILOG_OPTIONS_H
#define FABRIC_VERILOG_OPTIONS_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <string>

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
    bool support_icarus_simulator() const;
    bool include_timing() const;
    bool include_signal_init() const;
    bool explicit_port_mapping() const;
    bool compress_routing() const;
    bool print_top_testbench() const;
    bool print_formal_verification_top_netlist() const;
    bool print_autocheck_top_testbench() const;
    std::string reference_verilog_file_path() const;
    bool print_user_defined_template() const;
    bool verbose_output() const;
  public: /* Public mutators */
    void set_output_directory(const std::string& output_dir);
    void set_support_icarus_simulator(const bool& enabled);
    void set_include_timing(const bool& enabled);
    void set_include_signal_init(const bool& enabled);
    void set_explicit_port_mapping(const bool& enabled);
    void set_compress_routing(const bool& enabled);
    void set_print_top_testbench(const bool& enabled);
    void set_print_formal_verification_top_netlist(const bool& enabled);
    void set_print_autocheck_top_testbench(const std::string& reference_verilog_file_path);
    void set_print_user_defined_template(const bool& enabled);
    void set_verbose_output(const bool& enabled);
  private: /* Internal Data */
    std::string output_directory_;
    bool support_icarus_simulator_;
    bool include_signal_init_;
    bool include_timing_;
    bool explicit_port_mapping_;
    bool compress_routing_;
    bool print_top_testbench_;
    bool print_formal_verification_top_netlist_;
    /* print_autocheck_top_testbench will be enabled when reference file path is not empty */
    std::string reference_verilog_file_path_;
    bool print_user_defined_template_;
    bool verbose_output_;
};

} /* End namespace openfpga*/

#endif
