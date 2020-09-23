/************************************************************************
 * Function to perform fundamental operation for the circuit library 
 * These functions are not universal methods for the CircuitLibrary class
 * They are made to ease the development in some specific purposes
 * Please classify such functions in this file
 ***********************************************************************/

#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "check_circuit_library.h"
#include "decoder_library_utils.h"
#include "circuit_library_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Get the model id of a SRAM model that is used to configure 
 * a circuit model
 *******************************************************************/
std::vector<CircuitModelId> find_circuit_sram_models(const CircuitLibrary& circuit_lib,
                                                     const CircuitModelId& circuit_model) {
  /* SRAM model id is stored in the sram ports of a circuit model */
  std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_SRAM);
  std::vector<CircuitModelId> sram_models;
  
  /* Create a list of sram models, but avoid duplicated model ids */
  for (const auto& sram_port : sram_ports) {
    CircuitModelId sram_model = circuit_lib.port_tri_state_model(sram_port);
    VTR_ASSERT( true == circuit_lib.valid_model_id(sram_model) );
    if (sram_models.end() != std::find(sram_models.begin(), sram_models.end(), sram_model)) {
      continue;  /* Already in the list, skip the addition */
    }
    /* Not in the list, add it */
    sram_models.push_back(sram_model);
  }
  
  return sram_models;
}

/********************************************************************
 * Find regular (not mode select) sram ports of a circuit model
 *******************************************************************/
std::vector<CircuitPortId> find_circuit_regular_sram_ports(const CircuitLibrary& circuit_lib,
                                                           const CircuitModelId& circuit_model) {
  std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_SRAM, true);
  std::vector<CircuitPortId> regular_sram_ports;

  for (const auto& port : sram_ports) {
    if (true == circuit_lib.port_is_mode_select(port)) {
      continue;
    }
    regular_sram_ports.push_back(port);
  }

  return regular_sram_ports;
}

/********************************************************************
 * Find mode select sram ports of a circuit model
 *******************************************************************/
std::vector<CircuitPortId> find_circuit_mode_select_sram_ports(const CircuitLibrary& circuit_lib,
                                                               const CircuitModelId& circuit_model) {
  std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_SRAM, true);
  std::vector<CircuitPortId> mode_select_sram_ports;

  for (const auto& port : sram_ports) {
    if (false == circuit_lib.port_is_mode_select(port)) {
      continue;
    }
    mode_select_sram_ports.push_back(port);
  }

  return mode_select_sram_ports;
}


/********************************************************************
 * Find the number of shared configuration bits for a ReRAM circuit 
 * TODO: this function is subjected to be changed due to ReRAM-based SRAM cell design!!!
 *******************************************************************/
static 
size_t find_rram_circuit_num_shared_config_bits(const CircuitLibrary& circuit_lib,
                                                const CircuitModelId& rram_model,
                                                const e_config_protocol_type& sram_orgz_type) {
  size_t num_shared_config_bits = 0;

  /* Branch on the organization of configuration protocol */ 
  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE:
  case CONFIG_MEM_SCAN_CHAIN:
    break;
  case CONFIG_MEM_MEMORY_BANK: {
    /* Find BL/WL ports */
    std::vector<CircuitPortId> blb_ports = circuit_lib.model_ports_by_type(rram_model, CIRCUIT_MODEL_PORT_BLB);
    for (auto blb_port : blb_ports) {
      num_shared_config_bits = std::max((int)num_shared_config_bits, (int)circuit_lib.port_size(blb_port) - 1);
    }
    break;
  }    
  default:
    VTR_LOG_ERROR("Invalid type of SRAM organization!\n");
    exit(1);
  }

  return num_shared_config_bits;
}

/********************************************************************
 * A generic function to find the number of shared configuration bits 
 * for circuit model
 * It will return 0 for CMOS circuits 
 * It will return the maximum shared configuration bits across ReRAM models
 *
 * Note: This function may give WRONG results when all the SRAM ports
 * are not properly linked to its circuit models!
 * So, it should be called after the SRAM linking is done!!!
 *
 * IMPORTANT: This function should NOT be used to find the number of shared configuration bits
 * for a multiplexer, because the multiplexer size is determined during 
 * the FPGA architecture generation (NOT during the XML parsing). 
 *******************************************************************/
size_t find_circuit_num_shared_config_bits(const CircuitLibrary& circuit_lib,
                                           const CircuitModelId& circuit_model,
                                           const e_config_protocol_type& sram_orgz_type) {
  size_t num_shared_config_bits = 0;

  std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_SRAM);
  for (auto sram_port : sram_ports) {
    CircuitModelId sram_model = circuit_lib.port_tri_state_model(sram_port);
    VTR_ASSERT( true == circuit_lib.valid_model_id(sram_model) );

    /* Depend on the design technolgy of SRAM model, the number of configuration bits will be different */
    switch (circuit_lib.design_tech_type(sram_model)) {
    case CIRCUIT_MODEL_DESIGN_CMOS: 
      /* CMOS circuit do not need shared configuration bits */
      break;
    case CIRCUIT_MODEL_DESIGN_RRAM: 
       /* RRAM circuit do need shared configuration bits, but it is subjected to the largest one among different SRAM models */
       num_shared_config_bits = std::max((int)num_shared_config_bits, (int)find_rram_circuit_num_shared_config_bits(circuit_lib, sram_model, sram_orgz_type));
       break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid design technology for SRAM model!\n");
      exit(1);
    }
  } 

  return num_shared_config_bits;
}

/********************************************************************
 * A generic function to find the number of configuration bits 
 * for circuit model
 * It will sum up the sizes of all the sram ports 
 * 
 * IMPORTANT: This function should NOT be used to find the number of configuration bits
 * for a multiplexer, because the multiplexer size is determined during 
 * the FPGA architecture generation (NOT during the XML parsing). 
 *******************************************************************/
size_t find_circuit_num_config_bits(const e_config_protocol_type& config_protocol_type,
                                    const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model) {
  size_t num_config_bits = 0;

  std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_SRAM);
  for (auto sram_port : sram_ports) {
    num_config_bits += circuit_lib.port_size(sram_port); 
  } 

  switch (config_protocol_type) {
  case CONFIG_MEM_STANDALONE: 
  case CONFIG_MEM_SCAN_CHAIN: 
  case CONFIG_MEM_MEMORY_BANK: {
    break;
  }
  case CONFIG_MEM_FRAME_BASED: {
    /* For frame-based configuration protocol
     * The number of configuration bits is the address size
     */
    if (0 < num_config_bits) {
      num_config_bits = find_mux_local_decoder_addr_size(num_config_bits);
    }
    break;
  }
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of SRAM organization !\n");
    exit(1);
  }

  return num_config_bits;
}

/********************************************************************
 * A generic function to find all the global ports in a circuit library
 * 
 * IMPORTANT: This function will uniquify the global ports whose share 
 * share the same name !!!
 *******************************************************************/
std::vector<CircuitPortId> find_circuit_library_global_ports(const CircuitLibrary& circuit_lib) {
  std::vector<CircuitPortId> global_ports;

  for (auto port : circuit_lib.ports()) {
    /* By pass non-global ports*/
    if (false == circuit_lib.port_is_global(port)) {
      continue;
    }
    /* Check if a same port with the same name has already been in the list */
    bool add_to_list = true;
    for (const auto& global_port : global_ports) {
      if (0 == circuit_lib.port_prefix(port).compare(circuit_lib.port_prefix(global_port))) {
        /* Same name, skip list update */
        add_to_list = false;
        break;
      }
    }
    if (true == add_to_list) {
      /* Add the global_port to the list */
      global_ports.push_back(port);
    }
  } 

  return global_ports;
}

/********************************************************************
 * A generic function to find all the unique user-defined
 * Verilog netlists in a circuit library
 * Netlists with same names will be considered as one
 *******************************************************************/
std::vector<std::string> find_circuit_library_unique_verilog_netlists(const CircuitLibrary& circuit_lib) {
  std::vector<std::string> netlists;

  for (const CircuitModelId& model : circuit_lib.models()) {
    /* Skip empty netlist names */
    if (true == circuit_lib.model_verilog_netlist(model).empty()) {
      continue;
    }
    /* See if the netlist name is already in the list */
    std::vector<std::string>::iterator it = std::find(netlists.begin(), netlists.end(), circuit_lib.model_verilog_netlist(model));
    if (it == netlists.end()) {
      netlists.push_back(circuit_lib.model_verilog_netlist(model));
    }
  }

 return netlists;
}

/********************************************************************
 * A generic function to find all the unique user-defined
 * Verilog netlists in a circuit library
 * Netlists with same names will be considered as one
 *******************************************************************/
std::vector<std::string> find_circuit_library_unique_spice_netlists(const CircuitLibrary& circuit_lib) {
  std::vector<std::string> netlists;

  for (const CircuitModelId& model : circuit_lib.models()) {
    /* Skip empty netlist names */
    if (true == circuit_lib.model_spice_netlist(model).empty()) {
      continue;
    }
    /* See if the netlist name is already in the list */
    std::vector<std::string>::iterator it = std::find(netlists.begin(), netlists.end(), circuit_lib.model_spice_netlist(model));
    if (it == netlists.end()) {
      netlists.push_back(circuit_lib.model_spice_netlist(model));
    }
  }

 return netlists;
}

/************************************************************************
 * Advanced check if the circuit model of configurable memory
 * satisfy the needs of configuration protocol
 * - Configuration chain -based: we check if we have a CCFF model
 * - Frame -based: we check if we have a SRAM model which has BL and WL
 * 
 ***********************************************************************/
bool check_configurable_memory_circuit_model(const e_config_protocol_type& config_protocol_type,
                                             const CircuitLibrary& circuit_lib,
                                             const CircuitModelId& config_mem_circuit_model) {
  size_t num_err = 0;

  switch (config_protocol_type) {
  case CONFIG_MEM_SCAN_CHAIN:   
    num_err = check_ccff_circuit_model_ports(circuit_lib,
                                             config_mem_circuit_model);
    break;
  case CONFIG_MEM_STANDALONE: 
  case CONFIG_MEM_MEMORY_BANK:  
  case CONFIG_MEM_FRAME_BASED:  
    num_err = check_sram_circuit_model_ports(circuit_lib,
                                             config_mem_circuit_model,
                                             true);
    
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of configuration protocol!\n");
    return false;
  }
  
  VTR_LOG("Found %ld errors when checking configurable memory circuit models!\n",
          num_err);
  return (0 == num_err);
}

/************************************************************************
 * Try to find the enable port control power-gate for a power-gated circuit model
 * We will return the first port that meet the requirement:
 * - a global port
 * - its function is labelled as config_enable
 * - default value is 0
 * Return invalid id if not found
 ***********************************************************************/
CircuitPortId find_circuit_model_power_gate_en_port(const CircuitLibrary& circuit_lib,
                                                    const CircuitModelId& circuit_model) {
  VTR_ASSERT(true == circuit_lib.is_power_gated(circuit_model));
  std::vector<CircuitPortId> global_ports = circuit_lib.model_global_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true, true);

  /* Try to find an ENABLE port from the global ports */
  CircuitPortId en_port = CircuitPortId::INVALID();
  for (const auto& port : global_ports) {
    /* Focus on config_enable ports which are power-gate control signals */
    if (false == circuit_lib.port_is_config_enable(port)) {
      continue;
    }
    if (1 == circuit_lib.port_default_value(port)) {
      en_port = port;
      break;
    }
  }

  return en_port;
}

/************************************************************************
 * Try to find the enableB port control power-gate for a power-gated circuit model
 * We will return the first port that meet the requirement:
 * - a global port
 * - its function is labelled as config_enable
 * - default value is 1
 * Return invalid id if not found
 ***********************************************************************/
CircuitPortId find_circuit_model_power_gate_enb_port(const CircuitLibrary& circuit_lib,
                                                     const CircuitModelId& circuit_model) {
  CircuitPortId enb_port = CircuitPortId::INVALID();
  VTR_ASSERT(true == circuit_lib.is_power_gated(circuit_model));
  std::vector<CircuitPortId> global_ports = circuit_lib.model_global_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true, true);

  /* Try to find an ENABLE_B port from the global ports */
  for (const auto& port : global_ports) {
    /* Focus on config_enable ports which are power-gate control signals */
    if (false == circuit_lib.port_is_config_enable(port)) {
      continue;
    }
    if (0 == circuit_lib.port_default_value(port)) {
      enb_port = port;
      break;
    }
  }

  return enb_port;
}

} /* end namespace openfpga */
