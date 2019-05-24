/***********************************/
/*  	SDC Generation dumping     */
/*       Xifan TANG, EPFL/LSI      */
/*      Baudouin Chauviere LNIS    */
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
#include "path_delay.h"
#include "stats.h"
#include "route_common.h"

/* Include FPGA-SPICE utils */
#include "read_xml_spice_util.h"
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_backannotate_utils.h"
#include "fpga_x2p_globals.h"
#include "fpga_bitstream.h"

/* Include SynVerilog headers */
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_submodules.h"
#include "verilog_pbtypes.h"
#include "verilog_routing.h"
#include "verilog_compact_netlist.h"
#include "verilog_top_testbench.h"
#include "verilog_autocheck_top_testbench.h"
#include "verilog_verification_top_netlist.h"
#include "verilog_modelsim_autodeck.h"
#include "verilog_report_timing.h"
#include "verilog_sdc.h"
#include "verilog_formality_autodeck.h"
#include "verilog_sdc_pb_types.h"


void sdc_dump_annotation(char* from_path, // includes the cell
						char* to_path,
						FILE* fp,
						t_pb_graph_edge* cur_edge){

  float min_value = 0;
  float max_value = 0;
 
  // Find in the annotations the min and max
  if (0 != cur_edge->delay_min) {
    min_value = cur_edge->delay_min;
    min_value = min_value*pow(10,9);
    fprintf (fp, "set_min_delay -combinational_from_to -from %s -to %s ", from_path, to_path);
    fprintf (fp,"%f\n", min_value);
  }
  if (0 != cur_edge->delay_max) {
    max_value = cur_edge->delay_max;
    max_value = max_value*pow(10,9);
    fprintf (fp, "set_max_delay -combinational_from_to -from %s -to %s ", from_path, to_path);
    fprintf (fp,"%f\n", max_value);
  }
  return;
}

void dump_sdc_pb_graph_pin_interc(FILE* fp,
                                  t_pb_graph_pin* des_pb_graph_pin,
                                  t_mode* cur_mode,
                                  char* instance_name) {
  int iedge;
  int fan_in = 0;
  t_interconnect* cur_interc = NULL; 
  enum e_interconnect verilog_interc_type = DIRECT_INTERC;

  t_pb_graph_pin* src_pb_graph_pin = NULL;
  t_pb_graph_node* src_pb_graph_node = NULL;

  t_pb_graph_node* des_pb_graph_node = NULL;

  char* from_path = NULL;
  char* to_path = NULL;

  char* set_disable_path;
  t_pb_graph_pin* cur_pin_disable;
  char* input_buffer_path;
  char* input_buffer_name;
  char* input_buffer_in;
  char* input_buffer_out;
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf (TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* 1. identify pin interconnection type, 
   * 2. Identify the number of fan-in (Consider interconnection edges of only selected mode)
   * 3. Select and print the SPICE netlist
   */
  fan_in = 0;
  cur_interc = NULL;
  find_interc_fan_in_des_pb_graph_pin (des_pb_graph_pin, cur_mode, &cur_interc, &fan_in);
  if ((NULL == cur_interc) || (0 == fan_in)) { 
    /* No interconnection matched */
    /* Connect this pin to GND for better convergence */
    /* TODO: find the correct pin name!!!*/
    /*
    dump_verilog_dangling_des_pb_graph_pin_interc(fp, des_pb_graph_pin, cur_mode, pin2pin_interc_type,
                                                  formatted_parent_pin_prefix);
    */
    return;
  }
  /* Initialize the interconnection type that will be implemented in SPICE netlist*/
  verilog_interc_type = determine_actual_pb_interc_type (cur_interc, fan_in);
  /* This time, (2nd round), we print the subckt, according to interc type*/ 
  switch (verilog_interc_type) {
  case DIRECT_INTERC:
    /* Check : 
     * 1. Direct interc has only one fan-in!
     */
    assert (1 == fan_in);
    //assert(1 == des_pb_graph_pin->num_input_edges);
    /* For more than one mode defined, the direct interc has more than one input_edge ,
     * We need to find which edge is connected the pin we want
     */
    for (iedge = 0; iedge < des_pb_graph_pin->num_input_edges; iedge++) {
      if (cur_interc == des_pb_graph_pin->input_edges[iedge]->interconnect) {
        break;
      }
    }
    assert (iedge < des_pb_graph_pin->num_input_edges);
    /* 2. spice_model is a wire */ 
    assert (NULL != cur_interc->spice_model);
    assert (SPICE_MODEL_WIRE == cur_interc->spice_model->type);
    assert (NULL != cur_interc->spice_model->wire_param);
    /* Initialize*/
    /* Source pin, node, pb_type*/
    src_pb_graph_pin = des_pb_graph_pin->input_edges[iedge]->input_pins[0];
    src_pb_graph_node = src_pb_graph_pin->parent_node;
    /* Des pin, node, pb_type */
    des_pb_graph_node  = des_pb_graph_pin->parent_node;
    
    /* if clock, clock tree synthesis will take care of the timings */
    if (TRUE == src_pb_graph_pin->port->is_clock ||
        TRUE == des_pb_graph_pin->port->is_clock) {
      return;
    } 
	// Generation of the paths for the dumping of the annotations
    from_path = (char *) my_malloc(sizeof(char)*(strlen(instance_name) + 1 + strlen(gen_verilog_one_pb_graph_pin_full_name_in_hierarchy (src_pb_graph_pin)) + 1));	
    sprintf (from_path, "%s/%s", instance_name, gen_verilog_one_pb_graph_pin_full_name_in_hierarchy (src_pb_graph_pin));
    to_path = (char *) my_malloc(sizeof(char)*(strlen(instance_name) + 1 + strlen(gen_verilog_one_pb_graph_pin_full_name_in_hierarchy (des_pb_graph_pin)) + 1));	
    sprintf (to_path, "%s/%s", instance_name, gen_verilog_one_pb_graph_pin_full_name_in_hierarchy (des_pb_graph_pin));

	// Dumping of the annotations	
	sdc_dump_annotation (from_path, to_path, fp, des_pb_graph_pin->input_edges[iedge]);	
  break;
  case COMPLETE_INTERC:
  case MUX_INTERC:
    /* Check : 
     * MUX should have at least 2 fan_in
     */
    assert ((2 == fan_in) || (2 < fan_in));
    /* 2. spice_model is a wire */ 
    assert (NULL != cur_interc->spice_model);
    assert (SPICE_MODEL_MUX == cur_interc->spice_model->type);
    for (iedge = 0; iedge < des_pb_graph_pin->num_input_edges; iedge++) {
      if (cur_mode != des_pb_graph_pin->input_edges[iedge]->interconnect->parent_mode) {
        continue;
      }
      check_pb_graph_edge(*(des_pb_graph_pin->input_edges[iedge]));
      /* Initialize*/
      /* Source pin, node, pb_type*/
      src_pb_graph_pin = des_pb_graph_pin->input_edges[iedge]->input_pins[0];
      src_pb_graph_node = src_pb_graph_pin->parent_node;
      /* Des pin, node, pb_type */
      des_pb_graph_node  = des_pb_graph_pin->parent_node;
  	  // Generation of the paths for the dumping of the annotations
      from_path = (char *) my_malloc(sizeof(char)*(strlen(instance_name) + 1 + strlen(gen_verilog_one_pb_graph_pin_full_name_in_hierarchy (src_pb_graph_pin)) + 1));	
      sprintf (from_path, "%s/%s", instance_name, gen_verilog_one_pb_graph_pin_full_name_in_hierarchy (src_pb_graph_pin));
      to_path = (char *) my_malloc(sizeof(char)*(strlen(instance_name) + 1 + strlen(gen_verilog_one_pb_graph_pin_full_name_in_hierarchy (des_pb_graph_pin)) + 1));	
      sprintf (to_path, "%s/%s", instance_name, gen_verilog_one_pb_graph_pin_full_name_in_hierarchy (des_pb_graph_pin));

    /* If the pin is disabled, the dumping is different. We need to use the 
     * input and output of the inverter of the mux */ 
    if (TRUE == des_pb_graph_pin->input_edges[iedge]->is_disabled) {
      /* We need to find the highest node between src and des */
      if (src_pb_graph_node->parent_pb_graph_node == des_pb_graph_node->parent_pb_graph_node) {
        cur_pin_disable = src_pb_graph_pin->parent_node->parent_pb_graph_node->input_pins[0];
      }
      else if (src_pb_graph_node->parent_pb_graph_node == des_pb_graph_node) {
        cur_pin_disable = des_pb_graph_pin;
      }
      else {
        cur_pin_disable = src_pb_graph_pin;
      }
      if (cur_interc->spice_model->input_buffer == NULL) {
        vpr_printf (TIO_MESSAGE_ERROR,
                    "The loop_breaker annotation can only be applied when there is an input buffer"); 
      }
      if (0 == strcmp("",gen_verilog_one_pb_graph_pin_full_name_in_hierarchy_parent_node(cur_pin_disable))) {
        input_buffer_path = (char *) my_malloc(sizeof(char)*(strlen(instance_name) + 1 +
        strlen (cur_interc->spice_model->name) + 5 + strlen(my_itoa(cur_interc->fan_in)) + 1 +
        strlen (my_itoa(des_pb_graph_pin->input_edges[iedge]->nb_mux)) + 1 + 1)); 

        sprintf (input_buffer_path, "%s/%s_size%d_%d_",instance_name,
               cur_interc->spice_model->name, cur_interc->fan_in, 
               des_pb_graph_pin->input_edges[iedge]->nb_mux); 
      }
      else {
        input_buffer_path = (char *) my_malloc(sizeof(char)*(strlen(instance_name) + 1 +
        strlen (gen_verilog_one_pb_graph_pin_full_name_in_hierarchy_parent_node(cur_pin_disable)) +
        strlen (cur_interc->spice_model->name) + 5 + strlen(my_itoa(cur_interc->fan_in)) + 1 +
        strlen (my_itoa(des_pb_graph_pin->input_edges[iedge]->nb_mux)) + 1 + 1)); 

        sprintf (input_buffer_path, "%s/%s%s_size%d_%d_",instance_name,
                 gen_verilog_one_pb_graph_pin_full_name_in_hierarchy_parent_node(cur_pin_disable),
                 cur_interc->spice_model->name, cur_interc->fan_in ,
                 des_pb_graph_pin->input_edges[iedge]->nb_mux); 
      }
      input_buffer_name = cur_interc ->spice_model->input_buffer->spice_model_name;
      /* BChauviere: might need to find the right port if something other than an inverter is used */
      input_buffer_in = cur_interc ->spice_model->input_buffer->spice_model->ports[0].lib_name;
      input_buffer_out = cur_interc ->spice_model->input_buffer->spice_model->ports[1].lib_name;
      set_disable_path = (char*) my_malloc(sizeof(char)*(
                                            strlen(input_buffer_path) + 1 + 
                                            strlen(input_buffer_name) + 1 + 
                                            strlen(my_itoa(des_pb_graph_pin->input_edges[iedge]->nb_pin))
                                            + 1 + 1) ); 
      sprintf(set_disable_path, "%s/%s_%d_", input_buffer_path, input_buffer_name,
              des_pb_graph_pin->input_edges[iedge]->nb_pin); 
      
      if (NULL != des_pb_graph_pin->input_edges[iedge]->loop_breaker_delay_before_min) {
        fprintf (fp, "set_min_delay -from %s -to %s/%s %f \n", from_path, set_disable_path, input_buffer_in,
                 pow(10,9)*atof(des_pb_graph_pin->input_edges[iedge]->loop_breaker_delay_before_min)); 
      }
      if (NULL != des_pb_graph_pin->input_edges[iedge]->loop_breaker_delay_before_max) {
        fprintf (fp, "set_max_delay -from %s -to %s/%s %f \n", from_path, set_disable_path, input_buffer_in,
                 pow(10,9)*atof(des_pb_graph_pin->input_edges[iedge]->loop_breaker_delay_before_max)); 
      }
      fprintf (fp, "set_disable_timing -from %s -to %s %s \n", input_buffer_in, input_buffer_out, set_disable_path);
      if (NULL != des_pb_graph_pin->input_edges[iedge]->loop_breaker_delay_after_min) {
        fprintf (fp, "set_min_delay -from %s/%s -to %s %f \n", set_disable_path, input_buffer_out,
                 to_path, 
                 pow(10,9)*atof(des_pb_graph_pin->input_edges[iedge]->loop_breaker_delay_after_min)); 
      }
      if (NULL != des_pb_graph_pin->input_edges[iedge]->loop_breaker_delay_after_max) {
        fprintf (fp, "set_max_delay -from %s/%s -to %s %f \n", set_disable_path, input_buffer_out,
                 to_path, 
                 pow(10,9)*atof(des_pb_graph_pin->input_edges[iedge]->loop_breaker_delay_after_max)); 
      }
      my_free(input_buffer_path);
      my_free(set_disable_path);
    }
	else { 
  	  // Dumping of the annotations
  	  sdc_dump_annotation (from_path, to_path, fp, des_pb_graph_pin->input_edges[iedge]);	
      }
    }	
    break;
    

  default:
    vpr_printf (TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid interconnection type for %s (Arch[LINE%d])!\n",
               __FILE__, __LINE__, cur_interc->name, cur_interc->line_num);
    exit(1);
  }

  return;
}


/* Print the SPICE interconnections of a port defined in pb_graph */
void dump_sdc_pb_graph_port_interc(FILE* fp,
                                   t_pb_graph_node* cur_pb_graph_node,
                                   enum e_spice_pb_port_type pb_port_type,
                                   t_mode* cur_mode,
                                   char* instance_name) {
  int iport, ipin;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf (TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  switch (pb_port_type) {
  case SPICE_PB_PORT_INPUT:
    for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
        /* If this is a idle block, we set 0 to the selected edge*/
        /* Get the selected edge of current pin*/
        dump_sdc_pb_graph_pin_interc (fp, 
                                      &(cur_pb_graph_node->input_pins[iport][ipin]),
                                      cur_mode,
                                      instance_name);
      }
    }
    break;
  case SPICE_PB_PORT_OUTPUT:
    for (iport = 0; iport < cur_pb_graph_node->num_output_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_output_pins[iport]; ipin++) {
        dump_sdc_pb_graph_pin_interc(fp, 
                                     &(cur_pb_graph_node->output_pins[iport][ipin]),
                                     cur_mode,
                                     instance_name);
      }
    }
    break;
  case SPICE_PB_PORT_CLOCK:
    for (iport = 0; iport < cur_pb_graph_node->num_clock_ports; iport++) {
      for (ipin = 0; ipin < cur_pb_graph_node->num_clock_pins[iport]; ipin++) {
        dump_sdc_pb_graph_pin_interc(fp, 
                                     &(cur_pb_graph_node->clock_pins[iport][ipin]),
                                     cur_mode,
                                     instance_name);
      }
    }
    break;
  default:
   vpr_printf (TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid pb port type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

void sdc_dump_cur_node_constraints(FILE*  fp,
							       t_pb_graph_node* cur_pb_graph_node,
							       int select_mode_index,
                                   char* instance_name) {
  int ipb, jpb;
  t_mode* cur_mode = NULL;
  t_pb_type* cur_pb_type = cur_pb_graph_node->pb_type;
  t_pb_graph_node* child_pb_graph_node = NULL;
  

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf (TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check cur_pb_type*/
  if (NULL == cur_pb_type) {
    vpr_printf (TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb_type.\n", 
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
  dump_sdc_pb_graph_port_interc(fp,
                                cur_pb_graph_node, 
                                SPICE_PB_PORT_OUTPUT,
                                cur_mode,
                                instance_name);
  
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
      /* For each child_pb_graph_node input pins*/
      dump_sdc_pb_graph_port_interc(fp,
                                    child_pb_graph_node, 
                                    SPICE_PB_PORT_INPUT,
                                    cur_mode,
                                    instance_name);
      /* TODO: for clock pins, we should do the same work */
      dump_sdc_pb_graph_port_interc(fp,
                                    child_pb_graph_node, 
                                    SPICE_PB_PORT_CLOCK,
                                    cur_mode,
                                    instance_name);
    }
  }
  return; 
}

void sdc_rec_dump_child_pb_graph_node(t_sram_orgz_info* cur_sram_orgz_info,
									 FILE* fp,
									 t_pb_graph_node* cur_pb_graph_node,
                                     char* instance_name) {

  int mode_index, ipb, jpb;
  t_pb_type* cur_pb_type = NULL;

  /* Check the file handler */
  if (NULL == fp) {
	vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n",__FILE__, __LINE__);
  	  exit(1);
	}
/* Check current node */
  if (NULL == cur_pb_graph_node) {
	vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb_graph_node.\n",__FILE__, __LINE__);
	exit(1);
  }
  cur_pb_type = cur_pb_graph_node->pb_type;

/* First we only go through the graph through the physical nodes. The other modes don't have sdc constraints. We then check all the children nodes and repeat the operation until arriving to the leaf nodes. 
 * Once at the leaf, all the edges are dumped with a default set_min/max_delay of 0 if non is user-defined.
 * Contrary to the verilog, because the interconnections can get tricky through shift registers, carry-chains and all, we do not use wild cards or instantiation to be sure to completely constrain our design.
 */
/* Recursively finish all the child pb types */
  if (FALSE == is_primitive_pb_type(cur_pb_type)) {
	/* Find the mode that defines the physical mode*/
	mode_index = find_pb_type_physical_mode_index((*cur_pb_type));	
	for(ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
		for(jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++){
		/* Contrary to the verilog, we do not need to keep the prefix 
 * We go done to every child node to dump the constraints now*/ 		
		  sdc_rec_dump_child_pb_graph_node(cur_sram_orgz_info, fp, &(cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]), instance_name);
	  }
	}
    sdc_dump_cur_node_constraints(fp, cur_pb_graph_node, mode_index, instance_name); // graph_head only has one pb_type
  }

  return;
}

void sdc_dump_all_pb_graph_nodes(FILE* fp,
							t_sram_orgz_info* cur_sram_orgz_info,
							int type_descriptors_mode,
                            char* instance_name){

  // Give head of the pb_graph to the recursive function
  sdc_rec_dump_child_pb_graph_node (cur_sram_orgz_info, fp, type_descriptors[type_descriptors_mode].pb_graph_head, instance_name);

return;
}

void dump_sdc_physical_blocks(t_sram_orgz_info* cur_sram_orgz_info,
							char* sdc_path,
							int type_descriptor_mode,
                            char* instance_name) {

  FILE* fp;

  /* Check if the path exists*/
  fp = fopen (sdc_path,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in creating SDC for constraining Primitive Blocks %s!",__FILE__, __LINE__, sdc_path); 
    exit(1);
}
  
  vpr_printf (TIO_MESSAGE_INFO, "Generating SDC for constraining Primitive Blocks in P&R flow: (%s)...\n", 
             sdc_path);


  // Launch a recursive function to visit all the nodes of the correct mode
  sdc_dump_all_pb_graph_nodes(fp, cur_sram_orgz_info, type_descriptor_mode, instance_name);


  /* close file */ 
  fclose(fp);

return;
}


void verilog_generate_sdc_constrain_pb_types(t_sram_orgz_info* cur_sram_orgz_info,
                                             char* sdc_dir) {

  int itype;
  char* sdc_path;
  char* instance_name;

  sdc_path = my_strcat (sdc_dir, sdc_constrain_pb_type_file_name); // Global var

  for (itype = 0; itype < num_types; itype++){
    if (FILL_TYPE == &type_descriptors[itype]){
      instance_name = gen_verilog_one_phy_block_instance_name(&type_descriptors[itype],0); /* it is 0 because the CLBs only have 1 block.*/
	  dump_sdc_physical_blocks(cur_sram_orgz_info, sdc_path, itype, instance_name);
    }
  }

  return;
}

