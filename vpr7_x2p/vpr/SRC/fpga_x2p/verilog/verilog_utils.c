/***********************************/
/* Synthesizable Verilog Dumping   */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
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
#include "route_common.h"

/* FPGA-SPICE utils */
#include "read_xml_spice_util.h"
#include "linkedlist.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_globals.h"

/* syn_verilog globals */
#include "verilog_global.h"
#include "verilog_utils.h"

/****** Subroutines *******/
void init_list_include_verilog_netlists(t_spice* spice) { 
  int i, j, cur;
  int to_include = 0;
  int num_to_include = 0;

  /* Initialize */
  for (i = 0; i < spice->num_include_netlist; i++) { 
    FreeSpiceModelNetlist(&(spice->include_netlists[i]));
  }
  my_free(spice->include_netlists);
  spice->include_netlists = NULL;
  spice->num_include_netlist = 0;

  /* Generate include netlist list */
  vpr_printf(TIO_MESSAGE_INFO, "Listing Verilog Netlist Names to be included...\n");
  for (i = 0; i < spice->num_spice_model; i++) {
    if (NULL != spice->spice_models[i].verilog_netlist) {
      /* Check if this netlist name has already existed in the list */
      to_include = 1;
      for (j = 0; j < i; j++) {
        if (NULL == spice->spice_models[j].verilog_netlist) {
          continue;
        }
        if (0 == strcmp(spice->spice_models[j].verilog_netlist, spice->spice_models[i].verilog_netlist)) {
          to_include = 0;
          break;
        }
      }
      /* Increamental */
      if (1 == to_include) {
        num_to_include++;
      }
    }
  }

  /* realloc */
  spice->include_netlists = (t_spice_model_netlist*)my_realloc(spice->include_netlists, 
                              sizeof(t_spice_model_netlist)*(num_to_include + spice->num_include_netlist));

  /* Fill the new included netlists */
  cur = spice->num_include_netlist;
  for (i = 0; i < spice->num_spice_model; i++) {
    if (NULL != spice->spice_models[i].verilog_netlist) {
      /* Check if this netlist name has already existed in the list */
      to_include = 1;
      for (j = 0; j < i; j++) {
        if (NULL == spice->spice_models[j].verilog_netlist) {
          continue;
        }
        if (0 == strcmp(spice->spice_models[j].verilog_netlist, spice->spice_models[i].verilog_netlist)) {
          to_include = 0;
          break;
        }
      }
      /* Increamental */
      if (1 == to_include) {
        spice->include_netlists[cur].path = my_strdup(spice->spice_models[i].verilog_netlist); 
        spice->include_netlists[cur].included = 0;
        vpr_printf(TIO_MESSAGE_INFO, "[%d] %s\n", cur+1, spice->include_netlists[cur].path);
        cur++;
      }
    }
  }
  /* Check */
  assert(cur == (num_to_include + spice->num_include_netlist));
  /* Update */
  spice->num_include_netlist += num_to_include;
  
  return;
}


void init_include_user_defined_verilog_netlists(t_spice spice) {
  int i;

  /* Include user-defined sub-circuit netlist */
  for (i = 0; i < spice.num_include_netlist; i++) {
    spice.include_netlists[i].included = 0;
  }

  return;
}

void dump_include_user_defined_verilog_netlists(FILE* fp,
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
      fprintf(fp, "// `include \"%s\"\n", spice.include_netlists[i].path);
      spice.include_netlists[i].included = 1;
    } else {
      assert(1 == spice.include_netlists[i].included);
    }
  } 

  return;
}

void dump_verilog_file_header(FILE* fp,
                              char* usage) {
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d]) FileHandle is NULL!\n",__FILE__,__LINE__); 
    exit(1);
  } 
  fprintf(fp,"//-------------------------------------------\n");
  fprintf(fp,"//    FPGA Synthesizable Verilog Netlist     \n");
  fprintf(fp,"//    Description: %s \n",usage);
  fprintf(fp,"//           Author: Xifan TANG              \n");
  fprintf(fp,"//        Organization: EPFL/IC/LSI          \n");
  fprintf(fp,"//    Date: %s \n", my_gettime());
  fprintf(fp,"//-------------------------------------------\n");
  fprintf(fp,"//----- Time scale -----\n");
  fprintf(fp,"`timescale 1ns / 1ps\n");
  fprintf(fp,"\n");

  return;
}

/* Dump preproc */
void dump_verilog_preproc(FILE* fp, 
                          t_syn_verilog_opts fpga_verilog_opts,
                          enum e_verilog_tb_type verilog_tb_type) {

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d]) FileHandle is NULL!\n",__FILE__,__LINE__); 
    exit(1);
  } 

  /* To enable timing */
  if (TRUE == fpga_verilog_opts.include_timing) {
    fprintf(fp, "`define %s 1\n", verilog_timing_preproc_flag);
    fprintf(fp, "\n");
  } 

  /* To enable timing */
  if (TRUE == fpga_verilog_opts.include_signal_init) {
    fprintf(fp, "`define %s 1\n", verilog_signal_init_preproc_flag);
    fprintf(fp, "\n");
  } 

  /* To enable formal verfication flag */
  if (TRUE == fpga_verilog_opts.print_formal_verification_top_netlist) {
    fprintf(fp, "`define %s 1\n",
                 verilog_formal_verification_preproc_flag); // the flag to enable formal verification during compilation
    fprintf(fp, "\n");
  } 

  /* To enable functional verfication with Icarus */
  if (TRUE == fpga_verilog_opts.include_icarus_simulator) {
    fprintf(fp, "`define %s 1\n",
                 icarus_simulator_flag); // the flag to enable formal verification during compilation
    fprintf(fp, "\n");
  } 

  return;
}

void dump_simulation_preproc(FILE* fp, 
                             t_syn_verilog_opts fpga_verilog_opts,
                             enum e_verilog_tb_type verilog_tb_type) {

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s, LINE[%d]) FileHandle is NULL!\n",__FILE__,__LINE__); 
    exit(1);
  } 

  /* To enable manualy checked simulation */
  if (TRUE == fpga_verilog_opts.print_top_testbench) {
    fprintf(fp, "`define %s 1\n", initial_simulation_flag);
    fprintf(fp, "\n");
  } 

  /* To enable auto-checked simulation */
  if (TRUE == fpga_verilog_opts.print_autocheck_top_testbench) {
    fprintf(fp, "`define %s 1\n", autochecked_simulation_flag);
    fprintf(fp, "\n");
  } 

  /* To enable pre-configured FPGA simulation */
  if (TRUE == fpga_verilog_opts.print_formal_verification_top_netlist) {
    fprintf(fp, "`define %s 1\n", formal_simulation_flag); 
    fprintf(fp, "\n");
  } 

  return;
}

void dump_verilog_simulation_preproc(char* subckt_dir,
                                 t_syn_verilog_opts fpga_verilog_opts) {
  /* Create a file handler */
  FILE* fp = NULL;
  char* file_description = NULL;
  char* fname = NULL;

  fname = my_strcat(subckt_dir, 
                    defines_verilog_simulation_file_name);

  /* Create a file*/
  fp = fopen(fname, "w");

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Failure in create Verilog netlist %s",
               __FILE__, __LINE__, fname); 
    exit(1);
  } 

  /* Generate the descriptions*/
  file_description = "Simulation Flags";
  dump_verilog_file_header(fp, file_description);

  /* Dump the defines preproc flags*/
  dump_simulation_preproc(fp, fpga_verilog_opts, VERILOG_TB_TOP); 

  fclose(fp);

  /* Free */
  my_free(fname);

  return;
}

void dump_verilog_defines_preproc(char* subckt_dir,
                                 t_syn_verilog_opts fpga_verilog_opts) {
  /* Create a file handler */
  FILE* fp = NULL;
  char* file_description = NULL;
  char* fname = NULL;

  fname = my_strcat(subckt_dir, 
                    defines_verilog_file_name);

  /* Create a file*/
  fp = fopen(fname, "w");

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Failure in create Verilog netlist %s",
               __FILE__, __LINE__, fname); 
    exit(1);
  } 

  /* Generate the descriptions*/
  file_description = "Preproc Flags";
  dump_verilog_file_header(fp, file_description);

  /* Dump the defines preproc flags*/
  dump_verilog_preproc(fp, fpga_verilog_opts, VERILOG_TB_TOP); 

  fclose(fp);

  /* Free */
  my_free(fname);

  return;
}

void verilog_include_defines_preproc_file(FILE* fp, 
                                          char* verilog_dir) {
  char* temp_include_file_path = NULL;
  char* formatted_verilog_dir = NULL;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!",
               __FILE__, __LINE__); 
    exit(1);
  } 

  fprintf(fp, "//------ Include defines: preproc flags -----\n");
  formatted_verilog_dir = format_dir_path(verilog_dir);
  temp_include_file_path = my_strcat(formatted_verilog_dir, defines_verilog_file_name);
  fprintf(fp, "`include \"%s\"\n", temp_include_file_path);
  fprintf(fp, "//------ End Include defines: preproc flags -----\n");
  my_free(temp_include_file_path);

  return;
}

void verilog_include_simulation_defines_file(FILE* fp, 
                                             char* verilog_dir) {
  char* temp_include_file_path = NULL;
  char* formatted_verilog_dir = NULL;

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Invalid file handler!",
               __FILE__, __LINE__); 
    exit(1);
  } 

  fprintf(fp, "//------ Include simulation defines -----\n");
  formatted_verilog_dir = format_dir_path(verilog_dir);
  temp_include_file_path = my_strcat(formatted_verilog_dir, defines_verilog_simulation_file_name);
  fprintf(fp, "`include \"%s\"\n", temp_include_file_path);
  fprintf(fp, "//------ End Include simulation defines -----\n");
  my_free(temp_include_file_path);

  return;
}

/* Create a file handler for a subckt Verilog netlist */
FILE* verilog_create_one_subckt_file(char* subckt_dir,
                                     const char* subckt_name_prefix,
                                     const char* verilog_subckt_file_name_prefix,
                                     char** verilog_fname) {
  FILE* fp = NULL;
  char* file_description = NULL;

  char* temp = my_strcat(subckt_dir, verilog_subckt_file_name_prefix);
  (*verilog_fname) = my_strcat(temp, verilog_netlist_file_postfix);

  /* Create a file*/
  fp = fopen((*verilog_fname), "w");

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Failure in create Verilog netlist %s",
               __FILE__, __LINE__, (*verilog_fname)); 
    exit(1);
  } 

  /* Generate the descriptions*/
  file_description = (char*) my_malloc(sizeof(char) * (strlen(subckt_name_prefix) + 9));
  sprintf(file_description, 
          "%s in FPGA", 
          subckt_name_prefix);
 
  dump_verilog_file_header(fp, file_description);

  /* Free */
  my_free(temp);
  my_free(file_description);

  return fp;
}


/* Create a file handler for a subckt Verilog netlist */
FILE* verilog_create_one_subckt_file(char* subckt_dir,
                                     const char* subckt_name_prefix,
                                     const char* verilog_subckt_file_name_prefix,
                                     int grid_x, int grid_y,
                                     char** verilog_fname) {
  FILE* fp = NULL;
  char* file_description = NULL;

  (*verilog_fname) = my_strcat(subckt_dir, 
                               fpga_spice_create_one_subckt_filename(verilog_subckt_file_name_prefix, grid_x, grid_y, verilog_netlist_file_postfix));

  /* Create a file*/
  fp = fopen((*verilog_fname), "w");

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Failure in create Verilog netlist %s",
               __FILE__, __LINE__, (*verilog_fname)); 
    exit(1);
  } 

  /* Generate the descriptions*/
  if (-1 == grid_y) {
    file_description = (char*) my_malloc(sizeof(char) * (strlen(subckt_name_prefix) + 2
                                         + strlen(my_itoa(grid_x)) + 2 
                                         + 10));
    sprintf(file_description, 
            "%s [%d] in FPGA", 
            subckt_name_prefix, grid_x);
  } else {
    file_description = (char*) my_malloc(sizeof(char) * (strlen(subckt_name_prefix) + 2
                                         + strlen(my_itoa(grid_x)) + 2 + strlen(my_itoa(grid_y))
                                         + 10));
    sprintf(file_description, 
            "%s [%d][%d] in FPGA", 
            subckt_name_prefix, grid_x, grid_y);
  }
 
  dump_verilog_file_header(fp, file_description);

  /* Free */
  my_free(file_description);

  return fp;
}

/* Output all the created subckt file names in a header file,
 * that can be easily imported in a top-level netlist
 */
void dump_verilog_subckt_header_file(t_llist* subckt_llist_head,
                                     char* subckt_dir,
                                     char* header_file_name) {
  FILE* fp = NULL;
  char* verilog_fname = NULL;
  t_llist* temp = NULL; 

  verilog_fname = my_strcat(subckt_dir, 
                            header_file_name);

  /* Create a file*/
  fp = fopen(verilog_fname, "w");

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Failure in create Verilog netlist %s",
               __FILE__, __LINE__, verilog_fname); 
    exit(1);
  } 

  /* Generate the descriptions*/
  dump_verilog_file_header(fp, "Header file");

  /* Output file names */
  temp = subckt_llist_head;
  while (temp) {
    fprintf(fp, "`include \"%s\"\n",
            (char*)(temp->dptr));
    temp = temp->next;
  }
  
  /* Close fp */
  fclose(fp);

  /* Free */
  my_free(verilog_fname);
   
  return;
}

/* Determine the split sign for generic port */
char determine_verilog_generic_port_split_sign(enum e_dump_verilog_port_type dump_port_type) {
  char ret;

  switch (dump_port_type) {
  case VERILOG_PORT_INPUT:
  case VERILOG_PORT_OUTPUT:
  case VERILOG_PORT_INOUT:
  case VERILOG_PORT_CONKT:
    ret = ',';
    break;
  case VERILOG_PORT_WIRE:
  case VERILOG_PORT_REG:
    ret = ';';
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of Verilog port to be dumped !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return ret;
}

/* Dump a generic Verilog port */
void dump_verilog_generic_port(FILE* fp, 
                               enum e_dump_verilog_port_type dump_port_type,
                               char* port_name, int port_lsb, int port_msb) {  
  boolean dump_single_port = FALSE;
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(port_lsb < 0))&&(!(port_msb < 0)));
  if (port_lsb == port_msb) {
    dump_single_port = TRUE;
  }
  dump_single_port = FALSE; /* Disable it for a clear synthesis */

  switch (dump_port_type) {
  case VERILOG_PORT_INPUT:
    if (TRUE == dump_single_port) {
      fprintf(fp,"input %s ", 
              port_name);
    } else {
      assert(FALSE == dump_single_port);
      fprintf(fp,"input [%d:%d] %s ", 
              port_lsb, port_msb,
              port_name);
    }
    break;
  case VERILOG_PORT_OUTPUT:
    if (TRUE == dump_single_port) {
      fprintf(fp,"output %s ", 
              port_name);
    } else {
      assert(FALSE == dump_single_port);
      fprintf(fp,"output [%d:%d] %s ", 
              port_lsb, port_msb,
              port_name);
    }
    break;
  case VERILOG_PORT_INOUT:
    if (TRUE == dump_single_port) {
      fprintf(fp,"inout %s ", 
              port_name);
    } else {
      assert(FALSE == dump_single_port);
      fprintf(fp,"inout [%d:%d] %s ", 
              port_lsb, port_msb,
              port_name);
    }
    break;
  case VERILOG_PORT_WIRE:
    if (TRUE == dump_single_port) {
      fprintf(fp,"wire %s ", 
              port_name);
    } else {
      assert(FALSE == dump_single_port);
      fprintf(fp,"wire [%d:%d] %s ", 
              port_lsb, port_msb,
              port_name);
    }
    break;
  case VERILOG_PORT_REG:
    if (TRUE == dump_single_port) {
      fprintf(fp,"reg %s ", 
              port_name);
    } else {
      assert(FALSE == dump_single_port);
      fprintf(fp,"reg [%d:%d] %s ", 
              port_lsb, port_msb,
              port_name);
    }
    break;
  case VERILOG_PORT_CONKT:
    if (TRUE == dump_single_port) {
      fprintf(fp,"%s[%d] ", 
              port_name, port_lsb);
    } else {
      assert(FALSE == dump_single_port);
      fprintf(fp,"%s[%d:%d] ", 
              port_name,
              port_lsb, port_msb);
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of Verilog port to be dumped !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* Dump a generic Verilog port */
void dump_verilog_generic_port_no_repeat(FILE* fp, 
                               enum e_dump_verilog_port_type dump_port_type,
                               char* port_name, int port_lsb, int port_msb) {  
  boolean dump_single_port = FALSE;
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(port_lsb < 0))&&(!(port_msb < 0)));
  if (port_lsb == port_msb) {
    dump_single_port = TRUE;
  }
  //dump_single_port = FALSE; /* Disable it for a clear synthesis */

  switch (dump_port_type) {
  case VERILOG_PORT_INPUT:
    if (TRUE == dump_single_port) {
      fprintf(fp,"input %s ", 
              port_name);
    } else {
      assert(FALSE == dump_single_port);
      fprintf(fp,"input [%d:%d] %s ", 
              port_lsb, port_msb,
              port_name);
    }
    break;
  case VERILOG_PORT_OUTPUT:
    if (TRUE == dump_single_port) {
      fprintf(fp,"output %s ", 
              port_name);
    } else {
      assert(FALSE == dump_single_port);
      fprintf(fp,"output [%d:%d] %s ", 
              port_lsb, port_msb,
              port_name);
    }
    break;
  case VERILOG_PORT_INOUT:
    if (TRUE == dump_single_port) {
      fprintf(fp,"inout %s ", 
              port_name);
    } else {
      assert(FALSE == dump_single_port);
      fprintf(fp,"inout [%d:%d] %s ", 
              port_lsb, port_msb,
              port_name);
    }
    break;
  case VERILOG_PORT_WIRE:
    if (TRUE == dump_single_port) {
      fprintf(fp,"wire %s ", 
              port_name);
    } else {
      assert(FALSE == dump_single_port);
      fprintf(fp,"wire [%d:%d] %s ", 
              port_lsb, port_msb,
              port_name);
    }
    break;
  case VERILOG_PORT_REG:
    if (TRUE == dump_single_port) {
      fprintf(fp,"reg %s ", 
              port_name);
    } else {
      assert(FALSE == dump_single_port);
      fprintf(fp,"reg [%d:%d] %s ", 
              port_lsb, port_msb,
              port_name);
    }
    break;
  case VERILOG_PORT_CONKT:
    if (TRUE == dump_single_port) {
      fprintf(fp,"%s[%d] ", 
              port_name, port_lsb);
    } else {
      assert(FALSE == dump_single_port);
      fprintf(fp,"%s[%d:%d] ", 
              port_name,
              port_lsb, port_msb);
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of Verilog port to be dumped !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

char* chomp_verilog_prefix(char* verilog_node_prefix) {
  int len = 0;
  char* ret = NULL;

  if (NULL == verilog_node_prefix) {
    return NULL;
  }

  len = strlen(verilog_node_prefix); /* String length without the last "\0"*/
  ret = (char*)my_malloc(sizeof(char)*(len+1));
  
  /* Don't do anything when input is NULL*/
  if (NULL == verilog_node_prefix) {
    my_free(ret);
    return NULL;
  }
  strcpy(ret,verilog_node_prefix);
  /* If the path end up with "_" we should remove it*/
  while ('_' == ret[len-1]) {
    ret[len-1] = ret[len];
    len--;
  }

  return ret;
}

char* format_verilog_node_prefix(char* verilog_node_prefix) {
  int len = strlen(verilog_node_prefix); /* String length without the last "\0"*/
  char* ret = (char*)my_malloc(sizeof(char)*(len+1));
 
  /* Don't do anything when input is NULL*/ 
  if (NULL == verilog_node_prefix) {
    my_free(ret);
    return NULL;
  }

  strcpy(ret,verilog_node_prefix);
  /* If the path does not end up with "_" we should complete it*/
  /*
  if (ret[len-1] != '_') {
    strcat(ret, "_");
  }
  */
  return ret;
}

/* Return the port_type in a verilog format */
char* verilog_convert_port_type_to_string(enum e_spice_model_port_type port_type) {
  switch (port_type) {
  case SPICE_MODEL_PORT_INPUT: 
  case SPICE_MODEL_PORT_CLOCK: 
  case SPICE_MODEL_PORT_SRAM:
  case SPICE_MODEL_PORT_BL:
  case SPICE_MODEL_PORT_WL:
    return "input";
  case SPICE_MODEL_PORT_OUTPUT: 
    return "output";
  case SPICE_MODEL_PORT_INOUT: 
    return "inout";
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s, [LINE%d])Invalid port type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return NULL;
}

/* Dump all the global ports that are stored in the linked list 
 * Return the number of ports that have been dumped 
 */
int rec_dump_verilog_spice_model_lib_global_ports(FILE* fp, 
                                                  t_spice_model* cur_spice_model,
                                                  boolean dump_port_type, 
                                                  boolean recursive,
                                                  boolean require_explicit_port_map) {
  int dumped_port_cnt;
  boolean dump_comma = FALSE;
  t_spice_model_port* cur_spice_model_port = NULL;
  t_llist* spice_model_head = NULL;
  t_llist* head = NULL;

  dumped_port_cnt = 0;

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

  rec_stats_spice_model_global_ports(cur_spice_model,
                                     recursive,
                                     &spice_model_head);

  /* Traverse the linked list and dump the ports */
  head = spice_model_head;
  while (head) {
    /* Get the port to be dumped */
    cur_spice_model_port = (t_spice_model_port*)(head->dptr);
    /* We have some port to dump ! 
     * Print a comment line 
     */
    /* Check if we need to dump a comma */
    if (TRUE == dump_comma) {
      fprintf(fp, ", //----- Global port of SPICE_MODEL(%s) -----\n",
                  cur_spice_model->name);
    }
    if (TRUE == dump_port_type) {
      fprintf(fp, "%s [0:%d] %s", 
              verilog_convert_port_type_to_string(cur_spice_model_port->type),
              cur_spice_model_port->size - 1, 
              cur_spice_model_port->lib_name);
    } else {
      /* Add explicit port mapping if required */
      if ((TRUE == require_explicit_port_map) 
         && (TRUE == cur_spice_model->dump_explicit_port_map)) {
        fprintf(fp, ".%s(",
                cur_spice_model_port->lib_name);
      }
      fprintf(fp, "%s[0:%d]", 
            cur_spice_model_port->lib_name,
            cur_spice_model_port->size - 1); 
      if ((TRUE == require_explicit_port_map) 
         && (TRUE == cur_spice_model->dump_explicit_port_map)) {
        fprintf(fp, ")");
      }
    }
    /* Decide if we need a comma */
    dump_comma = TRUE; 
    /* Update counter */
    dumped_port_cnt++;

    /* Go to the next node */
    head = head->next;
  }

  /* We have dumped some port! 
   * Print another comment line  
   */
  if (0 < dumped_port_cnt) {
    fprintf(fp, "\n");
  }

  /* Free linked list */
  head = spice_model_head;
  while (head) {
    head->dptr = NULL;
    head = head->next;
  }
  free_llist(spice_model_head);

  return dumped_port_cnt;
}


/* Dump all the global ports that are stored in the linked list 
 * Return the number of ports that have been dumped 
 */
int rec_dump_verilog_spice_model_global_ports(FILE* fp, 
                                              t_spice_model* cur_spice_model,
                                              boolean dump_port_type, 
                                              boolean recursive,
                                              boolean require_explicit_port_map) {
  int dumped_port_cnt;
  boolean dump_comma = FALSE;
  t_spice_model_port* cur_spice_model_port = NULL;
  t_llist* spice_model_head = NULL;
  t_llist* head = NULL;

  dumped_port_cnt = 0;

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

  rec_stats_spice_model_global_ports(cur_spice_model,
                                     recursive,
                                     &spice_model_head);

  /* Traverse the linked list and dump the ports */
  head = spice_model_head;
  if (NULL != head) {
    fprintf(fp, "//----- Global port of SPICE_MODEL(%s) -----\n",
                cur_spice_model->name);
  }
  while (head) {
    /* Get the port to be dumped */
    cur_spice_model_port = (t_spice_model_port*)(head->dptr);
    /* We have some port to dump ! 
     * Print a comment line 
     */
    /* Check if we need to dump a comma */
    if (TRUE == dump_comma) {
      fprintf(fp, ",\n");
    }
    if (TRUE == dump_port_type) {
      fprintf(fp, "%s [0:%d] %s", 
              verilog_convert_port_type_to_string(cur_spice_model_port->type),
              cur_spice_model_port->size - 1, 
              cur_spice_model_port->prefix);
    } else {
      /* Add explicit port mapping if required */
      if ((TRUE == require_explicit_port_map) 
         && (TRUE == cur_spice_model->dump_explicit_port_map)) {
        fprintf(fp, ".%s(",
                cur_spice_model_port->lib_name);
      }
      fprintf(fp, "%s[0:%d]", 
            cur_spice_model_port->prefix,
            cur_spice_model_port->size - 1); 
      if ((TRUE == require_explicit_port_map) 
         && (TRUE == cur_spice_model->dump_explicit_port_map)) {
        fprintf(fp, ")");
      }
    }
    /* Decide if we need a comma */
    dump_comma = TRUE; 
    /* Update counter */
    dumped_port_cnt++;

    /* Go to the next node */
    head = head->next;
  }

  /* We have dumped some port! 
   * Print another comment line  
   */
  if (0 < dumped_port_cnt) {
    fprintf(fp, "\n");
  }

  /* Free linked list */
  head = spice_model_head;
  while (head) {
    head->dptr = NULL;
    head = head->next;
  }
  free_llist(spice_model_head);

  return dumped_port_cnt;
}

/* Dump all the global ports that are stored in the linked list */
int dump_verilog_global_ports(FILE* fp, t_llist* head,
                               boolean dump_port_type) {
  t_llist* temp = head;
  t_spice_model_port* cur_global_port = NULL;
  int dumped_port_cnt = 0;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
  }

  /* fprintf(fp, "//----- BEGIN Global ports -----\n"); */
  while(NULL != temp) {
    cur_global_port = (t_spice_model_port*)(temp->dptr); 
    if (TRUE == dump_port_type) {
      fprintf(fp, "%s [0:%d] %s", 
              verilog_convert_port_type_to_string(cur_global_port->type),
              cur_global_port->size - 1, 
              cur_global_port->prefix);
    } else {
      fprintf(fp, "%s[0:%d]", 
              cur_global_port->prefix,
              cur_global_port->size - 1); 
    }
    /* if this is the tail, we do not dump a comma */
    if (NULL != temp->next) {
     fprintf(fp, ", //---- global port \n");
    }
    /* Update counter */
    dumped_port_cnt++;
    /* Go to the next */
    temp = temp->next;
  }
  /* fprintf(fp, "//----- END Global ports -----\n"); */

  return dumped_port_cnt;
}

/* Always dump the output ports of a SRAM in MUX */
void dump_verilog_mux_sram_one_outport(FILE* fp, 
                                       t_sram_orgz_info* cur_sram_orgz_info,
                                       t_spice_model* cur_mux_spice_model, int mux_size,
                                       int sram_lsb, int sram_msb,
                                       int port_type_index, 
                                       enum e_dump_verilog_port_type dump_port_type) {
  t_spice_model* mem_model = NULL;
  char* port_name = NULL;
  char* port_full_name = NULL;

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
      port_name = "out";
    } else if (1 == port_type_index) {
      port_name = "outb";
    } else {
      assert(-1 == port_type_index);
      port_name = "in";
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
  port_full_name = (char*)my_malloc(sizeof(char)*
                     (strlen(cur_mux_spice_model->prefix) + 5
                    + strlen(my_itoa(mux_size)) + 1
                    + strlen(my_itoa(cur_mux_spice_model->cnt)) + 1
                    + strlen(mem_model->prefix) + 1
                    + strlen(port_name) + 1));
  sprintf(port_full_name, "%s_size%d_%d_%s_%s", 
                          cur_mux_spice_model->prefix, mux_size,
                          cur_mux_spice_model->cnt,
                           mem_model->prefix, port_name);

  dump_verilog_generic_port(fp, dump_port_type, port_full_name, sram_lsb, sram_msb); 

  /* Free */
  /* Local variables such as port1_name and port2 name are automatically freed  */
  my_free(port_full_name);

  return;
}

/* Always dump the output ports of a SRAM in MUX */
void dump_verilog_mux_sram_one_local_outport(FILE* fp, 
                                             t_sram_orgz_info* cur_sram_orgz_info,
                                             t_spice_model* cur_mux_spice_model, int mux_size,
                                             int sram_lsb, int sram_msb,
                                             int port_type_index, 
                                             enum e_dump_verilog_port_type dump_port_type) {
  t_spice_model* mem_model = NULL;
  char* port_name = NULL;
  char* port_full_name = NULL;

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
      port_name = "out_local_bus";
    } else {
      assert(1 == port_type_index);
      port_name = "outb_local_bus";
    }
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    if (0 == port_type_index) {
      port_name = "out_local_bus";
    } else if (1 == port_type_index) {
      port_name = "outb_local_bus";
    } else {
      assert(-1 == port_type_index);
      port_name = "in_local_bus";
    }
    break;
  case SPICE_SRAM_MEMORY_BANK:
    if (0 == port_type_index) {
      port_name = "out_local_bus";
    } else {
      assert(1 == port_type_index);
      port_name = "outb_local_bus";
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /*Malloc and generate the full name of port */
  port_full_name = (char*)my_malloc(sizeof(char)*
                     (strlen(cur_mux_spice_model->prefix) + 5
                    + strlen(my_itoa(mux_size)) + 1
                    + strlen(my_itoa(cur_mux_spice_model->cnt)) + 1
                    + strlen(mem_model->prefix) + 1
                    + strlen(port_name) + 1));
  sprintf(port_full_name, "%s_size%d_%d_%s_%s", 
                          cur_mux_spice_model->prefix, mux_size,
                          cur_mux_spice_model->cnt,
                           mem_model->prefix, port_name);

  dump_verilog_generic_port(fp, dump_port_type, port_full_name, sram_lsb, sram_msb); 

  /* Free */
  /* Local variables such as port1_name and port2 name are automatically freed  */
  my_free(port_full_name);

  return;
}


/* Always dump the output ports of a SRAM */
void dump_verilog_sram_one_local_outport(FILE* fp, 
                                         t_sram_orgz_info* cur_sram_orgz_info,
                                         int sram_lsb, int sram_msb,
                                         int port_type_index, 
                                         enum e_dump_verilog_port_type dump_port_type) {
  t_spice_model* mem_model = NULL;
  char* port_name = NULL;
  char* port_full_name = NULL;

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
      port_name = "out_local_bus";
    } else {
      assert(1 == port_type_index);
      port_name = "outb_local_bus";
    }
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    if (0 == port_type_index) {
      port_name = "scff_out_local_bus";
    } else if (1 == port_type_index) {
      port_name = "scff_outb_local_bus";
    } else {
      assert(-1 == port_type_index);
      port_name = "scff_in_local_bus";
    }
    break;
  case SPICE_SRAM_MEMORY_BANK:
    if (0 == port_type_index) {
      port_name = "out_local_bus";
    } else {
      assert(1 == port_type_index);
      port_name = "outb_local_bus";
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /*Malloc and generate the full name of port */
  port_full_name = (char*)my_malloc(sizeof(char)*(strlen(mem_model->prefix) + strlen(port_name) + 1 + 1));
  sprintf(port_full_name, "%s_%s", mem_model->prefix, port_name);

  dump_verilog_generic_port(fp, dump_port_type, port_full_name, sram_lsb, sram_msb); 

  /* Free */
  /* Local variables such as port1_name and port2 name are automatically freed  */
  my_free(port_full_name);

  return;
}



/* Always dump the output ports of a SRAM */
void dump_verilog_sram_one_outport(FILE* fp, 
                                   t_sram_orgz_info* cur_sram_orgz_info,
                                   int sram_lsb, int sram_msb,
                                   int port_type_index, 
                                   enum e_dump_verilog_port_type dump_port_type) {
  t_spice_model* mem_model = NULL;
  char* port_name = NULL;
  char* port_full_name = NULL;

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
    } else if (1 == port_type_index) {
      port_name = "scff_outb";
    } else {
      assert(-1 == port_type_index);
      port_name = "scff_in";
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
  port_full_name = (char*)my_malloc(sizeof(char)*(strlen(mem_model->prefix) + strlen(port_name) + 1 + 1));
  sprintf(port_full_name, "%s_%s", mem_model->prefix, port_name);

  dump_verilog_generic_port(fp, dump_port_type, port_full_name, sram_lsb, sram_msb); 

  /* Free */
  /* Local variables such as port1_name and port2 name are automatically freed  */
  my_free(port_full_name);

  return;
}

/* Dump SRAM output ports, including two ports, out and outb */
void dump_verilog_sram_outports(FILE* fp, 
                                t_sram_orgz_info* cur_sram_orgz_info,
                                int sram_lsb, int sram_msb,
                                enum e_dump_verilog_port_type dump_port_type) {
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  if (0 > (sram_msb - sram_lsb)) {
    return;
  }

  if ((sram_lsb < 0)||(sram_msb < 0)) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid sram_lsb(%d) and sram_msb(%d)!\n",
               __FILE__, __LINE__, sram_lsb, sram_msb);
    return;
  }

  /* Dump the first port: SRAM_out of CMOS MUX or BL of RRAM MUX */ 
  dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, 
                                sram_lsb, sram_msb, 
                                0, dump_port_type);
  fprintf(fp, ",\n");
  /* Dump the first port: SRAM_outb of CMOS MUX or WL of RRAM MUX */ 
  dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, 
                                sram_lsb, sram_msb, 
                                1, dump_port_type);
  
  return;
}

/* Dump SRAM ports visible for formal verification purpose, 
 * which is supposed to be the last port in the port list */
void dump_verilog_formal_verification_sram_ports(FILE* fp, 
                                                 t_sram_orgz_info* cur_sram_orgz_info,
                                                 int sram_lsb, int sram_msb,
                                                 enum e_dump_verilog_port_type dump_port_type) {
  t_spice_model* mem_model = NULL;
  char* port_name = NULL;
  char* port_full_name = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Get memory_model */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
 
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    mem_model = cur_sram_orgz_info->standalone_sram_info->mem_model;
    port_name = "out_fm";
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    mem_model = cur_sram_orgz_info->scff_info->mem_model;
    port_name = "out_fm";
    break;
  case SPICE_SRAM_MEMORY_BANK:
    mem_model = cur_sram_orgz_info->mem_bank_info->mem_model;
    port_name = "out_fm";
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /*Malloc and generate the full name of port */
  port_full_name = (char*)my_malloc(sizeof(char)*(strlen(mem_model->prefix) + strlen(port_name) + 1 + 1));
  sprintf(port_full_name, "%s_%s", mem_model->prefix, port_name);

  dump_verilog_generic_port(fp, dump_port_type, port_full_name, sram_lsb, sram_msb); 

  /* Free */
  /* Local variables such as port1_name and port2 name are automatically freed  */
  my_free(port_full_name);

  return;
}


void dump_verilog_sram_one_port(FILE* fp, 
                                t_sram_orgz_info* cur_sram_orgz_info,
                                int sram_lsb, int sram_msb,
                                int port_type_index,
                                enum e_dump_verilog_port_type dump_port_type) {
  t_spice_model* mem_model = NULL;
  char* port_name = NULL;
  char* port_full_name = NULL;
  enum e_dump_verilog_port_type actual_dump_port_type = dump_port_type;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Get memory_model */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
 
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    mem_model = cur_sram_orgz_info->standalone_sram_info->mem_model;
    if (0 == port_type_index) {
      port_name = "out";
    } else {
      assert(1 == port_type_index);
      port_name = "outb";
    }
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    mem_model = cur_sram_orgz_info->scff_info->mem_model;
    if (0 == port_type_index) {
      port_name = "scff_head";
    } else if (1 == port_type_index) {
      assert(1 == port_type_index);
      port_name = "scff_tail";
      /* Special case: scan-chain ff output should be an output always */
      if (VERILOG_PORT_INPUT == dump_port_type) {
        actual_dump_port_type = VERILOG_PORT_OUTPUT;
      }
    }
    break;
  case SPICE_SRAM_MEMORY_BANK:
    mem_model = cur_sram_orgz_info->mem_bank_info->mem_model;
    if (0 == port_type_index) {
      port_name = "bl";
    } else if (1 == port_type_index) {
      port_name = "wl";
    /* Create inverted BL and WL signals */
    } else if (2 == port_type_index) {
      port_name = "blb";
    } else if (3 == port_type_index) {
      port_name = "wlb";
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /*Malloc and generate the full name of port */
  port_full_name = (char*)my_malloc(sizeof(char)*(strlen(mem_model->prefix) + strlen(port_name) + 1 + 1));
  sprintf(port_full_name, "%s_%s", mem_model->prefix, port_name);

  dump_verilog_generic_port(fp, actual_dump_port_type, port_full_name, sram_lsb, sram_msb); 

  /* Free */
  /* Local variables such as port1_name and port2 name are automatically freed  */
  my_free(port_full_name);

  return;
}

/* Wire SRAM ports in formal verififcation purpose to regular SRAM ports */
void dump_verilog_formal_verification_sram_ports_wiring(FILE* fp, 
                                                        t_sram_orgz_info* cur_sram_orgz_info,
                                                        int sram_lsb, int sram_msb) {
  fprintf(fp, "assign ");

  dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info,
                                      sram_lsb, sram_msb,
                                      0, VERILOG_PORT_CONKT);

  fprintf(fp, " = ");

  dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info,
                                              sram_lsb, sram_msb,
                                              VERILOG_PORT_CONKT);
  fprintf(fp, ";\n");

  return;
}

/* Wire SRAM ports in formal verififcation purpose to regular SRAM ports */
void dump_verilog_formal_verification_mux_sram_ports_wiring(FILE* fp, 
                                                            t_sram_orgz_info* cur_sram_orgz_info,
                                                            t_spice_model* cur_mux_spice_model, int mux_size,
                                                            int sram_lsb, int sram_msb) {
  fprintf(fp, "assign ");

  dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info,
                                    cur_mux_spice_model, mux_size,
                                    sram_lsb, sram_msb,
                                    0, VERILOG_PORT_CONKT);

  fprintf(fp, " = ");

  dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info,
                                              sram_lsb, sram_msb,
                                              VERILOG_PORT_CONKT);
  fprintf(fp, ";\n");

  return;
}

/* Dump SRAM ports, which is supposed to be the last port in the port list */
void dump_verilog_sram_local_ports(FILE* fp, 
                                   t_sram_orgz_info* cur_sram_orgz_info,
                                   int sram_lsb, int sram_msb,
                                   enum e_dump_verilog_port_type dump_port_type) {
  /* Need to dump inverted BL/WL if needed */
  int num_blb_ports, num_wlb_ports;
  t_spice_model_port** blb_port = NULL;
  t_spice_model_port** wlb_port = NULL;
  t_spice_model* cur_sram_verilog_model = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  if (0 > (sram_msb - sram_lsb)) {
    return;
  }

  if ((sram_lsb < 0)||(sram_msb < 0)) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid sram_lsb(%d) and sram_msb(%d)!\n",
               __FILE__, __LINE__, sram_lsb, sram_msb);
    return;
  }
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_MEMORY_BANK:
    /* Dump the first port: SRAM_out of CMOS MUX or BL of RRAM MUX */ 
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                               sram_lsb, sram_msb, 
                               0, dump_port_type);
    fprintf(fp, ",\n");
    /* Dump the first port: SRAM_outb of CMOS MUX or WL of RRAM MUX */ 
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                               sram_lsb, sram_msb, 
                               1, dump_port_type);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* Dump the first port: SRAM_out of CMOS MUX or BL of RRAM MUX */ 
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 
                                        sram_lsb, sram_lsb, 
                                        -1, dump_port_type);
    fprintf(fp, ",\n");
    /* Dump the first port: SRAM_outb of CMOS MUX or WL of RRAM MUX */ 
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 
                                        sram_msb, sram_msb, 
                                        0, dump_port_type);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Find the BLB and WLB port, if there is any */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &cur_sram_verilog_model);
  find_blb_wlb_ports_spice_model(cur_sram_verilog_model, 
                                 &num_blb_ports, &blb_port, &num_wlb_ports, &wlb_port); 
  /* BL inverted port */
  if (1 == num_blb_ports) {
    fprintf(fp, ",\n");
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                               sram_lsb, sram_msb, 
                               2, dump_port_type);
  }
  /* WL inverted port */
  if (1 == num_wlb_ports) {
    fprintf(fp, ",\n");
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                               sram_lsb, sram_msb, 
                               3, dump_port_type);
  }
  
  return;
}


/* Dump SRAM ports, which is supposed to be the last port in the port list */
void dump_verilog_sram_ports(FILE* fp, 
                             t_sram_orgz_info* cur_sram_orgz_info,
                             int sram_lsb, int sram_msb,
                             enum e_dump_verilog_port_type dump_port_type) {
  /* Need to dump inverted BL/WL if needed */
  int num_blb_ports, num_wlb_ports;
  t_spice_model_port** blb_port = NULL;
  t_spice_model_port** wlb_port = NULL;
  t_spice_model* cur_sram_verilog_model = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  if (0 > (sram_msb - sram_lsb)) {
    return;
  }

  if ((sram_lsb < 0)||(sram_msb < 0)) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid sram_lsb(%d) and sram_msb(%d)!\n",
               __FILE__, __LINE__, sram_lsb, sram_msb);
    return;
  }
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_MEMORY_BANK:
    /* Dump the first port: SRAM_out of CMOS MUX or BL of RRAM MUX */ 
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                               sram_lsb, sram_msb, 
                               0, dump_port_type);
    fprintf(fp, ",\n");
    /* Dump the first port: SRAM_outb of CMOS MUX or WL of RRAM MUX */ 
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                               sram_lsb, sram_msb, 
                               1, dump_port_type);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* Dump the first port: SRAM_out of CMOS MUX or BL of RRAM MUX */ 
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                               sram_lsb, sram_lsb, 
                               0, dump_port_type);
    fprintf(fp, ",\n");
    /* Dump the first port: SRAM_outb of CMOS MUX or WL of RRAM MUX */ 
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                               sram_msb, sram_msb, 
                               1, dump_port_type);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Find the BLB and WLB port, if there is any */
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &cur_sram_verilog_model);
  find_blb_wlb_ports_spice_model(cur_sram_verilog_model, 
                                 &num_blb_ports, &blb_port, &num_wlb_ports, &wlb_port); 
  /* BL inverted port */
  if (1 == num_blb_ports) {
    fprintf(fp, ",\n");
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                               sram_lsb, sram_msb, 
                               2, dump_port_type);
  }
  /* WL inverted port */
  if (1 == num_wlb_ports) {
    fprintf(fp, ",\n");
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                               sram_lsb, sram_msb, 
                               3, dump_port_type);
  }
  
  return;
}

void dump_verilog_reserved_sram_one_port(FILE* fp, 
                                         t_sram_orgz_info* cur_sram_orgz_info,
                                         int sram_lsb, int sram_msb,
                                         int port_type_index,
                                         enum e_dump_verilog_port_type dump_port_type) {
  t_spice_model* mem_model = NULL;
  char* port_name = NULL;
  char* port_full_name = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
 
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    return;
  case SPICE_SRAM_MEMORY_BANK:
    get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
    if (0 == port_type_index) {
      port_name = "reserved_bl";
    } else {
      assert(1 == port_type_index);
      port_name = "reserved_wl";
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /*Malloc and generate the full name of port */
  port_full_name = (char*)my_malloc(sizeof(char)*(strlen(mem_model->prefix) + strlen(port_name) + 1 + 1));
  sprintf(port_full_name, "%s_%s", mem_model->prefix, port_name);

  dump_verilog_generic_port(fp, dump_port_type, port_full_name, sram_lsb, sram_msb); 

  /* Free */
  /* Local variables such as port1_name and port2 name are automatically freed  */
  my_free(port_full_name);

  return;
}


/* Dump SRAM ports, which is supposed to be the last port in the port list */
void dump_verilog_reserved_sram_ports(FILE* fp, 
                                      t_sram_orgz_info* cur_sram_orgz_info,
                                      int sram_lsb, int sram_msb,
                                      enum e_dump_verilog_port_type dump_port_type) {
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  if (0 > (sram_msb - sram_lsb)) {
    return;
  }

  /* Dump the first port: SRAM_out of CMOS MUX or BL of RRAM MUX */ 
  
  if ((sram_lsb < 0)||(sram_msb < 0)) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid sram_lsb(%d) and sram_msb(%d)!\n",
               __FILE__, __LINE__, sram_lsb, sram_msb);
    return;
  }

  dump_verilog_reserved_sram_one_port(fp, cur_sram_orgz_info,
                                      sram_lsb, sram_msb,
                                      0, dump_port_type);
  fprintf(fp, ",\n");
  /* Dump the first port: SRAM_outb of CMOS MUX or WL of RRAM MUX */ 
  dump_verilog_reserved_sram_one_port(fp, cur_sram_orgz_info,
                                      sram_lsb, sram_msb,
                                      1, dump_port_type);
 
  return;
}


/* Dump a verilog submodule of SRAMs in MUX, according to SRAM organization type */
void dump_verilog_mux_sram_submodule(FILE* fp, t_sram_orgz_info* cur_sram_orgz_info,
                                     t_spice_model* cur_mux_verilog_model, int mux_size,
                                     t_spice_model* cur_sram_verilog_model) {
  int cur_bl, cur_wl, cur_num_sram;
  int num_bl_ports, num_wl_ports;
  t_spice_model_port** bl_port = NULL;
  t_spice_model_port** wl_port = NULL;
  int num_blb_ports, num_wlb_ports;
  t_spice_model_port** blb_port = NULL;
  t_spice_model_port** wlb_port = NULL;

  int num_bl_per_sram = 0;
  int num_wl_per_sram = 0;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  assert(NULL != cur_sram_orgz_info);
  assert(NULL != cur_sram_verilog_model);
  assert((SPICE_MODEL_SRAM == cur_sram_verilog_model->type)
        || (SPICE_MODEL_SCFF == cur_sram_verilog_model->type));

  /* Get current index of SRAM module */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 

  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_MEMORY_BANK:
    /* Detect the SRAM SPICE model linked to this SRAM port */
    find_bl_wl_ports_spice_model(cur_sram_verilog_model, 
                                 &num_bl_ports, &bl_port, &num_wl_ports, &wl_port); 
    assert(1 == num_bl_ports);
    assert(1 == num_wl_ports);
    num_bl_per_sram = bl_port[0]->size; 
    num_wl_per_sram = wl_port[0]->size; 
    /* Find the BLB and WLB port, if there is any */
    find_blb_wlb_ports_spice_model(cur_sram_verilog_model, 
                                   &num_blb_ports, &blb_port, &num_wlb_ports, &wlb_port); 
    if (1 == num_blb_ports) {
      assert(num_bl_per_sram == blb_port[0]->size);
    } else {
      assert(0 == num_blb_ports);
    }
    if (1 == num_wlb_ports) {
      assert(num_wl_per_sram == wlb_port[0]->size);
    } else {
      assert(0 == num_wlb_ports);
    }

    /* SRAM subckts*/
    fprintf(fp, "%s %s_%d_ (", cur_sram_verilog_model->name, cur_sram_verilog_model->prefix, 
                               cur_sram_verilog_model->cnt); 
    /* Only dump the global ports belonging to a spice_model */
    if (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_sram_verilog_model, FALSE, TRUE, FALSE)) {
      fprintf(fp, ",\n");
    }
    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      cur_mux_verilog_model, mux_size,
                                      cur_num_sram, cur_num_sram,
                                      0, VERILOG_PORT_CONKT);
    fprintf(fp, ",");
    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      cur_mux_verilog_model, mux_size,
                                      cur_num_sram, cur_num_sram,
                                      0, VERILOG_PORT_CONKT);
    fprintf(fp, ",");
    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      cur_mux_verilog_model, mux_size,
                                      cur_num_sram, cur_num_sram,
                                      1, VERILOG_PORT_CONKT);
    fprintf(fp, ",");
    get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl); 
    /* Connect to Bit lines and Word lines, consider each conf_bit */
    fprintf(fp, "%s_size%d_%d_configbus0[%d:%d], ", 
            cur_mux_verilog_model->prefix, mux_size, cur_mux_verilog_model->cnt, 
            cur_num_sram, cur_num_sram + num_bl_per_sram - 1); 
    fprintf(fp, "%s_size%d_%d_configbus1[%d:%d] ", 
            cur_mux_verilog_model->prefix, mux_size, cur_mux_verilog_model->cnt, 
            cur_num_sram, cur_num_sram + num_wl_per_sram - 1); /* Outputs */
    /* If we have a BLB or WLB, we need to dump inverted config_bus */
    if (1 == num_blb_ports) { 
      fprintf(fp, ", ");
      fprintf(fp, "%s_size%d_%d_configbus0_b[%d:%d] ", 
              cur_mux_verilog_model->prefix, mux_size, cur_mux_verilog_model->cnt, 
              cur_num_sram, cur_num_sram + num_bl_per_sram - 1); 
    }
    if (1 == num_wlb_ports) { 
      fprintf(fp, ", ");
      fprintf(fp, "%s_size%d_%d_configbus1_b[%d:%d] ", 
              cur_mux_verilog_model->prefix, mux_size, cur_mux_verilog_model->cnt, 
              cur_num_sram, cur_num_sram + num_wl_per_sram - 1); 
    }

    fprintf(fp, ");\n");  //
    /* Update the counter */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info,
                                      cur_num_sram + 1);
    update_sram_orgz_info_num_blwl(cur_sram_orgz_info, 
                                   cur_bl + 1,
                                   cur_wl + 1);
    break;
  case SPICE_SRAM_STANDALONE:
    /* SRAM subckts*/
    fprintf(fp, "%s %s_%d_ (", cur_sram_verilog_model->name, cur_sram_verilog_model->prefix, 
                               cur_sram_verilog_model->cnt); 
    /* Only dump the global ports belonging to a spice_model */
    if (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_sram_verilog_model, FALSE, TRUE, FALSE)) {
      fprintf(fp, ",\n");
    }
    fprintf(fp, "%s_out[%d], ", cur_sram_verilog_model->prefix, cur_sram_verilog_model->cnt); /* Input*/
    fprintf(fp, "%s_out[%d], %s_outb[%d] ", 
            cur_sram_verilog_model->prefix, cur_sram_verilog_model->cnt, 
            cur_sram_verilog_model->prefix, cur_sram_verilog_model->cnt); /* Outputs */
    fprintf(fp, ");\n");  //
    /* Update the counter */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info,
                                      cur_num_sram + 1);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* Add a scan-chain DFF module here ! */
    fprintf(fp, "%s %s_%d_ (", cur_sram_verilog_model->name, cur_sram_verilog_model->prefix, 
                               cur_sram_verilog_model->cnt); 
    /* Only dump the global ports belonging to a spice_model */
    if (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_sram_verilog_model, FALSE, TRUE, FALSE)) {
      fprintf(fp, ",\n");
    }
    /* Input of Scan-chain DFF, should be connected to the output of its precedent */
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info,
                               cur_num_sram,
                               cur_num_sram,
                               0, VERILOG_PORT_CONKT);
    fprintf(fp, ", \n");  //
    /* Output of Scan-chain DFF, should be connected to the output of its successor */
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info,
                               cur_num_sram,
                               cur_num_sram,
                               1, VERILOG_PORT_CONKT);
    fprintf(fp, ", \n");  //
    /* Memory outputs of Scan-chain DFF, should be connected to the SRAM(memory port) of IOPAD, MUX and LUT */
    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      cur_mux_verilog_model, mux_size,
                                      cur_num_sram,
                                      cur_num_sram,
                                      1, VERILOG_PORT_CONKT);
    fprintf(fp, ");\n");  //
    /* Update the counter */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info,
                                      cur_num_sram + 1);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Update the counter */
  cur_sram_verilog_model->cnt++;

  return;
}



/* Dump a verilog submodule of SRAM, according to SRAM organization type */
void dump_verilog_sram_submodule(FILE* fp, t_sram_orgz_info* cur_sram_orgz_info,
                                 t_spice_model* cur_sram_verilog_model) {
  int cur_bl, cur_wl, cur_num_sram;
  int num_bl_ports, num_wl_ports;
  t_spice_model_port** bl_port = NULL;
  t_spice_model_port** wl_port = NULL;
  int num_blb_ports, num_wlb_ports;
  t_spice_model_port** blb_port = NULL;
  t_spice_model_port** wlb_port = NULL;

  int num_bl_per_sram = 0;
  int num_wl_per_sram = 0;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  assert(NULL != cur_sram_orgz_info);
  assert(NULL != cur_sram_verilog_model);
  assert((SPICE_MODEL_SRAM == cur_sram_verilog_model->type)
        || (SPICE_MODEL_SCFF == cur_sram_verilog_model->type));

  /* Get current index of SRAM module */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 

  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_MEMORY_BANK:
    /* Detect the SRAM SPICE model linked to this SRAM port */
    find_bl_wl_ports_spice_model(cur_sram_verilog_model, 
                                 &num_bl_ports, &bl_port, &num_wl_ports, &wl_port); 
    assert(1 == num_bl_ports);
    assert(1 == num_wl_ports);
    num_bl_per_sram = bl_port[0]->size; 
    num_wl_per_sram = wl_port[0]->size; 
    /* Find the BLB and WLB port, if there is any */
    find_blb_wlb_ports_spice_model(cur_sram_verilog_model, 
                                   &num_blb_ports, &blb_port, &num_wlb_ports, &wlb_port); 
    if (1 == num_blb_ports) {
      assert(num_bl_per_sram == blb_port[0]->size);
    } else {
      assert(0 == num_blb_ports);
    }
    if (1 == num_wlb_ports) {
      assert(num_wl_per_sram == wlb_port[0]->size);
    } else {
      assert(0 == num_wlb_ports);
    }

    /* SRAM subckts*/
    fprintf(fp, "%s %s_%d_ (", cur_sram_verilog_model->name, cur_sram_verilog_model->prefix, 
                               cur_sram_verilog_model->cnt); 
    /* Only dump the global ports belonging to a spice_model */
    if (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_sram_verilog_model, FALSE, TRUE, FALSE)) {
      fprintf(fp, ",\n");
    }
    fprintf(fp, "%s_out[%d], ", cur_sram_verilog_model->prefix, cur_num_sram); /* Input*/
    fprintf(fp, "%s_out[%d], %s_outb[%d], ", 
            cur_sram_verilog_model->prefix, cur_num_sram, 
            cur_sram_verilog_model->prefix, cur_num_sram); /* Outputs */
    get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl); 
    /* Connect to Bit lines and Word lines, consider each conf_bit */
    fprintf(fp, "%s_%d_configbus0[%d:%d], ", 
            cur_sram_verilog_model->prefix, cur_sram_verilog_model->cnt, 
            cur_bl, cur_bl + num_bl_per_sram - 1); 
    fprintf(fp, "%s_%d_configbus1[%d:%d] ", 
            cur_sram_verilog_model->prefix, cur_sram_verilog_model->cnt, 
            cur_wl, cur_wl + num_wl_per_sram - 1); /* Outputs */
    if (1 == num_blb_ports) {
      fprintf(fp, ", ");
      fprintf(fp, "%s_%d_configbus0_b[%d:%d] ", 
              cur_sram_verilog_model->prefix, cur_sram_verilog_model->cnt, 
              cur_bl, cur_bl + num_bl_per_sram - 1); 
    }
    if (1 == num_wlb_ports) {
      fprintf(fp, ", ");
      fprintf(fp, "%s_%d_configbus1_b[%d:%d] ", 
              cur_sram_verilog_model->prefix, cur_sram_verilog_model->cnt, 
              cur_wl, cur_wl + num_wl_per_sram - 1); /* Outputs */
    }
    fprintf(fp, ");\n");  //
    /* Update the counter */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info,
                                      cur_num_sram + 1);
    update_sram_orgz_info_num_blwl(cur_sram_orgz_info, 
                                   cur_bl + 1,
                                   cur_wl + 1);
    break;
  case SPICE_SRAM_STANDALONE:
    /* SRAM subckts*/
    fprintf(fp, "%s %s_%d_ (", cur_sram_verilog_model->name, cur_sram_verilog_model->prefix, 
                               cur_sram_verilog_model->cnt); 
    /* Only dump the global ports belonging to a spice_model */
    if (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_sram_verilog_model, FALSE, TRUE, FALSE)) {
      fprintf(fp, ",\n");
    }
    fprintf(fp, "%s_out[%d], ", cur_sram_verilog_model->prefix, cur_sram_verilog_model->cnt); /* Input*/
    fprintf(fp, "%s_out[%d], %s_outb[%d] ", 
            cur_sram_verilog_model->prefix, cur_sram_verilog_model->cnt, 
            cur_sram_verilog_model->prefix, cur_sram_verilog_model->cnt); /* Outputs */
    fprintf(fp, ");\n");  //
    /* Update the counter */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info,
                                      cur_num_sram + 1);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* Add a scan-chain DFF module here ! */
    fprintf(fp, "%s %s_%d_ (", cur_sram_verilog_model->name, cur_sram_verilog_model->prefix, 
                               cur_sram_verilog_model->cnt); 
    /* Only dump the global ports belonging to a spice_model */
    if (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_sram_verilog_model, FALSE, TRUE, FALSE)) {
      fprintf(fp, ",\n");
    }
   /* Input of Scan-chain DFF, should be connected to the output of its precedent */
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info,
                               cur_num_sram,
                               cur_num_sram,
                               0, VERILOG_PORT_CONKT);
    fprintf(fp, ", \n");  //
    /* Output of Scan-chain DFF, should be connected to the output of its successor */
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info,
                               cur_num_sram,
                               cur_num_sram,
                               1, VERILOG_PORT_CONKT);
    fprintf(fp, ", \n");  //
    /* Memory outputs of Scan-chain DFF, should be connected to the SRAM(memory port) of IOPAD, MUX and LUT */
    dump_verilog_sram_one_outport(fp,  
                                  cur_sram_orgz_info,
                                  cur_num_sram, cur_num_sram,
                                  1, VERILOG_PORT_CONKT);
    fprintf(fp, ");\n");  //
    /* Update the counter */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info,
                                      cur_num_sram + 1);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Update the counter */
  cur_sram_verilog_model->cnt++;

  return;
}

void dump_verilog_scff_config_bus(FILE* fp,
                                 t_spice_model* mem_spice_model, 
                                 t_sram_orgz_info* cur_sram_orgz_info,
                                 int lsb, int msb,
                                 enum e_dump_verilog_port_type dump_port_type) {
  char* port_full_name = NULL;   
 
  /* Check */
  assert(NULL != mem_spice_model);
  assert(SPICE_MODEL_SCFF == mem_spice_model->type);

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /*Malloc and generate the full name of port */
  port_full_name = (char*)my_malloc(sizeof(char)*
                                   ( strlen(mem_spice_model->prefix) + 1
                                   + strlen(my_itoa(mem_spice_model->cnt)) + 12 + 1));
  sprintf(port_full_name, "%s_%d_config_bus0", mem_spice_model->prefix, mem_spice_model->cnt);

  dump_verilog_generic_port(fp, dump_port_type, port_full_name, lsb, msb); 

  /* Free */
  /* Local variables such as port1_name and port2 name are automatically freed  */
  my_free(port_full_name);

  return;
}

/* Dump MUX reserved and normal configuration wire bus */
void dump_verilog_mem_config_bus(FILE* fp, t_spice_model* mem_spice_model, 
                                 t_sram_orgz_info* cur_sram_orgz_info,
                                 int cur_num_sram,
                                 int num_mem_reserved_conf_bits,
                                 int num_mem_conf_bits) { 
  int num_blb_ports, num_wlb_ports;
  t_spice_model_port** blb_port = NULL;
  t_spice_model_port** wlb_port = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert(NULL != mem_spice_model);
  assert((SPICE_MODEL_SRAM == mem_spice_model->type)
        || (SPICE_MODEL_SCFF == mem_spice_model->type));

  /* Depend on the style of configuraion circuit */
  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* We need to connect SCFF inputs and outputs in cacading
     * Scan-chain FF outputs are directly wired to SRAM inputs of MUXes  
     */ 
    /* Connect first SCFF to the head */
    /*
    fprintf(fp, "assign ");
    dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, cur_num_sram, cur_num_sram, -1, VERILOG_PORT_CONKT); 
    fprintf(fp, " = ");
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, cur_num_sram, cur_num_sram, 0, VERILOG_PORT_CONKT); 
    fprintf(fp, ";\n");
     */ 
    /* Connect last SCFF to the tail */
    /*
    fprintf(fp, "assign ");
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, cur_num_sram + num_mem_conf_bits - 1, cur_num_sram + num_mem_conf_bits - 1, 1, VERILOG_PORT_CONKT); 
    fprintf(fp, " = ");
    dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, cur_num_sram + num_mem_conf_bits - 1, cur_num_sram + num_mem_conf_bits - 1, 0, VERILOG_PORT_CONKT); 
    fprintf(fp, ";\n");
     */ 
    /* Connect SCFFs into chains */
    /* Cascade the SCFF between head and tail */
    /*
    if (1 < num_mem_conf_bits) {
      fprintf(fp, "assign ");
      dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, 
                                    cur_num_sram + 1, cur_num_sram + num_mem_conf_bits - 1,
                                    -1, VERILOG_PORT_CONKT);
      fprintf(fp, " = ");
      dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, 
                                    cur_num_sram, cur_num_sram + num_mem_conf_bits - 2,
                                    0, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
    }
     */ 
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* Find the BLB and WLB port, if there is any */
    find_blb_wlb_ports_spice_model(mem_spice_model, 
                                   &num_blb_ports, &blb_port, &num_wlb_ports, &wlb_port); 
    /* configuration wire bus */
    if (0 < (num_mem_reserved_conf_bits + num_mem_conf_bits)) {
      /* First bus is for sram_out in CMOS MUX or BL in RRAM MUX */
      fprintf(fp, "wire [%d:%d] %s_%d_configbus0;\n",
              cur_num_sram,
              cur_num_sram + num_mem_reserved_conf_bits + num_mem_conf_bits - 1,
              mem_spice_model->prefix, mem_spice_model->cnt);
      /* Second bus is for sram_out_inv in CMOS MUX or WL in RRAM MUX */
      fprintf(fp, "wire [%d:%d] %s_%d_configbus1;\n",
              cur_num_sram,
              cur_num_sram + num_mem_reserved_conf_bits + num_mem_conf_bits - 1,
              mem_spice_model->prefix, mem_spice_model->cnt);
      if (1 == num_blb_ports) {
        fprintf(fp, "wire [%d:%d] %s_%d_configbus0_b;\n",
                cur_num_sram,
                cur_num_sram + num_mem_reserved_conf_bits + num_mem_conf_bits - 1,
                mem_spice_model->prefix, mem_spice_model->cnt);
      }
      if (1 == num_wlb_ports) {
        fprintf(fp, "wire [%d:%d] %s_%d_configbus1_b;\n",
                cur_num_sram,
                cur_num_sram + num_mem_reserved_conf_bits + num_mem_conf_bits - 1,
                mem_spice_model->prefix, mem_spice_model->cnt);
      }
    }
    /* Connect wires to config bus */
    /* reserved configuration bits */
    if (0 < num_mem_reserved_conf_bits) {
      fprintf(fp, "assign %s_%d_configbus0[%d:%d] = ",
              mem_spice_model->prefix, mem_spice_model->cnt,
              cur_num_sram,
              cur_num_sram + num_mem_reserved_conf_bits - 1);
      dump_verilog_reserved_sram_one_port(fp, cur_sram_orgz_info, 
                                          0, num_mem_reserved_conf_bits - 1,
                                          0, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
      fprintf(fp, "assign %s_%d_configbus1[%d:%d] = ",
              mem_spice_model->prefix, mem_spice_model->cnt,
              cur_num_sram,
              cur_num_sram + num_mem_reserved_conf_bits - 1);
      dump_verilog_reserved_sram_one_port(fp, cur_sram_orgz_info, 
                                          0, num_mem_reserved_conf_bits - 1,
                                          1, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
    }
    /* normal configuration bits */
    if (0 < num_mem_conf_bits) {
      fprintf(fp, "assign %s_%d_configbus0[%d:%d] = ",
              mem_spice_model->prefix, mem_spice_model->cnt,
              cur_num_sram + num_mem_reserved_conf_bits,
              cur_num_sram + num_mem_reserved_conf_bits + num_mem_conf_bits - 1);
      dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                                 cur_num_sram, cur_num_sram + num_mem_conf_bits - 1,
                                 0, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
      fprintf(fp, "assign %s_%d_configbus1[%d:%d] = ",
              mem_spice_model->prefix,  mem_spice_model->cnt,
              cur_num_sram + num_mem_reserved_conf_bits,
              cur_num_sram + num_mem_reserved_conf_bits + num_mem_conf_bits - 1);
      dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                                 cur_num_sram, cur_num_sram + num_mem_conf_bits - 1,
                                 1, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
      /* Dump inverted config_bus if needed */
      if (1 == num_blb_ports) {
        fprintf(fp, "assign %s_%d_configbus0_b[%d:%d] = ",
                mem_spice_model->prefix, mem_spice_model->cnt,
                cur_num_sram + num_mem_reserved_conf_bits,
                cur_num_sram + num_mem_reserved_conf_bits + num_mem_conf_bits - 1);
        dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                                   cur_num_sram, cur_num_sram + num_mem_conf_bits - 1,
                                   2, VERILOG_PORT_CONKT);
        fprintf(fp, ";\n");
      }
      if (1 == num_wlb_ports) {
        fprintf(fp, "assign %s_%d_configbus1_b[%d:%d] = ",
                mem_spice_model->prefix,  mem_spice_model->cnt,
                cur_num_sram + num_mem_reserved_conf_bits,
                cur_num_sram + num_mem_reserved_conf_bits + num_mem_conf_bits - 1);
        dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                                   cur_num_sram, cur_num_sram + num_mem_conf_bits - 1,
                                   3, VERILOG_PORT_CONKT);
        fprintf(fp, ";\n");
      }
    }
      break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* Dump MUX reserved and normal configuration wire bus */
void dump_verilog_cmos_mux_config_bus(FILE* fp, t_spice_model* mux_spice_model, 
                                      t_sram_orgz_info* cur_sram_orgz_info,
                                      int mux_size, int cur_num_sram,
                                      int num_mux_reserved_conf_bits,
                                      int num_mux_conf_bits) { 
  int num_blb_ports, num_wlb_ports;
  t_spice_model_port** blb_port = NULL;
  t_spice_model_port** wlb_port = NULL;
  t_spice_model* cur_sram_verilog_model = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert(NULL != mux_spice_model);
  assert(SPICE_MODEL_MUX == mux_spice_model->type);

  switch(cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* We do not need any configuration bus
     * Scan-chain FF outputs are directly wired to SRAM inputs of MUXes  
     */ 
    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      mux_spice_model, mux_size,
                                      cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                      -1, VERILOG_PORT_WIRE);
    fprintf(fp, ";\n");

    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      mux_spice_model, mux_size,
                                      cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                      0, VERILOG_PORT_WIRE);
    fprintf(fp, ";\n");

    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      mux_spice_model, mux_size,
                                      cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                      1, VERILOG_PORT_WIRE);
    fprintf(fp, ";\n");
    /* We need to connect SCFF inputs and outputs in cacading
     * Scan-chain FF outputs are directly wired to SRAM inputs of MUXes  
     */ 
    /* Connect first SCFF to the head */
    fprintf(fp, "assign ");
    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      mux_spice_model, mux_size,
                                      cur_num_sram, cur_num_sram, 
                                      -1, VERILOG_PORT_CONKT); 
    fprintf(fp, " = ");
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 
                                        cur_num_sram, cur_num_sram, 
                                        -1, VERILOG_PORT_CONKT); 
    fprintf(fp, ";\n");
    /* Connect last SCFF to the tail */
    fprintf(fp, "assign ");
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 
                                        cur_num_sram + num_mux_conf_bits - 1, cur_num_sram + num_mux_conf_bits - 1, 
                                        0, VERILOG_PORT_CONKT); 
    fprintf(fp, " = ");
    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      mux_spice_model, mux_size,
                                      cur_num_sram + num_mux_conf_bits - 1, cur_num_sram + num_mux_conf_bits - 1,
                                      0, VERILOG_PORT_CONKT); 
    fprintf(fp, ";\n");
    /* Connect SCFFs into chains */
    /* Connect the first SCFF (LSB) to the head */
    /* Connect the last SCFF (MSB) to the tail */
    /* Cascade the SCFF between head and tail */
    if (1 < num_mux_conf_bits) {
      fprintf(fp, "assign ");
      dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                        mux_spice_model, mux_size,
                                        cur_num_sram + 1, cur_num_sram + num_mux_conf_bits - 1,
                                        -1, VERILOG_PORT_CONKT);
      fprintf(fp, " = ");
      dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                        mux_spice_model, mux_size,
                                        cur_num_sram, cur_num_sram + num_mux_conf_bits - 2,
                                        0, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
    }
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* configuration wire bus */
    /* First bus is for sram_out in CMOS MUX */
    fprintf(fp, "wire [%d:%d] %s_size%d_%d_configbus0;\n",
                cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                mux_spice_model->prefix, mux_size, mux_spice_model->cnt);
    /* Second bus is for sram_out_inv in CMOS MUX */
    fprintf(fp, "wire [%d:%d] %s_size%d_%d_configbus1;\n",
                cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                mux_spice_model->prefix, mux_size, mux_spice_model->cnt);
    /* Declare output ports as wires */
    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      mux_spice_model, mux_size,
                                      cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                      0, VERILOG_PORT_WIRE);
    fprintf(fp, ";\n");
    /* Declare output ports as wires */
    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      mux_spice_model, mux_size,
                                      cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                      1, VERILOG_PORT_WIRE);
    fprintf(fp, ";\n");
    /* Connect wires to config bus */
    fprintf(fp, "assign %s_size%d_%d_configbus0[%d:%d] = ",
                mux_spice_model->prefix, mux_size, mux_spice_model->cnt,
                cur_num_sram, cur_num_sram + num_mux_conf_bits - 1);
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                               cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                               0, VERILOG_PORT_CONKT);
    fprintf(fp, ";\n");
    fprintf(fp, "assign %s_size%d_%d_configbus1[%d:%d] = ",
            mux_spice_model->prefix, mux_size, mux_spice_model->cnt,
            cur_num_sram, cur_num_sram + num_mux_conf_bits - 1);
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                               cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                               1, VERILOG_PORT_CONKT);
    fprintf(fp, ";\n");
    /* Find the BLB and WLB port, if there is any */
    get_sram_orgz_info_mem_model(cur_sram_orgz_info, &cur_sram_verilog_model);
    find_blb_wlb_ports_spice_model(cur_sram_verilog_model, 
                                   &num_blb_ports, &blb_port, &num_wlb_ports, &wlb_port); 
    /* Dump inverted config_bus if needed */
    if (1 == num_blb_ports) {
      fprintf(fp, "wire [%d:%d] %s_size%d_%d_configbus0_b;\n",
                  cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                  mux_spice_model->prefix, mux_size, mux_spice_model->cnt);
      fprintf(fp, "assign %s_size%d_%d_configbus0_b[%d:%d] = ",
                  mux_spice_model->prefix, mux_size, mux_spice_model->cnt,
                  cur_num_sram, cur_num_sram + num_mux_conf_bits - 1);
       dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                                  cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                  2, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
    }
    if (1 == num_wlb_ports) {
    fprintf(fp, "wire [%d:%d] %s_size%d_%d_configbus1_b;\n",
                cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                mux_spice_model->prefix, mux_size, mux_spice_model->cnt);
      fprintf(fp, "assign %s_size%d_%d_configbus1_b[%d:%d] = ",
              mux_spice_model->prefix, mux_size, mux_spice_model->cnt,
              cur_num_sram, cur_num_sram + num_mux_conf_bits - 1);
      dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                                 cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                 3, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid SRAM organization!\n", 
               __FILE__, __LINE__);
    exit(1);
  }
  return;

}

/* Dump MUX reserved and normal configuration wire bus */
void dump_verilog_mux_config_bus(FILE* fp, t_spice_model* mux_spice_model, 
                                 t_sram_orgz_info* cur_sram_orgz_info,
                                 int mux_size, int cur_num_sram,
                                 int num_mux_reserved_conf_bits,
                                 int num_mux_conf_bits) { 
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert(NULL != mux_spice_model);
  assert(SPICE_MODEL_MUX == mux_spice_model->type);

  /* depend on the design technology of this MUX:
   * bus connections are different
   * SRAM MUX: bus is connected to the output ports of SRAM
   * RRAM MUX: bus is connected to the BL/WL of MUX
   * TODO: Maybe things will become even more complicated, 
   * the bus connections may depend on the type of configuration circuit...
   * Currently, this is fine.
   */
  switch (mux_spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    dump_verilog_cmos_mux_config_bus(fp, mux_spice_model, cur_sram_orgz_info, 
                                     mux_size, cur_num_sram,
                                     num_mux_reserved_conf_bits, num_mux_conf_bits); 
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* configuration wire bus */
    /* First bus is for sram_out in CMOS MUX or BL in RRAM MUX */
    fprintf(fp, "wire [0:%d] %s_size%d_%d_configbus0;\n",
            num_mux_reserved_conf_bits + num_mux_conf_bits - 1,
            mux_spice_model->prefix, mux_size, mux_spice_model->cnt);
    /* Second bus is for sram_out_inv in CMOS MUX or WL in RRAM MUX */
    fprintf(fp, "wire [0:%d] %s_size%d_%d_configbus1;\n",
            num_mux_reserved_conf_bits + num_mux_conf_bits - 1,
            mux_spice_model->prefix, mux_size, mux_spice_model->cnt);
    /* Connect wires to config bus */
    /* reserved configuration bits */
    if (0 < num_mux_reserved_conf_bits) {
      fprintf(fp, "assign %s_size%d_%d_configbus0[%d:%d] = ",
              mux_spice_model->prefix, mux_size, mux_spice_model->cnt,
              0, num_mux_reserved_conf_bits - 1);
      dump_verilog_reserved_sram_one_port(fp, cur_sram_orgz_info, 
                                          0, num_mux_reserved_conf_bits - 1,
                                          0, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
      fprintf(fp, "assign %s_size%d_%d_configbus1[%d:%d] = ",
              mux_spice_model->prefix, mux_size, mux_spice_model->cnt,
              0, num_mux_reserved_conf_bits - 1);
      dump_verilog_reserved_sram_one_port(fp, cur_sram_orgz_info, 
                                          0, num_mux_reserved_conf_bits - 1,
                                          1, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
    }
    /* normal configuration bits */
    if (0 < num_mux_conf_bits) {
      fprintf(fp, "assign %s_size%d_%d_configbus0[%d:%d] = ",
              mux_spice_model->prefix, mux_size, mux_spice_model->cnt,
              num_mux_reserved_conf_bits,
              num_mux_reserved_conf_bits + num_mux_conf_bits - 1);
      dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                                 cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                 0, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
      fprintf(fp, "assign %s_size%d_%d_configbus1[%d:%d] = ",
              mux_spice_model->prefix, mux_size, mux_spice_model->cnt,
              num_mux_reserved_conf_bits,
              num_mux_reserved_conf_bits + num_mux_conf_bits - 1);
      dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                                 cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                 1, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for SRAM!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* Dump CMOS verilog MUX configuraiton bus ports */
void dump_verilog_cmos_mux_config_bus_ports(FILE* fp, t_spice_model* mux_spice_model, 
                                            t_sram_orgz_info* cur_sram_orgz_info,
                                            int mux_size, int cur_num_sram,
                                            int num_mux_reserved_conf_bits,
                                            int num_mux_conf_bits) { 
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert(NULL != mux_spice_model);
  assert(SPICE_MODEL_MUX == mux_spice_model->type);

  switch(cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* configuration wire bus */
    /* FOR Scan-chain, we need regular output of a scan-chain FF
     * We do not need a prefix implying MUX name, size and index 
     */
    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      mux_spice_model, mux_size,
                                      cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                      0, VERILOG_PORT_CONKT);
    fprintf(fp, ",\n");
    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      mux_spice_model, mux_size,
                                      cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                      1, VERILOG_PORT_CONKT);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* configuration wire bus */
    /* First bus is for sram_out in CMOS MUX 
     * We need a prefix implying MUX name, size and index 
     */
    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      mux_spice_model, mux_size,
                                      cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                      0, VERILOG_PORT_CONKT);
    fprintf(fp, ",\n");
    dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                      mux_spice_model, mux_size,
                                      cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                      1, VERILOG_PORT_CONKT);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid SRAM organization!\n", 
               __FILE__, __LINE__);
    exit(1);
  }
  return;
}


/* Dump MUX reserved and normal configuration wire bus */
void dump_verilog_mux_config_bus_ports(FILE* fp, t_spice_model* mux_spice_model, 
                                       t_sram_orgz_info* cur_sram_orgz_info,
                                       int mux_size, int cur_num_sram,
                                       int num_mux_reserved_conf_bits,
                                       int num_mux_conf_bits) { 
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert(NULL != mux_spice_model);
  assert(SPICE_MODEL_MUX == mux_spice_model->type);

  /* depend on the design technology of this MUX:
   * bus connections are different
   * SRAM MUX: bus is connected to the output ports of SRAM
   */
  switch (mux_spice_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    dump_verilog_cmos_mux_config_bus_ports(fp, mux_spice_model, cur_sram_orgz_info,
                                           mux_size, cur_num_sram,
                                           num_mux_reserved_conf_bits, num_mux_conf_bits); 
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* configuration wire bus */
    fprintf(fp, "%s_size%d_%d_configbus0, ",
            mux_spice_model->prefix, mux_size, mux_spice_model->cnt);

    fprintf(fp, "%s_size%d_%d_configbus1 ",
            mux_spice_model->prefix, mux_size, mux_spice_model->cnt);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for SRAM!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}


/* Dump common ports of each pb_type in physical mode,
 * common ports include:
 * 1. inpad; 2. outpad; 3. iopad; TODO: merge other two to iopad 
 * 4. SRAMs (standalone)
 * 5. BL/WLs
 * 6. Scan-chain FFs 
 */
void dump_verilog_grid_common_port(FILE* fp, t_spice_model* cur_verilog_model,
                                   char* general_port_prefix, int lsb, int msb,
                                   enum e_dump_verilog_port_type dump_port_type) {
  char* port_full_name = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  assert(NULL != cur_verilog_model);
  if (0 >  msb - lsb) {
    return;
  }

  /*Malloc and generate the full name of port */
  port_full_name = (char*)my_malloc(sizeof(char)*(strlen(general_port_prefix) + strlen(cur_verilog_model->prefix) + 1));
  sprintf(port_full_name, "%s%s", general_port_prefix, cur_verilog_model->prefix);

  fprintf(fp, ",\n");
  dump_verilog_generic_port(fp, dump_port_type, port_full_name, msb, lsb); 

  /* Free */
  /* Local variables such as port1_name and port2 name are automatically freed  */
  my_free(port_full_name);

  return;
}

/* A widely used function to dump the configuration bus
 * This is supposed to be called when declaring local wires in the main body of a module
 * We will the internal wires (bus) used for connect SRAMs/RRAMs to other modules,
 * such as LUTs, MUXes and IOs 
 */
void dump_verilog_sram_config_bus_internal_wires(FILE* fp, t_sram_orgz_info* cur_sram_orgz_info, 
                                                 int lsb, int msb) {
  t_spice_model* mem_model = NULL;

  /* Get the memory spice model*/
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
  assert (NULL != mem_model);

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Depend on the configuraion style */
  switch(cur_sram_orgz_info->type) {
  case SPICE_SRAM_STANDALONE:
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, lsb, msb, -1, VERILOG_PORT_WIRE); 
    fprintf(fp, ";\n");
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, lsb, msb, 0, VERILOG_PORT_WIRE); 
    fprintf(fp, ";\n");
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, lsb, msb, 1, VERILOG_PORT_WIRE); 
    fprintf(fp, ";\n");
    /* Connect first SCFF to the head */
    fprintf(fp, "assign ");
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, lsb, lsb, -1, VERILOG_PORT_CONKT); 
    fprintf(fp, " = ");
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, lsb, lsb, 0, VERILOG_PORT_CONKT); 
    fprintf(fp, ";\n");
    /* Connect last SCFF to the tail */
    fprintf(fp, "assign ");
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, msb, msb, 1, VERILOG_PORT_CONKT); 
    fprintf(fp, " = ");
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, msb, msb, 0, VERILOG_PORT_CONKT); 
    fprintf(fp, ";\n");
    /* Connect SCFFs into chains */
    /* Cascade the SCFF between head and tail */
    if (1 < msb - lsb) {
      fprintf(fp, "assign ");
      dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 
                                          lsb + 1, msb,
                                           -1, VERILOG_PORT_CONKT);
      fprintf(fp, " = ");
      dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info, 
                                          lsb, msb - 1,
                                          0, VERILOG_PORT_CONKT);
      fprintf(fp, ";\n");
    }

    break;
  case SPICE_SRAM_MEMORY_BANK:
    dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, lsb, msb, 0, VERILOG_PORT_WIRE); 
    fprintf(fp, ";\n");
    dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, lsb, msb, 1, VERILOG_PORT_WIRE); 
    fprintf(fp, ";\n");
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid SRAM organization!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

void dump_verilog_toplevel_one_grid_side_pin_with_given_index(FILE* fp, t_rr_type pin_type, 
                                                              int pin_index, int side,
                                                              int x, int y,
                                                              boolean dump_port_type) {
  int height;  
  t_type_ptr type = NULL;
  char* verilog_port_type = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  type = grid[x][y].type;
  assert(NULL != type);

  assert((!(0 > pin_index))&&(pin_index < type->num_pins));
  assert((!(0 > side))&&(!(side > 3)));

  /* Assign the type of PIN*/ 
  switch (pin_type) {
  case IPIN:
  /* case SINK: */
    verilog_port_type = "output";
    break;
  /* case SOURCE: */
  case OPIN:
    verilog_port_type = "input";
    break;
  /* SINK and SOURCE are hypothesis nodes */
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid pin_type!\n", __FILE__, __LINE__);
    exit(1); 
  }

  /* Output the pins on the side*/ 
  height = get_grid_pin_height(x, y, pin_index);
  if (1 == type->pinloc[height][side][pin_index]) {
    /* Not sure if we need to plus a height */
    /* fprintf(fp, "grid_%d__%d__pin_%d__%d__%d_ ", x, y, height, side, pin_index); */
    if (TRUE == dump_port_type) {
      fprintf(fp, "%s ", verilog_port_type);
    }
    fprintf(fp, " grid_%d__%d__pin_%d__%d__%d_", x, y, height, side, pin_index);
    if (TRUE == dump_port_type) {
      fprintf(fp, ",\n");
    }
  } else {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Fail to print a grid pin (x=%d, y=%d, height=%d, side=%d, index=%d)\n",
              __FILE__, __LINE__, x, y, height, side, pin_index);
    exit(1);
  } 

  return;
}

/* Generate the subckt name for a MUX module/submodule */
char* generate_verilog_subckt_name(t_spice_model* spice_model, 
                                   char* postfix) {
  char* subckt_name = NULL;

  subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model->name) 
                                  + strlen(postfix) + 1)); 
  sprintf(subckt_name, "%s%s",
          spice_model->name, postfix);

  return subckt_name;
}

/* Generate the subckt name for a MUX module/submodule */
char* generate_verilog_mem_subckt_name(t_spice_model* spice_model, 
                                       t_spice_model* mem_model,
                                       char* postfix) {
  char* subckt_name = NULL;

  subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model->name) 
                                 + strlen(mem_model->name) + 1 + strlen(postfix) + 1)); 
  sprintf(subckt_name, "%s_%s%s",
          spice_model->name, mem_model->name, postfix);

  return subckt_name;
}


/* Generate the subckt name for a MUX module/submodule */
char* generate_verilog_mux_subckt_name(t_spice_model* spice_model, 
                                       int mux_size, char* postfix) {
  char* mux_subckt_name = NULL;

  mux_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model->name) + 5 
                                     + strlen(my_itoa(mux_size)) + strlen(postfix) + 1)); 
  sprintf(mux_subckt_name, "%s_size%d%s",
          spice_model->name, mux_size, postfix);

  return mux_subckt_name;
}

enum e_dump_verilog_port_type 
convert_spice_model_port_type_to_verilog_port_type(enum e_spice_model_port_type spice_model_port_type) {
  enum e_dump_verilog_port_type verilog_port_type; 

  switch (spice_model_port_type) {
  case SPICE_MODEL_PORT_INPUT: 
    verilog_port_type = VERILOG_PORT_INPUT;
    break;
  case SPICE_MODEL_PORT_OUTPUT: 
    verilog_port_type = VERILOG_PORT_OUTPUT;
    break;
  case SPICE_MODEL_PORT_INOUT: 
    verilog_port_type = VERILOG_PORT_INOUT;
    break;
  case SPICE_MODEL_PORT_CLOCK: 
  case SPICE_MODEL_PORT_SRAM:
  case SPICE_MODEL_PORT_BL:
  case SPICE_MODEL_PORT_BLB:
  case SPICE_MODEL_PORT_WL:
  case SPICE_MODEL_PORT_WLB:
    verilog_port_type = VERILOG_PORT_INPUT;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of Verilog port to be dumped !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return verilog_port_type;
}

int dump_verilog_mem_module_one_port_map(FILE* fp,
                                         t_spice_model* mem_model,
                                         enum e_spice_model_port_type port_type_to_dump,
                                         boolean dump_port_type,
                                         int index, int num_mem, boolean dump_first_comma,
                                         boolean require_explicit_port_map) {
  int iport;
  int cnt = 0;
  enum e_dump_verilog_port_type verilog_port_type; 
  int lsb = 0;

  for (iport = 0; iport < mem_model->num_port; iport++) {
    /* bypass global ports */
    if (TRUE == mem_model->ports[iport].is_global) {
      continue;
    }
    /* bypass non-input ports */
    if (port_type_to_dump != mem_model->ports[iport].type) {
      continue;
    }
    if (((0 == cnt) && (TRUE == dump_first_comma))
       || (0 < cnt)) {
      fprintf(fp, ",\n");
    }
    if (TRUE == dump_port_type) {
      verilog_port_type = convert_spice_model_port_type_to_verilog_port_type(port_type_to_dump);
    } else {
      assert (FALSE == dump_port_type);
      verilog_port_type = VERILOG_PORT_CONKT;
      /* Dump explicit port mapping if needed */
      if ( (TRUE == require_explicit_port_map) 
         && (TRUE == mem_model->dump_explicit_port_map)) {
        fprintf(fp, " .%s(", mem_model->ports[iport].lib_name);
      }
    }
    /* The LSB depends on the port size */
    assert (-1 < index);
    lsb = index * mem_model->ports[iport].size;
    dump_verilog_generic_port(fp, verilog_port_type,
                              mem_model->ports[iport].prefix, lsb, lsb + num_mem * mem_model->ports[iport].size - 1);
    if (  (FALSE == dump_port_type) 
        && (TRUE == require_explicit_port_map) 
        &&(TRUE == mem_model->dump_explicit_port_map)) {
      fprintf(fp, ")");
    }
    cnt++;
  }

  return cnt;
}

/* Output the ports of a SRAM MUX */
void dump_verilog_mem_module_port_map(FILE* fp, 
                                      t_spice_model* mem_model,
                                      boolean dump_port_type,
                                      int lsb, int num_mem,
                                      boolean require_explicit_port_map) {
  boolean dump_first_comma = FALSE;
  /* Here we force the sequence of ports: of a memory subumodule:
   * 1. Global ports 
   * 2. input ports 
   * 3. output ports 
   * 4. bl ports 
   * 5. wl ports 
   * 6. blb ports 
   * 7. wlb ports 
   * Other ports are not accepted!!! 
   */
  /* 1. Global ports!  */
  if (0 < rec_dump_verilog_spice_model_global_ports(fp, mem_model, dump_port_type, TRUE, require_explicit_port_map)) {
    dump_first_comma = TRUE;
  }

  /* 2. input ports */ 
  if (0 < dump_verilog_mem_module_one_port_map(fp, mem_model, SPICE_MODEL_PORT_INPUT, 
                                               dump_port_type, lsb, num_mem, dump_first_comma,
                                               require_explicit_port_map)) {
    dump_first_comma = TRUE;
  } else {
    dump_first_comma = FALSE;
  }

  /* 3. output ports */ 
  if (0 < dump_verilog_mem_module_one_port_map(fp, mem_model, SPICE_MODEL_PORT_OUTPUT, 
                                               dump_port_type, lsb, num_mem, dump_first_comma,
                                               require_explicit_port_map)) {
    dump_first_comma = TRUE;
  } else {
    dump_first_comma = FALSE;
  }

  /* 4. bl ports */ 
  if (0 < dump_verilog_mem_module_one_port_map(fp, mem_model, SPICE_MODEL_PORT_BL, 
                                               dump_port_type, lsb, num_mem, dump_first_comma,
                                               require_explicit_port_map)) {
    dump_first_comma = TRUE;
  } else {
    dump_first_comma = FALSE;
  }

  /* 5. wl ports */ 
  if (0 < dump_verilog_mem_module_one_port_map(fp, mem_model, SPICE_MODEL_PORT_WL, 
                                               dump_port_type, lsb, num_mem, dump_first_comma,
                                               require_explicit_port_map)) {
    dump_first_comma = TRUE;
  } else {
    dump_first_comma = FALSE;
  }

  /* 6. blb ports */ 
  if (0 < dump_verilog_mem_module_one_port_map(fp, mem_model, SPICE_MODEL_PORT_BLB, 
                                               dump_port_type, lsb, num_mem, dump_first_comma,
                                               require_explicit_port_map)) {
    dump_first_comma = TRUE;
  } else {
    dump_first_comma = FALSE;
  }

  /* 7. wlb ports */ 
  if (0 < dump_verilog_mem_module_one_port_map(fp, mem_model, SPICE_MODEL_PORT_WLB, 
                                               dump_port_type, lsb, num_mem, dump_first_comma,
                                               require_explicit_port_map)) {
    dump_first_comma = TRUE;
  } else {
    dump_first_comma = FALSE;
  }

  return;
} 

/* Dump a verilog submodule in the mem submodule (part of MUX, LUT and other ), according to SRAM organization type */
void dump_verilog_mem_sram_submodule(FILE* fp,
                                     t_sram_orgz_info* cur_sram_orgz_info,
                                     t_spice_model* cur_verilog_model, int mux_size,
                                     t_spice_model* cur_sram_verilog_model,
                                     int lsb, int msb) {
  int cur_bl, cur_wl;
  int num_bl_ports, num_wl_ports;
  t_spice_model_port** bl_port = NULL;
  t_spice_model_port** wl_port = NULL;
  int num_blb_ports, num_wlb_ports;
  t_spice_model_port** blb_port = NULL;
  t_spice_model_port** wlb_port = NULL;

  int num_bl_per_sram = 0;
  int num_wl_per_sram = 0;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  assert(NULL != cur_sram_orgz_info);
  assert(NULL != cur_sram_verilog_model);
  assert((SPICE_MODEL_SRAM == cur_sram_verilog_model->type)
        || (SPICE_MODEL_SCFF == cur_sram_verilog_model->type));

  switch (cur_sram_orgz_info->type) {
  case SPICE_SRAM_MEMORY_BANK:
    /* Detect the SRAM SPICE model linked to this SRAM port */
    find_bl_wl_ports_spice_model(cur_sram_verilog_model, 
                                 &num_bl_ports, &bl_port, &num_wl_ports, &wl_port); 
    assert(1 == num_bl_ports);
    assert(1 == num_wl_ports);
    num_bl_per_sram = bl_port[0]->size; 
    num_wl_per_sram = wl_port[0]->size; 
    /* Find the BLB and WLB port, if there is any */
    find_blb_wlb_ports_spice_model(cur_sram_verilog_model, 
                                   &num_blb_ports, &blb_port, &num_wlb_ports, &wlb_port); 
    if (1 == num_blb_ports) {
      assert(num_bl_per_sram == blb_port[0]->size);
    } else {
      assert(0 == num_blb_ports);
    }
    if (1 == num_wlb_ports) {
      assert(num_wl_per_sram == wlb_port[0]->size);
    } else {
      assert(0 == num_wlb_ports);
    }

    /* Only dump the global ports belonging to a spice_model */
    if (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_sram_verilog_model, FALSE, TRUE, FALSE)) {
      fprintf(fp, ",\n");
    }

    if (SPICE_MODEL_MUX == cur_verilog_model->type) {
      fprintf(fp, "%s_size%d_%d_", 
              cur_verilog_model->name, mux_size, cur_verilog_model->cnt);
    }
    dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, 
                                      lsb, msb,
                                      0, VERILOG_PORT_CONKT);
    fprintf(fp, ",");
    if (SPICE_MODEL_MUX == cur_verilog_model->type) {
      fprintf(fp, "%s_size%d_%d_", 
              cur_verilog_model->name, mux_size, cur_verilog_model->cnt);
    }
    dump_verilog_sram_one_outport(fp, cur_sram_orgz_info, 
                                      lsb, msb,
                                      1, VERILOG_PORT_CONKT);
    fprintf(fp, ",");
    get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl); 
    /* Connect to Bit lines and Word lines, consider each conf_bit */
    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                                lsb, msb,
                                0, VERILOG_PORT_CONKT);
    fprintf(fp, ",");

    dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                                lsb, msb,
                                1, VERILOG_PORT_CONKT);
    /* If we have a BLB or WLB, we need to dump inverted config_bus */
    if (1 == num_blb_ports) { 
      fprintf(fp, ", ");
      dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                                 lsb, msb,
                                 2, VERILOG_PORT_CONKT);
    }
    if (1 == num_wlb_ports) { 
      fprintf(fp, ", ");
      dump_verilog_sram_one_port(fp, cur_sram_orgz_info, 
                                 lsb, msb,
                                 3, VERILOG_PORT_CONKT);
    }

    break;
  case SPICE_SRAM_STANDALONE:
    /* SRAM subckts*/
    /* Only dump the global ports belonging to a spice_model */
    if (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_sram_verilog_model, FALSE, TRUE, FALSE)) {
      fprintf(fp, ",\n");
    }
    fprintf(fp, "%s_out[%d:%d], ", 
            cur_sram_verilog_model->prefix, lsb, msb); /* Input*/
    fprintf(fp, "%s_out[%d:%d], %s_outb[%d:%d] ", 
            cur_sram_verilog_model->prefix, lsb, msb, 
            cur_sram_verilog_model->prefix, lsb, msb); /* Outputs */
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    /* Only dump the global ports belonging to a spice_model */
    if (0 < rec_dump_verilog_spice_model_global_ports(fp, cur_sram_verilog_model, FALSE, TRUE, FALSE)) {
      fprintf(fp, ",\n");
    }
    if (SPICE_MODEL_MUX == cur_verilog_model->type) {
      /* Input of Scan-chain DFF, should be connected to the output of its precedent */
      dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info,
                                        cur_verilog_model, mux_size,
                                        lsb, msb,
                                        -1, VERILOG_PORT_CONKT);
      fprintf(fp, ", \n");  //
      /* Output of Scan-chain DFF, should be connected to the output of its successor */
      dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info,
                                        cur_verilog_model, mux_size,
                                        lsb, msb,
                                        0, VERILOG_PORT_CONKT);
      fprintf(fp, ", \n");  //
      dump_verilog_mux_sram_one_outport(fp, cur_sram_orgz_info, 
                                        cur_verilog_model, mux_size,
                                        lsb, msb,
                                        1, VERILOG_PORT_CONKT);
      break;
    } 
    /* Input of Scan-chain DFF, should be connected to the output of its precedent */
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info,
                                        lsb, msb,
                                         -1, VERILOG_PORT_CONKT);
    fprintf(fp, ", \n");  //
    /* Output of Scan-chain DFF, should be connected to the output of its successor */
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info,
                                        lsb, msb,
                                         0, VERILOG_PORT_CONKT);
    fprintf(fp, ", \n");  //
    dump_verilog_sram_one_local_outport(fp, cur_sram_orgz_info,
                                        lsb, msb,
                                        1, VERILOG_PORT_CONKT);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}
 
char* gen_verilog_grid_one_pin_name(int x, int y,
                                    int height, int side, int pin_index,
                                    boolean for_top_netlist) {
  char* ret = NULL;

  /* This pin appear at this side! */
  if (TRUE == for_top_netlist) {
    ret = (char*) my_malloc(sizeof(char) * 
                            (5 + strlen(my_itoa(x)) + 2 + strlen(my_itoa(y))
                             + 6 + strlen(my_itoa(height)) 
                             + 2 + strlen(my_itoa(side))
                             + 2 + strlen(my_itoa(pin_index))
                             + 2 ));    
    sprintf(ret, "grid_%d__%d__pin_%d__%d__%d_", 
            x, y,
            height, side, pin_index);
  } else {
    assert(FALSE == for_top_netlist);
    ret = (char*) my_malloc(sizeof(char) * 
                            (strlen(convert_side_index_to_string(side)) 
                             + 8 + strlen(my_itoa(height)) 
                             + 6 + strlen(my_itoa(pin_index))
                             + 2 ));    
    sprintf(ret, "%s_height_%d__pin_%d_", 
            convert_side_index_to_string(side), height, pin_index);
  }

  return ret;
}

char* gen_verilog_routing_channel_one_pin_name(t_rr_node* chan_rr_node,
                                              int x, int y, int track_idx,
                                              enum PORTS pin_direction) {

  char* ret = NULL;
  
  ret = (char*)my_malloc(strlen(convert_chan_type_to_string(chan_rr_node->type))
                         + 1 + strlen(my_itoa(x))
                         + 2 + strlen(my_itoa(y))
                         + 6 + strlen(my_itoa(track_idx))
                         + 2 + 1);

   switch (pin_direction) {
   case OUT_PORT:
     sprintf(ret, "%s_%d__%d__out_%d_", 
             convert_chan_type_to_string(chan_rr_node->type), 
             x, y, track_idx); 
     break;
   case IN_PORT:
     sprintf(ret, "%s_%d__%d__in_%d_",
             convert_chan_type_to_string(chan_rr_node->type), 
             x, y, track_idx); 
     break;
   default:
     vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of chan_rr_node!\n",
                __FILE__, __LINE__);
     exit(1);
  }

  return ret;
}

char* gen_verilog_routing_channel_one_midout_name(t_cb* cur_cb_info,
                                                 int track_idx) {
  char* ret = NULL;
  
  ret = (char*)my_malloc(strlen(convert_chan_type_to_string(cur_cb_info->type))
                         + 1 + strlen(my_itoa(cur_cb_info->x))
                         + 2 + strlen(my_itoa(cur_cb_info->y))
                         + 9 + strlen(my_itoa(track_idx))
                         + 1 + 1);

  sprintf(ret, "%s_%d__%d__midout_%d_",
          convert_chan_type_to_string(cur_cb_info->type),
          cur_cb_info->x, cur_cb_info->y, track_idx);

  return ret;
}

char* gen_verilog_one_cb_module_name(t_cb* cur_cb_info) {
  char* ret = NULL;
  
  ret = (char*)my_malloc(strlen(convert_cb_type_to_string(cur_cb_info->type))
                         + 1 + strlen(my_itoa(cur_cb_info->x))
                         + 2 + strlen(my_itoa(cur_cb_info->y))
                         + 1 + 1); 

  sprintf(ret, "%s_%d__%d_", 
          convert_cb_type_to_string(cur_cb_info->type),
          cur_cb_info->x, cur_cb_info->y);

  return ret;
}

char* gen_verilog_one_cb_instance_name(t_cb* cur_cb_info) {
  char* ret = NULL;
  
  ret = (char*)my_malloc(strlen(convert_cb_type_to_string(cur_cb_info->type))
                         + 1 + strlen(my_itoa(cur_cb_info->x))
                         + 2 + strlen(my_itoa(cur_cb_info->y))
                         + 4 + 1); 

  sprintf(ret, "%s_%d__%d__0_", 
          convert_cb_type_to_string(cur_cb_info->type),
          cur_cb_info->x, cur_cb_info->y);

  return ret;
}

char* gen_verilog_one_sb_module_name(t_sb* cur_sb_info) {
  char* ret = NULL;
  
  ret = (char*)my_malloc(2 + 1 + strlen(my_itoa(cur_sb_info->x))
                         + 2 + strlen(my_itoa(cur_sb_info->y))
                         + 1 + 1); 

  sprintf(ret, "sb_%d__%d_", 
          cur_sb_info->x, cur_sb_info->y);

  return ret;
}

char* gen_verilog_one_sb_instance_name(t_sb* cur_sb_info) {
  char* ret = NULL;
  
  ret = (char*)my_malloc(2 + 1 + strlen(my_itoa(cur_sb_info->x))
                         + 2 + strlen(my_itoa(cur_sb_info->y))
                         + 4 + 1); 

  sprintf(ret, "sb_%d__%d__0_", 
          cur_sb_info->x, cur_sb_info->y);

  return ret;
}

char* gen_verilog_one_routing_channel_module_name(t_rr_type chan_type,
                                                  int x, int y) {
  char* ret = NULL;
  
  if (-1 == y) {
    ret = (char*)my_malloc(strlen(convert_chan_type_to_string(chan_type))
                           + 1 + strlen(my_itoa(x))
                           + 1 + 1); 
    sprintf(ret, "%s_%d_", 
            convert_chan_type_to_string(chan_type), 
            x);
  } else {
    ret = (char*)my_malloc(strlen(convert_chan_type_to_string(chan_type))
                           + 1 + strlen(my_itoa(x))
                           + 2 + strlen(my_itoa(y))
                           + 1 + 1); 
    sprintf(ret, "%s_%d__%d_", 
            convert_chan_type_to_string(chan_type), 
            x, y);
  }

  return ret;
}

char* gen_verilog_one_routing_channel_instance_name(t_rr_type chan_type,
                                                    int x, int y) {
  char* ret = NULL;
  
  ret = (char*)my_malloc(strlen(convert_chan_type_to_string(chan_type))
                         + 1 + strlen(my_itoa(x))
                         + 2 + strlen(my_itoa(y))
                         + 4 + 1); 

  sprintf(ret, "%s_%d__%d__0_", 
          convert_chan_type_to_string(chan_type), 
          x, y);

  return ret;
}

/* Generate the subckt name for a MUX module/submodule */
char* gen_verilog_one_mux_module_name(t_spice_model* spice_model, 
                                      int mux_size) {
  char* mux_subckt_name = NULL;

  mux_subckt_name = (char*)my_malloc(sizeof(char)*(strlen(spice_model->name) + 5 
                                     + strlen(my_itoa(mux_size)) + 1)); 
  sprintf(mux_subckt_name, "%s_size%d",
          spice_model->name, mux_size);

  return mux_subckt_name;
}

/* Generate the full path of a pb_graph_pin in the hierarchy of verilog netlists
 * For example: grid_<x>__<y>_/grid_<mode_name>_<z>/.../<port_name>_<pin_number>_
 */
char* gen_verilog_one_grid_instance_name(int grid_x, int grid_y) {
  char* ret = NULL;
  
  ret = (char*)my_malloc(sizeof(char) * 
                        ( 5 + strlen(my_itoa(grid_x))
                        + 2 + strlen(my_itoa(grid_y))
                        + 1 + 1));

  sprintf(ret, "grid_%d__%d_",
          grid_x, grid_y);

  return ret;
}

char* gen_verilog_one_grid_module_name(int grid_x, int grid_y) {
  return gen_verilog_one_grid_instance_name(grid_x, grid_y);
}

char* gen_verilog_one_block_instance_name(int grid_x, int grid_y, int grid_z) {
  char* ret = NULL;
  
  ret = (char*)my_malloc(sizeof(char) * 
                        ( 5 + strlen(my_itoa(grid_x))
                        + 2 + strlen(my_itoa(grid_y))
                        + 2 + strlen(my_itoa(grid_z))
                        + 1 + 1));

  sprintf(ret, "grid_%d__%d__%d_",
          grid_x, grid_y, grid_z);

  return ret;
}

char* gen_verilog_one_phy_block_instance_name(t_type_ptr cur_type_ptr, 
                                              int block_z) {
  char* ret = NULL;
  
  ret = (char*)my_malloc(sizeof(char) * 
                        ( 5 + strlen(cur_type_ptr->name)
                        + 1 + strlen(my_itoa(block_z))
                        + 1 + 1));

  sprintf(ret, "grid_%s_%d_",
          cur_type_ptr->name, block_z);

  return ret;
}

char* gen_verilog_one_pb_graph_node_instance_name(t_pb_graph_node* cur_pb_graph_node) {
  char* ret = NULL;
  
  ret = (char*)my_malloc(sizeof(char) * 
                        ( strlen(cur_pb_graph_node->pb_type->name)
                        + 1 + strlen(my_itoa(cur_pb_graph_node->placement_index))
                        + 1 + 1));

  sprintf(ret, "%s_%d_",
          cur_pb_graph_node->pb_type->name, cur_pb_graph_node->placement_index);

  return ret;
}

char* gen_verilog_one_pb_type_pin_name(char* prefix, 
                                       t_port* cur_port, int pin_number) {
  char* ret = NULL;
  
  ret = (char*)my_malloc(sizeof(char) * 
                        (strlen(prefix) + 2 
                        + strlen(cur_port->name)
                        + 1 + strlen(my_itoa(pin_number))
                        + 1 + 1));

  sprintf(ret, "%s__%s_%d_",
          prefix, cur_port->name, pin_number);
   
  return ret;
}

/* Generate the full path of a pb_graph_pin in the hierarchy of verilog netlists
 * For example: grid_<x>__<y>_/grid_<mode_name>_<z>/.../<port_name>_<pin_number>_
 */
char* gen_verilog_one_pb_graph_pin_full_name_in_hierarchy(t_pb_graph_pin* cur_pb_graph_pin) {
  char* full_name = NULL;
  char* cur_name = NULL;
  t_pb_graph_node* temp = cur_pb_graph_pin->parent_node;

  /* Give the pin name */
  full_name = gen_verilog_one_pb_type_pin_name(cur_pb_graph_pin->parent_node->pb_type->name, 
                                               cur_pb_graph_pin->port, 
                                               cur_pb_graph_pin->pin_number);

  /* The instance name of the top-level graph node is very special 
   * we output it in another function  
   */
  while (NULL != temp->parent_pb_graph_node) {
    /* Generate the instance name of current pb_graph_node
     * and add a slash to separate the upper level 
     */
    cur_name = gen_verilog_one_pb_graph_node_instance_name(temp);
    cur_name = my_strcat(cur_name, "/");
    full_name = my_strcat(cur_name, full_name);
    /* Go to upper level */
    temp = temp->parent_pb_graph_node;
    my_free(cur_name);
  } 
  return full_name;
}

char* gen_verilog_top_module_io_port_prefix(char* global_prefix, 
                                            char* io_port_prefix) {
  char* port_name = NULL;

  port_name = (char*)my_malloc(sizeof(char)*(strlen(global_prefix) + strlen(io_port_prefix) + 1));
  sprintf(port_name, "%s%s", global_prefix, io_port_prefix);

  return port_name;
}

char* gen_verilog_one_pb_graph_pin_full_name_in_hierarchy_parent_node(t_pb_graph_pin* cur_pb_graph_pin) {
  char* full_name = NULL;
  char* cur_name = NULL;
  t_pb_graph_node* temp = cur_pb_graph_pin->parent_node;
  //t_pb_graph_node* temp = cur_pb_graph_pin->parent_node->parent_pb_graph_node;

  full_name = "";
  /* The instance name of the top-level graph node is very special 
   * we output it in another function  
   */
  while (NULL != temp->parent_pb_graph_node) {
    /* Generate the instance name of current pb_graph_node
     * and add a slash to separate the upper level 
     */
    cur_name = gen_verilog_one_pb_graph_node_instance_name(temp);
    cur_name = my_strcat(cur_name, "/");
    full_name = my_strcat(cur_name, full_name);
    /* Go to upper level */
    temp = temp->parent_pb_graph_node;
    my_free(cur_name);
  }
 
  return full_name;
}

char* gen_verilog_one_pb_graph_pin_full_name_in_hierarchy_grand_parent_node(t_pb_graph_pin* cur_pb_graph_pin) {
  char* full_name = "";
  char* cur_name = NULL;
  t_pb_graph_node* temp = cur_pb_graph_pin->parent_node;
  
  if (NULL != temp->parent_pb_graph_node) {
    temp = temp->parent_pb_graph_node;
  }
  else {
    return full_name;
  }
  
  /* The instance name of the top-level graph node is very special 
   * we output it in another function  
   */
  while (NULL != temp->parent_pb_graph_node) {
    /* Generate the instance name of current pb_graph_node
     * and add a slash to separate the upper level 
     */
    cur_name = gen_verilog_one_pb_graph_node_instance_name(temp);
    cur_name = my_strcat(cur_name, "/");
    full_name = my_strcat(cur_name, full_name);
    /* Go to upper level */
    temp = temp->parent_pb_graph_node;
    my_free(cur_name);
  }
 
  return full_name;
}

char* gen_verilog_one_pb_graph_node_full_name_in_hierarchy(t_pb_graph_node* cur_pb_graph_node) {
  char* full_name = NULL;
  char* cur_name = NULL;
  t_pb_graph_node* temp = cur_pb_graph_node;

  full_name = "";
  /* The instance name of the top-level graph node is very special 
   * we output it in another function  
   */
  while (NULL != temp->parent_pb_graph_node) {
    /* Generate the instance name of current pb_graph_node
     * and add a slash to separate the upper level 
     */
    cur_name = gen_verilog_one_pb_graph_node_instance_name(temp);
    cur_name = my_strcat(cur_name, "/");
    full_name = my_strcat(cur_name, full_name);
    /* Go to upper level */
    temp = temp->parent_pb_graph_node;
    my_free(cur_name);
  }
 
  return full_name;
}

