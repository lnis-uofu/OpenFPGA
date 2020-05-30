/************************************************
 * Include functions for most frequently
 * used Verilog writers 
 ***********************************************/
#include <chrono>
#include <ctime>
#include <string>
#include <fstream>
#include <iomanip>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from readarchopenfpga library */
#include "circuit_types.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "openfpga_naming.h"
#include "circuit_library_utils.h"
#include "verilog_constants.h"
#include "verilog_writer_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/************************************************
 * Generate header comments for a Verilog netlist
 * include the description 
 ***********************************************/
void print_verilog_file_header(std::fstream& fp,
                               const std::string& usage) {
  VTR_ASSERT(true == valid_file_stream(fp));
 
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
  fp << std::endl;
}

/********************************************************************
 * Print Verilog codes to include a netlist  
 *******************************************************************/
void print_verilog_include_netlist(std::fstream& fp, 
                                   const std::string& netlist_name) {
  VTR_ASSERT(true == valid_file_stream(fp));

  fp << "`include \"" << netlist_name << "\"" << std::endl; 
}

/********************************************************************
 * Print Verilog codes to define a preprocessing flag
 *******************************************************************/
void print_verilog_define_flag(std::fstream& fp, 
                               const std::string& flag_name,
                               const int& flag_value) {
  VTR_ASSERT(true == valid_file_stream(fp));

  fp << "`define " << flag_name << " " << flag_value << std::endl; 
}

/************************************************
 * Generate include files for a Verilog netlist
 ***********************************************/
void print_verilog_include_defines_preproc_file(std::fstream& fp, 
                                                const std::string& verilog_dir) {
  
  /* Generate the file name */
  std::string include_file_path = format_dir_path(verilog_dir);
  include_file_path += std::string(DEFINES_VERILOG_FILE_NAME);

  print_verilog_include_netlist(fp, include_file_path); 
}

/************************************************
 * Print a Verilog comment line
 ***********************************************/
void print_verilog_comment(std::fstream& fp, 
                           const std::string& comment) {
  VTR_ASSERT(true == valid_file_stream(fp));

  fp << "// " << comment << std::endl;
}

/************************************************
 * Print the declaration of a Verilog preprocessing flag
 ***********************************************/
void print_verilog_preprocessing_flag(std::fstream& fp,
                                      const std::string& preproc_flag) {
  VTR_ASSERT(true == valid_file_stream(fp));

  fp << "`ifdef " << preproc_flag << std::endl;
}

/************************************************
 * Print the endif of a Verilog preprocessing flag
 ***********************************************/
void print_verilog_endif(std::fstream& fp) {
  VTR_ASSERT(true == valid_file_stream(fp));

  fp << "`endif" << std::endl;
}

/************************************************
 * Print a Verilog module definition
 * We use the following format:
 * module <module_name> (<ports without directions>);
 ***********************************************/
void print_verilog_module_definition(std::fstream& fp, 
                                     const ModuleManager& module_manager, const ModuleId& module_id) {
  VTR_ASSERT(true == valid_file_stream(fp));

  print_verilog_comment(fp, std::string("----- Verilog module for " + module_manager.module_name(module_id) + " -----"));

  std::string module_head_line = "module " + module_manager.module_name(module_id) + "(";
  fp << module_head_line;

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
  bool printed_ifdef = false; /* A flag to tell if an ifdef has been printed for the last port */
  for (const auto& kv : port_type2type_map) {
    for (const auto& port : module_manager.module_ports_by_type(module_id, kv.first)) {
      if (0 != port_cnt) {
        /* Do not dump a comma for the first port */
        fp << "," << std::endl; 
      }

      if (true == printed_ifdef) {
        /* Print an endif to pair the ifdef */
        print_verilog_endif(fp);
        /* Reset the flag */
        printed_ifdef = false;
      }

      ModulePortId port_id = module_manager.find_module_port(module_id, port.get_name());
      VTR_ASSERT(ModulePortId::INVALID() != port_id);
      /* Print pre-processing flag for a port, if defined */
      std::string preproc_flag = module_manager.port_preproc_flag(module_id, port_id);
      if (false == preproc_flag.empty()) {
        /* Print an ifdef Verilog syntax */
        print_verilog_preprocessing_flag(fp, preproc_flag);
        /* Raise the flag */
        printed_ifdef = true;
      }

      /* Create a space for "module <module_name>" except the first line! */
      if (0 != port_cnt) {
        std::string port_whitespace(module_head_line.length(), ' ');
        fp << port_whitespace;
      }
      /* Print port: only the port name is enough */
      fp << port.get_name();

      /* Increase the counter */
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
  VTR_ASSERT(true == valid_file_stream(fp));

  /* port type2type mapping */
  std::map<ModuleManager::e_module_port_type, enum e_dump_verilog_port_type> port_type2type_map;
  port_type2type_map[ModuleManager::MODULE_GLOBAL_PORT] = VERILOG_PORT_INPUT;
  port_type2type_map[ModuleManager::MODULE_GPIN_PORT] = VERILOG_PORT_INPUT;
  port_type2type_map[ModuleManager::MODULE_GPOUT_PORT] = VERILOG_PORT_OUTPUT;
  port_type2type_map[ModuleManager::MODULE_GPIO_PORT] = VERILOG_PORT_INOUT;
  port_type2type_map[ModuleManager::MODULE_INOUT_PORT] = VERILOG_PORT_INOUT;
  port_type2type_map[ModuleManager::MODULE_INPUT_PORT] = VERILOG_PORT_INPUT;
  port_type2type_map[ModuleManager::MODULE_OUTPUT_PORT] = VERILOG_PORT_OUTPUT;
  port_type2type_map[ModuleManager::MODULE_CLOCK_PORT] = VERILOG_PORT_INPUT;

  /* Port sequence: global, inout, input, output and clock ports, */
  for (const auto& kv : port_type2type_map) {
    for (const auto& port : module_manager.module_ports_by_type(module_id, kv.first)) {
      ModulePortId port_id = module_manager.find_module_port(module_id, port.get_name());
      VTR_ASSERT(ModulePortId::INVALID() != port_id);
      /* Print pre-processing flag for a port, if defined */
      std::string preproc_flag = module_manager.port_preproc_flag(module_id, port_id);
      if (false == preproc_flag.empty()) {
        /* Print an ifdef Verilog syntax */
        print_verilog_preprocessing_flag(fp, preproc_flag);
      }

      /* Print port */
      fp << "//----- " << module_manager.module_port_type_str(kv.first)  << " -----" << std::endl; 
      fp << generate_verilog_port(kv.second, port);
      fp << ";" << std::endl;

      if (false == preproc_flag.empty()) {
        /* Print an endif to pair the ifdef */
        print_verilog_endif(fp);
      }
    }
  }

  /* Output any port that is also wire connection */
  fp << std::endl;
  fp << "//----- BEGIN wire-connection ports -----" << std::endl; 
  for (const auto& kv : port_type2type_map) {
    for (const auto& port : module_manager.module_ports_by_type(module_id, kv.first)) {
      /* Skip the ports that are not registered */
      ModulePortId port_id = module_manager.find_module_port(module_id, port.get_name());
      VTR_ASSERT(ModulePortId::INVALID() != port_id);
      if (false == module_manager.port_is_wire(module_id, port_id)) {
        continue;
      }

      /* Print pre-processing flag for a port, if defined */
      std::string preproc_flag = module_manager.port_preproc_flag(module_id, port_id);
      if (false == preproc_flag.empty()) {
        /* Print an ifdef Verilog syntax */
        print_verilog_preprocessing_flag(fp, preproc_flag);
      }

      /* Print port */
      fp << generate_verilog_port(VERILOG_PORT_WIRE, port);
      fp << ";" << std::endl;

      if (false == preproc_flag.empty()) {
        /* Print an endif to pair the ifdef */
        print_verilog_endif(fp);
      }
    }
  }
  fp << "//----- END wire-connection ports -----" << std::endl; 
  fp << std::endl;

 
  /* Output any port that is registered */
  fp << std::endl;
  fp << "//----- BEGIN Registered ports -----" << std::endl; 
  for (const auto& kv : port_type2type_map) {
    for (const auto& port : module_manager.module_ports_by_type(module_id, kv.first)) {
      /* Skip the ports that are not registered */
      ModulePortId port_id = module_manager.find_module_port(module_id, port.get_name());
      VTR_ASSERT(ModulePortId::INVALID() != port_id);
      if (false == module_manager.port_is_register(module_id, port_id)) {
        continue;
      }

      /* Print pre-processing flag for a port, if defined */
      std::string preproc_flag = module_manager.port_preproc_flag(module_id, port_id);
      if (false == preproc_flag.empty()) {
        /* Print an ifdef Verilog syntax */
        print_verilog_preprocessing_flag(fp, preproc_flag);
      }

      /* Print port */
      fp << generate_verilog_port(VERILOG_PORT_REG, port);
      fp << ";" << std::endl;

      if (false == preproc_flag.empty()) {
        /* Print an endif to pair the ifdef */
        print_verilog_endif(fp);
      }
    }
  }
  fp << "//----- END Registered ports -----" << std::endl; 
  fp << std::endl;
}

/************************************************
 * Print a Verilog module declaration (definition + port list
 * We use the following format:
 * module <module_name> (<ports without directions>);
 * <tab><port definition with direction> 
 ***********************************************/
void print_verilog_module_declaration(std::fstream& fp, 
                                      const ModuleManager& module_manager, const ModuleId& module_id) {
  VTR_ASSERT(true == valid_file_stream(fp));

  print_verilog_module_definition(fp, module_manager, module_id);

  print_verilog_module_ports(fp, module_manager, module_id);
}


/********************************************************************
 * Print an instance in Verilog format (a generic version)
 * This function will require user to provide an instance name
 *
 * This function will output the port map by referring to a port-to-port 
 * mapping:
 *   <module_port_name> -> <instance_port_name>
 * The key of the port-to-port mapping is the port name of the module: 
 * The value of the port-to-port mapping is the port information of the instance
 * With link between module and instance, the function can output a Verilog 
 * instance easily, supporting both explicit port mapping:
 *   .<module_port_name>(<instance_port_name>)
 * and inexplicit port mapping
 *   <instance_port_name>
 *
 * Note that, it is not necessary that the port-to-port mapping 
 * covers all the module ports.
 * Any instance/module port which are not specified in the port-to-port 
 * mapping will be output by the module port name.
 *******************************************************************/
void print_verilog_module_instance(std::fstream& fp, 
                                   const ModuleManager& module_manager, 
                                   const ModuleId& module_id,
                                   const std::string& instance_name,
                                   const std::map<std::string, BasicPort>& port2port_name_map,
                                   const bool& use_explicit_port_map) {

  VTR_ASSERT(true == valid_file_stream(fp));

  /* Check: all the key ports in the port2port_name_map does exist in the child module */
  for (const auto& kv : port2port_name_map) {
    ModulePortId module_port_id = module_manager.find_module_port(module_id, kv.first);
    VTR_ASSERT(ModulePortId::INVALID() != module_port_id);
  }

  /* Print module name */
  fp << "\t" << module_manager.module_name(module_id) << " ";
  /* Print instance name */
  fp << instance_name << " (" << std::endl;
  
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
    for (const auto& port : module_manager.module_ports_by_type(module_id, kv.first)) {
      if (0 != port_cnt) {
        /* Do not dump a comma for the first port */
        fp << "," << std::endl; 
      }
      /* Print port */
      fp << "\t\t";
      /* if explicit port map is required, output the port name */
      if (true == use_explicit_port_map) {
        fp << "." << port.get_name() << "(";
      }
      /* Try to find the instanced port name in the name map */
      if (port2port_name_map.find(port.get_name()) != port2port_name_map.end()) {
        /* Found it, we assign the port name */ 
        /* TODO: make sure the port width matches! */
        ModulePortId module_port_id = module_manager.find_module_port(module_id, port.get_name());
        /* Get the port from module */
        BasicPort module_port = module_manager.module_port(module_id, module_port_id);
        VTR_ASSERT(module_port.get_width() == port2port_name_map.at(port.get_name()).get_width());
        fp << generate_verilog_port(kv.second, port2port_name_map.at(port.get_name()));
      } else {
        /* Not found, we give the default port name */
        fp << generate_verilog_port(kv.second, port);
      }
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


/************************************************
 * Print an instance for a Verilog module
 * This function is a wrapper for the generic version of
 * print_verilog_module_instance() 
 * This function create an instance name based on the index
 * of the child module in its parent module
 ***********************************************/
void print_verilog_module_instance(std::fstream& fp, 
                                   const ModuleManager& module_manager, 
                                   const ModuleId& parent_module_id, const ModuleId& child_module_id,
                                   const std::map<std::string, BasicPort>& port2port_name_map,
                                   const bool& use_explicit_port_map) {

  /* Create instance name, <name>_<num_instance_in_parent_module> */
  std::string instance_name = module_manager.module_name(child_module_id) 
                            + "_" 
                            + std::to_string(module_manager.num_instance(parent_module_id, child_module_id)) 
                            + "_";

  print_verilog_module_instance(fp, module_manager, child_module_id, instance_name,
                                port2port_name_map, use_explicit_port_map);
}

/************************************************
 * Print an end line for a Verilog module
 ***********************************************/
void print_verilog_module_end(std::fstream& fp, 
                              const std::string& module_name) {
  VTR_ASSERT(true == valid_file_stream(fp));

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

/********************************************************************
 * Evaluate if two Verilog ports can be merged:
 * If the port name is same, it can merged 
 *******************************************************************/
bool two_verilog_ports_mergeable(const BasicPort& portA,
                                 const BasicPort& portB) {
  if (0 == portA.get_name().compare(portB.get_name())) {
    return true;
  }
  return false;
}

/********************************************************************
 * Merge two Verilog ports, return the merged port
 * The ports should have the same name
 * The new LSB will be minimum of the LSBs of the two ports
 * The new MSB will the maximum of the MSBs of the two ports
 *******************************************************************/
BasicPort merge_two_verilog_ports(const BasicPort& portA,
                                  const BasicPort& portB) {
  BasicPort merged_port;
  
  VTR_ASSERT(true == two_verilog_ports_mergeable(portA, portB));

  merged_port.set_name(portA.get_name());
  merged_port.set_lsb((size_t)std::min((int)portA.get_lsb(), (int)portB.get_lsb()));
  merged_port.set_msb((size_t)std::max((int)portA.get_msb(), (int)portB.get_msb()));

  return merged_port;
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
    bool merged = false;
    for (auto& merged_port : merged_ports) {
      if (false == port.mergeable(merged_port)) {
        /* Unable to merge, Go to next */
        continue;
      }
      /* May be merged, check LSB of port and MSB of merged_port */
      if (merged_port.get_msb() + 1 != port.get_lsb()) {
        /* Unable to merge, Go to next */
        continue;
      } 
      /* Reach here, we should merge the ports,
       * LSB of merged_port remains the same,
       * MSB of merged_port will be updated 
       * to the MSB of port 
       */
      merged_port.set_msb(port.get_msb());
      merged = true;
      break;
    }
    if (false == merged) {
      /* Unable to merge, add the port to merged port list */
      merged_ports.push_back(port);
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
    verilog_line += generate_verilog_port(VERILOG_PORT_CONKT, port);  
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
 *
 * Optimization: short_constant
 *   When this switch is turned on, we will generate short version
 *   for all-zero/all-one vectors
 *   {<length>{1'b<zero/one>}}
 *******************************************************************/
std::string generate_verilog_constant_values(const std::vector<size_t>& const_values,
                                             const bool& short_constant) {
  VTR_ASSERT(!const_values.empty());

  bool same_values = true;
  size_t first_val = const_values.back();
  if (true == short_constant) {
    for (const auto& val : const_values) {
      if (first_val != val) {
        same_values = false;
        break;
      }
    }
  }

  std::string str;

  if ( (true == short_constant) 
    && (true == same_values) ) {
    str = "{" + std::to_string(const_values.size()) + "{1'b" + std::to_string(first_val) + "}}";
  } else {
    str = std::to_string(const_values.size());
    str += "'b";
    for (const auto& val : const_values) {
      str += std::to_string(val);
    }
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
  VTR_ASSERT(true == valid_file_stream(fp));

  fp << "\t";
  fp << "assign ";
  fp << generate_verilog_port_constant_values(output_port, const_values);
  fp << ";" << std::endl;
}

/********************************************************************
 * Deposit constant values to a Verilog port 
 *******************************************************************/
void print_verilog_deposit_wire_constant_values(std::fstream& fp,
                                                const BasicPort& output_port,
                                                const std::vector<size_t>& const_values) {
  /* Make sure we have a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

  fp << "\t";
  fp << "$deposit(";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, output_port);
  fp << ", ";
  fp << generate_verilog_constant_values(const_values);
  fp << ");" << std::endl;
}

/********************************************************************
 * Generate a wire connection, that assigns constant values to a 
 * Verilog port 
 *******************************************************************/
void print_verilog_force_wire_constant_values(std::fstream& fp,
                                              const BasicPort& output_port,
                                              const std::vector<size_t>& const_values) {
  /* Make sure we have a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

  fp << "\t";
  fp << "force ";
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
  VTR_ASSERT(true == valid_file_stream(fp));

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
 * Generate a wire connection for two Verilog ports 
 * using "assign" syntax  
 *******************************************************************/
void print_verilog_register_connection(std::fstream& fp,
                                       const BasicPort& output_port,
                                       const BasicPort& input_port, 
                                       const bool& inverted) {
  /* Make sure we have a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Must check: the port width matches */
  VTR_ASSERT( input_port.get_width() == output_port.get_width() );

  fp << "\t";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, output_port);
  fp << " <= ";
  
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
  VTR_ASSERT(true == valid_file_stream(fp));

  /* To match the context, Buffer should have only 2 non-global ports: 1 input port and 1 output port */
  std::vector<CircuitPortId> buffer_model_input_ports = circuit_lib.model_ports_by_type(buffer_model, CIRCUIT_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> buffer_model_output_ports = circuit_lib.model_ports_by_type(buffer_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
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

/********************************************************************
 * Print local wires that are used for SRAM configuration 
 * The local wires are strongly dependent on the organization of SRAMs.
 * Standalone SRAMs: 
 * -----------------
 *    No need for local wires, their outputs are port of the module
 *        
 *              Module
 *             +------------------------------+
 *             |  Sub-module                  |
 *             |  +---------------------+     |
 *             |  |             sram_out|---->|---->sram_out
 *             |  |                     |     |
 *             |  |             sram_out|---->|---->sram_out
 *             |  |                     |     |
 *             |  +---------------------+     |
 *             +------------------------------+      
 *
 * Configuration chain-style
 * -------------------------
 * wire [0:N] config_bus
 *
 *
 *           Module
 *          +--------------------------------------------------------------+
 *          |  config_bus    config_bus    config_bus       config_bus     |
 *          |     [0]           [1]           [2]              [N]         |
 *          |      |             |             |                |          |
 *          |      v             v             v                v          |
 * ccff_head| ----------+  +---------+   +------------+   +----------------|-> ccff_tail
 *          |           |  ^         |   ^            |   ^                |
 *          |      head v  |tail     v   |            v   |                |
 *          |       +----------+  +----------+     +----------+            |
 *          |       |  Memory  |  |  Memory  |     |  Memory  |            |
 *          |       |  Module  |  |  Module  | ... |  Module  |            |
 *          |       |   [0]    |  |   [1]    |     |    [N]   |            |
 *          |       +----------+  +----------+     +----------+            |
 *          |            |             |                 |                 |
 *          |            v             v                 v                 |
 *          |       +----------+  +----------+     +----------+            |
 *          |       |  MUX     |  |  MUX     |     |  MUX     |            |
 *          |       |  Module  |  |  Module  | ... |  Module  |            |
 *          |       |   [0]    |  |   [1]    |     |    [N]   |            |
 *          |       +----------+  +----------+     +----------+            |
 *          |                                                              |
 *          +--------------------------------------------------------------+
 *
 * Memory bank-style
 * -----------------
 *    two ports will be added, which are regular output and inverted output 
 *    Note that the outputs are the data outputs of SRAMs 
 *    BL/WLs of memory decoders are ports of module but not local wires
 *
 *             Module
 *            +-------------------------------------------------+
 *            |                                                 |
  BL/WL bus --+--------+------------+-----------------+         |
 *            |         |            |                |         |
 *            |   BL/WL v      BL/WL v          BL/WL v         |
 *            |   +----------+  +----------+     +----------+   |
 *            |   |  Memory  |  |  Memory  |     |  Memory  |   |
 *            |   |  Module  |  |  Module  | ... |  Module  |   |
 *            |   |   [0]    |  |   [1]    |     |    [N]   |   |
 *            |   +----------+  +----------+     +----------+   |
 *            |        |             |                 |        |
 *            |        v             v                 v        |
 *            |   +----------+  +----------+     +----------+   |
 *            |   |  MUX     |  |  MUX     |     |  MUX     |   |
 *            |   |  Module  |  |  Module  | ... |  Module  |   |
 *            |   |   [0]    |  |   [1]    |     |    [N]   |   |
 *            |   +----------+  +----------+     +----------+   |
 *            |                                                 |
 *            +-------------------------------------------------+
 *
 ********************************************************************/
void print_verilog_local_sram_wires(std::fstream& fp,
                                    const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& sram_model,
                                    const e_config_protocol_type sram_orgz_type,
                                    const size_t& port_size) {
  /* Make sure we have a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Port size must be at least one! */
  if (0 == port_size) {
    return;
  }

  /* Depend on the configuraion style */
  switch(sram_orgz_type) {
  case CONFIG_MEM_STANDALONE:
    /* Nothing to do here */
    break;
  case CONFIG_MEM_SCAN_CHAIN: {
    /* Generate the name of local wire for the CCFF inputs, CCFF output and inverted output */
    /* [0] => CCFF input */
    BasicPort ccff_config_bus_port(generate_local_config_bus_port_name(), port_size);
    fp << generate_verilog_port(VERILOG_PORT_WIRE, ccff_config_bus_port) << ";" << std::endl; 
    /* Connect first CCFF to the head */
    /* Head is always a 1-bit port */
    BasicPort ccff_head_port(generate_sram_port_name(sram_orgz_type, CIRCUIT_MODEL_PORT_INPUT), 1); 
    BasicPort ccff_head_local_port(ccff_config_bus_port.get_name(), 1); 
    print_verilog_wire_connection(fp, ccff_head_local_port, ccff_head_port, false); 
    /* Connect last CCFF to the tail */
    /* Tail is always a 1-bit port */
    BasicPort ccff_tail_port(generate_sram_port_name(sram_orgz_type, CIRCUIT_MODEL_PORT_OUTPUT), 1); 
    BasicPort ccff_tail_local_port(ccff_config_bus_port.get_name(), ccff_config_bus_port.get_msb(), ccff_config_bus_port.get_msb()); 
    print_verilog_wire_connection(fp, ccff_tail_local_port, ccff_tail_port, false); 
    break;
  }
  case CONFIG_MEM_MEMORY_BANK: {
    /* Generate the name of local wire for the SRAM output and inverted output */
    std::vector<BasicPort> sram_ports;
    /* [0] => SRAM output */
    sram_ports.push_back(BasicPort(generate_sram_local_port_name(circuit_lib, sram_model, sram_orgz_type, CIRCUIT_MODEL_PORT_INPUT), port_size));
    /* [1] => SRAM inverted output */
    sram_ports.push_back(BasicPort(generate_sram_local_port_name(circuit_lib, sram_model, sram_orgz_type, CIRCUIT_MODEL_PORT_OUTPUT), port_size));
    /* Print local wire definition */
    for (const auto& sram_port : sram_ports) {
      fp << generate_verilog_port(VERILOG_PORT_WIRE, sram_port) << ";" << std::endl; 
    }

    break;
  }
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid SRAM organization!\n");
    exit(1);
  }
}

/*********************************************************************
 * Print a number of bus ports which are wired to the configuration
 * ports of a CMOS (SRAM-based) routing multiplexer
 * This port is supposed to be used locally inside a Verilog/SPICE module 
 *
 * The following shows a few representative examples:
 *
 * For standalone configuration style:
 * ------------------------------------
 * No bus needed
 *
 * Configuration chain-style
 * -------------------------
 * wire [0:N] config_bus
 *
 *            config_bus    config_bus    config_bus       config_bus
 *               [0]           [1]           [2]              [N]
 *                |             |             |                |
 *                v             v             v                v
 * ccff_head ----------+  +---------+   +------------+   +----> ccff_tail
 *                     |  ^         |   ^            |   ^
 *                head v  |tail     v   |            v   |
 *                 +----------+  +----------+     +----------+
 *                 |  Memory  |  |  Memory  |     |  Memory  |
 *                 |  Module  |  |  Module  | ... |  Module  |
 *                 |   [0]    |  |   [1]    |     |    [N]   |
 *                 +----------+  +----------+     +----------+
 *                      |             |                 |
 *                      v             v                 v
 *                 +----------+  +----------+     +----------+
 *                 |  MUX     |  |  MUX     |     |  MUX     |
 *                 |  Module  |  |  Module  | ... |  Module  |
 *                 |   [0]    |  |   [1]    |     |    [N]   |
 *                 +----------+  +----------+     +----------+
 *
 * Memory bank-style
 * -----------------
 *         BL/WL bus --+------------+-------------------->
 *                     |            |                |
 *               BL/WL v      BL/WL v          BL/WL v
 *               +----------+  +----------+     +----------+
 *               |  Memory  |  |  Memory  |     |  Memory  |
 *               |  Module  |  |  Module  | ... |  Module  |
 *               |   [0]    |  |   [1]    |     |    [N]   |
 *               +----------+  +----------+     +----------+
 *                    |             |                 |
 *                    v             v                 v
 *               +----------+  +----------+     +----------+
 *               |  MUX     |  |  MUX     |     |  MUX     |
 *               |  Module  |  |  Module  | ... |  Module  |
 *               |   [0]    |  |   [1]    |     |    [N]   |
 *               +----------+  +----------+     +----------+
 *     
 *********************************************************************/
void print_verilog_local_config_bus(std::fstream& fp, 
                                    const std::string& prefix,
                                    const e_config_protocol_type& sram_orgz_type,
                                    const size_t& instance_id,
                                    const size_t& num_conf_bits) { 
  /* Make sure we have a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

  switch(sram_orgz_type) {
  case CONFIG_MEM_STANDALONE:
    /* Not need for configuration bus
     * The configuration ports of SRAM are directly wired to the ports of modules
     */
    break;
  case CONFIG_MEM_SCAN_CHAIN: 
  case CONFIG_MEM_MEMORY_BANK: {
    /* Two configuration buses should be outputted
     * One for the regular SRAM ports of a routing multiplexer
     * The other for the inverted SRAM ports of a routing multiplexer
     */
    BasicPort config_port(generate_local_sram_port_name(prefix, instance_id, CIRCUIT_MODEL_PORT_INPUT), 
                          num_conf_bits);
    fp << generate_verilog_port(VERILOG_PORT_WIRE, config_port) << ";" << std::endl;
    BasicPort inverted_config_port(generate_local_sram_port_name(prefix, instance_id, CIRCUIT_MODEL_PORT_OUTPUT), 
                                   num_conf_bits); 
    fp << generate_verilog_port(VERILOG_PORT_WIRE, inverted_config_port) << ";" << std::endl;
    break;
  }
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid SRAM organization!\n");
    exit(1);
  }
}

/*********************************************************************
 * Print a number of bus ports which are wired to the configuration
 * ports of a ReRAM-based routing multiplexer
 * This port is supposed to be used locally inside a Verilog/SPICE module 
 *
 * Currently support: 
 * For memory-bank configuration style:
 * ------------------------------------
 * Different than CMOS routing multiplexers, ReRAM multiplexers require 
 * reserved BL/WLs to be grouped in buses
 *
 *            Module Port
 *                |
 *                v
 *  regular/reserved bus_port  --+----------------+---->     ...
 *                               |                |
 *          bl/wl/../sram_ports  v                v
 *                        +-----------+     +-----------+
 *                        |  Memory   |     |  Memory   |
 *                        | Module[0] |     | Module[1] |    ...
 *                        +-----------+     +-----------+
 *                               |                |
 *                               v                v
 *                        +-----------+     +-----------+
 *                        |  Routing  |     |  Routing  |
 *                        |   MUX [0] |     |   MUX[1]  |    ...
 *                        +-----------+     +-----------+
 * 
 *********************************************************************/
static 
void print_verilog_rram_mux_config_bus(std::fstream& fp, 
                                       const CircuitLibrary& circuit_lib,
                                       const CircuitModelId& mux_model,
                                       const e_config_protocol_type& sram_orgz_type,
                                       const size_t& mux_size,
                                       const size_t& mux_instance_id,
                                       const size_t& num_reserved_conf_bits, 
                                       const size_t& num_conf_bits) { 
  /* Make sure we have a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

  switch(sram_orgz_type) {
  case CONFIG_MEM_STANDALONE:
    /* Not need for configuration bus
     * The configuration ports of SRAM are directly wired to the ports of modules
     */
    break;
  case CONFIG_MEM_SCAN_CHAIN: {
    /* Not supported yet. 
     * Configuration chain may be only applied to ReRAM-based multiplexers with local decoders
     */
    break;
  }
  case CONFIG_MEM_MEMORY_BANK: {
    /* This is currently most used in ReRAM FPGAs */
    /* Print configuration bus to group reserved BL/WLs */
    BasicPort reserved_bl_bus(generate_reserved_sram_port_name(CIRCUIT_MODEL_PORT_BL), 
                              num_reserved_conf_bits);
    fp << generate_verilog_port(VERILOG_PORT_WIRE, reserved_bl_bus) << ";" << std::endl;
    BasicPort reserved_wl_bus(generate_reserved_sram_port_name(CIRCUIT_MODEL_PORT_WL), 
                              num_reserved_conf_bits);
    fp << generate_verilog_port(VERILOG_PORT_WIRE, reserved_wl_bus) << ";" << std::endl;

    /* Print configuration bus to group BL/WLs */
    BasicPort bl_bus(generate_mux_config_bus_port_name(circuit_lib, mux_model, mux_size, 0, false), 
                     num_conf_bits + num_reserved_conf_bits);
    fp << generate_verilog_port(VERILOG_PORT_WIRE, bl_bus) << ";" << std::endl;
    BasicPort wl_bus(generate_mux_config_bus_port_name(circuit_lib, mux_model, mux_size, 1, false), 
                     num_conf_bits + num_reserved_conf_bits);
    fp << generate_verilog_port(VERILOG_PORT_WIRE, wl_bus) << ";" << std::endl;

    /* Print bus to group SRAM outputs, this is to interface memory cells to routing multiplexers */
    BasicPort sram_output_bus(generate_mux_sram_port_name(circuit_lib, mux_model, mux_size, mux_instance_id, CIRCUIT_MODEL_PORT_INPUT), 
                          num_conf_bits);
    fp << generate_verilog_port(VERILOG_PORT_WIRE, sram_output_bus) << ";" << std::endl;
    BasicPort inverted_sram_output_bus(generate_mux_sram_port_name(circuit_lib, mux_model, mux_size, mux_instance_id, CIRCUIT_MODEL_PORT_OUTPUT), 
                                       num_conf_bits); 
    fp << generate_verilog_port(VERILOG_PORT_WIRE, inverted_sram_output_bus) << ";" << std::endl;

    /* Get the SRAM model of the mux_model */
    std::vector<CircuitModelId> sram_models = find_circuit_sram_models(circuit_lib, mux_model);
    /* TODO: maybe later multiplexers may have mode select ports... This should be relaxed */
    VTR_ASSERT( 1 == sram_models.size() );

    /* Wire the reserved configuration bits to part of bl/wl buses */
    BasicPort bl_bus_reserved_bits(bl_bus.get_name(), num_reserved_conf_bits);
    print_verilog_wire_connection(fp, bl_bus_reserved_bits, reserved_bl_bus, false);
    BasicPort wl_bus_reserved_bits(wl_bus.get_name(), num_reserved_conf_bits);
    print_verilog_wire_connection(fp, wl_bus_reserved_bits, reserved_wl_bus, false);

    /* Connect SRAM BL/WLs to bus */
    BasicPort mux_bl_wire(generate_sram_port_name(sram_orgz_type, CIRCUIT_MODEL_PORT_BL), 
                          num_conf_bits); 
    BasicPort bl_bus_regular_bits(bl_bus.get_name(), num_reserved_conf_bits, num_reserved_conf_bits + num_conf_bits - 1);
    print_verilog_wire_connection(fp, bl_bus_regular_bits, mux_bl_wire, false);
    BasicPort mux_wl_wire(generate_sram_port_name(sram_orgz_type, CIRCUIT_MODEL_PORT_WL), 
                          num_conf_bits); 
    BasicPort wl_bus_regular_bits(wl_bus.get_name(), num_reserved_conf_bits, num_reserved_conf_bits + num_conf_bits - 1);
    print_verilog_wire_connection(fp, wl_bus_regular_bits, mux_wl_wire, false);

    break;
  }
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid SRAM organization!\n");
    exit(1);
  }

}

/*********************************************************************
 * Print a number of bus ports which are wired to the configuration
 * ports of a memory module, which consists of a number of configuration
 * memory cells, such as SRAMs.
 * Note that the configuration bus will only interface the memory
 * module, rather than the programming routing multiplexers, LUTs, IOs
 * etc. This helps us to keep clean and simple Verilog generation
 *********************************************************************/
void print_verilog_mux_config_bus(std::fstream& fp, 
                                  const CircuitLibrary& circuit_lib,
                                  const CircuitModelId& mux_model,
                                  const e_config_protocol_type& sram_orgz_type,
                                  const size_t& mux_size,
                                  const size_t& mux_instance_id,
                                  const size_t& num_reserved_conf_bits, 
                                  const size_t& num_conf_bits) { 
  /* Depend on the design technology of this MUX:
   * bus connections are different
   * SRAM MUX: bus is connected to the output ports of SRAM
   * RRAM MUX: bus is connected to the BL/WL of MUX
   * TODO: Maybe things will become even more complicated, 
   * the bus connections may depend on the type of configuration circuit...
   * Currently, this is fine.
   */
  switch (circuit_lib.design_tech_type(mux_model)) {
  case CIRCUIT_MODEL_DESIGN_CMOS: {
    std::string prefix = generate_mux_subckt_name(circuit_lib, mux_model, mux_size, std::string());
    print_verilog_local_config_bus(fp, prefix, sram_orgz_type, mux_instance_id, num_conf_bits);
    break;
  }
  case CIRCUIT_MODEL_DESIGN_RRAM:
    print_verilog_rram_mux_config_bus(fp, circuit_lib, mux_model, sram_orgz_type, mux_size, mux_instance_id, num_reserved_conf_bits, num_conf_bits);
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid design technology for routing multiplexer!\n");
    exit(1);
  }
}

/*********************************************************************
 * Print a wire to connect MUX configuration ports 
 * This function connects the sram ports to the ports of a Verilog module
 * used for formal verification
 *
 * Note: MSB and LSB of formal verification configuration bus MUST be updated 
 * before running this function !!!!
 *********************************************************************/
void print_verilog_formal_verification_mux_sram_ports_wiring(std::fstream& fp, 
                                                             const CircuitLibrary& circuit_lib,
                                                             const CircuitModelId& mux_model,
                                                             const size_t& mux_size,
                                                             const size_t& mux_instance_id,
                                                             const size_t& num_conf_bits, 
                                                             const BasicPort& fm_config_bus) { 
  BasicPort mux_sram_output(generate_mux_sram_port_name(circuit_lib, mux_model, mux_size, mux_instance_id, CIRCUIT_MODEL_PORT_INPUT),
                            num_conf_bits);
  /* Get the SRAM model of the mux_model */
  std::vector<CircuitModelId> sram_models = find_circuit_sram_models(circuit_lib, mux_model);
  /* TODO: maybe later multiplexers may have mode select ports... This should be relaxed */
  VTR_ASSERT( 1 == sram_models.size() );
  BasicPort formal_verification_port;
  formal_verification_port.set_name(generate_formal_verification_sram_port_name(circuit_lib, sram_models[0]));
  VTR_ASSERT(num_conf_bits == fm_config_bus.get_width());
  formal_verification_port.set_lsb(fm_config_bus.get_lsb());
  formal_verification_port.set_msb(fm_config_bus.get_msb());
  print_verilog_wire_connection(fp, mux_sram_output, formal_verification_port, false);
}

/********************************************************************
 * Print stimuli for a pulse generation
 *
 *                |<--- pulse width --->|
 *                                      +------ flip_value
 *                                      |      
 *  initial_value ----------------------+       
 *
 *******************************************************************/
void print_verilog_pulse_stimuli(std::fstream& fp, 
                                 const BasicPort& port,
                                 const size_t& initial_value,
                                 const float& pulse_width,
                                 const size_t& flip_value) {
  /* Validate the file stream */
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Config_done signal: indicate when configuration is finished */
  fp << "initial" << std::endl;
  fp << "\tbegin" << std::endl;
  fp << "\t";
  std::vector<size_t> initial_values(port.get_width(), initial_value);
  fp << "\t";
  fp << generate_verilog_port_constant_values(port, initial_values);
  fp << ";" << std::endl;
  
  /* if flip_value is the same as initial value, we do not need to flip the signal ! */
  if (flip_value != initial_value) {
    fp << "\t" << "#" << std::setprecision(10) << pulse_width;
    std::vector<size_t> port_flip_values(port.get_width(), flip_value);
    fp << "\t";
    fp << generate_verilog_port_constant_values(port, port_flip_values);
    fp << ";" << std::endl;
  }

  fp << "\tend" << std::endl;

  /* Print an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Print stimuli for a pulse generation
 * This function supports multiple signal switching under different pulse width
 *
 *                 |<-- wait condition -->|
 *                                        |<--- pulse width --->|
 *                                                              +------ flip_values
 *                                                              |      
 *  initial_value -------  ...  --------------------------------+       
 *
 *******************************************************************/
void print_verilog_pulse_stimuli(std::fstream& fp, 
                                 const BasicPort& port,
                                 const size_t& initial_value,
                                 const std::vector<float>& pulse_widths,
                                 const std::vector<size_t>& flip_values,
                                 const std::string& wait_condition) {
  /* Validate the file stream */
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Config_done signal: indicate when configuration is finished */
  fp << "initial" << std::endl;
  fp << "\tbegin" << std::endl;
  fp << "\t";
  std::vector<size_t> initial_values(port.get_width(), initial_value);
  fp << "\t";
  fp << generate_verilog_port_constant_values(port, initial_values);
  fp << ";" << std::endl;

  /* Set a wait condition if specified */
  if (false == wait_condition.empty()) {
    fp << "\twait(" << wait_condition << ")" << std::endl;
  }
  
  /* Number of flip conditions and values should match */
  VTR_ASSERT(flip_values.size() == pulse_widths.size());
  for (size_t ipulse = 0; ipulse < pulse_widths.size(); ++ipulse) {
    fp << "\t" << "#" << std::setprecision(10) << pulse_widths[ipulse];
    std::vector<size_t> port_flip_value(port.get_width(), flip_values[ipulse]);
    fp << "\t";
    fp << generate_verilog_port_constant_values(port, port_flip_value);
    fp << ";" << std::endl;
  }

  fp << "\tend" << std::endl;

  /* Print an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Print stimuli for a clock signal
 * This function can support if the clock signal should wait for a period
 * of time and then start 
 *                          pulse width
 *                           |<----->|
 *                           +-------+       +-------+
 *                           |       |       |       |
 *  initial_value --- ... ---+       +-------+       +------ ...
 *           |<--wait_condition-->|
 *
 *******************************************************************/
void print_verilog_clock_stimuli(std::fstream& fp, 
                                 const BasicPort& port,
                                 const size_t& initial_value,
                                 const float& pulse_width,
                                 const std::string& wait_condition) {
  /* Validate the file stream */
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Config_done signal: indicate when configuration is finished */
  fp << "initial" << std::endl;
  fp << "\tbegin" << std::endl;

  std::vector<size_t> initial_values(port.get_width(), initial_value);
  fp << "\t\t";
  fp << generate_verilog_port_constant_values(port, initial_values);
  fp << ";" << std::endl;

  fp << "\tend" << std::endl;
  fp << "always";

  /* Set a wait condition if specified */
  if (true == wait_condition.empty()) {
    fp << std::endl;
  } else {
    fp << " wait(" << wait_condition << ")" << std::endl;
  }

  fp << "\tbegin" << std::endl;
  fp << "\t\t" << "#" << std::setprecision(10) << pulse_width;

  fp << "\t";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, port);
  fp << " = ";
  fp << "~";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, port);
  fp << ";" << std::endl;

  fp << "\tend" << std::endl;

  /* Print an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Output a header file that includes a number of Verilog netlists
 * so that it can be easily included in a top-level netlist
 ********************************************************************/
void print_verilog_netlist_include_header_file(const std::vector<std::string>& netlists_to_be_included,
                                               const char* subckt_dir,
                                               const char* header_file_name) {

  std::string verilog_fname(std::string(subckt_dir) + std::string(header_file_name));

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  VTR_ASSERT(true == valid_file_stream(fp));

  /* Generate the descriptions*/
  print_verilog_file_header(fp, "Header file to include other Verilog netlists"); 

  /* Output file names */
  for (const std::string& netlist_name : netlists_to_be_included) {
    fp << "`include \"" << netlist_name << "\"" << std::endl;
  }

  /* close file stream */
  fp.close();
}

} /* end namespace openfpga */
