/********************************************************************
 * This file includes functions to write a SPICE module
 * based on its definition in Module Manager
 *
 * Note that SPICE writer functions are just an outputter for the
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


#include "spice_constants.h"
#include "spice_writer_utils.h"
#include "spice_subckt_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Generate the name of a local wire for a undriven port inside SPICE 
 * module 
 *******************************************************************/
static 
std::string generate_spice_undriven_local_wire_name(const ModuleManager& module_manager, 
                                                    const ModuleId& parent, 
                                                    const ModuleId& child, 
                                                    const size_t& instance_id, 
                                                    const ModulePortId& child_port_id) {
  std::string wire_name;
  if (!module_manager.instance_name(parent, child, instance_id).empty()) {
    wire_name = module_manager.instance_name(parent, child, instance_id);
  } else {
    wire_name = module_manager.module_name(parent) + std::string("_") + std::to_string(instance_id); 
    wire_name += std::string("_");
  }
  
  wire_name += std::string("_undriven_");
  wire_name += module_manager.module_port(child, child_port_id).get_name();
  
  return wire_name;
}


/********************************************************************
 * Name a net for a local wire for a SPICE subckt
 * 1. If this is a local wire, name it after the <src_module_name>_<instance_id>_<src_port_name>
 * 2. If this is not a local wire, name it after the port name of parent module 
 *
 * In addition, it will assign the pin index as well  
 *
 * Restriction: this function requires each net has single driver
 * which is definitely always true in circuits.
 *******************************************************************/
static 
BasicPort generate_spice_port_for_module_net(const ModuleManager& module_manager,
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
 * Print a SPICE wire connection 
 * We search all the sinks of the net, 
 * if we find a module output, we try to find the next module output 
 * among the sinks of the net
 * For each module output (except the first one), we print a wire connection 
 *******************************************************************/
static 
void print_spice_subckt_output_short_connection(std::fstream& fp, 
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
    VTR_ASSERT(src_port.get_width() == sink_port.get_width());
    for (size_t ipin = 0; ipin < src_port.pins().size(); ++ipin) {
      BasicPort src_spice_pin(src_port.get_name(), src_port.pins()[ipin], src_port.pins()[ipin]);
      BasicPort sink_spice_pin(sink_port.get_name(), sink_port.pins()[ipin], sink_port.pins()[ipin]);
      print_spice_short_connection(fp,
                                   generate_spice_port(src_spice_pin),
                                   generate_spice_port(sink_spice_pin));
    }
  }
}


/********************************************************************
 * Print a SPICE wire connection 
 * We search all the sources of the net, 
 * if we find a module input, we try to find a module output 
 * among the sinks of the net
 * If we find such a pair, we print a wire connection 
 *******************************************************************/
static 
void print_spice_subckt_local_short_connection(std::fstream& fp, 
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
    print_spice_comment(fp, std::string("Net source id " + std::to_string(size_t(net_src))));
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
      print_spice_comment(fp, std::string("Net sink id " + std::to_string(size_t(net_sink))));
      ModulePortId sink_port_id = module_manager.net_sink_ports(module_id, module_net)[net_sink];
      size_t sink_pin = module_manager.net_sink_pins(module_id, module_net)[net_sink];
      BasicPort sink_port(module_manager.module_port(module_id, sink_port_id).get_name(), sink_pin, sink_pin);

      /* We need to print a wire connection here */
      VTR_ASSERT(src_port.get_width() == sink_port.get_width());
      for (size_t ipin = 0; ipin < src_port.pins().size(); ++ipin) {
        BasicPort src_spice_pin(src_port.get_name(), src_port.pins()[ipin], src_port.pins()[ipin]);
        BasicPort sink_spice_pin(sink_port.get_name(), sink_port.pins()[ipin], sink_port.pins()[ipin]);
        print_spice_short_connection(fp,
                                     generate_spice_port(src_spice_pin),
                                     generate_spice_port(sink_spice_pin));
      }
    }
  }
}

/********************************************************************
 * Print short connections inside a SPICE module
 * The short connection is defined as the direct connection
 * between an input port of the module and an output port of the module
 * This type of connection is not covered when printing SPICE instances
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
void print_spice_subckt_local_short_connections(std::fstream& fp, 
                                                const ModuleManager& module_manager,
                                                const ModuleId& module_id) {
  /* Local wires come from the child modules */
  for (ModuleNetId module_net : module_manager.module_nets(module_id)) {
    /* We only care the nets that indicate short connections */ 
    if (false == module_net_include_local_short_connection(module_manager, module_id, module_net)) {
      continue;
    }
    print_spice_comment(fp, std::string("Local connection due to Wire " + std::to_string(size_t(module_net))));
    print_spice_subckt_local_short_connection(fp, module_manager, module_id, module_net); 
  }
}

/********************************************************************
 * Print output short connections inside a SPICE module
 * The output short connection is defined as the direct connection
 * between two output ports of the module
 * This type of connection is not covered when printing SPICE instances
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
void print_spice_subckt_output_short_connections(std::fstream& fp, 
                                                 const ModuleManager& module_manager,
                                                 const ModuleId& module_id) {
  /* Local wires come from the child modules */
  for (ModuleNetId module_net : module_manager.module_nets(module_id)) {
    /* We only care the nets that indicate short connections */ 
    if (false == module_net_include_output_short_connection(module_manager, module_id, module_net)) {
      continue;
    }
    print_spice_subckt_output_short_connection(fp, module_manager, module_id, module_net); 
  }
}

/********************************************************************
 * Write a SPICE instance to a file
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
void write_spice_instance_to_file(std::fstream& fp,
                                  const ModuleManager& module_manager,
                                  const ModuleId& parent_module,
                                  const ModuleId& child_module,
                                  const size_t& instance_id) {
  /* Ensure a valid file stream */
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Print instance name: 
   * if we have an instance name, use it;
   * if not, we use a default name <name>_<num_instance_in_parent_module> 
   */
  std::string instance_head_line = "X ";
  if (true == module_manager.instance_name(parent_module, child_module, instance_id).empty()) {
    instance_head_line += generate_instance_name(module_manager.module_name(child_module), instance_id);
  } else {
    instance_head_line += module_manager.instance_name(parent_module, child_module, instance_id);
  }
  instance_head_line += " ";
  fp << instance_head_line;

  /* Port sequence: global, inout, input, output and clock ports, */
  bool fit_one_line = true;
  bool new_line = false;
  size_t pin_cnt = 0;
  for (int port_type = ModuleManager::MODULE_GLOBAL_PORT;
       port_type < ModuleManager::NUM_MODULE_PORT_TYPES;
       ++port_type) {
    for (const auto& child_port_id : module_manager.module_port_ids_by_type(child_module, static_cast<ModuleManager::e_module_port_type>(port_type))) {

      BasicPort child_port = module_manager.module_port(child_module, child_port_id);

      /* Create the port name and width to be used by the instance */
      std::vector<BasicPort> instance_ports; 
      for (size_t child_pin : child_port.pins()) {
        /* Find the net linked to the pin */
        ModuleNetId net = module_manager.module_instance_port_net(parent_module, child_module, instance_id, 
                                                                  child_port_id, child_pin);
        BasicPort instance_port;
        if (ModuleNetId::INVALID() == net) {
          /* We give the same port name as child module, this case happens to global ports */
          instance_port.set_name(generate_spice_undriven_local_wire_name(module_manager, parent_module, child_module, instance_id, child_port_id));
          instance_port.set_width(child_pin, child_pin); 
        } else {
          /* Find the name for this child port */
          instance_port = generate_spice_port_for_module_net(module_manager, parent_module, net);
        }

        if (true == new_line) {
          std::string port_whitespace(instance_head_line.length() - 2, ' ');
          fp << "+ " << port_whitespace;
        }
 
        if (0 != pin_cnt) {
          write_space_to_file(fp, 1);
        }
        
        VTR_ASSERT(1 == instance_port.get_width());

        /* For single-bit port,
         * we can print the port name directly
         */
        bool omit_pin_zero = false;
        if ((1 == instance_port.pins().size())
           && (0 == instance_port.get_lsb())) {
          omit_pin_zero = true;
        }

        fp << generate_spice_port(instance_port, omit_pin_zero);

        /* Increase the counter */
        pin_cnt++;

        /* Currently we limit 10 ports per line to keep a clean netlist */
        new_line = false;
        if (SPICE_NETLIST_MAX_NUM_PORTS_PER_LINE == pin_cnt) {
          pin_cnt = 0;
          fp << std::endl;
          new_line = true;
          fit_one_line = false;
        }
      } 
    }
  }

  /* Print VDD and VSS ports
   * TODO: the supply ports should be derived from module manager
   */
  if (true == new_line) {
    std::string port_whitespace(instance_head_line.length() - 2, ' ');
    fp << "+ " << port_whitespace;
  }
  write_space_to_file(fp, 1);
  fp << SPICE_SUBCKT_VDD_PORT_NAME;
  write_space_to_file(fp, 1);
  fp << SPICE_SUBCKT_GND_PORT_NAME;

  pin_cnt += 2;

  /* Check if we need a new line */
  new_line = false;
  if (SPICE_NETLIST_MAX_NUM_PORTS_PER_LINE == pin_cnt) {
    pin_cnt = 0;
    fp << std::endl;
    new_line = true;
    fit_one_line = false;
  }

  /* Print module name: 
   * if port print cannot fit one line, we create a new line for the module for a clean format
   */
  if (false == fit_one_line) {
    fp << std::endl;
    fp << "+";
  }
  write_space_to_file(fp, 1);
  fp << module_manager.module_name(child_module);
  
  /* Print an end to the instance */
  fp << std::endl;
}

/********************************************************************
 * Write a SPICE sub-circuit to a file
 * This is a key function, maybe most frequently called in our SPICE writer
 * Note that file stream must be valid 
 *******************************************************************/
void write_spice_subckt_to_file(std::fstream& fp,
                                  const ModuleManager& module_manager,
                                  const ModuleId& module_id) {

  VTR_ASSERT(true == valid_file_stream(fp));

  /* Ensure we have a valid module_id */
  VTR_ASSERT(module_manager.valid_module_id(module_id)); 

  /* Print module declaration */
  print_spice_subckt_definition(fp, module_manager, module_id);

  /* Print an empty line as splitter */
  fp << std::endl;

  /* Print an empty line as splitter */
  fp << std::endl;

  /* Print local connection (from module inputs to output! */
  print_spice_comment(fp, std::string("BEGIN Local short connections"));
  print_spice_subckt_local_short_connections(fp, module_manager, module_id);
  print_spice_comment(fp, std::string("END Local short connections"));

  print_spice_comment(fp, std::string("BEGIN Local output short connections"));
  print_spice_subckt_output_short_connections(fp, module_manager, module_id);
 
  print_spice_comment(fp, std::string("END Local output short connections"));
  /* Print an empty line as splitter */
  fp << std::endl;

  /* Print instances */
  for (ModuleId child_module : module_manager.child_modules(module_id)) {
    for (size_t instance : module_manager.child_module_instances(module_id, child_module)) {
      /* Print an instance */
      write_spice_instance_to_file(fp, module_manager, module_id, child_module, instance); 
      /* Print an empty line as splitter */
      fp << std::endl;
    }
  }

  /* Print an end for the module */
  print_spice_subckt_end(fp, module_manager.module_name(module_id)); 

  /* Print an empty line as splitter */
  fp << std::endl;
}

} /* end namespace openfpga */
