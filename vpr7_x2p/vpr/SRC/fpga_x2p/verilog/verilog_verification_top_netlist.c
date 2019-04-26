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
#include "route_common.h"

/* Include SPICE support headers*/
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_mux_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "spice_mux.h"
#include "fpga_x2p_globals.h"

/* Include Synthesizable Verilog headers */
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_primitives.h"
#include "verilog_pbtypes.h"
#include "verilog_top_netlist_utils.h"
#include "verilog_top_testbench.h"

static 
void dump_verilog_formal_verification_top_netlist_ports(t_sram_orgz_info* cur_sram_orgz_info, 
                                                        FILE* fp, 
                                                        char* circuit_name) {
  int iblock, cnt;
  char* port_name = NULL;
  
  fprintf(fp, "module %s%s(\n", 
          circuit_name, 
          formal_verification_top_module_postfix);
 
  cnt = 0;

  /* Print all the I/Os of the circuit implementation to be tested*/
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    /* Make sure We find the correct logical block !*/
    switch (logical_block[iblock].type) {
    case VPACK_INPAD:
      if (0 != cnt) { 
        fprintf(fp, ",\n");
      }
      port_name = my_strdup(logical_block[iblock].name);
      dump_verilog_generic_port(fp, VERILOG_PORT_INPUT, 
                                my_strcat(port_name, formal_verification_top_module_port_postfix), 
                                0, 0);
      my_free(port_name);
      cnt++;
      break;
    case VPACK_OUTPAD:
      if (0 != cnt) { 
        fprintf(fp, ",\n");
      }
      port_name = my_strdup(logical_block[iblock].name);
      dump_verilog_generic_port(fp, VERILOG_PORT_OUTPUT, 
                                my_strcat(port_name, formal_verification_top_module_port_postfix), 
                                0, 0);
      my_free(port_name);
      cnt++;
      break;
	case VPACK_COMB: 
    case VPACK_LATCH: 
    case VPACK_EMPTY:
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) Invalid type of logical block[%d]!\n",
                 __FILE__, __LINE__, iblock);
      exit(1);
    }
  }

  fprintf(fp, ");\n");

  return;
}

static 
void dump_verilog_formal_verification_top_netlist_internal_wires(t_sram_orgz_info* cur_sram_orgz_info, 
                                                                 FILE* fp, 
                                                                 char* circuit_name) {
  char* port_name = NULL;
  int num_array_bl, num_array_wl;
  int bl_decoder_size, wl_decoder_size;
  t_spice_model* mem_model = NULL;
  
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model); 
  
  /* Print internal wires */
  /* Connect to defined signals */
  /* set and reset signals */
  fprintf(fp, "\n");
  dump_verilog_top_testbench_global_ports(fp, global_ports_head, VERILOG_PORT_WIRE);
  fprintf(fp, "\n");

  /* Inputs and outputs of I/O pads */
  /* Inout Pads */
  assert(NULL != iopad_verilog_model);
  if ((NULL == iopad_verilog_model)
   ||(iopad_verilog_model->cnt > 0)) {
    /* Malloc and assign port_name */
    port_name = (char*)my_malloc(sizeof(char)*(strlen(gio_inout_prefix) + strlen(iopad_verilog_model->prefix) + 1));
    sprintf(port_name, "%s%s", gio_inout_prefix, iopad_verilog_model->prefix);
    /* Dump a wired port */
    dump_verilog_generic_port(fp, VERILOG_PORT_WIRE, 
                              port_name, iopad_verilog_model->cnt - 1, 0);
    fprintf(fp, "; //--- FPGA iopads \n"); 
    /* Free port_name */
    my_free(port_name);
  }
  
  /* Programming Circuits inputs */
  /* Configuration ports depend on the organization of SRAMs */
  switch(cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    dump_verilog_generic_port(fp, VERILOG_PORT_WIRE, 
                              sram_verilog_model->prefix, sram_verilog_model->cnt - 1, 0);
    fprintf(fp, "; //---- SRAM outputs \n");
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* We put the head of scan-chains here  
	 */
    dump_verilog_generic_port(fp, VERILOG_PORT_WIRE, 
                              top_netlist_scan_chain_head_prefix, 0, 0);
    fprintf(fp, "; //---- Scan-chain head \n");
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* Get the number of array BLs/WLs, decoder sizes */
    determine_blwl_decoder_size(cur_sram_orgz_info,
                                &num_array_bl, &num_array_wl, &bl_decoder_size, &wl_decoder_size);

    fprintf(fp, "  wire [0:0] %s;\n",
            top_netlist_bl_enable_port_name
            );
    fprintf(fp, "  wire [0:0] %s;\n",
            top_netlist_wl_enable_port_name
            );
    dump_verilog_generic_port(fp, VERILOG_PORT_WIRE, 
                              top_netlist_addr_bl_port_name, bl_decoder_size - 1, 0);
    fprintf(fp, "; //--- Address of bit lines \n"); 
    dump_verilog_generic_port(fp, VERILOG_PORT_WIRE, 
                              top_netlist_addr_wl_port_name, wl_decoder_size - 1, 0);
    fprintf(fp, "; //--- Address of word lines \n"); 
    /* data_in is only require by BL decoder of SRAM array 
     * As for RRAM array, the data_in signal will not be used 
     */
    if (SPICE_MODEL_DESIGN_CMOS == mem_model->design_tech) {
      fprintf(fp, "  wire [0:0] %s; // --- Data_in signal for BL decoder, only required by SRAM array \n",
                     top_netlist_bl_data_in_port_name);
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

static 
void dump_verilog_formal_verfication_top_netlist_call_top_module(t_sram_orgz_info* cur_sram_orgz_info, 
                                                                 FILE* fp, 
                                                                 char* circuit_name) {
  /* Include defined top-level module */
  fprintf(fp, "//----- FPGA top-level module to be capsulated  ----\n");
  fprintf(fp, "//------Call defined Top-level Verilog Module -----\n");
  fprintf(fp, "%s_top %s (\n", 
              circuit_name,
              formal_verification_top_module_uut_name); 

  dump_verilog_top_module_ports(cur_sram_orgz_info, fp, VERILOG_PORT_CONKT);

  fprintf(fp, ");\n");
  return;
}

/* Connect global ports of FPGA top module to constants except:
 * 1. operating clock
 */
static 
void dump_verilog_formal_verification_top_netlist_connect_global_ports(t_sram_orgz_info* cur_sram_orgz_info, 
                                                                       FILE* fp,
                                                                       t_llist* head) {
  t_llist* temp = head;
  t_spice_model_port* cur_global_port = NULL;
  int ibit, iblock;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
  }

  fprintf(fp, "//-----  Connect Global ports of FPGA top module -----\n");
  while(NULL != temp) {
    cur_global_port = (t_spice_model_port*)(temp->dptr); 
    if ((SPICE_MODEL_PORT_CLOCK == cur_global_port->type) 
       && (FALSE == cur_global_port->is_prog)) { 
      /* Wire this port to the clock of benchmark */
      for (iblock = 0; iblock < num_logical_blocks; iblock++) {
        /* By pass non-input ports !*/
        if (VPACK_INPAD != logical_block[iblock].type) {
          continue;
        }
        /*  See if this is a clock net */
        if (FALSE == logical_block[iblock].is_clock) {  
          continue; 
        }
        /* Reach here we have found a clock! */
        for (ibit = 0; ibit < cur_global_port->size; ibit++) {
          fprintf(fp, "assign ");
          dump_verilog_generic_port(fp, VERILOG_PORT_CONKT, 
                                    cur_global_port->prefix, ibit, ibit);
          fprintf(fp, " = %s%s;\n", 
                  logical_block[iblock].name, formal_verification_top_module_port_postfix); 
        }
      }
    } else { 
      fprintf(fp, "assign ");
      dump_verilog_generic_port(fp, VERILOG_PORT_CONKT, 
                                    cur_global_port->prefix, 0, cur_global_port->size - 1);
      fprintf(fp, " = %d'b", cur_global_port->size);
      for (ibit = 0; ibit < cur_global_port->size; ibit++) {
        fprintf(fp, "%d", cur_global_port->default_val);
      } 
      fprintf(fp, ";\n"); 
    }
    /* Go to the next */
    temp = temp->next;
  }
  fprintf(fp, "//----- END Global ports -----\n");

  return;
}

/* Add stimuli for unused iopads and configuration memories */
static 
void dump_verilog_formal_verification_top_netlist_connect_ios(t_sram_orgz_info* cur_sram_orgz_info, 
                                                              FILE* fp) {
  int iblock, jiopad, iopad_idx;
  boolean* used_iopad = (boolean*) my_calloc (iopad_verilog_model->cnt, sizeof(boolean));

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
  }

  /* Initialize */
  for (jiopad = 0; jiopad < iopad_verilog_model->cnt - 1; jiopad++) {
    used_iopad[jiopad] = FALSE;
  }

  /* See if this IO should be wired to a benchmark input/output */
  /* Add signals from blif benchmark and short-wire them to FPGA I/O PADs
   * This brings convenience to checking functionality  
   */
  fprintf(fp, "//-----Link Blif Benchmark inputs to FPGA IOPADs -----\n");
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    /* General INOUT*/
    if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      iopad_idx = logical_block[iblock].mapped_spice_model_index;
      /* Make sure We find the correct logical block !*/
      assert((VPACK_INPAD == logical_block[iblock].type)||(VPACK_OUTPAD == logical_block[iblock].type));
      fprintf(fp, "//----- Blif Benchmark inout %s is mapped to FPGA IOPAD %s[%d] -----\n", 
              logical_block[iblock].name, gio_inout_prefix, iopad_idx);
      fprintf(fp, "//-----  name_tag: %s -----\n", 
              logical_block[iblock].pb->spice_name_tag);
      if (VPACK_INPAD == logical_block[iblock].type) {
        fprintf(fp, "assign %s%s[%d] = %s%s;\n",
                gio_inout_prefix, iopad_verilog_model->prefix, iopad_idx,
                logical_block[iblock].name, formal_verification_top_module_port_postfix);
      }
      if (VPACK_OUTPAD == logical_block[iblock].type) {
        fprintf(fp, "assign %s%s = %s%s[%d];\n",
                logical_block[iblock].name, formal_verification_top_module_port_postfix,
                gio_inout_prefix, iopad_verilog_model->prefix, iopad_idx);
      }
      /* Mark this iopad has been used! */
      used_iopad[iopad_idx] = TRUE;
    }
  }

  /* Wire the unused iopads to a constant */
  for (jiopad = 0; jiopad < iopad_verilog_model->cnt - 1; jiopad++) {
    /* Bypass used iopads */
    if (TRUE == used_iopad[jiopad]) {
      continue;
    }
    /* TODO: identify if this iopad is set to input or output by default
     * and see if it should be driven by a constant  
     */
    /* Wire to a contant */
    fprintf(fp, "assign %s%s[%d] = 1'b%d;\n",
            gio_inout_prefix, iopad_verilog_model->prefix, jiopad,
            verilog_default_signal_init_value);
  }

  return;
}

/* Impose the bitstream on the configuration memories */
static 
void dump_verilog_formal_verification_top_netlist_config_bitstream(t_sram_orgz_info* cur_sram_orgz_info, 
                                                                   FILE* fp) {
  t_llist* head = cur_sram_orgz_info->conf_bit_head;
  t_llist* temp = head;
  t_conf_bit_info* cur_conf_bit = NULL;

  fprintf(fp, "//----- BEGIN load bitstream to configuration memories -----\n");

  /* traverse the bitstream and assign values to configuration memories output ports */
  while (NULL != temp) { 
    /* Get conf bits */
    cur_conf_bit = (t_conf_bit_info*) (temp->dptr); 
    /* Assign */
    fprintf(fp, "assign %s.", formal_verification_top_module_uut_name); 
    /* According to the type, we allocate structs */
    switch (cur_sram_orgz_info->type) {
    case SPICE_SRAM_STANDALONE:
    case SPICE_SRAM_SCAN_CHAIN:
      dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                                  cur_conf_bit->index, cur_conf_bit->index,
                                                  VERILOG_PORT_CONKT);
      fprintf(fp, " = 1'b%d",
              cur_conf_bit->sram_bit->val); 
      break;
    case SPICE_SRAM_MEMORY_BANK:
      dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                                  cur_conf_bit->bl->addr, cur_conf_bit->bl->addr,
                                                  VERILOG_PORT_CONKT);
      fprintf(fp, " = 1'b%d",
              cur_conf_bit->bl->val); 
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid type of SRAM organization!",
                 __FILE__, __LINE__);
      exit(1);
    }

    fprintf(fp, ";\n");
    /* Go to the next */
    temp = temp->next;
  }

  fprintf(fp, "//----- END load bitstream to configuration memories -----\n");

  return;
}

/* Add stimuli for unused iopads and configuration memories */
static 
void dump_verilog_formal_verification_top_netlist_initialization(t_sram_orgz_info* cur_sram_orgz_info, 
                                                                 FILE* fp, 
                                                                 t_syn_verilog_opts syn_verilog_opts,
                                                                 t_spice verilog) {
  /* Connect FPGA top module global ports to constant or benchmark global signals! */
  dump_verilog_formal_verification_top_netlist_connect_global_ports(cur_sram_orgz_info, 
                                                                    fp, global_ports_head);

  /* Connect I/Os to benchmark I/Os or constant driver */
  dump_verilog_formal_verification_top_netlist_connect_ios(cur_sram_orgz_info, 
                                                           fp); 

  /* Assign FPGA internal SRAM/Memory ports to bitstream values */
  dump_verilog_formal_verification_top_netlist_config_bitstream(cur_sram_orgz_info, 
                                                                fp);
  
  return;
}

/** Top level function 2: the top-level netlist for formal verification purpose
 * This testbench includes a top-level module and initialization for all the configuration memories 
 */
void dump_verilog_formal_verification_top_netlist(t_sram_orgz_info* cur_sram_orgz_info,
                                                  char* circuit_name,
                                                  char* top_netlist_name,
                                                  char* verilog_dir_path,
                                                  int num_clock,
                                                  t_syn_verilog_opts fpga_verilog_opts,
                                                  t_spice verilog) {
  FILE* fp = NULL;
  char* title = my_strcat("FPGA Verilog Top-level netlist in formal verification purpose of Design: ", circuit_name);

  /* Check if the path exists*/
  fp = fopen(top_netlist_name,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create top Verilog testbench %s!",__FILE__, __LINE__, top_netlist_name); 
    exit(1);
  } 
  
  vpr_printf(TIO_MESSAGE_INFO, "Writing FPGA Top-level Verilog netlist in formal verification purpose for design %s...\n", circuit_name);
 
  /* Print the title */
  dump_verilog_file_header(fp, title);
  my_free(title);

  /* Print preprocessing flags */
  verilog_include_defines_preproc_file(fp, verilog_dir_path);

  /* Start with module declaration */
  dump_verilog_formal_verification_top_netlist_ports(cur_sram_orgz_info, fp, circuit_name);

  /* Define internal wires */
  dump_verilog_formal_verification_top_netlist_internal_wires(cur_sram_orgz_info, fp, circuit_name);

  /* Call defined top-level module */
  dump_verilog_formal_verfication_top_netlist_call_top_module(cur_sram_orgz_info, fp, circuit_name);

  /* Add stimuli for reset, set, clock and iopad signals */
  dump_verilog_formal_verification_top_netlist_initialization(cur_sram_orgz_info, fp, fpga_verilog_opts, verilog);

  /* Testbench ends*/
  fprintf(fp, "endmodule\n");

  /* Close the file*/
  fclose(fp);

  return;
}

