/********************************************************************
 * This file includes functions that build io mapping information 
 *******************************************************************/
#include <chrono>
#include <ctime>
#include <fstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from archopenfpga library */
#include "openfpga_naming.h"

#include "module_manager_utils.h"
#include "build_io_mapping_info.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function 
 * - builds the net-to-I/O mapping
 * - identifies each I/O directionality
 * - return a database containing the above information
 *
 * TODO: This function duplicates codes from 
 *       function:  print_verilog_testbench_connect_fpga_ios() in 
 *       source file: verilog_testbench_utils.cpp
 *       Should consider how to merge the codes and share same builder function 
 *******************************************************************/
IoMap build_fpga_io_mapping_info(const ModuleManager& module_manager,
                                 const ModuleId& top_module,
                                 const AtomContext& atom_ctx,
                                 const PlacementContext& place_ctx,
                                 const IoLocationMap& io_location_map,
                                 const VprNetlistAnnotation& netlist_annotation,
                                 const std::string& io_input_port_name_postfix,
                                 const std::string& io_output_port_name_postfix) {
  IoMap io_map;

  /* Only mappable i/o ports can be considered */
  std::vector<ModulePortId> module_io_ports;
  for (const ModuleManager::e_module_port_type& module_io_port_type : MODULE_IO_PORT_TYPES) {
    for (const ModulePortId& gpio_port_id : module_manager.module_port_ids_by_type(top_module, module_io_port_type)) {
      /* Only care mappable I/O */
      if (false == module_manager.port_is_mappable_io(top_module, gpio_port_id)) {
        continue;
      }
      module_io_ports.push_back(gpio_port_id);
    }
  }

  /* Type mapping between VPR block and Module port */
  std::map<AtomBlockType, ModuleManager::e_module_port_type> atom_block_type_to_module_port_type;
  atom_block_type_to_module_port_type[AtomBlockType::INPAD] = ModuleManager::MODULE_GPIN_PORT;
  atom_block_type_to_module_port_type[AtomBlockType::OUTPAD] = ModuleManager::MODULE_GPOUT_PORT;

  /* Type mapping between VPR block and io mapping direction */
  std::map<AtomBlockType, IoMap::e_direction> atom_block_type_to_io_map_direction;
  atom_block_type_to_io_map_direction[AtomBlockType::INPAD] = IoMap::IO_MAP_DIR_INPUT;
  atom_block_type_to_io_map_direction[AtomBlockType::OUTPAD] = IoMap::IO_MAP_DIR_OUTPUT;

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-I/O atom blocks ! */
    if ( (AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk))
      && (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) ) {
      continue;
    }
    
    /* If there is a GPIO port, use it directly
     * Otherwise, should find a GPIN for INPAD
     *         or should find a GPOUT for OUTPAD
     */ 
    std::pair<ModulePortId, size_t> mapped_module_io_info = std::make_pair(ModulePortId::INVALID(), -1);
    for (const ModulePortId& module_io_port_id : module_io_ports) {
      const BasicPort& module_io_port = module_manager.module_port(top_module, module_io_port_id);

      /* Find the index of the mapped GPIO in top-level FPGA fabric */
      size_t temp_io_index = io_location_map.io_index(place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.x,
                                                      place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.y,
                                                      place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.z,
                                                      module_io_port.get_name());

      /* Bypass invalid index (not mapped to this GPIO port) */
      if (size_t(-1) == temp_io_index) {
        continue;
      }

      /* If the port is an GPIO port, just use it */
      if (ModuleManager::MODULE_GPIO_PORT == module_manager.port_type(top_module, module_io_port_id)) {
        mapped_module_io_info = std::make_pair(module_io_port_id, temp_io_index);
        break;
      }

      /* If this is an INPAD, we can use an GPIN port (if available) */
      if (atom_block_type_to_module_port_type[atom_ctx.nlist.block_type(atom_blk)] == module_manager.port_type(top_module, module_io_port_id)) {
        mapped_module_io_info = std::make_pair(module_io_port_id, temp_io_index);
        break;
      }
    }

    /* We must find a valid one */
    VTR_ASSERT(true == module_manager.valid_module_port_id(top_module, mapped_module_io_info.first));
    VTR_ASSERT(size_t(-1) != mapped_module_io_info.second);

    /* Ensure that IO index is in range */
    BasicPort module_mapped_io_port = module_manager.module_port(top_module, mapped_module_io_info.first); 
    size_t io_index = mapped_module_io_info.second;

    /* Set the port pin index */ 
    VTR_ASSERT(io_index < module_mapped_io_port.get_width());
    module_mapped_io_port.set_width(io_index, io_index);

    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    } 

    /* Create the port for benchmark I/O, due to BLIF benchmark, each I/O always has a size of 1 
     * In addition, the input and output ports may have different postfix in naming
     * due to verification context! Here, we give full customization on naming 
     */
    BasicPort benchmark_io_port;
    if (AtomBlockType::INPAD == atom_ctx.nlist.block_type(atom_blk)) {
      benchmark_io_port.set_name(std::string(block_name + io_input_port_name_postfix)); 
      benchmark_io_port.set_width(1);
    } else {
      VTR_ASSERT(AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk));
      benchmark_io_port.set_name(std::string(block_name + io_output_port_name_postfix)); 
      benchmark_io_port.set_width(1);
    }

    io_map.create_io_mapping(module_mapped_io_port,
                             benchmark_io_port,
                             atom_block_type_to_io_map_direction[atom_ctx.nlist.block_type(atom_blk)]);
  }
  
  return io_map;
}

} /* end namespace openfpga */
