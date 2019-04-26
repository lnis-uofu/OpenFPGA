
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
#include "token.h"

/* Include SPICE support headers*/
#include "quicksort.h"
#include "linkedlist.h"
#include "fpga_x2p_globals.h"
#include "fpga_x2p_utils.h"

/* Build the list of spice_model_ports provided in the cur_spice_model delay_info */
t_spice_model_port** get_spice_model_delay_info_ports(t_spice_model* cur_spice_model, 
                                                      char* port_list,
                                                      int* num_port) {
  int itok;
  int num_token = 0;
  char** tokens = NULL;
  t_spice_model_port** port = NULL;

  /* Get input ports */
  tokens = fpga_spice_strtok(port_list, " ", &num_token); 
  /* allocate in_port */
  port = (t_spice_model_port**) my_malloc(sizeof(t_spice_model_port*) * num_token);
  /* Find corresponding spice_model_port */
  for (itok = 0; itok < num_token; itok++) {
    port[itok] = find_spice_model_port_by_name(cur_spice_model, tokens[itok]);
    /* Error out if we cannot find a port */
    if (NULL == port[itok]) {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Fail to find a port listed in delay_info (port_name=%s)!\n",
                 __FILE__, __LINE__, tokens[itok]);
      exit(1);
    }
    /* TODO: Error out if port type does not match */
  }

  /* give return value */
  (*num_port) = num_token; 

  return port; 
}

/* Determine the number of tedges (timing edges) for each output pin of a SPICE model 
 * For each output pin, we need a tedge connected to all the input pins
 * The number of tedges per pin is the number of input pins
 */
int get_spice_model_num_tedges_per_pin(t_spice_model* cur_spice_model,
                                       enum PORTS port_type) {
  int iport;
  int num_tedges = 0;

  /* Get num_edges for each output pin */
  for (iport = 0; iport < cur_spice_model->num_port; iport++) {
    switch (port_type) {
    case IN_PORT:
      if (SPICE_MODEL_PORT_OUTPUT != cur_spice_model->ports[iport].type) {
        continue; /* ALL output ports requires a tedge */ 
      } 
      num_tedges += cur_spice_model->ports[iport].size;
    case OUT_PORT:
      if (SPICE_MODEL_PORT_OUTPUT == cur_spice_model->ports[iport].type) {
        continue; /* ALL non-output ports requires a tedge */ 
      } 
      num_tedges += cur_spice_model->ports[iport].size;
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid port type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }

  return num_tedges;
}

t_spice_model_tedge* get_unused_spice_model_port_tedge(t_spice_model_port* cur_port,
                                                      int pin_index) {
  int iedge;

  /* Check the edge array */
  for (iedge = 0; iedge < cur_port->num_tedges[pin_index]; iedge++) {
    /* See if this is an unused edge */
    if (NULL == cur_port->tedge[pin_index][iedge]->from_port) {
       assert (OPEN == cur_port->tedge[pin_index][iedge]->from_port_pin_number);
       return cur_port->tedge[pin_index][iedge];
    }
  }  

  return NULL;
}

/* Allocate tedges (timing edges) for a SPICE model 
 * For each output pin, we need a tedge connected to all the input pins
 */
void alloc_spice_model_num_tedges(t_spice_model* cur_spice_model) {
  int iport, ipin, iedge;
  int jport, jpin;
  int num_out_tedges = 0;
  int num_in_tedges = 0;

  /* Get num_edges for each output pin */
  num_out_tedges = get_spice_model_num_tedges_per_pin(cur_spice_model, OUT_PORT);
  num_in_tedges = get_spice_model_num_tedges_per_pin(cur_spice_model, IN_PORT);

  /* Allocate tedges for output ports */
  for (iport = 0; iport < cur_spice_model->num_port; iport++) {
    if (  (SPICE_MODEL_PORT_OUTPUT != cur_spice_model->ports[iport].type) 
       && (SPICE_MODEL_PORT_INOUT != cur_spice_model->ports[iport].type)) {
      continue; /* We only care OUTPUT and INOUT ports */ 
    } 
    /* allocate num_tedges */
    cur_spice_model->ports[iport].num_tedges = (int*) my_malloc(sizeof(int) * cur_spice_model->ports[iport].size);
    for (ipin = 0; ipin < cur_spice_model->ports[iport].size; ipin++) {
      cur_spice_model->ports[iport].num_tedges[ipin] = num_out_tedges;
    } 
    /* allocate tedges */
    cur_spice_model->ports[iport].tedge = (t_spice_model_tedge***) my_calloc(cur_spice_model->ports[iport].size, sizeof(t_spice_model_tedge**));
    for (ipin = 0; ipin < cur_spice_model->ports[iport].size; ipin++) {
      cur_spice_model->ports[iport].tedge[ipin] = (t_spice_model_tedge**) my_calloc(cur_spice_model->ports[iport].num_tedges[ipin], sizeof(t_spice_model_tedge*));
      /* Allocate tedge and fill the pointor array */
      for (iedge = 0; iedge < cur_spice_model->ports[iport].num_tedges[ipin]; iedge++) {
        cur_spice_model->ports[iport].tedge[ipin][iedge] = (t_spice_model_tedge*) my_calloc(1, sizeof(t_spice_model_tedge));
        /* Initialize the to_port information of the tedges */
        cur_spice_model->ports[iport].tedge[ipin][iedge]->to_port = &(cur_spice_model->ports[iport]);
        cur_spice_model->ports[iport].tedge[ipin][iedge]->to_port_pin_number = ipin;
        cur_spice_model->ports[iport].tedge[ipin][iedge]->from_port = NULL;
        cur_spice_model->ports[iport].tedge[ipin][iedge]->from_port_pin_number = OPEN;
      }
    }
  }

  /* Allocate tedges for input ports */
  for (iport = 0; iport < cur_spice_model->num_port; iport++) {
    if (SPICE_MODEL_PORT_OUTPUT == cur_spice_model->ports[iport].type) {
      continue; /* We only care non-OUTPUT ports */ 
    } 
    /* allocate num_tedges */
    cur_spice_model->ports[iport].num_tedges = (int*) my_malloc(sizeof(int) * cur_spice_model->ports[iport].size);
    for (ipin = 0; ipin < cur_spice_model->ports[iport].size; ipin++) {
      cur_spice_model->ports[iport].num_tedges[ipin] = num_in_tedges;
    } 
   /* allocate tedges */
    cur_spice_model->ports[iport].tedge = (t_spice_model_tedge***) my_calloc(cur_spice_model->ports[iport].size, sizeof(t_spice_model_tedge**));
    for (ipin = 0; ipin < cur_spice_model->ports[iport].size; ipin++) {
      cur_spice_model->ports[iport].tedge[ipin] = (t_spice_model_tedge**) my_calloc(cur_spice_model->ports[iport].num_tedges[ipin], sizeof(t_spice_model_tedge*));
    }
  }

  /* Find tedge and fill the pointor array */
  /* Get the unused edge from each output edge */
  for (iport = 0; iport < cur_spice_model->num_port; iport++) {
    if (SPICE_MODEL_PORT_OUTPUT == cur_spice_model->ports[iport].type) {
      continue; /* We only care non-OUTPUT ports */ 
    } 
    for (ipin = 0; ipin < cur_spice_model->ports[iport].size; ipin++) {
      for (iedge = 0; iedge < cur_spice_model->ports[iport].num_tedges[ipin]; iedge++) {
        /* Find each output edge */
        for (jport = 0; jport < cur_spice_model->num_port; jport++) {
          if (  (SPICE_MODEL_PORT_OUTPUT != cur_spice_model->ports[jport].type) 
            && (SPICE_MODEL_PORT_INOUT != cur_spice_model->ports[jport].type)) {
            continue; /* We only care OUTPUT and INOUT ports */ 
          } 
          for (jpin = 0; jpin < cur_spice_model->ports[jport].size; jpin++) {
            /* get the first unused edge */
            cur_spice_model->ports[iport].tedge[ipin][iedge] = get_unused_spice_model_port_tedge(&(cur_spice_model->ports[jport]), jpin);
            assert(NULL != cur_spice_model->ports[iport].tedge[ipin][iedge]);
            /* Configure the tedge */
            cur_spice_model->ports[iport].tedge[ipin][iedge]->from_port = &(cur_spice_model->ports[iport]);
            cur_spice_model->ports[iport].tedge[ipin][iedge]->from_port_pin_number = ipin;
          }
        } 
      }
    }
  }

  return; 
}

/* Depend on the type of delay, configure the tedge content */
void configure_one_tedge_delay(t_spice_model_tedge* cur_tedge,
                               enum spice_model_delay_type delay_type,
                               float delay) {

  switch (delay_type) {
  case SPICE_MODEL_DELAY_RISE:
    cur_tedge->trise = delay;
    break;
  case SPICE_MODEL_DELAY_FALL:
    cur_tedge->tfall = delay;
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d])Invalid delay type!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
} 

void configure_tedges_delay_matrix(enum spice_model_delay_type delay_type,
                                   int num_in_port, t_spice_model_port** in_port,
                                   int num_out_port, t_spice_model_port** out_port,
                                   float** delay_matrix) {
  int iedge, iport, ipin;
  int jport;

  /* Configure timing edges for this spice_model */
  for (iport = 0; iport < num_in_port; iport++) {
    for (ipin = 0; ipin < in_port[iport]->size; ipin++) {
      for (iedge = 0; iedge < in_port[iport]->num_tedges[ipin]; iedge++) {
        /* check each edge, see if the from_port and to_port match! */
        /* Src should match! */
        assert ( in_port[iport] == in_port[iport]->tedge[ipin][iedge]->from_port );
        for (jport = 0; jport < num_out_port; jport++) {
          /* Check if des matches */
          if ( out_port[jport] == in_port[iport]->tedge[ipin][iedge]->to_port ) {
            configure_one_tedge_delay(in_port[iport]->tedge[ipin][iedge], delay_type, delay_matrix[iport][jport]);
          }
        }
      }
    }
  } 

  return;
}

/* allocate and parse delay_matix */
float** fpga_spice_atof_2D(int num_in_port, int num_out_port, char* str) {
  int i, j;
  float** delay_matrix = NULL;

  /* allocate */
  delay_matrix = (float**)my_calloc(num_in_port, sizeof(float*));
  for (i = 0; i < num_in_port; i++) {
    delay_matrix[i] = (float*)my_calloc(num_out_port, sizeof(float));
  }

  my_atof_2D(delay_matrix, num_in_port, num_out_port, str);
  
  return delay_matrix;
}

void free_2D_matrix(void** delay_matrix,
                    int num_in_port, int num_out_port) {
  int i;

  for (i = 0; i < num_in_port; i++) {
    my_free(delay_matrix[i]);
  }

  my_free(delay_matrix);
 
  return;
}

/* Build timing graph for a spice_model */
void annotate_spice_model_timing(t_spice_model* cur_spice_model) {
  int i;
  int num_in_port = 0;
  t_spice_model_port** in_port = NULL;
  int num_out_port = 0;
  t_spice_model_port** out_port = NULL;
  float** delay_matrix = NULL;

  /* check */
  assert ( 0 < cur_spice_model->num_delay_info );

  /* Allocate edges */
  alloc_spice_model_num_tedges(cur_spice_model);

  /* Parse each delay_info */ 
  for (i = 0; i < cur_spice_model->num_delay_info; i++) {
    /* Get input and output ports */
    in_port = get_spice_model_delay_info_ports(cur_spice_model, cur_spice_model->delay_info[i].in_port_name, &num_in_port);
    out_port = get_spice_model_delay_info_ports(cur_spice_model, cur_spice_model->delay_info[i].out_port_name, &num_out_port);
    /* create fpga_spice atof_2D !!! */
    delay_matrix = fpga_spice_atof_2D(num_in_port, num_out_port, cur_spice_model->delay_info[i].value);
    /* create tedges with delay_matrix */
    configure_tedges_delay_matrix(cur_spice_model->delay_info[i].type,
                                  num_in_port, in_port,
                                  num_out_port, out_port,
                                  delay_matrix);
    /* Free */
    free_2D_matrix((void**)delay_matrix, num_in_port, num_out_port);
  }

  return;
}

