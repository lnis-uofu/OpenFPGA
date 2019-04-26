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
#include "fpga_x2p_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "fpga_x2p_globals.h"

/* Include verilog utils */
#include "verilog_global.h"
#include "verilog_utils.h"

/***** Subroutines *****/

void dump_verilog_decoder_memory_bank_ports(t_sram_orgz_info* cur_sram_orgz_info, 
                                            FILE* fp, 
                                            enum e_dump_verilog_port_type dump_port_type) {
  t_spice_model* mem_model = NULL;
  int num_array_bl, num_array_wl;
  int bl_decoder_size, wl_decoder_size;
  char split_sign;

  split_sign = determine_verilog_generic_port_split_sign(dump_port_type);

  /* Only accept two types of dump_port_type here! */
  assert((VERILOG_PORT_INPUT == dump_port_type)||(VERILOG_PORT_CONKT == dump_port_type));

  /* Check */
  assert (cur_sram_orgz_info->type == SPICE_SRAM_MEMORY_BANK);

  /* A valid file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Depending on the memory technology*/
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
  assert(NULL != mem_model);

  determine_blwl_decoder_size(cur_sram_orgz_info,
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
void dump_verilog_decoder(FILE* fp,
                          t_sram_orgz_info* cur_sram_orgz_info) {
  int num_array_bl, num_array_wl;
  int bl_decoder_size, wl_decoder_size;
  t_spice_model* mem_model = NULL;
  boolean bl_inverted = FALSE;
  boolean wl_inverted = FALSE;

  /* Check */
  assert(SPICE_SRAM_MEMORY_BANK == cur_sram_orgz_info->type);

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!\n", 
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Get number of BLs,WLs and decoder sizes */
  determine_blwl_decoder_size(cur_sram_orgz_info, 
                              &num_array_bl, &num_array_wl, 
                              &bl_decoder_size, &wl_decoder_size);

  /* Different design technology requires different BL decoder logic */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model); 
  /* Find if we need an inversion of the BL */
  check_mem_model_blwl_inverted(mem_model, SPICE_MODEL_PORT_BL, &bl_inverted); 
  check_mem_model_blwl_inverted(mem_model, SPICE_MODEL_PORT_WL, &wl_inverted); 

  switch (mem_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS: /* CMOS SRAM*/
    /* SRAM technology requires its BL decoder has an additional input called data_in 
     * only the selected BL will be set to the value of data_in, other BLs will be in high-resistance state
     */
     /* Start the BL decoder module definition */
    fprintf(fp, "//----- BL Decoder convert %d bits to binary %d bits -----\n",
            bl_decoder_size, num_array_bl);
    fprintf(fp, "module bl_decoder%dto%d (\n",
                 bl_decoder_size, num_array_bl);
    fprintf(fp, "input wire enable,\n"); 
    fprintf(fp, "input wire [%d:0] addr_in,\n",
               bl_decoder_size - 1);
    fprintf(fp, "input wire data_in,\n"); 
    fprintf(fp, "output reg [0:%d] addr_out\n",
               num_array_bl - 1);
    fprintf(fp, ");\n");
  
    /* Wee need to know the default value of bl port and wl port */

    /* Internal logics */

    fprintf(fp, "always@(addr_out,addr_in,enable, data_in)\n");
    fprintf(fp, "begin\n");
    fprintf(fp, "\taddr_out = %d'bz;\n", num_array_bl);
    fprintf(fp, "\tif (1'b1 == enable) begin\n");
    fprintf(fp, "\t\taddr_out[addr_in] = data_in;\n");
    fprintf(fp, "\tend\n");
    fprintf(fp, "end\n");

    fprintf(fp, "endmodule\n");
    break;
  case SPICE_MODEL_DESIGN_RRAM: /* RRAM */
    /* For RRAM technology, BL decoder should be same as the WL decoder */
    /* Start the BL decoder module definition */
    fprintf(fp, "//----- BL Decoder convert %d bits to binary %d bits -----\n",
            bl_decoder_size, num_array_bl);
    fprintf(fp, "module bl_decoder%dto%d (\n",
                 bl_decoder_size, num_array_bl);
    fprintf(fp, "input wire enable,\n"); 
    fprintf(fp, "input wire [%d:0] addr_in,\n",
               bl_decoder_size-1);
    fprintf(fp, "output reg [0:%d] addr_out\n",
               num_array_bl-1);
    fprintf(fp, ");\n");
  
    /* Internal logics */
    fprintf(fp, "always@(addr_out,addr_in,enable)\n");
    fprintf(fp, "begin\n");
    if (TRUE == bl_inverted) {
      fprintf(fp, "\taddr_out = %d'b1;\n", num_array_bl);
    } else {
      assert (FALSE == bl_inverted);
      fprintf(fp, "\taddr_out = %d'b0;\n", num_array_bl);
    }
    fprintf(fp, "\tif (1'b1 == enable) begin\n");
    if (TRUE == bl_inverted) {
      fprintf(fp, "\t\taddr_out[addr_in] = 1'b0;\n");
    } else {
      assert (FALSE == bl_inverted);
      fprintf(fp, "\t\taddr_out[addr_in] = 1'b1;\n");
    }
    fprintf(fp, "\tend\n");
    fprintf(fp, "end\n");

    fprintf(fp, "endmodule\n");
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology [CMOS|RRAM] for memory technology!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* WL decoder logic is the same whatever SRAM or RRAM technology is considered */
  /* Start the WL module definition */
  fprintf(fp, "//----- WL Decoder convert %d bits to binary %d bits -----\n",
          wl_decoder_size, num_array_wl);
  fprintf(fp, "module wl_decoder%dto%d (\n",
               wl_decoder_size, num_array_wl);
  fprintf(fp, "input wire enable,\n"); 
  fprintf(fp, "input wire [%d:0] addr_in,\n",
               wl_decoder_size-1);
  fprintf(fp, "output reg [0:%d] addr_out\n",
               num_array_bl-1);
  fprintf(fp, ");\n");
  
  /* Internal logics */
  fprintf(fp, "always@(addr_out,addr_in,enable)\n");
  fprintf(fp, "begin\n");
  if (TRUE == wl_inverted) {
    fprintf(fp, "\taddr_out = %d'b1;\n", num_array_wl);
  } else {
    assert (FALSE == wl_inverted);
    fprintf(fp, "\taddr_out = %d'b0;\n", num_array_wl);
  }
  fprintf(fp, "\tif (1'b1 == enable) begin\n");
  if (TRUE == wl_inverted) {
    fprintf(fp, "\t\taddr_out[addr_in] = 1'b0;\n");
  } else {
    assert (FALSE == wl_inverted);
    fprintf(fp, "\t\taddr_out[addr_in] = 1'b1;\n");
  }
  fprintf(fp, "\tend\n");
  fprintf(fp, "end\n");

  fprintf(fp, "endmodule\n");
  
  return;
}

/* For standalone-SRAM  configuration organization:
 * Dump the module of configuration module which connect configuration ports to SRAMs/SCFFs 
 */ 
static 
void dump_verilog_standalone_sram_config_module(FILE* fp,
                                                t_sram_orgz_info* cur_sram_orgz_info) {
  int i, num_mem_bits;

  /* Check */
  assert(SPICE_SRAM_STANDALONE == cur_sram_orgz_info->type);
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Get the total memory bits */
  num_mem_bits = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info);

  /* Dump each SRAM */
  fprintf(fp, "//------ Configuration Peripheral for Standalone SRAMs -----\n");
  fprintf(fp, "module %s (\n", 
              verilog_config_peripheral_prefix);
  /* Dump port map*/
  fprintf(fp, "input %s_in[%d:%d],\n",
              sram_verilog_model->prefix, 
              0, num_mem_bits - 1);
  fprintf(fp, "output %s_out[%d:%d],\n",
              sram_verilog_model->prefix, 
              0, num_mem_bits - 1);
  fprintf(fp, "output %s_outb[%d:%d]);\n",
              sram_verilog_model->prefix, 
              0, num_mem_bits - 1);

  for (i = 0; i < num_mem_bits; i++) {
    /* Input and 2 outputs */
    fprintf(fp, "assign %s_out[%d] = %s_in[%d];\n",
            sram_verilog_model->prefix, i,
            sram_verilog_model->prefix, i);
    fprintf(fp, ");\n");
  }
  fprintf(fp, "endmodule\n");
  fprintf(fp, "//------ END Standalone SRAMs -----\n");

  return;
}


/* For scan-chain configuration organization:
 * Dump the module of configuration module which connect configuration ports to SRAMs/SCFFs 
 */ 
static 
void dump_verilog_scan_chain_config_module(FILE* fp,
                                    t_sram_orgz_info* cur_sram_orgz_info) {
  int i, num_mem_bits;

  /* Check */
  assert(SPICE_SRAM_SCAN_CHAIN == cur_sram_orgz_info->type);
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Get the total memory bits */
  num_mem_bits = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info);

  /* Dump each Scan-chain FF */
  fprintf(fp, "//------ Configuration Peripheral for Scan-chain FFs -----\n");
  fprintf(fp, "module %s (\n", 
              verilog_config_peripheral_prefix);
  /* Port map definition */
  /* Scan-chain input*/
  dump_verilog_generic_port(fp, VERILOG_PORT_INPUT,
                            top_netlist_scan_chain_head_prefix, 0, 0);
  fprintf(fp, ",\n");
  /* Scan-chain regular inputs */
  dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 0, num_mem_bits - 1, -1, VERILOG_PORT_OUTPUT);
  fprintf(fp, ",\n");
  fprintf(fp, "input ");
  dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 0, num_mem_bits - 1, 0, VERILOG_PORT_WIRE);
  fprintf(fp, ");\n");

  /* Connect scan-chain input to the first scan-chain input */
  fprintf(fp, "        ");
  fprintf(fp, "assign ");
  dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 0, 0, -1, VERILOG_PORT_CONKT);
  fprintf(fp, " = ");
  dump_verilog_generic_port(fp, VERILOG_PORT_CONKT,
                            top_netlist_scan_chain_head_prefix, 0, 0);
  fprintf(fp, ";\n");

  /* Verilog Module body */
  /* Connect the head of current scff to the tail of previous scff*/
  fprintf(fp, "        ");
  fprintf(fp, "assign ");
  dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 1, num_mem_bits - 1, -1, VERILOG_PORT_CONKT);
  fprintf(fp, " = ");
  dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 0, num_mem_bits - 2, 0, VERILOG_PORT_CONKT);
  fprintf(fp, ";\n");
  fprintf(fp, "endmodule\n");
  fprintf(fp, "//------ END Scan-chain FFs -----\n");

  return;
}

/* For Memory-bank configuration organization:
 * Dump the module of configuration module which connect configuration ports to SRAMs/SCFFs 
 */ 
static 
void dump_verilog_membank_config_module(FILE* fp,
                                        t_sram_orgz_info* cur_sram_orgz_info) {
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
  assert(SPICE_SRAM_MEMORY_BANK == cur_sram_orgz_info->type);
  assert(NULL != cur_sram_orgz_info->mem_bank_info);

  /* A valid file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Depending on the memory technology*/
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
  assert(NULL != mem_model);

  /* Get the total number of BLs and WLs */
  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &num_bl, &num_wl);
  /* Get the reserved BLs and WLs */
  get_sram_orgz_info_reserved_blwl(cur_sram_orgz_info, &num_reserved_bl, &num_reserved_wl);

  determine_blwl_decoder_size(cur_sram_orgz_info,
                              &num_array_bl, &num_array_wl, &bl_decoder_size, &wl_decoder_size);

  /* Get BLB and WLB ports */
  find_blb_wlb_ports_spice_model(mem_model, &num_blb_ports, &blb_port,
                                 &num_wlb_ports, &wlb_port);
  /* Get inverter spice_model */

  /* Dump each SRAM */
  fprintf(fp, "//------ Configuration Peripheral for Memory-bank -----\n");
  fprintf(fp, "module %s (\n", 
              verilog_config_peripheral_prefix);
  /* Port map */
  /* Ports for memory decoders */
  dump_verilog_decoder_memory_bank_ports(cur_sram_orgz_info, fp, VERILOG_PORT_INPUT);
  fprintf(fp, ",\n");
  /* Ports for all the SRAM cells  */
  switch (mem_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    assert( 0 == num_reserved_bl );
    assert( 0 == num_reserved_wl );
    /* Declare normal BL / WL inputs */
    fprintf(fp, "  input wire [%d:%d] %s%s, //---- Normal Bit lines \n",
            0, num_bl - 1, mem_model->prefix, top_netlist_normal_bl_port_postfix);
    fprintf(fp, "  input wire [%d:%d] %s%s //---- Normal Word lines\n",
            0, num_wl - 1, mem_model->prefix, top_netlist_normal_wl_port_postfix);
    /* Declare inverted wires if needed */
    if (1 == num_blb_ports) {
      fprintf(fp, ",  \n");
    } else {
      fprintf(fp, ");\n");
    }
    if (1 == num_blb_ports) {
      fprintf(fp, " input wire [%d:%d] %s%s //---- Inverted Normal Bit lines \n",
              0, num_bl - 1, mem_model->prefix, top_netlist_normal_blb_port_postfix);
    }
    if (1 == num_wlb_ports) {
      fprintf(fp, ",  \n");
    } else {
      fprintf(fp, ");\n");
    }
    if (1 == num_wlb_ports) {
      fprintf(fp, " input wire [%d:%d] %s%s //---- Inverted Normal Word lines \n",
              0, num_wl - 1, mem_model->prefix, top_netlist_normal_wlb_port_postfix);
      fprintf(fp, ");\n");
    }
    break; 
  case SPICE_MODEL_DESIGN_RRAM: 
    /* Check: there should be reserved BLs and WLs */
    assert( 0 < num_reserved_bl );
    assert( 0 < num_reserved_wl );
    /* Declare reserved and normal conf_bits ports  */
    fprintf(fp, "  input wire [0:%d] %s%s, //---- Reserved Bit lines \n",
            num_reserved_bl - 1, mem_model->prefix, top_netlist_reserved_bl_port_postfix);
    fprintf(fp, "  input wire [0:%d] %s%s, //---- Reserved Word lines \n",
            num_reserved_wl - 1, mem_model->prefix, top_netlist_reserved_wl_port_postfix);
    fprintf(fp, "  input wire [%d:%d] %s%s, //---- Normal Bit lines \n",
            num_reserved_bl, num_array_bl - 1, mem_model->prefix, top_netlist_normal_bl_port_postfix);
    fprintf(fp, "  input wire [%d:%d] %s%s); //---- Normal Word lines \n",
            num_reserved_wl, num_array_wl - 1, mem_model->prefix, top_netlist_normal_wl_port_postfix);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Important!!!:
   * BL/WL should always start from LSB to MSB!
   * In order to follow this convention in primitive nodes. 
   */
  fprintf(fp, "\n");
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
    /* Declare inverted wires if needed */
    if (1 == num_blb_ports) {
      /* get inv_spice_model */
      blb_inv_spice_model = blb_port[0]->inv_spice_model;
      /* Make an inversion of the BL */
      for (iinv = 0; iinv < num_array_bl; iinv++) {
        fprintf(fp, " %s %s_blb_%d (%s[%d], %s[%d]);\n",
                blb_inv_spice_model->name, blb_inv_spice_model->prefix, 
                iinv, 
                top_netlist_array_bl_port_name, iinv,
                top_netlist_array_blb_port_name, iinv);
      }
    }
    if (1 == num_wlb_ports) {
      /* get inv_spice_model */
      wlb_inv_spice_model = wlb_port[0]->inv_spice_model;
      /* Make an inversion of the WL */
      for (iinv = 0; iinv < num_array_wl; iinv++) {
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

  /* Comment lines */
  fprintf(fp, "//----- BEGIN call decoders for memory bank controller -----\n");

  /* Dump Decoders for Bit lines and Word lines */
  /* Two huge decoders
   * TODO: divide to a number of small decoders ?
   */
  /* Bit lines decoder */
  fprintf(fp, "  ");
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
  fprintf(fp, "  ");
  fprintf(fp, "wl_decoder%dto%d mem_bank_wl_decoder (",
          wl_decoder_size, num_array_wl);
  fprintf(fp, "%s, %s[%d:0], ",
          top_netlist_wl_enable_port_name,
          top_netlist_addr_wl_port_name, wl_decoder_size - 1); 
  fprintf(fp, "%s[0:%d]", 
          top_netlist_array_wl_port_name, num_array_wl - 1);
  fprintf(fp, ");\n");
  fprintf(fp, "//----- END call decoders for memory bank controller -----\n\n");

  /* Comment lines */
  fprintf(fp, "endmodule\n");
  fprintf(fp, "//----- END configuration peripheral for memory-bank -----\n\n");

}

/* Top-level function */
void dump_verilog_config_peripherals(t_sram_orgz_info* cur_sram_orgz_info,
                                     char* verilog_dir_path,
                                     char* submodule_dir_path) {
  FILE* fp = NULL;

  char* verilog_name = my_strcat(submodule_dir_path, config_peripheral_verilog_file_name);

  /* Open file and file handler */
  fp = fopen(verilog_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create decoder SPICE netlist %s", 
               __FILE__, __LINE__, verilog_name); 
    exit(1);
  } 
  /* Generate file header*/ 
  vpr_printf(TIO_MESSAGE_INFO, "Writing configuration peripheral verilog netlist...\n");
 
  /* Generate the descriptions*/
  dump_verilog_file_header(fp, " Verilog Configuration Peripheral");

  verilog_include_defines_preproc_file(fp, verilog_dir_path);

  switch(cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    dump_verilog_scan_chain_config_module(fp, cur_sram_orgz_info);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* Dump verilog decoder */
    dump_verilog_decoder(fp, cur_sram_orgz_info);
    dump_verilog_membank_config_module(fp, cur_sram_orgz_info);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Close the file*/
  fclose(fp);

  /* Add fname to the linked list */
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_name);  

  return;
}

