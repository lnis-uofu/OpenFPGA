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
#include "fpga_spice_globals.h"
#include "spice_globals.h"
#include "fpga_spice_utils.h"
#include "spice_utils.h"
#include "spice_mux.h"
#include "spice_pbtypes.h"
#include "spice_lut.h"


/***** Subroutines *****/

void fprint_spice_lut_subckt(FILE* fp,
                             t_spice_model spice_model) {
  int i;
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;
  float total_width;
  int width_cnt;

  /* Ensure a valid file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler!\n",
               __FILE__, __LINE__);
  }
  
  /* Find input ports, output ports and sram ports*/
  input_ports = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_ports = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  sram_ports = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);

  /* Check */
  assert(1 == num_input_port);
  assert(1 == num_output_port);
  assert(1 == num_sram_port);

  fprintf(fp, "***** Auto-generated LUT info: spice_model_name = %s, size = %d *****\n",
          spice_model.name, input_ports[0]->size);
  /* Define the subckt*/
  fprintf(fp, ".subckt %s ", spice_model.name); /* Subckt name*/
  /* Input ports*/
  for (i = 0; i < input_ports[0]->size; i++) {
    fprintf(fp, "%s%d ", input_ports[0]->prefix, i); 
  }
  /* output ports*/
  assert(1 == output_ports[0]->size); 
  fprintf(fp, "%s ", output_ports[0]->prefix);
  /* sram ports */
  for (i = 0; i < sram_ports[0]->size; i++) {
    fprintf(fp, "%s%d ", sram_ports[0]->prefix, i); 
  }
  /* local vdd and gnd*/
  fprintf(fp, "svdd sgnd\n");

  /* Input buffers */
  for (i = 0; i < input_ports[0]->size; i++) {
    /* For negative input of LUT MUX*/
    /* Output inverter with maximum size allowed 
     * until the rest of width is smaller than threshold */
    total_width = spice_model.lut_input_buffer->size * spice_model.lut_input_buffer->f_per_stage;
    width_cnt = 0;
    while (total_width > max_width_per_trans) { 
      fprintf(fp, "Xinv0_in%d_no%d %s%d lut_mux_in%d_inv svdd sgnd inv size=\'%g\'",
              i, width_cnt, 
              input_ports[0]->prefix, i, 
              i, max_width_per_trans);
      fprintf(fp, "\n");
      /* Update */
      total_width = total_width - max_width_per_trans;
      width_cnt++;
    }
    /* Print if we still have to */
    if (total_width > 0) {
      fprintf(fp, "Xinv0_in%d_no%d %s%d lut_mux_in%d_inv svdd sgnd inv size=\'%g\'",
              i, width_cnt, 
              input_ports[0]->prefix, i, 
              i, total_width);
      fprintf(fp, "\n");
    }
    /* For postive input of LUT MUX, we use the tapered_buffer subckt directly */
    assert(1 == spice_model.lut_input_buffer->tapered_buf);
    fprintf(fp, "X%s_in%d %s%d lut_mux_in%d svdd sgnd tapbuf_level%d_f%d\n",
            spice_model.lut_input_buffer->spice_model->prefix, i,
            input_ports[0]->prefix, i, i,
            spice_model.lut_input_buffer->tap_buf_level, 
            spice_model.lut_input_buffer->f_per_stage); 
    fprintf(fp, "\n");
  }

  /* Output buffers already included in LUT MUX */
  /* LUT MUX*/
  assert(sram_ports[0]->size == (int)pow(2.,(double)(input_ports[0]->size)));
  fprintf(fp, "Xlut_mux ");
  /* SRAM ports of LUT, they are inputs of lut_muxes*/
  for (i = 0; i < sram_ports[0]->size; i++) {
    fprintf(fp, "%s%d ", sram_ports[0]->prefix, i);
  } 
  /* Output port, LUT output is LUT MUX output*/
  fprintf(fp, "%s ", output_ports[0]->prefix);
  /* input port, LUT input is LUT MUX sram*/
  for (i = 0; i < input_ports[0]->size; i++) {
    fprintf(fp, "lut_mux_in%d lut_mux_in%d_inv ", i, i); 
  }
  /* Local vdd and gnd*/
  fprintf(fp, "svdd sgnd %s_mux_size%d\n", spice_model.name, sram_ports[0]->size);

  /* End of LUT subckt*/
  fprintf(fp, ".eom\n");

  /* Free */
  free(input_ports);
  free(output_ports);
  free(sram_ports);

  return;
} 

/* Print LUT subckts into a SPICE file*/
void generate_spice_luts(char* subckt_dir, 
                         int num_spice_model, 
                         t_spice_model* spice_models) {
  FILE* fp = NULL;
  char* sp_name = my_strcat(subckt_dir, luts_spice_file_name);
  int imodel = 0;

  /* Create FILE*/
  fp = fopen(sp_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create SPICE netlist %s",__FILE__, __LINE__, wires_spice_file_name); 
    exit(1);
  } 
  fprint_spice_head(fp,"LUTs");

  for (imodel = 0; imodel < num_spice_model; imodel++) {
    if ((SPICE_MODEL_LUT == spice_models[imodel].type)
       &&(NULL == spice_models[imodel].model_netlist)) {
      fprint_spice_lut_subckt(fp, spice_models[imodel]);
    }
  }

  /* Close*/
  fclose(fp);

  return;
}

void fprint_pb_primitive_lut(FILE* fp,
                             char* subckt_prefix,
                             t_pb* prim_pb,
                             t_logical_block* mapped_logical_block,
                             t_pb_graph_node* cur_pb_graph_node,
                             int index,
                             t_spice_model* spice_model,
                             int lut_status) {
  int i;
  int num_sram = 0;
  int* sram_bits = NULL; /* decoded SRAM bits */ 
  int truth_table_length = 0;
  char** truth_table = NULL;
  int lut_size = 0;
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  char* formatted_subckt_prefix = format_spice_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  t_pb_type* cur_pb_type = NULL;
  char* port_prefix = NULL;
  int cur_num_sram = 0;
  char* sram_vdd_port_name = NULL;

  int num_lut_pin_nets;
  int* lut_pin_net = NULL;

  /* Ensure a valid file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Ensure a valid pb_graph_node */ 
  if (NULL == cur_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb_graph_node!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  /* Asserts */
  assert(SPICE_MODEL_LUT == spice_model->type);

  /* Check if this is an idle logical block mapped*/
  switch (lut_status) {
  case PRIMITIVE_WIRED_LUT:
   if (NULL == mapped_logical_block) {
     break; /* Jump out if there is no mapped logical block */
   }
   /* Give a special truth table */
    assert (VPACK_COMB == mapped_logical_block->type);
    /* Get the mapped vpack_net_num of this physical LUT pb */
    get_mapped_lut_pb_input_pin_vpack_net_num(prim_pb, &num_lut_pin_nets, &lut_pin_net);
    /* consider LUT pin remapping when assign lut truth tables */
    /* Match truth table and post-routing results */
    truth_table = assign_post_routing_wired_lut_truth_table(mapped_logical_block, 
                                                            num_lut_pin_nets, lut_pin_net, &truth_table_length); 
    break;
  case PRIMITIVE_IDLE:
    break;
  case PRIMITIVE_NORMAL:
    assert (NULL != mapped_logical_block);
    /* Back-annotate to logical block */
    /* Back-annotate to logical block */
    mapped_logical_block->mapped_spice_model = spice_model;
    mapped_logical_block->mapped_spice_model_index = spice_model->cnt;
 
    assert (VPACK_COMB == mapped_logical_block->type);
    /* Get the mapped vpack_net_num of this physical LUT pb */
    get_mapped_lut_pb_input_pin_vpack_net_num(prim_pb, &num_lut_pin_nets, &lut_pin_net);
    /* consider LUT pin remapping when assign lut truth tables */
    /* Match truth table and post-routing results */
    truth_table = assign_post_routing_lut_truth_table(mapped_logical_block, 
                                                      num_lut_pin_nets, lut_pin_net, &truth_table_length); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(FILE:%s, [LINE%d]) Invalid status(=%d) for LUT!\n",
               __FILE__, __LINE__, lut_status);
    exit(1);
  }
  /* Determine size of LUT*/
  input_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  sram_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
  assert(1 == num_input_port);
  assert(1 == num_output_port);
  assert(1 == num_sram_port);
  lut_size = input_ports[0]->size;
  num_sram = (int)pow(2.,(double)(lut_size));
  assert(num_sram == sram_ports[0]->size);
  assert(1 == output_ports[0]->size);

  /* Get current counter of mem_bits, bl and wl */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(sram_spice_orgz_info); 

  /* Generate sram bits, use the default value of SRAM port */
  sram_bits = generate_lut_sram_bits(truth_table_length, truth_table, 
                                     lut_size, sram_ports[0]->default_val);
 
  /* Print the subckts*/ 
  cur_pb_type = cur_pb_graph_node->pb_type;

  if (NULL != mapped_logical_block) {
    fprintf(fp, "***** Logical block mapped to this LUT: %s *****\n", 
                mapped_logical_block->name);
  }

  /* Subckt definition*/
  fprintf(fp, ".subckt %s%s[%d] ", formatted_subckt_prefix, cur_pb_type->name, index);
  /* Print inputs, outputs, inouts, clocks, NO SRAMs*/
  /*
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(formatted_subckt_prefix) + strlen(cur_pb_type->name) + 1 +
                 strlen(my_itoa(index)) + 1 + 1));
  sprintf(port_prefix, "%s%s[%d]", formatted_subckt_prefix, cur_pb_type->name, index);
  */
  /* Simplify the prefix, make the SPICE netlist readable*/
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(cur_pb_type->name) + 1 +
                 strlen(my_itoa(index)) + 1 + 1));
  sprintf(port_prefix, "%s[%d]", cur_pb_type->name, index);

  /* Only dump the global ports belonging to a spice_model 
   * Do not go recursive, we can freely define global ports anywhere in SPICE netlist
   */
  rec_fprint_spice_model_global_ports(fp, spice_model, FALSE); 
  
  fprint_pb_type_ports(fp, port_prefix, 0, cur_pb_type); 
  /* SRAM bits are fixed in this subckt, no need to define them here*/
  /* Local Vdd and gnd*/ 
  fprintf(fp, "svdd sgnd\n");
  /* Definition ends*/

  /* Print the encoding in SPICE netlist for debugging */
  fprintf(fp, "***** Truth Table for LUT[%d], size=%d. *****\n", 
          spice_model->cnt, lut_size);
  for (i = 0; i < truth_table_length; i++) {
    fprintf(fp,"* %s *\n", truth_table[i]);
  } 

  fprintf(fp, "***** SRAM bits for LUT[%d], size=%d, num_sram=%d. *****\n", 
          spice_model->cnt, lut_size, num_sram);
  fprintf(fp, "*****");
  for (i = 0; i < num_sram; i++) {
     fprintf(fp, "%d", sram_bits[i]);
  }
  fprintf(fp, "*****\n");

  /* Call SRAM subckts*/
  /* Give the VDD port name for SRAMs */
  sram_vdd_port_name = (char*)my_malloc(sizeof(char)*
                                       (strlen(spice_tb_global_vdd_lut_sram_port_name) 
                                        + 1 ));
  sprintf(sram_vdd_port_name, "%s",
                              spice_tb_global_vdd_lut_sram_port_name);
  /* Now Print SRAMs one by one */
  for (i = 0; i < num_sram; i++) {
    fprint_spice_one_sram_subckt(fp, sram_spice_orgz_info, spice_model, sram_vdd_port_name);
  }

  /* Call LUT subckt*/
  fprintf(fp, "X%s[%d] ", spice_model->prefix, spice_model->cnt);
  /* Connect inputs*/ 
  /* Connect outputs*/
  fprint_pb_type_ports(fp, port_prefix, 0, cur_pb_type); 
  /* Connect srams*/
  for (i = 0; i < num_sram; i++) {
    assert( (0 == sram_bits[i]) || (1 == sram_bits[i]) );
    fprint_spice_sram_one_outport(fp, sram_spice_orgz_info, cur_num_sram + i, sram_bits[i]);
  }

  /* vdd should be connected to special global wire gvdd_lut and gnd,
   * Every LUT has a special VDD for statistics
   */
  fprintf(fp, "gvdd_%s[%d] sgnd %s\n", spice_model->prefix, spice_model->cnt, spice_model->name);
  /* TODO: Add a nodeset for convergence */

  /* End of subckt*/
  fprintf(fp, ".eom\n");

  /* Store the configuraion bit to linked-list */
  add_sram_conf_bits_to_llist(sram_spice_orgz_info, cur_num_sram,
                              num_sram, sram_bits);
  
  spice_model->cnt++;

  /*Free*/
  for (i = 0; i < truth_table_length; i++) {
    free(truth_table[i]);
  }
  free(truth_table);
  my_free(formatted_subckt_prefix);
  my_free(input_ports);
  my_free(output_ports);
  my_free(sram_ports);
  my_free(sram_bits);
  my_free(port_prefix);
  my_free(sram_vdd_port_name);

  return;
}
