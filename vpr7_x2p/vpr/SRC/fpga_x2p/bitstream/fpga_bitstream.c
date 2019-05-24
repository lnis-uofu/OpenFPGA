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
#include "read_xml_spice_util.h"
#include "linkedlist.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_backannotate_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "fpga_x2p_globals.h"
#include "fpga_bitstream_pbtypes.h"
#include "fpga_bitstream_routing.h"
#include "fpga_bitstream.h"

/* Global variables only in file */
static int dumped_num_conf_bits = 0;

/* Local Subroutines */
static 
void rec_dump_conf_bits_to_bitstream_file(FILE* fp, 
                                          t_sram_orgz_info* cur_sram_orgz_info,
                                          t_llist* cur_conf_bit);

/* USE while LOOP !!! Recursive method causes memory corruptions!*/
static 
void dump_conf_bits_to_bitstream_file(FILE* fp, 
                                      t_sram_orgz_info* cur_sram_orgz_info,
                                      t_llist* cur_conf_bit_head);


/* Generate a file contain all the configuration bits of the mapped FPGA.
 * The configuration bits are loaded to FPGA in a stream, which is called bitstream
 * In this file, the property of configuration bits will be shown as comments,
 * which is easy for developers to debug
 */
void dump_fpga_spice_bitstream(char* bitstream_file_name, 
                               char* circuit_name,
                               t_sram_orgz_info* cur_sram_orgz_info) {
  FILE* fp;

  /* Check if the path exists*/
  fp = fopen(bitstream_file_name,"w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create bitstream %s!",__FILE__, __LINE__, bitstream_file_name); 
    exit(1);
  } 
  
  vpr_printf(TIO_MESSAGE_INFO, "Writing bitstream file (%s) for %s...\n", 
             bitstream_file_name, circuit_name);

  /* Reset counter */
  dumped_num_conf_bits = 0;

  /* Find the head of bitstream: which is the tail of linked list */ 
  /* rec_dump_conf_bits_to_bitstream_file(fp, cur_sram_orgz_info, cur_sram_orgz_info->conf_bit_head); */
  dump_conf_bits_to_bitstream_file(fp, cur_sram_orgz_info, cur_sram_orgz_info->conf_bit_head);

  /* close file */
  fclose(fp);

  vpr_printf(TIO_MESSAGE_INFO, "Dumped %d configuration bits into bitstream file...\n", 
             dumped_num_conf_bits);

  /* Free the linked-list contain configuration bits ? */

  return;
}

/* Encode the given input to the address for a decode*/
void encode_decoder_addr(int input,
                         int decoder_size, char* addr) {
  int temp = input; 
  int i;

  assert(NULL != addr);
  /* Check the length of addr !*/
  // assert((decoder_size + 1) == len_addr);
  /* Add the end of a string */
  addr[decoder_size] = '\0';

  for (i = 0; i < decoder_size; i++) {
    addr[i] = '0';
  }

  i = decoder_size - 1;
 
  /* Actually, we convert a decimal number to its binary format */
  while (0 != temp) {
    addr[i] = (temp % 2) + '0'; 
    temp = temp/2;
    i--;
    /* Check i is still in the boundary */
    if (i < 0) {
      break; /* Quit the loop */ 
    }
  } 
  /* May be the decoder size is too small */
  if (0 != temp) {
    vpr_printf(TIO_MESSAGE_WARNING, "(File:%s,[LINE%d])Decoder size(%d) is too small for input(%d)!\n",
               __FILE__, __LINE__, decoder_size, input);
  }
   
  return; 
}

/* Recursively dump configuration bits which are stored in the linked list 
 * We start dump configuration bit from the tail of the linked list
 * until the head of the linked list
 */
static 
void rec_dump_conf_bits_to_bitstream_file(FILE* fp, 
                                          t_sram_orgz_info* cur_sram_orgz_info,
                                          t_llist* cur_conf_bit) {
  t_conf_bit_info* cur_conf_bit_info = (t_conf_bit_info*)(cur_conf_bit->dptr);
  int num_bl, num_wl, bl_decoder_size, wl_decoder_size;
  char* bl_addr = NULL;
  char* wl_addr = NULL; 

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!",__FILE__, __LINE__); 
    exit(1);
  } 
 
  /* Check */
  assert(NULL != cur_conf_bit_info); 

  if (NULL != cur_conf_bit->next) {
    /* This is not the tail, keep going */
    rec_dump_conf_bits_to_bitstream_file(fp, cur_sram_orgz_info, cur_conf_bit->next);
  }

  /* We alraedy touch the tail, start dump */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    /* Scan-chain only loads the SRAM values */
    fprintf(fp, "%d, ", cur_conf_bit_info->sram_bit->val),
    fprintf(fp, "// Configuration bit No.: %d, ", cur_conf_bit_info->index);
    fprintf(fp, " SRAM value: %d, ", cur_conf_bit_info->sram_bit->val);
    fprintf(fp, " SPICE model name: %s, ", cur_conf_bit_info->parent_spice_model->name);
    fprintf(fp, " SPICE model index: %d ", cur_conf_bit_info->parent_spice_model_index);
    fprintf(fp, "\n");
    /* Update the counter */
    dumped_num_conf_bits++;
    break;
  case SPICE_SRAM_MEMORY_BANK:
    get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &num_bl, &num_wl);
    bl_decoder_size = determine_decoder_size(num_bl);
    wl_decoder_size = determine_decoder_size(num_wl);

    /* Memory bank requires the address to be given to the decoder*/
    /* Word line address */
    bl_addr = (char*)my_calloc(bl_decoder_size + 1, sizeof(char));
    /* If this WL is selected , we decode its index to address */
    assert(NULL != cur_conf_bit_info->bl);
    encode_decoder_addr(cur_conf_bit_info->bl->addr, bl_decoder_size, bl_addr);
    fprintf(fp, "bl'%s = %d, ", bl_addr, cur_conf_bit_info->bl->val);
    fprintf(fp, "// Configuration bit No.: %d, ", cur_conf_bit_info->index);
    fprintf(fp, " Bit Line: %d, ", cur_conf_bit_info->bl->val);
    fprintf(fp, " SPICE model name: %s, ", cur_conf_bit_info->parent_spice_model->name);
    fprintf(fp, " SPICE model index: %d ", cur_conf_bit_info->parent_spice_model_index);
    fprintf(fp, "\n");
    /* Bit line address */
    /* If this WL is selected , we decode its index to address */
    wl_addr = (char*)my_calloc(wl_decoder_size + 1, sizeof(char));
    assert(NULL != cur_conf_bit_info->wl);
    encode_decoder_addr(cur_conf_bit_info->wl->addr, wl_decoder_size, wl_addr);
    fprintf(fp, "wl'%s = %d, ", wl_addr, cur_conf_bit_info->wl->val);
    fprintf(fp, "// Configuration bit No.: %d, ", cur_conf_bit_info->index);
    fprintf(fp, " Word Line: %d, ", cur_conf_bit_info->wl->val);
    fprintf(fp, " SPICE model name: %s, ", cur_conf_bit_info->parent_spice_model->name);
    fprintf(fp, " SPICE model index: %d ", cur_conf_bit_info->parent_spice_model_index);
    fprintf(fp, "\n");
    /* Update the counter */
    dumped_num_conf_bits++;
    /* Free */
    my_free(wl_addr);
    my_free(bl_addr);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }


  return;
}

/* Dump a bitstream by using while loop */
static 
void dump_conf_bits_to_bitstream_file(FILE* fp, 
                                      t_sram_orgz_info* cur_sram_orgz_info,
                                      t_llist* cur_conf_bit_head) {
  t_llist* temp = cur_conf_bit_head;
  t_conf_bit_info* cur_conf_bit_info = NULL;
  int num_bl, num_wl, bl_decoder_size, wl_decoder_size;
  char* bl_addr = NULL;
  char* wl_addr = NULL; 

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!",__FILE__, __LINE__); 
    exit(1);
  } 
 
  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &num_bl, &num_wl);
  bl_decoder_size = determine_decoder_size(num_bl);
  wl_decoder_size = determine_decoder_size(num_wl);

  while (NULL != temp) {
    cur_conf_bit_info = (t_conf_bit_info*)(temp->dptr);
    /* We alraedy touch the tail, start dump */
    switch (cur_sram_orgz_info->type) {
    case SPICE_SRAM_STANDALONE:
    case SPICE_SRAM_SCAN_CHAIN:
      /* Scan-chain only loads the SRAM values */
      fprintf(fp, "%d, ", cur_conf_bit_info->sram_bit->val),
      fprintf(fp, "// Configuration bit No.: %d, ", cur_conf_bit_info->index);
      fprintf(fp, " SRAM value: %d, ", cur_conf_bit_info->sram_bit->val);
      fprintf(fp, " SPICE model name: %s, ", cur_conf_bit_info->parent_spice_model->name);
      fprintf(fp, " SPICE model index: %d ", cur_conf_bit_info->parent_spice_model_index);
      fprintf(fp, "\n");
      /* Update the counter */
      dumped_num_conf_bits++;
      break;
    case SPICE_SRAM_MEMORY_BANK:
      /* Memory bank requires the address to be given to the decoder*/
      /* Word line address */
      bl_addr = (char*)my_calloc(bl_decoder_size + 1, sizeof(char));
      /* If this WL is selected , we decode its index to address */
      assert(NULL != cur_conf_bit_info->bl);
      encode_decoder_addr(cur_conf_bit_info->bl->addr, bl_decoder_size, bl_addr);
      fprintf(fp, "bl'%s = %d, ", bl_addr, cur_conf_bit_info->bl->val);
      fprintf(fp, "// Configuration bit No.: %d, ", cur_conf_bit_info->index);
      fprintf(fp, " Bit Line: %d, ", cur_conf_bit_info->bl->val);
      fprintf(fp, " SPICE model name: %s, ", cur_conf_bit_info->parent_spice_model->name);
      fprintf(fp, " SPICE model index: %d ", cur_conf_bit_info->parent_spice_model_index);
      fprintf(fp, "\n");
      /* Bit line address */
      /* If this WL is selected , we decode its index to address */
      wl_addr = (char*)my_calloc(wl_decoder_size + 1, sizeof(char));
      assert(NULL != cur_conf_bit_info->wl);
      encode_decoder_addr(cur_conf_bit_info->wl->addr, wl_decoder_size, wl_addr);
      fprintf(fp, "wl'%s = %d, ", wl_addr, cur_conf_bit_info->wl->val);
      fprintf(fp, "// Configuration bit No.: %d, ", cur_conf_bit_info->index);
      fprintf(fp, " Word Line: %d, ", cur_conf_bit_info->wl->val);
      fprintf(fp, " SPICE model name: %s, ", cur_conf_bit_info->parent_spice_model->name);
      fprintf(fp, " SPICE model index: %d ", cur_conf_bit_info->parent_spice_model_index);
      fprintf(fp, "\n");
      /* Free */
      my_free(wl_addr);
      my_free(bl_addr);
      /* Update the counter */
      dumped_num_conf_bits++;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    /* Go to next */
    temp = temp->next;
  }
  
  return;
}

/* Top-level function*/
void vpr_fpga_generate_bitstream(t_vpr_setup vpr_setup,
                                 t_arch Arch,
                                 char* circuit_name,
                                 char* bitstream_file_path,
                                 t_sram_orgz_info** cur_sram_orgz_info) {
  /* Timer */
  clock_t t_start;
  clock_t t_end;
  float run_time_sec;

  char* chomped_parent_dir = NULL;
  char* chomped_circuit_name = NULL;

  char* routing_bitstream_log_file_path = NULL;
  char* lb_bitstream_log_file_path = NULL;

  /* Check if the routing architecture we support*/
  if (UNI_DIRECTIONAL != vpr_setup.RoutingArch.directionality) {
    vpr_printf(TIO_MESSAGE_ERROR, "FPGA Bitstream Generator only support uni-directional routing architecture!\n");
    exit(1);
  }
  
  /* We don't support mrFPGA */
#ifdef MRFPGA_H
  if (is_mrFPGA) {
    vpr_printf(TIO_MESSAGE_ERROR, "FPGA Bitstream Generator do not support mrFPGA!\n");
    exit(1);
  }
#endif
  
  assert (TRUE == vpr_setup.FPGA_SPICE_Opts.BitstreamGenOpts.gen_bitstream);

  /* Format the directory paths */
  split_path_prog_name(circuit_name, '/', &chomped_parent_dir, &chomped_circuit_name);

  /* VerilogGenerator formally starts*/
  vpr_printf(TIO_MESSAGE_INFO, "\nFPGA Bitstream generator starts...\n");
 
  /* Start time count */
  t_start = clock();

  /* assign the global variable of SRAM model */
  assert(NULL != Arch.sram_inf.verilog_sram_inf_orgz); /* Check !*/
  /* initialize the SRAM organization information struct */
  (*cur_sram_orgz_info) = alloc_one_sram_orgz_info();
  init_sram_orgz_info(*cur_sram_orgz_info, Arch.sram_inf.verilog_sram_inf_orgz->type, 
                      Arch.sram_inf.verilog_sram_inf_orgz->spice_model, nx + 2, ny + 2);
  /* Check all the SRAM port is using the correct SRAM SPICE MODEL */
  config_spice_models_sram_port_spice_model(Arch.spice->num_spice_model, 
                                            Arch.spice->spice_models,
                                            Arch.sram_inf.verilog_sram_inf_orgz->spice_model);

  /* zero the counter of each spice_model */
  zero_spice_models_cnt(Arch.spice->num_spice_model, Arch.spice->spice_models);

  /* Generate Bitstreams 
   * Bitstream generation must follow the sequence: CB => SB => Grid 
   * (To be consistent with Verilog Generator !!!)
   */
  init_sram_orgz_info_reserved_blwl(*cur_sram_orgz_info, vpr_setup.RoutingArch.num_switch, 
                                    switch_inf, Arch.spice, &vpr_setup.RoutingArch);

  /* Routing: Connection Boxes and Switch Boxes */
  routing_bitstream_log_file_path = my_strcat(circuit_name, fpga_spice_bitstream_routing_log_file_postfix);
  fpga_spice_generate_bitstream_routing_resources(routing_bitstream_log_file_path,
                                                  Arch, &vpr_setup.RoutingArch, *cur_sram_orgz_info,
                                                  vpr_setup.FPGA_SPICE_Opts.compact_routing_hierarchy);


  /* Logic blocks */
  lb_bitstream_log_file_path = my_strcat(circuit_name, fpga_spice_bitstream_logic_block_log_file_postfix);
  fpga_spice_generate_bitstream_logic_block(lb_bitstream_log_file_path,
                                            &Arch, *cur_sram_orgz_info);


  /* Dump bitstream file */
  dump_fpga_spice_bitstream(bitstream_file_path, chomped_circuit_name, *cur_sram_orgz_info);

  /* End time count */
  t_end = clock();
 
  run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, "Bitstream Generation took %g seconds\n", run_time_sec);  

  /* Free */
 
  return;

}

/* This is a shell for bitstream generation
 * Prepare all the variables required by the core generator
 */
void vpr_fpga_bitstream_generator(t_vpr_setup vpr_setup,
                                        t_arch Arch,
                                        char* circuit_name,
                                        t_sram_orgz_info** cur_sram_orgz_info) {
  char* bitstream_file_path = NULL; 

  if (NULL == vpr_setup.FPGA_SPICE_Opts.BitstreamGenOpts.bitstream_output_file) {
    bitstream_file_path = my_strcat(circuit_name, fpga_spice_bitstream_output_file_postfix);
  } else {
    bitstream_file_path = my_strdup(vpr_setup.FPGA_SPICE_Opts.BitstreamGenOpts.bitstream_output_file);
  }

  /* Run bitstream generation and dump output file */
  vpr_fpga_generate_bitstream(vpr_setup, Arch, circuit_name, bitstream_file_path, cur_sram_orgz_info);

  /* Free */
  my_free(bitstream_file_path);

}


