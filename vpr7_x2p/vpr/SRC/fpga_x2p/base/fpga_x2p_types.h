#ifndef FPGA_X2P_TYPES_H
#define FPGA_X2P_TYPES_H

#include "vpr_types.h"
#include "route_common.h"

/* Define the basic data structures used for FPGA-SPICE */

/* Default ID of switch used in rr_node */
#define DEFAULT_SWITCH_ID 0

/* Default prev_node ID of a rr_node */
#define DEFAULT_PREV_NODE -1

/* Default path ID of a unused multiplexer */
#define DEFAULT_PATH_ID -1

/* Default path ID of a unused multiplexer when there are no constant inputs*/
#define DEFAULT_MUX_PATH_ID 0

/* Index of logical block indicating a wired LUT */
#define WIRED_LUT_LOGICAL_BLOCK_ID -2

#define BLIF_LUT_KEYWORD "names"

/* Mode Index of a LUT: 
 * 0 indicates a wired mode (a buffer) 
 * 0 indicates a regular mode (a buffer) 
 */
#define WIRED_LUT_MODE_INDEX 0
#define NORMAL_LUT_MODE_INDEX 1

/* Key data structure for router: routing resource graph 
 * This data structure store the key parameters that
 * models a routing resource graph used by router.
 * 1. number of routing resource nodes in the graph
 * 2. all the routing resource nodes
 * 3. router information for each routing resource node 
 */
typedef struct fpga_spice_rr_graph t_rr_graph;
struct fpga_spice_rr_graph {
  /* Routing Resource nodes */
  int num_rr_nodes;
  t_rr_node* rr_node;
  t_ivec*** rr_node_indices;

  /* Switches between routing resource nodes */
  int num_switch_inf;
  t_switch_inf* switch_inf;
  int delayless_switch_index;

  int num_nets; /* number of nets to route */
  t_net** net; /* nets to route, this is pointer to the existing nets */
  int* net_to_vpack_net_mapping;
  int* net_num_sources;
  int* net_num_sinks;

  /* Gives the rr_node indices of net terminals. */
  int **net_rr_sources; /* [0..num_nets-1][0..num_pins-1] */
  int **net_rr_sinks; /* [0..num_nets-1][0..num_pins-1] */
  int **net_rr_terminals; /* [0..num_nets-1][0..num_pins-1] */
  t_chunk rr_mem_ch;

  /* Routing statisitics */
  int num_rr_indexed_data;
  t_rr_indexed_data *rr_indexed_data; /* [0..(num_rr_indexed_data-1)] */

  t_rr_node_route_inf* rr_node_route_inf;
  t_bb *route_bb; /* [0..num_nets-1]. Limits area in which each  */

  /* Linked list start pointers.  Define the routing. */
  t_trace **trace_head; /* [0..(num_nets-1)] */
  t_trace **trace_tail; /* [0..(num_nets-1)] */
  t_trace *trace_free_head;
  t_chunk trace_ch;

  /**************** Static variables local to route_common.c ******************/
  t_heap **heap; /* Indexed from [1..heap_size] */
  int heap_size; /* Number of slots in the heap array */
  int heap_tail; /* Index of first unused slot in the heap array */

  /* For managing my own list of currently free heap data structures.     */
  t_heap *heap_free_head;
  /* For keeping track of the sudo malloc memory for the heap*/
  t_chunk heap_ch;

  t_linked_f_pointer *rr_modified_head;
  t_linked_f_pointer *linked_f_pointer_free_head;

  t_chunk linked_f_pointer_ch;

  #ifdef DEBUG
  int num_trace_allocated; /* To watch for memory leaks. */
  int num_heap_allocated;
  int num_linked_f_pointer_allocated;
  #endif

};

/* Key data structure for physical pb 
 * This data structure store the key parameters that
 * models a physical_pb.
 * This is a simplified copy of original t_pb
 * except the rr_graph part 
 */
typedef struct fpga_spice_phy_pb t_phy_pb;
struct fpga_spice_phy_pb {
  char *name; /* Name of this physical block */
  t_pb_graph_node *pb_graph_node; /* pointer to pb_graph_node this pb corresponds to */
  int  num_logical_blocks;
  int* logical_block; /* If this is a terminating pb, gives the logical (netlist) block that it contains */
  boolean* is_wired_lut; /* Specify if this is a wired LUT (used as buffer) */
  t_pb_graph_pin** lut_output_pb_graph_pin;
  int* lut_size;

  int mode; /* mode that this pb is set to */
  char* mode_bits; /* Mode bits for the logical block */

  t_phy_pb **child_pbs; /* children pbs attached to this pb [0..num_child_pb_types - 1][0..child_type->num_pb - 1] */
  t_phy_pb *parent_pb; /* pointer to parent node */

  /* Xifan TANG: FPGA-SPICE*/
  t_rr_graph* rr_graph;
  /* END */
  t_phy_pb **rr_node_to_pb_mapping; /* [0..num_local_rr_nodes-1] pointer look-up of which pb this rr_node belongs based on index, NULL if pb does not exist  */

  int *lut_pin_remap; /* [0..num_lut_inputs-1] applies only to LUT primitives, stores how LUT inputs were swapped during CAD flow, 
   LUT inputs can be swapped by changing the logic in the LUT, this is useful because the fastest LUT input compared to the slowest is often significant (2-5x),
   so this optimization is crucial for handling LUT based FPGAs.
   */

  /* Xifan TANG: SPICE model support*/
  char* spice_name_tag;

  /* Xifan TANG: FPGA-SPICE and SynVerilog */
  int num_reserved_conf_bits;
  int num_conf_bits;
  int num_mode_bits;
  int num_inpads;
  int num_outpads;
  int num_iopads;
};

#endif
