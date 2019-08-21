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
  fp << "//\t Organization: University of Utah" << std::endl;
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
  fp << "//------ End Include defines: preproc flags -----" << std::endl;

  return;
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
    verilog_line = port_info.get_name() + " " + size_str;
  } else { 
    verilog_line = VERILOG_PORT_TYPE_STRING[verilog_port_type]; 
    verilog_line += "" + size_str + " " + port_info.get_name();
  }

  return verilog_line;
}


