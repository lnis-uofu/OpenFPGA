/************************************************
 * Include functions for most frequently
 * used Verilog writers 
 ***********************************************/
#include "vtr_assert.h"

/* Device-level header files */

/* FPGA-X2P context header files */
#include "spice_types.h"

/* FPGA-Verilog context header files */
#include "verilog_global.h"
#include "verilog_writer_utils.h"

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


