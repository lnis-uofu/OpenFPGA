/**********************************************************
 * MIT License
 *
 * Copyright (c) 2018 LNIS - The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ***********************************************************************/

/************************************************************************
 * Filename:    circuit_library_utils.cpp
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/09/27  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/

/************************************************************************
 * Function to perform fundamental operation for the circuit library 
 * These functions are not universal methods for the CircuitLibrary class
 * They are made to ease the development in some specific purposes
 * Please classify such functions in this file
 ***********************************************************************/

/* Header files should be included in a sequence */
/* Standard header files required go first */
#include <algorithm>

#include "vtr_assert.h"

#include "util.h"

#include "circuit_library_utils.h"

/********************************************************************
 * Get the model id of a SRAM model that is used to configure 
 * a circuit model
 *******************************************************************/
std::vector<CircuitModelId> find_circuit_sram_models(const CircuitLibrary& circuit_lib,
                                                     const CircuitModelId& circuit_model) {
  /* SRAM model id is stored in the sram ports of a circuit model */
  std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_SRAM);
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
  std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_SRAM, true);
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
  std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_SRAM, true);
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
                                                const e_sram_orgz& sram_orgz_type) {
  size_t num_shared_config_bits = 0;

  /* Branch on the organization of configuration protocol */ 
  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    break;
  case SPICE_SRAM_MEMORY_BANK: {
    /* Find BL/WL ports */
    std::vector<CircuitPortId> blb_ports = circuit_lib.model_ports_by_type(rram_model, SPICE_MODEL_PORT_BLB);
    for (auto blb_port : blb_ports) {
      num_shared_config_bits = std::max((int)num_shared_config_bits, (int)circuit_lib.port_size(blb_port) - 1);
    }
    break;
  }    
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d]) Invalid type of SRAM organization!\n",
               __FILE__, __LINE__);
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
                                           const e_sram_orgz& sram_orgz_type) {
  size_t num_shared_config_bits = 0;

  std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_SRAM);
  for (auto sram_port : sram_ports) {
    CircuitModelId sram_model = circuit_lib.port_tri_state_model(sram_port);
    VTR_ASSERT( true == circuit_lib.valid_model_id(sram_model) );

    /* Depend on the design technolgy of SRAM model, the number of configuration bits will be different */
    switch (circuit_lib.design_tech_type(sram_model)) {
    case SPICE_MODEL_DESIGN_CMOS: 
      /* CMOS circuit do not need shared configuration bits */
      break;
    case SPICE_MODEL_DESIGN_RRAM: 
       /* RRAM circuit do need shared configuration bits, but it is subjected to the largest one among different SRAM models */
       num_shared_config_bits = std::max((int)num_shared_config_bits, (int)find_rram_circuit_num_shared_config_bits(circuit_lib, sram_model, sram_orgz_type));
       break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,
                 "(File:%s,[LINE%d]) Invalid design technology for SRAM model!\n",
                 __FILE__, __LINE__);
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
size_t find_circuit_num_config_bits(const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model) {
  size_t num_config_bits = 0;

  std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_SRAM);
  for (auto sram_port : sram_ports) {
    num_config_bits += circuit_lib.port_size(sram_port); 
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
      if (0 == circuit_lib.port_lib_name(port).compare(circuit_lib.port_lib_name(global_port))) {
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
