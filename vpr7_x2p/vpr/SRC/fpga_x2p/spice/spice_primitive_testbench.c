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
#include "rr_graph_util.h"
#include "rr_graph.h"
#include "rr_graph2.h"
#include "vpr_utils.h"
#include "route_common.h"

/* Include spice support headers*/
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_globals.h"
#include "spice_globals.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "spice_utils.h"
#include "spice_mux.h"
#include "spice_pbtypes.h"
#include "spice_subckt.h"

/* local global variables */
static int tb_num_primitive = 0;
static int testbench_load_cnt = 0;
static int upbound_sim_num_clock_cycles = 2;
static int max_sim_num_clock_cycles = 2;
static int auto_select_max_sim_num_clock_cycles = TRUE;

/* Subroutines in this source file*/
/* Initialize the global parameters in this source file */
static 
void init_spice_primitive_testbench_globals(t_spice spice) {
  tb_num_primitive = 0;
  auto_select_max_sim_num_clock_cycles = spice.spice_params.meas_params.auto_select_sim_num_clk_cycle;
  upbound_sim_num_clock_cycles = spice.spice_params.meas_params.sim_num_clock_cycle + 1;
  if (FALSE == auto_select_max_sim_num_clock_cycles) {
    max_sim_num_clock_cycles = spice.spice_params.meas_params.sim_num_clock_cycle + 1;
  } else {
    max_sim_num_clock_cycles = 2;
  }
}

/* Print Common global ports in the testbench */
static 
void fprint_spice_primitive_testbench_global_ports(FILE* fp, int grid_x, int grid_y, 
                                                   int num_clock, 
                                                   enum e_spice_tb_type primitive_tb_type,
                                                   t_spice spice) {
  /* int i; */
  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 
 
  /* Global nodes: Vdd for SRAMs, Logic Blocks(Include IO), Switch Boxes, Connection Boxes */
  /* Print generic global ports*/
  fprint_spice_generic_testbench_global_ports(fp, 
                                              sram_spice_orgz_info, 
                                              global_ports_head); 
  /* VDD Load port name */
  fprintf(fp, ".global %s\n",
               spice_tb_global_vdd_load_port_name);

  /*Global Vdds for PRIMITIVEs */
  switch (primitive_tb_type) {
  case SPICE_LUT_TB:
    fprint_grid_global_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_LUT, spice);
    break;
  case SPICE_HARDLOGIC_TB:
    fprint_grid_global_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_FF, spice);
    fprint_grid_global_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_HARDLOGIC, spice);
    break;
  case SPICE_IO_TB:
    fprint_grid_global_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_IOPAD, spice);
    /* Global VDDs for SRAMs of IOPADs */
    fprintf(fp, ".global %s\n\n",
                 spice_tb_global_vdd_io_sram_port_name);

    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid primitive_tb_type!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* Dump the subckt of a hardlogic and also the input stimuli */
void fprint_spice_primitive_testbench_call_one_primitive(FILE* fp, 
                                                         char* subckt_name, 
                                                         char* primitive_type,
                                                         t_spice_model* primitive_spice_model) {
  int iport, ipin;

  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  t_spice_model_port** inout_ports = NULL;
  int num_clk_port = 0;
  t_spice_model_port** clk_ports = NULL;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert(NULL != primitive_spice_model);

  /* identify the type of spice model */
  /* Call defined subckt */
  fprintf(fp, "X%s_%s[%d] \n+ ", 
          primitive_type,
          primitive_spice_model->prefix,
          primitive_spice_model->tb_cnt);

  /* Sequence in dumping ports: 
   * 1. Global ports - INCLUDED IN THE MODULE, SO WE SKIP THIS
   * 2. Input ports
   * 3. Output ports
   * 4. Inout ports
   * 5. Configuration ports    
   * 6. VDD and GND ports 
   */

  /* 2. Input ports (TODO: check the number of inputs matches the spice model definition) */
  /* Find pb_type input ports */
  input_ports = find_spice_model_ports(primitive_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  for (iport = 0; iport < num_input_port; iport++) {
    for (ipin = 0; ipin < input_ports[iport]->size; ipin++) {
      fprintf(fp, "%s_%s[%d]->%s[%d] ", 
                  primitive_type,
                  primitive_spice_model->prefix,
                  primitive_spice_model->tb_cnt, 
                  input_ports[iport]->prefix, ipin);
    }
  }
  if (NULL != input_ports) {
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }

  /* 3. Output ports */
  /* Find pb_type output ports */
  output_ports = find_spice_model_ports(primitive_spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  for (iport = 0; iport < num_output_port; iport++) {
    for (ipin = 0; ipin < output_ports[iport]->size; ipin++) {
      fprintf(fp, "%s_%s[%d]->%s[%d] ", 
                  primitive_type,
                  primitive_spice_model->prefix,
                  primitive_spice_model->tb_cnt, 
                  output_ports[iport]->prefix, ipin);
    }
  }
  if (NULL != output_ports) {
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }

  /* 4. Inout ports: INOUTs are currently global ports, so we do not put it here. */
  /* INOUT ports */
  /* Find pb_type inout ports */
  /*
  inout_ports = find_spice_model_ports(primitive_spice_model, SPICE_MODEL_PORT_INOUT, &num_inout_port, TRUE);
  for (iport = 0; iport < num_inout_port; iport++) {
    for (ipin = 0; ipin < inout_ports[iport]->size; ipin++) {
      fprintf(fp, "%s_%s[%d]->%s[%d] ", 
                  primitive_type,
                  primitive_spice_model->prefix,
                  primitive_spice_model->tb_cnt, 
                  inout_ports[iport]->prefix, ipin);
    }
  }
  if (NULL != inout_ports) {
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }
  */

  /* Clocks */
  /* Identify if the clock port is a global signal */
  /* Find pb_type clock ports */
  clk_ports = find_spice_model_ports(primitive_spice_model, SPICE_MODEL_PORT_CLOCK, &num_clk_port, TRUE);
  for (iport = 0; iport < num_clk_port; iport++) {
    for (ipin = 0; ipin < clk_ports[iport]->size; ipin++) {
      fprintf(fp, "%s_%s[%d]->%s[%d] ", 
                  primitive_type,
                  primitive_spice_model->prefix,
                  primitive_spice_model->tb_cnt, 
                  clk_ports[iport]->prefix, ipin);
    }
  }
  if (NULL != clk_ports) {
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }

  /* 5. Configuration ports */
  /* Generate SRAMs? */

  /* 6. VDD and GND ports */
  fprintf(fp, "%s_%s[%d] %s ",
          spice_tb_global_vdd_port_name, 
          primitive_spice_model->prefix, 
          primitive_spice_model->tb_cnt,
          spice_tb_global_gnd_port_name);
  fprintf(fp, "\n");
  fprintf(fp, "+ ");

  /* Call the name of subckt */
  fprintf(fp, "%s\n", subckt_name);

  /* Free */
  my_free(input_ports);
  my_free(output_ports);
  my_free(inout_ports);
  my_free(clk_ports);

  return; 
}

void fprint_spice_primitive_testbench_one_primitive_input_stimuli(FILE* fp, 
                                                                  t_phy_pb* cur_pb, 
                                                                  t_pb_graph_node* cur_pb_graph_node, 
                                                                  char* prefix,
                                                                  int x, int y,
                                                                  char* primitive_type,
                                                                  t_ivec*** LL_rr_node_indices) {
  int iport, ipin;
  t_spice_model* pb_spice_model = cur_pb_graph_node->pb_type->spice_model;
  t_rr_node* local_rr_graph = NULL;

  /* For pb_spice_model */
  int num_input_port;
  t_spice_model_port** input_ports;

  /* Two-dimension arrays, corresponding to the port map [port_id][pin_id] */
  float** input_density = NULL;
  float** input_probability = NULL;
  int** input_init_value = NULL;
  int** input_net_num = NULL;

  float average_density = 0.;
  int avg_density_cnt = 0;
  int num_sim_clock_cycles = 0;

  /* Malloc */
  /* First dimension */
  input_density = (float**)my_malloc(sizeof(float*) * cur_pb_graph_node->num_input_ports); 
  input_probability = (float**)my_malloc(sizeof(float*) * cur_pb_graph_node->num_input_ports); 
  input_init_value = (int**)my_malloc(sizeof(int*) * cur_pb_graph_node->num_input_ports); 
  input_net_num = (int**)my_malloc(sizeof(int*) * cur_pb_graph_node->num_input_ports); 
  /* Second dimension */
  for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
    input_density[iport] = (float*)my_malloc(sizeof(float) * cur_pb_graph_node->num_input_pins[iport]);
    input_probability[iport] = (float*)my_malloc(sizeof(float) * cur_pb_graph_node->num_input_pins[iport]);
    input_init_value[iport] = (int*)my_malloc(sizeof(int) * cur_pb_graph_node->num_input_pins[iport]);
    input_net_num[iport] = (int*)my_malloc(sizeof(int) * cur_pb_graph_node->num_input_pins[iport]);
  }

  /* Get activity information */
  for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
      /* if we find a mapped logic block */
      if (NULL != cur_pb) {
        local_rr_graph = cur_pb->rr_graph->rr_node;
      } else {
        local_rr_graph = NULL;
      }
      input_net_num[iport][ipin] = pb_pin_net_num(local_rr_graph, &(cur_pb_graph_node->input_pins[iport][ipin]));
      input_density[iport][ipin] = pb_pin_density(local_rr_graph, &(cur_pb_graph_node->input_pins[iport][ipin]));
      input_probability[iport][ipin] = pb_pin_probability(local_rr_graph, &(cur_pb_graph_node->input_pins[iport][ipin]));
      input_init_value[iport][ipin] =  pb_pin_init_value(local_rr_graph, &(cur_pb_graph_node->input_pins[iport][ipin]));
    } 
  }

  /* Add Input stimulates */ 
  /* Get the input port list of spice model */
  input_ports = find_spice_model_ports(pb_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  /* Check if the port map of current pb_graph_node matches that of the spice model !!!*/
  assert(num_input_port == cur_pb_graph_node->num_input_ports);
  for (iport = 0; iport < num_input_port; iport++) {
    assert(input_ports[iport]->size == cur_pb_graph_node->num_input_pins[iport]); 
    for (ipin = 0; ipin < input_ports[iport]->size; ipin++) {
      /* Check the port size should match!*/
      fprintf(fp, "V%s_%s[%d]->%s[%d] %s_%s[%d]->%s[%d] 0 \n",
              primitive_type, 
              pb_spice_model->prefix, 
              pb_spice_model->tb_cnt, 
              cur_pb_graph_node->input_pins[iport]->port->name,
              ipin, 
              primitive_type, 
              pb_spice_model->prefix, 
              pb_spice_model->tb_cnt, 
              input_ports[iport]->prefix,
              ipin);
      fprint_voltage_pulse_params(fp, input_init_value[iport][ipin], input_density[iport][ipin], input_probability[iport][ipin]);
    }
  }

  /* Calculate average density of this hardlogic */
  average_density = 0.;
  avg_density_cnt = 0;
  for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
    for (ipin = 0; ipin < cur_pb_graph_node->num_input_pins[iport]; ipin++) {
      assert(!(0 > input_density[iport][ipin]));
      if (0. < input_density[iport][ipin]) {
        average_density += input_density[iport][ipin];
        avg_density_cnt++;
      }
    }
  }
  /* Calculate the num_sim_clock_cycle for this MUX, update global max_sim_clock_cycle in this testbench */
  if (0 < avg_density_cnt) {
    average_density = average_density/avg_density_cnt;
  } else {
    assert(0 == avg_density_cnt);
    average_density = 0.;
  }
  if (0. == average_density) {
    num_sim_clock_cycles = 2;
  } else {
    assert(0. < average_density);
    num_sim_clock_cycles = (int)(1/average_density) + 1;
  }
  if (TRUE == auto_select_max_sim_num_clock_cycles) {
    /* for idle blocks, 2 clock cycle is well enough... */
    if (2 < num_sim_clock_cycles) {
      num_sim_clock_cycles = upbound_sim_num_clock_cycles;
    } else {
      num_sim_clock_cycles = 2;
    }
    if (max_sim_num_clock_cycles < num_sim_clock_cycles) {
      max_sim_num_clock_cycles = num_sim_clock_cycles;
    }
  } else {
    num_sim_clock_cycles = max_sim_num_clock_cycles;
  }

  /* Free */
  for (iport = 0; iport < cur_pb_graph_node->num_input_ports; iport++) {
    my_free(input_net_num[iport]);
    my_free(input_init_value[iport]);
    my_free(input_density[iport]);
    my_free(input_probability[iport]);
  }
  my_free(input_net_num);
  my_free(input_init_value);
  my_free(input_density);
  my_free(input_probability);
  my_free(input_ports);

  return;
}

void fprint_spice_primitive_testbench_one_primitive_output_loads(FILE* fp, 
                                                                 t_phy_pb* cur_pb, 
                                                                 t_pb_graph_node* cur_pb_graph_node, 
                                                                 char* prefix,
                                                                 int x, int y,
                                                                 char* primitive_type,
                                                                 t_ivec*** LL_rr_node_indices) {
  int iport, ipin;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  t_spice_model* pb_spice_model = cur_pb_graph_node->pb_type->spice_model;
  char* outport_name = NULL;

  /* Add loads: Recursively */
  /* Get the output port list of spice model */
  output_ports = find_spice_model_ports(pb_spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  for (iport = 0; iport < num_output_port; iport++) {
    for (ipin = 0; ipin < output_ports[iport]->size; ipin++) {
      outport_name = (char*)my_malloc(sizeof(char)*( strlen(primitive_type) + 1
                                      + strlen(pb_spice_model->prefix) + 1 
                                      + strlen(my_itoa(pb_spice_model->tb_cnt)) 
                                      + 3 + strlen(output_ports[iport]->prefix) + 1 
                                      + strlen(my_itoa(ipin)) + 2 ));
      sprintf(outport_name, "%s_%s[%d]->%s[%d]",
                            primitive_type,
                            pb_spice_model->prefix,
                            pb_spice_model->tb_cnt,
                            output_ports[iport]->prefix,
                            ipin);
      if (TRUE == run_testbench_load_extraction) { /* Additional switch, default on! */
        if (NULL != cur_pb) {
          fprint_spice_testbench_pb_graph_pin_inv_loads_rec(fp, &testbench_load_cnt,
                                                            x, y, 
                                                            &(cur_pb_graph_node->output_pins[0][0]), 
                                                            cur_pb, 
                                                            outport_name, 
                                                            FALSE, 
                                                            LL_rr_node_indices); 
        } else {
          fprint_spice_testbench_pb_graph_pin_inv_loads_rec(fp, &testbench_load_cnt,
                                                            x, y, 
                                                            &(cur_pb_graph_node->output_pins[0][0]), 
                                                            NULL, 
                                                            outport_name, 
                                                            FALSE, 
                                                            LL_rr_node_indices); 
        }
      }
      /* Free outport_name in each iteration */
      my_free(outport_name);
    }
  }

  /* Free */
  my_free(output_ports);

  return;
}

/** Core function: print the main body of this testbench 
 * 1. Print the primitive subckt
 * 2. Add input stimuli 
 * 3. Add loads
 */
void fprint_spice_primitive_testbench_one_pb_primitive(FILE* fp, 
                                                       t_phy_pb* cur_pb, 
                                                       t_pb_graph_node* cur_pb_graph_node, 
                                                       char* prefix,
                                                       int x, int y,
                                                       enum e_spice_tb_type primitive_tb_type,
                                                       t_ivec*** LL_rr_node_indices) {
  t_spice_model* pb_spice_model = NULL;
  char* primitive_type = NULL;

  assert(NULL != cur_pb_graph_node);
  assert(NULL != prefix);

  pb_spice_model = cur_pb_graph_node->pb_type->spice_model; 

  /* Name the primitive subckt */
  switch (primitive_tb_type) {
  case SPICE_LUT_TB:
    primitive_type = "lut";
    /* If the spice model is not the type we want, return here */
    if (SPICE_MODEL_LUT != pb_spice_model->type) {
      return;
    }
    break;
  case SPICE_HARDLOGIC_TB:
    primitive_type = "hardlogic";
    /* If the spice model is not the type we want, return here */
    if ((SPICE_MODEL_FF != pb_spice_model->type) 
      && (SPICE_MODEL_HARDLOGIC != pb_spice_model->type)) {
      return;
    }
    break;
  case SPICE_IO_TB:
    primitive_type = "io";
    /* If the spice model is not the type we want, return here */
    if (SPICE_MODEL_IOPAD != pb_spice_model->type) {
      return;
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid primitive_tb_type!\n", 
               __FILE__, __LINE__);
    exit(1);
  }
 
  /* Now, we print the SPICE subckt of a hard logic */
  fprint_spice_primitive_testbench_call_one_primitive(fp, 
                                                      prefix, 
                                                      primitive_type, 
                                                      pb_spice_model);
  
  /* Add input stimuli */
  fprint_spice_primitive_testbench_one_primitive_input_stimuli(fp, 
                                                               cur_pb, 
                                                               cur_pb_graph_node, 
                                                               prefix, 
                                                               x, y, 
                                                               primitive_type, 
                                                               LL_rr_node_indices); 

  /* Add loads */
  fprint_spice_primitive_testbench_one_primitive_output_loads(fp,
                                                              cur_pb, 
                                                              cur_pb_graph_node, 
                                                              prefix, 
                                                              x, y, 
                                                              primitive_type, 
                                                              LL_rr_node_indices); 

  /* Increment the counter of the hardlogic spice model */
  pb_spice_model->tb_cnt++;
  tb_num_primitive++;

  return; 
}

void fprint_spice_primitive_testbench_rec_pb_primitives(FILE* fp, 
                                                        t_phy_pb* cur_pb, 
                                                        t_pb_graph_node* cur_pb_graph_node, 
                                                        char* prefix, 
                                                        int x, int y,
                                                        enum e_spice_tb_type primitive_tb_type,
                                                        t_ivec*** LL_rr_node_indices) {
  char* formatted_prefix = format_spice_node_prefix(prefix); 
  int ipb, jpb;
  int mode_index;
  char* rec_prefix = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  if (NULL == cur_pb) {
    assert(NULL != cur_pb_graph_node);
  } else {
    assert (cur_pb_graph_node == cur_pb->pb_graph_node);
  }

  /* If we touch the leaf, there is no need print interc*/
  if (TRUE == is_primitive_pb_type(cur_pb_graph_node->pb_type)) {
    /* Generate rec_prefix */
    rec_prefix = (char*)my_malloc(sizeof(char) * (strlen(formatted_prefix) 
                   + strlen(cur_pb_graph_node->pb_type->name) + 1 
                   + strlen(my_itoa(cur_pb_graph_node->placement_index))
                   + 1 + 1));
    sprintf(rec_prefix, "%s%s[%d]",
            formatted_prefix, 
            cur_pb_graph_node->pb_type->name, 
            cur_pb_graph_node->placement_index);
    /* Print a lut tb: call spice_model, stimulates */
    fprint_spice_primitive_testbench_one_pb_primitive(fp, 
                                                      cur_pb, 
                                                      cur_pb_graph_node, 
                                                      rec_prefix, x, y, 
                                                      primitive_tb_type, 
                                                      LL_rr_node_indices);
    my_free(rec_prefix);
    return;
  }
  
  /* Go recursively ... */
  if (NULL == cur_pb) {
    mode_index = find_pb_type_physical_mode_index(*(cur_pb_graph_node->pb_type));
  } else {
    mode_index = cur_pb->mode;
  }

  if (!(0 < cur_pb_graph_node->pb_type->num_modes)) {
    return;
  }
  for (ipb = 0; ipb < cur_pb_graph_node->pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb_graph_node->pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Generate rec_prefix */
      rec_prefix = (char*)my_malloc(sizeof(char) * (strlen(formatted_prefix) 
                     + strlen(cur_pb_graph_node->pb_type->name) + 1 
                     + strlen(my_itoa(cur_pb_graph_node->placement_index)) + 7 
                     + strlen(cur_pb_graph_node->pb_type->modes[mode_index].name) + 1 + 1));
      sprintf(rec_prefix, "%s%s[%d]_mode[%s]",
              formatted_prefix, cur_pb_graph_node->pb_type->name, 
              cur_pb_graph_node->placement_index,
              cur_pb_graph_node->pb_type->modes[mode_index].name);
      if (((NULL == cur_pb) 
         || ((NULL == cur_pb->child_pbs[ipb])||(NULL == cur_pb->child_pbs[ipb][jpb].name)))) {
        /* Then we go on */
        fprint_spice_primitive_testbench_rec_pb_primitives(fp, 
														   NULL, 
                                                           &(cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]), 
                                                           rec_prefix, x, y, 
                                                           primitive_tb_type, 
                                                           LL_rr_node_indices);
      } else {
        assert ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name));
        /* Refer to pack/output_clustering.c [LINE 392] */
        fprint_spice_primitive_testbench_rec_pb_primitives(fp, 
                                                           &(cur_pb->child_pbs[ipb][jpb]), 
                                                           cur_pb->child_pbs[ipb][jpb].pb_graph_node, 
                                                           rec_prefix, x, y, 
                                                           primitive_tb_type, 
                                                           LL_rr_node_indices);
      }
    }
  }
  
  return;
}

void fprint_spice_primitive_testbench_call_one_grid_defined_primitives(FILE* fp,
                                                                       int ix, int iy,
                                                                       enum e_spice_tb_type primitive_tb_type,
                                                                       t_ivec*** LL_rr_node_indices) {
  int iblk;
  char* prefix = NULL;
 
  if ((NULL == grid[ix][iy].type)
     ||(EMPTY_TYPE == grid[ix][iy].type)
     ||(0 != grid[ix][iy].offset)) {
    return; 
  }

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d])Invalid File Handler!\n",
                __FILE__, __LINE__);
    exit(1);
  }

  for (iblk = 0; iblk < grid[ix][iy].usage; iblk++) {
    prefix = (char*)my_malloc(sizeof(char)* (5 
                    + strlen(my_itoa(ix)) 
                    + 2 + strlen(my_itoa(iy)) 
                    + 3 ));
    sprintf(prefix, "grid[%d][%d]_", 
            ix,
            iy);
    /* Only for mapped block */
    assert(NULL != block[grid[ix][iy].blocks[iblk]].phy_pb);
    /* It is weird that some used block has an invalid ID */
    if (OPEN == grid[ix][iy].blocks[iblk]) { 
      /* Mark the temporary net_num for the type pins*/
      mark_grid_type_pb_graph_node_pins_temp_net_num(ix, iy);
      /* Go into the hierachy and dump hardlogics */
      fprint_spice_primitive_testbench_rec_pb_primitives(fp, 
                                                         NULL, 
                                                         grid[ix][iy].type->pb_graph_head, 
                                                         prefix, ix, iy, 
                                                         primitive_tb_type, 
                                                         LL_rr_node_indices); 
      continue;
    }
    /* Mark the temporary net_num for the type pins*/
    mark_one_pb_parasitic_nets((t_phy_pb*)block[grid[ix][iy].blocks[iblk]].phy_pb);
    /* Go into the hierachy and dump hardlogics */
    fprint_spice_primitive_testbench_rec_pb_primitives(fp, 
                                                       (t_phy_pb*)block[grid[ix][iy].blocks[iblk]].phy_pb, 
                                                       grid[ix][iy].type->pb_graph_head, 
                                                       prefix, ix, iy, 
                                                       primitive_tb_type, 
                                                       LL_rr_node_indices);
    /* Free */
    my_free(prefix);
  }
  /* Bypass unused blocks */
  for (iblk = grid[ix][iy].usage; iblk < grid[ix][iy].type->capacity; iblk++) {
    prefix = (char*)my_malloc(sizeof(char)* (5 + strlen(my_itoa(ix)) 
                              + 2 + strlen(my_itoa(iy)) + 3 ));
    sprintf(prefix, "grid[%d][%d]_", ix, iy);
    assert(NULL != grid[ix][iy].type->pb_graph_head);
    /* Mark the temporary net_num for the type pins*/
    mark_grid_type_pb_graph_node_pins_temp_net_num(ix, iy);
    /* Go into the hierachy and dump hardlogics */
    fprint_spice_primitive_testbench_rec_pb_primitives(fp, 
                                                       NULL, 
                                                       grid[ix][iy].type->pb_graph_head, 
                                                       prefix, ix, iy, 
                                                       primitive_tb_type, 
                                                       LL_rr_node_indices); 
    /* Free */
    my_free(prefix);
  }

  return;
}

static 
void fprint_spice_primitive_testbench_stimulations(FILE* fp, int grid_x, int grid_y, 
                                                   int num_clocks, 
                                                   t_spice spice, 
                                                   enum e_spice_tb_type primitive_tb_type,
                                                   t_ivec*** LL_rr_node_indices) {
  /* Print generic stimuli */
  fprint_spice_testbench_generic_global_ports_stimuli(fp, num_clocks);
  
  /* Generate global ports stimuli */
  fprint_spice_testbench_global_ports_stimuli(fp, global_ports_head);

  /* SRAM ports */
  fprintf(fp, "***** Global Inputs for SRAMs *****\n");
  fprint_spice_testbench_global_sram_inport_stimuli(fp, sram_spice_orgz_info);

  fprintf(fp, "***** Global VDD for SRAMs *****\n");
  fprint_spice_testbench_global_vdd_port_stimuli(fp,
                                                 spice_tb_global_vdd_sram_port_name,
                                                 "vsp");

  fprintf(fp, "***** Global VDD for load inverters *****\n");
  fprint_spice_testbench_global_vdd_port_stimuli(fp,
                                                 spice_tb_global_vdd_load_port_name,
                                                 "vsp");

  switch (primitive_tb_type) {
  case SPICE_LUT_TB:
    /* Every LUT use an independent Voltage source */
    fprintf(fp, "***** Global VDD for LUTs *****\n");
    fprint_grid_splited_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_LUT, spice);
    break;
  case SPICE_HARDLOGIC_TB:
    /* Every Hardlogic use an independent Voltage source */
    fprintf(fp, "***** Global VDD for FFs *****\n");
    fprint_grid_splited_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_FF, spice);
    fprintf(fp, "***** Global VDD for Hardlogics *****\n");
    fprint_grid_splited_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_HARDLOGIC, spice);
    break;
  case SPICE_IO_TB:
    /* Every IO use an independent Voltage source */
    fprintf(fp, "***** Global VDD for IOs *****\n");
    fprint_grid_splited_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_IOPAD, spice);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid primitive_tb_type!\n", 
               __FILE__, __LINE__);
    exit(1);
  }
 
  return;
}

void fprint_spice_primitive_testbench_measurements(FILE* fp, int grid_x, int grid_y, 
                                                   t_spice spice, 
                                                   enum e_spice_tb_type primitive_tb_type,
                                                   boolean leakage_only) {
 
  /* int i; */
  /* First cycle reserved for measuring leakage */
  int num_clock_cycle = max_sim_num_clock_cycles;
  
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  
  fprint_spice_netlist_transient_setting(fp, spice, num_clock_cycle, leakage_only);
  fprint_spice_netlist_generic_measurements(fp, spice.spice_params.mc_params, spice.num_spice_model, spice.spice_models);

  /* TODO: Measure the delay of each mapped net and logical block */

  /* Measure the power */
  /* Leakage ( the first cycle is reserved for leakage measurement) */
  switch (primitive_tb_type) {
  case SPICE_LUT_TB:
    /* Leakage power of LUTs*/
    fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_LUT, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, spice, leakage_only);
    break;
  case SPICE_HARDLOGIC_TB:
    /* Leakage power of FFs*/
    fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_FF, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, spice, leakage_only);
    /* Leakage power of Hardlogic */
    fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_HARDLOGIC, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, spice, leakage_only);
    break;
  case SPICE_IO_TB:
    /* Leakage power of LUTs*/
    fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_IOPAD, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, spice, leakage_only);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid primitive_tb_type!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  if (TRUE == leakage_only) {
    return;
  }

  /* Dynamic power */
  switch (primitive_tb_type) {
  case SPICE_LUT_TB:
    /* Dynamic power of FFs */
    fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_LUT, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, spice, leakage_only);
    break;
  case SPICE_HARDLOGIC_TB:
    /* Dynamic power of FFs */
    fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_FF, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, spice, leakage_only);
    /* Dynamic power of Hardlogics */
    fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_HARDLOGIC, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, spice, leakage_only);
    break;
  case SPICE_IO_TB:
    /* Dynamic power of FFs */
    fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_IOPAD, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, spice, leakage_only);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid primitive_tb_type!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* Top-level function in this source file */
int fprint_spice_one_primitive_testbench(char* formatted_spice_dir,
                                         char* circuit_name,
                                         char* primitive_testbench_name,
                                         char* include_dir_path,
                                         char* subckt_dir_path,
                                         t_ivec*** LL_rr_node_indices,
                                         int num_clock,
                                         t_arch arch,
                                         int grid_x, int grid_y,
                                         enum e_spice_tb_type primitive_tb_type,
                                         boolean leakage_only) {
  FILE* fp = NULL;
  char* formatted_subckt_dir_path = format_dir_path(subckt_dir_path);
  char* temp_include_file_path = NULL;
  char* title = NULL;
  char* primitive_testbench_file_path = NULL;
  int used;

  /* Name the testbench */
  switch (primitive_tb_type) {
  case SPICE_LUT_TB:
    title = my_strcat("FPGA LUT Testbench for Design: ", circuit_name);
    /* vpr_printf(TIO_MESSAGE_INFO, "Writing LUT Testbench for %s...\n", circuit_name); */
    break;
  case SPICE_HARDLOGIC_TB:
    title = my_strcat("FPGA Hard Logic Testbench for Design: ", circuit_name);
    /* vpr_printf(TIO_MESSAGE_INFO, "Writing Hard Logic Testbench for %s...\n", circuit_name); */
    break;
  case SPICE_IO_TB:
    title = my_strcat("FPGA IO Testbench for Design: ", circuit_name);
    /* vpr_printf(TIO_MESSAGE_INFO, "Writing IO Testbench for %s...\n", circuit_name); */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid primitive_tb_type!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  primitive_testbench_file_path = my_strcat(formatted_spice_dir, primitive_testbench_name);

  /* Check if the path exists*/
  fp = fopen(primitive_testbench_file_path,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Failure in create Primitive Testbench SPICE netlist %s!",
               __FILE__, __LINE__, 
               primitive_testbench_file_path); 
    exit(1);
  } 

  /* Reset tb_cnt for all the spice models */
  init_spice_models_grid_tb_cnt(arch.spice->num_spice_model, arch.spice->spice_models, grid_x, grid_y);
  
  testbench_load_cnt = 0;
 
  /* Print the title */
  fprint_spice_head(fp, title);
  my_free(title);

  /* print technology library and design parameters*/
  /* fprint_tech_lib(fp, arch.spice->tech_lib);*/

  /* Include parameter header files */
  fprint_spice_include_param_headers(fp, include_dir_path);

  /* Include Key subckts */
  fprint_spice_include_key_subckts(fp, subckt_dir_path);

  /* Include user-defined sub-circuit netlist */
  init_include_user_defined_netlists(*(arch.spice));
  fprint_include_user_defined_netlists(fp, *(arch.spice));

  fprintf(fp, "****** Include subckt netlists: LUTs *****\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, luts_spice_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  /* Generate filename */
  fprintf(fp, "****** Include subckt netlists: Grid[%d][%d] *****\n",
          grid_x, grid_y);
  temp_include_file_path = fpga_spice_create_one_subckt_filename(grid_spice_file_name_prefix, grid_x, grid_y, spice_netlist_file_postfix);
  /* Check if we include an existing file! */
  if (FALSE == check_subckt_file_exist_in_llist(grid_spice_subckt_file_path_head, 
                                                my_strcat(formatted_subckt_dir_path, temp_include_file_path))) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Intend to include a non-existed SPICE netlist %s!\n",
               __FILE__, __LINE__, temp_include_file_path); 
    exit(1);
  }
  spice_print_one_include_subckt_line(fp, formatted_subckt_dir_path, temp_include_file_path);


  /* Print simulation temperature and other options for SPICE */
  fprint_spice_options(fp, arch.spice->spice_params);

  /* Global nodes: Vdd for SRAMs, Logic Blocks(Include IO), Switch Boxes, Connection Boxes */
  fprint_spice_primitive_testbench_global_ports(fp, 
                                                grid_x, grid_y, num_clock, 
                                                primitive_tb_type, 
                                                (*arch.spice));

  /* Initialize global variables in this testbench */
  init_spice_primitive_testbench_globals(*(arch.spice));

  /* Quote defined Logic blocks subckts (Grids) */
  switch (primitive_tb_type) {
  case SPICE_LUT_TB:
    init_logical_block_spice_model_type_temp_used(arch.spice->num_spice_model, 
                                                  arch.spice->spice_models, 
                                                  SPICE_MODEL_LUT);
    break;
  case SPICE_HARDLOGIC_TB:
    init_logical_block_spice_model_type_temp_used(arch.spice->num_spice_model, 
                                                  arch.spice->spice_models, 
                                                  SPICE_MODEL_FF);
    init_logical_block_spice_model_type_temp_used(arch.spice->num_spice_model, 
                                                  arch.spice->spice_models, 
                                                  SPICE_MODEL_HARDLOGIC);
    break;
  case SPICE_IO_TB:
    init_logical_block_spice_model_type_temp_used(arch.spice->num_spice_model, 
                                                  arch.spice->spice_models, 
                                                  SPICE_MODEL_IOPAD);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid primitive_tb_type!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  /* Now start our job formally: dump hard logic circuit one by one */
  fprint_spice_primitive_testbench_call_one_grid_defined_primitives(fp, 
                                                                    grid_x, grid_y, 
                                                                    primitive_tb_type, 
                                                                    LL_rr_node_indices);

  /* Back-anotate activity information to each routing resource node 
   * (We should have activity of each Grid port) 
   */

  /* Check if the all hardlogic located in this grid have been printed */
  switch (primitive_tb_type) {
  case SPICE_LUT_TB:
    check_spice_models_grid_tb_cnt(arch.spice->num_spice_model, arch.spice->spice_models, grid_x, grid_y, SPICE_MODEL_LUT);
    break;
  case SPICE_HARDLOGIC_TB:
    check_spice_models_grid_tb_cnt(arch.spice->num_spice_model, arch.spice->spice_models, grid_x, grid_y, SPICE_MODEL_FF);
    check_spice_models_grid_tb_cnt(arch.spice->num_spice_model, arch.spice->spice_models, grid_x, grid_y, SPICE_MODEL_HARDLOGIC);
    break;
  case SPICE_IO_TB:
    check_spice_models_grid_tb_cnt(arch.spice->num_spice_model, arch.spice->spice_models, grid_x, grid_y, SPICE_MODEL_IOPAD);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid primitive_tb_type!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  /* Add stimulations */
  fprint_spice_primitive_testbench_stimulations(fp, grid_x, grid_y, num_clock, (*arch.spice), primitive_tb_type, LL_rr_node_indices);

  /* Add measurements */  
  fprint_spice_primitive_testbench_measurements(fp, grid_x, grid_y, (*arch.spice), primitive_tb_type, leakage_only);

  /* SPICE ends*/
  fprintf(fp, ".end\n");

  /* Close the file*/
  fclose(fp);

  if (0 < tb_num_primitive) {
    /*
    vpr_printf(TIO_MESSAGE_INFO, "Writing Grid[%d][%d] SPICE Hard Logic Testbench for %s...\n",
               grid_x, grid_y, circuit_name);
    */
    /* Push the testbench to the linked list */
    tb_head = add_one_spice_tb_info_to_llist(tb_head, primitive_testbench_file_path, 
                                             max_sim_num_clock_cycles);
    used = 1;
  } else {
    /* Remove the file generated */
    my_remove_file(primitive_testbench_file_path);
    used = 0;
  }

  return used;
}

/* Top-level function in this source file */
void spice_print_primitive_testbench(char* formatted_spice_dir,
                                     char* circuit_name,
                                     char* include_dir_path,
                                     char* subckt_dir_path,
                                     t_ivec*** LL_rr_node_indices,
                                     int num_clock,
                                     t_arch arch,
                                     enum e_spice_tb_type primitive_tb_type,
                                     boolean leakage_only) {
  char* primitive_testbench_name = NULL;
  char* primitive_testbench_postfix = NULL;
  int ix, iy;
  int cnt = 0;
  int used = 0;


  /* Other testbenches consider the core logic only */
  for (ix = 0; ix < (nx + 2); ix++) {
    for (iy = 0; iy < (ny + 2); iy++) {
      /* Bypass EMPTY GRIDs */
      if (EMPTY_TYPE == grid[ix][iy].type) {
        continue;
      }
      /* If this is a block on perimeter,
       * bypass the grid unless IO_testbench is required  
       */
      if ( ( (nx + 1 == ix) || (0 == ix)
         || (ny + 1 == iy) || (0 == iy) )
         && (SPICE_IO_TB != primitive_tb_type)) {
        continue;
      }
      /* Name the testbench */
      switch (primitive_tb_type) {
      case SPICE_LUT_TB:
        primitive_testbench_postfix = spice_lut_testbench_postfix;
        break;
      case SPICE_HARDLOGIC_TB:
        primitive_testbench_postfix = spice_hardlogic_testbench_postfix;
        break;
      case SPICE_IO_TB:
        primitive_testbench_postfix = spice_io_testbench_postfix;
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File:%s, [LINE%d]) Invalid primitive_tb_type!\n", 
                   __FILE__, __LINE__);
        exit(1);
      }
      primitive_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                            + 6 + strlen(my_itoa(ix)) + 1
                                            + strlen(my_itoa(iy)) + 1
                                            + strlen(primitive_testbench_postfix)  + 1 ));
      sprintf(primitive_testbench_name, "%s_grid%d_%d%s",
              circuit_name, ix, iy, primitive_testbench_postfix);
      /* Start building one testbench */
      used = fprint_spice_one_primitive_testbench(formatted_spice_dir, 
                                                  circuit_name, primitive_testbench_name, 
                                                  include_dir_path, subckt_dir_path, LL_rr_node_indices,
                                                  num_clock, arch, ix, iy, 
                                                  primitive_tb_type, 
                                                  leakage_only);
      if (1 == used) {
        cnt += used;
      }
      /* free */
      my_free(primitive_testbench_name);
    }  
  } 
  /* Update the global counter */
  switch (primitive_tb_type) {
  case SPICE_LUT_TB:
    num_used_lut_tb = cnt;
    vpr_printf(TIO_MESSAGE_INFO,
               "No. of generated LUT testbench = %d\n", 
               num_used_lut_tb);
    break;
  case SPICE_HARDLOGIC_TB:
    num_used_hardlogic_tb = cnt;
    vpr_printf(TIO_MESSAGE_INFO,
               "No. of generated hardlogic testbench = %d\n", 
               num_used_hardlogic_tb);
    break;
  case SPICE_IO_TB:
    num_used_io_tb = cnt;
    vpr_printf(TIO_MESSAGE_INFO,
               "No. of generated IO testbench = %d\n", 
               num_used_io_tb);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid primitive_tb_type!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

