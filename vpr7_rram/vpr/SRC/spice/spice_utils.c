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
#include "rr_graph_util.h"
#include "rr_graph.h"
#include "rr_graph2.h"
#include "vpr_utils.h"

/* Include spice support headers*/
#include "linkedlist.h"
#include "fpga_spice_globals.h"
#include "spice_globals.h"
#include "fpga_spice_utils.h"
#include "spice_mux.h"
#include "spice_pbtypes.h"
#include "spice_routing.h"
#include "fpga_spice_backannotate_utils.h"
#include "spice_utils.h"

/***** Subroutines *****/
void fprint_spice_head(FILE* fp,
                       char* usage) {
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d]) FileHandle is NULL!\n",__FILE__,__LINE__); 
    exit(1);
  } 
  fprintf(fp,"*****************************\n");
  fprintf(fp,"*     FPGA SPICE Netlist    *\n");
  fprintf(fp,"* Description: %s *\n",usage);
  fprintf(fp,"*    Author: Xifan TANG     *\n");
  fprintf(fp,"* Organization: EPFL/IC/LSI *\n");
  fprintf(fp,"* Date: %s *\n",my_gettime());
  fprintf(fp,"*****************************\n");
  return;
}


/* Print all the global ports that are stored in the linked list 
 * Return the number of ports that have been dumped 
 */
int rec_fprint_spice_model_global_ports(FILE* fp, 
                                        t_spice_model* cur_spice_model,
                                        boolean recursive) {
  int i, iport, dumped_port_cnt, rec_dumped_port_cnt;

  dumped_port_cnt = 0;
  rec_dumped_port_cnt = 0;

  /* Check */
  assert(NULL != cur_spice_model);
  if (0 < cur_spice_model->num_port) {
    assert(NULL != cur_spice_model->ports);
  }

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
  }

  for (iport = 0; iport < cur_spice_model->num_port; iport++) {
    /* if this spice model requires customized netlist to be included, we do not go recursively */
    if (TRUE == recursive) { 
      /* GO recursively first, and meanwhile count the number of global ports */
      /* For the port that requires another spice_model, i.e., SRAM
       * We need include any global port in that spice model
       */
      if (NULL != cur_spice_model->ports[iport].spice_model) {
        /* Check if we need to dump a comma */
        rec_dumped_port_cnt += 
           rec_fprint_spice_model_global_ports(fp, cur_spice_model->ports[iport].spice_model, 
                                               recursive);
        /* Update counter */
        dumped_port_cnt += rec_dumped_port_cnt;
        continue;
      }
    }
    /* By pass non-global ports*/
    if (FALSE == cur_spice_model->ports[iport].is_global) {
      continue;
    }
    /* We have some port to dump ! 
     * Print a comment line 
     */
    if (0 == dumped_port_cnt) {
      fprintf(fp, "\n");
      fprintf(fp, "***** BEGIN Global ports of SPICE_MODEL(%s) *****\n",
              cur_spice_model->name);
      fprintf(fp, "+ ");
    }
    /* Check if we need to dump a comma */
    for (i = 0; i < cur_spice_model->ports[iport].size; i++) {
      fprintf(fp, " %s[%d] ", 
              cur_spice_model->ports[iport].prefix,
              i);
    }
    /* Update counter */
    dumped_port_cnt++;
  }
  
  /* We have dumped some port! 
   * Print another comment line  
   */
  if (0 < dumped_port_cnt) {
    fprintf(fp, "\n");
    fprintf(fp, "***** END Global ports of SPICE_MODEL(%s) *****\n",
            cur_spice_model->name);
  }

  return dumped_port_cnt;
}

/* Print all the global ports that are stored in the linked list */
int fprint_spice_global_ports(FILE* fp, t_llist* head) {
  t_llist* temp = head;
  t_spice_model_port* cur_global_port = NULL;
  int dumped_port_cnt = 0;
  int i;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
  }

  fprintf(fp, "\n");
  fprintf(fp, "***** BEGIN Global ports *****\n");
  fprintf(fp, "+ ");
  while(NULL != temp) {
    cur_global_port = (t_spice_model_port*)(temp->dptr); 
    for (i = 0; i < cur_global_port->size; i++) {
      fprintf(fp, " %s[%d] ", 
              cur_global_port->prefix,
              i); 
    }
    /* Update counter */
    dumped_port_cnt++;
    /* Go to the next */
    temp = temp->next;
  }
  fprintf(fp, "\n");
  fprintf(fp, "***** END Global ports *****\n");

  return dumped_port_cnt;
}

void fprint_spice_generic_testbench_global_ports(FILE* fp, 
                                                 t_sram_orgz_info* cur_sram_orgz_info,
                                                 t_llist* head) {
  t_spice_model* mem_model = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
  }

  fprintf(fp, "***** Generic global ports ***** \n");
  fprintf(fp, "***** VDD, GND  ***** \n");
  fprintf(fp, ".global %s\n", 
              spice_tb_global_vdd_port_name);
  fprintf(fp, ".global %s\n", 
              spice_tb_global_gnd_port_name);
  fprintf(fp, "***** Global set ports ***** \n");
  fprintf(fp, ".global %s %s%s \n", 
              spice_tb_global_set_port_name,
              spice_tb_global_set_port_name,
              spice_tb_global_port_inv_postfix);
  fprintf(fp, "***** Global reset ports ***** \n");
  fprintf(fp, ".global %s %s%s\n", 
              spice_tb_global_reset_port_name,
              spice_tb_global_reset_port_name,
              spice_tb_global_port_inv_postfix);
  fprintf(fp, "***** Configuration done ports ***** \n");
  fprintf(fp, ".global %s %s%s\n", 
              spice_tb_global_config_done_port_name,
              spice_tb_global_config_done_port_name,
              spice_tb_global_port_inv_postfix);

  /* Get memory spice model */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
  fprintf(fp, "***** Global SRAM input ***** \n");
  fprintf(fp, ".global %s->in\n", mem_model->prefix);

  /* Print scan-chain global ports */
  if (SPICE_SRAM_SCAN_CHAIN == sram_spice_orgz_type) {
  fprintf(fp, "***** Scan-chain FF: head of scan-chain *****\n");
    fprintf(fp, "*.global %s[0]->in\n", sram_spice_model->prefix);
  }

  /* Define a global clock port if we need one*/
  fprintf(fp, "***** Global Clock Signals *****\n");
  fprintf(fp, ".global %s\n", 
              spice_tb_global_clock_port_name);
  fprintf(fp, ".global %s%s\n", 
              spice_tb_global_clock_port_name,
              spice_tb_global_port_inv_postfix);

  fprintf(fp, "***** User-defined global ports ****** \n");
  if (NULL != head) {
    fprintf(fp, ".global \n");
  }
  fprint_spice_global_ports(fp, head);

  return;
}

/* Print a SRAM output port in SPICE format */
void fprint_spice_sram_one_outport(FILE* fp,
                                   t_sram_orgz_info* cur_sram_orgz_info,
                                   int cur_sram,
                                   int port_type_index) {
  t_spice_model* mem_model = NULL;
  char* port_name = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Get memory_model */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
 
  /* Keep the branch as it is, in case thing may become more complicated*/
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    if (0 == port_type_index) {
      port_name = "out";
    } else {
      assert(1 == port_type_index);
      port_name = "outb";
    }
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    if (0 == port_type_index) {
      port_name = "scff_out";
    } else {
      assert(1 == port_type_index);
      port_name = "scff_outb";
    }
    break;
  case SPICE_SRAM_MEMORY_BANK:
    if (0 == port_type_index) {
      port_name = "out";
    } else {
      assert(1 == port_type_index);
      port_name = "outb";
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /*Malloc and generate the full name of port */
  fprintf(fp, "%s[%d]->%s ", 
          mem_model->prefix, cur_sram, port_name); /* Outputs */

  /* Free */
  /* Local variables such as port1_name and port2 name are automatically freed  */

  return;
}

/* Print a SRAM module in SPICE format */
void fprint_spice_one_specific_sram_subckt(FILE* fp,
                                          t_sram_orgz_info* cur_sram_orgz_info,
                                          t_spice_model* parent_spice_model,
                                          char* vdd_port_name,
                                          int sram_index) {
  t_spice_model* mem_model = NULL;
  int cur_sram = 0;
  
  /* Get memory model */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);

  /* Get current index of SRAM module */
  cur_sram = sram_index; 

  /* Depend on the type of SRAM organization */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_MEMORY_BANK:
    fprintf(fp, "X%s[%d] ", mem_model->prefix, cur_sram); /* SRAM subckts*/
    /* fprintf(fp, "%s[%d]->in ", sram_spice_model->prefix, cur_sram);*/ /* Input*/
    /* Global ports : 
     * Only dump the global ports belonging to a spice_model 
     * Do not go recursive, we can freely define global ports anywhere in SPICE netlist
     */
    rec_fprint_spice_model_global_ports(fp, mem_model, FALSE); 
    /* Local ports */
    fprintf(fp, "%s->in ", mem_model->prefix); /* Input*/
    fprintf(fp, "%s[%d]->out %s[%d]->outb ", 
            mem_model->prefix, cur_sram, mem_model->prefix, cur_sram); /* Outputs */
    fprintf(fp, "%s sgnd ", 
                vdd_port_name);  //
    fprintf(fp, " %s\n", mem_model->name);  //
    /* Add nodeset to help convergence */ 
    fprintf(fp, ".nodeset V(%s[%d]->out) 0\n", mem_model->prefix, cur_sram);
    fprintf(fp, ".nodeset V(%s[%d]->outb) vsp\n", mem_model->prefix, cur_sram);
    break;
  case SPICE_SRAM_SCAN_CHAIN: 
    fprintf(fp, "X%s[%d] ", mem_model->prefix, cur_sram); /* SRAM subckts*/
    /* Global ports : 
     * Only dump the global ports belonging to a spice_model 
     * Do not go recursive, we can freely define global ports anywhere in SPICE netlist
     */
    rec_fprint_spice_model_global_ports(fp, mem_model, FALSE); 
    /* Local ports */
    fprintf(fp, "%s[%d]->in ", mem_model->prefix, cur_sram); /* Input*/
    fprintf(fp, "%s[%d]->out %s[%d]->outb ", 
            mem_model->prefix, cur_sram, mem_model->prefix, cur_sram); /* Outputs */
    fprintf(fp, "sc_clk sc_rst sc_set \n");  //
    fprintf(fp, "%s sgnd ", 
                 vdd_port_name);  //
    fprintf(fp, " %s\n", mem_model->name);  //
    /* Add nodeset to help convergence */ 
    fprintf(fp, ".nodeset V(%s[%d]->out) 0\n", mem_model->prefix, cur_sram);
    fprintf(fp, ".nodeset V(%s[%d]->outb) vsp\n", mem_model->prefix, cur_sram);
    /* Connect to the tail of previous Scan-chain FF*/
    fprintf(fp,"R%s[%d]_short %s[%d]->out %s[%d]->in 0\n", 
            mem_model->prefix, cur_sram,
            mem_model->prefix, cur_sram,
            sram_spice_model->prefix, cur_sram + 1);
    /* Specify this is a global signal*/
    fprintf(fp, ".global %s[%d]->in\n", sram_spice_model->prefix, cur_sram);
    /* Specify the head and tail of the scan-chain of this LUT */
    fprintf(fp,"R%s[%d]_sc_head %s[%d]_sc_head %s[%d]->in 0\n", 
            mem_model->prefix, mem_model->cnt, 
            mem_model->prefix, mem_model->cnt, 
            mem_model->prefix, mem_model->cnt);
    fprintf(fp,"R%s[%d]_sc_tail %s[%d]_sc_tail %s[%d]->in 0\n", 
            mem_model->prefix, mem_model->cnt, 
            mem_model->prefix, mem_model->cnt, 
            mem_model->prefix, cur_sram);
    fprintf(fp,".global %s[%d]_sc_head %s[%d]_sc_tail\n",
            mem_model->prefix, mem_model->cnt, 
            mem_model->prefix, mem_model->cnt);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,LINE[%d]) Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  
  return;
}


/* Print a SRAM module in SPICE format */
void fprint_spice_one_sram_subckt(FILE* fp,
                                  t_sram_orgz_info* cur_sram_orgz_info,
                                  t_spice_model* parent_spice_model,
                                  char* vdd_port_name) {
  t_spice_model* mem_model = NULL;
  int cur_num_sram = 0;
  
  /* Get memory model */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);

  /* Get current index of SRAM module */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  
  /* Call a subroutine: fprint_spice_one_specific_sram_subckt */
  fprint_spice_one_specific_sram_subckt(fp, cur_sram_orgz_info, 
                                        parent_spice_model, 
                                        vdd_port_name, cur_num_sram); 

  /* Update the memory counter in sram_orgz_info */
  update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info,
                                    cur_num_sram + 1);
  /* Update the memory counter */
  mem_model->cnt++;

  return;
}

/* Include user defined SPICE netlists */
void init_include_user_defined_netlists(t_spice spice) {
  int i;

  /* Include user-defined sub-circuit netlist */
  for (i = 0; i < spice.num_include_netlist; i++) {
    spice.include_netlists[i].included = 0;
  }

  return;
}

void fprint_include_user_defined_netlists(FILE* fp,
                                          t_spice spice) {
  int i;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Include user-defined sub-circuit netlist */
  for (i = 0; i < spice.num_include_netlist; i++) {
    if (0 == spice.include_netlists[i].included) {
      assert(NULL != spice.include_netlists[i].path);
      fprintf(fp, ".include \'%s\'\n", spice.include_netlists[i].path);
      spice.include_netlists[i].included = 1;
    } else {
      assert(1 == spice.include_netlists[i].included);
    }
  } 

  return;
}

void fprint_splited_vdds_spice_model(FILE* fp,
                                     enum e_spice_model_type spice_model_type,
                                     t_spice spice) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
        fprintf(fp, "V%s_%s[%d] %s_%s[%d] 0 vsp\n", 
                spice_tb_global_vdd_port_name,
                spice.spice_models[imodel].prefix, i, 
                spice_tb_global_vdd_port_name,
                spice.spice_models[imodel].prefix, i);
        /* For some gvdd maybe floating, I add a huge resistance to make their leakage power trival 
         * which does no change to the delay result.
         * The resistance value is co-related to the vsp, which produces a trival leakage current (1e-15).
         */
        fprintf(fp, "Rgvdd_%s[%d]_huge gvdd_%s[%d] 0 'vsp/10e-15'\n", 
                spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
      }
    }
  }
  
  return;
}

void fprint_grid_splited_vdds_spice_model(FILE* fp, int grid_x, int grid_y,
                                          enum e_spice_model_type spice_model_type,
                                          t_spice spice) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      /* Bypass zero-usage spice_model in this grid*/
      if (spice.spice_models[imodel].grid_index_low[grid_x][grid_y]
          == spice.spice_models[imodel].grid_index_high[grid_x][grid_y]) {
        continue;
      }
      for (i = spice.spice_models[imodel].grid_index_low[grid_x][grid_y]; 
           i < spice.spice_models[imodel].grid_index_high[grid_x][grid_y]; 
           i++) {
        fprintf(fp, "Vgvdd_%s[%d] gvdd_%s[%d] 0 vsp\n", 
                spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
        /* For some gvdd maybe floating, I add a huge resistance to make their leakage power trival 
         * which does no change to the delay result.
         * The resistance value is co-related to the vsp, which produces a trival leakage current (1e-15).
         */
        fprintf(fp, "Rgvdd_%s[%d]_huge gvdd_%s[%d] 0 'vsp/10e-15'\n", 
                spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
      }
    }
  }
 
  return;
}

void fprint_global_vdds_spice_model(FILE* fp, 
                                    enum e_spice_model_type spice_model_type,
                                    t_spice spice) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  fprintf(fp, "***** Global VDD ports of %s *****\n", generate_string_spice_model_type(spice_model_type));
  fprintf(fp, ".global \n");

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
        fprintf(fp, "+ %s_%s[%d]\n", 
                spice_tb_global_vdd_port_name,
                spice.spice_models[imodel].prefix, i);
      }
    }
  }
  
  fprintf(fp, "\n");

  return;
}

void fprint_grid_global_vdds_spice_model(FILE* fp, int x, int y, 
                                         enum e_spice_model_type spice_model_type,
                                         t_spice spice) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  fprintf(fp, "***** Global VDD ports of %s *****\n", generate_string_spice_model_type(spice_model_type));

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    /* Bypass non-matched SPICE model */
    if (spice_model_type != spice.spice_models[imodel].type) {
       continue;
    }
    /* Bypass zero-usage spice_model in this grid*/
    if (spice.spice_models[imodel].grid_index_low[x][y]
        == spice.spice_models[imodel].grid_index_high[x][y]) {
      continue;
    }
    fprintf(fp, ".global \n");
    for (i = spice.spice_models[imodel].grid_index_low[x][y]; 
         i < spice.spice_models[imodel].grid_index_high[x][y]; 
         i++) {
      fprintf(fp, "+ gvdd_%s[%d]\n", 
              spice.spice_models[imodel].prefix, i);
    }
  }
  
  fprintf(fp, "\n");

  return;
}

void fprint_global_pad_ports_spice_model(FILE* fp, 
                                         t_spice spice) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  fprintf(fp, "***** Global input/output ports of I/O Pads *****\n");
  fprintf(fp, ".global \n");

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    switch (spice.spice_models[imodel].type) {
    /* Handle multiple INPAD/OUTPAD spice models*/
    case SPICE_MODEL_IOPAD:
      for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
        fprintf(fp, "+ %s%s[%d]\n", gio_inout_prefix, spice.spice_models[imodel].prefix, i);
      }
      break; 
    /* SRAM inputs*/
    case SPICE_MODEL_SRAM:
      /*
      for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
        fprintf(fp, "+ %s[%d]->in\n", spice.spice_models[imodel].prefix, i);
      }
      fprintf(fp, "+ %s->in\n", spice.spice_models[imodel].prefix);
      */
      break;
    /* Other types we do not care*/
    case SPICE_MODEL_CHAN_WIRE:
    case SPICE_MODEL_WIRE:
    case SPICE_MODEL_MUX:
    case SPICE_MODEL_LUT:
    case SPICE_MODEL_FF:
    case SPICE_MODEL_HARDLOGIC:
    case SPICE_MODEL_SCFF:
    case SPICE_MODEL_INVBUF:
    case SPICE_MODEL_PASSGATE: 
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Unknown type for spice model!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }

  fprintf(fp, "\n");
 
  return; 
}

void fprint_spice_global_vdd_switch_boxes(FILE* fp) {
  int ix, iy;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  fprintf(fp, "***** Global Vdds for Switch Boxes *****\n");
  fprintf(fp, ".global ");
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 0; iy < (ny + 1); iy++) {
      fprintf(fp, "gvdd_sb[%d][%d] ", ix, iy);
    }
  }
  fprintf(fp, "\n");

  return;
}

/* Call the sub-circuits for connection boxes */
void fprint_spice_global_vdd_connection_boxes(FILE* fp) {
  int ix, iy;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  fprintf(fp, "***** Global Vdds for Connection Blocks - X channels *****\n");
  fprintf(fp, ".global ");
  /* X - channels [1...nx][0..ny]*/
  for (iy = 0; iy < (ny + 1); iy++) {
    for (ix = 1; ix < (nx + 1); ix++) {
      fprintf(fp, "gvdd_cbx[%d][%d] ", ix, iy);
    }
  }
  fprintf(fp, "\n");

  fprintf(fp, "***** Global Vdds for Connection Blocks - Y channels *****\n");
  fprintf(fp, ".global ");
  /* Y - channels [1...ny][0..nx]*/
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      fprintf(fp, "gvdd_cby[%d][%d] ", ix, iy);
    }
  }
  fprintf(fp, "\n");
 
  return; 
}

void fprint_measure_vdds_spice_model(FILE* fp,
                                     enum e_spice_model_type spice_model_type,
                                     enum e_measure_type meas_type,
                                     int num_cycle,
                                     t_spice spice,
                                     boolean leakage_only) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
        switch (meas_type) {
        case SPICE_MEASURE_LEAKAGE_POWER:
          if (TRUE == leakage_only) {
            fprintf(fp, ".measure tran leakage_power_%s[%d] find p(Vgvdd_%s[%d]) at=0\n",
                    spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
          } else {
            fprintf(fp, ".measure tran leakage_power_%s[%d] avg p(Vgvdd_%s[%d]) from=0 to='clock_period'\n",
                    spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
          }
          break;
        case SPICE_MEASURE_DYNAMIC_POWER:
          fprintf(fp, ".measure tran dynamic_power_%s[%d] avg p(Vgvdd_%s[%d]) from='clock_period' to='%d*clock_period'\n",
                  spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i, num_cycle);
          break;
        default: 
          vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid meas_type!\n", __FILE__, __LINE__);
          exit(1);
        }
      }
    }
  }

  /* Measure the total power of this kind of spice model */
  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      switch (meas_type) {
      case SPICE_MEASURE_LEAKAGE_POWER:
        for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
          fprintf(fp, ".measure tran leakage_power_%s[0to%d] \n", spice.spice_models[imodel].prefix, i);
          if (0 == i) {
            fprintf(fp, "+ param = 'leakage_power_%s[%d]'\n", spice.spice_models[imodel].prefix, i);
          } else {
            fprintf(fp, "+ param = 'leakage_power_%s[%d]+leakage_power_%s[0to%d]'\n", 
                    spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i-1);
          }
        }
        /* Spot the total leakage power of this spice model */
        fprintf(fp, ".measure tran total_leakage_power_%s \n", spice.spice_models[imodel].prefix);
        fprintf(fp, "+ param = 'leakage_power_%s[0to%d]'\n", 
                spice.spice_models[imodel].prefix, spice.spice_models[imodel].cnt-1);
        break;
      case SPICE_MEASURE_DYNAMIC_POWER:
        for (i = 0; i < spice.spice_models[imodel].cnt; i++) {
          fprintf(fp, ".measure tran dynamic_power_%s[0to%d] \n", spice.spice_models[imodel].prefix, i);
          if (0 == i) {
            fprintf(fp, "+ param = 'dynamic_power_%s[%d]'\n", spice.spice_models[imodel].prefix, i);
          } else {
            fprintf(fp, "+ param = 'dynamic_power_%s[%d]+dynamic_power_%s[0to%d]'\n", 
                    spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i-1);
          }
        }
        /* Spot the total dynamic power of this spice model */
        fprintf(fp, ".measure tran total_dynamic_power_%s \n", spice.spice_models[imodel].prefix);
        fprintf(fp, "+ param = 'dynamic_power_%s[0to%d]'\n", 
                spice.spice_models[imodel].prefix, spice.spice_models[imodel].cnt-1);
        fprintf(fp, ".measure tran total_energy_per_cycle_%s \n", spice.spice_models[imodel].prefix);
        fprintf(fp, "+ param = 'dynamic_power_%s[0to%d]*clock_period'\n", 
                spice.spice_models[imodel].prefix, spice.spice_models[imodel].cnt-1);
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid meas_type!\n", __FILE__, __LINE__);
        exit(1);
      }
    }
  }
  
  
  return;
}

void fprint_measure_grid_vdds_spice_model(FILE* fp, int grid_x, int grid_y,
                                          enum e_spice_model_type spice_model_type,
                                          enum e_measure_type meas_type,
                                          int num_cycle,
                                          t_spice spice,
                                          boolean leakage_only) {
  int imodel, i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      /* Bypass zero-usage spice_model in this grid*/
      if (spice.spice_models[imodel].grid_index_low[grid_x][grid_y]
          == spice.spice_models[imodel].grid_index_high[grid_x][grid_y]) {
        continue;
      }
      for (i = spice.spice_models[imodel].grid_index_low[grid_x][grid_y]; 
           i < spice.spice_models[imodel].grid_index_high[grid_x][grid_y]; 
           i++) {
        switch (meas_type) {
        case SPICE_MEASURE_LEAKAGE_POWER:
          if (TRUE == leakage_only) {
            fprintf(fp, ".measure tran leakage_power_%s[%d] find p(Vgvdd_%s[%d]) at=0\n",
                    spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
          } else {
            fprintf(fp, ".measure tran leakage_power_%s[%d] avg p(Vgvdd_%s[%d]) from=0 to='clock_period'\n",
                    spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i);
          }
          break;
        case SPICE_MEASURE_DYNAMIC_POWER:
          fprintf(fp, ".measure tran dynamic_power_%s[%d] avg p(Vgvdd_%s[%d]) from='clock_period' to='%d*clock_period'\n",
                  spice.spice_models[imodel].prefix, i, spice.spice_models[imodel].prefix, i, num_cycle);
          break;
        default: 
          vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid meas_type!\n", __FILE__, __LINE__);
          exit(1);
        }
      }
    }
  }

  /* Measure the total power of this kind of spice model */
  for (imodel = 0; imodel < spice.num_spice_model; imodel++) {  
    if (spice_model_type == spice.spice_models[imodel].type) {
      switch (meas_type) {
      case SPICE_MEASURE_LEAKAGE_POWER:
        /* Bypass zero-usage spice_model in this grid*/
        if (spice.spice_models[imodel].grid_index_low[grid_x][grid_y]
            == spice.spice_models[imodel].grid_index_high[grid_x][grid_y]) {
          continue;
        }
        for (i = spice.spice_models[imodel].grid_index_low[grid_x][grid_y]; 
             i < spice.spice_models[imodel].grid_index_high[grid_x][grid_y]; 
             i++) {
          fprintf(fp, ".measure tran leakage_power_%s[%dto%d] \n", 
                  spice.spice_models[imodel].prefix, 
                  spice.spice_models[imodel].grid_index_low[grid_x][grid_y], 
                  i);
          if (spice.spice_models[imodel].grid_index_low[grid_x][grid_y] == i) {
            fprintf(fp, "+ param = 'leakage_power_%s[%d]'\n", spice.spice_models[imodel].prefix, i);
          } else {
            fprintf(fp, "+ param = 'leakage_power_%s[%d]+leakage_power_%s[%dto%d]'\n", 
                    spice.spice_models[imodel].prefix, 
                    i, spice.spice_models[imodel].prefix, 
                    spice.spice_models[imodel].grid_index_low[grid_x][grid_y], 
                    i-1);
          }
        }
        /* Spot the total leakage power of this spice model */
        fprintf(fp, ".measure tran total_leakage_power_%s \n", spice.spice_models[imodel].prefix);
        fprintf(fp, "+ param = 'leakage_power_%s[%dto%d]'\n", 
                spice.spice_models[imodel].prefix, 
                spice.spice_models[imodel].grid_index_low[grid_x][grid_y],
                spice.spice_models[imodel].grid_index_high[grid_x][grid_y]-1); 
        break;
      case SPICE_MEASURE_DYNAMIC_POWER:
        /* Bypass zero-usage spice_model in this grid*/
        if (spice.spice_models[imodel].grid_index_low[grid_x][grid_y]
            == spice.spice_models[imodel].grid_index_high[grid_x][grid_y]) {
          continue;
        }
        for (i = spice.spice_models[imodel].grid_index_low[grid_x][grid_y]; 
             i < spice.spice_models[imodel].grid_index_high[grid_x][grid_y]; 
             i++) {
          fprintf(fp, ".measure tran dynamic_power_%s[%dto%d] \n", 
                  spice.spice_models[imodel].prefix, 
                  spice.spice_models[imodel].grid_index_low[grid_x][grid_y], 
                  i);
          if (spice.spice_models[imodel].grid_index_low[grid_x][grid_y] == i) {
            fprintf(fp, "+ param = 'dynamic_power_%s[%d]'\n", spice.spice_models[imodel].prefix, i);
          } else {
            fprintf(fp, "+ param = 'dynamic_power_%s[%d]+dynamic_power_%s[%dto%d]'\n", 
                    spice.spice_models[imodel].prefix, i, 
                    spice.spice_models[imodel].prefix, 
                    spice.spice_models[imodel].grid_index_low[grid_x][grid_y], 
                    i-1);
          }
        }
        /* Spot the total dynamic power of this spice model */
        fprintf(fp, ".measure tran total_dynamic_power_%s \n", spice.spice_models[imodel].prefix);
        fprintf(fp, "+ param = 'dynamic_power_%s[%dto%d]'\n", 
                spice.spice_models[imodel].prefix,
                spice.spice_models[imodel].grid_index_low[grid_x][grid_y],
                spice.spice_models[imodel].grid_index_high[grid_x][grid_y]-1); 
        fprintf(fp, ".measure tran total_energy_per_cycle_%s \n", spice.spice_models[imodel].prefix);
        fprintf(fp, "+ param = 'dynamic_power_%s[%dto%d]*clock_period'\n", 
                spice.spice_models[imodel].prefix,
                spice.spice_models[imodel].grid_index_low[grid_x][grid_y],
                spice.spice_models[imodel].grid_index_high[grid_x][grid_y]-1); 
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid meas_type!\n", __FILE__, __LINE__);
        exit(1);
      }
    }
  }
  
  
  return;
}


/***** Print (call) the defined grids *****/
void fprint_call_defined_grids(FILE* fp) {
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
      fprintf(fp, "Xgrid[%d][%d] ", ix, iy);
      fprintf(fp, "\n");
      fprint_grid_pins(fp, ix, iy, 1);
      fprintf(fp, "+ ");
      fprintf(fp, "gvdd 0 grid[%d][%d]\n", ix, iy); /* Call the name of subckt */ 
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
    fprintf(fp, "Xgrid[%d][%d] ", ix, iy);
    fprintf(fp, "\n");
    fprint_io_grid_pins(fp, ix, iy, 1);
    fprintf(fp, "+ ");
    /* Connect to a speical vdd port for statistics power */
    fprintf(fp, "gvdd_io 0 grid[%d][%d]\n", ix, iy); /* Call the name of subckt */ 
  }

  /* RIGHT side */
  ix = nx + 1;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    fprintf(fp, "Xgrid[%d][%d] ", ix, iy);
    fprintf(fp, "\n");
    fprint_io_grid_pins(fp, ix, iy, 1);
    fprintf(fp, "+ ");
    /* Connect to a speical vdd port for statistics power */
    fprintf(fp, "gvdd_io 0 grid[%d][%d]\n", ix, iy); /* Call the name of subckt */ 
  }

  /* BOTTOM side */
  iy = 0;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    fprintf(fp, "Xgrid[%d][%d] ", ix, iy);
    fprintf(fp, "\n");
    fprint_io_grid_pins(fp, ix, iy, 1);
    fprintf(fp, "+ ");
    /* Connect to a speical vdd port for statistics power */
    fprintf(fp, "gvdd_io 0 grid[%d][%d]\n", ix, iy); /* Call the name of subckt */ 
  } 

  /* TOP side */
  iy = ny + 1;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    fprintf(fp, "Xgrid[%d][%d] ", ix, iy);
    fprintf(fp, "\n");
    fprint_io_grid_pins(fp, ix, iy, 1);
    fprintf(fp, "+ ");
    /* Connect to a speical vdd port for statistics power */
    fprintf(fp, "gvdd_io 0 grid[%d][%d]\n", ix, iy); /* Call the name of subckt */ 
  } 

  return;
}

/* Call defined channels. 
 * Ensure the port name here is co-herent to other sub-circuits(SB,CB,grid)!!!
 */
void fprint_call_defined_one_channel(FILE* fp,
                                     t_rr_type chan_type,
                                     int x, int y,
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
  
  /* Call the define sub-circuit */
  fprintf(fp, "X%s[%d][%d] ", 
          convert_chan_type_to_string(chan_type),
          x, y);
  fprintf(fp, "\n");
  /* LEFT/BOTTOM side port of CHANX/CHANY */
  /* We apply an opposite port naming rule than function: fprint_routing_chan_subckt 
   * In top-level netlists, we follow the same port name as switch blocks and connection blocks 
   * When a track is in INC_DIRECTION, the LEFT/BOTTOM port would be an output of a switch block
   * When a track is in DEC_DIRECTION, the LEFT/BOTTOM port would be an input of a switch block
   */
  for (itrack = 0; itrack < chan_width; itrack++) {
    fprintf(fp, "+ ");
    switch (chan_rr_nodes[itrack]->direction) {
    case INC_DIRECTION:
      fprintf(fp, "%s[%d][%d]_out[%d] ", 
              convert_chan_type_to_string(chan_type),
              x, y, itrack);
      fprintf(fp, "\n");
      break;
    case DEC_DIRECTION:
      fprintf(fp, "%s[%d][%d]_in[%d] ", 
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
    fprintf(fp, "+ ");
    switch (chan_rr_nodes[itrack]->direction) {
    case INC_DIRECTION:
      fprintf(fp, "%s[%d][%d]_in[%d] ", 
              convert_chan_type_to_string(chan_type),
              x, y, itrack);
      fprintf(fp, "\n");
      break;
    case DEC_DIRECTION:
      fprintf(fp, "%s[%d][%d]_out[%d] ", 
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
    fprintf(fp, "+ ");
    fprintf(fp, "%s[%d][%d]_midout[%d] ", 
            convert_chan_type_to_string(chan_type),
            x, y, itrack);
    fprintf(fp, "\n");
  }
  fprintf(fp, "+ gvdd 0 %s[%d][%d]\n", 
          convert_chan_type_to_string(chan_type),
          x, y);

  /* Free */
  my_free(chan_rr_nodes);

  return;
}

/* Call the sub-circuits for channels : Channel X and Channel Y*/
void fprint_call_defined_channels(FILE* fp,
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
      fprint_call_defined_one_channel(fp, CHANX, ix, iy, 
                                      LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
    }
  }

  /* Channel Y */
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      fprint_call_defined_one_channel(fp, CHANY, ix, iy,
                                      LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);
    }
  }

  return;
}

/* Call the defined sub-circuit of connection box
 * TODO: actually most of this function is copied from
 * spice_routing.c : fprint_conneciton_box_interc
 * Should be more clever to use the original function
 */
void fprint_call_defined_one_connection_box(FILE* fp,
                                            t_cb cur_cb_info) {
  int itrack, inode, side;
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
  
  /* Print the definition of subckt*/
  /* Identify the type of connection box */
  switch(cur_cb_info.type) {
  case CHANX:
    fprintf(fp, "Xcbx[%d][%d] ", cur_cb_info.x, cur_cb_info.y);
    break;
  case CHANY:
    fprintf(fp, "Xcby[%d][%d] ", cur_cb_info.x, cur_cb_info.y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }
 
  fprintf(fp, "\n");

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
    for (itrack = 0; itrack < cur_cb_info.chan_width[side]; itrack++) {
      fprintf(fp, "+ ");
      fprintf(fp, "%s[%d][%d]_midout[%d] ", 
              convert_chan_type_to_string(cur_cb_info.type),
              cur_cb_info.x, cur_cb_info.y, itrack);
      fprintf(fp, "\n");
    }
  }
  /*check side_cnt */
  assert(1 == side_cnt);

  side_cnt = 0;
  /* Print the ports of grids*/
  /* only check ipin_rr_nodes of cur_cb_info */
  for (side = 0; side < cur_cb_info.num_sides; side++) {
    /* Bypass side with zero IPINs*/
    if (0 == cur_cb_info.num_ipin_rr_nodes[side]) {
      continue;
    }
    side_cnt++;
    assert(0 < cur_cb_info.num_ipin_rr_nodes[side]);
    assert(NULL != cur_cb_info.ipin_rr_node[side]);
    for (inode = 0; inode < cur_cb_info.num_ipin_rr_nodes[side]; inode++) {
      fprintf(fp, "+ ");
      /* Print each INPUT Pins of a grid */
      fprint_grid_side_pin_with_given_index(fp, cur_cb_info.ipin_rr_node[side][inode]->ptc_num,
                                            cur_cb_info.ipin_rr_node_grid_side[side][inode],
                                            cur_cb_info.ipin_rr_node[side][inode]->xlow,
                                            cur_cb_info.ipin_rr_node[side][inode]->ylow); 
      fprintf(fp, "\n");
    }
  }
  /* Make sure only 2 sides of IPINs are printed */
  assert((1 == side_cnt)||(2 == side_cnt));
  
  fprintf(fp, "+ ");
  /* Identify the type of connection box */
  switch(cur_cb_info.type) {
  case CHANX:
    /* Need split vdd port for each Connection Box */
    fprintf(fp, "gvdd_cbx[%d][%d] 0 ", cur_cb_info.x, cur_cb_info.y);
    fprintf(fp, "cbx[%d][%d]\n", cur_cb_info.x, cur_cb_info.y);
    break;
  case CHANY:
    /* Need split vdd port for each Connection Box */
    fprintf(fp, "gvdd_cby[%d][%d] 0 ", cur_cb_info.x, cur_cb_info.y);
    fprintf(fp, "cby[%d][%d]\n", cur_cb_info.x, cur_cb_info.y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }
   
  /* Free */
 
  return;
}

/* Call the sub-circuits for connection boxes */
void fprint_call_defined_connection_boxes(FILE* fp) {
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
        fprint_call_defined_one_connection_box(fp, cbx_info[ix][iy]);
      }
    }
  }
  /* Y - channels [1...ny][0..nx]*/
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 1; iy < (ny + 1); iy++) {
      if ((TRUE == is_cb_exist(CHANY, ix, iy))
         &&(0 < count_cb_info_num_ipin_rr_nodes(cby_info[ix][iy]))) {
        fprint_call_defined_one_connection_box(fp, cby_info[ix][iy]);
      }
    }
  }
 
  return; 
}

/* Call the defined switch box sub-circuit
 * Critical difference between this function and 
 * spice_routing.c : fprint_routing_switch_box_subckt
 * Whether a channel node of a Switch block should be input or output depends on it location:
 * For example, a channel chanX INC_DIRECTION on the right side of a SB, it is marked as an input  
 * In fprint_routing_switch_box_subckt: it is marked as an output.
 * For channels chanY with INC_DIRECTION on the top/bottom side, they should be marked as inputs
 * For channels chanY with DEC_DIRECTION on the top/bottom side, they should be marked as outputs
 * For channels chanX with INC_DIRECTION on the left/right side, they should be marked as inputs
 * For channels chanX with DEC_DIRECTION on the left/right side, they should be marked as outputs
 */
void fprint_call_defined_one_switch_box(FILE* fp,
                                        t_sb cur_sb_info) {
  int ix, iy, side, itrack, inode;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cur_sb_info.x))&&(!(cur_sb_info.x > (nx + 1)))); 
  assert((!(0 > cur_sb_info.y))&&(!(cur_sb_info.y > (ny + 1)))); 

  fprintf(fp, "Xsb[%d][%d] ", cur_sb_info.x, cur_sb_info.y);
  fprintf(fp, "\n");

  for (side = 0; side < cur_sb_info.num_sides; side++) {
    determine_sb_port_coordinator(cur_sb_info, side, &ix, &iy); 

    fprintf(fp, "+ ");
    for (itrack = 0; itrack < cur_sb_info.chan_width[side]; itrack++) {
      switch (cur_sb_info.chan_rr_node_direction[side][itrack]) {
      case OUT_PORT:
        fprintf(fp, "%s[%d][%d]_out[%d] ", 
                convert_chan_type_to_string(cur_sb_info.chan_rr_node[side][itrack]->type), 
                ix, iy, itrack); 
        break;
      case IN_PORT:
        fprintf(fp, "%s[%d][%d]_in[%d] ",
                convert_chan_type_to_string(cur_sb_info.chan_rr_node[side][itrack]->type), 
                ix, iy, itrack); 
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of sb[%d][%d] side[%d] track[%d]!\n",
                   __FILE__, __LINE__, cur_sb_info.x, cur_sb_info.y, side, itrack);
        exit(1);
      }
    }
    fprintf(fp, "\n");
    fprintf(fp, "+ ");
    /* Dump OPINs of adjacent CLBs */
    for (inode = 0; inode < cur_sb_info.num_opin_rr_nodes[side]; inode++) {
      fprint_grid_side_pin_with_given_index(fp, cur_sb_info.opin_rr_node[side][inode]->ptc_num,
                                            cur_sb_info.opin_rr_node_grid_side[side][inode],
                                            cur_sb_info.opin_rr_node[side][inode]->xlow,
                                            cur_sb_info.opin_rr_node[side][inode]->ylow); 
    } 
    fprintf(fp, "\n");
  }


  /* Connect to separate vdd port for each switch box??? */
  fprintf(fp, "+ ");
  fprintf(fp, " gvdd_sb[%d][%d] 0 sb[%d][%d]\n", 
          cur_sb_info.x, cur_sb_info.y, cur_sb_info.x, cur_sb_info.y);

  /* Free */

  return;
}

void fprint_call_defined_switch_boxes(FILE* fp) {
  int ix, iy;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 0; iy < (ny + 1); iy++) {
      fprint_call_defined_one_switch_box(fp, sb_info[ix][iy]);
    }
  }

  return;
}

/* Print stimulations for floating ports in Grid
 * Some ports of CLB or I/O Pads is floating.
 * There are two cases : 
 * 1. Their corresponding rr_node (SOURE or OPIN) has 0 fan-out.
 * 2. Their corresponding rr_node (SINK or IPIN) has 0 fan-in.
 * In these cases, we short connect them to global GND.
 */
void fprint_grid_float_port_stimulation(FILE* fp) {
  int inode;
  int num_float_port = 0;
  int port_x, port_y, port_height;
  int side, class_id, pin_index, pin_written_times;
  t_type_ptr type = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Search all rr_nodes */
  for (inode = 0; inode < num_rr_nodes; inode++) {
    switch (rr_node[inode].type) {
    case SOURCE:
    case OPIN:
      /*  Make sure 0 fan-in, 1 fan-in is connected to SOURCE */
      assert((0 == rr_node[inode].fan_in)||(1 == rr_node[inode].fan_in));
      if (1 == rr_node[inode].fan_in) {
        assert(SOURCE == rr_node[rr_node[inode].prev_node].type);
      }
      /* Check if there is 0 fan-out */
      if (0 == rr_node[inode].num_edges) {
        port_x = rr_node[inode].xlow;
        port_y = rr_node[inode].ylow;
        port_height = grid[port_x][port_y].offset;
        port_y = port_y + port_height;
        type = grid[port_x][port_y].type;
        assert(NULL != type);
        /* Get pin information */
        pin_index = rr_node[inode].ptc_num;
        class_id = type->pin_class[pin_index];
        assert(DRIVER == type->class_inf[class_id].type);
        pin_written_times = 0;
        for (side = 0; side < 4; side++) {
          /* Special Care for I/O pad */
          if (IO_TYPE == type) { 
            side = determine_io_grid_side(port_x, port_y);
          }
          if (1 == type->pinloc[port_height][side][pin_index]) { 
            fprintf(fp, "Vfloat_port_%d grid[%d][%d]_pin[%d][%d][%d] 0 0\n",
                    num_float_port, port_x, port_y, port_height, side, pin_index);
            pin_written_times++;
            num_float_port++;
          }
          /* Special Care for I/O pad */
          if (IO_TYPE == type) { 
            break;
          }
        }
        assert(1 == pin_written_times);
      }
      break; 
    case SINK:
    case IPIN:
      /*  Make sure 0 fan-out, 1 fan-out is connected to SINK */
      assert((0 == rr_node[inode].num_edges)||(1 == rr_node[inode].num_edges));
      if (1 == rr_node[inode].num_edges) {
        assert(SINK == rr_node[rr_node[inode].edges[0]].type);
      }
      /* Check if there is 0 fan-out */
      if (0 == rr_node[inode].fan_in) {
        port_x = rr_node[inode].xlow;
        port_y = rr_node[inode].ylow;
        port_height = grid[port_x][port_y].offset;
        port_y = port_y + port_height;
        type = grid[port_x][port_y + port_height].type;
        assert(NULL != type);
        /* Get pin information */
        pin_index = rr_node[inode].ptc_num;
        class_id = type->pin_class[pin_index];
        assert(RECEIVER == type->class_inf[class_id].type);
        pin_written_times = 0;
        for (side = 0; side < 4; side++) {
          /* Special Care for I/O pad */
          if (IO_TYPE == type) { 
            side = determine_io_grid_side(port_x, port_y);
          }
          if (1 == type->pinloc[port_height][side][pin_index]) { 
            fprintf(fp, "Vfloat_port_%d grid[%d][%d]_pin[%d][%d][%d] 0 0\n",
                    num_float_port, port_x, port_y, port_height, side, pin_index);
            pin_written_times++;
            num_float_port++;
          }
          /* Special Care for I/O pad */
          if (IO_TYPE == type) { 
            break;
          }
        }
        assert(1 == pin_written_times);
      }
      break;
    case CHANX:
    case CHANY:
      /*TODO: check 0 fan-in, fan-out channel*/
    case INTRA_CLUSTER_EDGE:
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid rr_node type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }

  vpr_printf(TIO_MESSAGE_INFO, "Connect %d floating grid pin to global gnd.\n", num_float_port);

  return;
}

void fprint_one_design_param_w_wo_variation(FILE* fp,
                                            char* param_name,
                                            float avg_val,
                                            t_spice_mc_variation_params variation_params) {
  /* Check */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d]) FileHandle is NULL!\n",
               __FILE__,__LINE__); 
    exit(1);
  } 

  fprintf(fp,".param %s=", param_name); 
  if (FALSE == variation_params.variation_on) {
    fprintf(fp, "%g", avg_val);
  /* We do not allow any negative value exist in the variation,
   * This could be too tight, could be removed   
   */
  } else if (TRUE == check_negative_variation(avg_val, variation_params)) {
    fprintf(fp, "%g", avg_val);
  } else {
    fprintf(fp, "agauss(%g, '%g*%g', %d)", 
            avg_val, 
            variation_params.abs_variation, avg_val,
            variation_params.num_sigma);
  }
  fprintf(fp, "\n");

  return;
}
 

/* Print Technology Library and Design Parameters*/
void fprint_tech_lib(FILE* fp,
                     t_spice_mc_variation_params cmos_variation_params,
                     t_spice_tech_lib tech_lib) {
  /* Standard transistors*/
  t_spice_transistor_type* nmos_trans = NULL;
  t_spice_transistor_type* pmos_trans = NULL;

  /* I/O transistors*/
  t_spice_transistor_type* io_nmos_trans = NULL;
  t_spice_transistor_type* io_pmos_trans = NULL;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d]) FileHandle is NULL!\n",
               __FILE__,__LINE__); 
    exit(1);
  } 
  /* Include Technology Library*/
  fprintf(fp, "****** Include Technology Library ******\n");
  if (SPICE_LIB_INDUSTRY == tech_lib.type) {
    fprintf(fp, ".lib \'%s\' %s\n", tech_lib.path, tech_lib.transistor_type);
  } else {
    fprintf(fp, ".include \'%s\'\n", tech_lib.path);
  }

  /* Print Transistor parameters*/ 
  /* Define the basic transistor parameters: nl, pl, wn, wp, pn_ratio*/
  fprintf(fp, "****** Transistor Parameters ******\n");
  fprintf(fp,".param beta=%g\n",tech_lib.pn_ratio);
  /* Make sure we have only 2 transistor*/
  assert((2 == tech_lib.num_transistor_type)||(4 == tech_lib.num_transistor_type));
  /* Find NMOS*/
  nmos_trans = find_mosfet_tech_lib(tech_lib,SPICE_TRANS_NMOS);
  if (NULL == nmos_trans) {
    vpr_printf(TIO_MESSAGE_ERROR,"NMOS transistor is not defined in architecture XML!\n");
    exit(1);
  }

  fprint_one_design_param_w_wo_variation(fp, "nl", nmos_trans->chan_length, cmos_variation_params);
  fprint_one_design_param_w_wo_variation(fp, "wn", nmos_trans->min_width, cmos_variation_params);

  /* Find PMOS*/
  pmos_trans = find_mosfet_tech_lib(tech_lib,SPICE_TRANS_PMOS);
  if (NULL == pmos_trans) {
    vpr_printf(TIO_MESSAGE_ERROR,"PMOS transistor is not defined in architecture XML!\n");
    exit(1);
  }

  fprint_one_design_param_w_wo_variation(fp, "pl", pmos_trans->chan_length, cmos_variation_params);
  fprint_one_design_param_w_wo_variation(fp, "wp", pmos_trans->min_width, cmos_variation_params);

  /* Print I/O NMOS and PMOS */
  io_nmos_trans = find_mosfet_tech_lib(tech_lib, SPICE_TRANS_IO_NMOS);
  if ((NULL == io_nmos_trans) && (4 == tech_lib.num_transistor_type)) {
    vpr_printf(TIO_MESSAGE_WARNING,"I/O NMOS transistor is not defined in architecture XML!\n");
    exit(1);
  }
  if (NULL != io_nmos_trans) {
    fprint_one_design_param_w_wo_variation(fp, "io_nl", io_nmos_trans->chan_length, cmos_variation_params);
    fprint_one_design_param_w_wo_variation(fp, "io_wn", io_nmos_trans->min_width, cmos_variation_params);
  }

  io_pmos_trans = find_mosfet_tech_lib(tech_lib,SPICE_TRANS_IO_PMOS);
  if ((NULL == io_pmos_trans) && (4 == tech_lib.num_transistor_type)) {
    vpr_printf(TIO_MESSAGE_WARNING,"I/O PMOS transistor is not defined in architecture XML!\n");
    exit(1);
  }
  if (NULL != io_nmos_trans) {
    fprint_one_design_param_w_wo_variation(fp, "io_pl", io_pmos_trans->chan_length, cmos_variation_params);
    fprint_one_design_param_w_wo_variation(fp, "io_wp", io_pmos_trans->min_width, cmos_variation_params);
  }

  /* Print nominal Vdd */
  fprintf(fp, ".param vsp=%g\n", tech_lib.nominal_vdd);
  /* Print I/O VDD */
  fprintf(fp, ".param io_vsp=%g\n", tech_lib.io_vdd);
  
  return;
}

/* Print all the circuit design parameters */
void fprint_spice_circuit_param(FILE* fp,
                                t_spice_mc_params mc_params,
                                int num_spice_models,
                                t_spice_model* spice_model) {
  int imodel;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!",
               __FILE__, __LINE__); 
    exit(1);
  } 

  fprintf(fp, "***** Parameters for Circuits *****\n");
  for (imodel = 0; imodel < num_spice_models; imodel++) {
     fprintf(fp, "***** Parameters for SPICE MODEL: %s *****\n",
             spice_model[imodel].name);
     /* Regular design parameters: input buf sizes, output buf sizes*/ 
     if ((NULL != spice_model[imodel].input_buffer) 
        &&(TRUE == spice_model[imodel].input_buffer->exist)) {
       fprint_one_design_param_w_wo_variation(fp,
                                              my_strcat(spice_model[imodel].name, design_param_postfix_input_buf_size), 
                                              spice_model[imodel].input_buffer->size,
                                              mc_params.cmos_variation); 
     }

     if ((NULL != spice_model[imodel].output_buffer) 
        &&(TRUE == spice_model[imodel].output_buffer->exist)) {
       fprint_one_design_param_w_wo_variation(fp,
                                              my_strcat(spice_model[imodel].name, design_param_postfix_output_buf_size), 
                                              spice_model[imodel].output_buffer->size,
                                              mc_params.cmos_variation); 
     }

     if (NULL != spice_model[imodel].pass_gate_logic) {
       fprint_one_design_param_w_wo_variation(fp,
                                              my_strcat(spice_model[imodel].name, design_param_postfix_pass_gate_logic_pmos_size), 
                                              spice_model[imodel].pass_gate_logic->pmos_size, 
                                              mc_params.cmos_variation); 
       fprint_one_design_param_w_wo_variation(fp,
                                              my_strcat(spice_model[imodel].name, design_param_postfix_pass_gate_logic_nmos_size), 
                                              spice_model[imodel].pass_gate_logic->nmos_size, 
                                              mc_params.cmos_variation); 
     }

     /* Exclusive parameters WIREs */
     if ((SPICE_MODEL_CHAN_WIRE == spice_model[imodel].type)
        ||(SPICE_MODEL_WIRE == spice_model[imodel].type)) {
       fprint_one_design_param_w_wo_variation(fp,
                                              my_strcat(spice_model[imodel].name, design_param_postfix_wire_param_res_val), 
                                              spice_model[imodel].wire_param->res_val,
                                              mc_params.wire_variation); 
       fprint_one_design_param_w_wo_variation(fp,
                                              my_strcat(spice_model[imodel].name, design_param_postfix_wire_param_cap_val), 
                                              spice_model[imodel].wire_param->cap_val,
                                              mc_params.wire_variation); 
     }
       
     /* We care the spice models built with RRAMs */
     if (SPICE_MODEL_DESIGN_RRAM == spice_model[imodel].design_tech) {
       /* Print Ron */
       fprint_one_design_param_w_wo_variation(fp,
                                              my_strcat(spice_model[imodel].name, design_param_postfix_rram_ron), 
                                              spice_model[imodel].design_tech_info.ron,
                                              mc_params.rram_variation); 
       /* Print Roff */
       fprint_one_design_param_w_wo_variation(fp,
                                              my_strcat(spice_model[imodel].name, design_param_postfix_rram_roff), 
                                              spice_model[imodel].design_tech_info.roff,
                                              mc_params.rram_variation); 
       /* Print Wprog_set_nmos */
       fprint_one_design_param_w_wo_variation(fp,
                                              my_strcat(spice_model[imodel].name, design_param_postfix_rram_wprog_set_nmos), 
                                              spice_model[imodel].design_tech_info.wprog_set_nmos,
                                              mc_params.cmos_variation); 
       /* Print Wprog_set_pmos */
       fprint_one_design_param_w_wo_variation(fp,
                                              my_strcat(spice_model[imodel].name, design_param_postfix_rram_wprog_set_pmos), 
                                              spice_model[imodel].design_tech_info.wprog_set_pmos,
                                              mc_params.cmos_variation); 
       /* Print Wprog_reset_nmos */
       fprint_one_design_param_w_wo_variation(fp,
                                              my_strcat(spice_model[imodel].name, design_param_postfix_rram_wprog_reset_nmos), 
                                              spice_model[imodel].design_tech_info.wprog_reset_nmos,
                                              mc_params.cmos_variation); 
       /* Print Wprog_reset_pmos */
       fprint_one_design_param_w_wo_variation(fp,
                                              my_strcat(spice_model[imodel].name, design_param_postfix_rram_wprog_reset_pmos), 
                                              spice_model[imodel].design_tech_info.wprog_reset_pmos,
                                              mc_params.cmos_variation); 
     } 
  }

  return;
}

void fprint_spice_netlist_measurement_one_design_param(FILE* fp,
                                                       char* param_name) {
  
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!",
               __FILE__, __LINE__); 
    exit(1);
  } 

  fprintf(fp, ".meas tran actual_%s param='%s'\n",
          param_name, param_name);

  return;
}

void fprint_spice_netlist_generic_measurements(FILE* fp, 
                                               t_spice_mc_params mc_params,
                                               int num_spice_models,
                                               t_spice_model* spice_model) {
  
  int imodel;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!",
               __FILE__, __LINE__); 
    exit(1);
  } 

  fprintf(fp, "***** Generic Measurements for Circuit Parameters *****\n");

  if ((FALSE == mc_params.cmos_variation.variation_on)
     &&(FALSE == mc_params.rram_variation.variation_on)) {
    return;
  }

  for (imodel = 0; imodel < num_spice_models; imodel++) {
     fprintf(fp, "***** Measurements for Parameters for SPICE MODEL: %s *****\n",
             spice_model[imodel].name);
     /* Regular design parameters: input buf sizes, output buf sizes*/ 
     if ((NULL != spice_model[imodel].input_buffer) 
        &&(TRUE == spice_model[imodel].input_buffer->exist)) {
       fprint_spice_netlist_measurement_one_design_param(fp,
                                                         my_strcat(spice_model[imodel].name, design_param_postfix_input_buf_size)); 
     }

     if ((NULL != spice_model[imodel].output_buffer) 
        &&(TRUE == spice_model[imodel].output_buffer->exist)) {
       fprint_spice_netlist_measurement_one_design_param(fp,
                                                         my_strcat(spice_model[imodel].name, design_param_postfix_output_buf_size)); 
     }

     if (NULL != spice_model[imodel].pass_gate_logic) {
       fprint_spice_netlist_measurement_one_design_param(fp,
                                                         my_strcat(spice_model[imodel].name, design_param_postfix_pass_gate_logic_pmos_size)); 
       fprint_spice_netlist_measurement_one_design_param(fp,
                                                         my_strcat(spice_model[imodel].name, design_param_postfix_pass_gate_logic_nmos_size)); 
     }

     /* Exclusive parameters WIREs */
     if ((SPICE_MODEL_CHAN_WIRE == spice_model[imodel].type)
        ||(SPICE_MODEL_WIRE == spice_model[imodel].type)) {
       fprint_spice_netlist_measurement_one_design_param(fp,
                                                         my_strcat(spice_model[imodel].name, design_param_postfix_wire_param_res_val)); 
       fprint_spice_netlist_measurement_one_design_param(fp,
                                                         my_strcat(spice_model[imodel].name, design_param_postfix_wire_param_cap_val)); 
     }
       
     /* We care the spice models built with RRAMs */
     if (SPICE_MODEL_DESIGN_RRAM == spice_model[imodel].design_tech) {
       /* Print Ron */
       fprint_spice_netlist_measurement_one_design_param(fp,
                                                         my_strcat(spice_model[imodel].name, design_param_postfix_rram_ron)); 
       /* Print Roff */
       fprint_spice_netlist_measurement_one_design_param(fp,
                                                         my_strcat(spice_model[imodel].name, design_param_postfix_rram_roff)); 
       /* Print Wprog_set_nmos */
       fprint_spice_netlist_measurement_one_design_param(fp,
                                                         my_strcat(spice_model[imodel].name, design_param_postfix_rram_wprog_set_nmos)); 
       /* Print Wprog_set_pmos */
       fprint_spice_netlist_measurement_one_design_param(fp,
                                                         my_strcat(spice_model[imodel].name, design_param_postfix_rram_wprog_set_pmos)); 
       /* Print Wprog_reset_nmos */
       fprint_spice_netlist_measurement_one_design_param(fp,
                                                         my_strcat(spice_model[imodel].name, design_param_postfix_rram_wprog_reset_nmos)); 
       /* Print Wprog_reset_pmos */
       fprint_spice_netlist_measurement_one_design_param(fp,
                                                         my_strcat(spice_model[imodel].name, design_param_postfix_rram_wprog_reset_pmos)); 
     } 
  }


  return;
}

/* This function may expand. 
 * It prints temperature, and options for a SPICE simulation
 */
void fprint_spice_options(FILE* fp,
                          t_spice_params spice_params) {

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!",__FILE__, __LINE__); 
    exit(1);
  } 
  
  /* Temperature */
  fprintf(fp, ".temp %d\n", spice_params.sim_temp);

  /* Options: print capacitances of all nodes */
  if (TRUE == spice_params.captab) {
    fprintf(fp, ".option captab\n");
  }
  /* Add post could make SPICE very slow for large benchmarks!!! Be careful*/
  if (TRUE == spice_params.post) {
    fprintf(fp, ".option post\n");
  }
  /* Use fast */
  if (TRUE == spice_params.fast) {
    fprintf(fp, ".option fast\n");
  }

  return;
}

/* This function may expand. 
 * It prints include paramters for SPICE netlists
 */
void fprint_spice_include_param_headers(FILE* fp,
                                        char* include_dir_path) {
  char* temp_include_file_path = NULL;
  char* formatted_include_dir_path = format_dir_path(include_dir_path);

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Include headers for circuit designs, measurements and stimulates */

  fprintf(fp, "****** Include Header file: circuit design parameters *****\n");
  temp_include_file_path = my_strcat(formatted_include_dir_path, design_param_header_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "****** Include Header file: measurement parameters *****\n");
  temp_include_file_path = my_strcat(formatted_include_dir_path, meas_header_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "****** Include Header file: stimulation parameters *****\n");
  temp_include_file_path = my_strcat(formatted_include_dir_path, stimu_header_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);
  
  return;
}


/* This function may expand. 
 * It prints include sub-circuit SPICE netlists
 */
void fprint_spice_include_key_subckts(FILE* fp,
                                      char* subckt_dir_path) {
  char* temp_include_file_path = NULL;
  char* formatted_subckt_dir_path = format_dir_path(subckt_dir_path);

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Include necessary sub-circuits */
  fprintf(fp, "****** Include subckt netlists: NMOS and PMOS *****\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, nmos_pmos_spice_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  if (1 == rram_design_tech) {
    fprintf(fp, "****** Include subckt netlists: RRAM behavior VerilogA model *****\n");
    /* This is a HSPICE Bug! When the verilogA file contain a dir_path, the sim results become weired! */
    /*
    temp_include_file_path = my_strcat(formatted_subckt_dir_path, rram_veriloga_file_name);
    fprintf(fp, ".hdl \'%s\'\n", temp_include_file_path);
    my_free(temp_include_file_path);
    */
    fprintf(fp, ".hdl \'%s\'\n", rram_veriloga_file_name);
  }

  fprintf(fp, "****** Include subckt netlists: Inverters, Buffers *****\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, basics_spice_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "****** Include subckt netlists: Multiplexers *****\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, muxes_spice_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  fprintf(fp, "****** Include subckt netlists: Wires *****\n");
  temp_include_file_path = my_strcat(formatted_subckt_dir_path, wires_spice_file_name);
  fprintf(fp, ".include \'%s\'\n", temp_include_file_path);
  my_free(temp_include_file_path);

  return;
}

void fprint_voltage_pulse_params(FILE* fp,
                                 int init_val,
                                 float density,
                                 float probability) {

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* TODO: check codes for density and probability, init_val */
  /* If density = 0, this is a constant signal */
  if (0. == density) {
    if (0. == probability) {
      fprintf(fp, "+  0\n");
    } else {
      fprintf(fp, "+  vsp\n");
    }
    return;
  }

  if (0 == init_val) {
    fprintf(fp, "+  pulse(0 vsp 'clock_period' \n");
  } else {
    fprintf(fp, "+  pulse(vsp 0 'clock_period' \n");
  }
  /*
  fprintf(fp, "+  'input_slew_pct_rise*%g*clock_period' 'input_slew_pct_fall*%g*clock_period'\n",
          2./density, 2./density);
  */
  /* TODO: Think about a reasonable slew for signals with diverse density */
  fprintf(fp, "+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'\n");
  fprintf(fp, "+  '%g*%g*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '%g*clock_period')\n",
          probability, 2./density, 2./density);

  return;
}


void fprint_spice_netlist_transient_setting(FILE* fp, 
                                            t_spice spice, 
                                            int num_sim_clock_cycles,
                                            boolean leakage_only) {
  int num_clock_cycle = spice.spice_params.meas_params.sim_num_clock_cycle + 1;
 
  /* Overwrite the sim if auto is turned on */
  if ((TRUE == spice.spice_params.meas_params.auto_select_sim_num_clk_cycle)
    &&(num_sim_clock_cycles < num_clock_cycle)) {
    num_clock_cycle = num_sim_clock_cycles;
  }
  /*
  if (TRUE == spice.spice_params.meas_params.auto_select_sim_num_clk_cycle) {
    assert(!(num_sim_clock_cycles > num_clock_cycle));
  }
  */

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!",__FILE__, __LINE__); 
    exit(1);
  } 

  /* Leakage power only, use a simplified tran sim*/
  if (TRUE == leakage_only) {
    fprintf(fp, "***** Transient simulation only for leakage power  *****\n");
  } else {
    fprintf(fp, "***** %d Clock Simulation, accuracy=%g *****\n",
            num_clock_cycle, spice.spice_params.meas_params.accuracy);
  }

  /* Determine the transistion time to simulate */
  switch (spice.spice_params.meas_params.accuracy_type) {
  case SPICE_ABS: 
    fprintf(fp, ".tran %g ", 
            spice.spice_params.meas_params.accuracy);
    break;
   case SPICE_FRAC:
    fprintf(fp, ".tran '%d*clock_period/%d' ", 
            num_clock_cycle, (int)spice.spice_params.meas_params.accuracy);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid accuracy type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  if (TRUE == leakage_only) {
    fprintf(fp, " 'clock_period'");
  } else {
    fprintf(fp, " '%d*clock_period'", 
            num_clock_cycle);
  }

  if ((TRUE == spice.spice_params.mc_params.cmos_variation.variation_on)
     ||(TRUE == spice.spice_params.mc_params.rram_variation.variation_on)
     ||(TRUE == spice.spice_params.mc_params.mc_sim)) {
    fprintf(fp, " sweep monte=%d ",
            spice.spice_params.mc_params.num_mc_points);
  }

  fprintf(fp, "\n");

  return;
}

void fprint_stimulate_dangling_one_grid_pin(FILE* fp,
                                            int x, int y,
                                            int height, int side, int pin_index,
                                            t_ivec*** LL_rr_node_indices) {
  t_type_ptr type_descriptor = grid[x][y].type;
  int capacity = grid[x][y].type->capacity;
  int class_id;
  int rr_node_index;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  assert(NULL != type_descriptor);
  assert(0 < capacity);

  class_id = type_descriptor->pin_class[pin_index];
  if (DRIVER == type_descriptor->class_inf[class_id].type) {
    rr_node_index = get_rr_node_index(x, y, OPIN, pin_index, LL_rr_node_indices); 
    /* Zero fan-out OPIN */
    if (0 == rr_node[rr_node_index].num_edges) {
      fprintf(fp, "Rdangling_grid[%d][%d]_pin[%d][%d][%d] grid[%d][%d]_pin[%d][%d][%d] 0 1e9\n",
              x, y, height, side, pin_index,
              x, y, height, side, pin_index);
      fprintf(fp, "*.nodeset V(grid[%d][%d]_pin[%d][%d][%d]) 0 \n",
              x, y, height, side, pin_index);
    }
    return;
  }
  if (RECEIVER == type_descriptor->class_inf[class_id].type) {
    rr_node_index = get_rr_node_index(x, y, IPIN, pin_index, LL_rr_node_indices); 
    /* Zero fan-in IPIN */
    if (0 == rr_node[rr_node_index].fan_in) {
      fprintf(fp, "Rdangling_grid[%d][%d]_pin[%d][%d][%d] grid[%d][%d]_pin[%d][%d][%d] 0 0\n",
              x, y, height, side, pin_index,
              x, y, height, side, pin_index);
      fprintf(fp, ".nodeset V(grid[%d][%d]_pin[%d][%d][%d]) 0\n",
              x, y, height, side, pin_index);
    }
    return;
  }

  return;
}

void fprint_stimulate_dangling_io_grid_pins(FILE* fp,
                                            int x, int y) {
  int iheight, side, ipin; 
  t_type_ptr type_descriptor = grid[x][y].type;
  int capacity = grid[x][y].type->capacity;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  assert(NULL != type_descriptor);
  assert(0 < capacity);

  /* identify the location of IO grid and 
   * decide which side of ports we need
   */
  side = determine_io_grid_side(x,y);

  /* Count the number of pins */
  //for (iz = 0; iz < capacity; iz++) {
  for (iheight = 0; iheight < type_descriptor->height; iheight++) {
    for (ipin = 0; ipin < type_descriptor->num_pins; ipin++) {
      if (1 == type_descriptor->pinloc[iheight][side][ipin]) {
        fprint_stimulate_dangling_one_grid_pin(fp, x, y, iheight, side, ipin, rr_node_indices);
      }
    }
  }  
  //}

  return;
}

void fprint_stimulate_dangling_normal_grid_pins(FILE* fp,
                                                int x, int y) {
  int iheight, side, ipin; 
  t_type_ptr type_descriptor = grid[x][y].type;
  int capacity = grid[x][y].type->capacity;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  assert(NULL != type_descriptor);
  assert(0 < capacity);

  for (side = 0; side < 4; side++) {
    /* Count the number of pins */
    for (iheight = 0; iheight < type_descriptor->height; iheight++) {
      for (ipin = 0; ipin < type_descriptor->num_pins; ipin++) {
        if (1 == type_descriptor->pinloc[iheight][side][ipin]) {
          fprint_stimulate_dangling_one_grid_pin(fp, x, y, iheight, side, ipin, rr_node_indices);
        }
      }
    }  
  }

  return;

}

void fprint_stimulate_dangling_grid_pins(FILE* fp) {
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
      /* zero-fan-in CLB IPIN*/
      fprint_stimulate_dangling_normal_grid_pins(fp, ix, iy);
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
    fprint_stimulate_dangling_io_grid_pins(fp, ix, iy);
  }

  /* RIGHT side */
  ix = nx + 1;
  for (iy = 1; iy < (ny + 1); iy++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    fprint_stimulate_dangling_io_grid_pins(fp, ix, iy);
  }

  /* BOTTOM side */
  iy = 0;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    fprint_stimulate_dangling_io_grid_pins(fp, ix, iy);
  } 

  /* TOP side */
  iy = ny + 1;
  for (ix = 1; ix < (nx + 1); ix++) {
    /* Bypass EMPTY grid */
    if (EMPTY_TYPE == grid[ix][iy].type) {
      continue;
    }
    assert(IO_TYPE == grid[ix][iy].type);
    fprint_stimulate_dangling_io_grid_pins(fp, ix, iy);
  } 

  return;
}

void init_logical_block_spice_model_temp_used(t_spice_model* spice_model) {
  int i;

  /* For each logical block, we print a vdd */
  for (i = 0; i < num_logical_blocks; i++) {
    if (logical_block[i].mapped_spice_model == spice_model) {
      logical_block[i].temp_used = 0;
    }
  }

  return;  
}

void init_logical_block_spice_model_type_temp_used(int num_spice_models, t_spice_model* spice_model,
                                                   enum e_spice_model_type spice_model_type) {
  int i;

  /* For each logical block, we print a vdd */
  for (i = 0; i < num_spice_models; i++) {
    if (spice_model_type == spice_model[i].type) {
      init_logical_block_spice_model_temp_used(&(spice_model[i]));
    }
  }

  return;  
}

void fprint_global_vdds_logical_block_spice_model(FILE* fp,
                                                  t_spice_model* spice_model) {
  int i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* For each logical block, we print a vdd */
  for (i = 0; i < num_logical_blocks; i++) {
    if ((logical_block[i].mapped_spice_model == spice_model)
       &&(1 == logical_block[i].temp_used)){
      fprintf(fp, ".global gvdd_%s[%d]\n",
              spice_model->prefix, logical_block[i].mapped_spice_model_index);
    } 
  }

  return;
}

void fprint_splited_vdds_logical_block_spice_model(FILE* fp,
                                                   t_spice_model* spice_model) {
  int i;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* For each logical block, we print a vdd */
  for (i = 0; i < num_logical_blocks; i++) {
    if ((logical_block[i].mapped_spice_model == spice_model) 
       &&(1 == logical_block[i].temp_used)){
      fprintf(fp, "Vgvdd_%s[%d] gvdd_%s[%d] 0 vsp\n",
              spice_model->prefix, logical_block[i].mapped_spice_model_index,
              spice_model->prefix, logical_block[i].mapped_spice_model_index);
    } 
  }

  return;
}

void fprint_measure_vdds_logical_block_spice_model(FILE* fp,
                                                   t_spice_model* spice_model,
                                                   enum e_measure_type meas_type,
                                                   int num_clock_cycle,
                                                   boolean leakage_only) {
  int i, iport, ipin, cur;
  float average_output_density = 0.;
  int output_cnt = 0;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* For each logical block, we print a vdd */
  for (i = 0; i < num_logical_blocks; i++) {
    if ((logical_block[i].mapped_spice_model == spice_model) 
       &&(1 == logical_block[i].temp_used)) {
      /* Get the average output density */
      output_cnt = 0;
      for (iport = 0; iport < logical_block[i].pb->pb_graph_node->num_output_ports; iport++) {
        for (ipin = 0; ipin < logical_block[i].pb->pb_graph_node->num_output_pins[iport]; ipin++) {
          average_output_density += vpack_net[logical_block[i].output_nets[iport][ipin]].spice_net_info->density; 
          output_cnt++;
        }
      }
      average_output_density = average_output_density/output_cnt;
      switch (meas_type) {
      case SPICE_MEASURE_LEAKAGE_POWER:
        if (TRUE == leakage_only) {
          fprintf(fp, ".measure tran leakage_power_%s[%d] find p(Vgvdd_%s[%d]) at=0\n",
              spice_model->prefix, logical_block[i].mapped_spice_model_index,
              spice_model->prefix, logical_block[i].mapped_spice_model_index);
        } else {
          fprintf(fp, ".measure tran leakage_power_%s[%d] avg p(Vgvdd_%s[%d]) from=0 to='clock_period'\n",
              spice_model->prefix, logical_block[i].mapped_spice_model_index,
              spice_model->prefix, logical_block[i].mapped_spice_model_index);
        }
        break;
      case SPICE_MEASURE_DYNAMIC_POWER:
        fprintf(fp, ".measure tran dynamic_power_%s[%d] avg p(Vgvdd_%s[%d]) from='clock_period' to='%d*clock_period'\n",
                spice_model->prefix, logical_block[i].mapped_spice_model_index,
                spice_model->prefix, logical_block[i].mapped_spice_model_index,
                num_clock_cycle);
        fprintf(fp, ".measure tran energy_per_cycle_%s[%d] param='dynamic_power_%s[%d]*clock_period'\n",
                spice_model->prefix, logical_block[i].mapped_spice_model_index,
                spice_model->prefix, logical_block[i].mapped_spice_model_index);
        break;
      default: 
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid meas_type!\n", __FILE__, __LINE__);
        exit(1);
      }
    } 
  }

  /* Measure the total power of this kind of spice model */
  cur = 0;
  switch (meas_type) {
  case SPICE_MEASURE_LEAKAGE_POWER:
    for (i = 0; i < num_logical_blocks; i++) {
      if ((logical_block[i].mapped_spice_model == spice_model) 
        &&(1 == logical_block[i].temp_used)) {
        fprintf(fp, ".measure tran leakage_power_%s[0to%d] \n",
              spice_model->prefix, cur);
        if (0 == cur) {
          fprintf(fp, "+ param = 'leakage_power_%s[%d]'\n",
              spice_model->prefix, logical_block[i].mapped_spice_model_index);
        } else {
          fprintf(fp, "+ param = 'leakage_power_%s[%d]+leakage_power_%s[0to%d]'\n", 
              spice_model->prefix, logical_block[i].mapped_spice_model_index,
              spice_model->prefix, cur-1);
        }
        cur++;
      }
    }
    if (0 == cur) {
      break;
    }
    /* Spot the total leakage power of this spice model */
    fprintf(fp, ".measure tran total_leakage_power_%s \n", spice_model->prefix);
    fprintf(fp, "+ param = 'leakage_power_%s[0to%d]'\n", 
       spice_model->prefix, cur-1);
    break;
  case SPICE_MEASURE_DYNAMIC_POWER:
    for (i = 0; i < num_logical_blocks; i++) {
      if ((logical_block[i].mapped_spice_model == spice_model)
        &&(1 == logical_block[i].temp_used)) {
        fprintf(fp, ".measure tran energy_per_cycle_%s[0to%d] \n",
            spice_model->prefix, cur);
        if (0 == cur) {
          fprintf(fp, "+ param = 'energy_per_cycle_%s[%d]'\n",
              spice_model->prefix, logical_block[i].mapped_spice_model_index);
        } else {
          fprintf(fp, "+ param = 'energy_per_cycle_%s[%d]+energy_per_cycle_%s[0to%d]'\n", 
              spice_model->prefix, logical_block[i].mapped_spice_model_index,
             spice_model->prefix, cur-1);
        }
        cur++;
      }
    }
    if (0 == cur) {
      break;
    }
    /* Spot the total dynamic power of this spice model */
    fprintf(fp, ".measure tran total_energy_per_cycle_%s \n", spice_model->prefix);
    fprintf(fp, "+ param = 'energy_per_cycle_%s[0to%d]'\n", 
            spice_model->prefix, cur-1);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid meas_type!\n", __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* Give voltage stimuli of one global port */
void fprint_spice_testbench_wire_one_global_port_stimuli(FILE* fp, 
                                                         t_spice_model_port* cur_global_port, 
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
    fprintf(fp, "Rshortwire%s[%d] %s[%d]  ",
                cur_global_port->prefix, ipin,
                cur_global_port->prefix, ipin);
    assert((0 == cur_global_port->default_val)||(1 == cur_global_port->default_val));
    fprintf(fp, "%s",
                voltage_stimuli_port_name);
    if (1 == cur_global_port->default_val) {
      fprintf(fp, "%s ", 
                  spice_tb_global_port_inv_postfix);
    }
    fprintf(fp, " 0\n");
  }

  return;

}

/* Give voltage stimuli of global ports */
void fprint_spice_testbench_global_ports_stimuli(FILE* fp, 
                                                 t_llist* head) {
  t_llist* temp = head;
  t_spice_model_port* cur_global_port = NULL;
  int ipin;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  fprintf(fp, "***** Connecting Global ports *****\n");
  while(NULL != temp) {
    cur_global_port = (t_spice_model_port*)(temp->dptr); 
    /* Make sure this is a global port */
    assert(TRUE == cur_global_port->is_global);
    /* If this is a clock signal, connect to op_clock signal */
    if (SPICE_MODEL_PORT_CLOCK == cur_global_port->type) {
      /* Special for programming clock */
      if (TRUE == cur_global_port->is_prog) {
        /* We do need to program SRAMs/RRAM in spice netlist
         * Just wire it to a constant GND/VDD 
         */
        fprint_spice_testbench_wire_one_global_port_stimuli(fp, cur_global_port, 
                                                            spice_tb_global_config_done_port_name);
      } else {
        assert(FALSE == cur_global_port->is_prog);
        fprint_spice_testbench_wire_one_global_port_stimuli(fp, cur_global_port, 
                                                                spice_tb_global_clock_port_name);
      }
    /* If this is a config_enable signal, connect to config_done signal */
    } else if (TRUE == cur_global_port->is_config_enable) {
      fprint_spice_testbench_wire_one_global_port_stimuli(fp, cur_global_port, 
                                                              spice_tb_global_config_done_port_name);
    /* If this is a set/reset signal, connect to global reset and set signals */
    } else if (TRUE == cur_global_port->is_reset) {
      /* Special for programming reset */
      if (TRUE == cur_global_port->is_prog) {
        fprint_spice_testbench_wire_one_global_port_stimuli(fp, cur_global_port, 
                                                                spice_tb_global_reset_port_name);
      } else {
        assert(FALSE == cur_global_port->is_prog);
        fprint_spice_testbench_wire_one_global_port_stimuli(fp, cur_global_port, 
                                                                spice_tb_global_reset_port_name);
      }
    /* If this is a set/reset signal, connect to global reset and set signals */
    } else if (TRUE == cur_global_port->is_set) {
      /* Special for programming reset */
      if (TRUE == cur_global_port->is_prog) {
        fprint_spice_testbench_wire_one_global_port_stimuli(fp, cur_global_port, 
                                                                spice_tb_global_set_port_name);
      } else {
        assert(FALSE == cur_global_port->is_prog);
        fprint_spice_testbench_wire_one_global_port_stimuli(fp, cur_global_port, 
                                                                spice_tb_global_set_port_name);
      }
    } else {
    /* Other global signals stuck at the default values */
      for (ipin = 0; ipin < cur_global_port->size; ipin++) {
        fprintf(fp, "R%s[%d] %s[%d] ", 
                    cur_global_port->prefix, ipin, 
                    cur_global_port->prefix, ipin); 
        assert( (0 == cur_global_port->default_val)||(1 == cur_global_port->default_val) );
        if ( 0 == cur_global_port->default_val ) {
          /* Default value is 0: Connect to the global GND port */
          fprintf(fp, " %s",
                  spice_tb_global_gnd_port_name);
        } else {
          /* Default value is 1: Connect to the global VDD port */
          fprintf(fp, " %s",
                  spice_tb_global_vdd_port_name);
        }
       fprintf(fp, " 0\n");
      }
    }
    /* Go to the next */
    temp = temp->next;
  }
  fprintf(fp, "***** End Connecting Global ports *****\n");

  return;
}

void fprint_spice_testbench_global_vdd_port_stimuli(FILE* fp,
                                                    char* global_vdd_port_name,
                                                    char* voltage_level) {

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  
  /* Print a Voltage Stimuli */
  fprintf(fp, "V%s %s 0 %s\n",
             global_vdd_port_name,
             global_vdd_port_name,
             voltage_level);
  return;
} 

void fprint_spice_testbench_global_sram_inport_stimuli(FILE* fp,
                                                       t_sram_orgz_info* cur_sram_orgz_info) {
  t_spice_model* mem_model = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Get memory spice model */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);

  /* Every SRAM inputs should have a voltage source */
  fprintf(fp, "***** Global Inputs for SRAMs *****\n");
  if (SPICE_SRAM_SCAN_CHAIN == cur_sram_orgz_info->type) {
    fprintf(fp, "Vsc_clk sc_clk 0 0\n");
    fprintf(fp, "Vsc_rst sc_rst 0 0\n");
    fprintf(fp, "Vsc_set sc_set 0 0\n");
    fprintf(fp, "V%s[0]->in %s[0]->in 0 0\n", mem_model->prefix, mem_model->prefix);
    fprintf(fp, ".nodeset V(%s[0]->in) 0\n", mem_model->prefix);
  } else {
    fprintf(fp, "V%s->in %s->in 0 0\n", 
            mem_model->prefix, mem_model->prefix);
    fprintf(fp, ".nodeset V(%s->in) 0\n", mem_model->prefix);
  }

  return;
}

void fprint_spice_testbench_generic_global_ports_stimuli(FILE* fp,
                                                         int num_clock) {

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Global GND */
  fprintf(fp, "***** Global VDD port *****\n");
  fprintf(fp, "V%s %s 0 vsp\n",
              spice_tb_global_vdd_port_name,
              spice_tb_global_vdd_port_name);
  fprintf(fp, "***** Global GND port *****\n");
  fprintf(fp, "V%s %s 0 0\n",
              spice_tb_global_gnd_port_name,
              spice_tb_global_gnd_port_name);

  /* Global set and reset */
  fprintf(fp, "***** Global Net for reset signal *****\n");
  fprintf(fp, "V%s %s 0 0\n",
              spice_tb_global_reset_port_name,
              spice_tb_global_reset_port_name);
  fprintf(fp, "V%s%s %s%s 0 vsp\n",
              spice_tb_global_reset_port_name,
              spice_tb_global_port_inv_postfix,
              spice_tb_global_reset_port_name,
              spice_tb_global_port_inv_postfix);
  fprintf(fp, "***** Global Net for set signal *****\n");
  fprintf(fp, "V%s %s 0 0\n",
              spice_tb_global_set_port_name,
              spice_tb_global_set_port_name);
  fprintf(fp, "V%s%s %s%s 0 vsp\n",
              spice_tb_global_set_port_name,
              spice_tb_global_port_inv_postfix,
              spice_tb_global_set_port_name,
              spice_tb_global_port_inv_postfix);

  /* Global config done */
  fprintf(fp, "***** Global Net for configuration done signal *****\n");
  fprintf(fp, "V%s %s 0 0\n",
              spice_tb_global_config_done_port_name,
              spice_tb_global_config_done_port_name);
  fprintf(fp, "V%s%s %s%s 0 vsp\n",
              spice_tb_global_config_done_port_name,
              spice_tb_global_port_inv_postfix,
              spice_tb_global_config_done_port_name,
              spice_tb_global_port_inv_postfix);

  /* Global clock if we need one */
  if (1 == num_clock) {
    /* First cycle reserved for measuring leakage */
    fprintf(fp, "***** Global Clock signal *****\n");
    fprintf(fp, "***** pulse(vlow vhigh tdelay trise tfall pulse_width period *****\n");
    fprintf(fp, "V%s %s 0 pulse(0 vsp 'clock_period'\n", 
                spice_tb_global_clock_port_name,
                spice_tb_global_clock_port_name);
    fprintf(fp, "+                      'clock_slew_pct_rise*clock_period' 'clock_slew_pct_fall*clock_period'\n");
    fprintf(fp, "+                      '0.5*(1-clock_slew_pct_rise-clock_slew_pct_fall)*clock_period' 'clock_period')\n");
    fprintf(fp, "\n");
    fprintf(fp, "***** pulse(vlow vhigh tdelay trise tfall pulse_width period *****\n");
    fprintf(fp, "V%s%s %s%s 0 pulse(0 vsp 'clock_period'\n",
                spice_tb_global_clock_port_name,
                spice_tb_global_port_inv_postfix,
                spice_tb_global_clock_port_name,
                spice_tb_global_port_inv_postfix);
    fprintf(fp, "+                              'clock_slew_pct_rise*clock_period' 'clock_slew_pct_fall*clock_period'\n");
    fprintf(fp, "+                              '0.5*(1-clock_slew_pct_rise-clock_slew_pct_fall)*clock_period' 'clock_period')\n");
  } else {
    assert(0 == num_clock);
    /* Give constant value */
    fprintf(fp, "V%s %s 0 0\n", 
                spice_tb_global_clock_port_name,
                spice_tb_global_clock_port_name);
    fprintf(fp, "V%s%s %s%s 0 vsp\n",
                spice_tb_global_clock_port_name,
                spice_tb_global_port_inv_postfix,
                spice_tb_global_clock_port_name,
                spice_tb_global_port_inv_postfix);
  }

  return;
}

/* Find the inverter size of a Programmable Logic Block Pin
 *
 */
float find_spice_testbench_pb_pin_mux_load_inv_size(t_spice_model* fan_out_spice_model) {
  float load_inv_size = 0;

  /* Check */
  assert(NULL != fan_out_spice_model);
  assert(NULL != fan_out_spice_model->input_buffer);

  /* Special: this is a LUT, we should consider more inv size */
  if (SPICE_MODEL_LUT == fan_out_spice_model->type) {
    assert(1 == fan_out_spice_model->lut_input_buffer->exist);
    assert((SPICE_MODEL_BUF_INV == fan_out_spice_model->lut_input_buffer->type)
          ||(SPICE_MODEL_BUF_BUF == fan_out_spice_model->lut_input_buffer->type));
    assert(TRUE == fan_out_spice_model->lut_input_buffer->tapered_buf);
    assert(2 == fan_out_spice_model->lut_input_buffer->tap_buf_level);
    load_inv_size = fan_out_spice_model->lut_input_buffer->size 
                  + fan_out_spice_model->lut_input_buffer->f_per_stage;
    return load_inv_size;
  }

  /* depend on the input_buffer type */
  if (1 == fan_out_spice_model->input_buffer->exist) {
    switch(fan_out_spice_model->input_buffer->type) {
    case SPICE_MODEL_BUF_INV:
      load_inv_size = fan_out_spice_model->input_buffer->size;
      break;
    case SPICE_MODEL_BUF_BUF:
      load_inv_size = 1.;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid fanout spice_model input_buffer type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  } else {
    /* TODO: If there is no inv/buffer at input, we should traversal until there is one 
     * However, now we just simply give a minimum sized inverter
     */
    load_inv_size = 1.;
  }
 
  return load_inv_size;
}

float find_spice_testbench_rr_mux_load_inv_size(t_rr_node* load_rr_node,
                                                int switch_index) {
  float load_inv_size = 0;
  t_spice_model* fan_out_spice_model = NULL;

  fan_out_spice_model = switch_inf[switch_index].spice_model;

  /* Check */
  assert(NULL != fan_out_spice_model);
  assert(NULL != fan_out_spice_model->input_buffer);

  /* depend on the input_buffer type */
  if (1 == fan_out_spice_model->input_buffer->exist) {
    switch(fan_out_spice_model->input_buffer->type) {
    case SPICE_MODEL_BUF_INV:
      load_inv_size = fan_out_spice_model->input_buffer->size;
      break;
    case SPICE_MODEL_BUF_BUF:
      load_inv_size = fan_out_spice_model->input_buffer->size;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid fanout spice_model input_buffer type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  } else {
    /* TODO: If there is no inv/buffer at input, we should traversal until there is one 
     * However, now we just simply give a minimum sized inverter
     *   fprint_spice_testbench_pb_graph_pin_inv_loads_rec(fp, &testbench_load_cnt,
                                                      x, y, 
                                                      &(cur_pb_graph_node->output_pins[0][0]), 
                                                      logical_block[logical_block_index].pb, 
                                                      outport_name, 
                                                      FALSE, 
                                                      LL_rr_node_indices); 

     */
    load_inv_size = 1;
  }
 
  return load_inv_size;

}

void fprint_spice_testbench_pb_graph_pin_inv_loads_rec(FILE* fp, int* testbench_load_cnt, 
                                                       int grid_x, int grid_y,
                                                       t_pb_graph_pin* src_pb_graph_pin, 
                                                       t_pb* src_pb, 
                                                       char* outport_name,
                                                       boolean consider_parent_node,
                                                       t_ivec*** LL_rr_node_indices) {
  int iedge, mode_index, ipb, jpb;
  t_interconnect* cur_interc = NULL;
  char* rec_outport_name = NULL;
  t_pb* des_pb = NULL;
  int src_rr_node_index = -1;
  float load_inv_size = 0.;
  float total_width;
  int width_cnt;

  /* A valid file handler*/
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid File Handler!\n",__FILE__, __LINE__); 
    exit(1);
  } 

  assert(NULL != src_pb_graph_pin);
  
  if (TRUE == consider_parent_node) {
    if (NULL != src_pb_graph_pin->parent_node->pb_type->spice_model) {
      load_inv_size = find_spice_testbench_pb_pin_mux_load_inv_size(src_pb_graph_pin->parent_node->pb_type->spice_model);
      /* Print inverters by considering maximum width */
      total_width = load_inv_size;
      width_cnt = 0;
      while (total_width > max_width_per_trans) {
        fprintf(fp, "Xload_inv[%d]_no%d %s %s_out[0] gvdd_load 0 inv size=%g\n",
                    (*testbench_load_cnt), width_cnt,  outport_name, outport_name, max_width_per_trans);
        /* Update counter */
        total_width = total_width - max_width_per_trans;
        width_cnt++; 
      }
      if (total_width > 0) {
        fprintf(fp, "Xload_inv[%d]_no%d %s %s_out[0] gvdd_load 0 inv size=%g\n",
                    (*testbench_load_cnt), width_cnt,  outport_name, outport_name, total_width);
        (*testbench_load_cnt)++;
      }
      return;
    }
  }

  /* Get the mode_index */
  if (NULL == src_pb) {
    mode_index = find_pb_type_idle_mode_index(*(src_pb_graph_pin->parent_node->pb_type)); 
  } else {
    mode_index = src_pb->mode;
  }

  /* If this pb belongs to a pb_graph_head, 
   * the src_pb_graph_pin is a OPIN, we should find the rr_node */
  if ((OUT_PORT == src_pb_graph_pin->port->type)
     &&(NULL == src_pb_graph_pin->parent_node->parent_pb_graph_node)) {
    /* Find the corresponding rr_node */
    assert(grid[grid_x][grid_y].type->pb_graph_head == src_pb_graph_pin->parent_node);
    src_rr_node_index = get_rr_node_index(grid_x, grid_y, OPIN, src_pb_graph_pin->pin_count_in_cluster, LL_rr_node_indices); 
    for (iedge = 0; iedge < rr_node[src_rr_node_index].num_edges; iedge++) {
      /* Detect its input buffers */
      load_inv_size = find_spice_testbench_rr_mux_load_inv_size(&rr_node[rr_node[src_rr_node_index].edges[iedge]], 
                                                                rr_node[src_rr_node_index].switches[iedge]);
      /* Print inverters by considering maximum width */
      total_width = load_inv_size;
      width_cnt = 0;
      while (total_width > max_width_per_trans) {
        fprintf(fp, "Xload_inv[%d]_no%d %s load_inv[%d]_out gvdd_load 0 inv size=%g\n",
                    (*testbench_load_cnt), width_cnt, outport_name, 
                    (*testbench_load_cnt), max_width_per_trans);
        /* Update counter */
        total_width = total_width - max_width_per_trans;
        width_cnt++; 
      }
      if (total_width > 0) {
        fprintf(fp, "Xload_inv[%d]_no%d %s load_inv[%d]_out gvdd_load 0 inv size=%g\n",
                (*testbench_load_cnt), width_cnt, outport_name, 
                (*testbench_load_cnt), total_width);
      }
      (*testbench_load_cnt)++;
    }
    return;
  }

  /* Search output edges */
  for (iedge = 0; iedge < src_pb_graph_pin->num_output_edges; iedge++) {
    check_pb_graph_edge(*(src_pb_graph_pin->output_edges[iedge])); 
    /* We care only the edges in selected mode */
    cur_interc = src_pb_graph_pin->output_edges[iedge]->interconnect;
    assert(NULL != cur_interc);
    if (mode_index == cur_interc->parent_mode_index) {
      rec_outport_name = (char*)my_malloc(sizeof(char)* (strlen(outport_name) + 5 + strlen(my_itoa(iedge)) +2 ));
      sprintf(rec_outport_name, "%s_out[%d]", outport_name, iedge);
      /* check the interc has spice_model and if it is buffered */
      assert(NULL != cur_interc->spice_model);
      if (TRUE == cur_interc->spice_model->input_buffer->exist) {
        /* Print a inverter, and we stop this branch */
        load_inv_size = find_spice_testbench_pb_pin_mux_load_inv_size(cur_interc->spice_model);
        /* Print inverters by considering maximum width */
        total_width = load_inv_size;
        width_cnt = 0;
        while (total_width > max_width_per_trans) {
          fprintf(fp, "Xload_inv[%d]_no%d %s %s gvdd_load 0 inv size=%g\n",
                  (*testbench_load_cnt), width_cnt, outport_name, rec_outport_name, max_width_per_trans);
          /* Update counter */
          total_width = total_width - max_width_per_trans;
          width_cnt++; 
        }
        if (total_width > 0) {
          fprintf(fp, "Xload_inv[%d]_no%d %s %s gvdd_load 0 inv size=%g\n",
                  (*testbench_load_cnt), width_cnt, outport_name, rec_outport_name, total_width);
        }
        (*testbench_load_cnt)++;
      } else {
        /*
        fprintf(fp, "R%s_to_%s %s %s 0\n",
                outport_name, rec_outport_name,  
                outport_name, rec_outport_name); 
        */
        /* Go recursively */
        if (NULL == src_pb) {
          des_pb = NULL;
        } else {
          if (IN_PORT == src_pb_graph_pin->port->type) {
            ipb = src_pb_graph_pin->output_edges[iedge]->output_pins[0]->parent_node->pb_type 
                  - src_pb_graph_pin->parent_node->pb_type->modes[mode_index].pb_type_children; 
            jpb = src_pb_graph_pin->output_edges[iedge]->output_pins[0]->parent_node->placement_index; 
            if ((NULL != src_pb->child_pbs[ipb])&&(NULL != src_pb->child_pbs[ipb][jpb].name)) {
              des_pb = &(src_pb->child_pbs[ipb][jpb]);
            } else {
              des_pb = NULL;
            }
          } else if (OUT_PORT == src_pb_graph_pin->port->type) {
            des_pb = src_pb->parent_pb;
          } else if (INOUT_PORT == src_pb_graph_pin->port->type) {
            des_pb = NULL; /* I don't know what to do...*/
          }
        }
        fprint_spice_testbench_pb_graph_pin_inv_loads_rec(fp, testbench_load_cnt, grid_x, grid_y,  
                                                          src_pb_graph_pin->output_edges[iedge]->output_pins[0],
                                                          des_pb, outport_name, TRUE, LL_rr_node_indices);
        
      }
    }
  }

  return;
}

char* fprint_spice_testbench_rr_node_load_version(FILE* fp, int* testbench_load_cnt,
                                                  int num_segments,
                                                  t_segment_inf* segments,
                                                  int load_level,
                                                  t_rr_node cur_rr_node, 
                                                  char* outport_name) {
  char* ret_outport_name = NULL;
  char* mid_outport_name = NULL;
  int cost_index;
  int iseg, i, iedge, chan_wire_length, cur_x, cur_y;
  t_rr_node to_node;
  t_spice_model* wire_spice_model = NULL;
  float load_inv_size = 0.;
  float total_width;
  int  width_cnt;

  /* We only process CHANX or CHANY*/
  if (!((CHANX == cur_rr_node.type)
    ||(CHANY == cur_rr_node.type))) {
    ret_outport_name = my_strdup(outport_name);
    return ret_outport_name;
  }

  /* Important: 
   * As the cur_rr_node can only be channel wires 
   * and channel wires can only be driven at the starting point,
   * in this function,
   * the loads to be added will always starts from the starting point of a channel wire.
   * We will consider the length of channel wires when adding the loads.
   * For example, a length-4 wire will introduce 4 levels of channel segments to the loads.
   * To handle the corner case, we will consider the border when adding channel segments
   * If the length of wire exceeds the borderline of FPGA, 
   * we will adapt the number of channel segments.
   */
  cost_index = cur_rr_node.cost_index;
  iseg = rr_indexed_data[cost_index].seg_index;
  assert((!(iseg < 0))&&(iseg < num_segments));
  wire_spice_model = segments[iseg].spice_model;
  assert(SPICE_MODEL_CHAN_WIRE == wire_spice_model->type);
  /* Check if the coordinate is correct */
  assert((0 == cur_rr_node.xhigh - cur_rr_node.xlow)
        ||(0 == cur_rr_node.yhigh - cur_rr_node.ylow));
  chan_wire_length = cur_rr_node.xhigh - cur_rr_node.xlow 
                   + cur_rr_node.yhigh - cur_rr_node.ylow;

  fprintf(fp, "**** Loads for rr_node: xlow=%d, ylow=%d, xhigh=%d, yhigh=%d, ptc_num=%d, type=%d *****\n", 
          cur_rr_node.xlow, cur_rr_node.ylow,
          cur_rr_node.xhigh, cur_rr_node.yhigh,
          cur_rr_node.ptc_num, cur_rr_node.type);

  for (i = 0; i < chan_wire_length + 1; i++) { 
    ret_outport_name = (char*)my_malloc(sizeof(char)*( strlen(outport_name)
                       + 9 + strlen(my_itoa(load_level + i)) + 6
                       + 1 ));
    sprintf(ret_outport_name,"%s_loadlvl[%d]_out",
            outport_name, load_level + i);
    mid_outport_name = (char*)my_malloc(sizeof(char)*( strlen(outport_name)
                       + 9 + strlen(my_itoa(load_level + i)) + 9
                       + 1 ));
    sprintf(mid_outport_name,"%s_loadlvl[%d]_midout",
            outport_name, load_level + i);
    if (0 == i) {
      fprintf(fp, "Xchan_%s %s %s %s gvdd_load 0 %s_seg%d\n",
              ret_outport_name, outport_name, ret_outport_name, mid_outport_name, 
              wire_spice_model->name, iseg); 
    } else {
      fprintf(fp, "Xchan_%s %s_loadlvl[%d]_out %s %s gvdd_load 0 %s_seg%d\n",
              ret_outport_name, outport_name, load_level + i -1, ret_outport_name, mid_outport_name,
              wire_spice_model->name, iseg); 
    }
    /* Print CB inv loads connected to the mid_out */
    switch (cur_rr_node.type) {
    case CHANX:
      /* Update the cur_x & cur_y */
      if (INC_DIRECTION == cur_rr_node.direction) { 
        cur_x = cur_rr_node.xlow + i;
        cur_y = cur_rr_node.ylow;
      } else {
        assert(DEC_DIRECTION == cur_rr_node.direction);
        cur_x = cur_rr_node.xhigh - i;
        cur_y = cur_rr_node.ylow;
      }
      for (iedge = 0; iedge < cur_rr_node.num_edges; iedge++) {
        /*Identify if the des node is a IPIN and fit the current(x,y)*/
        to_node = rr_node[cur_rr_node.edges[iedge]]; 
        switch (to_node.type) {
        case IPIN:
          /* The assert only works for homogeneous blocks 
          assert(to_node.xhigh == to_node.xlow);
          assert(to_node.yhigh == to_node.ylow);
          */
          if (((cur_x == to_node.xlow)&&(cur_y == to_node.ylow))
             ||((cur_x == to_node.xlow)&&((cur_y + 1) == to_node.ylow))) {
            /* We find a CB! */
            /* Detect its input buffers */
            load_inv_size = find_spice_testbench_rr_mux_load_inv_size(&to_node, 
                                                                      cur_rr_node.switches[iedge]);
            /* TODO: Need to find the downsteam inv_loads if it is not bufferred  
            fprint_spice_testbench_pb_graph_pin_inv_loads_rec(fp, &testbench_load_cnt,
                                                      x, y, 
                                                      &(cur_pb_graph_node->output_pins[0][0]), 
                                                      logical_block[logical_block_index].pb, 
                                                      outport_name, 
                                                      FALSE, 
                                                      LL_rr_node_indices); 
            */
            assert(0. < load_inv_size);
            /* Print an inverter */
            total_width = load_inv_size;
            width_cnt = 0;
            while (total_width > max_width_per_trans) { 
              fprintf(fp, "Xload_inv[%d]_no%d %s %s_out[%d] gvdd_load 0 inv size=%g\n",
                      (*testbench_load_cnt), width_cnt, mid_outport_name, mid_outport_name, iedge,
                      max_width_per_trans);
              /* Update */
              total_width = total_width - max_width_per_trans;
              width_cnt++;
            }
            if (total_width > 0) {
              fprintf(fp, "Xload_inv[%d]_no%d %s %s_out[%d] gvdd_load 0 inv size=%g\n",
                      (*testbench_load_cnt), width_cnt, mid_outport_name, mid_outport_name, iedge,
                      total_width);
            }
            (*testbench_load_cnt)++;
          }
          break;
        case CHANX:
        case CHANY:
          /* We find a SB! */
          /* Detect its input buffers */
          load_inv_size = find_spice_testbench_rr_mux_load_inv_size(&to_node, 
                                                                    cur_rr_node.switches[iedge]);
          assert(0. < load_inv_size);
          /* Print an inverter */
          total_width = load_inv_size;
          width_cnt = 0;
          while (total_width > max_width_per_trans) { 
            fprintf(fp, "Xload_inv[%d]_no%d %s %s_out[%d] gvdd_load 0 inv size=%g\n",
                    (*testbench_load_cnt), width_cnt, ret_outport_name, ret_outport_name, iedge,
                    max_width_per_trans);
            /* Update */
            total_width = total_width - max_width_per_trans;
            width_cnt++;
          }
          if (total_width > 0) {
            fprintf(fp, "Xload_inv[%d]_no%d %s %s_out[%d] gvdd_load 0 inv size=%g\n",
                    (*testbench_load_cnt), width_cnt, ret_outport_name, ret_outport_name, iedge,
                    total_width);
          }
          (*testbench_load_cnt)++;
          break;
        default:
          vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid src_rr_node_type!\n",
                     __FILE__, __LINE__);
          exit(1);
        }
      }
      break;
    case CHANY:
      /* Update the cur_x & cur_y */
      if (INC_DIRECTION == cur_rr_node.direction) { 
        cur_x = cur_rr_node.xlow;
        cur_y = cur_rr_node.ylow + i;
      } else {
        assert(DEC_DIRECTION == cur_rr_node.direction);
        cur_x = cur_rr_node.xlow;
        cur_y = cur_rr_node.yhigh - i;
      }
      for (iedge = 0; iedge < cur_rr_node.num_edges; iedge++) {
        /*Identify if the des node is a IPIN and fit the current(x,y)*/
        to_node = rr_node[cur_rr_node.edges[iedge]]; 
        switch (to_node.type) {
        case IPIN:
          /* The assert only works for homogeneous blocks 
          assert(to_node.xhigh == to_node.xlow);
          assert(to_node.yhigh == to_node.ylow);
          */
          if (((cur_y == to_node.ylow)&&(cur_x == to_node.xlow))
             ||((cur_y == to_node.xlow)&&((cur_x + 1) == to_node.xlow))) {
            /* We find a CB! */
            /* Detect its input buffers */
            load_inv_size = find_spice_testbench_rr_mux_load_inv_size(&to_node, 
                                                                      cur_rr_node.switches[iedge]);
            assert(0. < load_inv_size);
            /* Print an inverter */
            total_width = load_inv_size;
            width_cnt = 0;
            while (total_width > max_width_per_trans) { 
              fprintf(fp, "Xload_inv[%d]_no%d %s %s_out[%d] gvdd_load 0 inv size=%g\n",
                      (*testbench_load_cnt), width_cnt, mid_outport_name, mid_outport_name, iedge,
                      max_width_per_trans);
              /* Update */
              total_width = total_width - max_width_per_trans;
              width_cnt++;
            }
            if (total_width > 0) {
              fprintf(fp, "Xload_inv[%d]_no%d %s %s_out[%d] gvdd_load 0 inv size=%g\n",
                      (*testbench_load_cnt), width_cnt, mid_outport_name, mid_outport_name, iedge,
                      total_width);
            }
            (*testbench_load_cnt)++;
          }
          break;
        case CHANX:
        case CHANY:
          /* We find a SB! */
          /* Detect its input buffers */
          load_inv_size = find_spice_testbench_rr_mux_load_inv_size(&to_node,
                                                                    cur_rr_node.switches[iedge]);
          assert(0. < load_inv_size);
          /* Print an inverter */
          total_width = load_inv_size;
          width_cnt = 0;
          while (total_width > max_width_per_trans) { 
            fprintf(fp, "Xload_inv[%d]_no%d %s %s_out[%d] gvdd_load 0 inv size=%g\n",
                    (*testbench_load_cnt), width_cnt, ret_outport_name, ret_outport_name, iedge,
                    total_width);
            /* Update */
            total_width = total_width - max_width_per_trans;
            width_cnt++;
          }
          if (total_width > 0) {
            fprintf(fp, "Xload_inv[%d]_no%d %s %s_out[%d] gvdd_load 0 inv size=%g\n",
                    (*testbench_load_cnt), width_cnt, ret_outport_name, ret_outport_name, iedge,
                    total_width);
          }
          (*testbench_load_cnt)++;
          break;
        default:
          vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid src_rr_node_type!\n",
                     __FILE__, __LINE__);
          exit(1);
        }
      }
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid src_rr_node_type!\n",
                 __FILE__, __LINE__);
      exit(1);
    } 
  }

  return ret_outport_name;
} 

void fprint_spice_testbench_one_cb_mux_loads(FILE* fp, int* testbench_load_cnt,
                                             t_rr_node* src_rr_node,
                                             char* outport_name,
                                             t_ivec*** LL_rr_node_indices) {
  t_type_ptr cb_out_grid_type = NULL;
  t_pb_graph_pin* cb_out_pb_graph_pin = NULL;              
  t_pb* cb_out_pb = NULL;

  assert(IPIN == src_rr_node->type);
  /* The assert only works for homogeneous blocks 
  assert(src_rr_node->xlow == src_rr_node->xhigh);
  assert(src_rr_node->ylow == src_rr_node->yhigh);
  */

  cb_out_grid_type = grid[src_rr_node->xlow][src_rr_node->ylow].type; 
  assert(NULL != cb_out_grid_type);

  cb_out_pb_graph_pin = src_rr_node->pb_graph_pin;
  assert(NULL != cb_out_pb_graph_pin);
   
  /* Get the pb ! Get the mode_index */
  cb_out_pb = src_rr_node->pb;

  if (IO_TYPE == cb_out_grid_type) {
    fprintf(fp, "******* IO_TYPE loads *******\n");
  } else {
    fprintf(fp, "******* Normal TYPE loads *******\n");
  }

  /* Recursively find all the inv load inside pb_graph_node */
  fprint_spice_testbench_pb_graph_pin_inv_loads_rec(fp,
                                                    testbench_load_cnt, 
                                                    src_rr_node->xlow, 
                                                    src_rr_node->ylow, 
                                                    cb_out_pb_graph_pin, 
                                                    cb_out_pb, 
                                                    outport_name, 
                                                    TRUE, 
                                                    LL_rr_node_indices);
  

  fprintf(fp, "******* END loads *******\n");
  return;
}

void fprint_spice_testbench_one_grid_pin_stimulation(FILE* fp, int x, int y, 
                                                     int height, int side, 
                                                     int ipin,
                                                     t_ivec*** LL_rr_node_indices) {
  int ipin_rr_node_index;
  float ipin_density, ipin_probability;
  int ipin_init_value;
  int class_id;
  t_rr_type grid_pin_rr_node_type;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 

  /* Identify the type of rr_node */
  class_id = grid[x][y].type->pin_class[ipin];
  if (DRIVER == grid[x][y].type->class_inf[class_id].type) {
    grid_pin_rr_node_type = OPIN;
  } else if (RECEIVER == grid[x][y].type->class_inf[class_id].type) {
    grid_pin_rr_node_type = IPIN;
  } else {
    /* Error */
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Unknown pin type for grid(x=%d, y=%d, pin_index=%d)!\n",
               __FILE__, __LINE__,
               x, y, ipin);
    exit(1);
  }

  /* Print a voltage source according to density and probability */
  ipin_rr_node_index = get_rr_node_index(x, y, grid_pin_rr_node_type, ipin, LL_rr_node_indices);
  /* Get density and probability */
  ipin_density = get_rr_node_net_density(rr_node[ipin_rr_node_index]); 
  ipin_probability = get_rr_node_net_probability(rr_node[ipin_rr_node_index]); 
  ipin_init_value = get_rr_node_net_init_value(rr_node[ipin_rr_node_index]); 
  /* Print voltage source */
  fprintf(fp, "Vgrid[%d][%d]_pin[%d][%d][%d] grid[%d][%d]_pin[%d][%d][%d] 0 \n",
          x, y, height, side, ipin, x, y, height, side, ipin);
  fprint_voltage_pulse_params(fp, ipin_init_value, ipin_density, ipin_probability);

  return;
}

void fprint_spice_testbench_one_grid_pin_loads(FILE* fp, int x, int y, 
                                               int height, int side, 
                                               int ipin,
                                               int* testbench_load_cnt,
                                               t_ivec*** LL_rr_node_indices) {

  int ipin_rr_node_index;
  int iedge, iswitch;
  char* prefix = NULL;
  t_spice_model* switch_spice_model = NULL;
  int inv_cnt = 0;
  int class_id;
  t_rr_type grid_pin_rr_node_type;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid File Handler!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  
  /* Identify the type of rr_node */
  class_id = grid[x][y].type->pin_class[ipin];
  if (DRIVER == grid[x][y].type->class_inf[class_id].type) {
    grid_pin_rr_node_type = OPIN;
  } else if (RECEIVER == grid[x][y].type->class_inf[class_id].type) {
    grid_pin_rr_node_type = IPIN;
  } else {
    /* Error */
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Unknown pin type for grid(x=%d, y=%d, pin_index=%d)!\n",
               __FILE__, __LINE__,
               x, y, ipin);
    exit(1);
  }

  /* Print a voltage source according to density and probability */
  ipin_rr_node_index = get_rr_node_index(x, y, grid_pin_rr_node_type, ipin, LL_rr_node_indices);
  /* Generate prefix */
  prefix = (char*)my_malloc(sizeof(char)*(5 + strlen(my_itoa(x))
             + 2 + strlen(my_itoa(y)) + 6 + strlen(my_itoa(height))
             + 2 + strlen(my_itoa(side)) + 2 + strlen(my_itoa(ipin))
             + 2 + 1));
  sprintf(prefix, "grid[%d][%d]_pin[%d][%d][%d]",
          x, y, height, side, ipin);

  /* Depending on the type of this pin */
  /* For OPIN, we search the rr_node graph */
  if (OPIN == grid_pin_rr_node_type) {
    /* Print all the inverter load now*/
    for (iedge = 0; iedge < rr_node[ipin_rr_node_index].num_edges; iedge++) {
      /* Get the switch spice model */
      /* inode = rr_node[ipin_rr_node_index].edges[iedge]; */
      iswitch = rr_node[ipin_rr_node_index].switches[iedge]; 
      switch_spice_model = switch_inf[iswitch].spice_model;
      if (NULL == switch_spice_model) {
        continue;
      }
      /* Add inv/buf here */
      fprintf(fp, "X%s_inv[%d] %s %s_out[%d] gvdd_load 0 inv size=%g\n",
              prefix, iedge, 
              prefix, prefix, iedge, 
              switch_spice_model->input_buffer->size);
      inv_cnt++;
    }
    /* TODO: go recursively ? */
  /* For IPIN, we should search the internal rr_graph of the grid */
  } else if (IPIN == grid_pin_rr_node_type) {
    fprint_spice_testbench_one_cb_mux_loads(fp, testbench_load_cnt, &rr_node[ipin_rr_node_index], 
                                            prefix, LL_rr_node_indices);
  } else {
    /* Error */
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Unknown pin type for grid(x=%d, y=%d, pin_index=%d)!\n",
               __FILE__, __LINE__,
               x, y, ipin);
    exit(1);
  } 
  /* TODO: Generate loads recursively */
  /*fprint_rr_node_loads_rec(fp, rr_node[ipin_rr_node_index],prefix);*/

  /*Free */
  my_free(prefix);

  return;
}


/* Add one SPICE TB information to linked list */
t_llist* add_one_spice_tb_info_to_llist(t_llist* cur_head, 
                                        char* tb_file_path, 
                                        int num_sim_clock_cycles) {
  t_llist* new_head = NULL;

  if (NULL == cur_head) {
    new_head = create_llist(1);
    new_head->dptr = my_malloc(sizeof(t_spicetb_info));
    ((t_spicetb_info*)(new_head->dptr))->tb_name = my_strdup(tb_file_path);
    ((t_spicetb_info*)(new_head->dptr))->num_sim_clock_cycles = num_sim_clock_cycles;
  } else {
    new_head = insert_llist_node_before_head(cur_head);
    new_head->dptr = my_malloc(sizeof(t_spicetb_info));
    ((t_spicetb_info*)(new_head->dptr))->tb_name = my_strdup(tb_file_path);
    ((t_spicetb_info*)(new_head->dptr))->num_sim_clock_cycles = num_sim_clock_cycles;
  }

  return new_head;
}

