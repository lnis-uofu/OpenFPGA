#ifndef CONFIG_PROTOCOL_H
#define CONFIG_PROTOCOL_H

#include <string>
#include "circuit_types.h"
#include "circuit_library_fwd.h"

/* Data type to define the protocol through which BL/WL can be manipulated */
enum e_blwl_protocol_type {
  BLWL_PROTOCOL_FLATTEN,
  BLWL_PROTOCOL_DECODER,
  BLWL_PROTOCOL_SHIFT_REGISTER,
  NUM_BLWL_PROTOCOL_TYPES
};
constexpr std::array<const char*, NUM_BLWL_PROTOCOL_TYPES> BLWL_PROTOCOL_TYPE_STRING = {{"flatten", "decoder", "shift_register"}};

/********************************************************************
 * A data structure to store configuration protocol information
 *******************************************************************/
class ConfigProtocol {
  public:  /* Constructors */
    ConfigProtocol();
  public: /* Public Accessors */
    e_config_protocol_type type() const;
    std::string memory_model_name() const;
    CircuitModelId memory_model() const;
    int num_regions() const;

    e_blwl_protocol_type bl_protocol_type() const;
    std::string bl_memory_model_name() const;
    CircuitModelId bl_memory_model() const;
    size_t bl_num_banks() const;
    e_blwl_protocol_type wl_protocol_type() const;
    std::string wl_memory_model_name() const;
    CircuitModelId wl_memory_model() const;
    size_t wl_num_banks() const;
  public: /* Public Mutators */
    void set_type(const e_config_protocol_type& type);
    void set_memory_model_name(const std::string& memory_model_name);
    void set_memory_model(const CircuitModelId& memory_model);
    void set_num_regions(const int& num_regions);

    void set_bl_protocol_type(const e_blwl_protocol_type& type);
    void set_bl_memory_model_name(const std::string& memory_model_name);
    void set_bl_memory_model(const CircuitModelId& memory_model);
    void set_bl_num_banks(const size_t& num_banks);
    void set_wl_protocol_type(const e_blwl_protocol_type& type);
    void set_wl_memory_model_name(const std::string& memory_model_name);
    void set_wl_memory_model(const CircuitModelId& memory_model);
    void set_wl_num_banks(const size_t& num_banks);
  private: /* Internal data */
    /* The type of configuration protocol. 
     * In other words, it is about how to organize and access each configurable memory 
     */
    e_config_protocol_type type_;

    /* The circuit model of configuration memory to be used in the protocol */
    std::string memory_model_name_;
    CircuitModelId memory_model_;

    /* Number of configurable regions */
    int num_regions_;

    /* BL & WL protocol: This is only applicable to memory-bank configuration protocols
     * - type:               defines which protocol to be used. By default, we consider decoders
     * - bl/wl_memory_model: defines the circuit model to be used when building shift register chains for BL/WL configuration. 
     *                       It must be a valid CCFF circuit model. This is only applicable when shift-register protocol is selected
     *                       for BL or WL.
     * - bl/wl_num_banks:    defines the number of independent shift register chains (with separated head and tail ports)
     *                       for a given BL protocol per configuration region
     */
    e_blwl_protocol_type bl_protocol_type_ = BLWL_PROTOCOL_DECODER;
    std::string bl_memory_model_name_;
    CircuitModelId bl_memory_model_;
    size_t bl_num_banks_;
    e_blwl_protocol_type wl_protocol_type_ = BLWL_PROTOCOL_DECODER;
    std::string wl_memory_model_name_;
    CircuitModelId wl_memory_model_;
    size_t wl_num_banks_;
};

#endif
