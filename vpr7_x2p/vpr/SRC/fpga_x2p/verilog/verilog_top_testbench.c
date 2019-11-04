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
#include "route_common.h"
#include "vpr_utils.h"

/* Include spice support headers*/
#include "read_xml_spice_util.h"
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_backannotate_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "fpga_x2p_globals.h"
#include "fpga_bitstream.h"

/* Include verilog support headers*/
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_routing.h"
#include "verilog_pbtypes.h"
#include "verilog_decoder.h"
#include "verilog_top_netlist_utils.h"

#include "verilog_top_testbench.h"

/* Dump all the global ports that are stored in the linked list */
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

/* Print stimuli for global ports in top-level testbench */
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
void dump_verilog_top_testbench_ports(t_sram_orgz_info* cur_sram_orgz_info, 
                                      FILE* fp,
                                      char* circuit_name) {
  int num_array_bl, num_array_wl;
  int bl_decoder_size, wl_decoder_size;
  int iblock, iopad_idx;
  t_spice_model* mem_model = NULL;
  char* port_name = NULL;
 
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);

  fprintf(fp, "module %s_top_tb;\n", circuit_name);
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
  switch(cur_sram_orgz_info->type) {
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
    determine_blwl_decoder_size(cur_sram_orgz_info,
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
      fprintf(fp, "//-----  name_tag: %s -----\n", 
              logical_block[iblock].pb->spice_name_tag);
      fprintf(fp, "wire %s_%s_%d_;\n",
              logical_block[iblock].name, gio_inout_prefix, iopad_idx);
      fprintf(fp, "assign %s_%s_%d_ = %s%s[%d];\n",
              logical_block[iblock].name, gio_inout_prefix, iopad_idx,
              gio_inout_prefix, iopad_verilog_model->prefix, iopad_idx);
    }
  }

  return;
}

void dump_verilog_top_testbench_call_top_module(t_sram_orgz_info* cur_sram_orgz_info,
                                                FILE* fp,
                                                char* circuit_name,
                                                bool is_explicit_mapping) {
  /*
  int iblock, iopad_idx;
  */

  /* Include defined top-level module */
  fprintf(fp, "//----- Device Under Test (DUT) ----\n");
  fprintf(fp, "//------Call defined Top-level Verilog Module -----\n");
  fprintf(fp, "%s_top U0 (\n", circuit_name);

  dump_verilog_top_module_ports(cur_sram_orgz_info, fp, VERILOG_PORT_CONKT, is_explicit_mapping);

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
int dump_verilog_top_testbench_find_num_config_clock_cycles(t_sram_orgz_info* cur_sram_orgz_info,
                                                            t_llist* head) {
  int cnt = 0;
  t_llist* temp = head; 
  t_conf_bit_info* temp_conf_bit_info = NULL; 

  while (NULL != temp) {
    /* Fetch the conf_bit_info */
    temp_conf_bit_info = (t_conf_bit_info*)(temp->dptr);
    /* Check if conf_bit_info needs a clock cycle*/
    switch (cur_sram_orgz_info->type) {
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


/* Dump configuration bits of a SRAM memory bank
 * The efficient sharing BLs/WLs force configuration to be done WL by WL
 * (We always assume that Word Lines are the write/read enable signal for SRAMs)
 * All the SRAMs controlled by the same WL will be configuration during one programming cycle
 * by assign different BL values for each of them. 
 */
static 
void dump_verilog_top_testbench_sram_memory_bank_conf_bits_serial(t_sram_orgz_info* cur_sram_orgz_info, 
                                                                  FILE* fp, 
                                                                  t_llist* cur_conf_bit) {
  int num_array_bl, num_array_wl;
  int bl_decoder_size, wl_decoder_size;
  int** array_bit = NULL; 
  t_conf_bit_info*** array_conf_bit_info = NULL;

  int ibl, iwl, cur_wl, cur_bl;
  char* wl_addr = NULL;
  char* bl_addr = NULL; 

  t_llist* temp = cur_conf_bit;
  t_conf_bit_info* cur_conf_bit_info = NULL;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Dump one configuring operation on BL and WL addresses */
  determine_blwl_decoder_size(cur_sram_orgz_info,
                              &num_array_bl, &num_array_wl, &bl_decoder_size, &wl_decoder_size);

  /* We will allocate the configuration bits to be written for each programming cycle */
  array_bit = (int**)my_malloc(sizeof(int*)*num_array_wl);
  array_conf_bit_info = (t_conf_bit_info***)my_malloc(sizeof(t_conf_bit_info**)*num_array_wl);
  /* Initialization */
  for (iwl = 0; iwl < num_array_wl; iwl++) {
    array_bit[iwl] = (int*)my_malloc(sizeof(int)*num_array_bl);
    array_conf_bit_info[iwl] = (t_conf_bit_info**)my_malloc(sizeof(t_conf_bit_info*)*num_array_bl);
    for (ibl = 0; ibl < num_array_bl; ibl++) {
      array_bit[iwl][ibl] = 0;
      array_conf_bit_info[iwl][ibl] = NULL;
    }
  } 

  /* Classify each configuration bit info to its assciated bl/wl codes 
   * The bl_addr and wl_addr recorded in the list is the absoluate address,
   */
  while (NULL != temp) {
    cur_conf_bit_info = (t_conf_bit_info*)(temp->dptr);
    /* Memory bank requires the address to be given to the decoder*/
    assert( cur_conf_bit_info->bl->addr == cur_conf_bit_info->wl->addr );
    cur_wl = cur_conf_bit_info->bl->addr / num_array_bl;
    cur_bl = cur_conf_bit_info->bl->addr % num_array_bl; 
    /* Update the SRAM bit array */
    array_bit[cur_wl][cur_bl] = cur_conf_bit_info->bl->val;
    array_conf_bit_info[cur_wl][cur_bl] = cur_conf_bit_info; 
    /* Check if SRAM bit is valid */
    assert( (0 == array_bit[cur_wl][cur_bl]) || (1 == array_bit[cur_wl][cur_bl]) );
    /* Go to next */
    temp = temp->next;
  }

  /* Dump decoder input for each programming cycle */ 
  for (iwl = 0; iwl < num_array_wl; iwl++) {
    for (ibl = 0; ibl < num_array_bl; ibl++) {
      /* Bypass configuration bit not available */
      if ( NULL == array_conf_bit_info[iwl][ibl]) {
        continue;
      }
      /* Check */
      assert(( 1 == array_bit[iwl][ibl] )||( 0 == array_bit[iwl][ibl] ));
      assert( NULL != array_conf_bit_info[iwl][ibl]);
      /* If this WL is selected , we decode its index to address */
      bl_addr = (char*)my_calloc(bl_decoder_size + 1, sizeof(char));
      /* If this WL is selected , we decode its index to address */
      encode_decoder_addr(ibl, bl_decoder_size, bl_addr);
      /* Word line address */
      wl_addr = (char*)my_calloc(wl_decoder_size + 1, sizeof(char));
      encode_decoder_addr(iwl, wl_decoder_size, wl_addr);
      /* Get corresponding conf_bit */
      cur_conf_bit_info = array_conf_bit_info[iwl][ibl];
      /* One operation per clock cycle */
      /* Information about this configuration bit */
      fprintf(fp, "    //--- Configuration bit No.: %d \n", cur_conf_bit_info->index);
      fprintf(fp, "    //--- Bit Line Address: %d, \n", cur_conf_bit_info->bl->addr);
      fprintf(fp, "    //--- Word Line Address: %d \n ", cur_conf_bit_info->wl->addr);
      fprintf(fp, "    //--- Bit Line index: %d, \n", ibl);
      fprintf(fp, "    //--- Word Line index: %d \n ", iwl);
      fprintf(fp, "    //--- Bit Line Value: %d, \n", cur_conf_bit_info->bl->val);
      fprintf(fp, "    //--- Word Line Value: %d \n ", cur_conf_bit_info->wl->val);
      fprintf(fp, "    //--- SPICE model name: %s \n", cur_conf_bit_info->parent_spice_model->name);
      fprintf(fp, "    //--- SPICE model index: %d \n", cur_conf_bit_info->parent_spice_model_index);
      fprintf(fp, "    prog_cycle_blwl(%d'b%s, %d'b%s, 1'b%d); //--- (BL_addr_code, WL_addr_code, bl_data_in) \n",
                  bl_decoder_size, bl_addr, wl_decoder_size, wl_addr, array_bit[iwl][ibl]);
      fprintf(fp, "\n");
    }
  }

  /* Free */
  for (iwl = 0; iwl < num_array_wl; iwl++) {
    my_free(array_bit[iwl]);
    my_free(array_conf_bit_info[iwl]);
  }
  my_free(array_bit);
  my_free(array_conf_bit_info);

  return;
}

static 
void dump_verilog_top_testbench_rram_memory_bank_conf_bits_serial(t_sram_orgz_info* cur_sram_orgz_info, 
                                                                  FILE* fp, 
                                                                  t_llist* cur_conf_bit) {
  int num_bl, num_wl;
  int bl_decoder_size, wl_decoder_size;
  char* wl_addr = NULL;
  char* bl_addr = NULL; 

  t_llist* temp = cur_conf_bit;
  t_conf_bit_info* cur_conf_bit_info = NULL;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!",__FILE__, __LINE__); 
    exit(1);
  } 


  /* Dump one configuring operation on BL and WL addresses */
  determine_blwl_decoder_size(cur_sram_orgz_info,
                              &num_bl, &num_wl, &bl_decoder_size, &wl_decoder_size);

  while (NULL != temp) {
    cur_conf_bit_info = (t_conf_bit_info*)(temp->dptr);
    /* For memory bank, we do not care the sequence.
     * To be easy to understand, we go from the first to the last
     * IMPORTANT: sequence seems to be critical.
     * Reversing the sequence leading to functional incorrect.
     */
    /* Memory bank requires the address to be given to the decoder*/
    /* Decode address to BLs and WLs*/
    /* Encode addresses */
    /* Bit line address */
    /* If this WL is selected , we decode its index to address */
    bl_addr = (char*)my_calloc(bl_decoder_size + 1, sizeof(char));
    /* If this WL is selected , we decode its index to address */
    assert(NULL != cur_conf_bit_info->bl);
    encode_decoder_addr(cur_conf_bit_info->bl->addr, bl_decoder_size, bl_addr);
    /* Word line address */
    wl_addr = (char*)my_calloc(wl_decoder_size + 1, sizeof(char));
    encode_decoder_addr(cur_conf_bit_info->wl->addr, wl_decoder_size, wl_addr);
    assert(NULL != cur_conf_bit_info->wl);
    /* One operation per clock cycle */
    /* Information about this configuration bit */
    fprintf(fp, "    //--- Configuration bit No.: %d \n", cur_conf_bit_info->index);
    fprintf(fp, "    //--- Bit Line: %d, \n", cur_conf_bit_info->bl->val);
    fprintf(fp, "    //--- Word Line: %d \n ", cur_conf_bit_info->wl->val);
    fprintf(fp, "    //--- SPICE model name: %s \n", cur_conf_bit_info->parent_spice_model->name);
    fprintf(fp, "    //--- SPICE model index: %d \n", cur_conf_bit_info->parent_spice_model_index);
    fprintf(fp, "    prog_cycle_blwl(%d'b%s, %d'b%s); //--- (BL_addr_code, WL_addr_code) \n",
                bl_decoder_size, bl_addr, wl_decoder_size, wl_addr);
    fprintf(fp, "\n");
    /* Free */
    my_free(wl_addr);
    my_free(bl_addr);
    /* Go to next */
    temp = temp->next;
  }

  return;
}

static 
void dump_verilog_top_testbench_memory_bank_conf_bits_serial(t_sram_orgz_info* cur_sram_orgz_info, 
                                                             FILE* fp, 
                                                             t_llist* cur_conf_bit) {
  t_spice_model* mem_model = NULL;

  /* Depending on the memory technology*/
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
  assert(NULL != mem_model);

  /* Check */
  assert(SPICE_SRAM_MEMORY_BANK == cur_sram_orgz_info->type);

  /* fork on the memory technology */
  switch (mem_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    dump_verilog_top_testbench_sram_memory_bank_conf_bits_serial(cur_sram_orgz_info, fp, cur_conf_bit); 
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    dump_verilog_top_testbench_rram_memory_bank_conf_bits_serial(cur_sram_orgz_info, fp, cur_conf_bit); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for memory in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* For scan chain, the last bit should go first!*/
/* The sequence of linked list is already reversed, we just need to output each bit */
static 
void dump_verilog_top_testbench_scan_chain_conf_bits_serial(FILE* fp, 
                                                            t_llist* cur_conf_bit) { 
  t_llist* temp = cur_conf_bit;
  t_conf_bit_info* cur_conf_bit_info = NULL;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Configuration phase: configure each SRAM or BL/WL */
  fprintf(fp, "//----- Configuration phase -----\n");
  fprintf(fp, "initial\n");
  fprintf(fp, "  begin //--- Scan_chain default input\n");
  /* Initial value should be the first configuration bits
   * In the rest of programming cycles, 
   * configuration bits are fed at the falling edge of programming clock.
   */
  /* We do not care the value of scan_chain head during the first programming cycle 
   * It is reset anyway
   */
  fprintf(fp, "    "); 
  dump_verilog_generic_port(fp, VERILOG_PORT_CONKT,
                            top_netlist_scan_chain_head_prefix, 0, 0);
  fprintf(fp, " = 1'b%d;\n", 
              0 );
  fprintf(fp, "\n");
  /* Check we have a valid first bit */
  while (NULL != temp) {
    cur_conf_bit_info = (t_conf_bit_info*)(temp->dptr);
    /* Scan-chain only loads the SRAM values */ 
    fprintf(fp, "//---- Configuration bit No.: %d \n ", cur_conf_bit_info->index);
    fprintf(fp, "//---- SRAM value: %d \n", cur_conf_bit_info->sram_bit->val);
    fprintf(fp, "//---- SPICE model name: %s \n ", cur_conf_bit_info->parent_spice_model->name);
    fprintf(fp, "//---- SPICE model index: %d \n", cur_conf_bit_info->parent_spice_model_index);
    fprintf(fp, "\n");
    /* First bit is special */
    fprintf(fp, " prog_cycle_scan_chain(1'b%d); //--- (Scan_chain bits) \n",
                cur_conf_bit_info->sram_bit->val);
    fprintf(fp, "\n");
    /* Go to next */
    temp = temp->next;
  }
  fprintf(fp, "  end\n");
  fprintf(fp, "//----- END of Configuration phase -----\n");

  return;
}

/* dump configuration bits which are stored in the linked list 
 * USE while LOOP !!! Recursive method causes memory corruptions!
 * We start dump configuration bit from the tail of the linked list
 * until the head of the linked list
 */
static 
void dump_verilog_top_testbench_one_conf_bit_serial(t_sram_orgz_info* cur_sram_orgz_info, 
                                                    FILE* fp, 
                                                    t_llist* cur_conf_bit) {
    						
  /* We already touch the tail, start dump */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    dump_verilog_top_testbench_scan_chain_conf_bits_serial(fp, cur_conf_bit); 
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* Should work differently depending on memory technology */
    dump_verilog_top_testbench_memory_bank_conf_bits_serial(cur_sram_orgz_info, fp, cur_conf_bit); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }
 
  return;
}

static 
void dump_verilog_top_testbench_conf_bits_serial(t_sram_orgz_info* cur_sram_orgz_info, 
                                                 FILE* fp, 
                                                 t_llist* head) {
  int num_bl, num_wl;
  int bl_decoder_size, wl_decoder_size;
  t_llist* new_head = head;

  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    /* For scan chain, the last bit should go first!*/
    /* We do not need to reverse the linked list HERE !!! 
     * Because, it is already arranged in the seqence of MSB to LSB
     * Note that the first element in the linked list is the last bit now!
     */
    /* For each element in linked list, generate a voltage stimuli */
    dump_verilog_top_testbench_one_conf_bit_serial(cur_sram_orgz_info,
                                                   fp, head);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* Configuration bits are loaded differently depending on memory technology */
    determine_blwl_decoder_size(cur_sram_orgz_info,
                                &num_bl, &num_wl, &bl_decoder_size, &wl_decoder_size);
    /* Configuration phase: configure each SRAM or BL/WL */
    fprintf(fp, "//----- Configuration phase -----\n");
    fprintf(fp, "initial\n");
    fprintf(fp, "  begin //--- BL_WL_ADDR\n");
    fprintf(fp, "    addr_bl = {%d {1'b0}};\n", bl_decoder_size);
    fprintf(fp, "    addr_wl = {%d {1'b0}};\n", wl_decoder_size);
    /* For each element in linked list, generate a voltage stimuli */
    /* Reverse the linked list first !!! */
    new_head = reverse_llist(new_head); 
    dump_verilog_top_testbench_one_conf_bit_serial(cur_sram_orgz_info, fp, new_head);
    /* Recover the sequence of linked list (reverse again) !!! */
    new_head = reverse_llist(new_head); 
    /* Check */
    assert(head == new_head);
    fprintf(fp, "  end\n");
    fprintf(fp, "//----- END of Configuration phase -----\n");
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* Print tasks, which is very useful in generating stimuli for each clock cycle 
 * For memory-bank manipulations only 
 */
static 
void dump_verilog_top_testbench_stimuli_serial_version_tasks_memory_bank(t_sram_orgz_info* cur_sram_orgz_info, 
                                                                         FILE* fp) {
  int num_array_bl, num_array_wl;
  int bl_decoder_size, wl_decoder_size;
  t_spice_model* mem_model = NULL;

  /* Check */
  assert (cur_sram_orgz_info->type == SPICE_SRAM_MEMORY_BANK);

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Find the number of array BLs/WLs and decoder sizes */
  determine_blwl_decoder_size(cur_sram_orgz_info,
                              &num_array_bl, &num_array_wl, &bl_decoder_size, &wl_decoder_size);

  /* Depending on the memory technology*/
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
  assert(NULL != mem_model);

  /* Depend on the memory technology */
  switch (mem_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    /* Declare two tasks:
     * 1. assign BL/WL IO pads values during a programming clock cycle
     * 2. assign BL/WL IO pads values during a operating clock cycle
     */
    fprintf(fp, "\n");
    fprintf(fp, "//----- Task 1: input values during a programming clock cycle -----\n");
    fprintf(fp, "task prog_cycle_blwl;\n");
    fprintf(fp, "  input [%d:0] bl_addr_val;\n", bl_decoder_size - 1);
    fprintf(fp, "  input [%d:0] wl_addr_val;\n", wl_decoder_size - 1);
    fprintf(fp, "  input [0:0] bl_data_in_val;\n");
    fprintf(fp, "  begin\n");
    fprintf(fp, "    @(posedge %s);\n", top_tb_prog_clock_port_name);
    /*
    fprintf(fp, "    $display($time, \"Programming cycle: load BL address with \%h, load WL address with \%h\",\n");
    fprintf(fp, "             bl_addr_val, wl_addr_val);\n");
    */
    fprintf(fp, "    %s = bl_addr_val;\n", top_netlist_addr_bl_port_name);
    fprintf(fp, "    %s = wl_addr_val;\n", top_netlist_addr_wl_port_name);
    fprintf(fp, "    %s = bl_data_in_val;\n", top_netlist_bl_data_in_port_name);
    fprintf(fp, "  end\n");
    fprintf(fp, "endtask //---prog_cycle_stimuli\n");
    fprintf(fp, "\n");
    fprintf(fp, "//----- Task 2: input values during a operating clock cycle -----\n");
    fprintf(fp, "task op_cycle_blwl;\n");
    fprintf(fp, "  input [%d:0] bl_addr_val;\n", bl_decoder_size - 1);
    fprintf(fp, "  input [%d:0] wl_addr_val;\n", wl_decoder_size - 1);
    fprintf(fp, "  input [0:0] bl_data_in_val;\n");
    fprintf(fp, "  begin\n");
    fprintf(fp, "    @(posedge %s);\n", top_tb_op_clock_port_name);
    /*
    fprintf(fp, "    $display($time, \"Operating cycle: load BL address with \%h, load WL address with \%h\",\n");
    fprintf(fp, "             bl_addr_val, wl_addr_val);\n");
    */
    fprintf(fp, "    %s = bl_addr_val;\n", top_netlist_addr_bl_port_name);
    fprintf(fp, "    %s = wl_addr_val;\n", top_netlist_addr_wl_port_name);
    fprintf(fp, "    %s = bl_data_in_val;\n", top_netlist_bl_data_in_port_name);
    fprintf(fp, "  end\n");
    fprintf(fp, "endtask //---op_cycle_stimuli\n");
    fprintf(fp, "\n");
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* Declare two tasks:
     * 1. assign BL/WL IO pads values during a programming clock cycle
     * 2. assign BL/WL IO pads values during a operating clock cycle
     */
    fprintf(fp, "\n");
    fprintf(fp, "//----- Task 1: input values during a programming clock cycle -----\n");
    fprintf(fp, "task prog_cycle_blwl;\n");
    fprintf(fp, "  input [%d:0] bl_addr_val;\n", bl_decoder_size - 1);
    fprintf(fp, "  input [%d:0] wl_addr_val;\n", wl_decoder_size - 1);
    fprintf(fp, "  begin\n");
    fprintf(fp, "    @(posedge %s);\n", top_tb_prog_clock_port_name);
    /*
    fprintf(fp, "    $display($time, \"Programming cycle: load BL address with \%h, load WL address with \%h\",\n");
    fprintf(fp, "             bl_addr_val, wl_addr_val);\n");
    */
    fprintf(fp, "    %s = bl_addr_val;\n", top_netlist_addr_bl_port_name);
    fprintf(fp, "    %s = wl_addr_val;\n", top_netlist_addr_wl_port_name);
    fprintf(fp, "  end\n");
    fprintf(fp, "endtask //---prog_cycle_stimuli\n");
    fprintf(fp, "\n");
    fprintf(fp, "//----- Task 2: input values during a operating clock cycle -----\n");
    fprintf(fp, "task op_cycle_blwl;\n");
    fprintf(fp, "  input [%d:0] bl_addr_val;\n", bl_decoder_size - 1);
    fprintf(fp, "  input [%d:0] wl_addr_val;\n", wl_decoder_size - 1);
    fprintf(fp, "  begin\n");
    fprintf(fp, "    @(posedge %s);\n", top_tb_op_clock_port_name);
    /*
    fprintf(fp, "    $display($time, \"Operating cycle: load BL address with \%h, load WL address with \%h\",\n");
    fprintf(fp, "             bl_addr_val, wl_addr_val);\n");
    */
    fprintf(fp, "    %s = bl_addr_val;\n", top_netlist_addr_bl_port_name);
    fprintf(fp, "    %s = wl_addr_val;\n", top_netlist_addr_wl_port_name);
    fprintf(fp, "  end\n");
    fprintf(fp, "endtask //---op_cycle_stimuli\n");
    fprintf(fp, "\n");
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* Print tasks, which is very useful in generating stimuli for each clock cycle 
 * For scan-chain manipulation: 
 * During each programming cycle, we feed the input of scan chain with a memory bit
 */
static 
void dump_verilog_top_testbench_stimuli_serial_version_tasks_scan_chain(t_sram_orgz_info* cur_sram_orgz_info, 
                                                                        FILE* fp) {

  /* Check */
  assert ( SPICE_SRAM_SCAN_CHAIN == cur_sram_orgz_info->type);

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Feed the scan-chain input at each falling edge of programming clock 
   * It aims at avoid racing the programming clock (scan-chain data changes at the rising edge). 
   */
  fprintf(fp, "\n");
  fprintf(fp, "//----- Task: input values during a programming clock cycle -----\n");
  fprintf(fp, "task prog_cycle_scan_chain;\n");
  fprintf(fp, "  input %s_val;\n", 
              top_netlist_scan_chain_head_prefix);
  fprintf(fp, "  begin\n");
  fprintf(fp, "    @(negedge %s);\n", top_tb_prog_clock_port_name);
  fprintf(fp, "    "); 
  dump_verilog_generic_port(fp, VERILOG_PORT_CONKT,
                            top_netlist_scan_chain_head_prefix, 0, 0);
  fprintf(fp, " = %s_val;\n", 
              top_netlist_scan_chain_head_prefix);
  fprintf(fp, "  end\n");
  fprintf(fp, "endtask //---prog_cycle_stimuli\n");
  fprintf(fp, "\n");
  
  return;
}

/* Print tasks, which is very useful in generating stimuli for each clock cycle */
static 
void dump_verilog_top_testbench_stimuli_serial_version_tasks(t_sram_orgz_info* cur_sram_orgz_info, 
                                                             FILE* fp) {

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  } 

  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    dump_verilog_top_testbench_stimuli_serial_version_tasks_scan_chain(cur_sram_orgz_info, fp);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    dump_verilog_top_testbench_stimuli_serial_version_tasks_memory_bank(cur_sram_orgz_info, fp);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}


/* Print Stimulations for top-level netlist 
 * Test a complete configuration phase: 
 * configuration bits are programming in serial (one by one)
 * Task list:
 * 1. For clock signal, we should create voltage waveforms for two types of clock signals:
 *    a. operation clock
 *    b. programming clock 
 * 2. For Set/Reset, TODO: should we reset the chip after programming phase ends and before operation phase starts
 * 3. For input/output clb nets (mapped to I/O grids), we should create voltage waveforms only after programming phase 
 */
static 
void dump_verilog_top_testbench_stimuli_serial_version(t_sram_orgz_info* cur_sram_orgz_info, 
                                                       FILE* fp,
                                                       t_spice spice) {
  int inet, iblock, iopad_idx;
  int found_mapped_inpad = 0;
  int num_config_clock_cycles = 0;
  float prog_clock_period = (1./spice.spice_params.stimulate_params.prog_clock_freq);
  float op_clock_period = (1./spice.spice_params.stimulate_params.op_clock_freq);

  /* Find Input Pad Spice model */
  t_spice_net_info* cur_spice_net_info = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Dump tasks */
  dump_verilog_top_testbench_stimuli_serial_version_tasks(cur_sram_orgz_info, fp);

  /* Estimate the number of configuration clock cycles 
   * by traversing the linked-list and count the number of SRAM=1 or BL=1&WL=1 in it.
   * We plus 1 additional config clock cycle here because we need to reset everything during the first clock cycle
   */
  num_config_clock_cycles = 1 + dump_verilog_top_testbench_find_num_config_clock_cycles(cur_sram_orgz_info, 
                                                                                        cur_sram_orgz_info->conf_bit_head);
  fprintf(fp, "//----- Number of clock cycles in configuration phase: %d -----\n", 
              num_config_clock_cycles);

  /* config_done signal: indicate when configuration is finished */
  fprintf(fp, "//----- %s ----\n", top_tb_config_done_port_name);
  fprintf(fp, "initial\n");
  fprintf(fp, "  begin //--- CONFIG_DONE GENERATOR\n");
  fprintf(fp, "    %s = 1'b0;\n", top_tb_config_done_port_name);
  fprintf(fp, "    //----- %s signal is enabled after %d configurating operations are done ----\n", 
              top_tb_config_done_port_name, num_config_clock_cycles);
  fprintf(fp, "    #%.2f %s = 1'b1;\n", 
              num_config_clock_cycles * prog_clock_period / verilog_sim_timescale, top_tb_config_done_port_name);
  fprintf(fp, "  end\n");
  fprintf(fp, "//----- END of %s ----\n", 
              top_tb_config_done_port_name);
  fprintf(fp, "\n");

  /* Generate stimilus of programming clock */
  fprintf(fp, "//----- Raw Programming clock ----\n");
  fprintf(fp, "initial\n");
  fprintf(fp, "  begin //--- PROG_CLOCK INITIALIZATION\n");
  fprintf(fp, "    %s%s = 1'b0;\n", top_tb_prog_clock_port_name, top_tb_clock_reg_postfix);
  fprintf(fp, "  end\n");
  fprintf(fp, "always\n"); 
  fprintf(fp, "  begin //--- PROG_CLOCK GENERATOR\n");
  fprintf(fp, "    #%.2f %s%s = ~%s%s;\n", 
              0.5*prog_clock_period / verilog_sim_timescale,
              top_tb_prog_clock_port_name, top_tb_clock_reg_postfix, 
              top_tb_prog_clock_port_name, top_tb_clock_reg_postfix);
  fprintf(fp, "  end\n");
  fprintf(fp, "//----- END of  Programming clock ----\n");
  fprintf(fp, "\n");
  /* Programming clock should be only enabled during programming phase.
   * When configuration is done (config_done is enabled), programming clock should be always zero.
   */
  fprintf(fp, "//---- Actual programming clock is triggered only when %s and %s are disabled\n", 
              top_tb_config_done_port_name, 
              top_tb_prog_reset_port_name); 
  fprintf(fp, "  assign %s = %s%s & (~%s) & (~%s);\n",
              top_tb_prog_clock_port_name,
              top_tb_prog_clock_port_name, top_tb_clock_reg_postfix,
              top_tb_config_done_port_name,
              top_tb_prog_reset_port_name); 
  /*
  fprintf(fp, "  assign %s = %s%s & (~%s);\n",
              top_tb_prog_clock_port_name,
              top_tb_prog_clock_port_name, top_tb_clock_reg_postfix,
              top_tb_config_done_port_name); 
  */
  fprintf(fp, "//----- END of Actual Programming clock ----\n");
  fprintf(fp, "\n");

  /* Generate stimilus of programming clock */
  fprintf(fp, "//----- Raw Operation clock ----\n");
  fprintf(fp, "initial\n");
  fprintf(fp, "  begin //--- OP_CLOCK INITIALIZATION\n");
  fprintf(fp, "    %s%s = 1'b0;\n", top_tb_op_clock_port_name, top_tb_clock_reg_postfix);
  fprintf(fp, "  end\n");
  fprintf(fp, "always wait(~%s)\n", top_tb_reset_port_name); 
  fprintf(fp, "  begin //--- OP_CLOCK GENERATOR\n");
  fprintf(fp, "    #%.2f %s%s = ~%s%s;\n", 
              0.5*op_clock_period / verilog_sim_timescale,
              top_tb_op_clock_port_name, top_tb_clock_reg_postfix,
              top_tb_op_clock_port_name, top_tb_clock_reg_postfix);
  fprintf(fp, "  end\n");
  fprintf(fp, "//----- END of Operation clock ----\n");
  /* Operation clock should be enabled after programming phase finishes.
   * Before configuration is done (config_done is enabled), operation clock should be always zero.
   */
  fprintf(fp, "//---- Actual operation clock is triggered only when %s is enabled \n", 
              top_tb_config_done_port_name); 
  fprintf(fp, "  assign %s = %s%s & (%s);\n",
              top_tb_op_clock_port_name,
              top_tb_op_clock_port_name, top_tb_clock_reg_postfix,
              top_tb_config_done_port_name);
  fprintf(fp, "//----- END of Actual Operation clock ----\n");
  fprintf(fp, "\n");

  /* Reset signal for configuration circuit : only enable during the first clock cycle in programming phase */
  fprintf(fp, "//----- Programming Reset Stimuli ----\n");
  fprintf(fp, "initial\n");
  fprintf(fp, "  begin //--- PROGRAMMING RESET GENERATOR\n");
  fprintf(fp, " %s = 1'b1;\n", top_tb_prog_reset_port_name);
  /* Reset is enabled until the first clock cycle in operation phase */
  fprintf(fp, "//----- Reset signal is enabled until the first clock cycle in programming phase ----\n");
  fprintf(fp, "#%.2f %s = 1'b0;\n", 
              (1 * prog_clock_period) / verilog_sim_timescale,
              top_tb_prog_reset_port_name);
  fprintf(fp, "end\n");
  fprintf(fp, "\n");

  /* Set signal for configuration circuit : only enable during the first clock cycle in programming phase */
  fprintf(fp, "//----- Programming set Stimuli ----\n");
  fprintf(fp, "initial\n");
  fprintf(fp, "  begin //--- PROGRAMMING SET GENERATOR\n");
  fprintf(fp, "%s = 1'b0;\n", top_tb_prog_set_port_name);
  fprintf(fp, "//----- Programming set signal is always disabled -----\n");
  fprintf(fp, "end\n");
  fprintf(fp, "\n");

  /* reset signals: only enabled during the first clock cycle in operation phase */
  fprintf(fp, "//----- Reset Stimuli ----\n");
  fprintf(fp, "initial\n");
  fprintf(fp, "  begin //--- RESET GENERATOR\n");
  fprintf(fp, " %s = 1'b1;\n", top_tb_reset_port_name);
  /* Reset is enabled until the first clock cycle in operation phase */
  fprintf(fp, "//----- Reset signal is enabled until the first clock cycle in operation phase ----\n");
  fprintf(fp, "wait(%s);\n", 
               top_tb_config_done_port_name); 
  fprintf(fp, "#%.2f %s = 1'b1;\n", 
              (1 * op_clock_period)/ verilog_sim_timescale,
              top_tb_reset_port_name);
  fprintf(fp, "#%.2f %s = 1'b0;\n", 
              (2 * op_clock_period) / verilog_sim_timescale,
              top_tb_reset_port_name);
  fprintf(fp, "end\n");
  fprintf(fp, "\n");

  /* set signals */
  fprintf(fp, "//----- Set Stimuli ----\n");
  fprintf(fp, "initial\n");
  fprintf(fp, "  begin //--- SET GENERATOR\n");
  fprintf(fp, "%s = 1'b0;\n", top_tb_set_port_name);
  fprintf(fp, "//----- Set signal is always disabled -----\n");
  fprintf(fp, "end\n");
  fprintf(fp, "\n");

  /* Inputs stimuli: BL/WL address lines */
  dump_verilog_top_testbench_conf_bits_serial(cur_sram_orgz_info, fp, cur_sram_orgz_info->conf_bit_head); 
  
  /* For each input_signal
   * TODO: this part is low-efficent for run-time concern... Need improve
   */
  assert(NULL != iopad_verilog_model);
  for (iopad_idx = 0; iopad_idx < iopad_verilog_model->cnt; iopad_idx++) {
    /* Find if this inpad is mapped to a logical block */
    found_mapped_inpad = 0;
    for (iblock = 0; iblock < num_logical_blocks; iblock++) {
      /* Bypass OUTPAD: donot put any voltage stimuli */
      /* Make sure We find the correct logical block !*/
      if ((iopad_verilog_model == logical_block[iblock].mapped_spice_model)
         &&(iopad_idx == logical_block[iblock].mapped_spice_model_index)) {
        /* Output PAD only need a short connection */
        if (VPACK_OUTPAD == logical_block[iblock].type) {
          fprintf(fp, "//----- Output %s does not need a Stimuli ----\n", logical_block[iblock].name);
          fprintf(fp, "initial\n");
          fprintf(fp, "  begin //--- Input %s[%d] GENERATOR\n", gio_inout_prefix, iopad_idx);
          fprintf(fp, "    %s%s%s[%d] = 1'b%d;\n", 
                  gio_inout_prefix, iopad_verilog_model->prefix, top_tb_inout_reg_postfix, iopad_idx,
                  verilog_default_signal_init_value);
          fprintf(fp, "end\n");
          found_mapped_inpad = 1;
          break;
        }
      /* Input PAD only need a short connection */
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
        /* Connect the reg to inouts */
        fprintf(fp, "//----- Input %s Stimuli ----\n", logical_block[iblock].name);
        fprintf(fp, "assign %s%s[%d] = %s%s%s[%d];\n",
                gio_inout_prefix, iopad_verilog_model->prefix, iopad_idx,
                gio_inout_prefix, iopad_verilog_model->prefix, top_tb_inout_reg_postfix, iopad_idx);
        /* Get the net information */
        /* TODO: Give the net name in the blif file !*/
        fprintf(fp, "initial\n");
        fprintf(fp, "  begin //--- Input %s GENERATOR\n", logical_block[iblock].name);
        fprintf(fp, "    %s%s%s[%d] = 1'b%d;\n", 
                gio_inout_prefix, iopad_verilog_model->prefix, top_tb_inout_reg_postfix, iopad_idx,
                cur_spice_net_info->init_val);
        fprintf(fp, "end\n");
        fprintf(fp, "always wait (~%s)\n", top_tb_reset_port_name);
        fprintf(fp, "  begin \n");
        fprintf(fp, "    %s%s%s[%d] = ~%s%s%s[%d];\n #%.2f;\n", 
                gio_inout_prefix, iopad_verilog_model->prefix, top_tb_inout_reg_postfix, iopad_idx,
                gio_inout_prefix, iopad_verilog_model->prefix, top_tb_inout_reg_postfix, iopad_idx,
                (((float)(int)((100 * op_clock_period) / verilog_sim_timescale) / 100) * ((int)(cur_spice_net_info->probability / cur_spice_net_info->density)+ iblock)));
        fprintf(fp, "    %s%s%s[%d] = ~%s%s%s[%d];\n #%.2f;\n", 
                gio_inout_prefix, iopad_verilog_model->prefix, top_tb_inout_reg_postfix, iopad_idx,
                gio_inout_prefix, iopad_verilog_model->prefix, top_tb_inout_reg_postfix, iopad_idx,
                (((float)(int)((100 * op_clock_period) / verilog_sim_timescale) / 100) * ((int)(cur_spice_net_info->density / cur_spice_net_info->probability) * 2.5 + iblock)));
        fprintf(fp, "  end \n");
        fprintf(fp, "\n");
        found_mapped_inpad++;
      }
    } 
    assert((0 == found_mapped_inpad)||(1 == found_mapped_inpad));
    /* If we find one iopad already, we finished in this round here */
    if (1 == found_mapped_inpad) {
      continue;
    }
    /* TODO: identify if this iopad is set to input or output by default
     * and see if it should be driven by a constant  
     */
    /* if we cannot find any mapped inpad from tech.-mapped netlist, give a default */
    /* Connect the reg to inouts */
    fprintf(fp, "//----- Input %s[%d] Stimuli ----\n", gio_inout_prefix, iopad_idx);
    fprintf(fp, "assign %s%s[%d] = %s%s%s[%d];\n",
            gio_inout_prefix, iopad_verilog_model->prefix, iopad_idx,
            gio_inout_prefix, iopad_verilog_model->prefix, top_tb_inout_reg_postfix, iopad_idx);
    /* TODO: Give the net name in the blif file !*/
    fprintf(fp, "initial\n");
    fprintf(fp, "  begin //--- Input %s[%d] GENERATOR\n", gio_inout_prefix, iopad_idx);
    fprintf(fp, "    %s%s%s[%d] = 1'b%d;\n", 
            gio_inout_prefix, iopad_verilog_model->prefix, top_tb_inout_reg_postfix, iopad_idx,
            verilog_default_signal_init_value);
    fprintf(fp, "end\n");
  }

  /* Finish */
  
  return;
}

/* Generate the stimuli for the top-level testbench 
 * The simulation consists of two phases: configuration phase and operation phase
 * We have two choices for the configuration phase:
 * 1. Configuration bits are loaded serially. 
 *    This is actually what we do for a physical FPGA
 * 2. Configuration bits are loaded parallel. 
 *    This is only feasible in simulation, which is convenient to check functionality.
 */
void dump_verilog_top_testbench_stimuli(t_sram_orgz_info* cur_sram_orgz_info, 
                                        FILE* fp,
                                        t_spice verilog) {

  /* Only serial version is avaiable now */
  dump_verilog_top_testbench_stimuli_serial_version(cur_sram_orgz_info, fp, verilog);
  /*
  if (TRUE == syn_verilog_opts.tb_serial_config_mode) {
  } else {
    dump_verilog_top_testbench_stimuli_parallel_version(fp, num_clock, verilog);
  }
  */

  return;
}

/* Dump the ports for input blif nestlist */
static 
void dump_verilog_input_blif_testbench_ports(FILE* fp,
                                             char* circuit_name) {
  int iblock;
  
  fprintf(fp, "module %s_blif_tb;\n", circuit_name);
 
  fprintf(fp, "  reg %s;\n", top_tb_reset_port_name);

  /* Print all the inputs/outputs*/
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    /* Make sure We find the correct logical block !*/
    switch (logical_block[iblock].type) {
    case VPACK_INPAD:
      fprintf(fp, "  reg %s;\n", logical_block[iblock].name);
      break;
    case VPACK_OUTPAD:
      fprintf(fp, "  wire %s;\n", logical_block[iblock].name);
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

  return;
}

/* Dump the module to be tested for input blif nestlist */
static 
void dump_verilog_input_blif_testbench_call_top_module(FILE* fp, 
                                                       char* circuit_name) {
  int iblock, cnt;
  /* Initialize a counter */
  cnt = 0;

  fprintf(fp, "%s uut (\n", circuit_name);
  /* Print all the inputs/outputs*/
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    /* Make sure We find the correct logical block !*/
    switch (logical_block[iblock].type) {
    case VPACK_INPAD:
    case VPACK_OUTPAD:
      /* Dump a comma only when needed*/
      if (0 < cnt) {
        fprintf(fp, ",\n");
      }
      /* Dump an input/output, avoid additional comma leading to wrong connections */
      fprintf(fp, "%s", logical_block[iblock].name);
      /* Update counter */
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
  fprintf(fp, "//---- End call module %s (%d inputs and outputs) -----\n",
               circuit_name, cnt);

  return;
}

/* Dump voltage stimuli for input blif nestlist */
static 
void dump_verilog_input_blif_testbench_stimuli(FILE* fp,
                                               t_spice spice) {
  int iblock, inet;
  t_spice_net_info* cur_spice_net_info = NULL;
  float op_clock_period = 1./spice.spice_params.stimulate_params.op_clock_freq;

  fprintf(fp, "//----- Operation clock period: %.2g -----\n", op_clock_period);

  /* reset signals: only enabled during the first clock cycle in operation phase */
  fprintf(fp, "//----- Reset Stimuli ----\n");
  fprintf(fp, "initial\n");
  fprintf(fp, "  begin //--- RESET GENERATOR\n");
  fprintf(fp, " %s = 1'b1;\n", top_tb_reset_port_name);
  /* Reset is enabled until the first clock cycle in operation phase */
  fprintf(fp, "//----- Reset signal is enabled until the first clock cycle in operation phase ----\n");
  fprintf(fp, "#%.2f %s = 1'b0;\n", 
              (1 * op_clock_period) / verilog_sim_timescale,
              top_tb_reset_port_name);
  fprintf(fp, "end\n");
  fprintf(fp, "\n");

  /* Regular inputs */   
  for (iblock = 0; iblock < num_logical_blocks; iblock++) {
    /* Make sure We find the correct logical block !*/
    switch (logical_block[iblock].type) {
    case VPACK_INPAD:
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
      /* TODO: Give the net name in the blif file !*/
      fprintf(fp, "//----- Input %s Stimuli ----\n", logical_block[iblock].name);
      fprintf(fp, "initial\n");
      fprintf(fp, "  begin //--- Input %s GENERATOR\n", logical_block[iblock].name);
      fprintf(fp, "    %s = 1'b%d;\n", 
              logical_block[iblock].name,
              cur_spice_net_info->init_val);
      fprintf(fp, "end\n");
      fprintf(fp, "always wait (~%s)\n", top_tb_reset_port_name);
      fprintf(fp, "  begin \n");
      fprintf(fp, "    #%.2f %s = ~%s;\n", 
              (op_clock_period * cur_spice_net_info->density * 2. / cur_spice_net_info->probability) / verilog_sim_timescale, 
              logical_block[iblock].name,
              logical_block[iblock].name);
      fprintf(fp, "  end \n");
      fprintf(fp, "\n");
      break;
    case VPACK_OUTPAD:
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

  return;
}

/** Top level function 1: Testbench for the top-level netlist
 * This testbench includes a top-level module of a mapped FPGA and voltage pulses
 */
void dump_verilog_top_testbench(t_sram_orgz_info* cur_sram_orgz_info,
                                char* circuit_name,
                                const char* top_netlist_name,
                                char* verilog_dir_path,
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

  /* Print preprocessing flags */
  verilog_include_defines_preproc_file(fp, verilog_dir_path);

  /* Start of testbench */
  dump_verilog_top_testbench_ports(cur_sram_orgz_info, fp, circuit_name);

  /* Call defined top-level module */
  dump_verilog_top_testbench_call_top_module(cur_sram_orgz_info, fp, circuit_name, false);

  /* Add stimuli for reset, set, clock and iopad signals */
  dump_verilog_top_testbench_stimuli(cur_sram_orgz_info, fp, verilog);

  /* Testbench ends*/
  fprintf(fp, "endmodule\n");

  /* Close the file*/
  fclose(fp);

  return;
}

/** Top level function 2: Testbench for the top-level blif netlist (before FPGA mapping)
 * This testbench includes a top-level module of a mapped FPGA and voltage pulses
 */
void dump_verilog_input_blif_testbench(char* circuit_name,
                                       char* top_netlist_name,
                                       char* verilog_dir_path,
                                       t_spice verilog) {
  FILE* fp = NULL;
  char* title = my_strcat("FPGA Verilog Testbench for input blif netlist of Design: ", circuit_name);

  /* Check if the path exists*/
  fp = fopen(top_netlist_name,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create top Verilog testbench %s!",__FILE__, __LINE__, top_netlist_name); 
    exit(1);
  } 
  
  vpr_printf(TIO_MESSAGE_INFO, "Writing Testbench for blif netlist:  %s...\n", circuit_name);
 
  /* Print the title */
  dump_verilog_file_header(fp, title);
  my_free(title);

  verilog_include_defines_preproc_file(fp, verilog_dir_path);

  /* Start of testbench */
  dump_verilog_input_blif_testbench_ports(fp, circuit_name);

  /* Call defined top-level module */
  dump_verilog_input_blif_testbench_call_top_module(fp, circuit_name);

  /* Add stimuli for reset, set, clock and iopad signals */
  dump_verilog_input_blif_testbench_stimuli(fp, verilog);

  /* Testbench ends*/
  fprintf(fp, "endmodule\n");

  /* Close the file*/
  fclose(fp);

  return;
}


