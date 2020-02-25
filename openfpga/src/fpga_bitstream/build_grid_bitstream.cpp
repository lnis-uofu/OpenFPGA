/********************************************************************
 * This file includes functions that are used for building bitstreams
 * for grids (CLBs, heterogenerous blocks, I/Os, etc.)
 *******************************************************************/
#include <string>

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from vpr library */
#include "vpr_utils.h"

#include "mux_utils.h"

#include "circuit_library_utils.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"
#include "mux_bitstream_constants.h"
#include "pb_type_utils.h"

#include "build_mux_bitstream.h"
//#include "build_lut_bitstream.h"
#include "build_grid_bitstream.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Decode mode bits "01..." to a bitstream vector
 *******************************************************************/
static 
std::vector<bool> generate_mode_select_bitstream(const std::vector<size_t>& mode_bits) {
  std::vector<bool> mode_select_bitstream;
 
  for (const size_t& mode_bit : mode_bits) {
    /* Error out for unexpected bits */
    VTR_ASSERT((0 == mode_bit) || (1 == mode_bit));
    mode_select_bitstream.push_back(1 == mode_bit);
  }

  return mode_select_bitstream;
}

/********************************************************************
 * Generate bitstream for a primitive node and add it to bitstream manager
 *******************************************************************/
static 
void build_primitive_bitstream(BitstreamManager& bitstream_manager,
                               const ConfigBlockId& parent_configurable_block,
                               const ModuleManager& module_manager,
                               const CircuitLibrary& circuit_lib,
                               const VprDeviceAnnotation& device_annotation,
                               const PhysicalPb& physical_pb,
                               const PhysicalPbId& primitive_pb_id,
                               t_pb_type* primitive_pb_type) {

  /* Ensure a valid physical pritimive pb */ 
  if (nullptr == primitive_pb_type) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid primitive_pb_type!\n");
    exit(1);
  }

  CircuitModelId primitive_model = device_annotation.pb_type_circuit_model(primitive_pb_type);
  VTR_ASSERT(CircuitModelId::INVALID() != primitive_model);
  VTR_ASSERT( (CIRCUIT_MODEL_IOPAD == circuit_lib.model_type(primitive_model))
           || (CIRCUIT_MODEL_HARDLOGIC == circuit_lib.model_type(primitive_model))
           || (CIRCUIT_MODEL_FF == circuit_lib.model_type(primitive_model)) );

  /* Find SRAM ports for mode-selection */
  std::vector<CircuitPortId> primitive_mode_select_ports = find_circuit_mode_select_sram_ports(circuit_lib, primitive_model);

  /* We may have a port for mode select or not. */
  VTR_ASSERT( (0 == primitive_mode_select_ports.size())
           || (1 == primitive_mode_select_ports.size()) );

  /* Generate bitstream for mode-select ports */
  if (0 == primitive_mode_select_ports.size()) {
    return; /* Nothing to do, return directly */
  }

  std::vector<bool> mode_select_bitstream;
  if (true == physical_pb.valid_pb_id(primitive_pb_id)) {
    mode_select_bitstream = generate_mode_select_bitstream(physical_pb.mode_bits(primitive_pb_id));
  } else { /* get default mode_bits */
    mode_select_bitstream = generate_mode_select_bitstream(device_annotation.pb_type_mode_bits(primitive_pb_type));
  }

  /* Ensure the length of bitstream matches the side of memory circuits */
  std::vector<CircuitModelId> sram_models = find_circuit_sram_models(circuit_lib, primitive_model);
  VTR_ASSERT(1 == sram_models.size());
  std::string mem_block_name = generate_memory_module_name(circuit_lib, primitive_model, sram_models[0], std::string(MEMORY_MODULE_POSTFIX));
  ModuleId mem_module = module_manager.find_module(mem_block_name);
  VTR_ASSERT (true == module_manager.valid_module_id(mem_module));
  ModulePortId mem_out_port_id = module_manager.find_module_port(mem_module, generate_configuration_chain_data_out_name());
  VTR_ASSERT(mode_select_bitstream.size() == module_manager.module_port(mem_module, mem_out_port_id).get_width());

  /* Create a block for the bitstream which corresponds to the memory module associated to the LUT */
  ConfigBlockId mem_block = bitstream_manager.add_block(mem_block_name);
  bitstream_manager.add_child_block(parent_configurable_block, mem_block);

  /* Add the bitstream to the bitstream manager */
  for (const bool& bit : mode_select_bitstream) {
    ConfigBitId config_bit = bitstream_manager.add_bit(bit);
    /* Link the memory bits to the mux mem block */
    bitstream_manager.add_bit_to_block(mem_block, config_bit);
  }
}

/********************************************************************
 * This function generates bitstream for a physical block, which is 
 * a child block of a grid 
 * This function will follow a recursive way in generating bitstreams
 * It will follow the same sequence in visiting all the sub blocks
 * in a physical as we did during module generation
 *
 * Note: if you want to bind your bitstream with a FPGA fabric generated by FPGA-X2P
 * Please follow the same sequence in visiting pb_graph nodes!!!
 * For more details, you may refer to function rec_build_physical_block_modules()
 *******************************************************************/
static 
void rec_build_physical_block_bitstream(BitstreamManager& bitstream_manager,
                                        const ConfigBlockId& parent_configurable_block,
                                        const ModuleManager& module_manager,
                                        const CircuitLibrary& circuit_lib,
                                        const MuxLibrary& mux_lib,
                                        const VprDeviceAnnotation& device_annotation,
                                        const e_side& border_side,
                                        const PhysicalPb& physical_pb, 
                                        const PhysicalPbId& pb_id, 
                                        t_pb_graph_node* physical_pb_graph_node,
                                        const size_t& pb_graph_node_index) {
  /* Get the physical pb_type that is linked to the pb_graph node */
  t_pb_type* physical_pb_type = physical_pb_graph_node->pb_type;

  /* Find the mode that define_idle_mode*/
  t_mode* physical_mode = device_annotation.physical_mode(physical_pb_type);

  /* Create a block for the physical block under the grid block in bitstream manager */
  std::string pb_block_name = generate_physical_block_instance_name(physical_pb_type, pb_graph_node_index);
  ConfigBlockId pb_configurable_block = bitstream_manager.add_block(pb_block_name);
  bitstream_manager.add_child_block(parent_configurable_block, pb_configurable_block);

  /* Recursively finish all the child pb_types*/
  if (false == is_primitive_pb_type(physical_pb_type)) { 
    for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
      for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb; ++jpb) {
        PhysicalPbId child_pb = PhysicalPbId::INVALID();
        /* Find the child pb that is mapped, and the mapping info is not stored in the physical mode ! */
        if (true == physical_pb.valid_pb_id(pb_id)) {
          child_pb = physical_pb.child(pb_id, &(physical_mode->pb_type_children[ipb]), jpb);
          VTR_ASSERT(true == physical_pb.valid_pb_id(child_pb));
        }
        /* Go recursively */
        rec_build_physical_block_bitstream(bitstream_manager, pb_configurable_block,
                                           module_manager, circuit_lib, mux_lib, 
                                           device_annotation, 
                                           border_side, 
                                           physical_pb, child_pb,
                                           &(physical_pb_graph_node->child_pb_graph_nodes[physical_mode->index][ipb][jpb]),
                                           jpb);
      }
    }
  }

  /* Check if this has defined a circuit_model*/
  if (true == is_primitive_pb_type(physical_pb_type)) { 
    switch (physical_pb_type->class_type) {
    case LUT_CLASS: 
      /* Special case for LUT !!!
       * Mapped logical block information is stored in child_pbs of this pb!!!
       */
      //build_lut_bitstream(bitstream_manager, pb_configurable_block,
      //                    module_manager, circuit_lib, mux_lib, 
      //                    physical_pb, pb_id, physical_pb_type);
      break;
    case LATCH_CLASS:
    case UNKNOWN_CLASS:
    case MEMORY_CLASS:
      /* For other types of blocks, we can apply a generic therapy */
      build_primitive_bitstream(bitstream_manager, pb_configurable_block,
                                module_manager, circuit_lib, device_annotation, 
                                physical_pb, pb_id, physical_pb_type);
      break;  
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__, 
                     "Unknown class type of pb_type '%s'!\n",
                     physical_pb_type->name);
      exit(1);
    }
    /* Finish for primitive node, return */
    return;
  }

  /* Generate the bitstream for the interconnection in this physical block */
  //build_physical_block_interc_bitstream(bitstream_manager, pb_configurable_block,
  //                                      module_manager, circuit_lib, mux_lib,
  //                                      physical_pb_graph_node, physical_pb,
  //                                      pb_id, physical_mode_index);
}

/********************************************************************
 * This function generates bitstream for a grid, which could be a
 * CLB, a heterogenerous block, an I/O, etc.
 * Note that each grid may contain a number of physical blocks,
 * this function will iterate over them
 *******************************************************************/
static 
void build_physical_block_bitstream(BitstreamManager& bitstream_manager,
                                    const ConfigBlockId& top_block,
                                    const ModuleManager& module_manager,
                                    const CircuitLibrary& circuit_lib,
                                    const MuxLibrary& mux_lib,
                                    const VprDeviceAnnotation& device_annotation,
                                    const VprClusteringAnnotation& cluster_annotation,
                                    const VprPlacementAnnotation& place_annotation,
                                    const DeviceGrid& grids,
                                    const vtr::Point<size_t>& grid_coord,
                                    const e_side& border_side) {
  /* Create a block for the grid in bitstream manager */
  t_physical_tile_type_ptr grid_type = grids[grid_coord.x()][grid_coord.y()].type;
  std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string grid_block_name = generate_grid_block_instance_name(grid_module_name_prefix, std::string(grid_type->name), 
                                                                  is_io_type(grid_type), border_side, grid_coord);
  ConfigBlockId grid_configurable_block = bitstream_manager.add_block(grid_block_name);
  bitstream_manager.add_child_block(top_block, grid_configurable_block);

  /* Iterate over the capacity of the grid
   * Now each physical tile may have a number of logical blocks
   * OpenFPGA only considers the physical implementation of the tiles.
   * So, we do not allow multiple equivalent sites to be defined 
   * under a physical tile type. 
   * If you need different equivalent sites, you can always define 
   * it as a mode under a <pb_type>
   */
  for (size_t z = 0; z < place_annotation.grid_blocks(grid_coord).size(); ++z) {
    VTR_ASSERT(1 == grid_type->equivalent_sites.size());
    for (t_logical_block_type_ptr lb_type : grid_type->equivalent_sites) {
      /* Bypass empty pb_graph */
      if (nullptr == lb_type->pb_graph_head) {
        continue;
      }

      if (ClusterBlockId::INVALID() == place_annotation.grid_blocks(grid_coord)[z]) {
        /* Recursively traverse the pb_graph and generate bitstream */
        rec_build_physical_block_bitstream(bitstream_manager, grid_configurable_block, 
                                           module_manager, circuit_lib, mux_lib, 
                                           device_annotation, border_side, 
                                           PhysicalPb(), PhysicalPbId::INVALID(),
                                           lb_type->pb_graph_head, z);
      } else {
        const PhysicalPb& phy_pb = cluster_annotation.physical_pb(place_annotation.grid_blocks(grid_coord)[z]);

        /* Get the top-level node of the pb_graph */
        t_pb_graph_node* pb_graph_head = lb_type->pb_graph_head;
        VTR_ASSERT(nullptr != pb_graph_head);
        const PhysicalPbId& top_pb_id = phy_pb.find_pb(pb_graph_head);

        /* Recursively traverse the pb_graph and generate bitstream */
        rec_build_physical_block_bitstream(bitstream_manager, grid_configurable_block, 
                                           module_manager, circuit_lib, mux_lib, 
                                           device_annotation, border_side, 
                                           phy_pb, top_pb_id, pb_graph_head, z);
      }
    }
  } 
}


/********************************************************************
 * Top-level function of this file: 
 * Generate bitstreams for all the grids, including 
 * 1. core grids that sit in the center of the fabric
 * 2. side grids (I/O grids) that sit in the borders for the fabric
 *******************************************************************/
void build_grid_bitstream(BitstreamManager& bitstream_manager,
                          const ConfigBlockId& top_block,
                          const ModuleManager& module_manager,
                          const CircuitLibrary& circuit_lib,
                          const MuxLibrary& mux_lib,
                          const DeviceGrid& grids,
                          const VprDeviceAnnotation& device_annotation,
                          const VprClusteringAnnotation& cluster_annotation,
                          const VprPlacementAnnotation& place_annotation,
                          const bool& verbose) {

  VTR_LOGV(verbose, "Generating bitstream for core grids...");

  /* Generate bitstream for the core logic block one by one */
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      /* Bypass EMPTY grid */
      if (true == is_empty_type(grids[ix][iy].type)) {
        continue;
      } 
      /* Skip width > 1 or height > 1 tiles (mostly heterogeneous blocks) */
      if ( (0 < grids[ix][iy].width_offset)
        || (0 < grids[ix][iy].height_offset) ) {
        continue;
      }
      /* We should not meet any I/O grid */
      VTR_ASSERT(true != is_io_type(grids[ix][iy].type));
      /* Add a grid module to top_module*/
      vtr::Point<size_t> grid_coord(ix, iy);
      build_physical_block_bitstream(bitstream_manager, top_block, module_manager,
                                     circuit_lib, mux_lib,
                                     device_annotation, cluster_annotation,
                                     place_annotation,
                                     grids, grid_coord, NUM_SIDES);
    }
  }
  VTR_LOGV(verbose, "Done\n");

  VTR_LOGV(verbose, "Generating bitstream for I/O grids...");

  /* Create the coordinate range for each side of FPGA fabric */
  std::vector<e_side> io_sides{TOP, RIGHT, BOTTOM, LEFT};
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates;

  /* TOP side*/
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) { 
    io_coordinates[TOP].push_back(vtr::Point<size_t>(ix, grids.height() - 1));
  } 

  /* RIGHT side */
  for (size_t iy = 1; iy < grids.height() - 1; ++iy) { 
    io_coordinates[RIGHT].push_back(vtr::Point<size_t>(grids.width() - 1, iy));
  } 

  /* BOTTOM side*/
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) { 
    io_coordinates[BOTTOM].push_back(vtr::Point<size_t>(ix, 0));
  } 

  /* LEFT side */
  for (size_t iy = 1; iy < grids.height() - 1; ++iy) { 
    io_coordinates[LEFT].push_back(vtr::Point<size_t>(0, iy));
  }

  /* Add instances of I/O grids to top_module */
  for (const e_side& io_side : io_sides) {
    for (const vtr::Point<size_t>& io_coordinate : io_coordinates[io_side]) {
      /* Bypass EMPTY grid */
      if (true == is_empty_type(grids[io_coordinate.x()][io_coordinate.y()].type)) {
        continue;
      } 
      /* Skip height > 1 tiles (mostly heterogeneous blocks) */
      if ( (0 < grids[io_coordinate.x()][io_coordinate.y()].width_offset)
        || (0 < grids[io_coordinate.x()][io_coordinate.y()].height_offset) ) {
        continue;
      }
      /* We should not meet any I/O grid */
      VTR_ASSERT(true == is_io_type(grids[io_coordinate.x()][io_coordinate.y()].type));
      build_physical_block_bitstream(bitstream_manager, top_block, module_manager,
                                     circuit_lib, mux_lib,
                                     device_annotation, cluster_annotation, 
                                     place_annotation,
                                     grids, io_coordinate, io_side);
    }
  }
  VTR_LOGV(verbose, "Done\n");
}

} /* end namespace openfpga */
