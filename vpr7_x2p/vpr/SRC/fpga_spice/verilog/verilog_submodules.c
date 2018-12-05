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

/* Include vpr structs*/
#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph.h"
#include "vpr_utils.h"
#include "path_delay.h"
#include "stats.h"

/* Include FPGA-SPICE utils */
#include "linkedlist.h"
#include "fpga_spice_utils.h"
#include "spice_mux.h"
#include "fpga_spice_globals.h"

/* Include verilog utils */
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_pbtypes.h"

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

  fprintf(fp, "\n`ifdef %s\n", verilog_timing_preproc_flag);
  fprintf(fp, "  //------ BEGIN Pin-to-pin Timing constraints -----\n");
  fprintf(fp, "  specify\n");
  /* Give pin-to-pin delays */
  /* Enumerate timing edges of each input ports */
  for (iport = 0; iport < num_input_port; iport++) {
    for (ipin = 0; ipin < input_port[iport]->size; ipin++) {
      for (iedge = 0; iedge < input_port[iport]->num_tedges[ipin]; iedge++) {
        fprintf(fp, "    (%s[%d] => %s[%d]) = (%.2g, %.2g);\n",
                input_port[iport]->prefix, ipin,
                input_port[iport]->tedge[ipin][iedge]->to_port->prefix,
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

void dump_verilog_submodule_init_sim(FILE* fp,
                                   t_spice_model* cur_spice_model) {
  int iport, ipin;
  int num_input_port;
  t_spice_model_port** input_port= NULL;

  input_port = find_spice_model_ports(cur_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);

  /* Ensure a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File handler.\n",
               __FILE__, __LINE__); 
    exit(1);
  }

  fprintf(fp, "\n`ifdef %s\n", verilog_init_sim_preproc_flag);
  fprintf(fp, "  //------ BEGIN driver initialization -----\n");
  fprintf(fp, "initial begin\n");

  for (iport = 0; iport < num_input_port; iport++) {
     fprintf(fp, "  $signal_force(\"%s\", \"0\", 0, 1, , 1);\n",
                input_port[iport]->prefix);
  }
  fprintf(fp, "end\n");

  fprintf(fp, "  //------ END driver initialization -----\n");
  fprintf(fp, "`endif\n");

  return;
}


/***** Subroutines *****/
/* Dump a module of inverter or buffer or tapered buffer */
void dump_verilog_invbuf_module(FILE* fp,
                                t_spice_model* invbuf_spice_model,
								boolean include_timing,
								boolean init_sim) {
  int ipin, iport, port_cnt;
  int num_input_port = 0;
  int num_output_port = 0;
  int num_powergate_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
  t_spice_model_port** powergate_port = NULL;

  /* Ensure a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File handler.\n",
               __FILE__, __LINE__); 
    exit(1);
  }

  fprintf(fp, "//----- Verilog module for %s -----\n",
          invbuf_spice_model->name);

  /* Find the input port, output port*/
  input_port = find_spice_model_ports(invbuf_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_port = find_spice_model_ports(invbuf_spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  powergate_port = find_spice_model_config_done_ports(invbuf_spice_model, SPICE_MODEL_PORT_INPUT, &num_powergate_port, FALSE);

  /* Make sure:
   * There is only 1 input port and 1 output port, 
   * each size of which is 1
   */
  assert(1 == num_input_port);
  assert(1 == input_port[0]->size);
  assert(1 == num_output_port);
  assert(1 == output_port[0]->size);

  /* If power-gated, we need to find enable signals */
  if (TRUE == invbuf_spice_model->design_tech_info.power_gated) {
    if (0 ==  num_powergate_port) {
      vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Inverter, buffer SPICE model is power-gated, but cannot find any power-gate port!\n",
                 __FILE__, __LINE__); 
      exit(1);
    }
    assert ( 0 < num_powergate_port);
  }

  /* dump module body */
  fprintf(fp, "module %s (\n",
          invbuf_spice_model->name);
  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, invbuf_spice_model, TRUE, FALSE)) {
    fprintf(fp, ",\n");
  }
  /* Dump ports */
  fprintf(fp, "  input [0:0] %s,\n", input_port[0]->prefix);
  fprintf(fp, "  output [0:0] %s\n", output_port[0]->prefix);
  fprintf(fp, "  );\n");
  /* Finish dumping ports */

  /* Assign logics : depending on topology */
  switch (invbuf_spice_model->design_tech_info.buffer_info->type) {
  case SPICE_MODEL_BUF_INV:
    if (TRUE == invbuf_spice_model->design_tech_info.power_gated) {
      /* Create a sensitive list */
      fprintf(fp, "reg %s_reg;\n", output_port[0]->prefix);
      fprintf(fp, "always @(");
      /* Power-gate port first*/
      for (iport = 0; iport < num_powergate_port; iport++) {
        fprintf(fp, "%s,", powergate_port[iport]->prefix);
      }
      fprintf(fp, "%s) begin\n", 
                  input_port[0]->prefix); 
      /* Dump the case of power-gated */
      fprintf(fp, "  if (");
      port_cnt = 0; /* Initialize the counter: decide if we need to put down '&&' */
      for (iport = 0; iport < num_powergate_port; iport++) {
        if (0 == powergate_port[iport]->default_val) {
          for (ipin = 0; ipin < powergate_port[iport]->size; ipin++) {
            if ( 0 < port_cnt ) {
              fprintf(fp, "\n\t&&");
            }
            /* Power-gated signal are disable during operating, enabled during configuration,
             * Therefore, we need to reverse them here   
             */
            fprintf(fp, "(~%s[%d])", 
                         powergate_port[iport]->prefix,
                         ipin);
            port_cnt++; /* Update port counter*/
          }
        } else {
          assert (1 == powergate_port[iport]->default_val);
          for (ipin = 0; ipin < powergate_port[iport]->size; ipin++) {
            if ( 0 < port_cnt ) {
              fprintf(fp, "\n\t&&");
            }
            /* Power-gated signal are disable during operating, enabled during configuration,
             * Therefore, we need to reverse them here   
             */
            fprintf(fp, "(%s[%d])", 
                        powergate_port[iport]->prefix,
                        ipin);
            port_cnt++; /* Update port counter*/
          }
        }
      }
      fprintf(fp, ") begin\n");
      fprintf(fp, "\t\tassign %s_reg = ~%s;\n",
                  output_port[0]->prefix,
                  input_port[0]->prefix);
      fprintf(fp, "\tend else begin\n");
      fprintf(fp, "\t\tassign %s_reg = 1'bz;\n",
                  output_port[0]->prefix);
      fprintf(fp, "\tend\n");
      fprintf(fp, "end\n");
      fprintf(fp, "assign %s = %s_reg;\n",
                  output_port[0]->prefix,
                  output_port[0]->prefix);
    } else {
      fprintf(fp, "assign %s = ~%s;\n",
                  output_port[0]->prefix,
                  input_port[0]->prefix);
    }
    break;
  case SPICE_MODEL_BUF_BUF:
    if (TRUE == invbuf_spice_model->design_tech_info.power_gated) {
      /* Create a sensitive list */
      fprintf(fp, "reg %s_reg;\n", output_port[0]->prefix);
      fprintf(fp, "always @(");
      /* Power-gate port first*/
      for (iport = 0; iport < num_powergate_port; iport++) {
        fprintf(fp, "%s,", powergate_port[iport]->prefix);
      }
      fprintf(fp, "%s) begin\n", 
                  input_port[0]->prefix);
      /* Dump the case of power-gated */
      fprintf(fp, "  if (");
      port_cnt = 0; /* Initialize the counter: decide if we need to put down '&&' */
      for (iport = 0; iport < num_powergate_port; iport++) {
        if (0 == powergate_port[iport]->default_val) {
          for (ipin = 0; ipin < powergate_port[iport]->size; ipin++) {
            if ( 0 < port_cnt ) {
              fprintf(fp, "\n\t&&");
            }
            /* Power-gated signal are disable during operating, enabled during configuration,
             * Therefore, we need to reverse them here   
             */
            fprintf(fp, "(~%s[%d])", 
                         powergate_port[iport]->prefix,
                         ipin);
            port_cnt++; /* Update port counter*/
          }
        } else {
          assert (1 == powergate_port[iport]->default_val);
          for (ipin = 0; ipin < powergate_port[iport]->size; ipin++) {
            if ( 0 < port_cnt ) {
              fprintf(fp, "\n\t&&");
            }
            /* Power-gated signal are disable during operating, enabled during configuration,
             * Therefore, we need to reverse them here   
             */
            fprintf(fp, "(%s[%d])", 
                        powergate_port[iport]->prefix,
                        ipin);
            port_cnt++; /* Update port counter*/
          }
        }
      }
      fprintf(fp, ") begin\n");
      fprintf(fp, "\t\tassign %s_reg = %s;\n",
                  output_port[0]->prefix,
                  input_port[0]->prefix);
      fprintf(fp, "\tend else begin\n");
      fprintf(fp, "\t\tassign %s_reg = 1'bz;\n",
                  output_port[0]->prefix);
      fprintf(fp, "\tend\n");
      fprintf(fp, "end\n");
      fprintf(fp, "assign %s = %s_reg;\n",
                  output_port[0]->prefix,
                  output_port[0]->prefix);

    } else if (FALSE == invbuf_spice_model->design_tech_info.buffer_info->tapered_buf) {
      fprintf(fp, "assign %s = %s;\n",
                  output_port[0]->prefix,
                  input_port[0]->prefix);
    } else {
      assert (TRUE == invbuf_spice_model->design_tech_info.buffer_info->tapered_buf);
      fprintf(fp, "assign %s = ",
                  output_port[0]->prefix);
      /* depend on the stage, we may invert the output */
      if (1 == invbuf_spice_model->design_tech_info.buffer_info->tap_buf_level % 2) {
        fprintf(fp, "~");
      }
      fprintf(fp, "%s;\n",
                  input_port[0]->prefix);
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid topology for spice model (%s)!\n",
               __FILE__, __LINE__, invbuf_spice_model->name);
    exit(1);
  }

  /* Print timing info */
  if (TRUE == include_timing) {
    dump_verilog_submodule_timing(fp, invbuf_spice_model);
  }

  /* Print simulation initialization info */
  if (TRUE == init_sim) {
    dump_verilog_submodule_init_sim(fp, invbuf_spice_model);
  }

  fprintf(fp, "endmodule\n");

  fprintf(fp, "\n");

  /* Free */
  my_free(input_port);
  my_free(output_port);

  return;
}

/* Dump a module of pass-gate logic */
void dump_verilog_passgate_module(FILE* fp,
                                  t_spice_model* passgate_spice_model,
								  boolean include_timing) {
  int iport;
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

  /* Find the input port, output port*/
  input_port = find_spice_model_ports(passgate_spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_port = find_spice_model_ports(passgate_spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);

  /* Make sure:
   * There is only 1 output port, 
   * each size of which is 1
   */
  assert(1 == num_output_port);
  assert(1 == output_port[0]->size);

  fprintf(fp, "//----- Verilog module for %s -----\n",
          passgate_spice_model->name);

  /* dump module body */
  fprintf(fp, "module %s (\n",
          passgate_spice_model->name);

  /* Assign ports : depending on topology */
  switch (passgate_spice_model->design_tech_info.pass_gate_info->type) {
  case SPICE_MODEL_PASS_GATE_TRANSMISSION:
    /* Make sure:
     * There is only 3 input port (in, sel, selb), 
     * each size of which is 1
     */
    assert(3 == num_input_port);
    for (iport = 0; iport < num_input_port; iport++) {
      assert(1 == input_port[iport]->size);
    }
    /* Dump ports */
    fprintf(fp, "  input [0:0] in,\n");
    fprintf(fp, "  input [0:0] sel,\n");
    fprintf(fp, "  input [0:0] selb,\n");
    fprintf(fp, "  output [0:0] %s\n", output_port[0]->prefix);
    fprintf(fp, "  );\n");
    /* Finish dumping ports */

    break;
  case SPICE_MODEL_PASS_GATE_TRANSISTOR:
    /* Make sure:
     * There is only 2 input port (in, sel), 
     * each size of which is 1
     */
    assert(2 == num_input_port);
    for (iport = 0; iport < num_input_port; iport++) {
      assert(1 == input_port[iport]->size);
    }
    /* Dump ports */
    fprintf(fp, "  input [0:0] in,\n");
    fprintf(fp, "  input [0:0] sel,\n");
    fprintf(fp, "  output [0:0] %s\n", output_port[0]->prefix);
    fprintf(fp, "  );\n");
    /* Finish dumping ports */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid topology for spice model (%s)!\n",
               __FILE__, __LINE__, passgate_spice_model->name);
    exit(1);
  }

  /* Dump logics */
  fprintf(fp, "assign %s = sel? in : 1'bz;\n",
              output_port[0]->prefix);

  /* Print timing info */
  if (TRUE == include_timing) {
    dump_verilog_submodule_timing(fp, passgate_spice_model);
  }

  fprintf(fp, "endmodule\n");

  fprintf(fp, "\n");

  /* Free */
  my_free(input_port);
  my_free(output_port);

  return;
}


/* Dump Essential modules: 
 * 1. inverters
 * 2. buffers
 * 3. pass-gate logics */
void dump_verilog_submodule_essentials(char* submodule_dir,
                                       int num_spice_model,
                                       t_spice_model* spice_models,
									   boolean include_timing,
									   boolean init_sim) {
  int imodel; 
  char* verilog_name = my_strcat(submodule_dir, essentials_verilog_file_name);
  FILE* fp = NULL;

  /* Create file */
  fp = fopen(verilog_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create Verilog netlist %s",
                                 __FILE__, __LINE__, essentials_verilog_file_name); 
    exit(1);
  } 
  dump_verilog_file_header(fp,"Essential gates");

  dump_verilog_preproc(fp, include_timing);

  /* Output essential models*/
  for (imodel = 0; imodel < num_spice_model; imodel++) {
    /* By pass user-defined modules */
    if (NULL != spice_models[imodel].verilog_netlist) {
      continue;
    }
    if (SPICE_MODEL_INVBUF == spice_models[imodel].type) {
      dump_verilog_invbuf_module(fp, &(spice_models[imodel]), include_timing, init_sim);
    }
    if (SPICE_MODEL_PASSGATE == spice_models[imodel].type) {
      dump_verilog_passgate_module(fp, &(spice_models[imodel]), include_timing);
    }
  }

  /* Close file handler*/
  fclose(fp); 

  /* Add fname to the linked list */
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_name);

  /* Free */
  
  return;
}

/* Dump a CMOS MUX basis module */
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
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_spice_model, TRUE, FALSE)) {
    fprintf(fp, ",\n");
  }
  /* Port list */
  fprintf(fp, "  input [0:%d] in,\n", num_input_basis_subckt - 1);
  fprintf(fp, "  output out,\n");
  fprintf(fp, "  input [0:%d] mem,\n",
          num_mem - 1);
  fprintf(fp, "  input [0:%d] mem_inv);\n",
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

  assert(TRUE == cur_spice_model->dump_structural_verilog);

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
  fprintf(fp, "//---- Structural Verilog for CMOS MUX basis module: %s -----\n", mux_basis_subckt_name);

  /* Print the port list and definition */
  fprintf(fp, "module %s (\n", mux_basis_subckt_name);
  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_spice_model, TRUE, FALSE)) {
    fprintf(fp, ",\n");
  }
  /* Port list */
  fprintf(fp, "  input [0:%d] in,\n", num_input_basis_subckt - 1);
  fprintf(fp, "  output out,\n");
  fprintf(fp, "  input [0:%d] mem,\n",
          num_mem - 1);
  fprintf(fp, "  input [0:%d] mem_inv);\n",
          num_mem - 1);
  /* Verilog Behavior description for a MUX */
  fprintf(fp, "//---- Structure-level description -----\n");
  /* Special case: only one memory, switch case is simpler 
   * When mem = 1, propagate input 0; 
   * when mem = 0, propagate input 1;
   */
  if (1 == num_mem) {
    /* Transmission gates are connected to each input and also the output*/
    fprintf(fp, "  %s %s_0 (in[0], mem[0], mem_inv[0], out);\n",
                tgate_module_name, tgate_module_name);
    fprintf(fp, "  %s %s_1 (in[1], mem_inv[0], mem[0], out);\n",
                tgate_module_name, tgate_module_name);
  } else {
  /* Other cases, we need to follow the rules:
   * When mem[k] is enabled, switch on input[k]
   * Only one memory bit is enabled!
   */
    for (i = 0; i < num_mem; i++) {
      fprintf(fp, "  %s %s_%d (in[%d], mem[%d], mem_inv[%d], out);\n",
                  tgate_module_name, tgate_module_name, i,
                  i, i, i);
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
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_spice_model, TRUE, FALSE)) {
    fprintf(fp, ",\n");
  }
  /* Port list */
  fprintf(fp, "  input wire [0:%d] in,\n", num_input_basis_subckt - 1);
  fprintf(fp, "  output wire out,\n");
  fprintf(fp, "  input wire [0:%d] bl,\n",
          num_mem - 1);
  fprintf(fp, "  input wire [0:%d] wl);\n",
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
    fprintf(fp, "%s %s_%d (in[%d], wl[%d], bl[%d], out);\n",
                 progTE_module_name, progTE_module_name, i,
                 i, i, i);
  }
  /* RIGHT part */
  fprintf(fp, "%s %s_%d (out, wl[%d], bl[%d]);\n",
               progBE_module_name, progBE_module_name, i,
               i, i);

  /* Put an end to this module */
  fprintf(fp, "endmodule\n");

  /* Comment lines */
  fprintf(fp, "//---- END Structural Verilog for RRAM MUX basis module: %s -----\n\n", mux_basis_subckt_name);

  return;
}


/* Dump a RRAM MUX basis module */
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
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_spice_model, TRUE, FALSE)) {
    fprintf(fp, ",\n");
  }
  /* Port list */
  fprintf(fp, "  input wire [0:%d] in,\n", num_input_basis_subckt - 1);
  fprintf(fp, "  output wire out,\n");
  fprintf(fp, "  input wire [0:%d] bl,\n",
          num_mem - 1);
  fprintf(fp, "  input wire [0:%d] wl);\n",
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
  init_spice_mux_arch(spice_mux_model->spice_model, spice_mux_model->spice_mux_arch, spice_mux_model->size);

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
  mux_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_mux_model->spice_model->name) + 5 
                                           + strlen(my_itoa(spice_mux_model->size)) + strlen(verilog_mux_basis_posfix) + 1)); 
  sprintf(mux_basis_subckt_name, "%s_size%d%s",
          spice_mux_model->spice_model->name, spice_mux_model->size, verilog_mux_basis_posfix);

  special_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_mux_model->spice_model->name) + 5 
                                           + strlen(my_itoa(spice_mux_model->size)) + strlen(verilog_mux_special_basis_posfix) + 1)); 
  sprintf(special_basis_subckt_name, "%s_size%d%s",
          spice_mux_model->spice_model->name, spice_mux_model->size, verilog_mux_special_basis_posfix);

  /* deteremine the number of inputs of basis subckt */ 
  num_input_basis_subckt = spice_mux_model->spice_mux_arch->num_input_basis;

  /* Print the basis subckt*/
  dump_verilog_mux_one_basis_module(fp, mux_basis_subckt_name, spice_mux_model->size,
                                    num_input_basis_subckt, spice_mux_model->spice_model, 
                                    FALSE);
  /* See if we need a special basis */
  switch (spice_mux_model->spice_model->design_tech_info.structure) {
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

void dump_verilog_cmos_mux_tree_structure(FILE* fp, 
                                          char* mux_basis_subckt_name,
                                          t_spice_model spice_model,
                                          t_spice_mux_arch spice_mux_arch,
                                          int num_sram_port, t_spice_model_port** sram_port) {
  int i, j, level, nextlevel;
  int nextj, out_idx;
  int mux_basis_cnt = 0;

  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  mux_basis_cnt = 0;
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
      /* Dump global ports */
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE)) {
        fprintf(fp, ",\n");
      }
      fprintf(fp, "mux2_l%d_in[%d:%d], ", level, j, nextj); /* input0 input1 */
      fprintf(fp, "mux2_l%d_in[%d], ", nextlevel, out_idx); /* output */
      fprintf(fp, "%s[%d], %s_inv[%d]);\n", sram_port[0]->prefix, i, sram_port[0]->prefix, i); /* sram sram_inv */
      /* Update the counter */
      j = nextj;
      mux_basis_cnt++;
    } 
  }   
  /* Assert */
  assert(0 == nextlevel);
  assert(0 == out_idx);
  assert(mux_basis_cnt == spice_mux_arch.num_input - 1);

  return;
}

void dump_verilog_cmos_mux_multilevel_structure(FILE* fp, 
                                                char* mux_basis_subckt_name,
                                                char* mux_special_basis_subckt_name,
                                                t_spice_model spice_model,
                                                t_spice_mux_arch spice_mux_arch,
                                                int num_sram_port, t_spice_model_port** sram_port) {
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
 
  for (i = 0; i < spice_mux_arch.num_level; i++) {
    level = spice_mux_arch.num_level - i;
    nextlevel = spice_mux_arch.num_level - i - 1; 
    sram_idx = nextlevel * spice_mux_arch.num_input_basis;
    /* Check */
    assert(nextlevel > -1);
    /* Print basis muxQto1 for each level*/
    for (j = 0; j < spice_mux_arch.num_input_per_level[nextlevel]; j = j+cur_num_input_basis) {
      /* output index */
      out_idx = j/spice_mux_arch.num_input_basis; 
      /* Determine the number of input of this basis */
      cur_num_input_basis = spice_mux_arch.num_input_basis;
      if ((j + cur_num_input_basis) > spice_mux_arch.num_input_per_level[nextlevel]) {
        cur_num_input_basis = find_spice_mux_arch_special_basis_size(spice_mux_arch);
        if (0 < cur_num_input_basis) {
          /* Print the special basis */
          fprintf(fp, "%s special_basis(", mux_special_basis_subckt_name);
          /* Dump global ports */
          if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE)) {
            fprintf(fp, ",\n");
          }
          fprintf(fp, "mux2_l%d_in[%d:%d], ", level, j, j + cur_num_input_basis - 1); /* input0 input1 */
          fprintf(fp, "mux2_l%d_in[%d], ", nextlevel, out_idx); /* output */
          fprintf(fp, "%s[%d:%d], %s_inv[%d:%d] ", 
          sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis -1,
          sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis -1);
          fprintf(fp, ");\n");
          special_basis_cnt++;
        }
        continue;
      }
      /* Each basis muxQto1: <given_name> <input0> <input1> <output> <sram> <sram_inv> svdd sgnd <subckt_name> */
      fprintf(fp, "%s ", mux_basis_subckt_name); /* subckt_name */
      fprintf(fp, "mux_basis_no%d (", mux_basis_cnt); /* given_name */
      /* Dump global ports */
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE)) {
        fprintf(fp, ",\n");
      }
      fprintf(fp, "mux2_l%d_in[%d:%d], ", level, j, j + cur_num_input_basis - 1); /* input0 input1 */
      fprintf(fp, "mux2_l%d_in[%d], ", nextlevel, out_idx); /* output */
      /* Print number of sram bits for this basis */
      fprintf(fp, "%s[%d:%d], %s_inv[%d:%d] ", 
      sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis -1,
      sram_port[0]->prefix, sram_idx, sram_idx + cur_num_input_basis -1);
      fprintf(fp, ");\n");
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

void dump_verilog_cmos_mux_onelevel_structure(FILE* fp, 
                                              char* mux_basis_subckt_name,
                                              t_spice_model spice_model,
                                              t_spice_mux_arch spice_mux_arch,
                                              int num_sram_port, t_spice_model_port** sram_port) {
  /* Make sure we have a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  assert(SPICE_MODEL_DESIGN_CMOS == spice_model.design_tech);

  fprintf(fp, "wire [0:%d] mux2_l%d_in; \n", spice_mux_arch.num_input - 1, 1); /* input0  */
  fprintf(fp, "wire [0:%d] mux2_l%d_in; \n", 0, 0); /* output */

  fprintf(fp, "%s mux_basis (\n", mux_basis_subckt_name); /* given_name */
  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE)) {
    fprintf(fp, ",\n");
  }
  fprintf(fp, "//----- MUX inputs -----\n");
  fprintf(fp, "mux2_l%d_in[0:%d], ", 1, spice_mux_arch.num_input - 1); /* input0  */
  fprintf(fp, "mux2_l%d_in[%d], ", 0, 0); /* output */
  fprintf(fp, "\n");
  fprintf(fp, "//----- SRAM ports -----\n");
  /* Special basis for 2-input MUX, there is only one configuration bit */
  if (2 == spice_mux_arch.num_input) {
    fprintf(fp, "%s[0:%d], %s_inv[0:%d] ", 
    sram_port[0]->prefix, 0, 
    sram_port[0]->prefix, 0); /* sram sram_inv */
  } else {
    fprintf(fp, "%s[0:%d], %s_inv[0:%d] ", 
    sram_port[0]->prefix, spice_mux_arch.num_input - 1, 
    sram_port[0]->prefix, spice_mux_arch.num_input - 1); /* sram sram_inv */
  }
  fprintf(fp, "\n");
  fprintf(fp, ");\n");
 
  return;
}

void dump_verilog_cmos_mux_submodule(FILE* fp,
                                     int mux_size,
                                     t_spice_model spice_model,
                                     t_spice_mux_arch spice_mux_arch) {
  int i, num_conf_bits;
  int num_input_port = 0;
  int num_output_port = 0;
  int num_sram_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
  t_spice_model_port** sram_port = NULL;

  /* Find the basis subckt*/
  char* mux_basis_subckt_name = NULL;
  char* mux_special_basis_subckt_name = NULL;

  mux_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model.name) + 5 
                                           + strlen(my_itoa(mux_size)) + strlen(verilog_mux_basis_posfix) + 1)); 
  sprintf(mux_basis_subckt_name, "%s_size%d%s",
          spice_model.name, mux_size, verilog_mux_basis_posfix);

  mux_special_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model.name) + 5 
                                           + strlen(my_itoa(spice_mux_arch.num_input)) 
                                           + strlen(verilog_mux_special_basis_posfix) + 1)); 
  sprintf(mux_special_basis_subckt_name, "%s_size%d%s",
          spice_model.name, spice_mux_arch.num_input, verilog_mux_special_basis_posfix);

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
  assert(1 == num_input_port);
  assert(1 == num_output_port);
  assert(1 == num_sram_port);
  assert(1 == output_port[0]->size);

  /* We have two types of naming rules in terms of the usage of MUXes: 
   * 1. MUXes, the naming rule is <mux_spice_model_name>_<structure>_size<input_size>
   * 2. LUTs, the naming rule is <lut_spice_model_name>_mux_size<sram_port_size>
   */
  num_conf_bits = count_num_sram_bits_one_spice_model(&spice_model, 
                                                      /* sram_verilog_orgz_info->type, */ 
                                                      mux_size);
  if (SPICE_MODEL_LUT == spice_model.type) {
    /* Special for LUT MUX */
    fprintf(fp, "//------ CMOS MUX info: spice_model_name= %s_MUX, size=%d -----\n", spice_model.name, mux_size);
    fprintf(fp, "module %s_mux(\n", spice_model.name);
    /* Dump global ports */
    if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, TRUE, FALSE)) {
      fprintf(fp, ",\n");
    }
    /* Print input ports*/
    assert(mux_size == num_conf_bits);
    fprintf(fp, "input wire [0:%d] %s,\n", num_conf_bits - 1,  input_port[0]->prefix);
    /* Print output ports*/
    fprintf(fp, "output wire %s,\n", output_port[0]->prefix);
    /* Print configuration ports*/
    /* The configuration port in MUX context is the input port in LUT context ! */
    fprintf(fp, "input wire [0:%d] %s,\n", 
            input_port[0]->size - 1, sram_port[0]->prefix);
    fprintf(fp, "input wire [0:%d] %s_inv\n", 
            input_port[0]->size - 1, sram_port[0]->prefix);
  } else {
    fprintf(fp, "//----- CMOS MUX info: spice_model_name=%s, size=%d, structure: %s -----\n", 
            spice_model.name, mux_size, gen_str_spice_model_structure(spice_model.design_tech_info.structure));
    fprintf(fp, "module %s_size%d (", spice_model.name, mux_size);
    /* Print input ports*/
    fprintf(fp, "input wire [0:%d] %s,\n", mux_size - 1,  input_port[0]->prefix);
    /* Print output ports*/
    fprintf(fp, "output wire %s,\n", output_port[0]->prefix);
    /* Print configuration ports*/
    fprintf(fp, "input wire [0:%d] %s,\n", 
            num_conf_bits - 1, sram_port[0]->prefix);
    fprintf(fp, "input wire [0:%d] %s_inv\n", 
            num_conf_bits - 1, sram_port[0]->prefix);
  }

  /* Print local vdd and gnd*/
  fprintf(fp, ");");
  fprintf(fp, "\n");
  
  /* Print internal architecture*/ 
  switch (spice_model.design_tech_info.structure) {
  case SPICE_MODEL_STRUCTURE_TREE:
    dump_verilog_cmos_mux_tree_structure(fp, mux_basis_subckt_name, 
                                         spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    dump_verilog_cmos_mux_onelevel_structure(fp, mux_basis_subckt_name, 
                                             spice_model, spice_mux_arch, num_sram_port, sram_port);
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    dump_verilog_cmos_mux_multilevel_structure(fp, mux_basis_subckt_name, mux_special_basis_subckt_name,
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
      switch (spice_model.input_buffer->type) {
      case SPICE_MODEL_BUF_INV:
        /* Each inv: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
        fprintf(fp, "%s inv%d (", 
                spice_model.input_buffer->spice_model_name, i); /* Given name*/
        /* Dump global ports */
        if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.input_buffer->spice_model, FALSE, FALSE)) {
          fprintf(fp, ",\n");
        }
        fprintf(fp, "%s[%d], ", input_port[0]->prefix, i); /* input port */ 
        fprintf(fp, "mux2_l%d_in[%d]); ", spice_mux_arch.input_level[i], spice_mux_arch.input_offset[i]); /* output port*/
        fprintf(fp, "\n");
        break;
      case SPICE_MODEL_BUF_BUF:
        /* TODO: what about tapered buffer, can we support? */
        /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
        fprintf(fp, "%s buf%d (", 
                spice_model.input_buffer->spice_model_name, i); /* Given name*/
        /* Dump global ports */
        if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.input_buffer->spice_model, FALSE, FALSE)) {
          fprintf(fp, ",\n");
        }
        fprintf(fp, "%s[%d], ", input_port[0]->prefix, i); /* input port */ 
        fprintf(fp, "mux2_l%d_in[%d]); ", spice_mux_arch.input_level[i], spice_mux_arch.input_offset[i]); /* output port*/
        fprintf(fp, "\n");
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type for spice_model_buffer.\n",
                   __FILE__, __LINE__);
        exit(1);   
      }
    } else {
      /* There is no buffer, I create a zero resisitance between*/
      /* Resistance R<given_name> <input> <output> 0*/
      fprintf(fp, "assign %s[%d] = mux2_l%d_in[%d];\n", 
              input_port[0]->prefix, i, spice_mux_arch.input_level[i], 
              spice_mux_arch.input_offset[i]);
    }
  }

  /* Output buffer*/
  if (1 == spice_model.output_buffer->exist) {
    switch (spice_model.output_buffer->type) {
    case SPICE_MODEL_BUF_INV:
      if (TRUE == spice_model.output_buffer->tapered_buf) {
        break;
      }
      /* Each inv: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "%s inv_out (",
              spice_model.output_buffer->spice_model_name); /* Given name*/
      /* Dump global ports */
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.output_buffer->spice_model, FALSE, FALSE)) {
        fprintf(fp, ",\n");
      }
      fprintf(fp, "mux2_l%d_in[%d], ", 0, 0); /* input port */ 
      fprintf(fp, "%s );", output_port[0]->prefix); /* Output port*/
      fprintf(fp, "\n");
      break;
    case SPICE_MODEL_BUF_BUF:
      if (TRUE == spice_model.output_buffer->tapered_buf) {
        break;
      }
      /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "%s buf_out (",
              spice_model.output_buffer->spice_model_name); /* Given name*/
      /* Dump global ports */
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.output_buffer->spice_model, FALSE, FALSE)) {
        fprintf(fp, ",\n");
      }
      fprintf(fp, "mux2_l%d_in[%d], ", 0, 0); /* input port */ 
      fprintf(fp, "%s );", output_port[0]->prefix); /* Output port*/
      fprintf(fp, "\n");
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type for spice_model_buffer.\n",
                 __FILE__, __LINE__);
      exit(1);   
    }
    /* Tapered buffer support */
    if (TRUE == spice_model.output_buffer->tapered_buf) {
      /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "%s buf_out (",
              spice_model.output_buffer->spice_model_name); /* subckt name */
      /* Dump global ports */
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.output_buffer->spice_model, FALSE, FALSE)) {
        fprintf(fp, ",\n");
      }
      fprintf(fp, "mux2_l%d_in[%d], ", 0, 0); /* input port */ 
      fprintf(fp, "%s );", output_port[0]->prefix); /* Output port*/
      fprintf(fp, "\n");
    }
  } else {
    /* There is no buffer, I create a zero resisitance between*/
    /* Resistance R<given_name> <input> <output> 0*/
    fprintf(fp, "assign mux2_l0_in[0] = %s;\n",output_port[0]->prefix);
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
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE)) {
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

void dump_verilog_rram_mux_multilevel_structure(FILE* fp, 
                                                char* mux_basis_subckt_name,
                                                char* mux_special_basis_subckt_name,
                                                t_spice_model spice_model,
                                                t_spice_mux_arch spice_mux_arch,
                                                int num_sram_port, t_spice_model_port** sram_port) {
  int i, j, level, nextlevel, sram_idx;
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
    sram_idx = nextlevel * spice_mux_arch.num_input_basis;
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
          if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE)) {
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
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE)) {
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
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, FALSE, FALSE)) {
    fprintf(fp, ",\n");
  }
  fprintf(fp, "//----- MUX inputs -----\n");
  fprintf(fp, "mux2_l%d_in[0:%d],\n ", 1, spice_mux_arch.num_input - 1); /* inputs  */
  fprintf(fp, "mux2_l%d_in[%d],\n", 0, 0); /* output */
  fprintf(fp, "//----- SRAM ports -----\n");
  num_conf_bits = count_num_sram_bits_one_spice_model(&spice_model, 
                                                      /* sram_verilog_orgz_info->type,*/ 
                                                      spice_mux_arch.num_input);
  fprintf(fp, "%s[0:%d], %s_inv[0:%d]", 
            sram_port[0]->prefix, num_conf_bits - 1, 
            sram_port[0]->prefix, num_conf_bits - 1); /* sram sram_inv */
  fprintf(fp, "\n");
  fprintf(fp, ");\n");
 
  return;
}

void dump_verilog_rram_mux_submodule(FILE* fp,
                                     int mux_size,
                                     t_spice_model spice_model,
                                     t_spice_mux_arch spice_mux_arch) {
  int i, num_conf_bits;
  int num_input_port = 0;
  int num_output_port = 0;
  int num_sram_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
  t_spice_model_port** sram_port = NULL;

  /* Find the basis subckt*/
  char* mux_basis_subckt_name = NULL;
  char* mux_special_basis_subckt_name = NULL;

  mux_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model.name) + 5 
                                           + strlen(my_itoa(mux_size)) + strlen(verilog_mux_basis_posfix) + 1)); 
  sprintf(mux_basis_subckt_name, "%s_size%d%s",
          spice_model.name, mux_size, verilog_mux_basis_posfix);

  mux_special_basis_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model.name) + 5 
                                           + strlen(my_itoa(spice_mux_arch.num_input)) 
                                           + strlen(verilog_mux_special_basis_posfix) + 1)); 
  sprintf(mux_special_basis_subckt_name, "%s_size%d%s",
          spice_model.name, spice_mux_arch.num_input, verilog_mux_special_basis_posfix);

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
            spice_model.name, mux_size, gen_str_spice_model_structure(spice_model.design_tech_info.structure));
    fprintf(fp, "module %s_size%d( \n", spice_model.name, mux_size);
  }
  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &spice_model, TRUE, FALSE)) {
    fprintf(fp, ",\n");
  }
  /* Print input ports*/
  fprintf(fp, "input wire [0:%d] %s,\n ", mux_size - 1, input_port[0]->prefix);
  /* Print output ports*/
  fprintf(fp, "output wire %s,\n ", output_port[0]->prefix);
  /* Print configuration ports */
  num_conf_bits = count_num_sram_bits_one_spice_model(&spice_model, 
                                                      /* sram_verilog_orgz_info->type,*/
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
  switch (spice_model.design_tech_info.structure) {
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
      switch (spice_model.input_buffer->type) {
      case SPICE_MODEL_BUF_INV:
        /* Each inv: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
        fprintf(fp, "%s inv%d (", 
                    spice_model.input_buffer->spice_model_name, i); /* Given name*/
        /* Dump global ports */
        if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.input_buffer->spice_model, FALSE, FALSE)) {
          fprintf(fp, ",\n");
        }
        fprintf(fp, "%s[%d], ", input_port[0]->prefix, i); /* input port */ 
        fprintf(fp, "mux2_l%d_in[%d]);", spice_mux_arch.input_level[i], spice_mux_arch.input_offset[i]); /* output port*/
        fprintf(fp, "\n");
        break;
      case SPICE_MODEL_BUF_BUF:
        /* TODO: what about tapered buffer, can we support? */
        /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
        fprintf(fp, "%s buf%d (", 
                    spice_model.input_buffer->spice_model_name, i); /* Given name*/
        /* Dump global ports */
        if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.input_buffer->spice_model, FALSE, FALSE)) {
          fprintf(fp, ",\n");
        }
        fprintf(fp, "%s[%d], ", input_port[0]->prefix, i); /* input port */ 
        fprintf(fp, "mux2_l%d_in[%d)];", spice_mux_arch.input_level[i], spice_mux_arch.input_offset[i]); /* output port*/
        fprintf(fp, "\n");
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type for spice_model_buffer.\n",
                   __FILE__, __LINE__);
        exit(1);   
      }
    } else {
      /* There is no buffer, I create a zero resisitance between*/
      /* Resistance R<given_name> <input> <output> 0*/
      fprintf(fp, "assign %s[%d] = mux2_l%d_in[%d];\n", 
              input_port[0]->prefix, i, spice_mux_arch.input_level[i], 
              spice_mux_arch.input_offset[i]);
    }
  }

  /* Output buffer*/
  if (1 == spice_model.output_buffer->exist) {
    switch (spice_model.output_buffer->type) {
    case SPICE_MODEL_BUF_INV:
      if (TRUE == spice_model.output_buffer->tapered_buf) {
        break;
      }
      /* Each inv: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "%s inv_out (", 
                  spice_model.output_buffer->spice_model_name); /* Given name*/
      /* Dump global ports */
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.output_buffer->spice_model, FALSE, FALSE)) {
        fprintf(fp, ",\n");
      }
      fprintf(fp, "mux2_l%d_in[%d], ", 0, 0); /* input port */ 
      fprintf(fp, "%s );", output_port[0]->prefix); /* Output port*/
      fprintf(fp, "\n");
      break;
    case SPICE_MODEL_BUF_BUF:
      if (TRUE == spice_model.output_buffer->tapered_buf) {
        break;
      }
      /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "%s buf_out (",
                  spice_model.output_buffer->spice_model_name); /* Given name*/
      /* Dump global ports */
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.output_buffer->spice_model, FALSE, FALSE)) {
        fprintf(fp, ",\n");
      }
      fprintf(fp, "mux2_l%d_in[%d], ", 0, 0); /* input port */ 
      fprintf(fp, "%s );", output_port[0]->prefix); /* Output port*/
      fprintf(fp, "\n");
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type for spice_model_buffer.\n",
                 __FILE__, __LINE__);
      exit(1);   
    }
    /* Tapered buffer support */
    if (TRUE == spice_model.output_buffer->tapered_buf) {
      /* Each buf: <given_name> <input0> <output> svdd sgnd <subckt_name> size=param*/
      fprintf(fp, "%s buf_out (",
              spice_model.output_buffer->spice_model_name); /* subckt name */
      /* Dump global ports */
      if  (0 < rec_dump_verilog_spice_model_global_ports(fp, spice_model.output_buffer->spice_model, FALSE, FALSE)) {
        fprintf(fp, ",\n");
      }
      fprintf(fp, "mux2_l%d_in[%d], ", 0 , 0); /* input port */ 
      fprintf(fp, "%s );", output_port[0]->prefix); /* Output port*/
      fprintf(fp, "\n");
    }
  } else {
    /* There is no buffer, I create a zero resisitance between*/
    /* Resistance R<given_name> <input> <output> 0*/
    fprintf(fp, "assign mux2_l0_in[0] %s;\n",output_port[0]->prefix);
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

/** Dump a verilog module for a MUX
 * We always dump a basis submodule for a MUX
 * whatever structure it is: one-level, two-level or multi-level
 */
void dump_verilog_mux_module(FILE* fp, 
                             t_spice_mux_model* spice_mux_model) {
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
                                    *(spice_mux_model->spice_mux_arch));
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    dump_verilog_rram_mux_submodule(fp, spice_mux_model->size,
                                    *(spice_mux_model->spice_model), 
                                    *(spice_mux_model->spice_mux_arch));
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

/**/
void dump_verilog_submodule_muxes(char* submodule_dir,
                                  int num_switch,
                                  t_switch_inf* switches,
                                  t_spice* spice,
                                  t_det_routing_arch* routing_arch) {
  
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
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create subckt SPICE netlist %s",__FILE__, __LINE__, verilog_name); 
    exit(1);
  } 
  /* Generate the descriptions*/
  dump_verilog_file_header(fp,"MUXes used in FPGA");

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
                                                                 cur_spice_mux_model->spice_model->design_tech_info.mux_num_level);
      if ((num_input_basis * cur_spice_mux_model->spice_model->design_tech_info.mux_num_level) != sram_ports[0]->size) {
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File:%s,[LINE%d])User-defined MUX SPICE MODEL(%s) SRAM size(%d) unmatch with the num of level(%d)!\n",
                   __FILE__, __LINE__, cur_spice_mux_model->spice_model->name, sram_ports[0]->size, cur_spice_mux_model->spice_model->design_tech_info.mux_num_level*num_input_basis);
        exit(1);
      }
      /* Move on to the next*/
      temp = temp->next;
      continue;
    }
    /* Let's have a N:1 MUX as basis*/
    dump_verilog_mux_basis_module(fp, cur_spice_mux_model);
    /* Print the mux subckt */
    dump_verilog_mux_module(fp, cur_spice_mux_model);
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
  try_update_sram_orgz_info_reserved_blwl(sram_verilog_orgz_info, 
                                          max_routing_mux_size, max_routing_mux_size);

  vpr_printf(TIO_MESSAGE_INFO,"Generated %d Multiplexer submodules.\n",
             mux_cnt);
  vpr_printf(TIO_MESSAGE_INFO,"Max. MUX size = %d.\t",
             max_mux_size);
  vpr_printf(TIO_MESSAGE_INFO,"Min. MUX size = %d.\n",
             min_mux_size);

  /* Add fname to the linked list */
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_name);

  /* remember to free the linked list*/
  free_muxes_llist(muxes_head);
  /* Free strings */
  free(verilog_name);

  /* Close the file*/
  fclose(fp);

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
    if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &verilog_model, TRUE, FALSE)) {
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
    if  (0 < rec_dump_verilog_spice_model_global_ports(fp, &verilog_model, TRUE, FALSE)) {
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
void dump_verilog_submodule_one_lut(FILE* fp, 
                                    t_spice_model* verilog_model,
									boolean include_timing) {
  int num_input_port = 0;
  int num_output_port = 0;
  int num_sram_port = 0;
  t_spice_model_port** input_port = NULL;
  t_spice_model_port** output_port = NULL;
  t_spice_model_port** sram_port = NULL;
  int iport, ipin, iedge;

  /* Check */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File handler.\n",
               __FILE__, __LINE__); 
    exit(1);}
  assert(SPICE_MODEL_LUT == verilog_model->type);

  /* Print module name */
  fprintf(fp, "//-----LUT module, verilog_model_name=%s -----\n", verilog_model->name);  
  fprintf(fp, "module %s (", verilog_model->name);
  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, TRUE, FALSE)) {
    fprintf(fp, ",\n");
  }
  /* Print module port list */
  /* Find the input port, output port, and sram port*/
  input_port = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
  output_port = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  sram_port = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_SRAM, &num_sram_port, TRUE);

  /* Asserts*/
  assert(1 == num_input_port);
  assert(1 == num_output_port);
  assert(1 == num_sram_port);
  assert(1 == output_port[0]->size);

  /* input port */
  fprintf(fp, "input wire [0:%d] %s,\n",  
          input_port[0]->size - 1, input_port[0]->prefix);
  /* Print output ports*/
  fprintf(fp, "output wire [0:%d] %s,\n", 
          output_port[0]->size - 1, output_port[0]->prefix);
  /* Print configuration ports*/
  fprintf(fp, "input wire [0:%d] %s_out,\n", 
          sram_port[0]->size - 1, sram_port[0]->prefix);
  /* Inverted configuration port is not connected to any internal signal of a LUT */
  fprintf(fp, "input wire [0:%d] %s_outb\n", 
          sram_port[0]->size - 1, sram_port[0]->prefix);
  /* End of port list */
  fprintf(fp, ");\n");

  /* Create inverted input port */
  fprintf(fp, "  wire [0:%d] %s_b;\n", 
          input_port[0]->size - 1, input_port[0]->prefix);
  /* Create inverters between input port and its inversion */
  fprintf(fp, "  assign %s_b = ~ %s;\n", 
          input_port[0]->prefix, input_port[0]->prefix);
 
  /* Internal structure of a LUT */ 
  /* Call the LUT MUX */
  fprintf(fp, "  %s_mux %s_mux_0_ (", 
          verilog_model->name, verilog_model->name);
  /* Connect MUX inputs to LUT configuration port */
  fprintf(fp, " %s_out,", 
          sram_port[0]->prefix);
  /* Connect MUX output to LUT output */
  fprintf(fp, " %s,", 
          output_port[0]->prefix);
  /* Connect MUX configuration port to LUT inputs */
  fprintf(fp, " %s,", 
          input_port[0]->prefix);
  /* Connect MUX inverted configuration port to inverted LUT inputs */
  fprintf(fp, " %s_b", 
          input_port[0]->prefix);
  /* End of call LUT MUX */
  fprintf(fp, ");\n");

  /* Give timing information */
   if (TRUE == include_timing) {
    dump_verilog_submodule_timing(fp, verilog_model);
   }



  /* Print end of module */
  fprintf(fp, "endmodule\n");
  fprintf(fp, "//-----END LUT module, verilog_model_name=%s -----\n", verilog_model->name);  
  fprintf(fp, "\n");

  return;
}

/* Dump verilog top-level module for LUTs */
void dump_verilog_submodule_luts(char* submodule_dir,
                                 int num_spice_model,
                                 t_spice_model* spice_models,
								 boolean include_timing) {
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

  dump_verilog_preproc(fp, include_timing);

  /* Search for each LUT spice model */
  for (imodel = 0; imodel < num_spice_model; imodel++) {
    /* Bypass user-defined spice models */
    if (NULL != spice_models[imodel].verilog_netlist) {
      continue;
    }
    if (SPICE_MODEL_LUT == spice_models[imodel].type) {
      dump_verilog_submodule_one_lut(fp, &(spice_models[imodel]), include_timing);
    }
  }

  /* Close the file handler */
  fclose(fp);

  /* Add fname to the linked list */
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_name);

  return;
}

/* Dump a submodule which is a constant vdd */
void dump_verilog_hard_wired_vdd(FILE* fp, 
                                 t_spice_model verilog_model) {
  int num_output_port = 0;
  t_spice_model_port** output_port = NULL;

  /* Find the input port, output port*/
  output_port = find_spice_model_ports(&verilog_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);

  /* Asserts*/
  assert(1 == num_output_port);
  assert(1 == output_port[0]->size);

  /* Ensure a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File handler.\n",
               __FILE__, __LINE__); 
    exit(1);
  }

  /* print the spice model*/
  fprintf(fp, "//-----Hard-wired VDD module, verilog_model_name=%s -----\n", verilog_model.name);  
  fprintf(fp, "module %s(output wire %s);\n", verilog_model.name, output_port[0]->prefix);
  /* Constant logic 1*/
  fprintf(fp, "assign %s = 1\'b1;\n", output_port[0]->prefix);
  /* Finish*/ 
  fprintf(fp, "endmodule\n");
  fprintf(fp, "//-----END VDD module, verilog_model_name=%s -----\n", verilog_model.name);  
  fprintf(fp, "\n");
  return;
}

/* Dump a submodule which is a constant vdd */
void dump_verilog_hard_wired_gnd(FILE* fp, 
                                 t_spice_model verilog_model) {
  int num_output_port = 0;
  t_spice_model_port** output_port = NULL;

  /* Find the input port, output port*/
  output_port = find_spice_model_ports(&verilog_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);

  /* Asserts*/
  assert(1 == num_output_port);
  assert(1 == output_port[0]->size);

  /* Ensure a valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File handler.\n",
               __FILE__, __LINE__); 
    exit(1);
  }

  /* print the spice model*/
  fprintf(fp, "//-----Hard-wired GND module, verilog_model_name=%s -----\n", verilog_model.name);  
  fprintf(fp, "module %s(output wire %s);\n", verilog_model.name, output_port[0]->prefix);
  /* Constant logic 1*/
  fprintf(fp, "assign %s = 1\'b0;\n", output_port[0]->prefix);
  /* Finish*/ 
  fprintf(fp, "endmodule\n");
  fprintf(fp, "//-----END GND module, verilog_model_name=%s -----\n", verilog_model.name);  
  fprintf(fp, "\n");
  return;
}

void dump_verilog_submodule_wires(char* subckt_dir,
                                  int num_segments,
                                  t_segment_inf* segments,
                                  int num_spice_model,
                                  t_spice_model* spice_models) {
  FILE* fp = NULL;
  char* verilog_name = my_strcat(subckt_dir, wires_verilog_file_name);
  char* seg_wire_subckt_name = NULL;
  char* seg_index_str = NULL;
  int iseg, imodel, len_seg_subckt_name;
 
  fp = fopen(verilog_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create Verilog netlist %s",__FILE__, __LINE__, wires_verilog_file_name); 
    exit(1);
  } 
  dump_verilog_file_header(fp,"Wires");
  /* Output wire models*/
  for (imodel = 0; imodel < num_spice_model; imodel++) {
    /* Bypass user-defined spice models */
    if (NULL != spice_models[imodel].verilog_netlist) {
      continue;
    }
    if (SPICE_MODEL_WIRE == spice_models[imodel].type) {
      assert(NULL != spice_models[imodel].wire_param);
      dump_verilog_wire_module(fp, spice_models[imodel].name,
                              spice_models[imodel]);
    }
  }
 
  /* Create wire models for routing segments*/
  fprintf(fp,"//----- Wire models for segments in routing -----\n");
  for (iseg = 0; iseg < num_segments; iseg++) {
    assert(NULL != segments[iseg].spice_model);
    assert(SPICE_MODEL_CHAN_WIRE == segments[iseg].spice_model->type);
    assert(NULL != segments[iseg].spice_model->wire_param);
    /* Give a unique name for subckt of wire_model of segment, 
     * spice_model name is unique, and segment name is unique as well
     */
    seg_index_str = my_itoa(iseg);
    len_seg_subckt_name = strlen(segments[iseg].spice_model->name)
                        + 4 + strlen(seg_index_str) + 1; /* '\0'*/
    seg_wire_subckt_name = (char*)my_malloc(sizeof(char)*len_seg_subckt_name);
    sprintf(seg_wire_subckt_name,"%s_seg%s",
            segments[iseg].spice_model->name, seg_index_str);
    /* Bypass user-defined spice models */
    if (NULL != segments[iseg].spice_model->verilog_netlist) {
      continue;
    }
    dump_verilog_wire_module(fp, seg_wire_subckt_name,
                            *(segments[iseg].spice_model));
  }

  /* Create module for hard-wired VDD and GND */
  for (imodel = 0; imodel < num_spice_model; imodel++) {
    if (SPICE_MODEL_VDD == spice_models[imodel].type) {
      dump_verilog_hard_wired_vdd(fp, spice_models[imodel]);
    } else if (SPICE_MODEL_GND == spice_models[imodel].type) {
      dump_verilog_hard_wired_gnd(fp, spice_models[imodel]);
    }
  }
  
  /* Close the file handler */
  fclose(fp);

  /* Add fname to the linked list */
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_name);

  /*Free*/
  my_free(seg_index_str);
  my_free(seg_wire_subckt_name);

  return;
}


/* Dump verilog files of submodules to be used in FPGA components :
 * 1. MUXes
 */
void dump_verilog_submodules(char* submodule_dir, 
                             t_arch Arch, 
                             t_det_routing_arch* routing_arch,
							 boolean include_timing,
							 boolean init_sim) {

  /* 0. basic units: inverter, buffers and pass-gate logics, */
  vpr_printf(TIO_MESSAGE_INFO, "Generating essential modules...\n");
  dump_verilog_submodule_essentials(submodule_dir, 
                                    Arch.spice->num_spice_model, 
                                    Arch.spice->spice_models,
									include_timing,
									init_sim);

  /* 1. MUXes */
  vpr_printf(TIO_MESSAGE_INFO, "Generating modules of multiplexers...\n");
  dump_verilog_submodule_muxes(submodule_dir, routing_arch->num_switch, 
                               switch_inf, Arch.spice, routing_arch);
 
  /* 2. LUTes */
  vpr_printf(TIO_MESSAGE_INFO, "Generating modules of LUTs...\n");
  dump_verilog_submodule_luts(submodule_dir,
                              Arch.spice->num_spice_model, Arch.spice->spice_models,
							  include_timing);

  /* 3. Hardwires */
  vpr_printf(TIO_MESSAGE_INFO, "Generating modules of hardwires...\n");
  dump_verilog_submodule_wires(submodule_dir, Arch.num_segments, Arch.Segments,
                               Arch.spice->num_spice_model, Arch.spice->spice_models);


  /*Create a header file to include all the subckts */

  vpr_printf(TIO_MESSAGE_INFO,"Generating header file for basic submodules...\n");
  dump_verilog_subckt_header_file(submodule_verilog_subckt_file_path_head,
                                  submodule_dir,
                                  sub_module_verilog_file_name);

  return;
}

