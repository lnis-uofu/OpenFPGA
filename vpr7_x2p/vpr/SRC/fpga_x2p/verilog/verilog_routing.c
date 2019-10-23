/*********************************************************************
 * This file includes functions that are used for 
 * Verilog generation of FPGA routing architecture (global routing) 
 *********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>

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

#include "vtr_assert.h"

/* Include SPICE support headers*/
#include "linkedlist.h"
#include "rr_blocks.h"
#include "rr_blocks_utils.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_backannotate_utils.h"
#include "fpga_x2p_mux_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "fpga_x2p_bitstream_utils.h"
#include "fpga_x2p_globals.h"
#include "fpga_x2p_naming.h"
#include "mux_utils.h"
#include "module_manager.h"
#include "module_manager_utils.h"
#include "fpga_x2p_mem_utils.h"

/* Include Verilog support headers*/
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_writer_utils.h"
#include "verilog_module_writer.h"
#include "verilog_routing.h"

/*********************************************************************
 * Generate the Verilog module for a routing channel
 * Routing track wire, which is 1-input and dual output
 * This type of wires are used in the global routing architecture.
 * One of the output is wired to another Switch block multiplexer, 
 * while the mid-output is wired to a Connection block multiplexer.
 *     
 *                  |    CLB     |
 *                  +------------+
 *                        ^
 *                        |
 *           +------------------------------+
 *           | Connection block multiplexer |
 *           +------------------------------+
 *                        ^
 *                        |  mid-output         +--------------
 *              +--------------------+          |
 *    input --->| Routing track wire |--------->| Switch Block
 *              +--------------------+  output  |
 *                                              +--------------
 *
 * IMPORTANT: This function is designed for outputting unique Verilog modules                                             
 *            of routing channels
 *
 * TODO: This function should be adapted to the RRGraph object
 *********************************************************************/
static 
void print_verilog_routing_unique_chan_subckt(ModuleManager& module_manager, 
                                              const std::string& verilog_dir,
                                              const std::string& subckt_dir,
                                              const size_t& rr_chan_subckt_id, 
                                              const RRChan& rr_chan) {
  std::string fname_prefix;

  /* TODO: use a constexpr String arrary to replace this switch cases? */
  /* Find the prefix for the Verilog file name */
  switch (rr_chan.get_type()) {
  case CHANX:
    fname_prefix = std::string(chanx_verilog_file_name_prefix); 
    break;
  case CHANY:
    fname_prefix = std::string(chany_verilog_file_name_prefix); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d])Invalid Channel type! Should be CHANX or CHANY.\n",
               __FILE__, __LINE__);
    exit(1);
  }

  std::string verilog_fname(subckt_dir + generate_routing_block_netlist_name(fname_prefix, rr_chan_subckt_id, std::string(verilog_netlist_file_postfix)));
  /* TODO: remove the bak file when the file is ready */
  verilog_fname += ".bak";

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  print_verilog_file_header(fp, "Verilog modules for routing channel in X- and Y-direction"); 

  /* Print preprocessing flags */
  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = module_manager.add_module(generate_routing_channel_module_name(rr_chan.get_type(), rr_chan_subckt_id)); 

  /* Add ports to the module */
  /* For the LEFT side of a X-direction routing channel 
   * or the BOTTOM bottom side of a Y-direction routing channel 
   * Routing Resource Nodes in INC_DIRECTION are inputs of the module 
   *
   * For the RIGHT side of a X-direction routing channel 
   * or the TOP bottom side of a Y-direction routing channel 
   * Routing Resource Nodes in INC_DIRECTION are outputs of the module 
   *
   * An example of X-direction routing channel consisting of W routing nodes:
   *                            +--------------------------+
   *    nodeA(INC_DIRECTION)--->| in[0]             out[0] |---> nodeA(INC_DIRECTION) 
   *    nodeB(DEC_DIRECTION)<---| out[1]             in[1] |<--- nodeB(DEC_DIRECTION) 
   *          ...                  ...               ...           ...
   *    nodeX(INC_DIRECTION)--->| in[W-1]         out[W-1] |---> nodeX(INC_DIRECTION) 
   *                            +--------------------------+
   *
   * An example of Y-direction routing channel consisting of W routing nodes:
   *
   *                           nodeA              nodeB              nodeX
   *                      (INC_DIRECTION)      (DEC_DIRECTION)   (DEC_DIRECTION)
   *                             ^                  |       ...      |
   *                             |                  v                v
   *                      +------------------------------   ...   -------+
   *                      |  out[0]               in[1]            in[X] |
   *                      |                                              |
   *                      |                                              |
   *                      |   in[0]               out[1]    ...   out[X] |
   *                      +------------------------------   ...   -------+
   *                             ^                  |                |
   *                             |                  v                v
   *                           nodeA              nodeB            nodeX
   *                      (INC_DIRECTION)      (DEC_DIRECTION)   (DEC_DIRECTION)
   */
  /* Add ports at LEFT/BOTTOM side of the module */
  for (size_t itrack = 0; itrack < rr_chan.get_chan_width(); ++itrack) {
    switch (rr_chan.get_node(itrack)->direction) {
    case INC_DIRECTION: {
      /* TODO: naming should be more flexible !!! */
      BasicPort input_port(std::string("in" + std::to_string(itrack)), 1);
      module_manager.add_port(module_id, input_port, ModuleManager::MODULE_INPUT_PORT);
      break;
    }
    case DEC_DIRECTION: {
      /* TODO: naming should be more flexible !!! */
      BasicPort output_port(std::string("out" + std::to_string(itrack)), 1);
      module_manager.add_port(module_id, output_port, ModuleManager::MODULE_OUTPUT_PORT);
      break;
    }
    case BI_DIRECTION:
    default:
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File: %s [LINE%d]) Invalid direction of rr_node %s[%lu]_in/out[%lu]!\n",
                 __FILE__, __LINE__, 
                 convert_chan_type_to_string(rr_chan.get_type()),
                 rr_chan_subckt_id, itrack);
      exit(1);
    }
  }
  /* Add ports at RIGHT/TOP side of the module */
  for (size_t itrack = 0; itrack < rr_chan.get_chan_width(); ++itrack) {
    switch (rr_chan.get_node(itrack)->direction) {
    case INC_DIRECTION: {
      /* TODO: naming should be more flexible !!! */
      BasicPort output_port(std::string("out" + std::to_string(itrack)), 1);
      module_manager.add_port(module_id, output_port, ModuleManager::MODULE_OUTPUT_PORT);
      break;
    }
    case DEC_DIRECTION: {
      /* TODO: naming should be more flexible !!! */
      BasicPort input_port(std::string("in" + std::to_string(itrack)), 1);
      module_manager.add_port(module_id, input_port, ModuleManager::MODULE_INPUT_PORT);
      break;
    }
    case BI_DIRECTION:
    default:
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File: %s [LINE%d]) Invalid direction of rr_node %s[%lu]_in/out[%lu]!\n",
                 __FILE__, __LINE__, 
                 convert_chan_type_to_string(rr_chan.get_type()),
                 rr_chan_subckt_id, itrack);
      exit(1);
    }
  }
  /* Add middle-point output for connection box inputs */
  for (size_t itrack = 0; itrack < rr_chan.get_chan_width(); ++itrack) {
    /* TODO: naming should be more flexible !!! */
    BasicPort mid_output_port(std::string("mid_out" + std::to_string(itrack)), 1);
    module_manager.add_port(module_id, mid_output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id);
  /* Finish dumping ports */

  /* Print short-wire connection: 
   *    
   *   in[i] ----------> out[i]
   *             |
   *             +-----> mid_out[i]
   */
  for (size_t itrack = 0; itrack < rr_chan.get_chan_width(); ++itrack) {
    /* short connecting inputs and outputs: 
     * length of metal wire and parasitics are handled by semi-custom flow
     */
    BasicPort input_port(std::string("in" + std::to_string(itrack)), 1);
    BasicPort output_port(std::string("out" + std::to_string(itrack)), 1);
    BasicPort mid_output_port(std::string("mid_out" + std::to_string(itrack)), 1);
    print_verilog_wire_connection(fp, output_port, input_port, false);
    print_verilog_wire_connection(fp, mid_output_port, input_port, false);
  }

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, module_manager.module_name(module_id));

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Close file handler */
  fp.close();

  /* Add fname to the linked list */
  /* Uncomment this when it is ready
  routing_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(routing_verilog_subckt_file_path_head, verilog_fname.c_str());  
   */

  return;
}

static 
void dump_verilog_routing_chan_subckt(char* verilog_dir,
                                      char* subckt_dir,
                                      size_t rr_chan_subckt_id, 
                                      const RRChan& rr_chan) {
  FILE* fp = NULL;
  char* fname = NULL;

  /* Initial chan_prefix*/
  switch (rr_chan.get_type()) {
  case CHANX:
    /* Create file handler */
    fp = verilog_create_one_subckt_file(subckt_dir, "Routing Channel - X direction ", chanx_verilog_file_name_prefix, rr_chan_subckt_id, 0, &fname);
    /* Print preprocessing flags */
    verilog_include_defines_preproc_file(fp, verilog_dir);
    /* Comment lines */
    fprintf(fp, "//----- Verilog Module of Channel X [%lu] -----\n", rr_chan_subckt_id);
    break;
  case CHANY:
    /* Create file handler */
    fp = verilog_create_one_subckt_file(subckt_dir, "Routing Channel - Y direction ", chany_verilog_file_name_prefix, rr_chan_subckt_id, 0, &fname);
    /* Print preprocessing flags */
    verilog_include_defines_preproc_file(fp, verilog_dir);
    /* Comment lines */
    fprintf(fp, "//----- Verilog Module Channel Y [%lu] -----\n", rr_chan_subckt_id);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d])Invalid Channel type! Should be CHANX or CHANY.\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Chan subckt definition */
  fprintf(fp, "module %s ( \n", 
          gen_verilog_one_routing_channel_module_name(rr_chan.get_type(), rr_chan_subckt_id, -1));
  fprintf(fp, "\n");
  /* dump global ports */
  /*
  if (0 < dump_verilog_global_ports(fp, global_ports_head, TRUE, false)) {
    fprintf(fp, ",\n");
  }
  */
  /* Inputs and outputs,
   * Rules for CHANX:
   * print left-hand ports(in) first, then right-hand ports(out)
   * Rules for CHANX:
   * print bottom ports(in) first, then top ports(out)
   */
  for (size_t itrack = 0; itrack < rr_chan.get_chan_width(); ++itrack) {
    switch (rr_chan.get_node(itrack)->direction) {
    case INC_DIRECTION:
      fprintf(fp, "  input in%lu, //--- track %lu input \n", itrack, itrack);
      break;
    case DEC_DIRECTION:
      fprintf(fp, "  output out%lu, //--- track %lu output \n", itrack, itrack);
      break;
    case BI_DIRECTION:
    default:
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File: %s [LINE%d]) Invalid direction of rr_node %s[%lu]_in/out[%lu]!\n",
                 __FILE__, __LINE__, 
                 convert_chan_type_to_string(rr_chan.get_type()),
                 rr_chan_subckt_id, itrack);
      exit(1);
    }
  }
  for (size_t itrack = 0; itrack < rr_chan.get_chan_width(); ++itrack) {
    switch (rr_chan.get_node(itrack)->direction) {
    case INC_DIRECTION:
      fprintf(fp, "  output out%lu, //--- track %lu output\n", itrack, itrack);
      break;
    case DEC_DIRECTION:
      fprintf(fp, "  input in%lu, //--- track %lu input \n", itrack, itrack);
      break;
    case BI_DIRECTION:
    default:
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File: %s [LINE%d]) Invalid direction of rr_node %s[%lu]_in/out[%lu]!\n",
                 __FILE__, __LINE__, 
                 convert_chan_type_to_string(rr_chan.get_type()),
                 rr_chan_subckt_id, itrack);
      exit(1);
    }
  }
  /* Middle point output for connection box inputs */
  for (size_t itrack = 0; itrack < rr_chan.get_chan_width(); ++itrack) {
    fprintf(fp, "  output mid_out%lu", itrack);
    if (itrack < (rr_chan.get_chan_width() - 1)) {
      fprintf(fp, ",");
    }
    fprintf(fp, " // Middle output %lu to logic blocks \n", itrack);
  }
  fprintf(fp, "  );\n");

  /* Print segments models*/
  for (size_t itrack = 0; itrack < rr_chan.get_chan_width(); ++itrack) {
    /* short connecting inputs and outputs: 
     * length of metal wire and parasitics are handled by semi-custom flow
     */
    fprintf(fp, "assign out%lu = in%lu; \n", itrack, itrack); 
    fprintf(fp, "assign mid_out%lu = in%lu; \n", itrack, itrack);
  }

  fprintf(fp, "endmodule\n");

  /* Comment lines */
  fprintf(fp, 
          "//----- END Verilog Module of %s [%lu] -----\n\n", 
          convert_chan_type_to_string(rr_chan.get_type()),
          rr_chan_subckt_id);

  /* Close file handler */
  fclose(fp);

  /* Add fname to the linked list */
  routing_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(routing_verilog_subckt_file_path_head, fname);  

  /* Free */
  my_free(fname);

  return;
}

static 
void dump_verilog_routing_chan_subckt(char* verilog_dir,
                                      char* subckt_dir,
                                      int x,  int y,
                                      t_rr_type chan_type, 
                                      int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                      t_ivec*** LL_rr_node_indices,
                                      t_rr_indexed_data* LL_rr_indexed_data,
                                      int num_segment) {
  int itrack, iseg, cost_index;
  int chan_width = 0;
  t_rr_node** chan_rr_nodes = NULL;
  FILE* fp = NULL;
  char* fname = NULL;

  /* Check */
  assert((!(0 > x))&&(!(x > (nx + 1)))); 
  assert((!(0 > y))&&(!(y > (ny + 1)))); 
  assert((CHANX == chan_type)||(CHANY == chan_type));

  /* Initial chan_prefix*/
  switch (chan_type) {
  case CHANX:
    /* Create file handler */
    fp = verilog_create_one_subckt_file(subckt_dir, "Routing Channel - X direction ", chanx_verilog_file_name_prefix, x, y, &fname);
    /* Print preprocessing flags */
    verilog_include_defines_preproc_file(fp, verilog_dir);
    /* Comment lines */
    fprintf(fp, "//----- Verilog Module of Channel X [%d][%d] -----\n", x, y);
    break;
  case CHANY:
    /* Create file handler */
    fp = verilog_create_one_subckt_file(subckt_dir, "Routing Channel - Y direction ", chany_verilog_file_name_prefix, x, y, &fname);
    /* Print preprocessing flags */
    verilog_include_defines_preproc_file(fp, verilog_dir);
    /* Comment lines */
    fprintf(fp, "//----- Verilog Module Channel Y [%d][%d] -----\n", x, y);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid Channel type! Should be CHANX or CHANY.\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Collect rr_nodes for Tracks for chanx[ix][iy] */
  chan_rr_nodes = get_chan_rr_nodes(&chan_width, chan_type, x, y,
                                    LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices);

  /* Chan subckt definition */
  fprintf(fp, "module %s ( \n", 
          gen_verilog_one_routing_channel_module_name(chan_type, x, y));
  fprintf(fp, "\n");
  /* dump global ports */
  /*
  if (0 < dump_verilog_global_ports(fp, global_ports_head, TRUE, false)) {
    fprintf(fp, ",\n");
  }
   */
  /* Inputs and outputs,
   * Rules for CHANX:
   * print left-hand ports(in) first, then right-hand ports(out)
   * Rules for CHANX:
   * print bottom ports(in) first, then top ports(out)
   */
  for (itrack = 0; itrack < chan_width; itrack++) {
    switch (chan_rr_nodes[itrack]->direction) {
    case INC_DIRECTION:
      fprintf(fp, "  input in%d, //--- track %d input \n", itrack, itrack);
      break;
    case DEC_DIRECTION:
      fprintf(fp, "  output out%d, //--- track %d output \n", itrack, itrack);
      break;
    case BI_DIRECTION:
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of rr_node chany[%d][%d]_in/out[%d]!\n",
                 __FILE__, __LINE__, x, y + 1, itrack);
      exit(1);
    }
  }
  for (itrack = 0; itrack < chan_width; itrack++) {
    switch (chan_rr_nodes[itrack]->direction) {
    case INC_DIRECTION:
      fprintf(fp, "  output out%d, //--- track %d output\n", itrack, itrack);
      break;
    case DEC_DIRECTION:
      fprintf(fp, "  input in%d, //--- track %d input \n", itrack, itrack);
      break;
    case BI_DIRECTION:
    default:
      vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of rr_node chany[%d][%d]_in/out[%d]!\n",
                 __FILE__, __LINE__, x, y + 1, itrack);
      exit(1);
    }
  }
  /* Middle point output for connection box inputs */
  for (itrack = 0; itrack < chan_width; itrack++) {
    fprintf(fp, "  output mid_out%d", itrack);
    if (itrack < (chan_width - 1)) {
      fprintf(fp, ",");
    }
    fprintf(fp, " // Middle output %d to logic blocks \n", itrack);
  }
  fprintf(fp, "  );\n");

  /* Print segments models*/
  for (itrack = 0; itrack < chan_width; itrack++) {
    cost_index = chan_rr_nodes[itrack]->cost_index;
    iseg = LL_rr_indexed_data[cost_index].seg_index; 
    /* Check */
    assert((!(iseg < 0))&&(iseg < num_segment));
    /* short connecting inputs and outputs: 
     * length of metal wire and parasitics are handled by semi-custom flow
     */
    fprintf(fp, "assign out%d = in%d; \n", itrack, itrack); 
    fprintf(fp, "assign mid_out%d = in%d; \n", itrack, itrack);
  }

  fprintf(fp, "endmodule\n");

  /* Comment lines */
  switch (chan_type) {
  case CHANX:
    /* Comment lines */
    fprintf(fp, "//----- END Verilog Module of Channel X [%d][%d] -----\n\n", x, y);
    break;
  case CHANY:
    /* Comment lines */
    fprintf(fp, "//----- END Verilog Module of Channel Y [%d][%d] -----\n\n", x, y);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid Channel type! Should be CHANX or CHANY.\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Close file handler */
  fclose(fp);

  /* Add fname to the linked list */
  routing_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(routing_verilog_subckt_file_path_head, fname);  

  /* Free */
  my_free(chan_rr_nodes);
  my_free(fname);

  return;
}

void dump_verilog_grid_side_pin_with_given_index(FILE* fp, t_rr_type pin_type, 
                                                 int pin_index, int side,
                                                 int x, int y,
                                                 int unique_x, int unique_y, /* If explicit, needs the coordinates of the mirror*/
                                                 boolean dump_port_type,
                                                 bool is_explicit_mapping) {
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
      is_explicit_mapping = false; /* Both cannot be true at the same time */
    }
    if (true == is_explicit_mapping) {
      fprintf(fp, ".%s(", gen_verilog_grid_one_pin_name(unique_x, unique_y, height, side, pin_index, TRUE));
    }
    fprintf(fp, "%s", gen_verilog_grid_one_pin_name(x, y, height, side, pin_index, TRUE));
    if (true == is_explicit_mapping) {
      fprintf(fp, ")");
    }
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

void dump_verilog_grid_side_pins(FILE* fp,
                           t_rr_type pin_type,
                           int x,
                           int y,
                           int side,
                           boolean dump_port_type) {
  int height, ipin, class_id;
  t_type_ptr type = NULL;
  enum e_pin_type pin_class_type;
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
 
  /* Assign the type of PIN*/ 
  switch (pin_type) {
  case IPIN:
  /* case SINK: */
    pin_class_type = RECEIVER; /* This is the end of a route path*/ 
    verilog_port_type = "output";
    break;
  /* case SOURCE: */
  case OPIN:
    pin_class_type = DRIVER; /* This is the start of a route path */ 
    verilog_port_type = "input";
    break;
  /* SINK and SOURCE are hypothesis nodes */
  default:
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid pin_type!\n", __FILE__, __LINE__);
    exit(1); 
  }

  /* Output the pins on the side*/ 
  for (ipin = 0; ipin < type->num_pins; ipin++) {
    class_id = type->pin_class[ipin];
    height = get_grid_pin_height(x, y, ipin);
    if ((1 == type->pinloc[height][side][ipin])&&(pin_class_type == type->class_inf[class_id].type)) {
      if (TRUE == dump_port_type) {
        fprintf(fp, "%s ", verilog_port_type);
      }
      fprintf(fp, " grid_%d__%d__pin_%d__%d__%d_", x, y, height, side, ipin);
      if (TRUE == dump_port_type) {
        fprintf(fp, ",\n");
      }
    }
  } 
  
  return;
}

void dump_verilog_switch_box_chan_port(FILE* fp,
                                       t_sb* cur_sb_info, 
                                       int chan_side,
                                       t_rr_node* cur_rr_node,
                                       enum PORTS cur_rr_node_direction) {
  int index = -1;
  t_rr_type chan_rr_node_type;
  int chan_rr_node_x, chan_rr_node_y;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Get the index in sb_info of cur_rr_node */
  index = get_rr_node_index_in_sb_info(cur_rr_node, (*cur_sb_info), chan_side, cur_rr_node_direction);
  /* Make sure this node is included in this sb_info */
  assert((-1 != index)&&(-1 != chan_side));

  get_chan_rr_node_coordinate_in_sb_info((*cur_sb_info), chan_side, 
                                         &chan_rr_node_type, &chan_rr_node_x, &chan_rr_node_y);

  assert(cur_rr_node->type == chan_rr_node_type);

  fprintf(fp, "%s_%d__%d__%s_%d_ ", 
          convert_chan_type_to_string(chan_rr_node_type),
          chan_rr_node_x, chan_rr_node_y, 
          convert_chan_rr_node_direction_to_string(cur_sb_info->chan_rr_node_direction[chan_side][index]),
          cur_rr_node->ptc_num);

  return;
}

static 
void dump_verilog_unique_switch_box_chan_port(FILE* fp,
                                              const RRGSB& rr_sb, 
                                              enum e_side chan_side,
                                              t_rr_node* cur_rr_node,
                                              enum PORTS cur_rr_node_direction) {
  int index = -1;
  t_rr_type chan_rr_node_type;
  DeviceCoordinator chan_rr_node_coordinator;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Get the index in sb_info of cur_rr_node */
  index = rr_sb.get_node_index(cur_rr_node, chan_side, cur_rr_node_direction);
  /* Make sure this node is included in this sb_info */
  if (!((-1 != index)&&(NUM_SIDES != chan_side)))
  assert((-1 != index)&&(NUM_SIDES != chan_side));

  chan_rr_node_type = cur_rr_node->type;
  chan_rr_node_coordinator = rr_sb.get_side_block_coordinator(chan_side);

  fprintf(fp, "%s_%lu__%lu__%s_%d_ ", 
          convert_chan_type_to_string(chan_rr_node_type),
          chan_rr_node_coordinator.get_x(), chan_rr_node_coordinator.get_y(), 
          convert_chan_rr_node_direction_to_string(cur_rr_node_direction),
          index); /* use node index since ptc_num is no longer unique */

  return;
}


/* Print a short interconneciton in switch box
 * There are two cases should be noticed.
 * 1. The actual fan-in of cur_rr_node is 0. In this case,
      the cur_rr_node need to be short connected to itself which is on the opposite side of this switch
 * 2. The actual fan-in of cur_rr_node is 0. In this case,
 *    The cur_rr_node need to connected to the drive_rr_node
 */
static 
void dump_verilog_unique_switch_box_short_interc(FILE* fp, 
                                                 const RRGSB& rr_sb,
                                                 enum e_side chan_side,
                                                 t_rr_node* cur_rr_node,
                                                 int actual_fan_in,
                                                 t_rr_node* drive_rr_node) {
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((0 == actual_fan_in)||(1 == actual_fan_in));

  char* chan_name = convert_chan_type_to_string(cur_rr_node->type);

  /* Get the index in sb_info of cur_rr_node */
  int index = rr_sb.get_node_index(cur_rr_node, chan_side, OUT_PORT);
  char* des_chan_port_name = "out"; 
  
  fprintf(fp, "//----- Short connection %s[%lu][%lu]_%s[%d] -----\n", 
          chan_name, rr_sb.get_sb_coordinator().get_x(), rr_sb.get_sb_coordinator().get_y(), des_chan_port_name, index);
  fprintf(fp, "assign "); 

  /* Output port */
  dump_verilog_unique_switch_box_chan_port(fp, rr_sb, chan_side, cur_rr_node, OUT_PORT);
  fprintf(fp, " = "); 

  /* Check the driver*/
  if (0 == actual_fan_in) {
    assert(drive_rr_node == cur_rr_node);
  } else {
    assert (1 == actual_fan_in);
  }

  int grid_x = drive_rr_node->xlow; 
  int grid_y = drive_rr_node->ylow; /*Plus the offset in function fprint_grid_side_pin_with_given_index */
  switch (drive_rr_node->type) {
  /* case SOURCE: */
  case OPIN:
    /* Find grid_x and grid_y */
    /* Print a grid pin */
    dump_verilog_grid_side_pin_with_given_index(fp, IPIN, /* this is an input of a Switch Box */
                                                drive_rr_node->ptc_num, 
                                                rr_sb.get_opin_node_grid_side(drive_rr_node),
                                                grid_x, grid_y, 
                                                0, 0, /* No explicit mapping*/
                                                FALSE, false); /* Do not dump the direction of the port! */
    break;
  case CHANX:
  case CHANY:
    enum e_side side;
    /* Should an input */
    if (cur_rr_node == drive_rr_node) {
      /* To be strict, the input should locate on the opposite side. 
       * Use the else part if this may change in some architecture.
       */
      Side side_manager(chan_side);
      side = side_manager.get_opposite(); 
    } else {
      rr_sb.get_node_side_and_index(drive_rr_node, IN_PORT, &side, &index);
      assert ( -1 != index ); 
      assert ( NUM_SIDES != side );
    }
    /* We need to be sure that drive_rr_node is part of the SB */
    dump_verilog_unique_switch_box_chan_port(fp, rr_sb, side, drive_rr_node, IN_PORT);
    break;
  /* SOURCE is invalid as well */
  default: /* IPIN, SINK are invalid*/
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* END */
  fprintf(fp, ";\n");

  return;
}


/* Print a short interconneciton in switch box
 * There are two cases should be noticed.
 * 1. The actual fan-in of cur_rr_node is 0. In this case,
      the cur_rr_node need to be short connected to itself which is on the opposite side of this switch
 * 2. The actual fan-in of cur_rr_node is 0. In this case,
 *    The cur_rr_node need to connected to the drive_rr_node
 */
void dump_verilog_switch_box_short_interc(FILE* fp, 
                                          t_sb* cur_sb_info,
                                          int chan_side,
                                          t_rr_node* cur_rr_node,
                                          int actual_fan_in,
                                          t_rr_node* drive_rr_node,
                                          bool is_explicit_mapping) {
  int side, index; 
  int grid_x, grid_y;
  char* chan_name = NULL;
  char* des_chan_port_name = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cur_sb_info->x))&&(!(cur_sb_info->x > (nx + 1)))); 
  assert((!(0 > cur_sb_info->y))&&(!(cur_sb_info->y > (ny + 1)))); 
  assert((0 == actual_fan_in)||(1 == actual_fan_in));

  chan_name = convert_chan_type_to_string(cur_rr_node->type);

  /* Get the index in sb_info of cur_rr_node */
  index = get_rr_node_index_in_sb_info(cur_rr_node, (*cur_sb_info), chan_side, OUT_PORT);
  des_chan_port_name = "out"; 
  
  fprintf(fp, "//----- Short connection %s[%d][%d]_%s[%d] -----\n", 
          chan_name, cur_sb_info->x, cur_sb_info->y, des_chan_port_name, cur_rr_node->ptc_num);
  fprintf(fp, "assign "); 

  /* Output port */
  dump_verilog_switch_box_chan_port(fp, cur_sb_info, chan_side, cur_rr_node, OUT_PORT);
  fprintf(fp, " = "); 

  /* Check the driver*/
  if (0 == actual_fan_in) {
    assert(drive_rr_node == cur_rr_node);
  } else {
    /* drive_rr_node = &(rr_node[cur_rr_node->prev_node]); */
    assert(1 == rr_node_drive_switch_box(drive_rr_node, cur_rr_node, cur_sb_info->x, cur_sb_info->y, chan_side));
  }
  switch (drive_rr_node->type) {
  /* case SOURCE: */
  case OPIN:
    /* Indicate a CLB Outpin*/
    /* Search all the sides of a SB, see this drive_rr_node is an INPUT of this SB */
    get_rr_node_side_and_index_in_sb_info(drive_rr_node, (*cur_sb_info), IN_PORT, &side, &index);
    /* We need to be sure that drive_rr_node is part of the SB */
    assert((-1 != index)&&(-1 != side));
    /* Find grid_x and grid_y */
    grid_x = drive_rr_node->xlow; 
    grid_y = drive_rr_node->ylow; /*Plus the offset in function fprint_grid_side_pin_with_given_index */
    /* Print a grid pin */
    dump_verilog_grid_side_pin_with_given_index(fp, IPIN, /* this is an input of a Switch Box */
                                                drive_rr_node->ptc_num, 
                                                cur_sb_info->opin_rr_node_grid_side[side][index],
                                                grid_x, grid_y, 
                                                0, /*Used in more recent version*/
                                                0, /*Used in more recent version*/
                                                FALSE, is_explicit_mapping); /* Do not dump the direction of the port! */
    break;
  case CHANX:
  case CHANY:
    /* Should an input */
    if (cur_rr_node == drive_rr_node) {
      /* To be strict, the input should locate on the opposite side. 
       * Use the else part if this may change in some architecture.
       */
      side = get_opposite_side(chan_side); 
      index = get_rr_node_index_in_sb_info(drive_rr_node, (*cur_sb_info), side, IN_PORT);
    } else {
      get_rr_node_side_and_index_in_sb_info(drive_rr_node, (*cur_sb_info), IN_PORT, &side, &index);
    }
    /* We need to be sure that drive_rr_node is part of the SB */
    assert((-1 != index)&&(-1 != side));
    dump_verilog_switch_box_chan_port(fp, cur_sb_info, side, drive_rr_node, IN_PORT);
    break;
  /* SOURCE is invalid as well */
  default: /* IPIN, SINK are invalid*/
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* END */
  fprintf(fp, ";\n");

  return;
}

/* Print the SPICE netlist of multiplexer that drive this rr_node */
void dump_verilog_switch_box_mux(t_sram_orgz_info* cur_sram_orgz_info,
                                 FILE* fp, 
                                 t_sb* cur_sb_info, 
                                 int chan_side,
                                 t_rr_node* cur_rr_node,
                                 int mux_size,
                                 t_rr_node** drive_rr_nodes,
                                 int switch_index,
                                 bool is_explicit_mapping) {
  int inode, side, index, input_cnt = 0;
  int grid_x, grid_y;
  t_spice_model* verilog_model = NULL;
  int mux_level, path_id, cur_num_sram;
  int num_mux_sram_bits = 0;
  int* mux_sram_bits = NULL;
  int num_mux_conf_bits = 0;
  int num_mux_reserved_conf_bits = 0;
  int cur_bl, cur_wl;
  t_spice_model* mem_model = NULL;
  char* mem_subckt_name = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cur_sb_info->x))&&(!(cur_sb_info->x > (nx + 1)))); 
  assert((!(0 > cur_sb_info->y))&&(!(cur_sb_info->y > (ny + 1)))); 

  /* Check current rr_node is CHANX or CHANY*/
  assert((CHANX == cur_rr_node->type)||(CHANY == cur_rr_node->type));
  
  /* Allocate drive_rr_nodes according to the fan-in*/
  assert((2 == mux_size)||(2 < mux_size));

  /* Get verilog model*/
  verilog_model = switch_inf[switch_index].spice_model;
  /* Specify the input bus */
  fprintf(fp, "wire [0:%d] %s_size%d_%d_inbus;\n",
          mux_size - 1,
          verilog_model->prefix, mux_size, verilog_model->cnt);
  char* name_mux = (char *) my_malloc(sizeof(char)*(1 
                                                    + strlen(verilog_model->prefix) + 5
                                                    + strlen(my_itoa(mux_size)) + 1 
                                                    + strlen(my_itoa(verilog_model->cnt)) + 5));
  sprintf(name_mux, "/%s_size%d_%d_/in", verilog_model->prefix, mux_size, verilog_model->cnt);
  char* path_hierarchy = (char *) my_malloc(sizeof(char)*(strlen(gen_verilog_one_sb_instance_name(cur_sb_info)))); 
  path_hierarchy = gen_verilog_one_sb_instance_name(cur_sb_info);
  cur_rr_node->name_mux = my_strcat(path_hierarchy,name_mux);
  /* Input ports*/
  /* Connect input ports to bus */
  for (inode = 0; inode < mux_size; inode++) {
    switch (drive_rr_nodes[inode]->type) {
    /* case SOURCE: */
    case OPIN:
      /* Indicate a CLB Outpin*/
      /* Search all the sides of a SB, see this drive_rr_node is an INPUT of this SB */
      get_rr_node_side_and_index_in_sb_info(drive_rr_nodes[inode], (*cur_sb_info), IN_PORT, &side, &index);
      /* We need to be sure that drive_rr_node is part of the SB */
      if (!((-1 != index)&&(-1 != side))) {
      assert((-1 != index)&&(-1 != side));
      }
      /* Find grid_x and grid_y */
      grid_x = drive_rr_nodes[inode]->xlow; 
      grid_y = drive_rr_nodes[inode]->ylow; /*Plus the offset in function fprint_grid_side_pin_with_given_index */
      /* Print a grid pin */
      fprintf(fp, "assign %s_size%d_%d_inbus[%d] = ",
              verilog_model->prefix, mux_size, verilog_model->cnt, input_cnt);
      dump_verilog_grid_side_pin_with_given_index(fp, IPIN, drive_rr_nodes[inode]->ptc_num, 
                                                  cur_sb_info->opin_rr_node_grid_side[side][index],
                                                  grid_x, grid_y,
                                                  0,/*Used in more recent version*/
                                                  0,/*Used in more recent version*/
                                                  FALSE, is_explicit_mapping);
      fprintf(fp, ";\n");
      input_cnt++;
      break;
    case CHANX:
    case CHANY:
      /* Should be an input ! */
      get_rr_node_side_and_index_in_sb_info(drive_rr_nodes[inode], (*cur_sb_info), IN_PORT, &side, &index);
      /* We need to be sure that drive_rr_node is part of the SB */
      assert((-1 != index)&&(-1 != side));
      fprintf(fp, "assign %s_size%d_%d_inbus[%d] = ",
              verilog_model->prefix, mux_size, verilog_model->cnt, input_cnt);
      dump_verilog_switch_box_chan_port(fp, cur_sb_info, side, drive_rr_nodes[inode], IN_PORT);
      fprintf(fp, ";\n");
      input_cnt++;
      break;
    default: /* IPIN, SINK are invalid*/
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }
  assert(input_cnt == mux_size);

  /* Print SRAMs that configure this MUX */
  /* cur_num_sram = sram_verilog_model->cnt; */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl);
  /* connect to reserved BL/WLs ? */
  num_mux_reserved_conf_bits = count_num_reserved_conf_bits_one_spice_model(verilog_model, 
                                                                            cur_sram_orgz_info->type, 
                                                                            mux_size);
  /* Get the number of configuration bits required by this MUX */
  num_mux_conf_bits = count_num_conf_bits_one_spice_model(verilog_model, 
                                                          cur_sram_orgz_info->type, 
                                                          mux_size);

  /* Dump the configuration port bus */
  dump_verilog_mux_config_bus(fp, verilog_model, cur_sram_orgz_info,
                              mux_size, cur_num_sram, num_mux_reserved_conf_bits, num_mux_conf_bits); 

  /* Dump ports visible only during formal verification */
  fprintf(fp, "\n");
  fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
  /*
  dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                              cur_num_sram, 
                                              cur_num_sram + num_mux_conf_bits - 1,
                                              VERILOG_PORT_WIRE);
  fprintf(fp, ";\n");
  */
  dump_verilog_formal_verification_mux_sram_ports_wiring(fp, cur_sram_orgz_info,
                                                         verilog_model, mux_size,
                                                         cur_num_sram, 
                                                         cur_num_sram + num_mux_conf_bits - 1);
  
  fprintf(fp, "`endif\n");
  
  /* Now it is the time print the SPICE netlist of MUX*/
  fprintf(fp, "%s_size%d %s_size%d_%d_ (", 
          verilog_model->prefix, mux_size,
          verilog_model->prefix, mux_size, verilog_model->cnt);

  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, FALSE, FALSE, my_bool_to_boolean(is_explicit_mapping), TRUE)) {
    fprintf(fp, ",\n");
  }
  if (true == is_explicit_mapping) {
    fprintf(fp, ".in(");
  }
  fprintf(fp, "%s_size%d_%d_inbus",
          verilog_model->prefix, mux_size, verilog_model->cnt);
  if (true == is_explicit_mapping) {
    fprintf(fp, ")");
  }
    fprintf(fp, " ,");

  /* Output port */
  if (true == is_explicit_mapping) {
    fprintf(fp, ".out(");
  }
  dump_verilog_switch_box_chan_port(fp, cur_sb_info, chan_side, cur_rr_node, OUT_PORT);
  if (true == is_explicit_mapping) {
    fprintf(fp, ")");
  }
  /* Add a comma because dump_verilog_switch_box_chan_port does not add so  */
  fprintf(fp, ", ");
  
  /* Different design technology requires different configuration bus! */
  dump_verilog_mux_config_bus_ports(fp, verilog_model, cur_sram_orgz_info,
                                    mux_size, cur_num_sram,
                                    num_mux_reserved_conf_bits, num_mux_conf_bits,
                                    is_explicit_mapping);

  fprintf(fp, ");\n");

  /* Configuration bits for this MUX*/
  path_id = DEFAULT_PATH_ID;
  for (inode = 0; inode < mux_size; inode++) {
    if (drive_rr_nodes[inode] == &(rr_node[cur_rr_node->prev_node])) {
      path_id = inode; 
      cur_rr_node->id_path = inode;
      break;
    }
  }

  /* Depend on both technology and structure of this MUX*/
  switch (verilog_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    decode_cmos_mux_sram_bits(verilog_model, mux_size, path_id, &num_mux_sram_bits, &mux_sram_bits, &mux_level);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    decode_rram_mux(verilog_model, mux_size, path_id, &num_mux_sram_bits, &mux_sram_bits, &mux_level);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
               __FILE__, __LINE__, verilog_model->name);
    exit(1);
  }
  
  /* Print the encoding in SPICE netlist for debugging */
  switch (verilog_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    fprintf(fp, "//----- SRAM bits for MUX[%d], level=%d, select_path_id=%d. -----\n", 
            verilog_model->cnt, mux_level, path_id);
    fprintf(fp, "//----- From LSB(LEFT) TO MSB (RIGHT) -----\n");
    fprintf(fp, "//-----");
    fprint_commented_sram_bits(fp, num_mux_sram_bits, mux_sram_bits);
    fprintf(fp, "-----\n");
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    fprintf(fp, "//----- BL/WL bits for 4T1R MUX[%d], level=%d, select_path_id=%d. -----\n", 
            verilog_model->cnt, mux_level, path_id);
    fprintf(fp, "//----- From LSB(LEFT) TO MSB (RIGHT) -----\n");
    fprintf(fp, "//---- BL: ");
    fprint_commented_sram_bits(fp, num_mux_sram_bits/2, mux_sram_bits);
    fprintf(fp, "-----\n");
    fprintf(fp, "//----- From LSB(LEFT) TO MSB (RIGHT) -----\n");
    fprintf(fp, "//---- WL: ");
    fprint_commented_sram_bits(fp, num_mux_sram_bits/2, mux_sram_bits + num_mux_sram_bits/2);
    fprintf(fp, "-----\n");
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
               __FILE__, __LINE__, verilog_model->name);
  }

  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
  /* Dump sram modules */
  switch (verilog_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    /* Call the memory module defined for this SRAM-based MUX! */
    mem_subckt_name = generate_verilog_mux_subckt_name(verilog_model, mux_size, verilog_mem_posfix);
    fprintf(fp, "%s %s_%d_ ( ", 
            mem_subckt_name, mem_subckt_name, verilog_model->cnt);
    dump_verilog_mem_sram_submodule(fp, cur_sram_orgz_info, verilog_model, mux_size, mem_model, 
                                    cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                    my_bool_to_boolean(is_explicit_mapping)); 
    fprintf(fp, ");\n");
    /* update the number of memory bits */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_sram + num_mux_conf_bits);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* RRAM-based MUX does not need any SRAM dumping
     * But we have to get the number of configuration bits required by this MUX 
     * and update the number of memory bits 
     */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_sram + num_mux_conf_bits);
    update_sram_orgz_info_num_blwl(cur_sram_orgz_info, 
                                   cur_bl + num_mux_conf_bits, 
                                   cur_wl + num_mux_conf_bits);
    break;  
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
               __FILE__, __LINE__, verilog_model->name);
  }


  /* update sram counter */
  verilog_model->cnt++;

  /* Free */
  my_free(mux_sram_bits);
  my_free(mem_subckt_name);

  return;
}

/* Print the SPICE netlist of multiplexer that drive this rr_node */
static  
void dump_verilog_unique_switch_box_mux(t_sram_orgz_info* cur_sram_orgz_info,
                                        FILE* fp, 
                                        const RRGSB& rr_sb, 
                                        enum e_side chan_side,
                                        t_rr_node* cur_rr_node,
                                        int mux_size,
                                        t_rr_node** drive_rr_nodes,
                                        int switch_index,
                                        bool is_explicit_mapping) {
  int input_cnt = 0;
  t_spice_model* verilog_model = NULL;
  int mux_level, path_id, cur_num_sram;
  int num_mux_sram_bits = 0;
  int* mux_sram_bits = NULL;
  int num_mux_conf_bits = 0;
  int num_mux_reserved_conf_bits = 0;
  int cur_bl, cur_wl;
  t_spice_model* mem_model = NULL;
  char* mem_subckt_name = NULL;
  int num_input_port, num_output_port;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  /* Check current rr_node is CHANX or CHANY*/
  assert((CHANX == cur_rr_node->type)||(CHANY == cur_rr_node->type));
  
  /* Allocate drive_rr_nodes according to the fan-in*/
  assert((2 == mux_size)||(2 < mux_size));

  /* Get verilog model*/
  verilog_model = switch_inf[switch_index].spice_model;
  /* Specify the input bus */
  fprintf(fp, "wire [0:%d] %s_size%d_%d_inbus;\n",
          mux_size - 1,
          verilog_model->prefix, mux_size, verilog_model->cnt);
  char* name_mux = (char *) my_malloc(sizeof(char)*(1 
                                                    + strlen(verilog_model->prefix) + 5
                                                    + strlen(my_itoa(mux_size)) + 1 
                                                    + strlen(my_itoa(verilog_model->cnt)) + 5));
  sprintf(name_mux, "/%s_size%d_%d_/in", verilog_model->prefix, mux_size, verilog_model->cnt);

  const char* path_hierarchy = rr_sb.gen_sb_verilog_instance_name();
  cur_rr_node->name_mux = my_strcat(path_hierarchy, name_mux);

  /* Input ports*/
  /* Connect input ports to bus */
  for (size_t inode = 0; inode < size_t(mux_size); ++inode) {
    enum e_side side;
    int index;
    int grid_x = drive_rr_nodes[inode]->xlow; 
    int grid_y = drive_rr_nodes[inode]->ylow; /*Plus the offset in function fprint_grid_side_pin_with_given_index */
    switch (drive_rr_nodes[inode]->type) {
    /* case SOURCE: */
    case OPIN:
      /* Indicate a CLB Outpin*/
      /* Find grid_x and grid_y */
      /* Print a grid pin */
      fprintf(fp, "assign %s_size%d_%d_inbus[%d] = ",
              verilog_model->prefix, mux_size, verilog_model->cnt, input_cnt);
      dump_verilog_grid_side_pin_with_given_index(fp, IPIN, drive_rr_nodes[inode]->ptc_num, 
                                                  rr_sb.get_opin_node_grid_side(drive_rr_nodes[inode]),
                                                  grid_x, grid_y,
                                                  0,0,/*No explicit mapping */
                                                  FALSE, false);
      fprintf(fp, ";\n");
      input_cnt++;
      break;
    case CHANX:
    case CHANY:
      /* Should be an input ! */
      rr_sb.get_node_side_and_index(drive_rr_nodes[inode], IN_PORT, &side, &index);
      /* We need to be sure that drive_rr_node is part of the SB */
      assert((-1 != index) && (NUM_SIDES != side));
      fprintf(fp, "assign %s_size%d_%d_inbus[%d] = ",
              verilog_model->prefix, mux_size, verilog_model->cnt, input_cnt);
      dump_verilog_unique_switch_box_chan_port(fp, rr_sb, side, drive_rr_nodes[inode], IN_PORT);
      fprintf(fp, ";\n");
      input_cnt++;
      break;
    default: /* IPIN, SINK are invalid*/
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }
  assert(input_cnt == mux_size);

  /* Print SRAMs that configure this MUX */
  /* cur_num_sram = sram_verilog_model->cnt; */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl);
  /* connect to reserved BL/WLs ? */
  num_mux_reserved_conf_bits = count_num_reserved_conf_bits_one_spice_model(verilog_model, 
                                                                            cur_sram_orgz_info->type, 
                                                                            mux_size);
  /* Get the number of configuration bits required by this MUX */
  num_mux_conf_bits = count_num_conf_bits_one_spice_model(verilog_model, 
                                                          cur_sram_orgz_info->type, 
                                                          mux_size);

  /* Dump the configuration port bus */
  dump_verilog_mux_config_bus(fp, verilog_model, cur_sram_orgz_info,
                              mux_size, cur_num_sram, num_mux_reserved_conf_bits, num_mux_conf_bits); 

  /* Dump ports visible only during formal verification */
  fprintf(fp, "\n");
  fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
  /*
  dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                              cur_num_sram, 
                                              cur_num_sram + num_mux_conf_bits - 1,
                                              VERILOG_PORT_WIRE);
  fprintf(fp, ";\n");
  */
  dump_verilog_formal_verification_mux_sram_ports_wiring(fp, cur_sram_orgz_info,
                                                         verilog_model, mux_size,
                                                         cur_num_sram, 
                                                         cur_num_sram + num_mux_conf_bits - 1);
  
  fprintf(fp, "`endif\n");
  
  /* Now it is the time print the SPICE netlist of MUX*/
  fprintf(fp, "%s_size%d %s_size%d_%d_ (", 
          verilog_model->prefix, mux_size,
          verilog_model->prefix, mux_size, verilog_model->cnt);

  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, FALSE, FALSE, my_bool_to_boolean(is_explicit_mapping), TRUE)) {
    fprintf(fp, ",\n");
  }

    t_spice_model_port** input_port = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_INPUT, &num_input_port, TRUE);
    t_spice_model_port** output_port = find_spice_model_ports(verilog_model, SPICE_MODEL_PORT_OUTPUT, &num_output_port, TRUE);
  if (TRUE == is_explicit_mapping) {
    fprintf(fp, ".%s(",
            input_port[0]->prefix);
    fprintf(fp, "%s_size%d_%d_inbus), ",
            verilog_model->prefix, mux_size, verilog_model->cnt);
  }  
  else {
  fprintf(fp, "%s_size%d_%d_inbus, ",
          verilog_model->prefix, mux_size, verilog_model->cnt);
  }
  /* Output port */
  if (TRUE == is_explicit_mapping) {
    fprintf(fp, ".%s(",
            output_port[0]->prefix);
    dump_verilog_unique_switch_box_chan_port(fp, rr_sb, chan_side, cur_rr_node, OUT_PORT);
    fprintf(fp, ")");
  }
  else {
    dump_verilog_unique_switch_box_chan_port(fp, rr_sb, chan_side, cur_rr_node, OUT_PORT);
  }
  /* Add a comma because dump_verilog_switch_box_chan_port does not add so  */
  fprintf(fp, ", ");
  
  /* Different design technology requires different configuration bus! */
  dump_verilog_mux_config_bus_ports(fp, verilog_model, cur_sram_orgz_info,
                                    mux_size, cur_num_sram, num_mux_reserved_conf_bits, 
                                    num_mux_conf_bits, is_explicit_mapping);

  fprintf(fp, ");\n");

  /* Configuration bits for this MUX*/
  path_id = DEFAULT_PATH_ID;
  for (int inode = 0; inode < mux_size; ++inode) {
    if (drive_rr_nodes[inode] == &(rr_node[cur_rr_node->prev_node])) {
      path_id = inode; 
      cur_rr_node->id_path = inode;
      break;
    }
  }

  /* Depend on both technology and structure of this MUX*/
  switch (verilog_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    decode_cmos_mux_sram_bits(verilog_model, mux_size, path_id, &num_mux_sram_bits, &mux_sram_bits, &mux_level);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    decode_rram_mux(verilog_model, mux_size, path_id, &num_mux_sram_bits, &mux_sram_bits, &mux_level);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
               __FILE__, __LINE__, verilog_model->name);
    exit(1);
  }
  
  /* Print the encoding in SPICE netlist for debugging */
  switch (verilog_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    fprintf(fp, "//----- SRAM bits for MUX[%d], level=%d, select_path_id=%d. -----\n", 
            verilog_model->cnt, mux_level, path_id);
    fprintf(fp, "//----- From LSB(LEFT) TO MSB (RIGHT) -----\n");
    fprintf(fp, "//-----");
    fprint_commented_sram_bits(fp, num_mux_sram_bits, mux_sram_bits);
    fprintf(fp, "-----\n");
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    fprintf(fp, "//----- BL/WL bits for 4T1R MUX[%d], level=%d, select_path_id=%d. -----\n", 
            verilog_model->cnt, mux_level, path_id);
    fprintf(fp, "//----- From LSB(LEFT) TO MSB (RIGHT) -----\n");
    fprintf(fp, "//---- BL: ");
    fprint_commented_sram_bits(fp, num_mux_sram_bits/2, mux_sram_bits);
    fprintf(fp, "-----\n");
    fprintf(fp, "//----- From LSB(LEFT) TO MSB (RIGHT) -----\n");
    fprintf(fp, "//---- WL: ");
    fprint_commented_sram_bits(fp, num_mux_sram_bits/2, mux_sram_bits + num_mux_sram_bits/2);
    fprintf(fp, "-----\n");
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
               __FILE__, __LINE__, verilog_model->name);
  }

  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
  /* Dump sram modules */
  switch (verilog_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    /* Call the memory module defined for this SRAM-based MUX! */
    mem_subckt_name = generate_verilog_mux_subckt_name(verilog_model, mux_size, verilog_mem_posfix);
    fprintf(fp, "%s %s_%d_ ( ", 
            mem_subckt_name, mem_subckt_name, verilog_model->cnt);
    dump_verilog_mem_sram_submodule(fp, cur_sram_orgz_info, 
                                    verilog_model, mux_size, mem_model, 
                                    cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                    is_explicit_mapping); 
    fprintf(fp, ");\n");
    /* update the number of memory bits */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_sram + num_mux_conf_bits);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* RRAM-based MUX does not need any SRAM dumping
     * But we have to get the number of configuration bits required by this MUX 
     * and update the number of memory bits 
     */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_sram + num_mux_conf_bits);
    update_sram_orgz_info_num_blwl(cur_sram_orgz_info, 
                                   cur_bl + num_mux_conf_bits, 
                                   cur_wl + num_mux_conf_bits);
    break;  
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
               __FILE__, __LINE__, verilog_model->name);
  }


  /* update sram counter */
  verilog_model->cnt++;

  /* Free */
  my_free(mux_sram_bits);
  my_free(mem_subckt_name);

  return;
}


/* Count the number of configuration bits of a rr_node*/
int count_verilog_switch_box_interc_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                              t_sb cur_sb_info, int chan_side, 
                                              t_rr_node* cur_rr_node) {
  int num_conf_bits = 0;
  int switch_idx = 0;
  int num_drive_rr_nodes = 0;
  
  if (NULL == cur_rr_node) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])NULL cur_rr_node!\n",
               __FILE__, __LINE__);
    exit(1);    
    return num_conf_bits;
  }

  /* Determine if the interc lies inside a channel wire, that is interc between segments */
  if (1 == is_rr_node_exist_opposite_side_in_sb_info(cur_sb_info, cur_rr_node, chan_side)) {
    num_drive_rr_nodes = 0;
  } else {
    num_drive_rr_nodes = cur_rr_node->num_drive_rr_nodes;
  }

  /* fan_in >= 2 implies a MUX and requires configuration bits */
  if (2 > num_drive_rr_nodes) {
    return num_conf_bits;
  } else {
    switch_idx = cur_rr_node->drive_switches[0];
    assert(-1 < switch_idx);
    assert(SPICE_MODEL_MUX == switch_inf[switch_idx].spice_model->type);
    num_conf_bits = count_num_conf_bits_one_spice_model(switch_inf[switch_idx].spice_model, 
                                                        cur_sram_orgz_info->type, 
                                                        num_drive_rr_nodes);
    return num_conf_bits;
  }
}

/* Count the number of configuration bits of a rr_node*/
static  
size_t count_verilog_switch_box_interc_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                 const RRGSB& rr_sb, enum e_side chan_side, 
                                                 t_rr_node* cur_rr_node) {
  size_t num_conf_bits = 0;
  int switch_idx = 0;
  int num_drive_rr_nodes = 0;
  
  if (NULL == cur_rr_node) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d])NULL cur_rr_node!\n",
               __FILE__, __LINE__);
    exit(1);    
    return num_conf_bits;
  }

  /* Determine if the interc lies inside a channel wire, that is interc between segments */
  if (true == rr_sb.is_sb_node_exist_opposite_side(cur_rr_node, chan_side)) {
    num_drive_rr_nodes = 0;
  } else {
    num_drive_rr_nodes = cur_rr_node->num_drive_rr_nodes;
  }

  /* fan_in >= 2 implies a MUX and requires configuration bits */
  if (2 > num_drive_rr_nodes) {
    return num_conf_bits;
  } else {
    switch_idx = cur_rr_node->drive_switches[0];
    assert(-1 < switch_idx);
    assert(SPICE_MODEL_MUX == switch_inf[switch_idx].spice_model->type);
    num_conf_bits = count_num_conf_bits_one_spice_model(switch_inf[switch_idx].spice_model, 
                                                        cur_sram_orgz_info->type, 
                                                        num_drive_rr_nodes);
    return num_conf_bits;
  }
}

/* Count the number of reserved configuration bits of a rr_node*/
int count_verilog_switch_box_interc_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                       t_sb cur_sb_info, int chan_side, 
                                                       t_rr_node* cur_rr_node) {
  int num_reserved_conf_bits = 0;
  int switch_idx = 0;
  int num_drive_rr_nodes = 0;
  
  if (NULL == cur_rr_node) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])NULL cur_rr_node!\n",
               __FILE__, __LINE__);
    exit(1);    
    return num_reserved_conf_bits;
  }

  /* Determine if the interc lies inside a channel wire, that is interc between segments */
  if (1 == is_rr_node_exist_opposite_side_in_sb_info(cur_sb_info, cur_rr_node, chan_side)) {
    num_drive_rr_nodes = 0;
  } else {
    num_drive_rr_nodes = cur_rr_node->num_drive_rr_nodes;
  }

  /* fan_in >= 2 implies a MUX and requires configuration bits */
  if (2 > num_drive_rr_nodes) {
    return num_reserved_conf_bits;
  } else {
    switch_idx = cur_rr_node->drive_switches[0];
    assert(-1 < switch_idx);
    assert(SPICE_MODEL_MUX == switch_inf[switch_idx].spice_model->type);
    num_reserved_conf_bits = 
                    count_num_reserved_conf_bits_one_spice_model(switch_inf[switch_idx].spice_model, 
                                                                 cur_sram_orgz_info->type,
                                                                 num_drive_rr_nodes);
    return num_reserved_conf_bits;
  }
}

/* Count the number of reserved configuration bits of a rr_node*/
static  
size_t count_verilog_switch_box_interc_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                          const RRGSB& rr_sb, enum e_side chan_side, 
                                                          t_rr_node* cur_rr_node) {
  size_t num_reserved_conf_bits = 0;
  int switch_idx = 0;
  int num_drive_rr_nodes = 0;
  
  if (NULL == cur_rr_node) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])NULL cur_rr_node!\n",
               __FILE__, __LINE__);
    exit(1);    
    return num_reserved_conf_bits;
  }

  /* Determine if the interc lies inside a channel wire, that is interc between segments */
  if (1 == rr_sb.is_sb_node_exist_opposite_side(cur_rr_node, chan_side)) {
    num_drive_rr_nodes = 0;
  } else {
    num_drive_rr_nodes = cur_rr_node->num_drive_rr_nodes;
  }

  /* fan_in >= 2 implies a MUX and requires configuration bits */
  if (2 > num_drive_rr_nodes) {
    return num_reserved_conf_bits;
  } else {
    switch_idx = cur_rr_node->drive_switches[0];
    assert(-1 < switch_idx);
    assert(SPICE_MODEL_MUX == switch_inf[switch_idx].spice_model->type);
    num_reserved_conf_bits = 
                    count_num_reserved_conf_bits_one_spice_model(switch_inf[switch_idx].spice_model, 
                                                                 cur_sram_orgz_info->type,
                                                                 num_drive_rr_nodes);
    return num_reserved_conf_bits;
  }
}

void dump_verilog_switch_box_interc(t_sram_orgz_info* cur_sram_orgz_info,
                                    FILE* fp, 
                                    t_sb* cur_sb_info,
                                    int chan_side,
                                    t_rr_node* cur_rr_node,
                                    bool is_explicit_mapping) {
  int sb_x, sb_y;
  int num_drive_rr_nodes = 0;  
  t_rr_node** drive_rr_nodes = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  sb_x = cur_sb_info->x;
  sb_y = cur_sb_info->y;

  /* Check */
  assert((!(0 > sb_x))&&(!(sb_x > (nx + 1)))); 
  assert((!(0 > sb_y))&&(!(sb_y > (ny + 1)))); 

  /* Determine if the interc lies inside a channel wire, that is interc between segments */
  /* Check each num_drive_rr_nodes, see if they appear in the cur_sb_info */
  if (TRUE == check_drive_rr_node_imply_short(*cur_sb_info, cur_rr_node, chan_side)) {
    /* Double check if the interc lies inside a channel wire, that is interc between segments */
    assert(1 == is_rr_node_exist_opposite_side_in_sb_info(*cur_sb_info, cur_rr_node, chan_side));
    num_drive_rr_nodes = 0;
    drive_rr_nodes = NULL;
  } else {
    num_drive_rr_nodes = cur_rr_node->num_drive_rr_nodes;
    drive_rr_nodes = cur_rr_node->drive_rr_nodes;
  }

  if (0 == num_drive_rr_nodes) {
    /* Print a special direct connection*/
    dump_verilog_switch_box_short_interc(fp, cur_sb_info, chan_side, cur_rr_node, 
                                         num_drive_rr_nodes, cur_rr_node, is_explicit_mapping);
  } else if (1 == num_drive_rr_nodes) {
    /* Print a direct connection*/
    dump_verilog_switch_box_short_interc(fp, cur_sb_info, chan_side, cur_rr_node, 
                                         num_drive_rr_nodes, drive_rr_nodes[DEFAULT_SWITCH_ID],
                                         is_explicit_mapping);
  } else if (1 < num_drive_rr_nodes) {
    /* Print the multiplexer, fan_in >= 2 */
    dump_verilog_switch_box_mux(cur_sram_orgz_info, fp, cur_sb_info, chan_side, cur_rr_node, 
                                num_drive_rr_nodes, drive_rr_nodes, 
                                cur_rr_node->drive_switches[DEFAULT_SWITCH_ID],
                                is_explicit_mapping);
  } /*Nothing should be done else*/ 

  /* Free */

  return;
}

static  
void dump_verilog_unique_switch_box_interc(t_sram_orgz_info* cur_sram_orgz_info,
                                           FILE* fp, 
                                           const RRGSB& rr_sb,
                                           enum e_side chan_side,
                                           size_t chan_node_id,
                                           bool is_explicit_mapping) {
  int num_drive_rr_nodes = 0;  
  t_rr_node** drive_rr_nodes = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Get the node */
  t_rr_node* cur_rr_node = rr_sb.get_chan_node(chan_side, chan_node_id);

  /* Determine if the interc lies inside a channel wire, that is interc between segments */
  /* Check each num_drive_rr_nodes, see if they appear in the cur_sb_info */
  if (true == rr_sb.is_sb_node_passing_wire(chan_side, chan_node_id)) {
    num_drive_rr_nodes = 0;
    drive_rr_nodes = NULL;
  } else {
    num_drive_rr_nodes = cur_rr_node->num_drive_rr_nodes;
    drive_rr_nodes = cur_rr_node->drive_rr_nodes;
    /* Special: if there are zero-driver nodes. We skip here */
    if (0 == num_drive_rr_nodes) {
      return; 
    }
  }

  if (0 == num_drive_rr_nodes) {
    /* Print a special direct connection*/
    dump_verilog_unique_switch_box_short_interc(fp, rr_sb, chan_side, cur_rr_node, 
                                                num_drive_rr_nodes, cur_rr_node);
  } else if (1 == num_drive_rr_nodes) {
    /* Print a direct connection*/
    dump_verilog_unique_switch_box_short_interc(fp, rr_sb, chan_side, cur_rr_node, 
                                                num_drive_rr_nodes, drive_rr_nodes[DEFAULT_SWITCH_ID]);
  } else if (1 < num_drive_rr_nodes) {
    /* Print the multiplexer, fan_in >= 2 */
    dump_verilog_unique_switch_box_mux(cur_sram_orgz_info, fp, rr_sb, chan_side, cur_rr_node, 
                                       num_drive_rr_nodes, drive_rr_nodes, 
                                       cur_rr_node->drive_switches[DEFAULT_SWITCH_ID],
                                       is_explicit_mapping);
  } /*Nothing should be done else*/ 

  /* Free */

  return;
}


/* Count the number of configuration bits of a Switch Box */
static 
int count_verilog_switch_box_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                t_sb* cur_sb_info) {
  int side, itrack;
  int num_reserved_conf_bits = 0;
  int temp_num_reserved_conf_bits = 0;

  for (side = 0; side < cur_sb_info->num_sides; side++) {
    for (itrack = 0; itrack < cur_sb_info->chan_width[side]; itrack++) {
      switch (cur_sb_info->chan_rr_node_direction[side][itrack]) {
      case OUT_PORT:
        temp_num_reserved_conf_bits =
                 count_verilog_switch_box_interc_reserved_conf_bits(cur_sram_orgz_info, *cur_sb_info, side, 
                                                                    cur_sb_info->chan_rr_node[side][itrack]);
        /* Always select the largest number of reserved conf_bits */
        if (temp_num_reserved_conf_bits > num_reserved_conf_bits) {
          num_reserved_conf_bits = temp_num_reserved_conf_bits;
        }
        break;
      case IN_PORT:
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of port sb[%d][%d] Channel node[%d] track[%d]!\n",
                   __FILE__, __LINE__, cur_sb_info->x, cur_sb_info->y, side, itrack);
        exit(1);
      }
    }
  }

  return num_reserved_conf_bits;
}

/* Count the number of configuration bits of a Switch Box */
static 
size_t count_verilog_switch_box_side_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                        const RRGSB& rr_sb, enum e_side side, size_t seg_id) {
  size_t num_reserved_conf_bits = 0;
  size_t temp_num_reserved_conf_bits = 0;
  Side side_manager(side);

  for (size_t itrack = 0; itrack < rr_sb.get_chan_width(side); ++itrack) {
    /* Bypass unwanted segments */
    if (seg_id != rr_sb.get_chan_node_segment(side, itrack)) {
      continue;
    }
    switch (rr_sb.get_chan_node_direction(side, itrack)) {
    case OUT_PORT:
      temp_num_reserved_conf_bits =
               count_verilog_switch_box_interc_reserved_conf_bits(cur_sram_orgz_info, rr_sb, side, 
                                                                  rr_sb.get_chan_node(side, itrack));
      /* Always select the largest number of reserved conf_bits */
      num_reserved_conf_bits = std::max(num_reserved_conf_bits, temp_num_reserved_conf_bits);
      break;
    case IN_PORT:
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File: %s [LINE%d]) Invalid direction of port Channel node[%s] track[%d]!\n",
                 __FILE__, __LINE__, side_manager.c_str(), itrack);
      exit(1);
    }
  }

  return num_reserved_conf_bits;
}


/* Count the number of configuration bits of a Switch Box */
static 
size_t count_verilog_switch_box_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                   const RRGSB& rr_sb) {
  size_t num_reserved_conf_bits = 0;
  size_t temp_num_reserved_conf_bits = 0;

  for (size_t side = 0; side < rr_sb.get_num_sides(); ++side) {
    Side side_manager(side);
    /* get segment ids */
    std::vector<size_t> seg_ids = rr_sb.get_chan(side_manager.get_side()).get_segment_ids();
    for (size_t iseg = 0; iseg < seg_ids.size(); ++iseg) { 
      temp_num_reserved_conf_bits = count_verilog_switch_box_side_reserved_conf_bits(cur_sram_orgz_info, rr_sb, side_manager.get_side(), seg_ids[iseg]);
      /* Always select the largest number of reserved conf_bits */
      num_reserved_conf_bits = std::max(num_reserved_conf_bits, temp_num_reserved_conf_bits);
    }
  }

  return num_reserved_conf_bits;
}


/* Count the number of configuration bits of a Switch Box */
static 
int count_verilog_switch_box_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                       t_sb* cur_sb_info) {
  int side, itrack;
  int num_conf_bits = 0;

  for (side = 0; side < cur_sb_info->num_sides; side++) {
    for (itrack = 0; itrack < cur_sb_info->chan_width[side]; itrack++) {
      switch (cur_sb_info->chan_rr_node_direction[side][itrack]) {
      case OUT_PORT:
        num_conf_bits += count_verilog_switch_box_interc_conf_bits(cur_sram_orgz_info, *cur_sb_info, side, 
                                                                   cur_sb_info->chan_rr_node[side][itrack]);
        break;
      case IN_PORT:
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of port sb[%d][%d] Channel node[%d] track[%d]!\n",
                   __FILE__, __LINE__, cur_sb_info->x, cur_sb_info->y, side, itrack);
        exit(1);
      }
    }
  }

  return num_conf_bits;
}

/* Count the number of configuration bits of a Switch Box */
static 
size_t count_verilog_switch_box_side_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                               const RRGSB& rr_sb, 
                                               enum e_side side, size_t seg_id) {
  size_t num_conf_bits = 0;
  Side side_manager(side);

  for (size_t itrack = 0; itrack < rr_sb.get_chan_width(side); ++itrack) {
    /* Bypass unwanted segments */
    if (seg_id != rr_sb.get_chan_node_segment(side, itrack)) {
      continue;
    }
    switch (rr_sb.get_chan_node_direction(side, itrack)) {
    case OUT_PORT:
      num_conf_bits += count_verilog_switch_box_interc_conf_bits(cur_sram_orgz_info, rr_sb, side, 
                                                                 rr_sb.get_chan_node(side, itrack));
      break;
    case IN_PORT:
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File: %s [LINE%d]) Invalid direction of port Channel node[%s] track[%d]!\n",
                 __FILE__, __LINE__, side_manager.c_str(), itrack);
      exit(1);
    }
  }

  return num_conf_bits;
}

/* Count the number of configuration bits of a Switch Box */
static 
size_t count_verilog_switch_box_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                          const RRGSB& rr_sb) {
  size_t num_conf_bits = 0;

  for (size_t side = 0; side < rr_sb.get_num_sides(); ++side) {
    Side side_manager(side);
    /* get segment ids */
    std::vector<size_t> seg_ids = rr_sb.get_chan(side_manager.get_side()).get_segment_ids();
    for (size_t iseg = 0; iseg < seg_ids.size(); ++iseg) { 
      num_conf_bits += count_verilog_switch_box_side_conf_bits(cur_sram_orgz_info, rr_sb, side_manager.get_side(), seg_ids[iseg]);
    }
  }

  return num_conf_bits;
}

static 
void update_routing_switch_box_conf_bits(t_sram_orgz_info* cur_sram_orgz_info, 
                                         const RRGSB& rr_sb) {
  int cur_num_bl, cur_num_wl;

  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_num_bl, &cur_num_wl); 

  /* Record the index: TODO: clean this mess, move to FPGA_X2P_SETUP !!!*/
  DeviceCoordinator sb_coordinator(rr_sb.get_sb_x(), rr_sb.get_sb_y());

  /* Count the number of configuration bits to be consumed by this Switch block */
  int num_conf_bits = count_verilog_switch_box_conf_bits(cur_sram_orgz_info, rr_sb);
  /* Count the number of reserved configuration bits to be consumed by this Switch block */
  int num_reserved_conf_bits = count_verilog_switch_box_reserved_conf_bits(cur_sram_orgz_info, rr_sb);
  /* Estimate the sram_verilog_model->cnt */
  int cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 

  device_rr_gsb.set_sb_num_reserved_conf_bits(sb_coordinator, num_reserved_conf_bits);
  device_rr_gsb.set_sb_conf_bits_lsb(sb_coordinator, cur_num_sram);
  device_rr_gsb.set_sb_conf_bits_msb(sb_coordinator, cur_num_sram + num_conf_bits - 1);

  /* Update the counter */
  update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_sram + num_conf_bits);
  update_sram_orgz_info_num_blwl(cur_sram_orgz_info, cur_num_bl + num_conf_bits, cur_num_wl + num_conf_bits);

  return;
}

static 
void update_routing_connection_box_conf_bits(t_sram_orgz_info* cur_sram_orgz_info, 
                                             const RRGSB& rr_gsb, t_rr_type cb_type) {
  int cur_num_bl, cur_num_wl;

  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_num_bl, &cur_num_wl); 

  /* Record the index: TODO: clean this mess, move to FPGA_X2P_SETUP !!!*/
  DeviceCoordinator gsb_coordinator(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());

  /* Count the number of configuration bits to be consumed by this Switch block */
  int num_conf_bits = count_verilog_connection_box_conf_bits(cur_sram_orgz_info, rr_gsb, cb_type);
  /* Count the number of reserved configuration bits to be consumed by this Switch block */
  int num_reserved_conf_bits = count_verilog_connection_box_reserved_conf_bits(cur_sram_orgz_info, rr_gsb, cb_type);
  /* Estimate the sram_verilog_model->cnt */
  int cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 

  device_rr_gsb.set_cb_num_reserved_conf_bits(gsb_coordinator, cb_type, num_reserved_conf_bits);
  device_rr_gsb.set_cb_conf_bits_lsb(gsb_coordinator, cb_type, cur_num_sram);
  device_rr_gsb.set_cb_conf_bits_msb(gsb_coordinator, cb_type, cur_num_sram + num_conf_bits - 1);

  /* Update the counter */
  update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_sram + num_conf_bits);
  update_sram_orgz_info_num_blwl(cur_sram_orgz_info, cur_num_bl + num_conf_bits, cur_num_wl + num_conf_bits);

  return;
}
 
/* Task: Print the subckt of a Switch Box.
 * A Switch Box subckt consists of following ports:
 * 1. Channel Y [x][y] inputs 
 * 2. Channel X [x+1][y] inputs
 * 3. Channel Y [x][y-1] outputs
 * 4. Channel X [x][y] outputs
 * 5. Grid[x][y+1] Right side outputs pins
 * 6. Grid[x+1][y+1] Left side output pins
 * 7. Grid[x+1][y+1] Bottom side output pins
 * 8. Grid[x+1][y] Top side output pins
 * 9. Grid[x+1][y] Left side output pins
 * 10. Grid[x][y] Right side output pins
 * 11. Grid[x][y] Top side output pins
 * 12. Grid[x][y+1] Bottom side output pins
 *
 *    --------------          --------------
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |  [x][y+1]  | [x][y+1] | [x+1][y+1] |
 *    |            |          |            |
 *    --------------          --------------
 *                  ----------
 *       ChanX      | Switch |     ChanX 
 *       [x][y]     |   Box  |    [x+1][y]
 *                  | [x][y] |
 *                  ----------
 *    --------------          --------------
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |   [x][y]   |  [x][y]  |  [x+1][y]  |
 *    |            |          |            |
 *    --------------          --------------
 */
static 
void dump_verilog_routing_switch_box_unique_subckt(t_sram_orgz_info* cur_sram_orgz_info,
                                                   char* verilog_dir, char* subckt_dir, 
                                                   const RRGSB& rr_sb,
                                                   bool is_explicit_mapping) {
  FILE* fp = NULL; 
  char* fname = NULL;

  /* Count the number of configuration bits to be consumed by this Switch block */
  int num_conf_bits = count_verilog_switch_box_conf_bits(cur_sram_orgz_info, rr_sb);
  /* Count the number of reserved configuration bits to be consumed by this Switch block */
  int num_reserved_conf_bits = count_verilog_switch_box_reserved_conf_bits(cur_sram_orgz_info, rr_sb);
  /* Estimate the sram_verilog_model->cnt */
  int cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  int esti_sram_cnt = cur_num_sram + num_conf_bits;
  RRGSB rr_gsb = rr_sb; /* IMPORTANT: this copy will be removed when the config ports are initialized when created!!! */
  rr_gsb.set_sb_num_reserved_conf_bits(num_reserved_conf_bits);
  rr_gsb.set_sb_conf_bits_lsb(cur_num_sram);
  rr_gsb.set_sb_conf_bits_msb(cur_num_sram + num_conf_bits - 1);
 
  /* Create file handler */
  fp = verilog_create_one_subckt_file(subckt_dir, "Unique Switch Block ", 
                                      sb_verilog_file_name_prefix, rr_gsb.get_sb_x(), rr_gsb.get_sb_y(), &fname);

  /* Print preprocessing flags */
  verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Comment lines */
  fprintf(fp, "//----- Verilog Module of Unique Switch Box[%lu][%lu] -----\n", rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  /* Print the definition of subckt*/
  fprintf(fp, "module %s ( \n", rr_gsb.gen_sb_verilog_module_name());
  /* dump global ports */
  if (0 < dump_verilog_global_ports(fp, global_ports_head, TRUE, false)) {
    fprintf(fp, ",\n");
  }

  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    /* Print ports  */
    fprintf(fp, "//----- Inputs/outputs of %s side -----\n", side_manager.c_str());
    DeviceCoordinator port_coordinator = rr_gsb.get_side_block_coordinator(side_manager.get_side()); 

    for (size_t itrack = 0; itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      switch (rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
      case OUT_PORT:
        fprintf(fp, "  output %s,\n",
                gen_verilog_routing_channel_one_pin_name(rr_gsb.get_chan_node(side_manager.get_side(), itrack), 
                                                         port_coordinator.get_x(), port_coordinator.get_y(), itrack,
                                                         rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack))); 
        break;
      case IN_PORT:
        fprintf(fp, "  input %s,\n",
                gen_verilog_routing_channel_one_pin_name(rr_gsb.get_chan_node(side_manager.get_side(), itrack), 
                                                         port_coordinator.get_x(), port_coordinator.get_y(), itrack,
                                                         rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack))); 
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File: %s [LINE%d]) Invalid direction of chan[%d][%d]_track[%d]!\n",
                   __FILE__, __LINE__, rr_gsb.get_sb_x(), rr_gsb.get_sb_y(), itrack);
        exit(1);
      }
    }
    /* Dump OPINs of adjacent CLBs */
    for (size_t inode = 0; inode < rr_gsb.get_num_opin_nodes(side_manager.get_side()); ++inode) {
      fprintf(fp, "  ");
      dump_verilog_grid_side_pin_with_given_index(fp, OPIN, /* This is an input of a SB */
                                                  rr_gsb.get_opin_node(side_manager.get_side(), inode)->ptc_num,
                                                  rr_gsb.get_opin_node_grid_side(side_manager.get_side(), inode),
                                                  rr_gsb.get_opin_node(side_manager.get_side(), inode)->xlow,
                                                  rr_gsb.get_opin_node(side_manager.get_side(), inode)->ylow,
                                                  0,/*used in more recent version*/
                                                  0,/*used in more recent version*/
                                                  TRUE, is_explicit_mapping); /* Dump the direction of the port ! */ 
    } 
  }
  
  /* Put down configuration port */
  /* output of each configuration bit */
  /* Reserved sram ports */
  if (0 < rr_gsb.get_sb_num_reserved_conf_bits()) {
    dump_verilog_reserved_sram_ports(fp, cur_sram_orgz_info, 
                                     rr_gsb.get_sb_reserved_conf_bits_lsb(),
                                     rr_gsb.get_sb_reserved_conf_bits_msb(),
                                     VERILOG_PORT_INPUT);
    fprintf(fp, ",\n");
  }
  /* Normal sram ports */
  dump_verilog_sram_ports(fp, cur_sram_orgz_info, 
                          rr_gsb.get_sb_conf_bits_lsb(),
                          rr_gsb.get_sb_conf_bits_msb(),
                          VERILOG_PORT_INPUT);

  /* Dump ports only visible during formal verification*/
  if (0 < rr_gsb.get_sb_num_conf_bits()) {
    fprintf(fp, "\n");
    fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
    fprintf(fp, ",\n");
    dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                                rr_gsb.get_sb_conf_bits_lsb(),
                                                rr_gsb.get_sb_conf_bits_msb(),
                                                VERILOG_PORT_INPUT, false);
    fprintf(fp, "\n");
    fprintf(fp, "`endif\n");
  }
  fprintf(fp, "); \n");

  /* Local wires for memory configurations */
  dump_verilog_sram_config_bus_internal_wires(fp, cur_sram_orgz_info, 
                                              rr_gsb.get_sb_conf_bits_lsb(),
                                              rr_gsb.get_sb_conf_bits_msb());

  /* Put down all the multiplexers */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    fprintf(fp, "//----- %s side Multiplexers -----\n", 
            side_manager.c_str());
    for (size_t itrack = 0; itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      assert((CHANX == rr_gsb.get_chan_node(side_manager.get_side(), itrack)->type)
           ||(CHANY == rr_gsb.get_chan_node(side_manager.get_side(), itrack)->type));
      /* We care INC_DIRECTION tracks at this side*/
      if (OUT_PORT == rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        dump_verilog_unique_switch_box_interc(cur_sram_orgz_info, fp, rr_sb, 
                                              side_manager.get_side(), 
                                              itrack, is_explicit_mapping);
      } 
    }
  }
 
  fprintf(fp, "endmodule\n");

  /* Comment lines */
  fprintf(fp, "//----- END Verilog Module of Switch Box[%lu][%lu] -----\n\n", rr_gsb.get_sb_x(), rr_gsb.get_sb_y());

  /* Check */
  assert(esti_sram_cnt == get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info));

  /* Close file handler */
  fclose(fp);

  /* Add fname to the linked list */
  routing_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(routing_verilog_subckt_file_path_head, fname);  

  /* Free chan_rr_nodes */
  my_free(fname);

  return;
}


/* Task: Print the subckt of a Switch Box.
 * A Switch Box subckt consists of following ports:
 * 1. Channel Y [x][y] inputs 
 * 2. Channel X [x+1][y] inputs
 * 3. Channel Y [x][y-1] outputs
 * 4. Channel X [x][y] outputs
 * 5. Grid[x][y+1] Right side outputs pins
 * 6. Grid[x+1][y+1] Left side output pins
 * 7. Grid[x+1][y+1] Bottom side output pins
 * 8. Grid[x+1][y] Top side output pins
 * 9. Grid[x+1][y] Left side output pins
 * 10. Grid[x][y] Right side output pins
 * 11. Grid[x][y] Top side output pins
 * 12. Grid[x][y+1] Bottom side output pins
 *
 *    --------------          --------------
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |  [x][y+1]  | [x][y+1] | [x+1][y+1] |
 *    |            |          |            |
 *    --------------          --------------
 *                  ----------
 *       ChanX      | Switch |     ChanX 
 *       [x][y]     |   Box  |    [x+1][y]
 *                  | [x][y] |
 *                  ----------
 *    --------------          --------------
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |   [x][y]   |  [x][y]  |  [x+1][y]  |
 *    |            |          |            |
 *    --------------          --------------
 */
static 
void dump_verilog_routing_switch_box_subckt(t_sram_orgz_info* cur_sram_orgz_info,
                                            char* verilog_dir, char* subckt_dir, 
                                            t_sb* cur_sb_info,
                                            boolean compact_routing_hierarchy,
                                            bool is_explicit_mapping) {
  int itrack, inode, side, ix, iy, x, y;
  int cur_num_sram, num_conf_bits, num_reserved_conf_bits, esti_sram_cnt;
  FILE* fp = NULL; 
  char* fname = NULL;

  /* Check */
  assert((!(0 > cur_sb_info->x))&&(!(cur_sb_info->x > (nx + 1)))); 
  assert((!(0 > cur_sb_info->y))&&(!(cur_sb_info->y > (ny + 1)))); 

  x = cur_sb_info->x;
  y = cur_sb_info->y;

  /* Count the number of configuration bits to be consumed by this Switch block */
  num_conf_bits = count_verilog_switch_box_conf_bits(cur_sram_orgz_info, cur_sb_info);
  /* Count the number of reserved configuration bits to be consumed by this Switch block */
  num_reserved_conf_bits = count_verilog_switch_box_reserved_conf_bits(cur_sram_orgz_info, cur_sb_info);
  /* Estimate the sram_verilog_model->cnt */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  esti_sram_cnt = cur_num_sram + num_conf_bits;
  /* Record the index */
  cur_sb_info->num_reserved_conf_bits = num_reserved_conf_bits;
  cur_sb_info->conf_bits_lsb = cur_num_sram; 
  cur_sb_info->conf_bits_msb = cur_num_sram + num_conf_bits;
 
  /* Handle mirror switch blocks:
   * For mirrors, no need to output a file   
   * Just update the counter 
   */
  if (  (TRUE == compact_routing_hierarchy)
     && (NULL != cur_sb_info->mirror) ) {
    /* Again ensure the conf_bits should match !!! */
    /* Count the number  of configuration bits of the mirror */
    int mirror_num_conf_bits = count_verilog_switch_box_conf_bits(cur_sram_orgz_info, cur_sb_info->mirror);
    assert( mirror_num_conf_bits == num_conf_bits );
    /* update memory bits return directly */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_sb_info->conf_bits_msb);
    return;
  }

  /* Create file handler */
  fp = verilog_create_one_subckt_file(subckt_dir, "Switch Block ", sb_verilog_file_name_prefix, cur_sb_info->x, cur_sb_info->y, &fname);

  /* Print preprocessing flags */
  verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Comment lines */
  fprintf(fp, "//----- Verilog Module of Switch Box[%d][%d] -----\n", cur_sb_info->x, cur_sb_info->y);
  /* Print the definition of subckt*/
  fprintf(fp, "module %s ( \n", gen_verilog_one_sb_module_name(cur_sb_info));
  /* dump global ports */
  if (0 < dump_verilog_global_ports(fp, global_ports_head, TRUE, is_explicit_mapping)) {
    fprintf(fp, ",\n");
  }

  for (side = 0; side < cur_sb_info->num_sides; side++) {
    fprintf(fp, "//----- Inputs/outputs of %s side -----\n",convert_side_index_to_string(side));
    determine_sb_port_coordinator((*cur_sb_info), side, &ix, &iy); 

    for (itrack = 0; itrack < cur_sb_info->chan_width[side]; itrack++) {
      switch (cur_sb_info->chan_rr_node_direction[side][itrack]) {
      case OUT_PORT:
        fprintf(fp, "  output %s,\n",
                gen_verilog_routing_channel_one_pin_name(cur_sb_info->chan_rr_node[side][itrack], 
                                                         ix, iy, itrack,
                                                         cur_sb_info->chan_rr_node_direction[side][itrack])); 
        break;
      case IN_PORT:
        fprintf(fp, "  input %s,\n",
                gen_verilog_routing_channel_one_pin_name(cur_sb_info->chan_rr_node[side][itrack], 
                                                         ix, iy, itrack,
                                                         cur_sb_info->chan_rr_node_direction[side][itrack])); 
        break;
      default:
        vpr_printf(TIO_MESSAGE_ERROR, "(File: %s [LINE%d]) Invalid direction of chany[%d][%d]_track[%d]!\n",
                   __FILE__, __LINE__, x, y + 1, itrack);
        exit(1);
      }
    }
    /* Dump OPINs of adjacent CLBs */
    for (inode = 0; inode < cur_sb_info->num_opin_rr_nodes[side]; inode++) {
      fprintf(fp, "  ");
      dump_verilog_grid_side_pin_with_given_index(fp, OPIN, /* This is an input of a SB */
                                                  cur_sb_info->opin_rr_node[side][inode]->ptc_num,
                                                  cur_sb_info->opin_rr_node_grid_side[side][inode],
                                                  cur_sb_info->opin_rr_node[side][inode]->xlow,
                                                  cur_sb_info->opin_rr_node[side][inode]->ylow, 
                                                  cur_sb_info->opin_rr_node[side][inode]->xlow,
                                                  cur_sb_info->opin_rr_node[side][inode]->ylow, 
                                                  TRUE, is_explicit_mapping); /* Dump the direction of the port ! */ 
    } 
  }
  
  /* Put down configuration port */
  /* output of each configuration bit */
  /* Reserved sram ports */
  dump_verilog_reserved_sram_ports(fp, cur_sram_orgz_info, 
                                   0, cur_sb_info->num_reserved_conf_bits - 1,
                                   VERILOG_PORT_INPUT);
  if (0 < cur_sb_info->num_reserved_conf_bits) {
    fprintf(fp, ",\n");
  }
  /* Normal sram ports */
  dump_verilog_sram_ports(fp, cur_sram_orgz_info, 
                          cur_sb_info->conf_bits_lsb, 
                          cur_sb_info->conf_bits_msb - 1,
                          VERILOG_PORT_INPUT);

  /* Dump ports only visible during formal verification*/
  if (0 < (cur_sb_info->conf_bits_msb - 1 - cur_sb_info->conf_bits_lsb)) {
    fprintf(fp, "\n");
    fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
    fprintf(fp, ",\n");
    dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                                cur_sb_info->conf_bits_lsb, 
                                                cur_sb_info->conf_bits_msb - 1,
                                                VERILOG_PORT_INPUT, is_explicit_mapping);
    fprintf(fp, "\n");
    fprintf(fp, "`endif\n");
  }
  fprintf(fp, "); \n");

  /* Local wires for memory configurations */
  dump_verilog_sram_config_bus_internal_wires(fp, cur_sram_orgz_info, 
                                              cur_sb_info->conf_bits_lsb, cur_sb_info->conf_bits_msb - 1);

  /* Put down all the multiplexers */
  for (side = 0; side < cur_sb_info->num_sides; side++) {
    fprintf(fp, "//----- %s side Multiplexers -----\n", 
            convert_side_index_to_string(side));
    for (itrack = 0; itrack < cur_sb_info->chan_width[side]; itrack++) {
      assert((CHANX == cur_sb_info->chan_rr_node[side][itrack]->type)
           ||(CHANY == cur_sb_info->chan_rr_node[side][itrack]->type));
      /* We care INC_DIRECTION tracks at this side*/
      if (OUT_PORT == cur_sb_info->chan_rr_node_direction[side][itrack]) {
        dump_verilog_switch_box_interc(cur_sram_orgz_info, fp, cur_sb_info, side, 
                                       cur_sb_info->chan_rr_node[side][itrack], 
                                       is_explicit_mapping);
      } 
    }
  }
 
  fprintf(fp, "endmodule\n");

  /* Comment lines */
  fprintf(fp, "//----- END Verilog Module of Switch Box[%d][%d] -----\n\n", x, y);

  /* Check */
  assert(esti_sram_cnt == get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info));

  /* Close file handler */
  fclose(fp);

  /* Add fname to the linked list */
  routing_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(routing_verilog_subckt_file_path_head, fname);  

  /* Free chan_rr_nodes */
  my_free(fname);

  return;
}

/* Count the number of configuration bits of a rr_node*/
int count_verilog_connection_box_interc_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                  t_rr_node* cur_rr_node) {
  int num_conf_bits = 0;
  int switch_idx = 0;
  int num_drive_rr_nodes = cur_rr_node->num_drive_rr_nodes;
  
  if (NULL == cur_rr_node) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])NULL cur_rr_node!\n",
               __FILE__, __LINE__);
    exit(1);    
    return num_conf_bits;
  }

  /* fan_in >= 2 implies a MUX and requires configuration bits */
  if (2 > num_drive_rr_nodes) {
    return num_conf_bits;
  } else {
    switch_idx = cur_rr_node->drive_switches[0];
    assert(-1 < switch_idx);
    assert(SPICE_MODEL_MUX == switch_inf[switch_idx].spice_model->type);
    num_conf_bits = count_num_conf_bits_one_spice_model(switch_inf[switch_idx].spice_model, 
                                                        cur_sram_orgz_info->type,
                                                        num_drive_rr_nodes);
    return num_conf_bits;
  }
}

/* Count the number of configuration bits of a rr_node*/
int count_verilog_connection_box_interc_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                           t_rr_node* cur_rr_node) {
  int num_reserved_conf_bits = 0;
  int switch_idx = 0;
  int num_drive_rr_nodes = cur_rr_node->num_drive_rr_nodes;
  
  if (NULL == cur_rr_node) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])NULL cur_rr_node!\n",
               __FILE__, __LINE__);
    exit(1);    
    return num_reserved_conf_bits;
  }

  /* fan_in >= 2 implies a MUX and requires configuration bits */
  if (2 > num_drive_rr_nodes) {
    return num_reserved_conf_bits;
  } else {
    switch_idx = cur_rr_node->drive_switches[0];
    assert(-1 < switch_idx);
    assert(SPICE_MODEL_MUX == switch_inf[switch_idx].spice_model->type);
    num_reserved_conf_bits = 
           count_num_reserved_conf_bits_one_spice_model(switch_inf[switch_idx].spice_model, 
                                                        cur_sram_orgz_info->type,
                                                        num_drive_rr_nodes);
    return num_reserved_conf_bits;
  }
}


int count_verilog_connection_box_one_side_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                    const RRGSB& rr_gsb, enum e_side cb_side) {
  int num_conf_bits = 0;
  for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_side); ++inode) {
    num_conf_bits += count_verilog_connection_box_interc_conf_bits(cur_sram_orgz_info, rr_gsb.get_ipin_node(cb_side, inode));
  }

  return num_conf_bits;
}

int count_verilog_connection_box_one_side_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                    int num_ipin_rr_nodes,
                                                    t_rr_node** ipin_rr_node) {
  int num_conf_bits = 0;
  int inode;

  for (inode = 0; inode < num_ipin_rr_nodes; inode++) {
    num_conf_bits += count_verilog_connection_box_interc_conf_bits(cur_sram_orgz_info, ipin_rr_node[inode]);
  }     

  return num_conf_bits;
}

int count_verilog_connection_box_one_side_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                             const RRGSB& rr_gsb, enum e_side cb_side) {
  int num_reserved_conf_bits = 0;

  for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_side); ++inode) {
    int temp_num_reserved_conf_bits = count_verilog_connection_box_interc_reserved_conf_bits(cur_sram_orgz_info, rr_gsb.get_ipin_node(cb_side, inode));
    num_reserved_conf_bits = std::max(temp_num_reserved_conf_bits, num_reserved_conf_bits);
  }     

  return num_reserved_conf_bits;
}


int count_verilog_connection_box_one_side_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                             int num_ipin_rr_nodes,
                                                             t_rr_node** ipin_rr_node) {
  int num_reserved_conf_bits = 0;
  int temp_num_reserved_conf_bits = 0;
  int inode;

  for (inode = 0; inode < num_ipin_rr_nodes; inode++) {
    temp_num_reserved_conf_bits = count_verilog_connection_box_interc_reserved_conf_bits(cur_sram_orgz_info, 
                                                                                         ipin_rr_node[inode]);
    if (temp_num_reserved_conf_bits > num_reserved_conf_bits) {
      num_reserved_conf_bits = temp_num_reserved_conf_bits;
    }
  }     

  return num_reserved_conf_bits;
}

/* SRC rr_node is the IPIN of a grid.*/
static 
void dump_verilog_connection_box_short_interc(FILE* fp,
                                              const RRGSB& rr_gsb, t_rr_type cb_type,
                                              t_rr_node* src_rr_node) {
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert(1 == src_rr_node->fan_in);

  /* Check the driver*/
  t_rr_node* drive_rr_node = src_rr_node->drive_rr_nodes[0]; 
  /* We have OPINs since we may have direct connections:
   * These connections should be handled by other functions in the compact_netlist.c 
   * So we just return here for OPINs 
   */
  if (OPIN == drive_rr_node->type) {
    return;
  }

  assert((CHANX == drive_rr_node->type) || (CHANY == drive_rr_node->type));
  int check_flag = 0;
  for (int iedge = 0; iedge < drive_rr_node->num_edges; iedge++) {
    if (src_rr_node == &(rr_node[drive_rr_node->edges[iedge]])) {
      check_flag++;
    }
  }
  assert(1 == check_flag);

  int xlow = src_rr_node->xlow;
  int ylow = src_rr_node->ylow;
  int height = grid[xlow][ylow].offset;

  /* Call the zero-resistance model */
  fprintf(fp, "//----- short connection %s[%lu][%lu]_grid[%d][%d]_pin[%d] -----\n", 
          convert_cb_type_to_string(cb_type),
          rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type), 
          xlow, ylow + height, src_rr_node->ptc_num);

  fprintf(fp, "assign ");
  /* output port -- > connect to the output at middle point of a channel */
  int drive_node_index = rr_gsb.get_cb_chan_node_index(cb_type, drive_rr_node);
  assert (-1 != drive_node_index);
  fprintf(fp, "%s ", rr_gsb.gen_cb_verilog_routing_track_name(cb_type, drive_node_index));
  fprintf(fp, "= ");

  /* Input port*/
  assert(IPIN == src_rr_node->type);
  /* Search all the sides of a SB, see this drive_rr_node is an INPUT of this SB */
  enum e_side side = NUM_SIDES;
  int index = -1;
  rr_gsb.get_node_side_and_index(src_rr_node, OUT_PORT, &side, &index);
  /* We need to be sure that drive_rr_node is part of the SB */
  assert((-1 != index)&&(NUM_SIDES != side));
  dump_verilog_grid_side_pin_with_given_index(fp, OPIN, /* This is an output of a Connection Box */
                                              rr_gsb.get_ipin_node(side, index)->ptc_num, 
                                              rr_gsb.get_ipin_node_grid_side(side, index), 
                                              xlow, ylow, /* Coordinator of Grid */ 
                                              0,0, /*No explicit mapping */
                                              FALSE, false); /* Do not specify the direction of this pin */

  /* End */
  fprintf(fp, ";\n");

  return;
}


/* SRC rr_node is the IPIN of a grid.*/
static 
void dump_verilog_connection_box_short_interc(FILE* fp,
                                              t_cb* cur_cb_info,
                                              t_rr_node* src_rr_node) {
  t_rr_node* drive_rr_node = NULL;
  int iedge, check_flag;
  int xlow, ylow, height, side, index;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cur_cb_info->x))&&(!(cur_cb_info->x > (nx + 1)))); 
  assert((!(0 > cur_cb_info->y))&&(!(cur_cb_info->y > (ny + 1)))); 
  assert(1 == src_rr_node->fan_in);

  /* Check the driver*/
  drive_rr_node = src_rr_node->drive_rr_nodes[0]; 
  /* We have OPINs since we may have direct connections:
   * These connections should be handled by other functions in the compact_netlist.c 
   * So we just return here for OPINs 
   */
  if (OPIN == drive_rr_node->type) {
    return;
  }

  assert( (CHANX == drive_rr_node->type) 
       || (CHANY == drive_rr_node->type) ); 
  check_flag = 0;
  for (iedge = 0; iedge < drive_rr_node->num_edges; iedge++) {
    if (src_rr_node == &(rr_node[drive_rr_node->edges[iedge]])) {
      check_flag++;
    }
  }
  assert(1 == check_flag);

  xlow = src_rr_node->xlow;
  ylow = src_rr_node->ylow;
  height = grid[xlow][ylow].offset;

  /* Call the zero-resistance model */
  fprintf(fp, "//----- short connection %s[%d][%d]_grid[%d][%d]_pin[%d] -----\n", 
          convert_cb_type_to_string(cur_cb_info->type), cur_cb_info->x, cur_cb_info->y, xlow, ylow + height, src_rr_node->ptc_num);

  fprintf(fp, "assign ");

  /* output port -- > connect to the output at middle point of a channel */
  fprintf(fp, "%s_%d__%d__midout_%d_ ", 
          convert_chan_type_to_string(drive_rr_node->type),
          cur_cb_info->x, cur_cb_info->y, drive_rr_node->ptc_num);

  fprintf(fp, "= ");

  /* Input port*/
  assert(IPIN == src_rr_node->type);
  /* Search all the sides of a SB, see this drive_rr_node is an INPUT of this SB */
  get_rr_node_side_and_index_in_cb_info(src_rr_node, (*cur_cb_info), OUT_PORT, &side, &index);
  /* We need to be sure that drive_rr_node is part of the SB */
  assert((-1 != index)&&(-1 != side));
  dump_verilog_grid_side_pin_with_given_index(fp, OPIN, /* This is an output of a Connection Box */
                                              cur_cb_info->ipin_rr_node[side][index]->ptc_num, 
                                              cur_cb_info->ipin_rr_node_grid_side[side][index], 
                                              xlow, ylow, /* Coordinator of Grid */ 
                                              0,0, /*No explicit mapping */
                                              FALSE, false); /* Do not specify the direction of this pin */

  /* End */
  fprintf(fp, ";\n");

  return;
}

static 
void dump_verilog_connection_box_mux(t_sram_orgz_info* cur_sram_orgz_info,
                                     FILE* fp,
                                     const RRGSB& rr_gsb, t_rr_type cb_type,
                                     t_rr_node* src_rr_node,
                                     bool is_explicit_mapping) {
  int mux_size, cur_num_sram, input_cnt = 0;
  t_rr_node** drive_rr_nodes = NULL;
  int mux_level, path_id, switch_index;
  t_spice_model* verilog_model = NULL;
  int num_mux_sram_bits = 0;
  int* mux_sram_bits = NULL;
  t_rr_type drive_rr_node_type = NUM_RR_TYPES;
  int xlow, ylow, index;
  enum e_side side;
  int num_mux_conf_bits = 0;
  int num_mux_reserved_conf_bits = 0;
  int cur_bl, cur_wl;
  t_spice_model* mem_model = NULL;
  char* mem_subckt_name = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Find drive_rr_nodes*/
  mux_size = src_rr_node->num_drive_rr_nodes;
  drive_rr_nodes = src_rr_node->drive_rr_nodes; 

  /* Configuration bits for MUX*/
  path_id = DEFAULT_PATH_ID;
  for (int inode = 0; inode < mux_size; ++inode) {
    if (drive_rr_nodes[inode] == &(rr_node[src_rr_node->prev_node])) {
      path_id = inode;
      src_rr_node->id_path = inode;
      break;
    }
  }
  switch_index = src_rr_node->drive_switches[DEFAULT_SWITCH_ID];

  verilog_model = switch_inf[switch_index].spice_model;


  char* name_mux = (char *) my_malloc(sizeof(char)*(1 + strlen(verilog_model->prefix) + 5
                                                    + strlen(my_itoa(mux_size)) + 1 
                                                    + strlen(my_itoa(verilog_model->cnt)) + 5));
  sprintf(name_mux, "/%s_size%d_%d_/in", verilog_model->prefix, mux_size, verilog_model->cnt);
  const char* path_hierarchy = rr_gsb.gen_cb_verilog_instance_name(cb_type);
  src_rr_node->name_mux = my_strcat(path_hierarchy, name_mux);

  /* Specify the input bus */
  fprintf(fp, "wire [0:%d] %s_size%d_%d_inbus;\n",
          mux_size - 1,
          verilog_model->prefix, mux_size, verilog_model->cnt);

  /* Check drive_rr_nodes type, should be the same*/
  for (int inode = 0; inode < mux_size; inode++) {
    if (NUM_RR_TYPES == drive_rr_node_type) { 
      drive_rr_node_type = drive_rr_nodes[inode]->type;
    } else {
      assert(drive_rr_node_type == drive_rr_nodes[inode]->type);
      assert((CHANX == drive_rr_nodes[inode]->type)||(CHANY == drive_rr_nodes[inode]->type));
    }
  } 
  /* input port*/
  for (int inode = 0; inode < mux_size; ++inode) {
    fprintf(fp, "assign %s_size%d_%d_inbus[%d] = ",
                verilog_model->prefix, mux_size, verilog_model->cnt, input_cnt);
    int drive_node_index = rr_gsb.get_cb_chan_node_index(cb_type, drive_rr_nodes[inode]);
    assert (-1 != drive_node_index);
    fprintf(fp, "%s;\n", rr_gsb.gen_cb_verilog_routing_track_name(cb_type, drive_node_index));
    input_cnt++;
  }
  assert(input_cnt == mux_size);

  /* Print SRAMs that configure this MUX */
  /* cur_num_sram = sram_verilog_model->cnt; */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl);
  /* connect to reserved BL/WLs ? */
  num_mux_reserved_conf_bits = count_num_reserved_conf_bits_one_spice_model(verilog_model, 
                                                                            cur_sram_orgz_info->type,
                                                                            mux_size);
  /* Get the number of configuration bits required by this MUX */
  num_mux_conf_bits = count_num_conf_bits_one_spice_model(verilog_model, 
                                                          cur_sram_orgz_info->type,
                                                          mux_size);

  /* Dump the configuration port bus */
  dump_verilog_mux_config_bus(fp, verilog_model, cur_sram_orgz_info,
                              mux_size, cur_num_sram, num_mux_reserved_conf_bits, num_mux_conf_bits); 

  /* Dump ports visible only during formal verification */
  fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
  /*
  dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                              cur_num_sram, 
                                              cur_num_sram + num_mux_conf_bits - 1,
                                              VERILOG_PORT_WIRE);
  fprintf(fp, ";\n");
  */
  dump_verilog_formal_verification_mux_sram_ports_wiring(fp, cur_sram_orgz_info,
                                                         verilog_model, mux_size,
                                                         cur_num_sram, 
                                                         cur_num_sram + num_mux_conf_bits - 1);
  
  fprintf(fp, "`endif\n");


  /* Call the MUX SPICE model */
  fprintf(fp, "%s_size%d %s_size%d_%d_ (", 
          verilog_model->name, mux_size, 
          verilog_model->prefix, mux_size, verilog_model->cnt);

  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, FALSE, FALSE, my_bool_to_boolean(is_explicit_mapping), TRUE)) {
    fprintf(fp, ",\n");
  }

  /* connect to input bus*/
  if (true == is_explicit_mapping) {
    fprintf(fp, ".in(");
  }
  fprintf(fp, "%s_size%d_%d_inbus",
               verilog_model->prefix, mux_size, verilog_model->cnt);
  if (true == is_explicit_mapping) {
    fprintf(fp, ")");
  }
  fprintf(fp, ", ");

  /* output port*/
  xlow = src_rr_node->xlow;
  ylow = src_rr_node->ylow;

  assert(IPIN == src_rr_node->type);
  /* Search all the sides of a CB, see this drive_rr_node is an INPUT of this SB */
  rr_gsb.get_node_side_and_index(src_rr_node, OUT_PORT, &side, &index);
  /* We need to be sure that drive_rr_node is part of the CB */
  assert((-1 != index)&&(NUM_SIDES != side));
  if (true == is_explicit_mapping) {
    fprintf(fp, ".out(");
  }
  dump_verilog_grid_side_pin_with_given_index(fp, OPIN, /* This is an output of a connection box */
                                              rr_gsb.get_ipin_node(side, index)->ptc_num, 
                                              rr_gsb.get_ipin_node_grid_side(side, index), 
                                              xlow, ylow, /* Coordinator of Grid */ 
                                              0,0, /*No explicit mapping */
                                              FALSE, false); /* Do not specify the direction of port */
  if (true == is_explicit_mapping) {
    fprintf(fp, ")");
  }
  fprintf(fp, ", "); 

  /* Different design technology requires different configuration bus! */
  dump_verilog_mux_config_bus_ports(fp, verilog_model, cur_sram_orgz_info,
                                    mux_size, cur_num_sram,
                                    num_mux_reserved_conf_bits, 
                                    num_mux_conf_bits, is_explicit_mapping);


  fprintf(fp, ");\n");

  switch (verilog_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    decode_cmos_mux_sram_bits(verilog_model, mux_size, path_id, &num_mux_sram_bits, &mux_sram_bits, &mux_level);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    decode_rram_mux(verilog_model, mux_size, path_id, &num_mux_sram_bits, &mux_sram_bits, &mux_level);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
               __FILE__, __LINE__, verilog_model->name);
  }

  /* Print the encoding in SPICE netlist for debugging */
  switch (verilog_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    fprintf(fp, "//----- SRAM bits for MUX[%d], level=%d, select_path_id=%d. -----\n", 
            verilog_model->cnt, mux_level, path_id);
    fprintf(fp, "//----- From LSB(LEFT) TO MSB (RIGHT) -----\n");
    fprintf(fp, "//-----");
    fprint_commented_sram_bits(fp, num_mux_sram_bits, mux_sram_bits);
    fprintf(fp, "-----\n");
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    fprintf(fp, "//----- BL/WL bits for 4T1R MUX[%d], level=%d, select_path_id=%d. -----\n", 
            verilog_model->cnt, mux_level, path_id);
    fprintf(fp, "//----- From LSB(LEFT) TO MSB (RIGHT) -----\n");
    fprintf(fp, "//---- BL: ");
    fprint_commented_sram_bits(fp, num_mux_sram_bits/2, mux_sram_bits);
    fprintf(fp, "-----\n");
    fprintf(fp, "//----- From LSB(LEFT) TO MSB (RIGHT) -----\n");
    fprintf(fp, "//---- WL: ");
    fprint_commented_sram_bits(fp, num_mux_sram_bits/2, mux_sram_bits + num_mux_sram_bits/2);
    fprintf(fp, "-----\n");
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
               __FILE__, __LINE__, verilog_model->name);
  }

  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
  /* Dump sram modules */
  switch (verilog_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    /* Call the memory module defined for this SRAM-based MUX! */
    mem_subckt_name = generate_verilog_mux_subckt_name(verilog_model, mux_size, verilog_mem_posfix);
    fprintf(fp, "%s %s_%d_ ( ", 
            mem_subckt_name, mem_subckt_name, verilog_model->cnt);
    dump_verilog_mem_sram_submodule(fp, cur_sram_orgz_info, verilog_model, mux_size, mem_model, 
                                    cur_num_sram, cur_num_sram + num_mux_conf_bits - 1,
                                    my_bool_to_boolean(is_explicit_mapping)); 
    fprintf(fp, ");\n");
    /* update the number of memory bits */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_sram + num_mux_conf_bits);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* RRAM-based MUX does not need any SRAM dumping
     * But we have to get the number of configuration bits required by this MUX 
     * and update the number of memory bits 
     */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_sram + num_mux_conf_bits);
    update_sram_orgz_info_num_blwl(cur_sram_orgz_info, 
                                   cur_bl + num_mux_conf_bits, 
                                   cur_wl + num_mux_conf_bits);
    break;  
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
               __FILE__, __LINE__, verilog_model->name);
  }

  /* update sram counter */
  verilog_model->cnt++;

  /* Free */
  my_free(mux_sram_bits);
  my_free(mem_subckt_name);

  return;
}



void dump_verilog_connection_box_mux(t_sram_orgz_info* cur_sram_orgz_info,
                                     FILE* fp,
                                     t_cb* cur_cb_info,
                                     t_rr_node* src_rr_node,
                                     bool is_explicit_mapping) {
  int mux_size, cur_num_sram, input_cnt = 0;
  t_rr_node** drive_rr_nodes = NULL;
  int inode, mux_level, path_id, switch_index;
  t_spice_model* verilog_model = NULL;
  int num_mux_sram_bits = 0;
  int* mux_sram_bits = NULL;
  t_rr_type drive_rr_node_type = NUM_RR_TYPES;
  int xlow, ylow, side, index;
  int num_mux_conf_bits = 0;
  int num_mux_reserved_conf_bits = 0;
  int cur_bl, cur_wl;
  t_spice_model* mem_model = NULL;
  char* mem_subckt_name = NULL;

  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }
  /* Check */
  assert((!(0 > cur_cb_info->x))&&(!(cur_cb_info->x > (nx + 1)))); 
  assert((!(0 > cur_cb_info->y))&&(!(cur_cb_info->y > (ny + 1)))); 

  /* Find drive_rr_nodes*/
  mux_size = src_rr_node->num_drive_rr_nodes;
  drive_rr_nodes = src_rr_node->drive_rr_nodes; 

  /* Configuration bits for MUX*/
  path_id = DEFAULT_PATH_ID;
  for (inode = 0; inode < mux_size; inode++) {
    if (drive_rr_nodes[inode] == &(rr_node[src_rr_node->prev_node])) {
      path_id = inode;
      src_rr_node->id_path = inode;
      break;
    }
  }
  switch_index = src_rr_node->drive_switches[DEFAULT_SWITCH_ID];

  verilog_model = switch_inf[switch_index].spice_model;


  char* name_mux = (char *) my_malloc(sizeof(char)*(1 + strlen(verilog_model->prefix) + 5
                                                    + strlen(my_itoa(mux_size)) + 1 
                                                    + strlen(my_itoa(verilog_model->cnt)) + 5));
  sprintf(name_mux, "/%s_size%d_%d_/in", verilog_model->prefix, mux_size, verilog_model->cnt);
  char* path_hierarchy = (char *) my_malloc(sizeof(char)*(strlen(gen_verilog_one_cb_instance_name(cur_cb_info)))); 
  path_hierarchy = gen_verilog_one_cb_instance_name(cur_cb_info);
  src_rr_node->name_mux = my_strcat(path_hierarchy,name_mux);

  /* Specify the input bus */
  fprintf(fp, "wire [0:%d] %s_size%d_%d_inbus;\n",
          mux_size - 1,
          verilog_model->prefix, mux_size, verilog_model->cnt);

  /* Check drive_rr_nodes type, should be the same*/
  for (inode = 0; inode < mux_size; inode++) {
    if (NUM_RR_TYPES == drive_rr_node_type) { 
      drive_rr_node_type = drive_rr_nodes[inode]->type;
    } else {
      assert(drive_rr_node_type == drive_rr_nodes[inode]->type);
      assert((CHANX == drive_rr_nodes[inode]->type)||(CHANY == drive_rr_nodes[inode]->type));
    }
  } 
  /* input port*/
  for (inode = 0; inode < mux_size; inode++) {
    fprintf(fp, "assign %s_size%d_%d_inbus[%d] = ",
                verilog_model->prefix, mux_size, verilog_model->cnt, input_cnt);
    fprintf(fp, "%s_%d__%d__midout_%d_;\n",
            convert_chan_type_to_string(drive_rr_nodes[inode]->type),
            cur_cb_info->x, cur_cb_info->y, drive_rr_nodes[inode]->ptc_num);
    input_cnt++;
  }
  assert(input_cnt == mux_size);

  /* Print SRAMs that configure this MUX */
  /* cur_num_sram = sram_verilog_model->cnt; */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  get_sram_orgz_info_num_blwl(cur_sram_orgz_info, &cur_bl, &cur_wl);
  /* connect to reserved BL/WLs ? */
  num_mux_reserved_conf_bits = count_num_reserved_conf_bits_one_spice_model(verilog_model, 
                                                                            cur_sram_orgz_info->type,
                                                                            mux_size);
  /* Get the number of configuration bits required by this MUX */
  num_mux_conf_bits = count_num_conf_bits_one_spice_model(verilog_model, 
                                                          cur_sram_orgz_info->type,
                                                          mux_size);

  /* Dump the configuration port bus */
  dump_verilog_mux_config_bus(fp, verilog_model, cur_sram_orgz_info,
                              mux_size, cur_num_sram, num_mux_reserved_conf_bits, num_mux_conf_bits); 

  /* Dump ports visible only during formal verification */
  fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
  /*
  dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                              cur_num_sram, 
                                              cur_num_sram + num_mux_conf_bits - 1,
                                              VERILOG_PORT_WIRE);
  fprintf(fp, ";\n");
  */
  dump_verilog_formal_verification_mux_sram_ports_wiring(fp, cur_sram_orgz_info,
                                                         verilog_model, mux_size,
                                                         cur_num_sram, 
                                                         cur_num_sram + num_mux_conf_bits - 1);
  
  fprintf(fp, "`endif\n");


  /* Call the MUX SPICE model */
  fprintf(fp, "%s_size%d %s_size%d_%d_ (", 
          verilog_model->name, mux_size, 
          verilog_model->prefix, mux_size, verilog_model->cnt);

  /* Dump global ports */
  if  (0 < rec_dump_verilog_spice_model_global_ports(fp, verilog_model, FALSE, FALSE, my_bool_to_boolean(is_explicit_mapping), TRUE)) {
    fprintf(fp, ",\n");
  }

  /* connect to input bus*/
  if (true == is_explicit_mapping) {
    fprintf(fp, ".in(");
  }
  fprintf(fp, "%s_size%d_%d_inbus",
               verilog_model->prefix, mux_size, verilog_model->cnt);
  if (true == is_explicit_mapping) {
    fprintf(fp, ")");
  }
  fprintf(fp, ", ");

  /* output port*/
  xlow = src_rr_node->xlow;
  ylow = src_rr_node->ylow;

  assert(IPIN == src_rr_node->type);
  /* Search all the sides of a CB, see this drive_rr_node is an INPUT of this SB */
  get_rr_node_side_and_index_in_cb_info(src_rr_node, (*cur_cb_info), OUT_PORT, &side, &index);
  /* We need to be sure that drive_rr_node is part of the CB */
  assert((-1 != index)&&(-1 != side));
  if (true == is_explicit_mapping) {
    fprintf(fp, ".out(");
  }
  dump_verilog_grid_side_pin_with_given_index(fp, OPIN, /* This is an output of a connection box */
                                              cur_cb_info->ipin_rr_node[side][index]->ptc_num, 
                                              cur_cb_info->ipin_rr_node_grid_side[side][index], 
                                              xlow, ylow, /* Coordinator of Grid */ 
                                              0,0, /*No explicit mapping */
                                              FALSE, false); /* Do not specify the direction of port */
  if (true == is_explicit_mapping) {
    fprintf(fp, ")");
  }
  fprintf(fp, ", "); 

  /* Different design technology requires different configuration bus! */
  dump_verilog_mux_config_bus_ports(fp, verilog_model, cur_sram_orgz_info,
                                    mux_size, cur_num_sram, 
                                    num_mux_reserved_conf_bits, 
                                    num_mux_conf_bits, is_explicit_mapping);


  fprintf(fp, ");\n");

  switch (verilog_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    decode_cmos_mux_sram_bits(verilog_model, mux_size, path_id, &num_mux_sram_bits, &mux_sram_bits, &mux_level);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    decode_rram_mux(verilog_model, mux_size, path_id, &num_mux_sram_bits, &mux_sram_bits, &mux_level);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
               __FILE__, __LINE__, verilog_model->name);
  }

  /* Print the encoding in SPICE netlist for debugging */
  switch (verilog_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    fprintf(fp, "//----- SRAM bits for MUX[%d], level=%d, select_path_id=%d. -----\n", 
            verilog_model->cnt, mux_level, path_id);
    fprintf(fp, "//----- From LSB(LEFT) TO MSB (RIGHT) -----\n");
    fprintf(fp, "//-----");
    fprint_commented_sram_bits(fp, num_mux_sram_bits, mux_sram_bits);
    fprintf(fp, "-----\n");
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    fprintf(fp, "//----- BL/WL bits for 4T1R MUX[%d], level=%d, select_path_id=%d. -----\n", 
            verilog_model->cnt, mux_level, path_id);
    fprintf(fp, "//----- From LSB(LEFT) TO MSB (RIGHT) -----\n");
    fprintf(fp, "//---- BL: ");
    fprint_commented_sram_bits(fp, num_mux_sram_bits/2, mux_sram_bits);
    fprintf(fp, "-----\n");
    fprintf(fp, "//----- From LSB(LEFT) TO MSB (RIGHT) -----\n");
    fprintf(fp, "//---- WL: ");
    fprint_commented_sram_bits(fp, num_mux_sram_bits/2, mux_sram_bits + num_mux_sram_bits/2);
    fprintf(fp, "-----\n");
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
               __FILE__, __LINE__, verilog_model->name);
  }

  get_sram_orgz_info_mem_model(cur_sram_orgz_info, &mem_model);
  /* Dump sram modules */
  switch (verilog_model->design_tech) {
  case SPICE_MODEL_DESIGN_CMOS:
    /* Call the memory module defined for this SRAM-based MUX! */
    mem_subckt_name = generate_verilog_mux_subckt_name(verilog_model, mux_size, verilog_mem_posfix);
    fprintf(fp, "%s %s_%d_ ( ", 
            mem_subckt_name, mem_subckt_name, verilog_model->cnt);
    dump_verilog_mem_sram_submodule(fp, cur_sram_orgz_info, verilog_model, mux_size, mem_model, 
                                    cur_num_sram, cur_num_sram + num_mux_conf_bits - 1, 
                                    my_bool_to_boolean(is_explicit_mapping)); 
    fprintf(fp, ");\n");
    /* update the number of memory bits */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_sram + num_mux_conf_bits);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* RRAM-based MUX does not need any SRAM dumping
     * But we have to get the number of configuration bits required by this MUX 
     * and update the number of memory bits 
     */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_num_sram + num_mux_conf_bits);
    update_sram_orgz_info_num_blwl(cur_sram_orgz_info, 
                                   cur_bl + num_mux_conf_bits, 
                                   cur_wl + num_mux_conf_bits);
    break;  
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid design technology for verilog model (%s)!\n",
               __FILE__, __LINE__, verilog_model->name);
  }

  /* update sram counter */
  verilog_model->cnt++;

  /* Free */
  my_free(mux_sram_bits);
  my_free(mem_subckt_name);

  return;
}

static 
void dump_verilog_connection_box_interc(t_sram_orgz_info* cur_sram_orgz_info,
                                        FILE* fp,
                                        const RRGSB& rr_gsb, t_rr_type cb_type,
                                        t_rr_node* src_rr_node,
                                        bool is_explicit_mapping) {
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  if (1 == src_rr_node->fan_in) {
    /* Print a direct connection*/
    dump_verilog_connection_box_short_interc(fp, rr_gsb, cb_type, src_rr_node);
  } else if (1 < src_rr_node->fan_in) {
    /* Print the multiplexer, fan_in >= 2 */
    dump_verilog_connection_box_mux(cur_sram_orgz_info, fp, rr_gsb, cb_type,
                                    src_rr_node, is_explicit_mapping);
  } /*Nothing should be done else*/ 
   
  return;
}


void dump_verilog_connection_box_interc(t_sram_orgz_info* cur_sram_orgz_info,
                                        FILE* fp,
                                        t_cb* cur_cb_info,
                                        t_rr_node* src_rr_node,
                                        bool is_explicit_mapping) {
  /* Check the file handler*/ 
  if (NULL == fp) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid file handler.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Check */
  assert((!(0 > cur_cb_info->x))&&(!(cur_cb_info->x > (nx + 1)))); 
  assert((!(0 > cur_cb_info->y))&&(!(cur_cb_info->y > (ny + 1)))); 

  if (1 == src_rr_node->fan_in) {
    /* Print a direct connection*/
    dump_verilog_connection_box_short_interc(fp, cur_cb_info, src_rr_node);
  } else if (1 < src_rr_node->fan_in) {
    /* Print the multiplexer, fan_in >= 2 */
    dump_verilog_connection_box_mux(cur_sram_orgz_info, fp, cur_cb_info, 
                                    src_rr_node, is_explicit_mapping);
  } /*Nothing should be done else*/ 
   
  return;
}

/* Count the number of configuration bits of a connection box */
int count_verilog_connection_box_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                           const RRGSB& rr_gsb, t_rr_type cb_type) {
  int num_conf_bits = 0;

  std::vector<enum e_side> cb_sides = rr_gsb.get_cb_ipin_sides(cb_type);

  for (size_t side = 0; side < cb_sides.size(); ++side) {
    enum e_side cb_ipin_side = cb_sides[side];
    /* Count the number of configuration bits */
    num_conf_bits += count_verilog_connection_box_one_side_conf_bits(cur_sram_orgz_info,
                                                                     rr_gsb, cb_ipin_side); 
  }

  return num_conf_bits;
}


/* Count the number of configuration bits of a connection box */
int count_verilog_connection_box_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                           t_cb* cur_cb_info) {
  int side;
  int side_cnt = 0;
  int num_conf_bits = 0;

  for (side = 0; side < cur_cb_info->num_sides; side++) {
    /* Bypass side with zero IPINs*/
    if (0 == cur_cb_info->num_ipin_rr_nodes[side]) {
      continue;
    }
    side_cnt++;
    assert(0 < cur_cb_info->num_ipin_rr_nodes[side]);
    assert(NULL != cur_cb_info->ipin_rr_node[side]);
    /* Count the number of configuration bits */
    num_conf_bits += count_verilog_connection_box_one_side_conf_bits(cur_sram_orgz_info,
                                                                     cur_cb_info->num_ipin_rr_nodes[side], 
                                                                     cur_cb_info->ipin_rr_node[side]);
  }
  /* Make sure only 2 sides of IPINs are printed */
  assert((1 == side_cnt)||(2 == side_cnt));

  return num_conf_bits;
}

/* Count the number of reserved configuration bits of a connection box */
int count_verilog_connection_box_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                    const RRGSB& rr_gsb, t_rr_type cb_type) {
  int num_reserved_conf_bits = 0;
  std::vector<enum e_side> cb_sides = rr_gsb.get_cb_ipin_sides(cb_type);

  for (size_t side = 0; side < cb_sides.size(); ++side) {
    enum e_side cb_ipin_side = cb_sides[side];
    /* Count the number of reserved configuration bits */
    int temp_num_reserved_conf_bits = count_verilog_connection_box_one_side_reserved_conf_bits(cur_sram_orgz_info, rr_gsb, cb_ipin_side);
    /* Only consider the largest reserved configuration bits */
    num_reserved_conf_bits = std::max(num_reserved_conf_bits, temp_num_reserved_conf_bits); 
  }

  return num_reserved_conf_bits;
}


/* Count the number of reserved configuration bits of a connection box */
int count_verilog_connection_box_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                    t_cb* cur_cb_info) {
  int side;
  int side_cnt = 0;
  int num_reserved_conf_bits = 0;
  int temp_num_reserved_conf_bits = 0;

  for (side = 0; side < cur_cb_info->num_sides; side++) {
    /* Bypass side with zero IPINs*/
    if (0 == cur_cb_info->num_ipin_rr_nodes[side]) {
      continue;
    }
    side_cnt++;
    assert(0 < cur_cb_info->num_ipin_rr_nodes[side]);
    assert(NULL != cur_cb_info->ipin_rr_node[side]);
    /* Count the number of reserved configuration bits */
    temp_num_reserved_conf_bits = count_verilog_connection_box_one_side_reserved_conf_bits(cur_sram_orgz_info,
                                                                                           cur_cb_info->num_ipin_rr_nodes[side], 
                                                                                           cur_cb_info->ipin_rr_node[side]);
    /* Only consider the largest reserved configuration bits */
    if (temp_num_reserved_conf_bits > num_reserved_conf_bits) {
      num_reserved_conf_bits = temp_num_reserved_conf_bits;
    }
  }
  /* Make sure only 2 sides of IPINs are printed */
  assert((1 == side_cnt)||(2 == side_cnt));

  return num_reserved_conf_bits;
}

/* Print connection boxes
 * Print the sub-circuit of a connection Box (Type: [CHANX|CHANY])
 * Actually it is very similiar to switch box but
 * the difference is connection boxes connect Grid INPUT Pins to channels
 * TODO: merge direct connections into CB 
 *    --------------             --------------
 *    |            |             |            |
 *    |    Grid    |   ChanY     |    Grid    |
 *    |  [x][y+1]  |   [x][y]    | [x+1][y+1] |
 *    |            | Connection  |            |
 *    -------------- Box_Y[x][y] --------------
 *                   ----------
 *       ChanX       | Switch |        ChanX 
 *       [x][y]      |   Box  |       [x+1][y]
 *     Connection    | [x][y] |      Connection 
 *    Box_X[x][y]    ----------     Box_X[x+1][y]
 *    --------------             --------------
 *    |            |             |            |
 *    |    Grid    |  ChanY      |    Grid    |
 *    |   [x][y]   | [x][y-1]    |  [x+1][y]  |
 *    |            | Connection  |            |
 *    --------------Box_Y[x][y-1]--------------
 */
static 
void dump_verilog_routing_connection_box_unique_module(t_sram_orgz_info* cur_sram_orgz_info,
                                                       char* verilog_dir, char* subckt_dir, 
                                                       const RRGSB& rr_cb, t_rr_type cb_type,
                                                       bool is_explicit_mapping) {
  FILE* fp = NULL;
  char* fname = NULL;
  int cur_num_sram, num_conf_bits, num_reserved_conf_bits, esti_sram_cnt;

  RRGSB rr_gsb = rr_cb; /* IMPORTANT: this copy will be removed when the config ports are initialized when created!!! */

  /* Count the number of configuration bits */
  /* Count the number of configuration bits to be consumed by this Switch block */
  num_conf_bits = count_verilog_connection_box_conf_bits(cur_sram_orgz_info, rr_gsb, cb_type);
  /* Count the number of reserved configuration bits to be consumed by this Switch block */
  num_reserved_conf_bits = count_verilog_connection_box_reserved_conf_bits(cur_sram_orgz_info, rr_gsb, cb_type);
  /* Estimate the sram_verilog_model->cnt */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  esti_sram_cnt = cur_num_sram + num_conf_bits;
  /* Record index */
  rr_gsb.set_cb_num_reserved_conf_bits(cb_type, num_reserved_conf_bits);
  rr_gsb.set_cb_conf_bits_lsb(cb_type, cur_num_sram);
  rr_gsb.set_cb_conf_bits_msb(cb_type, cur_num_sram + num_conf_bits - 1);

  /* Print the definition of subckt*/
  /* Create file handler */
  fp = verilog_create_one_subckt_file(subckt_dir, 
                                      "Connection Block - X/Y direction ", 
                                      rr_gsb.gen_cb_verilog_module_name(cb_type), 
                                      &fname);

  /* Print preprocessing flags */
  verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Comment lines */
  fprintf(fp, 
          "//----- Verilog Module of Connection block %s[%lu][%lu] -----\n", 
          convert_cb_type_to_string(cb_type), rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
  fprintf(fp, "module ");
  fprintf(fp, "%s ", rr_gsb.gen_cb_verilog_module_name(cb_type));
  fprintf(fp, "(\n");
  /* dump global ports */
  if (0 < dump_verilog_global_ports(fp, global_ports_head, TRUE, false)) {
    fprintf(fp, ",\n");
  }
  /* Print the ports of channels*/
  /*connect to the mid point of a track*/
  /* Get the chan_rr_nodes: Only one side of a cb_info has chan_rr_nodes*/
  for (size_t inode = 0; inode < rr_gsb.get_cb_chan_width(cb_type); ++inode) {
    fprintf(fp, "input %s, \n",
            rr_gsb.gen_cb_verilog_routing_track_name(cb_type, inode));
  }

  /* Print the ports of grids*/
  /* only check ipin_rr_nodes of cur_cb_info */
  std::vector<enum e_side> cb_ipin_sides = rr_gsb.get_cb_ipin_sides(cb_type);
  for (size_t iside = 0; iside < cb_ipin_sides.size(); ++iside) {
    enum e_side cb_ipin_side = cb_ipin_sides[iside];
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side); ++inode) {
      /* Print each INPUT Pins of a grid */
      dump_verilog_grid_side_pin_with_given_index(fp, IPIN, /* This is an output of a connection box */
                                                  rr_gsb.get_ipin_node(cb_ipin_side, inode)->ptc_num,
                                                  rr_gsb.get_ipin_node_grid_side(cb_ipin_side, inode),
                                                  rr_gsb.get_ipin_node(cb_ipin_side, inode)->xlow,
                                                  rr_gsb.get_ipin_node(cb_ipin_side, inode)->ylow,
                                                  0,0, /*No explicit mapping */
                                                  TRUE, false); 

    }
  }

  /* Put down configuration port */
  /* output of each configuration bit */
  /* Reserved sram ports */
  if (0 < rr_gsb.get_cb_num_reserved_conf_bits(cb_type)) {
    dump_verilog_reserved_sram_ports(fp, cur_sram_orgz_info, 
                                     rr_gsb.get_cb_reserved_conf_bits_lsb(cb_type),
                                     rr_gsb.get_cb_reserved_conf_bits_msb(cb_type),
                                     VERILOG_PORT_INPUT);
    fprintf(fp, ",\n");
  }
  /* Normal sram ports */
  dump_verilog_sram_ports(fp, cur_sram_orgz_info, 
                          rr_gsb.get_cb_conf_bits_lsb(cb_type),
                          rr_gsb.get_cb_conf_bits_msb(cb_type),
                          VERILOG_PORT_INPUT);

  /* Dump ports only visible during formal verification*/
  if (0 < rr_gsb.get_cb_num_conf_bits(cb_type)) {
    fprintf(fp, "\n");
    fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
    fprintf(fp, ",\n");
    dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                                rr_gsb.get_cb_conf_bits_lsb(cb_type),
                                                rr_gsb.get_cb_conf_bits_msb(cb_type),
                                                VERILOG_PORT_INPUT, false);
    fprintf(fp, "\n");
    fprintf(fp, "`endif\n");
  }

  /* subckt definition ends with svdd and sgnd*/
  fprintf(fp, ");\n");

  /* Local wires for memory configurations */
  dump_verilog_sram_config_bus_internal_wires(fp, cur_sram_orgz_info, 
                                              rr_gsb.get_cb_conf_bits_lsb(cb_type),
                                              rr_gsb.get_cb_conf_bits_msb(cb_type));

  /* Record LSB and MSB of reserved_conf_bits and normal conf_bits */

  /* Print multiplexers or direct interconnect*/
  for (size_t iside = 0; iside < cb_ipin_sides.size(); ++iside) {
    enum e_side cb_ipin_side = cb_ipin_sides[iside];
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side); ++inode) {
      dump_verilog_connection_box_interc(cur_sram_orgz_info, fp, rr_gsb, cb_type, 
                                         rr_gsb.get_ipin_node(cb_ipin_side, inode),
                                         is_explicit_mapping);
    }
  }

  fprintf(fp, "endmodule\n");

  /* Comment lines */
  fprintf(fp, 
          "//----- END Verilog Module of Connection Box %s [%lu][%lu] -----\n\n", 
          convert_cb_type_to_string(cb_type), rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));

  /* Check */
  assert(esti_sram_cnt == get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info));

  /* Close file handler */
  fclose(fp);

  /* Add fname to the linked list */
  routing_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(routing_verilog_subckt_file_path_head, fname);  

  /* Free */
  my_free(fname);
 
  return;
}


/* Print connection boxes
 * Print the sub-circuit of a connection Box (Type: [CHANX|CHANY])
 * Actually it is very similiar to switch box but
 * the difference is connection boxes connect Grid INPUT Pins to channels
 * TODO: merge direct connections into CB 
 *    --------------             --------------
 *    |            |             |            |
 *    |    Grid    |   ChanY     |    Grid    |
 *    |  [x][y+1]  |   [x][y]    | [x+1][y+1] |
 *    |            | Connection  |            |
 *    -------------- Box_Y[x][y] --------------
 *                   ----------
 *       ChanX       | Switch |        ChanX 
 *       [x][y]      |   Box  |       [x+1][y]
 *     Connection    | [x][y] |      Connection 
 *    Box_X[x][y]    ----------     Box_X[x+1][y]
 *    --------------             --------------
 *    |            |             |            |
 *    |    Grid    |  ChanY      |    Grid    |
 *    |   [x][y]   | [x][y-1]    |  [x+1][y]  |
 *    |            | Connection  |            |
 *    --------------Box_Y[x][y-1]--------------
 */
void dump_verilog_routing_connection_box_subckt(t_sram_orgz_info* cur_sram_orgz_info,
                                                char* verilog_dir, char* subckt_dir, 
                                                t_cb* cur_cb_info,
                                                boolean compact_routing_hierarchy,
                                                bool is_explicit_mapping) {
  int itrack, inode, side, x, y;
  int side_cnt = 0;
  FILE* fp = NULL;
  char* fname = NULL;
  int cur_num_sram, num_conf_bits, num_reserved_conf_bits, esti_sram_cnt;
   
  /* Check */
  assert((!(0 > cur_cb_info->x))&&(!(cur_cb_info->x > (nx + 1)))); 
  assert((!(0 > cur_cb_info->y))&&(!(cur_cb_info->y > (ny + 1)))); 

  x= cur_cb_info->x;
  y= cur_cb_info->y;

  /* Count the number of configuration bits */
  /* Count the number of configuration bits to be consumed by this Switch block */
  num_conf_bits = count_verilog_connection_box_conf_bits(cur_sram_orgz_info, cur_cb_info);
  /* Count the number of reserved configuration bits to be consumed by this Switch block */
  num_reserved_conf_bits = count_verilog_connection_box_reserved_conf_bits(cur_sram_orgz_info, cur_cb_info);
  /* Estimate the sram_verilog_model->cnt */
  cur_num_sram = get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info); 
  esti_sram_cnt = cur_num_sram + num_conf_bits;
  /* Record index */
  cur_cb_info->num_reserved_conf_bits = num_reserved_conf_bits;
  cur_cb_info->conf_bits_lsb = cur_num_sram;
  cur_cb_info->conf_bits_msb = cur_num_sram + num_conf_bits;

  /* Handle mirror switch blocks:
   * For mirrors, no need to output a file   
   * Just update the counter 
   */
  if (  (TRUE == compact_routing_hierarchy)
     && (NULL != cur_cb_info->mirror) ) {
    /* Again ensure the conf_bits should match !!! */
    /* Count the number  of configuration bits of the mirror */
    int mirror_num_conf_bits = count_verilog_connection_box_conf_bits(cur_sram_orgz_info, cur_cb_info->mirror);
    assert( mirror_num_conf_bits == num_conf_bits );
    /* update memory bits return directly */
    update_sram_orgz_info_num_mem_bit(cur_sram_orgz_info, cur_cb_info->conf_bits_msb);
    return;
  }

  /* Print the definition of subckt*/
  /* Identify the type of connection box */
  switch(cur_cb_info->type) {
  case CHANX:
    /* Create file handler */
    fp = verilog_create_one_subckt_file(subckt_dir, "Connection Block - X direction ", cbx_verilog_file_name_prefix, cur_cb_info->x, cur_cb_info->y, &fname);

    /* Print preprocessing flags */
    verilog_include_defines_preproc_file(fp, verilog_dir);

    /* Comment lines */
    fprintf(fp, "//----- Verilog Module of Connection Box -X direction [%d][%d] -----\n", x, y);
    fprintf(fp, "module ");
    fprintf(fp, "cbx_%d__%d_ ", cur_cb_info->x, cur_cb_info->y);
    break;
  case CHANY:
    /* Create file handler */
    fp = verilog_create_one_subckt_file(subckt_dir, "Connection Block - Y direction ", cby_verilog_file_name_prefix, cur_cb_info->x, cur_cb_info->y, &fname);

    /* Print preprocessing flags */
    verilog_include_defines_preproc_file(fp, verilog_dir);
    /* Comment lines */
    fprintf(fp, "//----- Verilog Module of Connection Box -Y direction [%d][%d] -----\n", x, y);
    fprintf(fp, "module ");
    fprintf(fp, "cby_%d__%d_ ", cur_cb_info->x, cur_cb_info->y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }
 
  fprintf(fp, "(\n");
  /* dump global ports */
  if (0 < dump_verilog_global_ports(fp, global_ports_head, TRUE, false)) {
    fprintf(fp, ",\n");
  }
  /* Print the ports of channels*/
  /*connect to the mid point of a track*/
  /* Get the chan_rr_nodes: Only one side of a cb_info has chan_rr_nodes*/
  side_cnt = 0;
  for (side = 0; side < cur_cb_info->num_sides; side++) {
    /* Bypass side with zero channel width */
    if (0 == cur_cb_info->chan_width[side]) {
      continue;
    }
    assert (0 < cur_cb_info->chan_width[side]);
    side_cnt++;
    for (itrack = 0; itrack < cur_cb_info->chan_width[side]; itrack++) {
      fprintf(fp, "input %s, \n",
              gen_verilog_routing_channel_one_midout_name( cur_cb_info,
                                                           itrack));
    }
  }
  /*check side_cnt */
  assert((1 == side_cnt)||(2 == side_cnt));

  side_cnt = 0;
  /* Print the ports of grids*/
  /* only check ipin_rr_nodes of cur_cb_info */
  for (side = 0; side < cur_cb_info->num_sides; side++) {
    /* Bypass side with zero IPINs*/
    if (0 == cur_cb_info->num_ipin_rr_nodes[side]) {
      continue;
    }
    side_cnt++;
    assert(0 < cur_cb_info->num_ipin_rr_nodes[side]);
    assert(NULL != cur_cb_info->ipin_rr_node[side]);
    for (inode = 0; inode < cur_cb_info->num_ipin_rr_nodes[side]; inode++) {
      /* Print each INPUT Pins of a grid */
      dump_verilog_grid_side_pin_with_given_index(fp, IPIN, /* This is an output of a connection box */
                                                  cur_cb_info->ipin_rr_node[side][inode]->ptc_num,
                                                  cur_cb_info->ipin_rr_node_grid_side[side][inode],
                                                  cur_cb_info->ipin_rr_node[side][inode]->xlow,
                                                  cur_cb_info->ipin_rr_node[side][inode]->ylow,
                                                  0,/*Used in more recent version*/
                                                  0,/*Used in more recent version*/
                                                  TRUE, is_explicit_mapping); 

    }
  }
  /* Make sure only 2 sides of IPINs are printed */
  assert((1 == side_cnt)||(2 == side_cnt));


  /* Put down configuration port */
  /* output of each configuration bit */
  /* Reserved sram ports */
  dump_verilog_reserved_sram_ports(fp, cur_sram_orgz_info, 
                                   0, cur_cb_info->num_reserved_conf_bits - 1,
                                   VERILOG_PORT_INPUT);
  if (0 < cur_cb_info->num_reserved_conf_bits) {
    fprintf(fp, ",\n");
  }
  /* Normal sram ports */
  dump_verilog_sram_ports(fp, cur_sram_orgz_info, 
                          cur_cb_info->conf_bits_lsb,
                          cur_cb_info->conf_bits_msb - 1,
                          VERILOG_PORT_INPUT);

  /* Dump ports only visible during formal verification*/
  if (0 < (cur_cb_info->conf_bits_msb - 1 - cur_cb_info->conf_bits_lsb)) {
    fprintf(fp, "\n");
    fprintf(fp, "`ifdef %s\n", verilog_formal_verification_preproc_flag);
    fprintf(fp, ",\n");
    dump_verilog_formal_verification_sram_ports(fp, cur_sram_orgz_info, 
                                                cur_cb_info->conf_bits_lsb, 
                                                cur_cb_info->conf_bits_msb - 1,
                                                VERILOG_PORT_INPUT, is_explicit_mapping);
    fprintf(fp, "\n");
    fprintf(fp, "`endif\n");
  }

  /* subckt definition ends with svdd and sgnd*/
  fprintf(fp, ");\n");

  /* Local wires for memory configurations */
  dump_verilog_sram_config_bus_internal_wires(fp, cur_sram_orgz_info, 
                                              cur_cb_info->conf_bits_lsb, cur_cb_info->conf_bits_msb - 1);

  /* Record LSB and MSB of reserved_conf_bits and normal conf_bits */

  /* Print multiplexers or direct interconnect*/
  side_cnt = 0;
  for (side = 0; side < cur_cb_info->num_sides; side++) {
    /* Bypass side with zero IPINs*/
    if (0 == cur_cb_info->num_ipin_rr_nodes[side]) {
      continue;
    }
    side_cnt++;
    assert(0 < cur_cb_info->num_ipin_rr_nodes[side]);
    assert(NULL != cur_cb_info->ipin_rr_node[side]);
    for (inode = 0; inode < cur_cb_info->num_ipin_rr_nodes[side]; inode++) { 
      dump_verilog_connection_box_interc(cur_sram_orgz_info, fp, cur_cb_info, 
                                         cur_cb_info->ipin_rr_node[side][inode],
                                         is_explicit_mapping);
    }
  }

  fprintf(fp, "endmodule\n");

  /* Comment lines */
  switch(cur_cb_info->type) {
  case CHANX:
    fprintf(fp, "//----- END Verilog Module of Connection Box -X direction [%d][%d] -----\n\n", x, y);
    break;
  case CHANY:
    fprintf(fp, "//----- END Verilog Module of Connection Box -Y direction [%d][%d] -----\n\n", x, y);
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid type of channel!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Check */
  assert(esti_sram_cnt == get_sram_orgz_info_num_mem_bit(cur_sram_orgz_info));

  /* Close file handler */
  fclose(fp);

  /* Add fname to the linked list */
  routing_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(routing_verilog_subckt_file_path_head, fname);  

  /* Free */
  my_free(fname);
 
  return;
}

/*********************************************************************
 * Top-level function: 
 * Build the Verilog modules for global routing architecture
 * 1. Routing channels
 * 2. Switch blocks
 * 3. Connection blocks
 *
 * This function supports two styles in Verilog generation:
 * 1. Explicit port mapping
 * 2. Inexplicit port mapping
 *
 * This function also supports high hierarchical Verilog generation
 * (when the compact_routing_hierarchy is set true)
 * In this mode, Verilog generation will be done for only those
 * unique modules in terms of internal logics
 *********************************************************************/
void print_verilog_routing_resources(ModuleManager& module_manager,
                                     t_sram_orgz_info* cur_sram_orgz_info,
                                     char* verilog_dir,
                                     char* subckt_dir,
                                     const t_arch& arch,
                                     const t_det_routing_arch& routing_arch,
                                     int LL_num_rr_nodes, t_rr_node* LL_rr_node, /* To be replaced by RRGraph object */
                                     t_ivec*** LL_rr_node_indices,
                                     t_rr_indexed_data* LL_rr_indexed_data,
                                     const t_fpga_spice_opts& FPGA_SPICE_Opts) {
  VTR_ASSERT (UNI_DIRECTIONAL == routing_arch.directionality);
  
  boolean compact_routing_hierarchy = FPGA_SPICE_Opts.compact_routing_hierarchy;
  boolean explicit_port_mapping = FPGA_SPICE_Opts.SynVerilogOpts.dump_explicit_verilog;

  /* Create a vector of switch infs. TODO: this should be replaced switch objects!!! */
  std::vector<t_switch_inf> rr_switches;
  for (short i = 0; i < routing_arch.num_switch; ++i) {
    rr_switches.push_back(switch_inf[i]);
  }

  /* Two major tasks: 
   * 1. Generate sub-circuits for Routing Channels 
   * 2. Generate sub-circuits for Switch Boxes
   */ 
  /* Now: First task: Routing channels
   * Sub-circuits are named as chanx[ix][iy] or chany[ix][iy] for horizontal or vertical channels
   * each channels consist of a number of routing tracks. (Actually they are metal wires)
   * We only support single-driver routing architecture. 
   * The direction is defined as INC_DIRECTION ------> and DEC_DIRECTION <-------- for chanx
   * The direction is defined as INC_DIRECTION /|\ and DEC_DIRECTION | for chany
   *                                            |                    |
   *                                            |                    |
   *                                            |                   \|/
   * For INC_DIRECTION chanx, the inputs are at the left of channels, the outputs are at the right of channels
   * For DEC_DIRECTION chanx, the inputs are at the right of channels, the outputs are at the left of channels
   * For INC_DIRECTION chany, the inputs are at the bottom of channels, the outputs are at the top of channels
   * For DEC_DIRECTION chany, the inputs are at the top of channels, the outputs are at the bottom of channels
   */
  if (TRUE == compact_routing_hierarchy) { 
    /* Call all the unique mirrors in a DeviceRRChan */
    vpr_printf(TIO_MESSAGE_INFO, "Writing X-direction Channels...\n");
    /* X - channels [1...nx][0..ny]*/
    for (size_t ichan = 0; ichan < device_rr_chan.get_num_modules(CHANX); ++ichan) {
      dump_verilog_routing_chan_subckt(verilog_dir, subckt_dir, 
                                       ichan, device_rr_chan.get_module(CHANX, ichan));

      print_verilog_routing_unique_chan_subckt(module_manager, std::string(verilog_dir), std::string(subckt_dir), 
                                               ichan, device_rr_chan.get_module(CHANX, ichan));
    }
    /* Y - channels [1...ny][0..nx]*/
    vpr_printf(TIO_MESSAGE_INFO, "Writing Y-direction Channels...\n");
    for (size_t ichan = 0; ichan < device_rr_chan.get_num_modules(CHANY); ++ichan) {
      dump_verilog_routing_chan_subckt(verilog_dir, subckt_dir, 
                                       ichan, device_rr_chan.get_module(CHANY, ichan));

      print_verilog_routing_unique_chan_subckt(module_manager, std::string(verilog_dir), std::string(subckt_dir), 
                                               ichan, device_rr_chan.get_module(CHANY, ichan));
    }
  } else { 
    /* Output the full array of routing channels */
    vpr_printf(TIO_MESSAGE_INFO, "Writing X-direction Channels...\n");
    for (int iy = 0; iy < (ny + 1); iy++) {
      for (int ix = 1; ix < (nx + 1); ix++) {
        dump_verilog_routing_chan_subckt(verilog_dir, subckt_dir, ix, iy, CHANX, 
                                         LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices, LL_rr_indexed_data, 
                                         arch.num_segments);

      }
    }
    /* Y - channels [1...ny][0..nx]*/
    vpr_printf(TIO_MESSAGE_INFO, "Writing Y-direction Channels...\n");
    for (int ix = 0; ix < (nx + 1); ix++) {
      for (int iy = 1; iy < (ny + 1); iy++) {
        dump_verilog_routing_chan_subckt(verilog_dir, subckt_dir, ix, iy, CHANY,
                                         LL_num_rr_nodes, LL_rr_node, LL_rr_node_indices, LL_rr_indexed_data, 
                                         arch.num_segments);

      }
    }
  }

  /* Switch Boxes*/
  if (TRUE == compact_routing_hierarchy) { 
    /* Create a snapshot on sram_orgz_info */
    t_sram_orgz_info* stamped_sram_orgz_info = snapshot_sram_orgz_info(cur_sram_orgz_info);

    /* Output unique modules */
    for (size_t isb = 0; isb < device_rr_gsb.get_num_sb_unique_module(); ++isb) {
      const RRGSB& unique_mirror = device_rr_gsb.get_sb_unique_module(isb);
      dump_verilog_routing_switch_box_unique_subckt(cur_sram_orgz_info, verilog_dir,
                                                    subckt_dir, unique_mirror, explicit_port_mapping);
    }

    /* Restore sram_orgz_info to the base */ 
    copy_sram_orgz_info (cur_sram_orgz_info, stamped_sram_orgz_info);

    DeviceCoordinator sb_range = device_rr_gsb.get_gsb_range();
    for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
      for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
        const RRGSB& rr_sb = device_rr_gsb.get_gsb(ix, iy);
        update_routing_switch_box_conf_bits(cur_sram_orgz_info, rr_sb);
      }
    }
    /* Free */
    free_sram_orgz_info(stamped_sram_orgz_info, stamped_sram_orgz_info->type);
  } else {
    for (int ix = 0; ix < (nx + 1); ix++) {
      for (int iy = 0; iy < (ny + 1); iy++) {
        /* vpr_printf(TIO_MESSAGE_INFO, "Writing Switch Boxes[%d][%d]...\n", ix, iy); */
        update_spice_models_routing_index_low(ix, iy, SOURCE, arch.spice->num_spice_model, arch.spice->spice_models);
        dump_verilog_routing_switch_box_subckt(cur_sram_orgz_info, verilog_dir, 
                                               subckt_dir, &(sb_info[ix][iy]),
                                               compact_routing_hierarchy,
                                               explicit_port_mapping);
        update_spice_models_routing_index_high(ix, iy, SOURCE, arch.spice->num_spice_model, arch.spice->spice_models);
      }
    }
  }

  /* Connection Boxes */
  if (TRUE == compact_routing_hierarchy) { 
    /* Create a snapshot on sram_orgz_info */
    t_sram_orgz_info* stamped_sram_orgz_info = snapshot_sram_orgz_info(cur_sram_orgz_info);

    DeviceCoordinator cb_range = device_rr_gsb.get_gsb_range();

    /* X - channels [1...nx][0..ny]*/
    for (size_t icb = 0; icb < device_rr_gsb.get_num_cb_unique_module(CHANX); ++icb) {
      const RRGSB& unique_mirror = device_rr_gsb.get_cb_unique_module(CHANX, icb);
      dump_verilog_routing_connection_box_unique_module(cur_sram_orgz_info, 
                             verilog_dir, subckt_dir, unique_mirror, CHANX,
                             explicit_port_mapping);

    }

    /* Y - channels [1...ny][0..nx]*/
    for (size_t icb = 0; icb < device_rr_gsb.get_num_cb_unique_module(CHANY); ++icb) {
      const RRGSB& unique_mirror = device_rr_gsb.get_cb_unique_module(CHANY, icb);
      dump_verilog_routing_connection_box_unique_module(cur_sram_orgz_info, 
                             verilog_dir, subckt_dir, unique_mirror, CHANY,
                             explicit_port_mapping);

    }

    /* Restore sram_orgz_info to the base */ 
    copy_sram_orgz_info (cur_sram_orgz_info, stamped_sram_orgz_info);

    /* TODO: when we follow a tile organization, 
     * updating the conf bits should follow a tile organization: CLB, SB and CBX, CBY */
    for (size_t ix = 0; ix < cb_range.get_x(); ++ix) {
      for (size_t iy = 0; iy < cb_range.get_y(); ++iy) {
        const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
        update_routing_connection_box_conf_bits(cur_sram_orgz_info, rr_gsb, CHANX);
        update_routing_connection_box_conf_bits(cur_sram_orgz_info, rr_gsb, CHANY);
      }
    }

    /* Free */
    free_sram_orgz_info(stamped_sram_orgz_info, stamped_sram_orgz_info->type);
  } else {
    /* X - channels [1...nx][0..ny]*/
    for (int iy = 0; iy < (ny + 1); iy++) {
      for (int ix = 1; ix < (nx + 1); ix++) {
        /* vpr_printf(TIO_MESSAGE_INFO, "Writing X-direction Connection Boxes[%d][%d]...\n", ix, iy); */
        update_spice_models_routing_index_low(ix, iy, CHANX, arch.spice->num_spice_model, arch.spice->spice_models);
        if ((TRUE == is_cb_exist(CHANX, ix, iy))
           &&(0 < count_cb_info_num_ipin_rr_nodes(cbx_info[ix][iy]))) {
          dump_verilog_routing_connection_box_subckt(cur_sram_orgz_info, 
                                                     verilog_dir, subckt_dir, 
                                                     &(cbx_info[ix][iy]),
                                                     compact_routing_hierarchy,
                                                     explicit_port_mapping);
        }
        update_spice_models_routing_index_high(ix, iy, CHANX, arch.spice->num_spice_model, arch.spice->spice_models);
      }
    }
    /* Y - channels [1...ny][0..nx]*/
    for (int ix = 0; ix < (nx + 1); ix++) {
      for (int iy = 1; iy < (ny + 1); iy++) {
        /* vpr_printf(TIO_MESSAGE_INFO, "Writing Y-direction Connection Boxes[%d][%d]...\n", ix, iy); */
        update_spice_models_routing_index_low(ix, iy, CHANY, arch.spice->num_spice_model, arch.spice->spice_models);
        if ((TRUE == is_cb_exist(CHANY, ix, iy)) 
           &&(0 < count_cb_info_num_ipin_rr_nodes(cby_info[ix][iy]))) {
          dump_verilog_routing_connection_box_subckt(cur_sram_orgz_info, 
                                                     verilog_dir, subckt_dir, 
                                                     &(cby_info[ix][iy]),
                                                     compact_routing_hierarchy,
                                                     explicit_port_mapping);
        }
        update_spice_models_routing_index_high(ix, iy, CHANY, arch.spice->num_spice_model, arch.spice->spice_models);
      }
    }
  }

  /* Output a header file for all the routing blocks */
  vpr_printf(TIO_MESSAGE_INFO,"Generating header file for routing submodules...\n");
  dump_verilog_subckt_header_file(routing_verilog_subckt_file_path_head,
                                  subckt_dir,
                                  routing_verilog_file_name);
  
  return;
}


/******************************************************************** 
 * Print the sub-circuit of a connection Box (Type: [CHANX|CHANY])
 * Actually it is very similiar to switch box but
 * the difference is connection boxes connect Grid INPUT Pins to channels
 * NOTE: direct connection between CLBs should NOT be included inside this
 *       module! They should be added in the top-level module as their connection
 *       is not limited to adjacent CLBs!!!
 *
 * Location of a X- and Y-direction Connection Block in FPGA fabric
 *               +------------+       +-------------+ 
 *               |            |------>|             |
 *               |     CLB    |<------| Y-direction |
 *               |            | ...   |  Connection |
 *               |            |------>|    Block    |
 *               +------------+       +-------------+
 *                 |  ^ ... |            | ^ ... |
 *                 v  |     v            v |     v
 *           +-------------------+    +-------------+
 *       --->|                   |--->|             |
 *       <---|     X-direction   |<---|    Switch   |                           
 *        ...|  Connection block |... |    Block    |
 *       --->|                   |--->|             |
 *           +-------------------+    +-------------+
 *
 *  Internal structure:
 *  This is an example of a X-direction connection block  
 *  Note that middle output ports are shorted wire from inputs of routing tracks, 
 *  which are also the inputs of routing multiplexer of the connection block 
 *
 *                      CLB Input Pins
 *                         (IPINs)
 *                       ^   ^     ^
 *                       |   | ... |
 *              +--------------------------+
 *              |       ^    ^     ^       |
 *              |       |    | ... |       |   
 *              |  +--------------------+  | 
 *              |  |       routing      |  |
 *              |  |    multiplexers    |  |
 *              |  +--------------------+  | 
 *              |      middle outputs      |
 *              |    of routing channel    |
 *              |    ^ ^ ^ ^     ^ ^ ^ ^   |
 *              |    | | | | ... | | | |   |
 *     in[0] -->|------------------------->|---> out[0]
 *    out[1] <--|<-------------------------|<--- in[1]
 *              |           ...            |
 *   in[W-2] -->|------------------------->|---> out[W-2]
 *  out[W-1] <--|<-------------------------|<--- in[W-1]
 *              +--------------------------+
 *
 *  W: routing channel width
 *              
 ********************************************************************/
static 
void print_verilog_routing_connection_box_unique_module(ModuleManager& module_manager, 
                                                        const std::string& verilog_dir, 
                                                        const std::string& subckt_dir, 
                                                        const RRGSB& rr_gsb,
                                                        const t_rr_type& cb_type,
                                                        const bool& use_explicit_port_map) {
  /* Create the netlist */
  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
  std::string verilog_fname(subckt_dir + generate_connection_block_netlist_name(cb_type, gsb_coordinate, std::string(verilog_netlist_file_postfix)));
  /* TODO: remove the bak file when the file is ready */
  verilog_fname += ".bak";

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  print_verilog_file_header(fp, std::string("Verilog modules for Unique Connection Blocks[" + std::to_string(rr_gsb.get_cb_x(cb_type)) + "]["+ std::to_string(rr_gsb.get_cb_y(cb_type)) + "]")); 

  /* Print preprocessing flags */
  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId cb_module = module_manager.find_module(generate_connection_block_module_name(cb_type, gsb_coordinate)); 
  VTR_ASSERT(true == module_manager.valid_module_id(cb_module));

  /* Write the verilog module */
  write_verilog_module_to_file(fp, module_manager, cb_module, use_explicit_port_map);
 
  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Close file handler */
  fp.close();

  /* Add fname to the linked list */
  /*
  routing_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(routing_verilog_subckt_file_path_head, fname);  
   */
}

/*********************************************************************
 * Generate the Verilog module for a Switch Box.
 * A Switch Box module consists of following ports:
 * 1. Channel Y [x][y] inputs 
 * 2. Channel X [x+1][y] inputs
 * 3. Channel Y [x][y-1] outputs
 * 4. Channel X [x][y] outputs
 * 5. Grid[x][y+1] Right side outputs pins
 * 6. Grid[x+1][y+1] Left side output pins
 * 7. Grid[x+1][y+1] Bottom side output pins
 * 8. Grid[x+1][y] Top side output pins
 * 9. Grid[x+1][y] Left side output pins
 * 10. Grid[x][y] Right side output pins
 * 11. Grid[x][y] Top side output pins
 * 12. Grid[x][y+1] Bottom side output pins
 *
 * Location of a Switch Box in FPGA fabric:
 *
 *    --------------          --------------
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |  [x][y+1]  | [x][y+1] | [x+1][y+1] |
 *    |            |          |            |
 *    --------------          --------------
 *                  ----------
 *       ChanX      | Switch |     ChanX 
 *       [x][y]     |   Box  |    [x+1][y]
 *                  | [x][y] |
 *                  ----------
 *    --------------          --------------
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |   [x][y]   |  [x][y]  |  [x+1][y]  |
 *    |            |          |            |
 *    --------------          --------------
 *
 * Switch Block pin location map
 *
 *                       Grid[x][y+1]   ChanY[x][y+1]  Grid[x+1][y+1] 
 *                        right_pins  inputs/outputs     left_pins
 *                            |             ^                |
 *                            |             |                |
 *                            v             v                v
 *                    +-----------------------------------------------+
 *                    |                                               |
 *    Grid[x][y+1]    |                                               |    Grid[x+1][y+1]
 *    bottom_pins---->|                                               |<---- bottom_pins
 *                    |                                               |
 * ChanX[x][y]        |              Switch Box [x][y]                |     ChanX[x+1][y]
 * inputs/outputs<--->|                                               |<---> inputs/outputs
 *                    |                                               |
 *    Grid[x][y+1]    |                                               |    Grid[x+1][y+1]
 *       top_pins---->|                                               |<---- top_pins
 *                    |                                               |
 *                    +-----------------------------------------------+
 *                            ^             ^                ^
 *                            |             |                |
 *                            |             v                |
 *                       Grid[x][y]     ChanY[x][y]      Grid[x+1][y] 
 *                       right_pins    inputs/outputs      left_pins
 *
 *
 ********************************************************************/
static 
void print_verilog_routing_switch_box_unique_module(ModuleManager& module_manager, 
                                                    const std::string& verilog_dir, 
                                                    const std::string& subckt_dir, 
                                                    const RRGSB& rr_gsb,
                                                    const bool& use_explicit_port_map) {
  /* Create the netlist */
  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  std::string verilog_fname(subckt_dir + generate_routing_block_netlist_name(sb_verilog_file_name_prefix, gsb_coordinate, std::string(verilog_netlist_file_postfix)));
  /* TODO: remove the bak file when the file is ready */
  verilog_fname += ".bak";

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  print_verilog_file_header(fp, std::string("Verilog modules for Unique Switch Blocks[" + std::to_string(rr_gsb.get_sb_x()) + "]["+ std::to_string(rr_gsb.get_sb_y()) + "]")); 

  /* Print preprocessing flags */
  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId sb_module = module_manager.find_module(generate_switch_block_module_name(gsb_coordinate)); 
  VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

  /* Write the verilog module */
  write_verilog_module_to_file(fp, module_manager, sb_module, use_explicit_port_map);
 
  /* Close file handler */
  fp.close();

  /* Add fname to the linked list */
  /*
  routing_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(routing_verilog_subckt_file_path_head, verilog_fname.c_str());  
   */

  return;
}


/********************************************************************
 * Iterate over all the connection blocks in a device
 * and build a module for each of them 
 *******************************************************************/
static 
void print_verilog_flatten_connection_block_modules(ModuleManager& module_manager, 
                                                    const DeviceRRGSB& L_device_rr_gsb,
                                                    const std::string& verilog_dir,
                                                    const std::string& subckt_dir,
                                                    const t_rr_type& cb_type,
                                                    const bool& use_explicit_port_map) {
  /* Build unique X-direction connection block modules */
  DeviceCoordinator cb_range = L_device_rr_gsb.get_gsb_range();

  for (size_t ix = 0; ix < cb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < cb_range.get_y(); ++iy) {
      /* Check if the connection block exists in the device!
       * Some of them do NOT exist due to heterogeneous blocks (height > 1) 
       * We will skip those modules
       */
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      if ( (TRUE != is_cb_exist(CHANX, rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type)))
        || (true != rr_gsb.is_cb_exist(cb_type))) {
        continue;
      }
      print_verilog_routing_connection_box_unique_module(module_manager, 
                                                         verilog_dir,
                                                         subckt_dir, 
                                                         rr_gsb, cb_type,  
                                                         use_explicit_port_map);
    }
  }
}

/********************************************************************
 * A top-level function of this file
 * Print all the modules for global routing architecture of a FPGA fabric
 * in Verilog format  in a flatten way:
 *   Each connection block and switch block will be generated as a unique module
 * Covering:
 * 1. Connection blocks
 * 2. Switch blocks
 *******************************************************************/
void print_verilog_flatten_routing_modules(ModuleManager& module_manager,
                                           const DeviceRRGSB& L_device_rr_gsb,
                                           const t_det_routing_arch& routing_arch,
                                           const std::string& verilog_dir,
                                           const std::string& subckt_dir,
                                           const bool& use_explicit_port_map) {
  /* We only support uni-directional routing architecture now */
  VTR_ASSERT (UNI_DIRECTIONAL == routing_arch.directionality);

  /* TODO: deprecate DeviceCoordinator, use vtr::Point<size_t> only! */
  DeviceCoordinator sb_range = L_device_rr_gsb.get_gsb_range();

  /* Build unique switch block modules */
  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      print_verilog_routing_switch_box_unique_module(module_manager, 
                                                     verilog_dir,
                                                     subckt_dir, 
                                                     rr_gsb, 
                                                     use_explicit_port_map);
    }
  }

  print_verilog_flatten_connection_block_modules(module_manager, L_device_rr_gsb, verilog_dir, subckt_dir, CHANX, use_explicit_port_map);

  print_verilog_flatten_connection_block_modules(module_manager, L_device_rr_gsb, verilog_dir, subckt_dir, CHANY, use_explicit_port_map);

}


/********************************************************************
 * A top-level function of this file
 * Print all the unique modules for global routing architecture of a FPGA fabric
 * in Verilog format, including: 
 * 1. Connection blocks
 * 2. Switch blocks
 *
 * Note: this function SHOULD be called only when 
 * the option compact_routing_hierarchy is turned on!!!
 *******************************************************************/
void print_verilog_unique_routing_modules(ModuleManager& module_manager,
                                          const DeviceRRGSB& L_device_rr_gsb,
                                          const t_det_routing_arch& routing_arch,
                                          const std::string& verilog_dir,
                                          const std::string& subckt_dir,
                                          const bool& use_explicit_port_map) {
  /* We only support uni-directional routing architecture now */
  VTR_ASSERT (UNI_DIRECTIONAL == routing_arch.directionality);

  /* Build unique switch block modules */
  for (size_t isb = 0; isb < L_device_rr_gsb.get_num_sb_unique_module(); ++isb) {
    const RRGSB& unique_mirror = L_device_rr_gsb.get_sb_unique_module(isb);
    print_verilog_routing_switch_box_unique_module(module_manager, 
                                                   verilog_dir,
                                                   subckt_dir, 
                                                   unique_mirror, 
                                                   use_explicit_port_map);
  }

  /* Build unique X-direction connection block modules */
  for (size_t icb = 0; icb < L_device_rr_gsb.get_num_cb_unique_module(CHANX); ++icb) {
    const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(CHANX, icb);

    print_verilog_routing_connection_box_unique_module(module_manager, 
                                                       verilog_dir,
                                                       subckt_dir, 
                                                       unique_mirror, CHANX,  
                                                       use_explicit_port_map);
  }

  /* Build unique X-direction connection block modules */
  for (size_t icb = 0; icb < L_device_rr_gsb.get_num_cb_unique_module(CHANY); ++icb) {
    const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(CHANY, icb);

    print_verilog_routing_connection_box_unique_module(module_manager, 
                                                       verilog_dir,
                                                       subckt_dir, 
                                                       unique_mirror, CHANY,  
                                                       use_explicit_port_map);
  }
}
