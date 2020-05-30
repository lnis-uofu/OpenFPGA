#ifndef VERILOG_TESTBENCH_OPTIONS_H
#define VERILOG_TESTBENCH_OPTIONS_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <string>

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Options for Verilog Testbench generator
 * Typicall usage:
 *   VerilogTestbench verilog_tb_opt();
 *   // Set options
 *   ...
 *
 *******************************************************************/
class VerilogTestbenchOption {
  public: /* Public constructor */
    /* Set default options */
    VerilogTestbenchOption();
  public: /* Public accessors */
    std::string output_directory() const;
    std::string reference_benchmark_file_path() const;
    bool fast_configuration() const;
    bool print_formal_verification_top_netlist() const;
    bool print_preconfig_top_testbench() const;
    bool print_top_testbench() const;
    bool print_simulation_ini() const;
    std::string simulation_ini_path() const;
    bool explicit_port_mapping() const;
    bool verbose_output() const;
  public: /* Public validator */
    bool validate() const;
  public: /* Public mutators */
    void set_output_directory(const std::string& output_dir);
    /* The reference verilog file path is the key parameters that will have an impact on other options:
     *  - print_preconfig_top_testbench
     *  - print_top_testbench
     * If the file path is empty, the above testbench generation will not be enabled 
     */
    void set_reference_benchmark_file_path(const std::string& reference_benchmark_file_path);
    void set_print_formal_verification_top_netlist(const bool& enabled);
    /* The preconfig top testbench generation can be enabled only when formal verification top netlist is enabled */
    void set_print_preconfig_top_testbench(const bool& enabled);
    void set_fast_configuration(const bool& enabled);
    void set_print_top_testbench(const bool& enabled);
    void set_print_simulation_ini(const std::string& simulation_ini_path);
    void set_explicit_port_mapping(const bool& enabled);
    void set_verbose_output(const bool& enabled);
  private: /* Internal Data */
    std::string output_directory_;
    std::string reference_benchmark_file_path_;
    bool fast_configuration_;
    bool print_formal_verification_top_netlist_;
    bool print_preconfig_top_testbench_;
    bool print_top_testbench_;
    /* Print simulation ini is enabled only when the path is not empty */
    std::string simulation_ini_path_;
    bool explicit_port_mapping_;
    bool verbose_output_;
};

} /* End namespace openfpga*/

#endif
