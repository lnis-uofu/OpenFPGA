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

/* Include spice support headers*/
#include "linkedlist.h"
#include "fpga_spice_globals.h"
#include "spice_globals.h"
#include "fpga_spice_utils.h"
#include "spice_utils.h"
#include "spice_mux.h"
#include "spice_pbtypes.h"
#include "spice_subckt.h"

/* local global variables */
static int tb_num_hardlogic = 0;
static int testbench_load_cnt = 0;
static int upbound_sim_num_clock_cycles = 2;
static int max_sim_num_clock_cycles = 2;
static int auto_select_max_sim_num_clock_cycles = TRUE;

/* Subroutines in this source file*/
/* Initialize the global parameters in this source file */
static 
void init_spice_hardlogic_testbench_globals(t_spice spice) {
  tb_num_hardlogic = 0;
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
void fprint_spice_hardlogic_testbench_global_ports(FILE* fp, int grid_x, int grid_y, 
                                                   int num_clock, 
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

  /*Global Vdds for FFs: TODO: TO BE REMOVED */
  fprint_grid_global_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_FF, spice);

  /*Global Vdds for hardlogic */
  fprint_grid_global_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_HARDLOGIC, spice);

  /*Global Vdds for IOPADs (TODO: TO BE MOVED TO IO_TB SOURCE FILE */
  fprint_grid_global_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_IOPAD, spice);

  /* Global VDDs for SRAMs of IOPADs */
  fprintf(fp, ".global %s\n",
               spice_tb_global_vdd_io_sram_port_name);

  return;
}

/* Dump the subckt of a hardlogic and also the input stimuli */
void fprint_spice_hardlogic_testbench_one_hardlogic(FILE* fp, 
                                                    char* subckt_name, 
                                                    t_spice_model* hardlogic_spice_model) {
  int iport, ipin;
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;

  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;

  int num_inout_port = 0;
  t_spice_model_port** inout_ports = NULL;

  int num_clk_port = 0;
  t_spice_model_port** clk_ports = NULL;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert(NULL != hardlogic_spice_model);

  /* identify the type of spice model */
  /* Call defined subckt */
  fprintf(fp, "Xhardlogic_%s[%d] \n", 
          hardlogic_spice_model->prefix,
          hardlogic_spice_model->tb_cnt);

  /* Sequence in dumping ports: 
   * 1. Global ports
   * 2. Input ports
   * 3. Output ports
   * 4. Inout ports
   * 5. Configuration ports    
   * 6. VDD and GND ports 
   */

  /* 1. Global ports */
  if (0 < rec_fprint_spice_model_global_ports(fp, hardlogic_spice_model, FALSE)) { 
    fprintf(fp, "+ ");
  }

  /* 2. Input ports (TODO: check the number of inputs matches the spice model definition) */
  /* Find pb_type input ports */
  input_ports = find_spice_model_ports(hardlogic_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  for (iport = 0; iport < num_input_port; iport++) {
    for (ipin = 0; ipin < input_ports[iport]->size; ipin++) {
      fprintf(fp, "hardlogic_%s[%d]->%s[%d] ", 
                  hardlogic_spice_model->prefix,
                  hardlogic_spice_model->tb_cnt, 
                  input_ports[iport]->prefix, ipin);
    }
  }
  if (NULL != input_ports) {
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }

  /* 3. Output ports */
  /* Find pb_type output ports */
  output_ports = find_spice_model_ports(hardlogic_spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  for (iport = 0; iport < num_output_port; iport++) {
    for (ipin = 0; ipin < output_ports[iport]->size; ipin++) {
      fprintf(fp, "hardlogic_%s[%d]->%s[%d] ", 
                  hardlogic_spice_model->prefix,
                  hardlogic_spice_model->tb_cnt, 
                  output_ports[iport]->prefix, ipin);
    }
  }
  if (NULL != output_ports) {
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }

  /* 4. Inout ports */
  /* INOUT ports */
  /* Find pb_type inout ports */
  inout_ports = find_spice_model_ports(hardlogic_spice_model, SPICE_MODEL_PORT_INOUT, &num_inout_port, TRUE);
  for (iport = 0; iport < num_inout_port; iport++) {
    for (ipin = 0; ipin < inout_ports[iport]->size; ipin++) {
      fprintf(fp, "hardlogic_%s[%d]->%s[%d] ", 
                  hardlogic_spice_model->prefix,
                  hardlogic_spice_model->tb_cnt, 
                  inout_ports[iport]->prefix, ipin);
    }
  }
  if (NULL != inout_ports) {
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
  }

  /* Clocks */
  /* Identify if the clock port is a global signal */
  /* Find pb_type clock ports */
  clk_ports = find_spice_model_ports(hardlogic_spice_model, SPICE_MODEL_PORT_CLOCK, &num_clk_port, TRUE);
  for (iport = 0; iport < num_clk_port; iport++) {
    for (ipin = 0; ipin < clk_ports[iport]->size; ipin++) {
      fprintf(fp, "hardlogic_%s[%d]->%s[%d] ", 
                  hardlogic_spice_model->prefix,
                  hardlogic_spice_model->tb_cnt, 
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
          spice_tb_global_vdd_port_name, hardlogic_spice_model->prefix, hardlogic_spice_model->tb_cnt,
          spice_tb_global_gnd_port_name);
  fprintf(fp, "\n");
  fprintf(fp, "+ ");

  /* Call the name of subckt */
  fprintf(fp, "%s\n", hardlogic_spice_model->name);

  /* Free */
  my_free(input_ports);
  my_free(output_ports);
  my_free(inout_ports);
  my_free(clk_ports);

  return; 
}

void fprint_spice_hardlogic_testbench_one_pb_graph_node_hardlogic(FILE* fp, 
                                                                  t_pb_graph_node* cur_pb_graph_node, 
                                                                  char* prefix,
                                                                  int x, int y,
                                                                  t_ivec*** LL_rr_node_indices) {
  int logical_block_index = OPEN;
  t_spice_model* pb_spice_model = NULL;
  t_pb_type* cur_pb_type = NULL;
  int iport, ipin;

  /* For pb_spice_model */
  int num_input_port;
  t_spice_model_port** input_ports;
  int num_output_port;
  t_spice_model_port** output_ports;

  /* Two-dimension arrays, corresponding to the port map [port_id][pin_id] */
  float** input_density = NULL;
  float** input_probability = NULL;
  int** input_init_value = NULL;
  int** input_net_num = NULL;

  char* outport_name = NULL;
  t_rr_node* local_rr_graph = NULL;
  float average_density = 0.;
  int avg_density_cnt = 0;
  int num_sim_clock_cycles = 0;

  assert(NULL != cur_pb_graph_node);
  assert(NULL != prefix);

  cur_pb_type = cur_pb_graph_node->pb_type;
  assert(NULL != cur_pb_type);
  pb_spice_model = cur_pb_type->spice_model; 

  /* Just a double check*/
  if ((SPICE_MODEL_HARDLOGIC != pb_spice_model->type)
     &&(SPICE_MODEL_FF != pb_spice_model->type)) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Type of SPICE models should be either Flip-Flop or Hard Logic!\n",
               __FILE__, __LINE__);
    exit(1);
  }
 
  /* Try to find the mapped logic block index */
  logical_block_index = find_grid_mapped_logical_block(x, y, 
                                                       pb_spice_model, prefix);

  /* UNCOMMENT THIS, IF YOU DO NOT WANT SIMULATE THE IDLE ELEMENTS
  if (OPEN == logical_block_index) {
    return;
  }
  */
 
  /* Call the subckt and give stimulates, measurements */
  if (OPEN != logical_block_index) {
    fprintf(fp,"***** Hardlogic[%d]: logical_block_index[%d], gvdd_index[%d]*****\n", 
            pb_spice_model->cnt, logical_block_index, logical_block[logical_block_index].mapped_spice_model_index);
  } else {
    fprintf(fp,"***** Hardlogic[%d]: logical_block_index[%d], gvdd_index[%d]*****\n", 
            pb_spice_model->cnt, -1, -1);
  }

  /* Now, we print the SPICE subckt of a hard logic */
  fprint_spice_hardlogic_testbench_one_hardlogic(fp, prefix, pb_spice_model);

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
      if (OPEN != logical_block_index) {
        local_rr_graph = logical_block[logical_block_index].pb->parent_pb->rr_graph;
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
      fprintf(fp, "Vhardlogic_%s[%d]->%s[%d] hardlogic_%s[%d]->%s[%d] 0 \n",
              pb_spice_model->prefix, 
              pb_spice_model->tb_cnt, 
              cur_pb_graph_node->input_pins[iport]->port->name,
              ipin, 
              pb_spice_model->prefix, 
              pb_spice_model->tb_cnt, 
              input_ports[iport]->prefix,
              ipin);
      fprint_voltage_pulse_params(fp, input_init_value[iport][ipin], input_density[iport][ipin], input_probability[iport][ipin]);
    }
  }

  /* Add loads: Recursively */
  /* Get the output port list of spice model */
  output_ports = find_spice_model_ports(pb_spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  for (iport = 0; iport < num_output_port; iport++) {
    for (ipin = 0; ipin < output_ports[iport]->size; ipin++) {
      outport_name = (char*)my_malloc(sizeof(char)*( 10 + 
                                      + strlen(pb_spice_model->prefix) + 1 
                                      + strlen(my_itoa(pb_spice_model->tb_cnt)) 
                                      + 3 + strlen(output_ports[iport]->prefix) + 1 
                                      + strlen(my_itoa(ipin)) + 2 ));
      sprintf(outport_name, "hardlogic_%s[%d]->%s[%d]",
                            pb_spice_model->prefix,
                            pb_spice_model->tb_cnt,
                            output_ports[iport]->prefix,
                            ipin);
      if (TRUE == run_testbench_load_extraction) { /* Additional switch, default on! */
        if (OPEN != logical_block_index) {
          fprint_spice_testbench_pb_graph_pin_inv_loads_rec(fp, &testbench_load_cnt,
                                                            x, y, 
                                                            &(cur_pb_graph_node->output_pins[0][0]), 
                                                            logical_block[logical_block_index].pb, 
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

  /* Mark temporary used */
  if (OPEN != logical_block_index) {
    logical_block[logical_block_index].temp_used = 1;
  }

  /* Increment the counter of the hardlogic spice model */
  pb_spice_model->tb_cnt++;
  tb_num_hardlogic++;

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
  my_free(output_ports);
  
  return; 
}

void fprint_spice_hardlogic_testbench_rec_pb_graph_node_hardlogics(FILE* fp,
                                                                  t_pb_graph_node* cur_pb_graph_node, 
                                                                  char* prefix,
                                                                  int x, int y,
                                                                  t_ivec*** LL_rr_node_indices) {
  char* formatted_prefix = format_spice_node_prefix(prefix); 
  int ipb, jpb, mode_index; 
  t_pb_type* cur_pb_type = NULL;
  char* rec_prefix = NULL;
  
  assert(NULL != cur_pb_graph_node);
  cur_pb_type = cur_pb_graph_node->pb_type;
  assert(NULL != cur_pb_type);
  /* Until we reach a FF */
  if (NULL != cur_pb_type->spice_model) {
    if (SPICE_MODEL_FF != cur_pb_type->spice_model->type) {
      return;
    }
    /* Generate rec_prefix */
    rec_prefix = (char*)my_malloc(sizeof(char) * (strlen(formatted_prefix) 
                   + strlen(cur_pb_type->name) + 1 
                   + strlen(my_itoa(cur_pb_graph_node->placement_index))
                   + 1 + 1));
    sprintf(rec_prefix, "%s%s[%d]",
            formatted_prefix, cur_pb_type->name, cur_pb_graph_node->placement_index);
    /* Print a hardlogic tb: call spice_model, stimulates */
    fprint_spice_hardlogic_testbench_one_pb_graph_node_hardlogic(fp, cur_pb_graph_node, rec_prefix, x, y, LL_rr_node_indices);
    my_free(rec_prefix);
    return;
  }

  /* Go recursively ... */
  mode_index = find_pb_type_idle_mode_index(*(cur_pb_type));
  for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Generate rec_prefix */
      rec_prefix = (char*)my_malloc(sizeof(char) * (strlen(formatted_prefix) 
                     + strlen(cur_pb_type->name) + 1 
                     + strlen(my_itoa(cur_pb_graph_node->placement_index)) + 7 
                     + strlen(cur_pb_type->modes[mode_index].name) + 1 + 1));
      sprintf(rec_prefix, "%s%s[%d]_mode[%s]",
              formatted_prefix, cur_pb_type->name, cur_pb_graph_node->placement_index,
              cur_pb_type->modes[mode_index].name);
      /* Go recursively */
      fprint_spice_hardlogic_testbench_rec_pb_graph_node_hardlogics(fp, &(cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]),
                                                                    rec_prefix, x, y, LL_rr_node_indices);
      my_free(rec_prefix);
    }
  }
  
  return;
}

void fprint_spice_hardlogic_testbench_rec_pb_hardlogics(FILE* fp, 
                                                        t_pb* cur_pb, char* prefix, 
                                                        int x, int y,
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
  assert(NULL != cur_pb);

  /* If we touch the leaf, there is no need print interc*/
  if (NULL != cur_pb->pb_graph_node->pb_type->spice_model) {
    if ((SPICE_MODEL_HARDLOGIC != cur_pb->pb_graph_node->pb_type->spice_model->type)
       &&(SPICE_MODEL_FF != cur_pb->pb_graph_node->pb_type->spice_model->type)) {
      return;
    }
    /* Generate rec_prefix */
    rec_prefix = (char*)my_malloc(sizeof(char) * (strlen(formatted_prefix) 
                   + strlen(cur_pb->pb_graph_node->pb_type->name) + 1 
                   + strlen(my_itoa(cur_pb->pb_graph_node->placement_index))
                   + 1 + 1));
    sprintf(rec_prefix, "%s%s[%d]",
            formatted_prefix, cur_pb->pb_graph_node->pb_type->name, cur_pb->pb_graph_node->placement_index);
    /* Print a lut tb: call spice_model, stimulates */
    fprint_spice_hardlogic_testbench_one_pb_graph_node_hardlogic(fp, cur_pb->pb_graph_node, rec_prefix, x, y, LL_rr_node_indices);
    my_free(rec_prefix);
    return;
  }
  
  /* Go recursively ... */
  mode_index = cur_pb->mode;
  if (!(0 < cur_pb->pb_graph_node->pb_type->num_modes)) {
    return;
  }
  for (ipb = 0; ipb < cur_pb->pb_graph_node->pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for (jpb = 0; jpb < cur_pb->pb_graph_node->pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      /* Generate rec_prefix */
      rec_prefix = (char*)my_malloc(sizeof(char) * (strlen(formatted_prefix) 
                     + strlen(cur_pb->pb_graph_node->pb_type->name) + 1 
                     + strlen(my_itoa(cur_pb->pb_graph_node->placement_index)) + 7 
                     + strlen(cur_pb->pb_graph_node->pb_type->modes[mode_index].name) + 1 + 1));
      sprintf(rec_prefix, "%s%s[%d]_mode[%s]",
              formatted_prefix, cur_pb->pb_graph_node->pb_type->name, 
              cur_pb->pb_graph_node->placement_index,
              cur_pb->pb_graph_node->pb_type->modes[mode_index].name);
      /* Refer to pack/output_clustering.c [LINE 392] */
      if ((NULL != cur_pb->child_pbs[ipb])&&(NULL != cur_pb->child_pbs[ipb][jpb].name)) {
        fprint_spice_hardlogic_testbench_rec_pb_hardlogics(fp, &(cur_pb->child_pbs[ipb][jpb]), rec_prefix, x, y, LL_rr_node_indices);
      } else {
        /* Then we go on */
        fprint_spice_hardlogic_testbench_rec_pb_graph_node_hardlogics(fp, cur_pb->child_pbs[ipb][jpb].pb_graph_node, 
                                                                      rec_prefix, x, y, LL_rr_node_indices);
      }
    }
  }
  
  return;
}

void fprint_spice_hardlogic_testbench_call_one_grid_defined_hardlogics(FILE* fp,
                                                                       int ix, int iy,
                                                                       t_ivec*** LL_rr_node_indices) {
  int iblk;
  char* prefix = NULL;
 
  if (NULL == grid[ix][iy].type) {
    return; 
  }

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  for (iblk = 0; iblk < grid[ix][iy].usage; iblk++) {
    prefix = (char*)my_malloc(sizeof(char)* (5 
                    + strlen(my_itoa(block[grid[ix][iy].blocks[iblk]].x)) 
                    + 2 + strlen(my_itoa(block[grid[ix][iy].blocks[iblk]].y)) 
                    + 3 ));
    sprintf(prefix, "grid[%d][%d]_", 
            block[grid[ix][iy].blocks[iblk]].x,
            block[grid[ix][iy].blocks[iblk]].y);
    /* Only for mapped block */
    assert(NULL != block[grid[ix][iy].blocks[iblk]].pb);
    /* Mark the temporary net_num for the type pins*/
    mark_one_pb_parasitic_nets(block[grid[ix][iy].blocks[iblk]].pb);
    /* Go into the hierachy and dump hardlogics */
    fprint_spice_hardlogic_testbench_rec_pb_hardlogics(fp, block[grid[ix][iy].blocks[iblk]].pb, prefix, ix, iy, LL_rr_node_indices);
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
    fprint_spice_hardlogic_testbench_rec_pb_graph_node_hardlogics(fp, grid[ix][iy].type->pb_graph_head, prefix, ix, iy, LL_rr_node_indices); 
    /* Free */
    my_free(prefix);
  }

  return;
}

void fprint_spice_hardlogic_testbench_call_defined_hardlogics(FILE* fp, 
                                                              t_ivec*** LL_rr_node_indices) {
  int ix, iy;
 
  for (ix = 1; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      fprint_spice_hardlogic_testbench_call_one_grid_defined_hardlogics(fp, ix, iy, LL_rr_node_indices);
    }
  }

  return;
}

static 
void fprint_spice_hardlogic_testbench_stimulations(FILE* fp, int grid_x, int grid_y, 
                                                   int num_clocks, 
                                                   t_spice spice, 
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
  /*
  fprintf(fp, "***** Global VDD for IOPADs *****\n");
  fprint_spice_testbench_global_vdd_port_stimuli(fp,
                                                 spice_tb_global_vdd_io_port_name,
                                                 "vsp");

  fprintf(fp, "***** Global VDD for IOPAD SRAMs *****\n");
  fprint_spice_testbench_global_vdd_port_stimuli(fp,
                                                 spice_tb_global_vdd_io_sram_port_name,
                                                 "vsp");
  */

  /* Every LUT use an independent Voltage source */
  fprintf(fp, "***** Global VDD for FFs *****\n");
  fprint_grid_splited_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_FF, spice);
  fprintf(fp, "***** Global VDD for Hardlogics *****\n");
  fprint_grid_splited_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_HARDLOGIC, spice);
 
  return;
}

void fprint_spice_hardlogic_testbench_measurements(FILE* fp, int grid_x, int grid_y, 
                                                   t_spice spice, 
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
  /* Leakage power of FFs*/
  fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_FF, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, spice, leakage_only);
  /* Leakage power of Hardlogic */
  fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_HARDLOGIC, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, spice, leakage_only);

  if (TRUE == leakage_only) {
    return;
  }

  /* Dynamic power */
  /* Dynamic power of FFs */
  fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_FF, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, spice, leakage_only);

  /* Dynamic power of Hardlogics */
  fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_HARDLOGIC, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, spice, leakage_only);

  return;
}

/* Top-level function in this source file */
int fprint_spice_one_hardlogic_testbench(char* formatted_spice_dir,
                                         char* circuit_name,
                                         char* hardlogic_testbench_name,
                                         char* include_dir_path,
                                         char* subckt_dir_path,
                                         t_ivec*** LL_rr_node_indices,
                                         int num_clock,
                                         t_arch arch,
                                         int grid_x, int grid_y,
                                         boolean leakage_only) {
  FILE* fp = NULL;
  char* formatted_subckt_dir_path = format_dir_path(subckt_dir_path);
  char* temp_include_file_path = NULL;
  char* title = my_strcat("FPGA Hard Logic Testbench for Design: ", circuit_name);
  char* hardlogic_testbench_file_path = my_strcat(formatted_spice_dir, hardlogic_testbench_name);
  int used;

  /* Check if the path exists*/
  fp = fopen(hardlogic_testbench_file_path,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create DFF Testbench SPICE netlist %s!",__FILE__, __LINE__, 
               hardlogic_testbench_file_path); 
    exit(1);
  } 

  /* Reset tb_cnt for all the spice models */
  init_spice_models_grid_tb_cnt(arch.spice->num_spice_model, arch.spice->spice_models, grid_x, grid_y);
  
  /* vpr_printf(TIO_MESSAGE_INFO, "Writing DFF Testbench for %s...\n", circuit_name); */
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

  /* Print simulation temperature and other options for SPICE */
  fprint_spice_options(fp, arch.spice->spice_params);

  /* Global nodes: Vdd for SRAMs, Logic Blocks(Include IO), Switch Boxes, Connection Boxes */
  fprint_spice_hardlogic_testbench_global_ports(fp, grid_x, grid_y, num_clock, (*arch.spice));
 
  /* Quote defined Logic blocks subckts (Grids) */
  init_spice_hardlogic_testbench_globals(*(arch.spice));
  init_logical_block_spice_model_type_temp_used(arch.spice->num_spice_model, arch.spice->spice_models, SPICE_MODEL_FF);
  init_logical_block_spice_model_type_temp_used(arch.spice->num_spice_model, arch.spice->spice_models, SPICE_MODEL_HARDLOGIC);

  /* Now start our job formally: dump hard logic circuit one by one */
  fprint_spice_hardlogic_testbench_call_one_grid_defined_hardlogics(fp, grid_x, grid_y, LL_rr_node_indices);

  /* Back-anotate activity information to each routing resource node 
   * (We should have activity of each Grid port) 
   */

  /* Check if the all hardlogic located in this grid have been printed */
  check_spice_models_grid_tb_cnt(arch.spice->num_spice_model, arch.spice->spice_models, grid_x, grid_y, SPICE_MODEL_FF);
  check_spice_models_grid_tb_cnt(arch.spice->num_spice_model, arch.spice->spice_models, grid_x, grid_y, SPICE_MODEL_HARDLOGIC);

  /* Add stimulations */
  fprint_spice_hardlogic_testbench_stimulations(fp, grid_x, grid_y, num_clock, (*arch.spice), LL_rr_node_indices);

  /* Add measurements */  
  fprint_spice_hardlogic_testbench_measurements(fp, grid_x, grid_y, (*arch.spice), leakage_only);

  /* SPICE ends*/
  fprintf(fp, ".end\n");

  /* Close the file*/
  fclose(fp);

  if (0 < tb_num_hardlogic) {
    vpr_printf(TIO_MESSAGE_INFO, "Writing Grid[%d][%d] SPICE Hard Logic Testbench for %s...\n",
               grid_x, grid_y, circuit_name);
    /* Push the testbench to the linked list */
    tb_head = add_one_spice_tb_info_to_llist(tb_head, hardlogic_testbench_file_path, 
                                             max_sim_num_clock_cycles);
    used = 1;
  } else {
    /* Remove the file generated */
    my_remove_file(hardlogic_testbench_file_path);
    used = 0;
  }

  return used;
}

/* Top-level function in this source file */
void spice_print_hardlogic_testbench(char* formatted_spice_dir,
                                      char* circuit_name,
                                      char* include_dir_path,
                                      char* subckt_dir_path,
                                      t_ivec*** LL_rr_node_indices,
                                      int num_clock,
                                      t_arch arch,
                                      boolean leakage_only) {
  char* hardlogic_testbench_name = NULL;
  int ix, iy;
  int cnt = 0;
  int used = 0;

  for (ix = 1; ix < (nx+1); ix++) {
    for (iy = 1; iy < (ny+1); iy++) {
      /* Name the testbench */
      hardlogic_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                            + 6 + strlen(my_itoa(ix)) + 1
                                            + strlen(my_itoa(iy)) + 1
                                            + strlen(spice_hardlogic_testbench_postfix)  + 1 ));
      sprintf(hardlogic_testbench_name, "%s_grid%d_%d%s",
              circuit_name, ix, iy, spice_hardlogic_testbench_postfix);
      /* Start building one testbench */
      used = fprint_spice_one_hardlogic_testbench(formatted_spice_dir, circuit_name, hardlogic_testbench_name, 
                                                  include_dir_path, subckt_dir_path, LL_rr_node_indices,
                                                  num_clock, arch, ix, iy, 
                                                  leakage_only);
      if (1 == used) {
        cnt += used;
      }
      /* free */
      my_free(hardlogic_testbench_name);
    }  
  } 
  /* Update the global counter */
  num_used_hardlogic_tb = cnt;
  vpr_printf(TIO_MESSAGE_INFO,"No. of generated hard logic testbench = %d\n", num_used_hardlogic_tb);

  return;
}

