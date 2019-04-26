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
#include "fpga_x2p_utils.h"
#include "fpga_x2p_lut_utils.h"
#include "fpga_x2p_mux_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_bitstream_utils.h"

/* Generate the bitstream of a generic primitive node: 
 * this node can be HARD LOGIC, IO, FF  */
void fpga_spice_generate_bitstream_pb_generic_primitive(FILE* fp,
                                                        t_phy_pb* prim_phy_pb,
                                                        t_pb_type* prim_pb_type,
                                                        t_sram_orgz_info* cur_sram_orgz_info) {
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  t_spice_model* verilog_model = NULL;
  
  int i, mapped_logical_block_index;
  int num_sram = 0;
  int* sram_bits = NULL;

  /* For each SRAM, we could have multiple BLs/WLs */
  int num_bl_ports = 0;
  t_spice_model_port** bl_port = NULL;
  int num_wl_ports = 0;
  t_spice_model_port** wl_port = NULL;
  int num_bl_per_sram = 0;
  int num_wl_per_sram = 0;
  int expected_num_sram;

  int cur_num_sram = 0;
  t_spice_model* mem_model = NULL;

  /* Ensure a valid physical pritimive pb */ 
  if (NULL == prim_pb_type) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid prim_pb_type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Asserts */
  if (NULL != prim_phy_pb) {
    assert (prim_phy_pb->pb_graph_node->pb_type->phy_pb_type == prim_pb_type);
  }
  assert (NULL != prim_pb_type->spice_model);

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  verilog_model = prim_pb_type->spice_model; 

  assert((SPICE_MODEL_IOPAD == verilog_model->type)
         || (SPICE_MODEL_HARDLOGIC == verilog_model->type)
         || (SPICE_MODEL_FF == verilog_model->type));

  /* Find ports*/
  sram_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
  
  /* if there is no SRAM ports, we can return */
  if ( 0 == num_sram_port) {
    /* Back-annotate to logical block */
    if (NULL != prim_phy_pb) {
      for (i = 0; i < prim_phy_pb->num_logical_blocks; i++) {
        mapped_logical_block_index = prim_phy_pb->logical_block[i];
        logical_block[mapped_logical_block_index].mapped_spice_model = verilog_model;
        logical_block[mapped_logical_block_index].mapped_spice_model_index = verilog_model->cnt;
      }
    }
    /* Update the verilog_model counter */
    verilog_model->cnt++;
    return;
  }
  
  /* Initialize */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);

  num_sram = count_num_sram_bits_one_spice_model(verilog_model, -1);

  switch (cur_sram_orgz_info->type) {
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

  /* Check */
  /* what is the SRAM bit of a mode? */
  /* If logical block is not NULL, we need to decode the sram bit */
  if (NULL != prim_phy_pb) {
    sram_bits = decode_mode_bits(prim_phy_pb->mode_bits, &expected_num_sram);
  } else { /* get default mode_bits */
    sram_bits = decode_mode_bits(prim_pb_type->mode_bits, &expected_num_sram);
  }
  assert(expected_num_sram == num_sram);

  /* SRAM_bit will be later reconfigured according to operating mode */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_MEMORY_BANK:
    cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info);
    for (i = 0; i < num_sram; i++) {
      /* Decode the SRAM bits to BL/WL bits.
       * first half part is BL, the other half part is WL 
       */
      decode_and_add_sram_membank_conf_bit_to_llist(cur_sram_orgz_info, cur_num_sram + i,
                                                    num_bl_per_sram, num_wl_per_sram,
                                                    sram_bits[i]);
    }
    break;
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    /* Store the configuraion bit to linked-list */
    add_mux_conf_bits_to_llist(0, cur_sram_orgz_info, 
                               num_sram, sram_bits,
                               verilog_model);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }
 
  /* Back-annotate to logical block */
  if (NULL != prim_phy_pb) {
    for (i = 0; i < prim_phy_pb->num_logical_blocks; i++) {
      mapped_logical_block_index = prim_phy_pb->logical_block[i];
      logical_block[mapped_logical_block_index].mapped_spice_model = verilog_model;
      logical_block[mapped_logical_block_index].mapped_spice_model_index = verilog_model->cnt;
    }
  }  
  /* Synchronize the internal counters of sram_orgz_info with generated bitstreams*/
  add_sram_conf_bits_to_sram_orgz_info(cur_sram_orgz_info, verilog_model);

  /* Print the encoding in SPICE netlist for debugging */
  if (NULL != prim_phy_pb) {
    fprintf(fp, "***** Logic Block %s *****\n", 
            prim_phy_pb->spice_name_tag);
  }
  fprintf(fp, "***** SRAM bits for %s[%d] *****\n", 
          verilog_model->name, verilog_model->cnt);
  fprintf(fp, "*****");
  for (i = 0; i < num_sram; i++) {
    fprintf(fp, "%d", sram_bits[i]);
  }
  fprintf(fp, "*****\n");


  /* Update the verilog_model counter */
  verilog_model->cnt++;

  /*Free*/ 
  my_free(sram_ports);
  my_free(sram_bits);
  my_free(bl_port);
  my_free(wl_port);

  return;
}

void fpga_spice_generate_bitstream_pb_primitive_lut(FILE* fp,
                                                    t_phy_pb* prim_phy_pb,
                                                    t_pb_type* prim_pb_type,
                                                    t_sram_orgz_info* cur_sram_orgz_info) {

  int i, j;
  int* lut_sram_bits = NULL; /* decoded SRAM bits */ 
  int* mode_sram_bits = NULL; /* decoded SRAM bits */ 
  int* sram_bits = NULL; /* decoded SRAM bits */ 
  int* truth_table_length = 0;
  char*** truth_table = NULL;
  int lut_size = 0;
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;
  t_spice_model_port* lut_sram_port = NULL;
  t_spice_model_port* mode_bit_port = NULL;
  int num_lut_pin_nets;
  int* lut_pin_net = NULL;
  int mapped_logical_block_index;

  int cur_num_sram = 0;
  int num_sram = 0;
  int num_lut_sram = 0;
  int num_mode_sram = 0;
  /* For each SRAM, we could have multiple BLs/WLs */
  int num_bl_ports = 0;
  t_spice_model_port** bl_port = NULL;
  int num_wl_ports = 0;
  t_spice_model_port** wl_port = NULL;
  int num_bl_per_sram = 0;
  int num_wl_per_sram = 0;
  t_spice_model* mem_model = NULL;
  int expected_num_sram = 0;
  t_spice_model* verilog_model = NULL;

  /* Ensure a valid physical pritimive pb */ 
  if (NULL == prim_pb_type) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid prim_pb_type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Asserts */
  if (NULL != prim_phy_pb) {
    assert (prim_phy_pb->pb_graph_node->pb_type->phy_pb_type == prim_pb_type);
  }
  assert (NULL != prim_pb_type->spice_model);

  verilog_model = prim_pb_type->spice_model; 
  assert(SPICE_MODEL_LUT == verilog_model->type);

  /* Find the input ports for LUT size */
  input_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  assert(1 == num_input_port);
  lut_size = input_ports[0]->size;

  /* Find SRAM ports for truth tables and mode bits */
  sram_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
  assert((1 == num_sram_port) || (2 == num_sram_port));
  for (i = 0; i < num_sram_port; i++) {
    if (FALSE == sram_ports[i]->mode_select) {
      lut_sram_port = sram_ports[i];
      num_lut_sram =  sram_ports[i]->size;
      assert (num_lut_sram == (int)pow(2.,(double)(lut_size)));
    } else {
      assert (TRUE == sram_ports[i]->mode_select);
      mode_bit_port = sram_ports[i];
      num_mode_sram = sram_ports[i]->size;
    }
  }
  /* Must have a lut_sram_port, while mode_bit_port is optional */
  assert (NULL != lut_sram_port);

  /* Count the number of configuration bits */
  num_sram = count_num_sram_bits_one_spice_model(verilog_model, -1);
  /* Get memory model */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);

  /* Find the number of BLs/WLs of each SRAM */
  switch (cur_sram_orgz_info->type) {
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
  
  /* If this is an idle LUT, we give an empty truth table */
  if ((NULL == prim_phy_pb) 
    || ((NULL != prim_phy_pb && (0 == prim_phy_pb->num_logical_blocks)))) {
    lut_sram_bits = (int*) my_calloc ( num_lut_sram, sizeof(int)); 
    for (i = 0; i < num_lut_sram; i++) { 
      lut_sram_bits[i] = lut_sram_port->default_val;
    }
  } else { 
    assert (NULL != prim_phy_pb);
    /* Allocate truth tables  */
    truth_table_length = (int*) my_malloc (sizeof(int) * prim_phy_pb->num_logical_blocks);
    truth_table = (char***) my_malloc (sizeof(char**) * prim_phy_pb->num_logical_blocks);

    /* Output log for debugging purpose */
    fprintf(fp, "***** Logic Block %s *****\n", 
            prim_phy_pb->spice_name_tag);
    
    /* Find truth tables and decode them one by one 
     * Fracturable LUT may have multiple truth tables,
     * which should be grouped in a unique one 
     */
    for (i = 0; i < prim_phy_pb->num_logical_blocks; i++) {
      mapped_logical_block_index = prim_phy_pb->logical_block[i]; 
      /* For wired LUT we provide a default truth table */
      if (TRUE == prim_phy_pb->is_wired_lut[i]) {
        /* TODO: assign post-routing lut truth table!!!*/
        get_mapped_lut_phy_pb_input_pin_vpack_net_num(prim_phy_pb, &num_lut_pin_nets, &lut_pin_net);
        truth_table[i] = assign_post_routing_wired_lut_truth_table(prim_phy_pb->rr_graph->rr_node[prim_phy_pb->lut_output_pb_graph_pin[i]->rr_node_index_physical_pb].vpack_net_num,
                                                                   num_lut_pin_nets, lut_pin_net, &truth_table_length[i]); 
      } else {
        assert (FALSE == prim_phy_pb->is_wired_lut[i]);
        assert (VPACK_COMB == logical_block[mapped_logical_block_index].type);
        /* Get the mapped vpack_net_num of this physical LUT pb */
        get_mapped_lut_phy_pb_input_pin_vpack_net_num(prim_phy_pb, &num_lut_pin_nets, &lut_pin_net);
        /* consider LUT pin remapping when assign lut truth tables */
        /* Match truth table and post-routing results */
        truth_table[i] = assign_post_routing_lut_truth_table(&logical_block[mapped_logical_block_index], 
                                                             num_lut_pin_nets, lut_pin_net, &truth_table_length[i]); 
      }
      /* Adapt truth table for a fracturable LUT
       * TODO: Determine fixed input bits for this truth table:
       * 1. input bits within frac_level (all '-' if not specified) 
       * 2. input bits outside frac_level, decoded to its output mask (0 -> first part -> all '1') 
       */
      adapt_truth_table_for_frac_lut(prim_phy_pb->lut_output_pb_graph_pin[i], 
                                     truth_table_length[i], truth_table[i]);
      /* Output log for debugging purpose */
      if (WIRED_LUT_LOGICAL_BLOCK_ID == mapped_logical_block_index) {
        fprintf(fp, "***** Wired LUT: mapped to a buffer *****\n");
      } else {
        fprintf(fp, "***** Mapped Logic Block[%d] %s *****\n",
                i, logical_block[mapped_logical_block_index].name);
      }
      fprintf(fp, "***** Net map *****\n");
      for (j = 0; j < num_lut_pin_nets; j++) {
        if (OPEN == lut_pin_net[j]) {
          fprintf(fp, "OPEN, ");
        } else {
          assert (OPEN != lut_pin_net[j]);
          fprintf(fp, "%s, ", vpack_net[lut_pin_net[j]].name);
        }
      }  
      fprintf(fp, "\n");
      fprintf(fp, "***** Truth Table *****\n");
      for (j = 0; j < truth_table_length[i]; j++) {
        fprintf(fp, "%s\n", truth_table[i][j]);
      }
    }
    /* Generate base sram bits*/
    lut_sram_bits = generate_frac_lut_sram_bits(prim_phy_pb, truth_table_length, truth_table, lut_sram_port->default_val);
  }
  
  /* Add mode bits */
  if (NULL != mode_bit_port) {
    if (NULL != prim_phy_pb) {
      mode_sram_bits = decode_mode_bits(prim_phy_pb->mode_bits, &expected_num_sram);
    } else { /* get default mode_bits */
      mode_sram_bits = decode_mode_bits(prim_pb_type->mode_bits, &expected_num_sram);
    }
    assert(expected_num_sram == num_mode_sram);
  }

  /* Merge the SRAM bits from LUT SRAMs and Mode selection */
  assert ( num_sram == num_lut_sram + num_mode_sram ); 
  sram_bits = (int*)my_calloc(num_sram, sizeof(int));
  /* LUT SRAMs go first and then mode bits */
  memcpy(sram_bits, lut_sram_bits, num_lut_sram * sizeof(int));
  if (NULL != mode_bit_port) {
    memcpy(sram_bits + num_lut_sram, mode_sram_bits, num_mode_sram * sizeof(int));
  }

  /* Decode the SRAM bits to BL/WL bits. */ 
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_MEMORY_BANK:
    cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info);
    for (i = 0; i < num_sram; i++) {
      /* TODO: should be more structural in coding !!! */
      /* Decode the SRAM bits to BL/WL bits.
       * first half part is BL, the other half part is WL 
       */
      decode_and_add_sram_membank_conf_bit_to_llist(cur_sram_orgz_info, cur_num_sram + i,
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
    add_mux_conf_bits_to_llist(0, cur_sram_orgz_info, 
                               num_sram, sram_bits,
                               verilog_model);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Synchronize the internal counters of sram_orgz_info with generated bitstreams*/
  add_sram_conf_bits_to_sram_orgz_info(cur_sram_orgz_info, verilog_model);

  /* Back-annotate to logical block */
  if (NULL != prim_phy_pb) {
    for (i = 0; i < prim_phy_pb->num_logical_blocks; i++) {
      mapped_logical_block_index = prim_phy_pb->logical_block[i];
      logical_block[mapped_logical_block_index].mapped_spice_model = verilog_model;
      logical_block[mapped_logical_block_index].mapped_spice_model_index = verilog_model->cnt;
    }
  }

  /* Print the encoding in SPICE netlist for debugging */
  fprintf(fp, "***** LUT SRAM bits for %s[%d] *****\n", 
          verilog_model->name, verilog_model->cnt);
  fprintf(fp, "*****");
  for (i = 0; i < num_lut_sram; i++) {
    fprintf(fp, "%d", lut_sram_bits[i]);
  }
  fprintf(fp, "*****\n");
  if (0 < num_mode_sram) {
    fprintf(fp, "***** LUT Mode bits for %s[%d] *****\n", 
            verilog_model->name, verilog_model->cnt);
    fprintf(fp, "*****");
    for (i = 0; i < num_mode_sram; i++) {
      fprintf(fp, "%d", mode_sram_bits[i]);
    }
    fprintf(fp, "*****\n");
  }

  /* Update counter */
  verilog_model->cnt++;

  /*Free*/
  if (NULL != prim_phy_pb) {
    my_free(lut_pin_net);
    for (i = 0; i < prim_phy_pb->num_logical_blocks; i++) {
      for (j = 0; j < truth_table_length[i]; j++) {
        my_free(truth_table[i][j]); 
      }
      my_free(truth_table[i]);
    }
    my_free(truth_table_length); 
  }
  my_free(input_ports);
  my_free(sram_ports);
  my_free(sram_bits);
  my_free(lut_sram_bits);
  my_free(mode_sram_bits);

  return;
}
