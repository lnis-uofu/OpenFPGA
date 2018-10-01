/***********************************/
/*      SPICE Modeling for VPR     */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
#include <stdio.h>
#include <stdlib.h>
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
#include "spice_globals.h"
#include "spice_utils.h"
#include "spice_mux.h"
#include "spice_pbtypes.h"
#include "spice_routing.h"
#include "spice_netlist_utils.h"

/***** Subroutines *****/
void init_include_user_defined_netlists(t_spice spice) {
  int i;

  /* Include user-defined sub-circuit netlist */
  for (i = 0; i < spice.num_include_netlist; i++) {
    spice.include_netlists[i].included = 0;
  }

  return;
}

void fprint_include_user_defined_netlists(FILE* fp,
                                          t_spice spice) {
  int i;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Include user-defined sub-circuit netlist */
  for (i = 0; i < spice.num_include_netlist; i++) {
    if (0 == spice.include_netlists[i].included) {
      assert(NULL != spice.include_netlists[i].path);
      fprintf(fp, ".include \'%s\'\n", spice.include_netlists[i].path);
      spice.include_netlists[i].included = 1;
    } else {
      assert(1 == spice.include_netlists[i].included);
    }
  } 

  return;
}

void fprint_splited_vdds_spice_model(FILE* fp,
                                     enum e_spice_model_type spice_model_type,
                                     t_spice spice) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
        fprintf(fp, "Vgvdd_%s[%d] gvdd_%s[%d] 0 vsp\n", 
                spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
        /* For some gvdd maybe floating, I add a huge resistance to make their leakage power trival 
         * which does no change to the delay result.
         * The resistance value is co-related to the vsp, which produces a trival leakage current (1e-15).
         */
        fprintf(fp, "Rgvdd_%s[%d]_huge gvdd_%s[%d] 0 'vsp/10e-15'\n", 
                spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
      }
    }
  }
  
  return;
}

void fprint_grid_splited_vdds_spice_model(FILE* fp, int grid_x, int grid_y,
                                          enum e_spice_model_type spice_model_type,
                                          t_spice spice) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      /* Bypass zero-usage spice_model in this grid*/
      if (spice.spice_models[imodel].grid_index_low[grid_x][grid_y]
          == spice.spice_models[imodel].grid_index_high[grid_x][grid_y]) {
        continue;
      }
      for (i = spice.spice_models[imodel].grid_index_low[grid_x][grid_y]; 
           i < spice.spice_models[imodel].grid_index_high[grid_x][grid_y]; 
           i++) {
        fprintf(fp, "Vgvdd_%s[%d] gvdd_%s[%d] 0 vsp\n", 
                spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
        /* For some gvdd maybe floating, I add a huge resistance to make their leakage power trival 
         * which does no change to the delay result.
         * The resistance value is co-related to the vsp, which produces a trival leakage current (1e-15).
         */
        fprintf(fp, "Rgvdd_%s[%d]_huge gvdd_%s[%d] 0 'vsp/10e-15'\n", 
                spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
      }
    }
  }
 
  return;
}

void fprint_global_vdds_spice_model(FILE* fp, 
                                    enum e_spice_model_type spice_model_type,
                                    t_spice spice) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  fprintf(fp, "***** Global VDD ports of %s *****\n", generate_string_spice_model_type(spice_model_type));
  fprintf(fp, ".global \n");

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
        fprintf(fp, "+ gvdd_%s[%d]\n", 
                spice.spice_models[imodel].prefix, i);
      }
    }
  }
  
  fprintf(fp, "\n");

  return;
}

void fprint_grid_global_vdds_spice_model(FILE* fp, int x, int y, 
                                         enum e_spice_model_type spice_model_type,
                                         t_spice spice) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  fprintf(fp, "***** Global VDD ports of %s *****\n", generate_string_spice_model_type(spice_model_type));
  fprintf(fp, ".global \n");

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      /* Bypass zero-usage spice_model in this grid*/
      if (spice.spice_models[imodel].grid_index_low[x][y]
          == spice.spice_models[imodel].grid_index_high[x][y]) {
        continue;
      }
      for (i = spice.spice_models[imodel].grid_index_low[x][y]; 
           i < spice.spice_models[imodel].grid_index_high[x][y]; 
           i++) {
        fprintf(fp, "+ gvdd_%s[%d]\n", 
                spice.spice_models[imodel].prefix, i);
      }
    }
  }
  
  fprintf(fp, "\n");

  return;
}

void fprint_global_pad_ports_spice_model(FILE* fp, 
                                         t_spice spice) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  fprintf(fp, "***** Global input/output ports of I/O Pads *****\n");
  fprintf(fp, ".global \n");

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    switch (spice.spice_models[imodel].type) {
    /* Handle multiple INPAD/OUTPAD spice models*/
    case SPICE_MODEL_INPAD:
      for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
        fprintf(fp, "+ gfpga_input[%d]_%s[%d]\n", i, spice.spice_models[imodel].prefix, i);
      }
      break; 
    case SPICE_MODEL_OUTPAD:
      for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
        fprintf(fp, "+ gfpga_output[%d]_%s[%d]\n", i, spice.spice_models[imodel].prefix, i);
      }
      break;
    /* SRAM inputs*/
    case SPICE_MODEL_SRAM:
      /*
      for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
        fprintf(fp, "+ %s[%d]->in\n", spice.spice_models[imodel].prefix, i);
      }
      */
      fprintf(fp, "+ %s->in\n", spice.spice_models[imodel].prefix);
      break;
    /* Other types we do not care*/
    case SPICE_MODEL_CHAN_WIRE:
    case SPICE_MODEL_WIRE:
    case SPICE_MODEL_MUX:
    case SPICE_MODEL_LUT:
    case SPICE_MODEL_FF:
    case SPICE_MODEL_HARDLOGIC:
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Unknown type for spice model!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }

  fprintf(fp, "\n");
 
  return; 
}

void fprint_spice_global_vdd_switch_boxes(FILE* fp) {
  int ix, iy;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  fprintf(fp, "***** Global Vdds for Switch Boxes *****\n");
  fprintf(fp, ".global ");
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 0; iy < (ny + 1); iy++) {
      fprintf(fp, "gvdd_sb[%d][%d] ", ix, iy);
    }
  }
  fprintf(fp, "\n");

  return;
}

/* Call the sub-circuits for connection boxes */
void fprint_spice_global_vdd_connection_boxes(FILE* fp) {
  int ix, iy;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  fprintf(fp, "***** Global Vdds for Connection Blocks - X channels *****\n");
  fprintf(fp, ".global ");
  /* X - channels [1...nx][0..ny]*/
  for (iy = 0; iy < (ny + 1); iy++) {
    for (ix = 1; ix < (nx + 1); ix++) {
      fprintf(fp, "gvdd_cbx[%d][%d] ", ix, iy);
    }
  }
  fprintf(fp, "\n");

  fprintf(fp, "***** Global Vdds for Connection Blocks - Y channels *****\n");
  fprintf(fp, ".global ");
  /* Y - channels [1...ny][0..nx]*/
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      fprintf(fp, "gvdd_cby[%d][%d] ", ix, iy);
    }
  }
  fprintf(fp, "\n");
 
  return; 
}

void fprint_measure_vdds_spice_model(FILE* fp,
                                     enum e_spice_model_type spice_model_type,
                                     enum e_measure_type meas_type,
                                     int num_cycle,
                                     t_spice spice,
                                     boolean leakage_only) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
        switch (meas_type) {
        case SPICE_MEASURE_LEAKAGE_POWER:
          if (TRUE == leakage_only) {
            fprintf(fp, ".measure tran leakage_power_%s[%d] find p(Vgvdd_%s[%d]) at=0\n",
                    spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
          } else {
            fprintf(fp, ".measure tran leakage_power_%s[%d] avg p(Vgvdd_%s[%d]) from=0 to='clock_period'\n",
                    spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
          }
          break;
        case SPICE_MEASURE_DYNAMIC_POWER:
          fprintf(fp, ".measure tran dynamic_power_%s[%d] avg p(Vgvdd_%s[%d]) from='clock_period' to='%d*clock_period'\n",
                  spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i, num_cycle);
          break;
        default: 
          vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid meas_type!\n", __FILE__, __LINE__);
          exit(1);
        }
      }
    }
  }

  /* Measure the total power of this kind of spice model */
  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      switch (meas_type) {
      case SPICE_MEASURE_LEAKAGE_POWER:
        for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
          fprintf(fp, ".measure tran leakage_power_%s[0to%d] \n", spice.spice_models[imodel].prefix, i);
          if (0 == i) {
            fprintf(fp, "+ param = 'leakage_power_%s[%d]'\n", spice.spice_models[imodel].prefix, i);
          } else {
            fprintf(fp, "+ param = 'leakage_power_%s[%d]+leakage_power_%s[0to%d]'\n", 
                    spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i-1);
          }
        }
        /* Spot the total leakage power of this spice model */
        fprintf(fp, ".measure tran total_leakage_power_%s \n", spice.spice_models[imodel].prefix);
        fprintf(fp, "+ param = 'leakage_power_%s[0to%d]'\n", 
                spice.spice_models[imodel].prefix, spice.spice_models[imodel].cnt-1);
        break;
      case SPICE_MEASURE_DYNAMIC_POWER:
        for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
          fprintf(fp, ".measure tran dynamic_power_%s[0to%d] \n", spice.spice_models[imodel].prefix, i);
          if (0 == i) {
            fprintf(fp, "+ param = 'dynamic_power_%s[%d]'\n", spice.spice_models[imodel].prefix, i);
          } else {
            fprintf(fp, "+ param = 'dynamic_power_%s[%d]+dynamic_power_%s[0to%d]'\n", 
                    spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i-1);
          }
        }
        /* Spot the total dynamic power of this spice model */
        fprintf(fp, ".measure tran total_dynamic_power_%s \n", spice.spice_models[imodel].prefix);
        fprintf(fp, "+ param = 'dynamic_power_%s[0to%d]'\n", 
                spice.spice_models[imodel].prefix, spice.spice_models[imodel].cnt-1);
        fprintf(fp, ".measure tran total_energy_per_cycle_%s \n", spice.spice_models[imodel].prefix);
        fprintf(fp, "+ param = 'dynamic_power_%s[0to%d]*clock_period'\n", 
                spice.spice_models[imodel].prefix, spice.spice_models[imodel].cnt-1);
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid meas_type!\n", __FILE__, __LINE__);
        exit(1);
      }
    }
  }
  
  
  return;
}

void fprint_measure_grid_vdds_spice_model(FILE* fp, int grid_x, int grid_y,
                                          enum e_spice_model_type spice_model_type,
                                          enum e_measure_type meas_type,
                                          int num_cycle,
                                          t_spice spice,
                                          boolean leakage_only) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      /* Bypass zero-usage spice_model in this grid*/
      if (spice.spice_models[imodel].grid_index_low[grid_x][grid_y]
          == spice.spice_models[imodel].grid_index_high[grid_x][grid_y]) {
        continue;
      }
      for (i = spice.spice_models[imodel].grid_index_low[grid_x][grid_y]; 
           i < spice.spice_models[imodel].grid_index_high[grid_x][grid_y]; 
           i++) {
        switch (meas_type) {
        case SPICE_MEASURE_LEAKAGE_POWER:
          if (TRUE == leakage_only) {
            fprintf(fp, ".measure tran leakage_power_%s[%d] find p(Vgvdd_%s[%d]) at=0\n",
                    spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
          } else {
            fprintf(fp, ".measure tran leakage_power_%s[%d] avg p(Vgvdd_%s[%d]) from=0 to='clock_period'\n",
                    spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
          }
          break;
        case SPICE_MEASURE_DYNAMIC_POWER:
          fprintf(fp, ".measure tran dynamic_power_%s[%d] avg p(Vgvdd_%s[%d]) from='clock_period' to='%d*clock_period'\n",
                  spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i, num_cycle);
          break;
        default: 
          vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid meas_type!\n", __FILE__, __LINE__);
          exit(1);
        }
      }
    }
  }

  /* Measure the total power of this kind of spice model */
  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      switch (meas_type) {
      case SPICE_MEASURE_LEAKAGE_POWER:
        /* Bypass zero-usage spice_model in this grid*/
        if (spice.spice_models[imodel].grid_index_low[grid_x][grid_y]
            == spice.spice_models[imodel].grid_index_high[grid_x][grid_y]) {
          continue;
        }
        for (i = spice.spice_models[imodel].grid_index_low[grid_x][grid_y]; 
             i < spice.spice_models[imodel].grid_index_high[grid_x][grid_y]; 
             i++) {
          fprintf(fp, ".measure tran leakage_power_%s[%dto%d] \n", 
                  spice.spice_models[imodel].prefix, 
                  spice.spice_models[imodel].grid_index_low[grid_x][grid_y], 
                  i);
          if (spice.spice_models[imodel].grid_index_low[grid_x][grid_y] == i) {
            fprintf(fp, "+ param = 'leakage_power_%s[%d]'\n", spice.spice_models[imodel].prefix, i);
          } else {
            fprintf(fp, "+ param = 'leakage_power_%s[%d]+leakage_power_%s[%dto%d]'\n", 
                    spice.spice_models[imodel].prefix, 
                    i, spice.spice_models[imodel].prefix, 
                    spice.spice_models[imodel].grid_index_low[grid_x][grid_y], 
                    i-1);
          }
        }
        /* Spot the total leakage power of this spice model */
        fprintf(fp, ".measure tran total_leakage_power_%s \n", spice.spice_models[imodel].prefix);
        fprintf(fp, "+ param = 'leakage_power_%s[%dto%d]'\n", 
                spice.spice_models[imodel].prefix, 
                spice.spice_models[imodel].grid_index_low[grid_x][grid_y],
                spice.spice_models[imodel].grid_index_high[grid_x][grid_y]-1); 
        break;
      case SPICE_MEASURE_DYNAMIC_POWER:
        /* Bypass zero-usage spice_model in this grid*/
        if (spice.spice_models[imodel].grid_index_low[grid_x][grid_y]
            == spice.spice_models[imodel].grid_index_high[grid_x][grid_y]) {
          continue;
        }
        for (i = spice.spice_models[imodel].grid_index_low[grid_x][grid_y]; 
             i < spice.spice_models[imodel].grid_index_high[grid_x][grid_y]; 
             i++) {
          fprintf(fp, ".measure tran dynamic_power_%s[%dto%d] \n", 
                  spice.spice_models[imodel].prefix, 
                  spice.spice_models[imodel].grid_index_low[grid_x][grid_y], 
                  i);
          if (spice.spice_models[imodel].grid_index_low[grid_x][grid_y] == i) {
            fprintf(fp, "+ param = 'dynamic_power_%s[%d]'\n", spice.spice_models[imodel].prefix, i);
          } else {
            fprintf(fp, "+ param = 'dynamic_power_%s[%d]+dynamic_power_%s[%dto%d]'\n", 
                    spice.spice_models[imodel].prefix, i, 
                    spice.spice_models[imodel].prefix, 
                    spice.spice_models[imodel].grid_index_low[grid_x][grid_y], 
                    i-1);
          }
        }
        /* Spot the total dynamic power of this spice model */
        fprintf(fp, ".measure tran total_dynamic_power_%s \n", spice.spice_models[imodel].prefix);
        fprintf(fp, "+ param = 'dynamic_power_%s[%dto%d]'\n", 
                spice.spice_models[imodel].prefix,
                spice.spice_models[imodel].grid_index_low[grid_x][grid_y],
                spice.spice_models[imodel].grid_index_high[grid_x][grid_y]-1); 
        fprintf(fp, ".measure tran total_energy_per_cycle_%s \n", spice.spice_models[imodel].prefix);
        fprintf(fp, "+ param = 'dynamic_power_%s[%dto%d]*clock_period'\n", 
                spice.spice_models[imodel].prefix,
                spice.spice_models[imodel].grid_index_low[grid_x][grid_y],
                spice.spice_models[imodel].grid_index_high[grid_x][grid_y]-1); 
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid meas_type!\n", __FILE__, __LINE__);
        exit(1);
      }
    }
  }
  
  
  return;
}


/***** Print (call) the defined grids *****/
void fprint_call_defined_grids(FILE* fp) {
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
      fprintf(fp, "\n");
      fprint_grid_pins(fp, ix, iy, 1);
      fprintf(fp, "+ ");
      fprintf(fp, "gvdd 0 grid[%d][%d]\n", ix, iy); /* Call the name of subckt */ 
    }
  } 

  /* IO Grids */
  /* LEFT side */
  ix = 0;
  for (iy = 1; iy < (ny + 1); iy++) {
    assert(IO_TYPE == grid[ix][iy].type);
    fprintf(fp, "Xgrid[%d][%d] ", ix, iy);
    fprintf(fp, "\n");
    fprint_io_grid_pins(fp, ix, iy, 1);
    fprintf(fp, "+ ");
    /* Connect to a speical vdd port for statistics power */
    fprintf(fp, "gvdd_io 0 grid[%d][%d]\n", ix, iy); /* Call the name of subckt */ 
  }

  /* RIGHT side */
  ix = nx + 1;
  for (iy = 1; iy < (ny + 1); iy++) {
    assert(IO_TYPE == grid[ix][iy].type);
    fprintf(fp, "Xgrid[%d][%d] ", ix, iy);
    fprintf(fp, "\n");
    fprint_io_grid_pins(fp, ix, iy, 1);
    fprintf(fp, "+ ");
    /* Connect to a speical vdd port for statistics power */
    fprintf(fp, "gvdd_io 0 grid[%d][%d]\n", ix, iy); /* Call the name of subckt */ 
  }

  /* BOTTOM side */
  iy = 0;
  for (ix = 1; ix < (nx + 1); ix++) {
    assert(IO_TYPE == grid[ix][iy].type);
    fprintf(fp, "Xgrid[%d][%d] ", ix, iy);
    fprintf(fp, "\n");
    fprint_io_grid_pins(fp, ix, iy, 1);
    fprintf(fp, "+ ");
    /* Connect to a speical vdd port for statistics power */
    fprintf(fp, "gvdd_io 0 grid[%d][%d]\n", ix, iy); /* Call the name of subckt */ 
  } 

  /* TOP side */
  iy = ny + 1;
  for (ix = 1; ix < (nx + 1); ix++) {
    assert(IO_TYPE == grid[ix][iy].type);
    fprintf(fp, "Xgrid[%d][%d] ", ix, iy);
    fprintf(fp, "\n");
    fprint_io_grid_pins(fp, ix, iy, 1);
    fprintf(fp, "+ ");
    /* Connect to a speical vdd port for statistics power */
    fprintf(fp, "gvdd_io 0 grid[%d][%d]\n", ix, iy); /* Call the name of subckt */ 
  } 

  return;
}

/* Call defined channels. 
 * Ensure the port name here is co-herent to other sub-circuits(SB,CB,grid)!!!
 */
void fprint_call_defined_chan(FILE* fp,
                             t_rr_type chan_type,
                             int x,
                             int y,
                             int chan_width) {
  int itrack;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }
  
  /* Check */
  switch (chan_type) {
  case CHANX:
    /* check x*/
    assert((0 < x)&&(x < (nx + 1))); 
    /* check y*/
    assert((!(0 > y))&&(y < (ny + 1))); 
    /* Call the define sub-circuit */
    fprintf(fp, "Xchanx[%d][%d] ", x, y);
    fprintf(fp, "\n");
    for (itrack = 0; itrack < chan_width; itrack++) {
      fprintf(fp, "+ ");
      fprintf(fp, "chanx[%d][%d]_in[%d] ", x, y, itrack);
      fprintf(fp, "\n");
    }
    for (itrack = 0; itrack < chan_width; itrack++) {
      fprintf(fp, "+ ");
      fprintf(fp, "chanx[%d][%d]_out[%d] ", x, y, itrack);
      fprintf(fp, "\n");
    }
    /* output at middle point */
    for (itrack = 0; itrack < chan_width; itrack++) {
      fprintf(fp, "+ ");
      fprintf(fp, "chanx[%d][%d]_midout[%d] ", x, y, itrack);
      fprintf(fp, "\n");
    }
    fprintf(fp, "+ gvdd 0 chanx[%d][%d]\n", x, y);
    break;
  case CHANY:
    /* check x*/
    assert((!(0 > x))&&(x < (nx + 1))); 
    /* check y*/
    assert((0 < y)&&(y < (ny + 1))); 
    /* Call the define sub-circuit */
    fprintf(fp, "Xchany[%d][%d] ", x, y);
    fprintf(fp, "\n");
    for (itrack = 0; itrack < chan_width; itrack++) {
      fprintf(fp, "+ ");
      fprintf(fp, "chany[%d][%d]_in[%d] ", x, y, itrack);
      fprintf(fp, "\n");
    }
    for (itrack = 0; itrack < chan_width; itrack++) {
      fprintf(fp, "+ ");
      fprintf(fp, "chany[%d][%d]_out[%d] ", x, y, itrack);
      fprintf(fp, "\n");
    }
    /* output at middle point */
    for (itrack = 0; itrack < chan_width; itrack++) {
      fprintf(fp, "+ ");
      fprintf(fp, "chany[%d][%d]_midout[%d] ", x, y, itrack);
      fprintf(fp, "\n");
    }
    fprintf(fp, "+ gvdd 0 chany[%d][%d]\n", x, y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid Channel Type!\n", __FILE__, __LINE__);
    exit(1);
  }


  return;
}

/* Call the sub-circuits for channels : Channel X and Channel Y*/
void fprint_call_defined_channels(FILE* fp) {
  int ix, iy;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Channel X */
  for (iy = 0; iy < (ny + 1); iy++) {
    for (ix = 1; ix < (nx + 1); ix++) {
      fprint_call_defined_chan(fp, CHANX, ix, iy, chan_width_x[iy]);
    }
  }

  /* Channel Y */
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      fprint_call_defined_chan(fp, CHANY, ix, iy, chan_width_y[ix]);
    }
  }

  return;
}

/* Call the defined sub-circuit of connection box
 * TODO: actually most of this function is copied from
 * spice_routing.c : fprint_conneciton_box_interc
 * Should be more clever to use the original function
 */
void fprint_call_defined_connection_box(FILE* fp,
                                        t_rr_type chan_type,
                                        int x,
                                        int y,
                                        int chan_width,
                                        t_ivec*** LL_rr_node_indices) {
  int itrack, inode, side;
  int side_cnt = 0;
  int num_ipin_rr_node = 0;
  t_rr_node** ipin_rr_nodes = NULL;
  int num_temp_rr_node = 0;
  t_rr_node** temp_rr_nodes = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  
  /* Print the definition of subckt*/
  /* Identify the type of connection box */
  switch(chan_type) {
  case CHANX:
    fprintf(fp, "Xcbx[%d][%d] ", x, y);
    break;
  case CHANY:
    fprintf(fp, "Xcby[%d][%d] ", x, y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }
 
  fprintf(fp, "\n");
  /* Print the ports of channels*/
  /* connect to the mid point of a track*/
  for (itrack = 0; itrack < chan_width; itrack++) {
    fprintf(fp, "+ ");
    switch(chan_type) { 
    case CHANX:
      fprintf(fp, "chanx[%d][%d]_midout[%d] ", x, y, itrack);
      break;
    case CHANY:
      fprintf(fp, "chany[%d][%d]_midout[%d] ", x, y, itrack);
      break;
    default: 
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
      exit(1);
    }
    fprintf(fp, "\n");
  }
  /* Print the ports of grids*/
  side_cnt = 0;
  num_ipin_rr_node = 0;  
  for (side = 0; side < 4; side++) {
    fprintf(fp, "+ ");
    switch (side) {
    case 0: /* TOP */
      switch(chan_type) { 
      case CHANX:
        /* BOTTOM INPUT Pins of Grid[x][y+1] */
        fprint_grid_side_pins(fp, IPIN, x, y + 1, 2);
        /* Collect IPIN rr_nodes*/ 
        temp_rr_nodes = get_grid_side_pin_rr_nodes(&num_temp_rr_node, IPIN, x, y + 1, 2, LL_rr_node_indices);
        /* Update the ipin_rr_nodes, if ipin_rr_nodes is NULL, realloc will do a pure malloc */
        ipin_rr_nodes = (t_rr_node**)realloc(ipin_rr_nodes, sizeof(t_rr_node*)*(num_ipin_rr_node + num_temp_rr_node));
        for (inode = num_ipin_rr_node; inode < (num_ipin_rr_node + num_temp_rr_node); inode++) {
          ipin_rr_nodes[inode] = temp_rr_nodes[inode - num_ipin_rr_node];
        } 
        /* Count in the new members*/
        num_ipin_rr_node += num_temp_rr_node; 
        /* Free the temp_ipin_rr_node */
        my_free(temp_rr_nodes);
        num_temp_rr_node = 0;
        side_cnt++;
        break; 
      case CHANY:
        /* Nothing should be done */
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
        exit(1);
      }
      break;
    case 1: /* RIGHT */
      switch(chan_type) { 
      case CHANX:
        /* Nothing should be done */
        break; 
      case CHANY:
        /* LEFT INPUT Pins of Grid[x+1][y] */
        fprint_grid_side_pins(fp, IPIN, x + 1, y, 3);
        /* Collect IPIN rr_nodes*/ 
        temp_rr_nodes = get_grid_side_pin_rr_nodes(&num_temp_rr_node, IPIN, x + 1, y, 3, LL_rr_node_indices);
        /* Update the ipin_rr_nodes, if ipin_rr_nodes is NULL, realloc will do a pure malloc */
        ipin_rr_nodes = (t_rr_node**)realloc(ipin_rr_nodes, sizeof(t_rr_node*)*(num_ipin_rr_node + num_temp_rr_node));
        for (inode = num_ipin_rr_node; inode < (num_ipin_rr_node + num_temp_rr_node); inode++) {
          ipin_rr_nodes[inode] = temp_rr_nodes[inode - num_ipin_rr_node];
        } 
        /* Count in the new members*/
        num_ipin_rr_node += num_temp_rr_node; 
        /* Free the temp_ipin_rr_node */
        my_free(temp_rr_nodes);
        num_temp_rr_node = 0;
        side_cnt++;
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
        exit(1);
      }
      break;
    case 2: /* BOTTOM */
      switch(chan_type) { 
      case CHANX:
        /* TOP INPUT Pins of Grid[x][y] */
        fprint_grid_side_pins(fp, IPIN, x, y, 0);
        /* Collect IPIN rr_nodes*/ 
        temp_rr_nodes = get_grid_side_pin_rr_nodes(&num_temp_rr_node, IPIN, x, y, 0, LL_rr_node_indices);
        /* Update the ipin_rr_nodes, if ipin_rr_nodes is NULL, realloc will do a pure malloc */
        ipin_rr_nodes = (t_rr_node**)realloc(ipin_rr_nodes, sizeof(t_rr_node*)*(num_ipin_rr_node + num_temp_rr_node));
        for (inode = num_ipin_rr_node; inode < (num_ipin_rr_node + num_temp_rr_node); inode++) {
          ipin_rr_nodes[inode] = temp_rr_nodes[inode - num_ipin_rr_node];
        } 
        /* Count in the new members*/
        num_ipin_rr_node += num_temp_rr_node; 
        /* Free the temp_ipin_rr_node */
        my_free(temp_rr_nodes);
        num_temp_rr_node = 0;
        side_cnt++;
        break; 
      case CHANY:
        /* Nothing should be done */
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
        exit(1);
      }
      break;
    case 3: /* LEFT */
      switch(chan_type) { 
      case CHANX:
        /* Nothing should be done */
        break; 
      case CHANY:
        /* RIGHT INPUT Pins of Grid[x][y] */
        fprint_grid_side_pins(fp, IPIN, x, y, 1);
        /* Collect IPIN rr_nodes*/ 
        temp_rr_nodes = get_grid_side_pin_rr_nodes(&num_temp_rr_node, IPIN, x, y, 1, LL_rr_node_indices);
        /* Update the ipin_rr_nodes, if ipin_rr_nodes is NULL, realloc will do a pure malloc */
        ipin_rr_nodes = (t_rr_node**)realloc(ipin_rr_nodes, sizeof(t_rr_node*)*(num_ipin_rr_node + num_temp_rr_node));
        for (inode = num_ipin_rr_node; inode < (num_ipin_rr_node + num_temp_rr_node); inode++) {
          ipin_rr_nodes[inode] = temp_rr_nodes[inode - num_ipin_rr_node];
        } 
        /* Count in the new members*/
        num_ipin_rr_node += num_temp_rr_node; 
        /* Free the temp_ipin_rr_node */
        my_free(temp_rr_nodes);
        num_temp_rr_node = 0;
        side_cnt++;
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
        exit(1);
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid side index!\n", __FILE__, __LINE__);
      exit(1);
    }
    fprintf(fp, "\n");
  }
  /* Check */
  assert(2 == side_cnt);


  fprintf(fp, "+ ");
  /* Identify the type of connection box */
  switch(chan_type) {
  case CHANX:
    /* Need split vdd port for each Connection Box */
    fprintf(fp, "gvdd_cbx[%d][%d] 0 ", x, y);
    fprintf(fp, "cbx[%d][%d]\n", x, y);
    break;
  case CHANY:
    /* Need split vdd port for each Connection Box */
    fprintf(fp, "gvdd_cby[%d][%d] 0 ", x, y);
    fprintf(fp, "cby[%d][%d]\n", x, y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }
   
  /* Free */
  my_free(ipin_rr_nodes);
 
  return;
}

/* Call the sub-circuits for connection boxes */
void fprint_call_defined_connection_boxes(FILE* fp,
                                          t_ivec*** LL_rr_node_indices) {
  int ix, iy, chan_width;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* X - channels [1...nx][0..ny]*/
  for (iy = 0; iy < (ny + 1); iy++) {
    for (ix = 1; ix < (nx + 1); ix++) {
      chan_width = chan_width_x[iy];
      fprint_call_defined_connection_box(fp, CHANX, ix, iy, chan_width, LL_rr_node_indices);
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      chan_width = chan_width_y[ix];
      fprint_call_defined_connection_box(fp, CHANY, ix, iy, chan_width, LL_rr_node_indices);
    }
  }
 
  return; 
}

/* Call the defined switch box sub-circuit
 * TODO: This function is also copied from
 * spice_routing.c : fprint_routing_switch_box_subckt
 */
void fprint_call_defined_switch_box(FILE* fp,
                                    int x, 
                                    int y) {
  int side, itrack;
  int* chan_width = (int*)my_malloc(sizeof(int)*4);

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 

  /* find Chan width for each side */
  for (side = 0; side < 4; side++) {
    switch (side) {
    case 0:
      /* For the bording, we should take special care */
      if (y == ny) { 
        chan_width[side] = 0;
        break;
      }
      chan_width[side] = chan_width_y[x];
      break;
    case 1:
      /* For the bording, we should take special care */
      if (x == nx) { 
        chan_width[side] = 0;
        break;
      }
      chan_width[side] = chan_width_x[y];
      break;
    case 2:
      /* For the bording, we should take special care */
      if (0 == y) { 
        chan_width[side] = 0;
        break;
      }
      chan_width[side] = chan_width_y[x];
      break;
    case 3:
      /* For the bording, we should take special care */
      if (0 == x) { 
        chan_width[side] = 0;
        break;
      }
      chan_width[side] = chan_width_x[y];
      break;
    default: 
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid side index.\n", 
                 __FILE__, __LINE__); 
      exit(1);
    } 
  } 
                                  
  fprintf(fp, "Xsb[%d][%d] ", x, y);
  fprintf(fp, "\n");
  /* 1. Channel Y [x][y+1] inputs */
  for (itrack = 0; itrack < chan_width[0]; itrack++) {
    fprintf(fp, "+ ");
    fprintf(fp, "chany[%d][%d]_in[%d] ", x, y + 1, itrack);
    fprintf(fp, "\n");
  }
  /* 2. Channel X [x+1][y] inputs */
  for (itrack = 0; itrack < chan_width[1]; itrack++) {
    fprintf(fp, "+ ");
    fprintf(fp, "chanx[%d][%d]_in[%d] ", x + 1, y, itrack);
    fprintf(fp, "\n");
  }
  /* 3. Channel Y [x][y] outputs */
  for (itrack = 0; itrack < chan_width[2]; itrack++) {
    fprintf(fp, "+ ");
    fprintf(fp, "chany[%d][%d]_out[%d] ", x, y, itrack);
    fprintf(fp, "\n");
  }
  /* 4. Channel X [x][y] outputs */
  for (itrack = 0; itrack < chan_width[3]; itrack++) {
    fprintf(fp, "+ ");
    fprintf(fp, "chanx[%d][%d]_out[%d] ", x, y, itrack);
    fprintf(fp, "\n");
  }

  /* Considering the border */
  if (ny != y) {
    /* 5. Grid[x][y+1] Right side outputs pins */
    fprintf(fp, "+ ");
    fprint_grid_side_pins(fp, OPIN, x, y+1, 1);
    fprintf(fp, "\n");
  }
  if (0 != x) {
    /* 6. Grid[x][y+1] Bottom side outputs pins */
    fprintf(fp, "+ ");
    fprint_grid_side_pins(fp, OPIN, x, y+1, 2);
    fprintf(fp, "\n");
  }

  if (ny != y) {
    /* 7. Grid[x+1][y+1] Left side output pins */
    fprintf(fp, "+ ");
    fprint_grid_side_pins(fp, OPIN, x+1, y+1, 3);
    fprintf(fp, "\n");
  }
  if (nx != x) {
    /* 8. Grid[x+1][y+1] Bottom side output pins */
    fprintf(fp, "+ ");
    fprint_grid_side_pins(fp, OPIN, x+1, y+1, 2);
    fprintf(fp, "\n");
  }

  if (nx != x) {
    /* 9. Grid[x+1][y] Top side output pins */
    fprintf(fp, "+ ");
    fprint_grid_side_pins(fp, OPIN, x+1, y, 0);
    fprintf(fp, "\n");
  }
  if (0 != y) {
    /* 10. Grid[x+1][y] Left side output pins */
    fprintf(fp, "+ ");
    fprint_grid_side_pins(fp, OPIN, x+1, y, 3);
    fprintf(fp, "\n");
  }

  if (0 != y) {
    /* 11. Grid[x][y] Right side output pins */
    fprintf(fp, "+ ");
    fprint_grid_side_pins(fp, OPIN, x, y, 1);
    fprintf(fp, "\n");
  } 
  if (0 != x) {
    /* 12. Grid[x][y] Top side output pins */
    fprintf(fp, "+ ");
    fprint_grid_side_pins(fp, OPIN, x, y, 0);
    fprintf(fp, "\n");
  }

  /* Connect to separate vdd port for each switch box??? */
  fprintf(fp, "+ ");
  fprintf(fp, "gvdd_sb[%d][%d] 0 sb[%d][%d]\n", x, y, x, y);

  /* Free */
  my_free(chan_width);

  return;
}

void fprint_call_defined_switch_boxes(FILE* fp) {
  int ix, iy;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 0; iy < (ny + 1); iy++) {
      fprint_call_defined_switch_box(fp, ix, iy);
    }
  }

  return;
}

/* Print stimulations for floating ports in Grid
 * Some ports of CLB or I/O Pads is floating.
 * There are two cases : 
 * 1. Their corresponding rr_node (SOURE or OPIN) has 0 fan-out.
 * 2. Their corresponding rr_node (SINK or IPIN) has 0 fan-in.
 * In these cases, we short connect them to global GND.
 */
void fprint_grid_float_port_stimulation(FILE* fp) {
  int inode;
  int num_float_port = 0;
  int port_x, port_y, port_height;
  int side, class_id, pin_index, pin_written_times;
  t_type_ptr type = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Search all rr_nodes */
  for (inode = 0; inode < num_rr_nodes; inode++) {
    switch (rr_node[inode].type) {
    case SOURCE:
    case OPIN:
      /*  Make sure 0 fan-in, 1 fan-in is connected to SOURCE */
      assert((0 == rr_node[inode].fan_in)||(1 == rr_node[inode].fan_in));
      if (1 == rr_node[inode].fan_in) {
        assert(SOURCE == rr_node[rr_node[inode].prev_node].type);
      }
      /* Check if there is 0 fan-out */
      if (0 == rr_node[inode].num_edges) {
        port_x = rr_node[inode].xlow;
        port_y = rr_node[inode].ylow;
        port_height = grid[port_x][port_y].offset;
        port_y = port_y + port_height;
        type = grid[port_x][port_y].type;
        assert(NULL != type);
        /* Get pin information */
        pin_index = rr_node[inode].ptc_num;
        class_id = type->pin_class[pin_index];
        assert(DRIVER == type->class_inf[class_id].type);
        pin_written_times = 0;
        for (side = 0; side < 4; side++) {
          /* Special Care for I/O pad */
          if (IO_TYPE == type) { 
            side = determine_io_grid_side(port_x, port_y);
          }
          if (1 == type->pinloc[port_height][side][pin_index]) { 
            fprintf(fp, "Vfloat_port_%d grid[%d][%d]_pin[%d][%d][%d] 0 0\n",
                    num_float_port, port_x, port_y, port_height, side, pin_index);
            pin_written_times++;
            num_float_port++;
          }
          /* Special Care for I/O pad */
          if (IO_TYPE == type) { 
            break;
          }
        }
        assert(1 == pin_written_times);
      }
      break; 
    case SINK:
    case IPIN:
      /*  Make sure 0 fan-out, 1 fan-out is connected to SINK */
      assert((0 == rr_node[inode].num_edges)||(1 == rr_node[inode].num_edges));
      if (1 == rr_node[inode].num_edges) {
        assert(SINK == rr_node[rr_node[inode].edges[0]].type);
      }
      /* Check if there is 0 fan-out */
      if (0 == rr_node[inode].fan_in) {
        port_x = rr_node[inode].xlow;
        port_y = rr_node[inode].ylow;
        port_height = grid[port_x][port_y].offset;
        port_y = port_y + port_height;
        type = grid[port_x][port_y + port_height].type;
        assert(NULL != type);
        /* Get pin information */
        pin_index = rr_node[inode].ptc_num;
        class_id = type->pin_class[pin_index];
        assert(RECEIVER == type->class_inf[class_id].type);
        pin_written_times = 0;
        for (side = 0; side < 4; side++) {
          /* Special Care for I/O pad */
          if (IO_TYPE == type) { 
            side = determine_io_grid_side(port_x, port_y);
          }
          if (1 == type->pinloc[port_height][side][pin_index]) { 
            fprintf(fp, "Vfloat_port_%d grid[%d][%d]_pin[%d][%d][%d] 0 0\n",
                    num_float_port, port_x, port_y, port_height, side, pin_index);
            pin_written_times++;
            num_float_port++;
          }
          /* Special Care for I/O pad */
          if (IO_TYPE == type) { 
            break;
          }
        }
        assert(1 == pin_written_times);
      }
      break;
    case CHANX:
    case CHANY:
      /*TODO: check 0 fan-in, fan-out channel*/
    case INTRA_CLUSTER_EDGE:
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid rr_node type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }

  vpr_printf(TIO_MESSAGE_INFO, "Connect %d floating grid pin to global gnd.\n", num_float_port);

  return;
}

/* Print Technology Library and Design Parameters*/
void fprint_tech_lib(FILE* fp,
                     t_spice_tech_lib tech_lib) {
  t_spice_transistor_type* nmos_trans;
  t_spice_transistor_type* pmos_trans;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d]) FileHandle is NULL!\n",__FILE__,__LINE__); 
    exit(1);
  } 
  /* Include Technology Library*/
  fprintf(fp, "****** Include Technology Library ******\n");
  if (SPICE_LIB_INDUSTRY == tech_lib.type) {
    fprintf(fp, ".lib \'%s\' %s\n", tech_lib.path, tech_lib.transistor_type);
  } else {
    fprintf(fp, ".include \'%s\'\n", tech_lib.path);
  }

  /* Print Transistor parameters*/ 
  /* Define the basic transistor parameters: nl, pl, wn, wp, pn_ratio*/
  fprintf(fp, "****** Transistor Parameters ******\n");
  fprintf(fp,".param beta=%g\n",tech_lib.pn_ratio);
  /* Make sure we have only 2 transistor*/
  assert(2 == tech_lib.num_transistor_type);
  /* Find NMOS*/
  nmos_trans = find_mosfet_tech_lib(tech_lib,SPICE_TRANS_NMOS);
  if (NULL == nmos_trans) {
    vpr_printf(TIO_MESSAGE_ERROR,"NMOS transistor is not defined in architecture XML!\n");
    exit(1);
  } else {
    fprintf(fp,".param nl=%g\n",nmos_trans->chan_length);
    fprintf(fp,".param wn=%g\n",nmos_trans->min_width);
  }
  /* Find PMOS*/
  pmos_trans = find_mosfet_tech_lib(tech_lib,SPICE_TRANS_PMOS);
  if (NULL == pmos_trans) {
    vpr_printf(TIO_MESSAGE_ERROR,"PMOS transistor is not defined in architecture XML!\n");
    exit(1);
  } else {
    fprintf(fp,".param pl=%g\n",pmos_trans->chan_length);
    fprintf(fp,".param wp=%g\n",pmos_trans->min_width);
  }

  /* Print nominal Vdd */
  fprintf(fp, ".param vsp=%g\n", tech_lib.nominal_vdd);
  
  return;
}

/* This function may expand. 
 * It prints temperature, and options for a SPICE simulation
 */
void fprint_spice_options(FILE* fp,
                          t_spice_params spice_params) {

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!",__FILE__, __LINE__); 
    exit(1);
  } 
  
  /* Temperature */
  fprintf(fp, ".temp %d\n", spice_params.sim_temp);

  /* Options: print capacitances of all nodes */
  if (TRUE == spice_params.captab) {
    fprintf(fp, ".option captab\n");
  }
  /* Add post could make SPICE very slow for large benchmarks!!! Be careful*/
  if (TRUE == spice_params.post) {
    fprintf(fp, ".option post\n");
  }
  /* Use fast */
  if (TRUE == spice_params.fast) {
    fprintf(fp, ".option fast\n");
  }

  return;
}

/* This function may expand. 
 * It prints include paramters for SPICE netlists
 */
void fprint_spice_include_param_headers(FILE* fp,
                                        char* include_dir_path) {
  char* temp_include_file_path = NULL;
  char* formatted_include_dir_path = format_dir_path(include_dir_path);

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Include headers for measurements and stimulates */
  fprintf(fp, "****** Include Header file: measurement parameters *****\n");
  temp_include_file_path = my_strcat(formatted_include_dir_path, meas_header_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "****** Include Header file: stimulation parameters *****\n");
  temp_include_file_path = my_strcat(formatted_include_dir_path, stimu_header_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);
  
  return;
}


/* This function may expand. 
 * It prints include sub-circuit SPICE netlists
 */
void fprint_spice_include_key_subckts(FILE* fp,
                                      char* subckt_dir_path) {
  char* temp_include_file_path = NULL;
  char* formatted_subckt_dir_path = format_dir_path(subckt_dir_path);

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Include necessary sub-circuits */
  fprintf(fp, "****** Include subckt netlists: NMOS and PMOS *****\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, nmos_pmos_spice_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  if (1 == rram_design_tech) {
    fprintf(fp, "****** Include subckt netlists: RRAM behavior VerilogA model *****\n");
    /* This is a HSPICE Bug! When the verilogA file contain a dir_path, the sim results become weired! */
    /*
    temp_include_file_path = my_strcat(formatted_subckt_dir_path, rram_veriloga_file_name);
    fprintf(fp, ".hdl \'%s\'\n", temp_include_file_path);
    my_free(temp_include_file_path);
    */
    fprintf(fp, ".hdl \'%s\'\n", rram_veriloga_file_name);
  }

  fprintf(fp, "****** Include subckt netlists: Inverters, Buffers *****\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, basics_spice_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "****** Include subckt netlists: Multiplexers *****\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, muxes_spice_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "****** Include subckt netlists: Wires *****\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, wires_spice_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  return;
}

void fprint_voltage_pulse_params(FILE* fp,
                                 int init_val,
                                 float density,
                                 float probability) {

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* TODO: check codes for density and probability, init_val */
  /* If density = 0, this is a constant signal */
  if (0. == density) {
    if (0. == probability) {
      fprintf(fp, "+  0\n");
    } else {
      fprintf(fp, "+  vsp\n");
    }
    return;
  }

  if (0 == init_val) {
    fprintf(fp, "+  pulse(0 vsp 'clock_period' \n");
  } else {
    fprintf(fp, "+  pulse(vsp 0 'clock_period' \n");
  }
  /*
  fprintf(fp, "+  'input_slew_pct_rise*%g*clock_period' 'input_slew_pct_fall*%g*clock_period'\n",
          2./density, 2./density);
  */
  /* TODO: Think about a reasonable slew for signals with diverse density */
  fprintf(fp, "+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'\n");
  fprintf(fp, "+  '%g*%g*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '%g*clock_period')\n",
          probability, 2./density, 2./density);

  return;
}


void fprint_spice_netlist_transient_setting(FILE* fp, 
                                            t_spice spice, 
                                            int num_sim_clock_cycles,
                                            boolean leakage_only) {
  int num_clock_cycle = spice.spice_params.meas_params.sim_num_clock_cycle + 1;
 
  /* Overwrite the sim if auto is turned on */
  /*
  if ((TRUE == spice.spice_params.meas_params.auto_select_sim_num_clk_cycle)
    &&(num_sim_clock_cycles < num_clock_cycle)) {
    num_clock_cycle = num_sim_clock_cycles;
  }
  */

  if (TRUE == spice.spice_params.meas_params.auto_select_sim_num_clk_cycle) {
    assert(!(num_sim_clock_cycles > num_clock_cycle));
  }

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Leakage power only, use a simplified tran sim*/
  if (TRUE == leakage_only) {
    fprintf(fp, "***** Transient simulation only for leakage power  *****\n");
    fprintf(fp, ".tran 'clock_period/1' 'clock_period'\n");
  } else {
    /* Determine the transistion time to simulate */
    fprintf(fp, "***** %d Clock Simulation, accuracy=%g *****\n",
            num_clock_cycle, spice.spice_params.meas_params.accuracy);
    switch (spice.spice_params.meas_params.accuracy_type) {
    case SPICE_ABS: 
      fprintf(fp, ".tran %g '%d*clock_period'\n", 
              spice.spice_params.meas_params.accuracy, num_clock_cycle);
      break;
    case SPICE_FRAC:
      fprintf(fp, ".tran '%d*clock_period/%d' '%d*clock_period'\n", 
              num_clock_cycle, (int)spice.spice_params.meas_params.accuracy, num_clock_cycle);
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid accuracy type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }

  return;
}

void fprint_stimulate_dangling_one_grid_pin(FILE* fp,
                                            int x, int y,
                                            int height, int side, int pin_index,
                                            t_ivec*** LL_rr_node_indices) {
  t_type_ptr type_descriptor = grid[x][y].type;
  int capacity = grid[x][y].type->capacity;
  int class_id;
  int rr_node_index;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  assert(NULL != type_descriptor);
  assert(0 < capacity);

  class_id = type_descriptor->pin_class[pin_index];
  if (DRIVER == type_descriptor->class_inf[class_id].type) {
    rr_node_index = get_rr_node_index(x, y, OPIN, pin_index, LL_rr_node_indices); 
    /* Zero fan-out OPIN */
    if (0 == rr_node[rr_node_index].num_edges) {
      fprintf(fp, "Rdangling_grid[%d][%d]_pin[%d][%d][%d] grid[%d][%d]_pin[%d][%d][%d] 0 1e9\n",
              x, y, height, side, pin_index,
              x, y, height, side, pin_index);
      fprintf(fp, "*.nodeset V(grid[%d][%d]_pin[%d][%d][%d]) 0 \n",
              x, y, height, side, pin_index);
    }
    return;
  }
  if (RECEIVER == type_descriptor->class_inf[class_id].type) {
    rr_node_index = get_rr_node_index(x, y, IPIN, pin_index, LL_rr_node_indices); 
    /* Zero fan-in IPIN */
    if (0 == rr_node[rr_node_index].fan_in) {
      fprintf(fp, "Vdangling_grid[%d][%d]_pin[%d][%d][%d] grid[%d][%d]_pin[%d][%d][%d] 0 0\n",
              x, y, height, side, pin_index,
              x, y, height, side, pin_index);
      fprintf(fp, ".nodeset V(grid[%d][%d]_pin[%d][%d][%d]) 0\n",
              x, y, height, side, pin_index);
    }
    return;
  }

  return;
}

void fprint_stimulate_dangling_io_grid_pins(FILE* fp,
                                            int x, int y) {
  int iheight, side, ipin; 
  int side_pin_index;
  t_type_ptr type_descriptor = grid[x][y].type;
  int capacity = grid[x][y].type->capacity;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  assert(NULL != type_descriptor);
  assert(0 < capacity);

  /* identify the location of IO grid and 
   * decide which side of ports we need
   */
  side = determine_io_grid_side(x,y);

  /* Count the number of pins */
  side_pin_index = 0;
  //for (iz = 0; iz < capacity; iz++) {
  for (iheight = 0; iheight < type_descriptor->height; iheight++) {
    for (ipin = 0; ipin < type_descriptor->num_pins; ipin++) {
      if (1 == type_descriptor->pinloc[iheight][side][ipin]) {
        fprint_stimulate_dangling_one_grid_pin(fp, x, y, iheight, side, ipin, rr_node_indices);
      }
    }
  }  
  //}

  return;
}

void fprint_stimulate_dangling_normal_grid_pins(FILE* fp,
                                                int x, int y) {
  int iheight, side, ipin; 
  int side_pin_index;
  t_type_ptr type_descriptor = grid[x][y].type;
  int capacity = grid[x][y].type->capacity;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  assert(NULL != type_descriptor);
  assert(0 < capacity);

  for (side = 0; side < 4; side++) {
    /* Count the number of pins */
    side_pin_index = 0;
    for (iheight = 0; iheight < type_descriptor->height; iheight++) {
      for (ipin = 0; ipin < type_descriptor->num_pins; ipin++) {
        if (1 == type_descriptor->pinloc[iheight][side][ipin]) {
          fprint_stimulate_dangling_one_grid_pin(fp, x, y, iheight, side, ipin, rr_node_indices);
        }
      }
    }  
  }

  return;

}

void fprint_stimulate_dangling_grid_pins(FILE* fp) {
  int ix, iy;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Normal Grids */
  for (ix = 1; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      assert(IO_TYPE != grid[ix][iy].type);
      /* zero-fan-in CLB IPIN*/
      fprint_stimulate_dangling_normal_grid_pins(fp, ix, iy);
    }
  } 

  /* IO Grids */
  /* LEFT side */
  ix = 0;
  for (iy = 1; iy < (ny + 1); iy++) {
    assert(IO_TYPE == grid[ix][iy].type);
    fprint_stimulate_dangling_io_grid_pins(fp, ix, iy);
  }

  /* RIGHT side */
  ix = nx + 1;
  for (iy = 1; iy < (ny + 1); iy++) {
    assert(IO_TYPE == grid[ix][iy].type);
    fprint_stimulate_dangling_io_grid_pins(fp, ix, iy);
  }

  /* BOTTOM side */
  iy = 0;
  for (ix = 1; ix < (nx + 1); ix++) {
    assert(IO_TYPE == grid[ix][iy].type);
    fprint_stimulate_dangling_io_grid_pins(fp, ix, iy);
  } 

  /* TOP side */
  iy = ny + 1;
  for (ix = 1; ix < (nx + 1); ix++) {
    assert(IO_TYPE == grid[ix][iy].type);
    fprint_stimulate_dangling_io_grid_pins(fp, ix, iy);
  } 

  return;
}

void init_logical_block_spice_model_temp_used(t_spice_model* spice_model) {
  int i;

  /* For each logical block, we print a vdd */
  for (i = 0; i < num_logical_blocks; i++) {
    if (logical_block[i].mapped_spice_model == spice_model) {
      logical_block[i].temp_used = 0;
    }
  }

  return;  
}

void init_logical_block_spice_model_type_temp_used(int num_spice_models, t_spice_model* spice_model,
                                                   enum e_spice_model_type spice_model_type) {
  int i;

  /* For each logical block, we print a vdd */
  for (i = 0; i < num_spice_models; i++) {
    if (spice_model_type == spice_model[i].type) {
      init_logical_block_spice_model_temp_used(&(spice_model[i]));
    }
  }

  return;  
}

void fprint_global_vdds_logical_block_spice_model(FILE* fp,
                                                  t_spice_model* spice_model) {
  int i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* For each logical block, we print a vdd */
  for (i = 0; i < num_logical_blocks; i++) {
    if ((logical_block[i].mapped_spice_model == spice_model)
       &&(1 == logical_block[i].temp_used)){
      fprintf(fp, ".global gvdd_%s[%d]\n",
              spice_model->prefix, logical_block[i].mapped_spice_model_index);
    } 
  }

  return;
}

void fprint_splited_vdds_logical_block_spice_model(FILE* fp,
                                                   t_spice_model* spice_model) {
  int i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* For each logical block, we print a vdd */
  for (i = 0; i < num_logical_blocks; i++) {
    if ((logical_block[i].mapped_spice_model == spice_model) 
       &&(1 == logical_block[i].temp_used)){
      fprintf(fp, "Vgvdd_%s[%d] gvdd_%s[%d] 0 vsp\n",
              spice_model->prefix, logical_block[i].mapped_spice_model_index,
              spice_model->prefix, logical_block[i].mapped_spice_model_index);
    } 
  }

  return;
}

void fprint_measure_vdds_logical_block_spice_model(FILE* fp,
                                                   t_spice_model* spice_model,
                                                   enum e_measure_type meas_type,
                                                   int num_clock_cycle,
                                                   boolean leakage_only) {
  int i, iport, ipin, cur;
  float average_output_density = 0.;
  int output_cnt = 0;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* For each logical block, we print a vdd */
  for (i = 0; i < num_logical_blocks; i++) {
    if ((logical_block[i].mapped_spice_model == spice_model) 
       &&(1 == logical_block[i].temp_used)) {
      /* Get the average output density */
      output_cnt = 0;
      for (iport = 0; iport < logical_block[i].pb->pb_graph_node->num_output_ports; iport++) {
        for (ipin = 0; ipin < logical_block[i].pb->pb_graph_node->num_output_pins[iport]; ipin++) {
          average_output_density += vpack_net[logical_block[i].output_nets[iport][ipin]].spice_net_info->density; 
          output_cnt++;
        }
      }
      average_output_density = average_output_density/output_cnt;
      switch (meas_type) {
      case SPICE_MEASURE_LEAKAGE_POWER:
        if (TRUE == leakage_only) {
          fprintf(fp, ".measure tran leakage_power_%s[%d] find p(Vgvdd_%s[%d]) at=0\n",
              spice_model->prefix, logical_block[i].mapped_spice_model_index,
              spice_model->prefix, logical_block[i].mapped_spice_model_index);
        } else {
          fprintf(fp, ".measure tran leakage_power_%s[%d] avg p(Vgvdd_%s[%d]) from=0 to='clock_period'\n",
              spice_model->prefix, logical_block[i].mapped_spice_model_index,
              spice_model->prefix, logical_block[i].mapped_spice_model_index);
        }
        break;
      case SPICE_MEASURE_DYNAMIC_POWER:
        fprintf(fp, ".measure tran dynamic_power_%s[%d] avg p(Vgvdd_%s[%d]) from='clock_period' to='%d*clock_period'\n",
                spice_model->prefix, logical_block[i].mapped_spice_model_index,
                spice_model->prefix, logical_block[i].mapped_spice_model_index,
                num_clock_cycle);
        fprintf(fp, ".measure tran energy_per_cycle_%s[%d] param='dynamic_power_%s[%d]*clock_period'\n",
                spice_model->prefix, logical_block[i].mapped_spice_model_index,
                spice_model->prefix, logical_block[i].mapped_spice_model_index);
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid meas_type!\n", __FILE__, __LINE__);
        exit(1);
      }
    } 
  }

  /* Measure the total power of this kind of spice model */
  cur = 0;
  switch (meas_type) {
  case SPICE_MEASURE_LEAKAGE_POWER:
    for (i = 0; i < num_logical_blocks; i++) {
      if ((logical_block[i].mapped_spice_model == spice_model) 
        &&(1 == logical_block[i].temp_used)) {
        fprintf(fp, ".measure tran leakage_power_%s[0to%d] \n",
              spice_model->prefix, cur);
        if (0 == cur) {
          fprintf(fp, "+ param = 'leakage_power_%s[%d]'\n",
              spice_model->prefix, logical_block[i].mapped_spice_model_index);
        } else {
          fprintf(fp, "+ param = 'leakage_power_%s[%d]+leakage_power_%s[0to%d]'\n", 
              spice_model->prefix, logical_block[i].mapped_spice_model_index,
              spice_model->prefix, cur-1);
        }
        cur++;
      }
    }
    if (0 == cur) {
      break;
    }
    /* Spot the total leakage power of this spice model */
    fprintf(fp, ".measure tran total_leakage_power_%s \n", spice_model->prefix);
    fprintf(fp, "+ param = 'leakage_power_%s[0to%d]'\n", 
       spice_model->prefix, cur-1);
    break;
  case SPICE_MEASURE_DYNAMIC_POWER:
    for (i = 0; i < num_logical_blocks; i++) {
      if ((logical_block[i].mapped_spice_model == spice_model)
        &&(1 == logical_block[i].temp_used)) {
        fprintf(fp, ".measure tran energy_per_cycle_%s[0to%d] \n",
            spice_model->prefix, cur);
        if (0 == cur) {
          fprintf(fp, "+ param = 'energy_per_cycle_%s[%d]'\n",
              spice_model->prefix, logical_block[i].mapped_spice_model_index);
        } else {
          fprintf(fp, "+ param = 'energy_per_cycle_%s[%d]+energy_per_cycle_%s[0to%d]'\n", 
              spice_model->prefix, logical_block[i].mapped_spice_model_index,
             spice_model->prefix, cur-1);
        }
        cur++;
      }
    }
    if (0 == cur) {
      break;
    }
    /* Spot the total dynamic power of this spice model */
    fprintf(fp, ".measure tran total_energy_per_cycle_%s \n", spice_model->prefix);
    fprintf(fp, "+ param = 'energy_per_cycle_%s[0to%d]'\n", 
            spice_model->prefix, cur-1);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid meas_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  return;
}

