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
#include "spice_pbtypes.h"
#include "spice_subckt.h"
#include "spice_grid_testbench.h"

/* Global variable inside this C-source file*/
/*
static int num_inv_load = 0;
static int num_noninv_load = 0;
static int num_grid_load = 0;
*/
static int testbench_load_cnt = 0;
static int tb_num_grid = 0;
static int max_sim_num_clock_cycles = 2;
static int upbound_sim_num_clock_cycles = 2;
static int auto_select_max_sim_num_clock_cycles = TRUE;

/* Local subroutines only accessible in this C-source file */
static 
void init_spice_grid_testbench_globals(t_spice spice) {
  testbench_load_cnt = 0;
  tb_num_grid = 0;
  auto_select_max_sim_num_clock_cycles = spice.spice_params.meas_params.auto_select_sim_num_clk_cycle;
  upbound_sim_num_clock_cycles = spice.spice_params.meas_params.sim_num_clock_cycle + 1;
  if (FALSE == auto_select_max_sim_num_clock_cycles) {
    max_sim_num_clock_cycles = spice.spice_params.meas_params.sim_num_clock_cycle + 1;
  } else {
    max_sim_num_clock_cycles = 2;
  }
}

/* Subroutines in this source file*/
static 
void fprint_spice_grid_testbench_global_ports(FILE* fp, int x, int y,
                                              int num_clock, 
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

  fprintf(fp, ".global %s %s %s\n",
              spice_tb_global_vdd_localrouting_port_name,
              spice_tb_global_vdd_io_port_name,
              spice_tb_global_vdd_hardlogic_port_name);

  /* Print the VDD ports of SRAM belonging to other SPICE module */
  fprintf(fp, ".global %s\n",
          spice_tb_global_vdd_localrouting_sram_port_name);
  fprintf(fp, ".global %s\n",
          spice_tb_global_vdd_lut_sram_port_name);
  fprintf(fp, ".global %s\n",
          spice_tb_global_vdd_io_sram_port_name);
 
  /*Global Vdds for LUTs*/
  fprint_grid_global_vdds_spice_model(fp, x, y, SPICE_MODEL_LUT, spice);

  /*Global Vdds for FFs*/
  fprint_grid_global_vdds_spice_model(fp, x, y, SPICE_MODEL_FF, spice);

  /*Global Vdds for IOPADs*/
  fprint_grid_global_vdds_spice_model(fp, x, y, SPICE_MODEL_IOPAD, spice);

  /*Global Vdds for Hardlogics*/
  fprint_grid_global_vdds_spice_model(fp, x, y, SPICE_MODEL_HARDLOGIC, spice);

  return;
}

void fprint_spice_grid_testbench_call_defined_core_grids(FILE* fp) {
  int ix, iy;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Normal Grids */
  for (ix = 1; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      assert(IO_TYPE != grid[ix][iy].type);
      fprintf(fp, "Xgrid[%d][%d] ", ix, iy);
      fprint_grid_pins(fp, ix, iy, 1);
      fprintf(fp, "gvdd 0 grid[%d][%d]\n", ix, iy); /* Call the name of subckt */ 
    }
  } 

  return;
}

void fprint_spice_grid_testbench_call_one_defined_grid(FILE* fp, int ix, int iy) {

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* if ((NULL == grid[ix][iy].type)||(0 != grid[ix][iy].offset)||(0 == grid[ix][iy].usage)) { */
  if ((NULL == grid[ix][iy].type)
     ||(EMPTY_TYPE == grid[ix][iy].type)
     ||(0 != grid[ix][iy].offset)) {
    return;
  }

  if (IO_TYPE == grid[ix][iy].type) {
    fprintf(fp, "Xgrid[%d][%d] \n", ix, iy);
    fprint_io_grid_pins(fp, ix, iy, 1);
    fprintf(fp, "+ gvdd 0 grid[%d][%d]\n", ix, iy); /* Call the name of subckt */ 
    tb_num_grid++;
  } else {
    fprintf(fp, "Xgrid[%d][%d] \n", ix, iy);
    fprint_grid_pins(fp, ix, iy, 1);
    fprintf(fp, "+ gvdd 0 grid[%d][%d]\n", ix, iy); /* Call the name of subckt */ 
    tb_num_grid++;
  }

  return;
}

int get_grid_testbench_one_grid_num_sim_clock_cycles(FILE* fp, 
                                                     t_spice spice,
                                                     t_ivec*** LL_rr_node_indices,
                                                     int x, int y) {
  int ipin, class_id, side, iheight;
  t_type_ptr type = NULL;
  int ipin_rr_node_index;
  float ipin_density = 0.;
  float average_density = 0.;
  int avg_density_cnt = 0;
  int num_sim_clock_cycles = 0;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  type = grid[x][y].type;
  assert(NULL != type);
   
  average_density = 0.;
  avg_density_cnt = 0;
  /* For each input pin, we give a stimulate*/ 
  for (side = 0; side < 4; side++) {
    for (iheight = 0; iheight < type->height; iheight++) {
      for (ipin = 0; ipin < type->num_pins; ipin++) {
        if (1 == type->pinloc[iheight][side][ipin]) {
          class_id = type->pin_class[ipin];
          if (RECEIVER == type->class_inf[class_id].type) { 
            /* Print a voltage source according to density and probability */
            ipin_rr_node_index = get_rr_node_index(x, y, IPIN, ipin, LL_rr_node_indices);
            /* Get density and probability */
            ipin_density = get_rr_node_net_density(rr_node[ipin_rr_node_index]); 
            if (0. < ipin_density) {
              average_density += ipin_density;
              avg_density_cnt++;
            }
          }
        }
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
  if (TRUE == auto_select_max_sim_num_clock_cycles) {
    if (0. == average_density) {
      num_sim_clock_cycles = 2;
    } else {
      assert(0. < average_density);
      num_sim_clock_cycles = (int)(1/average_density) + 1;
    }
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

  return num_sim_clock_cycles;
}


void fprint_grid_testbench_one_grid_stimulation(FILE* fp, 
                                                t_spice spice,
                                                t_ivec*** LL_rr_node_indices,
                                                int x, int y) {
  int ipin, class_id, side, iheight;
  t_type_ptr type = NULL;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  type = grid[x][y].type;
  assert(NULL != type);
   
  /* For each input pin, we give a stimulate*/ 
  for (side = 0; side < 4; side++) {
    for (iheight = 0; iheight < type->height; iheight++) {
      for (ipin = 0; ipin < type->num_pins; ipin++) {
        if (1 == type->pinloc[iheight][side][ipin]) {
          class_id = type->pin_class[ipin];
          if (RECEIVER == type->class_inf[class_id].type) { 
            fprint_spice_testbench_one_grid_pin_stimulation(fp, x, y, iheight, side, ipin, LL_rr_node_indices);
          } else if (DRIVER == type->class_inf[class_id].type) { 
            if (TRUE == run_testbench_load_extraction) { /* Additional switch, default on! */
              fprint_spice_testbench_one_grid_pin_loads(fp, x, y, iheight, side, ipin, &testbench_load_cnt, LL_rr_node_indices);
            }
          } else {
            fprint_stimulate_dangling_one_grid_pin(fp, x, y, iheight, side, ipin, LL_rr_node_indices);
          }
        }
      }
    }
  }

  return;
}

static 
void fprint_spice_grid_testbench_stimulations(FILE* fp, 
                                              int num_clock,
                                              t_spice spice,
                                              int grid_x, int grid_y,
                                              t_ivec*** LL_rr_node_indices) {
  /* int ix, iy; */

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Print generic stimuli */
  fprint_spice_testbench_generic_global_ports_stimuli(fp, num_clock);
  
  /* Generate global ports stimuli */
  fprint_spice_testbench_global_ports_stimuli(fp, global_ports_head);

  /* SRAM ports */
  /* Every SRAM inputs should have a voltage source */
  fprintf(fp, "***** Global Inputs for SRAMs *****\n");
  fprint_spice_testbench_global_sram_inport_stimuli(fp, sram_spice_orgz_info);

  fprintf(fp, "***** Global VDD for SRAMs *****\n");
  fprint_spice_testbench_global_vdd_port_stimuli(fp,
                                                 spice_tb_global_vdd_sram_port_name,
                                                 "vsp");

  /* Global routing Vdds */
  fprint_spice_testbench_global_vdd_port_stimuli(fp, 
                                                 spice_tb_global_vdd_localrouting_port_name,
                                                 "vsp");

  /* Global Vdds for SRAMs */
  fprint_spice_testbench_global_vdd_port_stimuli(fp, 
                                                 spice_tb_global_vdd_lut_sram_port_name,
                                                 "vsp");

  fprint_spice_testbench_global_vdd_port_stimuli(fp, 
                                                 spice_tb_global_vdd_localrouting_sram_port_name,
                                                 "vsp");
  
  fprint_spice_testbench_global_vdd_port_stimuli(fp, 
                                                 spice_tb_global_vdd_io_sram_port_name,
                                                 "vsp");

  fprintf(fp, "***** Global VDD for load inverters *****\n");
  fprint_spice_testbench_global_vdd_port_stimuli(fp,
                                                 spice_tb_global_vdd_load_port_name,
                                                 "vsp");

  /* Every Hardlogic use an independent Voltage source */
  fprintf(fp, "***** Global VDD for Hard Logics *****\n");
  fprint_grid_splited_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_HARDLOGIC, spice);

  /* Every LUT use an independent Voltage source */
  fprintf(fp, "***** Global VDD for Look-Up Tables (LUTs) *****\n");
  fprint_grid_splited_vdds_spice_model(fp, grid_x, grid_y,SPICE_MODEL_LUT, spice);

  /* Every FF use an independent Voltage source */
  fprintf(fp, "***** Global VDD for Flip-flops (FFs) *****\n");
  fprint_grid_splited_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_FF, spice);

  /* For each grid input port, we generate the voltage pulses  */
  fprint_grid_testbench_one_grid_stimulation(fp, spice, LL_rr_node_indices,
                                             grid_x, grid_y);

  return;
}

static 
void fprint_spice_grid_testbench_measurements(FILE* fp, int grid_x, int grid_y, 
                                              t_spice spice,
                                              boolean leakage_only) {
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
  if (TRUE == leakage_only) {
    /* Leakage power of SRAMs */
    fprintf(fp, ".measure tran leakage_power_sram_local_routing find p(Vgvdd_sram_local_routing) at=0\n");
    fprintf(fp, ".measure tran leakage_power_sram_luts find p(Vgvdd_sram_luts) at=0\n");
    /* Global power of Local Interconnections*/
    fprintf(fp, ".measure tran leakage_power_local_routing find p(Vgvdd_local_interc) at=0\n");
  } else {
    /* Leakage power of SRAMs */
    fprintf(fp, ".measure tran leakage_power_sram_local_routing avg p(Vgvdd_sram_local_routing) from=0 to='clock_period'\n");
    fprintf(fp, ".measure tran leakage_power_sram_luts avg p(Vgvdd_sram_luts) from=0 to='clock_period'\n");
    /* Global power of Local Interconnections*/
    fprintf(fp, ".measure tran leakage_power_local_routing avg p(Vgvdd_local_interc) from=0 to='clock_period'\n");
  }
  /* Leakge power of Hard logic */
  fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_HARDLOGIC, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, spice, leakage_only);
  /* Leakage power of LUTs*/
  fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_LUT, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, spice, leakage_only);
  /* Leakage power of FFs*/
  fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_FF, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, spice, leakage_only);

  if (TRUE == leakage_only) {
    return;
  }

  /* Dynamic power */
  /* Dynamic power of SRAMs */
  fprintf(fp, ".measure tran dynamic_power_sram_local_routing avg p(Vgvdd_sram_local_routing) from='clock_period' to='%d*clock_period'\n", num_clock_cycle);
  fprintf(fp, ".measure tran total_energy_per_cycle_sram_local_routing param='dynamic_power_sram_local_routing*clock_period'\n");
  fprintf(fp, ".measure tran dynamic_power_sram_luts avg p(Vgvdd_sram_luts) from='clock_period' to='%d*clock_period'\n", num_clock_cycle);
  fprintf(fp, ".measure tran total_energy_per_cycle_sram_luts param='dynamic_power_sram_luts*clock_period'\n");
  /* Dynamic power of Local Interconnections */
  fprintf(fp, ".measure tran dynamic_power_local_interc avg p(Vgvdd_local_interc) from='clock_period' to='%d*clock_period'\n", num_clock_cycle);
  fprintf(fp, ".measure tran total_energy_per_cycle_local_routing param='dynamic_power_local_interc*clock_period'\n");
  /* Dynamic power of Hard Logic */
  fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_HARDLOGIC, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, spice, leakage_only);
  /* Dynamic power of LUTs */
  fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_LUT, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, spice, leakage_only);
  /* Dynamic power of FFs */
  fprint_measure_grid_vdds_spice_model(fp, grid_x, grid_y, SPICE_MODEL_FF, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, spice, leakage_only);

  return;
}

/* Top-level function in this source file */
int fprint_spice_one_grid_testbench(char* formatted_spice_dir,
                                    char* circuit_name,
                                    char* grid_test_bench_name,
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
  char* title = my_strcat("FPGA Grid Testbench for Design: ", circuit_name);
  char* grid_testbench_file_path = my_strcat(formatted_spice_dir, grid_test_bench_name);
  t_llist* temp = NULL;
  int used = 0;

  /* Check if the path exists*/
  fp = fopen(grid_testbench_file_path,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create Grid Testbench SPICE netlist %s!",__FILE__, __LINE__, grid_testbench_file_path); 
    exit(1);
  } 
  
 
  /* Print the title */
  fprint_spice_head(fp, title);
  my_free(title);

  /* print technology library and design parameters*/
  /* fprint_tech_lib(fp, arch.spice->tech_lib); */

  /* Include parameter header files */
  fprint_spice_include_param_headers(fp, include_dir_path);

  /* Include Key subckts */
  fprint_spice_include_key_subckts(fp, subckt_dir_path);

  /* Include user-defined sub-circuit netlist */
  init_include_user_defined_netlists(*(arch.spice));
  fprint_include_user_defined_netlists(fp, *(arch.spice));
  
  /* Special subckts for Top-level SPICE netlist */
  fprintf(fp, "****** Include subckt netlists: Look-Up Tables (LUTs) *****\n");
  spice_print_one_include_subckt_line(fp, formatted_subckt_dir_path, luts_spice_file_name);

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
  fprint_spice_grid_testbench_global_ports(fp, grid_x, grid_y, num_clock, (*arch.spice));
 
  /* Quote defined Logic blocks subckts (Grids) */
  init_spice_grid_testbench_globals(*(arch.spice));

  fprint_spice_grid_testbench_call_one_defined_grid(fp, grid_x, grid_y);

  /* Back-anotate activity information to each routing resource node 
   * (We should have activity of each Grid port) 
   */

  /* Add stimulations */
  max_sim_num_clock_cycles = get_grid_testbench_one_grid_num_sim_clock_cycles(fp, (*arch.spice), LL_rr_node_indices, grid_x, grid_y);
  fprint_spice_grid_testbench_stimulations(fp, num_clock, (*arch.spice), grid_x, grid_y,  LL_rr_node_indices);

  /* Add measurements */  
  fprint_spice_grid_testbench_measurements(fp, grid_x, grid_y, (*arch.spice), leakage_only);

  /* SPICE ends*/
  fprintf(fp, ".end\n");

  /* Close the file*/
  fclose(fp);

  if (0 < tb_num_grid) {
    /* 
    vpr_printf(TIO_MESSAGE_INFO, "Writing Grid[%d][%d] Testbench for %s...\n", grid_x, grid_y, circuit_name);
    */
    /* Push the testbench to the linked list */
    tb_head = add_one_spice_tb_info_to_llist(tb_head, grid_testbench_file_path, 
                                             max_sim_num_clock_cycles);
    used = 1;
  } else {
    my_remove_file(grid_testbench_file_path);
    used = 0;
  }

  return used;
}


/* Top-level function in this source file */
void spice_print_grid_testbench(char* formatted_spice_dir,
                                 char* circuit_name,
                                 char* include_dir_path,
                                 char* subckt_dir_path,
                                 t_ivec*** LL_rr_node_indices,
                                 int num_clock,
                                 t_arch arch,
                                 boolean leakage_only) {
  char* grid_testbench_name = NULL;
  char* temp_include_file_path = NULL;
  int ix, iy;
  int cnt = 0;
  int used;

  vpr_printf(TIO_MESSAGE_INFO,"Generating grid testbench...\n");

  for (ix = 1; ix < (nx+1); ix++) {
    for (iy = 1; iy < (ny+1); iy++) {
      /* Check if we include an existing subckt file! */
      temp_include_file_path = fpga_spice_create_one_subckt_filename(grid_spice_file_name_prefix, ix, iy, spice_netlist_file_postfix);
      if (FALSE == check_subckt_file_exist_in_llist(grid_spice_subckt_file_path_head, 
                                                    my_strcat(subckt_dir_path, temp_include_file_path))) {
        /* free */
        my_free(temp_include_file_path);
        continue;
      }
      /* Create a testbench for the existing subckt */
      grid_testbench_name = (char*)my_malloc(sizeof(char)*( strlen(circuit_name) 
                                            + 6 + strlen(my_itoa(ix)) + 1
                                            + strlen(my_itoa(iy)) + 1
                                            + strlen(spice_grid_testbench_postfix)  + 1 ));
      sprintf(grid_testbench_name, "%s_grid%d_%d%s",
              circuit_name, ix, iy, spice_grid_testbench_postfix);
      used = fprint_spice_one_grid_testbench(formatted_spice_dir, circuit_name, grid_testbench_name, 
                                             include_dir_path, subckt_dir_path, LL_rr_node_indices,
                                             num_clock, arch, ix, iy, 
                                             leakage_only);
      if (1 == used) {
        cnt += used;
      }
      /* free */
      my_free(grid_testbench_name);
      my_free(temp_include_file_path);
    }  
  } 
  /* Update the global counter */
  num_used_grid_tb = cnt;
  vpr_printf(TIO_MESSAGE_INFO,"No. of generated grid testbench = %d\n", num_used_grid_tb);

  return;
}
