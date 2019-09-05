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
 ***********************************************/
void print_verilog_module_definition(std::fstream& fp, 
                                     const std::string& module_name) {
  check_file_handler(fp);

  print_verilog_comment(fp, std::string("----- Verilog module for " + module_name + " -----"));
  fp << "module " << module_name << "(" << std::endl;
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
  size_t port_cnt = 0;
  for (const auto& kv : port_type2type_map) {
    for (const auto& port : module_manager.module_ports_by_type(module_id, kv.first)) {
      if (0 != port_cnt) {
        /* Do not dump a comma for the first port */
        fp << "," << std::endl; 
      }
      /* Print port */
      fp << "\t//----- " << module_manager.module_port_type_str(kv.first)  << " -----" << std::endl; 
      fp << "\t" << generate_verilog_port(kv.second, port);
      port_cnt++;
    }
  }
}

/************************************************
 * Print a Verilog module declaration (definition + port list
 ***********************************************/
void print_verilog_module_declaration(std::fstream& fp, 
                                      const ModuleManager& module_manager, const ModuleId& module_id) {
  check_file_handler(fp);

  print_verilog_module_definition(fp, module_manager.module_name(module_id));

  print_verilog_module_ports(fp, module_manager, module_id);

  fp << std::endl << ");" << std::endl;
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
  fp << "\t" << ");" << std::endl;
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
