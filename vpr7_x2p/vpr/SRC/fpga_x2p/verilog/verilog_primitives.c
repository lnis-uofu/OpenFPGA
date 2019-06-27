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
#include "route_common.h"
#include "vpr_utils.h"

/* Include spice support headers*/
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "fpga_x2p_globals.h"

/* Include verilog support headers*/
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_pbtypes.h"
#include "verilog_primitives.h"

/* Subroutines */
/* Dump a hard logic primitive node
 * This primitive can be a FF or a hard_logic, or an iopad */
void dump_verilog_pb_generic_primitive(t_sram_orgz_info* cur_sram_orgz_info,
                                       FILE* fp,
                                       char* subckt_prefix,
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
  
  int num_sram = 0;
  /* int* sram_bits = NULL; */

  char* formatted_subckt_prefix = format_verilog_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  t_pb_type* prim_pb_type = NULL;
  char* port_prefix = NULL;

  /* For each SRAM, we could have multiple BLs/WLs */
  int num_bl_ports = 0;
  t_spice_model_port** bl_port = NULL;
  int num_wl_ports = 0;
  t_spice_model_port** wl_port = NULL;

  int cur_num_sram = 0;
  int num_conf_bits = 0;
  int num_reserved_conf_bits = 0;
  t_spice_model* mem_model = NULL;
  int cur_bl, cur_wl;

  char* mem_subckt_name = NULL;

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
  assert((SPICE_MODEL_IOPAD == verilog_model->type) /* Support IO PAD which matches the physical design */
        || (SPICE_MODEL_FF == verilog_model->type) /* Support IO PAD which matches the physical design */
        || (SPICE_MODEL_HARDLOGIC == verilog_model->type)); /* Support IO PAD which matches the physical design */
  
  /* Initialize */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);

  prim_pb_type = prim_pb_graph_node->pb_type;

  /* Generate Subckt for pb_type*/
  /* Simplify the port prefix, make SPICE netlist readable */
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(prim_pb_type->name) + 1));
  sprintf(port_prefix, "%s", prim_pb_type->name);

  /* Print Comment lines depends on the type of this SPICE model */
  fprintf(fp, "//----- %s Verilog module: %s%s -----\n", 
          generate_string_spice_model_type(verilog_model->type),
          formatted_subckt_prefix, port_prefix);
  /* Definition line */
  fprintf(fp, "module %s%s (", formatted_subckt_prefix, port_prefix);
  fprintf(fp, "\n");
  /* Only dump the global ports belonging to a spice_model 
   */
  if (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, TRUE, TRUE, FALSE)) {
    fprintf(fp, ",\n");
  }

  /* TODO: assert this is physical mode */
  num_sram = count_num_sram_bits_one_spice_model(verilog_model, -1);
  /* Get current counter of mem_bits, bl and wl */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl);

  /* print ports --> input ports */
  dump_verilog_pb_type_ports(fp, port_prefix, 0, prim_pb_type, TRUE, FALSE, FALSE); 

  /* IOPADs requires a specical port to output */
  if (SPICE_MODEL_IOPAD == verilog_model->type) {
    fprintf(fp, ",\n");
    /* Print output port */
    fprintf(fp, "inout [%d:%d] %s%s\n",
            verilog_model->cnt, verilog_model->cnt,
            gio_inout_prefix, verilog_model->prefix);
  }

  /* Print SRAM ports */
  /* connect to reserved BL/WLs ? */
  num_reserved_conf_bits = count_num_reserved_conf_bits_one_spice_model(verilog_model, cur_sram_orgz_info->type, 0);
  /* Get the number of configuration bits required by this MUX */
  num_conf_bits = count_num_conf_bits_one_spice_model(verilog_model, cur_sram_orgz_info->type, 0);
  /* Reserved sram ports */
  if (0 < num_reserved_conf_bits) {
    fprintf(fp, ",\n");
    dump_verilog_reserved_sram_ports(fp, cur_sram_orgz_info, 
                                     0, num_reserved_conf_bits - 1,
                                     VERILOG_PORT_INPUT);
  }
  /* Normal sram ports */
  if (0 < num_conf_bits) {
    fprintf(fp, ",\n");
    dump_verilog_sram_ports(fp, cur_sram_orgz_info, 
                            cur_num_sram, cur_num_sram + num_sram - 1,
                            VERILOG_PORT_INPUT);

  }
  /* Dump ports only visible during formal verification*/
  if (0 < num_conf_bits) {
    fprintf(fp, "\n");
    fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
    fprintf(fp, ",\n");
    dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                                cur_num_sram,
                                                cur_num_sram + num_conf_bits - 1,
                                                VERILOG_PORT_INPUT);
    fprintf(fp, "\n");
    fprintf(fp, "`endif\n");
  }

  /* Local vdd and gnd*/
  fprintf(fp, ");\n");

  if (0 < num_sram_port) {
    dump_verilog_sram_config_bus_internal_wires(fp, cur_sram_orgz_info, 
                                                cur_num_sram, cur_num_sram + num_sram - 1);
  }

  if (0 < num_sram_port) {
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
      break;
    case SPICE_SRAM_STANDALONE:
    case SPICE_SRAM_SCAN_CHAIN:
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid SRAM organization type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  } 
  /* Definition ends*/

  /* Dump the configuration port bus */
  if ((0 < num_reserved_conf_bits) 
   || (0 < num_conf_bits)) {
    dump_verilog_mem_config_bus(fp, mem_model, cur_sram_orgz_info,
                                cur_num_sram, num_reserved_conf_bits, num_conf_bits); 
    /* Dump ports only visible during formal verification*/
    fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
    dump_verilog_formal_verification_sram_ports_wiring(fp, cur_sram_orgz_info, 
                                                       cur_num_sram,
                                                       cur_num_sram + num_conf_bits - 1);
    fprintf(fp, "`endif\n");
  }

  /* Call the subckt*/
  fprintf(fp, "%s %s_%d_ (", verilog_model->name, verilog_model->prefix, verilog_model->cnt);
  fprintf(fp, "\n");
  /* Only dump the global ports belonging to a spice_model 
   * Disable recursive here !
   */
  if (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, FALSE, FALSE, TRUE)) {
    fprintf(fp, ",\n");
  }
  
  /* assert */
  num_sram = count_num_sram_bits_one_spice_model(verilog_model, -1);
  /* print ports --> input ports */
  dump_verilog_pb_type_bus_ports(fp, port_prefix, 1, prim_pb_type, FALSE, FALSE, verilog_model->dump_explicit_port_map); 

  /* IOPADs requires a specical port to output */
  if (SPICE_MODEL_IOPAD == verilog_model->type) {
    fprintf(fp, ",\n");
    assert(1 == num_pad_port);
    assert(NULL != pad_ports[0]);
    /* Add explicit port mapping if required */
    if (TRUE == verilog_model->dump_explicit_port_map) {
      fprintf(fp, ".%s(",
              pad_ports[0]->lib_name);
    }
    /* Print inout port */
    fprintf(fp, "%s%s[%d]", gio_inout_prefix, 
                verilog_model->prefix, verilog_model->cnt);
    if (TRUE == verilog_model->dump_explicit_port_map) {
      fprintf(fp, ")");
    }
    fprintf(fp, ", ");
  }
  /* Print SRAM ports */
  /* Connect srams: TODO: to find the SRAM model used by this Verilog model */
  if (0 < num_sram) {
    switch (cur_sram_orgz_info->type) {
    case SPICE_SRAM_STANDALONE: 
      break;
    case SPICE_SRAM_SCAN_CHAIN:
      /* Add explicit port mapping if required */
      if ( (0 < num_sram) 
        && (TRUE == verilog_model->dump_explicit_port_map)) {
        assert( 1 == num_sram_port);
        assert( NULL != sram_ports[0]);
        fprintf(fp, ".%s(",
                sram_ports[0]->lib_name);
      }
      dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info,
                                          cur_num_sram, cur_num_sram + num_sram - 1,
                                          0, VERILOG_PORT_CONKT);
      if ( (0 < num_sram) 
        && (TRUE == verilog_model->dump_explicit_port_map)) {
        fprintf(fp, ")");
      }

      /* Check if we have an inverterd prefix */
      if (NULL == sram_ports[0]->inv_prefix) {
        break;
      }
      fprintf(fp, ", ");
      /* Add explicit port mapping if required */
      if ( (0 < num_sram) 
        && (TRUE == verilog_model->dump_explicit_port_map)) {
        assert( 1 == num_sram_port);
        assert( NULL != sram_ports[0]);
        fprintf(fp, ".%s(",
                sram_ports[0]->inv_prefix);
      }
      dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info,
                                          cur_num_sram, cur_num_sram + num_sram - 1,
                                          1, VERILOG_PORT_CONKT);
      if ( (0 < num_sram) 
        && (TRUE == verilog_model->dump_explicit_port_map)) {
        fprintf(fp, ")");
      }
      break;
    case SPICE_SRAM_MEMORY_BANK:
      /* Add explicit port mapping if required */
      if ( (0 < num_sram) 
        && (TRUE == verilog_model->dump_explicit_port_map)) {
        assert( 1 == num_sram_port);
        assert( NULL != sram_ports[0]);
        fprintf(fp, ".%s(",
                sram_ports[0]->lib_name);
      }
      dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, 
                                    cur_num_sram, cur_num_sram + num_sram - 1, 
                                    0, VERILOG_PORT_CONKT);
      if ( (0 < num_sram) 
        && (TRUE == verilog_model->dump_explicit_port_map)) {
        fprintf(fp, ")");
      }
      /* Check if we have an inverterd prefix */
      if (NULL == sram_ports[0]->inv_prefix) {
        break;
      }
      fprintf(fp, ", ");
      /* Add explicit port mapping if required */
      if ( (0 < num_sram) 
        && (TRUE == verilog_model->dump_explicit_port_map)) {
        assert( 1 == num_sram_port);
        assert( NULL != sram_ports[0]);
        fprintf(fp, ".%s(",
                sram_ports[0]->inv_prefix);
      }
      dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, 
                                    cur_num_sram, cur_num_sram + num_sram - 1, 
                                    1, VERILOG_PORT_CONKT);
      if ( (0 < num_sram) 
        && (TRUE == verilog_model->dump_explicit_port_map)) {
        fprintf(fp, ")");
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid SRAM organization type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }
  
  /* Local vdd and gnd, verilog_model name, 
   * TODO: Global vdd for i/o pad to split?
   */
  fprintf(fp, ");\n");

  /* Call SRAM subckt */
  /* what is the SRAM bit of a mode? */
  /* If logical block is not NULL, we need to decode the sram bit */
  /* SRAM bits are decoded in bitstream generator! NOT here 
  if (NULL != mapped_logical_block) {
    assert(NULL != mapped_logical_block->pb->pb_graph_node->pb_type->mode_bits);
    sram_bits = decode_mode_bits(mapped_logical_block->pb->pb_graph_node->pb_type->mode_bits, &expected_num_sram);
    assert(expected_num_sram == num_sram);
  } else {
    sram_bits = (int*)my_calloc(num_sram, sizeof(int));
    for (i = 0; i < num_sram; i++) { 
      sram_bits[i] = sram_ports[0]->default_val;
    }
  }
  */

  /* Call the memory module defined for this SRAM-based MUX! */
  if (0 < num_sram_port) {
    mem_subckt_name = generate_verilog_mem_subckt_name(verilog_model, mem_model, verilog_mem_posfix);
    fprintf(fp, "%s %s_%d_ ( ", 
            mem_subckt_name, mem_subckt_name, verilog_model->cnt);
    dump_verilog_mem_sram_submodule(fp, cur_sram_orgz_info, verilog_model, -1, 
                                    mem_model, cur_num_sram, cur_num_sram + num_sram - 1); 
    fprintf(fp, ");\n");
    /* update the number of memory bits */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_sram + num_sram);
  }

  /* End */
  fprintf(fp, "endmodule\n");
  /* Comment lines */
  fprintf(fp, "//----- END %s Verilog module: %s%s -----\n\n", 
          generate_string_spice_model_type(verilog_model->type),
          formatted_subckt_prefix, port_prefix);

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
  /* my_free(sram_bits); */

  return;
}

void dump_verilog_pb_primitive_lut(t_sram_orgz_info* cur_sram_orgz_info,
                                   FILE* fp,
                                   char* subckt_prefix,
                                   t_pb_graph_node* prim_pb_graph_node,
                                   int index,
                                   t_spice_model* verilog_model) {
  int i;
  int lut_size = 0;
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  int num_lut_sram = 0;
  int num_mode_sram = 0;
  t_spice_model_port* lut_sram_port = NULL;

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
  char* mem_subckt_name = NULL;

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
  assert(SPICE_MODEL_LUT == verilog_model->type);

  /* Determine size of LUT*/
  input_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_ports = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
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

  /* Disable Generate sram bits*/
  /* SRAM bits are decoded in bitstream generator! NOT here 
  sram_bits = generate_lut_sram_bits(truth_table_length, truth_table, 
                                     lut_size, sram_ports[0]->default_val);
  */

  /* Print the subckts*/ 
  cur_pb_type = prim_pb_graph_node->pb_type;

  /* Comment lines */
  fprintf(fp, "//----- LUT Verilog module: %s%s_%d_ -----\n",
          formatted_subckt_prefix, cur_pb_type->name, index);

  /* Simplify the prefix, make the SPICE netlist readable*/
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(cur_pb_type->name) + 1));
  sprintf(port_prefix, "%s", cur_pb_type->name);

  /* Subckt definition*/
  fprintf(fp, "module %s%s (", 
          formatted_subckt_prefix, cur_pb_type->name);
  fprintf(fp, "\n");
  /* Only dump the global ports belonging to a spice_model */
  if (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, TRUE, TRUE, FALSE)) {
    fprintf(fp, ",\n");
  }
  /* Print inputs, outputs, inouts, clocks, NO SRAMs*/
  dump_verilog_pb_type_ports(fp, port_prefix, 0, cur_pb_type, TRUE, TRUE, FALSE); 
  /* Print SRAM ports */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl);
  /* connect to reserved BL/WLs ? */
  num_reserved_conf_bits = count_num_reserved_conf_bits_one_spice_model(verilog_model, cur_sram_orgz_info->type, 0);
  /* Get the number of configuration bits required by this MUX */
  num_conf_bits = count_num_conf_bits_one_spice_model(verilog_model, cur_sram_orgz_info->type, 0);
  /* Reserved sram ports */
  if ( 0 < num_reserved_conf_bits) {
    dump_verilog_reserved_sram_ports(fp, cur_sram_orgz_info, 
                                     0, num_reserved_conf_bits - 1,
                                     VERILOG_PORT_INPUT);
    fprintf(fp, ",\n");
  }
  /* Normal sram ports */
  if (0 < num_conf_bits) {
    dump_verilog_sram_ports(fp, cur_sram_orgz_info, 
                            cur_num_sram, cur_num_sram + num_conf_bits - 1,
                            VERILOG_PORT_INPUT);
  }
  /* Dump ports only visible during formal verification*/
  if (0 < num_conf_bits) {
    fprintf(fp, "\n");
    fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
    fprintf(fp, ",\n");
    dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                                cur_num_sram,
                                                cur_num_sram + num_conf_bits - 1,
                                                VERILOG_PORT_INPUT);
    fprintf(fp, "\n");
    fprintf(fp, "`endif\n");
  }

  /* Local Vdd and gnd*/ 
  fprintf(fp, ");\n");
  /* Definition ends*/

  /* Specify SRAM output are wires */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info);
  dump_verilog_sram_config_bus_internal_wires(fp, cur_sram_orgz_info, cur_num_sram, cur_num_sram + num_sram - 1);

  /* Dump ports only visible during formal verification*/
  if ((0 < num_reserved_conf_bits) 
   || (0 < num_conf_bits)) {
    dump_verilog_mem_config_bus(fp, mem_model, cur_sram_orgz_info,
                                cur_num_sram, num_reserved_conf_bits, num_conf_bits); 

    fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
    dump_verilog_formal_verification_sram_ports_wiring(fp, cur_sram_orgz_info, 
                                                       cur_num_sram,
                                                       cur_num_sram + num_sram - 1);
    fprintf(fp, "`endif\n");
  }

  /*
  fprintf(fp, "wire [%d:%d] %s_out;\n",
          cur_num_sram, cur_num_sram + num_sram - 1, mem_model->prefix);
  fprintf(fp, "wire [%d:%d] %s_outb;\n",
          cur_num_sram, cur_num_sram + num_sram - 1, mem_model->prefix);
  */

  num_sram = count_num_sram_bits_one_spice_model(verilog_model, -1);
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info);

  /* Call LUT subckt*/
  fprintf(fp, "%s %s_%d_ (", verilog_model->name, verilog_model->prefix, verilog_model->cnt);
  fprintf(fp, "\n");
  /* if we have to add global ports when dumping submodules of LUTs
   * otherwise, the port map here does not match that of submodules 
   * Only dump the global ports belonging to a spice_model 
   * DISABLE recursive here !
   */
  if (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, FALSE, FALSE, FALSE)) {
    fprintf(fp, ",\n");
  }
  /* Connect inputs*/ 
  /* Connect outputs*/
  fprintf(fp, "//----- Input and output ports -----\n");
  dump_verilog_pb_type_bus_ports(fp, port_prefix, 1, cur_pb_type, FALSE, TRUE, verilog_model->dump_explicit_port_map); 
  fprintf(fp, "\n//----- SRAM ports -----\n");

  /* check */
  assert (num_sram == num_lut_sram + num_mode_sram);
  /* Connect srams: TODO: to find the SRAM model used by this Verilog model */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info);
  /* TODO: switch depending on the type of configuration circuit */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE: 
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 
                                        cur_num_sram, cur_num_sram + num_lut_sram - 1, 
                                        0, VERILOG_PORT_CONKT);
    fprintf(fp, ", ");
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 
                                        cur_num_sram, cur_num_sram + num_lut_sram - 1, 
                                        1, VERILOG_PORT_CONKT);
    if (0 < num_mode_sram) {
      fprintf(fp, ", ");
      dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 
                                          cur_num_sram + num_lut_sram, cur_num_sram + num_lut_sram + num_mode_sram - 1, 
                                          0, VERILOG_PORT_CONKT);
      fprintf(fp, ", ");
      dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 
                                          cur_num_sram + num_lut_sram, cur_num_sram + num_lut_sram + num_mode_sram - 1, 
                                          1, VERILOG_PORT_CONKT);
    }
    break;
  case SPICE_SRAM_MEMORY_BANK:
    dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, 
                                  cur_num_sram, cur_num_sram + num_lut_sram - 1, 
                                  0, VERILOG_PORT_CONKT);
    fprintf(fp, ", ");
    dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, 
                                  cur_num_sram, cur_num_sram + num_lut_sram - 1, 
                                  1, VERILOG_PORT_CONKT);
    if (0 < num_mode_sram) {
      fprintf(fp, ", ");
      dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, 
                                 cur_num_sram + num_lut_sram, cur_num_sram + num_lut_sram + num_mode_sram - 1, 
                                 0, VERILOG_PORT_CONKT);
      fprintf(fp, ", ");
      dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, 
                                    cur_num_sram + num_lut_sram, cur_num_sram + num_lut_sram + num_mode_sram - 1, 
                                    1, VERILOG_PORT_CONKT);
    }
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

  /* Call SRAM subckts only 
   * when Configuration organization style is memory bank */
  /* No. of SRAMs is different from the number of configuration lines.
   * Especially when SRAMs/RRAMs are configured with BL/WLs
   */
  num_sram = count_num_sram_bits_one_spice_model(verilog_model, -1);
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info);

  /* Call the memory module defined for this SRAM-based MUX! */
  mem_subckt_name = generate_verilog_mem_subckt_name(verilog_model, mem_model, verilog_mem_posfix);
  fprintf(fp, "%s %s_%d_ ( ", 
          mem_subckt_name, mem_subckt_name, verilog_model->cnt);
  dump_verilog_mem_sram_submodule(fp, cur_sram_orgz_info, verilog_model, -1, 
                                  mem_model, cur_num_sram, cur_num_sram + num_sram - 1); 
  fprintf(fp, ");\n");
  /* update the number of memory bits */
  update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_sram + num_sram);

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
  my_free(mem_subckt_name);
  /* my_free(sram_bits); */
  my_free(port_prefix);

  return;
}
