/***********************************/
/*  Dump Synthesizable Veriolog    */
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
#include "read_xml_spice_util.h"
#include "linkedlist.h"
#include "fpga_spice_utils.h"
#include "fpga_spice_backannotate_utils.h"
#include "fpga_spice_globals.h"
#include "fpga_spice_bitstream.h"

/* Include verilog support headers*/
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_routing.h"
#include "verilog_pbtypes.h"
#include "verilog_decoder.h"
#include "verilog_top_netlist.h"
#include "verilog_autocheck_tb.h"


/* Global varaiable only accessible in this source file*/
static char* top_netlist_bl_enable_port_name = "en_bl";
static char* top_netlist_wl_enable_port_name = "en_wl";
static char* top_netlist_bl_data_in_port_name = "data_in";
static char* top_netlist_addr_bl_port_name = "addr_bl";
static char* top_netlist_addr_wl_port_name = "addr_wl";
static char* top_netlist_array_bl_port_name = "bl_bus";
static char* top_netlist_array_wl_port_name = "wl_bus";
static char* top_netlist_array_blb_port_name = "blb_bus";
static char* top_netlist_array_wlb_port_name = "wlb_bus";
static char* top_netlist_reserved_bl_port_postfix = "_reserved_bl";
static char* top_netlist_reserved_wl_port_postfix = "_reserved_wl";
static char* top_netlist_normal_bl_port_postfix = "_bl";
static char* top_netlist_normal_wl_port_postfix = "_wl";
static char* top_netlist_normal_blb_port_postfix = "_blb";
static char* top_netlist_normal_wlb_port_postfix = "_wlb";
static char* top_netlist_scan_chain_head_prefix = "sc_in";


static char* top_tb_reset_port_name = "greset";
static char* top_tb_set_port_name = "gset";
static char* top_tb_prog_reset_port_name = "prog_reset";
static char* top_tb_prog_set_port_name = "prog_set";
static char* top_tb_config_done_port_name = "config_done";
static char* top_tb_op_clock_port_name = "op_clock";
static char* top_tb_prog_clock_port_name = "prog_clock";
static char* top_tb_inout_reg_postfix = "_reg";
static char* top_tb_clock_reg_postfix = "_reg";

/* Local Subroutines declaration */

/******** Subroutines ***********/
/** Top level function 2: Testbench for the top-level netlist
 * This testbench includes a top-level module of a mapped FPGA and voltage pulses
 */
static 
void dump_verilog_top_netlist_memory_bank_ports(FILE* fp, 
                                                enum e_dump_verilog_port_type dump_port_type) {
  t_spice_model* mem_model = NULL;
  int num_array_bl, num_array_wl;
  int bl_decoder_size, wl_decoder_size;
  char split_sign;

  split_sign = determine_verilog_generic_port_split_sign(dump_port_type);

  /* Only accept two types of dump_port_type here! */
  assert((VERILOG_PORT_INPUT == dump_port_type)||(VERILOG_PORT_CONKT == dump_port_type));

  /* Check */
  assert (sram_verilog_orgz_info->type == SPICE_SRAM_MEMORY_BANK);

  /* A valid file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Depending on the memory technology*/
  get_sram_orgz_info_mem_model(sram_verilog_orgz_info, &mem_model);
  assert(NULL != mem_model);

  determine_verilog_blwl_decoder_size(sram_verilog_orgz_info,
                                      &num_array_bl, &num_array_wl, &bl_decoder_size, &wl_decoder_size);

  /* Depend on the memory technology */
  switch (mem_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    dump_verilog_generic_port(fp, dump_port_type,
                              top_netlist_bl_enable_port_name, 0, 0);
    fprintf(fp, "%c //--- BL enable port \n", split_sign);
    dump_verilog_generic_port(fp, dump_port_type,
                              top_netlist_wl_enable_port_name, 0, 0);
    fprintf(fp, "%c //--- WL enable port \n", split_sign);
    dump_verilog_generic_port(fp, dump_port_type,
                              top_netlist_bl_data_in_port_name, 0, 0);
    fprintf(fp, "%c //--- BL data input port \n", split_sign);
    break;
  case SPICE_MODEL_DESIGN_RRAM: 
    dump_verilog_generic_port(fp, dump_port_type,
                              top_netlist_bl_enable_port_name, 0, 0);
    fprintf(fp, "%c //--- BL enable port \n", split_sign);
    dump_verilog_generic_port(fp, dump_port_type,
                              top_netlist_wl_enable_port_name, 0, 0);
    fprintf(fp, "%c //--- WL enable port \n", split_sign);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  dump_verilog_generic_port(fp, dump_port_type,
                            top_netlist_addr_bl_port_name, bl_decoder_size - 1, 0);
  fprintf(fp, "%c //--- Address of bit lines \n", split_sign);
  dump_verilog_generic_port(fp, dump_port_type,
                            top_netlist_addr_wl_port_name, wl_decoder_size - 1, 0);
  fprintf(fp, " //--- Address of word lines \n");
  
  return;
}

static 
void dump_verilog_top_testbench_global_ports(FILE* fp, t_llist* head,
                                             enum e_dump_verilog_port_type dump_port_type) {
  t_llist* temp = head;
  t_spice_model_port* cur_global_port = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
  }

  fprintf(fp, "//----- BEGIN Global ports -----\n");
  while(NULL != temp) {
    cur_global_port = (t_spice_model_port*)(temp->dptr); 
    dump_verilog_generic_port(fp, dump_port_type, 
                                  cur_global_port->prefix, 0, cur_global_port->size - 1);
    fprintf(fp, ";\n"); 
    /* Go to the next */
    temp = temp->next;
  }
  fprintf(fp, "//----- END Global ports -----\n");

  return;
}

/* Connect a global port to a voltage stimuli */
static 
void dump_verilog_top_testbench_wire_one_global_port_stimuli(FILE* fp, t_spice_model_port* cur_global_port, 
                                                             char* voltage_stimuli_port_name) {
  int ipin;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  assert(NULL != cur_global_port);

  for (ipin = 0; ipin < cur_global_port->size; ipin++) {
    fprintf(fp, "assign %s[%d] = ",
                cur_global_port->prefix, ipin);
    assert((0 == cur_global_port->default_val)||(1 == cur_global_port->default_val));
    if (1 == cur_global_port->default_val) {
      fprintf(fp, "~");
    }
    fprintf(fp, "%s;\n",
                voltage_stimuli_port_name);
  }

  return;
}

static 
void dump_verilog_top_testbench_global_ports_stimuli(FILE* fp, t_llist* head) {
  t_llist* temp = head;
  t_spice_model_port* cur_global_port = NULL;
  int ipin;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  fprintf(fp, "//----- Connecting Global ports -----\n");
  while(NULL != temp) {
    cur_global_port = (t_spice_model_port*)(temp->dptr); 
    /* Make sure this is a global port */
    assert(TRUE == cur_global_port->is_global);
    /* If this is a clock signal, connect to op_clock signal */
    if (SPICE_MODEL_PORT_CLOCK == cur_global_port->type) {
      /* Special for programming clock */
      if (TRUE == cur_global_port->is_prog) {
        dump_verilog_top_testbench_wire_one_global_port_stimuli(fp, cur_global_port, top_tb_prog_clock_port_name);
      } else {
        assert(FALSE == cur_global_port->is_prog);
        dump_verilog_top_testbench_wire_one_global_port_stimuli(fp, cur_global_port, top_tb_op_clock_port_name);
      }
    /* If this is a config_enable signal, connect to config_done signal */
    } else if (TRUE == cur_global_port->is_config_enable) {
      dump_verilog_top_testbench_wire_one_global_port_stimuli(fp, cur_global_port, top_tb_prog_clock_port_name);
    /* If this is a set/reset signal, connect to global reset and set signals */
    } else if (TRUE == cur_global_port->is_reset) {
      /* Special for programming reset */
      if (TRUE == cur_global_port->is_prog) {
        dump_verilog_top_testbench_wire_one_global_port_stimuli(fp, cur_global_port, top_tb_prog_reset_port_name);
      } else {
        assert(FALSE == cur_global_port->is_prog);
        dump_verilog_top_testbench_wire_one_global_port_stimuli(fp, cur_global_port, top_tb_reset_port_name);
      }
    /* If this is a set/reset signal, connect to global reset and set signals */
    } else if (TRUE == cur_global_port->is_set) {
      /* Special for programming reset */
      if (TRUE == cur_global_port->is_prog) {
        dump_verilog_top_testbench_wire_one_global_port_stimuli(fp, cur_global_port, top_tb_prog_set_port_name);
      } else {
        assert(FALSE == cur_global_port->is_prog);
        dump_verilog_top_testbench_wire_one_global_port_stimuli(fp, cur_global_port, top_tb_set_port_name);
      }
    } else {
    /* Other global signals stuck at the default values */
      for (ipin = 0; ipin < cur_global_port->size; ipin++) {
        fprintf(fp, "assign %s[%d] = 1'b%d;\n", 
                    cur_global_port->prefix, ipin, cur_global_port->default_val); 
      }
    }
    /* Go to the next */
    temp = temp->next;
  }
  fprintf(fp, "//----- End Connecting Global ports -----\n");

  return;
}

static 
void dump_verilog_top_netlist_scan_chain_ports(FILE* fp,
                                               enum e_dump_verilog_port_type dump_port_type) {
  /* Only accept two types of dump_port_type here! */
  assert((VERILOG_PORT_INPUT == dump_port_type)||(VERILOG_PORT_CONKT == dump_port_type));

  /* Check */
  assert (sram_verilog_orgz_info->type == SPICE_SRAM_SCAN_CHAIN);

  /* A valid file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Only the head of scan-chain will be the primary input in the top-level netlist 
   * TODO: we may have multiple scan-chains, their heads will be the primary outputs 
   */
  dump_verilog_generic_port(fp, dump_port_type,
                            top_netlist_scan_chain_head_prefix, 0, 0);
  fprintf(fp, " //---- Scan-chain head \n"); 

  return;
}

static 
void dump_verilog_top_module_ports(FILE* fp,
                                   enum e_dump_verilog_port_type dump_port_type) {
  char* port_name = NULL;
  char split_sign;
  enum e_dump_verilog_port_type actual_dump_port_type;
  boolean dump_global_port_type = FALSE;

  split_sign = determine_verilog_generic_port_split_sign(dump_port_type);

  /* Only accept two types of dump_port_type here! */
  assert((VERILOG_PORT_INPUT == dump_port_type)||(VERILOG_PORT_CONKT == dump_port_type));

  if (VERILOG_PORT_INPUT == dump_port_type) {
     dump_global_port_type = TRUE;
  }

  /* dump global ports */
  if (0 < dump_verilog_global_ports(fp, global_ports_head, dump_global_port_type)) {
    fprintf(fp, "%c\n", split_sign);
  }
  /* Inputs and outputs of I/O pads */
  /* Inout Pads */
  assert(NULL != iopad_verilog_model);
  if ((NULL == iopad_verilog_model)
   ||(iopad_verilog_model->cnt > 0)) {
    actual_dump_port_type = VERILOG_PORT_CONKT;
    if (VERILOG_PORT_INPUT == dump_port_type) {
      actual_dump_port_type = VERILOG_PORT_INOUT;
    }
    /* Malloc and assign port_name */
    port_name = (char*)my_malloc(sizeof(char)*(strlen(gio_inout_prefix) + strlen(iopad_verilog_model->prefix) + 1));
    sprintf(port_name, "%s%s", gio_inout_prefix, iopad_verilog_model->prefix);
    /* Dump a register port */
    dump_verilog_generic_port(fp, actual_dump_port_type, 
                              port_name, iopad_verilog_model->cnt - 1, 0);
    fprintf(fp, "%c //---FPGA inouts \n", split_sign); 
    /* Free port_name */
    my_free(port_name);
  }

  /* Configuration ports depend on the organization of SRAMs */
  switch(sram_verilog_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    dump_verilog_generic_port(fp, dump_port_type, 
                              sram_verilog_model->prefix, sram_verilog_model->cnt - 1, 0);
    fprintf(fp, " //--- SRAM outputs \n"); 
    /* Definition ends */
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    dump_verilog_top_netlist_scan_chain_ports(fp, dump_port_type);
    /* Definition ends */
    break;
  case SPICE_SRAM_MEMORY_BANK:
    dump_verilog_top_netlist_memory_bank_ports(fp, dump_port_type);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return; 
}

static 
void dump_verilog_top_testbench_call_top_module(FILE* fp,
                                                char* circuit_name) {

  /* Include defined top-level module */
  fprintf(fp, "//----- Device Under Test (DUT) ----\n");
  fprintf(fp, "//------Call defined Top-level Verilog Module -----\n");
  fprintf(fp, "%s_top U0 (\n", circuit_name);

  dump_verilog_top_module_ports(fp, VERILOG_PORT_CONKT);

  fprintf(fp, ");\n");
  return;
}

/* Find the number of configuration clock cycles for a top-level testbench
 * A valid configuration clock cycle is allocated for an element with 
 * (1) SRAM_val=1;
 * (2) BL = 1 && WL = 1;
 * (3) BL = 1 && WL = 0 with a paired conf_bit;
 */
static 
int dump_verilog_top_testbench_find_num_config_clock_cycles(t_llist* head) {
  int cnt = 0;
  t_llist* temp = head; 
  t_conf_bit_info* temp_conf_bit_info = NULL; 

  while (NULL != temp) {
    /* Fetch the conf_bit_info */
    temp_conf_bit_info = (t_conf_bit_info*)(temp->dptr);
    /* Check if conf_bit_info needs a clock cycle*/
    switch (sram_verilog_orgz_type) {
    case SPICE_SRAM_STANDALONE:
    case SPICE_SRAM_SCAN_CHAIN:
      cnt++;
      temp_conf_bit_info->index_in_top_tb = cnt;
      break;
    case SPICE_SRAM_MEMORY_BANK:
      cnt++;
      temp_conf_bit_info->index_in_top_tb = cnt;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    /* Go to the next */
    temp = temp->next;
  }
  
  return cnt;
}

static 
void dump_verilog_top_auto_testbench_ports(FILE* fp,
                                      char* circuit_name,
									  t_syn_verilog_opts syn_verilog_opts){
  int num_array_bl, num_array_wl;
  int bl_decoder_size, wl_decoder_size;
  int iblock, iopad_idx;
  t_spice_model* mem_model = NULL;
  char* port_name = NULL;
 
  get_sram_orgz_info_mem_model(sram_verilog_orgz_info, &mem_model);
  fprintf(fp, "`include \"%s\"\n", syn_verilog_opts.verilog_benchmark_file);
  fprintf(fp, "module %s_top_auto_tb;\n", circuit_name);
  /* Local wires */
  /* 1. reset, set, clock signals */
  /* 2. iopad signals */

  /* Connect to defined signals */
  /* set and reset signals */
  fprintf(fp, "\n");
  dump_verilog_top_testbench_global_ports(fp, global_ports_head, VERILOG_PORT_WIRE);
  fprintf(fp, "\n");

  /* TODO: dump each global signal as reg here */

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
    fprintf(fp, "; //--- FPGA inouts \n"); 
    /* Free port_name */
    my_free(port_name);
    /* Malloc and assign port_name */
    port_name = (char*)my_malloc(sizeof(char)*(strlen(gio_inout_prefix) + strlen(iopad_verilog_model->prefix) + strlen(top_tb_inout_reg_postfix) + 1));
    sprintf(port_name, "%s%s%s", gio_inout_prefix, iopad_verilog_model->prefix, top_tb_inout_reg_postfix);
    /* Dump a wired port */
    dump_verilog_generic_port(fp, VERILOG_PORT_REG, 
                              port_name, iopad_verilog_model->cnt - 1, 0);
    fprintf(fp, "; //--- reg for FPGA inouts \n"); 
    /* Free port_name */
    my_free(port_name);
  }

  /* Add a signal to identify the configuration phase is finished */
  fprintf(fp, "reg [0:0] %s;\n", top_tb_config_done_port_name);
  /* Programming clock */
  fprintf(fp, "wire [0:0] %s;\n", top_tb_prog_clock_port_name);
  fprintf(fp, "reg [0:0] %s%s;\n", top_tb_prog_clock_port_name, top_tb_clock_reg_postfix);
  /* Operation clock */
  fprintf(fp, "wire [0:0] %s;\n", top_tb_op_clock_port_name);
  fprintf(fp, "reg [0:0] %s%s;\n", top_tb_op_clock_port_name, top_tb_clock_reg_postfix);
  /* Programming set and reset */
  fprintf(fp, "reg [0:0] %s;\n", top_tb_prog_reset_port_name);
  fprintf(fp, "reg [0:0] %s;\n", top_tb_prog_set_port_name);
  /* Global set and reset */
  fprintf(fp, "reg [0:0] %s;\n", top_tb_reset_port_name);
  fprintf(fp, "reg [0:0] %s;\n", top_tb_set_port_name);
  /* Generate stimuli for global ports or connect them to existed signals */
  dump_verilog_top_testbench_global_ports_stimuli(fp, global_ports_head);

  /* Configuration ports depend on the organization of SRAMs */
  switch(sram_verilog_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    dump_verilog_generic_port(fp, VERILOG_PORT_WIRE, 
                              sram_verilog_model->prefix, sram_verilog_model->cnt - 1, 0);
    fprintf(fp, "; //---- SRAM outputs \n");
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* We put the head of scan-chains here  
	 */
    dump_verilog_generic_port(fp, VERILOG_PORT_REG, 
                              top_netlist_scan_chain_head_prefix, 0, 0);
    fprintf(fp, "; //---- Scan-chain head \n");
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* Get the number of array BLs/WLs, decoder sizes */
    determine_verilog_blwl_decoder_size(sram_verilog_orgz_info,
                                        &num_array_bl, &num_array_wl, &bl_decoder_size, &wl_decoder_size);

    fprintf(fp, "  wire [0:0] %s;\n",
            top_netlist_bl_enable_port_name
            );
    fprintf(fp, "  wire [0:0] %s;\n",
            top_netlist_wl_enable_port_name
            );
    /* Wire en_bl, en_wl to prog_clock */
    fprintf(fp, "assign %s[0:0] = %s[0:0];\n",
            top_netlist_bl_enable_port_name,
            top_tb_prog_clock_port_name);
    fprintf(fp, "assign %s [0:0]= %s[0:0];\n",
            top_netlist_wl_enable_port_name,
            top_tb_prog_clock_port_name);
    dump_verilog_generic_port(fp, VERILOG_PORT_REG, 
                              top_netlist_addr_bl_port_name, bl_decoder_size - 1, 0);
    fprintf(fp, "; //--- Address of bit lines \n"); 
    dump_verilog_generic_port(fp, VERILOG_PORT_REG, 
                              top_netlist_addr_wl_port_name, wl_decoder_size - 1, 0);
    fprintf(fp, "; //--- Address of word lines \n"); 
    /* data_in is only require by BL decoder of SRAM array 
     * As for RRAM array, the data_in signal will not be used 
     */
    if (SPICE_MODEL_DESIGN_CMOS == mem_model->design_tech) {
      fprintf(fp, "  reg [0:0] %s; // --- Data_in signal for BL decoder, only required by SRAM array \n",
                     top_netlist_bl_data_in_port_name);
    }
    /* I add all the Bit lines and Word lines here just for testbench usage
    fprintf(fp, "  input wire [%d:0] %s_out; //--- Bit lines \n", 
                   sram_verilog_model->cnt - 1, sram_verilog_model->prefix);
    fprintf(fp, "  input wire [%d:0] %s_outb; //--- Word lines \n", 
                   sram_verilog_model->cnt - 1, sram_verilog_model->prefix);
    */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }

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
      fprintf(fp, "wire %s_%s_%d_;\n",
              logical_block[iblock].name, gio_inout_prefix, iopad_idx);
      fprintf(fp, "assign %s_%s_%d_ = %s%s[%d];\n",
              logical_block[iblock].name, gio_inout_prefix, iopad_idx,
              gio_inout_prefix, iopad_verilog_model->prefix, iopad_idx);
	 // AA: Generate wire and reg to autocheck with benchmark
	  if(VPACK_OUTPAD == logical_block[iblock].type) {    
		fprintf(fp, "wire %s_benchmark;\n", logical_block[iblock].name);
		fprintf(fp, "reg %s_verification;\n", logical_block[iblock].name);
	  }	

    }
  }

  return;
}

static
void dump_verilog_top_auto_testbench_call_benchmark(FILE* fp, char* blif_circuit_name){
	int iblock, iopad_idx;
	fprintf(fp, "// Benchmark instanciation\n");
	fprintf(fp, "  %s Benchmark(\n", blif_circuit_name);
	for (iblock = 0; iblock < num_logical_blocks; iblock++) {
	  /* General INOUT*/
      if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      	iopad_idx = logical_block[iblock].mapped_spice_model_index;
      	/* Make sure We find the correct logical block !*/
      	assert((VPACK_INPAD == logical_block[iblock].type)||(VPACK_OUTPAD == logical_block[iblock].type));
		if(iblock>0){
			fprintf(fp, ",\n");
		}
	  	if(VPACK_INPAD == logical_block[iblock].type){
			/*  See if this is a clock net */
        	if (TRUE == vpack_net[logical_block[iblock].output_nets[0][0]].is_global) {  
				fprintf(fp, "        %s", top_tb_op_clock_port_name);
			}
			else{
				fprintf(fp, "        %s_%s_%d_", logical_block[iblock].name, gio_inout_prefix, iopad_idx);
			}
	  	}
		else if(VPACK_OUTPAD == logical_block[iblock].type){
			fprintf(fp, "        %s_benchmark", logical_block[iblock].name);
		}
	  }
	}
	fprintf(fp, " );\n");
	fprintf(fp, "// End Benchmark instanciation\n\n");
	return;
}

static
void dump_verilog_top_auto_testbench_check(FILE* fp){
	int iblock, iopad_idx;
	fprintf(fp, "  // Begin checking\n");
	fprintf(fp, "  always@(negedge %s) begin\n", top_tb_op_clock_port_name);
	for (iblock = 0; iblock < num_logical_blocks; iblock++) {
		if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      		iopad_idx = logical_block[iblock].mapped_spice_model_index;
      		/* Make sure We find the correct logical block !*/
      		assert((VPACK_INPAD == logical_block[iblock].type)||(VPACK_OUTPAD == logical_block[iblock].type));
			if(VPACK_OUTPAD == logical_block[iblock].type){
				fprintf(fp, "    %s_verification <= %s_benchmark ^ %s_%s_%d_ ;\n", logical_block[iblock].name, logical_block[iblock].name, logical_block[iblock].name, gio_inout_prefix, iopad_idx);
			}
		}
	}
	fprintf(fp, "  end\n\n");
	for (iblock = 0; iblock < num_logical_blocks; iblock++) {
		if (iopad_verilog_model == logical_block[iblock].mapped_spice_model) {
      		iopad_idx = logical_block[iblock].mapped_spice_model_index;
      		/* Make sure We find the correct logical block !*/
      		assert((VPACK_INPAD == logical_block[iblock].type)||(VPACK_OUTPAD == logical_block[iblock].type));
			if(VPACK_OUTPAD == logical_block[iblock].type){
				fprintf(fp, "  always@(posedge %s_verification) begin\n", logical_block[iblock].name);
				fprintf(fp, "    $display(\"Mismatch on %s_verification\");\n", logical_block[iblock].name);
				fprintf(fp, "    $finish;\n");
				fprintf(fp, "  end\n\n");
			}
		}
	}
	return;
}

void dump_verilog_top_auto_testbench(char* circuit_name,
                                char* top_netlist_name,
                                int num_clock,
                                t_syn_verilog_opts syn_verilog_opts,
                                t_spice verilog) {
  FILE* fp = NULL;
  char* title = my_strcat("FPGA Verilog Testbench for Top-level netlist of Design: ", circuit_name);
  
  /* Check if the path exists*/
  fp = fopen(top_netlist_name,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create top Verilog testbench %s!",__FILE__, __LINE__, top_netlist_name); 
    exit(1);
  } 
  
  vpr_printf(TIO_MESSAGE_INFO, "Writing Testbench for FPGA Top-level Verilog netlist for  %s...\n", circuit_name);
 
  /* Print the title */
  dump_verilog_file_header(fp, title);
  my_free(title);

  /* Start of testbench */
  dump_verilog_top_auto_testbench_ports(fp, circuit_name, syn_verilog_opts);

  /* Call defined top-level module */
  dump_verilog_top_testbench_call_top_module(fp, circuit_name);

  /* Call defined benchmark */
  dump_verilog_top_auto_testbench_call_benchmark(fp, blif_circuit_name);

  /* Add stimuli for reset, set, clock and iopad signals */
  dump_verilog_top_testbench_stimuli(fp, num_clock, syn_verilog_opts, verilog);

  /* Add output autocheck */
  dump_verilog_top_auto_testbench_check(fp);

  /* Testbench ends*/
  fprintf(fp, "endmodule\n");

  /* Close the file*/
  fclose(fp);

  return;
}

