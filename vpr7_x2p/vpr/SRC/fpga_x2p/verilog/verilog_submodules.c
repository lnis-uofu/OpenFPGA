/***********************************/
/*  Synthesizable Verilog Dumping  */
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
#include <algorithm>

/* Include vpr structs*/
#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph.h"
#include "vpr_utils.h"
#include "path_delay.h"
#include "stats.h"
#include "vtr_assert.h"

/* Include FPGA-SPICE utils */
#include "linkedlist.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_globals.h"
#include "fpga_x2p_mux_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "mux_library.h"
#include "module_manager.h"
#include "module_manager_utils.h"

/* Include verilog utils */
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_pbtypes.h"
#include "verilog_decoder.h"

#include "mux_utils.h"
#include "verilog_writer_utils.h"
#include "verilog_mux.h"
#include "verilog_essential_gates.h"
#include "verilog_decoders.h"
#include "verilog_lut.h"
#include "verilog_memory.h"
#include "verilog_wire.h"

#include "verilog_submodules.h"

/***** Subroutines *****/

static 
void dump_verilog_submodule_timing(FILE* fp,
                                   t_spice_model* cur_spice_model) {
  int iport, ipin, iedge;
  int num_input_port;
  t_spice_model_port** input_port= NULL;

  input_port = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);

  /* return if there is no delay info */
  if ( 0 == cur_spice_model->num_delay_info) {
    return;
  }

  /* Return if there is no input ports */
  if (0 == num_input_port) {
    return;
  }

  /* Ensure a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File handler.\n",
               __FILE__, __LINE__); 
    exit(1);
  }

  fprintf(fp, "`ifdef %s\n", verilog_timing_preproc_flag);
  fprintf(fp, "  //------ BEGIN Pin-to-pin Timing constraints -----\n");
  fprintf(fp, "  specify\n");
  /* Give pin-to-pin delays */
  /* Enumerate timing edges of each input ports */
  for (iport = 0; iport < num_input_port; iport++) {
    for (ipin = 0; ipin < input_port[iport]->size; ipin++) {
      for (iedge = 0; iedge < input_port[iport]->num_tedges[ipin]; iedge++) {
        fprintf(fp, "(%s[%d] => %s[%d]) = (%.2g, %.2g);\n",
                input_port[iport]->lib_name, ipin,
                input_port[iport]->tedge[ipin][iedge]->to_port->lib_name,
                input_port[iport]->tedge[ipin][iedge]->to_port_pin_number,
                input_port[iport]->tedge[ipin][iedge]->trise / verilog_sim_timescale,
                input_port[iport]->tedge[ipin][iedge]->tfall / verilog_sim_timescale);
      }
    }
  }
  fprintf(fp, "  endspecify\n");
  fprintf(fp, "  //------ END Pin-to-pin Timing constraints -----\n");
  fprintf(fp, "`endif\n");

  return;
}

static 
void dump_verilog_submodule_signal_init(FILE* fp,
                                        t_spice_model* cur_spice_model) {
  int iport;
  int num_input_port;
  t_spice_model_port** input_port= NULL;
  input_port = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);

  /* Ensure a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File handler.\n",
               __FILE__, __LINE__); 
    exit(1);
  }

  fprintf(fp, "\n`ifdef %s\n", verilog_signal_init_preproc_flag);
  fprintf(fp, "  //------ BEGIN driver initialization -----\n");
  fprintf(fp, "initial begin\n");
//  fprintf(fp, "`ifdef %s\n  #0.001\n`endif\n", 			// Commented, looks no longer needed
//				icarus_simulator_flag);
  fprintf(fp, "  `ifdef %s\n", verilog_formal_verification_preproc_flag);
  for (iport = 0; iport < num_input_port; iport++) {
     fprintf(fp, "  $deposit(%s, 1'b0);\n",
                input_port[iport]->lib_name);
  }
  fprintf(fp, "  `else\n");
  for (iport = 0; iport < num_input_port; iport++) {
     fprintf(fp, "  $deposit(%s, $random);\n",
                input_port[iport]->lib_name);
  }
  fprintf(fp, "  `endif\n");
  fprintf(fp, "end\n");
  fprintf(fp, "  //------ END driver initialization -----\n");
  fprintf(fp, "`endif\n");

  return;
}

/* Dump a CMOS MUX basis module */
static 
void dump_verilog_cmos_mux_one_basis_module(FILE* fp, 
                                            char* mux_basis_subckt_name, 
                                            int mux_size, 
                                            int num_input_basis_subckt, 
                                            t_spice_model* cur_spice_model,
                                            boolean special_basis) { 
  int cur_mem, i;
  int num_mem = num_input_basis_subckt;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Determine the number of memory bit
   * The function considers a special case :
   * 2-input basis in tree-like MUX only requires 1 memory bit */
  num_mem = determine_num_sram_bits_mux_basis_subckt(cur_spice_model, mux_size, num_input_basis_subckt, special_basis);

  /* Comment lines */
  fprintf(fp, "//---- CMOS MUX basis module: %s -----\n", mux_basis_subckt_name);

  /* Print the port list and definition */
  fprintf(fp, "module %s (\n", mux_basis_subckt_name);
  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_spice_model, TRUE, FALSE, FALSE, TRUE)) {
    fprintf(fp, ",\n");
  }
  /* Port list */
  fprintf(fp, "input [0:%d] in,\n", num_input_basis_subckt - 1);
  fprintf(fp, "output out,\n");
  fprintf(fp, "input [0:%d] mem,\n",
          num_mem - 1);
  fprintf(fp, "input [0:%d] mem_inv);\n",
          num_mem - 1);
  /* Verilog Behavior description for a MUX */
  fprintf(fp, "//---- Behavior-level description -----\n");
  /* Special case: only one memory, switch case is simpler 
   * When mem = 1, propagate input 0; 
   * when mem = 0, propagate input 1;
   */
  if (1 == num_mem) {
    fprintf(fp, "  reg out_reg;\n");
    fprintf(fp, "  always @(in, mem)\n");
    fprintf(fp, "  case (mem)\n");
    fprintf(fp, "    1'b1: out_reg = in[0];\n");
    fprintf(fp, "    1'b0: out_reg = in[1];\n");
    fprintf(fp, "    default: out_reg <= 1'bz;\n");
    fprintf(fp, "  endcase\n");
    fprintf(fp, "  assign out = out_reg;\n");
  } else {
  /* Other cases, we need to follow the rules:
   * When mem[k] is enabled, switch on input[k]
   * Only one memory bit is enabled!
   */
    fprintf(fp, "  reg out_reg;\n");
    fprintf(fp, "  always @(in, mem)\n");
    fprintf(fp, "  case (mem)\n");
    fprintf(fp, "//---- Note that MSB is mem[0] while LSB is mem[%d] -----\n", num_mem-1);
    fprintf(fp, "//---- Due to the delcare convention of port [MSB:LSB] -----\n");
    for (cur_mem = 0; cur_mem < num_mem; cur_mem++) {
      fprintf(fp, "    %d'b", num_mem);
      for (i = 0; i < cur_mem; i++) {
        fprintf(fp, "0");
      }
      fprintf(fp, "1");
      for (i = cur_mem + 1; i < num_mem; i++) {
        fprintf(fp, "0");
      }
      fprintf(fp, ":");
      fprintf(fp, " out_reg <= in[%d];\n", cur_mem);
    }
    fprintf(fp, "    default: out_reg <= 1'bz;\n");
    fprintf(fp, "  endcase\n");

    fprintf(fp, "  assign out = out_reg;\n");
  }

  /* Put an end to this module */
  fprintf(fp, "endmodule\n");

  /* Comment lines */
  fprintf(fp, "//---- END CMOS MUX basis module: %s -----\n\n", mux_basis_subckt_name);

  return;
}

/* Dump a structural verilog for SRAM-based MUX basis module 
 * This is only called when structural verilog dumping option is enabled for this spice model
 * Note that the structural verilog may be used for functionality verification!!!
 */
static 
void dump_verilog_cmos_mux_one_basis_module_structural(FILE* fp, 
                                                       char* mux_basis_subckt_name, 
                                                       int mux_size, 
                                                       int num_input_basis_subckt, 
                                                       t_spice_model* cur_spice_model,
                                                       boolean special_basis) { 
  int i;
  int num_mem = num_input_basis_subckt;
  /* Get the tgate module name */
  char* tgate_module_name = cur_spice_model->pass_gate_logic->spice_model_name;
  t_spice_model* tgate_spice_model = cur_spice_model->pass_gate_logic->spice_model;
  int num_input_port = 0;
  int num_output_port = 0;
  int num_sram_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
  t_spice_model_port** sram_port = NULL;

  assert(TRUE == cur_spice_model->dump_structural_verilog);

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Find the input port, output port, and sram port*/
  assert ( NULL != tgate_spice_model);
  input_port = find_spice_model_ports(tgate_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_port = find_spice_model_ports(tgate_spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  sram_port = find_spice_model_ports(tgate_spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);

  /* Check */
  assert ((3 == num_input_port));
  for (i = 0; i < num_input_port; i++) {
    assert ( 1 == input_port[i]->size );
  }
  assert ((1 == num_output_port) 
       && (1 == output_port[0]->size));

  /* Determine the number of memory bit
   * The function considers a special case :
   * 2-input basis in tree-like MUX only requires 1 memory bit */
  num_mem = determine_num_sram_bits_mux_basis_subckt(cur_spice_model, mux_size, num_input_basis_subckt, special_basis);

  /* Comment lines */
  fprintf(fp, "//---- Structural Verilog for CMOS MUX basis module: %s -----\n", mux_basis_subckt_name);

  /* Print the port list and definition */
  fprintf(fp, "module %s (\n", mux_basis_subckt_name);
  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_spice_model, TRUE, FALSE, FALSE, TRUE)) {
    fprintf(fp, ",\n");
  }
  /* Port list */
  fprintf(fp, "input [0:%d] in,\n", num_input_basis_subckt - 1);
  fprintf(fp, "output out,\n");
  fprintf(fp, "input [0:%d] mem,\n",
          num_mem - 1/*, sram_port[0]->prefix*/);
  fprintf(fp, "input [0:%d] mem_inv);\n",
          num_mem - 1/*, sram_port[0]->prefix*/);
  /* Verilog Behavior description for a MUX */
  fprintf(fp, "//---- Structure-level description -----\n");
  /* Special case: only one memory, switch case is simpler 
   * When mem = 1, propagate input 0; 
   * when mem = 0, propagate input 1;
   */
  if (1 == num_mem) {
    /* Transmission gates are connected to each input and also the output*/
    fprintf(fp, "  %s %s_0 ",
                tgate_module_name, tgate_module_name);
    /* Dump explicit port map if required */
    if (TRUE == tgate_spice_model->dump_explicit_port_map) {
      fprintf(fp, " (.%s(in[0]), .%s(mem[0]), .%s(mem_inv[0]), .%s(out));\n",
              input_port[0]->lib_name, 
              input_port[1]->lib_name, 
              input_port[2]->lib_name,
              output_port[0]->lib_name);
    } else {
      fprintf(fp, " (in[0], mem[0], mem_inv[0], out);\n");
    }
    fprintf(fp, "  %s %s_1 ",
                tgate_module_name, tgate_module_name);
    /* Dump explicit port map if required */
    if (TRUE == tgate_spice_model->dump_explicit_port_map) {
      fprintf(fp, " (.%s(in[1]), .%s(mem_inv[0]), .%s(mem[0]), .%s(out));\n",
              input_port[0]->lib_name, 
              input_port[1]->lib_name,
              input_port[2]->lib_name, 
              output_port[0]->lib_name);
    } else {
      fprintf(fp, " (in[1], mem_inv[0], mem[0], out);\n");
    }
 
  } else {
  /* Other cases, we need to follow the rules:
   * When mem[k] is enabled, switch on input[k]
   * Only one memory bit is enabled!
   */
    for (i = 0; i < num_mem; i++) {
      fprintf(fp, "  %s %s_%d ",
                  tgate_module_name, tgate_module_name, i);
      /* Dump explicit port map if required */
      if (TRUE == tgate_spice_model->dump_explicit_port_map) {
        fprintf(fp, " (.%s(in[%d]), .%s(mem[%d]), .%s(mem_inv[%d]), .%s(out));\n",
                input_port[0]->lib_name, i,
                input_port[1]->lib_name, i,
                input_port[2]->lib_name, i,
                output_port[0]->lib_name);
      } else {
        fprintf(fp, " (in[%d], mem[%d], mem_inv[%d], out);\n",
                i, i, i);
      }
    }
  }

  /* Put an end to this module */
  fprintf(fp, "endmodule\n");

  /* Comment lines */
  fprintf(fp, "//---- END Structural Verilog CMOS MUX basis module: %s -----\n\n", mux_basis_subckt_name);

  return;
}

/* Dump a structural verilog for RRAM MUX basis module 
 * This is only called when structural verilog dumping option is enabled for this spice model
 * Note that the structural verilog cannot be used for functionality verification!!!
 */
static 
void dump_verilog_rram_mux_one_basis_module_structural(FILE* fp, 
                                                       char* mux_basis_subckt_name, 
                                                       int num_input_basis_subckt, 
                                                       t_spice_model* cur_spice_model) { 
  /* RRAM MUX needs 2*(input_size + 1) memory bits for configuration purpose */
  int num_mem = num_input_basis_subckt + 1;
  int i;
  char* progTE_module_name = "PROG_TE";
  char* progBE_module_name = "PROG_BE";

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  assert(TRUE == cur_spice_model->dump_structural_verilog);

  /* Comment lines */
  fprintf(fp, "//---- Structural Verilog for RRAM MUX basis module: %s -----\n", mux_basis_subckt_name);

  /* Print the port list and definition */
  fprintf(fp, "module %s (\n", mux_basis_subckt_name);
  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_spice_model, TRUE, FALSE, FALSE, TRUE)) {
    fprintf(fp, ",\n");
  }
  /* Port list */
  fprintf(fp, "input wire [0:%d] in,\n", num_input_basis_subckt - 1);
  fprintf(fp, "output wire out,\n");
  fprintf(fp, "input wire [0:%d] bl,\n",
          num_mem - 1);
  fprintf(fp, "input wire [0:%d] wl);\n",
          num_mem - 1);

  /* Print internal structure of 4T1R programming structures
   * Written in structural Verilog
   * The whole structure-level description is divided into two parts:
   * 1. Left part consists of N PROG_TE modules, each of which
   * includes a PMOS, a NMOS and a RRAM, which is actually the left
   * part of a 4T1R programming structure 
   * 2. Right part includes only a PROG_BE module, which consists
   * of a PMOS and a NMOS, which is actually the right part of a
   * 4T1R programming sturcture
   */
  /* LEFT part */
  for (i = 0; i < num_input_basis_subckt - 1; i++) {
    fprintf(fp, "%s %s_%d (.A(in[%d]), .WL(wl[%d]), .BLB(bl[%d]), .Z(out));\n",
                 progTE_module_name, progTE_module_name, i,
                 i, i, i);
  }
  /* RIGHT part */
  fprintf(fp, "%s %s_%d (.INOUT(out), .WL(wl[%d]), .BLB(bl[%d]));\n",
               progBE_module_name, progBE_module_name, i,
               i, i);

  /* Put an end to this module */
  fprintf(fp, "endmodule\n");

  /* Comment lines */
  fprintf(fp, "//---- END Structural Verilog for RRAM MUX basis module: %s -----\n\n", mux_basis_subckt_name);

  return;
}


/* Dump a RRAM MUX basis module */
static 
void dump_verilog_rram_mux_one_basis_module(FILE* fp, 
                                            char* mux_basis_subckt_name, 
                                            int num_input_basis_subckt, 
                                            t_spice_model* cur_spice_model) { 
  /* RRAM MUX needs 2*(input_size + 1) memory bits for configuration purpose */
  int num_mem = num_input_basis_subckt + 1;
  int i, iport, ipin;
  int find_prog_EN = 0;
  int find_prog_ENb = 0;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Comment lines */
  fprintf(fp, "//---- RRAM MUX basis module: %s -----\n", mux_basis_subckt_name);

  /* Print the port list and definition */
  fprintf(fp, "module %s (\n", mux_basis_subckt_name);
  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_spice_model, TRUE, FALSE, FALSE, TRUE)) {
    fprintf(fp, ",\n");
  }
  /* Port list */
  fprintf(fp, "input wire [0:%d] in,\n", num_input_basis_subckt - 1);
  fprintf(fp, "output wire out,\n");
  fprintf(fp, "input wire [0:%d] bl,\n",
          num_mem - 1);
  fprintf(fp, "input wire [0:%d] wl);\n",
          num_mem - 1);

  /* Print the internal logics: 
   * ONLY 4T1R programming structure is supported up to now
   */
  fprintf(fp, "reg [0:%d] reg_out;\n", num_input_basis_subckt - 1);
  fprintf(fp, "always @(");
  for (i = 0; i < num_mem; i++) { 
    if (0 <  i) { 
      fprintf(fp, ",");
    }
    fprintf(fp, "wl[%d], bl[%d] ", i, i);
  }
  fprintf(fp, ")\n");
  fprintf(fp, "begin \n");

  /* Only when the last bit of wl is enabled, 
   * the propagating path can be changed 
   * (RRAM value can be changed) */
  fprintf(fp, "\tif ((wl[%d])", num_mem - 1);
  /* Find the config_enable ports (prog_EN and prog_ENb)
   * in global ports*/
  for (iport = 0; iport < cur_spice_model->num_port; iport++) {
    if (FALSE == cur_spice_model->ports[iport].is_config_enable) {
      continue;
    }
    /* Reach here, the port should be is_config_enable */
    if (0 == cur_spice_model->ports[iport].default_val) {
      for (ipin = 0; ipin < cur_spice_model->ports[iport].size; ipin++) {
        fprintf(fp, "\n\t&&(%s[%d])", 
                     cur_spice_model->ports[iport].prefix,
                     ipin);
      }
      /* Update counter */
      find_prog_EN++;
    } else {
      assert (1 == cur_spice_model->ports[iport].default_val);
      for (ipin = 0; ipin < cur_spice_model->ports[iport].size; ipin++) {
        fprintf(fp, "\n\t&&(~%s[%d])", 
                     cur_spice_model->ports[iport].prefix,
                     ipin);
      }
      /* Update counter */
      find_prog_ENb++;
    }
  }
  /* Check if we find any config_enable signals */
  if (0 == find_prog_EN) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Unable to find a config_enable signal with default value 0 for a RRAM MUX (%s)!\n",
               __FILE__, __LINE__, cur_spice_model->name); 
    exit(1);
  }
  if (0 == find_prog_ENb) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Unable to find a config_enable signal with default value 1 for a RRAM MUX (%s)!\n",
               __FILE__, __LINE__, cur_spice_model->name); 
    exit(1);
  }

  /* Finish the if clause */
  fprintf(fp, ") begin\n");

  for (i = 0; i < num_input_basis_subckt; i++) { 
    fprintf(fp, "\tif (1 == bl[%d]) begin\n", i);
    fprintf(fp, "\t\tassign reg_out = %d;\n",i);
    fprintf(fp, "\tend else ");
  }
  fprintf(fp, "\tbegin\n");
  fprintf(fp, "\t\t\tassign reg_out = 0;\n");
  fprintf(fp, "\t\tend\n");
  fprintf(fp, "\tend\n");
  fprintf(fp, "end\n");
 
  fprintf(fp, "assign out = in[reg_out];\n");

  /* Put an end to this module */
  fprintf(fp, "endmodule\n");

  /* Comment lines */
  fprintf(fp, "//---- END RRAM MUX basis module: %s -----\n\n", mux_basis_subckt_name);

  return;
}

/* Print a basis submodule */
static 
void dump_verilog_mux_one_basis_module(FILE* fp, 
                                       char* mux_basis_subckt_name, 
                                       int mux_size,
                                       int num_input_basis_subckt, 
                                       t_spice_model* cur_spice_model,
                                       boolean special_basis) { 
  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 
  /* Depend on the technology */
  switch (cur_spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    if (TRUE == cur_spice_model->dump_structural_verilog) {
      dump_verilog_cmos_mux_one_basis_module_structural(fp, mux_basis_subckt_name,
                                                        mux_size,
                                                        num_input_basis_subckt,
                                                        cur_spice_model,
                                                        special_basis);
    } else {
      dump_verilog_cmos_mux_one_basis_module(fp, mux_basis_subckt_name,
                                             mux_size,
                                             num_input_basis_subckt,
                                             cur_spice_model,
                                             special_basis);
    }
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* If requested, we can dump structural verilog for basis module */
    if (TRUE == cur_spice_model->dump_structural_verilog) {
      dump_verilog_rram_mux_one_basis_module_structural(fp, mux_basis_subckt_name,
                                                        num_input_basis_subckt,
                                                        cur_spice_model);
    } else {
      dump_verilog_rram_mux_one_basis_module(fp, mux_basis_subckt_name,
                                             num_input_basis_subckt,
                                             cur_spice_model);
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of MUX(name: %s)\n",
               __FILE__, __LINE__, cur_spice_model->name); 
    exit(1);
  }

  return;
}

/**
 * Dump a verilog module for the basis circuit of a MUX
 */
static 
void dump_verilog_mux_basis_module(FILE* fp, 
                                   t_spice_mux_model* spice_mux_model) {
  /** Act depends on the structure of MUX
   * 1. tree-like/one-level: we generate a basis module 
   * 2. two/multi-level: we generate a basis and a special module (if required)
   */
  int num_input_basis_subckt = 0;
  int num_input_special_basis_subckt = 0;

  char* mux_basis_subckt_name = NULL;
  char* special_basis_subckt_name = NULL;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 
 
  /* Try to find a mux in cmos technology, 
   * if we have, then build CMOS 2:1 MUX, and given cmos_mux2to1_subckt_name
   */
  /* Exception: LUT require an auto-generation of netlist can run as well*/ 
  assert((SPICE_MODEL_MUX == spice_mux_model->spice_model->type)
       ||(SPICE_MODEL_LUT == spice_mux_model->spice_model->type));

  /* Generate the spice_mux_arch */
  spice_mux_model->spice_mux_arch = (t_spice_mux_arch*)my_malloc(sizeof(t_spice_mux_arch));
  init_spice_mux_arch(spice_mux_model->spice_model, 
                      spice_mux_model->spice_mux_arch, 
                      spice_mux_model->size);

  /* Exception: if tgate is a standard cell, we skip the basis circuit generation */
  t_spice_model* tgate_spice_model = spice_mux_model->spice_model->pass_gate_logic->spice_model;
  if (SPICE_MODEL_GATE == tgate_spice_model->type) {
    assert (SPICE_MODEL_GATE_MUX2 == tgate_spice_model->design_tech_info.gate_info->type);
    /* Double check the mux structure, which should be tree-like */
    if ( SPICE_MODEL_STRUCTURE_TREE != spice_mux_model->spice_mux_arch->structure ) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Structure of Circuit model (%s) should be tree-like because it is linked to a 2:1 MUX!\n",
                 __FILE__, __LINE__, spice_mux_model->spice_model->name);
      exit(1);
    }
    return;
  }

  /* Corner case: Error out  MUX_SIZE = 2, automatcially give a one-level structure */
  /*
  if ((2 == spice_mux_model->size)&&(SPICE_MODEL_STRUCTURE_ONELEVEL != spice_mux_model->spice_model->design_tech_info.structure)) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Structure of SPICE model (%s) should be one-level because it is linked to a 2:1 MUX!\n",
               __FILE__, __LINE__, spice_mux_model->spice_model->name);
    exit(1);
  }
  */

  /* Prepare the basis subckt name:
   */
  mux_basis_subckt_name = generate_verilog_mux_basis_subckt_name(spice_mux_model->spice_model, spice_mux_model->size, verilog_mux_basis_posfix);

  special_basis_subckt_name = generate_verilog_mux_basis_subckt_name(spice_mux_model->spice_model, spice_mux_model->size, verilog_mux_special_basis_posfix);

  /* deteremine the number of inputs of basis subckt */ 
  num_input_basis_subckt = spice_mux_model->spice_mux_arch->num_input_basis;

  /* Print the basis subckt*/
  dump_verilog_mux_one_basis_module(fp, mux_basis_subckt_name, spice_mux_model->size,
                                    num_input_basis_subckt, spice_mux_model->spice_model, 
                                    FALSE);
  /* See if we need a special basis */
  switch (spice_mux_model->spice_model->design_tech_info.mux_info->structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    num_input_special_basis_subckt = find_spice_mux_arch_special_basis_size(*(spice_mux_model->spice_mux_arch));
    if (0 < num_input_special_basis_subckt) {
      dump_verilog_mux_one_basis_module(fp, special_basis_subckt_name, spice_mux_model->size,
                                        num_input_special_basis_subckt, spice_mux_model->spice_model,
                                        FALSE);
    } 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, spice_mux_model->spice_model->name);
    exit(1);
  }

  /* Free */
  my_free(mux_basis_subckt_name);
  my_free(special_basis_subckt_name);

  return;
}

static 
void dump_verilog_cmos_mux_tree_structure(FILE* fp, 
                                          char* mux_basis_subckt_name,
                                          t_spice_model spice_model,
                                          t_spice_mux_arch spice_mux_arch,
                                          int num_sram_port, t_spice_model_port** sram_port,
                                          bool is_explicit_mapping) {
  int i, j, level, nextlevel;
  int nextj, out_idx;
  int mux_basis_cnt = 0;

  int num_buf_input_port = 0;
  int num_buf_output_port = 0;

  t_spice_model_port** buf_input_port = NULL;
  t_spice_model_port** buf_output_port = NULL;
  
  boolean* inter_buf_loc = NULL;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Intermediate buffer location map */
  inter_buf_loc = (boolean*)my_calloc(spice_mux_arch.num_level + 1, sizeof(boolean));
  for (i = 0; i < spice_mux_arch.num_level + 1; i++) {
    inter_buf_loc[i] = FALSE;
  }
  if (NULL != spice_model.lut_intermediate_buffer->location_map) {
    assert ((size_t)spice_mux_arch.num_level - 1 == strlen(spice_model.lut_intermediate_buffer->location_map));
    /* For intermediate buffers */ 
    for (i = 0; i < spice_mux_arch.num_level - 1; i++) {
      if ('1' == spice_model.lut_intermediate_buffer->location_map[i]) {
        inter_buf_loc[spice_mux_arch.num_level - i - 1] = TRUE;
      }
    }
  }
  /*
  printf("inter_buf_loc[]=");
  for (i = 0; i < spice_mux_arch.num_level + 1; i++) {
    printf("%d", inter_buf_loc[i]);
  }
  printf("\n");
  */

  mux_basis_cnt = 0;
  for (i = 0; i < spice_mux_arch.num_level; i++) {
    level = spice_mux_arch.num_level - i;
    nextlevel = spice_mux_arch.num_level - i - 1; 
    /* Check */
    assert(nextlevel > -1);
    fprintf(fp, "wire [%d:%d] mux2_l%d_in; \n", 
            0, spice_mux_arch.num_input_per_level[nextlevel] -1, /* input0 input1 */
            level);
    /* For intermediate buffers */ 
    if (TRUE == inter_buf_loc[level]) {
      fprintf(fp, "wire [%d:%d] mux2_l%d_in_buf; \n", 
              0, spice_mux_arch.num_input_per_level[nextlevel] -1, /* input0 input1 */
              level);
    }
  }
  fprintf(fp, "wire [%d:%d] mux2_l%d_in; \n",
          0, 0, 0);

  for (i = 0; i < spice_mux_arch.num_level; i++) {
    level = spice_mux_arch.num_level - i;
    nextlevel = spice_mux_arch.num_level - i - 1; 
    /* Check */
    assert(nextlevel > -1);
    /* Print basis mux2to1 for each level*/
    for (j = 0; j < spice_mux_arch.num_input_per_level[nextlevel]; j++) {
      nextj = j + 1;
      out_idx = j/2; 
      /* Each basis mux2to1: <given_name> <input0> <input1> <output> <sram> <sram_inv> svdd sgnd <subckt_name> */
      fprintf(fp, "%s mux_basis_no%d (", mux_basis_subckt_name, mux_basis_cnt); /* given_name */
      /* For MUX2 standard cell */
      t_spice_model* tgate_spice_model = spice_model.pass_gate_logic->spice_model;
      /* For non-standard cells */
      if (SPICE_MODEL_GATE == tgate_spice_model->type) {
        assert(SPICE_MODEL_GATE_MUX2 == tgate_spice_model->design_tech_info.gate_info->type);
        int num_input_port = 0;
        int num_output_port = 0;
        t_spice_model_port** input_port = NULL;
        t_spice_model_port** output_port = NULL;

        input_port = find_spice_model_ports(tgate_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
        output_port = find_spice_model_ports(tgate_spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
        /* Quick check on the number of ports */
        assert(3 == num_input_port); /* A, B and SEL */
        assert(1 == num_output_port); /* OUT */

        bool use_explicit_port_map;
        if ( (true == is_explicit_mapping) 
          || (TRUE == tgate_spice_model->dump_explicit_port_map) ) {
          use_explicit_port_map = true;
        }
      
        /* Dump global ports */
        if  (0 < rec_dump_verilog_spice_model_global_ports(fp, tgate_spice_model, FALSE, FALSE, my_bool_to_boolean(use_explicit_port_map), TRUE)) {
          fprintf(fp, ",\n");
        }
        if (true == use_explicit_port_map) {
          fprintf(fp, ".%s(", input_port[0]->lib_name);
        }
        /* For intermediate buffers */ 
        if (TRUE == inter_buf_loc[level]) {
          fprintf(fp, "mux2_l%d_in_buf[%d]", level, j); /* input0  */
        } else {
          fprintf(fp, "mux2_l%d_in[%d]", level, j); /* input0  */
        }
        if (true == use_explicit_port_map) {
          fprintf(fp, "), .%s(", input_port[1]->lib_name);
        } else {
          fprintf(fp, ", ");
        }
        /* For intermediate buffers */ 
        if (TRUE == inter_buf_loc[level]) {
          fprintf(fp, "mux2_l%d_in_buf[%d]", level, nextj); /* input1 */
        } else {
          fprintf(fp, "mux2_l%d_in[%d]", level, nextj); /* input1 */
        }
        if (true == use_explicit_port_map) {
          fprintf(fp, "), .%s(", output_port[0]->lib_name);
        } else {
          fprintf(fp, ", ");
        }
        fprintf(fp, "mux2_l%d_in[%d]", nextlevel, out_idx); /* output */
        if (true == use_explicit_port_map) {
          fprintf(fp, "), .%s(", input_port[2]->lib_name);
        } else {
          fprintf(fp, ", ");
        }
        fprintf(fp, "%s[%d]", sram_port[0]->prefix, i); /* sram  */
        if (true == use_explicit_port_map) {
          fprintf(fp, "));\n");
        } else {
          fprintf(fp, ");\n");
        }
      } else {
        assert (SPICE_MODEL_PASSGATE == tgate_spice_model->type); 
        /* Dump global ports */
        if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE, my_bool_to_boolean(is_explicit_mapping), TRUE)) {
          fprintf(fp, ",\n");
        }
        if (true == is_explicit_mapping) {
          fprintf(fp, ".in(");
        }
        /* For intermediate buffers */ 
        if (TRUE == inter_buf_loc[level]) {
          fprintf(fp, "mux2_l%d_in_buf[%d:%d]", level, j, nextj); /* input0 input1 */
        } else {
          fprintf(fp, "mux2_l%d_in[%d:%d]", level, j, nextj); /* input0 input1 */
        }
        if (true == is_explicit_mapping) {
          fprintf(fp, "), .out(");
        } else {
          fprintf(fp, ", ");
        }
        fprintf(fp, "mux2_l%d_in[%d]", nextlevel, out_idx); /* output */
        if (true == is_explicit_mapping) {
          fprintf(fp, "), .mem(");
        } else {
          fprintf(fp, ", ");
        }
        fprintf(fp, "%s[%d]", sram_port[0]->prefix, i); /* sram  */
        if (true == is_explicit_mapping) {
          fprintf(fp, "), .mem_inv(");
        } else {
          fprintf(fp, ", ");
        }
        fprintf(fp, "%s_inv[%d]", sram_port[0]->prefix, i); /* sram_inv */
        if (true == is_explicit_mapping) {
          fprintf(fp, "));\n");
        } else {
          fprintf(fp, ");\n");
        }
      }
      /* For intermediate buffers */ 
      if (TRUE == inter_buf_loc[nextlevel]) {
        /* Find the input port, output port, and sram port*/
        buf_input_port = find_spice_model_ports(spice_model.lut_intermediate_buffer->spice_model, SPICE_MODEL_PORT_INPUT, &num_buf_input_port, TRUE);
        buf_output_port = find_spice_model_ports(spice_model.lut_intermediate_buffer->spice_model, SPICE_MODEL_PORT_OUTPUT, &num_buf_output_port, TRUE);
        /* Check */
        assert ( (1 == num_buf_input_port)
                 &&(1 == buf_input_port[0]->size));
        assert ( (1 == num_buf_output_port)
                 &&(1 == buf_output_port[0]->size));
  
        /* TODO: what about tapered buffer, can we support? */
        /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
        fprintf(fp, "%s %s_%d_%d (", 
                spice_model.lut_intermediate_buffer->spice_model_name,
                spice_model.lut_intermediate_buffer->spice_model_name, 
                nextlevel, out_idx); /* Given name*/
        /* Dump global ports */
        if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.lut_intermediate_buffer->spice_model, FALSE, FALSE, spice_model.lut_intermediate_buffer->spice_model->dump_explicit_port_map, TRUE)) {
          fprintf(fp, ",\n");
        }
        /* Dump explicit port map if required */
        if ( TRUE == spice_model.lut_intermediate_buffer->spice_model->dump_explicit_port_map) {
          fprintf(fp, ".%s(", 
                  buf_input_port[0]->lib_name); 
        }
        fprintf(fp, "mux2_l%d_in[%d] ", nextlevel, out_idx); /* output */
        if ( TRUE == spice_model.lut_intermediate_buffer->spice_model->dump_explicit_port_map) {
          fprintf(fp, ")");
        }
        fprintf(fp, ", ");
        /* Dump explicit port map if required */
        if ( TRUE == spice_model.lut_intermediate_buffer->spice_model->dump_explicit_port_map) {
          fprintf(fp, ".%s(", 
                  buf_output_port[0]->lib_name); 
        }
        fprintf(fp, "mux2_l%d_in_buf[%d] ", nextlevel, out_idx); /* output */
        if ( TRUE == spice_model.lut_intermediate_buffer->spice_model->dump_explicit_port_map) {
          fprintf(fp, ")");
        }
        fprintf(fp, ");\n");
        /* Free */
        my_free(buf_input_port);
        my_free(buf_output_port);
      }
      /* Update the counter */
      j = nextj;
      mux_basis_cnt++;
    } 
  }   
  /* Assert */
  assert(0 == nextlevel);
  assert(0 == out_idx);
  assert(mux_basis_cnt == spice_mux_arch.num_input - 1);

  /* Free */
  my_free(inter_buf_loc);

  return;
}

static 
void dump_verilog_cmos_mux_multilevel_structure(FILE* fp, 
                                                char* mux_basis_subckt_name,
                                                char* mux_special_basis_subckt_name,
                                                t_spice_model spice_model,
                                                t_spice_mux_arch spice_mux_arch,
                                                int num_sram_port, t_spice_model_port** sram_port,
                                                bool is_explicit_mapping) {
  int i, j, level, nextlevel, sram_idx;
  int out_idx;
  int mux_basis_cnt = 0;
  int special_basis_cnt = 0;
  int cur_num_input_basis = 0;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  mux_basis_cnt = 0;
  assert((2 == spice_mux_arch.num_input_basis)||(2 < spice_mux_arch.num_input_basis));
  for (i = 0; i < spice_mux_arch.num_level; i++) {
    level = spice_mux_arch.num_level - i;
    nextlevel = spice_mux_arch.num_level - i - 1; 
    sram_idx = nextlevel * spice_mux_arch.num_input_basis;
    /* Check */
    assert(nextlevel > -1);
    fprintf(fp, "wire [%d:%d] mux2_l%d_in; \n", 
            0, spice_mux_arch.num_input_per_level[nextlevel] -1, /* input0 input1 */
            level);
  }
  fprintf(fp, "wire [%d:%d] mux2_l%d_in; \n",
          0, 0, 0);

  if (TRUE == spice_model.design_tech_info.mux_info->local_encoder) {
    /* Print local wires for local encoders */
    fprintf(fp, "wire [0:%d] %s_data;\n", 
            spice_mux_arch.num_level * spice_mux_arch.num_input_basis - 1,
            sram_port[0]->prefix);
    fprintf(fp, "wire [0:%d] %s_data_inv;\n", 
            spice_mux_arch.num_level * spice_mux_arch.num_input_basis - 1,
            sram_port[0]->prefix);
  }
 
  for (i = 0; i < spice_mux_arch.num_level; i++) {
    level = spice_mux_arch.num_level - i;
    nextlevel = spice_mux_arch.num_level - i - 1; 
    sram_idx = nextlevel * spice_mux_arch.num_input_basis;
    /* Check */
    assert(nextlevel > -1);
    /* Determine the number of input of this basis */
    cur_num_input_basis = spice_mux_arch.num_input_basis;
    /* Instanciate local encoder circuit here */
    if (TRUE == spice_model.design_tech_info.mux_info->local_encoder) {
      /* Get the number of inputs */
      int num_outputs = cur_num_input_basis;
      int num_inputs =  determine_mux_local_encoder_num_inputs(num_outputs);
      /* Find the decoder name */
      fprintf(fp, "%s %s_%d_ (", 
              generate_verilog_decoder_subckt_name(num_inputs, num_outputs),
              generate_verilog_decoder_subckt_name(num_inputs, num_outputs),
              i);
      if (true == is_explicit_mapping) {
        fprintf(fp, ".addr(%s[%d:%d]), .data(%s_data[%d:%d]), .data_inv(%s_data_inv[%d:%d]) );\n", 
                sram_port[0]->prefix, nextlevel * num_inputs, (nextlevel + 1) * num_inputs - 1,
                sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis - 1,
                sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis - 1);
      } else {
        fprintf(fp, "%s[%d:%d], %s_data[%d:%d], %s_data_inv[%d:%d]);\n", 
                sram_port[0]->prefix, nextlevel * num_inputs, (nextlevel + 1) * num_inputs - 1,
                sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis - 1,
                sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis - 1);
      }
    }
    /* Print basis muxQto1 for each level*/
    for (j = 0; j < spice_mux_arch.num_input_per_level[nextlevel]; j = j + cur_num_input_basis) {
      /* output index */
      out_idx = j / spice_mux_arch.num_input_basis; 
      /* Determine the number of input of this basis */
      cur_num_input_basis = spice_mux_arch.num_input_basis;
      if ((j + cur_num_input_basis) > spice_mux_arch.num_input_per_level[nextlevel]) {
        cur_num_input_basis = find_spice_mux_arch_special_basis_size(spice_mux_arch);
        if (0 < cur_num_input_basis) {
          /* Print the special basis */
          fprintf(fp, "%s special_basis(", mux_special_basis_subckt_name);
          /* Dump global ports */
          if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE, my_bool_to_boolean(is_explicit_mapping), TRUE)) {
            fprintf(fp, ",\n");
          }
          if (true == is_explicit_mapping) {
            fprintf(fp, ".in(");
          }
          fprintf(fp, "mux2_l%d_in[%d:%d]", level, j, j + cur_num_input_basis - 1); /* input0 input1 */
          if (true == is_explicit_mapping) {
            fprintf(fp, "), .out(");
          } else {
            fprintf(fp, ", ");
          }
          fprintf(fp, "mux2_l%d_in[%d]", nextlevel, out_idx); /* output */
          if (true == is_explicit_mapping) {
            fprintf(fp, "), .mem(");
          } else {
            fprintf(fp, ", ");
          }
          if (TRUE == spice_model.design_tech_info.mux_info->local_encoder) {
            fprintf(fp, "%s_data[%d:%d]", 
                    sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis -1);
          } else {
            fprintf(fp, "%s[%d:%d]", 
                    sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis -1);
          }
          if (true == is_explicit_mapping) {
            fprintf(fp, "), .mem_inv(");
          } else {
            fprintf(fp, ", ");
          }
          if (TRUE == spice_model.design_tech_info.mux_info->local_encoder) {
            fprintf(fp, "%s_data_inv[%d:%d]", 
                    sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis -1);
          } else {
            fprintf(fp, "%s_inv[%d:%d]", 
                    sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis -1);
          }
          if (true == is_explicit_mapping) {
            fprintf(fp, ")");
          }
          fprintf(fp, ");\n");
          special_basis_cnt++;
        }
        continue;
      }
      /* Each basis muxQto1: <given_name> <input0> <input1> <output> <sram> <sram_inv> svdd sgnd <subckt_name> */
      fprintf(fp, "%s ", mux_basis_subckt_name); /* subckt_name */
      fprintf(fp, "mux_basis_no%d (", mux_basis_cnt); /* given_name */
      /* Dump global ports */
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE, my_bool_to_boolean(is_explicit_mapping), TRUE)) {
        fprintf(fp, ",\n");
      }
      if (true == is_explicit_mapping) {
        fprintf(fp, ".in(");
      }
      fprintf(fp, "mux2_l%d_in[%d:%d]", level, j, j + cur_num_input_basis - 1); /* input0 input1 */
      if (true == is_explicit_mapping) {
        fprintf(fp, "), .out(");
      } else {
        fprintf(fp, ", ");
      }
      fprintf(fp, "mux2_l%d_in[%d]", nextlevel, out_idx); /* output */
      /* Print number of sram bits for this basis */
      if (true == is_explicit_mapping) {
        fprintf(fp, "), .mem(");
      } else {
        fprintf(fp, ", ");
      }
      if (TRUE == spice_model.design_tech_info.mux_info->local_encoder) {
        fprintf(fp, "%s_data[%d:%d]", 
                sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis -1);
      } else {
        fprintf(fp, "%s[%d:%d]", 
                sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis -1);
      }
      if (true == is_explicit_mapping) {
        fprintf(fp, "), .mem_inv(");
      } else {
        fprintf(fp, ", ");
      }
      if (TRUE == spice_model.design_tech_info.mux_info->local_encoder) {
        fprintf(fp, "%s_data_inv[%d:%d]", 
                sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis -1);
      } else {
        fprintf(fp, "%s_inv[%d:%d]", 
                sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis -1);
      }
      if (true == is_explicit_mapping) {
        fprintf(fp, ")");
      }
      fprintf(fp, ");");
      fprintf(fp, "\n");
      /* Update the counter */
      mux_basis_cnt++;
    } 
  }   
  /* Assert */
  assert(0 == nextlevel);
  assert(0 == out_idx);
  assert((1 == special_basis_cnt)||(0 == special_basis_cnt));
  /* assert((mux_basis_cnt + special_basis_cnt) == (int)((spice_mux_arch.num_input - 1)/(spice_mux_arch.num_input_basis - 1)) + 1); */
 
  return;
}

static 
void dump_verilog_cmos_mux_onelevel_structure(FILE* fp, 
                                              char* mux_basis_subckt_name,
                                              t_spice_model spice_model,
                                              t_spice_mux_arch spice_mux_arch,
                                              int num_sram_port, t_spice_model_port** sram_port,
                                              bool is_explicit_mapping) {
  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  assert(SPICE_MODEL_DESIGN_CMOS == spice_model.design_tech);

  fprintf(fp, "wire [0:%d] mux2_l%d_in; \n", spice_mux_arch.num_input - 1, 1); /* input0  */
  fprintf(fp, "wire [0:%d] mux2_l%d_in; \n", 0, 0); /* output */

  /* Instanciate local encoder circuit here */
  if ( (TRUE == spice_model.design_tech_info.mux_info->local_encoder)
    && ( 2 < spice_mux_arch.num_input) ) {
    /* Get the number of inputs */
    int num_outputs = spice_mux_arch.num_input;
    int num_inputs = determine_mux_local_encoder_num_inputs(num_outputs);

    /* Print local wires for local encoders */
    fprintf(fp, "wire [0:%d] %s_data;\n", 
            spice_mux_arch.num_input - 1,
            sram_port[0]->prefix);
    fprintf(fp, "wire [0:%d] %s_data_inv;\n", 
            spice_mux_arch.num_input - 1,
            sram_port[0]->prefix);
    /* Find the decoder name */
    fprintf(fp, "%s %s_0_ (", 
            generate_verilog_decoder_subckt_name(num_inputs, num_outputs),
            generate_verilog_decoder_subckt_name(num_inputs, num_outputs));
    if (true == is_explicit_mapping) {
      fprintf(fp, ".addr(%s), .data(%s_data), .data_inv(%s_data_inv) );\n", 
              sram_port[0]->prefix,
              sram_port[0]->prefix,
              sram_port[0]->prefix);
    } else {
      fprintf(fp, "%s, %s_data, %s_data_inv);\n", 
              sram_port[0]->prefix,
              sram_port[0]->prefix,
              sram_port[0]->prefix);
    }
  } 

  fprintf(fp, "%s mux_basis (\n", mux_basis_subckt_name); /* given_name */
  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE, 
                                                     my_bool_to_boolean(is_explicit_mapping), TRUE)) {
    fprintf(fp, ",\n");
  }
  fprintf(fp, "//----- MUX inputs -----\n");
    if (true == is_explicit_mapping) {
      fprintf(fp, ".in(");
    }
  fprintf(fp, "mux2_l%d_in[0:%d]", 1, spice_mux_arch.num_input - 1); /* input0  */
    if (true == is_explicit_mapping) {
      fprintf(fp, "), .out(");
    } else {
      fprintf(fp, ", ");
    }
  fprintf(fp, "mux2_l%d_in[%d]", 0, 0); /* output */
    if (true == is_explicit_mapping) {
      fprintf(fp, "),");
    } else {
      fprintf(fp, ",");
    }
  fprintf(fp, "\n");
  fprintf(fp, "//----- SRAM ports -----\n");
  /* Special basis for 2-input MUX, there is only one configuration bit */
  if (2 == spice_mux_arch.num_input) {
    if (true == is_explicit_mapping) {
      fprintf(fp, ".mem(");
    }
    fprintf(fp, "%s[0:%d]", 
    sram_port[0]->prefix, 0); /* sram */ 
    if (true == is_explicit_mapping) {
      fprintf(fp, "), .mem_inv(");
    } else {
      fprintf(fp, ", ");
    }
    fprintf(fp, "%s_inv[0:%d]", 
    sram_port[0]->prefix, 0); /* sram_inv */
    if (true == is_explicit_mapping) {
      fprintf(fp, ")");
    }
  } else {
    if (true == is_explicit_mapping) {
      fprintf(fp, ".mem(");
    }
    if (TRUE == spice_model.design_tech_info.mux_info->local_encoder) {
      fprintf(fp, "%s_data[0:%d]", 
              sram_port[0]->prefix, spice_mux_arch.num_input - 1); /* sram */
    } else {
      fprintf(fp, "%s[0:%d]", 
              sram_port[0]->prefix, spice_mux_arch.num_input - 1); /* sram */
    }
    if (true == is_explicit_mapping) {
      fprintf(fp, "), .mem_inv(");
    } else {
      fprintf(fp, ", ");
    }
    if (TRUE == spice_model.design_tech_info.mux_info->local_encoder) {
      fprintf(fp, "%s_data_inv[0:%d]", 
              sram_port[0]->prefix, spice_mux_arch.num_input - 1); /* sram_inv */
    } else {
      fprintf(fp, "%s_inv[0:%d]", 
              sram_port[0]->prefix, spice_mux_arch.num_input - 1); /* sram_inv */
    }
    if (true == is_explicit_mapping) {
      fprintf(fp, ")");
    }
  }
  fprintf(fp, "\n");
  fprintf(fp, ");\n");
 
  return;
}

static 
void dump_verilog_cmos_mux_submodule(FILE* fp,
                                     int mux_size,
                                     t_spice_model spice_model,
                                     t_spice_mux_arch spice_mux_arch,
                                     bool is_explicit_mapping) {
  int i, num_conf_bits, iport, ipin, num_mode_bits;
  int num_input_port = 0;
  int num_output_port = 0;
  int num_sram_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
  t_spice_model_port** sram_port = NULL;

  int num_buf_input_port = 0;
  int num_buf_output_port = 0;

  t_spice_model_port** buf_input_port = NULL;
  t_spice_model_port** buf_output_port = NULL;

  enum e_spice_model_structure cur_mux_structure;

  /* Find the basis subckt*/
  char* mux_basis_subckt_name = NULL;
  char* mux_special_basis_subckt_name = NULL;

  mux_basis_subckt_name = generate_verilog_mux_basis_subckt_name(&spice_model, mux_size, verilog_mux_basis_posfix);

  mux_special_basis_subckt_name = generate_verilog_mux_basis_subckt_name(&spice_model, mux_size, verilog_mux_special_basis_posfix);
 
  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Ensure we have a CMOS MUX,
   * ATTENTION: support LUT as well
   */
  assert((SPICE_MODEL_MUX == spice_model.type)||(SPICE_MODEL_LUT == spice_model.type)); 
  assert(SPICE_MODEL_DESIGN_CMOS == spice_model.design_tech);

  /* Find the input port, output port, and sram port*/
  input_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  sram_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);

  /* Asserts*/
  if ((SPICE_MODEL_MUX == spice_model.type)
    || ((SPICE_MODEL_LUT == spice_model.type)
       && (FALSE == spice_model.design_tech_info.lut_info->frac_lut))) {
    assert(1 == num_input_port);
    assert(1 == num_output_port);
    assert(1 == num_sram_port);
    assert(1 == output_port[0]->size); 
  } else {
    assert((SPICE_MODEL_LUT == spice_model.type) 
           && (TRUE == spice_model.design_tech_info.lut_info->frac_lut));
    assert(1 == num_input_port);
    assert(2 == num_sram_port);
    for (iport = 0; iport < num_output_port; iport++) {
      assert(0 < output_port[iport]->size);
    }
  }

  /* Setup a reasonable frac_out level for the output port*/
  for (iport = 0; iport < num_output_port; iport++) {
    /* We always initialize the lut_frac_level when there is only 1 output!
     * It should be pointed the last level! 
     */
    if ((OPEN == output_port[iport]->lut_frac_level) 
       || (1 == num_output_port)) {
      output_port[iport]->lut_frac_level = spice_mux_arch.num_level;
    } 
  }

  /* Add Fracturable LUT outputs */

  /* We have two types of naming rules in terms of the usage of MUXes: 
   * 1. MUXes, the naming rule is <mux_spice_model_name>_<structure>_size<input_size>
   * 2. LUTs, the naming rule is <lut_spice_model_name>_mux_size<sram_port_size>
   */
  num_conf_bits = count_num_sram_bits_one_spice_model(&spice_model, 
                                                      mux_size);
  num_mode_bits = count_num_mode_bits_one_spice_model(&spice_model); 
  /* Knock out the SRAM bits for the mode selection, they are separated dealed */
  num_conf_bits = num_conf_bits - num_mode_bits;

  if (SPICE_MODEL_LUT == spice_model.type) {
    /* Special for LUT MUX */
    fprintf(fp, "//------ CMOS MUX info: spice_model_name= %s_MUX, size=%d -----\n", 
            spice_model.name, mux_size);
    fprintf(fp, "module %s_mux(\n", spice_model.name);
    /* Dump global ports */
    if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, TRUE, FALSE, FALSE, TRUE)) {
      fprintf(fp, ",\n");
    }
    /* Print input ports*/
    fprintf(fp, "input wire [0:%d] %s,\n", num_conf_bits - 1,  input_port[0]->prefix);
    /* Print output ports*/
    for (iport = 0; iport < num_output_port; iport++) {
      fprintf(fp, "output wire [0:%d] %s,\n", output_port[iport]->size - 1, output_port[iport]->prefix);
    }
    /* Print configuration ports*/
    /* The configuration port in MUX context is the input port in LUT context ! */
    fprintf(fp, "input wire [0:%d] %s,\n", 
            input_port[0]->size - 1, sram_port[0]->prefix);
    fprintf(fp, "input wire [0:%d] %s_inv\n", 
            input_port[0]->size - 1, sram_port[0]->prefix);
  } else {
    fprintf(fp, "//----- CMOS MUX info: spice_model_name=%s, size=%d, structure: %s -----\n", 
            spice_model.name, mux_size, gen_str_spice_model_structure(spice_model.design_tech_info.mux_info->structure));
    fprintf(fp, "module %s (\n", 
            gen_verilog_one_mux_module_name(&spice_model, mux_size));
    /* Print input ports*/
    fprintf(fp, "input wire [0:%d] %s,\n", mux_size - 1,  input_port[0]->prefix);
    /* Print output ports*/
    fprintf(fp, "output wire [0:%d] %s,\n", output_port[0]->size - 1, output_port[0]->prefix);
    /* Print configuration ports*/
    fprintf(fp, "input wire [0:%d] %s,\n", 
            num_conf_bits - 1, sram_port[0]->prefix);
    fprintf(fp, "input wire [0:%d] %s_inv\n", 
            num_conf_bits - 1, sram_port[0]->prefix);
  }

  /* Print local vdd and gnd*/
  fprintf(fp, ");");
  fprintf(fp, "\n");

  /* Handle the corner case: input size = 2 */
  cur_mux_structure = spice_model.design_tech_info.mux_info->structure;
  if (2 == spice_mux_arch.num_input) {
    cur_mux_structure = SPICE_MODEL_STRUCTURE_ONELEVEL;
  }

  /* Print internal architecture*/ 
  switch (cur_mux_structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    dump_verilog_cmos_mux_tree_structure(fp, mux_basis_subckt_name, 
                                         spice_model, spice_mux_arch, 
                                         num_sram_port, sram_port, is_explicit_mapping);
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    dump_verilog_cmos_mux_onelevel_structure(fp, mux_basis_subckt_name, 
                                             spice_model, spice_mux_arch, 
                                             num_sram_port, sram_port, is_explicit_mapping);
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    dump_verilog_cmos_mux_multilevel_structure(fp, mux_basis_subckt_name, mux_special_basis_subckt_name,
                                               spice_model, spice_mux_arch, num_sram_port, sram_port,
                                               is_explicit_mapping);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, spice_model.name);
    exit(1);
  }

  /* To connect the input ports*/
  for (i = 0; i < mux_size; i++) {
    if (1 == spice_model.input_buffer->exist) {
      /* Find the input port, output port, and sram port*/
      buf_input_port = find_spice_model_ports(spice_model.input_buffer->spice_model, SPICE_MODEL_PORT_INPUT, &num_buf_input_port, TRUE);
      buf_output_port = find_spice_model_ports(spice_model.input_buffer->spice_model, SPICE_MODEL_PORT_OUTPUT, &num_buf_output_port, TRUE);
      /* Check */
      assert ( (1 == num_buf_input_port)
               &&(1 == buf_input_port[0]->size));
      assert ( (1 == num_buf_output_port)
               &&(1 == buf_output_port[0]->size));

      /* TODO: what about tapered buffer, can we support? */
      /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "%s %s_%d_ (", 
              spice_model.input_buffer->spice_model_name,
              spice_model.input_buffer->spice_model_name, i); /* Given name*/
      /* Dump global ports */
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.input_buffer->spice_model, FALSE, FALSE, TRUE, TRUE)) {
        fprintf(fp, ",\n");
      }
      /* Dump explicit port map if required */
      if ( TRUE == spice_model.input_buffer->spice_model->dump_explicit_port_map) {
        fprintf(fp, ".%s(", 
                buf_input_port[0]->lib_name); 
      }
      fprintf(fp, "%s[%d]", input_port[0]->prefix, i); /* input port */ 
      if ( TRUE == spice_model.input_buffer->spice_model->dump_explicit_port_map) {
        fprintf(fp, ")");
      }
      fprintf(fp, ", ");
      /* Dump explicit port map if required */
      if ( TRUE == spice_model.input_buffer->spice_model->dump_explicit_port_map) {
        fprintf(fp, ".%s(", 
                buf_output_port[0]->lib_name); 
      }
      fprintf(fp, "mux2_l%d_in[%d] ", spice_mux_arch.input_level[i], spice_mux_arch.input_offset[i]); /* output port*/
      if ( TRUE == spice_model.input_buffer->spice_model->dump_explicit_port_map) {
        fprintf(fp, ")");
      }
      fprintf(fp, ");\n");
      /* Free */
      my_free(buf_input_port);
      my_free(buf_output_port);
    } else {
      /* There is no buffer, I create a zero resisitance between*/
      /* Resistance R<given_name> <input> <output> 0*/
      fprintf(fp, "assign %s[%d] = mux2_l%d_in[%d];\n", 
              input_port[0]->prefix, i, spice_mux_arch.input_level[i], 
              spice_mux_arch.input_offset[i]);
    }
  }
  /* Special: for the last inputs, we connect to VDD|GND 
   * TODO: create an option to select the connection VDD or GND  
   */
  if ((SPICE_MODEL_MUX == spice_model.type)
     && (TRUE == spice_model.design_tech_info.mux_info->add_const_input)) { 
    assert ( (0 == spice_model.design_tech_info.mux_info->const_input_val) 
            || (1 == spice_model.design_tech_info.mux_info->const_input_val) );
    fprintf(fp, "assign mux2_l%d_in[%d] = 1'b%d;\n", 
            spice_mux_arch.input_level[spice_mux_arch.num_input - 1], 
            spice_mux_arch.input_offset[spice_mux_arch.num_input - 1], 
            spice_model.design_tech_info.mux_info->const_input_val);
  }

  /* Output buffer*/
  for (iport = 0; iport < num_output_port; iport++) {
    for (ipin = 0; ipin < output_port[iport]->size; ipin++) {
      if (1 == spice_model.output_buffer->exist) {
        /* Find the input port, output port, and sram port*/
        buf_input_port = find_spice_model_ports(spice_model.input_buffer->spice_model, SPICE_MODEL_PORT_INPUT, &num_buf_input_port, TRUE);
        buf_output_port = find_spice_model_ports(spice_model.input_buffer->spice_model, SPICE_MODEL_PORT_OUTPUT, &num_buf_output_port, TRUE);
        /* Check */
        assert ( (1 == num_buf_input_port)
                 &&(1 == buf_input_port[0]->size));
        assert ( (1 == num_buf_output_port)
                 &&(1 == buf_output_port[0]->size));

        /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
        fprintf(fp, "%s %s_out_%d_%d (",
                spice_model.output_buffer->spice_model_name,
                spice_model.output_buffer->spice_model_name,
                iport, ipin); /* subckt name */
        /* Dump global ports */
        if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.output_buffer->spice_model, FALSE, FALSE, TRUE, TRUE)) {
          fprintf(fp, ",\n");
        }
        /* check */
        assert ( -1 < spice_mux_arch.num_level - output_port[iport]->lut_frac_level );
        /* Dump explicit port map if required */
        if ( TRUE == spice_model.output_buffer->spice_model->dump_explicit_port_map) {
          fprintf(fp, ".%s(", 
                  buf_input_port[0]->lib_name); 
        }
        fprintf(fp, "mux2_l%d_in[%d]",
                  spice_mux_arch.num_level - output_port[iport]->lut_frac_level, 
                  output_port[iport]->lut_output_mask[ipin]); /* input port */ 
        if ( TRUE == spice_model.output_buffer->spice_model->dump_explicit_port_map) {
          fprintf(fp, ")");
        }
        fprintf(fp, ", ");
        /* Dump explicit port map if required */
        if ( TRUE == spice_model.output_buffer->spice_model->dump_explicit_port_map) {
          fprintf(fp, ".%s(", 
                  buf_output_port[0]->lib_name); 
        }
        fprintf(fp, "%s[%d]", output_port[iport]->prefix, ipin); /* Output port*/
        if ( TRUE == spice_model.output_buffer->spice_model->dump_explicit_port_map) {
          fprintf(fp, ")");
        }
        fprintf(fp, ");\n");
        /* Free */
        my_free(buf_input_port);
        my_free(buf_output_port);
      } else {
        /* check */
        assert ( -1 < spice_mux_arch.num_level - output_port[iport]->lut_frac_level );
        /* There is no buffer, I create a zero resisitance between*/
        /* Resistance R<given_name> <input> <output> 0*/
        fprintf(fp, "assign mux2_l%d_in[%d] = %s[%d];\n",
                spice_mux_arch.num_level - output_port[iport]->lut_frac_level,
                output_port[iport]->lut_output_mask[ipin],
                output_port[iport]->prefix, ipin);
      }
    }
  }

  fprintf(fp, "endmodule\n");
  fprintf(fp, "//----- END CMOS MUX info: spice_model_name=%s, size=%d -----\n\n", spice_model.name, mux_size);
  fprintf(fp, "\n");

  /* Free */
  my_free(mux_basis_subckt_name);
  my_free(mux_special_basis_subckt_name);
  my_free(input_port);
  my_free(output_port);
  my_free(sram_port);

  return;
}

/* Print the RRAM MUX SPICE model.
 * The internal structures of CMOS and RRAM MUXes are similar. 
 * This one can be merged to CMOS function.
 * However I use another function, because in future the internal structure may change.
 * We will suffer less software problems.
 */
static 
void dump_verilog_rram_mux_tree_structure(FILE* fp, 
                                          char* mux_basis_subckt_name,
                                          t_spice_model spice_model,
                                          t_spice_mux_arch spice_mux_arch,
                                          int num_sram_port, t_spice_model_port** sram_port) {
  int i, j, level, nextlevel;
  int nextj, out_idx;
  int mux_basis_cnt = 0;
  int cur_mem_lsb = 0;
  int cur_mem_msb = 0;

  assert(SPICE_MODEL_DESIGN_RRAM == spice_model.design_tech);

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  for (i = 0; i < spice_mux_arch.num_level; i++) {
    level = spice_mux_arch.num_level - i;
    nextlevel = spice_mux_arch.num_level - i - 1; 
    /* Check */
    assert(nextlevel > -1);
    fprintf(fp, "wire [%d:%d] mux2_l%d_in; \n", 
            0, spice_mux_arch.num_input_per_level[nextlevel] -1, /* input0 input1 */
            level);
  }
  fprintf(fp, "wire [%d:%d] mux2_l%d_in; \n",
          0, 0, 0);

  mux_basis_cnt = 0;
  for (i = 0; i < spice_mux_arch.num_level; i++) {
    level = spice_mux_arch.num_level - i;
    nextlevel = spice_mux_arch.num_level - i - 1; 
    /* Check */
    assert(nextlevel > -1);
    /* Print basis mux2to1 for each level*/
    for (j = 0; j < spice_mux_arch.num_input_per_level[nextlevel]; j++) {
      nextj = j + 1;
      out_idx = j/2; 
      cur_mem_lsb = cur_mem_msb;
      cur_mem_msb += 6; 
      /* Each basis mux2to1: <given_name> <input0> <input1> <output> <sram> <sram_inv> svdd sgnd <subckt_name> */
      fprintf(fp, "%s mux_basis_no%d (", mux_basis_subckt_name, mux_basis_cnt); /* given_name */
      /* Dump global ports */
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE, FALSE, TRUE)) {
        fprintf(fp, ",\n");
      }
      fprintf(fp, "mux2_l%d_in[%d:%d], ", level, j, nextj); /* input0 input1 */
      fprintf(fp, "mux2_l%d_in[%d], ", nextlevel, out_idx); /* output */
      fprintf(fp, "%s[%d:%d] %s_inv[%d:%d]);\n", 
              sram_port[0]->prefix, cur_mem_lsb, cur_mem_msb - 1,
              sram_port[0]->prefix, cur_mem_lsb, cur_mem_msb - 1); /* sram sram_inv */
      /* Update the counter */
      j = nextj;
      mux_basis_cnt++;
    } 
  }   
  /* Assert */
  assert(0 == nextlevel);
  assert(0 == out_idx);
  assert(mux_basis_cnt == spice_mux_arch.num_input - 1);
  assert(cur_mem_msb == 6 * spice_mux_arch.num_level); 

  return;
}

static 
void dump_verilog_rram_mux_multilevel_structure(FILE* fp, 
                                                char* mux_basis_subckt_name,
                                                char* mux_special_basis_subckt_name,
                                                t_spice_model spice_model,
                                                t_spice_mux_arch spice_mux_arch,
                                                int num_sram_port, t_spice_model_port** sram_port) {
  int i, j, level, nextlevel;
  int out_idx;
  int mux_basis_cnt = 0;
  int special_basis_cnt = 0;
  int cur_num_input_basis = 0;

  int cur_mem_lsb = 0;
  int cur_mem_msb = 0;

  assert(SPICE_MODEL_DESIGN_RRAM == spice_model.design_tech);

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  for (i = 0; i < spice_mux_arch.num_level; i++) {
    level = spice_mux_arch.num_level - i;
    nextlevel = spice_mux_arch.num_level - i - 1; 
    /* Check */
    assert(nextlevel > -1);
    fprintf(fp, "wire [%d:%d] mux2_l%d_in; \n", 
            0, spice_mux_arch.num_input_per_level[nextlevel] -1, /* input0 input1 */
            level);
  }
  fprintf(fp, "wire [%d:%d] mux2_l%d_in; \n",
          0, 0, 0);

  mux_basis_cnt = 0;
  assert((2 == spice_mux_arch.num_input_basis)||(2 < spice_mux_arch.num_input_basis));
  for (i = 0; i < spice_mux_arch.num_level; i++) {
    level = spice_mux_arch.num_level - i;
    nextlevel = spice_mux_arch.num_level - i - 1; 
    /* Check */
    assert(nextlevel > -1);
    /* Memory port offset update */
    cur_mem_lsb = cur_mem_msb;
    /* Print basis muxQto1 for each level*/
    for (j = 0; j < spice_mux_arch.num_input_per_level[nextlevel]; j = j+cur_num_input_basis) {
      /* output index */
      out_idx = j/spice_mux_arch.num_input_basis; 
      /* Determine the number of input of this basis */
      cur_num_input_basis = spice_mux_arch.num_input_basis;
      cur_mem_msb = cur_mem_lsb + (cur_num_input_basis + 1);
      if ((j + cur_num_input_basis) > spice_mux_arch.num_input_per_level[nextlevel]) {
        cur_num_input_basis = find_spice_mux_arch_special_basis_size(spice_mux_arch);
        if (0 < cur_num_input_basis) {
          /* Print the special basis */
          fprintf(fp, "%s special_basis(\n", mux_special_basis_subckt_name);
          /* Dump global ports */
          if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE, FALSE, TRUE)) {
            fprintf(fp, ",\n");
          }
          fprintf(fp, "mux2_l%d_in[%d:%d], ", level, j, j + cur_num_input_basis - 1); /* inputs */
          fprintf(fp, "mux2_l%d_in[%d], ", nextlevel, out_idx); /* output */
          cur_mem_msb = cur_mem_lsb + (cur_num_input_basis + 1);
          fprintf(fp, "%s[%d:%d], %s_inv[%d,%d]", 
                  sram_port[0]->prefix, cur_mem_lsb, cur_mem_msb - 1,  
                  sram_port[0]->prefix, cur_mem_lsb, cur_mem_msb - 1); /* sram sram_inv */
          fprintf(fp, ");\n");
          special_basis_cnt++;
          continue;
        }
      }
      /* Each basis muxQto1: <given_name> <input0> <input1> <output> <sram> <sram_inv> svdd sgnd <subckt_name> */
      fprintf(fp, "%s ", mux_basis_subckt_name); /* subckt_name */
      fprintf(fp, "mux_basis_no%d (", mux_basis_cnt); /* given_name */
      /* Dump global ports */
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE, FALSE, TRUE)) {
        fprintf(fp, ",\n");
      }
      fprintf(fp, "mux2_l%d_in[%d:%d], ", level, j, j + cur_num_input_basis - 1); /* input0 input1 */
      fprintf(fp, "mux2_l%d_in[%d], ", nextlevel, out_idx); /* output */
      /* Print number of sram bits for this basis */
      fprintf(fp, "%s[%d:%d], %s_inv[%d:%d]", 
                  sram_port[0]->prefix, cur_mem_lsb, cur_mem_msb - 1,  
                  sram_port[0]->prefix, cur_mem_lsb, cur_mem_msb - 1); /* sram sram_inv */
      fprintf(fp, ");\n");
      /* Update the counter */
      mux_basis_cnt++;
    } 
  }   
  /* Assert */
  assert(0 == nextlevel);
  assert(0 == out_idx);
  assert((1 == special_basis_cnt)||(0 == special_basis_cnt));
  /* assert((mux_basis_cnt + special_basis_cnt) == (int)((spice_mux_arch.num_input - 1)/(spice_mux_arch.num_input_basis - 1)) + 1); */
 
  /* Free */

  return;
}

static 
void dump_verilog_rram_mux_onelevel_structure(FILE* fp, 
                                              char* mux_basis_subckt_name,
                                              t_spice_model spice_model,
                                              t_spice_mux_arch spice_mux_arch,
                                              int num_sram_port, t_spice_model_port** sram_port) {
  int num_conf_bits;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  assert(SPICE_MODEL_DESIGN_RRAM == spice_model.design_tech);

  fprintf(fp, "wire [0:%d] mux2_l%d_in; \n", spice_mux_arch.num_input - 1, 1); /* input0  */
  fprintf(fp, "wire [0:%d] mux2_l%d_in; \n", 0, 0); /* output */

  fprintf(fp, "%s mux_basis (\n", mux_basis_subckt_name); /* given_name */
  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE, FALSE, TRUE)) {
    fprintf(fp, ",\n");
  }
  fprintf(fp, "//----- MUX inputs -----\n");
  fprintf(fp, "mux2_l%d_in[0:%d],\n ", 1, spice_mux_arch.num_input - 1); /* inputs  */
  fprintf(fp, "mux2_l%d_in[%d],\n", 0, 0); /* output */
  fprintf(fp, "//----- SRAM ports -----\n");
  num_conf_bits = count_num_sram_bits_one_spice_model(&spice_model, 
                                                      spice_mux_arch.num_input);
  fprintf(fp, "%s[0:%d], %s_inv[0:%d]", 
            sram_port[0]->prefix, num_conf_bits - 1, 
            sram_port[0]->prefix, num_conf_bits - 1); /* sram sram_inv */
  fprintf(fp, "\n");
  fprintf(fp, ");\n");
 
  return;
}

static 
void dump_verilog_rram_mux_submodule(FILE* fp,
                                     int mux_size,
                                     t_spice_model spice_model,
                                     t_spice_mux_arch spice_mux_arch,
                                     bool is_explicit_mapping) {
  int i, num_conf_bits;
  int num_input_port = 0;
  int num_output_port = 0;
  int num_sram_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
  t_spice_model_port** sram_port = NULL;

  int num_buf_input_port = 0;
  int num_buf_output_port = 0;

  t_spice_model_port** buf_input_port = NULL;
  t_spice_model_port** buf_output_port = NULL;

  /* Find the basis subckt*/
  char* mux_basis_subckt_name = NULL;
  char* mux_special_basis_subckt_name = NULL;

  mux_basis_subckt_name = generate_verilog_mux_basis_subckt_name(&spice_model, mux_size, verilog_mux_basis_posfix);

  mux_special_basis_subckt_name = generate_verilog_mux_basis_subckt_name(&spice_model, mux_size, verilog_mux_special_basis_posfix);
 
  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Ensure we have a RRAM MUX*/
  assert((SPICE_MODEL_MUX == spice_model.type)||(SPICE_MODEL_LUT == spice_model.type)); 
  assert(SPICE_MODEL_DESIGN_RRAM == spice_model.design_tech);

  /* Find the input port, output port, and sram port*/
  input_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  sram_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);

  /* Asserts*/
  assert(1 == num_input_port);
  assert(1 == num_output_port);
  assert(1 == num_sram_port);
  assert(1 == output_port[0]->size);

  /* Print the definition of subckt*/
  if (SPICE_MODEL_LUT == spice_model.type) {
    /* RRAM LUT is not supported now... */
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])RRAM LUT is not supported!\n",
               __FILE__, __LINE__);
    exit(1);
    /* Special for LUT MUX*/
    /*
    fprintf(fp, "***** RRAM MUX info: spice_model_name= %s_MUX, size=%d *****\n", spice_model.name, mux_size);
    fprintf(fp, ".subckt %s_mux_size%d ", spice_model.name, mux_size);
    */
  } else {
    fprintf(fp, "//----- RRAM MUX info: spice_model_name=%s, size=%d, structure: %s -----\n", 
            spice_model.name, mux_size, gen_str_spice_model_structure(spice_model.design_tech_info.mux_info->structure));
    fprintf(fp, "module %s ( \n",
            gen_verilog_one_mux_module_name(&spice_model, mux_size));
  }
  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, TRUE, FALSE, FALSE, TRUE)) {
    fprintf(fp, ",\n");
  }
  /* Print input ports*/
  fprintf(fp, "input wire [0:%d] %s,\n ", mux_size - 1, input_port[0]->prefix);
  /* Print output ports*/
  fprintf(fp, "output wire %s,\n ", output_port[0]->prefix);
  /* Print configuration ports */
  num_conf_bits = count_num_sram_bits_one_spice_model(&spice_model, 
                                                      mux_size);
  fprintf(fp, "input wire [0:%d] %s,\n", 
          num_conf_bits - 1, sram_port[0]->prefix);
  fprintf(fp, "input wire [0:%d] %s_inv\n", 
          num_conf_bits - 1, sram_port[0]->prefix);
  /* Print local vdd and gnd*/
  fprintf(fp, ");\n");
  
  /* Print internal architecture*/ 
  /* RRAM MUX is optimal in terms of area, delay and power for one-level structure.
   */
  switch (spice_model.design_tech_info.mux_info->structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    dump_verilog_rram_mux_tree_structure(fp, mux_basis_subckt_name, 
                                         spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    dump_verilog_rram_mux_multilevel_structure(fp, mux_basis_subckt_name, mux_special_basis_subckt_name,
                                               spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    dump_verilog_rram_mux_onelevel_structure(fp, mux_basis_subckt_name, 
                                             spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid structure for spice model (%s)!\n",
               __FILE__, __LINE__, spice_model.name);
    exit(1);
  }

  /* To connect the input ports*/
  for (i = 0; i < mux_size; i++) {
    if (1 == spice_model.input_buffer->exist) {
      /* Find the input port, output port, and sram port*/
      buf_input_port = find_spice_model_ports(spice_model.input_buffer->spice_model, SPICE_MODEL_PORT_INPUT, &num_buf_input_port, TRUE);
      buf_output_port = find_spice_model_ports(spice_model.input_buffer->spice_model, SPICE_MODEL_PORT_OUTPUT, &num_buf_output_port, TRUE);
      /* Check */
      assert ( (1 == num_buf_input_port)
               &&(1 == buf_input_port[0]->size));
      assert ( (1 == num_buf_output_port)
               &&(1 == buf_output_port[0]->size));

      /* Each inv: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "%s %s%d (", 
                  spice_model.input_buffer->spice_model_name, 
                  spice_model.input_buffer->spice_model_name, i); /* Given name*/
      /* Dump global ports */
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.input_buffer->spice_model, FALSE, FALSE, TRUE, TRUE)) {
        fprintf(fp, ",\n");
      }
      /* Dump explicit port map if required */
      if ( TRUE == spice_model.input_buffer->spice_model->dump_explicit_port_map) {
        fprintf(fp, ".%s(", 
                buf_input_port[0]->lib_name); 
      }
      fprintf(fp, "%s[%d]", input_port[0]->prefix, i); /* input port */ 
      if ( TRUE == spice_model.input_buffer->spice_model->dump_explicit_port_map) {
        fprintf(fp, ")");
      } 
      fprintf(fp, ", ");
  
      /* Dump explicit port map if required */
      if ( TRUE == spice_model.input_buffer->spice_model->dump_explicit_port_map) {
        fprintf(fp, ".%s(", 
                buf_output_port[0]->lib_name); 
      }
      fprintf(fp, "mux2_l%d_in[%d]", spice_mux_arch.input_level[i], spice_mux_arch.input_offset[i]); /* output port*/
      if ( TRUE == spice_model.input_buffer->spice_model->dump_explicit_port_map) {
        fprintf(fp, ")");
      } 
      fprintf(fp, ");\n");
      /* Free */
      my_free(buf_input_port);
      my_free(buf_output_port);
    } else {
      /* There is no buffer, I create a zero resisitance between*/
      /* Resistance R<given_name> <input> <output> 0*/
      fprintf(fp, "assign %s[%d] = mux2_l%d_in[%d];\n", 
              input_port[0]->prefix, i, spice_mux_arch.input_level[i], 
              spice_mux_arch.input_offset[i]);
    }
  }

  /* Special: for the last inputs, we connect to VDD|GND 
   * TODO: create an option to select the connection VDD or GND  
   */
  if ((SPICE_MODEL_MUX == spice_model.type)
     && (TRUE == spice_model.design_tech_info.mux_info->add_const_input)) { 
    assert ( (0 == spice_model.design_tech_info.mux_info->const_input_val) 
            || (1 == spice_model.design_tech_info.mux_info->const_input_val) );
    fprintf(fp, "assign mux2_l%d_in[%d] = 1'b%d;\n", 
            spice_mux_arch.input_level[spice_mux_arch.num_input], 
            spice_mux_arch.input_offset[spice_mux_arch.num_input], spice_model.design_tech_info.mux_info->const_input_val);
  }

  /* Output buffer*/
  if (1 == spice_model.output_buffer->exist) {
    /* Find the input port, output port, and sram port*/
    buf_input_port = find_spice_model_ports(spice_model.output_buffer->spice_model, SPICE_MODEL_PORT_INPUT, &num_buf_input_port, TRUE);
    buf_output_port = find_spice_model_ports(spice_model.output_buffer->spice_model, SPICE_MODEL_PORT_OUTPUT, &num_buf_output_port, TRUE);
    /* Check */
    assert ( (1 == num_buf_input_port)
             &&(1 == buf_input_port[0]->size));
    assert ( (1 == num_buf_output_port)
             &&(1 == buf_output_port[0]->size));
    /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
    fprintf(fp, "%s %s_out (",
            spice_model.output_buffer->spice_model_name,
            spice_model.output_buffer->spice_model_name); /* subckt name */
    /* Dump global ports */
    if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.output_buffer->spice_model, FALSE, FALSE, TRUE, TRUE)) {
      fprintf(fp, ",\n");
    }
    /* Dump explicit port map if required */
    if ( TRUE == spice_model.output_buffer->spice_model->dump_explicit_port_map) {
      fprintf(fp, ".%s(", 
              buf_input_port[0]->lib_name); 
    }
    fprintf(fp, "mux2_l%d_in[%d]", 0 , 0); /* input port */ 
    if ( TRUE == spice_model.output_buffer->spice_model->dump_explicit_port_map) {
      fprintf(fp, ")");
    } 
    fprintf(fp, ", ");

    /* Dump explicit port map if required */
    if ( TRUE == spice_model.output_buffer->spice_model->dump_explicit_port_map) {
      fprintf(fp, ".%s(", 
              buf_output_port[0]->lib_name); 
    }
    fprintf(fp, "%s", output_port[0]->prefix); /* Output port*/
    if ( TRUE == spice_model.output_buffer->spice_model->dump_explicit_port_map) {
      fprintf(fp, ")");
    } 
    fprintf(fp, ");\n");
    /* Free */
    my_free(buf_input_port);
    my_free(buf_output_port);
  } else {
    /* There is no buffer, I create a zero resisitance between*/
    /* Resistance R<given_name> <input> <output> 0*/
    fprintf(fp, "assign mux2_l0_in[0] %s;\n", output_port[0]->prefix);
  }
 
  fprintf(fp, "endmodule\n");
  fprintf(fp, "//------ END RRAM MUX info: spice_model_name=%s, size=%d -----\n\n", spice_model.name, mux_size);
  fprintf(fp, "\n");

  /* Free */
  my_free(mux_basis_subckt_name);
  my_free(mux_special_basis_subckt_name);
  my_free(input_port);
  my_free(output_port);
  my_free(sram_port);

  return;
}

/* Dump a memory submodule for the MUX */
static 
void dump_verilog_cmos_mux_mem_submodule(FILE* fp,
                                         int mux_size,
                                         t_spice_model spice_model,
                                         t_spice_mux_arch spice_mux_arch,
                                         bool is_explicit_mapping) {
  int i, num_conf_bits;

  int num_sram_port = 0;
  t_spice_model_port** sram_port = NULL;

  /* Find the basis subckt*/
  char* mux_mem_subckt_name = NULL;
  t_spice_model* mem_model = NULL;
 
  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  /* We only do this for MUX not LUT
   * LUT memory block added at top-level 
   */
  assert((SPICE_MODEL_MUX == spice_model.type)||(SPICE_MODEL_LUT == spice_model.type)); 
  if (SPICE_MODEL_LUT == spice_model.type) {
    return;
  }

  /* Ensure we have a CMOS MUX */
  assert(SPICE_MODEL_DESIGN_CMOS == spice_model.design_tech);

  /* Generate subckt name */
  mux_mem_subckt_name = generate_verilog_mux_subckt_name(&spice_model, mux_size, verilog_mem_posfix);

  /* Get SRAM port */
  sram_port = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);

  /* Asserts*/
  assert ((1 == num_sram_port) && (NULL != sram_port));
  assert (NULL != sram_port[0]->spice_model);
  assert ((SPICE_MODEL_SCFF == sram_port[0]->spice_model->type) 
       || (SPICE_MODEL_SRAM == sram_port[0]->spice_model->type));

  /* Get the memory model */
  mem_model = sram_port[0]->spice_model;

  /* We have two types of naming rules in terms of the usage of MUXes: 
   * 1. MUXes, the naming rule is <mux_spice_model_name>_<structure>_size<input_size>
   * 2. LUTs, the naming rule is <lut_spice_model_name>_mux_size<sram_port_size>
   */
  num_conf_bits = count_num_sram_bits_one_spice_model(&spice_model, 
                                                      mux_size);

  fprintf(fp, "//----- CMOS MUX info: spice_model_name=%s, size=%d, structure: %s -----\n", 
            spice_model.name, mux_size, gen_str_spice_model_structure(spice_model.design_tech_info.mux_info->structure));
  fprintf(fp, "module %s (", mux_mem_subckt_name);
  /* Here we force the sequence of ports: of a memory subumodule:
   * 1. Global ports 
   * 2. input ports 
   * 3. output ports 
   * 4. bl/wl ports 
   */
  /* Local Encoding support */
  dump_verilog_mem_module_port_map(fp, mem_model, TRUE, 0, num_conf_bits, my_bool_to_boolean(is_explicit_mapping)); 
  fprintf(fp, ");\n");

  /* Dump all the submodules */
  for (i = 0 ; i < num_conf_bits; i++) { 
    fprintf(fp, "%s %s_%d_ ( ",
            mem_model->name, mem_model->prefix, i);
    dump_verilog_mem_module_port_map(fp, mem_model, FALSE, i, 1,
                                     mem_model->dump_explicit_port_map); 
    fprintf(fp, ");\n");
  }

  /* END of this submodule */
  fprintf(fp, "endmodule\n");

  /* Free */
  my_free(mux_mem_subckt_name);

  return;
}

/** Dump a verilog module for a MUX
 * We always dump a basis submodule for a MUX
 * whatever structure it is: one-level, two-level or multi-level
 */
static 
void dump_verilog_mux_mem_module(FILE* fp, 
                                 t_spice_mux_model* spice_mux_model,
                                 bool is_explicit_mapping) {
  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  }  
  /* Make sure we have a valid spice_model*/
  if (NULL == spice_mux_model) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid spice_mux_model!\n",__FILE__, __LINE__); 
    exit(1);
  }
  /* Make sure we have a valid spice_model*/
  if (NULL == spice_mux_model->spice_model) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid spice_model!\n",__FILE__, __LINE__); 
    exit(1);
  }

  /* Check the mux size */
  if (spice_mux_model->size < 2) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid MUX size(=%d)! Should be at least 2.\n",
               __FILE__, __LINE__, spice_mux_model->size); 
    exit(1);
  }

  /* Print the definition of subckt*/
  /* Check the design technology*/
  switch (spice_mux_model->spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    dump_verilog_cmos_mux_mem_submodule(fp, spice_mux_model->size,
                                        *(spice_mux_model->spice_model), 
                                        *(spice_mux_model->spice_mux_arch),
                                        is_explicit_mapping);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* We do not need a memory submodule for RRAM MUX,
     * RRAM are embedded in the datapath  
     */ 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of MUX(name: %s)\n",
               __FILE__, __LINE__, spice_mux_model->spice_model->name); 
    exit(1);
  }

  return;
}

/** Dump a verilog module for a MUX
 * We always dump a basis submodule for a MUX
 * whatever structure it is: one-level, two-level or multi-level
 */
static 
void dump_verilog_mux_module(FILE* fp, 
                             t_spice_mux_model* spice_mux_model,
                             bool is_explicit_mapping) {
  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  }  
  /* Make sure we have a valid spice_model*/
  if (NULL == spice_mux_model) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid spice_mux_model!\n",__FILE__, __LINE__); 
    exit(1);
  }
  /* Make sure we have a valid spice_model*/
  if (NULL == spice_mux_model->spice_model) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid spice_model!\n",__FILE__, __LINE__); 
    exit(1);
  }

  /* Check the mux size*/
  if (spice_mux_model->size < 2) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid MUX size(=%d)! Should be at least 2.\n",
               __FILE__, __LINE__, spice_mux_model->size); 
    exit(1);
  }

  /* Corner case: Error out  MUX_SIZE = 2, automatcially give a one-level structure */
  /*
  if ((2 == spice_mux_model->size)&&(SPICE_MODEL_STRUCTURE_ONELEVEL != spice_mux_model->spice_model->design_tech_info.structure)) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Structure of SPICE model (%s) should be one-level because it is linked to a 2:1 MUX!\n",
               __FILE__, __LINE__, spice_mux_model->spice_model->name);
    exit(1);
  }
  */

  /* Print the definition of subckt*/
  /* Check the design technology*/
  switch (spice_mux_model->spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    dump_verilog_cmos_mux_submodule(fp, spice_mux_model->size,
                                    *(spice_mux_model->spice_model), 
                                    *(spice_mux_model->spice_mux_arch),
                                    is_explicit_mapping);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    dump_verilog_rram_mux_submodule(fp, spice_mux_model->size,
                                    *(spice_mux_model->spice_model), 
                                    *(spice_mux_model->spice_mux_arch),
                                    is_explicit_mapping);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid design_technology of MUX(name: %s)\n",
               __FILE__, __LINE__, spice_mux_model->spice_model->name); 
    exit(1);
  }

  return;
}

/*** Top-level function *****/

/* We should count how many multiplexers with different sizes are needed */
static 
void dump_verilog_submodule_muxes(t_sram_orgz_info* cur_sram_orgz_info,
                                  char* verilog_dir,
                                  char* submodule_dir,
                                  int num_switch,
                                  t_switch_inf* switches,
                                  t_spice* spice,
                                  t_det_routing_arch* routing_arch,
                                  bool is_explicit_mapping) {
  
  /* Statisitcs for input sizes and structures of MUXes 
   * used in FPGA architecture 
   */
  /* We have linked list whichs stores spice model information of multiplexer*/
  t_llist* muxes_head = NULL; 
  t_llist* temp = NULL;
  int mux_cnt = 0;
  int max_mux_size = -1;
  int min_mux_size = -1;
  FILE* fp = NULL;
  char* verilog_name = my_strcat(submodule_dir,muxes_verilog_file_name);
  int num_input_ports = 0;
  t_spice_model_port** input_ports = NULL;
  int num_sram_ports = 0;
  t_spice_model_port** sram_ports = NULL;

  int num_input_basis = 0;
  t_spice_mux_model* cur_spice_mux_model = NULL;
  
  int max_routing_mux_size = -1;

  /* Alloc the muxes*/
  muxes_head = stats_spice_muxes(num_switch, switches, spice, routing_arch);

  /* Print the muxes netlist*/
  fp = fopen(verilog_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(FILE:%s,LINE[%d])Failure in create subckt SPICE netlist %s",
               __FILE__, __LINE__, verilog_name); 
    exit(1);
  } 
  /* Generate the descriptions*/
  dump_verilog_file_header(fp,"MUXes used in FPGA");

  verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Print mux netlist one by one*/
  temp = muxes_head;
  while(temp) {
    assert(NULL != temp->dptr);
    cur_spice_mux_model = (t_spice_mux_model*)(temp->dptr);
    /* Bypass the spice models who has a user-defined subckt */
    if (NULL != cur_spice_mux_model->spice_model->verilog_netlist) {
      input_ports = find_spice_model_ports(cur_spice_mux_model->spice_model, SPICE_MODEL_PORT_INPUT, &num_input_ports, TRUE);
      sram_ports = find_spice_model_ports(cur_spice_mux_model->spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_ports, TRUE);
      assert(0 != num_input_ports);
      assert(0 != num_sram_ports);
      /* Check the Input port size */
      if (cur_spice_mux_model->size != input_ports[0]->size) {
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File:%s,[LINE%d])User-defined MUX SPICE MODEL(%s) size(%d) unmatch with the architecture needs(%d)!\n",
                   __FILE__, __LINE__, cur_spice_mux_model->spice_model->name, input_ports[0]->size,cur_spice_mux_model->size);
        exit(1);
      }
      /* Check the SRAM port size */
      num_input_basis = determine_num_input_basis_multilevel_mux(cur_spice_mux_model->size, 
                                                                 cur_spice_mux_model->spice_model->design_tech_info.mux_info->mux_num_level);
      if ((num_input_basis * cur_spice_mux_model->spice_model->design_tech_info.mux_info->mux_num_level) != sram_ports[0]->size) {
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File:%s,[LINE%d])User-defined MUX SPICE MODEL(%s) SRAM size(%d) unmatch with the num of level(%d)!\n",
                   __FILE__, __LINE__, cur_spice_mux_model->spice_model->name, sram_ports[0]->size, cur_spice_mux_model->spice_model->design_tech_info.mux_info->mux_num_level*num_input_basis);
        exit(1);
      }
      /* Move on to the next*/
      temp = temp->next;
      continue;
    }
    /* Let's have a N:1 MUX as basis*/
    dump_verilog_mux_basis_module(fp, cur_spice_mux_model);
    /* Print the mux subckt */
    dump_verilog_mux_module(fp, cur_spice_mux_model, is_explicit_mapping);
    /* Update the statistics*/
    mux_cnt++;
    if ((-1 == max_mux_size)||(max_mux_size < cur_spice_mux_model->size)) {
      max_mux_size = cur_spice_mux_model->size;
    }
    if ((-1 == min_mux_size)||(min_mux_size > cur_spice_mux_model->size)) {
      min_mux_size = cur_spice_mux_model->size;
    }
    /* Exclude LUT MUX from this statistics */
    if ((SPICE_MODEL_MUX == cur_spice_mux_model->spice_model->type)
       &&((-1 == max_routing_mux_size)||(max_routing_mux_size < cur_spice_mux_model->size))) {
      max_routing_mux_size = cur_spice_mux_model->size;
    }
    /* Move on to the next*/
    temp = temp->next;
  }

  /* TODO: 
   * Scan-chain configuration circuit does not need any BLs/WLs! 
   * SRAM MUX does not need any reserved BL/WLs!
   */
  /* Determine reserved Bit/Word Lines if a memory bank is specified,
   * At least 1 BL/WL should be reserved! 
   */
  try_update_sram_orgz_info_reserved_blwl(cur_sram_orgz_info, 
                                          max_routing_mux_size, max_routing_mux_size);

  vpr_printf(TIO_MESSAGE_INFO,"Generated %d Multiplexer submodules.\n",
             mux_cnt);
  vpr_printf(TIO_MESSAGE_INFO,"Max. MUX size = %d.\t",
             max_mux_size);
  vpr_printf(TIO_MESSAGE_INFO,"Min. MUX size = %d.\n",
             min_mux_size);

  /* Add fname to the linked list */
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_name);  

  /* Close the file*/
  fclose(fp);

  /* remember to free the linked list*/
  free_muxes_llist(muxes_head);
  /* Free strings */
  free(verilog_name);

  return;
}

void dump_verilog_wire_module(FILE* fp,
                              char* wire_subckt_name,
                              t_spice_model verilog_model) {
  int num_input_port = 0;
  int num_output_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
 
  /* Ensure a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File handler.\n",
               __FILE__, __LINE__); 
    exit(1);
  } 
  /* Check the wire model*/
  assert(NULL != verilog_model.wire_param);
  assert(0 < verilog_model.wire_param->level);
  /* Find the input port, output port*/
  input_port = find_spice_model_ports(&verilog_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_port = find_spice_model_ports(&verilog_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);

  /* Asserts*/
  assert(1 == num_input_port);
  assert(1 == num_output_port);
  assert(1 == input_port[0]->size);
  assert(1 == output_port[0]->size);
  /* print the spice model*/
  fprintf(fp, "//-----Wire module, verilog_model_name=%s -----\n", verilog_model.name);  
  switch (verilog_model.type) {
  case SPICE_MODEL_CHAN_WIRE: 
    /* Add an output at middle point for connecting CB inputs */
    fprintf(fp, "module %s (\n", wire_subckt_name);
    /* Dump global ports */
    if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &verilog_model, TRUE, FALSE, FALSE, TRUE)) {
      fprintf(fp, ",\n");
    }
    fprintf(fp, "input wire %s, output wire %s, output wire mid_out);\n",
            input_port[0]->prefix, output_port[0]->prefix);
    fprintf(fp, "\tassign %s = %s;\n", output_port[0]->prefix, input_port[0]->prefix);
    fprintf(fp, "\tassign mid_out = %s;\n", input_port[0]->prefix);
    break;
  case SPICE_MODEL_WIRE: 
    /* Add an output at middle point for connecting CB inputs */
    fprintf(fp, "module %s (\n",
            wire_subckt_name);
    /* Dump global ports */
    if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &verilog_model, TRUE, FALSE, FALSE, TRUE)) {
      fprintf(fp, ",\n");
    }
    fprintf(fp, "input wire %s, output wire %s);\n",
            input_port[0]->prefix, output_port[0]->prefix);
    /* Direct shortcut */
    fprintf(fp, "\t\tassign %s = %s;\n", output_port[0]->prefix, input_port[0]->prefix);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of spice_model! Expect [chan_wire|wire].\n",
               __FILE__, __LINE__);
    exit(1);
  }
  
  /* Finish*/ 
  fprintf(fp, "endmodule\n");
  fprintf(fp, "//-----END Wire module, verilog_model_name=%s -----\n", verilog_model.name);  
  fprintf(fp, "\n");

  return;
}

/* Dump one module of a LUT */
static 
void dump_verilog_submodule_one_lut(FILE* fp, 
                                    t_spice_model* verilog_model,
                                    bool is_explicit_mapping) {
  int num_input_port = 0;
  int num_output_port = 0;
  int num_sram_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
  t_spice_model_port** sram_port = NULL;
  int iport, ipin;
  int sram_port_index = OPEN;
  int mode_port_index = OPEN;
  int mode_lsb = 0;
  int num_dumped_port = 0;
  char* mode_inport_postfix = "_mode";

  int num_buf_input_port = 0;
  int num_buf_output_port = 0;
  t_spice_model_port** buf_input_port = NULL;
  t_spice_model_port** buf_output_port = NULL;

  int jport, jpin, pin_cnt;
  int modegate_num_input_port = 0;
  int modegate_num_input_pins = 0;
  int modegate_num_output_port = 0;
  t_spice_model_port** modegate_input_port = NULL;
  t_spice_model_port** modegate_output_port = NULL;
  char* required_gate_type = NULL;
  enum e_spice_model_gate_type required_gate_model_type;
  
  /* Check */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File handler.\n",
               __FILE__, __LINE__); 
    exit(1);
  }
  assert(SPICE_MODEL_LUT == verilog_model->type);

  /* Print module name */
  fprintf(fp, "//-----LUT module, verilog_model_name=%s -----\n", verilog_model->name);  
  fprintf(fp, "module %s (", verilog_model->name);
  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, TRUE, FALSE, FALSE, TRUE)) {
    fprintf(fp, ",\n");
  }
  /* Print module port list */
  /* Find the input port, output port, and sram port*/
  input_port = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_port = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  sram_port = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);

  /* Asserts*/
  if (FALSE == verilog_model->design_tech_info.lut_info->frac_lut) {
    /* when fracturable LUT is considered
     * More than 1 output is allowed  
     * Only two SRAM ports are allowed
     */
    assert(1 == num_input_port);
    assert(1 == num_output_port);
    assert(1 == num_sram_port); 
  } else {
    assert (TRUE == verilog_model->design_tech_info.lut_info->frac_lut);
    /* when fracturable LUT is considered
     * More than 1 output is allowed  
     * Only two SRAM ports are allowed
     */
    assert(1 == num_input_port);
    for (iport = 0; iport < num_output_port; iport++) {
      assert(0 < output_port[iport]->size);
    }
    assert(2 == num_sram_port); 
  }

  /* input port */
  fprintf(fp, "input wire [0:%d] %s,\n",  
          input_port[0]->size - 1, input_port[0]->prefix);
  /* Print output ports*/
  for (iport = 0; iport < num_output_port; iport++) {
    fprintf(fp, "output wire [0:%d] %s,\n", 
            output_port[iport]->size - 1, output_port[iport]->prefix);
  }
  /* Print configuration ports*/
  num_dumped_port = 0;
  for (iport = 0; iport < num_sram_port; iport++) {
    /* By pass mode select ports */
    if (TRUE == sram_port[iport]->mode_select) {
      continue;
    } 
    assert(FALSE == sram_port[iport]->mode_select); 
    fprintf(fp, "input wire [0:%d] %s_out,\n", 
            sram_port[iport]->size - 1, sram_port[iport]->prefix);
    /* Inverted configuration port is not connected to any internal signal of a LUT */
    fprintf(fp, "input wire [0:%d] %s_outb\n", 
            sram_port[iport]->size - 1, sram_port[iport]->prefix);
    sram_port_index = iport;
    num_dumped_port++;
  }
  assert(1 == num_dumped_port);
  /* Print mode configuration ports*/
  num_dumped_port = 0;
  for (iport = 0; iport < num_sram_port; iport++) {
    /* By pass mode select ports */
    if (FALSE == sram_port[iport]->mode_select) {
      continue;
    } 
    fprintf(fp, ",\n");
    assert(TRUE == sram_port[iport]->mode_select); 
    fprintf(fp, "input wire [0:%d] %s_out,\n", 
            sram_port[iport]->size - 1, sram_port[iport]->prefix);
    /* Inverted configuration port is not connected to any internal signal of a LUT */
    fprintf(fp, "input wire [0:%d] %s_outb\n", 
            sram_port[iport]->size - 1, sram_port[iport]->prefix);
    mode_port_index = iport;
    num_dumped_port++;
  }
  /* Check if all required SRAMs ports*/
  if (TRUE == verilog_model->design_tech_info.lut_info->frac_lut) {
    if (1 != num_dumped_port) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                "(FILE:%s,LINE[%d]) Fracturable LUT (spice_model_name=%s) must have 1 mode port!\n",
                __FILE__, __LINE__, verilog_model->name); 
      exit(1);
    }
  }
  /* End of port list */
  fprintf(fp, ");\n");

  /* Add mode selector */
  fprintf(fp, "  wire [0:%d] %s%s;\n", 
          input_port[0]->size - 1, input_port[0]->prefix, mode_inport_postfix);
  fprintf(fp, "  wire [0:%d] %s_b;\n", 
          input_port[0]->size - 1, input_port[0]->prefix);
  fprintf(fp, "  wire [0:%d] %s_buf;\n", 
          input_port[0]->size - 1, input_port[0]->prefix);

  /* Regular ports */
  if (FALSE == verilog_model->design_tech_info.lut_info->frac_lut) {
    /* Wire the mode ports to regular inputs */
    for (ipin = 0; ipin < input_port[0]->size; ipin++) {
      fprintf(fp, "  assign %s%s[%d] = %s[%d];\n", 
              input_port[0]->prefix, mode_inport_postfix, ipin, 
              input_port[0]->prefix, ipin);
    }
  } else {
    assert (TRUE == verilog_model->design_tech_info.lut_info->frac_lut);
    assert( NULL != input_port[0]->tri_state_map );
    /* Create inverters between input port and its inversion */
    mode_lsb = 0;
    for (ipin = 0; ipin < input_port[0]->size; ipin++) {
      /* Set up checking flags */
      if ('0' == input_port[0]->tri_state_map[ipin]) {  
        required_gate_type = "AND"; 
        required_gate_model_type = SPICE_MODEL_GATE_AND; 
      }
      if ('1' == input_port[0]->tri_state_map[ipin]) {  
        required_gate_type = "OR"; 
        required_gate_model_type = SPICE_MODEL_GATE_OR; 
      }
      switch (input_port[0]->tri_state_map[ipin]) {  
      case '-':
        fprintf(fp, "  assign %s%s[%d] = %s[%d];\n", 
                input_port[0]->prefix, mode_inport_postfix, ipin, 
                input_port[0]->prefix, ipin);
        break;
      case '0':
      case '1':
        /* Check: we must have an AND2/OR2 gate */
        if (NULL == input_port[0]->spice_model) {
          vpr_printf(TIO_MESSAGE_ERROR,
                     "(FILE: %s, [LINE%d]) %s gate for the input port (name=%s) of  spice model (name=%s) is not defined!\n",
                     __FILE__, __LINE__, required_gate_type,
                     input_port[0]->prefix, verilog_model->name);
          exit(1);
        }
        if ((SPICE_MODEL_GATE != input_port[0]->spice_model->type)
          || (required_gate_model_type != input_port[0]->spice_model->design_tech_info.gate_info->type)) {
          vpr_printf(TIO_MESSAGE_ERROR,
                     "(FILE: %s, [LINE%d]) %s gate for the input port (name=%s) of  spice model (name=%s) is not defined as a AND logic gate!\n",
                     __FILE__, __LINE__, required_gate_type,
                     input_port[0]->prefix, verilog_model->name);
          exit(1);
        }
        /* Check input ports */
        modegate_input_port = find_spice_model_ports(input_port[0]->spice_model, SPICE_MODEL_PORT_INPUT, &modegate_num_input_port, TRUE);
        modegate_num_input_pins = 0;
        for (jport = 0; jport < modegate_num_input_port; jport++) {
          modegate_num_input_pins += modegate_input_port[jport]->size; 
        }
        if (2 != modegate_num_input_pins) { 
          vpr_printf(TIO_MESSAGE_ERROR,
                     "(FILE: %s, [LINE%d]) %s gate for the input port (name=%s) of spice model (name=%s) should have only 2 input pins!\n",
                     __FILE__, __LINE__, required_gate_type,
                     input_port[0]->prefix, verilog_model->name);
          exit(1);
        }
        /* Check output ports */
        modegate_output_port = find_spice_model_ports(input_port[0]->spice_model, SPICE_MODEL_PORT_OUTPUT, &modegate_num_output_port, TRUE);
        if (  (1 != modegate_num_output_port)
           || (1 != modegate_output_port[0]->size)) {
          vpr_printf(TIO_MESSAGE_ERROR,
                     "(FILE: %s, [LINE%d]) %s gate for the input port (name=%s) of spice model (name=%s) should have only 1 output!\n",
                     __FILE__, __LINE__, required_gate_type,
                     input_port[0]->prefix, verilog_model->name);
          exit(1);
        }
        /* Instance the AND2/OR2 gate */
        fprintf(fp, "  %s %s_%s_%d_(",
                input_port[0]->spice_model->name, 
                input_port[0]->spice_model->prefix, 
                input_port[0]->prefix, ipin);
        pin_cnt = 0;
        for (jport = 0; jport < modegate_num_input_port; jport++) {
          if (0 < jport) {
            fprintf(fp, ",");
          }
          for (jpin = 0; jpin < modegate_input_port[jport]->size; jpin++) {
            if (0 < jpin) {
              fprintf(fp, ",");
            }
            if (0 == pin_cnt) {
              /* Dump explicit port map if required */
              if (TRUE == input_port[0]->spice_model->dump_explicit_port_map) {
                fprintf(fp, ".%s(", 
                        modegate_input_port[jport]->lib_name);
              }
              fprintf(fp, "%s[%d]",
                      input_port[0]->prefix, ipin);
              if (TRUE == input_port[0]->spice_model->dump_explicit_port_map) {
                fprintf(fp, ")");
              }
            } else if (1 == pin_cnt) { 
              /* Dump explicit port map if required */
              if (TRUE == input_port[0]->spice_model->dump_explicit_port_map) {
                fprintf(fp, ".%s(", 
                        modegate_input_port[jport]->lib_name);
              }
              fprintf(fp, " %s_out[%d]",
                      sram_port[mode_port_index]->prefix, mode_lsb);
              if (TRUE == input_port[0]->spice_model->dump_explicit_port_map) {
                fprintf(fp, ")");
              }
            }
            pin_cnt++;
          }
        }
        assert(2 == pin_cnt);
        fprintf(fp, ", ");
        /* Dump explicit port map if required */
        if (TRUE == input_port[0]->spice_model->dump_explicit_port_map) {
          fprintf(fp, ".%s(", 
                  modegate_output_port[0]->lib_name);
        }
        fprintf(fp, " %s%s[%d]",
                input_port[0]->prefix, mode_inport_postfix, ipin); 
        if (TRUE == input_port[0]->spice_model->dump_explicit_port_map) {
          fprintf(fp, ")");
        }
        fprintf(fp, ");\n");
        mode_lsb++;
        /* Free ports */
        my_free(modegate_input_port);
        my_free(modegate_output_port);
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, 
                  "(FILE:%s,LINE[%d]) Invalid LUT tri_state_map = %s ",
                  __FILE__, __LINE__, input_port[0]->tri_state_map); 
        exit(1);
      }
    }
    if (mode_lsb != sram_port[mode_port_index]->size) {
      vpr_printf(TIO_MESSAGE_ERROR, 
                "(FILE:%s,LINE[%d]) SPICE model LUT (name=%s) has a unmatched tri-state map (%s) implied by mode_port size(%d)!\n",
                __FILE__, __LINE__, verilog_model->name, input_port[0]->tri_state_map[ipin], input_port[0]->size); 
      exit(1);
    }
  }


  /* Find the ports for input_inverter */
  buf_input_port = find_spice_model_ports(verilog_model->lut_input_buffer->spice_model, SPICE_MODEL_PORT_INPUT, &num_buf_input_port, TRUE);
  buf_output_port = find_spice_model_ports(verilog_model->lut_input_buffer->spice_model, SPICE_MODEL_PORT_OUTPUT, &num_buf_output_port, TRUE);
  /* Check */
  assert(1 == num_buf_input_port);
  assert(1 == num_buf_output_port);

  /* Create buffer input port */
  for (ipin = 0; ipin < input_port[0]->size; ipin++) {
    fprintf(fp, "%s %s_%s_%d_ ( ", 
          verilog_model->lut_input_buffer->spice_model->name,
          verilog_model->lut_input_buffer->spice_model->name,
          input_port[0]->prefix, ipin);
    /* Dump global ports */
    if  (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model->lut_input_buffer->spice_model, FALSE, FALSE, TRUE, TRUE)) {
      fprintf(fp, ",\n");
    }
    /* Dump explicit port map if required */
    if (TRUE == verilog_model->lut_input_buffer->spice_model->dump_explicit_port_map) {
      fprintf(fp, ".%s(", 
              buf_input_port[0]->lib_name); 
    }
    fprintf(fp, "%s%s[%d]", 
            input_port[0]->prefix, mode_inport_postfix, ipin);
    if (TRUE == verilog_model->lut_input_buffer->spice_model->dump_explicit_port_map) {
      fprintf(fp, ")"); 
    }
    fprintf(fp, ", "); 
    /* Dump explicit port map if required */
    if (TRUE == verilog_model->lut_input_buffer->spice_model->dump_explicit_port_map) {
      fprintf(fp, ".%s(", 
              buf_output_port[0]->lib_name); 
    }
    fprintf(fp, "%s_buf[%d]", 
          input_port[0]->prefix, ipin);
    if (TRUE == verilog_model->lut_input_buffer->spice_model->dump_explicit_port_map) {
      fprintf(fp, ")"); 
    }
    fprintf(fp, ");\n"); 
  }
  /* Free */
  my_free(buf_input_port);
  my_free(buf_output_port);

  /* Find the ports for input_inverter */
  buf_input_port = find_spice_model_ports(verilog_model->lut_input_inverter->spice_model, SPICE_MODEL_PORT_INPUT, &num_buf_input_port, TRUE);
  buf_output_port = find_spice_model_ports(verilog_model->lut_input_inverter->spice_model, SPICE_MODEL_PORT_OUTPUT, &num_buf_output_port, TRUE);
  /* Check */
  assert(1 == num_buf_input_port);
  assert(1 == num_buf_output_port);

  /* Create inverted input port */
  for (ipin = 0; ipin < input_port[0]->size; ipin++) {
    fprintf(fp, "%s %s_%s_%d_ ( ", 
          verilog_model->lut_input_inverter->spice_model->name,
          verilog_model->lut_input_inverter->spice_model->name,
          input_port[0]->prefix, ipin);
    /* Dump global ports */
    if  (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model->lut_input_inverter->spice_model, FALSE, FALSE, TRUE, TRUE)) {
      fprintf(fp, ",\n");
    }
    /* Dump explicit port map if required */
    if (TRUE == verilog_model->lut_input_inverter->spice_model->dump_explicit_port_map) {
      fprintf(fp, ".%s(", 
              buf_input_port[0]->lib_name); 
    }
    fprintf(fp, "%s%s[%d]", 
            input_port[0]->prefix, mode_inport_postfix, ipin);
    if (TRUE == verilog_model->lut_input_inverter->spice_model->dump_explicit_port_map) {
      fprintf(fp, ")"); 
    }
    fprintf(fp, ", "); 
    /* Dump explicit port map if required */
    if (TRUE == verilog_model->lut_input_inverter->spice_model->dump_explicit_port_map) {
      fprintf(fp, ".%s(", 
              buf_output_port[0]->lib_name); 
    }
    fprintf(fp, "%s_b[%d]", 
          input_port[0]->prefix, ipin);
    if (TRUE == verilog_model->lut_input_inverter->spice_model->dump_explicit_port_map) {
      fprintf(fp, ")"); 
    }
    fprintf(fp, ");\n"); 
  }
  /* Free */
  my_free(buf_input_port);
  my_free(buf_output_port);

 
  /* Internal structure of a LUT */ 
  /* Call the LUT MUX */
  fprintf(fp, "  %s_mux %s_mux_0_ (", 
          verilog_model->name, verilog_model->name);
  /* Connect MUX inputs to LUT configuration port */
  assert(FALSE == sram_port[sram_port_index]->mode_select); 
  if (true == is_explicit_mapping) {
    fprintf(fp, ".in(");
  }
  fprintf(fp, "%s_out",
          sram_port[sram_port_index]->prefix);
  if (true == is_explicit_mapping) {
    fprintf(fp, "), ");
  } else {
    fprintf(fp, ", ");
  }
  /* Connect MUX output to LUT output */
  for (iport = 0; iport < num_output_port; iport++) {
    if (true == is_explicit_mapping) {
      fprintf(fp, ".%s(",
              output_port[iport]->prefix);
    }
    fprintf(fp, "%s", 
            output_port[iport]->prefix);
    if (true == is_explicit_mapping) {
      fprintf(fp, "), ");
    } else {
      fprintf(fp, ", ");
    }
  }
  /* Connect MUX configuration port to LUT inputs */
  if (true == is_explicit_mapping) {
    fprintf(fp, ".sram(");
  }
  fprintf(fp, "%s_buf", 
          input_port[0]->prefix);
  /* Connect MUX inverted configuration port to inverted LUT inputs */
  if (true == is_explicit_mapping) {
    fprintf(fp, "), .sram_inv(");
  } else {
    fprintf(fp, ", ");
  }
  fprintf(fp, "%s_b", 
          input_port[0]->prefix);
  if (true == is_explicit_mapping) {
    fprintf(fp, ")");
  }
  /* End of call LUT MUX */
  fprintf(fp, ");\n");

  /* Print timing info */
  dump_verilog_submodule_timing(fp, verilog_model);

  /* Print signal initialization */
  dump_verilog_submodule_signal_init(fp, verilog_model);

  /* Print end of module */
  fprintf(fp, "endmodule\n");
  fprintf(fp, "//-----END LUT module, verilog_model_name=%s -----\n", verilog_model->name);  
  fprintf(fp, "\n");

  /* Free */
  my_free(input_port);
  my_free(output_port);
  my_free(sram_port);

  return;
}

/* Dump one module of a LUT */
static 
void dump_verilog_submodule_one_mem(FILE* fp, 
                                    t_spice_model* verilog_model) {
  int iport, ipin, pin_index;
  int num_conf_bits;

  int num_sram_port = 0;
  t_spice_model_port** sram_port = NULL;
  t_spice_model* mem_model = NULL;

  /* Find the basis subckt*/
  char* mem_subckt_name = NULL;
 
  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  sram_port = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);

  /* Return if there is no sram port */
  if (0 == num_sram_port) {
    return;
  }

  /* Currently, Only support one mem_model for each SPICE MODEL  */
  for (iport = 0; iport < num_sram_port; iport++) {
    if (NULL == mem_model) {
      mem_model = sram_port[iport]->spice_model;
      continue;
    } 
    if ( mem_model != sram_port[iport]->spice_model ) {
      vpr_printf(TIO_MESSAGE_ERROR,
                 "(FILE:%s,LINE[%d]) Different memory model has been found for a spice_model %s! Currently only support unified memory model\n",
                 __FILE__, __LINE__, verilog_model->name); 
      exit(1);
    }
  }

  /* Generate subckt name */
  mem_subckt_name = generate_verilog_mem_subckt_name(verilog_model, mem_model, verilog_mem_posfix);

  num_conf_bits = count_num_sram_bits_one_spice_model(verilog_model, -1);
  
  fprintf(fp, "//----- CMOS Mem info: spice_model_name=%s -----\n", 
            verilog_model->name);
  fprintf(fp, "module %s (", mem_subckt_name);
  dump_verilog_mem_module_port_map(fp, mem_model, TRUE, 0, num_conf_bits, FALSE); 
  fprintf(fp, ");\n");
 
  /* For each SRAM port we generate mem subckt */
  pin_index = 0;
  /* Dump all the submodules */
  for (ipin = 0 ; ipin < num_conf_bits; ipin++) { 
    fprintf(fp, "%s %s_%d_ ( ",
            mem_model->name, mem_model->prefix, ipin);
    dump_verilog_mem_module_port_map(fp, mem_model, FALSE, pin_index, 1, 
                                     mem_model->dump_explicit_port_map); 
    fprintf(fp, ");\n");
    pin_index++;
  }

  /* END of this submodule */
  fprintf(fp, "endmodule\n");

  /* Free */
  my_free(mem_subckt_name);


}

/* Dump verilog top-level module for LUTs */
static 
void dump_verilog_submodule_luts(char* verilog_dir,
                                 char* submodule_dir,
                                 int num_spice_model,
                                 t_spice_model* spice_models,
                                 boolean include_timing,
                                 boolean include_signal_init,
                                 bool is_explicit_mapping) {
  FILE* fp = NULL;
  char* verilog_name = my_strcat(submodule_dir, luts_verilog_file_name);
  int imodel; 

  /* Create File Handlers */
  fp = fopen(verilog_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create Verilog netlist %s",__FILE__, __LINE__, luts_verilog_file_name); 
    exit(1);
  } 
  dump_verilog_file_header(fp,"Look-Up Tables");

  verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Search for each LUT spice model */
  for (imodel = 0; imodel < num_spice_model; imodel++) {
    /* Bypass user-defined spice models */
    if (NULL != spice_models[imodel].verilog_netlist) {
      continue;
    }
    if (SPICE_MODEL_LUT == spice_models[imodel].type) {
      dump_verilog_submodule_one_lut(fp, &(spice_models[imodel]), is_explicit_mapping);
    }
  }

  /* Close the file handler */
  fclose(fp);

  /* Add fname to the linked list */
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_name);  

  return;
}

static 
void dump_verilog_submodule_memories(t_sram_orgz_info* cur_sram_orgz_info,
                                     char* verilog_dir,
                                     char* submodule_dir,
                                     int num_switch,
                                     t_switch_inf* switches,
                                     t_spice* spice,
                                     t_det_routing_arch* routing_arch,
                                     bool is_explicit_mapping) {
  
  /* Statisitcs for input sizes and structures of MUXes 
   * used in FPGA architecture 
   */
  /* We have linked list whichs stores spice model information of multiplexer*/
  t_llist* muxes_head = NULL; 
  t_llist* temp = NULL;
  FILE* fp = NULL;
  char* verilog_name = my_strcat(submodule_dir, memories_verilog_file_name);
  int num_input_ports = 0;
  t_spice_model_port** input_ports = NULL;
  int num_sram_ports = 0;
  t_spice_model_port** sram_ports = NULL;

  int num_input_basis = 0;
  t_spice_mux_model* cur_spice_mux_model = NULL;
  
  int imodel;

  /* Alloc the muxes*/
  muxes_head = stats_spice_muxes(num_switch, switches, spice, routing_arch);

  /* Print the muxes netlist*/
  fp = fopen(verilog_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create subckt SPICE netlist %s",
                __FILE__, __LINE__, verilog_name); 
    exit(1);
  } 
  /* Generate the descriptions*/
  dump_verilog_file_header(fp,"Memories used in FPGA");

  verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Print mux netlist one by one*/
  temp = muxes_head;
  while(temp) {
    assert(NULL != temp->dptr);
    cur_spice_mux_model = (t_spice_mux_model*)(temp->dptr);
    /* Bypass the spice models who has a user-defined subckt */
    if (NULL != cur_spice_mux_model->spice_model->verilog_netlist) {
      input_ports = find_spice_model_ports(cur_spice_mux_model->spice_model, SPICE_MODEL_PORT_INPUT, &num_input_ports, TRUE);
      sram_ports = find_spice_model_ports(cur_spice_mux_model->spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_ports, TRUE);
      assert(0 != num_input_ports);
      assert(0 != num_sram_ports);
      /* Check the Input port size */
      if (cur_spice_mux_model->size != input_ports[0]->size) {
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File:%s,[LINE%d])User-defined MUX SPICE MODEL(%s) size(%d) unmatch with the architecture needs(%d)!\n",
                   __FILE__, __LINE__, cur_spice_mux_model->spice_model->name, input_ports[0]->size,cur_spice_mux_model->size);
        exit(1);
      }
      /* Check the SRAM port size */
      num_input_basis = determine_num_input_basis_multilevel_mux(cur_spice_mux_model->size, 
                                                                 cur_spice_mux_model->spice_model->design_tech_info.mux_info->mux_num_level);
      if ((num_input_basis * cur_spice_mux_model->spice_model->design_tech_info.mux_info->mux_num_level) != sram_ports[0]->size) {
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File:%s,[LINE%d])User-defined MUX SPICE MODEL(%s) SRAM size(%d) unmatch with the num of level(%d)!\n",
                   __FILE__, __LINE__, cur_spice_mux_model->spice_model->name, sram_ports[0]->size, 
                   cur_spice_mux_model->spice_model->design_tech_info.mux_info->mux_num_level * num_input_basis);
        exit(1);
      }
      /* Move on to the next*/
      temp = temp->next;
      /* Free */
      my_free(input_ports);
      my_free(sram_ports);
      continue;
    }
    /* Generate the spice_mux_arch */
    cur_spice_mux_model->spice_mux_arch = (t_spice_mux_arch*)my_malloc(sizeof(t_spice_mux_arch));
    init_spice_mux_arch(cur_spice_mux_model->spice_model, cur_spice_mux_model->spice_mux_arch, cur_spice_mux_model->size);
    /* Print the mux mem subckt */
    dump_verilog_mux_mem_module(fp, cur_spice_mux_model,
                                is_explicit_mapping);
    /* Update the statistics*/
    /* Move on to the next*/
    temp = temp->next;
  }

  /* Search all the other SPICE models and create memory module */
  for (imodel = 0; imodel < spice->num_spice_model; imodel++) {
    /* Bypass MUX */
    if (SPICE_MODEL_MUX == spice->spice_models[imodel].type) {
      continue;
    }
    /* We only care those with SRAM ports */
    sram_ports = find_spice_model_ports(&(spice->spice_models[imodel]), SPICE_MODEL_PORT_SRAM, &num_sram_ports, TRUE);
    if (0 == num_sram_ports) {
      continue;
    }
    /* Create a memory submodule */
    dump_verilog_submodule_one_mem(fp, &(spice->spice_models[imodel]));
  }

  /* Close the file*/
  fclose(fp);

  /* Add fname to the linked list */
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_name);  

  /* remember to free the linked list*/
  free_muxes_llist(muxes_head);

  /* Free strings */
  free(verilog_name);

  return;
}

/*********************************************************************
 * Register all the user-defined modules in the module manager
 * Walk through the circuit library and add user-defined circuit models
 * to the module_manager
 ********************************************************************/
static 
void add_user_defined_verilog_modules(ModuleManager& module_manager, 
                                      const CircuitLibrary& circuit_lib, 
                                      const std::vector<t_segment_inf>& routing_segments) {
  /* Iterate over Verilog modules */
  for (const auto& model : circuit_lib.models()) {
    /* We only care about user-defined models */
    if (true == circuit_lib.model_verilog_netlist(model).empty()) {
      continue;
    }
    /* Skip Routing channel wire models because they need a different name. Do it later */
    if (SPICE_MODEL_CHAN_WIRE == circuit_lib.model_type(model)) {
      continue;
    }
    /* Reach here, the model requires a user-defined Verilog netlist, 
     * Register it in the module_manager  
     */
    add_circuit_model_to_module_manager(module_manager, circuit_lib, model);
  }

  /* Register the routing channel wires  */
  for (const auto& seg : routing_segments) {
    VTR_ASSERT( CircuitModelId::INVALID() != seg.circuit_model);
    VTR_ASSERT( SPICE_MODEL_CHAN_WIRE == circuit_lib.model_type(seg.circuit_model));
    /* We care only user-defined circuit models */
    if (circuit_lib.model_verilog_netlist(seg.circuit_model).empty()) {
      continue;
    }
    /* Give a unique name for subckt of wire_model of segment, 
     * circuit_model name is unique, and segment id is unique as well
     */
    std::string segment_wire_subckt_name = generate_segment_wire_subckt_name(circuit_lib.model_name(seg.circuit_model), &seg - &routing_segments[0]);

    /* Create a Verilog Module based on the circuit model, and add to module manager */
    ModuleId module_id = add_circuit_model_to_module_manager(module_manager, circuit_lib, seg.circuit_model, segment_wire_subckt_name); 

    /* Find the output port*/
    std::vector<CircuitPortId> output_ports = circuit_lib.model_ports_by_type(seg.circuit_model, SPICE_MODEL_PORT_OUTPUT, true);
    /* Make sure the port size is what we want */
    VTR_ASSERT (1 == circuit_lib.port_size(output_ports[0]));
  
    /* Add a mid-output port to the module */
    BasicPort module_mid_output_port(generate_segment_wire_mid_output_name(circuit_lib.port_lib_name(output_ports[0])), circuit_lib.port_size(output_ports[0]));
    module_manager.add_port(module_id, module_mid_output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }
}

/* Print a template for a user-defined circuit model
 * The template will include just the port declaration of the Verilog module
 * The template aims to help user to write Verilog codes with a guaranteed
 * module definition, which can be correctly instanciated (with correct
 * port mapping) in the FPGA fabric
 */
static 
void print_one_verilog_template_module(const ModuleManager& module_manager,
                                       std::fstream& fp,
                                       const std::string& module_name) {
  /* Ensure a valid file handler*/
  check_file_handler(fp);

  print_verilog_comment(fp, std::string("----- Template Verilog module for " + module_name + " -----"));

  /* Find the module in module manager, which should be already registered */
  /* TODO: routing channel wire model may have a different name! */
  ModuleId template_module = module_manager.find_module(module_name);
  VTR_ASSERT(ModuleId::INVALID() != template_module);

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, template_module);
  /* Finish dumping ports */

  print_verilog_comment(fp, std::string("----- Internal logic should start here -----"));

  /* Add some empty lines as placeholders for the internal logic*/
  fp << std::endl << std::endl;
 
  print_verilog_comment(fp, std::string("----- Internal logic should end here -----"));

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, module_name);

  /* Add an empty line as a splitter */
  fp << std::endl;
}

/* Print a template of all the submodules that are user-defined
 * The template will include just the port declaration of the submodule
 * The template aims to help user to write Verilog codes with a guaranteed
 * module definition, which can be correctly instanciated (with correct
 * port mapping) in the FPGA fabric
 */
static 
void print_verilog_submodule_templates(const ModuleManager& module_manager,
                                       const CircuitLibrary& circuit_lib,
                                       const std::vector<t_segment_inf>& routing_segments,
                                       const std::string& verilog_dir,
                                       const std::string& submodule_dir) {
  std::string verilog_fname(submodule_dir + user_defined_template_verilog_file_name);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  /* Print out debugging information for if the file is not opened/created properly */
  vpr_printf(TIO_MESSAGE_INFO,
             "Creating template for user-defined Verilog modules (%s)...\n",
             verilog_fname.c_str()); 

  print_verilog_file_header(fp, "Template for user-defined Verilog modules"); 

  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Output essential models*/
  for (const auto& model : circuit_lib.models()) {
    /* Focus on user-defined modules, which must have a Verilog netlist defined */
    if (circuit_lib.model_verilog_netlist(model).empty()) {
      continue;
    }
    /* Skip Routing channel wire models because they need a different name. Do it later */
    if (SPICE_MODEL_CHAN_WIRE == circuit_lib.model_type(model)) {
      continue;
    }
    /* Print a Verilog template for the circuit model */
    print_one_verilog_template_module(module_manager, fp, circuit_lib.model_name(model)); 
  }

  /* Register the routing channel wires  */
  for (const auto& seg : routing_segments) {
    VTR_ASSERT( CircuitModelId::INVALID() != seg.circuit_model);
    VTR_ASSERT( SPICE_MODEL_CHAN_WIRE == circuit_lib.model_type(seg.circuit_model));
    /* We care only user-defined circuit models */
    if (circuit_lib.model_verilog_netlist(seg.circuit_model).empty()) {
      continue;
    }
    /* Give a unique name for subckt of wire_model of segment, 
     * circuit_model name is unique, and segment id is unique as well
     */
    std::string segment_wire_subckt_name = generate_segment_wire_subckt_name(circuit_lib.model_name(seg.circuit_model), &seg - &routing_segments[0]);
    /* Print a Verilog template for the circuit model */
    print_one_verilog_template_module(module_manager, fp, segment_wire_subckt_name); 
  }

  /* close file stream */
  fp.close();
 
  /* No need to add the template to the subckt include files! */
}

/*********************************************************************
 * Dump verilog files of submodules to be used in FPGA components :
 * 1. MUXes
 ********************************************************************/
void dump_verilog_submodules(ModuleManager& module_manager, 
                             const MuxLibrary& mux_lib,
                             t_sram_orgz_info* cur_sram_orgz_info,
                             char* verilog_dir, 
                             char* submodule_dir, 
                             t_arch Arch, 
                             t_det_routing_arch* routing_arch,
                             t_syn_verilog_opts fpga_verilog_opts) {

  /* Create a vector of segments. TODO: should come from DeviceContext */
  std::vector<t_segment_inf> L_segment_vec;
  for (int i = 0; i < Arch.num_segments; ++i) {
    L_segment_vec.push_back(Arch.Segments[i]);
  }

  /* TODO: Register all the user-defined modules in the module manager
   * This should be done prior to other steps in this function, 
   * because they will be instanciated by other primitive modules 
   */
  vpr_printf(TIO_MESSAGE_INFO, "Registering user-defined modules...\n");
  add_user_defined_verilog_modules(module_manager, Arch.spice->circuit_lib, L_segment_vec);

  print_verilog_submodule_essentials(module_manager, 
                                     std::string(verilog_dir), 
                                     std::string(submodule_dir),
                                     Arch.spice->circuit_lib);

  /* 1. MUXes */
  vpr_printf(TIO_MESSAGE_INFO, "Generating modules of multiplexers...\n");
  dump_verilog_submodule_muxes(cur_sram_orgz_info, verilog_dir, submodule_dir, routing_arch->num_switch, 
                               switch_inf, Arch.spice, routing_arch, fpga_verilog_opts.dump_explicit_verilog);

  print_verilog_submodule_muxes(module_manager, mux_lib, Arch.spice->circuit_lib, cur_sram_orgz_info, std::string(verilog_dir), std::string(submodule_dir));

  print_verilog_submodule_mux_local_decoders(module_manager, mux_lib, Arch.spice->circuit_lib, std::string(verilog_dir), std::string(submodule_dir));
 
  /* 2. LUTes */
  vpr_printf(TIO_MESSAGE_INFO, "Generating modules of LUTs...\n");
  dump_verilog_submodule_luts(verilog_dir, submodule_dir,
                              Arch.spice->num_spice_model, Arch.spice->spice_models,
                              fpga_verilog_opts.include_timing, 
                              fpga_verilog_opts.include_signal_init,
                              fpga_verilog_opts.dump_explicit_verilog);
  print_verilog_submodule_luts(module_manager, Arch.spice->circuit_lib, std::string(verilog_dir), std::string(submodule_dir));

  /* 3. Hardwires */
  print_verilog_submodule_wires(module_manager, Arch.spice->circuit_lib, L_segment_vec, std::string(verilog_dir), std::string(submodule_dir));

  /* 4. Memories */
  vpr_printf(TIO_MESSAGE_INFO, "Generating modules of memories...\n");
  dump_verilog_submodule_memories(cur_sram_orgz_info, verilog_dir, submodule_dir, routing_arch->num_switch, 
                                  switch_inf, Arch.spice, routing_arch, fpga_verilog_opts.dump_explicit_verilog);
  print_verilog_submodule_memories(module_manager, mux_lib, Arch.spice->circuit_lib, std::string(verilog_dir), std::string(submodule_dir));

  /* 5. Dump decoder modules only when memory bank is required */
  dump_verilog_config_peripherals(cur_sram_orgz_info, verilog_dir, submodule_dir);

  /* 6. Dump template for all the modules */
  if (TRUE == fpga_verilog_opts.print_user_defined_template) { 
    print_verilog_submodule_templates(module_manager, Arch.spice->circuit_lib, L_segment_vec, std::string(verilog_dir), std::string(submodule_dir));
  }

  /* Create a header file to include all the subckts */
  vpr_printf(TIO_MESSAGE_INFO,"Generating header file for basic submodules...\n");
  dump_verilog_subckt_header_file(submodule_verilog_subckt_file_path_head,
                                  submodule_dir,
                                  submodule_verilog_file_name);

  return;
}

