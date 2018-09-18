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
#include "fpga_spice_utils.h"
#include "fpga_spice_globals.h"

/* Include verilog support headers*/
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_pbtypes.h"
#include "verilog_primitives.h"

/* Subroutines */
void dump_verilog_pb_primitive_ff(FILE* fp,
                            char* subckt_prefix,
                            t_logical_block* mapped_logical_block,
                            t_pb_graph_node* prim_pb_graph_node,
                            int index,
                            t_spice_model* verilog_model) {
  int i;
 
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_clock_port = 0;
  t_spice_model_port** clock_ports = NULL;

  char* formatted_subckt_prefix = format_verilog_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
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
  input_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_INPUT, &num_input_port, FALSE);
  output_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  clock_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_CLOCK, &num_clock_port, FALSE);

  /* Asserts */
  assert(3 == num_input_port); /* D, Set and Reset*/
  for (i = 0; i < num_input_port; i++) {
    assert(1 == input_ports[i]->size);
  }
  assert(1 == num_output_port);
  assert(1 == output_ports[0]->size);
  assert(1 == num_clock_port);
  assert(1 == clock_ports[0]->size);

  assert(SPICE_MODEL_FF == verilog_model->type);
  
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
  sprintf(port_prefix, "%s_%d_", prim_pb_type->name, index);


  if (NULL != mapped_logical_block) {
    fprintf(fp, "//----- Flip-flop Verilog module: %s -----\n", 
                mapped_logical_block->name);
  }
  /* Comment lines */
  fprintf(fp, "//----- Flip-flop Verilog module: %s%s -----\n", 
          formatted_subckt_prefix, port_prefix);
  /* Definition line */
  fprintf(fp, "module %s%s (", formatted_subckt_prefix, port_prefix);
  /* Only dump the global ports belonging to a spice_model */
  if (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, TRUE, TRUE)) {
    fprintf(fp, ",\n");
  }
  /* print ports*/
  dump_verilog_pb_type_ports(fp, port_prefix, 0, prim_pb_type, TRUE, FALSE); 
  /* Local vdd and gnd*/
  fprintf(fp, ");\n");
  /* Definition ends*/

  /* Call the dff subckt*/
  fprintf(fp, "%s %s_%d_ (", verilog_model->name, verilog_model->prefix, verilog_model->cnt);
  /* Only dump the global ports belonging to a spice_model */
  if (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, FALSE, TRUE)) {
    fprintf(fp, ",\n");
  }
  /* print ports*/
  dump_verilog_pb_type_ports(fp, port_prefix, 1, prim_pb_type, FALSE, FALSE); /* Use global clock for each DFF...*/ 

  /* Local vdd and gnd, verilog_model name
   * TODO: global vdd for ff
   */
  fprintf(fp, ");\n");

  /* Apply rising edge, and init value to the ff*/
  if (NULL != mapped_logical_block) {
    /* Back-annotate to logical block */
    mapped_logical_block->mapped_spice_model = verilog_model;
    mapped_logical_block->mapped_spice_model_index = verilog_model->cnt;
  }
  /* TODO: apply falling edge, initial value to FF!!!*/
  /*fprintf(fp, "\n");*/

  /* End */
  fprintf(fp, "endmodule\n");

  /* Comment lines */
  fprintf(fp, "//----- END Flip-flop Verilog module: %s%s -----\n\n", 
          formatted_subckt_prefix, port_prefix);

  verilog_model->cnt++;

  /*Free*/ 
  my_free(formatted_subckt_prefix);
  my_free(port_prefix);

  return;
}

/* Print hardlogic SPICE subckt*/
void dump_verilog_pb_primitive_hardlogic(FILE* fp,
                                   char* subckt_prefix,
                                   t_logical_block* mapped_logical_block,
                                   t_pb_graph_node* prim_pb_graph_node,
                                   int index,
                                   t_spice_model* verilog_model) {
  char* formatted_subckt_prefix = format_verilog_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
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

  /* Asserts */
  assert(SPICE_MODEL_HARDLOGIC == verilog_model->type);
  
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
  sprintf(port_prefix, "%s_%d_", prim_pb_type->name, index);
  /* Comment lines */
  fprintf(fp, "//----- Hardlogic Verilog module: %s%s -----\n", 
          formatted_subckt_prefix, port_prefix);
  /* Definition line */
  fprintf(fp, "module %s%s (", formatted_subckt_prefix, port_prefix);
  fprintf(fp, "\n");
  /* Only dump the global ports belonging to a spice_model */
  if (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, TRUE, TRUE)) {
    fprintf(fp, ",\n");
  }
  /* print ports*/
  dump_verilog_pb_type_ports(fp, port_prefix, 0, prim_pb_type, TRUE, FALSE); 
  /* Local vdd and gnd*/
  fprintf(fp, ");\n");
  /* Definition ends*/

  /* Back-annotate to logical block */
  if (NULL != mapped_logical_block) {
    mapped_logical_block->mapped_spice_model = verilog_model;
    mapped_logical_block->mapped_spice_model_index = verilog_model->cnt;
  }

  /* Call the hardlogic subckt*/
  fprintf(fp, "%s %s_%d_ (", verilog_model->name, verilog_model->prefix, verilog_model->cnt);
  fprintf(fp, "\n");
  /* Only dump the global ports belonging to a spice_model */
  if (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, FALSE, TRUE)) {
    fprintf(fp, ",\n");
  }
  /* print ports*/
  dump_verilog_pb_type_ports(fp, port_prefix, 0, prim_pb_type, FALSE, FALSE); 
  /* Local vdd and gnd, verilog_model name, 
   * Global vdd for hardlogic to split
   */
  fprintf(fp, ");\n");

  /* End */
  fprintf(fp, "endmodule\n");
  /* Comment lines */
  fprintf(fp, "//----- EDN Hardlogic Verilog module: %s%s -----\n", 
          formatted_subckt_prefix, port_prefix);

  verilog_model->cnt++;

  /*Free*/ 
  free(formatted_subckt_prefix);
  free(port_prefix);

  return;
}

/* Dump a I/O pad primitive node */
void dump_verilog_pb_primitive_io(FILE* fp,
                                  char* subckt_prefix,
                                  t_logical_block* mapped_logical_block,
                                  t_pb_graph_node* prim_pb_graph_node,
                                  int index,
                                  t_spice_model* verilog_model) {
  int num_pad_port = 0; /* INOUT port */
  t_spice_model_port** pad_ports = NULL;
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_clock_port = 0;
  t_spice_model_port** clock_ports = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;
  
  int i;
  int num_sram = 0;
  int* sram_bits = NULL;

  char* formatted_subckt_prefix = format_verilog_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  t_pb_type* prim_pb_type = NULL;
  char* port_prefix = NULL;

  /* For each SRAM, we could have multiple BLs/WLs */
  int num_bl_ports = 0;
  t_spice_model_port** bl_port = NULL;
  int num_wl_ports = 0;
  t_spice_model_port** wl_port = NULL;
  int num_bl_per_sram = 0;
  int num_wl_per_sram = 0;
  int expected_num_sram;

  int cur_num_sram = 0;
  int num_conf_bits = 0;
  int num_reserved_conf_bits = 0;
  t_spice_model* mem_model = NULL;
  int cur_bl, cur_wl;

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
  pad_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_INOUT, &num_pad_port, TRUE);
  input_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  clock_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_CLOCK, &num_clock_port, TRUE);
  sram_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);

  /* Asserts */
  assert(SPICE_MODEL_IOPAD == verilog_model->type); /* Support IO PAD which matches the physical design */
  
  /* Initialize */
  get_sram_orgz_info_mem_model(sram_verilog_orgz_info, &mem_model);

  prim_pb_type = prim_pb_graph_node->pb_type;

  /* Generate Subckt for pb_type*/
  /* Simplify the port prefix, make SPICE netlist readable */
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(prim_pb_type->name) + 1
                 + strlen(my_itoa(index)) + 1 + 1));
  sprintf(port_prefix, "%s_%d_", prim_pb_type->name, index);
  /* Comment lines */
  fprintf(fp, "//----- IO Verilog module: %s%s -----\n", 
          formatted_subckt_prefix, port_prefix);
  /* Definition line */
  fprintf(fp, "module %s%s (", formatted_subckt_prefix, port_prefix);
  fprintf(fp, "\n");
  /* Only dump the global ports belonging to a spice_model 
   */
  if (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, TRUE, TRUE)) {
    fprintf(fp, ",\n");
  }

  /* TODO: assert this is physical mode */
  assert((1 == num_sram_port)&&(NULL != sram_ports)&&(1 == sram_ports[0]->size));
  num_sram = count_num_sram_bits_one_spice_model(verilog_model, -1);
  /* Get current counter of mem_bits, bl and wl */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(sram_verilog_orgz_info); 
  get_sram_orgz_info_num_blwl(sram_verilog_orgz_info, &cur_bl, &cur_wl);
  /* print ports --> input ports */
  dump_verilog_pb_type_ports(fp, port_prefix, 0, prim_pb_type, TRUE, TRUE); 
  /* Print output port */
  fprintf(fp, "inout [%d:%d] %s%s\n",
          verilog_model->cnt, verilog_model->cnt,
          gio_inout_prefix, verilog_model->prefix);
  /* Print SRAM ports */
  /* connect to reserved BL/WLs ? */
  num_reserved_conf_bits = count_num_reserved_conf_bits_one_spice_model(verilog_model, sram_verilog_orgz_info->type, 0);
  /* Get the number of configuration bits required by this MUX */
  num_conf_bits = count_num_conf_bits_one_spice_model(verilog_model, sram_verilog_orgz_info->type, 0);
  /* Reserved sram ports */
  if (0 < num_reserved_conf_bits) {
    fprintf(fp, ",\n");
  }
  dump_verilog_reserved_sram_ports(fp, sram_verilog_orgz_info, 
                                   0, num_reserved_conf_bits - 1,
                                   VERILOG_PORT_INPUT);
  /* Normal sram ports */
  if (0 < num_conf_bits) {
    fprintf(fp, ",\n");
  }
  dump_verilog_sram_ports(fp, sram_verilog_orgz_info, 
                          cur_num_sram, cur_num_sram + num_sram - 1,
                          VERILOG_PORT_INPUT);
  /* Local vdd and gnd*/
  fprintf(fp, ");\n");

  dump_verilog_sram_config_bus_internal_wires(fp, sram_verilog_orgz_info, 
                                              cur_num_sram, cur_num_sram + num_sram - 1);
  switch (sram_verilog_orgz_type) {
  case SPICE_SRAM_MEMORY_BANK:
    /* Local wires */
    /* Find the number of BLs/WLs of each SRAM */
    /* Detect the SRAM SPICE model linked to this SRAM port */
    assert(NULL != sram_ports[0]->spice_model);
    assert(SPICE_MODEL_SRAM == sram_ports[0]->spice_model->type);
    find_bl_wl_ports_spice_model(sram_ports[0]->spice_model, 
                                 &num_bl_ports, &bl_port, &num_wl_ports, &wl_port); 
    assert(1 == num_bl_ports);
    assert(1 == num_wl_ports);
    num_bl_per_sram = bl_port[0]->size; 
    num_wl_per_sram = wl_port[0]->size; 
    break;
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  /* Definition ends*/

  /* Dump the configuration port bus */
  dump_verilog_mem_config_bus(fp, mem_model, sram_verilog_orgz_info,
                              cur_num_sram, num_reserved_conf_bits, num_conf_bits); 

  /* Call the I/O subckt*/
  fprintf(fp, "%s %s_%d_ (", verilog_model->name, verilog_model->prefix, verilog_model->cnt);
  fprintf(fp, "\n");
  /* Only dump the global ports belonging to a spice_model 
   * Disable recursive here !
   */
  if (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, FALSE, FALSE)) {
    fprintf(fp, ",\n");
  }
  
  /* assert */
  assert((1 == num_sram_port)&&(NULL != sram_ports)&&(1 == sram_ports[0]->size));
  num_sram = count_num_sram_bits_one_spice_model(verilog_model, -1);
  /* print ports --> input ports */
  dump_verilog_pb_type_ports(fp, port_prefix, 0, prim_pb_type, FALSE, TRUE); 
  /* Print inout port */
  fprintf(fp, "%s%s[%d], ", gio_inout_prefix, 
              verilog_model->prefix, verilog_model->cnt);
  /* Print SRAM ports */
  /* Connect srams: TODO: to find the SRAM model used by this Verilog model */
  dump_verilog_sram_one_outport(fp, sram_verilog_orgz_info,
                                cur_num_sram, cur_num_sram,
                                0, VERILOG_PORT_CONKT);
  fprintf(fp, ", ");
  dump_verilog_sram_one_outport(fp, sram_verilog_orgz_info,
                                cur_num_sram, cur_num_sram,
                                1, VERILOG_PORT_CONKT);
  
  /* Local vdd and gnd, verilog_model name, 
   * TODO: Global vdd for i/o pad to split?
   */
  fprintf(fp, ");\n");

  /* Call SRAM subckt */
  assert((1 == num_sram_port)&&(NULL != sram_ports)&&(1 == sram_ports[0]->size));
  /* what is the SRAM bit of a mode? */
  /* If logical block is not NULL, we need to decode the sram bit */
  if (NULL != mapped_logical_block) {
    assert(NULL != mapped_logical_block->pb->pb_graph_node->pb_type->mode_bits);
    sram_bits = decode_mode_bits(mapped_logical_block->pb->pb_graph_node->pb_type->mode_bits, &expected_num_sram);
    assert(expected_num_sram == num_sram);
  } else {
    /* Initialize */
    sram_bits = (int*)my_calloc(num_sram, sizeof(int));
    for (i = 0; i < num_sram; i++) { 
      sram_bits[i] = sram_ports[0]->default_val;
    }
  }
  /* SRAM_bit will be later reconfigured according to operating mode */
  switch (sram_verilog_orgz_type) {
  case SPICE_SRAM_MEMORY_BANK:
    for (i = 0; i < num_sram; i++) {
      /* Decode the SRAM bits to BL/WL bits.
       * first half part is BL, the other half part is WL 
       */
      decode_and_add_verilog_sram_membank_conf_bit_to_llist(sram_verilog_orgz_info, cur_num_sram + i,
                                                            num_bl_per_sram, num_wl_per_sram,
                                                            sram_bits[i]);
    }
    break;
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    /* Store the configuraion bit to linked-list */
    add_mux_conf_bits_to_llist(0, sram_verilog_orgz_info, 
                               num_sram, sram_bits,
                               verilog_model);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  /* Call SRAM subckts only 
   * when Configuration organization style is memory bank */
  num_sram = count_num_sram_bits_one_spice_model(verilog_model, -1);
  for (i = 0; i < num_sram; i++) {
    dump_verilog_sram_submodule(fp, sram_verilog_orgz_info,
                                mem_model); /* use the mem_model in sram_verilog_orgz_info */
  }

  /* End */
  fprintf(fp, "endmodule\n");
  /* Comment lines */
  fprintf(fp, "//----- END IO Verilog module: %s%s -----\n\n", 
          formatted_subckt_prefix, port_prefix);

  /* Back-annotate to logical block */
  if (NULL != mapped_logical_block) {
    mapped_logical_block->mapped_spice_model = verilog_model;
    mapped_logical_block->mapped_spice_model_index = verilog_model->cnt;
  }

  /* Update the verilog_model counter */
  verilog_model->cnt++;

  /*Free*/ 
  free(formatted_subckt_prefix);
  free(port_prefix);
  my_free(input_ports);
  my_free(output_ports);
  my_free(pad_ports);
  my_free(clock_ports);
  my_free(sram_ports);
  my_free(sram_bits);

  return;
}


