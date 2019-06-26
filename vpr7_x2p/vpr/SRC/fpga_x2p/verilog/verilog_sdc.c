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
#include <vector>

/* Include vpr structs*/
#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph_util.h"
#include "rr_graph.h"
#include "rr_graph2.h"
#include "route_common.h"
#include "vpr_utils.h"

/* Include SPICE support headers*/
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_backannotate_utils.h"
#include "fpga_x2p_mux_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "fpga_x2p_rr_graph_utils.h"
#include "fpga_x2p_globals.h"

/* Include Verilog support headers*/
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_routing.h"
#include "verilog_tcl_utils.h"
#include "verilog_sdc_pb_types.h"
#include "verilog_sdc.h"

/* options for report timing */
typedef struct s_sdc_opts t_sdc_opts;
struct s_sdc_opts {
  char* sdc_dir;
  boolean constrain_sbs;
  boolean constrain_cbs;
  boolean constrain_pbs;
  boolean constrain_routing_channels;
  boolean break_loops;
  boolean break_loops_mux;
  boolean compact_routing_hierarchy; /* This option is going to be deprecated when new data structure RRGSB replaces the old data structures */
};

static 
float get_switch_sdc_tmax (t_switch_inf* cur_switch_inf) {
  return cur_switch_inf->R * cur_switch_inf->Cout + cur_switch_inf->Tdel;
}

static 
float get_routing_seg_sdc_tmax (t_segment_inf* cur_seg) {
  return cur_seg->Rmetal * cur_seg->Cmetal;
}

static 
boolean is_rr_node_to_be_disable_for_analysis(t_rr_node* cur_rr_node) {
  /* Conditions to enable timing analysis for a node 
   * 1st condition: it have a valid vpack_net_number 
   * 2nd condition: it is not an parasitic net 
   * 3rd condition: it is not a global net
   */
  if ( (OPEN != cur_rr_node->vpack_net_num) 
    && (FALSE == cur_rr_node->is_parasitic_net)
    && (FALSE == vpack_net[cur_rr_node->vpack_net_num].is_global)
    && (FALSE == vpack_net[cur_rr_node->vpack_net_num].is_const_gen) ){
    return FALSE;
  }
  return TRUE;
}

/* TO avoid combinational loops caused by memories
 * We disable all the timing paths starting from an output of memory cell
 */
static 
void verilog_generate_sdc_break_loop_sram(FILE* fp, 
                                          t_sram_orgz_info* cur_sram_orgz_info) {
  t_spice_model* mem_model = NULL;
  int iport, ipin; 
  char* port_name = NULL;

  int num_output_ports = 0;
  t_spice_model_port** output_ports = NULL;

  /* Check */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
              "(FILE:%s,LINE[%d])Invalid file handler!\n",
              __FILE__, __LINE__); 
    exit(1);
  }

  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
  assert (NULL != mem_model);
  
  /* Find the output ports of mem_model */
  output_ports = find_spice_model_ports(mem_model, SPICE_MODEL_PORT_OUTPUT, &num_output_ports, TRUE);

  for (iport = 0; iport < num_output_ports; iport++) {
    for (ipin = 0; ipin < output_ports[iport]->size; ipin++) { 
      if (TRUE == mem_model->dump_explicit_port_map) {
        port_name = output_ports[iport]->lib_name;
      } else {
        port_name = output_ports[iport]->prefix;
      }
      /* Disable the timing for all the memory cells */
      fprintf(fp, 
              "set_disable_timing [get_pins -filter \"name == %s",
               port_name);
      if (1 < output_ports[iport]->size) {
        fprintf(fp, "[%d]", ipin);
      }
      fprintf(fp, "\" ");
      fprintf(fp, 
              "-of [get_cells -hier -filter \"ref_lib_cell_name == %s\"]]\n",
              mem_model->name);
    }
  }

  /* Free */
  my_free(output_ports);

  return;
}

/* Statisitcs for input sizes and structures of MUXes 
 * used in FPGA architecture
 * Disable timing starting from any MUX outputs 
 */
static 
void verilog_generate_sdc_break_loop_mux(FILE* fp,
                                         int num_switch,
                                         t_switch_inf* switches,
                                         t_spice* spice,
                                         t_det_routing_arch* routing_arch) {
  /* We have linked list whichs stores spice model information of multiplexer*/
  t_llist* muxes_head = NULL; 
  t_llist* temp = NULL;
  t_spice_mux_model* cur_spice_mux_model = NULL;

  int num_input_ports = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_ports = 0;
  t_spice_model_port** output_ports = NULL;

  char* SPC_cell_suffix = "_SPC";

  /* Check */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
              "(FILE:%s,LINE[%d])Invalid file handler!\n",
              __FILE__, __LINE__); 
    exit(1);
  } 

  /* Alloc the muxes*/
  muxes_head = stats_spice_muxes(num_switch, switches, spice, routing_arch);

  /* Print mux netlist one by one*/
  temp = muxes_head;
  while(temp) {
    assert(NULL != temp->dptr);
    cur_spice_mux_model = (t_spice_mux_model*)(temp->dptr);
    input_ports = find_spice_model_ports(cur_spice_mux_model->spice_model, SPICE_MODEL_PORT_INPUT, &num_input_ports, TRUE);
    output_ports = find_spice_model_ports(cur_spice_mux_model->spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_ports, TRUE);
    assert(1 == num_input_ports);
    assert(1 == num_input_ports);
    /* Check the Input port size */
    if ( (NULL != cur_spice_mux_model->spice_model->verilog_netlist) 
      && (cur_spice_mux_model->size != input_ports[0]->size)) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File:%s,[LINE%d])User-defined MUX SPICE MODEL(%s) size(%d) unmatch with the architecture needs(%d)!\n",
                 __FILE__, __LINE__, cur_spice_mux_model->spice_model->name, input_ports[0]->size,cur_spice_mux_model->size);
      exit(1);
    }
    /* Use the user defined ports, Bypass LUT MUXes */
    if (SPICE_MODEL_MUX == cur_spice_mux_model->spice_model->type) {
      fprintf(fp, 
              "set_disable_timing [get_pins -filter \"name =~ %s*\" ",
              output_ports[0]->prefix);
      fprintf(fp, 
              "-of [get_cells -hier -filter \"ref_lib_cell_name == %s\"]]\n",
              gen_verilog_one_mux_module_name(cur_spice_mux_model->spice_model, cur_spice_mux_model->size));
      /* For SPC cells*/
      fprintf(fp, 
              "set_disable_timing [get_pins -filter \"name =~ %s*\" ",
              output_ports[0]->prefix);
      fprintf(fp, 
              "-of [get_cells -hier -filter \"ref_lib_cell_name =~ %s%s*\"]]\n",
              gen_verilog_one_mux_module_name(cur_spice_mux_model->spice_model, cur_spice_mux_model->size),
              SPC_cell_suffix);
    }
    /* Free */
    my_free(output_ports);
    my_free(input_ports);
    /* Move on to the next*/
    temp = temp->next;
  }

  /* remember to free the linked list*/
  free_muxes_llist(muxes_head);

  return;
}

static 
void verilog_generate_sdc_clock_period(t_sdc_opts sdc_opts,
                                       float critical_path_delay) {
  FILE* fp = NULL;
  char* fname = my_strcat(sdc_opts.sdc_dir, sdc_clock_period_file_name);
  t_llist* temp = NULL;
  t_spice_model_port* temp_port = NULL;
  int ipin;
  float clock_period = 10.;
  int iport;
  int num_clock_ports = 0;
  t_spice_model_port** clock_port = NULL;

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for constraining clocks in P&R flow: %s ...\n",
             fname);
  
  /* Print the muxes netlist*/
  fp = fopen(fname, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
              "(FILE:%s,LINE[%d])Failure in create SDC constraints %s",
              __FILE__, __LINE__, fname); 
    exit(1);
  } 
  /* Generate the descriptions*/
  dump_verilog_sdc_file_header(fp, "Clock contraints for PnR");

  /* Create clock */
  /* Get clock port from the global port */
  get_fpga_x2p_global_all_clock_ports(global_ports_head, &num_clock_ports, &clock_port);

  /* Print comments */
  fprintf(fp,
          "##################################################\n"); 
  fprintf(fp, 
          "###             Create clock                     #\n");
  fprintf(fp,
          "##################################################\n"); 

  /* Create a clock */
  for (iport = 0; iport < num_clock_ports; iport++) {
    fprintf(fp, "create_clock ");
    if (NULL != strstr(clock_port[iport]->prefix,"prog")) {
      fprintf(fp, "%s -period 100 -waveform {0 50}\n",
              clock_port[iport]->prefix);
    }
    else {
      fprintf(fp, "%s -period %.4g -waveform {0 %.4g}\n",
              clock_port[iport]->prefix, 
              critical_path_delay, critical_path_delay/2);
    }
  }


  /* Find the global clock ports */
  temp = global_ports_head; 
  while (NULL != temp) {
    /* Get the port */
    temp_port = (t_spice_model_port*)(temp->dptr);
    /* We only care clock ports */
    if (SPICE_MODEL_PORT_CLOCK == temp_port->type) {
      /* Go to next */
      temp = temp->next;
      continue;
    }
    
    for (ipin = 0; ipin < temp_port->size; ipin++) {
      fprintf(fp, 
              "create_clock -name {%s[%d]} -period %.2g -waveform {0.00 %.2g} [list [get_ports {%s[%d]}]]\n",
              temp_port->prefix, ipin,
              clock_period, clock_period/2,
              temp_port->prefix, ipin);
      fprintf(fp, 
              "set_drive 0 %s[%d]\n", 
              temp_port->prefix, ipin);
    }
    /* Go to next */
    temp = temp->next;
  }

  /* close file */
  fclose(fp);

  return;
}
 
static 
void verilog_generate_sdc_break_loop_sb(FILE* fp,
                                        int LL_nx, int LL_ny) {
  int ix, iy;
  t_sb* cur_sb_info = NULL;
  int side, itrack;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Go for each SB */
  for (ix = 0; ix < (LL_nx + 1); ix++) {
    for (iy = 0; iy < (LL_ny + 1); iy++) {
      cur_sb_info = &(sb_info[ix][iy]);
      for (side = 0; side < cur_sb_info->num_sides; side++) {
        for (itrack = 0; itrack < cur_sb_info->chan_width[side]; itrack++) {
          assert((CHANX == cur_sb_info->chan_rr_node[side][itrack]->type)
               ||(CHANY == cur_sb_info->chan_rr_node[side][itrack]->type));
          /* We only care the output port and it should indicate a SB mux */
          if ( (OUT_PORT != cur_sb_info->chan_rr_node_direction[side][itrack]) 
             || (FALSE != check_drive_rr_node_imply_short(*cur_sb_info, cur_sb_info->chan_rr_node[side][itrack], side))) {
            continue; 
          }
          /* Bypass if we have only 1 driving node */
          if (1 == cur_sb_info->chan_rr_node[side][itrack]->num_drive_rr_nodes) {
            continue; 
          }
          /* Disable timing here */
          set_disable_timing_one_sb_output(fp, cur_sb_info, 
                                           cur_sb_info->chan_rr_node[side][itrack]); 
        }
      }
    }
  }
  
  return;
}

static 
void verilog_generate_sdc_break_loop_sb(FILE* fp,
                                        DeviceRRGSB& LL_device_rr_gsb) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Get the range of SB array */
  DeviceCoordinator sb_range = LL_device_rr_gsb.get_gsb_range();
  /* Go for each SB */
  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
      for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
        Side side_manager(side);
        for (size_t itrack = 0; itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
          t_rr_node* chan_rr_node = rr_gsb.get_chan_node(side_manager.get_side(), itrack);
          assert((CHANX == chan_rr_node->type)
               ||(CHANY == chan_rr_node->type));
          /* We only care the output port and it should indicate a SB mux */
          if ( (OUT_PORT != rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) 
             || (false != rr_gsb.is_sb_node_imply_short_connection(chan_rr_node))) {
            continue; 
          }
          /* Bypass if we have only 1 driving node */
          if (1 == chan_rr_node->num_drive_rr_nodes) {
            continue; 
          }
          /* Disable timing here */
          set_disable_timing_one_sb_output(fp, rr_gsb,
                                           chan_rr_node); 
        }
      }
    }
  }
  
  return;
}


static 
void verilog_generate_sdc_break_loops(t_sram_orgz_info* cur_sram_orgz_info,
                                      t_sdc_opts sdc_opts,
                                      int LL_nx, int LL_ny,
                                      int num_switch,
                                      t_switch_inf* switches,
                                      t_spice* spice, DeviceRRGSB& LL_device_rr_gsb, 
                                      t_det_routing_arch* routing_arch) {
  FILE* fp = NULL;
  char* fname = my_strcat(sdc_opts.sdc_dir, sdc_break_loop_file_name);

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for breaking combinational loops in P&R flow: %s ...\n",
             fname);

  /* Create file handler */
  fp = fopen(fname, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
              "(FILE:%s,LINE[%d])Failure in create SDC constraints %s",
              __FILE__, __LINE__, fname); 
    exit(1);
  } 
  /* Generate the descriptions*/
  dump_verilog_sdc_file_header(fp, "Break Combinational Loops for PnR");

  /* 1. Break loops from Memory Cells */
  verilog_generate_sdc_break_loop_sram(fp, cur_sram_orgz_info);

  /* 2. Break loops from Multiplexer Output */
  if (TRUE == sdc_opts.break_loops_mux) {
    verilog_generate_sdc_break_loop_mux(fp, num_switch, switches, spice, routing_arch); 
  }

  /* 3. Break loops from any SB output */
  if (TRUE == sdc_opts.compact_routing_hierarchy) {
    verilog_generate_sdc_break_loop_sb(fp, LL_device_rr_gsb);
  } else {
    verilog_generate_sdc_break_loop_sb(fp, LL_nx, LL_ny);
  }

  /* Close the file*/
  fclose(fp);

  /* Free strings */
  my_free(fname);

  return;
}

/* Constrain a path within a Switch block,
 * If this indicates a metal wire, we constraint to be 0 delay 
 */
static 
void verilog_generate_sdc_constrain_one_sb_path(FILE* fp,
                                                t_sb* cur_sb_info,
                                                t_rr_node* src_rr_node,
                                                t_rr_node* des_rr_node, 
                                                float tmax) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ((OPIN == src_rr_node->type)
        ||(CHANX == src_rr_node->type)
        ||(CHANY == src_rr_node->type));
  assert ((CHANX == des_rr_node->type)
        ||(CHANY == des_rr_node->type));

  fprintf(fp, "set_max_delay");

  fprintf(fp, " -from ");
  fprintf(fp, "%s/", 
          gen_verilog_one_sb_instance_name(cur_sb_info)); 
  dump_verilog_one_sb_routing_pin(fp, cur_sb_info, src_rr_node); 

  fprintf(fp, " -to ");
 
  fprintf(fp, "%s/", 
          gen_verilog_one_sb_instance_name(cur_sb_info)); 
  dump_verilog_one_sb_chan_pin(fp, cur_sb_info, des_rr_node, OUT_PORT); 

  /* If src_node == des_node, this is a metal wire */
  fprintf(fp, " %.2g", tmax);

  fprintf(fp, "\n");
 
  return; 
}

/* Constrain a path within a Switch block,
 * If this indicates a metal wire, we constraint to be 0 delay 
 */
static 
void verilog_generate_sdc_constrain_one_sb_path(FILE* fp,
                                                const RRGSB& rr_gsb,
                                                t_rr_node* src_rr_node,
                                                t_rr_node* des_rr_node, 
                                                float tmax) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ((OPIN == src_rr_node->type)
        ||(CHANX == src_rr_node->type)
        ||(CHANY == src_rr_node->type));
  assert ((CHANX == des_rr_node->type)
        ||(CHANY == des_rr_node->type));

  fprintf(fp, "set_max_delay");

  fprintf(fp, " -from ");
  fprintf(fp, "%s/", 
          rr_gsb.gen_sb_verilog_instance_name()); 
  dump_verilog_one_sb_routing_pin(fp, rr_gsb, src_rr_node); 

  fprintf(fp, " -to ");
 
  fprintf(fp, "%s/", 
          rr_gsb.gen_sb_verilog_instance_name()); 
  dump_verilog_one_sb_chan_pin(fp, rr_gsb, des_rr_node, OUT_PORT); 

  /* If src_node == des_node, this is a metal wire */
  fprintf(fp, " %.2g", tmax);

  fprintf(fp, "\n");
 
  return; 
}


static 
void verilog_generate_sdc_constrain_one_sb_mux(FILE* fp,
                                               t_sb* cur_sb_info,
                                               t_rr_node* wire_rr_node) {
  int iedge, switch_id;
  float switch_delay = 0.;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  assert(  ( CHANX == wire_rr_node->type )
        || ( CHANY == wire_rr_node->type ));

  /* Find the starting points */
  for (iedge = 0; iedge < wire_rr_node->num_drive_rr_nodes; iedge++) {
    /* Get the switch delay */
    switch_id = wire_rr_node->drive_switches[iedge];
    switch_delay = get_switch_sdc_tmax (&(switch_inf[switch_id]));
    /* Constrain a path */
    verilog_generate_sdc_constrain_one_sb_path(fp, cur_sb_info,
                                               wire_rr_node->drive_rr_nodes[iedge],
                                               wire_rr_node,
                                               switch_delay);
  }

  return;
}

static 
void verilog_generate_sdc_constrain_one_sb_mux(FILE* fp,
                                               const RRGSB& rr_gsb,
                                               t_rr_node* wire_rr_node) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  assert(  ( CHANX == wire_rr_node->type )
        || ( CHANY == wire_rr_node->type ));

  /* Find the starting points */
  for (int iedge = 0; iedge < wire_rr_node->num_drive_rr_nodes; iedge++) {
    /* Get the switch delay */
    int switch_id = wire_rr_node->drive_switches[iedge];
    float switch_delay = get_switch_sdc_tmax (&(switch_inf[switch_id]));
    /* Constrain a path */
    verilog_generate_sdc_constrain_one_sb_path(fp, rr_gsb,
                                               wire_rr_node->drive_rr_nodes[iedge],
                                               wire_rr_node,
                                               switch_delay);
  }

  return;
}


/* Constrain a path within a Switch block,
 * If this indicates a metal wire, we constraint to be 0 delay 
 */
static 
void verilog_generate_sdc_constrain_one_cb_path(FILE* fp,
                                                t_cb* cur_cb_info,
                                                t_rr_node* src_rr_node,
                                                t_rr_node* des_rr_node, 
                                                int des_rr_node_grid_side, 
                                                float tmax) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ((INC_DIRECTION == src_rr_node->direction)
        ||(DEC_DIRECTION == src_rr_node->direction));
  assert ((CHANX == src_rr_node->type)
        ||(CHANY == src_rr_node->type));
  assert (IPIN == des_rr_node->type);

  fprintf(fp, "set_max_delay");

  fprintf(fp, " -from ");
  fprintf(fp, "%s/", 
          gen_verilog_one_cb_instance_name(cur_cb_info)); 
  fprintf(fp, "%s",
          gen_verilog_routing_channel_one_midout_name( cur_cb_info,
                                                      src_rr_node->ptc_num));

  fprintf(fp, " -to ");
 
  fprintf(fp, "%s/", 
          gen_verilog_one_cb_instance_name(cur_cb_info)); 

  dump_verilog_grid_side_pin_with_given_index(fp, IPIN, /* This is an output of a connection box */
                                              des_rr_node->ptc_num,
                                              des_rr_node_grid_side,
                                              des_rr_node->xlow,
                                              des_rr_node->ylow,
                                              FALSE); 

  /* If src_node == des_node, this is a metal wire */
  fprintf(fp, " %.2g", tmax);

  fprintf(fp, "\n");
 
  return; 
}

/* Constrain a path within a Switch block,
 * If this indicates a metal wire, we constraint to be 0 delay 
 */
static 
void verilog_generate_sdc_constrain_one_cb_path(FILE* fp,
                                                const RRGSB& rr_gsb, t_rr_type cb_type,
                                                t_rr_node* src_rr_node,
                                                t_rr_node* des_rr_node, 
                                                int des_rr_node_grid_side, 
                                                float tmax) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ((INC_DIRECTION == src_rr_node->direction)
        ||(DEC_DIRECTION == src_rr_node->direction));
  assert ((CHANX == src_rr_node->type)
        ||(CHANY == src_rr_node->type));
  assert (IPIN == des_rr_node->type);

  fprintf(fp, "set_max_delay");

  fprintf(fp, " -from ");
  fprintf(fp, "%s/", 
          rr_gsb.gen_cb_verilog_instance_name(cb_type)); 
  fprintf(fp, "%s",
          rr_gsb.gen_cb_verilog_routing_track_name(cb_type, src_rr_node->ptc_num));

  fprintf(fp, " -to ");
 
  fprintf(fp, "%s/", 
          rr_gsb.gen_cb_verilog_instance_name(cb_type)); 

  dump_verilog_grid_side_pin_with_given_index(fp, IPIN, /* This is an output of a connection box */
                                              des_rr_node->ptc_num,
                                              des_rr_node_grid_side,
                                              des_rr_node->xlow,
                                              des_rr_node->ylow,
                                              FALSE); 

  /* If src_node == des_node, this is a metal wire */
  fprintf(fp, " %.2g", tmax);

  fprintf(fp, "\n");
 
  return; 
}


/* Constrain the inputs and outputs of SBs, with the Switch delays */
static 
void verilog_generate_sdc_constrain_sbs(t_sdc_opts sdc_opts,
                                        DeviceRRGSB& LL_device_rr_gsb) {
  FILE* fp = NULL;
  char* fname = my_strcat(sdc_opts.sdc_dir, sdc_constrain_sb_file_name);

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for constraining Switch Blocks in P&R flow: %s ...\n",
             fname);

  /* Print the muxes netlist*/
  fp = fopen(fname, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
              "(FILE:%s,LINE[%d])Failure in create SDC constraints %s",
              __FILE__, __LINE__, fname); 
    exit(1);
  } 
  /* Generate the descriptions*/
  dump_verilog_sdc_file_header(fp, "Constrain Switch Blocks for PnR");

  /* Get the range of SB array */
  DeviceCoordinator sb_range = LL_device_rr_gsb.get_gsb_range();
  /* Go for each SB */
  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
      for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
        Side side_manager(side);
        for (size_t itrack = 0; itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
          t_rr_node* chan_rr_node = rr_gsb.get_chan_node(side_manager.get_side(), itrack);
          assert((CHANX == chan_rr_node->type)
               ||(CHANY == chan_rr_node->type));
          /* We only care the output port and it should indicate a SB mux */
          if (OUT_PORT != rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) { 
            continue; 
          }
          /* Constrain thru wires */
          if (false != rr_gsb.is_sb_node_imply_short_connection(chan_rr_node)) {
            /* Set the max, min delay to 0? */ 
            verilog_generate_sdc_constrain_one_sb_path(fp, rr_gsb,
                                                       chan_rr_node,
                                                       chan_rr_node,
                                                       0.);
            continue;
          } 
          /* This is a MUX, constrain all the paths from an input to an output */
          verilog_generate_sdc_constrain_one_sb_mux(fp, rr_gsb,
                                                    chan_rr_node);
        }
      }
    }
  }

  /* Close the file*/
  fclose(fp);

  /* Free strings */
  my_free(fname);

  return;
}

/* Constrain the inputs and outputs of SBs, with the Switch delays */
static 
void verilog_generate_sdc_constrain_sbs(t_sdc_opts sdc_opts,
                                        int LL_nx, int LL_ny) {
  FILE* fp = NULL;
  int ix, iy;
  int side, itrack;
  t_sb* cur_sb_info = NULL;
  char* fname = my_strcat(sdc_opts.sdc_dir, sdc_constrain_sb_file_name);

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for constraining Switch Blocks in P&R flow: %s ...\n",
             fname);

  /* Print the muxes netlist*/
  fp = fopen(fname, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
              "(FILE:%s,LINE[%d])Failure in create SDC constraints %s",
              __FILE__, __LINE__, fname); 
    exit(1);
  } 
  /* Generate the descriptions*/
  dump_verilog_sdc_file_header(fp, "Constrain Switch Blocks for PnR");


  /* We start from a SB[x][y] */
  for (ix = 0; ix < (LL_nx + 1); ix++) {
    for (iy = 0; iy < (LL_ny + 1); iy++) {
      cur_sb_info = &(sb_info[ix][iy]);
      for (side = 0; side < cur_sb_info->num_sides; side++) {
        for (itrack = 0; itrack < cur_sb_info->chan_width[side]; itrack++) {
          assert((CHANX == cur_sb_info->chan_rr_node[side][itrack]->type)
               ||(CHANY == cur_sb_info->chan_rr_node[side][itrack]->type));
          /* We only care the output port and it should indicate a SB mux */
          if (OUT_PORT != cur_sb_info->chan_rr_node_direction[side][itrack]) {
            continue; 
          }
          /* Constrain thru wires */
          if (FALSE != check_drive_rr_node_imply_short(*cur_sb_info, cur_sb_info->chan_rr_node[side][itrack], side)) {
            /* Set the max, min delay to 0? */ 
            verilog_generate_sdc_constrain_one_sb_path(fp, cur_sb_info,
                                                       cur_sb_info->chan_rr_node[side][itrack],
                                                       cur_sb_info->chan_rr_node[side][itrack],
                                                       0.);
            continue;
          } 
          /* This is a MUX, constrain all the paths from an input to an output */
          verilog_generate_sdc_constrain_one_sb_mux(fp, cur_sb_info,
                                                    cur_sb_info->chan_rr_node[side][itrack]);
        }
      }
    }
  }

  /* Close the file*/
  fclose(fp);

  /* Free strings */
  my_free(fname);

  return;
}

static 
void verilog_generate_sdc_constrain_one_cb(FILE* fp,
                                           t_cb* cur_cb_info) {
  int side, side_cnt;
  int inode, iedge, switch_id;
  float switch_delay = 0.;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  side_cnt = 0;
  /* Print the ports of grids*/
  /* only check ipin_rr_nodes of cur_cb_info */
  for (side = 0; side < cur_cb_info->num_sides; side++) {
    /* Bypass side with zero IPINs*/
    if (0 == cur_cb_info->num_ipin_rr_nodes[side]) {
      continue;
    }
    side_cnt++;
    assert(0 < cur_cb_info->num_ipin_rr_nodes[side]);
    assert(NULL != cur_cb_info->ipin_rr_node[side]);
    for (inode = 0; inode < cur_cb_info->num_ipin_rr_nodes[side]; inode++) {
      for (iedge = 0; iedge < cur_cb_info->ipin_rr_node[side][inode]->num_drive_rr_nodes; iedge++) {
        /* Get the switch delay */
        switch_id = cur_cb_info->ipin_rr_node[side][inode]->drive_switches[iedge];
        switch_delay = get_switch_sdc_tmax (&(switch_inf[switch_id]));

        /* Print each INPUT Pins of a grid */
        verilog_generate_sdc_constrain_one_cb_path(fp, cur_cb_info,
                                                   cur_cb_info->ipin_rr_node[side][inode]->drive_rr_nodes[iedge],
                                                   cur_cb_info->ipin_rr_node[side][inode],
                                                   cur_cb_info->ipin_rr_node_grid_side[side][inode],
                                                   switch_delay); 
      }
    }
  }
  /* Make sure only 2 sides of IPINs are printed */
  assert((1 == side_cnt)||(2 == side_cnt));

  return;
}

static 
void verilog_generate_sdc_constrain_one_cb(FILE* fp,
                                           const RRGSB& rr_gsb, t_rr_type cb_type) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Print the ports of grids*/
  /* only check ipin_rr_nodes of cur_cb_info */
  std::vector<enum e_side> cb_sides = rr_gsb.get_cb_ipin_sides(cb_type);

  for (size_t side = 0; side < cb_sides.size(); ++side) {
    enum e_side cb_ipin_side = cb_sides[side];
    Side side_manager(cb_ipin_side);
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side); ++inode) {
      t_rr_node* cur_ipin_node = rr_gsb.get_ipin_node(cb_ipin_side, inode);
      for (int iedge = 0; iedge < cur_ipin_node->num_drive_rr_nodes; iedge++) {
        /* Get the switch delay */
        int switch_id = cur_ipin_node->drive_switches[iedge];
        float switch_delay = get_switch_sdc_tmax (&(switch_inf[switch_id]));

        /* Print each INPUT Pins of a grid */
        verilog_generate_sdc_constrain_one_cb_path(fp, rr_gsb, cb_type,
                                                   cur_ipin_node->drive_rr_nodes[iedge],
                                                   cur_ipin_node,
                                                   rr_gsb.get_ipin_node_grid_side(cb_ipin_side, inode),
                                                   switch_delay); 
      }
    }
  }

  return;
}


/* Constrain the inputs and outputs of Connection Blocks, with the Switch delays */
static 
void verilog_generate_sdc_constrain_cbs(t_sdc_opts sdc_opts,
                                        int LL_nx, int LL_ny) {
  FILE* fp = NULL;
  int ix, iy;
  char* fname = my_strcat(sdc_opts.sdc_dir, sdc_constrain_cb_file_name);

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for constraining Connection Blocks in P&R flow: %s ...\n",
             fname);

  /* Print the muxes netlist*/
  fp = fopen(fname, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
              "(FILE:%s,LINE[%d])Failure in create SDC constraints %s",
              __FILE__, __LINE__, fname); 
    exit(1);
  } 
  /* Generate the descriptions*/
  dump_verilog_sdc_file_header(fp, "Constrain Connection Blocks for PnR");

  /* Connection Boxes */
  /* X - channels [1...nx][0..ny]*/
  for (iy = 0; iy < (LL_ny + 1); iy++) {
    for (ix = 1; ix < (LL_nx + 1); ix++) {
      if ((TRUE == is_cb_exist(CHANX, ix, iy))
         &&(0 < count_cb_info_num_ipin_rr_nodes(cbx_info[ix][iy]))) {
        verilog_generate_sdc_constrain_one_cb(fp, &(cbx_info[ix][iy]));
      }
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  for (ix = 0; ix < (LL_nx + 1); ix++) {
    for (iy = 1; iy < (LL_ny + 1); iy++) {
      if ((TRUE == is_cb_exist(CHANY, ix, iy)) 
         &&(0 < count_cb_info_num_ipin_rr_nodes(cby_info[ix][iy]))) {
        verilog_generate_sdc_constrain_one_cb(fp, &(cby_info[ix][iy]));
      }
    }
  }

  /* Close the file*/
  fclose(fp);

  /* Free strings */
  my_free(fname);

  return;
}

/* Constrain the inputs and outputs of Connection Blocks, with the Switch delays */
static 
void verilog_generate_sdc_constrain_cbs(t_sdc_opts sdc_opts, int LL_nx, int LL_ny,
                                        DeviceRRGSB& LL_device_rr_gsb) {
  FILE* fp = NULL;
  char* fname = my_strcat(sdc_opts.sdc_dir, sdc_constrain_cb_file_name);

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for constraining Connection Blocks in P&R flow: %s ...\n",
             fname);

  /* Print the muxes netlist*/
  fp = fopen(fname, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
              "(FILE:%s,LINE[%d])Failure in create SDC constraints %s",
              __FILE__, __LINE__, fname); 
    exit(1);
  } 
  /* Generate the descriptions*/
  dump_verilog_sdc_file_header(fp, "Constrain Connection Blocks for PnR");

  /* Connection Boxes */
  /* X - channels [1...nx][0..ny]*/
  for (int iy = 0; iy < (LL_ny + 1); ++iy) {
    for (int ix = 1; ix < (LL_nx + 1); ++ix) {
      const RRGSB& rr_gsb = LL_device_rr_gsb.get_gsb(ix, iy);
      if ( (TRUE == is_cb_exist(CHANX, ix, iy))
         &&(true == rr_gsb.is_cb_exist(CHANX))) {
        verilog_generate_sdc_constrain_one_cb(fp, rr_gsb, CHANX);
      }
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  for (int ix = 0; ix < (LL_nx + 1); ++ix) {
    for (int iy = 1; iy < (LL_ny + 1); ++iy) {
      const RRGSB& rr_gsb = LL_device_rr_gsb.get_gsb(ix, iy);
      if ((TRUE == is_cb_exist(CHANY, ix, iy)) 
         &&(true == rr_gsb.is_cb_exist(CHANY))) {
        verilog_generate_sdc_constrain_one_cb(fp, rr_gsb, CHANY);
      }
    }
  }

  /* Close the file*/
  fclose(fp);

  /* Free strings */
  my_free(fname);

  return;
}

static 
void verilog_generate_sdc_constrain_one_chan(FILE* fp, 
                                             t_rr_type chan_type,
                                             int x, int y,
                                             t_arch arch,
                                             int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                             t_ivec*** LL_rr_node_indices,
                                             t_rr_indexed_data* LL_rr_indexed_data) {
  int chan_width = 0;
  t_rr_node** chan_rr_nodes = NULL;
  int cost_index, iseg, itrack;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Collect rr_nodes for Tracks for chanx[ix][iy] */
  chan_rr_nodes = get_chan_rr_nodes(&chan_width, chan_type, x, y,
                                    LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);

  for (itrack = 0; itrack < chan_width; itrack++) {
    fprintf(fp, "set_max_delay");
    fprintf(fp, " -from ");

    fprintf(fp, "%s/in%d", 
            gen_verilog_one_routing_channel_instance_name(chan_type, x, y),
            itrack);

    fprintf(fp, " -to ");

    fprintf(fp, "%s/out%d", 
            gen_verilog_one_routing_channel_instance_name(chan_type, x, y), 
            itrack);
    /* Find the segment delay ! */
    cost_index = chan_rr_nodes[itrack]->cost_index;
    iseg = LL_rr_indexed_data[cost_index].seg_index; 
    /* Check */
    assert((!(iseg < 0))&&(iseg < arch.num_segments));
    fprintf(fp, " %.2g", get_routing_seg_sdc_tmax(&(arch.Segments[iseg])));
    fprintf(fp, "\n");

    fprintf(fp, "set_max_delay");
    fprintf(fp, " -from ");

    fprintf(fp, "%s/in%d", 
            gen_verilog_one_routing_channel_instance_name(chan_type, x, y),
            itrack);

    fprintf(fp, " -to ");

    fprintf(fp, "%s/mid_out%d", 
            gen_verilog_one_routing_channel_instance_name(chan_type, x, y), 
            itrack);
    /* Check */
    fprintf(fp, " %.2g", get_routing_seg_sdc_tmax(&(arch.Segments[iseg])));
    fprintf(fp, "\n");

  }

  /* Free */
  my_free(chan_rr_nodes);

  return;
}

/** Disable timing analysis for a unused routing channel
 */
static 
void verilog_generate_sdc_disable_one_unused_chan(FILE* fp, 
                                                  int x, int y,
                                                  const RRChan& rr_chan) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Print comments */
  fprintf(fp,
          "##################################################\n"); 
  fprintf(fp, 
          "### Disable Timing for an %s[%d][%d] ###\n",
          convert_chan_type_to_string(rr_chan.get_type()),
          x, y);
  fprintf(fp,
          "##################################################\n"); 

  /* Collect rr_nodes for Tracks for chanx[ix][iy] */
  for (size_t itrack = 0; itrack < rr_chan.get_chan_width(); ++itrack) {
    /* We disable the timing of the input and output of a routing track,
     * when it is not mapped to a net or it is a parasitic net
     */
    if (FALSE == is_rr_node_to_be_disable_for_analysis(rr_chan.get_node(itrack))) {
      continue;
    }
    fprintf(fp, "set_disable_timing ");
    fprintf(fp, "%s/in%lu", 
            gen_verilog_one_routing_channel_instance_name(rr_chan.get_type(), x, y),
            itrack);
    fprintf(fp, "\n");

    fprintf(fp, "set_disable_timing ");
    fprintf(fp, "%s/out%lu", 
            gen_verilog_one_routing_channel_instance_name(rr_chan.get_type(), x, y), 
            itrack);
    fprintf(fp, "\n");

    fprintf(fp, "set_disable_timing ");
    fprintf(fp, "%s/mid_out%lu", 
            gen_verilog_one_routing_channel_instance_name(rr_chan.get_type(), x, y), 
            itrack);
    fprintf(fp, "\n");
  }

  /* Free */

  return;
}


static 
void verilog_generate_sdc_disable_one_unused_chan(FILE* fp, 
                                                  t_rr_type chan_type,
                                                  int x, int y,
                                                  int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                                  t_ivec*** LL_rr_node_indices) {
  int chan_width = 0;
  t_rr_node** chan_rr_nodes = NULL;
  int itrack;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Print comments */
  fprintf(fp,
          "##################################################\n"); 
  fprintf(fp, 
          "### Disable Timing for an %s[%d][%d] ###\n",
          convert_chan_type_to_string(chan_type),
          x, y);
  fprintf(fp,
          "##################################################\n"); 

  /* Collect rr_nodes for Tracks for chanx[ix][iy] */
  chan_rr_nodes = get_chan_rr_nodes(&chan_width, chan_type, x, y,
                                    LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);

  for (itrack = 0; itrack < chan_width; itrack++) {
    /* We disable the timing of the input and output of a routing track,
     * when it is not mapped to a net or it is a parasitic net
     */
    if (FALSE == is_rr_node_to_be_disable_for_analysis(chan_rr_nodes[itrack])) {
      continue;
    }
    fprintf(fp, "set_disable_timing ");
    fprintf(fp, "%s/in%d", 
            gen_verilog_one_routing_channel_instance_name(chan_type, x, y),
            itrack);
    fprintf(fp, "\n");

    fprintf(fp, "set_disable_timing ");
    fprintf(fp, "%s/out%d", 
            gen_verilog_one_routing_channel_instance_name(chan_type, x, y), 
            itrack);
    fprintf(fp, "\n");

    fprintf(fp, "set_disable_timing ");
    fprintf(fp, "%s/mid_out%d", 
            gen_verilog_one_routing_channel_instance_name(chan_type, x, y), 
            itrack);
    fprintf(fp, "\n");
  }

  /* Free */
  my_free(chan_rr_nodes);

  return;
}


/* Constrain the inputs and outputs of Connection Blocks, with the Switch delays */
static 
void verilog_generate_sdc_constrain_routing_channels(t_sdc_opts sdc_opts,
                                                     t_arch arch,
                                                     int LL_nx, int LL_ny,
                                                     int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                                     t_ivec*** LL_rr_node_indices,
                                                     t_rr_indexed_data* LL_rr_indexed_data) {
  FILE* fp = NULL;
  int ix, iy;
  char* fname = my_strcat(sdc_opts.sdc_dir, sdc_constrain_routing_chan_file_name);

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for constraining Routing Channels in P&R flow: %s ...\n",
             fname);

  /* Print the muxes netlist*/
  fp = fopen(fname, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
              "(FILE:%s,LINE[%d])Failure in create SDC constraints %s",
              __FILE__, __LINE__, fname); 
    exit(1);
  } 
  /* Generate the descriptions*/
  dump_verilog_sdc_file_header(fp, "Constrain Routing Channels for PnR");

  /* Routing channels */
  /* X - channels [1...nx][0..ny]*/
  for (iy = 0; iy < (LL_ny + 1); iy++) {
    for (ix = 1; ix < (LL_nx + 1); ix++) {
      verilog_generate_sdc_constrain_one_chan(fp, CHANX, ix, iy, arch,
                                              LL_num_rr_nodes, LL_rr_node, 
                                              LL_rr_node_indices, LL_rr_indexed_data);
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  for (ix = 0; ix < (LL_nx + 1); ix++) {
    for (iy = 1; iy < (LL_ny + 1); iy++) {
      verilog_generate_sdc_constrain_one_chan(fp, CHANY, ix, iy, arch,
                                              LL_num_rr_nodes, LL_rr_node, 
                                              LL_rr_node_indices, LL_rr_indexed_data);
    }
  }

  /* Close the file*/
  fclose(fp);

  /* Free strings */
  my_free(fname);

  return;
}

/* Disable the timing for all the global port
 * Except the clock ports
 */
static 
void verilog_generate_sdc_disable_global_ports(FILE* fp) {
  t_llist* temp = global_ports_head;
  t_spice_model_port* cur_port = NULL;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Print comments */
  fprintf(fp,
          "##################################################\n"); 
  fprintf(fp, 
          "### Disable Timing for global ports ###\n");
  fprintf(fp,
          "##################################################\n"); 

  while (NULL != temp) {
    /* Get the port */
    cur_port = (t_spice_model_port*)(temp->dptr);
    /* Only focus on the non-clock ports */
    if ( (SPICE_MODEL_PORT_CLOCK == cur_port->type) 
      && (FALSE == cur_port->is_prog) ) {
      /* Go to the next */
      temp = temp->next;
      continue;
    }
    /* Output disable timing command */
    fprintf(fp, 
            "set_disable_timing %s\n",
            cur_port->prefix);
    /* Go to the next */
    temp = temp->next;
  }

  return;
}

/* Disable the timing for SRAM outputs */
static 
void verilog_generate_sdc_disable_sram_orgz(FILE* fp, 
                                            t_sram_orgz_info* cur_sram_orgz_info) {
  
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Print comments */
  fprintf(fp,
          "##################################################\n"); 
  fprintf(fp, 
          "### Disable Timing for configuration memories ###\n");
  fprintf(fp,
          "##################################################\n"); 

  verilog_generate_sdc_break_loop_sram(fp, cur_sram_orgz_info); 

  return;
}

static 
void verilog_generate_sdc_disable_unused_sbs_muxs(FILE* fp) {

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  DeviceCoordinator sb_range = device_rr_gsb.get_gsb_range();

  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      const RRGSB& rr_sb = device_rr_gsb.get_gsb(ix, iy);
      /* Print comments */
      fprintf(fp,
              "########################################################\n"); 
      fprintf(fp, 
              "### Disable Timing for MUXES in Switch block[%lu][%lu] ###\n",
              ix, iy);
      fprintf(fp,
              "########################################################\n"); 
      for (size_t side = 0; side < rr_sb.get_num_sides(); ++side) {
        Side side_manager(side);
        for (size_t itrack = 0; itrack < rr_sb.get_chan_width(side_manager.get_side()); ++itrack) {
          /* bypass non-mux pins */
          if (OUT_PORT != rr_sb.get_chan_node_direction(side_manager.get_side(), itrack)) {
            continue;
          }
          t_rr_node* cur_rr_node = rr_sb.get_chan_node(side_manager.get_side(), itrack);
          for (int imux = 0 ; imux < cur_rr_node->fan_in; ++imux) {
            if (1 == cur_rr_node->fan_in) {
              continue;
            }
            if (imux == cur_rr_node->id_path) {
              fprintf(fp, "#"); // comments out if the node is active
            }
            //if(cur_rr_node->name_mux == NULL) assert (NULL != cur_rr_node->name_mux);
            fprintf(fp, "set_disable_timing  %s[%d]\n", 
                    cur_rr_node->name_mux, imux);
          }
        }
      } 
    }
  }

  return;
}

static 
void verilog_generate_sdc_disable_unused_sbs_muxs(FILE* fp, int LL_nx, int LL_ny) {

  int ix, iy, side, itrack, imux;
  t_rr_node* cur_rr_node;
  t_sb* cur_sb_info;

  for (ix = 0; ix < (LL_nx + 1); ix++) {
    for (iy = 0; iy < (LL_ny + 1); iy++) {
      cur_sb_info = &(sb_info[ix][iy]);
      /* Print comments */
      fprintf(fp,
              "########################################################\n"); 
      fprintf(fp, 
              "### Disable Timing for MUXES in Switch block[%d][%d] ###\n",
              ix, iy);
      fprintf(fp,
              "########################################################\n"); 
      for (side = 0; side < cur_sb_info->num_sides; side++) {
        for (itrack = 0; itrack < cur_sb_info->chan_width[side]; itrack++) {
          if (OUT_PORT == cur_sb_info->chan_rr_node_direction[side][itrack]) {
            cur_rr_node = cur_sb_info->chan_rr_node[side][itrack];
            for (imux = 0 ; imux < cur_rr_node-> fan_in; imux++) {
              if (1 == cur_rr_node->fan_in) {
                continue;
              }
              if (imux == cur_rr_node->id_path) {
                fprintf(fp, "#"); // comments out if the node is active
              }
              //if(cur_rr_node->name_mux == NULL) assert (NULL != cur_rr_node->name_mux);
              fprintf(fp, "set_disable_timing  %s[%d]\n", 
                      cur_rr_node->name_mux, imux);
            }
          }
        }
      } 
    }
  }

  return;
}

static 
void verilog_generate_sdc_disable_one_unused_cb_mux(FILE* fp, RRGSB& rr_gsb, t_rr_type cb_type) {
  std::vector<enum e_side> cb_sides = rr_gsb.get_cb_ipin_sides(cb_type);

  for (size_t side = 0; side < cb_sides.size(); ++side) {
    enum e_side cb_ipin_side = cb_sides[side];
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side); ++inode) {
      t_rr_node* ipin_node = rr_gsb.get_ipin_node(cb_ipin_side, inode);
      for (int imux = 0 ; imux < ipin_node->fan_in; ++imux) {
        if (imux == ipin_node->id_path) {
          fprintf(fp, "#"); // comments out if the node is active
        }
        fprintf(fp, "set_disable_timing %s[%d]\n", 
                ipin_node->name_mux, imux);
      }
    }
  }
  return;
}

static 
void verilog_generate_sdc_disable_unused_cbs_muxs(FILE* fp, int LL_nx, int LL_ny, DeviceRRGSB& LL_device_rr_gsb) {

  for (int iy = 0; iy < (LL_ny + 1); ++iy) {
    for (int ix = 1; ix < (LL_nx + 1); ++ix) {
      RRGSB rr_gsb = LL_device_rr_gsb.get_gsb(ix, iy);
      if ((TRUE == is_cb_exist(CHANX, ix, iy))
        &&(true == rr_gsb.is_cb_exist(CHANX))) {
        /* Print comments */
        fprintf(fp,
              "##############################################################\n"); 
        fprintf(fp, 
              "### Disable Timing for MUXES in Connection block X[%d][%d] ###\n",
              ix, iy);
        fprintf(fp,
              "##############################################################\n"); 
      
        verilog_generate_sdc_disable_one_unused_cb_mux(fp, rr_gsb, CHANX);
      }
    }
  }

  for (int iy = 1; iy < (LL_ny + 1); iy++) {
    for (int ix = 0; ix < (LL_nx + 1); ix++) {
      RRGSB rr_gsb = LL_device_rr_gsb.get_gsb(ix, iy);
      if ((TRUE == is_cb_exist(CHANY, ix, iy))
        &&(true == rr_gsb.is_cb_exist(CHANY))) {
        /* Print comments */
        fprintf(fp,
              "##############################################################\n"); 
        fprintf(fp, 
              "### Disable Timing for MUXES in Connection block Y[%d][%d] ###\n",
              ix, iy);
        fprintf(fp,
              "##############################################################\n"); 
        verilog_generate_sdc_disable_one_unused_cb_mux(fp, rr_gsb, CHANY);
      }
    }
  }

  return;
}

static 
void verilog_generate_sdc_disable_unused_cbs_muxs(FILE* fp,
                                                  int LL_nx, int LL_ny) {

  int ix, iy, iside, inode, imux;
  t_cb* cur_cb_info;
  t_rr_node* cur_rr_node;

  for (iy = 0; iy < (LL_ny + 1); iy++) {
    for (ix = 1; ix < (LL_nx + 1); ix++) {
      if (0 < count_cb_info_num_ipin_rr_nodes(cbx_info[ix][iy])) {
        cur_cb_info = &(cbx_info[ix][iy]);
        /* Print comments */
        fprintf(fp,
              "##############################################################\n"); 
        fprintf(fp, 
              "### Disable Timing for MUXES in Connection block X[%d][%d] ###\n",
              ix, iy);
        fprintf(fp,
              "##############################################################\n"); 
      
        for (iside = 0; iside < cur_cb_info->num_sides; iside++) {
          for (inode = 0; inode < cur_cb_info->num_ipin_rr_nodes[iside]; inode++) {
             cur_rr_node = cur_cb_info->ipin_rr_node[iside][inode];
            for (imux = 0 ; imux < cur_rr_node-> fan_in; imux++) {
              if (imux == cur_rr_node->id_path) {
                fprintf(fp, "#"); // comments out if the node is active
              }
			  fprintf(fp, "set_disable_timing %s[%d]\n", 
                      cur_rr_node->name_mux, imux);
            }
          }
        }
      }
    }
  }
  for (iy = 1; iy < (LL_ny + 1); iy++) {
    for (ix = 0; ix < (LL_nx + 1); ix++) {
      if (0 < count_cb_info_num_ipin_rr_nodes(cby_info[ix][iy])) {
        cur_cb_info = &(cby_info[ix][iy]);
        /* Print comments */
        fprintf(fp,
              "##############################################################\n"); 
        fprintf(fp, 
              "### Disable Timing for MUXES in Connection block Y[%d][%d] ###\n",
              ix, iy);
        fprintf(fp,
              "##############################################################\n"); 
        for (iside = 0; iside < cur_cb_info->num_sides; iside++) {
          for (inode = 0; inode < cur_cb_info->num_ipin_rr_nodes[iside]; inode++) {
             cur_rr_node = cur_cb_info->ipin_rr_node[iside][inode];
            for (imux = 0 ; imux < cur_rr_node-> fan_in; imux++) {
              if (imux == cur_rr_node->id_path) {
                fprintf(fp, "#"); // comments out if the node is active
              }
              fprintf(fp, "set_disable_timing %s[%d]\n", 
                      cur_rr_node->name_mux, imux);
            }
          }
        }
      }
    }
  }

  return;
}

static 
void verilog_generate_sdc_disable_unused_sbs(FILE* fp) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  DeviceCoordinator sb_range = device_rr_gsb.get_gsb_range();
  /* We start from a SB[x][y] */
  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      RRGSB rr_sb = device_rr_gsb.get_gsb(ix, iy);
      /* Print comments */
      fprintf(fp,
              "##################################################\n"); 
      fprintf(fp, 
              "### Disable Timing for an Switch block[%lu][%lu] ###\n",
              ix, iy);
      fprintf(fp,
              "##################################################\n"); 
      for (size_t side = 0; side < rr_sb.get_num_sides(); ++side) {
        Side side_manager(side);
        /* Disable Channel inputs and outputs*/
        for (size_t itrack = 0; itrack < rr_sb.get_chan_width(side_manager.get_side()); ++itrack) {
          assert((CHANX == rr_sb.get_chan_node(side_manager.get_side(), itrack)->type)
               ||(CHANY == rr_sb.get_chan_node(side_manager.get_side(), itrack)->type));
          if (FALSE == is_rr_node_to_be_disable_for_analysis(rr_sb.get_chan_node(side_manager.get_side(), itrack))) {
            continue;
          }
          fprintf(fp, "set_disable_timing ");
          fprintf(fp, "%s/", 
                  rr_sb.gen_sb_verilog_instance_name());
          dump_verilog_one_sb_chan_pin(fp, rr_sb, 
                                       rr_sb.get_chan_node(side_manager.get_side(), itrack), 
                                       rr_sb.get_chan_node_direction(side_manager.get_side(), itrack)); 
          fprintf(fp, "\n");
        }
        /* Disable OPINs*/
        for (size_t inode = 0; inode < rr_sb.get_num_opin_nodes(side_manager.get_side()); ++inode) {
          assert (OPIN == rr_sb.get_opin_node(side_manager.get_side(), inode)->type);
          if (FALSE == is_rr_node_to_be_disable_for_analysis(rr_sb.get_opin_node(side_manager.get_side(), inode))) {
            continue;
          }
          fprintf(fp, "set_disable_timing ");
          fprintf(fp, "%s/", 
                  rr_sb.gen_sb_verilog_instance_name());
          dump_verilog_one_sb_routing_pin(fp, rr_sb,
                                          rr_sb.get_opin_node(side_manager.get_side(), inode)); 
          fprintf(fp, "\n");
        }
      }
    }
  }

  return;
}

static 
void verilog_generate_sdc_disable_unused_sbs(FILE* fp,
                                             int LL_nx, int LL_ny) {
  int ix, iy;
  int side, itrack, inode;
  t_sb* cur_sb_info = NULL;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* We start from a SB[x][y] */
  for (ix = 0; ix < (LL_nx + 1); ix++) {
    for (iy = 0; iy < (LL_ny + 1); iy++) {
      cur_sb_info = &(sb_info[ix][iy]);
      /* Print comments */
      fprintf(fp,
              "##################################################\n"); 
      fprintf(fp, 
              "### Disable Timing for an Switch block[%d][%d] ###\n",
              ix, iy);
      fprintf(fp,
              "##################################################\n"); 
      for (side = 0; side < cur_sb_info->num_sides; side++) {
        /* Disable Channel inputs and outputs*/
        for (itrack = 0; itrack < cur_sb_info->chan_width[side]; itrack++) {
          assert((CHANX == cur_sb_info->chan_rr_node[side][itrack]->type)
               ||(CHANY == cur_sb_info->chan_rr_node[side][itrack]->type));
          if (FALSE == is_rr_node_to_be_disable_for_analysis(cur_sb_info->chan_rr_node[side][itrack])) {
            continue;
          }
          if (0 == cur_sb_info->chan_rr_node[side][itrack]->fan_in) {
            continue;
          }
          fprintf(fp, "set_disable_timing ");
          fprintf(fp, "%s/", 
                  gen_verilog_one_sb_instance_name(cur_sb_info));
          dump_verilog_one_sb_chan_pin(fp, cur_sb_info, 
                                       cur_sb_info->chan_rr_node[side][itrack], 
                                       cur_sb_info->chan_rr_node_direction[side][itrack]); 
          fprintf(fp, "\n");
        }
        /* Disable OPINs*/
        for (inode = 0; inode < cur_sb_info->num_opin_rr_nodes[side]; inode++) {
          assert (OPIN == cur_sb_info->opin_rr_node[side][inode]->type);
          if (FALSE == is_rr_node_to_be_disable_for_analysis(cur_sb_info->opin_rr_node[side][inode])) {
            continue;
          }
          if (0 == cur_sb_info->opin_rr_node[side][inode]->fan_in) {
            continue;
          }
          fprintf(fp, "set_disable_timing ");
          fprintf(fp, "%s/", 
                  gen_verilog_one_sb_instance_name(cur_sb_info));
          dump_verilog_one_sb_routing_pin(fp, cur_sb_info,
                                          cur_sb_info->opin_rr_node[side][inode]); 
          fprintf(fp, "\n");
        }
      }
    }
  }

  return;
}

static 
void verilog_generate_sdc_disable_one_unused_cb(FILE* fp, 
                                                RRGSB& rr_gsb, t_rr_type cb_type) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Print comments */
  fprintf(fp,
          "##################################################\n"); 
  fprintf(fp, 
          "### Disable Timing for an unused %s ###\n",
          rr_gsb.gen_cb_verilog_module_name(cb_type));
  fprintf(fp,
          "##################################################\n"); 

  std::vector<enum e_side> cb_sides = rr_gsb.get_cb_ipin_sides(cb_type);

  for (size_t side = 0; side < cb_sides.size(); ++side) {
    enum e_side cb_ipin_side = cb_sides[side];
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side); ++inode) {
      t_rr_node* ipin_node = rr_gsb.get_ipin_node(cb_ipin_side, inode);
      if (FALSE == is_rr_node_to_be_disable_for_analysis(ipin_node)) {
        continue;
      }
      if (0 == ipin_node->fan_in) {
        continue;
      }
      fprintf(fp, "set_disable_timing ");
      fprintf(fp, "%s/", 
              rr_gsb.gen_cb_verilog_instance_name(cb_type));
      dump_verilog_grid_side_pin_with_given_index(fp, IPIN,
                                                  ipin_node->ptc_num,
                                                  rr_gsb.get_ipin_node_grid_side(cb_ipin_side, inode),
                                                  ipin_node->xlow,
                                                  ipin_node->ylow,
                                                  FALSE); /* Do not specify direction of port */
      fprintf(fp, "\n");
    }
  }

  return;
}

static 
void verilog_generate_sdc_disable_one_unused_cb(FILE* fp, 
                                                t_cb* cur_cb_info) {
  int side, side_cnt;
  int inode;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Print comments */
  fprintf(fp,
          "##################################################\n"); 
  fprintf(fp, 
          "### Disable Timing for an unused %s[%d][%d] ###\n",
          convert_cb_type_to_string(cur_cb_info->type),
          cur_cb_info->x, cur_cb_info->y);
  fprintf(fp,
          "##################################################\n"); 

  side_cnt = 0;
  /* Print the ports of grids*/
  /* only check ipin_rr_nodes of cur_cb_info */
  for (side = 0; side < cur_cb_info->num_sides; side++) {
    /* Bypass side with zero IPINs*/
    if (0 == cur_cb_info->num_ipin_rr_nodes[side]) {
      continue;
    }
    side_cnt++;
    assert(0 < cur_cb_info->num_ipin_rr_nodes[side]);
    assert(NULL != cur_cb_info->ipin_rr_node[side]);
    for (inode = 0; inode < cur_cb_info->num_ipin_rr_nodes[side]; inode++) {
      if (FALSE == is_rr_node_to_be_disable_for_analysis(cur_cb_info->ipin_rr_node[side][inode])) {
        continue;
      }
      if (0 == cur_cb_info->ipin_rr_node[side][inode]->fan_in) {
        continue;
      }
      fprintf(fp, "set_disable_timing ");
      fprintf(fp, "%s/", 
              gen_verilog_one_cb_instance_name(cur_cb_info));
      dump_verilog_grid_side_pin_with_given_index(fp, IPIN,
                                                  cur_cb_info->ipin_rr_node[side][inode]->ptc_num,
                                                  cur_cb_info->ipin_rr_node_grid_side[side][inode],
                                                  cur_cb_info->ipin_rr_node[side][inode]->xlow,
                                                  cur_cb_info->ipin_rr_node[side][inode]->ylow,
                                                  FALSE); /* Do not specify direction of port */
      fprintf(fp, "\n");
    }
  }

  return;
}

static 
void verilog_generate_sdc_disable_unused_cbs(FILE* fp,
                                             int LL_nx, int LL_ny,
                                             DeviceRRGSB& LL_device_rr_gsb) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Connection Boxes */
  /* X - channels [1...nx][0..ny]*/
  for (int iy = 0; iy < (LL_ny + 1); iy++) {
    for (int ix = 1; ix < (LL_nx + 1); ix++) {
      RRGSB rr_gsb = LL_device_rr_gsb.get_gsb(ix, iy);
      if ((TRUE == is_cb_exist(CHANX, ix, iy))
        &&(true == rr_gsb.is_cb_exist(CHANX))) {
        verilog_generate_sdc_disable_one_unused_cb(fp, rr_gsb, CHANX);
      }
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  for (int ix = 0; ix < (LL_nx + 1); ix++) {
    for (int iy = 1; iy < (LL_ny + 1); iy++) {
      RRGSB rr_gsb = LL_device_rr_gsb.get_gsb(ix, iy);
      if ((TRUE == is_cb_exist(CHANY, ix, iy))
        &&(true == rr_gsb.is_cb_exist(CHANY))) {
        verilog_generate_sdc_disable_one_unused_cb(fp, rr_gsb, CHANY);
      }
    }
  }

  return;
}

static 
void verilog_generate_sdc_disable_unused_cbs(FILE* fp,
                                             int LL_nx, int LL_ny) {
  int ix, iy;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Connection Boxes */
  /* X - channels [1...nx][0..ny]*/
  for (iy = 0; iy < (LL_ny + 1); iy++) {
    for (ix = 1; ix < (LL_nx + 1); ix++) {
      if ((TRUE == is_cb_exist(CHANX, ix, iy))
         &&(0 < count_cb_info_num_ipin_rr_nodes(cbx_info[ix][iy]))) {
        verilog_generate_sdc_disable_one_unused_cb(fp, &(cbx_info[ix][iy]));
      }
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  for (ix = 0; ix < (LL_nx + 1); ix++) {
    for (iy = 1; iy < (LL_ny + 1); iy++) {
      if ((TRUE == is_cb_exist(CHANY, ix, iy)) 
         &&(0 < count_cb_info_num_ipin_rr_nodes(cby_info[ix][iy]))) {
        verilog_generate_sdc_disable_one_unused_cb(fp, &(cby_info[ix][iy]));
      }
    }
  }

  return;
}

/* Constrain the inputs and outputs of Connection Blocks, with the Switch delays */
static 
void verilog_generate_sdc_disable_unused_routing_channels(FILE* fp, 
                                                          int LL_nx, int LL_ny) {
  int ix, iy;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Routing channels */
  /* X - channels [1...nx][0..ny]*/
  for (iy = 0; iy < (LL_ny + 1); iy++) {
    for (ix = 1; ix < (LL_nx + 1); ix++) {
      verilog_generate_sdc_disable_one_unused_chan(fp, ix, iy,
                                                   device_rr_chan.get_module_with_coordinator(CHANX, ix, iy));
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  for (ix = 0; ix < (LL_nx + 1); ix++) {
    for (iy = 1; iy < (LL_ny + 1); iy++) {
      verilog_generate_sdc_disable_one_unused_chan(fp, ix, iy, 
                                                   device_rr_chan.get_module_with_coordinator(CHANY, ix, iy));
    }
  }

  return;
}


/* Constrain the inputs and outputs of Connection Blocks, with the Switch delays */
static 
void verilog_generate_sdc_disable_unused_routing_channels(FILE* fp, 
                                                          int LL_nx, int LL_ny, 
                                                          int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                                          t_ivec*** LL_rr_node_indices) {
  int ix, iy;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Routing channels */
  /* X - channels [1...nx][0..ny]*/
  for (iy = 0; iy < (LL_ny + 1); iy++) {
    for (ix = 1; ix < (LL_nx + 1); ix++) {
      verilog_generate_sdc_disable_one_unused_chan(fp, CHANX, ix, iy,
                                                   LL_num_rr_nodes, LL_rr_node, 
                                                   LL_rr_node_indices);
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  for (ix = 0; ix < (LL_nx + 1); ix++) {
    for (iy = 1; iy < (LL_ny + 1); iy++) {
      verilog_generate_sdc_disable_one_unused_chan(fp, CHANY, ix, iy, 
                                                   LL_num_rr_nodes, LL_rr_node, 
                                                   LL_rr_node_indices);
    }
  }

  return;
}

/* Go recursively in the hierarchy 
 * and disable all the pb_types 
 */
static 
void rec_verilog_generate_sdc_disable_unused_pb_types(FILE* fp, 
                                                      char* prefix,
                                                      t_pb_type* cur_pb_type) {
  int ipb;
  int mode_index;
  char* pass_on_prefix = NULL;

  /* Skip print the level for the top-level pb_type, 
   * it has been printed outside
   */
  if (NULL == cur_pb_type->parent_mode) {
    pass_on_prefix = my_strdup(prefix);
  } else {
    /* Special prefix for primitive node*/
    /* generate pass_on_prefix */
    pass_on_prefix = (char*) my_malloc(sizeof(char) * 
                                       ( strlen(prefix) + 1 
                                       + strlen(cur_pb_type->name) + 1 + 1)); 
    sprintf(pass_on_prefix, "%s/%s*",
            prefix, cur_pb_type->name); 

    /* Disable everything in this pb_type
     * Use the spice_model_name of current pb_type 
     */
    fprintf(fp, "set_disable_timing ");
    /* Print top-level hierarchy */
    fprintf(fp, "%s/*", 
            pass_on_prefix);
    fprintf(fp, "\n");
  } 

  /* Return if this is the primitive pb_type */
  if (TRUE == is_primitive_pb_type(cur_pb_type)) {
    return;
  }

  /* Go recursively */
  mode_index = find_pb_type_physical_mode_index(*cur_pb_type);
  for (ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
     rec_verilog_generate_sdc_disable_unused_pb_types(fp, pass_on_prefix, 
                                                      &(cur_pb_type->modes[mode_index].pb_type_children[ipb])); 
  }

  /* Free */
  my_free(pass_on_prefix);

  return;
}

/* This block is totally unused.
 * We just go through each pb_type and disable all the ports using wildcards
 */
static 
void verilog_generate_sdc_disable_one_unused_grid(FILE* fp,
                                                  t_type_ptr cur_grid_type,
                                                  int block_x, int block_y,
                                                  int block_z) {
  char* prefix = NULL;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Build prefix, which is the top-level hierarchy */
  prefix = (char*) my_malloc(sizeof(char) * 
                            ( strlen(gen_verilog_one_grid_instance_name(block_x, block_y))
                            + 1 
                            + strlen(gen_verilog_one_phy_block_instance_name(cur_grid_type, block_z))
                            + 2)); 
  sprintf(prefix, "%s/%s",
          gen_verilog_one_grid_instance_name(block_x, block_y),
          gen_verilog_one_phy_block_instance_name(cur_grid_type, block_z)); 

  /* Print comments */
  fprintf(fp,
          "####################################################\n"); 
  fprintf(fp, 
          "### Disable Timing for an unused Grid[%d][%d][%d] ###\n",
          block_x, block_y, block_z);
  fprintf(fp,
          "#####################################################\n"); 
          
  /* Disable everything under this level */
  fprintf(fp, "set_disable_timing ");
  fprintf(fp, "%s/*", prefix);
  fprintf(fp, "\n");

  /* Go recursively in the pb_type hierarchy */
  rec_verilog_generate_sdc_disable_unused_pb_types(fp, prefix,
                                                   cur_grid_type->pb_type); 
                                                  
  /* Free */
  my_free(prefix);

  return;
}

/* The block is used for mapping logic circuits
 * But typically, only part of the logic resources are used.
 * This function will search the local_rr_graph of a phy_pb of the block
 * And disable the unused resources in a SDC format
 */
static 
void verilog_generate_sdc_disable_one_unused_block(FILE* fp,
                                                   t_block* cur_block) {
  int inode;
  t_phy_pb* cur_phy_pb = NULL;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Get the phy_pb */
  cur_phy_pb = (t_phy_pb*)(cur_block->phy_pb);

  /* Print comments */
  fprintf(fp,
          "####################################################\n"); 
  fprintf(fp, 
          "### Disable Timing for a mapped Grid[%d][%d][%d] ###\n",
          cur_block->x, cur_block->y, cur_block->z);
  fprintf(fp,
          "####################################################\n"); 

  /* Search every nodes in the local_rr_graph */
  for (inode = 0; inode < cur_phy_pb->rr_graph->num_rr_nodes; inode++) {
    /* Focus on the SOURCE and SINK rr_nodes */
    if  ((SOURCE != cur_phy_pb->rr_graph->rr_node[inode].type)
      && (SINK   != cur_phy_pb->rr_graph->rr_node[inode].type)) {
       continue; 
    }
    /* Check if pin is virtual */
    if (NULL == cur_phy_pb->rr_graph->rr_node[inode].pb_graph_pin) {
      continue;
    }
    /* Identify if the rr_node is usused */
    if (FALSE == is_rr_node_to_be_disable_for_analysis(&(cur_phy_pb->rr_graph->rr_node[inode]))) {
      continue;
    }
    /* If pin is global port, don't dump */
    if (PB_PIN_CLOCK == cur_phy_pb->rr_graph->rr_node[inode].pb_graph_pin->type) {
      continue;
    }

    /* Get the pb_graph_pin */
    assert (NULL != cur_phy_pb->rr_graph->rr_node[inode].pb_graph_pin);
    /* Disable the timing of this node */
    fprintf(fp, "set_disable_timing ");
    /* Print top-level hierarchy */
    fprintf(fp, "%s/", 
            gen_verilog_one_grid_instance_name(cur_block->x, cur_block->y));
    fprintf(fp, "%s/", 
            gen_verilog_one_phy_block_instance_name(cur_block->type, cur_block->z)); 
    fprintf(fp, "%s", 
            gen_verilog_one_pb_graph_pin_full_name_in_hierarchy(cur_phy_pb->rr_graph->rr_node[inode].pb_graph_pin));
    fprintf(fp, "\n");
  } 

  return;
}

static 
void verilog_generate_sdc_disable_unused_grids(FILE* fp, 
                                               int LL_nx, int LL_ny, 
                                               t_grid_tile** LL_grid,
                                               t_block* LL_block) {
  int ix, iy, iblk; 
  int blk_id;
  t_type_ptr type = NULL;
  boolean* grid_usage = NULL;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  for (ix = 0; ix < (LL_nx + 2); ix++) {
    for (iy = 0; iy < (LL_ny + 2); iy++) {
      type = LL_grid[ix][iy].type;
      /* bypass EMPTY type */
      if ((NULL == type) || (EMPTY_TYPE == type)) {
        continue;
      }
      /* Allocate usage and initialize to unused */
      grid_usage = (boolean*) my_calloc(type->capacity, sizeof(boolean));
      for (iblk = 0; iblk < type->capacity; iblk++) {
        grid_usage[iblk] = FALSE;
      }
      /* Print comments */
      fprintf(fp,
             "#######################################\n"); 
      fprintf(fp, 
             "### Disable Timing for Grid[%d][%d] ###\n",
              ix, iy);
      fprintf(fp,
             "#######################################\n"); 
      /* For used grid, find the unused rr_node in the local rr_graph and disable the pb_graph_pin */
      for (iblk = 0; iblk < LL_grid[ix][iy].usage; iblk++) {
        blk_id = LL_grid[ix][iy].blocks[iblk];
        assert( (OPEN < LL_block[blk_id].z) && (LL_block[blk_id].z < type->capacity) ); 
        /* Label the grid_usage */
        grid_usage[LL_block[blk_id].z] = TRUE;
        verilog_generate_sdc_disable_one_unused_block(fp,
                                                      &(LL_block[blk_id]));
      }
      /* For unused grid, disable all the pins in the physical_pb_type */
      for (iblk = 0; iblk < type->capacity; iblk++) {
        /* Bypass used blocks */
        if (TRUE == grid_usage[iblk]) {
          continue;
        } 
        verilog_generate_sdc_disable_one_unused_grid(fp,
                                                     LL_grid[ix][iy].type, 
                                                     ix, iy, 
                                                     iblk);
        continue;
      }
    }
  }

  /* Free */
  my_free(grid_usage);

  return;
}

static 
void verilog_generate_sdc_disable_unused_grids_muxs(FILE* fp,
                                                   int LL_nx, int LL_ny,
                                                   t_grid_tile** LL_grid,
                                                   t_block* LL_block) {

  int ix, iy, iblk, itype;
  int blk_id;
  t_type_ptr type;
  t_phy_pb* cur_phy_pb;
  t_rr_graph* cur_rr_graph;
  char* grid_instance_name=NULL;
  char* grid_sub_instance_name=NULL;
  char* grid_prefix=NULL;
  
  

  for (ix = 1; ix < (LL_nx + 1); ix++) {
    for (iy = 1; iy < (LL_ny + 1); iy++) {
      type = LL_grid[ix][iy].type;
      /* Print comments */
      fprintf(fp,
             "###########################################\n"); 
      fprintf(fp, 
             "### Disable Timing for Grid[%d][%d] MUXES ###\n",
              ix, iy);
      fprintf(fp,
             "###########################################\n"); 

      grid_instance_name = (char *) my_malloc(sizeof(char) * strlen(gen_verilog_one_grid_instance_name(ix, iy)) + 1);
      grid_instance_name = gen_verilog_one_grid_instance_name(ix, iy);
      for (iblk = 0; iblk < LL_grid[ix][iy].usage; iblk++) {
        blk_id = LL_grid[ix][iy].blocks[iblk];
        grid_sub_instance_name = gen_verilog_one_phy_block_instance_name(type, LL_block[blk_id].z);
        grid_prefix = (char *) my_malloc(sizeof(char) * (strlen(grid_instance_name) + 1 + strlen(grid_sub_instance_name) + 1)); 
        sprintf (grid_prefix, "%s/%s", grid_instance_name, grid_sub_instance_name); 
        cur_phy_pb = (t_phy_pb*) LL_block[blk_id].phy_pb;
        if (NULL != cur_phy_pb) { 
          cur_rr_graph = cur_phy_pb->rr_graph;
          for (itype = 0; itype < num_types; itype++){
            if (FILL_TYPE == &type_descriptors[itype]){
              dump_sdc_one_clb_muxes(fp, grid_prefix, cur_rr_graph, type_descriptors[itype].pb_graph_head);
            }
          }
        }
        my_free(grid_sub_instance_name); 
        my_free(grid_prefix); 
      }
      my_free(grid_instance_name); 
    }
  }
  return;
}

/* Head function of the recursive generation of the sdc constraints
 * on the muxes inside of the CLBs */
void dump_sdc_one_clb_muxes(FILE* fp,
                      char* grid_instance_name,
                      t_rr_graph* rr_graph,
                      t_pb_graph_node* pb_graph_head) {

  /* Give head of the pb_graph to the recursive function*/
  dump_sdc_rec_one_pb_muxes(fp, grid_instance_name, rr_graph, pb_graph_head);

  return;
}

/* Recursive function going to the leaf nodes of the graph and dumping
 * the sdc constraints for the current node. We use the id present inside
 * of the rr_graph to comment the active path and the fan_in and name 
 * inside of the pb_graph to dump the name of the port we need to disable*/
void dump_sdc_rec_one_pb_muxes(FILE* fp,
                               char* grid_instance_name,
                               t_rr_graph* rr_graph,
                               t_pb_graph_node* cur_pb_graph_node) {

  int mode_index;
  int ipb, jpb;
  t_pb_type* cur_pb_type = NULL;

  cur_pb_type = cur_pb_graph_node->pb_type;
 
  if (TRUE == is_primitive_pb_type(cur_pb_type )) {
    return;
  }
  mode_index = find_pb_type_physical_mode_index(*cur_pb_type);  
  for(ipb = 0; ipb < cur_pb_type->modes[mode_index].num_pb_type_children; ipb++) {
    for(jpb = 0; jpb < cur_pb_type->modes[mode_index].pb_type_children[ipb].num_pb; jpb++) {
      dump_sdc_rec_one_pb_muxes(fp, grid_instance_name, rr_graph, 
                                  &(cur_pb_graph_node->child_pb_graph_nodes[mode_index][ipb][jpb]));
	}
  }
  dump_sdc_pb_graph_node_muxes(fp, grid_instance_name, rr_graph,
                                    cur_pb_graph_node);
 
  return;
}

void dump_sdc_pb_graph_node_muxes(FILE* fp,
                                  char* grid_instance_name, 
                                  t_rr_graph* rr_graph, 
                                  t_pb_graph_node* pb_graph_node) {
  int i_pin, i_port;
  // Input pins
  for (i_port = 0; i_port< pb_graph_node->num_input_ports; i_port++) {
    for (i_pin = 0; i_pin < pb_graph_node->num_input_pins[i_port]; i_pin++) {
      dump_sdc_pb_graph_pin_muxes (fp, grid_instance_name, rr_graph, pb_graph_node->input_pins[i_port][i_pin]);
    }
  }
  // Output pins
  for (i_port = 0; i_port< pb_graph_node->num_output_ports; i_port++) {
    for (i_pin = 0; i_pin < pb_graph_node->num_output_pins[i_port]; i_pin++) {
      dump_sdc_pb_graph_pin_muxes (fp, grid_instance_name, rr_graph, pb_graph_node->output_pins[i_port][i_pin]);
    }
  }
  // Clock pins
  for (i_port = 0; i_port< pb_graph_node->num_clock_ports; i_port++) {
    for (i_pin = 0; i_pin < pb_graph_node->num_clock_pins[i_port]; i_pin++) {
      dump_sdc_pb_graph_pin_muxes (fp, grid_instance_name, rr_graph, pb_graph_node->clock_pins[i_port][i_pin]);
    }
  }
  return;
}

void dump_sdc_pb_graph_pin_muxes(FILE* fp,
                                 char* grid_instance_name, 
                                 t_rr_graph* rr_graph, 
                                 t_pb_graph_pin pb_graph_pin) {
  int i_fan_in, datapath_id, fan_in;
  int level_changing = 0;
  t_spice_model* mux_spice_model;
  t_rr_node cur_node = rr_graph->rr_node[pb_graph_pin.rr_node_index_physical_pb]; 
  t_pb_type* cur_pb_type = pb_graph_pin.parent_node->pb_type; 
  int cur_mode_index = find_pb_type_physical_mode_index(*cur_pb_type);
  t_mode* cur_mode = cur_pb_type->modes;  
  t_interconnect* cur_interc; 

  /* There are three types of interconnection: same level, going down a level, going up a level
   * Since we check the fan_in, we need to get the right input edge mode */
  if (0 == pb_graph_pin.num_input_edges || 0 == pb_graph_pin.num_output_edges) {
    return;
  }
  if (pb_graph_pin.input_edges[cur_mode_index]->interconnect->parent_mode != pb_graph_pin.output_edges[cur_mode_index]->interconnect->parent_mode) {
    if (pb_graph_pin.input_edges[cur_mode_index]->interconnect->parent_mode != cur_mode) {
      cur_mode = pb_graph_pin.input_edges[cur_mode_index]->interconnect->parent_mode;
      level_changing = 1;
    }
  } 
 
  find_interc_fan_in_des_pb_graph_pin(&pb_graph_pin, cur_mode, &cur_interc, &fan_in);

  if (0 == fan_in || 1 == fan_in) {
    return; /* Returns if there is no mux */
  }
  /* Handle DEFAULT PATH ID */
  datapath_id = cur_node.id_path;
  if (DEFAULT_PATH_ID == datapath_id) {
    mux_spice_model = cur_interc->spice_model;
    datapath_id = get_mux_default_path_id(mux_spice_model, fan_in, datapath_id);
    /* Either the default is the last pin or the num 0. If fan_in was 0, we wouldn't be here
    * so if the next condition works, datapath_id is actually 1 too far */
    if (fan_in == datapath_id) {
      datapath_id --;
    }
  } else { 
    assert((DEFAULT_PATH_ID < datapath_id)&&(datapath_id < fan_in));
  }
  for (i_fan_in=0 ; i_fan_in < fan_in ; i_fan_in++) {  
    if (i_fan_in == datapath_id) {
      fprintf(fp, "#");
	}
    if (0 == level_changing) {
      fprintf(fp, "set_disable_timing ");
      fprintf(fp, "%s/%s%s/in[%d]\n", grid_instance_name, 
              gen_verilog_one_pb_graph_pin_full_name_in_hierarchy_parent_node(cur_node.pb_graph_pin),
              pb_graph_pin.name_mux, i_fan_in);
    }
    if (1 == level_changing) {
      fprintf(fp, "set_disable_timing ");
      fprintf(fp, "%s/%s%s/in[%d]\n", grid_instance_name, 
              gen_verilog_one_pb_graph_pin_full_name_in_hierarchy_grand_parent_node(cur_node.pb_graph_pin),
              pb_graph_pin.name_mux, i_fan_in);
     } 
            // Hierarchical dumping. Might be broken if extending the software hence going through a more direct method.
            //fprintf(fp, "set_disable_timing [get_pins -filter \"hierarchical_name =");
            //fprintf(fp, "~ *%s/in[%d]\" -of_objects [get_cells -hier -filter ", 
            //        pb_graph_pin->name_mux, i_fan_in);
            //printf("%s", pb_graph_pin->name_mux);

            //fprintf(fp, "\"hierarchical_name =~ %s*\"]]",
            //        grid_instance_name);
            // Might need to comment here the name of the verilog pin connected to ease the debugging
            //fprintf(fp, "\n");
  }
  return;
}

/* Generate SDC constaints for inputs and outputs
 * We consider the top module in formal verification purpose here
 * which is easier 
 */
static 
void verilog_generate_sdc_input_output_delays(FILE* fp,
                                              float critical_path_delay) {
  int iopad_idx, iblock, iport;
  int found_mapped_inpad;
  char* port_name = NULL;
  int num_clock_ports = 0;
  t_spice_model_port** clock_port = NULL;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Get clock port from the global port */
  get_fpga_x2p_global_op_clock_ports(global_ports_head, &num_clock_ports, &clock_port);

  /* Print comments */
  fprintf(fp,
          "##################################################\n"); 
  fprintf(fp, 
          "###             Create clock                     #\n");
  fprintf(fp,
          "##################################################\n"); 

  /* Create a clock */
  for (iport = 0; iport < num_clock_ports; iport++) {
    fprintf(fp, "create_clock ");
    fprintf(fp, "%s -period %.4g -waveform {0 %.4g}\n",
            clock_port[iport]->prefix, 
            critical_path_delay, critical_path_delay/2);
  }
  
  /* Print comments */
  fprintf(fp,
          "##################################################\n"); 
  fprintf(fp, 
          "###        Create input and output delays        #\n");
  fprintf(fp,
          "##################################################\n"); 

  fprintf(fp, "set input_pins \"\"\n");
  fprintf(fp, "set output_pins \"\"\n");
  assert(NULL != iopad_verilog_model);
  for (iopad_idx = 0; iopad_idx < iopad_verilog_model->cnt; iopad_idx++) {
    /* Find if this inpad is mapped to a logical block */
    found_mapped_inpad = 0;
    /* Malloc and assign port_name */
    port_name = gen_verilog_top_module_io_port_prefix(gio_inout_prefix, iopad_verilog_model->prefix);
    /* Find the linked logical block */
    for (iblock = 0; iblock < num_logical_blocks; iblock++) {
      /* Bypass OUTPAD: donot put any voltage stimuli */
      /* Make sure We find the correct logical block !*/
      if ((iopad_verilog_model == logical_block[iblock].mapped_spice_model)
         &&(iopad_idx == logical_block[iblock].mapped_spice_model_index)) {
        /* Output PAD only need a short connection */
        if (VPACK_OUTPAD == logical_block[iblock].type) {
          fprintf(fp, "set_output_delay ");
          fprintf(fp, "-clock ");
          /*for (iport = 0; iport < num_clock_ports; iport++) {
            fprintf(fp, "%s ",
                  clock_port[iport]->prefix);
          }*/
          fprintf(fp, "[get_clocks] ");
          fprintf(fp, "-max %.4g ", 
                  critical_path_delay);
          dump_verilog_generic_port_no_repeat(fp, VERILOG_PORT_CONKT, 
                                    port_name, 
                                    iopad_idx, iopad_idx);
          fprintf(fp, "\n");
          found_mapped_inpad = 1;
        fprintf(fp, "append output_pins \"%s[%d] \"\n",port_name ,iopad_idx);
          break;
        }
        /* Input PAD may drive a clock net or a constant generator */
        assert(VPACK_INPAD == logical_block[iblock].type);
        /* clock net or constant generator should be disabled in timing analysis */
        if (TRUE == logical_block[iblock].is_clock) {
          break;
        }
        fprintf(fp, "set_input_delay ");
        fprintf(fp, "-clock ");
        /*for (iport = 0; iport < num_clock_ports; iport++) {
          fprintf(fp, "%s ",
                clock_port[iport]->prefix);
        }*/
        fprintf(fp, "[get_clocks] ");
        fprintf(fp, "-max 0 ");
        dump_verilog_generic_port_no_repeat(fp, VERILOG_PORT_CONKT, 
                                  port_name, 
                                  iopad_idx, iopad_idx);
        fprintf(fp, "\n");
        found_mapped_inpad = 1;
        fprintf(fp, "append input_pins \"%s[%d] \"\n",port_name ,iopad_idx);
        
      }
    } 
    assert((0 == found_mapped_inpad)||(1 == found_mapped_inpad));
    /* If we find one iopad already, we finished in this round here */
    if (1 == found_mapped_inpad) {
      /* Free */
      my_free(port_name);
      continue;
    }
    /* if we cannot find any mapped inpad from tech.-mapped netlist, set the disable timing! */
    fprintf(fp, "set_disable_timing ");
    dump_verilog_generic_port_no_repeat(fp, VERILOG_PORT_CONKT, 
                              port_name, 
                              iopad_idx, iopad_idx);
    fprintf(fp, "\n");
    /* Free */
    my_free(port_name);
  }

  /* Free */
  my_free(clock_port);

  return;
}
 
static
void verilog_generate_wire_report_timing_blockage_direction(FILE* fp, 
                                                            char* direction,
                                                            char* enable,
                                                            int LL_nx, int LL_ny) {
 
  int ix, iy;
  int side, itrack, inode;
  t_sb* cur_sb_info = NULL;
  
  
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Print comments */
  fprintf(fp,
          "####################################\n"); 
  fprintf(fp, 
          "### Disable for %s Switch blocks ###\n", 
          direction);
  fprintf(fp,
          "####################################\n"); 
  /* We start from a SB[x][y] */
  for (ix = 0; ix < (LL_nx + 1); ix++) {
    for (iy = 0; iy < (LL_ny + 1); iy++) {
      cur_sb_info = &(sb_info[ix][iy]);
      for (side = 0; side < cur_sb_info->num_sides; side++) {
        switch (side) {
          case TOP:
          case BOTTOM:
          if( 0 != strcmp("VERTICAL",direction)) {
            continue;
          }
          for (itrack = 0; itrack < cur_sb_info->chan_width[side]; itrack++) {
            assert((CHANX == cur_sb_info->chan_rr_node[side][itrack]->type)
                 ||(CHANY == cur_sb_info->chan_rr_node[side][itrack]->type));
            if (0 == cur_sb_info->chan_rr_node[side][itrack]->fan_in) {
              continue;
            }
            if (OUT_PORT != cur_sb_info->chan_rr_node_direction[side][itrack]) {
              continue;
            }
            if (0 == strcmp("DISABLE",enable) ) {
              fprintf(fp, "set_disable_timing ");
            }
            if (0 == strcmp("RESTORE",enable) ) {
              fprintf(fp, "reset_disable_timing ");
            }
            fprintf(fp, "%s/", 
                    gen_verilog_one_sb_instance_name(cur_sb_info));
            dump_verilog_one_sb_chan_pin(fp, cur_sb_info,
                                         cur_sb_info->chan_rr_node[side][itrack],
                                         OUT_PORT); 
            fprintf(fp, "\n");
          }
          break;
          case LEFT:
          case RIGHT:
          if( 0 != strcmp("HORIZONTAL",direction)) {
            continue;
          }
          for (itrack = 0; itrack < cur_sb_info->chan_width[side]; itrack++) {
            assert((CHANX == cur_sb_info->chan_rr_node[side][itrack]->type)
                 ||(CHANY == cur_sb_info->chan_rr_node[side][itrack]->type));
            if (0 == cur_sb_info->chan_rr_node[side][itrack]->fan_in) {
              continue;
            }
            if (OUT_PORT != cur_sb_info->chan_rr_node_direction[side][itrack]) {
              continue;
            }
            if (0 == strcmp("DISABLE",enable) ) {
              fprintf(fp, "set_disable_timing ");
            }
            if (0 == strcmp("RESTORE",enable) ) {
              fprintf(fp, "reset_disable_timing ");
            }
            fprintf(fp, "%s/", 
                    gen_verilog_one_sb_instance_name(cur_sb_info));
            dump_verilog_one_sb_chan_pin(fp, cur_sb_info,
                                         cur_sb_info->chan_rr_node[side][itrack],
                                         OUT_PORT); 
            fprintf(fp, "\n");
          }
          break;
          default:
          break;
          } 
        }
      }
    }
  return;
}

static
void verilog_generate_sdc_wire_report_timing_blockage(t_sdc_opts sdc_opts,
                                                      int LL_nx, int LL_ny) {

  FILE* fp = NULL;
  char* sdc_fname = NULL;

  /* Create the vertical file */
  /* Break */
  sdc_fname = my_strcat(sdc_opts.sdc_dir, sdc_break_vertical_sbs_file_name);
  fp = fopen(sdc_fname, "w");
  verilog_generate_wire_report_timing_blockage_direction(fp, "VERTICAL", "DISABLE", LL_nx, LL_ny);
  fclose(fp);
  /* Restore */
  sdc_fname = my_strcat(sdc_opts.sdc_dir, sdc_restore_vertical_sbs_file_name);
  fp = fopen(sdc_fname, "w");
  verilog_generate_wire_report_timing_blockage_direction(fp, "VERTICAL", "RESTORE", LL_nx, LL_ny);
  fclose(fp);

  /* Create the horizontal file */
  
  sdc_fname = my_strcat(sdc_opts.sdc_dir, sdc_break_horizontal_sbs_file_name);
  fp = fopen(sdc_fname, "w");
  verilog_generate_wire_report_timing_blockage_direction(fp, "HORIZONTAL", "DISABLE", LL_nx, LL_ny);
  fclose(fp);
  sdc_fname = my_strcat(sdc_opts.sdc_dir, sdc_restore_horizontal_sbs_file_name);
  fp = fopen(sdc_fname, "w");
  verilog_generate_wire_report_timing_blockage_direction(fp, "HORIZONTAL", "RESTORE", LL_nx, LL_ny);
  fclose(fp);

  return;
}

void verilog_generate_sdc_pnr(t_sram_orgz_info* cur_sram_orgz_info,
                              char* sdc_dir,
                              t_arch arch,
                              t_det_routing_arch* routing_arch,
                              int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                              t_ivec*** LL_rr_node_indices,
                              t_rr_indexed_data* LL_rr_indexed_data,
                              int LL_nx, int LL_ny, DeviceRRGSB& LL_device_rr_gsb, 
                              boolean compact_routing_hierarchy) {
  t_sdc_opts sdc_opts;

  /* Initialize */
  sdc_opts.sdc_dir = my_strdup(sdc_dir);
  sdc_opts.constrain_pbs = TRUE;
  sdc_opts.constrain_routing_channels = TRUE;
  sdc_opts.constrain_sbs = TRUE;
  sdc_opts.constrain_cbs = TRUE;
  sdc_opts.break_loops = TRUE;
  sdc_opts.break_loops_mux = FALSE; /* By default, we turn it off to avoid a overkill */
  sdc_opts.compact_routing_hierarchy = compact_routing_hierarchy; /* By default, we turn it off to avoid a overkill */

  /* Part 1. Constrain clock cycles */
  verilog_generate_sdc_clock_period(sdc_opts, pow(10,9)*arch.spice->spice_params.stimulate_params.vpr_crit_path_delay);

  /* Part 2. Output Design Constraints for breaking loops */
  if (TRUE == sdc_opts.break_loops) {
    verilog_generate_sdc_break_loops(cur_sram_orgz_info, sdc_opts, 
                                     LL_nx, LL_ny, 
                                     routing_arch->num_switch, switch_inf,
                                     arch.spice, LL_device_rr_gsb,
                                     routing_arch); 
  } 

  /* Part 3. Output routing constraints for Switch Blocks */
  if (TRUE == sdc_opts.constrain_sbs) {
    if (TRUE == compact_routing_hierarchy) {
      verilog_generate_sdc_constrain_sbs(sdc_opts, LL_device_rr_gsb); 
    } else {
      verilog_generate_sdc_constrain_sbs(sdc_opts, 
                                         LL_nx, LL_ny); 
    }
    /* Generate sdc files to help get the timing report on Lwires */
    verilog_generate_sdc_wire_report_timing_blockage(sdc_opts, 
                                                     LL_nx, LL_ny);
  }

  /* Part 4. Output routing constraints for Connection Blocks */
  if (TRUE == sdc_opts.constrain_cbs) {
    if (TRUE == compact_routing_hierarchy) {
      verilog_generate_sdc_constrain_cbs(sdc_opts, LL_nx, LL_ny, LL_device_rr_gsb); 
    } else {
      verilog_generate_sdc_constrain_cbs(sdc_opts, 
                                         LL_nx, LL_ny); 
    }
  }

  /* Part 5. Output routing constraints for Connection Blocks */
  if (TRUE == sdc_opts.constrain_routing_channels) {
    verilog_generate_sdc_constrain_routing_channels(sdc_opts, arch, 
                                                    LL_nx, LL_ny, 
                                                    LL_num_rr_nodes, LL_rr_node, 
                                                    LL_rr_node_indices, LL_rr_indexed_data); 
  }

  /* Part 6. Output routing constraints for Programmable blocks */
  if (TRUE == sdc_opts.constrain_pbs) {
    verilog_generate_sdc_constrain_pb_types(cur_sram_orgz_info,
                                            sdc_dir);
  }

  return;
}

/* Output a SDC file to constrain a FPGA mapped with a benchmark */
void verilog_generate_sdc_analysis(t_sram_orgz_info* cur_sram_orgz_info,
                                   char* sdc_dir,
                                   t_arch arch,
                                   int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                   t_ivec*** LL_rr_node_indices,
                                   int LL_nx, int LL_ny, t_grid_tile** LL_grid,
                                   t_block* LL_block, DeviceRRGSB& LL_device_rr_gsb, 
                                   boolean compact_routing_hierarchy) {
  FILE* fp = NULL;
  char* fname = my_strcat(sdc_dir, sdc_analysis_file_name);

  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating SDC for Timing/Power analysis on the mapped FPGA: %s ...\n",
             fname);

  /* Create file handler */
  fp = fopen(fname, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
              "(FILE:%s,LINE[%d])Failure in create SDC constraints %s",
              __FILE__, __LINE__, fname); 
    exit(1);
  } 
  /* Generate the descriptions*/
  dump_verilog_sdc_file_header(fp, "Constrain for Timing/Power analysis on the mapped FPGA");
 
  /* Create clock and set input/output delays */
  verilog_generate_sdc_input_output_delays(fp, 
                                           arch.spice->spice_params.stimulate_params.vpr_crit_path_delay);

  /* Disable the timing for global ports */
  verilog_generate_sdc_disable_global_ports(fp);

  /* Disable the timing for configuration cells */ 
  verilog_generate_sdc_disable_sram_orgz(fp, cur_sram_orgz_info);

  /* Disable timing for un-used resources */
  /* Apply to Routing Channels */
  if (TRUE == compact_routing_hierarchy) {
    verilog_generate_sdc_disable_unused_routing_channels(fp, LL_nx, LL_ny);
  } else {
    verilog_generate_sdc_disable_unused_routing_channels(fp, LL_nx, LL_ny, 
                                                         LL_num_rr_nodes, LL_rr_node, 
                                                         LL_rr_node_indices);
  }

  /* Apply to Connection blocks */
  if (TRUE == compact_routing_hierarchy) {
    verilog_generate_sdc_disable_unused_cbs(fp, LL_nx, LL_ny, LL_device_rr_gsb); 
    verilog_generate_sdc_disable_unused_cbs_muxs(fp, LL_nx, LL_ny, LL_device_rr_gsb);
  } else {
    verilog_generate_sdc_disable_unused_cbs(fp, LL_nx, LL_ny); 
    verilog_generate_sdc_disable_unused_cbs_muxs(fp, LL_nx, LL_ny);
  }

  /* Apply to Switch blocks */
  if (TRUE == compact_routing_hierarchy) {
    verilog_generate_sdc_disable_unused_sbs(fp); 
    verilog_generate_sdc_disable_unused_sbs_muxs(fp);
  } else {
    verilog_generate_sdc_disable_unused_sbs(fp, LL_nx, LL_ny); 
    verilog_generate_sdc_disable_unused_sbs_muxs(fp, LL_nx, LL_ny);
  }

  /* Apply to Grids */
  verilog_generate_sdc_disable_unused_grids(fp, LL_nx, LL_ny, LL_grid, LL_block);
  verilog_generate_sdc_disable_unused_grids_muxs(fp, LL_nx, LL_ny, LL_grid, LL_block);

  /* Close the file*/
  fclose(fp);

  /* Free strings */
  my_free(fname);

  return;
}
