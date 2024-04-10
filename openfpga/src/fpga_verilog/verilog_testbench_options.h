#ifndef VERILOG_TESTBENCH_OPTIONS_H
#define VERILOG_TESTBENCH_OPTIONS_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <string>

#include "verilog_port_types.h"

/* Begin namespace openfpga */
namespace openfpga {

/* Embedded bitstream code style */
enum e_embedded_bitstream_hdl_type {
  EMBEDDED_BITSTREAM_HDL_IVERILOG,
  EMBEDDED_BITSTREAM_HDL_MODELSIM,
  NUM_EMBEDDED_BITSTREAM_HDL_TYPES
};

constexpr std::array<const char*, NUM_EMBEDDED_BITSTREAM_HDL_TYPES + 1>
  EMBEDDED_BITSTREAM_HDL_TYPE_STRING = {
    {"iverilog", "modelsim", "none"}};  // String versions of default net types

/********************************************************************
 * Options for Verilog Testbench generator
 * Typicall usage:
 *   VerilogTestbench verilog_tb_opt();
 *   // Set options
 *   ...
 *
 *******************************************************************/
class VerilogTestbenchOption {
  /* Public types */
 public:
  /* Embedded bitstream code style */
  enum class e_simulator_type { IVERILOG = 0, VCS, NUM_TYPES };
  /* Constants */
 private:
  /* String version of simulator types. Used for debugging/error messages */
  std::array<const char*, size_t(e_simulator_type::NUM_TYPES)>
    SIMULATOR_TYPE_STRING_;

 public: /* Public constructor */
  /* Set default options */
  VerilogTestbenchOption();

 public: /* Public accessors */
  std::string output_directory() const;
  std::string top_module() const;
  std::string dut_module() const;
  std::string fabric_netlist_file_path() const;
  std::string reference_benchmark_file_path() const;
  bool fast_configuration() const;
  bool print_formal_verification_top_netlist() const;
  bool print_preconfig_top_testbench() const;
  bool print_top_testbench() const;
  bool print_simulation_ini() const;
  std::string simulation_ini_path() const;
  bool explicit_port_mapping() const;
  bool include_signal_init() const;
  bool dump_waveform() const;
  bool no_self_checking() const;
  e_verilog_default_net_type default_net_type() const;
  e_embedded_bitstream_hdl_type embedded_bitstream_hdl_type() const;
  float time_unit() const;
  bool time_stamp() const;
  bool use_relative_path() const;
  bool verbose_output() const;
  e_simulator_type simulator_type() const;

 public: /* Public validator */
  bool validate() const;

 public: /* Public mutators */
  void set_output_directory(const std::string& output_dir);
  void set_top_module(const std::string& top_module);
  void set_dut_module(const std::string& dut_module);
  /* The reference verilog file path is the key parameters that will have an
   * impact on other options:
   *  - print_preconfig_top_testbench
   *  - print_top_testbench
   * If the file path is empty, the above testbench generation will not be
   * enabled
   */
  void set_reference_benchmark_file_path(
    const std::string& reference_benchmark_file_path);
  /* The fabric netlist file path is an optional parameter
   * to allow users to specify a fabric netlist at another location
   */
  void set_fabric_netlist_file_path(
    const std::string& fabric_netlist_file_path);
  void set_print_formal_verification_top_netlist(const bool& enabled);
  /* The preconfig top testbench generation can be enabled only when formal
   * verification top netlist is enabled */
  void set_print_preconfig_top_testbench(const bool& enabled);
  void set_fast_configuration(const bool& enabled);
  void set_print_top_testbench(const bool& enabled);
  void set_print_simulation_ini(const std::string& simulation_ini_path);
  void set_explicit_port_mapping(const bool& enabled);
  void set_include_signal_init(const bool& enabled);
  void set_dump_waveform(const bool& enabled);
  void set_default_net_type(const std::string& default_net_type);
  void set_time_unit(const float& time_unit);
  void set_embedded_bitstream_hdl_type(
    const std::string& embedded_bitstream_hdl_type);
  void set_time_stamp(const bool& enabled);
  void set_use_relative_path(const bool& enabled);
  void set_verbose_output(const bool& enabled);

  /* @brief Create the simulator type by parsing a given string. Return error
   * when failed */
  int set_simulator_type(const std::string& value);

 private: /* Private utility and validators */
  e_simulator_type str2simulator_type(const std::string& value,
                                      const bool& verbose = false) const;
  std::string simulator_type2str(const e_simulator_type& sim_type,
                                 const bool& verbose = false) const;
  std::string simulator_type_all2str() const;
  bool valid_simulator_type(const e_simulator_type& sim_type) const;

 private: /* Internal Data */
  std::string output_directory_;
  std::string top_module_;
  std::string dut_module_;
  std::string fabric_netlist_file_path_;
  std::string reference_benchmark_file_path_;
  bool fast_configuration_;
  bool print_formal_verification_top_netlist_;
  bool print_preconfig_top_testbench_;
  bool print_top_testbench_;
  /* Print simulation ini is enabled only when the path is not empty */
  std::string simulation_ini_path_;
  bool explicit_port_mapping_;
  bool include_signal_init_;
  bool dump_waveform_;
  e_verilog_default_net_type default_net_type_;
  e_embedded_bitstream_hdl_type embedded_bitstream_hdl_type_;
  e_simulator_type simulator_type_;
  float time_unit_;
  bool time_stamp_;
  bool use_relative_path_;
  bool verbose_output_;
};

} /* End namespace openfpga*/

#endif
