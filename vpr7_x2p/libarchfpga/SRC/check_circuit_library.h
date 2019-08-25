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
 * Filename:    check_circuit_library.h
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/08/12  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/

/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */
#ifndef CHECK_CIRCUIT_LIBRARY_H
#define CHECK_CIRCUIT_LIBRARY_H

/*
 * Notes in include header files in a head file 
 * Only include the neccessary header files 
 * that is required by the data types in the function/class declarations!
 */
/* Header files should be included in a sequence */
/* Standard header files required go first */
#include <vector>
#include "spice_types.h"
#include "circuit_library_fwd.h"

/* Check points to make sure we have a valid circuit library */
size_t check_one_circuit_model_port_required(const CircuitLibrary& circuit_lib,
                                             const CircuitModelId& circuit_model, 
                                             const std::vector<enum e_spice_model_port_type>& port_types_to_check);

size_t check_one_circuit_model_port_size_required(const CircuitLibrary& circuit_lib,
                                                  const CircuitModelId& circuit_model, 
                                                  const CircuitPortId& circuit_port,
                                                  const size_t& port_size_to_check);

size_t check_one_circuit_model_port_type_and_size_required(const CircuitLibrary& circuit_lib,
                                                           const CircuitModelId& circuit_model, 
                                                           const enum e_spice_model_port_type& port_type_to_check,
                                                           const size_t& num_ports_to_check,
                                                           const size_t& port_size_to_check,
                                                           const bool& include_global_ports);

size_t check_ff_circuit_model_ports(const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model);

size_t check_scff_circuit_model_ports(const CircuitLibrary& circuit_lib,
                                      const CircuitModelId& circuit_model);

size_t check_sram_circuit_model_ports(const CircuitLibrary& circuit_lib,
                                      const CircuitModelId& circuit_model,
                                      const bool& check_blwl);

void check_circuit_library(const CircuitLibrary& circuit_lib);

#endif

/************************************************************************
 * End of file : check_circuit_library.h
 ***********************************************************************/

