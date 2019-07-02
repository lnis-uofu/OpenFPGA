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
#include "fpga_x2p_utils.h"
#include "fpga_x2p_backannotate_utils.h"
#include "fpga_x2p_setup.h"
#include "spice_api.h"
#include "verilog_api.h"
#include "fpga_bitstream.h"

#include "fpga_x2p_api.h"

/* Top-level API of FPGA-SPICE */
void vpr_fpga_x2p_tool_suites(t_vpr_setup vpr_setup,
                                t_arch Arch) {
  t_sram_orgz_info* sram_bitstream_orgz_info = NULL;

  /* Common initializations and malloc operations */
  /* If FPGA-SPICE is not called, we should initialize the spice_models */
  if (TRUE == vpr_setup.FPGA_SPICE_Opts.do_fpga_spice) {
    fpga_x2p_setup(vpr_setup, &Arch);
  }

  /* Xifan TANG: SPICE Modeling, SPICE Netlist Output  */ 
  if (TRUE == vpr_setup.FPGA_SPICE_Opts.SpiceOpts.do_spice) {
    vpr_fpga_spice(vpr_setup, Arch, vpr_setup.FileNameOpts.CircuitName);
  }

  /* Xifan TANG: Synthesizable verilog dumping */
  if (TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.dump_syn_verilog) {
    vpr_fpga_verilog(vpr_setup, Arch, vpr_setup.FileNameOpts.CircuitName);
  }	

  /* Xifan Tang: Bitstream Generator */
  if ((TRUE == vpr_setup.FPGA_SPICE_Opts.BitstreamGenOpts.gen_bitstream)
    &&(FALSE == vpr_setup.FPGA_SPICE_Opts.SpiceOpts.do_spice)
    &&(FALSE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.dump_syn_verilog)) {
    /* Run bitstream generation here only when other functionalities are disabled;
     * bitstream will be run inside SPICE and Verilog Generators
     */
    vpr_fpga_bitstream_generator(vpr_setup, Arch, vpr_setup.FileNameOpts.CircuitName, &sram_bitstream_orgz_info);
    /* Free sram_orgz_info */
    free_sram_orgz_info(sram_bitstream_orgz_info,
                        sram_bitstream_orgz_info->type);
  }

  /* Free */
  if (TRUE == vpr_setup.FPGA_SPICE_Opts.do_fpga_spice) {
    /* Free all the backannotation containing post routing information */
    free_backannotate_vpr_post_route_info();
    /* TODO: free other linked lists ! */
    fpga_x2p_free(&Arch);
  }


  return;
}

