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

/***** Subroutines *****/
void determine_verilog_blwl_decoder_size(INP t_sram_orgz_info* cur_sram_verilog_orgz_info,
                                         OUTP int* num_array_bl, OUTP int* num_array_wl,
                                         OUTP int* bl_decoder_size, OUTP int* wl_decoder_size) {
  t_spice_model* mem_model = NULL;
  int num_mem_bit;
  int num_reserved_bl, num_reserved_wl;

  /* Check */
  assert(SPICE_SRAM_MEMORY_BANK == sram_verilog_orgz_info->type);

  num_mem_bit = get_sram_orgz_info_num_mem_bit(sram_verilog_orgz_info);
  get_sram_orgz_info_num_blwl(sram_verilog_orgz_info, num_array_bl, num_array_wl);
  get_sram_orgz_info_reserved_blwl(sram_verilog_orgz_info, &num_reserved_bl, &num_reserved_wl);

  /* Sizes of decodes depend on the Memory technology */
  get_sram_orgz_info_mem_model(sram_verilog_orgz_info, &mem_model); 
  switch (mem_model->design_tech) {
  /* CMOS SRAM*/
  case SPICE_MODEL_DESIGN_CMOS:
   /* SRAMs can efficiently share BLs and WLs, 
    * Actual number of BLs and WLs will be sqrt(num_bls) and sqrt(num_wls) 
    */
    assert(0 == num_reserved_bl);
    assert(0 == num_reserved_wl);
    (*num_array_bl) = ceil(sqrt(*num_array_bl));
    (*num_array_wl) = ceil(sqrt(*num_array_wl));
    (*bl_decoder_size) = determine_decoder_size(*num_array_bl);
    (*wl_decoder_size) = determine_decoder_size(*num_array_wl);
    break;
  /* RRAM */
  case SPICE_MODEL_DESIGN_RRAM:
    /* Currently we do not have more efficient way to share the BLs and WLs as CMOS SRAMs */
    (*bl_decoder_size) = determine_decoder_size(*num_array_bl);
    (*wl_decoder_size) = determine_decoder_size(*num_array_wl);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology [CMOS|RRAM] for memory technology!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

void dump_verilog_decoder(char* submodule_dir) {
  int num_array_bl, num_array_wl;
  int bl_decoder_size, wl_decoder_size;
  FILE* fp = NULL;
  t_spice_model* mem_model = NULL;
  boolean bl_inverted = FALSE;
  boolean wl_inverted = FALSE;

  char* verilog_name = my_strcat(submodule_dir, decoders_verilog_file_name);
  
  /* Print the muxes netlist*/
  fp = fopen(verilog_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create decoder SPICE netlist %s",__FILE__, __LINE__, verilog_name); 
    exit(1);
  } 

  /* Check */
  assert(SPICE_SRAM_MEMORY_BANK == sram_verilog_orgz_info->type);

  /* Get number of BLs,WLs and decoder sizes */
  determine_verilog_blwl_decoder_size(sram_verilog_orgz_info, 
                                      &num_array_bl, &num_array_wl, 
                                      &bl_decoder_size, &wl_decoder_size);
   
  /* Generate file header*/ 
  vpr_printf(TIO_MESSAGE_INFO, "Writing Decoder verilog netlist...\n");
 
  /* Generate the descriptions*/
  dump_verilog_file_header(fp, " Verilog Decoders");

  /* Different design technology requires different BL decoder logic */
  get_sram_orgz_info_mem_model(sram_verilog_orgz_info, &mem_model); 
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

  /* Close the file*/
  fclose(fp);
  
  /* Add fname to the linked list */
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_name);

  return;
}
