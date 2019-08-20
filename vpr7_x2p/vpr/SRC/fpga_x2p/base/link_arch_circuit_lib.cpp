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
 * Filename:    link_arch_circuit_lib.cpp
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/08/12  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/
/************************************************************************
 *  This file includes key functions to link circuit models to the architecture modules
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>

/* Include vpr structs*/
#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph.h"
#include "vpr_utils.h"
#include "path_delay.h"
#include "stats.h"
#include "route_common.h"

/* Include vtr libraries */
#include "vtr_assert.h"

/* Include spice support headers*/
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_globals.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_timing_utils.h"
#include "fpga_x2p_backannotate_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "verilog_api.h"

#include "check_circuit_library.h"
#include "link_arch_circuit_lib.h"

/************************************************************************
 * Find a circuit model with a given name
 * Case 1: if the circuit_model_name is not defined, 
 *         we find a default circuit model and check its type
 * Case 2: if the circuit_model_name is defined,
 *         we find a matched circuit model and check its type 
 ***********************************************************************/
CircuitModelId link_circuit_model_by_name_and_type(const char* circuit_model_name,
                                                   const CircuitLibrary& circuit_lib,
                                                   const enum e_spice_model_type& model_type) {
  CircuitModelId circuit_model = CircuitModelId::INVALID();
  /* If the circuit_model_name is not defined, we use the default*/
  if (NULL == circuit_model_name) {
    circuit_model = circuit_lib.default_model(model_type);
  } else {
    circuit_model = circuit_lib.model(circuit_model_name);
  }

  /* Check the circuit model, we should have one! */
  if (CircuitModelId::INVALID() == circuit_model) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,LINE[%d]) Fail to find a defined circuit model called %s!\n",
               __FILE__, __LINE__, 
               circuit_lib.model_name(circuit_model).c_str());
    return circuit_model; /* Return here, no need to check the model_type */
  } 

  /* Check the type of circuit model, make sure it is the one we want */
  if (model_type != circuit_lib.model_type(circuit_model)) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,LINE[%d]) Invalid type when trying to find circuit model called %s! Expect %s but found %s!\n",
               __FILE__, __LINE__, 
               circuit_model_name,
               CIRCUIT_MODEL_TYPE_STRING[size_t(model_type)],
               CIRCUIT_MODEL_TYPE_STRING[size_t(circuit_lib.model_type(circuit_model))]);
  }

  return circuit_model;
}

/************************************************************************
 * Link circuit model to the SRAM organization
 * Case 1: standalone organization required a SRAM circuit model
 * Case 1: scan-chain organization required a SCFF circuit model
 * Case 1: memory-bank organization required a SRAM circuit model
 ***********************************************************************/
static 
void link_one_sram_inf_orgz(t_sram_inf_orgz* cur_sram_inf_orgz,
                            const CircuitLibrary& circuit_lib) {
  /* If cur_sram_inf_orgz is not initialized, do nothing */
  if (NULL == cur_sram_inf_orgz) {
    return;
  } 

  /* Check the type of SRAM_Ciruit_MODEL required by different sram organization */
  /* check SRAM ports 
   * Checker for circuit models used by the SRAM organization
   * either SRAMs or SCFFs
   * 1. It will check the basic port required for SRAMs and SCFFs
   * 2. It will check any special ports required for SRAMs and SCFFs
   */
  switch (cur_sram_inf_orgz->type) {
  case SPICE_SRAM_STANDALONE:
    cur_sram_inf_orgz->circuit_model = link_circuit_model_by_name_and_type(cur_sram_inf_orgz->spice_model_name, circuit_lib, SPICE_MODEL_SRAM);
    /* check SRAM ports */
    check_sram_circuit_model_ports(circuit_lib, cur_sram_inf_orgz->circuit_model, false);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    cur_sram_inf_orgz->circuit_model = link_circuit_model_by_name_and_type(cur_sram_inf_orgz->spice_model_name, circuit_lib, SPICE_MODEL_SRAM);
    /* check if this one has bit lines and word lines */
    check_sram_circuit_model_ports(circuit_lib, cur_sram_inf_orgz->circuit_model, true);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* check Scan-chain Flip-flop ports */
    cur_sram_inf_orgz->circuit_model = link_circuit_model_by_name_and_type(cur_sram_inf_orgz->spice_model_name, circuit_lib, SPICE_MODEL_SCFF);
    check_scff_circuit_model_ports(circuit_lib, cur_sram_inf_orgz->circuit_model);
    break;
  case SPICE_SRAM_LOCAL_ENCODER:
    /* Wipe out LOCAL ENCODER, it is not supported here ! */
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s,LINE[%d]) Local encoder SRAM organization is not supported!\n",
               __FILE__, __LINE__);
    exit(1);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s,LINE[%d]) Invalid SRAM organization!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* RRAM Scan-chain is not supported yet. Now just forbidden this option */
  if ( (SPICE_SRAM_SCAN_CHAIN == cur_sram_inf_orgz->type) 
    && (SPICE_MODEL_DESIGN_RRAM == circuit_lib.design_tech_type(cur_sram_inf_orgz->circuit_model)) ) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File:%s,LINE[%d]) RRAM-based Scan-chain Flip-flop has not been supported yet!\n",
                 __FILE__, __LINE__);
      exit(1);
  }

  return;
}                         

static
void link_sram_inf(t_sram_inf* cur_sram_inf,
                   const CircuitLibrary& circuit_lib) {
  /* We have two branches: 
   * 1. SPICE SRAM organization information 
   * 2. Verilog SRAM organization information 
   */
  link_one_sram_inf_orgz(cur_sram_inf->spice_sram_inf_orgz, 
                         circuit_lib);
 
  link_one_sram_inf_orgz(cur_sram_inf->verilog_sram_inf_orgz, 
                         circuit_lib);

  return;
}


/************************************************************************** 
 * With given circuit port, find the pb_type port with same name and type
 **************************************************************************/
t_port* find_pb_type_port_match_circuit_model_port(const t_pb_type* pb_type,
                                                   const CircuitLibrary& circuit_lib,
                                                   const CircuitModelId& circuit_model,
                                                   const CircuitPortId& circuit_port) {
  t_port* ret = NULL; 
  size_t num_found = 0;

  /* Search ports */
  for (int iport = 0; iport < pb_type->num_ports; iport++) {
    /* Match the name and port size*/
    if ( (0 == circuit_lib.port_prefix(circuit_model, circuit_port).compare(pb_type->ports[iport].name)) 
      && (size_t(pb_type->ports[iport].num_pins) == circuit_lib.port_size(circuit_model, circuit_port))) {
      /* Match the type*/
      switch (circuit_lib.port_type(circuit_model, circuit_port)) {
      case SPICE_MODEL_PORT_INPUT:
        if ((IN_PORT == pb_type->ports[iport].type)
          &&(0 == pb_type->ports[iport].is_clock)) {
          ret = &(pb_type->ports[iport]);
          num_found++;
        }
        break;
      case SPICE_MODEL_PORT_OUTPUT:
        if (OUT_PORT == pb_type->ports[iport].type) {
          ret = &(pb_type->ports[iport]);
        }
        break;
      case SPICE_MODEL_PORT_CLOCK:
        if ((IN_PORT == pb_type->ports[iport].type)&&(1 == pb_type->ports[iport].is_clock)) {
          ret = &(pb_type->ports[iport]);
          num_found++;
        }
        break;
      case SPICE_MODEL_PORT_INOUT : 
        if ((INOUT_PORT == pb_type->ports[iport].type)&&(0 == pb_type->ports[iport].is_clock)) {
          ret = &(pb_type->ports[iport]);
          num_found++;
        }
        break;
      case SPICE_MODEL_PORT_SRAM:
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])Invalid type for circuit model port(%s)!\n",
                   __FILE__, __LINE__, circuit_lib.port_prefix(circuit_model, circuit_port));
        exit(1);
      }
    }
  }

  /* We should find only 1 match */
  if (1 < num_found) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s, [LINE%d])More than 1 pb_type(%s) port match spice_model_port(%s)!\n",
               __FILE__, __LINE__, pb_type->name, circuit_lib.port_prefix(circuit_model, circuit_port).c_str());
    exit(1);
  }

  return ret;
}


/************************************************************************
 * Map (synchronize) pb_type ports to circuit model ports
 ***********************************************************************/
static 
int link_pb_type_port_to_circuit_model_ports(const t_pb_type* cur_pb_type,
                                             const CircuitLibrary& circuit_lib,
                                             const CircuitModelId& circuit_model) {

  /* Check */
  assert(NULL != cur_pb_type);

  /* Initialize each port */
  for (int iport = 0; iport < cur_pb_type->num_ports; iport++) {
    cur_pb_type->ports[iport].circuit_model_port = CircuitPortId::INVALID(); 
  } 

  /* Return if SPICE_MODEL is NULL */
  if (CircuitModelId::INVALID() == circuit_model) {
    return 0;
  }

  /* For each port, find a SPICE model port, which has the same name and port size */
  for (auto& port : circuit_lib.ports(circuit_model)) {
    t_port* cur_pb_type_port = find_pb_type_port_match_circuit_model_port(cur_pb_type, circuit_lib, circuit_model, port); 
    /* Not every spice_model_port can find a mapped pb_type_port.
     * Since a pb_type only includes necessary ports in technology mapping.
     * ports for physical designs may be ignored ! 
     */
    if (NULL != cur_pb_type_port) {
      cur_pb_type_port->circuit_model_port = port; 
    }
  }
  /* Although some spice_model_port may not have a corresponding pb_type_port 
   * but each pb_type_port should be mapped to a spice_model_port
   */
  for (int iport = 0; iport < cur_pb_type->num_ports; iport++) {
    if (CircuitPortId::INVALID() == cur_pb_type->ports[iport].circuit_model_port) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File:%s, [LINE%d])Pb_type(%s) Port(%s) cannot find a corresponding port in SPICE model(%s)\n",
                 __FILE__, __LINE__, cur_pb_type->name, cur_pb_type->ports[iport].name, 
                 circuit_lib.model_name(circuit_model).c_str());
      exit(1);
    }
  }

  return cur_pb_type->num_ports;
}

/************************************************************************
 * Find a circuit model for an interconnect in pb_type 
 * Case 1: if the circuit_model_name is not defined, 
 *         we find a default circuit model and check its type
 * Case 2: if the circuit_model_name is defined,
 *         we find a matched circuit model and check its type 
 ***********************************************************************/
static 
void link_pb_type_interc_circuit_model_by_type(t_interconnect* cur_interc,
                                               const CircuitLibrary& circuit_lib,
                                               const enum e_spice_model_type& model_type) {

  /* If the circuit_model_name is not defined, we use the default*/
  cur_interc->circuit_model = link_circuit_model_by_name_and_type(cur_interc->spice_model_name,
                                                                  circuit_lib,
                                                                  model_type);
  /* Check the circuit model, we should have one! */
  if (CircuitModelId::INVALID() == cur_interc->circuit_model) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,LINE[%d]) Error in linking circuit model for interconnect(name %s)! Check [LINE%d] in architecture file)!\n",
               __FILE__, __LINE__, 
               cur_interc->name, 
               cur_interc->line_num);
    exit(1);
  } 

  /* Special check for MUXes: 
   * If the multiplexers do not have any input buffers, the loop breaker cannot be disabled
   */
  if (SPICE_MODEL_MUX == model_type) {
    if (NULL != cur_interc->loop_breaker_string) {
      if (false == circuit_lib.is_input_buffered(cur_interc->circuit_model)) {
        vpr_printf(TIO_MESSAGE_INFO,
                   "Line[%d] Cannot disable an interconnect without input buffering.\n",
                   cur_interc->line_num);
      }
    }   
  }

  return;
}

/************************************************************************
 * Find a circuit model for an interconnect in pb_type 
 * Case 1: if this is a DIRECT interconnection, 
 *         we will try to find a circuit model whose type is WIRE
 * Case 2: if this is a COMPLETE interconnection, we should evaluate 
 *         the number of multiplexer required.
 *         when it does require multiplexers
 *         we will try to find a circuit model whose type is MUX
 *         otherwise, 
 *         we will try to find a circuit model whose type is WIRE
 * Case 3: if this is a MUX interconnection, 
 *         we will try to find a circuit model whose type is WIRE
 ***********************************************************************/
static 
void link_pb_type_interc_circuit_model(t_interconnect* cur_interc,
                                       const CircuitLibrary& circuit_lib) {
  switch (cur_interc->type) {
  case DIRECT_INTERC:
    link_pb_type_interc_circuit_model_by_type(cur_interc, circuit_lib, SPICE_MODEL_WIRE);
    break;
  case COMPLETE_INTERC:
    /* Special for Completer Interconnection:
     * 1. The input number is 1, this infers a direct interconnection.
     * 2. The input number is larger than 1, this infers multplexers
     * according to interconnect[j].num_mux identify the number of input at this level
     */
    if (0 == cur_interc->num_mux) {
      link_pb_type_interc_circuit_model_by_type(cur_interc, circuit_lib, SPICE_MODEL_WIRE);
    } else {
      link_pb_type_interc_circuit_model_by_type(cur_interc, circuit_lib, SPICE_MODEL_MUX);
    } 
    break;
  case MUX_INTERC:
    link_pb_type_interc_circuit_model_by_type(cur_interc, circuit_lib, SPICE_MODEL_MUX);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s,LINE[%d]) Unknown type of interconnection (name=%s) defined in architecture file(LINE%d)!\n",
               __FILE__, __LINE__, cur_interc->name, cur_interc->line_num);
    exit(1);
  }
  return;
}

/************************************************************************
 * Walk through the pb_types in a recursive way 
 * Find circuit_model_name definition in pb_types
 * Try to match the name with defined spice_models
 ***********************************************************************/
static 
void link_pb_types_circuit_model_rec(t_pb_type* cur_pb_type,
                                     const CircuitLibrary& circuit_lib) {
  if (NULL == cur_pb_type) {
    vpr_printf(TIO_MESSAGE_WARNING,
              "(File:%s,LINE[%d])cur_pb_type is null pointor!\n",
              __FILE__, __LINE__);
    return;
  }

  /* If there is a circuit_model_name or physical_pb_type_name referring to a physical pb type, 
   * this is a leaf node!
   */
  if ( TRUE == is_primitive_pb_type(cur_pb_type) )  {
    /* What annoys me is VPR create a sub pb_type for each lut which suppose to be a leaf node
     * This may bring software convience but ruins SPICE modeling
     */
    if (NULL != cur_pb_type->physical_pb_type_name) {
      /* if this is not a physical pb_type, we do not care its circuit_model_name*/
      return;
    }
    /* Let's find a matched circuit model!*/
    cur_pb_type->circuit_model = circuit_lib.model(cur_pb_type->spice_model_name);
    if (CircuitModelId::INVALID() == cur_pb_type->circuit_model) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File:%s,LINE[%d]) Fail to find a defined circuit model called %s, in pb_type(%s)!\n",
                 __FILE__, __LINE__, cur_pb_type->spice_model_name, cur_pb_type->name);
      exit(1);
    }
    /* Map pb_type ports to SPICE model ports*/
    link_pb_type_port_to_circuit_model_ports(cur_pb_type, circuit_lib, cur_pb_type->circuit_model);
    return;
  }

  /* Otherwise, initialize it to be OPEN node */
  cur_pb_type->circuit_model = CircuitModelId::INVALID();

  /* Traversal the hierarchy*/
  for (int imode = 0; imode < cur_pb_type->num_modes; imode++) {
    /* Task 1: Find the interconnections and match the spice_model */
    for (int jinterc = 0; jinterc < cur_pb_type->modes[imode].num_interconnect; jinterc++) {
      /* Initialize it to be OPEN node */
      cur_pb_type->modes[imode].interconnect[jinterc].circuit_model = CircuitModelId::INVALID();
      link_pb_type_interc_circuit_model(&(cur_pb_type->modes[imode].interconnect[jinterc]),
                                        circuit_lib);
    }
    /* Task 2: Find the child pb_type, do matching recursively */
    for (int ipb = 0; ipb < cur_pb_type->modes[imode].num_pb_type_children; ipb++) {
      link_pb_types_circuit_model_rec(&(cur_pb_type->modes[imode].pb_type_children[ipb]), circuit_lib);
    }
  } 
  return;  
}

/* Check if the spice model structure is the same with the switch_inf structure */
static 
size_t check_circuit_model_structure_match_switch_inf(const t_switch_inf& target_switch_inf,
                                                     const CircuitLibrary& circuit_lib) {
  size_t num_err = 0;

  VTR_ASSERT_SAFE(CircuitModelId::INVALID() != target_switch_inf.circuit_model);
  if (target_switch_inf.structure != circuit_lib.mux_structure(target_switch_inf.circuit_model)) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s,[LINE%d]) Mismatch in MUX structure between circuit model(%s, %s) and switch_inf(%s, %s)!\n",
               __FILE__, __LINE__, 
               circuit_lib.model_name(target_switch_inf.circuit_model).c_str(), 
               CIRCUIT_MODEL_STRUCTURE_TYPE_STRING[size_t(circuit_lib.mux_structure(target_switch_inf.circuit_model))],
               target_switch_inf.name,
               CIRCUIT_MODEL_STRUCTURE_TYPE_STRING[size_t(target_switch_inf.structure)]);
    num_err++;
  }
  return num_err;
}


/************************************************************************
 * Initialize and check circuit models defined in architecture
 * Tasks:
 * 1. Link the circuit model defined in pb_types and routing switches
 * 2. Add default circuit model for any inexplicit definition  
 ***********************************************************************/
void link_circuit_library_to_arch(t_arch* arch,
                                  t_det_routing_arch* routing_arch) {

  vpr_printf(TIO_MESSAGE_INFO, "Linking circuit models to modules in FPGA architecture...\n");

  /* Check Circuit models first*/
  VTR_ASSERT_SAFE( (NULL != arch) && (NULL != arch->spice) );

  /* 1. Link the spice model defined in pb_types and routing switches */
  /* Step A:  Check routing switches, connection blocks*/
  if (0 >= arch->num_cb_switch) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s, LINE[%d]) Define Switches for Connection Blocks is mandatory in FPGA X2P support! Miss this part in architecture file.\n",
               __FILE__,__LINE__);
    exit(1);
  }
  
  for (int i = 0; i < arch->num_cb_switch; i++) {
    arch->cb_switches[i].circuit_model = link_circuit_model_by_name_and_type(arch->cb_switches[i].spice_model_name,
                                                                             arch->spice->circuit_lib, SPICE_MODEL_MUX);
    if (CircuitModelId::INVALID() == arch->cb_switches[i].circuit_model) {
      vpr_printf(TIO_MESSAGE_ERROR,
                 "(FILE:%s, LINE[%d])Invalid circuit model name(%s) of Switch(%s) is undefined in circuit models!\n",
                 __FILE__, __LINE__, arch->cb_switches[i].spice_model_name, arch->cb_switches[i].name);
      exit(1);
    }
    /* Check the spice model structure is matched with the structure in switch_inf */
    if (0 < check_circuit_model_structure_match_switch_inf(arch->cb_switches[i], arch->spice->circuit_lib)) {
      exit(1);
    }
  } 
 
  /* Step B: Check switch list: Switch Box*/
  if (0 >= arch->num_switches) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s, LINE[%d]) Define Switches for Switch Boxes is mandatory in FPGA X2P support! Miss this part in architecture file.\n",
               __FILE__, __LINE__);
    exit(1);
  }
  
  for (int i = 0; i < arch->num_switches; i++) {
    arch->Switches[i].circuit_model = link_circuit_model_by_name_and_type(arch->Switches[i].spice_model_name,
                                                                          arch->spice->circuit_lib, SPICE_MODEL_MUX);
    if (CircuitModelId::INVALID() == arch->Switches[i].circuit_model) {
      vpr_printf(TIO_MESSAGE_ERROR,
                 "(FILE:%s, LINE[%d])Invalid circuit model name(%s) of Switch(%s) is undefined in circuit models!\n",
                 __FILE__, __LINE__, arch->Switches[i].spice_model_name, arch->Switches[i].name);
      exit(1);
    }
    /* Check the spice model structure is matched with the structure in switch_inf */
    if (0 < check_circuit_model_structure_match_switch_inf(arch->Switches[i], arch->spice->circuit_lib)) {
      exit(1);
    }
  } 

  /* Update the switches in detailed routing architecture settings*/
  for (int i = 0; i < routing_arch->num_switch; i++) {
    switch_inf[i].circuit_model = link_circuit_model_by_name_and_type(switch_inf[i].spice_model_name,
                                                                      arch->spice->circuit_lib, SPICE_MODEL_MUX);
    if (CircuitModelId::INVALID() == switch_inf[i].circuit_model) {
      vpr_printf(TIO_MESSAGE_ERROR,
                 "(FILE:%s, LINE[%d])Invalid circuit model name(%s) of Switch(%s) is undefined in circuit models!\n",
                 __FILE__, __LINE__, switch_inf[i].spice_model_name, switch_inf[i].name);
      exit(1);
    }
  }

  /* Step C: Find SRAM Model*/
  link_sram_inf(&(arch->sram_inf), arch->spice->circuit_lib);

  /* Step D: Find the segment spice_model*/
  for (int i = 0; i < arch->num_segments; i++) {
    arch->Segments[i].circuit_model = link_circuit_model_by_name_and_type(arch->Segments[i].spice_model_name,
                                                                          arch->spice->circuit_lib, SPICE_MODEL_CHAN_WIRE);
    if (CircuitModelId::INVALID() == arch->Segments[i].circuit_model) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(FILE:%s, LINE[%d])Invalid circuit model name(%s) of Segment(Length:%d) is undefined in circuit models!\n",
                 __FILE__ ,__LINE__, 
                 arch->Segments[i].spice_model_name, 
                 arch->Segments[i].length);
      exit(1);
    }
  } 

  /* Step E: Direct connections between CLBs */
  for (int i = 0; i < arch->num_directs; i++) {
    arch->Directs[i].circuit_model = link_circuit_model_by_name_and_type(arch->Directs[i].spice_model_name,
                                                                         arch->spice->circuit_lib, SPICE_MODEL_WIRE);
    /* Check SPICE model type */
    if (CircuitModelId::INVALID() == arch->Directs[i].circuit_model) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(FILE:%s, LINE[%d])Invalid circuit model name(%s) of CLB to CLB Direct Connection (name=%s) is undefined in circuit models!\n",
                 __FILE__ ,__LINE__, 
                 arch->Directs[i].spice_model_name, 
                 arch->Directs[i].name);
      exit(1);
    }
    /* Copy it to clb2clb_directs */
    clb2clb_direct[i].circuit_model = arch->Directs[i].circuit_model; 
  } 

  /* 2. Search Complex Blocks (Pb_Types), Link spice_model according to the spice_model_name*/
  for (int i = 0; i < num_types; i++) {
    if (NULL != type_descriptors[i].pb_type) {
      link_pb_types_circuit_model_rec(type_descriptors[i].pb_type, arch->spice->circuit_lib);
    }
  }

  vpr_printf(TIO_MESSAGE_INFO, "Linking circuit models to modules in FPGA architecture...Completed\n");

  return;
}

/************************************************************************
 * End of file : link_arch_circuit_lib.cpp 
 ***********************************************************************/
