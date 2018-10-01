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
#include "rr_graph_swseg.h"
#include "vpr_utils.h"

/* Include spice support headers*/
#include "linkedlist.h"
#include "spice_globals.h"
#include "spice_utils.h"
#include "spice_mux.h"
#include "spice_pbtypes.h"
#include "spice_lut.h"


/***** Subroutines *****/
/* For LUTs without SPICE netlist defined, we can create a SPICE netlist
 * In this case, we need a MUX
 */
void stats_lut_spice_mux(t_llist** muxes_head,
                         t_spice_model* spice_model) {
  int lut_mux_size = 0; 
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;

  if (NULL == spice_model) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid Spice_model pointer!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Check */
  assert(SPICE_MODEL_LUT == spice_model->type); 

  /* Get input ports */
  input_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port);
  assert(1 == num_input_port);
  lut_mux_size = (int)pow(2.,(double)(input_ports[0]->size));

  /* MUX size = 2^lut_size */
  check_and_add_mux_to_linked_list(muxes_head, lut_mux_size, spice_model);

  return;
}

char* complete_truth_table_line(int lut_size,
                                char* input_truth_table_line) {
  char* ret = NULL;
  int num_token = 0;
  char** tokens = NULL;
  int cover_len = 0;
  int j;

  /* Due to the size of truth table may be less than the lut size.
   * i.e. in LUT-6 architecture, there exists LUT1-6 in technology-mapped netlists
   * So, in truth table line, there may be 10- 1
   * In this case, we should complete it by --10- 1
   */ 
  /*Malloc the completed truth table, lut_size + space + truth_val + '\0'*/
  ret = (char*)my_malloc(sizeof(char)*lut_size + 3);
  /* Split one line of truth table line*/
  tokens = my_strtok(input_truth_table_line, " ", &num_token); 
  /* Check, only 2 tokens*/
  /* Sometimes, the truth table is ' 0' or ' 1', which corresponds to a constant */
  if (1 == num_token) {
    /* restore the token[0]*/
    tokens = (char**)realloc(tokens, 2*sizeof(char*));
    tokens[1] = tokens[0];
    tokens[0] = my_strdup("-");
    num_token = 2;
  }

  /* In Most cases, there should be 2 tokens. */
  assert(2 == num_token);
  if ((0 != strcmp(tokens[1], "1"))&&(0 != strcmp(tokens[1], "0"))) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Last token of truth table line should be [0|1]!\n",
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Complete the truth table line*/
  cover_len = strlen(tokens[0]); 
  assert((cover_len < lut_size)||(cover_len == lut_size));
  /* Add the number of '-' we should add in the front*/
  for (j = 0; j < (lut_size - cover_len); j++) {
    ret[j] = '-';
  }
  /* Copy the original truth table line */ 
  sprintf(ret + lut_size - cover_len, "%s %s", tokens[0], tokens[1]); 

  /* Free */
  for (j = 0; j < num_token; j++) {
    my_free(tokens[j]);
  }

  return ret;
}

/* For each lut_bit_lines, we should recover the truth table,
 * and then set the sram bits to "1" if the truth table defines so.
 * Start_point: the position we start decode recursively
 */
void configure_lut_sram_bits_per_line_rec(int** sram_bits, 
                                          int lut_size,
                                          char* truth_table_line,
                                          int start_point) {
  int i;
  int num_sram_bit = (int)pow(2., (double)(lut_size));
  char* temp_line = my_strdup(truth_table_line);
  int do_config = 1;
  int sram_id = 0;

  /* Check the length of sram bits and truth table line */
  //assert((sizeof(int)*num_sram_bit) == sizeof(*sram_bits)); /*TODO: fix this assert*/
  assert((unsigned)(lut_size + 1 + 1)== strlen(truth_table_line)); /* lut_size + space + '1' */
  /* End of truth_table_line should be "space" and "1" */ 
  assert((0 == strcmp(" 1", truth_table_line + lut_size))||(0 == strcmp(" 0", truth_table_line + lut_size)));
  /* Make sure before start point there is no '-' */
  for (i = 0; i < start_point; i++) {
    assert('-' != truth_table_line[i]);
  }

  /* Configure sram bits recursively */
  for (i = start_point; i < lut_size; i++) {
    if ('-' == truth_table_line[i]) {
      do_config = 0;
      /* if we find a dont_care, we don't do configure now but recursively*/
      /* '0' branch */
      temp_line[i] = '0'; 
      configure_lut_sram_bits_per_line_rec(sram_bits, lut_size, temp_line, start_point + 1);
      /* '1' branch */
      temp_line[i] = '1'; 
      configure_lut_sram_bits_per_line_rec(sram_bits, lut_size, temp_line, start_point + 1);
      break; 
    }
  }

  /* do_config*/
  if (do_config) {
    for (i = 0; i < lut_size; i++) {
      /* Should be either '0' or '1' */
      switch (truth_table_line[i]) {
      case '0':
        /* We assume the 1-lut pass sram1 when input = 0 */
        sram_id += (int)pow(2., (double)(i));
        break;
      case '1':
        /* We assume the 1-lut pass sram0 when input = 1 */
        break;
      case '-':
        assert('-' != truth_table_line[i]); /* Make sure there is no dont_care */
      default :
        vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid truth_table bit(%c), should be [0|1|'-]!\n",
                   __FILE__, __LINE__, truth_table_line[i]); 
        exit(1);
      }
    }
    /* Set the sram bit to '1'*/
    assert(sram_id < num_sram_bit);
    if (0 == strcmp(" 1", truth_table_line + lut_size)) {
      (*sram_bits)[sram_id] = 1; /* on set*/
    } else if (0 == strcmp(" 0", truth_table_line + lut_size)) {
      (*sram_bits)[sram_id] = 0; /* off set */
    } else {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid truth_table_line ending(=%s)!\n",
                 __FILE__, __LINE__, truth_table_line + lut_size);
      exit(1);
    }
  }
  
  /* Free */
  my_free(temp_line);

  return; 
}

int* generate_lut_sram_bits(int truth_table_len,
                            char** truth_table,
                            int lut_size) {
  int num_sram = (int)pow(2.,(double)(lut_size));
  int* ret = (int*)my_malloc(sizeof(int)*num_sram); 
  char** completed_truth_table = (char**)my_malloc(sizeof(char*)*truth_table_len);
  int on_set = 0;
  int off_set = 0;
  int i;

  /* if No truth_table, do default*/
  if (0 == truth_table_len) {
    switch (default_signal_init_value) {
    case 0:
      off_set = 0;
      on_set = 1;
      break;
    case 1:
      off_set = 1;
      on_set = 0;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid default_signal_init_value(=%d)!\n",
                 __FILE__, __LINE__, default_signal_init_value);
      exit(1);
    }
  }

  /* Read in truth table lines, decode one by one */
  for (i = 0; i < truth_table_len; i++) {
    /* Complete the truth table line by line*/
    //printf("truth_table[%d] = %s\n", i, truth_table[i]);
    completed_truth_table[i] = complete_truth_table_line(lut_size, truth_table[i]);
    //printf("Completed_truth_table[%d] = %s\n", i, completed_truth_table[i]);
    if (0 == strcmp(" 1", completed_truth_table[i] + lut_size)) {
      on_set = 1;
    } else if (0 == strcmp(" 0", completed_truth_table[i] + lut_size)) {
      off_set = 1;
    }
  }
  //printf("on_set=%d off_set=%d", on_set, off_set);
  assert(1 == (on_set + off_set));

  if (1 == on_set) {
    /* Initial all sram bits to 0*/
    for (i = 0 ; i < num_sram; i++) {
      ret[i] = 0;
    }
  } else if (1 == off_set) {
    /* Initial all sram bits to 1*/
    for (i = 0 ; i < num_sram; i++) {
      ret[i] = 1;
    }
  }

  for (i = 0; i < truth_table_len; i++) {
    /* Update the truth table, sram_bits */
    configure_lut_sram_bits_per_line_rec(&ret, lut_size, completed_truth_table[i], 0);
  }

  /* Free */
  for (i = 0; i < truth_table_len; i++) {
    my_free(completed_truth_table[i]);
  }

  return ret;
}

void fprint_spice_lut_subckt(FILE* fp,
                             t_spice_model spice_model) {
  int i;
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  /* Ensure a valid file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler!\n",
               __FILE__, __LINE__);
  }
  
  /* Find input ports, output ports and sram ports*/
  input_ports = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port);
  output_ports = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port);
  sram_ports = find_spice_model_ports(&spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port);

  /* Check */
  assert(1 == num_input_port);
  assert(1 == num_output_port);
  assert(1 == num_sram_port);

  fprintf(fp, "***** Auto-generated LUT info: spice_model_name = %s, size = %d *****\n",
          spice_model.name, input_ports[0]->size);
  /* Define the subckt*/
  fprintf(fp, ".subckt %s ", spice_model.name); /* Subckt name*/
  /* Input ports*/
  for (i = 0; i < input_ports[0]->size; i++) {
    fprintf(fp, "%s%d ", input_ports[0]->prefix, i); 
  }
  /* output ports*/
  assert(1 == output_ports[0]->size); 
  fprintf(fp, "%s ", output_ports[0]->prefix);
  /* sram ports */
  for (i = 0; i < sram_ports[0]->size; i++) {
    fprintf(fp, "%s%d ", sram_ports[0]->prefix, i); 
  }
  /* local vdd and gnd*/
  fprintf(fp, "svdd sgnd\n");

  /* Input buffers */
  for (i = 0; i < input_ports[0]->size; i++) {
    /* For postive input of LUT MUX*/
    fprintf(fp, "Xinv0_in%d %s%d midinv_in%d svdd sgnd inv size=\'%g\'",
            i, input_ports[0]->prefix, i, i, spice_model.lut_input_buffer->size);
    fprintf(fp, "\n");
    fprintf(fp, "Xinv0mid_in%d midinv_in%d lut_mux_in%d svdd sgnd inv size=\'%g\'",
            i, i, i, 
            spice_model.lut_input_buffer->size*spice_model.lut_input_buffer->f_per_stage);
    fprintf(fp, "\n");
    /* For negative input of LUT MUX*/
    fprintf(fp, "Xinv1_in%d %s%d lut_mux_in%d_inv svdd sgnd inv size=\'%g\'",
            i, input_ports[0]->prefix, i, i, 
            spice_model.lut_input_buffer->size*spice_model.lut_input_buffer->f_per_stage);
    fprintf(fp, "\n");
  }

  /* Output buffers already included in LUT MUX */
  /* LUT MUX*/
  assert(sram_ports[0]->size == (int)pow(2.,(double)(input_ports[0]->size)));
  fprintf(fp, "Xlut_mux ");
  /* SRAM ports of LUT, they are inputs of lut_muxes*/
  for (i = 0; i < sram_ports[0]->size; i++) {
    fprintf(fp, "%s%d ", sram_ports[0]->prefix, i);
  } 
  /* Output port, LUT output is LUT MUX output*/
  fprintf(fp, "%s ", output_ports[0]->prefix);
  /* input port, LUT input is LUT MUX sram*/
  for (i = 0; i < input_ports[0]->size; i++) {
    fprintf(fp, "lut_mux_in%d lut_mux_in%d_inv ", i, i); 
  }
  /* Local vdd and gnd*/
  fprintf(fp, "svdd sgnd %s_mux_size%d\n", spice_model.name, sram_ports[0]->size);

  /* End of LUT subckt*/
  fprintf(fp, ".eom\n");

  /* Free */
  free(input_ports);
  free(output_ports);
  free(sram_ports);

  return;
} 

/* Print LUT subckts into a SPICE file*/
void generate_spice_luts(char* subckt_dir, 
                         int num_spice_model, 
                         t_spice_model* spice_models) {
  FILE* fp = NULL;
  char* sp_name = my_strcat(subckt_dir, luts_spice_file_name);
  int imodel = 0;

  /* Create FILE*/
  fp = fopen(sp_name, "w");
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(FILE:%s,LINE[%d])Failure in create SPICE netlist %s",__FILE__, __LINE__, wires_spice_file_name); 
    exit(1);
  } 
  fprint_spice_head(fp,"LUTs");

  for (imodel = 0; imodel < num_spice_model; imodel++) {
    if ((SPICE_MODEL_LUT == spice_models[imodel].type)
       &&(NULL == spice_models[imodel].model_netlist)) {
      fprint_spice_lut_subckt(fp, spice_models[imodel]);
    }
  }

  /* Close*/
  fclose(fp);

  return;
}

char** assign_lut_truth_table(t_logical_block* mapped_logical_block,
                              int* truth_table_length) {
  char** truth_table = NULL;
  t_linked_vptr* head = NULL;
  int cur = 0;

  if (NULL == mapped_logical_block) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid mapped_logical_block!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  /* Count the lines of truth table*/
  head = mapped_logical_block->truth_table;
  while(head) {
    (*truth_table_length)++;
    head = head->next;
  }
  /* Allocate truth_tables */
  truth_table = (char**)my_malloc(sizeof(char*)*(*truth_table_length));
  /* Fill truth_tables*/
  cur = 0;
  head = mapped_logical_block->truth_table;
  while(head) {
    truth_table[cur] = my_strdup((char*)head->data_vptr);
    head = head->next;
    cur++;
  }
  assert(cur == (*truth_table_length));

  return truth_table;
}

int get_lut_output_init_val(t_logical_block* lut_logical_block) {
  int i;
  int* sram_bits = NULL; /* decoded SRAM bits */ 
  int truth_table_length = 0;
  char** truth_table = NULL;
  int lut_size = 0;
  int input_net_index = OPEN;
  int* input_init_val = NULL;
  int init_path_id = 0;
  int output_init_val = 0;

  /* Ensure a valid file handler*/ 
  if (NULL == lut_logical_block) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid LUT logical block!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  /* Get the truth table */
  truth_table = assign_lut_truth_table(lut_logical_block, &truth_table_length); 
  lut_size = lut_logical_block->used_input_pins;
  assert(!(0 > lut_size));
  /* Special for LUT_size = 0 */
  if (0 == lut_size) {
    /* Generate sram bits*/
    sram_bits = generate_lut_sram_bits(truth_table_length, truth_table, 1);
    /* This is constant generator, SRAM bits should be the same */
    output_init_val = sram_bits[0];
    for (i = 0; i < (int)pow(2.,(double)lut_size); i++) { 
      assert(sram_bits[i] == output_init_val);
    } 
  } else { 
    /* Generate sram bits*/
    sram_bits = generate_lut_sram_bits(truth_table_length, truth_table, lut_size);

    assert(1 == lut_logical_block->pb->pb_graph_node->num_input_ports);
    assert(1 == lut_logical_block->pb->pb_graph_node->num_output_ports);
    /* Get the initial path id */
    input_init_val = (int*)my_malloc(sizeof(int)*lut_size);
    for (i = 0; i < lut_size; i++) {
      input_net_index = lut_logical_block->input_nets[0][i]; 
      input_init_val[i] = vpack_net[input_net_index].spice_net_info->init_val;
    } 

    init_path_id = determine_lut_path_id(lut_size, input_init_val);
    /* Check */  
    assert((!(0 > init_path_id))&&(init_path_id < (int)pow(2.,(double)lut_size)));
    output_init_val = sram_bits[init_path_id]; 
  }
   
  /*Free*/
  for (i = 0; i < truth_table_length; i++) {
    free(truth_table[i]);
  }
  free(truth_table);
  my_free(sram_bits);

  return output_init_val;
}

void fprint_pb_primitive_lut(FILE* fp,
                             char* subckt_prefix,
                             t_logical_block* mapped_logical_block,
                             t_pb_graph_node* cur_pb_graph_node,
                             int index,
                             t_spice_model* spice_model) {
  int i;
  int* sram_bits = NULL; /* decoded SRAM bits */ 
  int truth_table_length = 0;
  char** truth_table = NULL;
  int lut_size = 0;
  int num_input_port = 0;
  t_spice_model_port** input_ports = NULL;
  int num_output_port = 0;
  t_spice_model_port** output_ports = NULL;
  int num_sram_port = 0;
  t_spice_model_port** sram_ports = NULL;

  char* formatted_subckt_prefix = format_spice_node_prefix(subckt_prefix); /* Complete a "_" at the end if needed*/
  t_pb_type* cur_pb_type = NULL;
  char* port_prefix = NULL;
  int cur_sram = 0;
  int num_sram = 0;

  /* Ensure a valid file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Ensure a valid pb_graph_node */ 
  if (NULL == cur_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid cur_pb_graph_node!\n",
               __FILE__, __LINE__);
    exit(1);
  }
  /* Asserts */
  assert(SPICE_MODEL_LUT == spice_model->type);

  /* Check if this is an idle logical block mapped*/
  if (NULL != mapped_logical_block) {
    truth_table = assign_lut_truth_table(mapped_logical_block, &truth_table_length); 
    /* Back-annotate to logical block */
    mapped_logical_block->mapped_spice_model = spice_model;
    mapped_logical_block->mapped_spice_model_index = spice_model->cnt;
  }
  /* Determine size of LUT*/
  input_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_INPUT, &num_input_port);
  output_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port);
  sram_ports = find_spice_model_ports(spice_model, SPICE_MODEL_PORT_SRAM, &num_sram_port);
  assert(1 == num_input_port);
  assert(1 == num_output_port);
  assert(1 == num_sram_port);
  lut_size = input_ports[0]->size;
  num_sram = (int)pow(2.,(double)(lut_size));
  assert(num_sram == sram_ports[0]->size);
  assert(1 == output_ports[0]->size);

  /* Generate sram bits*/
  sram_bits = generate_lut_sram_bits(truth_table_length, truth_table, lut_size);
 
  /* Print the subckts*/ 
  cur_pb_type = cur_pb_graph_node->pb_type;
  /* Subckt definition*/
  fprintf(fp, ".subckt %s%s[%d] ", formatted_subckt_prefix, cur_pb_type->name, index);
  /* Print inputs, outputs, inouts, clocks, NO SRAMs*/
  /*
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(formatted_subckt_prefix) + strlen(cur_pb_type->name) + 1 +
                 strlen(my_itoa(index)) + 1 + 1));
  sprintf(port_prefix, "%s%s[%d]", formatted_subckt_prefix, cur_pb_type->name, index);
  */
  /* Simplify the prefix, make the SPICE netlist readable*/
  port_prefix = (char*)my_malloc(sizeof(char)*
                (strlen(cur_pb_type->name) + 1 +
                 strlen(my_itoa(index)) + 1 + 1));
  sprintf(port_prefix, "%s[%d]", cur_pb_type->name, index);
  
  fprint_pb_type_ports(fp, port_prefix, 0, cur_pb_type); 
  /* SRAM bits are fixed in this subckt, no need to define them here*/
  /* Local Vdd and gnd*/ 
  fprintf(fp, "svdd sgnd\n");
  /* Definition ends*/

  /* Print the encoding in SPICE netlist for debugging */
  fprintf(fp, "***** Truth Table for LUT[%d], size=%d. *****\n", 
          spice_model->cnt, lut_size);
  for (i = 0; i < truth_table_length; i++) {
    fprintf(fp,"* %s *\n", truth_table[i]);
  } 

  fprintf(fp, "***** SRAM bits for LUT[%d], size=%d, num_sram=%d. *****\n", 
          spice_model->cnt, lut_size, num_sram);
  fprintf(fp, "*****");
  for (i = 0; i < num_sram; i++) {
     fprintf(fp, "%d", sram_bits[i]);
  }
  fprintf(fp, "*****\n");

  /* Call SRAM subckts*/
  cur_sram = sram_spice_model->cnt;
  for (i = 0; i < num_sram; i++) {
    fprintf(fp, "X%s[%d] ", sram_spice_model->prefix, cur_sram); /* SRAM subckts*/
    /* fprintf(fp, "%s[%d]->in ", sram_spice_model->prefix, cur_sram);*/ /* Input*/
    fprintf(fp, "%s->in ", sram_spice_model->prefix); /* Input*/
    fprintf(fp, "%s[%d]->out %s[%d]->outb ", 
            sram_spice_model->prefix, cur_sram, sram_spice_model->prefix, cur_sram); /* Outputs */
    fprintf(fp, "gvdd_sram_luts sgnd %s\n", sram_spice_model->name);  //
    /* Add nodeset to help convergence */ 
    fprintf(fp, ".nodeset V(%s[%d]->out) 0\n", sram_spice_model->prefix, cur_sram);
    fprintf(fp, ".nodeset V(%s[%d]->outb) vsp\n", sram_spice_model->prefix, cur_sram);
    cur_sram++;
  }

  /* Call LUT subckt*/
  fprintf(fp, "X%s[%d] ", spice_model->name, spice_model->cnt);
  /* Connect inputs*/ 
  /* Connect outputs*/
  fprint_pb_type_ports(fp, port_prefix, 0, cur_pb_type); 
  /* Connect srams*/
  cur_sram = sram_spice_model->cnt;
  for (i = 0; i < num_sram; i++) {
    switch (sram_bits[i]) {
    /* the pull UP/down vdd/gnd should be connected to the local interc gvdd*/
    /* TODO: we want to see the dynamic power of each LUT, we may split these global vdd*/
    case 0: 
      fprintf(fp, "%s[%d]->out ", sram_spice_model->prefix, cur_sram); 
      break;
    case 1: 
      fprintf(fp, "%s[%d]->outb ", sram_spice_model->prefix, cur_sram); 
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid sram_bit(=%d)! Should be [0|1]).\n", __FILE__, __LINE__, sram_bits[i]);
      exit(1);
    }
    cur_sram++;
  }
  /* vdd should be connected to special global wire gvdd_lut and gnd,
   * Every LUT has a special VDD for statistics
   */
  fprintf(fp, "gvdd_%s[%d] sgnd %s\n", spice_model->prefix, spice_model->cnt, spice_model->name);
  /* TODO: Add a nodeset for convergence */

  /* End of subckt*/
  fprintf(fp, ".eom\n");
  
  spice_model->cnt++;
  sram_spice_model->cnt = cur_sram;

  /*Free*/
  for (i = 0; i < truth_table_length; i++) {
    free(truth_table[i]);
  }
  free(truth_table);
  my_free(formatted_subckt_prefix);
  my_free(input_ports);
  my_free(output_ports);
  my_free(sram_ports);
  my_free(sram_bits);
  my_free(port_prefix);

  return;
}
