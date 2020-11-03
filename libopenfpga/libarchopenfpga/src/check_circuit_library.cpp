/************************************************************************
 * Function to perform fundamental checking for the circuit library 
 * such as 
 * 1. if default circuit models are defined 
 * 2. if any circuit models shared the same name or prefix 
 * 3. if nay circuit model miss mandatory ports 
 ***********************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

#include "check_circuit_library.h"

/************************************************************************
 * Circuit models have unique names, return the number of errors 
 *  If not found, we give an error
 ***********************************************************************/
static  
size_t check_circuit_library_unique_names(const CircuitLibrary& circuit_lib) {
  size_t num_err = 0;

  for (size_t i = 0; i < circuit_lib.num_models(); ++i) {
    /* Skip for the last element, because the inner loop will access it */
    if (i == circuit_lib.num_models() - 1) {
      continue;
    }
    /* Get the name of reference */
    const std::string& i_name = circuit_lib.model_name(CircuitModelId(i));
    for (size_t j = i + 1; j < circuit_lib.num_models(); ++j) {
      /* Compare the name of candidate */
      const std::string& j_name = circuit_lib.model_name(CircuitModelId(j));
      /* Compare the name and skip for different names */
      if (0 != i_name.compare(j_name)) {
        continue;
      }
      VTR_LOG_ERROR("Circuit model(index=%d) and (index=%d) share the same name, which is invalid!\n",
                    i , j, i_name.c_str());
      /* Incremental the counter for errors */
      num_err++;
    }
  }

  return num_err;
}


/************************************************************************
 * Circuit models have unique names, return the number of errors 
 *  If not found, we give an error
 ***********************************************************************/
static 
size_t check_circuit_library_unique_prefix(const CircuitLibrary& circuit_lib) {
  size_t num_err = 0;

  for (size_t i = 0; i < circuit_lib.num_models(); ++i) {
    /* Skip for the last element, because the inner loop will access it */
    if (i == circuit_lib.num_models() - 1) {
      continue;
    }
    /* Get the name of reference */
    const std::string& i_prefix = circuit_lib.model_prefix(CircuitModelId(i));
    for (size_t j = i + 1; j < circuit_lib.num_models(); ++j) {
      /* Compare the name of candidate */
      const std::string& j_prefix = circuit_lib.model_prefix(CircuitModelId(j));
      /* Compare the name and skip for different prefix */
      if (0 != i_prefix.compare(j_prefix)) {
        continue;
      }
      VTR_LOG_ERROR("Circuit model(name=%s) and (name=%s) share the same prefix, which is invalid!\n",
                    circuit_lib.model_name(CircuitModelId(i)).c_str(),
                    circuit_lib.model_name(CircuitModelId(j)).c_str(),
                    i_prefix.c_str());
      /* Incremental the counter for errors */
      num_err++;
    }
  }

  return num_err;
}

/************************************************************************
 * A generic function to check the port list of a circuit model in a given type
 *  If not found, we give an error
 ***********************************************************************/
static 
size_t check_circuit_model_required(const CircuitLibrary& circuit_lib,
                                    const enum e_circuit_model_type& circuit_model_type_to_check) {
  size_t num_err = 0;

  /* We must have an IOPAD*/
  if ( 0 == circuit_lib.models_by_type(circuit_model_type_to_check).size()) {
    VTR_LOG_ERROR("At least one %s circuit model is required!\n",
                  CIRCUIT_MODEL_TYPE_STRING[size_t(circuit_model_type_to_check)]);
    /* Incremental the counter for errors */
    num_err++;
  }

  return num_err;
}

/************************************************************************
 * A generic function to check the port list of a circuit model in a given type 
 *  If not found, we give an error
 ***********************************************************************/
size_t check_one_circuit_model_port_required(const CircuitLibrary& circuit_lib,
                                             const CircuitModelId& circuit_model, 
                                             const std::vector<enum e_circuit_model_port_type>& port_types_to_check) {
  size_t num_err = 0;

  for (const auto& port_type: port_types_to_check) {
    if (0 == circuit_lib.model_ports_by_type(circuit_model, port_type).size()) {
      VTR_LOG_ERROR("%s circuit model(name=%s) does not have %s port\n",
                    CIRCUIT_MODEL_TYPE_STRING[size_t(circuit_lib.model_type(circuit_model))], 
                    circuit_lib.model_name(circuit_model).c_str(), 
                    CIRCUIT_MODEL_PORT_TYPE_STRING[size_t(port_type)]);
      /* Incremental the counter for errors */
      num_err++;
    }
  }

  return num_err;
}

/************************************************************************
 * A generic function to check the port size of a given circuit model
 * if the port size does not match, we give an error 
 ***********************************************************************/
size_t check_one_circuit_model_port_size_required(const CircuitLibrary& circuit_lib,
                                                  const CircuitModelId& circuit_model, 
                                                  const CircuitPortId& circuit_port,
                                                  const size_t& port_size_to_check) {

  size_t num_err = 0;

  if (port_size_to_check != circuit_lib.port_size(circuit_port)) {
    VTR_LOG_ERROR("Expect circuit model %s to have %d %s ports but only see %d!\n",
                  circuit_lib.model_name(circuit_model).c_str(), 
                  port_size_to_check,
                  CIRCUIT_MODEL_PORT_TYPE_STRING[size_t(circuit_lib.port_type(circuit_port))],
                  circuit_lib.port_size(circuit_port));
    /* Incremental the counter for errors */
    num_err++;
  }

  return num_err;
}

/************************************************************************
 * A generic function to check the port size of a given circuit model
 * if the number of ports in the given type does not match, we give an error 
 * for each port, if the port size does not match, we give an error 
 ***********************************************************************/
size_t check_one_circuit_model_port_type_and_size_required(const CircuitLibrary& circuit_lib,
                                                           const CircuitModelId& circuit_model, 
                                                           const enum e_circuit_model_port_type& port_type_to_check,
                                                           const size_t& num_ports_to_check,
                                                           const size_t& port_size_to_check,
                                                           const bool& include_global_ports) {

  size_t num_err = 0;

  std::vector<CircuitPortId> ports = circuit_lib.model_ports_by_type(circuit_model, port_type_to_check, false == include_global_ports);
  if (num_ports_to_check != ports.size()) {
    VTR_LOG_ERROR("Expect %d %s ports for a %s circuit model, but only have %d %s ports!\n",
                  num_ports_to_check,
                  CIRCUIT_MODEL_PORT_TYPE_STRING[size_t(port_type_to_check)],
                  CIRCUIT_MODEL_TYPE_STRING[size_t(circuit_lib.model_type(circuit_model))], 
               ports.size(),
               CIRCUIT_MODEL_PORT_TYPE_STRING[size_t(port_type_to_check)]);
    num_err++;
  } 
  for (const auto& port : ports) { 
    num_err += check_one_circuit_model_port_size_required(circuit_lib,
                                                          circuit_model, 
                                                          port, port_size_to_check);
  }

  return num_err;
}

/************************************************************************
 * A generic function to check the port list of circuit models in a given type 
 *  If not found, we give an error
 ***********************************************************************/
static 
size_t check_circuit_model_port_required(const CircuitLibrary& circuit_lib,
                                         const enum e_circuit_model_type& circuit_model_type_to_check, 
                                         const std::vector<enum e_circuit_model_port_type>& port_types_to_check) {
  size_t num_err = 0;

  for (const auto& id : circuit_lib.models_by_type(circuit_model_type_to_check)) {
    num_err += check_one_circuit_model_port_required(circuit_lib, id, port_types_to_check); 
  }

  return num_err;
}

/************************************************************************
 *  A generic function to search each default circuit model by types 
 *  that have been defined by users.
 *  If a type of circuit model is defined, we expect there is a default model 
 *  to be specified
 ***********************************************************************/
static 
size_t check_default_circuit_model_by_types(const CircuitLibrary& circuit_lib) {
  size_t num_err = 0;

  for (size_t itype = 0; itype < NUM_CIRCUIT_MODEL_TYPES; ++itype) {
    std::vector<CircuitModelId> curr_models = circuit_lib.models_by_type(e_circuit_model_type(itype));
    if (0 == curr_models.size()) {
       continue;
    }
    /* Go through the models and try to find a default one */
    size_t found_default_counter = 0;
    for (const auto& curr_model : curr_models) {
      if (true == circuit_lib.model_is_default(curr_model)) {
        found_default_counter++;
      }
    }
    if (0 == found_default_counter) {
      VTR_LOG_ERROR("Miss a default circuit model for the type %s! Try to define it in your architecture file!\n",
                    CIRCUIT_MODEL_TYPE_STRING[itype]);
      num_err++;
    }
    if (1 < found_default_counter) {
      VTR_LOG_ERROR("Found >1 default circuit models for the type %s! Expect only one!\n",
                    CIRCUIT_MODEL_TYPE_STRING[itype]);
      num_err++;
    }
  }

  return num_err;
}

/************************************************************************
 *  A generic function to find the default circuit model with a given type
 *  If not found, we give an error
 ***********************************************************************/
static 
size_t check_required_default_circuit_model(const CircuitLibrary& circuit_lib,
                                            const enum e_circuit_model_type& circuit_model_type) {
  size_t num_err = 0;

  if (CircuitModelId::INVALID() == circuit_lib.default_model(circuit_model_type)) {
    VTR_LOG_ERROR("Miss a default circuit model for the type %s! Try to define it in your architecture file!\n",
                  CIRCUIT_MODEL_TYPE_STRING[size_t(circuit_model_type)]);
    num_err++;
  }

  return num_err;
}

/************************************************************************
 * A function to check the port map of FF circuit model
 ***********************************************************************/
size_t check_ff_circuit_model_ports(const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model) {
  size_t num_err = 0;

  /* Check the type of circuit model */
  VTR_ASSERT(CIRCUIT_MODEL_FF == circuit_lib.model_type(circuit_model));
  /* Check if we have D, Set and Reset */
  num_err += check_one_circuit_model_port_type_and_size_required(circuit_lib, circuit_model, 
                                                                 CIRCUIT_MODEL_PORT_INPUT,
                                                                 3, 1, false);
  /* Check if we have a clock */
  num_err += check_one_circuit_model_port_type_and_size_required(circuit_lib, circuit_model, 
                                                                 CIRCUIT_MODEL_PORT_CLOCK,
                                                                 1, 1, false);


  /* Check if we have output */
  num_err += check_one_circuit_model_port_type_and_size_required(circuit_lib, circuit_model, 
                                                                 CIRCUIT_MODEL_PORT_OUTPUT,
                                                                 1, 1, false);

  return num_err;
}

/************************************************************************
 * A function to check the port map of CCFF circuit model
 ***********************************************************************/
size_t check_ccff_circuit_model_ports(const CircuitLibrary& circuit_lib,
                                      const CircuitModelId& circuit_model) {
  size_t num_err = 0;

  /* Check the type of circuit model */
  VTR_ASSERT(CIRCUIT_MODEL_CCFF == circuit_lib.model_type(circuit_model));

  /* Check if we have D, Set and Reset */
  num_err += check_one_circuit_model_port_type_and_size_required(circuit_lib, circuit_model, 
                                                                 CIRCUIT_MODEL_PORT_INPUT,
                                                                 1, 1, false);
  /* Check if we have a clock */
  num_err += check_one_circuit_model_port_type_and_size_required(circuit_lib, circuit_model, 
                                                                 CIRCUIT_MODEL_PORT_CLOCK,
                                                                 1, 1, true);


  /* Check if we have output */
  num_err += check_one_circuit_model_port_type_and_size_required(circuit_lib, circuit_model, 
                                                                 CIRCUIT_MODEL_PORT_OUTPUT,
                                                                 2, 1, false);

  return num_err;
}

/************************************************************************
 * A function to check the port map of SRAM circuit model
 ***********************************************************************/
size_t check_sram_circuit_model_ports(const CircuitLibrary& circuit_lib,
                                      const CircuitModelId& circuit_model,
                                      const bool& check_blwl) {
  size_t num_err = 0;

  /* Check the type of circuit model */
  VTR_ASSERT(CIRCUIT_MODEL_SRAM == circuit_lib.model_type(circuit_model));

  /* Check if we has 1 output with size 2 */
  num_err += check_one_circuit_model_port_type_and_size_required(circuit_lib, circuit_model, 
                                                                 CIRCUIT_MODEL_PORT_OUTPUT,
                                                                 2, 1, false);
  /* basic check finished here */
  if (false == check_blwl) {
    return num_err;
  }

  /* If bl and wl are required, check their existence */
  num_err += check_one_circuit_model_port_type_and_size_required(circuit_lib, circuit_model, 
                                                                 CIRCUIT_MODEL_PORT_BL,
                                                                 1, 1, false);

  num_err += check_one_circuit_model_port_type_and_size_required(circuit_lib, circuit_model, 
                                                                 CIRCUIT_MODEL_PORT_WL,
                                                                 1, 1, false);

  return num_err;
}

/************************************************************************
 * Check all the ports make sure, they satisfy the restriction 
 ***********************************************************************/
static 
size_t check_circuit_library_ports(const CircuitLibrary& circuit_lib) {
  size_t num_err = 0;
  
  /* Check global ports: make sure all the global ports are input ports */
  for (const auto& port : circuit_lib.ports()) {
    if ( (circuit_lib.port_is_global(port)) 
      && (!circuit_lib.is_input_port(port)) 
      && (!circuit_lib.is_output_port(port)) ) {
      VTR_LOG_ERROR("Circuit port (type=%s) of model (name=%s) is defined as global but not an input/output port!\n",
                    CIRCUIT_MODEL_PORT_TYPE_STRING[size_t(circuit_lib.port_type(port))],
                    circuit_lib.model_name(port).c_str());
      num_err++;
    }
  }

  /* Check global output ports: make sure they are all I/Os */
  for (const auto& port : circuit_lib.ports()) {
    if ( (circuit_lib.port_is_global(port)) 
      && (CIRCUIT_MODEL_PORT_OUTPUT == circuit_lib.port_type(port))
      && (false == circuit_lib.port_is_io(port)) ) {
      VTR_LOG_ERROR("Circuit port (type=%s) of model (name=%s) is defined as global output port but not an I/O!\n",
                    CIRCUIT_MODEL_PORT_TYPE_STRING[size_t(circuit_lib.port_type(port))],
                    circuit_lib.model_name(port).c_str());
      num_err++;
    }
  }

  /* Check set/reset/config_enable ports: make sure they are all global ports */
  for (const auto& port : circuit_lib.ports()) {
    if ( ( (circuit_lib.port_is_set(port)) 
        || (circuit_lib.port_is_reset(port)) 
        || (circuit_lib.port_is_config_enable(port)) )
      && (!circuit_lib.port_is_global(port)) ) {
      VTR_LOG_ERROR("Circuit port (type=%s) of model (name=%s) is defined as a set/reset/config_enable port but it is not global!\n",
                    CIRCUIT_MODEL_PORT_TYPE_STRING[size_t(circuit_lib.port_type(port))],
                    circuit_lib.model_name(port).c_str());
      num_err++;
    }
  }

  /* Check the tri-state map of ports, the length should match the port size! */
  for (const auto& port : circuit_lib.ports()) {
    if (circuit_lib.port_tri_state_map(port).empty()) {
      continue; /* No tri-state map is found, go to the next */
    }
    if (circuit_lib.port_tri_state_map(port).length() == circuit_lib.port_size(port)) {
      continue; /* Sizes match, go to the next */
    }
    /* We have a problem here, sizes do not match, leave a message and raise the error flag */
    VTR_LOG_ERROR("Tri-state map (=%s) of circuit port (type=%s) of model (name=%s) does not match the port size (=%lu)!\n",
               circuit_lib.port_tri_state_map(port).c_str(),
               CIRCUIT_MODEL_PORT_TYPE_STRING[size_t(circuit_lib.port_type(port))],
               circuit_lib.model_name(port).c_str(),
               circuit_lib.port_size(port));
    num_err++;
  }

  /* Check all the global ports which sare the same name also share the same attributes:
   * default_value, is_config, is_reset, is_set etc. 
   */
  std::vector<CircuitPortId> global_ports;

  /* Collect all the global ports */
  for (auto port : circuit_lib.ports()) {
    /* By pass non-global ports*/
    if (false == circuit_lib.port_is_global(port)) {
      continue;
    }
    global_ports.push_back(port);
  }

  for (size_t iport = 0; iport < global_ports.size() - 1; ++iport) {
    for (size_t jport = iport + 1; jport < global_ports.size(); ++jport) {
      /* Bypass those do not share the same name */
      if (0 != circuit_lib.port_prefix(global_ports[iport]).compare(circuit_lib.port_prefix(global_ports[jport]))) {
        continue;
      }

      /* Check if a same port share the same attributes */
      CircuitModelId iport_parent_model = circuit_lib.port_parent_model(global_ports[iport]);
      CircuitModelId jport_parent_model = circuit_lib.port_parent_model(global_ports[jport]);

      if (circuit_lib.port_default_value(global_ports[iport]) != circuit_lib.port_default_value(global_ports[jport])) { 
        VTR_LOG_ERROR("Global ports %s from circuit model %s and %s share the same name but have different dfefault values(%lu and %lu)!\n",
                      circuit_lib.port_prefix(global_ports[iport]).c_str(),
                      circuit_lib.model_name(iport_parent_model).c_str(),
                      circuit_lib.model_name(jport_parent_model).c_str(),
                      circuit_lib.port_default_value(global_ports[iport]),
                      circuit_lib.port_default_value(global_ports[jport])
                      ); 
        num_err++;
      }

      if (circuit_lib.port_is_reset(global_ports[iport]) != circuit_lib.port_is_reset(global_ports[jport])) { 
        VTR_LOG_ERROR("Global ports %s from circuit model %s and %s share the same name but have different is_reset attributes!\n",
                      circuit_lib.port_prefix(global_ports[iport]).c_str(),
                      circuit_lib.model_name(iport_parent_model).c_str(),
                      circuit_lib.model_name(jport_parent_model).c_str() 
                      ); 
        num_err++;
      }
      if (circuit_lib.port_is_set(global_ports[iport]) != circuit_lib.port_is_set(global_ports[jport])) { 
        VTR_LOG_ERROR("Global ports %s from circuit model %s and %s share the same name but have different is_set attributes!\n",
                      circuit_lib.port_prefix(global_ports[iport]).c_str(),
                      circuit_lib.model_name(iport_parent_model).c_str(),
                      circuit_lib.model_name(jport_parent_model).c_str() 
                      ); 
        num_err++;
      }
      if (circuit_lib.port_is_config_enable(global_ports[iport]) != circuit_lib.port_is_config_enable(global_ports[jport])) { 
        VTR_LOG_ERROR("Global ports %s from circuit model %s and %s share the same name but have different is_config_enable attributes!\n",
                      circuit_lib.port_prefix(global_ports[iport]).c_str(),
                      circuit_lib.model_name(iport_parent_model).c_str(),
                      circuit_lib.model_name(jport_parent_model).c_str() 
                      ); 
        num_err++;
      }
      if (circuit_lib.port_is_prog(global_ports[iport]) != circuit_lib.port_is_prog(global_ports[jport])) { 
        VTR_LOG_ERROR("Global ports %s from circuit model %s and %s share the same name but have different is_prog attributes!\n",
                      circuit_lib.port_prefix(global_ports[iport]).c_str(),
                      circuit_lib.model_name(iport_parent_model).c_str(),
                      circuit_lib.model_name(jport_parent_model).c_str() 
                      ); 
        num_err++;
      }
    }
  }

  return num_err;
}

/************************************************************************
 * Check the port requirements for a power-gated circuit model 
 * - It must have at least 2 global ports and which are config enable signals
 * - It must have an Enable port which control power gating 
 * - It must have an EnableB port which control power gating 
 ***********************************************************************/
static 
int check_power_gated_circuit_model(const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model) {
  int num_err = 0;

  std::vector<CircuitPortId> global_ports = circuit_lib.model_global_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true, true);

  /* If the circuit model is power-gated, we need to find at least one global config_enable signals */
  VTR_ASSERT(true == circuit_lib.is_power_gated(circuit_model));
  /* Check all the ports we have are good for a power-gated circuit model */
  /* We need at least one global port */
  if (2 > global_ports.size())  {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Expect at least two global ports (a pair of EN/Enb) for circuit model '%s' which is power-gated!\n",
                   circuit_lib.model_name(circuit_model).c_str()); 
    num_err++;
  }
  /* All the global ports should be config_enable */
  int num_config_enable_ports = 0;
  for (const auto& port : global_ports) {
    if (true == circuit_lib.port_is_config_enable(port)) {
      num_config_enable_ports++;
    }
  }

  if (2 != num_config_enable_ports) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Circuit model '%s' is power-gated. Two config-enable global ports are required!\n",
                   circuit_lib.model_name(circuit_model).c_str()); 
    num_err++;
  }
  /* Report errors if there are any */
  if (0 < num_err) {
    return num_err;
  }

  /* Try to find a pair of Enable and ENb ports from the global ports */
  CircuitPortId en_port = CircuitPortId::INVALID();
  CircuitPortId enb_port = CircuitPortId::INVALID();
  for (const auto& port : global_ports) {
    /* Focus on config_enable ports which are power-gate control signals */
    if (false == circuit_lib.port_is_config_enable(port)) {
      continue;
    }
    if (0 == circuit_lib.port_default_value(port)) {
      en_port = port;
    } else {
      VTR_ASSERT(1 == circuit_lib.port_default_value(port));
      enb_port = port;
    }
  }
  /* We must have valid EN/ENb ports */
  if (false == circuit_lib.valid_circuit_port_id(en_port)) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Fail to find an enable port for the circuit model '%s' is power-gated!\n",
                   circuit_lib.model_name(circuit_model).c_str()); 
  }
  if (false == circuit_lib.valid_circuit_port_id(enb_port)) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Fail to find an inverted enable port for the circuit model '%s' is power-gated!\n",
                   circuit_lib.model_name(circuit_model).c_str()); 
  }

  return num_err;
}

/************************************************************************
 * Check the port requirements for each power-gated circuit model 
 ***********************************************************************/
static 
int check_power_gated_circuit_models(const CircuitLibrary& circuit_lib) {
  int num_err = 0;

  for (const CircuitModelId& circuit_model : circuit_lib.models()) {
    if (true == circuit_lib.is_power_gated(circuit_model)) {
      num_err += check_power_gated_circuit_model(circuit_lib, circuit_model);
    }
  }

  return num_err;
}

/************************************************************************
 * Check io has been defined and has input and output ports 
 * - We must have global I/O port, either its type is inout, input or output
 * - For each IOPAD, we must have at least an input an output 
 ***********************************************************************/
static 
size_t check_io_circuit_model(const CircuitLibrary& circuit_lib) {
  size_t num_err = 0;

  /* Embedded I/O interface may not have inout port 
   * iopad_port_types_required.push_back(CIRCUIT_MODEL_PORT_INOUT);
   * Some I/Os may not have SRAM port, such as AIB interface
   * iopad_port_types_required.push_back(CIRCUIT_MODEL_PORT_SRAM);
   */
  std::vector<enum e_circuit_model_port_type> iopad_port_types_required;
  iopad_port_types_required.push_back(CIRCUIT_MODEL_PORT_INOUT);
  num_err += check_circuit_model_port_required(circuit_lib, CIRCUIT_MODEL_IOPAD, iopad_port_types_required);

  /* Each I/O cell must have 
   *  - One of the following ports
   *    - At least 1 ASIC-to-FPGA (A2F) port that is defined as global I/O 
   *    - At least 1 FPGA-to-ASIC (F2A) port that is defined as global I/O!
   *  - At least 1 regular port that is non-global which is connected to global routing architecture
   */
  for (const auto& io_model : circuit_lib.models_by_type(CIRCUIT_MODEL_IOPAD)) {
    bool has_global_io = false;
    bool has_internal_connection = false;

    for (const auto& port : circuit_lib.model_ports(io_model)) {
      if ( (true == circuit_lib.port_is_io(port)
        && (true == circuit_lib.port_is_global(port)))) {
        has_global_io = true;
        continue; /* Go to next */
      }
      if ( (false == circuit_lib.port_is_io(port)
        && (false == circuit_lib.port_is_global(port)))
        && (CIRCUIT_MODEL_PORT_SRAM != circuit_lib.port_type(port))) {
        has_internal_connection = true;
        continue; /* Go to next */
      }
    }
  
    if (false == has_global_io) {
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "I/O circuit model '%s' does not have any I/O port defined!\n",
                     circuit_lib.model_name(io_model).c_str()); 
      num_err++;
    }

    if (false == has_internal_connection) {
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "I/O circuit model '%s' does not have any port connected to FPGA core!\n",
                     circuit_lib.model_name(io_model).c_str()); 
      num_err++;
    }
  }

  return num_err;
}

/************************************************************************
 * Check points to make sure we have a valid circuit library
 * Detailed checkpoints: 
 * 1. Circuit models have unique names 
 * 2. Circuit models have unique prefix
 * 3. Check IOPADs have input and output ports
 * 4. Check MUXes has been defined and has input and output ports
 * 5. We must have at least one SRAM or CCFF 
 * 6. SRAM must have at least an input and an output ports
 * 7. CCFF must have at least a clock, an input and an output ports
 * 8. FF must have at least a clock, an input and an output ports
 * 9. LUT must have at least an input, an output and a SRAM ports
 * 10. We must have default circuit models for these types: MUX, channel wires and wires
 *
 * Note:
 *   - NO modification on the circuit library is allowed!
 *     The circuit library should be read-only!!!
 ***********************************************************************/
bool check_circuit_library(const CircuitLibrary& circuit_lib) {
  size_t num_err = 0;

  vtr::ScopedStartFinishTimer timer("Check circuit library");

  /* 1. Circuit models have unique names  
   * For each circuit model, we always make sure it does not share any name with any circuit model locating after it
   */
  num_err += check_circuit_library_unique_names(circuit_lib);

  /* 2. Circuit models have unique prefix
   * For each circuit model, we always make sure it does not share any prefix with any circuit model locating after it
   */
  num_err += check_circuit_library_unique_prefix(circuit_lib);

  /* Check global ports */
  num_err += check_circuit_library_ports(circuit_lib);

  /* 3. Check io has been defined and has input and output ports 
   * [a] We must have global I/O port, either its type is inout, input or output
   * [b] For each IOPAD, we must have at least an input an output 
   */
  num_err += check_circuit_model_required(circuit_lib, CIRCUIT_MODEL_IOPAD);
  num_err += check_io_circuit_model(circuit_lib);

  /* 4. Check mux has been defined and has input and output ports
   * [a] We must have a MUX! 
   * [b] For each MUX, we must have at least an input, an output, and an SRAM port
   */
  num_err += check_circuit_model_required(circuit_lib, CIRCUIT_MODEL_MUX);

  std::vector<enum e_circuit_model_port_type> mux_port_types_required;
  mux_port_types_required.push_back(CIRCUIT_MODEL_PORT_INPUT);
  mux_port_types_required.push_back(CIRCUIT_MODEL_PORT_OUTPUT);
  mux_port_types_required.push_back(CIRCUIT_MODEL_PORT_SRAM);

  num_err += check_circuit_model_port_required(circuit_lib, CIRCUIT_MODEL_MUX, mux_port_types_required);

  /* 5. We must have at least one SRAM or CCFF */
  if ( ( 0 == circuit_lib.models_by_type(CIRCUIT_MODEL_SRAM).size())
    && ( 0 == circuit_lib.models_by_type(CIRCUIT_MODEL_CCFF).size()) ) {
    VTR_LOG_ERROR("At least one %s or %s circuit model is required!\n",
                  CIRCUIT_MODEL_TYPE_STRING[size_t(CIRCUIT_MODEL_SRAM)], 
                  CIRCUIT_MODEL_TYPE_STRING[size_t(CIRCUIT_MODEL_CCFF)]);
    /* Incremental the counter for errors */
    num_err++;
  }

  /* 6. SRAM must have at least an input and an output ports*/
  std::vector<enum e_circuit_model_port_type> sram_port_types_required;
  sram_port_types_required.push_back(CIRCUIT_MODEL_PORT_OUTPUT);

  num_err += check_circuit_model_port_required(circuit_lib, CIRCUIT_MODEL_SRAM, sram_port_types_required);

  /* 7. CCFF must have at least a clock, an input and an output ports*/
  std::vector<enum e_circuit_model_port_type> ccff_port_types_required;
  ccff_port_types_required.push_back(CIRCUIT_MODEL_PORT_CLOCK);
  ccff_port_types_required.push_back(CIRCUIT_MODEL_PORT_INPUT);
  ccff_port_types_required.push_back(CIRCUIT_MODEL_PORT_OUTPUT);

  num_err += check_circuit_model_port_required(circuit_lib, CIRCUIT_MODEL_CCFF, ccff_port_types_required);

  /* 8. FF must have at least a clock, an input and an output ports*/
  std::vector<enum e_circuit_model_port_type> ff_port_types_required;
  ff_port_types_required.push_back(CIRCUIT_MODEL_PORT_CLOCK);
  ff_port_types_required.push_back(CIRCUIT_MODEL_PORT_INPUT);
  ff_port_types_required.push_back(CIRCUIT_MODEL_PORT_OUTPUT);

  num_err += check_circuit_model_port_required(circuit_lib, CIRCUIT_MODEL_FF, ff_port_types_required);

  /* 9. LUT must have at least an input, an output and a SRAM ports*/
  std::vector<enum e_circuit_model_port_type> lut_port_types_required;
  lut_port_types_required.push_back(CIRCUIT_MODEL_PORT_SRAM);
  lut_port_types_required.push_back(CIRCUIT_MODEL_PORT_INPUT);
  lut_port_types_required.push_back(CIRCUIT_MODEL_PORT_OUTPUT);

  num_err += check_circuit_model_port_required(circuit_lib, CIRCUIT_MODEL_LUT, lut_port_types_required);

  /* 10. For each type of circuit models that are define, we must have 1 default model
   *     We must have default circuit models for these types: MUX, channel wires and wires 
   */
  num_err += check_default_circuit_model_by_types(circuit_lib);
  num_err += check_required_default_circuit_model(circuit_lib, CIRCUIT_MODEL_MUX);
  num_err += check_required_default_circuit_model(circuit_lib, CIRCUIT_MODEL_CHAN_WIRE);
  num_err += check_required_default_circuit_model(circuit_lib, CIRCUIT_MODEL_WIRE);

  /* 11. Check power-gated inverter/buffer models */
  num_err += check_power_gated_circuit_models(circuit_lib);

  /* If we have any errors, exit */

  if (0 < num_err) {
    VTR_LOG("Finished checking circuit library with %d errors!\n",
            num_err);
    return false;
  }

  VTR_LOG("Checking circuit library passed.\n");

  return true;
}

