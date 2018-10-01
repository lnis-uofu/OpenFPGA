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

/* Include spice support headers*/
#include "linkedlist.h"
#include "spice_globals.h"
#include "spice_utils.h"
#include "spice_mux.h"
#include "spice_pbtypes.h"
#include "spice_routing.h"
#include "spice_subckt.h"
#include "spice_netlist_utils.h"
#include "spice_top_netlist.h"

/* Local Subroutines declaration */
static 
void fprint_top_netlist_global_ports(FILE* fp,
                                     int num_clock,
                                     t_spice spice);

static 
void fprint_measure_vdds_cbs(FILE* fp,
                             enum e_measure_type meas_type,
                             int num_clock_cycle,
                             boolean leakage_only);

static 
void fprint_measure_vdds_sbs(FILE* fp,
                             enum e_measure_type meas_type,
                             int num_clock_cycle,
                             boolean leakage_only);

static 
void fprint_top_netlist_stimulations(FILE* fp,
                                     int num_clock,
                                     t_spice spice);

static 
void fprint_top_netlist_measurements(FILE* fp, 
                                     t_spice spice,
                                     boolean leakage_only);


/******** Subroutines ***********/
static 
void fprint_top_netlist_global_ports(FILE* fp,
                                     int num_clock,
                                     t_spice spice) {

  /* A valid file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Global nodes: Vdd for SRAMs, Logic Blocks(Include IO), Switch Boxes, Connection Boxes */
  fprintf(fp, ".global gvdd gset greset\n");
  fprintf(fp, ".global gvdd_local_interc gvdd_io gvdd_hardlogic\n");
  fprintf(fp, ".global gvdd_sram_local_routing gvdd_sram_luts gvdd_sram_cbs gvdd_sram_sbs\n");
  fprintf(fp, ".global %s->in\n", sram_spice_model->prefix);
  /* Define a global clock port if we need one*/
  fprintf(fp, "***** Global Clock Signals *****\n");
  fprintf(fp, ".global gclock\n");
  /*Global Vdds for LUTs*/
  fprint_global_vdds_spice_model(fp, SPICE_MODEL_LUT, spice);
  /*Global Vdds for FFs*/
  fprint_global_vdds_spice_model(fp, SPICE_MODEL_FF, spice);
  /*Global Vdds for Hardlogics*/
  /* fprint_global_vdds_spice_model(fp, SPICE_MODEL_HARDLOGIC, spice);*/
  /* Global Vdds for Switch Boxes */
  fprint_spice_global_vdd_switch_boxes(fp);

  /* Global Vdds for Connection Blocks */
  fprint_spice_global_vdd_connection_boxes(fp);

  /*Global ports for INPUTs of I/O PADS, SRAMs */
  fprint_global_pad_ports_spice_model(fp, spice);


  return; 
}

/* Print Stimulations for top-level netlist 
 * Task list:
 * 1. For global ggnd: connect to 0(gnd)
 * 2. For global vdd ports: connect to nominal voltage source
 * 3. For clock signal, we should create voltage waveforms
 * 4. For Set/Reset, TODO: should we reset the chip in the first cycle ???
 * 5. For input/output clb nets (mapped to I/O grids), we should create voltage waveforms
 */
static 
void fprint_top_netlist_stimulations(FILE* fp,
                                     int num_clock,
                                     t_spice spice) {
  int inet, iblock, inpad_idx;
  int ix, iy;
  int found_mapped_inpad = 0;
  /* Find Input Pad Spice model */
  t_spice_model* inpad_spice_model = find_inpad_spice_model(spice.num_spice_model, spice.spice_models);
  t_spice_net_info* cur_spice_net_info = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Global GND */
  fprintf(fp, "***** Global VDD port *****\n");
  fprintf(fp, "Vgvdd gvdd 0 0\n");
  fprintf(fp, "***** Global GND port *****\n");
  fprintf(fp, "*Rggnd ggnd 0 0\n");

  /* Global set and reset */
  fprintf(fp, "***** Global Net for reset signal *****\n");
  fprintf(fp, "Vgvreset greset 0 0\n");
  fprintf(fp, "***** Global Net for set signal *****\n");
  fprintf(fp, "Vgvset gset 0 0\n");

  /* Global Vdd ports */
  fprintf(fp, "***** Global VDD for I/O pads *****\n");
  fprintf(fp, "Vgvdd_io gvdd_io 0 vsp\n");
  fprintf(fp, "***** Global VDD for Local Interconnection *****\n");
  fprintf(fp, "Vgvdd_local_interc gvdd_local_interc 0 vsp\n");
  fprintf(fp, "***** Global VDD for local routing SRAMs *****\n");
  fprintf(fp, "Vgvdd_sram_local_routing gvdd_sram_local_routing 0 vsp\n");
  fprintf(fp, "***** Global VDD for LUTs SRAMs *****\n");
  fprintf(fp, "Vgvdd_sram_luts gvdd_sram_luts 0 vsp\n");
  fprintf(fp, "***** Global VDD for Connection Boxes SRAMs *****\n");
  fprintf(fp, "Vgvdd_sram_cbs gvdd_sram_cbs 0 vsp\n");
  fprintf(fp, "***** Global VDD for Switch Boxes SRAMs *****\n");
  fprintf(fp, "Vgvdd_sram_sbs gvdd_sram_sbs 0 vsp\n");

  /* Every Hardlogic use an independent Voltage source */
  fprintf(fp, "***** Global VDD for Hard Logics *****\n");
  fprint_splited_vdds_spice_model(fp, SPICE_MODEL_HARDLOGIC, spice);

  /* Every LUT use an independent Voltage source */
  fprintf(fp, "***** Global VDD for Look-Up Tables (LUTs) *****\n");
  fprint_splited_vdds_spice_model(fp, SPICE_MODEL_LUT, spice);

  /* Every FF use an independent Voltage source */
  fprintf(fp, "***** Global VDD for Flip-flops (FFs) *****\n");
  fprint_splited_vdds_spice_model(fp, SPICE_MODEL_FF, spice);

  /* Every SRAM inputs should have a voltage source */
  fprintf(fp, "***** Global Inputs for SRAMs *****\n");
  /*
  for (i = 0; i < sram_spice_model->cnt; i++) {
    fprintf(fp, "V%s[%d]->in %s[%d]->in 0 0\n", 
            sram_spice_model->prefix, i, sram_spice_model->prefix, i);
  }
  */
  fprintf(fp, "V%s->in %s->in 0 0\n", 
          sram_spice_model->prefix, sram_spice_model->prefix);
  fprintf(fp, ".nodeset V(%s->in) 0\n", sram_spice_model->prefix);
  
  /* Every Switch Box (SB) use an independent Voltage source */
  fprintf(fp, "***** Global VDD for Switch Boxes(SBs) *****\n");
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 0; iy < (ny + 1); iy++) {
      fprintf(fp, "Vgvdd_sb[%d][%d] gvdd_sb[%d][%d] 0 vsp\n", ix, iy, ix, iy);
    }
  }

  /* Every Connection Box (CB) use an independent Voltage source */
  fprintf(fp, "***** Global VDD for Connection Boxes(CBs) *****\n");
  /* cbx */
  /* X - channels [1...nx][0..ny]*/
  for (iy = 0; iy < (ny + 1); iy++) {
    for (ix = 1; ix < (nx + 1); ix++) {
      fprintf(fp, "Vgvdd_cbx[%d][%d] gvdd_cbx[%d][%d] 0 vsp\n", ix, iy, ix, iy);
    }
  }

  /* cby */
  /* Y - channels [1...ny][0..nx]*/
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      fprintf(fp, "Vgvdd_cby[%d][%d] gvdd_cby[%d][%d] 0 vsp\n", ix, iy, ix, iy);
    }
  }

  /* Global clock if we need one */
  if (1 == num_clock) {
    /* First cycle reserved for measuring leakage */
    fprintf(fp, "***** Global Clock signal *****\n");
    fprintf(fp, "***** pulse(vlow vhigh tdelay trise tfall pulse_width period *****\n");
    fprintf(fp, "Vgclock gclock 0 pulse(0 vsp 'clock_period'\n");
    fprintf(fp, "+                      'clock_slew_pct_rise*clock_period' 'clock_slew_pct_fall*clock_period'\n");
    fprintf(fp, "+                      '0.5*(1-clock_slew_pct_rise-clock_slew_pct_fall)*clock_period' 'clock_period')\n");
  } else {
    assert(0 == num_clock);
  }
  
  /* For each input_signal
   * TODO: this part is low-efficent for run-time concern... Need improve
   */
  assert(NULL != inpad_spice_model);
  for (inpad_idx = 0; inpad_idx < inpad_spice_model->cnt; inpad_idx++) {
    /* Find if this inpad is mapped to a logical block */
    found_mapped_inpad = 0;
    for (iblock = 0; iblock < num_logical_blocks; iblock++) {
      if ((inpad_spice_model == logical_block[iblock].mapped_spice_model)
         &&(inpad_idx == logical_block[iblock].mapped_spice_model_index)) {
        /* Make sure We find the correct logical block !*/
        assert(VPACK_INPAD == logical_block[iblock].type);
        cur_spice_net_info = NULL;
        for (inet = 0; inet < num_nets; inet++) { 
          if (0 == strcmp(clb_net[inet].name, logical_block[iblock].name)) {
            cur_spice_net_info = clb_net[inet].spice_net_info;
            break;
          }
        }
        assert(NULL != cur_spice_net_info);
        assert(!(0 > cur_spice_net_info->density));
        assert(!(1 < cur_spice_net_info->density));
        assert(!(0 > cur_spice_net_info->probability));
        assert(!(1 < cur_spice_net_info->probability));
        /* Get the net information */
        /* First cycle reserved for measuring leakage */
        fprintf(fp, "Vgfpga_input[%d]_%s[%d] gfpga_input[%d]_%s[%d] 0 \n", 
                inpad_idx, inpad_spice_model->prefix, inpad_idx,
                inpad_idx, inpad_spice_model->prefix, inpad_idx);
        fprint_voltage_pulse_params(fp, cur_spice_net_info->init_val, cur_spice_net_info->density, cur_spice_net_info->probability);
        found_mapped_inpad++;
      }
    } 
    assert((0 == found_mapped_inpad)||(1 == found_mapped_inpad));
    /* if we cannot find any mapped inpad from tech.-mapped netlist, give a default */
    if (0 == found_mapped_inpad) {
      fprintf(fp, "Vgfpga_input[%d]_%s[%d] gfpga_input[%d]_%s[%d] 0 ",
              inpad_idx, inpad_spice_model->prefix, inpad_idx,
              inpad_idx, inpad_spice_model->prefix, inpad_idx);
      switch (default_signal_init_value) {
      case 0:
        fprintf(fp, "0\n");
        break;
      case 1:
        fprintf(fp, "vsp\n");
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid default_signal_init_value (=%d)!\n",
                   __FILE__, __LINE__, default_signal_init_value);
        exit(1);
      }
    }
  }

  /* Give the floating ports a GND */
  /* I found there is no difference in performance if we do not fix these floating ports */
  /* fprint_grid_float_port_stimulation(fp); */
  
  return;
}

static 
void fprint_measure_vdds_cbs(FILE* fp,
                             enum e_measure_type meas_type,
                             int num_clock_cycle,
                             boolean leakage_only) {
  int ix, iy, prev_ix, prev_iy; 

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  switch (meas_type) {
  case SPICE_MEASURE_LEAKAGE_POWER:
    /* Leakage power of CBs*/
    fprintf(fp, "***** Measure Leakage Power for Connection Boxes(CBs) *****\n");
    /* cbx */
    /* X - channels [1...nx][0..ny]*/
    for (iy = 0; iy < (ny + 1); iy++) {
      for (ix = 1; ix < (nx + 1); ix++) {
        if (TRUE == leakage_only) {
          fprintf(fp, ".measure tran leakage_power_cbx[%d][%d] find p(Vgvdd_cbx[%d][%d]) at=0\n",
                  ix, iy, ix, iy);
        } else {
          fprintf(fp, ".measure tran leakage_power_cbx[%d][%d] avg p(Vgvdd_cbx[%d][%d]) from=0 to='clock_period'\n",
                  ix, iy, ix, iy);
        }
      }
    }
    /* cby */
    /* Y - channels [1...ny][0..nx]*/
    for (ix = 0; ix < (nx + 1); ix++) {
      for (iy = 1; iy < (ny + 1); iy++) {
        if (TRUE == leakage_only) {
          fprintf(fp, ".measure tran leakage_power_cby[%d][%d] find p(Vgvdd_cby[%d][%d]) at=0\n",
                  ix, iy, ix, iy);
        } else {
          fprintf(fp, ".measure tran leakage_power_cby[%d][%d] avg p(Vgvdd_cby[%d][%d]) from=0 to='clock_period'\n",
                  ix, iy, ix, iy);
        }
      }
    }
    /* Measure Total Leakage Power of CBs */
    fprintf(fp, "***** Measure Total Leakage Power for Connection Boxes(CBs) *****\n");
    /* X - channels [1...nx][0..ny]*/
    for (iy = 0; iy < (ny + 1); iy++) {
      for (ix = 1; ix < (nx + 1); ix++) {
        fprintf(fp, ".measure tran leakage_power_cbx[1to%d][0to%d] \n", ix, iy);
        if ((1 == ix)&&(0 == iy)) {
          fprintf(fp, "+ param='leakage_power_cbx[%d][%d]'\n", ix, iy);
        } else {
          fprintf(fp, "+ param='leakage_power_cbx[%d][%d]+leakage_power_cbx[1to%d][0to%d]'\n",
                  ix, iy, prev_ix, prev_iy); 
        }
        prev_ix = ix;
        prev_iy = iy;
      }
    }
    /* Y - channels [1...ny][0..nx]*/
    for (ix = 0; ix < (nx + 1); ix++) {
      for (iy = 1; iy < (ny + 1); iy++) {
        fprintf(fp, ".measure tran leakage_power_cby[0to%d][1to%d] \n", ix, iy);
        if ((0 == ix)&&(1 == iy)) {
          fprintf(fp, "+ param='leakage_power_cby[%d][%d]'\n", ix, iy);
        } else {
          fprintf(fp, "+ param='leakage_power_cby[%d][%d]+leakage_power_cby[0to%d][1to%d]'\n",
                  ix, iy, prev_ix, prev_iy); 
        }
        prev_ix = ix;
        prev_iy = iy;
      }
    }
    /* Sum up the leakage power of cbx and cby*/
    fprintf(fp, ".measure tran leakage_power_cbs \n");
    fprintf(fp, "+ param='leakage_power_cbx[1to%d][0to%d]+leakage_power_cby[0to%d][1to%d]' \n",
            nx, ny, nx, ny);
    break;
  case SPICE_MEASURE_DYNAMIC_POWER:
    /* Dynamic power of CBs */
    fprintf(fp, "***** Measure Dynamic Power for Connection Boxes(CBs) *****\n");
    /* cbx */
    /* X - channels [1...nx][0..ny]*/
    for (iy = 0; iy < (ny + 1); iy++) {
      for (ix = 1; ix < (nx + 1); ix++) {
        fprintf(fp, ".measure tran dynamic_power_cbx[%d][%d] avg p(Vgvdd_cbx[%d][%d]) from='clock_period' to='%d*clock_period'\n",
                ix, iy, ix, iy,  num_clock_cycle);
      }
    }
    /* cby */
    /* Y - channels [1...ny][0..nx]*/
    for (ix = 0; ix < (nx + 1); ix++) {
      for (iy = 1; iy < (ny + 1); iy++) {
        fprintf(fp, ".measure tran dynamic_power_cby[%d][%d] avg p(Vgvdd_cby[%d][%d]) from='clock_period' to='%d*clock_period'\n",
                ix, iy, ix, iy,  num_clock_cycle);
      }
    }
    /* Measure Dynamic Power of CBs */
    fprintf(fp, "***** Measure Total Dynamic Power for Connection Boxes(CBs) *****\n");
    /* X - channels [1...nx][0..ny]*/
    for (iy = 0; iy < (ny + 1); iy++) {
      for (ix = 1; ix < (nx + 1); ix++) {
        fprintf(fp, ".measure tran dynamic_power_cbx[1to%d][0to%d] \n", ix, iy);
        if ((1 == ix)&&(0 == iy)) {
          fprintf(fp, "+ param='dynamic_power_cbx[%d][%d]'\n", ix, iy);
        } else {
          fprintf(fp, "+ param='dynamic_power_cbx[%d][%d]+dynamic_power_cbx[1to%d][0to%d]'\n",
                  ix, iy, prev_ix, prev_iy); 
        }
        prev_ix = ix;
        prev_iy = iy;
      }
    }
    /* Y - channels [1...ny][0..nx]*/
    for (ix = 0; ix < (nx + 1); ix++) {
      for (iy = 1; iy < (ny + 1); iy++) {
        fprintf(fp, ".measure tran dynamic_power_cby[0to%d][1to%d] \n", ix, iy);
        if ((0 == ix)&&(1 == iy)) {
          fprintf(fp, "+ param='dynamic_power_cby[%d][%d]'\n", ix, iy);
        } else {
          fprintf(fp, "+ param='dynamic_power_cby[%d][%d]+dynamic_power_cby[0to%d][1to%d]'\n",
                  ix, iy, prev_ix, prev_iy); 
        }
        prev_ix = ix;
        prev_iy = iy;
      }
    }
    /* Sum up the dynamic power of cbx and cby*/
    fprintf(fp, ".measure tran dynamic_power_cbs \n");
    fprintf(fp, "+ param='dynamic_power_cbx[1to%d][0to%d]+dynamic_power_cby[0to%d][1to%d]' \n",
            nx, ny, nx, ny);
    fprintf(fp, ".measure tran energy_per_cycle_cbs \n ");
    fprintf(fp, "+ param='dynamic_power_cbs*clock_period'\n");
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(FILE:%s,[LINE%d])Invalid Measure Type! Should be [SPICE_MEASURE_LEAKGE_POWER|SPICE_MEASURE_DYNAMIC_POWER]\n",
               __FILE__, __LINE__);
    exit(1); 
  }

  return;
}

static 
void fprint_measure_vdds_sbs(FILE* fp,
                             enum e_measure_type meas_type,
                             int num_clock_cycle,
                             boolean leakage_only) {
  int ix, iy, prev_ix, prev_iy; 

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  switch (meas_type) {
  case SPICE_MEASURE_LEAKAGE_POWER:
    /* Leakage power of SBs*/
    fprintf(fp, "***** Measure Leakage Power for Switch Boxes(SBs) *****\n");
    for (ix = 0; ix < (nx + 1); ix++) {
      for (iy = 0; iy < (ny + 1); iy++) {
        if (TRUE == leakage_only) {
          fprintf(fp, ".measure tran leakage_power_sb[%d][%d] find p(Vgvdd_sb[%d][%d]) at=0\n",
                  ix, iy, ix, iy);
        } else {
          fprintf(fp, ".measure tran leakage_power_sb[%d][%d] avg p(Vgvdd_sb[%d][%d]) from=0 to='clock_period'\n",
                  ix, iy, ix, iy);
        }
      }
    }
    /* Measure Total Leakage Power of SBs */
    fprintf(fp, "***** Measure Total Leakage Power for Switch Boxes(SBs) *****\n");
    for (ix = 0; ix < (nx + 1); ix++) {
      for (iy = 0; iy < (ny + 1); iy++) {
        fprintf(fp, ".measure tran leakage_power_sb[0to%d][0to%d] \n", ix, iy);
        if ((0 == ix)&&(0 == iy)) {
          fprintf(fp, "+ param='leakage_power_sb[%d][%d]'\n", ix, iy);
        } else {
          fprintf(fp, "+ param='leakage_power_sb[%d][%d]+leakage_power_sb[0to%d][0to%d]'\n",
                  ix, iy, prev_ix, prev_iy); 
        }
        prev_ix = ix;
        prev_iy = iy;
      }
    }
    /* Sum up the leakage power of sbs*/
    fprintf(fp, ".measure tran leakage_power_sbs \n");
    fprintf(fp, "+ param='leakage_power_sb[0to%d][0to%d]' \n",
            nx, ny);
    break;
  case SPICE_MEASURE_DYNAMIC_POWER:
    /* Dynamic power of SBs */
    fprintf(fp, "***** Measure Dynamic Power for Switch Boxes(SBs) *****\n");
    for (ix = 0; ix < (nx + 1); ix++) {
      for (iy = 0; iy < (ny + 1); iy++) {
        fprintf(fp, ".measure tran dynamic_power_sb[%d][%d] avg p(Vgvdd_sb[%d][%d]) from='clock_period' to='%d*clock_period'\n",
                ix, iy, ix, iy,  num_clock_cycle);
      }
    }
    /* Measure Total Dynamic Power of SBs */
    fprintf(fp, "***** Measure Total Dynamic Power for Switch Boxes(SBs) *****\n");
    for (ix = 0; ix < (nx + 1); ix++) {
      for (iy = 0; iy < (ny + 1); iy++) {
        fprintf(fp, ".measure tran dynamic_power_sb[0to%d][0to%d] \n", ix, iy);
        if ((0 == ix)&&(0 == iy)) {
          fprintf(fp, "+ param='dynamic_power_sb[%d][%d]'\n", ix, iy);
        } else {
          fprintf(fp, "+ param='dynamic_power_sb[%d][%d]+dynamic_power_sb[0to%d][0to%d]'\n",
                  ix, iy, prev_ix, prev_iy); 
        }
        prev_ix = ix;
        prev_iy = iy;
      }
    }
    /* Sum up the dynamic power of sbs*/
    fprintf(fp, ".measure tran dynamic_power_sbs \n");
    fprintf(fp, "+ param='dynamic_power_sb[0to%d][0to%d]' \n",
            nx, ny);
    fprintf(fp, ".measure tran energy_per_cycle_sbs \n ");
    fprintf(fp, "+ param='dynamic_power_sbs*clock_period'\n");
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
              "(FILE:%s,[LINE%d])Invalid Measure Type! Should be [SPICE_MEASURE_LEAKGE_POWER|SPICE_MEASURE_DYNAMIC_POWER]\n",
               __FILE__, __LINE__);
    exit(1); 
  }

  return;
}

static 
void fprint_top_netlist_measurements(FILE* fp, 
                                     t_spice spice,
                                     boolean leakage_only) {
  /* First cycle reserved for measuring leakage */
  int num_clock_cycle = spice.spice_params.meas_params.sim_num_clock_cycle + 1;
  
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  
  fprint_spice_netlist_transient_setting(fp, spice, num_clock_cycle, leakage_only);

  /* TODO: Measure the delay of each mapped net and logical block */

  /* Measure the power */
  /* Leakage ( the first cycle is reserved for leakage measurement) */
  if (TRUE == leakage_only) {
    /* Leakage power of SRAMs */
    fprintf(fp, ".measure tran leakage_power_sram_local_routing find p(Vgvdd_sram_local_routing) at=0\n");
    fprintf(fp, ".measure tran leakage_power_sram_luts find p(Vgvdd_sram_luts) at=0\n");
    fprintf(fp, ".measure tran leakage_power_sram_cbs find p(Vgvdd_sram_cbs) at=0\n");
    fprintf(fp, ".measure tran leakage_power_sram_sbs find p(Vgvdd_sram_sbs) at=0\n");
    /* Leakage power of I/O Pads */
    fprintf(fp, ".measure tran leakage_power_io find p(Vgvdd_io) at=0\n");
    /* Global power of Local Interconnections*/
    fprintf(fp, ".measure tran leakage_power_local_interc find p(Vgvdd_local_interc) at=0\n");
  } else {
    /* Leakage power of SRAMs */
    fprintf(fp, ".measure tran leakage_power_sram_local_routing avg p(Vgvdd_sram_local_routing) from=0 to='clock_period'\n");
    fprintf(fp, ".measure tran leakage_power_sram_luts avg p(Vgvdd_sram_luts) from=0 to='clock_period'\n");
    fprintf(fp, ".measure tran leakage_power_sram_cbs avg p(Vgvdd_sram_cbs) from=0 to='clock_period'\n");
    fprintf(fp, ".measure tran leakage_power_sram_sbs avg p(Vgvdd_sram_sbs) from=0 to='clock_period'\n");
    /* Leakage power of I/O Pads */
    fprintf(fp, ".measure tran leakage_power_io avg p(Vgvdd_io) from=0 to='clock_period'\n");
    /* Global power of Local Interconnections*/
    fprintf(fp, ".measure tran leakage_power_local_interc avg p(Vgvdd_local_interc) from=0 to='clock_period'\n");
  }
  /* Leakge power of Hard logic */
  fprint_measure_vdds_spice_model(fp, SPICE_MODEL_HARDLOGIC, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, spice, leakage_only);
  /* Leakage power of LUTs*/
  fprint_measure_vdds_spice_model(fp, SPICE_MODEL_LUT, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, spice, leakage_only);
  /* Leakage power of FFs*/
  fprint_measure_vdds_spice_model(fp, SPICE_MODEL_FF, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, spice, leakage_only);
  /* Leakage power of CBs */
  fprint_measure_vdds_cbs(fp, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, leakage_only);
  /* Leakage power of SBs */
  fprint_measure_vdds_sbs(fp, SPICE_MEASURE_LEAKAGE_POWER, num_clock_cycle, leakage_only);

  if (TRUE == leakage_only) {
    return;
  }

  /* Dynamic power */
  /* Dynamic power of SRAMs */
  fprintf(fp, ".measure tran dynamic_power_sram_local_routing avg p(Vgvdd_sram_local_routing) from='clock_period' to='%d*clock_period'\n", num_clock_cycle);
  fprintf(fp, ".measure tran energy_per_cycle_sram_local_routing \n ");
  fprintf(fp, "+ param='dynamic_power_sram_local_routing*clock_period'\n");
  fprintf(fp, ".measure tran dynamic_power_sram_luts avg p(Vgvdd_sram_luts) from='clock_period' to='%d*clock_period'\n", num_clock_cycle);
  fprintf(fp, ".measure tran energy_per_cycle_sram_luts \n ");
  fprintf(fp, "+ param='dynamic_power_sram_luts*clock_period'\n");
  fprintf(fp, ".measure tran dynamic_power_sram_cbs avg p(Vgvdd_sram_cbs) from='clock_period' to='%d*clock_period'\n", num_clock_cycle);
  fprintf(fp, ".measure tran energy_per_cycle_sram_cbs \n ");
  fprintf(fp, "+ param='dynamic_power_sram_cbs*clock_period'\n");
  fprintf(fp, ".measure tran dynamic_power_sram_sbs avg p(Vgvdd_sram_sbs) from='clock_period' to='%d*clock_period'\n", num_clock_cycle);
  fprintf(fp, ".measure tran energy_per_cycle_sram_sbs \n ");
  fprintf(fp, "+ param='dynamic_power_sram_sbs*clock_period'\n");
  /* Dynamic power of I/O pads */
  fprintf(fp, ".measure tran dynamic_power_io avg p(Vgvdd_io) from='clock_period' to='%d*clock_period'\n", num_clock_cycle);
  fprintf(fp, ".measure tran energy_per_cycle_io \n ");
  fprintf(fp, "+ param='dynamic_power_io*clock_period'\n");
  /* Dynamic power of Local Interconnections */
  fprintf(fp, ".measure tran dynamic_power_local_interc avg p(Vgvdd_local_interc) from='clock_period' to='%d*clock_period'\n", num_clock_cycle);
  fprintf(fp, ".measure tran energy_per_cycle_local_routing \n ");
  fprintf(fp, "+ param='dynamic_power_local_interc*clock_period'\n");
  /* Dynamic power of Hard Logic */
  fprint_measure_vdds_spice_model(fp, SPICE_MODEL_HARDLOGIC, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, spice, leakage_only);
  /* Dynamic power of LUTs */
  fprint_measure_vdds_spice_model(fp, SPICE_MODEL_LUT, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, spice, leakage_only);
  /* Dynamic power of FFs */
  fprint_measure_vdds_spice_model(fp, SPICE_MODEL_FF, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, spice, leakage_only);
  /* Dynamic power of CBs */
  fprint_measure_vdds_cbs(fp, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, leakage_only);
  /* Dynamic power of SBs */
  fprint_measure_vdds_sbs(fp, SPICE_MEASURE_DYNAMIC_POWER, num_clock_cycle, leakage_only);
  
  return;
}

/***** Print Top-level SPICE netlist *****/
void fprint_spice_top_netlist(char* circuit_name,
                              char* top_netlist_name,
                              char* include_dir_path,
                              char* subckt_dir_path,
                              t_ivec*** LL_rr_node_indices,
                              int num_clock,
                              t_spice spice,
                              boolean leakage_only) {
  FILE* fp = NULL;
  char* formatted_subckt_dir_path = format_dir_path(subckt_dir_path);
  char* temp_include_file_path = NULL;
  char* title = my_strcat("FPGA SPICE Netlist for Design: ", circuit_name);
  t_llist* temp = NULL;

  /* Check if the path exists*/
  fp = fopen(top_netlist_name,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create top SPICE netlist %s!",__FILE__, __LINE__, top_netlist_name); 
    exit(1);
  } 
  
  vpr_printf(TIO_MESSAGE_INFO, "Writing Top-level FPGA Netlist for %s...\n", circuit_name);
 
  /* Print the title */
  fprint_spice_head(fp, title);
  my_free(title);

  /* print technology library and design parameters*/
  fprint_tech_lib(fp, spice.tech_lib);

  /* Include parameter header files */
  fprint_spice_include_param_headers(fp, include_dir_path);

  /* Include Key subckts */
  fprint_spice_include_key_subckts(fp, subckt_dir_path);

  /* Include user-defined sub-circuit netlist */
  init_include_user_defined_netlists(spice);
  fprint_include_user_defined_netlists(fp, spice);
  
  /* Special subckts for Top-level SPICE netlist */
  fprintf(fp, "****** Include subckt netlists: Look-Up Tables (LUTs) *****\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, luts_spice_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "****** Include subckt netlists: Logic Blocks *****\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, logic_block_spice_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "****** Include subckt netlists: Routing structures (Switch Boxes, Channels, Connection Boxes) *****\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, routing_spice_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);
 
  /* Print all global wires*/
  fprint_top_netlist_global_ports(fp, num_clock, spice);
 
  /* Print simulation temperature and other options for SPICE */
  fprint_spice_options(fp, spice.spice_params);

  /* Quote defined Logic blocks subckts (Grids) */
  fprint_call_defined_grids(fp);
  fprint_stimulate_dangling_grid_pins(fp); 

  /* Quote Routing structures: Channels */
  fprint_call_defined_channels(fp);

  /* Quote Routing structures: Conneciton Boxes */
  fprint_call_defined_connection_boxes(fp, LL_rr_node_indices);
  
  /* Quote Routing structures: Switch Boxes */
  fprint_call_defined_switch_boxes(fp); 

  /* Add stimulations */
  fprint_top_netlist_stimulations(fp, num_clock, spice);

  /* Add measurements */  
  fprint_top_netlist_measurements(fp, spice, leakage_only);

  /* SPICE ends*/
  fprintf(fp, ".end\n");

  /* Close the file*/
  fclose(fp);

  /* Free */
  //my_free(title);
  //my_free(formatted_subckt_dir_path);
  if (NULL == tb_head) {
    tb_head = create_llist(1);
    tb_head->dptr = my_malloc(sizeof(t_spicetb_info));
    ((t_spicetb_info*)(tb_head->dptr))->tb_name = my_strdup(top_netlist_name);
    ((t_spicetb_info*)(tb_head->dptr))->num_sim_clock_cycles = spice.spice_params.meas_params.sim_num_clock_cycle + 1;
  } else {
    temp = insert_llist_node(tb_head);
    temp->dptr = my_malloc(sizeof(t_spicetb_info));
    ((t_spicetb_info*)(temp->dptr))->tb_name = my_strdup(top_netlist_name);
    ((t_spicetb_info*)(temp->dptr))->num_sim_clock_cycles = spice.spice_params.meas_params.sim_num_clock_cycle + 1;
  }

  return;
}

