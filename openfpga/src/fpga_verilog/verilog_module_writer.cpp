/********************************************************************
 * This file includes functions to write a Verilog module
 * based on its definition in Module Manager
 *
 * Note that Verilog writer functions are just an outputter for the
 * module definition. 
 * You should NOT modify any content of the module manager
 * Please use const keyword to restrict this!
 *******************************************************************/
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"
#include "openfpga_digest.h"

#include "openfpga_naming.h"

#include "module_manager_utils.h"
#include "verilog_port_types.h"
#include "verilog_writer_utils.h"
#include "verilog_module_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Generate the name of a local wire for a undriven port inside Verilog 
 * module 
 *******************************************************************/
static 
std::string generate_verilog_undriven_local_wire_name(const ModuleManager& module_manager, 
                                                      const ModuleId& parent, 
                                                      const ModuleId& child, 
                                                      const size_t& instance_id, 
                                                      const ModulePortId& child_port_id) {
  std::string wire_name;
  if (!module_manager.instance_name(parent, child, instance_id).empty()) {
    wire_name = module_manager.instance_name(parent, child, instance_id);
  } else {
    wire_name = module_manager.module_name(child) + std::string("_") + std::to_string(instance_id); 
    wire_name += std::string("_");
  }
  
  wire_name += std::string("_undriven_");
  wire_name += module_manager.module_port(child, child_port_id).get_name();
  
  return wire_name;
}


/********************************************************************
 * Name a net for a local wire for a verilog module 
 * 1. If this is a local wire, name it after the <src_module_name>_<instance_id>_<src_port_name>
 * 2. If this is not a local wire, name it after the port name of parent module 
 *
 * In addition, it will assign the pin index as well  
 *
 * Restriction: this function requires each net has single driver
 * which is definitely always true in circuits.
 *******************************************************************/
static 
BasicPort generate_verilog_port_for_module_net(const ModuleManager& module_manager,
                                               const ModuleId& module_id,
                                               const ModuleNetId& module_net) {
  /* Check all the sink modules of the net, 
   * if we have a source module is the current module, this is not local wire 
   */
  for (ModuleNetSrcId src_id : module_manager.module_net_sources(module_id, module_net)) {
    if (module_id == module_manager.net_source_modules(module_id, module_net)[src_id]) {
      /* Here, this is not a local wire, return the port name of the src_port */
      ModulePortId net_src_port = module_manager.net_source_ports(module_id, module_net)[src_id];
      size_t src_pin_index = module_manager.net_source_pins(module_id, module_net)[src_id];
      return BasicPort(module_manager.module_port(module_id, net_src_port).get_name(), src_pin_index, src_pin_index);
    }
  }

  /* Check all the sink modules of the net */
  for (ModuleNetSinkId sink_id : module_manager.module_net_sinks(module_id, module_net)) {
    if (module_id == module_manager.net_sink_modules(module_id, module_net)[sink_id]) {
      /* Here, this is not a local wire, return the port name of the sink_port */
      ModulePortId net_sink_port = module_manager.net_sink_ports(module_id, module_net)[sink_id];
      size_t sink_pin_index = module_manager.net_sink_pins(module_id, module_net)[sink_id];
      return BasicPort(module_manager.module_port(module_id, net_sink_port).get_name(), sink_pin_index, sink_pin_index);
    }
  }

  /* Reach here, this is a local wire */
  std::string net_name;

  /* Each net must only one 1 source */ 
  VTR_ASSERT(1 == module_manager.net_source_modules(module_id, module_net).size());

  /* Get the source module */
  ModuleId net_src_module = module_manager.net_source_modules(module_id, module_net)[ModuleNetSrcId(0)];
  /* Get the instance id */
  size_t net_src_instance = module_manager.net_source_instances(module_id, module_net)[ModuleNetSrcId(0)]; 
  /* Get the port id */
  ModulePortId net_src_port = module_manager.net_source_ports(module_id, module_net)[ModuleNetSrcId(0)]; 
  /* Get the pin id */
  size_t net_src_pin = module_manager.net_source_pins(module_id, module_net)[ModuleNetSrcId(0)]; 

  /* Load user-defined name if we have it */
  if (false == module_manager.net_name(module_id, module_net).empty()) {
    net_name = module_manager.net_name(module_id, module_net);
  } else {
    net_name  = module_manager.module_name(net_src_module); 
    net_name += std::string("_") + std::to_string(net_src_instance) + std::string("_");
    net_name += module_manager.module_port(net_src_module, net_src_port).get_name();
  }
  
  return BasicPort(net_name, net_src_pin, net_src_pin);
}

/********************************************************************
 * Find all the nets that are going to be local wires
 * And organize it in a vector of ports
 * Verilog wire writter function will use the output of this function
 * to write up local wire declaration in Verilog format
 *******************************************************************/
static 
std::map<std::string, std::vector<BasicPort>> find_verilog_module_local_wires(const ModuleManager& module_manager,
                                                                              const ModuleId& module_id) {
  std::map<std::string, std::vector<BasicPort>> local_wires;

  /* Local wires come from the child modules */
  for (ModuleNetId module_net : module_manager.module_nets(module_id)) {
    /* Bypass dangling nets:
     * Xifan Tang: I comment this part because it will shadow our problems in creating module graph
     * Indeed this make a robust and a smooth Verilog module writing
     * But I do want the module graph create is nice and clean !!! 
     */
    /*
    if ( (0 == module_manager.net_source_modules(module_id, module_net).size()) 
      && (0 == module_manager.net_source_modules(module_id, module_net).size()) ) {
      continue;
    }
    */

    /* We only care local wires */ 
    if (false == module_net_is_local_wire(module_manager, module_id, module_net)) {
      continue;
    }
    /* Find the name for this local wire */
    BasicPort local_wire_candidate = generate_verilog_port_for_module_net(module_manager, module_id, module_net);
    /* Cache the net name, try to find it in the cache.
     * If you can find one, it means this port may be mergeable, try to do merging. If merge fail, add to the local wire list
     * If you cannot find one, it means that this port is not mergeable, add to the local wire list immediately.
     */
    std::map<std::string, std::vector<BasicPort>>::iterator it = local_wires.find(local_wire_candidate.get_name());
    bool merged = false;
    if (it != local_wires.end()) {
      /* Try to merge to one the port in the list that can absorb the current local wire */
      for (BasicPort& local_wire : local_wires[local_wire_candidate.get_name()]) {
        /* check if the candidate can be combined to an existing local wire */
        if (true == two_verilog_ports_mergeable(local_wire, local_wire_candidate)) {
          /* Merge the ports */
          local_wire = merge_two_verilog_ports(local_wire, local_wire_candidate);
          merged = true;
          break;
        } 
      }
    }

    /* If not merged/not found in the cache, push the port to the list */
    if (false == merged) {
      local_wires[local_wire_candidate.get_name()].push_back(local_wire_candidate);
    }
  }

  /* Local wires could also happen for undriven ports of child module */
  for (const ModuleId& child : module_manager.child_modules(module_id)) {
    for (size_t instance : module_manager.child_module_instances(module_id, child)) {
      for (const ModulePortId& child_port_id : module_manager.module_ports(child)) {
        BasicPort child_port = module_manager.module_port(child, child_port_id);
        std::vector<size_t> undriven_pins;
        for (size_t child_pin : child_port.pins()) {
          /* Find the net linked to the pin */
          ModuleNetId net = module_manager.module_instance_port_net(module_id, child, instance, 
                                                                    child_port_id, child_pin);
          /* We only care undriven ports */
          if (ModuleNetId::INVALID() == net) {
            undriven_pins.push_back(child_pin);
          }
        }
        if (true == undriven_pins.empty()) {
          continue;
        }
        /* Reach here, we need a local wire, we will create a port only for the undriven pins of the port! */
        BasicPort instance_port;
        instance_port.set_name(generate_verilog_undriven_local_wire_name(module_manager, module_id, child, instance, child_port_id));
        /* We give the same port name as child module, this case happens to global ports */
        instance_port.set_width(*std::min_element(undriven_pins.begin(), undriven_pins.end()),
                                *std::max_element(undriven_pins.begin(), undriven_pins.end())); 

        local_wires[instance_port.get_name()].push_back(instance_port);
      }
    }
  }

  return local_wires;
}

/********************************************************************
 * Print a Verilog wire connection 
 * We search all the sinks of the net, 
 * if we find a module output, we try to find the next module output 
 * among the sinks of the net
 * For each module output (except the first one), we print a wire connection 
 *******************************************************************/
static 
void print_verilog_module_output_short_connection(std::fstream& fp, 
                                                  const ModuleManager& module_manager,
                                                  const ModuleId& module_id,
                                                  const ModuleNetId& module_net) {
  /* Ensure a valid file stream */
  VTR_ASSERT(true == valid_file_stream(fp));

  bool first_port = true;
  BasicPort src_port;

  /* We have found a module input, now check all the sink modules of the net */
  for (ModuleNetSinkId net_sink : module_manager.module_net_sinks(module_id, module_net)) {
    ModuleId sink_module = module_manager.net_sink_modules(module_id, module_net)[net_sink];
    if (module_id != sink_module) {
      continue;
    }

    /* Find the sink port and pin information */
    ModulePortId sink_port_id = module_manager.net_sink_ports(module_id, module_net)[net_sink];
    size_t sink_pin = module_manager.net_sink_pins(module_id, module_net)[net_sink];
    BasicPort sink_port(module_manager.module_port(module_id, sink_port_id).get_name(), sink_pin, sink_pin);

    /* For the first module output, this is the source port, we do nothing and go to the next */
    if (true == first_port) {
      src_port = sink_port;
      /* Flip the flag */
      first_port = false;
      continue;
    }

    /* We need to print a wire connection here */
    print_verilog_wire_connection(fp, sink_port, src_port, false);
  }
}


/********************************************************************
 * Print a Verilog wire connection 
 * We search all the sources of the net, 
 * if we find a module input, we try to find a module output 
 * among the sinks of the net
 * If we find such a pair, we print a wire connection 
 *******************************************************************/
static 
void print_verilog_module_local_short_connection(std::fstream& fp, 
                                                 const ModuleManager& module_manager,
                                                 const ModuleId& module_id,
                                                 const ModuleNetId& module_net) {
  /* Ensure a valid file stream */
  VTR_ASSERT(true == valid_file_stream(fp));

  for (ModuleNetSrcId net_src : module_manager.module_net_sources(module_id, module_net)) {
    ModuleId src_module = module_manager.net_source_modules(module_id, module_net)[net_src];
    if (module_id != src_module) {
      continue;
    }
    /* Find the source port and pin information */
    print_verilog_comment(fp, std::string("----- Net source id " + std::to_string(size_t(net_src)) + " -----"));
    ModulePortId src_port_id = module_manager.net_source_ports(module_id, module_net)[net_src];
    size_t src_pin = module_manager.net_source_pins(module_id, module_net)[net_src];
    BasicPort src_port(module_manager.module_port(module_id, src_port_id).get_name(), src_pin, src_pin);

    /* We have found a module input, now check all the sink modules of the net */
    for (ModuleNetSinkId net_sink : module_manager.module_net_sinks(module_id, module_net)) {
      ModuleId sink_module = module_manager.net_sink_modules(module_id, module_net)[net_sink];
      if (module_id != sink_module) {
        continue;
      }

      /* Find the sink port and pin information */
      print_verilog_comment(fp, std::string("----- Net sink id " + std::to_string(size_t(net_sink)) + " -----"));
      ModulePortId sink_port_id = module_manager.net_sink_ports(module_id, module_net)[net_sink];
      size_t sink_pin = module_manager.net_sink_pins(module_id, module_net)[net_sink];
      BasicPort sink_port(module_manager.module_port(module_id, sink_port_id).get_name(), sink_pin, sink_pin);

      /* We need to print a wire connection here */
      print_verilog_wire_connection(fp, sink_port, src_port, false);
    }
  }
}

/********************************************************************
 * Print short connections inside a Verilog module
 * The short connection is defined as the direct connection
 * between an input port of the module and an output port of the module
 * This type of connection is not covered when printing Verilog instances
 * Therefore, they are covered in this function 
 *
 *            module
 *            +-----------------------------+
 *            |                             |
 *  inputA--->|---------------------------->|--->outputB
 *            |                             |
 *            |                             |
 *            |                             |
 *            +-----------------------------+
 *******************************************************************/
static 
void print_verilog_module_local_short_connections(std::fstream& fp, 
                                                  const ModuleManager& module_manager,
                                                  const ModuleId& module_id) {
  /* Local wires come from the child modules */
  for (ModuleNetId module_net : module_manager.module_nets(module_id)) {
    /* We only care the nets that indicate short connections */ 
    if (false == module_net_include_local_short_connection(module_manager, module_id, module_net)) {
      continue;
    }
    print_verilog_comment(fp, std::string("----- Local connection due to Wire " + std::to_string(size_t(module_net)) + " -----"));
    print_verilog_module_local_short_connection(fp, module_manager, module_id, module_net); 
  }
}

/********************************************************************
 * Print output short connections inside a Verilog module
 * The output short connection is defined as the direct connection
 * between two output ports of the module
 * This type of connection is not covered when printing Verilog instances
 * Therefore, they are covered in this function 
 *
 *            module
 *            +-----------------------------+
 *                                          |
 *               src------>+--------------->|--->outputA
 *                         |                |
 *                         |                |
 *                         +--------------->|--->outputB
 *            +-----------------------------+
 *******************************************************************/
static 
void print_verilog_module_output_short_connections(std::fstream& fp, 
                                                   const ModuleManager& module_manager,
                                                   const ModuleId& module_id) {
  /* Local wires come from the child modules */
  for (ModuleNetId module_net : module_manager.module_nets(module_id)) {
    /* We only care the nets that indicate short connections */ 
    if (false == module_net_include_output_short_connection(module_manager, module_id, module_net)) {
      continue;
    }
    print_verilog_module_output_short_connection(fp, module_manager, module_id, module_net); 
  }
}

/********************************************************************
 * Write a Verilog instance to a file
 * This function will name the input and output connections to
 * the inputs/output or local wires available in the parent module
 *      
 *    Parent_module
 *    +-----------------------------+
 *    |                             |
 *    |       +--------------+      |
 *    |       |              |      |
 *    |       | child_module |      |
 *    |       |  [instance]  |      |
 *    |       +--------------+      |
 *    |                             |
 *    +-----------------------------+
 *
 *******************************************************************/
static 
void write_verilog_instance_to_file(std::fstream& fp,
                                    const ModuleManager& module_manager,
                                    const ModuleId& parent_module,
                                    const ModuleId& child_module,
                                    const size_t& instance_id,
                                    const bool& use_explicit_port_map) {
  /* Ensure a valid file stream */
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Print module name */
  fp << "\t" << module_manager.module_name(child_module) << " ";
  /* Print instance name: 
   * if we have an instance name, use it;
   * if not, we use a default name <name>_<num_instance_in_parent_module> 
   */
  if (true == module_manager.instance_name(parent_module, child_module, instance_id).empty()) {
    fp << generate_instance_name(module_manager.module_name(child_module), instance_id) << " (" << std::endl;
  } else {
    fp << module_manager.instance_name(parent_module, child_module, instance_id) << " (" << std::endl;
  }

  /* Print each port with/without explicit port map */
  /* port type2type mapping */
  std::map<ModuleManager::e_module_port_type, enum e_dump_verilog_port_type> port_type2type_map;
  port_type2type_map[ModuleManager::MODULE_GLOBAL_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_GPIN_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_GPOUT_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_GPIO_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_INOUT_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_INPUT_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_OUTPUT_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_CLOCK_PORT] = VERILOG_PORT_CONKT;

  /* Port sequence: global, inout, input, output and clock ports, */
  size_t port_cnt = 0;
  for (const auto& kv : port_type2type_map) {
    for (const ModulePortId& child_port_id : module_manager.module_port_ids_by_type(child_module, kv.first)) {
      BasicPort child_port = module_manager.module_port(child_module, child_port_id);
      if (0 != port_cnt) {
        /* Do not dump a comma for the first port */
        fp << "," << std::endl; 
      }
      /* Print port */
      fp << "\t\t";
      /* if explicit port map is required, output the port name */
      if (true == use_explicit_port_map) {
        fp << "." << child_port.get_name() << "(";
      }

      /* Create the port name and width to be used by the instance */
      std::vector<BasicPort> instance_ports; 
      for (size_t child_pin : child_port.pins()) {
        /* Find the net linked to the pin */
        ModuleNetId net = module_manager.module_instance_port_net(parent_module, child_module, instance_id, 
                                                                  child_port_id, child_pin);
        BasicPort instance_port;
        if (ModuleNetId::INVALID() == net) {
          /* We give the same port name as child module, this case happens to global ports */
          instance_port.set_name(generate_verilog_undriven_local_wire_name(module_manager, parent_module, child_module, instance_id, child_port_id));
          instance_port.set_width(child_pin, child_pin); 
        } else {
          /* Find the name for this child port */
          instance_port = generate_verilog_port_for_module_net(module_manager, parent_module, net);
        }
        /* Create the port information for the net */
        instance_ports.push_back(instance_port);
      } 
      /* Try to merge the ports */
      std::vector<BasicPort> merged_ports = combine_verilog_ports(instance_ports); 

      /* Print a verilog port by combining the instance ports */
      fp << generate_verilog_ports(merged_ports);

      /* if explicit port map is required, output the pair of branket */
      if (true == use_explicit_port_map) {
        fp << ")";
      }
      port_cnt++;
    }
  }
  
  /* Print an end to the instance */
  fp << ");" << std::endl;
}

/********************************************************************
 * Write a Verilog module to a file
 * This is a key function, maybe most frequently called in our Verilog writer
 * Note that file stream must be valid 
 *******************************************************************/
void write_verilog_module_to_file(std::fstream& fp,
                                  const ModuleManager& module_manager,
                                  const ModuleId& module_id,
                                  const bool& use_explicit_port_map) {

  VTR_ASSERT(true == valid_file_stream(fp));

  /* Ensure we have a valid module_id */
  VTR_ASSERT(module_manager.valid_module_id(module_id)); 

  /* Print module declaration */
  print_verilog_module_declaration(fp, module_manager, module_id);

  /* Print an empty line as splitter */
  fp << std::endl;
   
  /* Print internal wires */
  std::map<std::string, std::vector<BasicPort>> local_wires = find_verilog_module_local_wires(module_manager, module_id);
  for (std::pair<std::string, std::vector<BasicPort>> port_group : local_wires) {
    for (const BasicPort& local_wire : port_group.second) {
      fp << generate_verilog_port(VERILOG_PORT_WIRE, local_wire) << ";" << std::endl;
    }
  }

  /* Print an empty line as splitter */
  fp << std::endl;

  /* Print local connection (from module inputs to output! */
  print_verilog_comment(fp, std::string("----- BEGIN Local short connections -----"));
  print_verilog_module_local_short_connections(fp, module_manager, module_id);
  print_verilog_comment(fp, std::string("----- END Local short connections -----"));

  print_verilog_comment(fp, std::string("----- BEGIN Local output short connections -----"));
  print_verilog_module_output_short_connections(fp, module_manager, module_id);
 
  print_verilog_comment(fp, std::string("----- END Local output short connections -----"));
  /* Print an empty line as splitter */
  fp << std::endl;

  /* Print instances */
  for (ModuleId child_module : module_manager.child_modules(module_id)) {
    for (size_t instance : module_manager.child_module_instances(module_id, child_module)) {
      /* Print an instance */
      write_verilog_instance_to_file(fp, module_manager, module_id, child_module, instance, use_explicit_port_map); 
      /* Print an empty line as splitter */
      fp << std::endl;
    }
  }

  /* Print an end for the module */
  print_verilog_module_end(fp, module_manager.module_name(module_id)); 

  /* Print an empty line as splitter */
  fp << std::endl;
}

} /* end namespace openfpga */
