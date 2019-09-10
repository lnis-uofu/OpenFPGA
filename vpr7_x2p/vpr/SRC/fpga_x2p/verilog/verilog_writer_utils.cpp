/************************************************
 * Include functions for most frequently
 * used Verilog writers 
 ***********************************************/
#include <chrono>
#include <ctime>
#include <string>
#include <fstream>
#include "vtr_assert.h"

/* Device-level header files */

/* FPGA-X2P context header files */
#include "spice_types.h"
#include "fpga_x2p_utils.h"

/* FPGA-Verilog context header files */
#include "verilog_global.h"
#include "verilog_writer_utils.h"

/************************************************
 * Generate header comments for a Verilog netlist
 * include the description 
 ***********************************************/
void print_verilog_file_header(std::fstream& fp,
                               const std::string& usage) {
  check_file_handler(fp);
 
  auto end = std::chrono::system_clock::now(); 
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);

  fp << "//-------------------------------------------" << std::endl;
  fp << "//\tFPGA Synthesizable Verilog Netlist" << std::endl;
  fp << "//\tDescription: " << usage << std::endl;
  fp << "//\tAuthor: Xifan TANG" << std::endl;
  fp << "//\tOrganization: University of Utah" << std::endl;
  fp << "//\tDate: " << std::ctime(&end_time) ;
  fp << "//-------------------------------------------" << std::endl;
  fp << "//----- Time scale -----" << std::endl;
  fp << "`timescale 1ns / 1ps" << std::endl;
  fp << "\n";
}


/************************************************
 * Generate include files for a Verilog netlist
 ***********************************************/
void print_verilog_include_defines_preproc_file(std::fstream& fp, 
                                                const std::string& verilog_dir) {
  check_file_handler(fp);
  
  /* Generate the file name */
  std::string include_file_path = format_dir_path(verilog_dir);
  include_file_path += defines_verilog_file_name;

  fp << "//------ Include defines: preproc flags -----" << std::endl;
  fp << "`include \"" << include_file_path << "\"" << std::endl; 
  fp << "//------ End Include defines: preproc flags -----" << std::endl << std::endl;

  return;
}

/************************************************
 * Print a Verilog comment line
 ***********************************************/
void print_verilog_comment(std::fstream& fp, 
                           const std::string& comment) {
  check_file_handler(fp);

  fp << "// " << comment << std::endl;
}

/************************************************
 * Print a Verilog module definition
 * We use the following format:
 * module <module_name> (<ports without directions>);
 ***********************************************/
void print_verilog_module_definition(std::fstream& fp, 
                                     const ModuleManager& module_manager, const ModuleId& module_id) {
  check_file_handler(fp);

  print_verilog_comment(fp, std::string("----- Verilog module for " + module_manager.module_name(module_id) + " -----"));

  std::string module_head_line = "module " + module_manager.module_name(module_id) + "(";
  fp << module_head_line;

  /* port type2type mapping */
  std::map<ModuleManager::e_module_port_type, enum e_dump_verilog_port_type> port_type2type_map;
  port_type2type_map[ModuleManager::MODULE_GLOBAL_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_INOUT_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_INPUT_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_OUTPUT_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_CLOCK_PORT] = VERILOG_PORT_CONKT;

  /* Port sequence: global, inout, input, output and clock ports, */
  size_t port_cnt = 0;
  for (const auto& kv : port_type2type_map) {
    for (const auto& port : module_manager.module_ports_by_type(module_id, kv.first)) {
      if (0 != port_cnt) {
        /* Do not dump a comma for the first port */
        fp << "," << std::endl; 
      }
      /* Create a space for "module <module_name>" except the first line! */
      if (0 != port_cnt) {
        std::string port_whitespace(module_head_line.length(), ' ');
        fp << port_whitespace;
      }
      /* Print port */
      fp << generate_verilog_port(kv.second, port);
      port_cnt++;
    }
  }
  fp << ");" << std::endl;
}

/************************************************
 * Print a Verilog module ports based on the module id 
 ***********************************************/
void print_verilog_module_ports(std::fstream& fp, 
                                const ModuleManager& module_manager, const ModuleId& module_id) {
  check_file_handler(fp);

  /* port type2type mapping */
  std::map<ModuleManager::e_module_port_type, enum e_dump_verilog_port_type> port_type2type_map;
  port_type2type_map[ModuleManager::MODULE_GLOBAL_PORT] = VERILOG_PORT_INPUT;
  port_type2type_map[ModuleManager::MODULE_INOUT_PORT] = VERILOG_PORT_INOUT;
  port_type2type_map[ModuleManager::MODULE_INPUT_PORT] = VERILOG_PORT_INPUT;
  port_type2type_map[ModuleManager::MODULE_OUTPUT_PORT] = VERILOG_PORT_OUTPUT;
  port_type2type_map[ModuleManager::MODULE_CLOCK_PORT] = VERILOG_PORT_INPUT;

  /* Port sequence: global, inout, input, output and clock ports, */
  for (const auto& kv : port_type2type_map) {
    for (const auto& port : module_manager.module_ports_by_type(module_id, kv.first)) {
      /* Print port */
      fp << "//----- " << module_manager.module_port_type_str(kv.first)  << " -----" << std::endl; 
      fp << generate_verilog_port(kv.second, port);
      fp << ";" << std::endl;
    }
  }
 
  /* Output any port that is registered */
  fp << "//----- Registered ports -----" << std::endl; 
  for (const auto& kv : port_type2type_map) {
    for (const auto& port : module_manager.module_ports_by_type(module_id, kv.first)) {
      /* Skip the ports that are not registered */
      ModulePortId port_id = module_manager.find_module_port(module_id, port.get_name());
      VTR_ASSERT(ModulePortId::INVALID() != port_id);
      if (false == module_manager.port_is_register(module_id, port_id)) {
        continue;
      }
      /* Print port */
      fp << generate_verilog_port(VERILOG_PORT_REG, port);
      fp << ";" << std::endl;
    }
  }
}

/************************************************
 * Print a Verilog module declaration (definition + port list
 * We use the following format:
 * module <module_name> (<ports without directions>);
 * <tab><port definition with direction> 
 ***********************************************/
void print_verilog_module_declaration(std::fstream& fp, 
                                      const ModuleManager& module_manager, const ModuleId& module_id) {
  check_file_handler(fp);

  print_verilog_module_definition(fp, module_manager, module_id);

  print_verilog_module_ports(fp, module_manager, module_id);
}

/************************************************
 * Print an instance for a Verilog module
 * This function will output the port map
 * by referring to a port-to-port mapping:
 *   <module_port_name> -> <instance_port_name>
 * The key of the port-to-port mapping is the 
 * port name of the module: 
 * The value of the port-to-port mapping is the
 * port information of the instance
 * With link between module and instance, the function
 * can output a Verilog instance easily, supporting
 * both explicit port mapping:
 *   .<module_port_name>(<instance_port_name>)
 * and inexplicit port mapping
 *   <instance_port_name>
 *
 * Note that, it is not necessary that 
 * the port-to-port mapping covers all the module ports.
 * Any instance/module port which are not specified in the 
 * port-to-port mapping will be output by the module 
 * port name.
 ***********************************************/
void print_verilog_module_instance(std::fstream& fp, 
                                   const ModuleManager& module_manager, 
                                   const ModuleId& parent_module_id, const ModuleId& child_module_id,
                                   const std::map<std::string, BasicPort>& port2port_name_map,
                                   const bool& explicit_port_map) {

  check_file_handler(fp);

  /* Check: all the key ports in the port2port_name_map does exist in the child module */
  for (const auto& kv : port2port_name_map) {
    ModulePortId module_port_id = module_manager.find_module_port(child_module_id, kv.first);
    VTR_ASSERT(ModulePortId::INVALID() != module_port_id);
  }

  /* Print module name */
  fp << "\t" << module_manager.module_name(child_module_id) << " ";
  /* Print instance name, <name>_<num_instance_in_parent_module> */
  fp << module_manager.module_name(child_module_id) << "_" << module_manager.num_instance(parent_module_id, child_module_id) << "_" << " (" << std::endl;
  
  /* Print each port with/without explicit port map */
  /* port type2type mapping */
  std::map<ModuleManager::e_module_port_type, enum e_dump_verilog_port_type> port_type2type_map;
  port_type2type_map[ModuleManager::MODULE_GLOBAL_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_INOUT_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_INPUT_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_OUTPUT_PORT] = VERILOG_PORT_CONKT;
  port_type2type_map[ModuleManager::MODULE_CLOCK_PORT] = VERILOG_PORT_CONKT;

  /* Port sequence: global, inout, input, output and clock ports, */
  size_t port_cnt = 0;
  for (const auto& kv : port_type2type_map) {
    for (const auto& port : module_manager.module_ports_by_type(child_module_id, kv.first)) {
      if (0 != port_cnt) {
        /* Do not dump a comma for the first port */
        fp << "," << std::endl; 
      }
      /* Print port */
      fp << "\t\t";
      /* if explicit port map is required, output the port name */
      if (true == explicit_port_map) {
        fp << "." << port.get_name() << "(";
      }
      /* Try to find the instanced port name in the name map */
      if (port2port_name_map.find(port.get_name()) != port2port_name_map.end()) {
        /* Found it, we assign the port name */ 
        /* TODO: make sure the port width matches! */
        ModulePortId module_port_id = module_manager.find_module_port(child_module_id, port.get_name());
        /* Get the port from module */
        BasicPort module_port = module_manager.module_port(child_module_id, module_port_id);
        VTR_ASSERT(module_port.get_width() == port2port_name_map.at(port.get_name()).get_width());
        fp << generate_verilog_port(kv.second, port2port_name_map.at(port.get_name()));
      } else {
        /* Not found, we give the default port name */
        fp << generate_verilog_port(kv.second, port);
      }
      /* if explicit port map is required, output the pair of branket */
      if (true == explicit_port_map) {
        fp << ")";
      }
      port_cnt++;
    }
  }
  
  /* Print an end to the instance */
  fp << ");" << std::endl;
}

/************************************************
 * Print an end line for a Verilog module
 ***********************************************/
void print_verilog_module_end(std::fstream& fp, 
                              const std::string& module_name) {
  check_file_handler(fp);

  fp << "endmodule" << std::endl;
  print_verilog_comment(fp, std::string("----- END Verilog module for " + module_name + " -----"));
  fp << std::endl;
}

/************************************************
 * Generate a string of a Verilog port  
 ***********************************************/
std::string generate_verilog_port(const enum e_dump_verilog_port_type& verilog_port_type,
                                  const BasicPort& port_info) {  
  std::string verilog_line;

  /* Ensure the port type is valid */
  VTR_ASSERT(verilog_port_type < NUM_VERILOG_PORT_TYPES);

  std::string size_str = "[" + std::to_string(port_info.get_lsb()) + ":" + std::to_string(port_info.get_msb()) + "]";

  /* Only connection require a format of <port_name>[<lsb>:<msb>]
   * others require a format of <port_type> [<lsb>:<msb>] <port_name> 
   */
  if (VERILOG_PORT_CONKT == verilog_port_type) {
    /* When LSB == MSB, we can use a simplified format <port_type>[<lsb>]*/
    if ( 1 == port_info.get_width()) {
      size_str = "[" + std::to_string(port_info.get_lsb()) + "]";
    }
    verilog_line = port_info.get_name() + size_str;
  } else { 
    verilog_line = VERILOG_PORT_TYPE_STRING[verilog_port_type]; 
    verilog_line += " " + size_str + " " + port_info.get_name();
  }

  return verilog_line;
}

/************************************************
 * This function takes a list of ports and
 * combine the port string by comparing the name 
 * and width of ports.
 * For example, two ports A and B share the same name is 
 * mergable as long as A's MSB + 1 == B's LSB
 * Note that the port sequence really matters!
 * This function will NOT change the sequence
 * of ports in the list port_info
 ***********************************************/
std::vector<BasicPort> combine_verilog_ports(const std::vector<BasicPort>& ports) { 
  std::vector<BasicPort> merged_ports;

  /* Directly return if there are no ports */
  if (0 == ports.size()) {
    return merged_ports;
  }
  /* Push the first port to the merged ports */
  merged_ports.push_back(ports[0]);

  /* Iterate over ports */
  for (const auto& port : ports) {
    /* Bypass the first port, it is already in the list */
    if (&port == &ports[0]) {
      continue;
    } 
    /* Identify if the port name can be potentially merged: if the port name is already in the merged port list, it may be merged */
    for (auto& merged_port : merged_ports) {
      if (0 != port.get_name().compare(merged_port.get_name())) {
        /* Unable to merge, add the port to merged port list */
        merged_ports.push_back(port);
        /* Go to next */
        break;
      }
      /* May be merged, check LSB of port and MSB of merged_port */
      if (merged_port.get_msb() + 1 != port.get_lsb()) {
        /* Unable to merge, add the port to merged port list */
        merged_ports.push_back(port);
        /* Go to next */
        break;
      } 
      /* Reach here, we should merge the ports,
       * LSB of merged_port remains the same,
       * MSB of merged_port will be updated 
       * to the MSB of port 
       */
      merged_port.set_msb(port.get_msb());
      break;
    }
  }

  return merged_ports;
}

/************************************************
 * Generate the string of a list of verilog ports 
 ***********************************************/
std::string generate_verilog_ports(const std::vector<BasicPort>& merged_ports) {  

  /* Output the string of ports:
   * If there is only one port in the merged_port list
   * we only output the port.
   * If there are more than one port in the merged port list, we output an concatenated port:
   *  {<port1>, <port2>, ... <last_port>}  
   */
  VTR_ASSERT(0 < merged_ports.size());
  if ( 1 == merged_ports.size()) {
    /* Use connection type of verilog port */
    return generate_verilog_port(VERILOG_PORT_CONKT, merged_ports[0]);  
  }

  std::string verilog_line = "{";
  for (const auto& port : merged_ports) {
    /* The first port does not need a comma */
    if (&port != &merged_ports[0]) {
      verilog_line += ", ";
    }
    verilog_line += generate_verilog_port(VERILOG_PORT_CONKT, merged_ports[0]);  
  }
  verilog_line += "}";

  return verilog_line;
}

/********************************************************************
 * Generate a bus port (could be used to create a local wire) 
 * for a list of Verilog ports
 * The bus port will be created by aggregating the ports in the list 
 * A bus port name may be need only there are many ports with 
 * different names. It is hard to name the bus port
 *******************************************************************/
BasicPort generate_verilog_bus_port(const std::vector<BasicPort>& input_ports, 
                                    const std::string& bus_port_name) {
  /* Try to combine the ports */
  std::vector<BasicPort> combined_input_ports = combine_verilog_ports(input_ports);
  
  /* Create a port data structure that is to be returned */
  BasicPort bus_port;

  if (1 == combined_input_ports.size()) {
    bus_port = combined_input_ports[0];
  } else {
    /* TODO: the naming could be more flexible? */
    bus_port.set_name(bus_port_name);
    /* Deposite a [0:0] port */
    bus_port.set_width(1);
    for (const auto& port : combined_input_ports) {
      bus_port.combine(port);
    }
  }
  
  return bus_port;
}

/********************************************************************
 * Generate a bus wire declaration for a list of Verilog ports
 * Output ports: the local_wire name
 * Input ports: the driving ports
 * When there are more than two ports, a bus wiring will be created
 *     {<port0>, <port1>, ... <last_port>}
 *******************************************************************/
std::string generate_verilog_local_wire(const BasicPort& output_port,
                                        const std::vector<BasicPort>& input_ports) {
  /* Try to combine the ports */
  std::vector<BasicPort> combined_input_ports = combine_verilog_ports(input_ports);

  /* If we have more than 1 port in the combined ports , 
   * output a local wire */
  VTR_ASSERT(0 < combined_input_ports.size());

  /* Must check: the port width matches */
  size_t input_ports_width = 0;
  for (const auto& port : combined_input_ports) {
    /* We must have valid ports! */
    VTR_ASSERT( 0 < port.get_width() );
    input_ports_width += port.get_width();
  }
  VTR_ASSERT( input_ports_width == output_port.get_width() );

  std::string wire_str;
  wire_str += generate_verilog_port(VERILOG_PORT_WIRE, output_port);
  wire_str += " = ";
  wire_str += generate_verilog_ports(combined_input_ports);
  wire_str += ";";

  return wire_str;
}

/********************************************************************
 * Generate a string for a constant value in Verilog format:
 *  <#.of bits>'b<binary numbers>
 *******************************************************************/
std::string generate_verilog_constant_values(const std::vector<size_t>& const_values) {
  std::string str = std::to_string(const_values.size());
  str += "'b";
  for (const auto& val : const_values) {
    str += std::to_string(val);
  }
  return str;
}

/********************************************************************
 * Generate a verilog port with a deposite of constant values
 ********************************************************************/
std::string generate_verilog_port_constant_values(const BasicPort& output_port,
                                                  const std::vector<size_t>& const_values) {
  std::string port_str;

  /* Must check: the port width matches */
  VTR_ASSERT( const_values.size() == output_port.get_width() );

  port_str = generate_verilog_port(VERILOG_PORT_CONKT, output_port);
  port_str += " = ";
  port_str += generate_verilog_constant_values(const_values);
  return port_str;
}

/********************************************************************
 * Generate a wire connection, that assigns constant values to a 
 * Verilog port 
 *******************************************************************/
void print_verilog_wire_constant_values(std::fstream& fp,
                                        const BasicPort& output_port,
                                        const std::vector<size_t>& const_values) {
  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

  fp << "\t";
  fp << "assign ";
  fp << generate_verilog_port_constant_values(output_port, const_values);
  fp << ";" << std::endl;
}

/********************************************************************
 * Generate a wire connection for two Verilog ports 
 * using "assign" syntax  
 *******************************************************************/
void print_verilog_wire_connection(std::fstream& fp,
                                   const BasicPort& output_port,
                                   const BasicPort& input_port, 
                                   const bool& inverted) {
  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

  /* Must check: the port width matches */
  VTR_ASSERT( input_port.get_width() == output_port.get_width() );

  fp << "\t";
  fp << "assign ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, output_port);
  fp << " = ";
  
  if (true == inverted) {
    fp << "~";
  }

  fp << generate_verilog_port(VERILOG_PORT_CONKT, input_port);
  fp << ";" << std::endl;
}

/********************************************************************
 * Generate an instance of a buffer module  
 * with given information about the input and output ports of instance
 *
 *                                          Buffer instance
 *                             +----------------------------------------+
 *     instance_input_port --->| buffer_input_port    buffer_output_port|----> instance_output_port 
 *                             +----------------------------------------+
 *
 * Restrictions:
 *    Buffer must have only 1 input (non-global) port and 1 output (non-global) port
 *******************************************************************/
void print_verilog_buffer_instance(std::fstream& fp,
                                   ModuleManager& module_manager, 
                                   const CircuitLibrary& circuit_lib, 
                                   const ModuleId& parent_module_id, 
                                   const CircuitModelId& buffer_model, 
                                   const BasicPort& instance_input_port,
                                   const BasicPort& instance_output_port) {
  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

  /* To match the context, Buffer should have only 2 non-global ports: 1 input port and 1 output port */
  std::vector<CircuitPortId> buffer_model_input_ports = circuit_lib.model_ports_by_type(buffer_model, SPICE_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> buffer_model_output_ports = circuit_lib.model_ports_by_type(buffer_model, SPICE_MODEL_PORT_OUTPUT, true);
  VTR_ASSERT(1 == buffer_model_input_ports.size());
  VTR_ASSERT(1 == buffer_model_output_ports.size());

  /* Get the moduleId for the buffer module */
  ModuleId buffer_module_id = module_manager.find_module(circuit_lib.model_name(buffer_model));
  /* We must have one */
  VTR_ASSERT(ModuleId::INVALID() != buffer_module_id);

  /* Create a port-to-port map */
  std::map<std::string, BasicPort> buffer_port2port_name_map;

  /* Build the link between buffer_input_port[0] and output_node_pre_buffer 
   * Build the link between buffer_output_port[0] and output_node_bufferred
   */
  { /* Create a code block to accommodate the local variables */
    std::string module_input_port_name = circuit_lib.port_lib_name(buffer_model_input_ports[0]);
    buffer_port2port_name_map[module_input_port_name] = instance_input_port; 
    std::string module_output_port_name = circuit_lib.port_lib_name(buffer_model_output_ports[0]);
    buffer_port2port_name_map[module_output_port_name] = instance_output_port; 
  }

  /* Output an instance of the module */
  print_verilog_module_instance(fp, module_manager, parent_module_id, buffer_module_id, buffer_port2port_name_map, circuit_lib.dump_explicit_port_map(buffer_model));

  /* IMPORTANT: this update MUST be called after the instance outputting!!!!
   * update the module manager with the relationship between the parent and child modules 
   */
  module_manager.add_child_module(parent_module_id, buffer_module_id);
}

