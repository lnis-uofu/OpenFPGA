/********************************************************************
 * This file includes functions that are used to add module nets
 * for direct connections between CLBs/heterogeneous blocks
 * in the top-level module of a FPGA fabric
 *******************************************************************/
#include <algorithm>

#include "vtr_assert.h"
#include "util.h"
#include "device_port.h"

#include "fpga_x2p_naming.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "module_manager_utils.h"

#include "globals.h"
#include "verilog_global.h"

#include "build_top_module_directs.h"

/********************************************************************
 * Check if the grid coorindate given is in the device grid range 
 *******************************************************************/
static 
bool is_grid_coordinate_exist_in_device(const vtr::Point<size_t>& device_size,
                                        const vtr::Point<size_t>& grid_coordinate) {
  return (grid_coordinate < device_size);
}

/********************************************************************
 * Add module net for one direction connection between two CLBs or
 * two grids
 * This function will 
 * 1. find the pin id and port id of the source clb port in module manager
 * 2. find the pin id and port id of the destination clb port in module manager
 * 3. add a direct connection module to the top module 
 * 4. add a first module net and configure its source and sink, 
 * in order to connect the source pin to the input of the top module
 * 4. add a second module net and configure its source and sink, 
 * in order to connect the sink pin to the output of the top module
 *******************************************************************/
static 
void add_module_nets_clb2clb_direct_connection(ModuleManager& module_manager, 
                                               const ModuleId& top_module,  
                                               const CircuitLibrary& circuit_lib, 
                                               const vtr::Point<size_t>& device_size,
                                               const std::vector<std::vector<t_grid_tile>>& grids,
                                               const std::vector<std::vector<size_t>>& grid_instance_ids,
                                               const vtr::Point<size_t>& src_clb_coord, 
                                               const vtr::Point<size_t>& des_clb_coord,  
                                               const t_clb_to_clb_directs& direct) {
  /* Find the source port and destination port on the CLBs */
  BasicPort src_clb_port;
  BasicPort des_clb_port;

  src_clb_port.set_width(direct.from_clb_pin_start_index, direct.from_clb_pin_end_index);
  des_clb_port.set_width(direct.to_clb_pin_start_index, direct.to_clb_pin_end_index);

  /* Check bandwidth match between from_clb and to_clb pins */
  if (src_clb_port.get_width() != des_clb_port.get_width()) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Unmatch pin bandwidth in direct connection (name=%s)!\n",
               __FILE__, __LINE__, direct.name);
    exit(1);
  }

  /* Find the module name of source clb */
  t_type_ptr src_grid_type = grids[src_clb_coord.x()][src_clb_coord.y()].type;
  e_side src_grid_border_side = find_grid_border_side(device_size, src_clb_coord);
  std::string src_module_name_prefix(grid_verilog_file_name_prefix);
  std::string src_module_name = generate_grid_block_module_name(src_module_name_prefix, std::string(src_grid_type->name), IO_TYPE == src_grid_type, src_grid_border_side);
  ModuleId src_grid_module = module_manager.find_module(src_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(src_grid_module));
  /* Record the instance id */
  size_t src_grid_instance = grid_instance_ids[src_clb_coord.x()][src_clb_coord.y()];

  /* Find the module name of sink clb */
  t_type_ptr sink_grid_type = grids[des_clb_coord.x()][des_clb_coord.y()].type;
  e_side sink_grid_border_side = find_grid_border_side(device_size, des_clb_coord);
  std::string sink_module_name_prefix(grid_verilog_file_name_prefix);
  std::string sink_module_name = generate_grid_block_module_name(sink_module_name_prefix, std::string(sink_grid_type->name), IO_TYPE == sink_grid_type, sink_grid_border_side);
  ModuleId sink_grid_module = module_manager.find_module(sink_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(sink_grid_module));
  /* Record the instance id */
  size_t sink_grid_instance = grid_instance_ids[des_clb_coord.x()][des_clb_coord.y()];

  /* Find the module id of a direct connection module */
  std::string direct_module_name = circuit_lib.model_name(direct.circuit_model);
  ModuleId direct_module = module_manager.find_module(direct_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(direct_module));

  /* Find inputs and outputs of the direct circuit module */
  std::vector<CircuitPortId> direct_input_ports = circuit_lib.model_ports_by_type(direct.circuit_model, SPICE_MODEL_PORT_INPUT, true);
  VTR_ASSERT(1 == direct_input_ports.size());
  ModulePortId direct_input_port_id = module_manager.find_module_port(direct_module, circuit_lib.port_prefix(direct_input_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(direct_module, direct_input_port_id));
  VTR_ASSERT(1 == module_manager.module_port(direct_module, direct_input_port_id).get_width());

  std::vector<CircuitPortId> direct_output_ports = circuit_lib.model_ports_by_type(direct.circuit_model, SPICE_MODEL_PORT_OUTPUT, true);
  VTR_ASSERT(1 == direct_output_ports.size());
  ModulePortId direct_output_port_id = module_manager.find_module_port(direct_module, circuit_lib.port_prefix(direct_output_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(direct_module, direct_output_port_id));
  VTR_ASSERT(1 == module_manager.module_port(direct_module, direct_output_port_id).get_width());

  for (size_t pin_id = 0; pin_id < src_clb_port.pins().size(); ++pin_id) {
    /* Generate the pin name of source port/pin in the grid */
    size_t src_pin_height = find_grid_pin_height(grids, src_clb_coord, src_clb_port.pins()[pin_id]);
    e_side src_pin_grid_side = find_grid_pin_side(device_size, grids, src_clb_coord, src_pin_height, src_clb_port.pins()[pin_id]);
    std::string src_port_name = generate_grid_port_name(src_clb_coord, src_pin_height, src_pin_grid_side, src_clb_port.pins()[pin_id], false);
    ModulePortId src_port_id = module_manager.find_module_port(src_grid_module, src_port_name); 
    VTR_ASSERT(true == module_manager.valid_module_port_id(src_grid_module, src_port_id));
    VTR_ASSERT(1 == module_manager.module_port(src_grid_module, src_port_id).get_width());

    /* Generate the pin name of sink port/pin in the grid */
    size_t sink_pin_height = find_grid_pin_height(grids, des_clb_coord, des_clb_port.pins()[pin_id]);
    e_side sink_pin_grid_side = find_grid_pin_side(device_size, grids, des_clb_coord, sink_pin_height, des_clb_port.pins()[pin_id]);
    std::string sink_port_name = generate_grid_port_name(des_clb_coord, sink_pin_height, sink_pin_grid_side, des_clb_port.pins()[pin_id], false);
    ModulePortId sink_port_id = module_manager.find_module_port(sink_grid_module, sink_port_name); 
    VTR_ASSERT(true == module_manager.valid_module_port_id(sink_grid_module, sink_port_id));
    VTR_ASSERT(1 == module_manager.module_port(sink_grid_module, sink_port_id).get_width());

    /* Add a submodule of direct connection module to the top-level module */
    size_t direct_instance_id = module_manager.num_instance(top_module, direct_module);
    module_manager.add_child_module(top_module, direct_module);

    /* Create the 1st module net */
    ModuleNetId net_direct_src = module_manager.create_module_net(top_module); 
    /* Connect the wire between src_pin of clb and direct_instance input*/
    module_manager.add_module_net_source(top_module, net_direct_src, src_grid_module, src_grid_instance, src_port_id, 0);
    module_manager.add_module_net_sink(top_module, net_direct_src, direct_module, direct_instance_id, direct_input_port_id, 0);

    /* Create the 2nd module net */
    ModuleNetId net_direct_sink = module_manager.create_module_net(top_module); 
    /* Connect the wire between direct_instance output and sink_pin of clb */
    module_manager.add_module_net_source(top_module, net_direct_sink, direct_module, direct_instance_id, direct_output_port_id, 0);
    module_manager.add_module_net_sink(top_module, net_direct_sink, sink_grid_module, sink_grid_instance, sink_port_id, 0);
  }
}


/********************************************************************
 * Add module net of clb-to-clb direct connections to module manager
 * Note that the direct connections are not limited to CLBs only.
 * It can be more generic and thus cover all the grid types,
 * such as heterogeneous blocks
 *
 * This function supports the following type of direct connection:
 * 1. Direct connection between grids in the same column or row
 *     +------+      +------+
 *     |      |      |      |
 *     | Grid |----->| Grid |
 *     |      |      |      |
 *     +------+      +------+
 *         | direction connection 
 *         v
 *     +------+
 *     |      |
 *     | Grid |
 *     |      |
 *     +------+
 *
 *******************************************************************/
static 
void add_top_module_nets_intra_clb2clb_direct_connections(ModuleManager& module_manager, 
                                                          const ModuleId& top_module, 
                                                          const CircuitLibrary& circuit_lib, 
                                                          const vtr::Point<size_t>& device_size,
                                                          const std::vector<std::vector<t_grid_tile>>& grids,
                                                          const std::vector<std::vector<size_t>>& grid_instance_ids,
                                                          const std::vector<t_clb_to_clb_directs>& clb2clb_directs) {
  /* Scan the grid, visit each grid and apply direct connections */
  for (size_t ix = 0; ix < device_size.x(); ++ix) {
    for (size_t iy = 0; iy < device_size.y(); ++iy) {
      /* Bypass EMPTY_TYPE*/
      if ( (NULL == grids[ix][iy].type)
        || (EMPTY_TYPE == grids[ix][iy].type)) {
        continue;
      }
      /* Bypass any grid with a non-zero offset! They have been visited in the offset=0 case */
      if (0 != grids[ix][iy].offset) {
        continue;
      }
      /* Check each clb2clb directs by comparing the source and destination clb types
       * Direct connections are made only for those matched clbs
       */ 
      for (const t_clb_to_clb_directs& direct : clb2clb_directs) {
        /* Bypass unmatched clb type */
        if (grids[ix][iy].type != direct.from_clb_type) {
          continue;
        }
        
        /* See if the destination CLB is in the bound */
        vtr::Point<size_t> src_clb_coord(ix, iy);
        vtr::Point<size_t> des_clb_coord(ix + direct.x_offset, iy + direct.y_offset);
        if (false == is_grid_coordinate_exist_in_device(device_size, des_clb_coord)) {
          continue;
        }

        /* Check if the destination clb_type matches */
        if (grids[des_clb_coord.x()][des_clb_coord.y()].type == direct.to_clb_type) {
          /* Add a module net for a direct connection with the two grids in top_model */
          add_module_nets_clb2clb_direct_connection(module_manager, top_module, circuit_lib, 
                                                    device_size, grids, grid_instance_ids,
                                                    src_clb_coord, des_clb_coord,  
                                                    direct);
        }
      }
    }
  }
}

/********************************************************************
 * Find the coordinate of a grid in a specific column 
 * with a given type
 * This function will return the coordinate of the grid that satifies
 * the type requirement
 *******************************************************************/
static 
vtr::Point<size_t> find_grid_coordinate_given_type(const vtr::Point<size_t>& device_size,
                                                   const std::vector<std::vector<t_grid_tile>>& grids,
                                                   const std::vector<vtr::Point<size_t>>& candidate_coords, 
                                                   t_type_ptr wanted_grid_type) {
  for (vtr::Point<size_t> coord : candidate_coords) {
    /* If the next column is not longer in device range, we can return */
    if (false == is_grid_coordinate_exist_in_device(device_size, coord)) {
      continue;
    }
    if (wanted_grid_type == grids[coord.x()][coord.y()].type) {
      return coord;
    }
  }
  /* Return an valid coordinate */
  return vtr::Point<size_t>(size_t(-1), size_t(-1)); 
}

/********************************************************************
 * Find the coordinate of the destination clb/heterogeneous block
 * considering intra column/row direct connections in core grids
 *******************************************************************/
static 
vtr::Point<size_t> find_intra_direct_destination_coordinate(const vtr::Point<size_t>& device_size,
                                                            const std::vector<std::vector<t_grid_tile>>& grids,
                                                            const vtr::Point<size_t> src_coord,
                                                            const t_clb_to_clb_directs& direct) {
  vtr::Point<size_t> des_coord(size_t(-1), size_t(-1));
  t_type_ptr src_grid_type = grids[src_coord.x()][src_coord.y()].type;

  std::vector<size_t> first_search_space;
  std::vector<size_t> second_search_space;

  /* Cross column connection from Bottom to Top on Right 
   * The next column may NOT have the grid type we want!
   * Think about heterogeneous architecture!  
   * Our search space will start from the next column 
   * and ends at the RIGHT side of fabric 
   */
  if (P2P_DIRECT_COLUMN == direct.interconnection_type) {
    if (POSITIVE_DIR == direct.x_dir) {
     /* Our first search space will be in x-direction:
      *
      *      x      ...      nx 
      *   +-----+
      *   |Grid |  ----->
      *   +-----+
      */
      for (size_t ix = src_coord.x() + 1; ix < device_size.x() - 1; ++ix) {
        first_search_space.push_back(ix);
      }
    } else { 
      VTR_ASSERT(NEGATIVE_DIR == direct.x_dir);
     /* Our first search space will be in x-direction:
      *
      *    1   ...      x    
      *              +-----+
      *     < -------|Grid | 
      *              +-----+
      */
      for (size_t ix = src_coord.x() - 1; ix >= 1; --ix) {
        first_search_space.push_back(ix);
      }
    }

    /* Our second search space will be in y-direction:
     *
     *  +------+  
     *  | Grid | ny
     *  +------+
     *     ^     .
     *     |     .
     *     |     .
     *  +------+
     *  | Grid | 1
     *  +------+
     */
    for (size_t iy = 1 ; iy < device_size.y() - 1; ++iy) {
      second_search_space.push_back(iy);
    }

    /* For negative direction, our second search space will be in y-direction:
     *
     *  +------+  
     *  | Grid | ny
     *  +------+
     *     |     .
     *     |     .
     *     v     .
     *  +------+
     *  | Grid | 1
     *  +------+
     */
    if (POSITIVE_DIR == direct.y_dir) {
      std::reverse(second_search_space.begin(), second_search_space.end());
    }
  }


  /* Cross row connection from Bottom to Top on Right 
   * The next column may NOT have the grid type we want!
   * Think about heterogeneous architecture!  
   * Our search space will start from the next column 
   * and ends at the RIGHT side of fabric 
   */
  if (P2P_DIRECT_ROW == direct.interconnection_type) {
    if (POSITIVE_DIR == direct.y_dir) {
     /* Our first search space will be in y-direction:
      *
      *  +------+  
      *  | Grid | ny
      *  +------+
      *     ^     .
      *     |     .
      *     |     .
      *  +------+
      *  | Grid | y
      *  +------+
      */
      for (size_t iy = src_coord.y() + 1; iy < device_size.y() - 1; ++iy) {
        first_search_space.push_back(iy);
      }
    } else { 
      VTR_ASSERT(NEGATIVE_DIR == direct.y_dir);
      /* For negative y-direction,
       * Our first search space will be in y-direction:
       *
       *  +------+  
       *  | Grid | ny
       *  +------+
       *     |     .
       *     |     .
       *     v     .
       *  +------+
       *  | Grid | y
       *  +------+
       */
      for (size_t iy = src_coord.y() - 1; iy >= 1; --iy) {
        first_search_space.push_back(iy);
      }
    }

    /* Our second search space will be in x-direction:
     *
     *     1      ...     nx
     *  +------+       +------+
     *  | Grid |<------| Grid |
     *  +------+       +------+
     */
    for (size_t ix = 1 ; ix < device_size.x() - 1; ++ix) {
      second_search_space.push_back(ix);
    }

    /* For negative direction,
     * our second search space will be in x-direction:
     *
     *     1      ...     nx
     *  +------+       +------+
     *  | Grid |------>| Grid |
     *  +------+       +------+
     */
    if (POSITIVE_DIR == direct.x_dir) {
      std::reverse(second_search_space.begin(), second_search_space.end());
    }
  }

  for (size_t ix : first_search_space) {
    std::vector<vtr::Point<size_t>> next_col_row_coords;
    for (size_t iy : second_search_space) {
      if (P2P_DIRECT_COLUMN == direct.interconnection_type) {
        next_col_row_coords.push_back(vtr::Point<size_t>(ix, iy));
      } else {
        VTR_ASSERT(P2P_DIRECT_ROW == direct.interconnection_type);
        /* For cross-row connection, our search space is flipped */
        next_col_row_coords.push_back(vtr::Point<size_t>(iy, ix));
      }
    }
    vtr::Point<size_t> des_coord_cand = find_grid_coordinate_given_type(device_size, grids, next_col_row_coords, src_grid_type); 
    /* For a valid coordinate, we can return */
    if ( (size_t(-1) != des_coord_cand.x()) 
      && (size_t(-1) != des_coord_cand.y()) ) {
      return des_coord_cand;
    }
  }
  return des_coord;
}

/********************************************************************
 * Add module net of clb-to-clb direct connections to module manager
 * Note that the direct connections are not limited to CLBs only.
 * It can be more generic and thus cover all the grid types,
 * such as heterogeneous blocks
 *
 * This function supports the following type of direct connection:
 *
 * 1. Direct connections across columns and rows 
 *               +------+
 *               |      |
 *               |      v 
 *     +------+  |   +------+
 *     |      |  |   |      |
 *     | Grid |  |   | Grid |
 *     |      |  |   |      |
 *     +------+  |   +------+
 *               |
 *     +------+  |   +------+
 *     |      |  |   |      |
 *     | Grid |  |   | Grid |
 *     |      |  |   |      |
 *     +------+  |   +------+
 *        |      |
 *        +------+
 *
 * Note that: this will only apply to the core grids!
 *            I/Os or any blocks on the border of fabric are NOT supported!
 *
 *******************************************************************/
static 
void add_top_module_nets_inter_clb2clb_direct_connections(ModuleManager& module_manager, 
                                                          const ModuleId& top_module, 
                                                          const CircuitLibrary& circuit_lib, 
                                                          const vtr::Point<size_t>& device_size,
                                                          const std::vector<std::vector<t_grid_tile>>& grids,
                                                          const std::vector<std::vector<size_t>>& grid_instance_ids,
                                                          const std::vector<t_clb_to_clb_directs>& clb2clb_directs) {

  std::vector<e_side> border_sides = {TOP, RIGHT, BOTTOM, LEFT};

  /* Go through the direct connection list, see if we need intra-column/row connection here */
  for (const t_clb_to_clb_directs& direct: clb2clb_directs) {
    if ( (P2P_DIRECT_COLUMN != direct.interconnection_type)
      && (P2P_DIRECT_ROW != direct.interconnection_type)) {
      continue;
    }
    /* For cross-column connection, we will search the first valid grid in each column 
     * from y = 1 to y = ny
     *
     *   +------+
     *   | Grid |  y=ny
     *   +------+
     *      ^
     *      |  search direction (when y_dir is negative)
     *     ...
     *      |
     *   +------+
     *   | Grid |  y=1
     *   +------+
     * 
     */
    if (P2P_DIRECT_COLUMN == direct.interconnection_type) {
      for (size_t ix = 1; ix < device_size.x() - 1; ++ix) {
        std::vector<vtr::Point<size_t>> next_col_src_grid_coords;
        /* For negative y- direction, we should start from y = ny */
        for (size_t iy = 1; iy < device_size.y() - 1; ++iy) {
          next_col_src_grid_coords.push_back(vtr::Point<size_t>(ix, iy));
        }
        /* For positive y- direction, we should start from y = 1 */
        if (NEGATIVE_DIR == direct.y_dir) {
          std::reverse(next_col_src_grid_coords.begin(), next_col_src_grid_coords.end());
        }
        vtr::Point<size_t> src_clb_coord = find_grid_coordinate_given_type(device_size, grids, next_col_src_grid_coords, direct.from_clb_type); 
        /* Skip if we do not have a valid coordinate for source CLB/heterogeneous block */
        if ( (size_t(-1) == src_clb_coord.x()) 
          || (size_t(-1) == src_clb_coord.y()) ) {
          continue;
        }
        /* For a valid coordinate, we can find the coordinate of the destination clb */
         vtr::Point<size_t> des_clb_coord = find_intra_direct_destination_coordinate(device_size, grids, src_clb_coord, direct);
         /* If destination clb is valid, we should add something */
         if ( (size_t(-1) == des_clb_coord.x()) 
           || (size_t(-1) == des_clb_coord.y()) ) {
           continue;
         }
         add_module_nets_clb2clb_direct_connection(module_manager, top_module, circuit_lib, 
                                                   device_size, grids, grid_instance_ids,
                                                   src_clb_coord, des_clb_coord,  
                                                   direct);
      }
      continue; /* Go to next direct type */
    }
    
    /* Reach here, it must be a cross-row connection */
    VTR_ASSERT(P2P_DIRECT_ROW == direct.interconnection_type);
    /* For cross-row connection, we will search the first valid grid in each column 
     * from x = 1 to x = nx
     *
     *     x=1                    x=nx
     *   +------+               +------+
     *   | Grid | <--- ... ---- | Grid |
     *   +------+               +------+
     * 
     */
    for (size_t iy = 1; iy < device_size.y() - 1; ++iy) {
      std::vector<vtr::Point<size_t>> next_col_src_grid_coords;
      /* For negative x- direction, we should start from x = nx */
      for (size_t ix = 1; ix < device_size.x() - 1; ++ix) {
        next_col_src_grid_coords.push_back(vtr::Point<size_t>(ix, iy));
      }
      /* For positive x- direction, we should start from x = 1 */
      if (POSITIVE_DIR == direct.x_dir) {
        std::reverse(next_col_src_grid_coords.begin(), next_col_src_grid_coords.end());
      }
      vtr::Point<size_t> src_clb_coord = find_grid_coordinate_given_type(device_size, grids, next_col_src_grid_coords, direct.from_clb_type); 
      /* Skip if we do not have a valid coordinate for source CLB/heterogeneous block */
      if ( (size_t(-1) == src_clb_coord.x()) 
        || (size_t(-1) == src_clb_coord.y()) ) {
        continue;
      }
      /* For a valid coordinate, we can find the coordinate of the destination clb */
       vtr::Point<size_t> des_clb_coord = find_intra_direct_destination_coordinate(device_size, grids, src_clb_coord, direct);
       /* If destination clb is valid, we should add something */
       if ( (size_t(-1) == des_clb_coord.x()) 
         || (size_t(-1) == des_clb_coord.y()) ) {
         continue;
       }
       add_module_nets_clb2clb_direct_connection(module_manager, top_module, circuit_lib, 
                                                 device_size, grids, grid_instance_ids,
                                                 src_clb_coord, des_clb_coord,  
                                                 direct);
    }
  }
}

/********************************************************************
 * Add module net of clb-to-clb direct connections to module manager
 * Note that the direct connections are not limited to CLBs only.
 * It can be more generic and thus cover all the grid types,
 * such as heterogeneous blocks
 *******************************************************************/
void add_top_module_nets_clb2clb_direct_connections(ModuleManager& module_manager, 
                                                    const ModuleId& top_module, 
                                                    const CircuitLibrary& circuit_lib, 
                                                    const vtr::Point<size_t>& device_size,
                                                    const std::vector<std::vector<t_grid_tile>>& grids,
                                                    const std::vector<std::vector<size_t>>& grid_instance_ids,
                                                    const std::vector<t_clb_to_clb_directs>& clb2clb_directs) {

  add_top_module_nets_intra_clb2clb_direct_connections(module_manager, top_module, circuit_lib, 
                                                       device_size, grids, grid_instance_ids,
                                                       clb2clb_directs);

  add_top_module_nets_inter_clb2clb_direct_connections(module_manager, top_module, circuit_lib, 
                                                       device_size, grids, grid_instance_ids,
                                                       clb2clb_directs);
}

