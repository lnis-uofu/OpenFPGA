/************************************************************************
 *  This file contains functions that are used to allocate nodes 
 *  for the tileable routing resource graph builder
 ***********************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_geometry.h"

/* Headers from openfpgautil library */
#include "openfpga_side_manager.h"

#include "vpr_types.h"
#include "vpr_utils.h"

#include "rr_node.h"

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
void alloc_tileable_rr_graph_nodes(RRGraph& rr_graph,
                                   vtr::vector<RRNodeId, RRSwitchId>& rr_node_driver_switches, 
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

  rr_node_driver_switches.reserve(num_nodes); 
}

/************************************************************************
 * Configure OPIN rr_nodes for this grid 
 * coordinates: xlow, ylow, xhigh, yhigh, 
 * features: capacity, ptc_num (pin_num),
 *
 * Note: this function should be applied ONLY to grid with 0 width offset and 0 height offset!!!
 ***********************************************************************/
static 
void load_one_grid_opin_nodes_basic_info(RRGraph& rr_graph,
                                         vtr::vector<RRNodeId, RRSwitchId>& rr_node_driver_switches, 
                                         const vtr::Point<size_t>& grid_coordinate, 
                                         const t_grid_tile& cur_grid, 
                                         const e_side& io_side, 
                                         const RRSwitchId& delayless_switch) {
  SideManager io_side_manager(io_side);

  /* Walk through the width height of each grid,
   * get pins and configure the rr_nodes
   */
  for (int width = 0; width < cur_grid.type->width; ++width) {
    for (int height = 0; height < cur_grid.type->height; ++height) {
      /* Walk through sides */
      for (size_t side = 0; side < NUM_SIDES; ++side) {
        SideManager side_manager(side);
        /* skip unwanted sides */
        if ( (true == is_io_type(cur_grid.type))
          && (side != io_side_manager.to_size_t()) ) { 
          continue;
        }
        /* Find OPINs */
        /* Configure pins by pins */
        std::vector<int> opin_list = get_grid_side_pins(cur_grid, DRIVER, side_manager.get_side(),
                                                        width, height);
        for (const int& pin_num : opin_list) {
          /* Create a new node and fill information */
          const RRNodeId& node = rr_graph.create_node(OPIN);
 
          /* node bounding box */
          rr_graph.set_node_bounding_box(node, vtr::Rect<short>(grid_coordinate.x() + width,
                                                                grid_coordinate.x() + width,
                                                                grid_coordinate.y() + height,
                                                                grid_coordinate.y() + height));
          rr_graph.set_node_side(node, side_manager.get_side());  
          rr_graph.set_node_pin_num(node, pin_num);  

          rr_graph.set_node_capacity(node, 1);

          /* cost index is a FIXED value for OPIN */
          rr_graph.set_node_cost_index(node, OPIN_COST_INDEX); 

          /* Switch info */
          VTR_ASSERT(size_t(node) == rr_node_driver_switches.size());
          rr_node_driver_switches.push_back(delayless_switch); 

          /* RC data */
          rr_graph.set_node_rc_data_index(node, find_create_rr_rc_data(0., 0.));

        } /* End of loading OPIN rr_nodes */
      } /* End of side enumeration */
    } /* End of height enumeration */
  } /* End of width enumeration */
}

/************************************************************************
 * Configure IPIN rr_nodes for this grid 
 * coordinates: xlow, ylow, xhigh, yhigh, 
 * features: capacity, ptc_num (pin_num),
 *
 * Note: this function should be applied ONLY to grid with 0 width offset and 0 height offset!!!
 ***********************************************************************/
static 
void load_one_grid_ipin_nodes_basic_info(RRGraph& rr_graph,
                                         vtr::vector<RRNodeId, RRSwitchId>& rr_node_driver_switches, 
                                         const vtr::Point<size_t>& grid_coordinate, 
                                         const t_grid_tile& cur_grid, 
                                         const e_side& io_side, 
                                         const RRSwitchId& wire_to_ipin_switch) {
  SideManager io_side_manager(io_side);

  /* Walk through the width and height of each grid,
   * get pins and configure the rr_nodes
   */
  for (int width = 0; width < cur_grid.type->width; ++width) {
    for (int height = 0; height < cur_grid.type->height; ++height) {
      /* Walk through sides */
      for (size_t side = 0; side < NUM_SIDES; ++side) {
        SideManager side_manager(side);
        /* skip unwanted sides */
        if ( (true == is_io_type(cur_grid.type))
          && (side != io_side_manager.to_size_t()) ) { 
          continue;
        }

        /* Find IPINs */
        /* Configure pins by pins */
        std::vector<int> ipin_list = get_grid_side_pins(cur_grid, RECEIVER, side_manager.get_side(), width, height);
        for (const int& pin_num : ipin_list) {
          /* Create a new node and fill information */
          const RRNodeId& node = rr_graph.create_node(IPIN);
 
          /* node bounding box */
          rr_graph.set_node_bounding_box(node, vtr::Rect<short>(grid_coordinate.x() + width,
                                                                grid_coordinate.x() + width,
                                                                grid_coordinate.y() + height,
                                                                grid_coordinate.y() + height));
          rr_graph.set_node_side(node, side_manager.get_side());  
          rr_graph.set_node_pin_num(node, pin_num);  

          rr_graph.set_node_capacity(node, 1);

          /* cost index is a FIXED value for OPIN */
          rr_graph.set_node_cost_index(node, IPIN_COST_INDEX); 

          /* Switch info */
          VTR_ASSERT(size_t(node) == rr_node_driver_switches.size());
          rr_node_driver_switches.push_back(wire_to_ipin_switch); 

          /* RC data */
          rr_graph.set_node_rc_data_index(node, find_create_rr_rc_data(0., 0.));

        } /* End of loading IPIN rr_nodes */
      } /* End of side enumeration */
    } /* End of height enumeration */
  } /* End of width enumeration */
}

/************************************************************************
 * Configure SOURCE rr_nodes for this grid 
 * coordinates: xlow, ylow, xhigh, yhigh, 
 * features: capacity, ptc_num (pin_num),
 *
 * Note: this function should be applied ONLY to grid with 0 width offset and 0 height offset!!!
 ***********************************************************************/
static 
void load_one_grid_source_nodes_basic_info(RRGraph& rr_graph,
                                           vtr::vector<RRNodeId, RRSwitchId>& rr_node_driver_switches, 
                                           const vtr::Point<size_t>& grid_coordinate, 
                                           const t_grid_tile& cur_grid, 
                                           const e_side& io_side, 
                                           const RRSwitchId& delayless_switch) {
  SideManager io_side_manager(io_side);

  /* Set a SOURCE rr_node for each DRIVER class */
  for (int iclass = 0; iclass < cur_grid.type->num_class; ++iclass) {
    /* Set a SINK rr_node for the OPIN */
    if (DRIVER != cur_grid.type->class_inf[iclass].type) {
      continue; 
    } 

    /* Create a new node and fill information */
    const RRNodeId& node = rr_graph.create_node(SOURCE);

    /* node bounding box */
    rr_graph.set_node_bounding_box(node, vtr::Rect<short>(grid_coordinate.x(),
                                                          grid_coordinate.x(),
                                                          grid_coordinate.y(),
                                                          grid_coordinate.y()));
    rr_graph.set_node_class_num(node, iclass);  

    rr_graph.set_node_capacity(node, 1);

    /* The capacity should be the number of pins in this class*/ 
    rr_graph.set_node_capacity(node, cur_grid.type->class_inf[iclass].num_pins); 

    /* cost index is a FIXED value for SOURCE */
    rr_graph.set_node_cost_index(node, SOURCE_COST_INDEX); 

    /* Switch info */
    VTR_ASSERT(size_t(node) == rr_node_driver_switches.size());
    rr_node_driver_switches.push_back(delayless_switch); 

    /* RC data */
    rr_graph.set_node_rc_data_index(node, find_create_rr_rc_data(0., 0.));

  } /* End of class enumeration */
}

/************************************************************************
 * Configure SINK rr_nodes for this grid 
 * coordinates: xlow, ylow, xhigh, yhigh, 
 * features: capacity, ptc_num (pin_num),
 *
 * Note: this function should be applied ONLY to grid with 0 width offset and 0 height offset!!!
 ***********************************************************************/
static 
void load_one_grid_sink_nodes_basic_info(RRGraph& rr_graph,
                                         vtr::vector<RRNodeId, RRSwitchId>& rr_node_driver_switches, 
                                         const vtr::Point<size_t>& grid_coordinate, 
                                         const t_grid_tile& cur_grid, 
                                         const e_side& io_side, 
                                         const RRSwitchId& delayless_switch) {
  SideManager io_side_manager(io_side);

  /* Set a SINK rr_node for each RECEIVER class */
  for (int iclass = 0; iclass < cur_grid.type->num_class; ++iclass) {
    /* Set a SINK rr_node for the OPIN */
    if (RECEIVER != cur_grid.type->class_inf[iclass].type) {
      continue; 
    }

    /* Create a new node and fill information */
    const RRNodeId& node = rr_graph.create_node(SINK);

    /* node bounding box */
    rr_graph.set_node_bounding_box(node, vtr::Rect<short>(grid_coordinate.x(),
                                                          grid_coordinate.x(),
                                                          grid_coordinate.y(),
                                                          grid_coordinate.y()));
    rr_graph.set_node_class_num(node, iclass);  

    rr_graph.set_node_capacity(node, 1);

    /* The capacity should be the number of pins in this class*/ 
    rr_graph.set_node_capacity(node, cur_grid.type->class_inf[iclass].num_pins); 

    /* cost index is a FIXED value for SINK */
    rr_graph.set_node_cost_index(node, SINK_COST_INDEX); 

    /* Switch info */
    VTR_ASSERT(size_t(node) == rr_node_driver_switches.size());
    rr_node_driver_switches.push_back(delayless_switch); 

    /* RC data */
    rr_graph.set_node_rc_data_index(node, find_create_rr_rc_data(0., 0.));

  } /* End of class enumeration */
}

/************************************************************************
 * Create all the rr_nodes for grids
 ***********************************************************************/
static 
void load_grid_nodes_basic_info(RRGraph& rr_graph,
                                vtr::vector<RRNodeId, RRSwitchId>& rr_node_driver_switches, 
                                const DeviceGrid& grids, 
                                const RRSwitchId& wire_to_ipin_switch,
                                const RRSwitchId& delayless_switch) {

  for (size_t iy = 0; iy < grids.height(); ++iy) { 
    for (size_t ix = 0; ix < grids.width(); ++ix) {
      /* Skip EMPTY tiles */
      if (true == is_empty_type(grids[ix][iy].type)) {
        continue;
      }

      /* We only build rr_nodes for grids with width_offset = 0 and height_offset = 0 */
      if ( (0 < grids[ix][iy].width_offset)
        || (0 < grids[ix][iy].height_offset) ) {
        continue;
      }

      vtr::Point<size_t> grid_coordinate(ix, iy);
      enum e_side io_side = NUM_SIDES;

      /* If this is the block on borders, we consider IO side */
      if (true == is_io_type(grids[ix][iy].type)) {
        vtr::Point<size_t> io_device_size(grids.width() - 1, grids.height() - 1);
        io_side = determine_io_grid_pin_side(io_device_size, grid_coordinate);
      }

      /* Configure source rr_nodes for this grid */
      load_one_grid_source_nodes_basic_info(rr_graph,
                                            rr_node_driver_switches, 
                                            grid_coordinate, 
                                            grids[ix][iy], 
                                            io_side, 
                                            delayless_switch);

      /* Configure sink rr_nodes for this grid */
      load_one_grid_sink_nodes_basic_info(rr_graph,
                                          rr_node_driver_switches, 
                                          grid_coordinate, 
                                          grids[ix][iy], 
                                          io_side, 
                                          delayless_switch);

      /* Configure opin rr_nodes for this grid */
      load_one_grid_opin_nodes_basic_info(rr_graph,
                                            rr_node_driver_switches, 
                                            grid_coordinate, 
                                            grids[ix][iy], 
                                            io_side, 
                                            delayless_switch);

      /* Configure ipin rr_nodes for this grid */
      load_one_grid_ipin_nodes_basic_info(rr_graph,
                                          rr_node_driver_switches, 
                                          grid_coordinate, 
                                          grids[ix][iy], 
                                          io_side, 
                                          wire_to_ipin_switch);

    }
  }
}

/************************************************************************
 * Create all the rr_nodes covering both grids and routing channels
 ***********************************************************************/
void create_tileable_rr_graph_nodes(RRGraph& rr_graph,
                                    vtr::vector<RRNodeId, RRSwitchId>& rr_node_driver_switches, 
                                    const DeviceGrid& grids, 
                                    const RRSwitchId& wire_to_ipin_switch,
                                    const RRSwitchId& delayless_switch) {
  load_grid_nodes_basic_info(rr_graph,
                             rr_node_driver_switches, 
                             grids, 
                             wire_to_ipin_switch,
                             delayless_switch);

}

} /* end namespace openfpga */
