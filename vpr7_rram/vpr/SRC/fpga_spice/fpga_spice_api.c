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
#include "path_delay.h"
#include "stats.h"

/* Include spice support headers*/
#include "linkedlist.h"
#include "fpga_spice_utils.h"
#include "fpga_spice_backannotate_utils.h"
#include "fpga_spice_setup.h"
#include "spice_api.h"
#include "syn_verilog_api.h"

/* Top-level API of FPGA-SPICE */
void vpr_fpga_spice_tool_suites(t_vpr_setup vpr_setup,
                                t_arch Arch) {
  /* Common initializations and malloc operations */
  /* If FPGA-SPICE is not called, we should initialize the spice_models */
  if (TRUE == vpr_setup.FPGA_SPICE_Opts.do_fpga_spice) {
    fpga_spice_setup(vpr_setup, &Arch);
  }

  /* Xifan TANG: SPICE Modeling, SPICE Netlist Output  */ 
  if (vpr_setup.FPGA_SPICE_Opts.SpiceOpts.do_spice) {
    vpr_print_spice_netlists(vpr_setup, Arch, vpr_setup.FileNameOpts.CircuitName);
  }

  /* Xifan TANG: Synthesizable verilog dumping */
  if (vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.dump_syn_verilog) {
    vpr_dump_syn_verilog(vpr_setup, Arch, vpr_setup.FileNameOpts.CircuitName);
  }	

  /* Free */
  if (TRUE == vpr_setup.FPGA_SPICE_Opts.do_fpga_spice) {
    /* Free all the backannotation containing post routing information */
    free_backannotate_vpr_post_route_info();
    /* TODO: free other linked lists ! */
    fpga_spice_free(&Arch);
  }


  return;
}

