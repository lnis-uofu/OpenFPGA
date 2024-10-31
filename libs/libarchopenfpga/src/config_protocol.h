#ifndef CONFIG_PROTOCOL_H
#define CONFIG_PROTOCOL_H

#include <map>
#include <string>

#include "circuit_library_fwd.h"
#include "circuit_types.h"
#include "openfpga_port.h"
#include "ql_memory_bank_config_setting.h"

/* Data type to define the protocol through which BL/WL can be manipulated */
enum e_blwl_protocol_type {
  BLWL_PROTOCOL_FLATTEN,
  BLWL_PROTOCOL_DECODER,
  BLWL_PROTOCOL_SHIFT_REGISTER,
  NUM_BLWL_PROTOCOL_TYPES
};
constexpr std::array<const char*, NUM_BLWL_PROTOCOL_TYPES>
  BLWL_PROTOCOL_TYPE_STRING = {{"flatten", "decoder", "shift_register"}};

/********************************************************************
 * A data structure to store configuration protocol information
 *******************************************************************/
class ConfigProtocol {
 public: /* Constructors */
  ConfigProtocol();

 public: /* Public Accessors */
  e_config_protocol_type type() const;
  std::string memory_model_name() const;
  CircuitModelId memory_model() const;
  int num_regions() const;

  /* Find the number of programming clocks, only valid for configuration chain
   * type! */
  size_t num_prog_clocks() const;
  /* Get information of the programming clock port: name and width */
  openfpga::BasicPort prog_clock_port_info() const;
  /* Get a list of programming clock pins, flatten from the programming clock
   * port */
  std::vector<openfpga::BasicPort> prog_clock_pins() const;
  /* Get a list of programming clock ports */
  std::string prog_clock_pin_ccff_head_indices_str(
    const openfpga::BasicPort& port) const;
  std::vector<size_t> prog_clock_pin_ccff_head_indices(
    const openfpga::BasicPort& port) const;

  e_blwl_protocol_type bl_protocol_type() const;
  std::string bl_memory_model_name() const;
  CircuitModelId bl_memory_model() const;
  size_t bl_num_banks() const;
  e_blwl_protocol_type wl_protocol_type() const;
  std::string wl_memory_model_name() const;
  CircuitModelId wl_memory_model() const;
  size_t wl_num_banks() const;

  /* QL Memory Bank Config Setting */
  const QLMemoryBankConfigSetting* ql_memory_bank_config_setting() const;

 public: /* Public Mutators */
  void set_type(const e_config_protocol_type& type);
  void set_memory_model_name(const std::string& memory_model_name);
  void set_memory_model(const CircuitModelId& memory_model);
  void set_num_regions(const int& num_regions);

  /* Add the programming clock port */
  void set_prog_clock_port(const openfpga::BasicPort& port);
  /* Add a pair of programming clock pin and ccff head indices. This API will
   * parse the index list, e.g., "0,1" to a vector of integers [0 1] */
  void set_prog_clock_pin_ccff_head_indices_pair(
    const openfpga::BasicPort& pin, const std::string& indices_str);

  void set_bl_protocol_type(const e_blwl_protocol_type& type);
  void set_bl_memory_model_name(const std::string& memory_model_name);
  void set_bl_memory_model(const CircuitModelId& memory_model);
  void set_bl_num_banks(const size_t& num_banks);
  void set_wl_protocol_type(const e_blwl_protocol_type& type);
  void set_wl_memory_model_name(const std::string& memory_model_name);
  void set_wl_memory_model(const CircuitModelId& memory_model);
  void set_wl_num_banks(const size_t& num_banks);

  /* QL Memory Bank Config Setting */
  QLMemoryBankConfigSetting* get_ql_memory_bank_config_setting();

 public: /* Public validators */
  /* Check if internal data has any conflicts to each other. Return number of
   * errors detected */
  int validate() const;

 private: /* Private validators */
  /* For configuration chains, to validate if
   * - programming clocks is smaller than the number of regions
   * - programming clocks does not have any conflicts in controlling regions (no
   * overlaps)
   * - each region has been assigned to a programming clock
   * Return number of errors detected
   */
  int validate_ccff_prog_clocks() const;

 private: /* Internal data */
  /* The type of configuration protocol.
   * In other words, it is about how to organize and access each configurable
   * memory
   */
  e_config_protocol_type type_;

  /* The circuit model of configuration memory to be used in the protocol */
  std::string memory_model_name_;
  CircuitModelId memory_model_;

  /* Number of configurable regions */
  int num_regions_;

  /* Programming clock managment: This is only applicable to configuration chain
   * protocols */
  openfpga::BasicPort prog_clk_port_;
  std::vector<std::vector<size_t>> prog_clk_ccff_head_indices_;
  char INDICE_STRING_DELIM_;

  /* BL & WL protocol: This is only applicable to memory-bank configuration
   * protocols
   * - type:               defines which protocol to be used. By default, we
   * consider decoders
   * - bl/wl_memory_model: defines the circuit model to be used when building
   * shift register chains for BL/WL configuration. It must be a valid CCFF
   * circuit model. This is only applicable when shift-register protocol is
   * selected for BL or WL.
   * - bl/wl_num_banks:    defines the number of independent shift register
   * chains (with separated head and tail ports) for a given BL protocol per
   * configuration region
   */
  e_blwl_protocol_type bl_protocol_type_ = BLWL_PROTOCOL_DECODER;
  std::string bl_memory_model_name_;
  CircuitModelId bl_memory_model_;
  size_t bl_num_banks_;
  e_blwl_protocol_type wl_protocol_type_ = BLWL_PROTOCOL_DECODER;
  std::string wl_memory_model_name_;
  CircuitModelId wl_memory_model_;
  size_t wl_num_banks_;

  /* QL Memory Bank Config Setting */
  QLMemoryBankConfigSetting ql_memory_bank_config_setting_;
};

#endif
