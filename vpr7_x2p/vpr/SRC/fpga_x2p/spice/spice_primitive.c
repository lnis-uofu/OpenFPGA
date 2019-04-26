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
#include "route_common.h"

/* Include spice support headers*/
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_globals.h"
#include "spice_globals.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "spice_utils.h"
#include "spice_pbtypes.h"
#include "spice_primitive.h"

void fprint_pb_primitive_generic(FILE* fp,
                                 char* subckt_prefix,
                                 t_phy_pb* prim_phy_pb,
                                 t_pb_type* prim_pb_type,
                                 int index,
                                 t_spice_model* spice_model) {
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_clock_port = 0;
  t_spice_model_port** clock_ports = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;
  t_spice_model_port* regular_sram_port = NULL;
  t_spice_model_port* mode_bit_port = NULL;

  int i;
  int num_sram = 0;
  int expected_num_sram = 0;
  int* sram_bits = NULL;
  int cur_num_sram = 0;
  t_spice_model* mem_model = NULL;
  int mapped_logical_block_index = OPEN;

  char* formatted_subckt_prefix = format_spice_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  char* port_prefix = NULL;
  char* sram_vdd_port_name = NULL;

  /* Ensure a valid file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Asserts */
  assert((SPICE_MODEL_FF == spice_model->type)
        ||(SPICE_MODEL_HARDLOGIC == spice_model->type)
        ||(SPICE_MODEL_IOPAD == spice_model->type));

  /* Find ports*/
  input_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  clock_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_CLOCK, &num_clock_port, TRUE);
  sram_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
  
  /* Find mapped logical block */
  if (NULL != prim_phy_pb) {
    for (i = 0; i < prim_phy_pb->num_logical_blocks; i++) {
      mapped_logical_block_index = prim_phy_pb->logical_block[i];
      /* Back-annotate to logical block */
      logical_block[mapped_logical_block_index].mapped_spice_model = spice_model;
      logical_block[mapped_logical_block_index].mapped_spice_model_index = spice_model->cnt;
      fprintf(fp, "***** Logical block mapped to this primitive node: %s *****\n", 
              logical_block[mapped_logical_block_index].name);
    }
  }

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

  /* Decode SRAM bits */
  num_sram = count_num_sram_bits_one_spice_model(spice_model, -1);
  /* what is the SRAM bit of a mode? */
  /* If logical block is not NULL, we need to decode the sram bit */
  if ( 0 < num_sram_port) {
    assert (1 == num_sram_port);
    if (NULL != prim_phy_pb) {
      sram_bits = decode_mode_bits(prim_phy_pb->mode_bits, &expected_num_sram);
    } else { /* get default mode_bits */
      sram_bits = decode_mode_bits(prim_pb_type->mode_bits, &expected_num_sram);
    }
    assert(expected_num_sram == num_sram);
  }

  /* Get current counter of mem_bits, bl and wl */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(sram_spice_orgz_info); 

  /* Definition line */
  fprintf(fp, ".subckt %s%s ", formatted_subckt_prefix, port_prefix);
  /* print ports*/
  fprint_pb_type_ports(fp, port_prefix, 0, prim_pb_type); 
  /* Local vdd and gnd*/
  fprintf(fp, "svdd sgnd\n");
  /* Definition ends*/

  /* Call the iopad subckt*/
  fprintf(fp, "X%s[%d] ", spice_model->prefix, spice_model->cnt);
  /* Only dump the global ports belonging to a spice_model 
   * Do not go recursive, we can freely define global ports anywhere in SPICE netlist
   */
  if (0 < rec_fprint_spice_model_global_ports(fp, spice_model, FALSE)) {
    fprintf(fp, "+ ");
  }
  /* print regular ports*/
  fprint_pb_type_ports(fp, port_prefix, 0, prim_pb_type); 
  /* Print inout port */
  if (SPICE_MODEL_IOPAD == spice_model->type) {
    fprintf(fp, " %s%s[%d] ", 
                gio_inout_prefix, 
                spice_model->prefix, 
                spice_model->cnt);
  }
  /* Print SRAM ports */
  for (i = 0; i < num_sram; i++) {
    fprint_spice_sram_one_outport(fp, sram_spice_orgz_info, cur_num_sram + i, sram_bits[i]);
    /* We need the invertered signal for better convergency */
    fprint_spice_sram_one_outport(fp, sram_spice_orgz_info, cur_num_sram + i, 1 - sram_bits[i]);
  }

  /* Local vdd and gnd, spice_model name, 
   * TODO: Global vdd for i/o pad to split?
   */
  fprintf(fp, "%s_%s[%d] sgnd %s\n", 
               spice_tb_global_vdd_port_name,
               spice_model->prefix, 
               spice_model->cnt,
               spice_model->name);

 
  /* Print the encoding in SPICE netlist for debugging */
  fprintf(fp, "***** SRAM bits for %s[%d] *****\n", 
          spice_model->prefix,
          spice_model->cnt);
  fprintf(fp, "*****");
  for (i = 0; i < num_sram; i++) {
    fprintf(fp, "%d", sram_bits[i]);
  }
  fprintf(fp, "*****\n");

  /* Call SRAM subckts*/
  /* Give the VDD port name for SRAMs */
  sram_vdd_port_name = (char*)my_malloc(sizeof(char)*
                                       (strlen(spice_tb_global_vdd_io_sram_port_name) 
                                        + 1 ));
  sprintf(sram_vdd_port_name, "%s",
                              spice_tb_global_vdd_io_sram_port_name);
  /* Now Print SRAMs one by one */
  for (i = 0; i < num_sram; i++) {
    fprint_spice_one_sram_subckt(fp, sram_spice_orgz_info, spice_model, sram_vdd_port_name);
  }

  /* Store the configuraion bit to linked-list */
  add_sram_conf_bits_to_llist(sram_spice_orgz_info, cur_num_sram,
                              num_sram, sram_bits);

  /* End */
  fprintf(fp, ".eom\n");

  /* Update the spice_model counter */
  spice_model->cnt++;

  /*Free*/ 
  my_free(formatted_subckt_prefix);
  my_free(port_prefix);
  my_free(sram_vdd_port_name);

  return;
}


