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

  print_verilog_comment(fp, std::string("//----- Verilog module for " + module_name + " -----"));
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
 * Print an end line for a Verilog module
 ***********************************************/
void print_verilog_module_end(std::fstream& fp, 
                              const std::string& module_name) {
  check_file_handler(fp);

  fp << "endmodule" << std::endl;
  print_verilog_comment(fp, std::string("//----- END Verilog module for " + module_name + " -----"));
  fp << std::endl;
}

/* Generate a string of a Verilog port */
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


