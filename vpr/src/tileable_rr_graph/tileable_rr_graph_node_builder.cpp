/************************************************************************
 *  This file contains functions that are used to allocate nodes 
 *  for the tileable routing resource graph builder
 ***********************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_geometry.h"

#include "vpr_utils.h"

#include "rr_graph_builder_utils.h"
#include "tileable_chan_details_builder.h"
#include "tileable_rr_graph_node_builder.h"

/* begin namespace openfpga */
namespace openfpga {

/************************************************************************
 * Find the number output pins by considering all the grid
 ***********************************************************************/
static 
size_t estimate_num_grid_rr_nodes_by_type(const DeviceGrid& grids,
                                          const t_rr_type& node_type) {
  size_t num_grid_rr_nodes = 0;

  for (size_t ix = 0; ix < grids.width(); ++ix) {
    for (size_t iy = 0; iy < grids.height(); ++iy) { 

      /* Skip EMPTY tiles */
      if (true == is_empty_type(grids[ix][iy].type)) {
        continue;
      }

      /* Skip height > 1 or width > 1 tiles (mostly heterogeneous blocks) */
      if ( (0 < grids[ix][iy].width_offset)
        || (0 < grids[ix][iy].height_offset) ) {
        continue;
      }

      enum e_side io_side = NUM_SIDES;

      /* If this is the block on borders, we consider IO side */
      if (true == is_io_type(grids[ix][iy].type)) {
        vtr::Point<size_t> io_device_size(grids.width() - 1, grids.height() - 1);
        vtr::Point<size_t> grid_coordinate(ix, iy);
        io_side = determine_io_grid_pin_side(io_device_size, grid_coordinate);
      }

      switch (node_type) {
      case OPIN:
        /* get the number of OPINs */
        num_grid_rr_nodes += get_grid_num_pins(grids[ix][iy], DRIVER, io_side);
        break;
      case IPIN:
        /* get the number of IPINs */
        num_grid_rr_nodes += get_grid_num_pins(grids[ix][iy], RECEIVER, io_side);
        break;
      case SOURCE:
        /* SOURCE: number of classes whose type is DRIVER */
        num_grid_rr_nodes += get_grid_num_classes(grids[ix][iy], DRIVER);
        break;
      case SINK: 
        /* SINK: number of classes whose type is RECEIVER */
        num_grid_rr_nodes += get_grid_num_classes(grids[ix][iy], RECEIVER);
        break;
      default:
        VTR_LOGF_ERROR(__FILE__, __LINE__,
                       "Invalid routing resource node!\n");
        exit(1);
      }
    }
  }

  return num_grid_rr_nodes; 
}

/************************************************************************
 * For X-direction Channel: CHANX
 * We pair each x-direction routing channel to the grid below it
 * as they share the same coordinate
 *
 * As such, the range of CHANX coordinate starts from x = 1, y = 0
 * which is the grid (I/O) at the left bottom of the fabric
 * 
 * As such, the range of CHANX coordinate ends to x = width - 2, y = height - 2
 * which is the grid at the top right of the core fabric
 * Note that the I/O ring is 
 *
 *                             TOP SIDE OF FPGA
 *
 *           +-------------+       +-------------+        +---------------------+
 *           |     Grid    |       |     Grid    |   ...  |    Grid             |
 *           |    [1][0]   |       |    [2][0]   |        | [width-2][height-1] |
 *           +-------------+       +-------------+        +---------------------+
 *
 *           +-------------+       +-------------+        +---------------------+
 *           |  X-Channel  |       |  X-Channel  |   ...  |  X-Channel          |
 *           |    [1][0]   |       |   [2][0]    |        | [width-2][height-2] |
 *           +-------------+       +-------------+        +---------------------+
 *
 *           +-------------+       +-------------+        +---------------------+
 *           |     Grid    |       |     Grid    |   ...  |    Grid             |
 *           |    [1][0]   |       |    [2][0]   |        | [width-2][height-2] |
 *           +-------------+       +-------------+        +---------------------+
 *              
 *                 ...                   ...                    ...
 *     
 *           +-------------+       +-------------+        +--------------+
 *           |  X-Channel  |       |  X-Channel  |   ...  |  X-Channel   |
 *           |    [1][1]   |       |   [2][1]    |        | [width-2][1] |
 *           +-------------+       +-------------+        +--------------+
 *
 *  LEFT     +-------------+       +-------------+        +--------------+          RIGHT
 *  SIDE     |     Grid    |       |     Grid    |   ...  |    Grid      |          SIDE 
 *  GRID     |    [1][1]   |       |    [2][1]   |        | [width-2][1] |          GRID
 *           +-------------+       +-------------+        +--------------+
 *
 *           +-------------+       +-------------+        +--------------+
 *           |  X-Channel  |       |  X-Channel  |   ...  |  X-Channel   |
 *           |    [1][0]   |       |   [2][0]    |        | [width-2][0] |
 *           +-------------+       +-------------+        +--------------+
 *
 *           +-------------+       +-------------+        +--------------+
 *           |     Grid    |       |     Grid    |   ...  |    Grid      |
 *           |    [1][0]   |       |    [2][0]   |        | [width-2][0] |
 *           +-------------+       +-------------+        +--------------+
 *
 *                                 BOTTOM SIDE OF FPGA
 *
 *  The figure above describe how the X-direction routing channels are 
 *  organized in a homogeneous FPGA fabric 
 *  Note that we talk about general-purpose uni-directional routing architecture here 
 *  It means that a routing track may span across multiple grids
 *  However, the hard limits are as follows
 *  All the routing tracks will start at the most LEFT routing channel
 *  All the routing tracks will end at the most RIGHT routing channel
 *
 *  Things will become more complicated in terms of track starting and end
 *  in the context of heterogeneous FPGAs
 *  We may have a grid which span multiple column and rows, as exemplified in the figure below
 *  In such case,
 *  all the routing tracks [x-1][y] at the left side of the grid [x][y] are forced to end 
 *  all the routing tracks [x+2][y] at the right side of the grid [x][y] are forced to start
 *  And there are no routing tracks inside the grid[x][y]
 *  It means that X-channel [x][y] & [x+1][y] will no exist
 *
 *   +------------+     +-------------+       +-------------+        +--------------+
 *   | X-Channel  |     |  X-Channel  |       |  X-Channel  |        |  X-Channel   |
 *   | [x-1][y+2] |     |   [x][y+2]  |       | [x+1][y+2]  |        |  [x+2][y+2]  |
 *   +------------+     +-------------+       +-------------+        +--------------+
 *
 *   +------------+     +-----------------------------------+        +--------------+
 *   |    Grid    |     |                                   |        |    Grid      |
 *   | [x-1][y+1] |     |                                   |        |  [x+2][y+1]  |
 *   +------------+     |                                   |        +--------------+
 *                      |                                   |
 *   +------------+     |                                   |        +--------------+
 *   | X-channel  |     |               Grid                |        |  X-Channel   |
 *   | [x-1][y]   |     |        [x][y] - [x+1][y+1]        |        |   [x+2][y]   |
 *   +------------+     |                                   |        +--------------+
 *                      |                                   |
 *   +------------+     |                                   |        +--------------+
 *   |   Grid     |     |                                   |        |    Grid      |
 *   |  [x-1][y]  |     |                                   |        |   [x+2][y]   |
 *   +------------+     +-----------------------------------+        +--------------+
 *
 *
 *
 ***********************************************************************/
static 
size_t estimate_num_chanx_rr_nodes(const DeviceGrid& grids,
                                   const size_t& chan_width,
                                   const std::vector<t_segment_inf>& segment_infs) {
  size_t num_chanx_rr_nodes = 0;

  for (size_t iy = 0; iy < grids.height() - 1; ++iy) { 
    for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
      vtr::Point<size_t> chanx_coord(ix, iy);

      /* Bypass if the routing channel does not exist */
      if (false == is_chanx_exist(grids, chanx_coord)) {
        continue;
      }

      bool force_start = false;
      bool force_end = false;

      /* All the tracks have to start when 
       *  - the routing channel touch the RIGHT side a heterogeneous block
       *  - the routing channel touch the LEFT side of FPGA
       */
      if (true == is_chanx_right_to_multi_height_grid(grids, chanx_coord)) {
        force_start = true;
      }

      /* All the tracks have to end when 
       *  - the routing channel touch the LEFT side a heterogeneous block
       *  - the routing channel touch the RIGHT side of FPGA
       */
      if (true == is_chanx_left_to_multi_height_grid(grids, chanx_coord)) {
        force_end = true;
      }
    
      /* Evaluate if the routing channel locates in the middle of a grid */
      ChanNodeDetails chanx_details = build_unidir_chan_node_details(chan_width, grids.width() - 2, force_start, force_end, segment_infs); 
      /* When an INC_DIRECTION CHANX starts, we need a new rr_node */
      num_chanx_rr_nodes += chanx_details.get_num_starting_tracks(INC_DIRECTION);
      /* When an DEC_DIRECTION CHANX ends, we need a new rr_node */
      num_chanx_rr_nodes += chanx_details.get_num_ending_tracks(DEC_DIRECTION);
    }
  }

  return num_chanx_rr_nodes;
}

/************************************************************************
 * Estimate the number of CHANY rr_nodes for Y-direction routing channels
 * The technical rationale is very similar to the X-direction routing channel
 * Refer to the detailed explanation there
 ***********************************************************************/
static 
size_t estimate_num_chany_rr_nodes(const DeviceGrid& grids,
                                   const size_t& chan_width,
                                   const std::vector<t_segment_inf>& segment_infs) {
  size_t num_chany_rr_nodes = 0;

  for (size_t ix = 0; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) { 
      vtr::Point<size_t> chany_coord(ix, iy);

      /* Bypass if the routing channel does not exist */
      if (false == is_chany_exist(grids, chany_coord)) {
        continue;
      }

      bool force_start = false;
      bool force_end = false;

      /* All the tracks have to start when 
       *  - the routing channel touch the TOP side a heterogeneous block
       *  - the routing channel touch the BOTTOM side of FPGA
       */
      if (true == is_chany_top_to_multi_width_grid(grids, chany_coord)) {
        force_start = true;
      }

      /* All the tracks have to end when 
       *  - the routing channel touch the BOTTOM side a heterogeneous block
       *  - the routing channel touch the TOP side of FPGA
       */
      if (true == is_chany_bottom_to_multi_width_grid(grids, chany_coord)) {
        force_end = true;
      }

      ChanNodeDetails chany_details = build_unidir_chan_node_details(chan_width, grids.height() - 2, force_start, force_end, segment_infs); 
      /* When an INC_DIRECTION CHANX starts, we need a new rr_node */
      num_chany_rr_nodes += chany_details.get_num_starting_tracks(INC_DIRECTION);
      /* When an DEC_DIRECTION CHANX ends, we need a new rr_node */
      num_chany_rr_nodes += chany_details.get_num_ending_tracks(DEC_DIRECTION);
    }
  }

  return num_chany_rr_nodes;
}

/************************************************************************
 * Estimate the number of nodes by each type in a routing resource graph
 ***********************************************************************/
static 
std::vector<size_t> estimate_num_rr_nodes(const DeviceGrid& grids,
                                          const vtr::Point<size_t>& chan_width,
                                          const std::vector<t_segment_inf>& segment_infs) {

  /* Reset the OPIN, IPIN, SOURCE, SINK counter to be zero */
  std::vector<size_t> num_rr_nodes_per_type(NUM_RR_TYPES, 0);

  /**
   * 1 Find number of rr nodes related to grids
   */
  num_rr_nodes_per_type[OPIN] = estimate_num_grid_rr_nodes_by_type(grids, OPIN);
  num_rr_nodes_per_type[IPIN] = estimate_num_grid_rr_nodes_by_type(grids, IPIN);
  num_rr_nodes_per_type[SOURCE] = estimate_num_grid_rr_nodes_by_type(grids, SOURCE);
  num_rr_nodes_per_type[SINK] = estimate_num_grid_rr_nodes_by_type(grids, SINK);

  /**
   * 2. Assign the segments for each routing channel,
   *    To be specific, for each routing track, we assign a routing segment.
   *    The assignment is subject to users' specifications, such as 
   *    a. length of each type of segment
   *    b. frequency of each type of segment.
   *    c. routing channel width
   *
   *    SPECIAL for fringes:
   *    All segments will start and ends with no exception
   *
   *    IMPORTANT: we should be aware that channel width maybe different 
   *    in X-direction and Y-direction channels!!!
   *    So we will load segment details for different channels 
   */
  num_rr_nodes_per_type[CHANX] = estimate_num_chanx_rr_nodes(grids, chan_width.x(), segment_infs);
  num_rr_nodes_per_type[CHANY] = estimate_num_chany_rr_nodes(grids, chan_width.y(), segment_infs);

  return num_rr_nodes_per_type;
}

/************************************************************************
 * Allocate rr_nodes to a rr_graph object 
 * This function just allocate the memory and ensure its efficiency
 * It will NOT fill detailed information for each node!!!
 *
 * Note: ensure that there are NO nodes in the rr_graph
 ***********************************************************************/
void alloc_rr_graph_nodes(RRGraph& rr_graph,
                          const DeviceGrid& grids,
                          const vtr::Point<size_t>& chan_width,
                          const std::vector<t_segment_inf>& segment_infs) {
  VTR_ASSERT(0 == rr_graph.nodes().size());

  std::vector<size_t> num_rr_nodes_per_type = estimate_num_rr_nodes(grids, chan_width, segment_infs);

  /* Reserve the number of node to be memory efficient */  
  size_t num_nodes = 0;
  for (const size_t& num_node_per_type : num_rr_nodes_per_type) {
    num_nodes += num_node_per_type;
  }

  rr_graph.reserve_nodes(num_nodes);

  /* Add nodes by types */
  for (const t_rr_type& node_type : {SOURCE, SINK, IPIN, OPIN, CHANX, CHANY}) {
    for (size_t inode = 0; inode < num_rr_nodes_per_type[size_t(node_type)]; ++inode) {
      rr_graph.create_node(node_type);
    }
  }

  VTR_ASSERT(num_nodes == rr_graph.nodes().size());
}

} /* end namespace openfpga */
