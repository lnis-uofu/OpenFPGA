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
#include "rr_graph_swseg.h"
#include "vpr_utils.h"

/* Include spice support headers*/
#include "linkedlist.h"
#include "spice_globals.h"
#include "spice_utils.h"
#include "spice_pbtypes.h"
#include "spice_primitives.h"

enum e_ff_trigger_type {
  FF_RE, FF_FE
};

/* Subroutines */
void fprint_pb_primitive_ff(FILE* fp,
                            char* subckt_prefix,
                            t_logical_block* mapped_logical_block,
                            t_pb_graph_node* prim_pb_graph_node,
                            int index,
                            t_spice_model* spice_model) {
  int i;
  /* Default FF settings, applied when this FF is idle*/
  enum e_ff_trigger_type trigger_type = FF_RE;
  int init_val = 0;
 
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_clock_port = 0;
  t_spice_model_port** clock_ports = NULL;

  int iport, ipin;
  int num_pb_type_output_port = 0;
  t_port** pb_type_output_ports = NULL;

  char* formatted_subckt_prefix = format_spice_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  t_pb_type* prim_pb_type = NULL;
  char* port_prefix = NULL;

  /* Ensure a valid file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Ensure a valid pb_graph_node */ 
  if (NULL == prim_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid prim_pb_graph_node!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Find ports*/
  input_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port);
  output_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port);
  clock_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_CLOCK, &num_clock_port);

  /* Asserts */
  assert(3 == num_input_port); /* D, Set and Reset*/
  for (i = 0; i < num_input_port; i++) {
    assert(1 == input_ports[i]->size);
  }
  assert(1 == num_output_port);
  assert(1 == output_ports[0]->size);
  assert(1 == num_clock_port);
  assert(1 == clock_ports[0]->size);

  assert(SPICE_MODEL_FF == spice_model->type);
  
  /* Initialize */
  prim_pb_type = prim_pb_graph_node->pb_type;

  /* Generate Subckt for pb_type*/
  /*
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(formatted_subckt_prefix) + strlen(prim_pb_type->name) + 1
                 + strlen(my_itoa(index)) + 1 + 1));
  sprintf(port_prefix, "%s%s[%d]", formatted_subckt_prefix, prim_pb_type->name, index);
  */
  /* Simplify the port prefix, make SPICE netlist readable */
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(prim_pb_type->name) + 1
                 + strlen(my_itoa(index)) + 1 + 1));
  sprintf(port_prefix, "%s[%d]", prim_pb_type->name, index);
  /* Definition line */
  fprintf(fp, ".subckt %s%s ", formatted_subckt_prefix, port_prefix);
  /* print ports*/
  fprint_pb_type_ports(fp, port_prefix, 0, prim_pb_type); 
  /* Local vdd and gnd*/
  fprintf(fp, "svdd sgnd\n");
  /* Definition ends*/

  /* Call the dff subckt*/
  fprintf(fp, "X%s[%d] ", spice_model->prefix, spice_model->cnt);
  /* print ports*/
  fprint_pb_type_ports(fp, port_prefix, 1, prim_pb_type); /* Use global clock for each DFF...*/ 
  /* print global set and reset */
  fprintf(fp, "gset greset ");
  /* Local vdd and gnd, spice_model name
   * TODO: global vdd for ff
   */
  fprintf(fp, "gvdd_%s[%d] sgnd %s\n", spice_model->prefix, spice_model->cnt, spice_model->name);

  /* Apply rising edge, and init value to the ff*/
  if (NULL != mapped_logical_block) {
    /* Consider the rising edge|falling edge */
    if (0 == strcmp("re", mapped_logical_block->trigger_type)) { 
      trigger_type = FF_RE;
    } else if (0 == strcmp("fe", mapped_logical_block->trigger_type)) { 
      trigger_type = FF_FE;
    } else {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid ff trigger type! Should be [re|fe].\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    /* Assign initial value */
    if (1 == mapped_logical_block->init_val) {
      init_val = 1;
    } else {
      init_val = 0;
    }

    /* Back-annotate to logical block */
    mapped_logical_block->mapped_spice_model = spice_model;
    mapped_logical_block->mapped_spice_model_index = spice_model->cnt;
  } else {
    trigger_type = FF_RE;
    init_val = 0;
  }
  /* TODO: apply falling edge, initial value to FF!!!*/
  /*fprintf(fp, "\n");*/

  /* Add nodeset */
  pb_type_output_ports = find_pb_type_ports_match_spice_model_port_type(prim_pb_type, SPICE_MODEL_PORT_OUTPUT, &num_pb_type_output_port); 
  for (iport = 0; iport < num_pb_type_output_port; iport++) {
    for (ipin = 0; ipin < pb_type_output_ports[iport]->num_pins; ipin++) {
      fprintf(fp, ".nodeset V(%s->%s[%d]) ", port_prefix, pb_type_output_ports[iport]->name, ipin);
      if (0 == init_val) { 
        fprintf(fp, "0\n");
      } else {
        assert(1 == init_val);
        fprintf(fp, "vsp\n");
      }
    }
  }

  /* End */
  fprintf(fp, ".eom\n");

  spice_model->cnt++;

  /*Free*/ 
  my_free(formatted_subckt_prefix);
  my_free(port_prefix);

  return;
}

/* Print hardlogic SPICE subckt*/
void fprint_pb_primitive_hardlogic(FILE* fp,
                                   char* subckt_prefix,
                                   t_logical_block* mapped_logical_block,
                                   t_pb_graph_node* prim_pb_graph_node,
                                   int index,
                                   t_spice_model* spice_model) {
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_clock_port = 0;
  t_spice_model_port** clock_ports = NULL;

  char* formatted_subckt_prefix = format_spice_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  t_pb_type* prim_pb_type = NULL;
  char* port_prefix = NULL;

  /* Ensure a valid file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Ensure a valid pb_graph_node */ 
  if (NULL == prim_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid prim_pb_graph_node!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Find ports*/
  input_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port);
  output_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port);
  clock_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_CLOCK, &num_clock_port);

  /* Asserts */
  assert(SPICE_MODEL_HARDLOGIC == spice_model->type);
  
  /* Initialize */
  prim_pb_type = prim_pb_graph_node->pb_type;

  /* Generate Subckt for pb_type*/
  /*
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(formatted_subckt_prefix) + strlen(prim_pb_type->name) + 1
                 + strlen(my_itoa(index)) + 1 + 1));
  sprintf(port_prefix, "%s%s[%d]", formatted_subckt_prefix, prim_pb_type->name, index);
  */
  /* Simplify the port prefix, make SPICE netlist readable */
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(prim_pb_type->name) + 1
                 + strlen(my_itoa(index)) + 1 + 1));
  sprintf(port_prefix, "%s[%d]", prim_pb_type->name, index);
  /* Definition line */
  fprintf(fp, ".subckt %s%s ", formatted_subckt_prefix, port_prefix);
  /* print ports*/
  fprint_pb_type_ports(fp, port_prefix, 0, prim_pb_type); 
  /* Local vdd and gnd*/
  fprintf(fp, "svdd sgnd\n");
  /* Definition ends*/

  /* Back-annotate to logical block */
  if (NULL != mapped_logical_block) {
    mapped_logical_block->mapped_spice_model = spice_model;
    mapped_logical_block->mapped_spice_model_index = spice_model->cnt;
  }

  /* Call the dff subckt*/
  fprintf(fp, "X%s[%d] ", spice_model->prefix, spice_model->cnt);
  /* print ports*/
  fprint_pb_type_ports(fp, port_prefix, 0, prim_pb_type); 
  /* Local vdd and gnd, spice_model name, 
   * Global vdd for hardlogic to split
   */
  fprintf(fp, "gvdd_%s[%d] sgnd %s\n", spice_model->prefix, spice_model->cnt, spice_model->name);

  /* End */
  fprintf(fp, ".eom\n");

  spice_model->cnt++;

  /*Free*/ 
  free(formatted_subckt_prefix);
  free(port_prefix);

  return;
}

void fprint_pb_primitive_io(FILE* fp,
                            char* subckt_prefix,
                            t_logical_block* mapped_logical_block,
                            t_pb_graph_node* prim_pb_graph_node,
                            int index,
                            t_spice_model* spice_model) {
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_clock_port = 0;
  t_spice_model_port** clock_ports = NULL;

  char* formatted_subckt_prefix = format_spice_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  t_pb_type* prim_pb_type = NULL;
  char* port_prefix = NULL;

  /* Ensure a valid file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Ensure a valid pb_graph_node */ 
  if (NULL == prim_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid prim_pb_graph_node!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Find ports*/
  input_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port);
  output_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port);
  clock_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_CLOCK, &num_clock_port);

  /* Asserts */
  assert((SPICE_MODEL_INPAD == spice_model->type)||(SPICE_MODEL_OUTPAD == spice_model->type));
  
  /* Initialize */
  prim_pb_type = prim_pb_graph_node->pb_type;

  /* Generate Subckt for pb_type*/
  /*
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(formatted_subckt_prefix) + strlen(prim_pb_type->name) + 1
                 + strlen(my_itoa(index)) + 1 + 1));
  sprintf(port_prefix, "%s%s[%d]", formatted_subckt_prefix, prim_pb_type->name, index);
  */
  /* Simplify the port prefix, make SPICE netlist readable */
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(prim_pb_type->name) + 1
                 + strlen(my_itoa(index)) + 1 + 1));
  sprintf(port_prefix, "%s[%d]", prim_pb_type->name, index);
  /* Definition line */
  fprintf(fp, ".subckt %s%s ", formatted_subckt_prefix, port_prefix);
  /* print ports*/
  fprint_pb_type_ports(fp, port_prefix, 0, prim_pb_type); 
  /* Local vdd and gnd*/
  fprintf(fp, "svdd sgnd\n");
  /* Definition ends*/

  /* Call the dff subckt*/
  fprintf(fp, "X%s[%d] ", spice_model->prefix, spice_model->cnt);
  if ((0 == strcmp(".input", prim_pb_type->blif_model))||(0 == strcmp(".clock", prim_pb_type->blif_model))) {
    /* Add input port to Input Pad */
    /* Print input port */
    fprintf(fp, "gfpga_input[%d]_%s[%d] ", spice_model->cnt, spice_model->prefix, spice_model->cnt);
    /* print ports --> output ports */
    fprint_pb_type_ports(fp, port_prefix, 0, prim_pb_type); 
  } else if (0 == strcmp(".output", prim_pb_type->blif_model)){
    /* Add output port to Output Pad */
    /* print ports --> input ports */
    fprint_pb_type_ports(fp, port_prefix, 0, prim_pb_type); 
    /* Print output port */
    fprintf(fp, "gfpga_output[%d]_%s[%d] ", spice_model->cnt, spice_model->prefix, spice_model->cnt);

    /* Add clock port to Input Pad */
  } else {
    /* The rest is invalid */ 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid blif_model(%s) for prim_pb_type(%s)!\n", 
               __FILE__, __LINE__, prim_pb_type->blif_model, prim_pb_type->name);
  } 
  
  /* Back-annotate to logical block */
  if (NULL != mapped_logical_block) {
    mapped_logical_block->mapped_spice_model = spice_model;
    mapped_logical_block->mapped_spice_model_index = spice_model->cnt;
  }

  /* Local vdd and gnd, spice_model name, 
   * TODO: Global vdd for i/o pad to split?
   */
  fprintf(fp, "gvdd_io sgnd %s\n", spice_model->name);

  /* End */
  fprintf(fp, ".eom\n");
  /* Update the spice_model counter */
  spice_model->cnt++;

  /*Free*/ 
  free(formatted_subckt_prefix);
  free(port_prefix);

  return;
}


