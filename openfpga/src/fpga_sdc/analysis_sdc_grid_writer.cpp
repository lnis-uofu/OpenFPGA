/********************************************************************
 * This file includes functions that are used to write SDC commands
 * to disable unused ports of grids, such as Configurable Logic Block
 * (CLBs), heterogeneous blocks, etc.
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

/* Headers from vprutil library */
#include "vpr_utils.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"

#include "pb_type_utils.h"
#include "openfpga_device_grid_utils.h"

#include "sdc_writer_utils.h" 
#include "analysis_sdc_writer_utils.h" 
#include "analysis_sdc_grid_writer.h" 

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Recursively visit all the pb_types in the hierarchy 
 * and disable all the ports
 *
 * Note: it is a must to disable all the ports in all the child pb_types!
 * This can prohibit timing analyzer to consider any FF-to-FF path or 
 * combinatinal path inside an unused grid, when finding critical paths!!!
 *******************************************************************/
static 
void rec_print_analysis_sdc_disable_unused_pb_graph_nodes(std::fstream& fp, 
                                                          const VprDeviceAnnotation& device_annotation,
                                                          const ModuleManager& module_manager,
                                                          const ModuleId& parent_module,
                                                          const std::string& hierarchy_name,
                                                          t_pb_graph_node* physical_pb_graph_node) {
  t_pb_type* physical_pb_type = physical_pb_graph_node->pb_type;

  /* Validate file stream */
  valid_file_stream(fp);
    
  /* Disable all the ports of current module (parent_module)!
   * Hierarchy name already includes the instance name of parent_module 
   */
  fp << "#######################################" << std::endl; 
  fp << "# Disable all the ports for pb_graph_node " << physical_pb_graph_node->pb_type->name << "[" << physical_pb_graph_node->placement_index << "]" << std::endl;
  fp << "#######################################" << std::endl; 

  fp << "set_disable_timing ";
  fp << hierarchy_name; 
  fp << "*";
  fp << std::endl;

  /* Return if this is the primitive pb_type */
  if (true == is_primitive_pb_type(physical_pb_type)) {
    return;
  }

  /* Go recursively */
  t_mode* physical_mode = device_annotation.physical_mode(physical_pb_type);

  /* Disable all the ports by iterating over its instance in the parent module */
  for (int ichild = 0; ichild < physical_mode->num_pb_type_children; ++ichild) {
    /* Generate the name of the Verilog module for this child */
    std::string child_module_name = generate_physical_block_module_name(&(physical_mode->pb_type_children[ichild]));

    ModuleId child_module = module_manager.find_module(child_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(child_module));

    /* Each child may exist multiple times in the hierarchy*/
    for (int inst = 0; inst < physical_mode->pb_type_children[ichild].num_pb; ++inst) {
      std::string child_instance_name = module_manager.instance_name(parent_module, child_module, module_manager.child_module_instances(parent_module, child_module)[inst]);
      /* Must have a valid instance name!!! */
      VTR_ASSERT(false == child_instance_name.empty()); 

      std::string updated_hierarchy_name = hierarchy_name + child_instance_name + std::string("/");

      rec_print_analysis_sdc_disable_unused_pb_graph_nodes(fp, device_annotation, module_manager, child_module, updated_hierarchy_name, 
                                                           &(physical_pb_graph_node->child_pb_graph_nodes[physical_mode->index][ichild][inst])); 
    }
  }
}

/********************************************************************
 * Disable an unused pin of a pb_graph_node (parent_module) 
 *******************************************************************/
static
void disable_pb_graph_node_unused_pin(std::fstream& fp, 
                                      const ModuleManager& module_manager,
                                      const ModuleId& parent_module,
                                      const std::string& hierarchy_name,
                                      const t_pb_graph_pin* pb_graph_pin,
                                      const PhysicalPb& physical_pb,
                                      const PhysicalPbId& pb_id) {
  /* Validate file stream */
  valid_file_stream(fp);
    
  /* Identify if the pb_graph_pin has been used or not
   * TODO: identify if this is a parasitic net
   */ 
  if (AtomNetId::INVALID() != physical_pb.pb_graph_pin_atom_net(pb_id, pb_graph_pin)) {
    /* Used pin; Nothing to do */
    return;
  }

  /* Reach here, it means that this pin is not used. Disable timing analysis for the pin */
  /* Find the module port by name */
  std::string module_port_name = generate_pb_type_port_name(pb_graph_pin->port);
  ModulePortId module_port = module_manager.find_module_port(parent_module, module_port_name);
  VTR_ASSERT(true == module_manager.valid_module_port_id(parent_module, module_port));
  BasicPort port_to_disable = module_manager.module_port(parent_module, module_port);
  port_to_disable.set_width(pb_graph_pin->pin_number, pb_graph_pin->pin_number);

  fp << "set_disable_timing ";
  fp << hierarchy_name; 
  fp << generate_sdc_port(port_to_disable);
  fp << std::endl;
}

/********************************************************************
 * Disable unused input ports and output ports of this pb_graph_node (parent_module) 
 * This function will iterate over all the input pins, output pins
 * of the physical_pb_graph_node, and check if they are mapped
 * For unused pins, we will find the port in parent_module
 * and then print SDC commands to disable them
 *******************************************************************/
static
void disable_pb_graph_node_unused_pins(std::fstream& fp, 
                                       const ModuleManager& module_manager,
                                       const ModuleId& parent_module,
                                       const std::string& hierarchy_name,
                                       t_pb_graph_node* physical_pb_graph_node,
                                       const PhysicalPb& physical_pb) {
  const PhysicalPbId& pb_id = physical_pb.find_pb(physical_pb_graph_node);
  VTR_ASSERT(true == physical_pb.valid_pb_id(pb_id));

  fp << "#######################################" << std::endl; 
  fp << "# Disable unused pins for pb_graph_node " << physical_pb_graph_node->pb_type->name << "[" << physical_pb_graph_node->placement_index << "]" << std::endl;
  fp << "#######################################" << std::endl; 

  /* Disable unused input pins */
  for (int iport = 0; iport < physical_pb_graph_node->num_input_ports; ++iport) {
    for (int ipin = 0; ipin < physical_pb_graph_node->num_input_pins[iport]; ++ipin) {
      disable_pb_graph_node_unused_pin(fp, module_manager, parent_module,
                                       hierarchy_name,
                                       &(physical_pb_graph_node->input_pins[iport][ipin]),
                                       physical_pb, pb_id);
    }
  }

  /* Disable unused output pins */
  for (int iport = 0; iport < physical_pb_graph_node->num_output_ports; ++iport) {
    for (int ipin = 0; ipin < physical_pb_graph_node->num_output_pins[iport]; ++ipin) {
      disable_pb_graph_node_unused_pin(fp, module_manager, parent_module,
                                       hierarchy_name,
                                       &(physical_pb_graph_node->output_pins[iport][ipin]),
                                       physical_pb, pb_id);
    }
  }

  /* Disable unused clock pins */
  for (int iport = 0; iport < physical_pb_graph_node->num_clock_ports; ++iport) {
    for (int ipin = 0; ipin < physical_pb_graph_node->num_clock_pins[iport]; ++ipin) {
      disable_pb_graph_node_unused_pin(fp, module_manager, parent_module,
                                       hierarchy_name,
                                       &(physical_pb_graph_node->clock_pins[iport][ipin]),
                                       physical_pb, pb_id);
    }
  }
}

/********************************************************************
 * Disable unused inputs of routing multiplexers of this pb_graph_node 
 * This function will first cache the nets for each input and output pins 
 * and store the results in a mux_name-to-net mapping
 *******************************************************************/
static 
void disable_pb_graph_node_unused_mux_inputs(std::fstream& fp, 
                                             const VprDeviceAnnotation& device_annotation,
                                             const ModuleManager& module_manager,
                                             const ModuleId& parent_module,
                                             const std::string& hierarchy_name,
                                             t_pb_graph_node* physical_pb_graph_node,
                                             const PhysicalPb& physical_pb) {

  fp << "#######################################" << std::endl; 
  fp << "# Disable unused mux_inputs for pb_graph_node " << physical_pb_graph_node->pb_type->name << "[" << physical_pb_graph_node->placement_index << "]" << std::endl;
  fp << "#######################################" << std::endl; 

  t_pb_type* physical_pb_type = physical_pb_graph_node->pb_type;

  t_mode* physical_mode = device_annotation.physical_mode(physical_pb_type);

  std::map<std::string, AtomNetId> mux_instance_to_net_map;

  /* Cache the nets for each input pins of each child pb_graph_node */
  for (int ichild = 0; ichild < physical_mode->num_pb_type_children; ++ichild) {
    for (int inst = 0; inst < physical_mode->pb_type_children[ichild].num_pb; ++inst) {

      t_pb_graph_node* child_pb_graph_node = &(physical_pb_graph_node->child_pb_graph_nodes[physical_mode->index][ichild][inst]); 

      /* Cache the nets for input pins of the child pb_graph_node */
      for (int iport = 0; iport < child_pb_graph_node->num_input_ports; ++iport) {
        for (int ipin = 0; ipin < child_pb_graph_node->num_input_pins[iport]; ++ipin) {
          const PhysicalPbId& pb_id = physical_pb.find_pb(child_pb_graph_node); 
          VTR_ASSERT(true == physical_pb.valid_pb_id(pb_id));
          /* Generate the mux name */ 
          std::string mux_instance_name = generate_pb_mux_instance_name(GRID_MUX_INSTANCE_PREFIX, &(child_pb_graph_node->input_pins[iport][ipin]), std::string(""));
          /* Cache the net */
          mux_instance_to_net_map[mux_instance_name] = physical_pb.pb_graph_pin_atom_net(pb_id, &(child_pb_graph_node->input_pins[iport][ipin]));
        }
      }

      /* Cache the nets for clock pins of the child pb_graph_node */
      for (int iport = 0; iport < child_pb_graph_node->num_clock_ports; ++iport) {
        for (int ipin = 0; ipin < child_pb_graph_node->num_clock_pins[iport]; ++ipin) {
          const PhysicalPbId& pb_id = physical_pb.find_pb(child_pb_graph_node); 
          /* Generate the mux name */ 
          std::string mux_instance_name = generate_pb_mux_instance_name(GRID_MUX_INSTANCE_PREFIX, &(child_pb_graph_node->clock_pins[iport][ipin]), std::string(""));
          /* Cache the net */
          mux_instance_to_net_map[mux_instance_name] = physical_pb.pb_graph_pin_atom_net(pb_id, &(child_pb_graph_node->clock_pins[iport][ipin]));
        }
      }

    }
  }

  /* Cache the nets for each output pins of this pb_graph_node */
  for (int iport = 0; iport < physical_pb_graph_node->num_output_ports; ++iport) {
    for (int ipin = 0; ipin < physical_pb_graph_node->num_output_pins[iport]; ++ipin) {
      const PhysicalPbId& pb_id = physical_pb.find_pb(physical_pb_graph_node); 
      /* Generate the mux name */ 
      std::string mux_instance_name = generate_pb_mux_instance_name(GRID_MUX_INSTANCE_PREFIX, &(physical_pb_graph_node->output_pins[iport][ipin]), std::string(""));
      /* Cache the net */
      mux_instance_to_net_map[mux_instance_name] = physical_pb.pb_graph_pin_atom_net(pb_id, &(physical_pb_graph_node->output_pins[iport][ipin]));
    }
  }

  /* Now disable unused inputs of routing multiplexers, by tracing from input pins of the parent_module */ 
  for (int iport = 0; iport < physical_pb_graph_node->num_input_ports; ++iport) {
    for (int ipin = 0; ipin < physical_pb_graph_node->num_input_pins[iport]; ++ipin) {
      /* Find the module port by name */
      std::string module_port_name = generate_pb_type_port_name(physical_pb_graph_node->input_pins[iport][ipin].port);
      ModulePortId module_port = module_manager.find_module_port(parent_module, module_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(parent_module, module_port));

      const PhysicalPbId& pb_id = physical_pb.find_pb(physical_pb_graph_node); 
      const AtomNetId& mapped_net = physical_pb.pb_graph_pin_atom_net(pb_id, &(physical_pb_graph_node->input_pins[iport][ipin])); 

      /* If the pin has not fan-out, we do not need to disable anything  */
      if (0 == physical_pb_graph_node->input_pins[iport][ipin].num_output_edges) {
        /* Make sure that we do not have any module net associated to this pin */
        ModuleNetId module_net = module_manager.module_instance_port_net(parent_module, parent_module, 0, module_port, ipin); 
        VTR_ASSERT(false == module_manager.valid_module_net_id(parent_module, module_net));
        continue;
      }

      disable_analysis_module_input_pin_net_sinks(fp, module_manager, parent_module,
                                                  hierarchy_name,
                                                  module_port, ipin,
                                                  mapped_net,
                                                  mux_instance_to_net_map);
    }
  }

  for (int iport = 0; iport < physical_pb_graph_node->num_clock_ports; ++iport) {
    for (int ipin = 0; ipin < physical_pb_graph_node->num_clock_pins[iport]; ++ipin) {
      /* Find the module port by name */
      std::string module_port_name = generate_pb_type_port_name(physical_pb_graph_node->clock_pins[iport][ipin].port);
      ModulePortId module_port = module_manager.find_module_port(parent_module, module_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(parent_module, module_port));

      const PhysicalPbId& pb_id = physical_pb.find_pb(physical_pb_graph_node); 
      const AtomNetId& mapped_net = physical_pb.pb_graph_pin_atom_net(pb_id, &(physical_pb_graph_node->clock_pins[iport][ipin])); 

      /* If the pin has not fan-out, we do not need to disable anything  */
      if (0 == physical_pb_graph_node->clock_pins[iport][ipin].num_output_edges) {
        /* Make sure that we do not have any module net associated to this pin */
        ModuleNetId module_net = module_manager.module_instance_port_net(parent_module, parent_module, 0, module_port, ipin); 
        VTR_ASSERT(false == module_manager.valid_module_net_id(parent_module, module_net));
        continue;
      }

      disable_analysis_module_input_pin_net_sinks(fp, module_manager, parent_module,
                                                  hierarchy_name,
                                                  module_port, ipin,
                                                  mapped_net,
                                                  mux_instance_to_net_map);
    }
  }

  /* Now disable unused inputs of routing multiplexers, by tracing from output pins of the child_module */ 
  for (int ichild = 0; ichild < physical_mode->num_pb_type_children; ++ichild) {
    /* Generate the name of the Verilog module for this child */
    std::string child_module_name = generate_physical_block_module_name(&(physical_mode->pb_type_children[ichild]));

    ModuleId child_module = module_manager.find_module(child_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(child_module));

    for (int inst = 0; inst < physical_mode->pb_type_children[ichild].num_pb; ++inst) {

      t_pb_graph_node* child_pb_graph_node = &(physical_pb_graph_node->child_pb_graph_nodes[physical_mode->index][ichild][inst]); 

      for (int iport = 0; iport < child_pb_graph_node->num_output_ports; ++iport) {
        for (int ipin = 0; ipin < child_pb_graph_node->num_output_pins[iport]; ++ipin) {
          /* Find the module port by name */
          std::string module_port_name = generate_pb_type_port_name(child_pb_graph_node->output_pins[iport][ipin].port);
          ModulePortId module_port = module_manager.find_module_port(child_module, module_port_name);
          VTR_ASSERT(true == module_manager.valid_module_port_id(child_module, module_port));

          const PhysicalPbId& pb_id = physical_pb.find_pb(child_pb_graph_node); 
          const AtomNetId& mapped_net = physical_pb.pb_graph_pin_atom_net(pb_id, &(child_pb_graph_node->output_pins[iport][ipin])); 

          /* Corner case: if the pb_graph_pin has no fan-out we will skip this pin */
          if (0 == child_pb_graph_node->output_pins[iport][ipin].num_output_edges) {
            continue;
          }

          disable_analysis_module_output_pin_net_sinks(fp, module_manager, parent_module,
                                                       hierarchy_name,
                                                       child_module, inst, 
                                                       module_port, ipin,
                                                       mapped_net, 
                                                       mux_instance_to_net_map);
        }
      }
    }
  }
}

/********************************************************************
 * Recursively visit all the pb_types in the hierarchy 
 * and disable all the unused resources, including:
 * 1. input ports
 * 2. output ports
 * 3. unused inputs of routing multiplexers
 *
 * As this function is executed in a recursive way. 
 * To avoid repeated disable timing for ports, during each run of this function,
 * only the unused input ports, output ports of the parent module will be disabled.
 * In addition, we will cache all the net ids mapped to the input ports of 
 * child modules, and the net ids mapped to the output ports of parent module.
 * As such, we can trace from 
 * 1. the input ports of parent module to disable unused inputs of routing multiplexer
 *    which drives the inputs of child modules
 *                      
 *                          Parent_module
 *                         +---------------------------------------------
 *                         |          MUX                  child_module
 *                         |         +-------------+       +--------
 *    input_pin0(netA) --->|-------->| Routing     |------>|
 *    input_pin1(netB) --->|----x--->| Multiplexer | netA  |
 *                         |         +-------------+       |
 *                         |                               |
 *
 * 2. the output ports of child module to disable unused inputs of routing multiplexer
 *    which drives the outputs of parent modules
 *
 *   Case 1: 
 *                                  parent_module
 *         --------------------------------------+
 *         child_module                          |
 *        -------------+                         |
 *                     |    +-------------+      |
 *  output_pin0 (netA) |--->| Routing     |----->|---->
 *  output_pin1 (netB) |-x->| Multiplexer | netA |
 *                     |    +-------------+      |
 *
 *    Case 2: 
 *
 *                         Parent_module
 *                         +---------------------------------------------
 *                         |
 *                         |    +--------------------------------------------+
 *                         |    |     MUX                  child_module      |
 *                         |    |    +-------------+       +-----------+     |
 *                         |    +--->| Routing     |------>|           |     |
 *    input_pin0(netA) --->|----x--->| Multiplexer | netA  | output_pin|-----+
 *                         |         +-------------+       |           | netA
 *                         |                               |           |
 *
 *
 * Note: it is a must to disable all the ports in all the child pb_types!
 * This can prohibit timing analyzer to consider any FF-to-FF path or 
 * combinatinal path inside an unused grid, when finding critical paths!!!
 *******************************************************************/
static 
void rec_print_analysis_sdc_disable_pb_graph_node_unused_resources(std::fstream& fp, 
                                                                   const VprDeviceAnnotation& device_annotation,
                                                                   const ModuleManager& module_manager,
                                                                   const ModuleId& parent_module,
                                                                   const std::string& hierarchy_name,
                                                                   t_pb_graph_node* physical_pb_graph_node,
                                                                   const PhysicalPb& physical_pb) {
  t_pb_type* physical_pb_type = physical_pb_graph_node->pb_type;

  /* Disable unused input ports and output ports of this pb_graph_node (parent_module) */
  disable_pb_graph_node_unused_pins(fp, module_manager, parent_module,
                                    hierarchy_name, physical_pb_graph_node, physical_pb); 

  /* Return if this is the primitive pb_type 
   * Note: this must return before we disable any unused inputs of routing multiplexer!
   * This is due to that primitive pb_type does NOT contain any routing multiplexers inside!!!   
   */
  if (true == is_primitive_pb_type(physical_pb_type)) {
    return;
  }

  /* Disable unused inputs of routing multiplexers of this pb_graph_node */
  disable_pb_graph_node_unused_mux_inputs(fp, device_annotation,
                                          module_manager, parent_module, 
                                          hierarchy_name, physical_pb_graph_node,
                                          physical_pb);


  t_mode* physical_mode = device_annotation.physical_mode(physical_pb_type);

  /* Disable all the ports by iterating over its instance in the parent module */
  for (int ichild = 0; ichild < physical_mode->num_pb_type_children; ++ichild) {
    /* Generate the name of the Verilog module for this child */
    std::string child_module_name = generate_physical_block_module_name(&(physical_mode->pb_type_children[ichild]));

    ModuleId child_module = module_manager.find_module(child_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(child_module));

    /* Each child may exist multiple times in the hierarchy*/
    for (int inst = 0; inst < physical_pb_type->modes[physical_mode->index].pb_type_children[ichild].num_pb; ++inst) {
      std::string child_instance_name = module_manager.instance_name(parent_module, child_module, module_manager.child_module_instances(parent_module, child_module)[inst]);
      /* Must have a valid instance name!!! */
      VTR_ASSERT(false == child_instance_name.empty()); 

      std::string updated_hierarchy_name = hierarchy_name + child_instance_name + std::string("/");

      rec_print_analysis_sdc_disable_pb_graph_node_unused_resources(fp, device_annotation,
                                                                    module_manager, child_module, updated_hierarchy_name, 
                                                                    &(physical_pb_graph_node->child_pb_graph_nodes[physical_mode->index][ichild][inst]), 
                                                                    physical_pb); 
    }
  }
}

/********************************************************************
 * This function can work in two differnt modes:
 * 1. For partially unused pb blocks
 * ---------------------------------
 * Disable the timing for only unused resources in a physical block
 * We have to walk through pb_graph node, port by port and pin by pin.
 * Identify which pins have not been used, and then disable the timing 
 * for these ports. 
 * Plus, for input ports, we will trace the routing multiplexers
 * and disable the timing for unused inputs.
 *
 * 2. For fully unused pb_blocks
 * ----------------------------- 
 * Disable the timing for a fully unused grid!
 * This is very straightforward!
 * Just walk through each pb_type and disable all the ports using wildcards
 *******************************************************************/
static 
void print_analysis_sdc_disable_pb_block_unused_resources(std::fstream& fp,
                                                          t_physical_tile_type_ptr grid_type,
                                                          const vtr::Point<size_t>& grid_coordinate,
                                                          const VprDeviceAnnotation& device_annotation,
                                                          const ModuleManager& module_manager,
                                                          const std::string& grid_instance_name,
                                                          const size_t& grid_z,
                                                          const PhysicalPb& physical_pb,
                                                          const bool& unused_block) {
  /* If the block is partially unused, we should have a physical pb */
  if (false == unused_block) {
    VTR_ASSERT(false == physical_pb.empty());
  }

  VTR_ASSERT(1 == grid_type->equivalent_sites.size());
  t_pb_graph_node* pb_graph_head = grid_type->equivalent_sites[0]->pb_graph_head; 
  VTR_ASSERT(nullptr != pb_graph_head);

  /* Find an unique name to the pb instance in this grid
   * Note: this must be consistent with the instance name we used in build_grid_module()!!!
   */
  /* TODO: validate that the instance name is used in module manager!!! */
  std::string pb_module_name = generate_physical_block_module_name(pb_graph_head->pb_type);
  std::string pb_instance_name = generate_physical_block_instance_name(pb_graph_head->pb_type, grid_z);

  ModuleId pb_module = module_manager.find_module(pb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(pb_module));

  /* Print comments */
  fp << "#######################################" << std::endl; 
 
  if (true == unused_block) {
    fp << "# Disable Timing for unused grid[" << grid_coordinate.x() << "][" << grid_coordinate.y() << "][" << grid_z << "]" << std::endl;
  } else {
    VTR_ASSERT_SAFE(false == unused_block);
    fp << "# Disable Timing for unused resources in grid[" << grid_coordinate.x() << "][" << grid_coordinate.y() << "][" << grid_z << "]" << std::endl;
  }

  fp << "#######################################" << std::endl; 

  std::string hierarchy_name = grid_instance_name + std::string("/") + pb_instance_name + std::string("/");

  /* Go recursively through the pb_graph hierarchy, and disable all the ports level by level */
  if (true == unused_block) {
    rec_print_analysis_sdc_disable_unused_pb_graph_nodes(fp, device_annotation,
                                                         module_manager, pb_module, hierarchy_name,
                                                         pb_graph_head); 
  } else { 
    VTR_ASSERT_SAFE(false == unused_block);
    rec_print_analysis_sdc_disable_pb_graph_node_unused_resources(fp, device_annotation,
                                                                  module_manager, pb_module, hierarchy_name,
                                                                  pb_graph_head, physical_pb); 
  }
}

/********************************************************************
 * Disable the timing for a fully unused grid!
 * This is very straightforward!
 * Just walk through each pb_type and disable all the ports using wildcards
 *******************************************************************/
static 
void print_analysis_sdc_disable_unused_grid(std::fstream& fp, 
                                            const vtr::Point<size_t>& grid_coordinate,
                                            const DeviceGrid& grids, 
                                            const VprDeviceAnnotation& device_annotation,
                                            const VprClusteringAnnotation& cluster_annotation,
                                            const VprPlacementAnnotation& place_annotation,
                                            const ModuleManager& module_manager,
                                            const e_side& border_side) {
  /* Validate file stream */
  valid_file_stream(fp);

  t_physical_tile_type_ptr grid_type = grids[grid_coordinate.x()][grid_coordinate.y()].type;
  /* Bypass conditions for grids : 
   * 1. EMPTY type, which is by nature unused
   * 2. Offset > 0, which has already been processed when offset = 0
   */
  if ( (true == is_empty_type(grid_type))
    || (0 < grids[grid_coordinate.x()][grid_coordinate.y()].width_offset)
    || (0 < grids[grid_coordinate.x()][grid_coordinate.y()].height_offset) ) {
    return;
  }

  /* Find an unique name to the grid instane
   * Note: this must be consistent with the instance name we used in build_top_module()!!!
   */
  /* TODO: validate that the instance name is used in module manager!!! */
  std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string grid_module_name = generate_grid_block_module_name(grid_module_name_prefix, std::string(grid_type->name), is_io_type(grid_type), border_side);
  std::string grid_instance_name = generate_grid_block_instance_name(grid_module_name_prefix, std::string(grid_type->name), is_io_type(grid_type), border_side, grid_coordinate);

  ModuleId grid_module = module_manager.find_module(grid_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(grid_module));

  /* Print comments */
  fp << "#######################################" << std::endl; 
  fp << "# Disable Timing for grid[" << grid_coordinate.x() << "][" << grid_coordinate.y() << "]" << std::endl;
  fp << "#######################################" << std::endl; 

  /* For used grid, find the unused rr_node in the local rr_graph 
   * and then disable each port which is not used
   * as well as the unused inputs of routing multiplexers!
   */
  size_t grid_z = 0;
  for (const ClusterBlockId& blk_id : place_annotation.grid_blocks(grid_coordinate)) {
    if (ClusterBlockId::INVALID() != blk_id) { 
      const PhysicalPb& physical_pb = cluster_annotation.physical_pb(blk_id);
      print_analysis_sdc_disable_pb_block_unused_resources(fp, grid_type, grid_coordinate,
                                                           device_annotation,
                                                           module_manager, grid_instance_name, grid_z,
                                                           physical_pb, false);
    } else {
      VTR_ASSERT(ClusterBlockId::INVALID() == blk_id);
      /* For unused grid, disable all the pins in the physical_pb_type */
      print_analysis_sdc_disable_pb_block_unused_resources(fp, grid_type, grid_coordinate,
                                                           device_annotation, 
                                                           module_manager, grid_instance_name, grid_z,
                                                           PhysicalPb(), true);
    }
    grid_z++;
  }
}

/********************************************************************
 * Top-level function writes SDC commands to disable unused ports 
 * of grids, such as Configurable Logic Block (CLBs), heterogeneous blocks, etc.
 *
 * This function will iterate over all the grids available in the FPGA fabric
 * It will disable the timing analysis for
 * 1. Grids, which are totally not used (no logic has been mapped to)
 * 2. Unused part of grids, including the ports, inputs of routing multiplexers 
 *
 * Note that it is a must to disable the unused inputs of routing multiplexers
 * because it will cause unexpected paths in timing analysis
 * For example:
 *                           +---------------------+
 *     inputA (net0) ------->|                     |
 *                           | Routing multiplexer |----> output (net0)
 *     inputB (net1) ------->|                     |
 *                           +---------------------+
 *
 * During timing analysis, the path from inputA to output should be considered
 * while the path from inputB to output should NOT be considered!!!
 *
 *******************************************************************/
void print_analysis_sdc_disable_unused_grids(std::fstream& fp, 
                                             const DeviceGrid& grids, 
                                             const VprDeviceAnnotation& device_annotation,
                                             const VprClusteringAnnotation& cluster_annotation,
                                             const VprPlacementAnnotation& place_annotation,
                                             const ModuleManager& module_manager) {

  /* Process unused core grids */
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      print_analysis_sdc_disable_unused_grid(fp, vtr::Point<size_t>(ix, iy),
                                             grids, device_annotation, cluster_annotation, place_annotation,
                                             module_manager, NUM_SIDES);
    }
  }

  /* Instanciate I/O grids */
  /* Create the coordinate range for each side of FPGA fabric */
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates = generate_perimeter_grid_coordinates( grids);

  /* Add instances of I/O grids to top_module */
  for (const e_side& io_side : FPGA_SIDES_CLOCKWISE) {
    for (const vtr::Point<size_t>& io_coordinate : io_coordinates[io_side]) {
      print_analysis_sdc_disable_unused_grid(fp, io_coordinate,
                                             grids, device_annotation, cluster_annotation, place_annotation,
                                             module_manager, io_side);
    }
  }
}

} /* end namespace openfpga */
