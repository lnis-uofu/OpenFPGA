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
#include "route_common.h"

/* Include SPICE support headers*/
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_globals.h"
#include "fpga_x2p_mux_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "fpga_bitstream_primitives.h"


/***** Subroutines *****/

/* We check output_pins of cur_pb_graph_node and its the input_edges
 * Built the interconnections between outputs of cur_pb_graph_node and outputs of child_pb_graph_node
 *   src_pb_graph_node.[in|out]_pins -----------------> des_pb_graph_node.[in|out]pins
 *                                        /|\
 *                                         |
 *                         input_pins,   edges,       output_pins
 */ 
void fpga_spice_generate_bitstream_pb_graph_pin_interc(FILE* fp,
                                                       enum e_spice_pin2pin_interc_type pin2pin_interc_type,
                                                       t_pb_graph_pin* des_pb_graph_pin,
                                                       t_mode* cur_mode,
                                                       int select_edge,
                                                       t_sram_orgz_info* cur_sram_orgz_info) {
  int iedge, ilevel;
  int fan_in = 0;
  t_interconnect* cur_interc = NULL; 
  enum e_interconnect verilog_interc_type = DIRECT_INTERC;

  int num_mux_sram_bits = 0;
  int* mux_sram_bits = NULL;
  int mux_level = 0;
  int cur_bl, cur_wl;

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
    return;
  }
  verilog_interc_type = determine_actual_pb_interc_type(cur_interc, fan_in);
  /* This time, (2nd round), we print the subckt, according to interc type*/ 
  switch (verilog_interc_type) {
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
    /* Call the subckt that has already been defined before */
    cur_interc->spice_model->cnt++; /* Stats the number of spice_model used*/
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
    /* Print SRAMs that configure this MUX */
    get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl);
    /* SRAMs */
    switch (cur_interc->spice_model->design_tech) {
    case SPICE_MODEL_DESIGN_CMOS:
      decode_cmos_mux_sram_bits(cur_interc->spice_model, fan_in, select_edge, 
                                &num_mux_sram_bits, &mux_sram_bits, &mux_level);
      break;
    case SPICE_MODEL_DESIGN_RRAM:
      decode_rram_mux(cur_interc->spice_model, fan_in, select_edge, 
                      &num_mux_sram_bits, &mux_sram_bits, &mux_level);
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
                 __FILE__, __LINE__, cur_interc->spice_model->name);
    }
    
    /* Print the encoding in SPICE netlist for debugging */
    fprintf(fp, "***** SRAM bits for MUX[%d], mux_size=%d, level=%d, select_path_id=%d. *****\n", 
            cur_interc->spice_model->cnt, fan_in, mux_level, select_edge);
    fprintf(fp, "*****");
    for (ilevel = 0; ilevel < num_mux_sram_bits; ilevel++) {
      fprintf(fp, "%d", mux_sram_bits[ilevel]);
    }
    fprintf(fp, "*****\n\n");
  
    /* Store the configuraion bit to linked-list */
    add_mux_conf_bits_to_llist(fan_in, cur_sram_orgz_info, 
                               num_mux_sram_bits, mux_sram_bits,
                               cur_interc->spice_model);
    /* Synchronize the sram_orgz_info with mem_bits */
    add_mux_conf_bits_to_sram_orgz_info(cur_sram_orgz_info, cur_interc->spice_model, fan_in); 
    /* update sram counter */
    cur_interc->spice_model->cnt++;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid interconnection type for %s (Arch[LINE%d])!\n",
               __FILE__, __LINE__, cur_interc->name, cur_interc->line_num);
    exit(1);
  }

  return;
}

void fpga_spice_generate_bitstream_pb_graph_port_interc(FILE* fp,
                                                        t_pb_graph_node* cur_pb_graph_node,
                                                        t_phy_pb* cur_pb,
                                                        enum e_spice_pb_port_type pb_port_type,
                                                        t_mode* cur_mode,
                                                        t_sram_orgz_info* cur_sram_orgz_info) {
  int iport, ipin;
  int node_index = -1;
  int prev_node = -1; 
  int path_id = -1;
  t_rr_node* pb_rr_nodes = NULL;

  if (NULL != cur_pb) {
    fprintf(fp, "***** Logic block:%s *****\n", 
            cur_pb->spice_name_tag);
    fprintf(fp, "***** Pb_graph_node: %s[%d]*****\n", 
            cur_pb_graph_node->pb_type->name, cur_pb_graph_node->placement_index);
  }

  switch (pb_port_type) {
  case SPICE_PB_PORT_INPUT:
    for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
        fprintf(fp, "***** Input Port: %s[%d]*****\n", 
                cur_pb_graph_node->input_pins[iport][ipin].port->name, 
                cur_pb_graph_node->input_pins[iport][ipin].pin_number);
        /* If this is a idle block, we set 0 to the selected edge*/
        /* Get the selected edge of current pin*/
        if (NULL == cur_pb) {
          path_id = DEFAULT_PATH_ID;
        } else {
          assert(NULL != cur_pb);
          pb_rr_nodes = cur_pb->rr_graph->rr_node;
          node_index = cur_pb_graph_node->input_pins[iport][ipin].rr_node_index_physical_pb;
          prev_node = pb_rr_nodes[node_index].prev_node;
          /* prev_edge = pb_rr_nodes[node_index].prev_edge; */
          /* Make sure this pb_rr_node is not OPEN and is not a primitive output*/
          if (OPEN == prev_node) {
            path_id = DEFAULT_PATH_ID; 
          } else {
            /* Find the path_id */
            path_id = find_path_id_between_pb_rr_nodes(pb_rr_nodes, prev_node, node_index);
            assert(DEFAULT_PATH_ID != path_id);
          }
          /* Path id for the sdc generation */
          pb_rr_nodes[node_index].id_path = path_id;

          if (OPEN != pb_rr_nodes[node_index].vpack_net_num) {
            fprintf(fp, "***** Net name: %s *****\n", 
                    vpack_net[pb_rr_nodes[node_index].vpack_net_num].name);
          }
        }
        fpga_spice_generate_bitstream_pb_graph_pin_interc(fp, INPUT2INPUT_INTERC,
                                                          &(cur_pb_graph_node->input_pins[iport][ipin]),
                                                          cur_mode,
                                                          path_id, cur_sram_orgz_info);
      }
    }
    break;
  case SPICE_PB_PORT_OUTPUT:
    for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
        fprintf(fp, "***** Output Port: %s[%d]*****\n", 
                cur_pb_graph_node->output_pins[iport][ipin].port->name, 
                cur_pb_graph_node->output_pins[iport][ipin].pin_number);
        /* If this is a idle block, we set 0 to the selected edge*/
        /* Get the selected edge of current pin*/
        if (NULL == cur_pb) {
          path_id = DEFAULT_PATH_ID;
        } else {
          assert(NULL != cur_pb);
          pb_rr_nodes = cur_pb->rr_graph->rr_node;
          node_index = cur_pb_graph_node->output_pins[iport][ipin].rr_node_index_physical_pb;
          prev_node = pb_rr_nodes[node_index].prev_node;
          /* prev_edge = pb_rr_nodes[node_index].prev_edge; */
          /* Make sure this pb_rr_node is not OPEN and is not a primitive output*/
          if (OPEN == prev_node) {
            path_id = DEFAULT_PATH_ID; 
          } else {
            /* Find the path_id */
            path_id = find_path_id_between_pb_rr_nodes(pb_rr_nodes, prev_node, node_index);
            assert(DEFAULT_PATH_ID != path_id);
          }
          // Path id for the sdc generation
          pb_rr_nodes[node_index].id_path = path_id;

          if (OPEN != pb_rr_nodes[node_index].vpack_net_num) {
            fprintf(fp, "***** Net name: %s *****\n", 
                    vpack_net[pb_rr_nodes[node_index].vpack_net_num].name);
          }
        }
        fpga_spice_generate_bitstream_pb_graph_pin_interc(fp, OUTPUT2OUTPUT_INTERC,
                                                          &(cur_pb_graph_node->output_pins[iport][ipin]),
                                                          cur_mode,
                                                          path_id, cur_sram_orgz_info);
      }
    }
    break;
  case SPICE_PB_PORT_CLOCK:
    for (iport = 0; iport < cur_pb_graph_node->num_clock_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_clock_pins[iport]; ipin++) {
        fprintf(fp, "***** Clock Port: %s[%d]*****\n", 
                cur_pb_graph_node->clock_pins[iport][ipin].port->name, 
                cur_pb_graph_node->clock_pins[iport][ipin].pin_number);
        /* If this is a idle block, we set 0 to the selected edge*/
        /* Get the selected edge of current pin*/
        if (NULL == cur_pb) {
          path_id = DEFAULT_PATH_ID;
        } else {
          assert(NULL != cur_pb);
          pb_rr_nodes = cur_pb->rr_graph->rr_node;
          node_index = cur_pb_graph_node->clock_pins[iport][ipin].rr_node_index_physical_pb;
          prev_node = pb_rr_nodes[node_index].prev_node;
          /* prev_edge = pb_rr_nodes[node_index].prev_edge; */
          /* Make sure this pb_rr_node is not OPEN and is not a primitive output*/
          if (OPEN == prev_node) {
            path_id = DEFAULT_PATH_ID; 
          } else {
            /* Find the path_id */
            path_id = find_path_id_between_pb_rr_nodes(pb_rr_nodes, prev_node, node_index);
            assert(DEFAULT_PATH_ID != path_id);
          }
          // Path id for the sdc generation
          pb_rr_nodes[node_index].id_path = path_id;

          if (OPEN != pb_rr_nodes[node_index].vpack_net_num) {
            fprintf(fp, "***** Net name: %s *****\n", 
                    vpack_net[pb_rr_nodes[node_index].vpack_net_num].name);
          }
        }
        fpga_spice_generate_bitstream_pb_graph_pin_interc(fp, INPUT2INPUT_INTERC,
                                                          &(cur_pb_graph_node->clock_pins[iport][ipin]),
                                                          cur_mode,
                                                          path_id, cur_sram_orgz_info);

      }
    }
    break;
  default:
   vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid pb port type!\n",
               __FILE__, __LINE__);
    exit(1);
  }


  return;
}

/* Print the SPICE interconnections according to pb_graph */
void fpga_spice_generate_bitstream_pb_graph_interc(FILE* fp,
                                                   t_pb_graph_node* cur_pb_graph_node,
                                                   t_phy_pb* cur_pb,
                                                   int select_mode_index,
                                                   t_sram_orgz_info* cur_sram_orgz_info) {
  int ipb, jpb;
  t_mode* cur_mode = NULL;
  t_pb_type* cur_pb_type = cur_pb_graph_node->pb_type;
  t_pb_graph_node* child_pb_graph_node = NULL;
  t_phy_pb* child_pb = NULL;
  
  /* Check cur_pb_type*/
  if (NULL == cur_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb_graph_node.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
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
  fpga_spice_generate_bitstream_pb_graph_port_interc(fp, cur_pb_graph_node,
                                                     cur_pb,
                                                     SPICE_PB_PORT_OUTPUT,
                                                     cur_mode,
                                                     cur_sram_orgz_info);
  
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
      /* branch on empty pb */
      if (NULL == cur_pb) {
        child_pb = NULL;
      } else {
        child_pb = &(cur_pb->child_pbs[ipb][jpb]);
      }
      /* For each child_pb_graph_node input pins*/
      fpga_spice_generate_bitstream_pb_graph_port_interc(fp, child_pb_graph_node,
                                                         child_pb,
                                                         SPICE_PB_PORT_INPUT,
                                                         cur_mode,
                                                         cur_sram_orgz_info);
      /* TODO: for clock pins, we should do the same work */
      fpga_spice_generate_bitstream_pb_graph_port_interc(fp, child_pb_graph_node,
                                                         child_pb,
                                                         SPICE_PB_PORT_CLOCK,
                                                         cur_mode,
                                                         cur_sram_orgz_info);
    }
  }

  return; 
}

/* Print the subckt of a primitive pb */
void fpga_spice_generate_bitstream_pb_primitive(FILE* fp,
                                                t_phy_pb* prim_pb,
                                                t_pb_type* prim_pb_type,
                                                t_sram_orgz_info* cur_sram_orgz_info) {
 /* Check cur_pb_graph_node*/
  if (NULL == prim_pb_type) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid prim_pb_type.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  assert (TRUE == prim_pb_type->parent_mode->define_physical_mode); 

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* According to different type, we print netlist*/
  switch (prim_pb_type->spice_model->type) {
  case SPICE_MODEL_LUT:
    /* If this is a idle block we should set sram_bits to zero*/
    fpga_spice_generate_bitstream_pb_primitive_lut(fp, prim_pb, prim_pb_type, cur_sram_orgz_info);
    break;
  case SPICE_MODEL_FF:
    assert(NULL != prim_pb_type->spice_model->model_netlist);
    /* TODO : We should learn trigger type and initial value!!! and how to apply them!!! */
    fpga_spice_generate_bitstream_pb_generic_primitive(fp, prim_pb, prim_pb_type, cur_sram_orgz_info);
    break;
  case SPICE_MODEL_IOPAD:
    assert(NULL != prim_pb_type->spice_model->model_netlist);
    fpga_spice_generate_bitstream_pb_generic_primitive(fp, prim_pb, prim_pb_type, cur_sram_orgz_info);
    break;
  case SPICE_MODEL_HARDLOGIC:
    assert(NULL != prim_pb_type->spice_model->model_netlist);
    fpga_spice_generate_bitstream_pb_generic_primitive(fp, prim_pb, prim_pb_type, cur_sram_orgz_info);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of verilog_model(%s), should be [LUT|FF|HARD_LOGIC|IO]!\n",
               __FILE__, __LINE__, prim_pb_type->spice_model->name);
    exit(1);
  }
 
  return; 
}

/* Print physical mode of pb_types and configure it to the idle pb_types recursively
 * search the idle_mode until we reach the leaf node
 */
void fpga_spice_generate_bitstream_phy_pb_graph_node_rec(FILE* fp,
                                                         t_phy_pb* cur_pb, 
                                                         t_pb_graph_node* cur_pb_graph_node,
                                                         int pb_type_index,
                                                         t_sram_orgz_info* cur_sram_orgz_info) {
  int mode_index, ipb, jpb;
  t_pb_type* cur_pb_type = NULL;
  t_phy_pb* child_pb = NULL;
 
  /* Check cur_pb_graph_node*/
  if (NULL == cur_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb_graph_node.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  cur_pb_type = cur_pb_graph_node->pb_type;

  /* Recursively finish all the child pb_types*/
  if (NULL == cur_pb_type->spice_model) { 
    /* Find the mode that define_idle_mode*/
    mode_index = find_pb_type_physical_mode_index((*cur_pb_type));
    for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
      for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
        /* Recursive*/
        /* Refer to pack/output_clustering.c [LINE 392] */
        /* Find the child pb that is mapped, and the mapping info is not stored in the physical mode ! */
        if (NULL == cur_pb) {
          child_pb = NULL;
        } else {
          assert (NULL != cur_pb);
          child_pb = get_phy_child_pb_for_phy_pb_graph_node(cur_pb, ipb, jpb);
        }
        fpga_spice_generate_bitstream_phy_pb_graph_node_rec(fp, child_pb,
                                                            &(cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]), jpb,
                                                            cur_sram_orgz_info);
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
      fpga_spice_generate_bitstream_pb_primitive( fp, cur_pb, 
                                                  cur_pb_type,
                                                  cur_sram_orgz_info); 
      break;
    case LATCH_CLASS:
      assert(0 == cur_pb_type->num_modes);
      /* Consider the num_pb, create all the subckts*/
      fpga_spice_generate_bitstream_pb_primitive( fp, cur_pb,
                                                  cur_pb_type, 
                                                  cur_sram_orgz_info); 
      break;
    case UNKNOWN_CLASS:
    case MEMORY_CLASS:
      /* Consider the num_pb, create all the subckts*/
      fpga_spice_generate_bitstream_pb_primitive( fp, cur_pb,  
                                                  cur_pb_type, 
                                                  cur_sram_orgz_info); 
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

  /* Print interconnections, set is_idle as TRUE*/
  fpga_spice_generate_bitstream_pb_graph_interc(fp, cur_pb_graph_node, cur_pb, mode_index, cur_sram_orgz_info);

  return;
}


/* Print an physical logic block
 * Find the physical_mode in arch files,
 * And print the verilog netlist into file
 */
void fpga_spice_generate_bitstream_one_physical_block(FILE* fp,
                                                      int x, int y, int z,
                                                      t_type_ptr type_descriptor,
                                                      t_sram_orgz_info* cur_sram_orgz_info) {
  t_pb_graph_node* top_pb_graph_node = NULL;
  t_block* mapped_block = NULL;
  t_phy_pb* top_pb = NULL;

  /* Ensure we have a valid type_descriptor*/ 
  assert(NULL != type_descriptor);

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Go for the pb_types*/
  top_pb_graph_node = type_descriptor->pb_graph_head;
  assert(NULL != top_pb_graph_node);

  /* Check in all the mapped blocks(clustered logic block), there is a match x,y,z*/
  mapped_block = search_mapped_block(x, y, z); 
  if (NULL != mapped_block) {
    top_pb = (t_phy_pb*)mapped_block->phy_pb; 
    assert(NULL != top_pb);
  }

  /* Recursively find all idle mode and print netlist*/
  fpga_spice_generate_bitstream_phy_pb_graph_node_rec(fp, top_pb, top_pb_graph_node, 
                                                      z, cur_sram_orgz_info);

  return;
}

/* Print the SPICE netlist for a I/O grid blocks */
void fpga_spice_generate_bitstream_physical_grid_block(FILE* fp,
                                                       int ix, int iy,
                                                       t_arch* arch,
                                                       t_sram_orgz_info* cur_sram_orgz_info) {
  int iz;
  int capacity; 

  /* Check */
  assert((!(0 > ix))&&(!(ix > (nx + 1)))); 
  assert((!(0 > iy))&&(!(iy > (ny + 1)))); 

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* generate_grid_subckt, type_descriptor of each grid defines the capacity,
   * for example, each grid may contains more than one top-level pb_types, such as I/O
   */
  if ((NULL == grid[ix][iy].type)
      ||(EMPTY_TYPE == grid[ix][iy].type)
      ||(0 != grid[ix][iy].offset)) {
    return; 
  }

  capacity= grid[ix][iy].type->capacity;
  assert(0 < capacity);

  /* check capacity and if this has been mapped */
  for (iz = 0; iz < capacity; iz++) {
    /* Print a NULL logic block...*/
    fpga_spice_generate_bitstream_one_physical_block(fp, ix, iy, iz, grid[ix][iy].type, cur_sram_orgz_info);
  } 

  return;
}

/* Print all logic blocks SPICE models 
 * Each logic blocks in the grid that allocated for the FPGA
 * will be printed. May have an additional option that only
 * output the used logic blocks 
 */
void fpga_spice_generate_bitstream_logic_block(char* lb_bitstream_log_file_path,
                                               t_arch* arch,
                                               t_sram_orgz_info* cur_sram_orgz_info) {
  int ix, iy; 
  FILE* fp = NULL;

  /* Create a file handler */
  fp = fopen(lb_bitstream_log_file_path, "w");

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Failure in creating log file %s",
               __FILE__, __LINE__, lb_bitstream_log_file_path); 
    exit(1);
  } 
  
  /* Check the grid*/
  if ((0 == nx)||(0 == ny)) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid grid size (nx=%d, ny=%d)!\n", __FILE__, __LINE__, nx, ny);
    return;    
  }

  assert(NULL != grid);
 
  /* Print the core logic block one by one
   * Note ix=0 and ix = nx + 1 are IO pads. They surround the core logic blocks
   */
  vpr_printf(TIO_MESSAGE_INFO,"Generating bitstream for core grids...\n");
  for (ix = 1; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      /* Ensure this is not a io */
      assert(IO_TYPE != grid[ix][iy].type);
      /* Ensure a valid usage */
      assert((0 == grid[ix][iy].usage)||(0 < grid[ix][iy].usage));
      fpga_spice_generate_bitstream_physical_grid_block(fp, ix, iy, arch, cur_sram_orgz_info); 
    }
  }

  vpr_printf(TIO_MESSAGE_INFO,"Generating bitstream for IO grids...\n");
  /* Print the IO pads */
  /* Top side : x = 1 .. nx + 1, y = nx + 1  */
  iy = ny + 1;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    fpga_spice_generate_bitstream_physical_grid_block(fp, ix, iy, arch, cur_sram_orgz_info); 
  }

  /* Right side : x = nx + 1, y = 1 .. ny*/
  ix = nx + 1;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    fpga_spice_generate_bitstream_physical_grid_block(fp, ix, iy, arch, cur_sram_orgz_info); 
  }
  /* Bottom  side : x = 1 .. nx + 1, y = 0 */
  iy = 0;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    fpga_spice_generate_bitstream_physical_grid_block(fp, ix, iy, arch, cur_sram_orgz_info); 
  }
  /* Left side: x = 0, y = 1 .. ny*/
  ix = 0;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Ensure this is a io */
    assert(IO_TYPE == grid[ix][iy].type);
    fpga_spice_generate_bitstream_physical_grid_block(fp, ix, iy, arch, cur_sram_orgz_info); 
  }

  /* Close log file */
  fclose(fp);

  /* Free */
   
  return; 
}
