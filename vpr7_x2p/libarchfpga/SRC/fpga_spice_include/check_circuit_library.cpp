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
 * Filename:    check_circuit_library.cpp
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/08/12  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/

/* Header files should be included in a sequence */
/* Standard header files required go first */
#include "util.h"
#include "check_circuit_library.h"


/* 1. Circuit models have unique names, return the number of errors */
static  
size_t check_circuit_library_unique_names(const CircuitLibrary& circuit_lib) {
  size_t num_err = 0;

  for (size_t i = 0; i < circuit_lib.num_circuit_models(); ++i) {
    /* Skip for the last element, because the inner loop will access it */
    if (i == circuit_lib.num_circuit_models() - 1) {
      continue;
    }
    /* Get the name of reference */
    const std::string& i_name = circuit_lib.circuit_model_name(CircuitModelId(i));
    for (size_t j = i + 1; j < circuit_lib.num_circuit_models(); ++j) {
      /* Compare the name of candidate */
      const std::string& j_name = circuit_lib.circuit_model_name(CircuitModelId(j));
      /* Compare the name and skip for different names */
      if (0 != i_name.compare(j_name)) {
        continue;
      }
      vpr_printf(TIO_MESSAGE_ERROR,
                 "Circuit model(index=%d) and (index=%d) share the same name, which is invalid!\n",
                 i , j, i_name.c_str());
      /* Incremental the counter for errors */
      num_err++;
    }
  }

  return num_err;
}


/* 1. Circuit models have unique names, return the number of errors */
static 
size_t check_circuit_library_unique_prefix(const CircuitLibrary& circuit_lib) {
  size_t num_err = 0;

  for (size_t i = 0; i < circuit_lib.num_circuit_models(); ++i) {
    /* Skip for the last element, because the inner loop will access it */
    if (i == circuit_lib.num_circuit_models() - 1) {
      continue;
    }
    /* Get the name of reference */
    const std::string& i_prefix = circuit_lib.circuit_model_prefix(CircuitModelId(i));
    for (size_t j = i + 1; j < circuit_lib.num_circuit_models(); ++j) {
      /* Compare the name of candidate */
      const std::string& j_prefix = circuit_lib.circuit_model_prefix(CircuitModelId(j));
      /* Compare the name and skip for different prefix */
      if (0 != i_prefix.compare(j_prefix)) {
        continue;
      }
      vpr_printf(TIO_MESSAGE_ERROR,
                 "Circuit model(name=%s) and (name=%s) share the same prefix, which is invalid!\n",
                 circuit_lib.circuit_model_name(CircuitModelId(i)).c_str(),
                 circuit_lib.circuit_model_name(CircuitModelId(j)).c_str(),
                 i_prefix.c_str());
      /* Incremental the counter for errors */
      num_err++;
    }
  }

  return num_err;
}

/* A generic function to check the port list of a circuit model in a given type */
static 
size_t check_circuit_model_required(const CircuitLibrary& circuit_lib,
                                    const enum e_spice_model_type& circuit_model_type_to_check) {
  size_t num_err = 0;

  /* We must have an IOPAD*/
  if ( 0 == circuit_lib.circuit_models_by_type(circuit_model_type_to_check).size()) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "At least one %s circuit model is required!\n",
               CIRCUIT_MODEL_TYPE_STRING[size_t(circuit_model_type_to_check)]);
    /* Incremental the counter for errors */
    num_err++;
  }

  return num_err;
}

/* A generic function to check the port list of a circuit model in a given type */
static 
size_t check_circuit_model_port_required(const CircuitLibrary& circuit_lib,
                                         const enum e_spice_model_type& circuit_model_type_to_check, 
                                         const std::vector<enum e_spice_model_port_type>& port_types_to_check) {
  size_t num_err = 0;

  for (const auto& id : circuit_lib.circuit_models_by_type(circuit_model_type_to_check)) {
    for (const auto& port_type: port_types_to_check) {
      if (0 == circuit_lib.ports_by_type(id, port_type).size()) {
        vpr_printf(TIO_MESSAGE_ERROR,
                   "%s circuit model(name=%s) does not have %s port\n",
                   CIRCUIT_MODEL_TYPE_STRING[size_t(circuit_model_type_to_check)], 
                   circuit_lib.circuit_model_name(id).c_str(), 
                   CIRCUIT_MODEL_PORT_TYPE_STRING[size_t(port_type)]);
        /* Incremental the counter for errors */
        num_err++;
      }
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
 ***********************************************************************/
void check_circuit_library(const CircuitLibrary& circuit_lib) {
  size_t num_err = 0;

  vpr_printf(TIO_MESSAGE_INFO, "Checking circuit models...\n");

  /* 1. Circuit models have unique names  
   * For each circuit model, we always make sure it does not share any name with any circuit model locating after it
   */
  num_err += check_circuit_library_unique_names(circuit_lib);

  /* 2. Circuit models have unique prefix
   * For each circuit model, we always make sure it does not share any prefix with any circuit model locating after it
   */
  num_err += check_circuit_library_unique_prefix(circuit_lib);

  /* 3. Check io has been defined and has input and output ports 
   * [a] We must have an IOPAD! 
   * [b] For each IOPAD, we must have at least an input, an output, an INOUT and an SRAM port
   */
  num_err += check_circuit_model_required(circuit_lib, SPICE_MODEL_IOPAD);

  std::vector<enum e_spice_model_port_type> iopad_port_types_required;
  iopad_port_types_required.push_back(SPICE_MODEL_PORT_INPUT);
  iopad_port_types_required.push_back(SPICE_MODEL_PORT_OUTPUT);
  iopad_port_types_required.push_back(SPICE_MODEL_PORT_INOUT);
  iopad_port_types_required.push_back(SPICE_MODEL_PORT_SRAM);

  num_err += check_circuit_model_port_required(circuit_lib, SPICE_MODEL_IOPAD, iopad_port_types_required);

  /* 4. Check mux has been defined and has input and output ports
   * [a] We must have a MUX! 
   * [b] For each MUX, we must have at least an input, an output, and an SRAM port
   */
  num_err += check_circuit_model_required(circuit_lib, SPICE_MODEL_MUX);

  std::vector<enum e_spice_model_port_type> mux_port_types_required;
  mux_port_types_required.push_back(SPICE_MODEL_PORT_INPUT);
  mux_port_types_required.push_back(SPICE_MODEL_PORT_OUTPUT);
  mux_port_types_required.push_back(SPICE_MODEL_PORT_SRAM);

  num_err += check_circuit_model_port_required(circuit_lib, SPICE_MODEL_MUX, mux_port_types_required);

  /* 5. We must have at least one SRAM or SCFF */
  if ( ( 0 == circuit_lib.circuit_models_by_type(SPICE_MODEL_SRAM).size())
    && ( 0 == circuit_lib.circuit_models_by_type(SPICE_MODEL_SCFF).size()) ) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "At least one %s or %s circuit model is required!\n",
               CIRCUIT_MODEL_TYPE_STRING[size_t(SPICE_MODEL_SRAM)], 
               CIRCUIT_MODEL_TYPE_STRING[size_t(SPICE_MODEL_SCFF)]);
    /* Incremental the counter for errors */
    num_err++;
  }

  /* 6. SRAM must have at least an input and an output ports*/
  std::vector<enum e_spice_model_port_type> sram_port_types_required;
  sram_port_types_required.push_back(SPICE_MODEL_PORT_INPUT);
  sram_port_types_required.push_back(SPICE_MODEL_PORT_OUTPUT);

  num_err += check_circuit_model_port_required(circuit_lib, SPICE_MODEL_SRAM, sram_port_types_required);

  /* 7. SCFF must have at least an input and an output ports*/
  std::vector<enum e_spice_model_port_type> scff_port_types_required;
  scff_port_types_required.push_back(SPICE_MODEL_PORT_CLOCK);
  scff_port_types_required.push_back(SPICE_MODEL_PORT_INPUT);
  scff_port_types_required.push_back(SPICE_MODEL_PORT_OUTPUT);

  num_err += check_circuit_model_port_required(circuit_lib, SPICE_MODEL_SCFF, scff_port_types_required);

  /* 8. FF must have at least an input and an output ports*/
  std::vector<enum e_spice_model_port_type> ff_port_types_required;
  ff_port_types_required.push_back(SPICE_MODEL_PORT_CLOCK);
  ff_port_types_required.push_back(SPICE_MODEL_PORT_INPUT);
  ff_port_types_required.push_back(SPICE_MODEL_PORT_OUTPUT);

  num_err += check_circuit_model_port_required(circuit_lib, SPICE_MODEL_FF, ff_port_types_required);

  /* 9. LUY must have at least an input, an output and a SRAM ports*/
  std::vector<enum e_spice_model_port_type> lut_port_types_required;
  lut_port_types_required.push_back(SPICE_MODEL_PORT_SRAM);
  lut_port_types_required.push_back(SPICE_MODEL_PORT_INPUT);
  lut_port_types_required.push_back(SPICE_MODEL_PORT_OUTPUT);

  num_err += check_circuit_model_port_required(circuit_lib, SPICE_MODEL_LUT, lut_port_types_required);

  /* If we have any errors, exit */
  vpr_printf(TIO_MESSAGE_ERROR,
             "Finished checking circuit library with %d errors!\n",
             num_err);

  if (0 < num_err) {
    exit(1);
  }

  return;
}

/************************************************************************
 * End of file : check_circuit_library.cpp
 ***********************************************************************/

