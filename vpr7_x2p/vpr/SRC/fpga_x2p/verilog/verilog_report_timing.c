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
#include "route_common.h"
#include "vpr_utils.h"

/* Include SPICE support headers*/
#include "linkedlist.h"
#include "rr_blocks.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_backannotate_utils.h"
#include "fpga_x2p_mux_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "fpga_x2p_rr_graph_utils.h"
#include "fpga_x2p_globals.h"

/* Include Verilog support headers*/
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_routing.h"
#include "verilog_tcl_utils.h"

#include "verilog_report_timing.h"

/* options for report timing */
typedef struct s_trpt_opts t_trpt_opts;
struct s_trpt_opts {
  char* sdc_dir;
  boolean longest_path_only;
  boolean report_pb_timing;
  boolean report_cb_timing;
  boolean report_sb_timing;
  boolean report_routing_timing;
  boolean print_thru_pins;
  boolean compact_routing_hierarchy; /* TODO: TO BE DEPRECATED WHEN NEW DATA STRUCTURE HAS BEEN USED */
};

typedef struct s_wireL_cnt t_wireL_cnt;
struct s_wireL_cnt {
  int L_wire;
  FILE* file_handler;
  int cnt;
};

/***** Subroutines *****/
static 
char* gen_verilog_one_routing_report_timing_Lwire_dir_path(char* report_timing_path, 
                                                           int L_wire) {
  char* ret = NULL;
  char* formatted_path = format_dir_path(report_timing_path);

  /* The report will be named after L<lenght>_path<ID>*/
  ret = (char*) my_malloc (sizeof(char) * (strlen(formatted_path)
                           + 1 + strlen(my_itoa(L_wire))
                           + 6 + 1));

  sprintf(ret, 
          "%sL%d_wire/",
          formatted_path, L_wire);

  return ret;
}

static 
char* gen_verilog_one_routing_report_timing_rpt_name(char* report_timing_path,
                                                     char* direction,
                                                     int L_wire, 
                                                     int path_id) {
  char* ret = NULL;
  char* formatted_path = gen_verilog_one_routing_report_timing_Lwire_dir_path(report_timing_path, L_wire);

  /* The report will be named after L<lenght>_path<ID>*/
  ret = (char*) my_malloc (sizeof(char) * (strlen(formatted_path)
                           + 1 + strlen(direction)
                           + 1 + strlen(my_itoa(L_wire))
                           + 5 + strlen(my_itoa(path_id)) 
                           + 5));

  sprintf(ret, 
          "%sL%d_%s_path%d.rpt",
          formatted_path, L_wire, direction, path_id);

  return ret;
}

static 
FILE* create_wireL_report_timing_tcl_file_handler(t_trpt_opts trpt_opts, 
                                                  char* direction,
                                                  int L_wire) {
  FILE* fp = NULL;
  char* sdc_dir_path = NULL;
  char* L_wire_str = (char*) my_malloc(sizeof(char) * (1 + strlen(my_itoa(L_wire)) + 2));
  char* descr = NULL;
  char* sdc_fname = NULL;
  char* tmp_direction;

  sprintf(L_wire_str, "L%d_", L_wire);
  tmp_direction = (char*) my_malloc(sizeof(char) * (strlen(direction) + 2));
  sprintf(tmp_direction, "%s_", direction);

  sdc_dir_path = (char*) my_malloc(sizeof(char) * 
                 (strlen(gen_verilog_one_routing_report_timing_Lwire_dir_path(trpt_opts.sdc_dir,L_wire)) + 1
                 + strlen(direction) + 1));
 // sprintf(sdc_dir_path, "%s%s/", 
 //                gen_verilog_one_routing_report_timing_Lwire_dir_path(trpt_opts.sdc_dir, L_wire), direction); 
  sdc_dir_path = gen_verilog_one_routing_report_timing_Lwire_dir_path(trpt_opts.sdc_dir, L_wire); 
  
  /* Create dir path*/
  create_dir_path(sdc_dir_path);

  /* Create the file handler */
  sdc_fname = my_strcat(sdc_dir_path, L_wire_str);
  sdc_fname = my_strcat(sdc_fname, tmp_direction);
  sdc_fname = my_strcat(sdc_fname, trpt_routing_file_name);

  /* Create a file*/
  fp = fopen(sdc_fname, "w");

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Failure in create SDC constraints %s",
               __FILE__, __LINE__, sdc_fname); 
    exit(1);
  } 

  descr = (char*)my_malloc(sizeof(char) * (strlen("Report Timing for L-")
                                         + strlen(my_itoa(L_wire)) + 1
                                         + strlen(direction) + 1
                                         + strlen(" wires") + 1));
  sprintf(descr, "Report Timing for L-%d %s wires", L_wire, direction); 

  /* Generate SDC header */
  dump_verilog_sdc_file_header(fp, descr);

  /* Free */
  my_free(descr);
  my_free(sdc_dir_path);
  my_free(L_wire_str);
  my_free(sdc_fname);
  my_free(tmp_direction);

  return fp;
}

/* Find a wire length in the linked list,
 * And return the counter if we found,
 * Otherwise, we allocate a new member to the linked list */
static 
t_llist* get_wire_L_counter_in_llist(t_llist* rr_path_cnt, 
                                     t_trpt_opts trpt_opts,
                                     char* direction,
                                     int L_wire,
                                     t_wireL_cnt** ret_cnt) {
  t_llist* temp = rr_path_cnt;
  t_wireL_cnt* temp_cnt = NULL;
  
  while (NULL != temp) {
    temp_cnt = (t_wireL_cnt*)(temp->dptr);
    if (L_wire == temp_cnt->L_wire) {
      /* We find it! Return here */ 
      (*ret_cnt) = temp_cnt;
      return rr_path_cnt;
    }
    /* Go to next */
    temp = temp->next;
  }
  /* If temp is empty, we have to allocate a new node */
  if (NULL == temp) {
    temp = insert_llist_node_before_head(rr_path_cnt);
    /* Allocate new wireL */
    temp_cnt = (t_wireL_cnt*)my_malloc(sizeof(t_wireL_cnt));
    /* Initialization */
    temp_cnt->L_wire = L_wire;
    temp_cnt->file_handler = create_wireL_report_timing_tcl_file_handler(trpt_opts, direction, L_wire);
    temp_cnt->cnt = 0;
    temp->dptr = (void*)temp_cnt;
    /* Prepare the counter to return */ 
    (*ret_cnt) = temp_cnt;
  }

  return temp;
}

static 
void fclose_wire_L_file_handler_in_llist(t_llist* rr_path_cnt) {
  t_llist* temp = rr_path_cnt;
  t_wireL_cnt* temp_cnt = NULL;

  while (NULL != temp) {
    temp_cnt = (t_wireL_cnt*)(temp->dptr);
    fclose(temp_cnt->file_handler);
    /* Go to next */
    temp = temp->next;
  }

  return;
} 
                                   
static 
void update_wire_L_counter_in_llist(t_llist* rr_path_cnt, 
                                    int L_wire,
                                    int path_cnt) {
  t_llist* temp = rr_path_cnt;
  t_wireL_cnt* temp_cnt = NULL;
  
  while (NULL != temp) {
    temp_cnt = (t_wireL_cnt*)(temp->dptr);
    if (L_wire == temp_cnt->L_wire) {
      /* We find it! Return here */ 
      temp_cnt->cnt = path_cnt;
      return;
    }
    /* Go to next */
    temp = temp->next;
  }

  /* We must find something! */
  assert (NULL != temp);

  return;
}

static 
void free_wire_L_llist(t_llist* rr_path_cnt) {
  t_llist* temp = rr_path_cnt;

  while (NULL != temp) {
    my_free(temp->dptr); 
    temp->dptr = NULL;
    /* Go to next */
    temp = temp->next;
  }
  
  free_llist(rr_path_cnt);

  return;
}

/* Reporting timing from a SB input to an output
 */
static 
void verilog_generate_one_report_timing_within_sb(FILE* fp,
                                                  const RRGSB& rr_sb,
                                                  t_rr_node* src_rr_node,
                                                  t_rr_node* des_rr_node) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ( (CHANX == src_rr_node->type) || (CHANY == src_rr_node->type) || (OPIN == src_rr_node->type) );
  assert ( (CHANX == des_rr_node->type) || (CHANY == des_rr_node->type) );

  fprintf(fp, "report_timing -from "); 

  /* output instance name */
  fprintf(fp, "%s/", 
          rr_sb.gen_sb_verilog_instance_name()); 
  /* Find which side the ending pin locates, and determine the coordinate */
  dump_verilog_one_sb_routing_pin(fp, rr_sb, src_rr_node);

  fprintf(fp, " -to "); 

  /* output instance name */
  fprintf(fp, "%s/",
          rr_sb.gen_sb_verilog_instance_name()); 
  /* Find which side the ending pin locates, and determine the coordinate */
  dump_verilog_one_sb_chan_pin(fp, rr_sb, des_rr_node, OUT_PORT); 

  fprintf(fp, " -point_to_point"); 
  fprintf(fp, " -unconstrained"); 

  return;
}


/* Reporting timing from a SB input to an output
 */
static 
void verilog_generate_one_report_timing_within_sb(FILE* fp,
                                                 t_sb* cur_sb_info,
                                                 t_rr_node* src_rr_node,
                                                 t_rr_node* des_rr_node) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ( (CHANX == src_rr_node->type) || (CHANY == src_rr_node->type) || (OPIN == src_rr_node->type) );
  assert ( (CHANX == des_rr_node->type) || (CHANY == des_rr_node->type) );

  fprintf(fp, "report_timing -from "); 

  /* output instance name */
  fprintf(fp, "%s/", 
              gen_verilog_one_sb_instance_name(cur_sb_info)); 
  /* Find which side the ending pin locates, and determine the coordinate */
  dump_verilog_one_sb_routing_pin(fp, cur_sb_info, src_rr_node);

  fprintf(fp, " -to "); 

  /* output instance name */
  fprintf(fp, "%s/",
          gen_verilog_one_sb_instance_name(cur_sb_info));
  /* Find which side the ending pin locates, and determine the coordinate */
  dump_verilog_one_sb_chan_pin(fp, cur_sb_info, des_rr_node, OUT_PORT); 

  fprintf(fp, " -point_to_point"); 
  fprintf(fp, " -unconstrained"); 

  return;
}


/* Reporting timing from a SB output to another CB input
 */
static 
void verilog_generate_one_report_timing_sb_to_cb(FILE* fp,
                                                 t_sb* src_sb_info,
                                                 t_rr_node* src_rr_node,
                                                 t_cb* des_cb_info,
                                                 t_rr_node* des_rr_node) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ( (CHANX == src_rr_node->type) || (CHANY == src_rr_node->type) );
  assert ( (IPIN == des_rr_node->type) );

  fprintf(fp, "report_timing -from "); 
  /* output instance name */
  fprintf(fp, "%s/", 
              gen_verilog_one_sb_instance_name(src_sb_info)); 
  /* output pin name */
  dump_verilog_one_sb_chan_pin(fp, src_sb_info, 
                               src_rr_node, OUT_PORT); 
  fprintf(fp, " -to "); 
  /* output instance name */
  fprintf(fp, "%s/",
          gen_verilog_one_cb_instance_name(des_cb_info));
  /* output pin name */
  fprintf(fp, "%s",
          gen_verilog_routing_channel_one_midout_name( des_cb_info,
                                                       src_rr_node->ptc_num));

  fprintf(fp, " -point_to_point"); 
  fprintf(fp, " -unconstrained"); 

  return;
}

/* Reporting timing from a SB output to another CB input
 */
static 
void verilog_generate_one_report_timing_sb_to_cb(FILE* fp,
                                                 const RRGSB& src_sb,
                                                 t_rr_node* src_rr_node,
                                                 t_cb* des_cb_info,
                                                 t_rr_node* des_rr_node) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ( (CHANX == src_rr_node->type) || (CHANY == src_rr_node->type) );
  assert ( (IPIN == des_rr_node->type) );

  fprintf(fp, "report_timing -from "); 
  /* output instance name */
  fprintf(fp, "%s/", 
              src_sb.gen_sb_verilog_instance_name()); 
  /* output pin name */
  dump_verilog_one_sb_chan_pin(fp, src_sb, 
                               src_rr_node, OUT_PORT); 
  fprintf(fp, " -to "); 
  /* output instance name */
  fprintf(fp, "%s/",
          gen_verilog_one_cb_instance_name(des_cb_info));
  /* output pin name */
  fprintf(fp, "%s",
          gen_verilog_routing_channel_one_midout_name( des_cb_info,
                                                       src_rr_node->ptc_num));

  fprintf(fp, " -point_to_point"); 
  fprintf(fp, " -unconstrained"); 

  return;
}

/* Reporting timing from a SB output to another SB input
 */
static 
void verilog_generate_one_report_timing_sb_to_sb(FILE* fp,
                                                 const RRGSB& src_sb,
                                                 t_rr_node* src_rr_node,
                                                 const RRGSB& des_sb,
                                                 t_rr_node* des_rr_node) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ( (CHANX == src_rr_node->type) || (CHANY == src_rr_node->type) );
  assert ( (CHANX == des_rr_node->type) || (CHANY == des_rr_node->type) );

  fprintf(fp, "report_timing -from "); 

  /* output instance name */
  fprintf(fp, "%s/", 
          src_sb.gen_sb_verilog_instance_name()); 
  /* Find which side the ending pin locates, and determine the coordinate */
  dump_verilog_one_sb_chan_pin(fp, src_sb, src_rr_node, OUT_PORT); 

  fprintf(fp, " -to "); 

  /* output instance name */
  fprintf(fp, "%s/",
          des_sb.gen_sb_verilog_instance_name());
  /* Find which side the ending pin locates, and determine the coordinate */
  dump_verilog_one_sb_chan_pin(fp, des_sb, des_rr_node, IN_PORT); 

  fprintf(fp, " -point_to_point"); 
  fprintf(fp, " -unconstrained"); 

  return;
}


/* Reporting timing from a SB output to another SB input
 */
static 
void verilog_generate_one_report_timing_sb_to_sb(FILE* fp,
                                                 t_sb* src_sb_info,
                                                 t_rr_node* src_rr_node,
                                                 t_sb* des_sb_info,
                                                 t_rr_node* des_rr_node) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ( (CHANX == src_rr_node->type) || (CHANY == src_rr_node->type) );
  assert ( (CHANX == des_rr_node->type) || (CHANY == des_rr_node->type) );

  fprintf(fp, "report_timing -from "); 

  /* output instance name */
  fprintf(fp, "%s/", 
              gen_verilog_one_sb_instance_name(src_sb_info)); 
  /* Find which side the ending pin locates, and determine the coordinate */
  dump_verilog_one_sb_chan_pin(fp, src_sb_info, src_rr_node, OUT_PORT); 

  fprintf(fp, " -to "); 

  /* output instance name */
  fprintf(fp, "%s/",
          gen_verilog_one_sb_instance_name(des_sb_info));
  /* Find which side the ending pin locates, and determine the coordinate */
  dump_verilog_one_sb_chan_pin(fp, des_sb_info, des_rr_node, IN_PORT); 

  fprintf(fp, " -point_to_point"); 
  fprintf(fp, " -unconstrained"); 

  return;
}

static 
void build_ending_rr_node_for_one_sb_wire(t_rr_node* wire_rr_node, 
                                          t_rr_node* LL_rr_node, 
                                          int* num_end_rr_nodes,
                                          t_rr_node*** end_rr_node) {
  int x_end, y_end;
  int inode, iedge;
  int cur_node = 0;
  t_cb* next_cb = NULL;
  t_sb* next_sb = NULL;
  
  /* Initialization */
  (*num_end_rr_nodes) = 0;

  /* Find where the destination pin belongs to */
  get_chan_rr_node_end_coordinate(wire_rr_node, &x_end, &y_end);

  for (iedge = 0; iedge < wire_rr_node->num_edges; iedge++) {
    inode = wire_rr_node->edges[iedge];
    /* Build a list of ending rr_node we care */
    /* Find the SB/CB block that it belongs to */
    switch (LL_rr_node[inode].type) {
    case IPIN:
      /* Get the coordinate of ending CB */
      next_cb = get_chan_rr_node_ending_cb(wire_rr_node, &(LL_rr_node[inode]));
      /* This will not be the longest path unless the cb is close to the ending SB */
      if ((next_cb->x != x_end) || (next_cb->y != y_end)) {
        break;
      }
      (*num_end_rr_nodes)++;
      break;
    case CHANX:
    case CHANY:
      /* Get the coordinate of ending SB */
      next_sb = get_chan_rr_node_ending_sb(wire_rr_node, &(LL_rr_node[inode]));
      /* This will not be the longest path unless the cb is close to the ending SB */
      if ((next_sb->x != x_end) || (next_sb->y != y_end)) {
        break;
      }
      (*num_end_rr_nodes)++;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid type of ending point rr_node!\n",
                 __FILE__, __LINE__);
 
      exit(1);
    }
  }

  /* Malloc */
  (*end_rr_node) = (t_rr_node**) my_calloc((*num_end_rr_nodes), sizeof(t_rr_node*));

  cur_node = 0;
  for (iedge = 0; iedge < wire_rr_node->num_edges; iedge++) {
    inode = wire_rr_node->edges[iedge];
    /* Build a list of ending rr_node we care */
    /* Find the SB/CB block that it belongs to */
    switch (LL_rr_node[inode].type) {
    case IPIN:
      /* Get the coordinate of ending CB */
      next_cb = get_chan_rr_node_ending_cb(wire_rr_node, &(LL_rr_node[inode]));
      /* This will not be the longest path unless the cb is close to the ending SB */
      if ((next_cb->x != x_end) || (next_cb->y != y_end)) {
        break;
      }
      (*end_rr_node)[cur_node] = &(LL_rr_node[inode]);
      cur_node++;
      break;
    case CHANX:
    case CHANY:
      /* Get the coordinate of ending SB */
      next_sb = get_chan_rr_node_ending_sb(wire_rr_node, &(LL_rr_node[inode]));
      /* This will not be the longest path unless the cb is close to the ending SB */
      if ((next_sb->x != x_end) || (next_sb->y != y_end)) {
        break;
      }
      (*end_rr_node)[cur_node] = &(LL_rr_node[inode]);
      cur_node++;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid type of ending point rr_node!\n",
                 __FILE__, __LINE__);
 
      exit(1);
    }
  }

  /* Check */
  assert( (*num_end_rr_nodes) == cur_node);

  return;
}

/* Generate the report timing commands for a through wire across two SBs
 * This includes a sb-to-sb wire and a within-sb wire
 *  --------       -------- 
 * | src_sb |      |des_sb |
 * |        |----->|------>|
 * |[x-1][y]|      | [x][y]|
 *  --------       -------- 
 */
static 
void verilog_generate_report_timing_one_sb_thru_segments(FILE* fp, 
                                                         RRGSB src_sb,
                                                         t_rr_node* src_rr_node, 
                                                         RRGSB des_sb,
                                                         t_rr_node* des_rr_node,
                                                         char* rpt_name) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Report timing for the downstream segements, from a SB output to an adjacent SB input */
  verilog_generate_one_report_timing_sb_to_sb(fp, src_sb, src_rr_node, 
                                              des_sb, des_rr_node);

  if (NULL != rpt_name) {
    fprintf(fp, " >> %s\n", rpt_name); 
  } else {
    fprintf(fp, "\n"); 
  }

  /* Report timing for the downstream segements, within a SB */
  verilog_generate_one_report_timing_within_sb(fp, des_sb,
                                               des_rr_node, des_rr_node);

  if (NULL != rpt_name) {
    fprintf(fp, " >> %s\n", rpt_name); 
  } else {
    fprintf(fp, "\n"); 
  }

  return;
}


/* Generate the report timing commands for a through wire across two SBs
 * This includes a sb-to-sb wire and a within-sb wire
 *  --------       -------- 
 * | src_sb |      |des_sb |
 * |        |----->|------>|
 * |[x-1][y]|      | [x][y]|
 *  --------       -------- 
 */
static 
void verilog_generate_report_timing_one_sb_thru_segments(FILE* fp, 
                                                         t_sb* src_sb_info,
                                                         t_rr_node* src_rr_node, 
                                                         t_sb* des_sb_info,
                                                         t_rr_node* des_rr_node,
                                                         char* rpt_name) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 


  /* Report timing for the downstream segements, from a SB output to an adjacent SB input */
  verilog_generate_one_report_timing_sb_to_sb(fp, src_sb_info, src_rr_node, 
                                              des_sb_info, des_rr_node);

  if (NULL != rpt_name) {
    fprintf(fp, " >> %s\n", rpt_name); 
  } else {
    fprintf(fp, "\n"); 
  }

  /* Report timing for the downstream segements, within a SB */
  verilog_generate_one_report_timing_within_sb(fp, des_sb_info,
                                               des_rr_node, des_rr_node);

  if (NULL != rpt_name) {
    fprintf(fp, " >> %s\n", rpt_name); 
  } else {
    fprintf(fp, "\n"); 
  }

  return;
}

/* Generate the report timing commands for a through wire across two SBs
 * This includes either a sb-to-sb wire or a sb-to-cb wire
 *  --------       ------- 
 * | src_sb |      |des_cb |
 * |        |----->|------>|
 * |[x-1][y]|      | [x][y]|
 *  --------       ------- 
 */
static 
void verilog_generate_report_timing_one_sb_ending_segments(FILE* fp, 
                                                           const RRGSB& src_sb,
                                                           t_rr_node* src_rr_node, 
                                                           t_rr_node* des_rr_node,
                                                           char* rpt_name) {
  t_cb* next_cb = NULL;
  DeviceCoordinator next_sb_coordinator;
  RRGSB next_sb;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  switch (des_rr_node->type) {
  case IPIN:
    /* Get the coordinate of ending CB */
    next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
    verilog_generate_one_report_timing_sb_to_cb(fp, src_sb, src_rr_node, 
                                                next_cb, des_rr_node);
    break;
  case CHANX:
  case CHANY:
    /* Get the coordinate of ending SB */
    next_sb_coordinator = get_chan_node_ending_sb_coordinator(src_rr_node, des_rr_node);
    next_sb = device_rr_gsb.get_gsb(next_sb_coordinator); 
    verilog_generate_one_report_timing_sb_to_sb(fp, src_sb, src_rr_node, 
                                                next_sb, src_rr_node);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid type of ending point rr_node!\n",
               __FILE__, __LINE__);
 
    exit(1);
  }

  if (NULL != rpt_name) {
    fprintf(fp, " >> %s\n", rpt_name); 
  } else {
    fprintf(fp, "\n"); 
  }

  return;
}


/* Generate the report timing commands for a through wire across two SBs
 * This includes either a sb-to-sb wire or a sb-to-cb wire
 *  --------       ------- 
 * | src_sb |      |des_cb |
 * |        |----->|------>|
 * |[x-1][y]|      | [x][y]|
 *  --------       ------- 
 */
static 
void verilog_generate_report_timing_one_sb_ending_segments(FILE* fp, 
                                                           t_sb* src_sb_info,
                                                           t_rr_node* src_rr_node, 
                                                           t_rr_node* des_rr_node,
                                                           char* rpt_name) {
  t_cb* next_cb = NULL;
  t_sb* next_sb = NULL;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  switch (des_rr_node->type) {
  case IPIN:
    /* Get the coordinate of ending CB */
    next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
    verilog_generate_one_report_timing_sb_to_cb(fp, src_sb_info, src_rr_node, 
                                                next_cb, des_rr_node);
    break;
  case CHANX:
  case CHANY:
    /* Get the coordinate of ending SB */
    next_sb = get_chan_rr_node_ending_sb(src_rr_node, des_rr_node);
    verilog_generate_one_report_timing_sb_to_sb(fp, src_sb_info, src_rr_node, 
                                                next_sb, src_rr_node);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid type of ending point rr_node!\n",
               __FILE__, __LINE__);
 
    exit(1);
  }

  if (NULL != rpt_name) {
    fprintf(fp, " >> %s\n", rpt_name); 
  } else {
    fprintf(fp, "\n"); 
  }

  return;
}

/* Print the pins of SBs that a routing wire will go through 
 * from the src_rr_node to the des_rr_node 
 */
static 
void dump_verilog_one_sb_wire_segemental_report_timing(FILE* fp,
                                                       t_syn_verilog_opts fpga_verilog_opts,
                                                       const RRGSB& src_sb,
                                                       t_rr_node* drive_rr_node, 
                                                       t_rr_node* src_rr_node, 
                                                       t_rr_node* des_rr_node,
                                                       char* direction,
                                                       int path_cnt) {
  int L_wire;
  int cur_sb_x, cur_sb_y;
  int end_sb_x, end_sb_y;
  t_cb* next_cb = NULL;
  char* rpt_name = NULL;
  DeviceCoordinator next_sb_coordinator;
  RRGSB next_sb;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ((INC_DIRECTION == src_rr_node->direction)
        ||(DEC_DIRECTION == src_rr_node->direction));
  assert ((CHANX == src_rr_node->type)
        ||(CHANY == src_rr_node->type));

  L_wire = get_rr_node_wire_length(src_rr_node);

  /* Get report name */
  rpt_name = gen_verilog_one_routing_report_timing_rpt_name(fpga_verilog_opts.report_timing_path, direction,
                                                            L_wire, path_cnt);

  /* Start printing report timing info  */
  fprintf(fp, "# L%d wire, Path ID: %d\n", 
          L_wire,
          path_cnt); 
  fprintf(fp, "report_timing -from "); 

  /* output instance name */
  fprintf(fp, "%s/", 
          src_sb.gen_sb_verilog_instance_name()); 
  /* Find which side the ending pin locates, and determine the coordinate */
  dump_verilog_one_sb_routing_pin(fp, src_sb, drive_rr_node);

  /* Switch depends on the type of des_rr_node  */
  switch(des_rr_node->type) {
  /* Range of SBs that on the path                        
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y]  |        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *                             ---------
   *                            |         |        
   *                            | thru_cb |
   *                            |         |        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *  --------      -------      ---------      -------      --------
   * |        |    |       |    |         |    |       |    |        |
   * | des_sb |<---|thru_cb|<---| src_sb  |--->|thru_cb|--->| des_sb |
   * |[x-1][y]|    | [x][y]|    |         |    | [x][y]|    |[x][y]  |
   *  --------      -------      ---------      -------      --------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            | thru_cb |
   *                            |         |        
   *                             ---------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y-1]|        
   *                             ---------
   */
  case IPIN: 
    /* Get the coordinate of ending CB */
    next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
    assert(next_cb->type == src_rr_node->type);
    /* 4 cases: */
    if ((INC_DIRECTION == src_rr_node->direction) 
       &&(CHANX == src_rr_node->type)) {
      end_sb_x = next_cb->x; 
      end_sb_y = next_cb->y;
    } else if ((INC_DIRECTION == src_rr_node->direction) 
       &&(CHANY == src_rr_node->type)) {
      end_sb_x = next_cb->x; 
      end_sb_y = next_cb->y;
    } else if ((DEC_DIRECTION == src_rr_node->direction) 
       &&(CHANX == src_rr_node->type)) {
      end_sb_x = next_cb->x - 1; 
      end_sb_y = next_cb->y;
    } else if ((DEC_DIRECTION == src_rr_node->direction) 
       &&(CHANY == src_rr_node->type)) {
      end_sb_x = next_cb->x; 
      end_sb_y = next_cb->y - 1;
    }
    break;
  /* Range of SBs that on the path                        
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y+1]|        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *                             ---------
   *                            |         |        
   *                            | thru_sb |
   *                            |         |        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *  --------      -------      ---------      -------      --------
   * |        |    |       |    |         |    |       |    |        |
   * | des_sb |<---|thru_sb|<---| src_sb  |--->|thru_sb|--->| des_sb |
   * |[x-1][y]|    | [x][y]|    |         |    | [x][y]|    |[x+1][y]|
   *  --------      -------      ---------      -------      --------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            | thru_sb |
   *                            |         |        
   *                             ---------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y-1]|        
   *                             ---------
   */
  case CHANX:
  case CHANY:
    /* Get the coordinate of ending CB */
    next_sb_coordinator = get_chan_node_ending_sb_coordinator(src_rr_node, des_rr_node);
    next_sb = device_rr_gsb.get_gsb(next_sb_coordinator);
    end_sb_x = next_sb.get_sb_x(); 
    end_sb_y = next_sb.get_sb_y();
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid type of rr_node!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Get the base coordinate of src_sb */
  cur_sb_x = src_sb.get_sb_x();
  cur_sb_y = src_sb.get_sb_y();
  /* 4 cases: */
  if ((INC_DIRECTION == src_rr_node->direction) 
     &&(CHANX == src_rr_node->type)) {
    /* Follow the graph above, go through X channel */
    for (int ix = src_sb.get_sb_x(); ix < end_sb_x; ix++) {
      DeviceCoordinator begin_sb_coordinator(ix, cur_sb_y);
      RRGSB begin_sb = device_rr_gsb.get_gsb(begin_sb_coordinator);
      /* If this is the ending point, we add a ending segment */
      if (ix == end_sb_x - 1) {
        switch (des_rr_node->type) {
        case IPIN:
          /* Get the coordinate of ending CB */
          next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
          fprintf(fp, " -to "); 
          /* output instance name */
          fprintf(fp, "%s/",
                  gen_verilog_one_cb_instance_name(next_cb));
          /* output pin name */
          fprintf(fp, "%s",
                  gen_verilog_routing_channel_one_midout_name( next_cb,
                                                               src_rr_node->ptc_num));

          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
          break;
        case CHANX:
        case CHANY:
          /* Get the coordinate of ending SB */
          next_sb_coordinator = get_chan_node_ending_sb_coordinator(src_rr_node, des_rr_node);
          next_sb = device_rr_gsb.get_gsb(next_sb_coordinator); 

          fprintf(fp, " -to "); 
        
          /* output instance name */
          fprintf(fp, "%s/",
                  next_sb.gen_sb_verilog_instance_name());
          /* Find which side the ending pin locates, and determine the coordinate */
          dump_verilog_one_sb_chan_pin(fp, next_sb, src_rr_node, IN_PORT); 
        
          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
        
          break;
        default:
          break;
        }
      }
    }
  } else if ((INC_DIRECTION == src_rr_node->direction) 
     &&(CHANY == src_rr_node->type)) {
      /* Follow the graph above, go through Y channel */
      for (int iy = src_sb.get_sb_y(); iy < end_sb_y; iy++) {
        DeviceCoordinator begin_sb_coordinator(cur_sb_x, iy);
        RRGSB begin_sb = device_rr_gsb.get_gsb(begin_sb_coordinator);
       /* If this is the ending point, we add a ending segment */
        if (iy == end_sb_y - 1) {
        switch (des_rr_node->type) {
        case IPIN:
          /* Get the coordinate of ending CB */
          next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
          fprintf(fp, " -to "); 
          /* output instance name */
          fprintf(fp, "%s/",
                  gen_verilog_one_cb_instance_name(next_cb));
          /* output pin name */
          fprintf(fp, "%s",
                  gen_verilog_routing_channel_one_midout_name( next_cb,
                                                               src_rr_node->ptc_num));
  
          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
          break;
        case CHANX:
        case CHANY:
          /* Get the coordinate of ending SB */
          next_sb_coordinator = get_chan_node_ending_sb_coordinator(src_rr_node, des_rr_node);
          next_sb = device_rr_gsb.get_gsb(next_sb_coordinator); 
  
          fprintf(fp, " -to "); 
        
          /* output instance name */
          fprintf(fp, "%s/",
                  next_sb.gen_sb_verilog_instance_name());
          /* Find which side the ending pin locates, and determine the coordinate */
          dump_verilog_one_sb_chan_pin(fp, next_sb, src_rr_node, IN_PORT); 
        
          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
        
          break;
        }
      }
    }
  } else if ((DEC_DIRECTION == src_rr_node->direction) 
     &&(CHANX == src_rr_node->type)) {
      /* Follow the graph above, go through X channel */
      for (int ix = src_sb.get_sb_x() - 1; ix > end_sb_x; ix--) {
        DeviceCoordinator begin_sb_coordinator(ix, cur_sb_y);
        RRGSB begin_sb = device_rr_gsb.get_gsb(begin_sb_coordinator);
        /* If this is the ending point, we add a ending segment */
        if (ix == end_sb_x + 1) {
        switch (des_rr_node->type) {
        case IPIN:
          /* Get the coordinate of ending CB */
          next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
          fprintf(fp, " -to "); 
          /* output instance name */
          fprintf(fp, "%s/",
                  gen_verilog_one_cb_instance_name(next_cb));
          /* output pin name */
          fprintf(fp, "%s",
                  gen_verilog_routing_channel_one_midout_name( next_cb,
                                                               src_rr_node->ptc_num));

          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
          break;
        case CHANX:
        case CHANY:
          /* Get the coordinate of ending SB */
          next_sb_coordinator = get_chan_node_ending_sb_coordinator(src_rr_node, des_rr_node);
          next_sb = device_rr_gsb.get_gsb(next_sb_coordinator); 

          fprintf(fp, " -to "); 
        
          /* output instance name */
          fprintf(fp, "%s/",
                  next_sb.gen_sb_verilog_instance_name());
          /* Find which side the ending pin locates, and determine the coordinate */
          dump_verilog_one_sb_chan_pin(fp, next_sb, src_rr_node, IN_PORT); 
        
          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
        
          break;
        }
      }
    }
  } else if ((DEC_DIRECTION == src_rr_node->direction) 
     &&(CHANY == src_rr_node->type)) {
      /* Follow the graph above, go through Y channel */
      for (int iy = src_sb.get_sb_y() - 1; iy > end_sb_y; iy--) {
        DeviceCoordinator begin_sb_coordinator(cur_sb_x, iy);
        RRGSB begin_sb = device_rr_gsb.get_gsb(begin_sb_coordinator);
       /* If this is the ending point, we add a ending segment */
        if (iy == end_sb_y + 1) {
        switch (des_rr_node->type) {
        case IPIN:
          /* Get the coordinate of ending CB */
          next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
          fprintf(fp, " -to "); 
          /* output instance name */
          fprintf(fp, "%s/",
                  gen_verilog_one_cb_instance_name(next_cb));
          /* output pin name */
          fprintf(fp, "%s",
                  gen_verilog_routing_channel_one_midout_name( next_cb,
                                                               src_rr_node->ptc_num));

          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
          break;
        case CHANX:
        case CHANY:
          /* Get the coordinate of ending SB */
          next_sb_coordinator = get_chan_node_ending_sb_coordinator(src_rr_node, des_rr_node);
          next_sb = device_rr_gsb.get_gsb(next_sb_coordinator); 

          fprintf(fp, " -to "); 
        
          /* output instance name */
          fprintf(fp, "%s/",
                  next_sb.gen_sb_verilog_instance_name());
          /* Find which side the ending pin locates, and determine the coordinate */
          dump_verilog_one_sb_chan_pin(fp, next_sb, src_rr_node, IN_PORT); 
        
          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
        
          break;
        } 
      }
    }
  }

  if (NULL != rpt_name) {
    fprintf(fp, " > %s\n", rpt_name); 
  } else {
    fprintf(fp, "\n"); 
  }

  /* Free */
  my_free(rpt_name);

  return;
}


/* Print the pins of SBs that a routing wire will go through 
 * from the src_rr_node to the des_rr_node 
 */
static 
void dump_verilog_one_sb_wire_segemental_report_timing(FILE* fp,
                                                       t_syn_verilog_opts fpga_verilog_opts,
                                                       t_sb* src_sb_info,
                                                       t_rr_node* drive_rr_node, 
                                                       t_rr_node* src_rr_node, 
                                                       t_rr_node* des_rr_node,
                                                       char* direction,
                                                       int path_cnt) {
  int L_wire;
  int ix, iy;
  int cur_sb_x, cur_sb_y;
  int end_sb_x, end_sb_y;
  t_cb* next_cb = NULL;
  t_sb* next_sb = NULL;
  char* rpt_name = NULL;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ((INC_DIRECTION == src_rr_node->direction)
        ||(DEC_DIRECTION == src_rr_node->direction));
  assert ((CHANX == src_rr_node->type)
        ||(CHANY == src_rr_node->type));

  L_wire = get_rr_node_wire_length(src_rr_node);

  /* Get report name */
  rpt_name = gen_verilog_one_routing_report_timing_rpt_name(fpga_verilog_opts.report_timing_path, direction,
                                                            L_wire, path_cnt);

  /* Start printing report timing info  */
  fprintf(fp, "# L%d wire, Path ID: %d\n", 
          L_wire,
          path_cnt); 

  fprintf(fp, "report_timing -from "); 

  /* output instance name */
  fprintf(fp, "%s/", 
              gen_verilog_one_sb_instance_name(src_sb_info)); 
  /* Find which side the ending pin locates, and determine the coordinate */
  dump_verilog_one_sb_routing_pin(fp, src_sb_info, drive_rr_node);

  /* Switch depends on the type of des_rr_node  */
  switch(des_rr_node->type) {
  /* Range of SBs that on the path                        
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y]  |        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *                             ---------
   *                            |         |        
   *                            | thru_cb |
   *                            |         |        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *  --------      -------      ---------      -------      --------
   * |        |    |       |    |         |    |       |    |        |
   * | des_sb |<---|thru_cb|<---| src_sb  |--->|thru_cb|--->| des_sb |
   * |[x-1][y]|    | [x][y]|    |         |    | [x][y]|    |[x][y]  |
   *  --------      -------      ---------      -------      --------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            | thru_cb |
   *                            |         |        
   *                             ---------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y-1]|        
   *                             ---------
   */
  case IPIN: 
    /* Get the coordinate of ending CB */
    next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
    assert(next_cb->type == src_rr_node->type);
    /* 4 cases: */
    if ((INC_DIRECTION == src_rr_node->direction) 
       &&(CHANX == src_rr_node->type)) {
      end_sb_x = next_cb->x; 
      end_sb_y = next_cb->y;
    } else if ((INC_DIRECTION == src_rr_node->direction) 
       &&(CHANY == src_rr_node->type)) {
      end_sb_x = next_cb->x; 
      end_sb_y = next_cb->y;
    } else if ((DEC_DIRECTION == src_rr_node->direction) 
       &&(CHANX == src_rr_node->type)) {
      end_sb_x = next_cb->x - 1; 
      end_sb_y = next_cb->y;
    } else if ((DEC_DIRECTION == src_rr_node->direction) 
       &&(CHANY == src_rr_node->type)) {
      end_sb_x = next_cb->x; 
      end_sb_y = next_cb->y - 1;
    }
    break;
  /* Range of SBs that on the path                        
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y+1]|        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *                             ---------
   *                            |         |        
   *                            | thru_sb |
   *                            |         |        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *  --------      -------      ---------      -------      --------
   * |        |    |       |    |         |    |       |    |        |
   * | des_sb |<---|thru_sb|<---| src_sb  |--->|thru_sb|--->| des_sb |
   * |[x-1][y]|    | [x][y]|    |         |    | [x][y]|    |[x+1][y]|
   *  --------      -------      ---------      -------      --------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            | thru_sb |
   *                            |         |        
   *                             ---------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y-1]|        
   *                             ---------
   */
  case CHANX:
  case CHANY:
    /* Get the coordinate of ending CB */
    next_sb = get_chan_rr_node_ending_sb(src_rr_node, des_rr_node);
    end_sb_x = next_sb->x; 
    end_sb_y = next_sb->y;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid type of rr_node!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Get the base coordinate of src_sb */
  cur_sb_x = src_sb_info->x;
  cur_sb_y = src_sb_info->y;
  /* 4 cases: */
  if ((INC_DIRECTION == src_rr_node->direction) 
     &&(CHANX == src_rr_node->type)) {
    /* Follow the graph above, go through X channel */
    for (ix = src_sb_info->x; ix < end_sb_x; ix++) {
      /* If this is the ending point, we add a ending segment */
      if (ix == end_sb_x - 1) {
        switch (des_rr_node->type) {
        case IPIN:
          /* Get the coordinate of ending CB */
          next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
          fprintf(fp, " -to "); 
          fprintf(fp, "%s/",
                  gen_verilog_one_cb_instance_name(next_cb));
          /* output pin name */
          fprintf(fp, "%s",
                  gen_verilog_routing_channel_one_midout_name( next_cb,
                                                               src_rr_node->ptc_num));
          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
          break;
        case CHANX:
        case CHANY:
          /* Get the coordinate of ending SB */
          next_sb = get_chan_rr_node_ending_sb(src_rr_node, des_rr_node);
          fprintf(fp, " -to "); 
          fprintf(fp, "%s/", 
                      gen_verilog_one_sb_instance_name(next_sb)); 
          /* Find which side the ending pin locates, and determine the coordinate */
          dump_verilog_one_sb_chan_pin(fp, next_sb, src_rr_node, IN_PORT); 

          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
        }
      }
    }
  } else if ((INC_DIRECTION == src_rr_node->direction) 
     &&(CHANY == src_rr_node->type)) {
    /* Follow the graph above, go through Y channel */
    for (iy = src_sb_info->y; iy < end_sb_y; iy++) {
     /* If this is the ending point, we add a ending segment */
      if (iy == end_sb_y - 1) {
        switch (des_rr_node->type) {
        case IPIN:
          /* Get the coordinate of ending CB */
          next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
          fprintf(fp, " -to "); 
          fprintf(fp, "%s/",
                  gen_verilog_one_cb_instance_name(next_cb));
          /* output pin name */
          fprintf(fp, "%s",
                  gen_verilog_routing_channel_one_midout_name( next_cb,
                                                               src_rr_node->ptc_num));
          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
          break;
        case CHANX:
        case CHANY:
          /* Get the coordinate of ending SB */
          next_sb = get_chan_rr_node_ending_sb(src_rr_node, des_rr_node);
          fprintf(fp, " -to "); 
          fprintf(fp, "%s/", 
                      gen_verilog_one_sb_instance_name(next_sb)); 
          /* Find which side the ending pin locates, and determine the coordinate */
          dump_verilog_one_sb_chan_pin(fp, next_sb, src_rr_node, IN_PORT); 

          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
        }
      }
    }
  } else if ((DEC_DIRECTION == src_rr_node->direction) 
     &&(CHANX == src_rr_node->type)) {
    /* Follow the graph above, go through X channel */
    for (ix = src_sb_info->x - 1; ix > end_sb_x; ix--) {
      /* If this is the ending point, we add a ending segment */
      if (ix == end_sb_x + 1) {
        switch (des_rr_node->type) {
        case IPIN:
          /* Get the coordinate of ending CB */
          next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
          fprintf(fp, " -to "); 
          fprintf(fp, "%s/",
                  gen_verilog_one_cb_instance_name(next_cb));
          /* output pin name */
          fprintf(fp, "%s",
                  gen_verilog_routing_channel_one_midout_name( next_cb,
                                                               src_rr_node->ptc_num));
          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
          break;
        case CHANX:
        case CHANY:
          /* Get the coordinate of ending SB */
          next_sb = get_chan_rr_node_ending_sb(src_rr_node, des_rr_node);
          fprintf(fp, " -to "); 
          fprintf(fp, "%s/", 
                      gen_verilog_one_sb_instance_name(next_sb)); 
          /* Find which side the ending pin locates, and determine the coordinate */
          dump_verilog_one_sb_chan_pin(fp, next_sb, src_rr_node, IN_PORT); 

          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
        }
      }
    }
  } else if ((DEC_DIRECTION == src_rr_node->direction) 
     &&(CHANY == src_rr_node->type)) {
    /* Follow the graph above, go through Y channel */
    for (iy = src_sb_info->y - 1; iy > end_sb_y; iy--) {
     /* If this is the ending point, we add a ending segment */
      if (iy == end_sb_y + 1) {
        switch (des_rr_node->type) {
        case IPIN:
          /* Get the coordinate of ending CB */
          next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
          fprintf(fp, " -to "); 
          fprintf(fp, "%s/",
                  gen_verilog_one_cb_instance_name(next_cb));
          /* output pin name */
          fprintf(fp, "%s",
                  gen_verilog_routing_channel_one_midout_name( next_cb,
                                                               src_rr_node->ptc_num));
          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
          break;
        case CHANX:
        case CHANY:
          /* Get the coordinate of ending SB */
          next_sb = get_chan_rr_node_ending_sb(src_rr_node, des_rr_node);
          fprintf(fp, " -to "); 
          fprintf(fp, "%s/", 
                      gen_verilog_one_sb_instance_name(next_sb)); 
          /* Find which side the ending pin locates, and determine the coordinate */
          dump_verilog_one_sb_chan_pin(fp, next_sb, src_rr_node, IN_PORT); 

          fprintf(fp, " -point_to_point"); 
          fprintf(fp, " -unconstrained"); 
        }
      }
    }
  }

  if (NULL != rpt_name) {
    fprintf(fp, " > %s\n", rpt_name); 
  } else {
    fprintf(fp, "\n"); 
  }

  /* Free */
  my_free(rpt_name);

  return;
}

///* Print the pins of SBs that a routing wire will go through 
// * from the src_rr_node to the des_rr_node 
// */
//static 
//void dump_verilog_one_sb_wire_segemental_report_timing(FILE* fp,
//                                                       t_syn_verilog_opts fpga_verilog_opts,
//                                                       const RRGSB& src_sb,
//                                                       t_rr_node* drive_rr_node, 
//                                                       t_rr_node* src_rr_node, 
//                                                       t_rr_node* des_rr_node,
//                                                       int path_cnt) {
//  int L_wire;
//  int cur_sb_x, cur_sb_y;
//  int end_sb_x, end_sb_y;
//  t_cb* next_cb = NULL;
//  char* rpt_name = NULL;
//  DeviceCoordinator next_sb_coordinator;
//  RRGSB next_sb;
//
//  /* Check the file handler */
//  if (NULL == fp) {
//    vpr_printf(TIO_MESSAGE_ERROR,
//               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
//               __FILE__, __LINE__); 
//    exit(1);
//  } 
//
//  /* Check */
//  assert ((INC_DIRECTION == src_rr_node->direction)
//        ||(DEC_DIRECTION == src_rr_node->direction));
//  assert ((CHANX == src_rr_node->type)
//        ||(CHANY == src_rr_node->type));
//
//  L_wire = get_rr_node_wire_length(src_rr_node);
//
//  /* Get report name */
//  rpt_name = gen_verilog_one_routing_report_timing_rpt_name(fpga_verilog_opts.report_timing_path,
//                                                            L_wire, path_cnt);
//
//  /* Start printing report timing info  */
//  fprintf(fp, "# L%d wire, Path ID: %d\n", 
//          L_wire,
//          path_cnt); 
//  /* Report timing for the SB MUX delay, from the drive_rr_node to the wire_rr_node */
//  verilog_generate_one_report_timing_within_sb(fp, src_sb, 
//                                               drive_rr_node, 
//                                               src_rr_node); 
//  if (NULL != rpt_name) {
//    fprintf(fp, " > %s\n", rpt_name); 
//  } else {
//    fprintf(fp, "\n"); 
//  }
//
//  /* Switch depends on the type of des_rr_node  */
//  switch(des_rr_node->type) {
//  /* Range of SBs that on the path                        
//   *                             ---------
//   *                            |         |        
//   *                            |  des_sb |
//   *                            | [x][y]  |        
//   *                             ---------
//   *                                /|\ 
//   *                                 |  
//   *                             ---------
//   *                            |         |        
//   *                            | thru_cb |
//   *                            |         |        
//   *                             ---------
//   *                                /|\ 
//   *                                 |  
//   *  --------      -------      ---------      -------      --------
//   * |        |    |       |    |         |    |       |    |        |
//   * | des_sb |<---|thru_cb|<---| src_sb  |--->|thru_cb|--->| des_sb |
//   * |[x-1][y]|    | [x][y]|    |         |    | [x][y]|    |[x][y]  |
//   *  --------      -------      ---------      -------      --------
//   *                                 |
//   *                                \|/
//   *                             ---------
//   *                            |         |        
//   *                            | thru_cb |
//   *                            |         |        
//   *                             ---------
//   *                                 |
//   *                                \|/
//   *                             ---------
//   *                            |         |        
//   *                            |  des_sb |
//   *                            | [x][y-1]|        
//   *                             ---------
//   */
//  case IPIN: 
//    /* Get the coordinate of ending CB */
//    next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
//    assert(next_cb->type == src_rr_node->type);
//    /* 4 cases: */
//    if ((INC_DIRECTION == src_rr_node->direction) 
//       &&(CHANX == src_rr_node->type)) {
//      end_sb_x = next_cb->x; 
//      end_sb_y = next_cb->y;
//    } else if ((INC_DIRECTION == src_rr_node->direction) 
//       &&(CHANY == src_rr_node->type)) {
//      end_sb_x = next_cb->x; 
//      end_sb_y = next_cb->y;
//    } else if ((DEC_DIRECTION == src_rr_node->direction) 
//       &&(CHANX == src_rr_node->type)) {
//      end_sb_x = next_cb->x - 1; 
//      end_sb_y = next_cb->y;
//    } else if ((DEC_DIRECTION == src_rr_node->direction) 
//       &&(CHANY == src_rr_node->type)) {
//      end_sb_x = next_cb->x; 
//      end_sb_y = next_cb->y - 1;
//    }
//    break;
//  /* Range of SBs that on the path                        
//   *                             ---------
//   *                            |         |        
//   *                            |  des_sb |
//   *                            | [x][y+1]|        
//   *                             ---------
//   *                                /|\ 
//   *                                 |  
//   *                             ---------
//   *                            |         |        
//   *                            | thru_sb |
//   *                            |         |        
//   *                             ---------
//   *                                /|\ 
//   *                                 |  
//   *  --------      -------      ---------      -------      --------
//   * |        |    |       |    |         |    |       |    |        |
//   * | des_sb |<---|thru_sb|<---| src_sb  |--->|thru_sb|--->| des_sb |
//   * |[x-1][y]|    | [x][y]|    |         |    | [x][y]|    |[x+1][y]|
//   *  --------      -------      ---------      -------      --------
//   *                                 |
//   *                                \|/
//   *                             ---------
//   *                            |         |        
//   *                            | thru_sb |
//   *                            |         |        
//   *                             ---------
//   *                                 |
//   *                                \|/
//   *                             ---------
//   *                            |         |        
//   *                            |  des_sb |
//   *                            | [x][y-1]|        
//   *                             ---------
//   */
//  case CHANX:
//  case CHANY:
//    /* Get the coordinate of ending CB */
//    next_sb_coordinator = get_chan_node_ending_sb_coordinator(src_rr_node, des_rr_node);
//    next_sb = device_rr_gsb.get_gsb(next_sb_coordinator);
//    end_sb_x = next_sb.get_sb_x(); 
//    end_sb_y = next_sb.get_sb_y();
//    break;
//  default:
//    vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid type of rr_node!\n",
//               __FILE__, __LINE__);
//    exit(1);
//  }
//
//  /* Get the base coordinate of src_sb */
//  cur_sb_x = src_sb.get_sb_x();
//  cur_sb_y = src_sb.get_sb_y();
//  /* 4 cases: */
//  if ((INC_DIRECTION == src_rr_node->direction) 
//     &&(CHANX == src_rr_node->type)) {
//    /* Follow the graph above, go through X channel */
//    for (int ix = src_sb.get_sb_x(); ix < end_sb_x; ix++) {
//      DeviceCoordinator begin_sb_coordinator(ix, cur_sb_y);
//      RRGSB begin_sb = device_rr_gsb.get_gsb(begin_sb_coordinator);
//      /* If this is the ending point, we add a ending segment */
//      if (ix == end_sb_x - 1) {
//        verilog_generate_report_timing_one_sb_ending_segments(fp,
//                                                              begin_sb, src_rr_node, 
//                                                              des_rr_node, 
//                                                              rpt_name); 
//
//        continue;
//      }
//      /* Report timing for the downstream segements, from a SB output to an adjacent CB input */
//      DeviceCoordinator end_sb_coordinator(ix + 1, cur_sb_y);
//      RRGSB end_sb = device_rr_gsb.get_gsb(end_sb_coordinator);
//      verilog_generate_report_timing_one_sb_thru_segments(fp,
//                                                          begin_sb, src_rr_node, 
//                                                          end_sb, src_rr_node,
//                                                          rpt_name); 
//    }
//  } else if ((INC_DIRECTION == src_rr_node->direction) 
//     &&(CHANY == src_rr_node->type)) {
//    /* Follow the graph above, go through Y channel */
//    for (int iy = src_sb.get_sb_y(); iy < end_sb_y; iy++) {
//      DeviceCoordinator begin_sb_coordinator(cur_sb_x, iy);
//      RRGSB begin_sb = device_rr_gsb.get_gsb(begin_sb_coordinator);
//     /* If this is the ending point, we add a ending segment */
//      if (iy == end_sb_y - 1) {
//        verilog_generate_report_timing_one_sb_ending_segments(fp,
//                                                              begin_sb, src_rr_node, 
//                                                              des_rr_node, 
//                                                              rpt_name); 
//        continue;
//      }
//      /* Report timing for the downstream segements, from a SB output to an adjacent CB input */
//      DeviceCoordinator end_sb_coordinator(cur_sb_x, iy + 1);
//      RRGSB end_sb = device_rr_gsb.get_gsb(end_sb_coordinator);
//      verilog_generate_report_timing_one_sb_thru_segments(fp,
//                                                          begin_sb, src_rr_node, 
//                                                          end_sb, src_rr_node,
//                                                          rpt_name); 
//    }
//  } else if ((DEC_DIRECTION == src_rr_node->direction) 
//     &&(CHANX == src_rr_node->type)) {
//    /* Follow the graph above, go through X channel */
//    for (int ix = src_sb.get_sb_x() - 1; ix > end_sb_x; ix--) {
//      DeviceCoordinator begin_sb_coordinator(ix, cur_sb_y);
//      RRGSB begin_sb = device_rr_gsb.get_gsb(begin_sb_coordinator);
//      /* If this is the ending point, we add a ending segment */
//      if (ix == end_sb_x + 1) {
//        verilog_generate_report_timing_one_sb_ending_segments(fp,
//                                                              begin_sb, src_rr_node, 
//                                                              des_rr_node, 
//                                                              rpt_name); 
//        continue;
//      }
//      /* Report timing for the downstream segements, from a SB output to an adjacent CB input */
//      DeviceCoordinator end_sb_coordinator(ix - 1, cur_sb_y);
//      RRGSB end_sb = device_rr_gsb.get_gsb(end_sb_coordinator);
//      verilog_generate_report_timing_one_sb_thru_segments(fp,
//                                                          begin_sb, src_rr_node, 
//                                                          end_sb, src_rr_node,
//                                                          rpt_name); 
//    }
//  } else if ((DEC_DIRECTION == src_rr_node->direction) 
//     &&(CHANY == src_rr_node->type)) {
//    /* Follow the graph above, go through Y channel */
//    for (int iy = src_sb.get_sb_y() - 1; iy > end_sb_y; iy--) {
//      DeviceCoordinator begin_sb_coordinator(cur_sb_x, iy);
//      RRGSB begin_sb = device_rr_gsb.get_gsb(begin_sb_coordinator);
//     /* If this is the ending point, we add a ending segment */
//      if (iy == end_sb_y + 1) {
//        verilog_generate_report_timing_one_sb_ending_segments(fp,
//                                                              begin_sb, src_rr_node, 
//                                                              des_rr_node, 
//                                                              rpt_name); 
//        continue;
//      }
//      /* Report timing for the downstream segements, from a SB output to an adjacent CB input */
//      DeviceCoordinator end_sb_coordinator(cur_sb_x, iy - 1);
//      RRGSB end_sb = device_rr_gsb.get_gsb(end_sb_coordinator);
//      verilog_generate_report_timing_one_sb_thru_segments(fp,
//                                                          begin_sb, src_rr_node, 
//                                                          end_sb, src_rr_node,
//                                                          rpt_name); 
//    }
//  }
//
//  /* Free */
//  my_free(rpt_name);
//
//  return;
//}
//
//
///* Print the pins of SBs that a routing wire will go through 
// * from the src_rr_node to the des_rr_node 
// */
//static 
//void dump_verilog_one_sb_wire_segemental_report_timing(FILE* fp,
//                                                       t_syn_verilog_opts fpga_verilog_opts,
//                                                       t_sb* src_sb_info,
//                                                       t_rr_node* drive_rr_node, 
//                                                       t_rr_node* src_rr_node, 
//                                                       t_rr_node* des_rr_node,
//                                                       int path_cnt) {
//  int L_wire;
//  int ix, iy;
//  int cur_sb_x, cur_sb_y;
//  int end_sb_x, end_sb_y;
//  t_cb* next_cb = NULL;
//  t_sb* next_sb = NULL;
//  char* rpt_name = NULL;
//
//  /* Check the file handler */
//  if (NULL == fp) {
//    vpr_printf(TIO_MESSAGE_ERROR,
//               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
//               __FILE__, __LINE__); 
//    exit(1);
//  } 
//
//  /* Check */
//  assert ((INC_DIRECTION == src_rr_node->direction)
//        ||(DEC_DIRECTION == src_rr_node->direction));
//  assert ((CHANX == src_rr_node->type)
//        ||(CHANY == src_rr_node->type));
//
//  L_wire = get_rr_node_wire_length(src_rr_node);
//
//  /* Get report name */
//  rpt_name = gen_verilog_one_routing_report_timing_rpt_name(fpga_verilog_opts.report_timing_path,
//                                                            L_wire, path_cnt);
//
//  /* Start printing report timing info  */
//  fprintf(fp, "# L%d wire, Path ID: %d\n", 
//          L_wire,
//          path_cnt); 
//  /* Report timing for the SB MUX delay, from the drive_rr_node to the wire_rr_node */
//  verilog_generate_one_report_timing_within_sb(fp, src_sb_info, 
//                                               drive_rr_node, 
//                                               src_rr_node); 
//  if (NULL != rpt_name) {
//    fprintf(fp, " > %s\n", rpt_name); 
//  } else {
//    fprintf(fp, "\n"); 
//  }
//
//  /* Switch depends on the type of des_rr_node  */
//  switch(des_rr_node->type) {
//  /* Range of SBs that on the path                        
//   *                             ---------
//   *                            |         |        
//   *                            |  des_sb |
//   *                            | [x][y]  |        
//   *                             ---------
//   *                                /|\ 
//   *                                 |  
//   *                             ---------
//   *                            |         |        
//   *                            | thru_cb |
//   *                            |         |        
//   *                             ---------
//   *                                /|\ 
//   *                                 |  
//   *  --------      -------      ---------      -------      --------
//   * |        |    |       |    |         |    |       |    |        |
//   * | des_sb |<---|thru_cb|<---| src_sb  |--->|thru_cb|--->| des_sb |
//   * |[x-1][y]|    | [x][y]|    |         |    | [x][y]|    |[x][y]  |
//   *  --------      -------      ---------      -------      --------
//   *                                 |
//   *                                \|/
//   *                             ---------
//   *                            |         |        
//   *                            | thru_cb |
//   *                            |         |        
//   *                             ---------
//   *                                 |
//   *                                \|/
//   *                             ---------
//   *                            |         |        
//   *                            |  des_sb |
//   *                            | [x][y-1]|        
//   *                             ---------
//   */
//  case IPIN: 
//    /* Get the coordinate of ending CB */
//    next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
//    assert(next_cb->type == src_rr_node->type);
//    /* 4 cases: */
//    if ((INC_DIRECTION == src_rr_node->direction) 
//       &&(CHANX == src_rr_node->type)) {
//      end_sb_x = next_cb->x; 
//      end_sb_y = next_cb->y;
//    } else if ((INC_DIRECTION == src_rr_node->direction) 
//       &&(CHANY == src_rr_node->type)) {
//      end_sb_x = next_cb->x; 
//      end_sb_y = next_cb->y;
//    } else if ((DEC_DIRECTION == src_rr_node->direction) 
//       &&(CHANX == src_rr_node->type)) {
//      end_sb_x = next_cb->x - 1; 
//      end_sb_y = next_cb->y;
//    } else if ((DEC_DIRECTION == src_rr_node->direction) 
//       &&(CHANY == src_rr_node->type)) {
//      end_sb_x = next_cb->x; 
//      end_sb_y = next_cb->y - 1;
//    }
//    break;
//  /* Range of SBs that on the path                        
//   *                             ---------
//   *                            |         |        
//   *                            |  des_sb |
//   *                            | [x][y+1]|        
//   *                             ---------
//   *                                /|\ 
//   *                                 |  
//   *                             ---------
//   *                            |         |        
//   *                            | thru_sb |
//   *                            |         |        
//   *                             ---------
//   *                                /|\ 
//   *                                 |  
//   *  --------      -------      ---------      -------      --------
//   * |        |    |       |    |         |    |       |    |        |
//   * | des_sb |<---|thru_sb|<---| src_sb  |--->|thru_sb|--->| des_sb |
//   * |[x-1][y]|    | [x][y]|    |         |    | [x][y]|    |[x+1][y]|
//   *  --------      -------      ---------      -------      --------
//   *                                 |
//   *                                \|/
//   *                             ---------
//   *                            |         |        
//   *                            | thru_sb |
//   *                            |         |        
//   *                             ---------
//   *                                 |
//   *                                \|/
//   *                             ---------
//   *                            |         |        
//   *                            |  des_sb |
//   *                            | [x][y-1]|        
//   *                             ---------
//   */
//  case CHANX:
//  case CHANY:
//    /* Get the coordinate of ending CB */
//    next_sb = get_chan_rr_node_ending_sb(src_rr_node, des_rr_node);
//    end_sb_x = next_sb->x; 
//    end_sb_y = next_sb->y;
//    break;
//  default:
//    vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid type of rr_node!\n",
//               __FILE__, __LINE__);
//    exit(1);
//  }
//
//  /* Get the base coordinate of src_sb */
//  cur_sb_x = src_sb_info->x;
//  cur_sb_y = src_sb_info->y;
//  /* 4 cases: */
//  if ((INC_DIRECTION == src_rr_node->direction) 
//     &&(CHANX == src_rr_node->type)) {
//    /* Follow the graph above, go through X channel */
//    for (ix = src_sb_info->x; ix < end_sb_x; ix++) {
//      /* If this is the ending point, we add a ending segment */
//      if (ix == end_sb_x - 1) {
//        verilog_generate_report_timing_one_sb_ending_segments(fp,
//                                                              &(sb_info[ix][cur_sb_y]), src_rr_node, 
//                                                              des_rr_node, 
//                                                              rpt_name); 
//
//        continue;
//      }
//      /* Report timing for the downstream segements, from a SB output to an adjacent CB input */
//      verilog_generate_report_timing_one_sb_thru_segments(fp,
//                                                          &(sb_info[ix][cur_sb_y]), src_rr_node, 
//                                                          &(sb_info[ix + 1][cur_sb_y]), src_rr_node,
//                                                          rpt_name); 
//    }
//  } else if ((INC_DIRECTION == src_rr_node->direction) 
//     &&(CHANY == src_rr_node->type)) {
//    /* Follow the graph above, go through Y channel */
//    for (iy = src_sb_info->y; iy < end_sb_y; iy++) {
//     /* If this is the ending point, we add a ending segment */
//      if (iy == end_sb_y - 1) {
//        verilog_generate_report_timing_one_sb_ending_segments(fp,
//                                                              &(sb_info[cur_sb_x][iy]), src_rr_node, 
//                                                              des_rr_node, 
//                                                              rpt_name); 
//        continue;
//      }
//      /* Report timing for the downstream segements, from a SB output to an adjacent CB input */
//      verilog_generate_report_timing_one_sb_thru_segments(fp,
//                                                          &(sb_info[cur_sb_x][iy]), src_rr_node, 
//                                                          &(sb_info[cur_sb_x][iy + 1]), src_rr_node,
//                                                          rpt_name); 
//    }
//  } else if ((DEC_DIRECTION == src_rr_node->direction) 
//     &&(CHANX == src_rr_node->type)) {
//    /* Follow the graph above, go through X channel */
//    for (ix = src_sb_info->x - 1; ix > end_sb_x; ix--) {
//      /* If this is the ending point, we add a ending segment */
//      if (ix == end_sb_x + 1) {
//        verilog_generate_report_timing_one_sb_ending_segments(fp,
//                                                              &(sb_info[ix][cur_sb_y]), src_rr_node, 
//                                                              des_rr_node, 
//                                                              rpt_name); 
//        continue;
//      }
//      /* Report timing for the downstream segements, from a SB output to an adjacent CB input */
//      verilog_generate_report_timing_one_sb_thru_segments(fp,
//                                                          &(sb_info[ix][cur_sb_y]), src_rr_node, 
//                                                          &(sb_info[ix - 1][cur_sb_y]), src_rr_node,
//                                                          rpt_name); 
//    }
//  } else if ((DEC_DIRECTION == src_rr_node->direction) 
//     &&(CHANY == src_rr_node->type)) {
//    /* Follow the graph above, go through Y channel */
//    for (iy = src_sb_info->y - 1; iy > end_sb_y; iy--) {
//     /* If this is the ending point, we add a ending segment */
//      if (iy == end_sb_y + 1) {
//        verilog_generate_report_timing_one_sb_ending_segments(fp,
//                                                              &(sb_info[cur_sb_x][iy]), src_rr_node, 
//                                                              des_rr_node, 
//                                                              rpt_name); 
//        continue;
//      }
//      /* Report timing for the downstream segements, from a SB output to an adjacent CB input */
//      verilog_generate_report_timing_one_sb_thru_segments(fp,
//                                                          &(sb_info[cur_sb_x][iy]), src_rr_node, 
//                                                          &(sb_info[cur_sb_x][iy - 1]), src_rr_node,
//                                                          rpt_name); 
//    }
//  }
//
//  /* Free */
//  my_free(rpt_name);
//
//  return;
//}

/* Print the pins of SBs that a routing wire will go through 
 * from the src_rr_node to the des_rr_node 
 */
static 
void dump_verilog_sb_through_routing_pins(FILE* fp,
                                          const RRGSB& src_rr_sb,
                                          t_rr_node* src_rr_node, 
                                          t_rr_node* des_rr_node) {
  size_t cur_sb_x, cur_sb_y;
  size_t end_sb_x, end_sb_y;
  t_cb* next_cb;
  DeviceCoordinator next_sb_coordinator;
  RRGSB next_sb;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ((INC_DIRECTION == src_rr_node->direction)
        ||(DEC_DIRECTION == src_rr_node->direction));
  assert ((CHANX == src_rr_node->type)
        ||(CHANY == src_rr_node->type));

  /* Switch depends on the type of des_rr_node  */
  switch(des_rr_node->type) {
  /* Range of SBs that on the path                        
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y]  |        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *                             ---------
   *                            |         |        
   *                            | thru_cb |
   *                            |         |        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *  --------      -------      ---------      -------      --------
   * |        |    |       |    |         |    |       |    |        |
   * | des_sb |<---|thru_cb|<---| src_sb  |--->|thru_cb|--->| des_sb |
   * |[x-1][y]|    | [x][y]|    |         |    | [x][y]|    |[x][y]  |
   *  --------      -------      ---------      -------      --------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            | thru_cb |
   *                            |         |        
   *                             ---------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y-1]|        
   *                             ---------
   */
  case IPIN: 
    /* Get the coordinate of ending CB */
    next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
    assert(next_cb->type == src_rr_node->type);
    /* 4 cases: */
    if ((INC_DIRECTION == src_rr_node->direction) 
       &&(CHANX == src_rr_node->type)) {
      end_sb_x = next_cb->x; 
      end_sb_y = next_cb->y;
    } else if ((INC_DIRECTION == src_rr_node->direction) 
       &&(CHANY == src_rr_node->type)) {
      end_sb_x = next_cb->x; 
      end_sb_y = next_cb->y;
    } else if ((DEC_DIRECTION == src_rr_node->direction) 
       &&(CHANX == src_rr_node->type)) {
      end_sb_x = next_cb->x - 1; 
      end_sb_y = next_cb->y;
    } else if ((DEC_DIRECTION == src_rr_node->direction) 
       &&(CHANY == src_rr_node->type)) {
      end_sb_x = next_cb->x; 
      end_sb_y = next_cb->y - 1;
    }
    break;
  /* Range of SBs that on the path                        
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y+1]|        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *                             ---------
   *                            |         |        
   *                            | thru_sb |
   *                            |         |        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *  --------      -------      ---------      -------      --------
   * |        |    |       |    |         |    |       |    |        |
   * | des_sb |<---|thru_sb|<---| src_sb  |--->|thru_sb|--->| des_sb |
   * |[x-1][y]|    | [x][y]|    |         |    | [x][y]|    |[x+1][y]|
   *  --------      -------      ---------      -------      --------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            | thru_sb |
   *                            |         |        
   *                             ---------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y-1]|        
   *                             ---------
   */
  case CHANX:
  case CHANY:
    /* Get the coordinate of ending CB */
    next_sb_coordinator = get_chan_node_ending_sb_coordinator(src_rr_node, des_rr_node);
    next_sb = device_rr_gsb.get_gsb(next_sb_coordinator);
    end_sb_x = next_sb.get_sb_x(); 
    end_sb_y = next_sb.get_sb_y();
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid type of rr_node!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Get the base coordinate of src_sb */
  cur_sb_x = src_rr_sb.get_sb_x();
  cur_sb_y = src_rr_sb.get_sb_y();
  /* 4 cases: */
  if ((INC_DIRECTION == src_rr_node->direction) 
     &&(CHANX == src_rr_node->type)) {
    /* Follow the graph above, go through X channel */
    for (size_t ix = src_rr_sb.get_sb_x() + 1; ix < end_sb_x; ++ix) {
      /* Print an IN_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      DeviceCoordinator inter_sb_coordinator(ix, cur_sb_y);
      RRGSB inter_sb = device_rr_gsb.get_gsb(inter_sb_coordinator);
      fprintf(fp, "%s/", 
              inter_sb.gen_sb_verilog_instance_name());
      dump_verilog_one_sb_chan_pin(fp, inter_sb, src_rr_node, IN_PORT); 
      /* Print an OUT_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      fprintf(fp, "%s/", 
              inter_sb.gen_sb_verilog_instance_name()); 
      dump_verilog_one_sb_chan_pin(fp, inter_sb, src_rr_node, OUT_PORT); 
    }
  } else if ((INC_DIRECTION == src_rr_node->direction) 
     &&(CHANY == src_rr_node->type)) {
    /* Follow the graph above, go through Y channel */
    for (size_t iy = src_rr_sb.get_sb_y() + 1; iy < end_sb_y; ++iy) {
      /* Print an IN_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      DeviceCoordinator inter_sb_coordinator(cur_sb_x, iy);
      RRGSB inter_sb = device_rr_gsb.get_gsb(inter_sb_coordinator);
      fprintf(fp, "%s/", 
              inter_sb.gen_sb_verilog_instance_name());
      dump_verilog_one_sb_chan_pin(fp, inter_sb, src_rr_node, IN_PORT); 
      /* Print an OUT_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      fprintf(fp, "%s/", 
              inter_sb.gen_sb_verilog_instance_name()); 
      dump_verilog_one_sb_chan_pin(fp, inter_sb, src_rr_node, OUT_PORT); 
    }
  } else if ((DEC_DIRECTION == src_rr_node->direction) 
     &&(CHANX == src_rr_node->type)) {
    /* Follow the graph above, go through X channel */
    for (size_t ix = src_rr_sb.get_sb_x() - 1; ix > end_sb_x; --ix) {
      /* Print an IN_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      DeviceCoordinator inter_sb_coordinator(ix, cur_sb_y);
      RRGSB inter_sb = device_rr_gsb.get_gsb(inter_sb_coordinator);
      fprintf(fp, "%s/", 
              inter_sb.gen_sb_verilog_instance_name());
      dump_verilog_one_sb_chan_pin(fp, inter_sb, src_rr_node, IN_PORT); 
      /* Print an OUT_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      fprintf(fp, "%s/", 
              inter_sb.gen_sb_verilog_instance_name());
      dump_verilog_one_sb_chan_pin(fp, inter_sb, src_rr_node, OUT_PORT); 
    }
  } else if ((DEC_DIRECTION == src_rr_node->direction) 
     &&(CHANY == src_rr_node->type)) {
    /* Follow the graph above, go through Y channel */
    for (size_t iy = src_rr_sb.get_sb_y() - 1; iy > end_sb_y; --iy) {
      /* Print an IN_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      DeviceCoordinator inter_sb_coordinator(cur_sb_x, iy);
      RRGSB inter_sb = device_rr_gsb.get_gsb(inter_sb_coordinator);
      fprintf(fp, "%s/", 
              inter_sb.gen_sb_verilog_instance_name());
      dump_verilog_one_sb_chan_pin(fp, inter_sb, src_rr_node, IN_PORT); 
      /* Print an OUT_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      fprintf(fp, "%s/", 
              inter_sb.gen_sb_verilog_instance_name());
      dump_verilog_one_sb_chan_pin(fp, inter_sb, src_rr_node, OUT_PORT); 
    }
  }

  return;
}



/* Print the pins of SBs that a routing wire will go through 
 * from the src_rr_node to the des_rr_node 
 */
static 
void dump_verilog_sb_through_routing_pins(FILE* fp,
                                          t_sb* src_sb_info,
                                          t_rr_node* src_rr_node, 
                                          t_rr_node* des_rr_node) {
  int ix, iy;
  int cur_sb_x, cur_sb_y;
  int end_sb_x, end_sb_y;
  t_cb* next_cb;
  t_sb* next_sb;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  /* Check */
  assert ((INC_DIRECTION == src_rr_node->direction)
        ||(DEC_DIRECTION == src_rr_node->direction));
  assert ((CHANX == src_rr_node->type)
        ||(CHANY == src_rr_node->type));

  /* Switch depends on the type of des_rr_node  */
  switch(des_rr_node->type) {
  /* Range of SBs that on the path                        
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y]  |        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *                             ---------
   *                            |         |        
   *                            | thru_cb |
   *                            |         |        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *  --------      -------      ---------      -------      --------
   * |        |    |       |    |         |    |       |    |        |
   * | des_sb |<---|thru_cb|<---| src_sb  |--->|thru_cb|--->| des_sb |
   * |[x-1][y]|    | [x][y]|    |         |    | [x][y]|    |[x][y]  |
   *  --------      -------      ---------      -------      --------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            | thru_cb |
   *                            |         |        
   *                             ---------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y-1]|        
   *                             ---------
   */
  case IPIN: 
    /* Get the coordinate of ending CB */
    next_cb = get_chan_rr_node_ending_cb(src_rr_node, des_rr_node);
    assert(next_cb->type == src_rr_node->type);
    /* 4 cases: */
    if ((INC_DIRECTION == src_rr_node->direction) 
       &&(CHANX == src_rr_node->type)) {
      end_sb_x = next_cb->x; 
      end_sb_y = next_cb->y;
    } else if ((INC_DIRECTION == src_rr_node->direction) 
       &&(CHANY == src_rr_node->type)) {
      end_sb_x = next_cb->x; 
      end_sb_y = next_cb->y;
    } else if ((DEC_DIRECTION == src_rr_node->direction) 
       &&(CHANX == src_rr_node->type)) {
      end_sb_x = next_cb->x - 1; 
      end_sb_y = next_cb->y;
    } else if ((DEC_DIRECTION == src_rr_node->direction) 
       &&(CHANY == src_rr_node->type)) {
      end_sb_x = next_cb->x; 
      end_sb_y = next_cb->y - 1;
    }
    break;
  /* Range of SBs that on the path                        
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y+1]|        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *                             ---------
   *                            |         |        
   *                            | thru_sb |
   *                            |         |        
   *                             ---------
   *                                /|\ 
   *                                 |  
   *  --------      -------      ---------      -------      --------
   * |        |    |       |    |         |    |       |    |        |
   * | des_sb |<---|thru_sb|<---| src_sb  |--->|thru_sb|--->| des_sb |
   * |[x-1][y]|    | [x][y]|    |         |    | [x][y]|    |[x+1][y]|
   *  --------      -------      ---------      -------      --------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            | thru_sb |
   *                            |         |        
   *                             ---------
   *                                 |
   *                                \|/
   *                             ---------
   *                            |         |        
   *                            |  des_sb |
   *                            | [x][y-1]|        
   *                             ---------
   */
  case CHANX:
  case CHANY:
    /* Get the coordinate of ending CB */
    next_sb = get_chan_rr_node_ending_sb(src_rr_node, des_rr_node);
    end_sb_x = next_sb->x; 
    end_sb_y = next_sb->y;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid type of rr_node!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Get the base coordinate of src_sb */
  cur_sb_x = src_sb_info->x;
  cur_sb_y = src_sb_info->y;
  /* 4 cases: */
  if ((INC_DIRECTION == src_rr_node->direction) 
     &&(CHANX == src_rr_node->type)) {
    /* Follow the graph above, go through X channel */
    for (ix = src_sb_info->x + 1; ix < end_sb_x; ix++) {
      /* Print an IN_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      fprintf(fp, "%s/", 
              gen_verilog_one_sb_instance_name(&sb_info[ix][cur_sb_y])); 
      dump_verilog_one_sb_chan_pin(fp, &(sb_info[ix][cur_sb_y]), src_rr_node, IN_PORT); 
      /* Print an OUT_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      fprintf(fp, "%s/", 
              gen_verilog_one_sb_instance_name(&sb_info[ix][cur_sb_y])); 
      dump_verilog_one_sb_chan_pin(fp, &(sb_info[ix][cur_sb_y]), src_rr_node, OUT_PORT); 
    }
  } else if ((INC_DIRECTION == src_rr_node->direction) 
     &&(CHANY == src_rr_node->type)) {
    /* Follow the graph above, go through Y channel */
    for (iy = src_sb_info->y + 1; iy < end_sb_y; iy++) {
      /* Print an IN_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      fprintf(fp, "%s/", 
              gen_verilog_one_sb_instance_name(&sb_info[cur_sb_x][iy])); 
      dump_verilog_one_sb_chan_pin(fp, &(sb_info[cur_sb_x][iy]), src_rr_node, IN_PORT); 
      /* Print an OUT_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      fprintf(fp, "%s/", 
              gen_verilog_one_sb_instance_name(&sb_info[cur_sb_x][iy])); 
      dump_verilog_one_sb_chan_pin(fp, &(sb_info[cur_sb_x][iy]), src_rr_node, OUT_PORT); 
    }
  } else if ((DEC_DIRECTION == src_rr_node->direction) 
     &&(CHANX == src_rr_node->type)) {
    /* Follow the graph above, go through X channel */
    for (ix = src_sb_info->x - 1; ix > end_sb_x; ix--) {
      /* Print an IN_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      fprintf(fp, "%s/", 
              gen_verilog_one_sb_instance_name(&sb_info[ix][cur_sb_y])); 
      dump_verilog_one_sb_chan_pin(fp, &(sb_info[ix][cur_sb_y]), src_rr_node, IN_PORT); 
      /* Print an OUT_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      fprintf(fp, "%s/", 
              gen_verilog_one_sb_instance_name(&sb_info[ix][cur_sb_y])); 
      dump_verilog_one_sb_chan_pin(fp, &(sb_info[ix][cur_sb_y]), src_rr_node, OUT_PORT); 
    }
  } else if ((DEC_DIRECTION == src_rr_node->direction) 
     &&(CHANY == src_rr_node->type)) {
    /* Follow the graph above, go through Y channel */
    for (iy = src_sb_info->y - 1; iy > end_sb_y; iy--) {
      /* Print an IN_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      fprintf(fp, "%s/", 
              gen_verilog_one_sb_instance_name(&sb_info[cur_sb_x][iy])); 
      dump_verilog_one_sb_chan_pin(fp, &(sb_info[cur_sb_x][iy]), src_rr_node, IN_PORT); 
      /* Print an OUT_PORT*/ 
      fprintf(fp, " ");
      /* output instance name */
      fprintf(fp, "%s/", 
              gen_verilog_one_sb_instance_name(&sb_info[cur_sb_x][iy])); 
      dump_verilog_one_sb_chan_pin(fp, &(sb_info[cur_sb_x][iy]), src_rr_node, OUT_PORT); 
    }
  }

  return;
}

/* Report timing for a routing wire,
 * Support uni-directional routing architecture
 * Each routing wire start from an OPIN 
 * We check each fan-out to find all possible ending point:
 * An ending point is supposed to be an OPIN or CHANX or CHANY
 */
static 
void verilog_generate_one_routing_wire_report_timing(FILE* fp, 
                                                     t_trpt_opts sdc_opts,
                                                     int L_wire,
                                                     const RRGSB& rr_sb,
                                                     t_rr_node* wire_rr_node,
                                                     t_rr_node* LL_rr_node) {
  int path_cnt = 0;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  assert(  ( CHANX == wire_rr_node->type )
        || ( CHANY == wire_rr_node->type ));
  int track_idx = wire_rr_node->ptc_num;
  t_rr_type cb_type = wire_rr_node->type;

  /* We only care a specific length of wires */
  if (L_wire != (abs(wire_rr_node->xlow - wire_rr_node->xhigh + wire_rr_node->ylow - wire_rr_node->yhigh) + 1)) {
    return; 
  }

  /* Find the starting points */
  for (int iedge = 0; iedge < wire_rr_node->num_drive_rr_nodes; iedge++) {
    boolean sb_dumped = FALSE;
    /* Find the ending points*/
    for (int jedge = 0; jedge < wire_rr_node->num_edges; jedge++) {
      int x_end, y_end;
      /* Find where the destination pin belongs to */
      get_chan_rr_node_end_coordinate(wire_rr_node, &x_end, &y_end);
      /* Reciever could be IPIN or CHANX or CHANY */
      int inode = wire_rr_node->edges[jedge];
      /* Find the SB/CB block that it belongs to */
      switch (LL_rr_node[inode].type) {
      case IPIN:
        {
        DeviceCoordinator next_cb_coordinator = get_chan_node_ending_cb(wire_rr_node, &(LL_rr_node[inode]));
        /* Get the coordinate of ending CB */
        const RRGSB& next_cb = device_rr_gsb.get_gsb(next_cb_coordinator);
        /* This will not be the longest path unless the cb is close to the ending SB */
        if ((TRUE == sdc_opts.longest_path_only)
           && ( ((int)next_cb.get_cb_x(cb_type) != x_end) || ((int)next_cb.get_cb_y(cb_type) != y_end))) {
          continue;
        }
        /* Driver could be OPIN or CHANX or CHANY,
          * and it must be in the cur_sb_info
          */
        /* Start printing report timing info  */
        fprintf(fp, "# L%d wire, Path %d\n", 
                abs(wire_rr_node->xlow - wire_rr_node->xhigh + wire_rr_node->ylow - wire_rr_node->yhigh) + 1,
                path_cnt); 
        fprintf(fp, "report_timing -from "); 
        /* output instance name */
        fprintf(fp, "%s/", rr_sb.gen_sb_verilog_instance_name()); 
        /* output pin name */
        dump_verilog_one_sb_routing_pin(fp, rr_sb, 
                                        wire_rr_node->drive_rr_nodes[iedge]);
        fprintf(fp, " -to "); 
        /* output instance name */
        fprintf(fp, "%s/",
                next_cb.gen_cb_verilog_instance_name(cb_type));
        /* output pin name */
        fprintf(fp, "%s",
                next_cb.gen_cb_verilog_routing_track_name(cb_type, track_idx));
        /* Print through pins */
        if (TRUE == sdc_opts.print_thru_pins) { 
          fprintf(fp, " -through_pins "); 
          dump_verilog_sb_through_routing_pins(fp, rr_sb, wire_rr_node, &(LL_rr_node[inode]));
        } else {
          fprintf(fp, " -point_to_point\n"); 
        }
        fprintf(fp, " -unconstrained\n"); 
        path_cnt++;
        }
        break;
      case CHANX:
      case CHANY:
        {
        DeviceCoordinator next_sb_coordinator;
        /* Get the coordinate of ending SB */
        next_sb_coordinator = get_chan_node_ending_sb_coordinator(wire_rr_node, &(LL_rr_node[inode]));
        const RRGSB& next_sb = device_rr_gsb.get_gsb(next_sb_coordinator);
        /* This will not be the longest path unless the cb is close to the ending SB */
        if ((TRUE == sdc_opts.longest_path_only)
           && ((next_sb.get_sb_x() != (size_t)x_end) || (next_sb.get_sb_y() != (size_t)y_end))) {
          continue;
        }
        if (TRUE == sb_dumped) {
          continue;
        }
        /* Driver could be OPIN or CHANX or CHANY,
          * and it must be in the cur_sb_info
          */
        /* Start printing report timing info  */
        fprintf(fp, "# L%d wire, Path %d\n", 
                abs(wire_rr_node->xlow - wire_rr_node->xhigh + wire_rr_node->ylow - wire_rr_node->yhigh) + 1,
                path_cnt); 
        fprintf(fp, "report_timing -from "); 
        /* output instance name */
        fprintf(fp, "%s/", 
                rr_sb.gen_sb_verilog_instance_name()); 
        /* output pin name */
        dump_verilog_one_sb_routing_pin(fp, rr_sb, 
                                        wire_rr_node->drive_rr_nodes[iedge]);
        fprintf(fp, " -to "); 
        /* output instance name */
        fprintf(fp, "%s/",
                next_sb.gen_sb_verilog_instance_name());
        /* Find which side the ending pin locates, and determine the coordinate */
        dump_verilog_one_sb_routing_pin(fp, next_sb, 
                                        wire_rr_node);
        /* Print through pins */
        if (TRUE == sdc_opts.print_thru_pins) { 
          fprintf(fp, " -through_pins "); 
          dump_verilog_sb_through_routing_pins(fp, rr_sb, 
                                               wire_rr_node, &(LL_rr_node[inode]));
        } else {
          fprintf(fp, " -point_to_point\n"); 
        }
        fprintf(fp, " -unconstrained\n"); 
        path_cnt++;
        /* Set the flag */
        sb_dumped = TRUE;
        }
        break;
      default:
       vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid type of ending point rr_node!\n",
                   __FILE__, __LINE__);
 
        exit(1);
      }
      /* Get the user-constrained delay of this routing wire */
      /* Find the pins/ports of SBs that this wire may across */
      /* Output the Report Timing commands */
    }
  }

  return;
}


/* Report timing for a routing wire,
 * Support uni-directional routing architecture
 * Each routing wire start from an OPIN 
 * We check each fan-out to find all possible ending point:
 * An ending point is supposed to be an OPIN or CHANX or CHANY
 */
static 
void verilog_generate_one_routing_wire_report_timing(FILE* fp, 
                                                     t_trpt_opts sdc_opts,
                                                     int L_wire,
                                                     t_sb* cur_sb_info,
                                                     t_rr_node* wire_rr_node,
                                                     t_rr_node* LL_rr_node) {
  int iedge, jedge, inode;
  int track_idx;
  int path_cnt = 0;
  t_sb* next_sb = NULL; 
  t_cb* next_cb = NULL; 
  int x_end, y_end;
  boolean sb_dumped = FALSE;

  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  assert(  ( CHANX == wire_rr_node->type )
        || ( CHANY == wire_rr_node->type ));
  track_idx = wire_rr_node->ptc_num;

  /* We only care a specific length of wires */
  if (L_wire != (abs(wire_rr_node->xlow - wire_rr_node->xhigh + wire_rr_node->ylow - wire_rr_node->yhigh) + 1)) {
    return; 
  }

  /* Find the starting points */
  for (iedge = 0; iedge < wire_rr_node->num_drive_rr_nodes; iedge++) {
    sb_dumped = FALSE;
    /* Find the ending points*/
    for (jedge = 0; jedge < wire_rr_node->num_edges; jedge++) {
      /* Find where the destination pin belongs to */
      get_chan_rr_node_end_coordinate(wire_rr_node, &x_end, &y_end);
      /* Reciever could be IPIN or CHANX or CHANY */
      inode = wire_rr_node->edges[jedge];
      /* Find the SB/CB block that it belongs to */
      switch (LL_rr_node[inode].type) {
      case IPIN:
        /* Get the coordinate of ending CB */
        next_cb = get_chan_rr_node_ending_cb(wire_rr_node, &(LL_rr_node[inode]));
        /* This will not be the longest path unless the cb is close to the ending SB */
        if ((TRUE == sdc_opts.longest_path_only)
           && ((next_cb->x != x_end) || (next_cb->y != y_end))) {
          continue;
        }
        /* Driver could be OPIN or CHANX or CHANY,
          * and it must be in the cur_sb_info
          */
        /* Start printing report timing info  */
        fprintf(fp, "# L%d wire, Path %d\n", 
                abs(wire_rr_node->xlow - wire_rr_node->xhigh + wire_rr_node->ylow - wire_rr_node->yhigh) + 1,
                path_cnt); 
        fprintf(fp, "report_timing -from "); 
        /* output instance name */
        fprintf(fp, "%s/", 
                    gen_verilog_one_sb_instance_name(cur_sb_info)); 
        /* output pin name */
        dump_verilog_one_sb_routing_pin(fp, cur_sb_info, 
                                        wire_rr_node->drive_rr_nodes[iedge]);
        fprintf(fp, " -to "); 
        /* output instance name */
        fprintf(fp, "%s/",
                gen_verilog_one_cb_instance_name(next_cb));
        /* output pin name */
        fprintf(fp, "%s",
                gen_verilog_routing_channel_one_midout_name( next_cb,
                                                             track_idx));
        /* Print through pins */
        if (TRUE == sdc_opts.print_thru_pins) { 
          fprintf(fp, " -through_pins "); 
          dump_verilog_sb_through_routing_pins(fp, cur_sb_info, wire_rr_node, &(LL_rr_node[inode]));
        } else {
          fprintf(fp, " -point_to_point\n"); 
        }
        fprintf(fp, " -unconstrained\n"); 
        path_cnt++;
        break;
      case CHANX:
      case CHANY:
        /* Get the coordinate of ending SB */
        next_sb = get_chan_rr_node_ending_sb(wire_rr_node, &(LL_rr_node[inode]));
        /* This will not be the longest path unless the cb is close to the ending SB */
        if ((TRUE == sdc_opts.longest_path_only)
           && ((next_sb->x != x_end) || (next_sb->y != y_end))) {
          continue;
        }
        if (TRUE == sb_dumped) {
          continue;
        }
        /* Driver could be OPIN or CHANX or CHANY,
          * and it must be in the cur_sb_info
          */
        /* Start printing report timing info  */
        fprintf(fp, "# L%d wire, Path %d\n", 
                abs(wire_rr_node->xlow - wire_rr_node->xhigh + wire_rr_node->ylow - wire_rr_node->yhigh) + 1,
                path_cnt); 
        fprintf(fp, "report_timing -from "); 
        /* output instance name */
        fprintf(fp, "%s/", 
                    gen_verilog_one_sb_instance_name(cur_sb_info)); 
        /* output pin name */
        dump_verilog_one_sb_routing_pin(fp, cur_sb_info, 
                                        wire_rr_node->drive_rr_nodes[iedge]);
        fprintf(fp, " -to "); 
        /* output instance name */
        fprintf(fp, "%s/",
                gen_verilog_one_sb_instance_name(next_sb));
        /* Find which side the ending pin locates, and determine the coordinate */
        dump_verilog_one_sb_routing_pin(fp, next_sb, 
                                        wire_rr_node);
        /* Print through pins */
        if (TRUE == sdc_opts.print_thru_pins) { 
          fprintf(fp, " -through_pins "); 
          dump_verilog_sb_through_routing_pins(fp, cur_sb_info, 
                                               wire_rr_node, &(LL_rr_node[inode]));
        } else {
          fprintf(fp, " -point_to_point\n"); 
        }
        fprintf(fp, " -unconstrained\n"); 
        path_cnt++;
        /* Set the flag */
        sb_dumped = TRUE;
        break;
      default:
       vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid type of ending point rr_node!\n",
                   __FILE__, __LINE__);
 
        exit(1);
      }
      /* Get the user-constrained delay of this routing wire */
      /* Find the pins/ports of SBs that this wire may across */
      /* Output the Report Timing commands */
    }
  }

  return;
}

/* Generate report timing for each routing wires/segments */
static 
void verilog_generate_routing_wires_report_timing(FILE* fp, 
                                                  t_trpt_opts sdc_opts,
                                                  int L_wire, 
                                                  t_rr_node* LL_rr_node) {
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  if (TRUE == sdc_opts.compact_routing_hierarchy) {
    DeviceCoordinator sb_range = device_rr_gsb.get_gsb_range();
    for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
      for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
        const RRGSB& rr_sb = device_rr_gsb.get_gsb(ix, iy);
        for (size_t side = 0; side < rr_sb.get_num_sides(); side++) {
          Side side_manager(side);
          for (size_t itrack = 0; itrack < rr_sb.get_chan_width(side_manager.get_side()); ++itrack) {
            assert((CHANX == rr_sb.get_chan_node(side_manager.get_side(), itrack)->type)
                 ||(CHANY == rr_sb.get_chan_node(side_manager.get_side(), itrack)->type));
            /* We only care the output port and it should indicate a SB mux */
            if ( (OUT_PORT != rr_sb.get_chan_node_direction(side_manager.get_side(), itrack)) 
               || (true == rr_sb.is_sb_node_imply_short_connection(rr_sb.get_chan_node(side_manager.get_side(), itrack)))) {
              continue; 
            }
            /* Bypass if we have only 1 driving node */
            if (1 == rr_sb.get_chan_node(side_manager.get_side(), itrack)->num_drive_rr_nodes) {
              continue; 
            }
            /* Reach here, it means a valid starting point of a routing wire */
            verilog_generate_one_routing_wire_report_timing(fp, sdc_opts, L_wire, rr_sb,
                                                            rr_sb.get_chan_node(side_manager.get_side(), itrack),
                                                            LL_rr_node);

          }
        }
      }
    }
  } else {
    t_sb* cur_sb_info = NULL;
    /* We start from a SB[x][y] */
    for (int ix = 0; ix < (nx + 1); ix++) {
      for (int iy = 0; iy < (ny + 1); iy++) {
        cur_sb_info = &(sb_info[ix][iy]);
        for (int side = 0; side < cur_sb_info->num_sides; side++) {
          for (int itrack = 0; itrack < cur_sb_info->chan_width[side]; itrack++) {
            assert((CHANX == cur_sb_info->chan_rr_node[side][itrack]->type)
                 ||(CHANY == cur_sb_info->chan_rr_node[side][itrack]->type));
            /* We only care the output port and it should indicate a SB mux */
            if ( (OUT_PORT != cur_sb_info->chan_rr_node_direction[side][itrack]) 
               || (FALSE != check_drive_rr_node_imply_short(*cur_sb_info, cur_sb_info->chan_rr_node[side][itrack], side))) {
              continue; 
            }
            /* Bypass if we have only 1 driving node */
            if (1 == cur_sb_info->chan_rr_node[side][itrack]->num_drive_rr_nodes) {
              continue; 
            }
            /* Reach here, it means a valid starting point of a routing wire */
            verilog_generate_one_routing_wire_report_timing(fp, sdc_opts, L_wire, &(sb_info[ix][iy]),
                                                            cur_sb_info->chan_rr_node[side][itrack],
                                                            LL_rr_node);
          }
        } 
      }
    }
  }

  return;
}

static 
void verilog_generate_sb_report_timing(t_trpt_opts sdc_opts,
                                       t_arch arch,
                                       t_rr_node* LL_rr_node) {
  char* sdc_fname = NULL;
  FILE* fp = NULL;
  int iseg;
  int L_max = OPEN;

  /* Create the file handler */
  sdc_fname = my_strcat(format_dir_path(sdc_opts.sdc_dir), trpt_sb_file_name);

  /* Create a file*/
  fp = fopen(sdc_fname, "w");

  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Failure in creating SDC constraints %s",
               __FILE__, __LINE__, sdc_fname); 
    exit(1);
  } 

  /* Generate SDC header */
  dump_verilog_sdc_file_header(fp, "Report Timing for Switch blocks");

  vpr_printf(TIO_MESSAGE_INFO,
             "Generating TCL script to report timing for Switch Blocks: %s\n",
             sdc_fname);
  /* Find the longest wires: we only care defined length of wires? */
  for (iseg = 0; iseg < arch.num_segments; iseg++) {
    /* Bypass zero frequency sgements */
    if (0 == arch.Segments[iseg].frequency) {
      continue;
    }
    if ((OPEN == L_max) || (L_max < arch.Segments[iseg].length)) {
      L_max = arch.Segments[iseg].length;
    }
  }
  /* In some case, FPGA array size is smaller than any segments.
   * Therefore, to be strict non-segment timing will be reported
   * We added the FPGA array size for report timing 
   */
  if ((L_max > nx) && (L_max > ny)) { 
    if (nx != ny) {
      verilog_generate_routing_wires_report_timing(fp, sdc_opts,
                                                   nx, LL_rr_node);
      verilog_generate_routing_wires_report_timing(fp, sdc_opts,
                                                   ny, LL_rr_node);
    } else {
      verilog_generate_routing_wires_report_timing(fp, sdc_opts,
                                                   nx, LL_rr_node);
    }
  } else {
    /* We only care defined length of wires? */
    for (iseg = 0; iseg < arch.num_segments; iseg++) {
      /* Bypass zero frequency sgements */
      if (0 == arch.Segments[iseg].frequency) {
        continue;
      }
      verilog_generate_routing_wires_report_timing(fp, sdc_opts, arch.Segments[iseg].length, 
                                                   LL_rr_node);
    }
  }

  /* close file*/
  fclose(fp);

  return;
}

/* Report timing for a routing wire divided in segements,
 * Support uni-directional routing architecture
 * Each routing wire start from an OPIN 
 * We check each fan-out to find all possible ending point:
 * An ending point is supposed to be an OPIN or CHANX or CHANY
 * We consider the farest ending point and report timing for each segements on the path
 * We output TCL commands to sum up the segmental delay 
 */
static 
void verilog_generate_one_routing_segmental_report_timing(FILE* fp, 
                                                          t_syn_verilog_opts fpga_verilog_opts,
                                                          const RRGSB& rr_sb,
                                                          t_rr_node* wire_rr_node,
                                                          t_rr_node* LL_rr_node,
                                                          char* direction,
                                                          int* path_cnt) {
  int num_end_rr_nodes = 0;
  t_rr_node** end_rr_node = NULL;
  
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  assert(  ( CHANX == wire_rr_node->type )
        || ( CHANY == wire_rr_node->type ));
 
  /* Find the farest ending points!*/
  build_ending_rr_node_for_one_sb_wire(wire_rr_node, LL_rr_node, 
                                       &num_end_rr_nodes, &end_rr_node);

  /* Find the starting points */
  for (int iedge = 0; iedge < wire_rr_node->num_drive_rr_nodes; iedge++) {
    /* Find the ending points*/
    for (int jedge = 0; jedge < num_end_rr_nodes; jedge++) {
      /* Report timing */
      dump_verilog_one_sb_wire_segemental_report_timing(fp, fpga_verilog_opts, 
                                                        rr_sb, 
                                                        wire_rr_node->drive_rr_nodes[iedge],
                                                        wire_rr_node,
                                                        end_rr_node[jedge],
                                                        direction,
                                                        *path_cnt);
      /* Update counter */
      (*path_cnt)++;
    }
  }

  /* Free */
  my_free(end_rr_node);

  return;
}


/* Report timing for a routing wire divided in segements,
 * Support uni-directional routing architecture
 * Each routing wire start from an OPIN 
 * We check each fan-out to find all possible ending point:
 * An ending point is supposed to be an OPIN or CHANX or CHANY
 * We consider the farest ending point and report timing for each segements on the path
 * We output TCL commands to sum up the segmental delay 
 */
static 
void verilog_generate_one_routing_segmental_report_timing(FILE* fp, 
                                                          t_syn_verilog_opts fpga_verilog_opts,
                                                          t_sb* cur_sb_info,
                                                          t_rr_node* wire_rr_node,
                                                          t_rr_node* LL_rr_node,
                                                          char* direction,
                                                          int* path_cnt) {
  int iedge, jedge;
  int num_end_rr_nodes = 0;
  t_rr_node** end_rr_node = NULL;
  
  /* Check the file handler */
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d])Invalid file handler for SDC generation",
               __FILE__, __LINE__); 
    exit(1);
  } 

  assert(  ( CHANX == wire_rr_node->type )
        || ( CHANY == wire_rr_node->type ));
 
  /* Find the farest ending points!*/
  build_ending_rr_node_for_one_sb_wire(wire_rr_node, LL_rr_node, 
                                       &num_end_rr_nodes, &end_rr_node);

  /* Find the starting points */
  for (iedge = 0; iedge < wire_rr_node->num_drive_rr_nodes; iedge++) {
    /* Find the ending points*/
    for (jedge = 0; jedge < num_end_rr_nodes; jedge++) {
      /* Report timing */
      dump_verilog_one_sb_wire_segemental_report_timing(fp, fpga_verilog_opts, 
                                                        cur_sb_info, 
                                                        wire_rr_node->drive_rr_nodes[iedge],
                                                        wire_rr_node,
                                                        end_rr_node[jedge],
                                                        direction,
                                                        *path_cnt);
      /* Update counter */
      (*path_cnt)++;
    }
  }

  /* Free */
  my_free(end_rr_node);

  return;
}

static 
void verilog_generate_routing_report_timing(t_trpt_opts trpt_opts,
                                            t_rr_node* LL_rr_node,
                                            t_syn_verilog_opts fpga_verilog_opts) {
  FILE* fp = NULL;
  int ix, iy;
  int L_wire;
  int side, itrack;
  t_sb* cur_sb_info = NULL;
  t_llist* rr_path_cnt = NULL;
  t_wireL_cnt* wireL_cnt = NULL;
  int path_cnt = 0; 

  vpr_printf(TIO_MESSAGE_INFO,
             "Generating TCL script to report timing for routing wires\n");

  vpr_printf(TIO_MESSAGE_INFO,
             "Generating TCL script to report timing for routing wires horizontal\n");
  /* Start with horizontal SBs*/
  /* We start from a SB[x][y] */
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 0; iy < (ny + 1); iy++) {
      cur_sb_info = &(sb_info[ix][iy]);
      for (side = 0; side < cur_sb_info->num_sides; side++) {
        for (itrack = 0; itrack < cur_sb_info->chan_width[side]; itrack++) {
          assert((CHANX == cur_sb_info->chan_rr_node[side][itrack]->type)
               ||(CHANY == cur_sb_info->chan_rr_node[side][itrack]->type));
          /* We only care the output port and it should indicate a SB mux */
          if ( (OUT_PORT != cur_sb_info->chan_rr_node_direction[side][itrack]) 
             || (FALSE != check_drive_rr_node_imply_short(*cur_sb_info, cur_sb_info->chan_rr_node[side][itrack], side))) {
            continue; 
          }
          /* Bypass if we have only 1 driving node */
          if (1 == cur_sb_info->chan_rr_node[side][itrack]->num_drive_rr_nodes) {
            continue; 
          }
          if (CHANY == cur_sb_info->chan_rr_node[side][itrack]->type) {
            continue;
          }
          /* Check if L_wire exists in the linked list */
          L_wire = get_rr_node_wire_length(cur_sb_info->chan_rr_node[side][itrack]);
          /* Get counter */
          rr_path_cnt = get_wire_L_counter_in_llist(rr_path_cnt, trpt_opts, "horizontal", L_wire, &wireL_cnt);
          path_cnt = wireL_cnt->cnt;
          fp = wireL_cnt->file_handler;
          /* This is a new L-wire, create the file handler and the mkdir command to the TCL script */
          if (0 == path_cnt) {
            fprintf(fp, "exec mkdir -p %s\n",
                    gen_verilog_one_routing_report_timing_Lwire_dir_path(fpga_verilog_opts.report_timing_path, L_wire)); 
          }
          /* Restore the disable_timing for the SB outputs on the path */
          /*fprintf(fp, "# Restore disable timing for the following Switch Block output:\n");
          restore_disable_timing_one_sb_output(fp, 
                                               cur_sb_info,
                                               cur_sb_info->chan_rr_node[side][itrack]);
          fprintf(fp, "# Report timing for all the paths using this output:\n");*/
          /* Dump report_timing command */
          verilog_generate_one_routing_segmental_report_timing(fp, fpga_verilog_opts,
                                                               cur_sb_info, 
                                                               cur_sb_info->chan_rr_node[side][itrack], 
                                                               LL_rr_node, "horizontal", &path_cnt);
          /* Disable the timing again */
          /*fprintf(fp, "# Set disable timing for the following Switch Block output:\n");
          set_disable_timing_one_sb_output(fp, 
                                           cur_sb_info,
                                           cur_sb_info->chan_rr_node[side][itrack]);*/
          /* Update the wire L*/
          update_wire_L_counter_in_llist(rr_path_cnt, L_wire, path_cnt);
        }
      }
    }
  }

  /* close file*/
  fclose_wire_L_file_handler_in_llist(rr_path_cnt);
  /* Need to reset the different variables */
  cur_sb_info = NULL;
  rr_path_cnt = NULL;
  wireL_cnt = NULL;
  path_cnt = 0; 

  vpr_printf(TIO_MESSAGE_INFO,
             "Generating TCL script to report timing for routing wires vertical\n");
  /* Start with vertical SBs*/
  /* We start from a SB[x][y] */
  for (ix = 0; ix < (nx + 1); ix++) {
    for (iy = 0; iy < (ny + 1); iy++) {
      cur_sb_info = &(sb_info[ix][iy]);
      for (side = 0; side < cur_sb_info->num_sides; side++) {
        for (itrack = 0; itrack < cur_sb_info->chan_width[side]; itrack++) {
          assert((CHANX == cur_sb_info->chan_rr_node[side][itrack]->type)
               ||(CHANY == cur_sb_info->chan_rr_node[side][itrack]->type));
          /* We only care the output port and it should indicate a SB mux */
          if ( (OUT_PORT != cur_sb_info->chan_rr_node_direction[side][itrack]) 
             || (FALSE != check_drive_rr_node_imply_short(*cur_sb_info, cur_sb_info->chan_rr_node[side][itrack], side))) {
            continue; 
          }
          /* Bypass if we have only 1 driving node */
          if (1 == cur_sb_info->chan_rr_node[side][itrack]->num_drive_rr_nodes) {
            continue; 
          }
          if (CHANX == cur_sb_info->chan_rr_node[side][itrack]->type) {
            continue;
          }
          /* Check if L_wire exists in the linked list */
          L_wire = get_rr_node_wire_length(cur_sb_info->chan_rr_node[side][itrack]);
          /* Get counter */
          rr_path_cnt = get_wire_L_counter_in_llist(rr_path_cnt, trpt_opts, "vertical", L_wire, &wireL_cnt);
          path_cnt = wireL_cnt->cnt;
          fp = wireL_cnt->file_handler;
          /* This is a new L-wire, create the file handler and the mkdir command to the TCL script */
          if (0 == path_cnt) {
            fprintf(fp, "exec mkdir -p %s\n",
                    gen_verilog_one_routing_report_timing_Lwire_dir_path(fpga_verilog_opts.report_timing_path, L_wire)); 
          }
          /* Restore the disable_timing for the SB outputs on the path */
          /*fprintf(fp, "# Restore disable timing for the following Switch Block output:\n");
          restore_disable_timing_one_sb_output(fp, 
                                               cur_sb_info,
                                               cur_sb_info->chan_rr_node[side][itrack]);*/
          fprintf(fp, "# Report timing for all the paths using this output:\n");
          /* Dump report_timing command */
          verilog_generate_one_routing_segmental_report_timing(fp, fpga_verilog_opts,
                                                               cur_sb_info, 
                                                               cur_sb_info->chan_rr_node[side][itrack], 
                                                               LL_rr_node, "vertical", &path_cnt);
          /* Disable the timing again */
          /*fprintf(fp, "# Set disable timing for the following Switch Block output:\n");
          set_disable_timing_one_sb_output(fp, 
                                           cur_sb_info,
                                           cur_sb_info->chan_rr_node[side][itrack]);*/
          /* Update the wire L*/
          update_wire_L_counter_in_llist(rr_path_cnt, L_wire, path_cnt);
        }
      }
    }
  }
  /* close file*/
  fclose_wire_L_file_handler_in_llist(rr_path_cnt);

  /* Free */
  free_wire_L_llist(rr_path_cnt);

  return;
}

static 
void verilog_generate_routing_wire_report_timing(t_trpt_opts trpt_opts,
                                                 t_rr_node* LL_rr_node,
                                                 t_syn_verilog_opts fpga_verilog_opts) {
  FILE* fp = NULL;
  int L_wire;
  t_llist* rr_path_cnt = NULL;
  t_wireL_cnt* wireL_cnt = NULL;
  int path_cnt = 0;

  vpr_printf(TIO_MESSAGE_INFO,
             "Generating TCL script to report timing for routing wires\n");

  /* We start from a SB[x][y] */
  DeviceCoordinator sb_range = device_rr_gsb.get_gsb_range();
  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      RRGSB rr_sb = device_rr_gsb.get_gsb(ix, iy);
      for (size_t side = 0; side < rr_sb.get_num_sides(); ++side) {
        Side side_manager(side);
        for (size_t itrack = 0; itrack < rr_sb.get_chan_width(side_manager.get_side()); ++itrack) {
          assert((CHANX == rr_sb.get_chan_node(side_manager.get_side(), itrack)->type)
               ||(CHANY == rr_sb.get_chan_node(side_manager.get_side(), itrack)->type));
          /* We only care the output port and it should indicate a SB mux */
          if ( (OUT_PORT != rr_sb.get_chan_node_direction(side_manager.get_side(), itrack)) 
             || (false != rr_sb.is_sb_node_imply_short_connection(rr_sb.get_chan_node(side_manager.get_side(), itrack)))) {
            continue; 
          }
          /* Bypass if we have only 1 driving node */
          if (1 == rr_sb.get_chan_node(side_manager.get_side(), itrack)->num_drive_rr_nodes) {
            continue; 
          }
          /* Check if L_wire exists in the linked list */
          L_wire = get_rr_node_wire_length(rr_sb.get_chan_node(side_manager.get_side(), itrack));
          /* Get counter */
          rr_path_cnt = get_wire_L_counter_in_llist(rr_path_cnt, trpt_opts, "horizontal", L_wire, &wireL_cnt);
          path_cnt = wireL_cnt->cnt;
          fp = wireL_cnt->file_handler;
          /* This is a new L-wire, create the file handler and the mkdir command to the TCL script */
          if (0 == path_cnt) {
            fprintf(fp, "exec mkdir -p %s\n",
                    gen_verilog_one_routing_report_timing_Lwire_dir_path(fpga_verilog_opts.report_timing_path, L_wire)); 
          }
          /* Restore the disable_timing for the SB outputs on the path */
          /*fprintf(fp, "# Restore disable timing for the following Switch Block output:\n");
          restore_disable_timing_one_sb_output(fp, 
                                               rr_sb,
                                               rr_sb.get_chan_node(side_manager.get_side(), itrack));*/
          fprintf(fp, "# Report timing for all the paths using this output:\n");
          /* Dump report_timing command */
          verilog_generate_one_routing_segmental_report_timing(fp, fpga_verilog_opts,
                                                               rr_sb, 
                                                               rr_sb.get_chan_node(side_manager.get_side(), itrack), 
                                                               LL_rr_node, "horizontal", &path_cnt);
          /* Disable the timing again */
          /*fprintf(fp, "# Set disable timing for the following Switch Block output:\n");
          set_disable_timing_one_sb_output(fp, 
                                           rr_sb,
                                           rr_sb.get_chan_node(side_manager.get_side(), itrack));*/
          /* Update the wire L*/
          update_wire_L_counter_in_llist(rr_path_cnt, L_wire, path_cnt);
        }
      }
    }
  }

  /* close file*/
  fclose_wire_L_file_handler_in_llist(rr_path_cnt);

  /* Free */
  free_wire_L_llist(rr_path_cnt);

  return;
}


/* Output a log file to guide routing report_timing */
void verilog_generate_report_timing(t_sram_orgz_info* cur_sram_orgz_info,
                                    char* sdc_dir,
                                    t_arch arch,
                                    t_det_routing_arch* routing_arch,
                                    int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                    t_ivec*** LL_rr_node_indices,
                                    t_syn_verilog_opts fpga_verilog_opts,
                                    boolean compact_routing_hierarchy) {
  t_trpt_opts trpt_opts;

  /* Initialize */
  trpt_opts.report_pb_timing = TRUE;
  trpt_opts.report_sb_timing = FALSE;
  trpt_opts.report_cb_timing = TRUE;
  trpt_opts.report_routing_timing = TRUE;
  trpt_opts.longest_path_only = TRUE;
  trpt_opts.print_thru_pins = TRUE;
  trpt_opts.sdc_dir = my_strdup(sdc_dir);
  trpt_opts.compact_routing_hierarchy = compact_routing_hierarchy;

  /* Part 1. Report timing for Programmable Logic Blocks */

  /* Part 2. Report timing for Connection Blocks */

  /* Part 3. Report timing for Switch Blocks */
  if (TRUE == trpt_opts.report_sb_timing) {
    verilog_generate_sb_report_timing(trpt_opts, arch, LL_rr_node);
  }

  /* Part 4. Report timing for routing segments of SB wires */
  if (TRUE == trpt_opts.report_routing_timing) {
    if (TRUE == trpt_opts.compact_routing_hierarchy) {
      verilog_generate_routing_wire_report_timing(trpt_opts,
                                                  LL_rr_node,
                                                  fpga_verilog_opts);
    } else {
      verilog_generate_routing_report_timing(trpt_opts,
                                             LL_rr_node,
                                             fpga_verilog_opts);
    }
  }

  return;
}


