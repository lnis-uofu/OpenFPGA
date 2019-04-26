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
#include "spice_routing.h"
#include "spice_subckt.h"

/* Global parameters */
static int num_segments;
static t_segment_inf* segments;
static int testbench_load_cnt = 0;
static int upbound_sim_num_clock_cycles = 2;
static int max_sim_num_clock_cycles = 2;
static int auto_select_max_sim_num_clock_cycles = TRUE;

static void init_spice_routing_testbench_globals(t_spice spice) {
  auto_select_max_sim_num_clock_cycles = spice.spice_params.meas_params.auto_select_sim_num_clk_cycle;
  upbound_sim_num_clock_cycles = spice.spice_params.meas_params.sim_num_clock_cycle + 1;
  if (FALSE == auto_select_max_sim_num_clock_cycles) {
    max_sim_num_clock_cycles = spice.spice_params.meas_params.sim_num_clock_cycle + 1;
  } else {
    max_sim_num_clock_cycles = 2;
  }
}

static 
void fprint_spice_cb_testbench_global_ports(FILE* fp,
                                            t_spice spice) {
  /* Declare the global SRAM ports */
  fprintf(fp, ".global %s\n",
          spice_tb_global_vdd_cb_sram_port_name);

  return;
}

static 
void fprint_spice_sb_testbench_global_ports(FILE* fp,
                                            t_spice spice) {
  /* Declare the global SRAM ports */
  fprintf(fp, ".global %s\n",
          spice_tb_global_vdd_sb_sram_port_name);

  return;
}


static 
void fprint_spice_routing_testbench_global_ports(FILE* fp,
                                                 t_spice spice) {
  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Print generic global ports*/
  fprint_spice_generic_testbench_global_ports(fp, 
                                              sram_spice_orgz_info,
                                              global_ports_head); 

  return;
}

static 
void fprintf_spice_routing_testbench_generic_stimuli(FILE* fp,
                                                     int num_clocks) {

  /* Give global vdd, gnd, voltage sources*/
  /* A valid file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

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


  return;
}

/** In a testbench, we call the subckt of defined connection box (cbx[x][y] or cby[x][y])
 *  For each input of connection box (channel track rr_nodes), 
 *  we find their activities and generate input voltage pulses.
 *  For each output of connection box, we add all the non-inverter downstream components as load.
 */
static 
int fprint_spice_routing_testbench_call_one_cb_tb(FILE* fp,
                                                  t_spice spice, 
                                                  t_rr_type chan_type,
                                                  int x, int y,
                                                  t_ivec*** LL_rr_node_indices) {
  int itrack, inode, side, ipin_height;
  int side_cnt = 0;
  int used = 0;
  t_cb cur_cb_info;

  float input_density;
  float input_probability;
  int input_init_value;
  float average_cb_input_density = 0.;
  int avg_density_cnt = 0;

  int num_sim_clock_cycles = 0;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 

  /* call the defined switch block sb[x][y]*/
  fprintf(fp, "***** Call defined Connection Box[%d][%d] *****\n", x, y);
  switch(chan_type) { 
  case CHANX:
    cur_cb_info = cbx_info[x][y];
    break;
  case CHANY:
    cur_cb_info = cby_info[x][y];
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }
  fprint_call_defined_one_connection_box(fp, cur_cb_info);

  /* Print input voltage pulses */
  /* connect to the mid point of a track*/
  side_cnt = 0;
  for (side = 0; side < cur_cb_info.num_sides; side++) {
    /* Bypass side with zero channel width */
    if (0 == cur_cb_info.chan_width[side]) {
      continue;
    }
    assert (0 < cur_cb_info.chan_width[side]);
    side_cnt++;
    for (itrack = 0; itrack < cur_cb_info.chan_width[side]; itrack++) {
      /* Add input voltage pulses*/
      input_density = get_rr_node_net_density(*cur_cb_info.chan_rr_node[side][itrack]);
      input_probability = get_rr_node_net_probability(*cur_cb_info.chan_rr_node[side][itrack]);
      input_init_value = get_rr_node_net_init_value(*cur_cb_info.chan_rr_node[side][itrack]);
      fprintf(fp, "***** Signal %s[%d][%d]_midout[%d] density = %g, probability=%g.*****\n",
              convert_chan_type_to_string(cur_cb_info.type),
              cur_cb_info.x, cur_cb_info.y, itrack,
              input_density, input_probability);
      fprintf(fp, "V%s[%d][%d]_midout[%d] %s[%d][%d]_midout[%d] 0 \n", 
              convert_chan_type_to_string(cur_cb_info.type),
              cur_cb_info.x, cur_cb_info.y, itrack,
              convert_chan_type_to_string(cur_cb_info.type),
              cur_cb_info.x, cur_cb_info.y, itrack);
      fprint_voltage_pulse_params(fp, input_init_value, input_density, input_probability);
      /* Update statistics */
      average_cb_input_density += input_density;
      if (0. < input_density) {
        avg_density_cnt++;
      }
    }
  }
  /*check side_cnt */
  assert(1 == side_cnt);

  /* Add loads */
  side_cnt = 0;
  /* Print the ports of grids*/
  /* only check ipin_rr_nodes of cur_cb_info */
  for (side = 0; side < cur_cb_info.num_sides; side++) {
    /* Bypass side with zero IPINs*/
    if (0 == cur_cb_info.num_ipin_rr_nodes[side]) {
      continue;
    }
    side_cnt++;
    assert(0 < cur_cb_info.num_ipin_rr_nodes[side]);
    assert(NULL != cur_cb_info.ipin_rr_node[side]);
    for (inode = 0; inode < cur_cb_info.num_ipin_rr_nodes[side]; inode++) {
      /* Print each INPUT Pins of a grid */
      ipin_height = get_grid_pin_height(cur_cb_info.ipin_rr_node[side][inode]->xlow,
                                        cur_cb_info.ipin_rr_node[side][inode]->ylow,
                                        cur_cb_info.ipin_rr_node[side][inode]->ptc_num);

      if (TRUE == run_testbench_load_extraction) { /* Additional switch, default on! */
        fprint_spice_testbench_one_grid_pin_loads(fp,  
                                                  cur_cb_info.ipin_rr_node[side][inode]->xlow,
                                                  cur_cb_info.ipin_rr_node[side][inode]->ylow, 
                                                  ipin_height,
                                                  cur_cb_info.ipin_rr_node_grid_side[side][inode],
                                                  cur_cb_info.ipin_rr_node[side][inode]->ptc_num,
                                                  &testbench_load_cnt,
                                                  LL_rr_node_indices); 
      }
      fprintf(fp, "\n");
      /* Get signal activity */
      input_density = get_rr_node_net_density(*cur_cb_info.ipin_rr_node[side][inode]);
      input_probability = get_rr_node_net_probability(*cur_cb_info.ipin_rr_node[side][inode]);
      input_init_value = get_rr_node_net_init_value(*cur_cb_info.ipin_rr_node[side][inode]);
      /* Update statistics */
      average_cb_input_density += input_density;
      if (0. < input_density) {
        avg_density_cnt++;
      }
    }
  }
  /* Make sure only 2 sides of IPINs are printed */
  assert((1== side_cnt)||(2 == side_cnt));

  /* Voltage stilumli */
  /* Connect to VDD supply */
  fprintf(fp, "***** Voltage supplies *****\n");
  switch(chan_type) { 
  case CHANX:
    /* Connect to VDD supply */
    fprintf(fp, "***** Voltage supplies *****\n");
    fprintf(fp, "Vgvdd_cb[%d][%d] gvdd_cbx[%d][%d] 0 vsp\n", x, y, x, y);
    break;
  case CHANY:
    /* Connect to VDD supply */
    fprintf(fp, "***** Voltage supplies *****\n");
    fprintf(fp, "Vgvdd_cb[%d][%d] gvdd_cby[%d][%d] 0 vsp\n", x, y, x, y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }
  /* SRAM Voltage stimulit */
  fprintf(fp, "V%s %s 0 vsp\n",
          spice_tb_global_vdd_cb_sram_port_name,
          spice_tb_global_vdd_cb_sram_port_name);

  /* Calculate the num_sim_clock_cycle for this MUX, update global max_sim_clock_cycle in this testbench */
  if (0 < avg_density_cnt) {
    average_cb_input_density = average_cb_input_density/avg_density_cnt;
    num_sim_clock_cycles = (int)(1/average_cb_input_density) + 1;
    used = 1;
  } else {
    assert(0 == avg_density_cnt);
    average_cb_input_density = 0.;
    num_sim_clock_cycles = 2;
    used = 0;
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

  /* Measurements */
  fprint_spice_netlist_transient_setting(fp, spice, num_sim_clock_cycles, FALSE);
  fprint_spice_netlist_generic_measurements(fp, spice.spice_params.mc_params, spice.num_spice_model, spice.spice_models);
  /* Measure the delay of MUX */
  fprintf(fp, "***** Measurements *****\n");
  /* Measure the leakage power of MUX */
  fprintf(fp, "***** Leakage Power Measurement *****\n");
  fprintf(fp, ".meas tran leakage_power_cb avg p(Vgvdd_cb[%d][%d]) from=0 to='clock_period'\n",
          x, y);
  /* Measure the leakage power of SRAMs */
  fprintf(fp, ".meas tran leakage_power_sram_cb avg p(V%s) from=0 to='clock_period'\n",
          spice_tb_global_vdd_cb_sram_port_name);
  /* Measure the dynamic power of MUX */
  fprintf(fp, "***** Dynamic Power Measurement *****\n");
  fprintf(fp, ".meas tran dynamic_power_cb avg p(Vgvdd_cb[%d][%d]) from='clock_period' to='%d*clock_period'\n",
          x, y, num_sim_clock_cycles);
  fprintf(fp, ".meas tran energy_per_cycle_cb param='dynamic_power_cb*clock_period'\n");
  /* Measure the dynamic power of SRAMs */
  fprintf(fp, ".meas tran dynamic_power_sram_cb avg p(V%s) from='clock_period' to='%d*clock_period'\n",
          spice_tb_global_vdd_cb_sram_port_name,
          num_sim_clock_cycles);
  fprintf(fp, ".meas tran energy_per_cycle_sram_cb param='dynamic_power_sram_cb*clock_period'\n");

  /* print average cb input density */
  switch(chan_type) { 
  case CHANX:
    /*
    vpr_printf(TIO_MESSAGE_INFO,"Average density of CBX[%d][%d] inputs is %.2g.\n", x, y, average_cb_input_density);
    */
    break;
  case CHANY:
    /*
    vpr_printf(TIO_MESSAGE_INFO,"Average density of CBY[%d][%d] inputs is %.2g.\n", x, y, average_cb_input_density);
    */
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }

  return used;
}

/** In a testbench, we call the subckt of a defined switch block (sb[x][y])
 *  For each input of switch block, we find their activities and generate input voltage pulses.
 *  For each output of switch block, we add all the non-inverter downstream components as load.
 */
static 
int fprint_spice_routing_testbench_call_one_sb_tb(FILE* fp, 
                                                  t_spice spice, 
                                                  int x, int y, 
                                                  t_ivec*** LL_rr_node_indices) {
  int itrack, inode, side, ipin_height, ix, iy;
  int used = 0;
  t_sb cur_sb_info;

  char* outport_name = NULL;
  char* rr_node_outport_name = NULL;

  float input_density;
  float input_probability;
  int input_init_value;
  float average_sb_input_density = 0.;
  int avg_density_cnt = 0;

  int num_sim_clock_cycles = 0;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 

  /* call the defined switch block sb[x][y]*/
  fprintf(fp, "***** Call defined Switch Box[%d][%d] *****\n", x, y);
  fprint_call_defined_one_switch_box(fp, sb_info[x][y]);

  cur_sb_info = sb_info[x][y];

  /* For each input of switch block, we generate a input voltage pulse
   * For each output of switch block, we generate downstream loads 
   */
  /* Find all rr_nodes of channels */
  for (side = 0; side < cur_sb_info.num_sides; side++) {
    determine_sb_port_coordinator(cur_sb_info, side, &ix, &iy); 

    for (itrack = 0; itrack < cur_sb_info.chan_width[side]; itrack++) {
      /* Print voltage stimuli and loads */
      switch (cur_sb_info.chan_rr_node_direction[side][itrack]) {
      case OUT_PORT:
        /* Output port requires loads*/
        /* We should not add any loads to those outputs that are driven simply by a wire in this switch box!
        if (1 == is_sb_interc_between_segments(cur_sb_info.x, cur_sb_info.y, 
                                               cur_sb_info.chan_rr_node[side][itrack], side)) {
          break;
        }
        */
        /* Only consider the outputs that are driven by a multiplexer */
        outport_name = (char*)my_malloc(sizeof(char)*(
                strlen(convert_chan_type_to_string(cur_sb_info.chan_rr_node[side][itrack]->type))
                + 1 + strlen(my_itoa(cur_sb_info.x)) + 2 + strlen(my_itoa(cur_sb_info.y))
                + 6 + strlen(my_itoa(itrack))
                + 1 + 1));
        sprintf(outport_name, "%s[%d][%d]_out[%d]", 
                convert_chan_type_to_string(cur_sb_info.chan_rr_node[side][itrack]->type), 
                ix, iy, itrack);
        if (TRUE == run_testbench_load_extraction) { /* Additional switch, default on! */
          fprintf(fp, "**** Load for rr_node[%ld] *****\n", cur_sb_info.chan_rr_node[side][itrack] - rr_node);
          rr_node_outport_name = fprint_spice_testbench_rr_node_load_version(fp, &testbench_load_cnt,
                                                                             num_segments, 
                                                                             segments, 
                                                                             0, 
                                                                             *cur_sb_info.chan_rr_node[side][itrack],
                                                                             outport_name); 
        }
        /* Free */
        my_free(rr_node_outport_name);
        break;
      case IN_PORT:
        /* Get signal activity */
        input_density = get_rr_node_net_density(*cur_sb_info.chan_rr_node[side][itrack]);
        input_probability = get_rr_node_net_probability(*cur_sb_info.chan_rr_node[side][itrack]);
        input_init_value = get_rr_node_net_init_value(*cur_sb_info.chan_rr_node[side][itrack]);
        /* Update statistics */
        average_sb_input_density += input_density;
        if (0. < input_density) {
         avg_density_cnt++;
        }
        /* Input port requires a voltage stimuli */
        /* Add input voltage pulses*/
        fprintf(fp, "***** Signal %s[%d][%d]_in[%d] density = %g, probability=%g.*****\n",
                convert_chan_type_to_string(cur_sb_info.chan_rr_node[side][itrack]->type), 
                ix, iy, itrack,
                input_density, input_probability);
        fprintf(fp, "V%s[%d][%d]_in[%d] %s[%d][%d]_in[%d] 0 \n", 
                convert_chan_type_to_string(cur_sb_info.chan_rr_node[side][itrack]->type), 
                ix, iy, itrack,
                convert_chan_type_to_string(cur_sb_info.chan_rr_node[side][itrack]->type), 
                ix, iy, itrack);
        fprint_voltage_pulse_params(fp, input_init_value, input_density, input_probability);
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of sb[%d][%d] side[%d] track[%d]!\n",
                   __FILE__, __LINE__, cur_sb_info.x, cur_sb_info.y, side, itrack);
        exit(1);
      }
    }
    /* OPINs of adjacent CLBs are inputs and requires a voltage stimuli */
    /* Input port requires a voltage stimuli */
    for (inode = 0; inode < cur_sb_info.num_opin_rr_nodes[side]; inode++) {
      /* Print voltage stimuli of each OPIN */
      ipin_height = get_grid_pin_height(cur_sb_info.opin_rr_node[side][inode]->xlow,
                                        cur_sb_info.opin_rr_node[side][inode]->ylow,
                                        cur_sb_info.opin_rr_node[side][inode]->ptc_num);
      fprint_spice_testbench_one_grid_pin_stimulation(fp,  
                                                      cur_sb_info.opin_rr_node[side][inode]->xlow,
                                                      cur_sb_info.opin_rr_node[side][inode]->ylow,
                                                      ipin_height,
                                                      cur_sb_info.opin_rr_node_grid_side[side][inode],
                                                      cur_sb_info.opin_rr_node[side][inode]->ptc_num,
                                                      LL_rr_node_indices);
      /* Get signal activity */
      input_density = get_rr_node_net_density(*cur_sb_info.opin_rr_node[side][inode]);
      input_probability = get_rr_node_net_probability(*cur_sb_info.opin_rr_node[side][inode]);
      input_init_value = get_rr_node_net_init_value(*cur_sb_info.opin_rr_node[side][inode]);
      /* Update statistics */
      average_sb_input_density += input_density;
      if (0. < input_density) {
       avg_density_cnt++;
      }
    } 
    fprintf(fp, "\n");
  }

  /* Connect to VDD supply */
  fprintf(fp, "***** Voltage supplies *****\n");
  fprintf(fp, "Vgvdd_sb[%d][%d] gvdd_sb[%d][%d] 0 vsp\n", x, y, x, y);
  /* SRAM Voltage stimulit */
  fprintf(fp, "V%s %s 0 vsp\n",
          spice_tb_global_vdd_sb_sram_port_name,
          spice_tb_global_vdd_sb_sram_port_name);

  /* Calculate the num_sim_clock_cycle for this MUX, update global max_sim_clock_cycle in this testbench */
  if (0 < avg_density_cnt) {
    average_sb_input_density = average_sb_input_density/avg_density_cnt;
    num_sim_clock_cycles = (int)(1/average_sb_input_density) + 1;
    used = 1;
  } else {
    assert(0 == avg_density_cnt);
    average_sb_input_density = 0.;
    num_sim_clock_cycles = 2;
    used = 0;
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

  /* Measurements */
  fprint_spice_netlist_transient_setting(fp, spice, num_sim_clock_cycles, FALSE);
  fprint_spice_netlist_generic_measurements(fp, spice.spice_params.mc_params, spice.num_spice_model, spice.spice_models);
  /* Measure the delay of MUX */
  fprintf(fp, "***** Measurements *****\n");
  /* Measure the leakage power of MUX */
  fprintf(fp, "***** Leakage Power Measurement *****\n");
  fprintf(fp, ".meas tran leakage_power_sb avg p(Vgvdd_sb[%d][%d]) from=0 to='clock_period'\n",
          x, y);
  /* Measure the leakage power of SRAMs */
  fprintf(fp, ".meas tran leakage_power_sram_sb avg p(V%s) from=0 to='clock_period'\n",
          spice_tb_global_vdd_sb_sram_port_name);
  /* Measure the dynamic power of MUX */
  fprintf(fp, "***** Dynamic Power Measurement *****\n");
  fprintf(fp, ".meas tran dynamic_power_sb avg p(Vgvdd_sb[%d][%d]) from='clock_period' to='%d*clock_period'\n",
          x, y, num_sim_clock_cycles);
  fprintf(fp, ".meas tran energy_per_cycle_sb param='dynamic_power_sb*clock_period'\n");
  /* Measure the dynamic power of SRAMs */
  fprintf(fp, ".meas tran dynamic_power_sram_sb avg p(V%s) from='clock_period' to='%d*clock_period'\n",
          spice_tb_global_vdd_sb_sram_port_name,
          num_sim_clock_cycles);
  fprintf(fp, ".meas tran energy_per_cycle_sram_sb param='dynamic_power_sram_sb*clock_period'\n");

  /* print average sb input density */
  /*
  vpr_printf(TIO_MESSAGE_INFO,"Average density of SB[%d][%d] inputs is %.2g.\n", x, y, average_sb_input_density);
  */

  /* Free */
 
  return used;
}

int fprint_spice_one_cb_testbench(char* formatted_spice_dir,
                                  char* circuit_name,
                                  char* cb_testbench_name, 
                                  char* include_dir_path,
                                  char* subckt_dir_path,
                                  t_ivec*** LL_rr_node_indices,
                                  int num_clocks,
                                  t_arch arch,
                                  int grid_x, int grid_y, t_rr_type cb_type,
                                  boolean leakage_only) {
  FILE* fp = NULL;
  char* formatted_subckt_dir_path = format_dir_path(subckt_dir_path);
  char* title = my_strcat("FPGA SPICE Connection Box Testbench Bench for Design: ", circuit_name);
  char* cb_testbench_file_path = my_strcat(formatted_spice_dir, cb_testbench_name);
  char* cb_tb_name = NULL;
  int used = 0;
  char* temp_include_file_path = NULL;

  /* one cbx, one cby*/
  switch (cb_type) {
  case CHANX:
    cb_tb_name = "Connection Box X-channel ";
    temp_include_file_path = fpga_spice_create_one_subckt_filename(cbx_spice_file_name_prefix, grid_x, grid_y, spice_netlist_file_postfix);
    break;
  case CHANY:
    cb_tb_name = "Connection Box Y-channel ";
    temp_include_file_path = fpga_spice_create_one_subckt_filename(cby_spice_file_name_prefix, grid_x, grid_y, spice_netlist_file_postfix);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d]) Invalid connection_box_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Check if the path exists*/
  fp = fopen(cb_testbench_file_path,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create SPICE %s Test bench netlist %s!\n", 
               __FILE__, __LINE__, cb_tb_name, cb_testbench_file_path); 
    exit(1);
  }

  /* Load global vars in this source file */
  num_segments = arch.num_segments;
  segments = arch.Segments;
  testbench_load_cnt = 0;

  /* Print the title */
  fprint_spice_head(fp, title);
  my_free(title);

  /* print technology library and design parameters*/
  /* fprint_tech_lib(fp, arch.spice->tech_lib); */

  /* Include parameter header files */
  fprint_spice_include_param_headers(fp, include_dir_path);

  /* Include Key subckts */
  fprint_spice_include_key_subckts(fp, formatted_subckt_dir_path);

  /* Include user-defined sub-circuit netlist */
  init_include_user_defined_netlists(*(arch.spice));
  fprint_include_user_defined_netlists(fp, *(arch.spice));

  /* Print simulation temperature and other options for SPICE */
  fprint_spice_options(fp, arch.spice->spice_params);

  /* Global nodes: Vdd for SRAMs, Logic Blocks(Include IO), Switch Boxes, Connection Boxes */
  fprint_spice_routing_testbench_global_ports(fp, *(arch.spice));
  fprint_spice_cb_testbench_global_ports(fp, *(arch.spice));
 
  /* Quote defined Logic blocks subckts (Grids) */
  init_spice_routing_testbench_globals(*(arch.spice));

  /* one cbx, one cby*/
  switch (cb_type) {
  case CHANX:
  case CHANY:
    /* Generate filename */
    fprintf(fp, "****** Include subckt netlists: %s [%d][%d] *****\n",
            cb_tb_name, grid_x, grid_y);
    /* Check if we include an existing file! */
    if (FALSE == check_subckt_file_exist_in_llist(routing_spice_subckt_file_path_head, 
                                                  my_strcat(formatted_subckt_dir_path, temp_include_file_path))) {
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Intend to include a non-existed SPICE netlist %s!",
                 __FILE__, __LINE__, temp_include_file_path); 
      exit(1);
    }
    spice_print_one_include_subckt_line(fp, formatted_subckt_dir_path, temp_include_file_path);
    used = fprint_spice_routing_testbench_call_one_cb_tb(fp, *(arch.spice), cb_type, grid_x, grid_y, LL_rr_node_indices);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d]) Invalid connection_box_type!\n", __FILE__, __LINE__);
    exit(1);
  }


  /* Generate SPICE routing testbench generic stimuli*/
  fprintf_spice_routing_testbench_generic_stimuli(fp, num_clocks);

  /* SPICE ends*/
  fprintf(fp, ".end\n");

  /* Close the file*/
  fclose(fp);

  /* Push the testbench to the linked list */
  tb_head = add_one_spice_tb_info_to_llist(tb_head, cb_testbench_file_path, 
                                           max_sim_num_clock_cycles);
  used = 1;

  /* Free */
  my_free(temp_include_file_path);

  return used;
}

int fprint_spice_one_sb_testbench(char* formatted_spice_dir,
                                  char* circuit_name,
                                  char* sb_testbench_name, 
                                  char* include_dir_path,
                                  char* subckt_dir_path,
                                  t_ivec*** LL_rr_node_indices,
                                  int num_clocks,
                                  t_arch arch,
                                  int grid_x, int grid_y, 
                                  boolean leakage_only) {
  FILE* fp = NULL;
  char* formatted_subckt_dir_path = format_dir_path(subckt_dir_path);
  char* title = my_strcat("FPGA SPICE Switch Block Testbench Bench for Design: ", circuit_name);
  char* sb_testbench_file_path = my_strcat(formatted_spice_dir, sb_testbench_name);
  char* sb_tb_name = NULL;
  int used = 0;
  char* temp_include_file_path = NULL;

  sb_tb_name = "Switch Block ";

  /* Check if the path exists*/
  fp = fopen(sb_testbench_file_path,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create SPICE %s Test bench netlist %s!\n", 
               __FILE__, __LINE__, sb_tb_name, sb_testbench_file_path); 
    exit(1);
  }

  /* Load global vars in this source file */
  num_segments = arch.num_segments;
  segments = arch.Segments;
  testbench_load_cnt = 0;

  /* Print the title */
  fprint_spice_head(fp, title);
  my_free(title);

  /* print technology library and design parameters*/

  /* Include parameter header files */
  fprint_spice_include_param_headers(fp, include_dir_path);

  /* Include Key subckts */
  fprint_spice_include_key_subckts(fp, formatted_subckt_dir_path);

  /* Include user-defined sub-circuit netlist */
  init_include_user_defined_netlists(*(arch.spice));
  fprint_include_user_defined_netlists(fp, *(arch.spice));

  /* Print simulation temperature and other options for SPICE */
  fprint_spice_options(fp, arch.spice->spice_params);

  /* Global nodes: Vdd for SRAMs, Logic Blocks(Include IO), Switch Boxes, Connection Boxes */
  fprint_spice_routing_testbench_global_ports(fp, *(arch.spice));
  fprint_spice_sb_testbench_global_ports(fp, *(arch.spice));
 
  /* Quote defined Logic blocks subckts (Grids) */
  init_spice_routing_testbench_globals(*(arch.spice));

  /* Generate filename */
  fprintf(fp, "****** Include subckt netlists: Switch Block[%d][%d] *****\n",
          grid_x, grid_y);
  temp_include_file_path = fpga_spice_create_one_subckt_filename(sb_spice_file_name_prefix, grid_x, grid_y, spice_netlist_file_postfix);
  /* Check if we include an existing file! */
  if (FALSE == check_subckt_file_exist_in_llist(routing_spice_subckt_file_path_head, 
                                                my_strcat(formatted_subckt_dir_path, temp_include_file_path))) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Intend to include a non-existed SPICE netlist %s!",
               __FILE__, __LINE__, temp_include_file_path); 
    exit(1);
  }
  spice_print_one_include_subckt_line(fp, formatted_subckt_dir_path, temp_include_file_path);

  used = fprint_spice_routing_testbench_call_one_sb_tb(fp, *(arch.spice), grid_x, grid_y, LL_rr_node_indices);


  /* Generate SPICE routing testbench generic stimuli*/
  fprintf_spice_routing_testbench_generic_stimuli(fp, num_clocks);

  /* SPICE ends*/
  fprintf(fp, ".end\n");

  /* Close the file*/
  fclose(fp);

  /* Push the testbench to the linked list */
  tb_head = add_one_spice_tb_info_to_llist(tb_head, sb_testbench_file_path, 
                                           max_sim_num_clock_cycles);
  used = 1;

  return used;
}

/* Top function: Generate testbenches for all Connection Boxes */
void spice_print_cb_testbench(char* formatted_spice_dir,
                               char* circuit_name,
                               char* include_dir_path,
                               char* subckt_dir_path,
                               t_ivec*** LL_rr_node_indices,
                               int num_clocks,
                               t_arch arch,
                               boolean leakage_only) {
  char* cb_testbench_name = NULL; 
  int ix, iy;
  int cnt = 0;
  int used = 0;

  /* X-channel Connection Blocks */
  vpr_printf(TIO_MESSAGE_INFO,"Generating X-channel Connection Block testbench...\n");
  for (iy = 0; iy < (ny+1); iy++) {
    for (ix = 1; ix < (nx+1); ix++) {
      /* Bypass non-exist CBs */
      if ((FALSE == is_cb_exist(CHANX, ix, iy))
         ||(0 == count_cb_info_num_ipin_rr_nodes(cbx_info[ix][iy]))) {
        continue;
      }
      cb_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                            + 4 + strlen(my_itoa(ix)) + 2 + strlen(my_itoa(iy)) + 1
                                            + strlen(spice_cb_testbench_postfix)  + 1 ));
      sprintf(cb_testbench_name, "%s_cbx%d_%d%s",
              circuit_name, ix, iy, spice_cb_testbench_postfix);
      used = fprint_spice_one_cb_testbench(formatted_spice_dir, circuit_name, cb_testbench_name, 
                                            include_dir_path, subckt_dir_path, LL_rr_node_indices,
                                            num_clocks, arch, ix, iy, CHANX, 
                                            leakage_only);
      if (1 == used) {
        cnt += used;
      }
      /* free */
      my_free(cb_testbench_name);
    }  
  } 

  /* Y-channel Connection Blocks */
  vpr_printf(TIO_MESSAGE_INFO,"Generating Y-channel Connection Block testbench...\n");
  for (ix = 0; ix < (nx+1); ix++) {
    for (iy = 1; iy < (ny+1); iy++) {
      /* Bypass non-exist CBs */
      if ((FALSE == is_cb_exist(CHANY, ix, iy))
         ||(0 == count_cb_info_num_ipin_rr_nodes(cby_info[ix][iy]))) {
        continue;
      }
      cb_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                            + 4 + strlen(my_itoa(ix)) + 2 + strlen(my_itoa(iy)) + 1
                                            + strlen(spice_cb_testbench_postfix)  + 1 ));
      sprintf(cb_testbench_name, "%s_cby%d_%d%s",
               circuit_name, ix, iy, spice_cb_testbench_postfix);
      used = fprint_spice_one_cb_testbench(formatted_spice_dir, circuit_name, cb_testbench_name, 
                                            include_dir_path, subckt_dir_path, LL_rr_node_indices,
                                            num_clocks, arch, ix, iy, CHANY,  
                                            leakage_only);
      if (1 == used) {
        cnt += used;
      }
      /* free */
      my_free(cb_testbench_name);
    }  
  } 
  /* Update the global counter */
  num_used_cb_tb = cnt;
  vpr_printf(TIO_MESSAGE_INFO,"No. of generated Connection Block testbench = %d\n", num_used_cb_tb);

  
  return;
}

/* Top function: Generate testbenches for all Switch Blocks */
void spice_print_sb_testbench(char* formatted_spice_dir,
                               char* circuit_name,
                               char* include_dir_path,
                               char* subckt_dir_path,
                               t_ivec*** LL_rr_node_indices,
                               int num_clocks,
                               t_arch arch,
                               boolean leakage_only) {

  char* sb_testbench_name = NULL; 
  int ix, iy;
  int cnt = 0;
  int used = 0;

  vpr_printf(TIO_MESSAGE_INFO,"Generating Switch Block testbench...\n");

  for (ix = 0; ix < (nx+1); ix++) {
    for (iy = 0; iy < (ny+1); iy++) {
      sb_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                            + 4 + strlen(my_itoa(ix)) + 2 + strlen(my_itoa(iy)) + 1
                                            + strlen(spice_sb_testbench_postfix)  + 1 ));
      sprintf(sb_testbench_name, "%s_sb%d_%d%s",
              circuit_name, ix, iy, spice_sb_testbench_postfix);
      used = fprint_spice_one_sb_testbench(formatted_spice_dir, circuit_name, sb_testbench_name, 
                                            include_dir_path, subckt_dir_path, LL_rr_node_indices,
                                            num_clocks, arch, ix, iy, 
                                            leakage_only);
      if (1 == used) {
        cnt += used;
      }
      /* free */
      my_free(sb_testbench_name);
    }  
  } 
  /* Update the global counter */
  num_used_sb_tb = cnt;
  vpr_printf(TIO_MESSAGE_INFO,"No. of generated Switch Block testbench = %d\n", num_used_sb_tb);

  return;
}
