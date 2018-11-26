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
#include "verilog_lut.h"


/***** Subroutines *****/
void dump_verilog_pb_primitive_lut(FILE* fp,
                                   char* subckt_prefix,
                                   t_pb* prim_pb,
                                   t_logical_block* mapped_logical_block,
                                   t_pb_graph_node* cur_pb_graph_node,
                                   int index,
                                   t_spice_model* verilog_model,
                                   int lut_status) {
  int i;
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

  int num_pb_type_input_port = 0;
  t_port** pb_type_input_ports = NULL;

  int num_pb_type_output_port = 0;
  t_port** pb_type_output_ports = NULL;

  char* formatted_subckt_prefix = format_verilog_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  t_pb_type* cur_pb_type = NULL;
  char* port_prefix = NULL;
  int cur_num_sram = 0;
  int num_sram = 0;
  /* For each SRAM, we could have multiple BLs/WLs */
  int num_bl_ports = 0;
  t_spice_model_port** bl_port = NULL;
  int num_wl_ports = 0;
  t_spice_model_port** wl_port = NULL;
  int num_bl_per_sram = 0;
  int num_wl_per_sram = 0;
  int num_conf_bits = 0;
  int num_reserved_conf_bits = 0;
  int cur_bl, cur_wl;
  t_spice_model* mem_model = NULL;

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
  assert(SPICE_MODEL_LUT == verilog_model->type);

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
    mapped_logical_block->mapped_spice_model = verilog_model;
    mapped_logical_block->mapped_spice_model_index = verilog_model->cnt;
 
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
  input_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  assert(1 == num_input_port);
  assert(1 == num_output_port);
  lut_size = input_ports[0]->size;
  assert(1 == output_ports[0]->size);
  /* Find SRAM ports */
  sram_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
  assert(1 == num_sram_port);
  /* Count the number of configuration bits */
  num_sram = count_num_sram_bits_one_spice_model(verilog_model, -1);
  /* Get memory model */
  get_sram_orgz_info_mem_model(sram_verilog_orgz_info, &mem_model);

  /* Find the number of BLs/WLs of each SRAM */
  switch (sram_verilog_orgz_type) {
  case SPICE_SRAM_MEMORY_BANK:
    /* Detect the SRAM SPICE model linked to this SRAM port */
    assert(NULL != sram_ports[0]->spice_model);
    assert(SPICE_MODEL_SRAM == sram_ports[0]->spice_model->type);
    find_bl_wl_ports_spice_model(sram_ports[0]->spice_model, 
                                 &num_bl_ports, &bl_port, &num_wl_ports, &wl_port); 
    assert(1 == num_bl_ports);
    assert(1 == num_wl_ports);
    num_bl_per_sram = bl_port[0]->size; 
    num_wl_per_sram = wl_port[0]->size; 
    /* Asserts */
    assert(num_bl_per_sram == num_wl_per_sram);
    break;
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Generate sram bits*/
  sram_bits = generate_lut_sram_bits(truth_table_length, truth_table, 
                                     lut_size, sram_ports[0]->default_val);

  /* Print the subckts*/ 
  cur_pb_type = cur_pb_graph_node->pb_type;

  /* Comment lines */
  fprintf(fp, "//----- LUT Verilog module: %s%s_%d_ -----\n",
          formatted_subckt_prefix, cur_pb_type->name, index);

  /* Simplify the prefix, make the SPICE netlist readable*/
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(cur_pb_type->name) + 1 +
                 strlen(my_itoa(index)) + 1 + 1));
  sprintf(port_prefix, "%s_%d_", cur_pb_type->name, index);

  /* Subckt definition*/
  fprintf(fp, "module %s%s_%d_ (", 
          formatted_subckt_prefix, cur_pb_type->name, index);
  fprintf(fp, "\n");
  /* Only dump the global ports belonging to a spice_model */
  if (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, TRUE, TRUE)) {
    fprintf(fp, ",\n");
  }
  /* Print inputs, outputs, inouts, clocks, NO SRAMs*/
  dump_verilog_pb_type_ports(fp, port_prefix, 0, cur_pb_type, TRUE, TRUE); 
  /* Print SRAM ports */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(sram_verilog_orgz_info); 
  get_sram_orgz_info_num_blwl(sram_verilog_orgz_info, &cur_bl, &cur_wl);
  /* connect to reserved BL/WLs ? */
  num_reserved_conf_bits = count_num_reserved_conf_bits_one_spice_model(verilog_model, sram_verilog_orgz_info->type, 0);
  /* Get the number of configuration bits required by this MUX */
  num_conf_bits = count_num_conf_bits_one_spice_model(verilog_model, sram_verilog_orgz_info->type, 0);
  /* Reserved sram ports */
  dump_verilog_reserved_sram_ports(fp, sram_verilog_orgz_info, 
                                   0, num_reserved_conf_bits - 1,
                                   VERILOG_PORT_INPUT);
  if ( 0 < num_reserved_conf_bits) {
    fprintf(fp, ",\n");
  }
  /* Normal sram ports */
  dump_verilog_sram_ports(fp, sram_verilog_orgz_info, 
                          cur_num_sram, cur_num_sram + num_conf_bits - 1,
                          VERILOG_PORT_INPUT);
  /* Local Vdd and gnd*/ 
  fprintf(fp, ");\n");
  /* Definition ends*/

  /* Specify inputs are wires */
  pb_type_input_ports = find_pb_type_ports_match_spice_model_port_type(cur_pb_type, SPICE_MODEL_PORT_INPUT, &num_pb_type_input_port); 
  assert(1 == num_pb_type_input_port);
  fprintf(fp, "wire [0:%d] %s__%s;\n",
          input_ports[0]->size - 1, port_prefix, pb_type_input_ports[0]->name);
  for (i = 0; i < input_ports[0]->size; i++) {
    fprintf(fp, "assign %s__%s[%d] = %s__%s_%d_;\n",
                port_prefix, pb_type_input_ports[0]->name, i,
                port_prefix, pb_type_input_ports[0]->name, i);
  }
  /* Specify outputs are wires */
  pb_type_output_ports = find_pb_type_ports_match_spice_model_port_type(cur_pb_type, SPICE_MODEL_PORT_OUTPUT, &num_pb_type_output_port); 
  assert(1 == num_pb_type_output_port);
  fprintf(fp, "wire [0:%d] %s__%s;\n",
          output_ports[0]->size - 1, port_prefix, pb_type_output_ports[0]->name);
  for (i = 0; i < output_ports[0]->size; i++) {
    fprintf(fp, "assign %s__%s_%d_ = %s__%s[%d];\n",
                port_prefix, pb_type_output_ports[0]->name, i,
                port_prefix, pb_type_output_ports[0]->name, i);
  }

  /* Specify SRAM output are wires */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(sram_verilog_orgz_info);
  dump_verilog_sram_config_bus_internal_wires(fp, sram_verilog_orgz_info, cur_num_sram, cur_num_sram + num_sram - 1);
  /*
  fprintf(fp, "wire [%d:%d] %s_out;\n",
          cur_num_sram, cur_num_sram + num_sram - 1, mem_model->prefix);
  fprintf(fp, "wire [%d:%d] %s_outb;\n",
          cur_num_sram, cur_num_sram + num_sram - 1, mem_model->prefix);
  */

  num_sram = count_num_sram_bits_one_spice_model(verilog_model, -1);
  cur_num_sram = get_sram_orgz_info_num_mem_bit(sram_verilog_orgz_info);

  /* Call LUT subckt*/
  fprintf(fp, "%s %s_%d_ (", verilog_model->name, verilog_model->prefix, verilog_model->cnt);
  fprintf(fp, "\n");
  /* if we have to add global ports when dumping submodules of LUTs
   * otherwise, the port map here does not match that of submodules 
   * Only dump the global ports belonging to a spice_model 
   * DISABLE recursive here !
   */
  if (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, FALSE, FALSE)) {
    fprintf(fp, ",\n");
  }
  /* Connect inputs*/ 
  /* Connect outputs*/
  fprintf(fp, "//----- Input and output ports -----\n");
  dump_verilog_pb_type_bus_ports(fp, port_prefix, 0, cur_pb_type, FALSE, TRUE); 
  fprintf(fp, "//----- SRAM ports -----\n");
  /* Connect srams: TODO: to find the SRAM model used by this Verilog model */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(sram_verilog_orgz_info);
  /* TODO: switch depending on the type of configuration circuit */
  switch (sram_verilog_orgz_info->type) {
  case SPICE_SRAM_STANDALONE: 
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    dump_verilog_sram_one_port(fp, sram_verilog_orgz_info, 
                               cur_num_sram, cur_num_sram + num_sram - 1, 
                               1, VERILOG_PORT_CONKT);
    fprintf(fp, ", ");
    dump_verilog_sram_one_outport(fp, sram_verilog_orgz_info, 
                                  cur_num_sram, cur_num_sram + num_sram - 1, 
                                  1, VERILOG_PORT_CONKT);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    dump_verilog_sram_one_outport(fp, sram_verilog_orgz_info, 
                                  cur_num_sram, cur_num_sram + num_sram - 1, 
                                  0, VERILOG_PORT_CONKT);
    fprintf(fp, ", ");
    dump_verilog_sram_one_outport(fp, sram_verilog_orgz_info, 
                                  cur_num_sram, cur_num_sram + num_sram - 1, 
                                  1, VERILOG_PORT_CONKT);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  /* vdd should be connected to special global wire gvdd_lut and gnd,
   * Every LUT has a special VDD for statistics
   */
  fprintf(fp, ");\n");

  /* Print the encoding in SPICE netlist for debugging */
  if (NULL != mapped_logical_block) {
    fprintf(fp, "//----- Truth Table for LUT node (%s). -----\n", 
                mapped_logical_block->name);
  }
  fprintf(fp, "//----- Truth Table for LUT[%d], size=%d. -----\n", 
          verilog_model->cnt, lut_size);
  for (i = 0; i < truth_table_length; i++) {
    fprintf(fp,"//  %s \n", truth_table[i]);
  } 

  fprintf(fp, "//----- SRAM bits for LUT[%d], size=%d, num_sram=%d. -----\n", 
          verilog_model->cnt, lut_size, num_sram);
  fprintf(fp, "//-----");
  fprint_commented_sram_bits(fp, num_sram, sram_bits);
  fprintf(fp, "-----\n");

  /* Decode the SRAM bits to BL/WL bits. */ 
  switch (sram_verilog_orgz_type) {
  case SPICE_SRAM_MEMORY_BANK:
    cur_num_sram = get_sram_orgz_info_num_mem_bit(sram_verilog_orgz_info);
    for (i = 0; i < num_sram; i++) {
      /* TODO: should be more structural in coding !!! */
      /* Decode the SRAM bits to BL/WL bits.
       * first half part is BL, the other half part is WL 
       */
      decode_and_add_verilog_sram_membank_conf_bit_to_llist(sram_verilog_orgz_info, cur_num_sram + i,
                                                            num_bl_per_sram, num_wl_per_sram,
                                                            sram_bits[i]);
    }
    /* NUM_SRAM is set to be consistent with number of BL/WLs
     * TODO: NUM_SRAM should be the as they are. 
     * Should use another variable i.e., num_bl
     */
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
  /* No. of SRAMs is different from the number of configuration lines.
   * Especially when SRAMs/RRAMs are configured with BL/WLs
   */
  num_sram = count_num_sram_bits_one_spice_model(verilog_model, -1);
  for (i = 0; i < num_sram; i++) {
    /* Dump the configuration port bus */
    /*TODO: to be more smart!!! num_reserved_conf_bits and num_conf_bits/num_sram should be determined by each mem_bit */ 
    cur_num_sram = get_sram_orgz_info_num_mem_bit(sram_verilog_orgz_info);
    dump_verilog_mem_config_bus(fp, mem_model, sram_verilog_orgz_info,
                                cur_num_sram, num_reserved_conf_bits, num_conf_bits/num_sram); 
    /* This function should be called in the very end, 
     * because we update the counter of mem_model after each sram submodule is dumped !!!
     */
    dump_verilog_sram_submodule(fp, sram_verilog_orgz_info,
                                mem_model); /* use the mem_model in sram_verilog_orgz_info */
  }

  /* End of subckt*/
  fprintf(fp, "endmodule\n");

  /* Comment lines */
  fprintf(fp, "//----- END LUT Verilog module: %s%s_%d_ -----\n\n",
          formatted_subckt_prefix, cur_pb_type->name, index);
 
  /* Update counter */
  verilog_model->cnt++;

  /*Free*/
  my_free(formatted_subckt_prefix);
  my_free(input_ports);
  my_free(output_ports);
  my_free(sram_ports);
  my_free(sram_bits);
  my_free(port_prefix);

  return;
}
