/********************************************************************
 * This file includes most utilized functions that are used to create
 * Verilog testbenches
 *
 * Note: please do NOT use global variables in this file
 * so that we can make it free to use anywhere
 *******************************************************************/
#include "vtr_assert.h"
#include "device_port.h"

#include "fpga_x2p_utils.h"
#include "fpga_x2p_benchmark_utils.h"

#include "verilog_writer_utils.h"
#include "verilog_testbench_utils.h"

/********************************************************************
 * Print an instance of the FPGA top-level module
 *******************************************************************/
void print_verilog_testbench_fpga_instance(std::fstream& fp,
                                           const ModuleManager& module_manager,
                                           const ModuleId& top_module,
                                           const std::string& top_instance_name) {
  /* Validate the file stream */
  check_file_handler(fp);

  /* Include defined top-level module */
  print_verilog_comment(fp, std::string("----- FPGA top-level module to be capsulated -----"));

  /* Create an empty port-to-port name mapping, because we use default names */
  std::map<std::string, BasicPort> port2port_name_map;

  /* Use explicit port mapping for a clean instanciation */
  print_verilog_module_instance(fp, module_manager, top_module, 
                                top_instance_name, 
                                port2port_name_map, true); 

  /* Add an empty line as a splitter */
  fp << std::endl;
}

/********************************************************************
 * Instanciate the input benchmark module
 *******************************************************************/
void print_verilog_testbench_benchmark_instance(std::fstream& fp,
                                                const std::string& module_name,
                                                const std::string& instance_name,
                                                const std::string& module_input_port_postfix,
                                                const std::string& module_output_port_postfix,
                                                const std::string& output_port_postfix,
                                                const std::vector<t_logical_block>& L_logical_blocks,
                                                const bool& use_explicit_port_map) {
  /* Validate the file stream */
  check_file_handler(fp);

  fp << "\t" << module_name << " " << instance_name << "(" << std::endl;

  size_t port_counter = 0;
  for (const t_logical_block& lb : L_logical_blocks) {
    /* Bypass non-I/O logical blocks ! */
    if ( (VPACK_INPAD != lb.type) && (VPACK_OUTPAD != lb.type) ) {
      continue;
    }
    /* The first port does not need a comma */
    if(0 < port_counter){
      fp << "," << std::endl;
    }
    /* Input port follows the logical block name while output port requires a special postfix */
    if (VPACK_INPAD == lb.type){
      fp << "\t\t";
      if (true == use_explicit_port_map) {
        fp << "." << std::string(lb.name) << module_input_port_postfix << "(";
      }
      fp << std::string(lb.name);
      if (true == use_explicit_port_map) {
        fp << ")";
      }
    } else {
      VTR_ASSERT_SAFE(VPACK_OUTPAD == lb.type);
      fp << "\t\t";
      if (true == use_explicit_port_map) {
        fp << "." << std::string(lb.name) << module_output_port_postfix << "(";
      }
      fp << std::string(lb.name) << output_port_postfix;
      if (true == use_explicit_port_map) {
        fp << ")";
      }
    }
    /* Update the counter */
    port_counter++;
  }
  fp << "\t);" << std::endl;
}

/********************************************************************
 * This function adds stimuli to I/Os of FPGA fabric
 * 1. For mapped I/Os, this function will wire them to the input ports
 *    of the pre-configured FPGA top module
 * 2. For unmapped I/Os, this function will assign a constant value
 *    by default 
 *******************************************************************/
void print_verilog_testbench_connect_fpga_ios(std::fstream& fp,
                                              const ModuleManager& module_manager,
                                              const ModuleId& top_module,
                                              const std::vector<t_logical_block>& L_logical_blocks,
                                              const vtr::Point<size_t>& device_size,
                                              const std::vector<std::vector<t_grid_tile>>& L_grids, 
                                              const std::vector<t_block>& L_blocks,
                                              const std::string& io_port_name_postfix,
                                              const size_t& unused_io_value) {
  /* Validate the file stream */
  check_file_handler(fp);

  /* In this function, we support only 1 type of I/Os */
  VTR_ASSERT(1 == module_manager.module_ports_by_type(top_module, ModuleManager::MODULE_GPIO_PORT).size());
  BasicPort module_io_port = module_manager.module_ports_by_type(top_module, ModuleManager::MODULE_GPIO_PORT)[0];

  /* Keep tracking which I/Os have been used */
  std::vector<bool> io_used(module_io_port.get_width(), false);

  /* See if this I/O should be wired to a benchmark input/output */
  /* Add signals from blif benchmark and short-wire them to FPGA I/O PADs
   * This brings convenience to checking functionality  
   */
  print_verilog_comment(fp, std::string("----- Link BLIF Benchmark I/Os to FPGA I/Os -----"));
  for (const t_logical_block& io_lb : L_logical_blocks) {
    /* We only care I/O logical blocks !*/
    if ( (VPACK_INPAD != io_lb.type) && (VPACK_OUTPAD != io_lb.type) ) {
      continue;
    }

    /* Find the index of the mapped GPIO in top-level FPGA fabric */
    size_t io_index = find_benchmark_io_index(io_lb, device_size, L_grids, L_blocks);

    /* Ensure that IO index is in range */
    BasicPort module_mapped_io_port = module_io_port; 
    /* Set the port pin index */ 
    VTR_ASSERT(io_index < module_mapped_io_port.get_width());
    module_mapped_io_port.set_width(io_index, io_index);

    /* Create the port for benchmark I/O, due to BLIF benchmark, each I/O always has a size of 1 */
    BasicPort benchmark_io_port(std::string(std::string(io_lb.name)+ io_port_name_postfix), 1); 

    print_verilog_comment(fp, std::string("----- Blif Benchmark inout " + std::string(io_lb.name) + " is mapped to FPGA IOPAD " + module_mapped_io_port.get_name() + "[" + std::to_string(io_index) + "] -----"));
    if (VPACK_INPAD == io_lb.type) {
      print_verilog_wire_connection(fp, module_mapped_io_port, benchmark_io_port, false);
    } else {
      VTR_ASSERT(VPACK_OUTPAD == io_lb.type);
      print_verilog_wire_connection(fp, benchmark_io_port, module_mapped_io_port, false);
    }

    /* Mark this I/O has been used/wired */
    io_used[io_index] = true;
  }

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Wire the unused iopads to a constant */
  print_verilog_comment(fp, std::string("----- Wire unused FPGA I/Os to constants -----"));
  for (size_t io_index = 0; io_index < io_used.size(); ++io_index) {
    /* Bypass used iopads */
    if (true == io_used[io_index]) {
      continue;
    }

    /* Wire to a contant */
    BasicPort module_unused_io_port = module_io_port; 
    /* Set the port pin index */ 
    module_unused_io_port.set_width(io_index, io_index);

    std::vector<size_t> default_values(module_unused_io_port.get_width(), unused_io_value);
    print_verilog_wire_constant_values(fp, module_unused_io_port, default_values);
  }

  /* Add an empty line as a splitter */
  fp << std::endl;
}
