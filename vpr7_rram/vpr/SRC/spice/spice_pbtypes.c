/***********************************/
/*      SPICE Modeling for VPR     */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
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

/* Include SPICE support headers*/
#include "linkedlist.h"
#include "fpga_spice_globals.h"
#include "spice_globals.h"
#include "fpga_spice_utils.h"
#include "spice_utils.h"
#include "spice_mux.h"
#include "spice_lut.h"
#include "spice_primitives.h"
#include "spice_pbtypes.h"

/***** Subroutines *****/

/* Print ports of pb_types,
 * SRAM ports are not printed here!!! 
 */
void fprint_pb_type_ports(FILE* fp,
                          char* port_prefix,
                          int use_global_clock,
                          t_pb_type* cur_pb_type) {
  int iport, ipin;
  int num_pb_type_input_port = 0;
  t_port** pb_type_input_ports = NULL;

  int num_pb_type_output_port = 0;
  t_port** pb_type_output_ports = NULL;

  int num_pb_type_inout_port = 0;
  t_port** pb_type_inout_ports = NULL;

  int num_pb_type_clk_port = 0;
  t_port** pb_type_clk_ports = NULL;

  char* formatted_port_prefix = chomp_spice_node_prefix(port_prefix);

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
  }

  /* Inputs */
  /* Find pb_type input ports */
  pb_type_input_ports = find_pb_type_ports_match_spice_model_port_type(cur_pb_type, SPICE_MODEL_PORT_INPUT, &num_pb_type_input_port); 
  for (iport = 0; iport < num_pb_type_input_port; iport++) {
    for (ipin = 0; ipin < pb_type_input_ports[iport]->num_pins; ipin++) {
      fprintf(fp, "%s->%s[%d] ", formatted_port_prefix, pb_type_input_ports[iport]->name, ipin);
    }
  }
  /* Outputs */
  /* Find pb_type output ports */
  pb_type_output_ports = find_pb_type_ports_match_spice_model_port_type(cur_pb_type, SPICE_MODEL_PORT_OUTPUT, &num_pb_type_output_port); 
  for (iport = 0; iport < num_pb_type_output_port; iport++) {
    for (ipin = 0; ipin < pb_type_output_ports[iport]->num_pins; ipin++) {
      fprintf(fp, "%s->%s[%d] ", formatted_port_prefix, pb_type_output_ports[iport]->name, ipin);
    }
  }
  /* INOUT ports */
  /* Find pb_type inout ports */
  pb_type_inout_ports = find_pb_type_ports_match_spice_model_port_type(cur_pb_type, SPICE_MODEL_PORT_INOUT, &num_pb_type_inout_port); 
  for (iport = 0; iport < num_pb_type_inout_port; iport++) {
    for (ipin = 0; ipin < pb_type_inout_ports[iport]->num_pins; ipin++) {
      fprintf(fp, "%s->%s[%d] ", formatted_port_prefix, pb_type_inout_ports[iport]->name, ipin);
    }
  }
  /* Clocks */
  /* Identify if the clock port is a global signal */
  /* Find pb_type clock ports */
  pb_type_clk_ports = find_pb_type_ports_match_spice_model_port_type(cur_pb_type, SPICE_MODEL_PORT_CLOCK, &num_pb_type_clk_port); 
  for (iport = 0; iport < num_pb_type_clk_port; iport++) {
    /* only search mapped ports for primitive node */
    if (NULL != cur_pb_type->spice_model) {
      /* We need to bypass global ports */
      assert(NULL != pb_type_clk_ports[iport]->spice_model_port);
      if (TRUE == pb_type_clk_ports[iport]->spice_model_port->is_global) {
        continue;
      }
    }
    for (ipin = 0; ipin < pb_type_clk_ports[iport]->num_pins; ipin++) {
      fprintf(fp, "%s->%s[%d] ", formatted_port_prefix, pb_type_clk_ports[iport]->name, ipin);
    }
  }

  /* Free */
  my_free(formatted_port_prefix);
  my_free(pb_type_input_ports);
  my_free(pb_type_output_ports);
  my_free(pb_type_inout_ports);
  my_free(pb_type_clk_ports);

  return;
}

/* This is a truncated version of generate_spice_src_des_pb_graph_pin_prefix
 * Only used to generate prefix for those dangling pin in PB Types
 */
void fprint_spice_dangling_des_pb_graph_pin_interc(FILE* fp,
                                                   t_pb_graph_pin* des_pb_graph_pin,
                                                   t_mode* cur_mode,
                                                   enum e_pin2pin_interc_type pin2pin_interc_type,
                                                   char* parent_pin_prefix) {
  t_pb_graph_node* des_pb_graph_node = NULL;
  t_pb_type* des_pb_type = NULL;
  int des_pb_type_index = -1;
  int fan_in = 0;
  t_interconnect* cur_interc = NULL;
  char* des_pin_prefix = NULL;
  
  /* char* formatted_parent_pin_prefix = format_spice_node_prefix(parent_pin_prefix);*/  /* Complete a "_" at the end if needed*/
  //char* chomped_parent_pin_prefix = chomp_spice_node_prefix(parent_pin_prefix); /* Remove a "_" at the end if needed*/

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check the pb_graph_nodes*/ 
  if (NULL == des_pb_graph_pin) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid pointer: des_pb_graph_pin.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  find_interc_fan_in_des_pb_graph_pin(des_pb_graph_pin, cur_mode, &cur_interc, &fan_in);
  if ((NULL != cur_interc)&&(0 != fan_in)) {
    return;
    /*
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Cur_interc not NULL & fan_in not zero!\n", 
               __FILE__, __LINE__); 
    exit(1);
    */
  }

  /* Initialize */
  des_pb_graph_node = des_pb_graph_pin->parent_node;
  des_pb_type = des_pb_graph_node->pb_type; 
  des_pb_type_index = des_pb_graph_node->placement_index;

  /* generate the pin prefix for src_pb_graph_node and des_pb_graph_node */
  switch (pin2pin_interc_type) {
  case INPUT2INPUT_INTERC:
    /* src_pb_graph_node.input_pins -----------------> des_pb_graph_node.input_pins 
     * des_pb_graph_node is a child of src_pb_graph_node 
     * parent_pin_prefix is the prefix from parent pb_graph_node, in this case, src_pb_graph_node 
     * src_pin_prefix: we need to handle the feedbacks, they comes from the same-level pb_graph_node 
     * src_pin_prefix = <formatted_parent_pin_prefix>
     * OR
     * src_pin_prefix = <formatted_parent_pin_prefix>_<src_pb_type>[<src_pb_type_index>]
     * des_pin_prefix = <formatted_parent_pin_prefix>mode[<mode_name>]_<des_pb_type>[<des_pb_type_index>]_
     */
    /*
    des_pin_prefix = (char*)my_malloc(sizeof(char)*
                        (strlen(formatted_parent_pin_prefix) + 5 + strlen(cur_mode->name)
                         + 2 + strlen(des_pb_type->name) + 1 + strlen(my_itoa(des_pb_type_index)) + 1 + 1));
    sprintf(des_pin_prefix, "%smode[%s]_%s[%d]",
            formatted_parent_pin_prefix, cur_mode->name, des_pb_type->name, des_pb_type_index);
    */
    /*Simplify the prefix, make the SPICE netlist readable*/
    des_pin_prefix = (char*)my_malloc(sizeof(char)*
                        (strlen(des_pb_type->name) + 1 + strlen(my_itoa(des_pb_type_index)) + 1 + 1));
    sprintf(des_pin_prefix, "%s[%d]",
             des_pb_type->name, des_pb_type_index);
    /* This is a start point, we connect it to gnd*/
    fprintf(fp, "Vdangling_%s->%s[%d] %s->%s[%d] 0 0\n", 
            des_pin_prefix, des_pb_graph_pin->port->name, des_pb_graph_pin->pin_number, 
            des_pin_prefix, des_pb_graph_pin->port->name, des_pb_graph_pin->pin_number); 
    fprintf(fp, ".nodeset V(%s->%s[%d]) 0\n", 
            des_pin_prefix, des_pb_graph_pin->port->name, des_pb_graph_pin->pin_number); 
    break;
  case OUTPUT2OUTPUT_INTERC:
    /* src_pb_graph_node.output_pins -----------------> des_pb_graph_node.output_pins 
     * src_pb_graph_node is a child of des_pb_graph_node 
     * parent_pin_prefix is the prefix from parent pb_graph_node, in this case, des_pb_graph_node 
     * src_pin_prefix = <formatted_parent_pin_prefix>mode[<mode_name>]_<src_pb_type>[<src_pb_type_index>]_
     * des_pin_prefix = <formatted_parent_pin_prefix>
     */
    /*
    des_pin_prefix = (char*)my_malloc(sizeof(char)*
                         (strlen(formatted_parent_pin_prefix) + 5 + strlen(cur_mode->name)
                         + 2 + strlen(des_pb_type->name) + 1 + strlen(my_itoa(des_pb_type_index)) + 1 + 1));
    sprintf(des_pin_prefix, "%smode[%s]_%s[%d]",
            formatted_parent_pin_prefix, cur_mode->name, des_pb_type->name, des_pb_type_index);
    */
    if (des_pb_type == cur_mode->parent_pb_type) { /* Interconnection from parent pb_type*/
      des_pin_prefix = (char*)my_malloc(sizeof(char)*
                           (5 + strlen(cur_mode->name) + 2 ));
      sprintf(des_pin_prefix, "mode[%s]", cur_mode->name);
    } else {
      des_pin_prefix = (char*)my_malloc(sizeof(char)*
                          (strlen(des_pb_type->name) + 1 + strlen(my_itoa(des_pb_type_index)) + 1 + 1));
      sprintf(des_pin_prefix, "%s[%d]",
               des_pb_type->name, des_pb_type_index);
    }
    /*Simplify the prefix, make the SPICE netlist readable*/
    /* This is a start point, we connect it to gnd*/
    fprintf(fp, "Vdangling_%s->%s[%d] %s->%s[%d] 0 0\n", 
            des_pin_prefix, des_pb_graph_pin->port->name, des_pb_graph_pin->pin_number, 
            des_pin_prefix, des_pb_graph_pin->port->name, des_pb_graph_pin->pin_number); 
    fprintf(fp, ".nodeset V(%s->%s[%d]) 0\n", 
            des_pin_prefix, des_pb_graph_pin->port->name, des_pb_graph_pin->pin_number); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s [LINE%d])Invalid pin to pin interconnection type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  my_free(des_pin_prefix);

  return;
}

void generate_spice_src_des_pb_graph_pin_prefix(t_pb_graph_node* src_pb_graph_node,
                                                t_pb_graph_node* des_pb_graph_node,
                                                enum e_pin2pin_interc_type pin2pin_interc_type,
                                                t_interconnect* pin2pin_interc,
                                                char* parent_pin_prefix,
                                                char** src_pin_prefix,
                                                char** des_pin_prefix) {
  t_pb_type* src_pb_type = NULL;
  int src_pb_type_index = -1;

  t_pb_type* des_pb_type = NULL;
  int des_pb_type_index = -1;
  
  //char* formatted_parent_pin_prefix = format_spice_node_prefix(parent_pin_prefix); /* Complete a "_" at the end if needed*/
  //char* chomped_parent_pin_prefix = chomp_spice_node_prefix(parent_pin_prefix); /* Remove a "_" at the end if needed*/
  
  t_mode* pin2pin_interc_parent_mode = NULL;

  /* Check the pb_graph_nodes*/ 
  if (NULL == src_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid pointer: src_pb_graph_node.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  if (NULL == des_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid pointer: des_pb_graph_node.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  if (NULL == pin2pin_interc) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid pointer: pin2pin_interc.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Initialize */
  src_pb_type = src_pb_graph_node->pb_type; 
  src_pb_type_index = src_pb_graph_node->placement_index;
  des_pb_type = des_pb_graph_node->pb_type; 
  des_pb_type_index = des_pb_graph_node->placement_index;
  
  pin2pin_interc_parent_mode = pin2pin_interc->parent_mode;

  assert(NULL == (*src_pin_prefix));
  assert(NULL == (*des_pin_prefix));
  /* generate the pin prefix for src_pb_graph_node and des_pb_graph_node */
  switch (pin2pin_interc_type) {
  case INPUT2INPUT_INTERC:
    /* src_pb_graph_node.input_pins -----------------> des_pb_graph_node.input_pins 
     * des_pb_graph_node is a child of src_pb_graph_node 
     * parent_pin_prefix is the prefix from parent pb_graph_node, in this case, src_pb_graph_node 
     * src_pin_prefix: we need to handle the feedbacks, they comes from the same-level pb_graph_node 
     * src_pin_prefix = <formatted_parent_pin_prefix>
     * OR
     * src_pin_prefix = <formatted_parent_pin_prefix>_<src_pb_type>[<src_pb_type_index>]
     * des_pin_prefix = <formatted_parent_pin_prefix>mode[<mode_name>]_<des_pb_type>[<des_pb_type_index>]_
     */
    if (src_pb_type == des_pb_type->parent_mode->parent_pb_type) { /* Interconnection from parent pb_type*/
      /*
      (*src_pin_prefix) = my_strdup(chomped_parent_pin_prefix);
      */
      /*Simplify the prefix, make the SPICE netlist readable*/
      (*src_pin_prefix) = (char*)my_malloc(sizeof(char)*
                           (5 + strlen(des_pb_type->parent_mode->name) + 2));
      sprintf((*src_pin_prefix), "mode[%s]", des_pb_type->parent_mode->name);
    } else {
      /*
      (*src_pin_prefix) = (char*)my_malloc(sizeof(char)*
                           (strlen(formatted_parent_pin_prefix) + 5 + strlen(pin2pin_interc_parent_mode->name)
                           + 2 + strlen(src_pb_type->name) + 1 + strlen(my_itoa(src_pb_type_index)) + 1 + 1));
      sprintf((*src_pin_prefix), "%smode[%s]_%s[%d]",
              formatted_parent_pin_prefix, pin2pin_interc_parent_mode->name, src_pb_type->name, src_pb_type_index);
       */
      /*Simplify the prefix, make the SPICE netlist readable*/
      (*src_pin_prefix) = (char*)my_malloc(sizeof(char)*
                           (strlen(src_pb_type->name) + 1 + strlen(my_itoa(src_pb_type_index)) + 1 + 1));
      sprintf((*src_pin_prefix), "%s[%d]",
              src_pb_type->name, src_pb_type_index);
    }
    /*
    (*des_pin_prefix) = (char*)my_malloc(sizeof(char)*
                        (strlen(formatted_parent_pin_prefix) + 5 + strlen(pin2pin_interc_parent_mode->name)
                         + 2 + strlen(des_pb_type->name) + 1 + strlen(my_itoa(des_pb_type_index)) + 1 + 1));
    sprintf((*des_pin_prefix), "%smode[%s]_%s[%d]",
            formatted_parent_pin_prefix, pin2pin_interc_parent_mode->name, des_pb_type->name, des_pb_type_index);
    */
    /*Simplify the prefix, make the SPICE netlist readable*/
    (*des_pin_prefix) = (char*)my_malloc(sizeof(char)*
                        (strlen(des_pb_type->name) + 1 + strlen(my_itoa(des_pb_type_index)) + 1 + 1));
    sprintf((*des_pin_prefix), "%s[%d]",
             des_pb_type->name, des_pb_type_index);
    break;
  case OUTPUT2OUTPUT_INTERC:
    /* src_pb_graph_node.output_pins -----------------> des_pb_graph_node.output_pins 
     * src_pb_graph_node is a child of des_pb_graph_node 
     * parent_pin_prefix is the prefix from parent pb_graph_node, in this case, des_pb_graph_node 
     * src_pin_prefix = <formatted_parent_pin_prefix>mode[<mode_name>]_<src_pb_type>[<src_pb_type_index>]_
     * des_pin_prefix = <formatted_parent_pin_prefix>
     */
    /*
    (*src_pin_prefix) = (char*)my_malloc(sizeof(char)*
                        (strlen(formatted_parent_pin_prefix) + 5 + strlen(pin2pin_interc_parent_mode->name)
                         + 2 + strlen(src_pb_type->name) + 1 + strlen(my_itoa(src_pb_type_index)) + 1 + 1));
    sprintf((*src_pin_prefix), "%smode[%s]_%s[%d]",
            formatted_parent_pin_prefix, pin2pin_interc_parent_mode->name, src_pb_type->name, src_pb_type_index);
    */
    /*Simplify the prefix, make the SPICE netlist readable*/
    (*src_pin_prefix) = (char*)my_malloc(sizeof(char)*
                        (strlen(src_pb_type->name) + 1 + strlen(my_itoa(src_pb_type_index)) + 1 + 1));
    sprintf((*src_pin_prefix), "%s[%d]",
            src_pb_type->name, src_pb_type_index);
    if (des_pb_type == src_pb_type->parent_mode->parent_pb_type) { /* Interconnection from parent pb_type*/
      /*
      (*des_pin_prefix) = my_strdup(chomped_parent_pin_prefix);
      */
      /*Simplify the prefix, make the SPICE netlist readable*/
      (*des_pin_prefix) = (char*)my_malloc(sizeof(char)*
                           (5 + strlen(src_pb_type->parent_mode->name) + 2));
      sprintf((*des_pin_prefix), "mode[%s]", src_pb_type->parent_mode->name);
    } else {
      /*
      (*des_pin_prefix) = (char*)my_malloc(sizeof(char)*
                           (strlen(formatted_parent_pin_prefix) + 5 + strlen(pin2pin_interc_parent_mode->name)
                           + 2 + strlen(des_pb_type->name) + 1 + strlen(my_itoa(des_pb_type_index)) + 1 + 1));
      sprintf((*des_pin_prefix), "%smode[%s]_%s[%d]",
              formatted_parent_pin_prefix, pin2pin_interc_parent_mode->name, des_pb_type->name, des_pb_type_index);
      */
      /*Simplify the prefix, make the SPICE netlist readable*/
      (*des_pin_prefix) = (char*)my_malloc(sizeof(char)*
                           (strlen(des_pb_type->name) + 1 + strlen(my_itoa(des_pb_type_index)) + 1 + 1));
      sprintf((*des_pin_prefix), "%s[%d]",
              des_pb_type->name, des_pb_type_index);
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s [LINE%d])Invalid pin to pin interconnection type!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  return;
}

void find_interc_fan_in_des_pb_graph_pin(t_pb_graph_pin* des_pb_graph_pin,
                                         t_mode* cur_mode,
                                         t_interconnect** cur_interc,
                                         int* fan_in) { 
  int iedge;
  
  (*cur_interc) = NULL;
  (*fan_in) = 0;  

  /* Search the input edges only, stats on the size of MUX we may need (fan-in) */
  for (iedge = 0; iedge < des_pb_graph_pin->num_input_edges; iedge++) {
    /* 1. First, we should make sure this interconnect is in the selected mode!!!*/
    if (cur_mode == des_pb_graph_pin->input_edges[iedge]->interconnect->parent_mode) {
      /* Check this edge*/
      check_pb_graph_edge(*(des_pb_graph_pin->input_edges[iedge]));
      /* Record the interconnection*/
      if (NULL == (*cur_interc)) {
        (*cur_interc) = des_pb_graph_pin->input_edges[iedge]->interconnect;
      } else { /* Make sure the interconnections for this pin is the same!*/
        assert((*cur_interc) == des_pb_graph_pin->input_edges[iedge]->interconnect);
      }
      /* Search the input_pins of input_edges only*/
      (*fan_in) += des_pb_graph_pin->input_edges[iedge]->num_input_pins;
    }
  }

  return;
}

/* We check output_pins of cur_pb_graph_node and its the input_edges
 * Built the interconnections between outputs of cur_pb_graph_node and outputs of child_pb_graph_node
 *   src_pb_graph_node.[in|out]_pins -----------------> des_pb_graph_node.[in|out]pins
 *                                        /|\
 *                                         |
 *                         input_pins,   edges,       output_pins
 */ 
void fprintf_spice_pb_graph_pin_interc(FILE* fp,
                                       char* parent_pin_prefix,
                                       enum e_pin2pin_interc_type pin2pin_interc_type,
                                       t_pb_graph_pin* des_pb_graph_pin,
                                       t_mode* cur_mode,
                                       int select_edge) {
  int iedge, ilevel;
  int fan_in = 0;
  t_interconnect* cur_interc = NULL; 
  enum e_interconnect spice_interc_type = DIRECT_INTERC;

  t_pb_graph_pin* src_pb_graph_pin = NULL;
  t_pb_graph_node* src_pb_graph_node = NULL;
  t_pb_type* src_pb_type = NULL;
  int src_pb_type_index = -1;

  t_pb_graph_node* des_pb_graph_node = NULL;
  t_pb_type* des_pb_type = NULL;
  int des_pb_type_index = -1;

  char* formatted_parent_pin_prefix = chomp_spice_node_prefix(parent_pin_prefix); /* Complete a "_" at the end if needed*/
  char* src_pin_prefix = NULL;
  char* des_pin_prefix = NULL;
  char* sram_vdd_port_name = NULL;

  int num_sram_bits = 0;
  int* sram_bits = NULL;
  int num_sram = 0;
  int cur_sram = 0;
  int mux_level = 0;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* 1. identify pin interconnection type, 
   * 2. Identify the number of fan-in (Consider interconnection edges of only selected mode)
   * 3. Select and print the SPICE netlist
   */
  fan_in = 0;
  cur_interc = NULL;
  find_interc_fan_in_des_pb_graph_pin(des_pb_graph_pin, cur_mode, &cur_interc, &fan_in);
  if ((NULL == cur_interc)||(0 == fan_in)) { 
    /* No interconnection matched */
    /* Connect this pin to GND for better convergence */
    /* TODO: find the correct pin name!!!*/
    fprint_spice_dangling_des_pb_graph_pin_interc(fp, des_pb_graph_pin, cur_mode, pin2pin_interc_type,
                                                  formatted_parent_pin_prefix);
    return;
  }
  /* Initialize the interconnection type that will be implemented in SPICE netlist*/
  switch (cur_interc->type) {
    case DIRECT_INTERC:
      assert(1 == fan_in);
      spice_interc_type = DIRECT_INTERC;
      break;
    case COMPLETE_INTERC:
      if (1 == fan_in) {
        spice_interc_type = DIRECT_INTERC;
      } else {
        assert((2 == fan_in)||(2 < fan_in));
        spice_interc_type = MUX_INTERC;
      }
      break;
    case MUX_INTERC:
      assert((2 == fan_in)||(2 < fan_in));
      spice_interc_type = MUX_INTERC;
      break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid interconnection type for %s (Arch[LINE%d])!\n",
               __FILE__, __LINE__, cur_interc->name, cur_interc->line_num);
    exit(1);
  }
  /* This time, (2nd round), we print the subckt, according to interc type*/ 
  switch (spice_interc_type) {
  case DIRECT_INTERC:
    /* Check : 
     * 1. Direct interc has only one fan-in!
     */
    assert(1 == fan_in);
    //assert(1 == des_pb_graph_pin->num_input_edges);
    /* For more than one mode defined, the direct interc has more than one input_edge ,
     * We need to find which edge is connected the pin we want
     */
    for (iedge = 0; iedge < des_pb_graph_pin->num_input_edges; iedge++) {
      if (cur_interc == des_pb_graph_pin->input_edges[iedge]->interconnect) {
        break;
      }
    }
    assert(iedge < des_pb_graph_pin->num_input_edges);
    /* 2. spice_model is a wire */ 
    assert(NULL != cur_interc->spice_model);
    assert(SPICE_MODEL_WIRE == cur_interc->spice_model->type);
    assert(NULL != cur_interc->spice_model->wire_param);
    /* Initialize*/
    /* Source pin, node, pb_type*/
    src_pb_graph_pin = des_pb_graph_pin->input_edges[iedge]->input_pins[0];
    src_pb_graph_node = src_pb_graph_pin->parent_node;
    src_pb_type = src_pb_graph_node->pb_type;
    src_pb_type_index = src_pb_graph_node->placement_index;
    /* Des pin, node, pb_type */
    des_pb_graph_node  = des_pb_graph_pin->parent_node;
    des_pb_type = des_pb_graph_node->pb_type;
    des_pb_type_index = des_pb_graph_node->placement_index;
    /* Generate the pin_prefix for src_pb_graph_node and des_pb_graph_node*/
    generate_spice_src_des_pb_graph_pin_prefix(src_pb_graph_node, des_pb_graph_node, pin2pin_interc_type, 
                                               cur_interc, formatted_parent_pin_prefix, &src_pin_prefix, &des_pin_prefix);
    /* Call the subckt that has already been defined before */
    fprintf(fp, "X%s[%d] ", cur_interc->spice_model->prefix, cur_interc->spice_model->cnt); 
    cur_interc->spice_model->cnt++; /* Stats the number of spice_model used*/
    /* Print the pin names! Input and output
     * Input: port_prefix_<child_pb_graph_node>-><port_name>[pin_index]
     * Output: port_prefix_<port_name>[pin_index]
     */
    /* Input */
    /* Make sure correctness*/
    assert(src_pb_type == des_pb_graph_pin->input_edges[iedge]->input_pins[0]->port->parent_pb_type);
    /* Print */
    fprintf(fp, "%s->%s[%d] ", 
            src_pin_prefix, src_pb_graph_pin->port->name, src_pb_graph_pin->pin_number);
    /* Output */
    fprintf(fp, "%s->%s[%d] ", 
            des_pin_prefix, des_pb_graph_pin->port->name, des_pb_graph_pin->pin_number); 
    /* Middle output for wires in logic blocks: TODO: Abolish to save simulation time */
    /* fprintf(fp, "gidle_mid_out "); */
    /* Local vdd and gnd, TODO: we should have an independent VDD for all local interconnections*/
    fprintf(fp, "gvdd_local_interc sgnd ");
    /* End with spice_model name */
    fprintf(fp, "%s\n", cur_interc->spice_model->name);
    /* Free */
    my_free(src_pin_prefix);
    my_free(des_pin_prefix);
    src_pin_prefix = NULL;
    des_pin_prefix = NULL;
    break;
  case COMPLETE_INTERC:
  case MUX_INTERC:
    /* Check : 
     * MUX should have at least 2 fan_in
     */
    assert((2 == fan_in)||(2 < fan_in));
    /* 2. spice_model is a wire */ 
    assert(NULL != cur_interc->spice_model);
    assert(SPICE_MODEL_MUX == cur_interc->spice_model->type);
    /* Call the subckt that has already been defined before */
    fprintf(fp, "X%s_size%d[%d] ", cur_interc->spice_model->prefix, fan_in, cur_interc->spice_model->cnt);
    /* Global ports */
    if (0 < rec_fprint_spice_model_global_ports(fp, cur_interc->spice_model, FALSE)) { 
      fprintf(fp, "+ ");
    }
    /* Inputs */
    for (iedge = 0; iedge < des_pb_graph_pin->num_input_edges; iedge++) {
      if (cur_mode != des_pb_graph_pin->input_edges[iedge]->interconnect->parent_mode) {
        continue;
      }
      check_pb_graph_edge(*(des_pb_graph_pin->input_edges[iedge]));
      /* Initialize*/
      /* Source pin, node, pb_type*/
      src_pb_graph_pin = des_pb_graph_pin->input_edges[iedge]->input_pins[0];
      src_pb_graph_node = src_pb_graph_pin->parent_node;
      src_pb_type = src_pb_graph_node->pb_type;
      src_pb_type_index = src_pb_graph_node->placement_index;
      /* Des pin, node, pb_type */
      des_pb_graph_node  = des_pb_graph_pin->parent_node;
      des_pb_type = des_pb_graph_node->pb_type;
      des_pb_type_index = des_pb_graph_node->placement_index;
      /* Generate the pin_prefix for src_pb_graph_node and des_pb_graph_node*/
      generate_spice_src_des_pb_graph_pin_prefix(src_pb_graph_node, des_pb_graph_node, pin2pin_interc_type, 
                                                 cur_interc, formatted_parent_pin_prefix, &src_pin_prefix, &des_pin_prefix);
      /* We need to find out if the des_pb_graph_pin is in the mode we want !*/
      /* Print */
      fprintf(fp, "%s->%s[%d] ", 
              src_pin_prefix, src_pb_graph_pin->port->name, src_pb_graph_pin->pin_number);
      /* Free */
      my_free(src_pin_prefix);
      my_free(des_pin_prefix);
      src_pin_prefix = NULL;
      des_pin_prefix = NULL;
    }
    /* Generate the pin_prefix for src_pb_graph_node and des_pb_graph_node*/
    generate_spice_src_des_pb_graph_pin_prefix(src_pb_graph_node, des_pb_graph_node, pin2pin_interc_type, 
                                               cur_interc, formatted_parent_pin_prefix, &src_pin_prefix, &des_pin_prefix);
    /* Outputs */
    fprintf(fp, "%s->%s[%d] ", 
            des_pin_prefix, des_pb_graph_pin->port->name, des_pb_graph_pin->pin_number);

    assert(select_edge < fan_in);
    /* SRAMs */
    switch (cur_interc->spice_model->design_tech_info.structure) {
    case SPICE_MODEL_STRUCTURE_TREE:
      /* 1. Get the mux level*/
      mux_level = determine_tree_mux_level(fan_in);
      /* Get the SRAM configurations*/
      /* Decode the selected_edge_index */
      num_sram_bits = mux_level;
      sram_bits = decode_tree_mux_sram_bits(fan_in, mux_level, select_edge);
      /* Print SRAM configurations, 
       * we should have a global SRAM vdd, AND it should be connected to a real sram subckt !!!
       */
      break;
    case SPICE_MODEL_STRUCTURE_ONELEVEL:
      mux_level = 1;
      /* Special for 2-input MUX */
      if (2 == fan_in) {
        num_sram_bits = 1;
        sram_bits = decode_tree_mux_sram_bits(fan_in, mux_level, select_edge); 
      } else {
        num_sram_bits = fan_in;
        sram_bits = decode_onelevel_mux_sram_bits(fan_in, mux_level, select_edge);
      }
      break;
    case SPICE_MODEL_STRUCTURE_MULTILEVEL:
      mux_level = cur_interc->spice_model->design_tech_info.mux_num_level;
      num_sram_bits = determine_num_input_basis_multilevel_mux(fan_in, mux_level) *mux_level;
      sram_bits = decode_multilevel_mux_sram_bits(fan_in, mux_level, select_edge);
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
                 __FILE__, __LINE__, cur_interc->spice_model->name);
      exit(1);
    } 
    cur_sram = get_sram_orgz_info_num_mem_bit(sram_spice_orgz_info); 
    /* Create wires to sram outputs*/
    for (ilevel = 0; ilevel < num_sram_bits; ilevel++) {
      assert( (0 == sram_bits[ilevel]) || (1 == sram_bits[ilevel]) );
      fprint_spice_sram_one_outport(fp, sram_spice_orgz_info, cur_sram + ilevel, sram_bits[ilevel]);
      fprint_spice_sram_one_outport(fp, sram_spice_orgz_info, cur_sram + ilevel, 1 - sram_bits[ilevel]);
    }
    /* Local vdd and gnd, TODO: we should have an independent VDD for all local interconnections*/
    fprintf(fp, "gvdd_local_interc sgnd ");
    /* End with spice_model name */
    fprintf(fp, "%s_size%d\n", cur_interc->spice_model->name, fan_in);
    /* Print the encoding in SPICE netlist for debugging */
    fprintf(fp, "***** SRAM bits for MUX[%d], level=%d, select_path_id=%d. *****\n", 
            cur_interc->spice_model->cnt, mux_level, select_edge);
    fprintf(fp, "*****");
    for (ilevel = 0; ilevel < num_sram_bits; ilevel++) {
      fprintf(fp, "%d", sram_bits[ilevel]);
    }
    fprintf(fp, "*****\n");
    /* Print all the srams*/
    cur_sram = get_sram_orgz_info_num_mem_bit(sram_spice_orgz_info); 
    /* Give the VDD port name for SRAMs */
    sram_vdd_port_name = (char*)my_malloc(sizeof(char)*
                                         (strlen(spice_tb_global_vdd_localrouting_sram_port_name) 
                                          + 1 ));
    sprintf(sram_vdd_port_name, "%s",
                                spice_tb_global_vdd_localrouting_sram_port_name);
    /* Now Print SRAMs one by one */
    for (ilevel = 0; ilevel < num_sram_bits; ilevel++) {
      fprint_spice_one_sram_subckt(fp, sram_spice_orgz_info, 
                                   cur_interc->spice_model,
                                   sram_vdd_port_name);
    }
    /* Store the configuraion bit to linked-list */
    add_sram_conf_bits_to_llist(sram_spice_orgz_info, cur_sram, 
                                num_sram_bits, sram_bits);
    /* Update spice_model counter */
    cur_interc->spice_model->cnt++;
    /* Free */
    my_free(sram_bits);
    my_free(src_pin_prefix);
    my_free(des_pin_prefix);
    my_free(sram_vdd_port_name);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid interconnection type for %s (Arch[LINE%d])!\n",
               __FILE__, __LINE__, cur_interc->name, cur_interc->line_num);
    exit(1);
  }
  return;
}


/* Print the SPICE interconnections according to pb_graph */
void fprint_spice_pb_graph_interc(FILE* fp, 
                                  char* pin_prefix,
                                  t_pb_graph_node* cur_pb_graph_node,
                                  t_pb* cur_pb,
                                  int select_mode_index,
                                  int is_idle) {
  int iport, ipin;
  int ipb, jpb;
  t_mode* cur_mode = NULL;
  t_pb_type* cur_pb_type = cur_pb_graph_node->pb_type;
  t_pb_graph_node* child_pb_graph_node = NULL;
  t_pb* child_pb = NULL;
  int is_child_pb_idle = 0;
  
  char* formatted_pin_prefix = format_spice_node_prefix(pin_prefix); /* Complete a "_" at the end if needed*/

  int node_index = -1;
  int prev_node = -1;
  int prev_edge = -1;
  int path_id = -1;
  t_rr_node* pb_rr_nodes = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check cur_pb_type*/
  if (NULL == cur_pb_type) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb_type.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  
  /* Assign current mode */
  cur_mode = &(cur_pb_graph_node->pb_type->modes[select_mode_index]);

  /* We check output_pins of cur_pb_graph_node and its the input_edges
   * Built the interconnections between outputs of cur_pb_graph_node and outputs of child_pb_graph_node
   *   child_pb_graph_node.output_pins -----------------> cur_pb_graph_node.outpins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
      /* If this is a idle block, we set 0 to the selected edge*/
      if (is_idle) {
        assert(NULL == cur_pb);
        fprintf_spice_pb_graph_pin_interc(fp,
                                          formatted_pin_prefix, /* parent_pin_prefix */
                                          OUTPUT2OUTPUT_INTERC,
                                          &(cur_pb_graph_node->output_pins[iport][ipin]),
                                          cur_mode,
                                          0);
      } else {
        /* Get the selected edge of current pin*/
        assert(NULL != cur_pb);
        pb_rr_nodes = cur_pb->rr_graph;
        node_index = cur_pb_graph_node->output_pins[iport][ipin].pin_count_in_cluster;
        prev_node = pb_rr_nodes[node_index].prev_node;
        prev_edge = pb_rr_nodes[node_index].prev_edge;
        /* Make sure this pb_rr_node is not OPEN and is not a primitive output*/
        if (OPEN == prev_node) {
          path_id = 0; //
        } else {
          /* Find the path_id */
          path_id = find_path_id_between_pb_rr_nodes(pb_rr_nodes, prev_node, node_index);
          assert(-1 != path_id);
        }
        fprintf_spice_pb_graph_pin_interc(fp,
                                          formatted_pin_prefix, /* parent_pin_prefix */
                                          OUTPUT2OUTPUT_INTERC,
                                          &(cur_pb_graph_node->output_pins[iport][ipin]),
                                          cur_mode,
                                          path_id);
      }
    }
  }
  
  /* We check input_pins of child_pb_graph_node and its the input_edges
   * Built the interconnections between inputs of cur_pb_graph_node and inputs of child_pb_graph_node
   *   cur_pb_graph_node.input_pins -----------------> child_pb_graph_node.input_pins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */ 
  for (ipb = 0; ipb < cur_pb_type->modes[select_mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb_type->modes[select_mode_index].pb_type_children[ipb].num_pb; jpb++) {
      child_pb_graph_node = &(cur_pb_graph_node->child_pb_graph_nodes[select_mode_index][ipb][jpb]);
      /* If this is a idle block, we set 0 to the selected edge*/
      if (is_idle) {
        assert(NULL == cur_pb);
        /* For each child_pb_graph_node input pins*/
        for (iport = 0; iport < child_pb_graph_node->num_input_ports; iport++) {
          for (ipin = 0; ipin < child_pb_graph_node->num_input_pins[iport]; ipin++) {
            fprintf_spice_pb_graph_pin_interc(fp,
                                              formatted_pin_prefix, /* parent_pin_prefix */
                                              INPUT2INPUT_INTERC,
                                              &(child_pb_graph_node->input_pins[iport][ipin]),
                                              cur_mode,
                                              0);
          }
        }
        /* TODO: for clock pins, we should do the same work */
        for (iport = 0; iport < child_pb_graph_node->num_clock_ports; iport++) {
          for (ipin = 0; ipin < child_pb_graph_node->num_clock_pins[iport]; ipin++) {
            fprintf_spice_pb_graph_pin_interc(fp,
                                              formatted_pin_prefix, /* parent_pin_prefix */
                                              INPUT2INPUT_INTERC,
                                              &(child_pb_graph_node->clock_pins[iport][ipin]),
                                              cur_mode,
                                              0);
          }
        }
        continue;
      }
      assert(NULL != cur_pb);
      child_pb = &(cur_pb->child_pbs[ipb][jpb]);
      /* Check if child_pb is empty */
      if (NULL != child_pb->name) { 
        is_child_pb_idle = 0;
      } else {
        is_child_pb_idle = 1;
      }
      /* Get pb_rr_graph of current pb*/
      pb_rr_nodes = child_pb->rr_graph;
      /* For each child_pb_graph_node input pins*/
      for (iport = 0; iport < child_pb_graph_node->num_input_ports; iport++) {
        for (ipin = 0; ipin < child_pb_graph_node->num_input_pins[iport]; ipin++) {
          if (is_child_pb_idle) {
            prev_edge = 0;
          } else {
            /* Get the index of the edge that are selected to pass signal*/
            node_index = child_pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster;
            prev_node = pb_rr_nodes[node_index].prev_node;
            prev_edge = pb_rr_nodes[node_index].prev_edge;
            /* Make sure this pb_rr_node is not OPEN and is not a primitive output*/
            if (OPEN == prev_node) {
              path_id = 0; //
            } else {
              /* Find the path_id */
              path_id = find_path_id_between_pb_rr_nodes(pb_rr_nodes, prev_node, node_index);
              assert(-1 != path_id);
            }
          }
          /* Write the interconnection*/
          fprintf_spice_pb_graph_pin_interc(fp,
                                            formatted_pin_prefix, /* parent_pin_prefix */
                                            INPUT2INPUT_INTERC,
                                            &(child_pb_graph_node->input_pins[iport][ipin]),
                                            cur_mode,
                                            path_id);
        }
      }
      /* TODO: for clock pins, we should do the same work */
      for (iport = 0; iport < child_pb_graph_node->num_clock_ports; iport++) {
        for (ipin = 0; ipin < child_pb_graph_node->num_clock_pins[iport]; ipin++) {
          if (is_child_pb_idle) {
            prev_edge = 0;
          } else {
            /* Get the index of the edge that are selected to pass signal*/
            node_index = child_pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster;
            prev_node = pb_rr_nodes[node_index].prev_node;
            prev_edge = pb_rr_nodes[node_index].prev_edge;
            /* Make sure this pb_rr_node is not OPEN and is not a primitive output*/
            if (OPEN == prev_node) {
              path_id = 0; //
            } else {
              /* Find the path_id */
              path_id = find_path_id_between_pb_rr_nodes(pb_rr_nodes, prev_node, node_index);
              assert(-1 != path_id);
            }
          }
          /* Write the interconnection*/
          fprintf_spice_pb_graph_pin_interc(fp,
                                            formatted_pin_prefix, /* parent_pin_prefix */
                                            INPUT2INPUT_INTERC,
                                            &(child_pb_graph_node->clock_pins[iport][ipin]),
                                            cur_mode,
                                            path_id);
        }
      }
    }
  }

  return; 
}

/* Print the netlist for primitive pb_types*/
void fprint_spice_pb_graph_primitive_node(FILE* fp,
                                          char* subckt_prefix,
                                          t_pb* cur_pb,
                                          t_pb_graph_node* cur_pb_graph_node,
                                          int pb_type_index) {
  int iport, ipin;
  t_pb_type* cur_pb_type = NULL;
  char* formatted_subckt_prefix = format_spice_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  t_spice_model* spice_model = NULL;
  char* subckt_name = NULL;
  char* subckt_port_name = NULL;
  
  int num_spice_model_sram_port = 0;
  t_spice_model_port** spice_model_sram_ports = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check cur_pb_graph_node*/
  if (NULL == cur_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb_graph_node.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  spice_model = cur_pb_type->spice_model;
  /* If we define a SPICE model for the pb_type,
   * We should print the subckt of it. 
   * 1. If the SPICE model defines an included netlist, we quote the netlist subckt 
   * 2. If not defined an included netlist, we built one only if this is a LUT
   */
  /* Example: <prefix><cur_pb_type_name>[<index>]*/
  subckt_name = (char*)my_malloc(sizeof(char)*
                (strlen(formatted_subckt_prefix) 
                + strlen(cur_pb_type->name) + 1
                + strlen(my_itoa(pb_type_index)) + 1 + 1)); /* Plus the '0' at the end of string*/
  sprintf(subckt_name, "%s%s[%d]", formatted_subckt_prefix, cur_pb_type->name, pb_type_index);
  /* Check if defines an included netlist*/
  if (NULL == spice_model->model_netlist) {
    if (LUT_CLASS == cur_pb_type->class_type) {
      /* For LUT, we have a built-in netlist, See "spice_lut.c", So we don't do anything here */
    } else {
      /* We report an error */
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Require an included netlist in Pb_type(%s) SPICE_model(%s)!\n",
                __FILE__, __LINE__, cur_pb_type->name, spice_model->name);
      exit(1);
    }
  }
  /* Print the definition line 
   * IMPORTANT: NO SRAMs ports are created here, they are fixed when quoting spice_models
   */
  fprintf(fp, ".subckt %s ", subckt_name);
  subckt_port_name = format_spice_node_prefix(subckt_name); 
  /* Inputs, outputs, inouts, clocks */
  fprint_pb_type_ports(fp, subckt_port_name, 0, cur_pb_type);
  /* Finish with local vdd and gnd */
  fprintf(fp, "svdd sgnd\n");
  /* Include the spice_model*/
  fprintf(fp, "X%s[%d] ", spice_model->prefix, spice_model->cnt); 
  spice_model->cnt++; /* Stats the number of spice_model used*/
  /* Make input, output, inout, clocks connected*/
  /* IMPORTANT: (sequence of these ports should be changed!) */
  fprint_pb_type_ports(fp, subckt_name, 0, cur_pb_type);
  /* Connected to SRAMs */
  /* Configure the SRAMs to be idle*/
  spice_model_sram_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_SRAM, &num_spice_model_sram_port, TRUE); 
  for (iport = 0; iport < num_spice_model_sram_port; iport++) {
    for(ipin = 0; ipin < spice_model_sram_ports[iport]->size; ipin++) {
      fprintf(fp, "sgnd ");
    }
  }
  /* Print the spice_model name defined */
  fprintf(fp, "%s\n", spice_model->name);
  /* Print end of subckt*/
  fprintf(fp, ".eom\n");
  /* Free */
  my_free(subckt_name);
  
  return;
}

/* Print the subckt of a primitive pb */
void fprint_pb_primitive_spice_model(FILE* fp,
                                     char* subckt_prefix,
                                     t_pb* prim_pb,
                                     t_pb_graph_node* prim_pb_graph_node,
                                     int pb_index,
                                     t_spice_model* spice_model,
                                     int is_idle) {
  t_pb_type* prim_pb_type = NULL;
  t_logical_block* mapped_logical_block = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check cur_pb_graph_node*/
  if (NULL == prim_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb_graph_node.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
 
  /* Initialize */ 
  prim_pb_type = prim_pb_graph_node->pb_type;
  if (is_idle) {
    mapped_logical_block = NULL;
  } else {
    mapped_logical_block = &logical_block[prim_pb->logical_block];
  }

  /* Asserts*/
  assert(pb_index == prim_pb_graph_node->placement_index);
  assert(0 == strcmp(spice_model->name, prim_pb_type->spice_model->name));

  if (is_idle) {
    assert(NULL == prim_pb); 
  } else {
    if (NULL == prim_pb) {
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid prim_pb.\n", 
                 __FILE__, __LINE__); 
      exit(1);
    }
  }

  /* According to different type, we print netlist*/
  switch (spice_model->type) {
  case SPICE_MODEL_LUT:
    /* If this is a idle block we should set sram_bits to zero*/
    fprint_pb_primitive_lut(fp, subckt_prefix, mapped_logical_block, prim_pb_graph_node,
                            pb_index, spice_model);
    break;
  case SPICE_MODEL_FF:
    assert(NULL != spice_model->model_netlist);
    /* TODO : We should learn trigger type and initial value!!! and how to apply them!!! */
    fprint_pb_primitive_ff(fp, subckt_prefix, mapped_logical_block, prim_pb_graph_node,
                           pb_index, spice_model);
    break;
  case SPICE_MODEL_IOPAD:
    assert(NULL != spice_model->model_netlist);
    fprint_pb_primitive_io(fp, subckt_prefix, mapped_logical_block, prim_pb_graph_node,
                           pb_index, spice_model);
    break;
  case SPICE_MODEL_HARDLOGIC:
    assert(NULL != spice_model->model_netlist);
    fprint_pb_primitive_hardlogic(fp, subckt_prefix, mapped_logical_block, prim_pb_graph_node,
                                  pb_index, spice_model);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of spice_model(%s), should be [LUT|FF|HARD_LOGIC|IO]!\n",
               __FILE__, __LINE__, spice_model->name);
    exit(1);
    break;
  }
 
  return; 
}

/* Print idle pb_types recursively
 * search the idle_mode until we reach the leaf node
 */
void fprint_spice_idle_pb_graph_node_rec(FILE* fp,
                                         char* subckt_prefix,
                                         t_pb_graph_node* cur_pb_graph_node,
                                         int pb_type_index) {
  int mode_index, ipb, jpb, child_mode_index;
  t_pb_type* cur_pb_type = NULL;
  char* subckt_name = NULL;
  char* formatted_subckt_prefix = format_spice_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  char* pass_on_prefix = NULL;
  char* child_pb_type_prefix = NULL;
  char* subckt_port_prefix = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check cur_pb_graph_node*/
  if (NULL == cur_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb_graph_node.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  cur_pb_type = cur_pb_graph_node->pb_type;

  /* Recursively finish all the child pb_types*/
  if (NULL == cur_pb_type->spice_model) { 
    /* Find the mode that define_idle_mode*/
    mode_index = find_pb_type_idle_mode_index((*cur_pb_type));
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
        /* Pass the SPICE mode prefix on, 
         * <subckt_name>mode[<mode_name>]_
         */
        pass_on_prefix = (char*)my_malloc(sizeof(char)*
                           (strlen(formatted_subckt_prefix) + strlen(cur_pb_type->name) + 1 
                            + strlen(my_itoa(pb_type_index)) + 7 + strlen(cur_pb_type->modes[mode_index].name) + 1 + 1 + 1));
        sprintf(pass_on_prefix, "%s%s[%d]_mode[%s]_", 
                formatted_subckt_prefix, cur_pb_type->name, pb_type_index, cur_pb_type->modes[mode_index].name);
        /* Recursive*/
        fprint_spice_idle_pb_graph_node_rec(fp, pass_on_prefix,
                                            &(cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]), jpb);
        /* Free */
        my_free(pass_on_prefix);
      }
    }
  }

  /* Check if this has defined a spice_model*/
  if (NULL != cur_pb_type->spice_model) {
    fprint_pb_primitive_spice_model(fp, formatted_subckt_prefix, 
                                    NULL, cur_pb_graph_node, 
                                    pb_type_index, cur_pb_type->spice_model, 1);
    /* Finish the primitive node, we return */
    return;
  }

  /* Find the mode that define_idle_mode*/
  mode_index = find_pb_type_idle_mode_index((*cur_pb_type));
  /* Create a new subckt */
  /* <formatted_subckt_prefix>mode[<mode_name>]
   */
  subckt_name = (char*)my_malloc(sizeof(char)*
                (strlen(formatted_subckt_prefix) + strlen(cur_pb_type->name) + 1 
                + strlen(my_itoa(pb_type_index)) + 7 + strlen(cur_pb_type->modes[mode_index].name) + 1 + 1)); 
  /* Definition*/
  sprintf(subckt_name, "%s%s[%d]_mode[%s]", 
          formatted_subckt_prefix, cur_pb_type->name, pb_type_index, cur_pb_type->modes[mode_index].name);
  fprintf(fp, ".subckt %s ", subckt_name);
  /* Inputs, outputs, inouts, clocks */
  subckt_port_prefix = (char*)my_malloc(sizeof(char)*
                                       (5 + strlen(cur_pb_type->modes[mode_index].name) + 1 + 1));
  sprintf(subckt_port_prefix, "mode[%s]", cur_pb_type->modes[mode_index].name);
  /*
  fprint_pb_type_ports(fp, subckt_name, 0, cur_pb_type);
  */
  /* Simplify the port prefix, make SPICE netlist readable */
  fprint_pb_type_ports(fp, subckt_port_prefix, 0, cur_pb_type);
  /* Finish with local vdd and gnd */
  fprintf(fp, "svdd sgnd\n");
  /* Definition ends*/

  /* Specify the head of scan-chain */
  if (SPICE_SRAM_SCAN_CHAIN == sram_spice_orgz_type) {
    fprintf(fp, "***** Head of scan-chain *****\n");
    fprintf(fp, "R%s_sc_head %s_sc_head %s[%d]->in 0\n",
            subckt_name, subckt_name, sram_spice_model->prefix, sram_spice_model->cnt);
  }

  /* Quote all child pb_types */
  for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    /* Each child may exist multiple times in the hierarchy*/
    for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* we should make sure this placement index == child_pb_type[jpb]*/
      assert(jpb == cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb].placement_index);
      /* <formatted_subckt_prefix>mode[<mode_name>]_<child_pb_type_name>[<ipb>]
       */
      fprintf(fp, "X%s[%d] ", cur_pb_type->modes[mode_index].pb_type_children[ipb].name, jpb);
      /* Pass the SPICE mode prefix on, 
       * <subckt_name>mode[<mode_name>]_<child_pb_type_name>[<jpb>]
       * <child_pb_type_name>[<jpb>]
       */
      /*
      child_pb_type_prefix = (char*)my_malloc(sizeof(char)*
                               (strlen(subckt_name) + 1 
                                + strlen(cur_pb_type->modes[imode].pb_type_children[ipb].name) + 1 
                                + strlen(my_itoa(jpb)) + 1 + 1));
      sprintf(child_pb_type_prefix, "%s_%s[%d]", subckt_name,
      cur_pb_type->modes[imode].pb_type_children[ipb].name, jpb);
      */
      /* Simplify the prefix! */
      child_pb_type_prefix = (char*)my_malloc(sizeof(char)* 
                                (strlen(cur_pb_type->modes[mode_index].pb_type_children[ipb].name) + 1 
                                  + strlen(my_itoa(jpb)) + 1 + 1));
      sprintf(child_pb_type_prefix, "%s[%d]",
               cur_pb_type->modes[mode_index].pb_type_children[ipb].name, jpb);
      /* Print inputs, outputs, inouts, clocks
        * NO SRAMs !!! They have already been fixed in the bottom level
        */
      fprint_pb_type_ports(fp, child_pb_type_prefix, 0, &(cur_pb_type->modes[mode_index].pb_type_children[ipb]));
      fprintf(fp, "svdd sgnd "); /* Local vdd and gnd*/

      /* Find the pb_type_children mode */
      if (NULL == cur_pb_type->modes[mode_index].pb_type_children[ipb].spice_model) { /* Find the idle_mode_index, if this is not a leaf node  */
         child_mode_index = find_pb_type_idle_mode_index(cur_pb_type->modes[mode_index].pb_type_children[ipb]);
      }
      /* If the pb_type_children is a leaf node, we don't use the mode to name it,
       * else we can use the mode to name it 
        */
      if (NULL == cur_pb_type->modes[mode_index].pb_type_children[ipb].spice_model) { /* Not a leaf node*/
        fprintf(fp, "%s_%s[%d]_mode[%s]\n",
                 subckt_name, cur_pb_type->modes[mode_index].pb_type_children[ipb].name, jpb, 
                cur_pb_type->modes[mode_index].pb_type_children[ipb].modes[child_mode_index].name);
      } else { /* Have a spice model definition, this is a leaf node*/
        fprintf(fp, "%s_%s[%d]\n",
                subckt_name, cur_pb_type->modes[mode_index].pb_type_children[ipb].name, jpb); 
      }
      my_free(child_pb_type_prefix);
    }
  }
  /* Print interconnections, set is_idle as TRUE*/
  fprint_spice_pb_graph_interc(fp, subckt_name, cur_pb_graph_node, NULL, mode_index, 1);
  /* Check each pins of pb_graph_node */ 

  /* Specify the Tail of scan-chain */
  if (SPICE_SRAM_SCAN_CHAIN == sram_spice_orgz_type) {
    fprintf(fp, "***** Tail of scan-chain *****\n");
    fprintf(fp, "R%s_sc_tail %s_sc_tail %s[%d]->in 0\n",
            subckt_name, subckt_name, sram_spice_model->prefix, sram_spice_model->cnt);
  }

  /* End the subckt */
  fprintf(fp, ".eom\n");
  /* Free subckt name*/
  my_free(subckt_name);

  return;
}

/* Print SPICE netlist for each pb and corresponding pb_graph_node*/
void fprint_spice_pb_graph_node_rec(FILE* fp, 
                                    char* subckt_prefix, 
                                    t_pb* cur_pb, 
                                    t_pb_graph_node* cur_pb_graph_node,
                                    int pb_type_index) {
  int mode_index, ipb, jpb, child_mode_index;
  t_pb_type* cur_pb_type = NULL;
  char* subckt_name = NULL;
  char* formatted_subckt_prefix = format_spice_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  char* pass_on_prefix = NULL;
  char* child_pb_type_prefix = NULL;

  char* subckt_port_prefix = NULL;
  t_pb* child_pb = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check cur_pb_graph_node*/
  if (NULL == cur_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb_graph_node.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  cur_pb_type = cur_pb_graph_node->pb_type;
  mode_index = cur_pb->mode; 

  /* Recursively finish all the child pb_types*/
  if (NULL == cur_pb_type->spice_model) { 
    /* recursive for the child_pbs*/
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
        /* Pass the SPICE mode prefix on, 
         * <subckt_name><pb_type_name>[<pb_index>]_mode[<mode_name>]_
         */
        pass_on_prefix = (char*)my_malloc(sizeof(char)*
                          (strlen(formatted_subckt_prefix) + strlen(cur_pb_type->name) + 1 
                           + strlen(my_itoa(pb_type_index)) + 7 + strlen(cur_pb_type->modes[mode_index].name) + 1 + 1 + 1));
        sprintf(pass_on_prefix, "%s%s[%d]_mode[%s]_", 
                formatted_subckt_prefix, cur_pb_type->name, pb_type_index, cur_pb_type->modes[mode_index].name);
        /* Recursive*/
        /* Refer to pack/output_clustering.c [LINE 392] */
        if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
          fprint_spice_pb_graph_node_rec(fp, pass_on_prefix, &(cur_pb->child_pbs[ipb][jpb]), 
                                         cur_pb->child_pbs[ipb][jpb].pb_graph_node, jpb);
        } else {
          /* Check if this pb has no children, no children mean idle*/
          fprint_spice_idle_pb_graph_node_rec(fp, pass_on_prefix,
                                            cur_pb->child_pbs[ipb][jpb].pb_graph_node, jpb);
        }
        /* Free */
        my_free(pass_on_prefix);
      }
    }
  }

  /* Check if this has defined a spice_model*/
  if (NULL != cur_pb_type->spice_model) {
    switch (cur_pb_type->class_type) {
    case LUT_CLASS: 
      /* Special care for LUT !!!
       * Mapped logical block information is stored in child_pbs
       */
      child_pb = get_lut_child_pb(cur_pb, mode_index); 
      fprint_pb_primitive_spice_model(fp, formatted_subckt_prefix, 
                                      child_pb, cur_pb_graph_node, 
                                      pb_type_index, cur_pb_type->spice_model, 0);
      break;
    case LATCH_CLASS:
      assert(0 == cur_pb_type->num_modes);
      /* Consider the num_pb, create all the subckts*/
      fprint_pb_primitive_spice_model(fp, formatted_subckt_prefix, 
                                      cur_pb, cur_pb_graph_node, 
                                      pb_type_index, cur_pb_type->spice_model, 0);
      break;
    case MEMORY_CLASS:
      child_pb = get_hardlogic_child_pb(cur_pb, mode_index); 
      /* Consider the num_pb, create all the subckts*/
      fprint_pb_primitive_spice_model(fp, formatted_subckt_prefix, 
                                      child_pb, cur_pb_graph_node, 
                                      pb_type_index, cur_pb_type->spice_model, 0);
      break; 
    case UNKNOWN_CLASS:
      /* Consider the num_pb, create all the subckts*/
      fprint_pb_primitive_spice_model(fp, formatted_subckt_prefix, 
                                      cur_pb, cur_pb_graph_node, 
                                      pb_type_index, cur_pb_type->spice_model, 0);
      break; 
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Unknown class type of pb_type(%s)!\n",
                 __FILE__, __LINE__, cur_pb_type->name);
      exit(1);
    }
    /* Finish for primitive node, return  */
    return;
  }

  /* Create a new subckt */
  /* <formatted_subckt_prefix>mode[<mode_name>]
   */
  subckt_name = (char*)my_malloc(sizeof(char)*
                (strlen(formatted_subckt_prefix) + strlen(cur_pb_type->name) + 1 
                 + strlen(my_itoa(pb_type_index)) + 7 + strlen(cur_pb_type->modes[mode_index].name) + 1 + 1)); 
  /* Definition*/
  sprintf(subckt_name, "%s%s[%d]_mode[%s]", 
          formatted_subckt_prefix, cur_pb_type->name, pb_type_index, cur_pb_type->modes[mode_index].name);
  fprintf(fp, ".subckt %s ", subckt_name);
  /* Inputs, outputs, inouts, clocks */
  subckt_port_prefix = (char*)my_malloc(sizeof(char)*
                        (5 + strlen(cur_pb_type->modes[mode_index].name) + 1 + 1));
  sprintf(subckt_port_prefix, "mode[%s]", cur_pb_type->modes[mode_index].name);
  /*
  fprint_pb_type_ports(fp, subckt_name, 0, cur_pb_type);
  */
  /* Simplify the prefix! Make the SPICE netlist readable*/
  fprint_pb_type_ports(fp, subckt_port_prefix, 0, cur_pb_type);
  /* Finish with local vdd and gnd */
  fprintf(fp, "svdd sgnd\n");
  /* Definition ends*/
  /* Quote all child pb_types */
  for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    /* Each child may exist multiple times in the hierarchy*/
    for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* we should make sure this placement index == child_pb_type[jpb]*/
      assert(jpb == cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb].placement_index);
      /* <formatted_subckt_prefix>mode[<mode_name>]_<child_pb_type_name>[<ipb>]
       */
      fprintf(fp, "X%s[%d] ", cur_pb_type->modes[mode_index].pb_type_children[ipb].name, jpb);
      /* Pass the SPICE mode prefix on, 
       * <subckt_name>mode[<mode_name>]_<child_pb_type_name>[<jpb>]
       */
      /*
      child_pb_type_prefix = (char*)my_malloc(sizeof(char)*
                             (strlen(subckt_name) + 1 
                              + strlen(cur_pb_type->modes[mode_index].pb_type_children[ipb].name) + 1 
                              + strlen(my_itoa(jpb)) + 1 + 1));
      sprintf(child_pb_type_prefix, "%s_%s[%d]", subckt_name,
              cur_pb_type->modes[mode_index].pb_type_children[ipb].name, jpb);
      */
      /* Simplify the prefix! Make the SPICE netlist readable*/
      child_pb_type_prefix = (char*)my_malloc(sizeof(char)*
                             (strlen(cur_pb_type->modes[mode_index].pb_type_children[ipb].name) + 1 
                              + strlen(my_itoa(jpb)) + 1 + 1));
      sprintf(child_pb_type_prefix, "%s[%d]", 
               cur_pb_type->modes[mode_index].pb_type_children[ipb].name, jpb);
      /* Print inputs, outputs, inouts, clocks
       * NO SRAMs !!! They have already been fixed in the bottom level
       */
      fprint_pb_type_ports(fp, child_pb_type_prefix, 0, &(cur_pb_type->modes[mode_index].pb_type_children[ipb]));
      fprintf(fp, "svdd sgnd "); /* Local vdd and gnd*/
      /* Find the pb_type_children mode */
      if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
        child_mode_index = cur_pb->child_pbs[ipb][jpb].mode; 
      } else if (NULL == cur_pb_type->modes[mode_index].pb_type_children[ipb].spice_model) { /* Find the idle_mode_index, if this is not a leaf node  */
         child_mode_index = find_pb_type_idle_mode_index(cur_pb_type->modes[mode_index].pb_type_children[ipb]);
      }
      /* If the pb_type_children is a leaf node, we don't use the mode to name it,
       * else we can use the mode to name it 
       */
      if (NULL == cur_pb_type->modes[mode_index].pb_type_children[ipb].spice_model) { /* Not a leaf node*/
        fprintf(fp, "%s_%s[%d]_mode[%s]\n",
                subckt_name, cur_pb_type->modes[mode_index].pb_type_children[ipb].name, jpb, 
                cur_pb_type->modes[mode_index].pb_type_children[ipb].modes[child_mode_index].name);
      } else { /* Have a spice model definition, this is a leaf node*/
        fprintf(fp, "%s_%s[%d]\n",
                subckt_name, cur_pb_type->modes[mode_index].pb_type_children[ipb].name, jpb); 
      }
      my_free(child_pb_type_prefix);
    }
  }
  /* Print interconnections, set is_idle as TRUE*/
  fprint_spice_pb_graph_interc(fp, subckt_name, cur_pb_graph_node, cur_pb, mode_index, 0);
  /* Check each pins of pb_graph_node */ 
  /* End the subckt */
  fprintf(fp, ".eom\n");
  /* Free subckt name*/
  my_free(subckt_name);

  return;
}

/* Print SPICE netlist for each pb and corresponding pb_graph_node 
 * at physical-level implementation */
void fprint_spice_phy_pb_graph_node_rec(FILE* fp, 
                                        char* subckt_prefix, 
                                        t_pb* cur_pb, 
                                        t_pb_graph_node* cur_pb_graph_node,
                                        int pb_type_index) {
  int mode_index, ipb, jpb, child_mode_index, is_idle;
  t_pb_type* cur_pb_type = NULL;
  char* subckt_name = NULL;
  char* formatted_subckt_prefix = format_spice_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  char* pass_on_prefix = NULL;
  char* child_pb_type_prefix = NULL;

  char* subckt_port_prefix = NULL;
  t_pb* child_pb = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check cur_pb_graph_node*/
  if (NULL == cur_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb_graph_node.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  cur_pb_type = cur_pb_graph_node->pb_type;

  is_idle = 1;
  if (NULL != cur_pb) {
    is_idle = 0;
  }

  /* Recursively finish all the child pb_types*/
  if (NULL == cur_pb_type->spice_model) { 
    /* Find the mode that define_idle_mode*/
    mode_index = find_pb_type_physical_mode_index((*cur_pb_type));
    /* recursive for the child_pbs*/
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
        /* Pass the SPICE mode prefix on, 
         * <subckt_name><pb_type_name>[<pb_index>]_mode[<mode_name>]_
         */
        pass_on_prefix = (char*)my_malloc(sizeof(char)*
                          (strlen(formatted_subckt_prefix) + strlen(cur_pb_type->name) + 1 
                           + strlen(my_itoa(pb_type_index)) + 7 + strlen(cur_pb_type->modes[mode_index].name) + 1 + 1 + 1));
        sprintf(pass_on_prefix, "%s%s[%d]_mode[%s]_", 
                formatted_subckt_prefix, cur_pb_type->name, pb_type_index, cur_pb_type->modes[mode_index].name);
        /* Recursive*/
        /* Refer to pack/output_clustering.c [LINE 392] */
        /* Find the child pb that is mapped, and the mapping info is not stored in the physical mode ! */
        child_pb = get_child_pb_for_phy_pb_graph_node(cur_pb, ipb, jpb);
        fprint_spice_phy_pb_graph_node_rec(fp, pass_on_prefix, child_pb, 
                                           &(cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]), jpb);
        /* Free */
        my_free(pass_on_prefix);
      }
    }
  }

  /* Check if this has defined a spice_model*/
  if (NULL != cur_pb_type->spice_model) {
    switch (cur_pb_type->class_type) {
    case LUT_CLASS: 
      if (1 == is_idle) {
        fprint_pb_primitive_spice_model(fp, formatted_subckt_prefix, 
                                        NULL, cur_pb_graph_node, 
                                        pb_type_index, cur_pb_type->spice_model, is_idle);
      } else {
        child_pb = get_lut_child_pb(cur_pb, mode_index); 
        /* Special care for LUT !!!
         * Mapped logical block information is stored in child_pbs
         */
        fprint_pb_primitive_spice_model(fp, formatted_subckt_prefix, 
                                        child_pb, cur_pb_graph_node, 
                                        pb_type_index, cur_pb_type->spice_model, is_idle);
      }
      break;
    case LATCH_CLASS:
      assert(0 == cur_pb_type->num_modes);
      /* Consider the num_pb, create all the subckts*/
      fprint_pb_primitive_spice_model(fp, formatted_subckt_prefix, 
                                      cur_pb, cur_pb_graph_node, 
                                      pb_type_index, cur_pb_type->spice_model, is_idle);
      break;
    case UNKNOWN_CLASS:
    case MEMORY_CLASS:
      /* Consider the num_pb, create all the subckts*/
      fprint_pb_primitive_spice_model(fp, formatted_subckt_prefix, 
                                      cur_pb, cur_pb_graph_node, 
                                      pb_type_index, cur_pb_type->spice_model, is_idle);
      break; 
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Unknown class type of pb_type(%s)!\n",
                 __FILE__, __LINE__, cur_pb_type->name);
      exit(1);
    }
    /* Finish for primitive node, return  */
    return;
  }

  /* Find the mode that define_idle_mode*/
  mode_index = find_pb_type_physical_mode_index((*cur_pb_type));
  /* Create a new subckt */
  /* <formatted_subckt_prefix>mode[<mode_name>]
   */
  subckt_name = (char*)my_malloc(sizeof(char)*
                (strlen(formatted_subckt_prefix) + strlen(cur_pb_type->name) + 1 
                 + strlen(my_itoa(pb_type_index)) + 7 + strlen(cur_pb_type->modes[mode_index].name) + 1 + 1)); 
  /* Definition*/
  sprintf(subckt_name, "%s%s[%d]_mode[%s]", 
          formatted_subckt_prefix, cur_pb_type->name, pb_type_index, cur_pb_type->modes[mode_index].name);
  fprintf(fp, ".subckt %s ", subckt_name);
  /* Inputs, outputs, inouts, clocks */
  subckt_port_prefix = (char*)my_malloc(sizeof(char)*
                        (5 + strlen(cur_pb_type->modes[mode_index].name) + 1 + 1));
  sprintf(subckt_port_prefix, "mode[%s]", cur_pb_type->modes[mode_index].name);
  /*
  fprint_pb_type_ports(fp, subckt_name, 0, cur_pb_type);
  */
  /* Simplify the prefix! Make the SPICE netlist readable*/
  fprint_pb_type_ports(fp, subckt_port_prefix, 0, cur_pb_type);
  /* Finish with local vdd and gnd */
  fprintf(fp, "svdd sgnd\n");
  /* Definition ends*/
  /* Quote all child pb_types */
  for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    /* Each child may exist multiple times in the hierarchy*/
    for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* we should make sure this placement index == child_pb_type[jpb]*/
      assert(jpb == cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb].placement_index);
      /* If the pb_type_children is a leaf node, we don't use the mode to name it,
       * else we can use the mode to name it 
       */
      fprintf(fp, "X%s[%d] ", cur_pb_type->modes[mode_index].pb_type_children[ipb].name, jpb);
      /* Simplify the prefix! Make the SPICE netlist readable*/
      child_pb_type_prefix = (char*)my_malloc(sizeof(char)*
                             (strlen(cur_pb_type->modes[mode_index].pb_type_children[ipb].name) + 1 
                              + strlen(my_itoa(jpb)) + 1 + 1));
      sprintf(child_pb_type_prefix, "%s[%d]", 
              cur_pb_type->modes[mode_index].pb_type_children[ipb].name, jpb);
      /* Print inputs, outputs, inouts, clocks
       * NO SRAMs !!! They have already been fixed in the bottom level
       */
      fprint_pb_type_ports(fp, child_pb_type_prefix, 0, &(cur_pb_type->modes[mode_index].pb_type_children[ipb]));
      fprintf(fp, "svdd sgnd "); /* Local vdd and gnd*/
      /* Find the pb_type_children mode */
      /* If the pb_type_children is a leaf node, we don't use the mode to name it,
       * else we can use the mode to name it 
       */
      if (NULL == cur_pb_type->modes[mode_index].pb_type_children[ipb].spice_model) { /* Not a leaf node*/
        child_mode_index = find_pb_type_idle_mode_index(cur_pb_type->modes[mode_index].pb_type_children[ipb]);
        fprintf(fp, "%s_%s[%d]_mode[%s]\n",
                subckt_name, cur_pb_type->modes[mode_index].pb_type_children[ipb].name, jpb, 
                cur_pb_type->modes[mode_index].pb_type_children[ipb].modes[child_mode_index].name);
      } else { /* Have a spice model definition, this is a leaf node*/
        fprintf(fp, "%s_%s[%d]\n",
                subckt_name, cur_pb_type->modes[mode_index].pb_type_children[ipb].name, jpb); 
      }
      my_free(child_pb_type_prefix);
    }
  }
  /* Print interconnections, set is_idle as TRUE*/
  fprint_spice_pb_graph_interc(fp, subckt_name, cur_pb_graph_node, cur_pb, mode_index, is_idle);
  /* Check each pins of pb_graph_node */ 
  /* End the subckt */
  fprintf(fp, ".eom\n");
  /* Free subckt name*/
  my_free(subckt_name);

  return;
}


/* Print the SPICE netlist of a block that has been mapped */
void fprint_spice_block(FILE* fp,
                        char* subckt_name, 
                        int x,
                        int y,
                        int z,
                        t_type_ptr type_descriptor,
                        t_block* mapped_block) {
  t_pb* top_pb = NULL; 
  t_pb_graph_node* top_pb_graph_node = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* check */
  assert(x == mapped_block->x);
  assert(y == mapped_block->y);
  assert(type_descriptor == mapped_block->type);
  assert(NULL != type_descriptor);

  /* Print SPICE netlist according to the hierachy of type descriptor recursively*/
  /* Go for the pb_types*/
  top_pb_graph_node = type_descriptor->pb_graph_head;
  assert(NULL != top_pb_graph_node);
  top_pb = mapped_block->pb; 
  assert(NULL != top_pb);

  /* Recursively find all mode and print netlist*/
  /* IMPORTANT: type_descriptor just say we have a block that in global view, how it connects to global routing arch.
   * Inside the type_descripor, there is a top_pb_graph_node(pb_graph_head), describe the top pb_type defined.
   * The index of such top pb_type is always 0. 
   */
  fprint_spice_pb_graph_node_rec(fp, subckt_name, top_pb, top_pb_graph_node, z);

  return;
}


/* Print an idle logic block
 * Find the idle_mode in arch files,
 * And print the spice netlist into file
 */
void fprint_spice_idle_block(FILE* fp,
                             char* subckt_name, 
                             int x,
                             int y,
                             int z,
                             t_type_ptr type_descriptor) {
  t_pb_graph_node* top_pb_graph_node = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Ensure we have a valid type_descriptor*/ 
  assert(NULL != type_descriptor);

  /* Go for the pb_types*/
  top_pb_graph_node = type_descriptor->pb_graph_head;
  assert(NULL != top_pb_graph_node);

  /* Recursively find all idle mode and print netlist*/
  fprint_spice_idle_pb_graph_node_rec(fp, subckt_name, top_pb_graph_node, z);

  return;
}

/* Print the SPICE netlist of a block that has been mapped */
void fprint_spice_physical_block(FILE* fp,
                                 char* subckt_name, 
                                 int x,
                                 int y,
                                 int z,
                                 t_type_ptr type_descriptor) {
  t_pb* top_pb = NULL; 
  t_pb_graph_node* top_pb_graph_node = NULL;
  t_block* mapped_block = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* check */
  assert(NULL != type_descriptor);
  top_pb_graph_node = type_descriptor->pb_graph_head;
  assert(NULL != top_pb_graph_node);

  /* Print SPICE netlist according to the hierachy of type descriptor recursively*/

  mapped_block = search_mapped_block(x, y, z); 
  /* Go for the pb_types*/
  if (NULL != mapped_block) {
    top_pb = mapped_block->pb; 
    assert(NULL != top_pb);
  }

  /* Recursively find all mode and print netlist*/
  /* IMPORTANT: type_descriptor just say we have a block that in global view, how it connects to global routing arch.
   * Inside the type_descripor, there is a top_pb_graph_node(pb_graph_head), describe the top pb_type defined.
   * The index of such top pb_type is always 0. 
   */
  fprint_spice_phy_pb_graph_node_rec(fp, subckt_name, top_pb, top_pb_graph_node, z);

  return;
}

/* We print all the pins of a type descriptor in the following sequence 
 * TOP, RIGHT, BOTTOM, LEFT
 */
void fprint_grid_pins(FILE* fp,
                      int x,
                      int y,
                      int top_level) {
  int iheight, side, ipin; 
  int side_pin_index;
  t_type_ptr type_descriptor = grid[x][y].type;
  int capacity = grid[x][y].type->capacity;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  assert(NULL != type_descriptor);
  assert(0 < capacity);

  for (side = 0; side < 4; side++) {
    /* Count the number of pins */
    side_pin_index = 0;
    //for (iz = 0; iz < capacity; iz++) {
      for (iheight = 0; iheight < type_descriptor->height; iheight++) {
        for (ipin = 0; ipin < type_descriptor->num_pins; ipin++) {
          if (1 == type_descriptor->pinloc[iheight][side][ipin]) {
            /* This pin appear at this side! */
            if (1 == top_level) {
              fprintf(fp, "+ grid[%d][%d]_pin[%d][%d][%d] \n", x, y,
                      iheight, side, ipin);
            } else {
              fprintf(fp, "+ %s_height[%d]_pin[%d] \n", 
                      convert_side_index_to_string(side), iheight, ipin);
            }
            side_pin_index++;
          }
        }
      }  
    //}
  }

  return;
} 

/* Special for I/O grid, we need only part of the ports
 * i.e., grid[0][0..ny] only need the right side ports.
 */
/* We print all the pins of a type descriptor in the following sequence 
 * TOP, RIGHT, BOTTOM, LEFT
 */
void fprint_io_grid_pins(FILE* fp,
                         int x,
                         int y,
                         int top_level) {
  int iheight, side, ipin; 
  int side_pin_index;
  t_type_ptr type_descriptor = grid[x][y].type;
  int capacity = grid[x][y].type->capacity;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  assert(NULL != type_descriptor);
  assert(0 < capacity);
  /* Make sure this is IO */
  assert(IO_TYPE == type_descriptor);

  /* identify the location of IO grid and 
   * decide which side of ports we need
   */
  side = determine_io_grid_side(x,y);
 
  /* Count the number of pins */
  side_pin_index = 0;
  //for (iz = 0; iz < capacity; iz++) {
    for (iheight = 0; iheight < type_descriptor->height; iheight++) {
      for (ipin = 0; ipin < type_descriptor->num_pins; ipin++) {
        if (1 == type_descriptor->pinloc[iheight][side][ipin]) {
          /* This pin appear at this side! */
          if (1 == top_level) {
            fprintf(fp, "+ grid[%d][%d]_pin[%d][%d][%d] \n", x, y,
                    iheight, side, ipin);
          } else {
            fprintf(fp, "+ %s_height[%d]_pin[%d] \n", 
                    convert_side_index_to_string(side), iheight, ipin);
          }
          side_pin_index++;
        }
      }  
    }
  //}

  return;
} 

char* get_grid_block_subckt_name(int x,
                                 int y,
                                 int z,
                                 char* subckt_prefix,
                                 t_block* mapped_block) {
  char* ret = NULL;
  int imode; 
  t_type_ptr type_descriptor = NULL;
  char* formatted_subckt_prefix = format_spice_node_prefix(subckt_prefix);
  int num_idle_mode = 0;

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 

  type_descriptor = grid[x][y].type;
  assert(NULL != type_descriptor);

  if (NULL == mapped_block) {
    /* This a NULL logic block... Find the idle mode*/
    for (imode = 0; imode < type_descriptor->pb_type->num_modes; imode++) {
      if (1 == type_descriptor->pb_type->modes[imode].define_idle_mode) {
        num_idle_mode++;
      }
    } 
    assert(1 == num_idle_mode);
    for (imode = 0; imode < type_descriptor->pb_type->num_modes; imode++) {
      if (1 == type_descriptor->pb_type->modes[imode].define_idle_mode) {
        ret = (char*)my_malloc(sizeof(char)* 
               (strlen(formatted_subckt_prefix) + strlen(type_descriptor->name) + 1
                + strlen(my_itoa(z)) + 7 + strlen(type_descriptor->pb_type->modes[imode].name) + 1 + 1)); 
        sprintf(ret, "%s%s[%d]_mode[%s]", formatted_subckt_prefix,
                type_descriptor->name, z, type_descriptor->pb_type->modes[imode].name);
        break;
      }
    } 
  } else {
    /* This is a logic block with specific configurations*/ 
    assert(NULL != mapped_block->pb);
    imode = mapped_block->pb->mode;
    ret = (char*)my_malloc(sizeof(char)* 
           (strlen(formatted_subckt_prefix) + strlen(type_descriptor->name) + 1
            + strlen(my_itoa(z)) + 7 + strlen(type_descriptor->pb_type->modes[imode].name) + 1 + 1)); 
    sprintf(ret, "%s%s[%d]_mode[%s]", formatted_subckt_prefix,
            type_descriptor->name, z, type_descriptor->pb_type->modes[imode].name);
  }

  return ret;
}                        

/* Physical mode subckt name */
char* get_grid_phy_block_subckt_name(int x, int y, int z,
                                     char* subckt_prefix,
                                     t_block* mapped_block) {
  char* ret = NULL;
  int imode; 
  t_type_ptr type_descriptor = NULL;
  char* formatted_subckt_prefix = format_spice_node_prefix(subckt_prefix);
  int num_physical_mode = 0;

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 

  type_descriptor = grid[x][y].type;
  assert(NULL != type_descriptor);

  if (NULL == mapped_block) {
    /* This a NULL logic block... Find the idle mode*/
    for (imode = 0; imode < type_descriptor->pb_type->num_modes; imode++) {
      if (1 == type_descriptor->pb_type->modes[imode].define_physical_mode) {
        num_physical_mode++;
      }
    } 
    assert(1 == num_physical_mode);
    for (imode = 0; imode < type_descriptor->pb_type->num_modes; imode++) {
      if (1 == type_descriptor->pb_type->modes[imode].define_physical_mode) {
        ret = (char*)my_malloc(sizeof(char)* 
               (strlen(formatted_subckt_prefix) + strlen(type_descriptor->name) + 1
                + strlen(my_itoa(z)) + 7 + strlen(type_descriptor->pb_type->modes[imode].name) + 1 + 1)); 
        sprintf(ret, "%s%s[%d]_mode[%s]", formatted_subckt_prefix,
                type_descriptor->name, z, type_descriptor->pb_type->modes[imode].name);
        break;
      }
    } 
  } else {
    /* This is a logic block with specific configurations*/ 
    assert(NULL != mapped_block->pb);
    imode = mapped_block->pb->mode;
    ret = (char*)my_malloc(sizeof(char)* 
           (strlen(formatted_subckt_prefix) + strlen(type_descriptor->name) + 1
            + strlen(my_itoa(z)) + 7 + strlen(type_descriptor->pb_type->modes[imode].name) + 1 + 1)); 
    sprintf(ret, "%s%s[%d]_mode[%s]", formatted_subckt_prefix,
            type_descriptor->name, z, type_descriptor->pb_type->modes[imode].name);
  }

  return ret;
}                        


/* Print the pins of grid subblocks */
void fprint_grid_block_subckt_pins(FILE* fp,
                                   int z,
                                   t_type_ptr type_descriptor) {
  int iport, ipin, side;
  int grid_pin_index, pin_height, side_pin_index;
  t_pb_graph_node* top_pb_graph_node = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert(NULL != type_descriptor);
  top_pb_graph_node = type_descriptor->pb_graph_head;
  assert(NULL != top_pb_graph_node); 

  for (iport = 0; iport < top_pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < top_pb_graph_node->num_input_pins[iport]; ipin++) {
      grid_pin_index = top_pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster 
                     + z * type_descriptor->num_pins / type_descriptor->capacity;
      /* num_pins/capacity = the number of pins that each type_descriptor has.
       * Capacity defines the number of type_descriptors in each grid
       * so the pin index at grid level = pin_index_in_type_descriptor 
       *                                + type_descriptor_index_in_capacity * num_pins_per_type_descriptor
       */
      pin_height = type_descriptor->pin_height[grid_pin_index];
      for (side = 0; side < 4; side++) {
        if (1 == type_descriptor->pinloc[pin_height][side][grid_pin_index]) {
          /* This pin appear at this side! */
          fprintf(fp, "+ %s_height[%d]_pin[%d] \n", 
                  convert_side_index_to_string(side), pin_height, grid_pin_index);
          side_pin_index++;
        }
      }
    }
  }

  for (iport = 0; iport < top_pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < top_pb_graph_node->num_output_pins[iport]; ipin++) {
      grid_pin_index = top_pb_graph_node->output_pins[iport][ipin].pin_count_in_cluster 
                     + z * type_descriptor->num_pins / type_descriptor->capacity;
      /* num_pins/capacity = the number of pins that each type_descriptor has.
       * Capacity defines the number of type_descriptors in each grid
       * so the pin index at grid level = pin_index_in_type_descriptor 
       *                                + type_descriptor_index_in_capacity * num_pins_per_type_descriptor
       */
      pin_height = type_descriptor->pin_height[grid_pin_index];
      for (side = 0; side < 4; side++) {
        if (1 == type_descriptor->pinloc[pin_height][side][grid_pin_index]) {
          /* This pin appear at this side! */
          fprintf(fp, "+ %s_height[%d]_pin[%d] \n", 
                  convert_side_index_to_string(side), pin_height, grid_pin_index);
          side_pin_index++;
        }
      }
    }
  }

  for (iport = 0; iport < top_pb_graph_node->num_clock_ports; iport++) {
    for (ipin = 0; ipin < top_pb_graph_node->num_clock_pins[iport]; ipin++) {
      grid_pin_index = top_pb_graph_node->clock_pins[iport][ipin].pin_count_in_cluster 
                     + z * type_descriptor->num_pins / type_descriptor->capacity;
      /* num_pins/capacity = the number of pins that each type_descriptor has.
       * Capacity defines the number of type_descriptors in each grid
       * so the pin index at grid level = pin_index_in_type_descriptor 
       *                                + type_descriptor_index_in_capacity * num_pins_per_type_descriptor
       */
      pin_height = type_descriptor->pin_height[grid_pin_index];
      for (side = 0; side < 4; side++) {
        if (1 == type_descriptor->pinloc[pin_height][side][grid_pin_index]) {
          /* This pin appear at this side! */
          fprintf(fp, "+ %s_height[%d]_pin[%d] \n", 
                  convert_side_index_to_string(side), pin_height, grid_pin_index);
          side_pin_index++;
        }
      }
    }
  }

  return;
}


/* Print the pins of grid subblocks */
void fprint_io_grid_block_subckt_pins(FILE* fp,
                                      int x,
                                      int y,
                                      int z,
                                      t_type_ptr type_descriptor) {
  int iport, ipin, side;
  int grid_pin_index, pin_height, side_pin_index;
  t_pb_graph_node* top_pb_graph_node = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert(NULL != type_descriptor);
  top_pb_graph_node = type_descriptor->pb_graph_head;
  assert(NULL != top_pb_graph_node); 

  /* Make sure this is IO */
  assert(IO_TYPE == type_descriptor);

  /* identify the location of IO grid and 
   * decide which side of ports we need
   */
  if (0 == x) {
    /* Left side */
    assert((0 < y)&&(y < (ny + 1)));
    /* Print Right side ports*/
    side = RIGHT;
  } else if ((nx + 1) == x) {
    /* Right side */
    assert((0 < y)&&(y < (ny + 1)));
    /* Print Left side ports*/
    side = LEFT;
  } else if (0 == y) {
    /* Bottom Side */
    assert((0 < x)&&(x < (nx + 1)));
    /* Print TOP side ports */
    side = TOP;
  } else if ((ny + 1) == y) {
    /* TOP Side */
    assert((0 < x)&&(x < (nx + 1)));
    /* Print BOTTOM side ports */
    side = BOTTOM;
  } else {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid co-ordinators(x=%d, y=%d) for I/O grid!\n",
               __FILE__, __LINE__, x, y);
    exit(1);
  }  

  for (iport = 0; iport < top_pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < top_pb_graph_node->num_input_pins[iport]; ipin++) {
      grid_pin_index = top_pb_graph_node->input_pins[iport][ipin].pin_count_in_cluster 
                     + z * type_descriptor->num_pins / type_descriptor->capacity;
      /* num_pins/capacity = the number of pins that each type_descriptor has.
       * Capacity defines the number of type_descriptors in each grid
       * so the pin index at grid level = pin_index_in_type_descriptor 
       *                                + type_descriptor_index_in_capacity * num_pins_per_type_descriptor
       */
      pin_height = type_descriptor->pin_height[grid_pin_index];
      if (1 == type_descriptor->pinloc[pin_height][side][grid_pin_index]) {
        /* This pin appear at this side! */
        fprintf(fp, "+ %s_height[%d]_pin[%d] \n", 
                convert_side_index_to_string(side), pin_height, grid_pin_index);
        side_pin_index++;
      }
    }
  }

  for (iport = 0; iport < top_pb_graph_node->num_output_ports; iport++) {
    for (ipin = 0; ipin < top_pb_graph_node->num_output_pins[iport]; ipin++) {
      grid_pin_index = top_pb_graph_node->output_pins[iport][ipin].pin_count_in_cluster 
                     + z * type_descriptor->num_pins / type_descriptor->capacity;
      /* num_pins/capacity = the number of pins that each type_descriptor has.
       * Capacity defines the number of type_descriptors in each grid
       * so the pin index at grid level = pin_index_in_type_descriptor 
       *                                + type_descriptor_index_in_capacity * num_pins_per_type_descriptor
       */
      pin_height = type_descriptor->pin_height[grid_pin_index];
      if (1 == type_descriptor->pinloc[pin_height][side][grid_pin_index]) {
        /* This pin appear at this side! */
        fprintf(fp, "+ %s_height[%d]_pin[%d] \n", 
                convert_side_index_to_string(side), pin_height, grid_pin_index);
        side_pin_index++;
      }
    }
  }

  for (iport = 0; iport < top_pb_graph_node->num_clock_ports; iport++) {
    for (ipin = 0; ipin < top_pb_graph_node->num_clock_pins[iport]; ipin++) {
      grid_pin_index = top_pb_graph_node->clock_pins[iport][ipin].pin_count_in_cluster 
                     + z * type_descriptor->num_pins / type_descriptor->capacity;
      /* num_pins/capacity = the number of pins that each type_descriptor has.
       * Capacity defines the number of type_descriptors in each grid
       * so the pin index at grid level = pin_index_in_type_descriptor 
       *                                + type_descriptor_index_in_capacity * num_pins_per_type_descriptor
       */
      pin_height = type_descriptor->pin_height[grid_pin_index];
      if (1 == type_descriptor->pinloc[pin_height][side][grid_pin_index]) {
        /* This pin appear at this side! */
        fprintf(fp, "+ %s_height[%d]_pin[%d] \n", 
                convert_side_index_to_string(side), pin_height, grid_pin_index);
        side_pin_index++;
      }
    }
  }

  return;
}
                                   
/* Print the SPICE netlist for a physical grid blocks */
void fprint_grid_physical_blocks(char* subckt_dir,
                                 int ix, int iy,
                                 t_arch* arch) {
  int subckt_name_str_len = 0;
  char* subckt_name = NULL;
  t_block* mapped_block = NULL;
  int iz;
  int cur_block_index = 0;
  int capacity; 
  FILE* fp = NULL;
  char* fname = NULL;

  /* Check */
  assert((!(0 > ix))&&(!(ix > (nx + 1)))); 
  assert((!(0 > iy))&&(!(iy > (ny + 1)))); 

  /* Update the grid_index_low for each spice_model */
  update_spice_models_grid_index_low(ix, iy, arch->spice->num_spice_model, arch->spice->spice_models);

  /* generate_grid_subckt, type_descriptor of each grid defines the capacity,
   * for example, each grid may contains more than one top-level pb_types, such as I/O
   */
  if ((NULL == grid[ix][iy].type)
      ||(EMPTY_TYPE == grid[ix][iy].type)
      ||(0 != grid[ix][iy].offset)) {
    /* Update the grid_index_high for each spice_model */
    update_spice_models_grid_index_high(ix, iy, arch->spice->num_spice_model, arch->spice->spice_models);
    return; 
  }

  /* Create file handler */
  fp = spice_create_one_subckt_file(subckt_dir, "Phyiscal Logic Block ", grid_spice_file_name_prefix, ix, iy, &fname); 

  capacity = grid[ix][iy].type->capacity;
  assert(0 < capacity);

  /* Make the sub-circuit name*/
  /* Name format: grid[<ix>][<iy>]_*/ 
  subckt_name_str_len = 4 + 1 + strlen(my_itoa(ix)) + 2 
                        + strlen(my_itoa(iy)) + 1 + 1 + 1; /* Plus '0' at the end of string*/
  subckt_name = (char*)my_malloc(sizeof(char)*subckt_name_str_len);
  sprintf(subckt_name, "grid[%d][%d]_", ix, iy);

  /* Specify the head of scan-chain */
  if (SPICE_SRAM_SCAN_CHAIN == sram_spice_orgz_type) {
    fprintf(fp, "***** Head of scan-chain *****\n");
    fprintf(fp, "Rgrid[%d][%d]_sc_head grid[%d][%d]_sc_head %s[%d]->in 0\n",
            ix, iy, ix, iy, sram_spice_model->prefix, sram_spice_model->cnt);
    fprintf(fp, ".global grid[%d][%d]_sc_head \n",
            ix, iy);
  }

  cur_block_index = 0;
  /* check capacity and if this has been mapped */
  for (iz = 0; iz < capacity; iz++) {
    /* Comments: Grid [x][y]*/
    fprintf(fp, "***** Grid[%d][%d] type_descriptor: %s[%d] *****\n", ix, iy, grid[ix][iy].type->name, iz);
    /* Print a physical logic block with specific configurations*/ 
    fprint_spice_physical_block(fp, subckt_name, ix, iy, iz, grid[ix][iy].type);
    fprintf(fp, "***** END *****\n\n");
  } 

  /* Specify the tail of scan-chain */
  if (SPICE_SRAM_SCAN_CHAIN == sram_spice_orgz_type) {
    fprintf(fp, "***** Tail of scan-chain *****\n");
    fprintf(fp, "Rgrid[%d][%d]_sc_tail grid[%d][%d]_sc_tail %s[%d]->in 0\n",
            ix, iy, ix, iy, sram_spice_model->prefix, sram_spice_model->cnt);
    fprintf(fp, ".global grid[%d][%d]_sc_tail \n",
            ix, iy);
  }

  /* Update the grid_index_high for each spice_model */
  update_spice_models_grid_index_high(ix, iy, arch->spice->num_spice_model, arch->spice->spice_models);

  fprintf(fp, "***** Grid[%d][%d], Capactity: %d *****\n", ix, iy, capacity);
  fprintf(fp, "***** Top Protocol *****\n");
  /* Definition */
  fprintf(fp, ".subckt grid[%d][%d] \n", ix, iy);
  /* Pins */
  /* Special Care for I/O grid */
  if (IO_TYPE == grid[ix][iy].type) {
    fprint_io_grid_pins(fp, ix, iy, 0);
  } else {
    fprint_grid_pins(fp, ix, iy, 0);
  }
  /* Local Vdd and GND */
  fprintf(fp, "+ svdd sgnd\n");

  /* Quote all the sub blocks*/
  for (iz = 0; iz < capacity; iz++) {
    fprintf(fp, "Xgrid[%d][%d][%d] \n", ix, iy, iz);
    /* Print all the pins */
    /* Special Care for I/O grid */
    if (IO_TYPE == grid[ix][iy].type) {
      fprint_io_grid_block_subckt_pins(fp, ix, iy, iz, grid[ix][iy].type);
    } else {
      fprint_grid_block_subckt_pins(fp, iz, grid[ix][iy].type);
    }
    /* Check in all the blocks(clustered logic block), there is a match x,y,z*/
    mapped_block = search_mapped_block(ix, iy, iz); 
    /* Local Vdd and Gnd, subckt name*/
    fprintf(fp, "+ svdd sgnd %s\n", get_grid_phy_block_subckt_name(ix, iy, iz, subckt_name, NULL));
  }

  fprintf(fp, ".eom\n");

  /* Close the file */
  fclose(fp);

  /* Add fname to the linked list */
  grid_spice_subckt_file_path_head = add_one_subckt_file_name_to_llist(grid_spice_subckt_file_path_head, fname);  

  /* Free */
  my_free(subckt_name);
  my_free(fname);

  return;
}



/* Print the SPICE netlist for a grid blocks */
void fprint_grid_blocks(char* subckt_dir,
                        int ix, int iy,
                        t_arch* arch) {
  int subckt_name_str_len = 0;
  char* subckt_name = NULL;
  t_block* mapped_block = NULL;
  int iz;
  int cur_block_index = 0;
  int capacity; 
  FILE* fp = NULL;
  char* fname = NULL;

  /* Check */
  assert((!(0 > ix))&&(!(ix > (nx + 1)))); 
  assert((!(0 > iy))&&(!(iy > (ny + 1)))); 

  /* Update the grid_index_low for each spice_model */
  update_spice_models_grid_index_low(ix, iy, arch->spice->num_spice_model, arch->spice->spice_models);

  /* generate_grid_subckt, type_descriptor of each grid defines the capacity,
   * for example, each grid may contains more than one top-level pb_types, such as I/O
   */
  /* Bypass EMPTY_TYPE */
  if ((NULL == grid[ix][iy].type)
     ||(EMPTY_TYPE == grid[ix][iy].type)
     ||(0 != grid[ix][iy].offset)) {
    /* Update the grid_index_high for each spice_model */
    update_spice_models_grid_index_high(ix, iy, arch->spice->num_spice_model, arch->spice->spice_models);
    return; 
  }

  /* Create file handler */
  fp = spice_create_one_subckt_file(subckt_dir, "Logic Block", grid_spice_file_name_prefix, ix, iy, &fname); 

  capacity= grid[ix][iy].type->capacity;
  assert(0 < capacity);

  /* Make the sub-circuit name*/
  /* Name format: grid[<ix>][<iy>]_*/ 
  subckt_name_str_len = 4 + 1 + strlen(my_itoa(ix)) + 2 
                        + strlen(my_itoa(iy)) + 1 + 1 + 1; /* Plus '0' at the end of string*/
  subckt_name = (char*)my_malloc(sizeof(char)*subckt_name_str_len);
  sprintf(subckt_name, "grid[%d][%d]_", ix, iy);

  /* Specify the head of scan-chain */
  if (SPICE_SRAM_SCAN_CHAIN == sram_spice_orgz_type) {
    fprintf(fp, "***** Head of scan-chain *****\n");
    fprintf(fp, "Rgrid[%d][%d]_sc_head grid[%d][%d]_sc_head %s[%d]->in 0\n",
            ix, iy, ix, iy, sram_spice_model->prefix, sram_spice_model->cnt);
    fprintf(fp, ".global grid[%d][%d]_sc_head \n",
            ix, iy);
  }

  cur_block_index = 0;
  /* check capacity and if this has been mapped */
  for (iz = 0; iz < capacity; iz++) {
    /* Check in all the blocks(clustered logic block), there is a match x,y,z*/
    mapped_block = search_mapped_block(ix, iy, iz); 
    /* Comments: Grid [x][y]*/
    fprintf(fp, "***** Grid[%d][%d] type_descriptor: %s[%d] *****\n", ix, iy, grid[ix][iy].type->name, iz);
    if (NULL == mapped_block) {
      /* Print a NULL logic block...*/
      fprint_spice_idle_block(fp, subckt_name, ix, iy, iz, grid[ix][iy].type);
    } else {
      if (iz == mapped_block->z) {
        // assert(mapped_block == &(block[grid[ix][iy].blocks[cur_block_index]]));
        cur_block_index++;
      }
      /* Print a logic block with specific configurations*/ 
      fprint_spice_block(fp, subckt_name, ix, iy, iz, grid[ix][iy].type, mapped_block);
    }
    fprintf(fp, "***** END *****\n\n");
  } 

  /* Specify the tail of scan-chain */
  if (SPICE_SRAM_SCAN_CHAIN == sram_spice_orgz_type) {
    fprintf(fp, "***** Tail of scan-chain *****\n");
    fprintf(fp, "Rgrid[%d][%d]_sc_tail grid[%d][%d]_sc_tail %s[%d]->in 0\n",
            ix, iy, ix, iy, sram_spice_model->prefix, sram_spice_model->cnt);
    fprintf(fp, ".global grid[%d][%d]_sc_tail \n",
            ix, iy);
  }

  assert(cur_block_index == grid[ix][iy].usage);

  /* Update the grid_index_high for each spice_model */
  update_spice_models_grid_index_high(ix, iy, arch->spice->num_spice_model, arch->spice->spice_models);

  fprintf(fp, "***** Grid[%d][%d], Capactity: %d *****\n", ix, iy, capacity);
  fprintf(fp, "***** Top Protocol *****\n");
  /* Definition */
  fprintf(fp, ".subckt grid[%d][%d] \n", ix, iy);
  /* Pins */
  /* Special Care for I/O grid */
  if (IO_TYPE == grid[ix][iy].type) {
    fprint_io_grid_pins(fp, ix, iy, 0);
  } else {
    fprint_grid_pins(fp, ix, iy, 0);
  }
  /* Local Vdd and GND */
  fprintf(fp, "+ svdd sgnd\n");

  /* Quote all the sub blocks*/
  for (iz = 0; iz < capacity; iz++) {
    fprintf(fp, "Xgrid[%d][%d][%d] \n", ix, iy, iz);
    /* Print all the pins */
    /* Special Care for I/O grid */
    if (IO_TYPE == grid[ix][iy].type) {
      fprint_io_grid_block_subckt_pins(fp, ix, iy, iz, grid[ix][iy].type);
    } else {
      fprint_grid_block_subckt_pins(fp, iz, grid[ix][iy].type);
    }
    /* Check in all the blocks(clustered logic block), there is a match x,y,z*/
    mapped_block = search_mapped_block(ix, iy, iz); 
    /* Local Vdd and Gnd, subckt name*/
    fprintf(fp, "+ svdd sgnd %s\n", get_grid_block_subckt_name(ix, iy, iz, subckt_name, mapped_block));
  }

  fprintf(fp, ".eom\n");

  /* Close the file */
  fclose(fp);

  /* Add fname to the linked list */
  grid_spice_subckt_file_path_head = add_one_subckt_file_name_to_llist(grid_spice_subckt_file_path_head, fname);  

  /* Free */
  my_free(subckt_name);
  my_free(fname);

  return;
}


/* Print all logic blocks SPICE models 
 * Each logic blocks in the grid that allocated for the FPGA
 * will be printed. May have an additional option that only
 * output the used logic blocks 
 */
void generate_spice_logic_blocks(char* subckt_dir,
                                 t_arch* arch) {
  /* Create file names */
  int ix, iy; 
  
  /* Check the grid*/
  if ((0 == nx)||(0 == ny)) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid grid size (nx=%d, ny=%d)!\n", __FILE__, __LINE__, nx, ny);
    return;    
  }
  vpr_printf(TIO_MESSAGE_INFO,"Grid size of FPGA: nx=%d ny=%d\n", nx + 1, ny + 1);
  assert(NULL != grid);
 
  /* Print the core logic block one by one
   * Note ix=0 and ix = nx + 1 are IO pads. They surround the core logic blocks
   */
  vpr_printf(TIO_MESSAGE_INFO,"Generating core grids...\n");
  for (ix = 1; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      /* Ensure this is not a io */
      assert(IO_TYPE != grid[ix][iy].type);
      /* Ensure a valid usage */
      assert((0 == grid[ix][iy].usage)||(0 < grid[ix][iy].usage));
      fprint_grid_blocks(subckt_dir, ix, iy, arch); 
    }
  }

  vpr_printf(TIO_MESSAGE_INFO,"Generating IO grids...\n");
  /* Print the IO pads */
  /* Left side: x = 0, y = 1 .. ny*/
  ix = 0;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    /* TODO: replace with physical block generator */
    fprint_grid_physical_blocks(subckt_dir, ix, iy, arch); 
  }
  /* Right side : x = nx + 1, y = 1 .. ny*/
  ix = nx + 1;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    /* TODO: replace with physical block generator */
    fprint_grid_physical_blocks(subckt_dir, ix, iy, arch); 
  }
  /* Bottom  side : x = 1 .. nx + 1, y = 0 */
  iy = 0;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    /* TODO: replace with physical block generator */
    fprint_grid_physical_blocks(subckt_dir, ix, iy, arch); 
  }
  /* Top side : x = 1 .. nx + 1, y = nx + 1  */
  iy = ny + 1;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    /* TODO: replace with physical block generator */
    fprint_grid_physical_blocks(subckt_dir, ix, iy, arch); 
  }

  /* Output a header file for all the logic blocks */
  vpr_printf(TIO_MESSAGE_INFO,"Generating header file for grid submodules...\n");
  spice_print_subckt_header_file(grid_spice_subckt_file_path_head,
                                 subckt_dir,
                                 logic_block_spice_file_name);

  return; 
}
