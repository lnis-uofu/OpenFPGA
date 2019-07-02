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
#include "fpga_x2p_lut_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "spice_utils.h"
#include "spice_mux.h"
#include "spice_pbtypes.h"
#include "spice_lut.h"


/***** Subroutines *****/

void fprint_spice_lut_subckt(FILE* fp,
                             t_spice_model* spice_model) {
  int num_input_port = 0;
  t_spice_model_port** input_port = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_port = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_port = NULL;

  int iport, ipin;
  int sram_port_index = OPEN;
  int mode_port_index = OPEN;
  int mode_lsb = 0;
  int num_dumped_port = 0;
  char* mode_inport_postfix = "_mode";

  int jport, jpin, pin_cnt;
  int modegate_num_input_port = 0;
  int modegate_num_input_pins = 0;
  int modegate_num_output_port = 0;
  t_spice_model_port** modegate_input_port = NULL;
  t_spice_model_port** modegate_output_port = NULL;
  char* required_gate_type = NULL;
  enum e_spice_model_gate_type required_gate_model_type;

  float total_width;
  int width_cnt;

  /* Ensure a valid file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler!\n",
               __FILE__, __LINE__);
  }
  
  /* Find input ports, output ports and sram ports*/
  input_port = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_port = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  sram_port = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);

  /* Check */
  if (FALSE == spice_model->design_tech_info.lut_info->frac_lut) {
    /* when fracturable LUT is considered
     * More than 1 output is allowed  
     * Only two SRAM ports are allowed
     */
    assert(1 == num_input_port);
    assert(1 == num_output_port);
    assert(1 == num_sram_port); 
  } else {
    assert (TRUE == spice_model->design_tech_info.lut_info->frac_lut);
    /* when fracturable LUT is considered
     * More than 1 output is allowed  
     * Only two SRAM ports are allowed
     */
    assert(1 == num_input_port);
    for (iport = 0; iport < num_output_port; iport++) {
      assert(0 < output_port[iport]->size);
    }
    assert(2 == num_sram_port); 
  }

  fprintf(fp, "***** Auto-generated LUT info: spice_model_name = %s, size = %d *****\n",
          spice_model->name, input_port[0]->size);
  /* Define the subckt*/
  fprintf(fp, ".subckt %s ", spice_model->name); /* Subckt name*/
  /* Input ports*/ 
  for (iport = 0; iport < num_input_port; iport++) {
    for (ipin = 0; ipin < input_port[iport]->size; ipin++) {
      fprintf(fp, "%s%d ", input_port[iport]->prefix, ipin); 
    }
  }
  /* output ports*/
  for (iport = 0; iport < num_output_port; iport++) {
    for (ipin = 0; ipin < output_port[iport]->size; ipin++) {
      fprintf(fp, "%s%d ", output_port[iport]->prefix, ipin);
    }
  }
  /* sram ports */
  /* Print configuration ports*/
  num_dumped_port = 0;
  for (iport = 0; iport < num_sram_port; iport++) {
    /* By pass mode select ports */
    if (TRUE == sram_port[iport]->mode_select) {
      continue;
    } 
    assert(FALSE == sram_port[iport]->mode_select); 
    for (ipin = 0; ipin < sram_port[iport]->size; ipin++) {
      fprintf(fp, "%s%d ", sram_port[iport]->prefix, ipin); 
    }
    sram_port_index = iport;
    num_dumped_port++;
  }
  /* Print mode configuration ports*/
  num_dumped_port = 0;
  for (iport = 0; iport < num_sram_port; iport++) {
    /* By pass mode select ports */
    if (FALSE == sram_port[iport]->mode_select) {
      continue;
    } 
    assert(TRUE == sram_port[iport]->mode_select); 
    for (ipin = 0; ipin < sram_port[iport]->size; ipin++) {
      fprintf(fp, "%s_out%d ", sram_port[iport]->prefix, ipin); 
    }
    mode_port_index = iport;
    num_dumped_port++;
  }
  /* Check if all required SRAMs ports*/
  if (TRUE == spice_model->design_tech_info.lut_info->frac_lut) {
    if (1 != num_dumped_port) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                "(FILE:%s,LINE[%d]) Fracturable LUT (spice_model_name=%s) must have 1 mode port!\n",
                __FILE__, __LINE__, spice_model->name); 
      exit(1);
    }
  }

  /* local vdd and gnd*/
  fprintf(fp, "svdd sgnd\n");

  /* Input buffers */
  assert (1 == num_input_port);
  assert (NULL != input_port);

  /* If this is a regular LUT, we give a default tri_state_map */
  if (FALSE == spice_model->design_tech_info.lut_info->frac_lut) {
    for (iport = 0; iport < num_input_port; iport++) {
      if (NULL == input_port[iport]->tri_state_map) {
        input_port[iport]->tri_state_map = (char*)my_calloc(input_port[iport]->size, sizeof(char)); 
      } else { 
        vpr_printf(TIO_MESSAGE_WARNING, 
                   "(File:%s, [LINE%d])Overwrite the tri-state map for the LUT inputs (name=%s)!\n",
                   __FILE__, __LINE__, spice_model->name);
      }
      for (ipin = 0; ipin < input_port[iport]->size; ipin++) {
        input_port[iport]->tri_state_map[ipin] = '-';
      }
    }
  }

  /* Add AND/OR gates if this is a fracturable LUT */
  for (iport = 0; iport < num_input_port; iport++) {
    /* Initialize lsb */
    mode_lsb = 0;
    for (ipin = 0; ipin < input_port[iport]->size; ipin++) {
      if ('0' == input_port[iport]->tri_state_map[ipin]) {  
        required_gate_type = "AND"; 
        required_gate_model_type = SPICE_MODEL_GATE_AND; 
      }
      if ('1' == input_port[iport]->tri_state_map[ipin]) {  
        required_gate_type = "OR"; 
        required_gate_model_type = SPICE_MODEL_GATE_OR; 
      }
      /* First check if we have the required gates*/
      switch (input_port[iport]->tri_state_map[ipin]) {  
      case '-':
        break;
      case '0':
      case '1':
       /* Check: we must have an AND2/OR2 gate */
        if (NULL == input_port[iport]->spice_model) {
          vpr_printf(TIO_MESSAGE_ERROR,
                     "(FILE: %s, [LINE%d]) %s gate for the input port (name=%s) of spice model (name=%s) is not defined!\n",
                     __FILE__, __LINE__, required_gate_type,
                     input_port[iport]->prefix, spice_model->name);
          exit(1);
        }
        if ((SPICE_MODEL_GATE != input_port[iport]->spice_model->type)
          || (required_gate_model_type != input_port[iport]->spice_model->design_tech_info.gate_info->type)) {
          vpr_printf(TIO_MESSAGE_ERROR,
                     "(FILE: %s, [LINE%d]) %s gate for the input port (name=%s) of spice model (name=%s) is not defined as a AND logic gate!\n",
                     __FILE__, __LINE__, required_gate_type,
                     input_port[iport]->prefix, spice_model->name);
          exit(1);
        }
        /* Check input ports */
        modegate_input_port = find_spice_model_ports(input_port[iport]->spice_model, SPICE_MODEL_PORT_INPUT, &modegate_num_input_port, TRUE);
        modegate_num_input_pins = 0;
        for (jport = 0; jport < modegate_num_input_port; jport++) {
          modegate_num_input_pins += modegate_input_port[jport]->size; 
        }
        if (2 != modegate_num_input_pins) { 
          vpr_printf(TIO_MESSAGE_ERROR,
                     "(FILE: %s, [LINE%d]) %s gate for the input port (name=%s) of spice model (name=%s) should have only 2 input pins!\n",
                     __FILE__, __LINE__, required_gate_type,
                     input_port[iport]->prefix, spice_model->name);
          exit(1);
        }
        /* Check output ports */
        modegate_output_port = find_spice_model_ports(input_port[iport]->spice_model, SPICE_MODEL_PORT_OUTPUT, &modegate_num_output_port, TRUE);
        if (  (1 != modegate_num_output_port)
           || (1 != modegate_output_port[0]->size)) {
          vpr_printf(TIO_MESSAGE_ERROR,
                     "(FILE: %s, [LINE%d]) %s gate for the input port (name=%s) of spice model (name=%s) should have only 1 output!\n",
                     __FILE__, __LINE__, required_gate_type,
                     input_port[iport]->prefix, spice_model->name);
          exit(1);
        }
        /* Instance the AND2/OR2 gate */
        fprintf(fp, "X%s_%s%d ",
                input_port[iport]->spice_model->prefix, 
                input_port[iport]->prefix, ipin);
        pin_cnt = 0;
        for (jport = 0; jport < modegate_num_input_port; jport++) {
          for (jpin = 0; jpin < modegate_input_port[jport]->size; jpin++) {
            if (0 == pin_cnt) {
              fprintf(fp, "%s%d",
                      input_port[iport]->prefix, ipin);
            } else if (1 == pin_cnt) { 
              fprintf(fp, " %s_out%d",
                      sram_port[mode_port_index]->prefix, mode_lsb);
            }
            pin_cnt++;
          }
        }
        assert(2 == pin_cnt);
        fprintf(fp, " %s%s%d",
                input_port[0]->prefix, mode_inport_postfix, ipin); 
        mode_lsb++;
        /* local vdd and gnd*/
        fprintf(fp, " svdd sgnd");
        /* Call subckt name */
        fprintf(fp, " %s\n", input_port[iport]->spice_model->name); 
        /* Free ports */
        my_free(modegate_input_port);
        my_free(modegate_output_port);
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, 
                  "(file:%s,line[%d]) invalid LUT tri_state_map = %s ",
                  __FILE__, __LINE__, input_port[iport]->tri_state_map); 
        exit(1);
      }
    }
    /* Check if we have dumped all the SRAM ports for mode selection */
    if ((OPEN != mode_port_index)
       &&(mode_lsb != sram_port[mode_port_index]->size)) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                "(FILE:%s,LINE[%d]) SPICE model LUT (name=%s) has a unmatched tri-state map (%s) implied by mode_port size(%d)!\n",
                __FILE__, __LINE__, 
                spice_model->name, input_port[iport]->tri_state_map[ipin], input_port[iport]->size); 
      exit(1);
    }

    /* Create inverters between input port and its inversion */
    for (ipin = 0; ipin < input_port[iport]->size; ipin++) {
      switch (input_port[iport]->tri_state_map[ipin]) {  
      case '-':
        /* For negative input of LUT MUX*/
        /* Output inverter with maximum size allowed 
         * until the rest of width is smaller than threshold */
        total_width = spice_model->lut_input_buffer->size * spice_model->lut_input_buffer->f_per_stage;
        width_cnt = 0;
        while (total_width > max_width_per_trans) { 
          fprintf(fp, "Xinv0_in%d_no%d %s%d lut_mux_in%d_inv svdd sgnd inv size=\'%g\'",
                  ipin, width_cnt, 
                  input_port[iport]->prefix, ipin, 
                  ipin, max_width_per_trans);
          fprintf(fp, "\n");
          /* Update */
          total_width = total_width - max_width_per_trans;
          width_cnt++;
        }
        /* Print if we still have to */
        if (total_width > 0) {
          fprintf(fp, "Xinv0_in%d_no%d %s%d lut_mux_in%d_inv svdd sgnd inv size=\'%g\'",
                  ipin, width_cnt, 
                  input_port[iport]->prefix, ipin, 
                  ipin, total_width);
          fprintf(fp, "\n");
        }
        /* For postive input of LUT MUX, we use the tapered_buffer subckt directly */
        assert(1 == spice_model->lut_input_buffer->tapered_buf);
        fprintf(fp, "X%s_in%d %s%d lut_mux_in%d svdd sgnd tapbuf_level%d_f%d\n",
                spice_model->lut_input_buffer->spice_model->prefix, ipin,
                input_port[iport]->prefix, ipin, ipin,
                spice_model->lut_input_buffer->tap_buf_level, 
                spice_model->lut_input_buffer->f_per_stage); 
        fprintf(fp, "\n");
        break;
      case '0':
      case '1':
        /* For negative input of LUT MUX*/
        /* Output inverter with maximum size allowed 
         * until the rest of width is smaller than threshold */
        total_width = spice_model->lut_input_buffer->size * spice_model->lut_input_buffer->f_per_stage;
        width_cnt = 0;
        while (total_width > max_width_per_trans) { 
          fprintf(fp, "Xinv0_in%d_no%d %s%s%d lut_mux_in%d_inv svdd sgnd inv size=\'%g\'",
                  ipin, width_cnt, 
                  input_port[iport]->prefix, mode_inport_postfix, ipin, 
                  ipin, max_width_per_trans);
          fprintf(fp, "\n");
          /* Update */
          total_width = total_width - max_width_per_trans;
          width_cnt++;
        }
        /* Print if we still have to */
        if (total_width > 0) {
          fprintf(fp, "Xinv0_in%d_no%d %s%s%d lut_mux_in%d_inv svdd sgnd inv size=\'%g\'",
                  ipin, width_cnt, 
                  input_port[iport]->prefix, mode_inport_postfix, ipin, 
                  ipin, total_width);
          fprintf(fp, "\n");
        }
        /* For postive input of LUT MUX, we use the tapered_buffer subckt directly */
        assert(1 == spice_model->lut_input_buffer->tapered_buf);
        fprintf(fp, "X%s_in%d %s%s%d lut_mux_in%d svdd sgnd tapbuf_level%d_f%d\n",
                spice_model->lut_input_buffer->spice_model->prefix, ipin,
                input_port[iport]->prefix, mode_inport_postfix, ipin, ipin,
                spice_model->lut_input_buffer->tap_buf_level, 
                spice_model->lut_input_buffer->f_per_stage); 
        fprintf(fp, "\n");
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, 
                  "(file:%s,line[%d]) invalid LUT tri_state_map = %s ",
                  __FILE__, __LINE__, input_port[iport]->tri_state_map); 
        exit(1);
      }
    }
  }


  /* Output buffers already included in LUT MUX */
  /* LUT MUX*/
  assert(sram_port[sram_port_index]->size == (int)pow(2.,(double)(input_port[0]->size)));
  fprintf(fp, "Xlut_mux ");
  /* SRAM ports of LUT, they are inputs of lut_muxes*/
  for (ipin = 0; ipin < sram_port[sram_port_index]->size; ipin++) {
    assert(FALSE == sram_port[sram_port_index]->mode_select); 
    fprintf(fp, "%s%d ", sram_port[sram_port_index]->prefix, ipin);
  } 
  /* Output port, LUT output is LUT MUX output*/
  for (iport = 0; iport < num_output_port; iport++) {
    for (ipin = 0; ipin < output_port[iport]->size; ipin++) {
      fprintf(fp, "%s%d ", output_port[iport]->prefix, ipin);
    }
  }
  /* Connect MUX configuration port to LUT inputs */
  /* input port, LUT input is LUT MUX sram*/
  for (iport = 0; iport < num_input_port; iport++) {
    for (ipin = 0; ipin < input_port[iport]->size; ipin++) {
      fprintf(fp, "lut_mux_in%d lut_mux_in%d_inv ", ipin, ipin); 
    }
  }

  /* Local vdd and gnd*/
  fprintf(fp, "svdd sgnd %s_mux_size%d\n", 
          spice_model->name, sram_port[sram_port_index]->size);

  /* End of LUT subckt*/
  fprintf(fp, ".eom\n");

  /* Free */
  my_free(input_port);
  my_free(output_port);
  my_free(sram_port);

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
      fprint_spice_lut_subckt(fp, &(spice_models[imodel]));
    }
  }

  /* Close*/
  fclose(fp);

  return;
}

void fprint_pb_primitive_lut(FILE* fp,
                             char* subckt_prefix,
                             t_phy_pb* prim_phy_pb,
                             t_pb_type* prim_pb_type,
                             int index,
                             t_spice_model* spice_model) {
  int i, j;
  int* lut_sram_bits = NULL; /* decoded SRAM bits */ 
  int* mode_sram_bits = NULL; /* decoded SRAM bits */ 
  int* sram_bits = NULL; /* decoded SRAM bits */ 
  int* truth_table_length = 0;
  char*** truth_table = NULL;

  int lut_size = 0;
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;
  t_spice_model_port* lut_sram_port = NULL;
  t_spice_model_port* mode_bit_port = NULL;
  int num_lut_pin_nets;
  int* lut_pin_net = NULL;
  int mapped_logical_block_index;

  char* formatted_subckt_prefix = format_spice_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  t_pb_type* cur_pb_type = prim_pb_type;
  char* port_prefix = NULL;

  int cur_num_sram = 0;
  int num_sram = 0;
  int num_lut_sram = 0;
  int num_mode_sram = 0;
  int expected_num_sram = 0;
  char* sram_vdd_port_name = NULL;

  /* Ensure a valid file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Asserts */
  assert(SPICE_MODEL_LUT == spice_model->type);

  /* Determine size of LUT*/
  input_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  sram_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
  assert(1 == num_input_port);
  lut_size = input_ports[0]->size;
  num_sram = (int)pow(2.,(double)(lut_size));
  /* Find SRAM ports for truth tables and mode bits */
  sram_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);
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
  num_sram = count_num_sram_bits_one_spice_model(spice_model, -1);

  /* Get current counter of mem_bits, bl and wl */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(sram_spice_orgz_info); 

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
      fprintf(fp, "***** Net map *****\n*");
      for (j = 0; j < num_lut_pin_nets; j++) {
        if (OPEN == lut_pin_net[j]) {
          fprintf(fp, " OPEN, ");
        } else {
          assert (OPEN != lut_pin_net[j]);
          fprintf(fp, " %s, ", vpack_net[lut_pin_net[j]].name);
        }
      }  
      fprintf(fp, "\n");
      fprintf(fp, "***** Truth Table *****\n");
      for (j = 0; j < truth_table_length[i]; j++) {
        fprintf(fp, "*%s\n", truth_table[i][j]);
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
  my_free(formatted_subckt_prefix);
  my_free(input_ports);
  my_free(output_ports);
  my_free(sram_ports);
  my_free(sram_bits);
  my_free(port_prefix);
  my_free(sram_vdd_port_name);

  return;
}
