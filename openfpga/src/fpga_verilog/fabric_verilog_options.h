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
 public: /* Types */
  enum class e_undriven_input_type {
    NONE = 0, /* Leave undriven input to be dangling */
    BUS0,     /* Wire to a bus format of constant 0 */
    BUS1,     /* Wire to a bus format of constant 1 */
    BIT0,     /* Wire to a blast-bit format of constant 0 */
    BIT1,     /* Wire to a blast-bit format of constant 1 */
    NUM_TYPES
  };

 public: /* Public constructor */
  /* Set default options */
  FabricVerilogOption();

 public: /* Public accessors */
  std::string output_directory() const;
  bool time_stamp() const;
  bool use_relative_path() const;
  bool include_timing() const;
  bool explicit_port_mapping() const;
  bool compress_routing() const;
  e_verilog_default_net_type default_net_type() const;
  bool print_user_defined_template() const;
  e_undriven_input_type constant_undriven_inputs() const;
  /* Identify if a bus format should be applied when wiring undriven inputs to
   * constants */
  bool constant_undriven_inputs_use_bus() const;
  /* Identify the logic value should be applied when wiring undriven inputs to
   * constants */
  size_t constant_undriven_inputs_value() const;
  std::string full_constant_undriven_input_type_str() const;
  bool little_endian() const;
  bool verbose_output() const;

 public: /* Public mutators */
  void set_output_directory(const std::string& output_dir);
  void set_use_relative_path(const bool& enabled);
  void set_time_stamp(const bool& enabled);
  void set_include_timing(const bool& enabled);
  void set_explicit_port_mapping(const bool& enabled);
  void set_compress_routing(const bool& enabled);
  void set_print_user_defined_template(const bool& enabled);
  void set_default_net_type(const std::string& default_net_type);
  /** Decode the type from string to enumeration
   * "none" -> NONE, "bus0" -> BUS0, "bus1" -> BUS1, "bit0" -> BIT0, "bit1" ->
   * BIT1 For invalid types, error out
   */
  bool set_constant_undriven_inputs(const std::string& type_str);
  /** For invalid types, error out */
  bool set_constant_undriven_inputs(const e_undriven_input_type& type);
  void set_little_endian(const bool& enabled);
  void set_verbose_output(const bool& enabled);

 private: /* Internal Data */
  std::string output_directory_;
  bool include_timing_;
  bool explicit_port_mapping_;
  bool compress_routing_;
  bool print_user_defined_template_;
  e_verilog_default_net_type default_net_type_;
  bool time_stamp_;
  bool use_relative_path_;
  e_undriven_input_type constant_undriven_inputs_;
  std::array<const char*,
             size_t(FabricVerilogOption::e_undriven_input_type::NUM_TYPES)>
    CONSTANT_UNDRIVEN_INPUT_TYPE_STRING_;  // String versions of constant
                                           // undriven input types
  bool little_endian_;
  bool verbose_output_;
};

} /* End namespace openfpga*/

#endif
