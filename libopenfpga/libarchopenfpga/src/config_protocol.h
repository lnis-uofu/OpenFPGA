#ifndef CONFIG_PROTOCOL_H
#define CONFIG_PROTOCOL_H

#include <string>
#include "circuit_types.h"
#include "circuit_library_fwd.h"

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
  public: /* Public Mutators */
    void set_type(const e_config_protocol_type& type);
    void set_memory_model_name(const std::string& memory_model_name);
    void set_memory_model(const CircuitModelId& memory_model);
  private: /* Internal data */
    /* The type of configuration protocol. 
     * In other words, it is about how to organize and access each configurable memory 
     */
    e_config_protocol_type type_;

    /* The circuit model of configuration memory to be used in the protocol */
    std::string memory_model_name_;
    CircuitModelId memory_model_;
};

#endif
