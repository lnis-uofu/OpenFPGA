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

/* Connect BLs and WLs to configuration bus in the top-level Verilog netlist*/
static 
void dump_verilog_top_netlist_memory_bank_internal_wires(FILE* fp) {
  t_spice_model* mem_model = NULL;
  int iinv, icol, irow;
  int num_bl, num_wl;
  int num_array_bl, num_array_wl;
  int num_reserved_bl, num_reserved_wl;
  int cur_bl_lsb, cur_wl_lsb;
  int cur_bl_msb, cur_wl_msb;
  int bl_decoder_size, wl_decoder_size;
  int num_blb_ports, num_wlb_ports;
  t_spice_model_port** blb_port = NULL;
  t_spice_model_port** wlb_port = NULL;
  t_spice_model* blb_inv_spice_model = NULL;
  t_spice_model* wlb_inv_spice_model = NULL;
  
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

  /* Get the total number of BLs and WLs */
  get_sram_orgz_info_num_blwl(sram_verilog_orgz_info, &num_bl, &num_wl);
  /* Get the reserved BLs and WLs */
  get_sram_orgz_info_reserved_blwl(sram_verilog_orgz_info, &num_reserved_bl, &num_reserved_wl);

  determine_verilog_blwl_decoder_size(sram_verilog_orgz_info,
                                      &num_array_bl, &num_array_wl, &bl_decoder_size, &wl_decoder_size);

  /* Get BLB and WLB ports */
  find_blb_wlb_ports_spice_model(mem_model, &num_blb_ports, &blb_port,
                                 &num_wlb_ports, &wlb_port);
  /* Get inverter spice_model */

  /* Important!!!:
   * BL/WL should always start from LSB to MSB!
   * In order to follow this convention in primitive nodes. 
   */
  /* No. of BLs and WLs in the array */
  dump_verilog_generic_port(fp, VERILOG_PORT_WIRE,
                            top_netlist_array_bl_port_name, 0, num_array_bl - 1);
  fprintf(fp, "; //--- Array Bit lines bus \n");
  dump_verilog_generic_port(fp, VERILOG_PORT_WIRE,
                            top_netlist_array_wl_port_name, 0, num_array_wl - 1);
  fprintf(fp, "; //--- Array Bit lines bus \n");
  if (1 == num_blb_ports) {
    dump_verilog_generic_port(fp, VERILOG_PORT_WIRE,
                              top_netlist_array_blb_port_name, 0, num_array_bl - 1);
    fprintf(fp, "; //--- Inverted Array Bit lines bus \n");
  }
  if (1 == num_wlb_ports) {
    dump_verilog_generic_port(fp, VERILOG_PORT_WIRE,
                              top_netlist_array_wlb_port_name, 0, num_array_wl - 1);
    fprintf(fp, "; //--- Inverted Array Word lines bus \n");
  }
  fprintf(fp, "\n");

  switch (mem_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    assert( 0 == num_reserved_bl );
    assert( 0 == num_reserved_wl );
    /* SRAMs are place in an array
     * BLs of SRAMs in the same column are connected to a common BL
     * BLs of SRAMs in the same row are connected to a common WL
     */
    /* Declare wires */
    fprintf(fp, "  wire [%d:%d] %s%s; //---- Normal Bit lines \n",
            0, num_bl - 1, mem_model->prefix, top_netlist_normal_bl_port_postfix);
    fprintf(fp, "  wire [%d:%d] %s%s; //---- Normal Word lines \n",
            0, num_wl - 1, mem_model->prefix, top_netlist_normal_wl_port_postfix);
    /* Declare inverted wires if needed */
    if (1 == num_blb_ports) {
      fprintf(fp, "  wire [%d:%d] %s%s; //---- Inverted Normal Bit lines \n",
              0, num_bl - 1, mem_model->prefix, top_netlist_normal_blb_port_postfix);
      /* get inv_spice_model */
      blb_inv_spice_model = blb_port[0]->inv_spice_model;
      /* Make an inversion of the BL */
      for (iinv = 0; iinv < num_array_bl - 1; iinv++) {
        fprintf(fp, " %s %s_blb_%d (%s[%d], %s[%d]);\n",
                blb_inv_spice_model->name, blb_inv_spice_model->prefix, 
                iinv, 
                top_netlist_array_bl_port_name, iinv,
                top_netlist_array_blb_port_name, iinv);
      }
    }
    if (1 == num_wlb_ports) {
      fprintf(fp, "  wire [%d:%d] %s%s; //---- Inverted Normal Word lines \n",
              0, num_wl - 1, mem_model->prefix, top_netlist_normal_wlb_port_postfix);
      /* get inv_spice_model */
      wlb_inv_spice_model = wlb_port[0]->inv_spice_model;
      /* Make an inversion of the WL */
      for (iinv = 0; iinv < num_array_wl - 1; iinv++) {
        fprintf(fp, " %s %s_wlb_%d (%s[%d], %s[%d]);\n",
                wlb_inv_spice_model->name, wlb_inv_spice_model->prefix, 
                iinv, 
                top_netlist_array_wl_port_name, iinv,
                top_netlist_array_wlb_port_name, iinv);
      }
    }
    /* Connections for columns */
    for (icol = 0; icol < num_array_bl; icol++) {
      cur_bl_lsb = icol * num_array_bl; 
      cur_bl_msb = (icol + 1) * num_array_bl - 1; 
      /* Check if the msb exceeds the upbound of num_bl */
      if (cur_bl_msb > num_bl - 1) {
        cur_bl_msb = num_bl - 1;
      }
      /* connect to the BLs of all the SRAMs in the column */
      fprintf(fp, "  assign %s%s[%d:%d] = %s[%d:%d];\n",
              mem_model->prefix, top_netlist_normal_bl_port_postfix, cur_bl_lsb, cur_bl_msb,
              top_netlist_array_bl_port_name, 0, cur_bl_msb - cur_bl_lsb);
      if (1 == num_blb_ports) {
        fprintf(fp, "  assign %s%s[%d:%d] = %s[%d:%d];\n",
                mem_model->prefix, top_netlist_normal_blb_port_postfix, cur_bl_lsb, cur_bl_msb,
                top_netlist_array_blb_port_name, 0, cur_bl_msb - cur_bl_lsb);
      }
      /* Finish if MSB meets the upbound */
      if (cur_bl_msb == num_bl - 1) {
        break;
      }
    }
    /* Connections for rows */
    for (irow = 0; irow < num_array_wl; irow++) {
      cur_wl_lsb = irow * num_array_wl; 
      cur_wl_msb = (irow + 1) * num_array_wl - 1; 
      /* Check if the msb exceeds the upbound of num_bl */
      if (cur_wl_msb > num_wl - 1) {
        cur_wl_msb = num_wl - 1;
      }
      /* connect to the BLs of all the SRAMs in the column */
      for (icol = cur_wl_lsb; icol < cur_wl_msb + 1; icol++) {
        fprintf(fp, "    assign %s%s[%d] = %s[%d];\n",
                mem_model->prefix, top_netlist_normal_wl_port_postfix, icol,
                top_netlist_array_wl_port_name, irow);
        if (1 == num_wlb_ports) {
          fprintf(fp, "    assign %s%s[%d] = %s[%d];\n",
                  mem_model->prefix, top_netlist_normal_wlb_port_postfix, icol,
                  top_netlist_array_wlb_port_name, irow);
        }
      }
      /* Finish if MSB meets the upbound */
      if (cur_wl_msb == num_wl - 1) {
        break;
      }
    }
    break; 
  case SPICE_MODEL_DESIGN_RRAM: 
    /* Check: there should be reserved BLs and WLs */
    assert( 0 < num_reserved_bl );
    assert( 0 < num_reserved_wl );
    /* Declare reserved and normal conf_bits ports  */
    fprintf(fp, "  wire [0:%d] %s%s; //---- Reserved Bit lines \n",
            num_reserved_bl - 1, mem_model->prefix, top_netlist_reserved_bl_port_postfix);
    fprintf(fp, "  wire [0:%d] %s%s; //---- Reserved Word lines \n",
            num_reserved_wl - 1, mem_model->prefix, top_netlist_reserved_wl_port_postfix);
    fprintf(fp, "  wire [%d:%d] %s%s; //---- Normal Bit lines \n",
            num_reserved_bl, num_array_bl - 1, mem_model->prefix, top_netlist_normal_bl_port_postfix);
    fprintf(fp, "  wire [%d:%d] %s%s; //---- Normal Word lines \n",
            num_reserved_wl, num_array_wl - 1, mem_model->prefix, top_netlist_normal_wl_port_postfix);
    /* Connect reserved conf_bits and normal conf_bits to the bus */
    fprintf(fp, "  assign %s%s[0:%d] = %s[0:%d];\n",
            mem_model->prefix, top_netlist_reserved_bl_port_postfix, num_reserved_bl - 1,
            top_netlist_array_bl_port_name, num_reserved_bl - 1);
    fprintf(fp, "  assign %s%s[0:%d] = %s[0:%d];\n",
            mem_model->prefix, top_netlist_reserved_wl_port_postfix, num_reserved_wl - 1,
            top_netlist_array_wl_port_name, num_reserved_wl - 1);
    fprintf(fp, "  assign %s%s[%d:%d] = %s[%d:%d];\n",
            mem_model->prefix, top_netlist_normal_bl_port_postfix, num_reserved_bl, num_array_bl - 1, 
            top_netlist_array_bl_port_name, num_reserved_bl, num_array_bl - 1);
    fprintf(fp, "  assign %s%s[%d:%d] = %s[%d:%d];\n",
            mem_model->prefix, top_netlist_normal_wl_port_postfix, num_reserved_wl, num_array_wl - 1,
            top_netlist_array_wl_port_name, num_reserved_wl, num_array_wl - 1);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* Delcare primary inputs/outputs for scan-chains in the top-level netlists
 */
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

/* Connect scan-chain flip-flops in the top-level netlist */
static 
void dump_verilog_top_netlist_scan_chain_internal_wires(FILE* fp) {
  t_spice_model* scff_mem_model = NULL;
  int iscff, num_scffs;

  /* A valid file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  num_scffs = get_sram_orgz_info_num_mem_bit(sram_verilog_orgz_info);
  get_sram_orgz_info_mem_model(sram_verilog_orgz_info, &scff_mem_model);
  /* Check */
  assert( SPICE_MODEL_SCFF == scff_mem_model->type );

  /* Delcare local wires */
  fprintf(fp, "  wire [0:%d] %s_scff_in;\n",
          num_scffs - 1, scff_mem_model->prefix);

  fprintf(fp, "  wire [0:%d] %s_scff_out;\n",
          num_scffs - 1, scff_mem_model->prefix);

  /* Exception for head: connect to primary inputs */ 
  fprintf(fp, "  assign %s_scff_in[%d] = %s;\n",
          scff_mem_model->prefix, 0,
          top_netlist_scan_chain_head_prefix);

  /* Connected the scan-chain flip-flops */
  /* Ensure we are in the correct range */
  /*
  fprintf(fp, "  genvar i;\n");
  fprintf(fp, "  generate\n");
  fprintf(fp, "    for (i = %d; i < %d; i = i + 1) begin\n", 
          1, num_scffs - 1);
  fprintf(fp,   "assign %s_scff_in[i] = %s_scff_out[i - 1];\n",
            scff_mem_model->prefix, 
            scff_mem_model->prefix);
  fprintf(fp, "    end\n");
  fprintf(fp, "  endgenerate;\n");
  */

  return;
}

/* Dump ports for the top-level module in Verilog netlist */
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

  
/* Dump ports for the top-level Verilog netlist */
static 
void dump_verilog_top_netlist_ports(FILE* fp,
                                    int num_clocks,
                                    char* circuit_name,
                                    t_spice verilog) {

  /* A valid file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  fprintf(fp, "//----- Top-level Verilog Module -----\n");
  fprintf(fp, "module %s_top (\n", circuit_name);
  fprintf(fp, "\n");

  dump_verilog_top_module_ports(fp, VERILOG_PORT_INPUT);

  fprintf(fp, ");\n");

  return;
}
 

static 
void dump_verilog_top_netlist_internal_wires(FILE* fp) {
  /* Configuration ports depend on the organization of SRAMs */
  switch(sram_verilog_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    /* Definition ends */
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    dump_verilog_top_netlist_scan_chain_internal_wires(fp);
    /* Definition ends */
    break;
  case SPICE_SRAM_MEMORY_BANK:
    dump_verilog_top_netlist_memory_bank_internal_wires(fp);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return; 
}

static 
void dump_verilog_defined_one_grid(FILE* fp,
                                   int ix, int iy) {
  /* Comment lines */
  fprintf(fp, "//----- BEGIN Call Grid[%d][%d] module -----\n", ix, iy);
  /* Print the Grid module */
  fprintf(fp, "grid_%d__%d_  ", ix, iy); /* Call the name of subckt */ 
  fprintf(fp, "grid_%d__%d__0_ ", ix, iy);
  fprintf(fp, "(");
  fprintf(fp, "\n");
  /* dump global ports */
  if (0 < dump_verilog_global_ports(fp, global_ports_head, FALSE)) {
    fprintf(fp, ",\n");
  }

  if (IO_TYPE == grid[ix][iy].type) {
    dump_verilog_io_grid_pins(fp, ix, iy, 1, FALSE, FALSE);
  } else {
    dump_verilog_grid_pins(fp, ix, iy, 1, FALSE, FALSE);
  }
 
  /* IO PAD */
  dump_verilog_grid_common_port(fp, iopad_verilog_model, gio_inout_prefix, 
                                iopad_verilog_model->grid_index_low[ix][iy],
                                iopad_verilog_model->grid_index_high[ix][iy] - 1,
                                VERILOG_PORT_CONKT); 

  /* Print configuration ports */
  /* Reserved configuration ports */
  if (0 < sram_verilog_orgz_info->grid_reserved_conf_bits[ix][iy]) {
    fprintf(fp, ",\n");
  }
  dump_verilog_reserved_sram_ports(fp, sram_verilog_orgz_info,
                                   0, 
                                   sram_verilog_orgz_info->grid_reserved_conf_bits[ix][iy] - 1,
                                   VERILOG_PORT_CONKT);
  /* Normal configuration ports */
  if (0 < (sram_verilog_orgz_info->grid_conf_bits_msb[ix][iy]
           - sram_verilog_orgz_info->grid_conf_bits_lsb[ix][iy])) {
    fprintf(fp, ",\n");
  }
  dump_verilog_sram_ports(fp, sram_verilog_orgz_info,
                          sram_verilog_orgz_info->grid_conf_bits_lsb[ix][iy],
                          sram_verilog_orgz_info->grid_conf_bits_msb[ix][iy] - 1,
                          VERILOG_PORT_CONKT);
  fprintf(fp, ");\n");
  /* Comment lines */
  fprintf(fp, "//----- END call Grid[%d][%d] module -----\n\n", ix, iy);

  return;
}


/***** Print (call) the defined grids *****/
static 
void dump_verilog_defined_grids(FILE* fp) {
  int ix, iy;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Normal Grids */
  for (ix = 1; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      /* Bypass EMPTY grid */
      if (EMPTY_TYPE == grid[ix][iy].type) {
        continue;
      }
      assert(IO_TYPE != grid[ix][iy].type);
      dump_verilog_defined_one_grid(fp, ix, iy);
    }
  } 

  /* IO Grids */
  /* LEFT side */
  ix = 0;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    dump_verilog_defined_one_grid(fp, ix, iy);
  }

  /* RIGHT side */
  ix = nx + 1;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    dump_verilog_defined_one_grid(fp, ix, iy);
  }

  /* BOTTOM side */
  iy = 0;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    dump_verilog_defined_one_grid(fp, ix, iy);
  } 

  /* TOP side */
  iy = ny + 1;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    dump_verilog_defined_one_grid(fp, ix, iy);
  } 

  return;
}

/* Call defined channels. 
 * Ensure the port name here is co-herent to other sub-circuits(SB,CB,grid)!!!
 */
static 
void dump_verilog_defined_one_channel(FILE* fp,
                                      t_rr_type chan_type, int x, int y,
                                      int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                      t_ivec*** LL_rr_node_indices) {
  int itrack;
  int chan_width = 0;
  t_rr_node** chan_rr_nodes = NULL;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Check */
  assert((CHANX == chan_type)||(CHANY == chan_type));
  /* check x*/
  assert((!(0 > x))&&(x < (nx + 1))); 
  /* check y*/
  assert((!(0 > y))&&(y < (ny + 1))); 

  /* Collect rr_nodes for Tracks for chanx[ix][iy] */
  chan_rr_nodes = get_chan_rr_nodes(&chan_width, chan_type, x, y,
                                    LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
 
  /* Comment lines */
  switch (chan_type) {
  case CHANX:
    fprintf(fp, "//----- BEGIN Call Channel-X [%d][%d] module -----\n", x, y);
    break;
  case CHANY:
    fprintf(fp, "//----- BEGIN call Channel-Y [%d][%d] module -----\n\n", x, y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid Channel Type!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Call the define sub-circuit */
  fprintf(fp, "%s_%d__%d_ ", 
          convert_chan_type_to_string(chan_type),
          x, y);
  fprintf(fp, "%s_%d__%d__0_ ", 
          convert_chan_type_to_string(chan_type),
          x, y);
  fprintf(fp, "(");
  fprintf(fp, "\n");
  /* dump global ports */
  if (0 < dump_verilog_global_ports(fp, global_ports_head, FALSE)) {
    fprintf(fp, ",\n");
  }

  /* LEFT/BOTTOM side port of CHANX/CHANY */
  /* We apply an opposite port naming rule than function: fprint_routing_chan_subckt 
   * In top-level netlists, we follow the same port name as switch blocks and connection blocks 
   * When a track is in INC_DIRECTION, the LEFT/BOTTOM port would be an output of a switch block
   * When a track is in DEC_DIRECTION, the LEFT/BOTTOM port would be an input of a switch block
   */
  for (itrack = 0; itrack < chan_width; itrack++) {
    switch (chan_rr_nodes[itrack]->direction) {
    case INC_DIRECTION:
      fprintf(fp, "%s_%d__%d__out_%d_, ", 
              convert_chan_type_to_string(chan_type),
              x, y, itrack);
      fprintf(fp, "\n");
      break;
    case DEC_DIRECTION:
      fprintf(fp, "%s_%d__%d__in_%d_, ", 
              convert_chan_type_to_string(chan_type),
              x, y, itrack);
      fprintf(fp, "\n");
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of %s[%d][%d]_track[%d]!\n",
                 __FILE__, __LINE__,
                 convert_chan_type_to_string(chan_type),
                 x, y, itrack);
      exit(1);
    }
  }
  /* RIGHT/TOP side port of CHANX/CHANY */
  /* We apply an opposite port naming rule than function: fprint_routing_chan_subckt 
   * In top-level netlists, we follow the same port name as switch blocks and connection blocks 
   * When a track is in INC_DIRECTION, the RIGHT/TOP port would be an input of a switch block
   * When a track is in DEC_DIRECTION, the RIGHT/TOP port would be an output of a switch block
   */
  for (itrack = 0; itrack < chan_width; itrack++) {
    switch (chan_rr_nodes[itrack]->direction) {
    case INC_DIRECTION:
      fprintf(fp, "%s_%d__%d__in_%d_, ", 
              convert_chan_type_to_string(chan_type),
              x, y, itrack);
      fprintf(fp, "\n");
      break;
    case DEC_DIRECTION:
      fprintf(fp, "%s_%d__%d__out_%d_, ", 
              convert_chan_type_to_string(chan_type),
              x, y, itrack);
      fprintf(fp, "\n");
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of %s[%d][%d]_track[%d]!\n",
                 __FILE__, __LINE__,
                 convert_chan_type_to_string(chan_type),
                 x, y, itrack);
      exit(1);
    }
  }

  /* output at middle point */
  for (itrack = 0; itrack < chan_width; itrack++) {
    fprintf(fp, "%s_%d__%d__midout_%d_ ", 
            convert_chan_type_to_string(chan_type),
            x, y, itrack);
    if (itrack < chan_width - 1) {
      fprintf(fp, ",");
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, ");\n");

  /* Comment lines */
  switch (chan_type) {
  case CHANX:
    fprintf(fp, "//----- END Call Channel-X [%d][%d] module -----\n", x, y);
    break;
  case CHANY:
    fprintf(fp, "//----- END call Channel-Y [%d][%d] module -----\n\n", x, y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid Channel Type!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Free */
  my_free(chan_rr_nodes);

  return;
}

/* Call the sub-circuits for channels : Channel X and Channel Y*/
static 
void dump_verilog_defined_channels(FILE* fp,
                                   int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                   t_ivec*** LL_rr_node_indices) {
  int ix, iy;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Channel X */
  for (iy = 0; iy < (ny + 1); iy++) {
    for (ix = 1; ix < (nx + 1); ix++) {
      dump_verilog_defined_one_channel(fp, CHANX, ix, iy,
                                       LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
    }
  }

  /* Channel Y */
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      dump_verilog_defined_one_channel(fp, CHANY, ix, iy,
                                       LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
    }
  }

  return;
}

/* Call the defined sub-circuit of connection box
 * TODO: actually most of this function is copied from
 * spice_routing.c : dump_verilog_conneciton_box_interc
 * Should be more clever to use the original function
 */
static 
void dump_verilog_defined_one_connection_box(FILE* fp,
                                             t_cb cur_cb_info) {
  int itrack, inode, side, x, y;
  int side_cnt = 0;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > cur_cb_info.x))&&(!(cur_cb_info.x > (nx + 1)))); 
  assert((!(0 > cur_cb_info.y))&&(!(cur_cb_info.y > (ny + 1)))); 

  x = cur_cb_info.x;
  y = cur_cb_info.y;
  
  /* Print the definition of subckt*/
  /* Identify the type of connection box */
  switch(cur_cb_info.type) {
  case CHANX:
    /* Comment lines */
    fprintf(fp, "//----- BEGIN Call Connection Box-X direction [%d][%d] module -----\n", x, y);
    /* Print module */
    fprintf(fp, "cbx_%d__%d_ ", x, y);
    fprintf(fp, "cbx_%d__%d__0_ ", x, y);
    break;
  case CHANY:
    /* Comment lines */
    fprintf(fp, "//----- BEGIN Call Connection Box-Y direction [%d][%d] module -----\n", x, y);
    /* Print module */
    fprintf(fp, "cby_%d__%d_ ", x, y);
    fprintf(fp, "cby_%d__%d__0_ ", x, y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }
 
  fprintf(fp, "(");
  fprintf(fp, "\n");
  /* dump global ports */
  if (0 < dump_verilog_global_ports(fp, global_ports_head, FALSE)) {
    fprintf(fp, ",\n");
  }

  /* Print the ports of channels*/
  /* connect to the mid point of a track*/
  side_cnt = 0;
  for (side = 0; side < cur_cb_info.num_sides; side++) {
    /* Bypass side with zero channel width */
    if (0 == cur_cb_info.chan_width[side]) {
      continue;
    }
    assert (0 < cur_cb_info.chan_width[side]);
    side_cnt++;
    fprintf(fp, "//----- %s side inputs: channel track middle outputs -----\n", convert_side_index_to_string(side));
    for (itrack = 0; itrack < cur_cb_info.chan_width[side]; itrack++) {
      fprintf(fp, "%s_%d__%d__midout_%d_, ",
              convert_chan_type_to_string(cur_cb_info.type),
              cur_cb_info.x, cur_cb_info.y, itrack);
      fprintf(fp, "\n");
    }
  }
  /*check side_cnt */
  assert(1 == side_cnt);
 
  side_cnt = 0;
  /* Print the ports of grids*/
  for (side = 0; side < cur_cb_info.num_sides; side++) {
    /* Bypass side with zero IPINs*/
    if (0 == cur_cb_info.num_ipin_rr_nodes[side]) {
      continue;
    }
    side_cnt++;
    assert(0 < cur_cb_info.num_ipin_rr_nodes[side]);
    assert(NULL != cur_cb_info.ipin_rr_node[side]);
    fprintf(fp, "//----- %s side outputs: CLB input pins -----\n", convert_side_index_to_string(side));
    for (inode = 0; inode < cur_cb_info.num_ipin_rr_nodes[side]; inode++) {
      /* Print each INPUT Pins of a grid */
      dump_verilog_grid_side_pin_with_given_index(fp, OPIN,
                                                 cur_cb_info.ipin_rr_node[side][inode]->ptc_num,
                                                 cur_cb_info.ipin_rr_node_grid_side[side][inode],
                                                 cur_cb_info.ipin_rr_node[side][inode]->xlow,
                                                 cur_cb_info.ipin_rr_node[side][inode]->ylow, 
                                                 FALSE); /* Do not specify direction of port */
      fprintf(fp, ", \n");
    }
  }
  /* Make sure only 2 sides of IPINs are printed */
  assert((1 == side_cnt)||(2 == side_cnt));
 
  /* Configuration ports */
  /* Reserved sram ports */
  dump_verilog_reserved_sram_ports(fp, sram_verilog_orgz_info, 
                                   0, cur_cb_info.num_reserved_conf_bits - 1,
                                   VERILOG_PORT_CONKT);
  /* Normal sram ports */
  if (0 < (cur_cb_info.num_reserved_conf_bits)) {
    fprintf(fp, ",\n");
  }
  dump_verilog_sram_ports(fp, sram_verilog_orgz_info, 
                          cur_cb_info.conf_bits_lsb, cur_cb_info.conf_bits_msb - 1,
                          VERILOG_PORT_CONKT);
 
  fprintf(fp, ");\n");

  /* Comment lines */
  switch(cur_cb_info.type) {
  case CHANX:
    fprintf(fp, "//----- END call Connection Box-X direction [%d][%d] module -----\n\n", x, y);
    break;
  case CHANY:
    fprintf(fp, "//----- END call Connection Box-Y direction [%d][%d] module -----\n\n", x, y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Check */
  assert((1 == side_cnt)||(2 == side_cnt));
 
  return;
}

/* Call the sub-circuits for connection boxes */
static 
void dump_verilog_defined_connection_boxes(FILE* fp) {
  int ix, iy;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* X - channels [1...nx][0..ny]*/
  for (iy = 0; iy < (ny + 1); iy++) {
    for (ix = 1; ix < (nx + 1); ix++) {
      if ((TRUE == is_cb_exist(CHANX, ix, iy))
         &&(0 < count_cb_info_num_ipin_rr_nodes(cbx_info[ix][iy]))) {
        dump_verilog_defined_one_connection_box(fp, cbx_info[ix][iy]);
      }
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      if ((TRUE == is_cb_exist(CHANY, ix, iy))
         &&(0 < count_cb_info_num_ipin_rr_nodes(cby_info[ix][iy]))) {
        dump_verilog_defined_one_connection_box(fp, cby_info[ix][iy]);
      }
    }
  }
 
  return; 
}

/* Call the defined switch box sub-circuit
 * TODO: This function is also copied from
 * spice_routing.c : dump_verilog_routing_switch_box_subckt
 */
static 
void dump_verilog_defined_one_switch_box(FILE* fp,
                                         t_sb cur_sb_info) {
  int ix, iy, side, itrack, x, y, inode;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cur_sb_info.x))&&(!(cur_sb_info.x > (nx + 1)))); 
  assert((!(0 > cur_sb_info.y))&&(!(cur_sb_info.y > (ny + 1)))); 

  x = cur_sb_info.x;
  y = cur_sb_info.y;
                 
  /* Comment lines */                 
  fprintf(fp, "//----- BEGIN call module Switch blocks [%d][%d] -----\n", 
          cur_sb_info.x, cur_sb_info.y);
  /* Print module*/
  fprintf(fp, "sb_%d__%d_ ", cur_sb_info.x, cur_sb_info.y);
  fprintf(fp, "sb_%d__%d__0_ ", cur_sb_info.x, cur_sb_info.y);
  fprintf(fp, "(");

  fprintf(fp, "\n");
  /* dump global ports */
  if (0 < dump_verilog_global_ports(fp, global_ports_head, FALSE)) {
    fprintf(fp, ",\n");
  }

  for (side = 0; side < cur_sb_info.num_sides; side++) {
    determine_sb_port_coordinator(cur_sb_info, side, &ix, &iy); 

    fprintf(fp, "//----- %s side channel ports-----\n", convert_side_index_to_string(side));
    for (itrack = 0; itrack < cur_sb_info.chan_width[side]; itrack++) {
      switch (cur_sb_info.chan_rr_node_direction[side][itrack]) {
      case OUT_PORT:
        fprintf(fp, "%s_%d__%d__out_%d_, ", 
                convert_chan_type_to_string(cur_sb_info.chan_rr_node[side][itrack]->type), 
                ix, iy, itrack); 
        break;
      case IN_PORT:
        fprintf(fp, "%s_%d__%d__in_%d_, ",
                convert_chan_type_to_string(cur_sb_info.chan_rr_node[side][itrack]->type), 
                ix, iy, itrack); 
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of sb[%d][%d] side[%d] track[%d]!\n",
                   __FILE__, __LINE__, x, y, side, itrack);
        exit(1);
      }
    }
    fprintf(fp, "\n");
    fprintf(fp, "//----- %s side inputs: CLB output pins -----\n", convert_side_index_to_string(side));
    /* Dump OPINs of adjacent CLBs */
    for (inode = 0; inode < cur_sb_info.num_opin_rr_nodes[side]; inode++) {
      dump_verilog_grid_side_pin_with_given_index(fp, IPIN,
                                                  cur_sb_info.opin_rr_node[side][inode]->ptc_num,
                                                  cur_sb_info.opin_rr_node_grid_side[side][inode],
                                                  cur_sb_info.opin_rr_node[side][inode]->xlow,
                                                  cur_sb_info.opin_rr_node[side][inode]->ylow,
                                                  FALSE); /* Do not specify the direction of port */ 
      fprintf(fp, ",");
    } 
    fprintf(fp, "\n");
  }

  /* Configuration ports */
  /* output of each configuration bit */
  /* Reserved sram ports */
  dump_verilog_reserved_sram_ports(fp, sram_verilog_orgz_info, 
                                   0, cur_sb_info.num_reserved_conf_bits - 1,
                                   VERILOG_PORT_CONKT);
  /* Normal sram ports */
  if (0 < (cur_sb_info.num_reserved_conf_bits)) {
    fprintf(fp, ",\n");
  }
  dump_verilog_sram_ports(fp, sram_verilog_orgz_info, 
                          cur_sb_info.conf_bits_lsb, 
                          cur_sb_info.conf_bits_msb - 1,
                          VERILOG_PORT_CONKT);

  fprintf(fp, ");\n");

  /* Comment lines */                 
  fprintf(fp, "//----- END call module Switch blocks [%d][%d] -----\n\n", x, y);

  /* Free */

  return;
}

static 
void dump_verilog_defined_switch_boxes(FILE* fp) {
  int ix, iy;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 0; iy < (ny + 1); iy++) {
      dump_verilog_defined_one_switch_box(fp, sb_info[ix][iy]);
    }
  }

  return;
}

/* Apply a CLB to CLB direct connection to a SPICE netlist 
 */
static 
void dump_verilog_one_clb2clb_direct(FILE* fp, 
                                     int from_grid_x, int from_grid_y,
                                     int to_grid_x, int to_grid_y,
                                     t_clb_to_clb_directs* cur_direct) {
  int ipin, cur_from_clb_pin_index, cur_to_clb_pin_index;
  int cur_from_clb_pin_side, cur_to_clb_pin_side;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check bandwidth match between from_clb and to_clb pins */
  if (0 != (cur_direct->from_clb_pin_end_index - cur_direct->from_clb_pin_start_index 
     - cur_direct->to_clb_pin_end_index - cur_direct->to_clb_pin_start_index)) {
    vpr_printf(TIO_MESSAGE_ERROR, "(%s, [LINE%d]) Unmatch pin bandwidth in direct connection (name=%s)!\n",
               __FILE__, __LINE__, cur_direct->name);
    exit(1);
  }

  for (ipin = 0; ipin < cur_direct->from_clb_pin_end_index - cur_direct->from_clb_pin_start_index; ipin++) {
    /* Update pin index and get the side of the pins on grids */
    cur_from_clb_pin_index = cur_direct->from_clb_pin_start_index + ipin;
    cur_to_clb_pin_index = cur_direct->to_clb_pin_start_index + ipin;
    cur_from_clb_pin_side = get_grid_pin_side(from_grid_x, from_grid_y, cur_from_clb_pin_index); 
    cur_to_clb_pin_side = get_grid_pin_side(to_grid_x, to_grid_y, cur_to_clb_pin_index); 
    /* Call the subckt that has already been defined before */
    fprintf(fp, "%s ", cur_direct->spice_model->name);
    fprintf(fp, "%s_%d_ (", cur_direct->spice_model->prefix, cur_direct->spice_model->cnt); 
    /* Dump global ports */
    if  (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_direct->spice_model, FALSE, FALSE)) {
      fprintf(fp, ",\n");
    }
    /* Input: Print the source grid pin */
    dump_verilog_toplevel_one_grid_side_pin_with_given_index(fp, OPIN,
                                                             cur_from_clb_pin_index,
                                                             cur_from_clb_pin_side,
                                                             from_grid_x, from_grid_y,
                                                             FALSE);
    fprintf(fp, ", ");
    /* Output: Print the destination grid pin */
    dump_verilog_toplevel_one_grid_side_pin_with_given_index(fp, IPIN, 
                                                             cur_to_clb_pin_index,
                                                             cur_to_clb_pin_side,
                                                             to_grid_x, from_grid_y,
                                                             FALSE);
    fprintf(fp, ");\n");
  
    /* Stats the number of spice_model used*/
    cur_direct->spice_model->cnt++; 
  }

  return;
}                 

/* Apply CLB to CLB direct connections to a Verilog netlist 
 */
static 
void dump_verilog_clb2clb_directs(FILE* fp, 
                                  int num_directs, t_clb_to_clb_directs* direct) {
  int ix, iy, idirect;   
  int to_clb_x, to_clb_y;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  fprintf(fp, "//----- BEGIN CLB to CLB Direct Connections -----\n");   

  /* Scan the grid, visit each grid and apply direct connections */
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 0; iy < (ny + 1); iy++) {
      /* Bypass EMPTY_TYPE*/
      if ((NULL == grid[ix][iy].type)
         || (EMPTY_TYPE == grid[ix][iy].type)) {
        continue;
      }
      /* Check each clb2clb directs, 
       * see if a match to the type
       */ 
      for (idirect = 0; idirect < num_directs; idirect++) {
        /* Bypass unmatch types */
        if (grid[ix][iy].type != direct[idirect].from_clb_type) {
          continue;
        }
        /* Apply x/y_offset */ 
        to_clb_x = ix + direct[idirect].x_offset;
        to_clb_y = iy + direct[idirect].y_offset;
        /* see if the destination CLB is in the bound */
        if ((FALSE == is_grid_coordinate_in_range(0, nx, to_clb_x))
           ||(FALSE == is_grid_coordinate_in_range(0, ny, to_clb_y))) {
          continue;
        }
        /* Check if capacity (z_offset) is in the range 
        if (FALSE == is_grid_coordinate_in_range(0, grid[ix][iy].type->capacity, grid[ix][iy].type->z + direct[idirect].z_offset)) {
          continue;
        }
        */
        /* Check if the to_clb_type matches */
        if (grid[to_clb_x][to_clb_y].type != direct[idirect].to_clb_type) {
          continue;
        }
        /* Bypass x/y_offset =  1 
         * since it may be addressed in Connection blocks 
        if (1 == (x_offset + y_offset)) {
          continue;
        }
         */
        /* Now we can print a direct connection with the spice models */
        dump_verilog_one_clb2clb_direct(fp, 
                                        ix, iy, 
                                        to_clb_x, to_clb_y, 
                                        &direct[idirect]);
      }
    }
  }

  fprintf(fp, "//----- END CLB to CLB Direct Connections -----\n");   

  return;

}

/** Dump Standalone SRAMs 
 */
static 
void dump_verilog_configuration_circuits_standalone_srams(FILE* fp) {
  int i;
  /* Check */
  assert(SPICE_SRAM_STANDALONE == sram_verilog_orgz_type);
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Dump each SRAM */
  fprintf(fp, "//------ Standalone SRAMs -----\n");
  for (i = 0; i < sram_verilog_model->cnt; i++) {
    fprintf(fp, "%s %s_%d (\n", 
            sram_verilog_model->name, sram_verilog_model->prefix, i);
    /* Input and 2 outputs */
    fprintf(fp, "%s_in[%d] %s_out[%d] %s_outb[%d] ",
            sram_verilog_model->prefix, i,
            sram_verilog_model->prefix, i,
            sram_verilog_model->prefix, i);
    fprintf(fp, ");\n");
  }
  fprintf(fp, "//------ END Standalone SRAMs -----\n");

  return;
}

/** Dump scan-chains 
 */
static 
void dump_verilog_configuration_circuits_scan_chains(FILE* fp) {
  int i;
  /* Check */
  assert(SPICE_SRAM_SCAN_CHAIN == sram_verilog_orgz_type);
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Dump each Scan-chain FF */
  fprintf(fp, "//------ Scan-chain FFs -----\n");
  for (i = 0; i < sram_verilog_model->cnt; i++) {
    /* Connect the head of current scff to the tail of previous scff*/
    if (0 < i) {
      fprintf(fp, "        ");
      fprintf(fp, "assign ");
      dump_verilog_sram_one_port(fp, sram_verilog_orgz_info, i, i, 0, VERILOG_PORT_CONKT);
      fprintf(fp, " = ");
      dump_verilog_sram_one_port(fp, sram_verilog_orgz_info, i - 1, i - 1, 1, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
    }
  }
  fprintf(fp, "//------ END Scan-chain FFs -----\n");

  return;
}

/* Dump a memory bank to configure all the Bit lines and Word lines */
static 
void dump_verilog_configuration_circuits_memory_bank(FILE* fp, 
                                                     t_sram_orgz_info* cur_sram_orgz_info) {
  int bl_decoder_size, wl_decoder_size;
  int num_array_bl, num_array_wl;
  t_spice_model* mem_model = NULL;

  /* Check */
  assert(SPICE_SRAM_MEMORY_BANK == cur_sram_orgz_info->type);
  assert(NULL != cur_sram_orgz_info->mem_bank_info);

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  determine_verilog_blwl_decoder_size(sram_verilog_orgz_info,
                                      &num_array_bl, &num_array_wl, &bl_decoder_size, &wl_decoder_size);
  /* Get memory model */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
  assert(NULL != mem_model);

  /* Comment lines */
  fprintf(fp, "//----- BEGIN call decoders for memory bank controller -----\n");

  /* Dump Decoders for Bit lines and Word lines */
  /* Two huge decoders
   * TODO: divide to a number of small decoders ?
   */
  /* Bit lines decoder */
  fprintf(fp, "bl_decoder%dto%d mem_bank_bl_decoder (",
          bl_decoder_size, num_array_bl);
  /* Prefix of BL & WL is fixed, in order to simplify grouping nets */
  fprintf(fp, "%s, %s[%d:0], ",
          top_netlist_bl_enable_port_name,
          top_netlist_addr_bl_port_name, bl_decoder_size - 1); 
  /* Port map depends on the memory technology */
  switch (mem_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    /* Data input port of BL decoder, only required by SRAM array */
    fprintf(fp, "%s, ",
            top_netlist_bl_data_in_port_name);
    break;
  case SPICE_MODEL_DESIGN_RRAM: 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  fprintf(fp, "%s[0:%d]", 
          top_netlist_array_bl_port_name, num_array_bl - 1);
  fprintf(fp, ");\n");

  /* Word lines decoder is the same for both technology */
  fprintf(fp, "wl_decoder%dto%d mem_bank_wl_decoder (",
          wl_decoder_size, num_array_wl);
  fprintf(fp, "%s, %s[%d:0], ",
          top_netlist_wl_enable_port_name,
          top_netlist_addr_wl_port_name, wl_decoder_size - 1); 
  fprintf(fp, "%s[0:%d]", 
          top_netlist_array_wl_port_name, num_array_wl - 1);
  fprintf(fp, ");\n");

  /* Comment lines */
  fprintf(fp, "//----- END call decoders for memory bank controller -----\n\n");

  return; 
}

/* Dump the configuration circuits in verilog according to user-specification
 * Supported styles of configuration circuits:
 * 1. Scan-chains
 * 2. Memory banks
 * 3. Standalone SRAMs
 */
static 
void dump_verilog_configuration_circuits(FILE* fp) {
  switch(sram_verilog_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    dump_verilog_configuration_circuits_standalone_srams(fp);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    dump_verilog_configuration_circuits_scan_chains(fp);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    dump_verilog_configuration_circuits_memory_bank(fp, sram_verilog_orgz_info);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  return;
}

/* Dump all the global ports that are stored in the linked list */
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

/* Print stimuli for global ports in top-level testbench */
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
void dump_verilog_top_testbench_ports(FILE* fp,
                                      char* circuit_name) {
  int num_array_bl, num_array_wl;
  int bl_decoder_size, wl_decoder_size;
  int iblock, iopad_idx;
  t_spice_model* mem_model = NULL;
  char* port_name = NULL;
 
  get_sram_orgz_info_mem_model(sram_verilog_orgz_info, &mem_model);

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
      fprintf(fp, "wire in_%s_%s_%d_;\n",
              logical_block[iblock].name, gio_inout_prefix, iopad_idx);
      fprintf(fp, "assign in_%s_%s_%d_ = %s%s[%d];\n",
              logical_block[iblock].name, gio_inout_prefix, iopad_idx,
              gio_inout_prefix, iopad_verilog_model->prefix, iopad_idx);
    }
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


/* Dump configuration bits of a SRAM memory bank
 * The efficient sharing BLs/WLs force configuration to be done WL by WL
 * (We always assume that Word Lines are the write/read enable signal for SRAMs)
 * All the SRAMs controlled by the same WL will be configuration during one programming cycle
 * by assign different BL values for each of them. 
 */
static 
void dump_verilog_top_testbench_sram_memory_bank_conf_bits_serial(FILE* fp, 
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
  determine_verilog_blwl_decoder_size(sram_verilog_orgz_info,
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
void dump_verilog_top_testbench_rram_memory_bank_conf_bits_serial(FILE* fp, 
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
  determine_verilog_blwl_decoder_size(sram_verilog_orgz_info,
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
void dump_verilog_top_testbench_memory_bank_conf_bits_serial(FILE* fp, 
                                                             t_llist* cur_conf_bit) {
  t_spice_model* mem_model = NULL;

  /* Depending on the memory technology*/
  get_sram_orgz_info_mem_model(sram_verilog_orgz_info, &mem_model);
  assert(NULL != mem_model);

  /* Check */
  assert(SPICE_SRAM_MEMORY_BANK == sram_verilog_orgz_info->type);

  /* fork on the memory technology */
  switch (mem_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    dump_verilog_top_testbench_sram_memory_bank_conf_bits_serial(fp, cur_conf_bit); 
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    dump_verilog_top_testbench_rram_memory_bank_conf_bits_serial(fp, cur_conf_bit); 
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
void dump_verilog_top_testbench_one_conf_bit_serial(FILE* fp, 
                                                    t_llist* cur_conf_bit) {
    						
  /* We already touch the tail, start dump */
  switch (sram_verilog_orgz_type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    dump_verilog_top_testbench_scan_chain_conf_bits_serial(fp, cur_conf_bit); 
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* Should work differently depending on memory technology */
    dump_verilog_top_testbench_memory_bank_conf_bits_serial(fp, cur_conf_bit); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }
 
  return;
}

static 
void dump_verilog_top_testbench_conf_bits_serial(FILE* fp, 
                                                 t_llist* head) {
  int num_bl, num_wl;
  int bl_decoder_size, wl_decoder_size;
  t_llist* new_head = head;

  switch (sram_verilog_orgz_type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    /* For scan chain, the last bit should go first!*/
    /* We do not need to reverse the linked list HERE !!! 
     * Because, it is already arranged in the seqence of MSB to LSB
     * Note that the first element in the linked list is the last bit now!
     */
    /* For each element in linked list, generate a voltage stimuli */
    dump_verilog_top_testbench_one_conf_bit_serial(fp, head);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* Configuration bits are loaded differently depending on memory technology */
    determine_verilog_blwl_decoder_size(sram_verilog_orgz_info,
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
    dump_verilog_top_testbench_one_conf_bit_serial(fp, new_head);
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
void dump_verilog_top_testbench_stimuli_serial_version_tasks_memory_bank(FILE* fp) {
  int num_array_bl, num_array_wl;
  int bl_decoder_size, wl_decoder_size;
  t_spice_model* mem_model = NULL;

  /* Check */
  assert (sram_verilog_orgz_info->type == SPICE_SRAM_MEMORY_BANK);

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Find the number of array BLs/WLs and decoder sizes */
  determine_verilog_blwl_decoder_size(sram_verilog_orgz_info,
                                      &num_array_bl, &num_array_wl, &bl_decoder_size, &wl_decoder_size);

  /* Depending on the memory technology*/
  get_sram_orgz_info_mem_model(sram_verilog_orgz_info, &mem_model);
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
void dump_verilog_top_testbench_stimuli_serial_version_tasks_scan_chain(FILE* fp) {

  /* Check */
  assert ( SPICE_SRAM_SCAN_CHAIN == sram_verilog_orgz_info->type);

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
void dump_verilog_top_testbench_stimuli_serial_version_tasks(FILE* fp) {

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  } 

  switch (sram_verilog_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    dump_verilog_top_testbench_stimuli_serial_version_tasks_scan_chain(fp);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    dump_verilog_top_testbench_stimuli_serial_version_tasks_memory_bank(fp);
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
void dump_verilog_top_testbench_stimuli_serial_version(FILE* fp,
                                                       int num_clock,
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
  dump_verilog_top_testbench_stimuli_serial_version_tasks(fp);

  /* Estimate the number of configuration clock cycles 
   * by traversing the linked-list and count the number of SRAM=1 or BL=1&WL=1 in it.
   * We plus 1 additional config clock cycle here because we need to reset everything during the first clock cycle
   */
  num_config_clock_cycles = 1 + dump_verilog_top_testbench_find_num_config_clock_cycles(sram_verilog_orgz_info->conf_bit_head);
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
  dump_verilog_top_testbench_conf_bits_serial(fp, sram_verilog_orgz_info->conf_bit_head); 
  
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
        fprintf(fp, "    %s%s%s[%d] = ~%s%s%s[%d];\n    #%.2f\n", 
                gio_inout_prefix, iopad_verilog_model->prefix, top_tb_inout_reg_postfix, iopad_idx,
                gio_inout_prefix, iopad_verilog_model->prefix, top_tb_inout_reg_postfix, iopad_idx,
                (((float)(int)((100 * op_clock_period) / verilog_sim_timescale) / 100) * ((int)(cur_spice_net_info->probability / cur_spice_net_info->density)+ iblock)));
        fprintf(fp, "    %s%s%s[%d] = ~%s%s%s[%d];\n    #%.2f;\n", 
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
void dump_verilog_top_testbench_stimuli(FILE* fp,
                                        int num_clock,
                                        t_syn_verilog_opts syn_verilog_opts,
                                        t_spice verilog) {

  /* Only serial version is avaiable now */
  dump_verilog_top_testbench_stimuli_serial_version(fp, num_clock, verilog);
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
                                               int num_clock,
                                               t_syn_verilog_opts syn_verilog_opts,
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

/***** Print Top-level SPICE netlist *****/
void dump_verilog_top_netlist(char* circuit_name,
                              char* top_netlist_name,
                              char* include_dir_path,
                              char* subckt_dir_path,
                              int LL_num_rr_nodes,
                              t_rr_node* LL_rr_node,
                              t_ivec*** LL_rr_node_indices,
                              int num_clock,
                              t_spice verilog) {
  FILE* fp = NULL;
  char* formatted_subckt_dir_path = format_dir_path(subckt_dir_path);
  char* temp_include_file_path = NULL;
  char* title = my_strcat("FPGA Verilog Netlist for Design: ", circuit_name);

  /* Check if the path exists*/
  fp = fopen(top_netlist_name,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create top Verilog netlist %s!",__FILE__, __LINE__, top_netlist_name); 
    exit(1);
  } 
  
  vpr_printf(TIO_MESSAGE_INFO, "Writing FPGA Top-level Verilog Netlist for %s...\n", circuit_name);
 
  /* Print the title */
  dump_verilog_file_header(fp, title);
  my_free(title);

  /* Include user-defined sub-circuit netlist */
  fprintf(fp, "//----- Include User-defined netlists -----\n");
  init_include_user_defined_verilog_netlists(verilog);
  dump_include_user_defined_verilog_netlists(fp, verilog);
  
  /* Special subckts for Top-level SPICE netlist */
  fprintf(fp, "//----- Include subckt netlists: Multiplexers -----\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, muxes_verilog_file_name);
  fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "//----- Include subckt netlists: Wires -----\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, wires_verilog_file_name);
  fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "//----- Include subckt netlists: Look-Up Tables (LUTs) -----\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, luts_verilog_file_name);
  fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "//------ Include subckt netlists: Logic Blocks -----\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, logic_block_verilog_file_name);
  fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "//----- Include subckt netlists: Routing structures (Switch Boxes, Channels, Connection Boxes) -----\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, routing_verilog_file_name);
  fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
  my_free(temp_include_file_path);
 
  /* Include decoders if required */ 
  switch(sram_verilog_orgz_type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* Include verilog decoder */
    fprintf(fp, "//----- Include subckt netlists: Decoders (controller for memeory bank) -----\n");
    temp_include_file_path = my_strcat(formatted_subckt_dir_path, decoders_verilog_file_name);
    fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
    my_free(temp_include_file_path);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }
 
  /* Print all global wires*/
  dump_verilog_top_netlist_ports(fp, num_clock, circuit_name, verilog);

  dump_verilog_top_netlist_internal_wires(fp);

  /* Quote defined Logic blocks subckts (Grids) */
  dump_verilog_defined_grids(fp);

  /* Quote Routing structures: Channels */
  dump_verilog_defined_channels(fp, LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);

  /* Quote Routing structures: Conneciton Boxes */
  dump_verilog_defined_connection_boxes(fp); 
  
  /* Quote Routing structures: Switch Boxes */
  dump_verilog_defined_switch_boxes(fp); 

  /* Apply CLB to CLB direct connections */
  dump_verilog_clb2clb_directs(fp, num_clb2clb_directs, clb2clb_direct);

  /* Dump configuration circuits */
  dump_verilog_configuration_circuits(fp);

  /* verilog ends*/
  fprintf(fp, "endmodule\n");

  /* Close the file*/
  fclose(fp);

  return;
}

/***** Print Top-level SPICE netlist *****/
void dump_verilog_top_netlist_tile_orgz(char* circuit_name,
                                        char* top_netlist_name,
                                        char* include_dir_path,
                                        char* subckt_dir_path,
                                        int LL_num_rr_nodes,
                                        t_rr_node* LL_rr_node,
                                        t_ivec*** LL_rr_node_indices,
                                        int num_clock,
                                        t_spice verilog) {
  FILE* fp = NULL;
  char* formatted_subckt_dir_path = format_dir_path(subckt_dir_path);
  char* temp_include_file_path = NULL;
  char* title = my_strcat("FPGA Verilog Netlist for Design: ", circuit_name);

  /* Check if the path exists*/
  fp = fopen(top_netlist_name,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create top Verilog netlist %s!",__FILE__, __LINE__, top_netlist_name); 
    exit(1);
  } 
  
  vpr_printf(TIO_MESSAGE_INFO, "Writing FPGA Top-level Verilog Netlist for %s...\n", circuit_name);
 
  /* Print the title */
  dump_verilog_file_header(fp, title);
  my_free(title);

  /* Include user-defined sub-circuit netlist */
  fprintf(fp, "//----- Include User-defined netlists -----\n");
  init_include_user_defined_verilog_netlists(verilog);
  dump_include_user_defined_verilog_netlists(fp, verilog);
  
  /* Special subckts for Top-level SPICE netlist */
  fprintf(fp, "//----- Include subckt netlists: Multiplexers -----\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, muxes_verilog_file_name);
  fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "//----- Include subckt netlists: Wires -----\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, wires_verilog_file_name);
  fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "//----- Include subckt netlists: Look-Up Tables (LUTs) -----\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, luts_verilog_file_name);
  fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "//------ Include subckt netlists: Logic Blocks -----\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, logic_block_verilog_file_name);
  fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "//----- Include subckt netlists: Routing structures (Switch Boxes, Channels, Connection Boxes) -----\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, routing_verilog_file_name);
  fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
  my_free(temp_include_file_path);
 
  /* Include decoders if required */ 
  switch(sram_verilog_orgz_type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* Include verilog decoder */
    fprintf(fp, "//----- Include subckt netlists: Decoders (controller for memeory bank) -----\n");
    temp_include_file_path = my_strcat(formatted_subckt_dir_path, decoders_verilog_file_name);
    fprintf(fp, "// `include \"%s\"\n", temp_include_file_path);
    my_free(temp_include_file_path);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }
 
  /* Print all global wires*/
  dump_verilog_top_netlist_ports(fp, num_clock, circuit_name, verilog);

  dump_verilog_top_netlist_internal_wires(fp);

  /* Quote defined Logic blocks subckts (Grids) */
  dump_verilog_defined_grids(fp);

  /* Quote Routing structures: Channels */
  dump_verilog_defined_channels(fp, LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);

  /* Quote Routing structures: Conneciton Boxes */
  dump_verilog_defined_connection_boxes(fp); 
  
  /* Quote Routing structures: Switch Boxes */
  dump_verilog_defined_switch_boxes(fp); 

  /* Apply CLB to CLB direct connections */
  dump_verilog_clb2clb_directs(fp, num_clb2clb_directs, clb2clb_direct);

  /* Dump configuration circuits */
  dump_verilog_configuration_circuits(fp);

  /* verilog ends*/
  fprintf(fp, "endmodule\n");

  /* Close the file*/
  fclose(fp);

  return;
}

/** Top level function 2: Testbench for the top-level netlist
 * This testbench includes a top-level module of a mapped FPGA and voltage pulses
 */
void dump_verilog_top_testbench(char* circuit_name,
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
  dump_verilog_top_testbench_ports(fp, circuit_name);

  /* Call defined top-level module */
  dump_verilog_top_testbench_call_top_module(fp, circuit_name);

  /* Add stimuli for reset, set, clock and iopad signals */
  dump_verilog_top_testbench_stimuli(fp, num_clock, syn_verilog_opts, verilog);

  /* Testbench ends*/
  fprintf(fp, "endmodule\n");

  /* Close the file*/
  fclose(fp);

  return;
}

/** Top level function 3: Testbench for the top-level blif netlist (before FPGA mapping)
 * This testbench includes a top-level module of a mapped FPGA and voltage pulses
 */
void dump_verilog_input_blif_testbench(char* circuit_name,
                                       char* top_netlist_name,
                                       int num_clock,
                                       t_syn_verilog_opts syn_verilog_opts,
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

  /* Start of testbench */
  dump_verilog_input_blif_testbench_ports(fp, circuit_name);

  /* Call defined top-level module */
  dump_verilog_input_blif_testbench_call_top_module(fp, circuit_name);

  /* Add stimuli for reset, set, clock and iopad signals */
  dump_verilog_input_blif_testbench_stimuli(fp, num_clock, syn_verilog_opts, verilog);

  /* Testbench ends*/
  fprintf(fp, "endmodule\n");

  /* Close the file*/
  fclose(fp);

  return;

}
