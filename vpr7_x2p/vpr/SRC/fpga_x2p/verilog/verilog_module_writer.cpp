/********************************************************************
 * This file includes functions to write a Verilog module
 * based on its definition in Module Manager
 *
 * Note that Verilog writer functions are just an outputter for the
 * module definition. 
 * You should NOT modify any content of the module manager
 * Please use const keyword to restrict this!
 *******************************************************************/
#include "vtr_assert.h"
#include "fpga_x2p_utils.h"
#include "verilog_writer_utils.h"
#include "verilog_module_writer.h"

/********************************************************************
 * Collect all the nets that are going to be local wires
 * And organize it in a vector of ports
 * Verilog wire writter function will use the output of this function
 * to write up local wire declaration in Verilog format
 *
 * Here is the strategy to identify local wires: 
 *   Iterate over the nets inside the module. 
 *   A net is a local wire if it connects between two instances,
 *   This is valid when all the sinks of the net are pins/ports of instances  
 *******************************************************************/
static 
std::vector<BasicPort> find_verilog_module_local_wires(const ModuleManager& module_manager,
                                                       const ModuleId& module_id) {
  std::vector<BasicPort> local_wire_ports;
  
  /* Find local wires between instances */
  for (ModuleNetId module_net : module_manager.module_nets(module_id)) {
    /* A flag to identify local wire */
    bool is_local_wire = false;

    /* Each net must only one 1 source */ 
    VTR_ASSERT(1 == module_manager.net_source_modules(module_id, module_net).size());

    /* Check if source module is the module itself */
    if (module_id != module_manager.net_source_modules(module_id, module_net)[0]) {
      is_local_wire = true;
    }

    /* Check all the sink modules of the net */
    for (ModuleId sink_module : module_manager.net_sink_modules(module_id, module_net)) {
      if (module_id == sink_module) {
        is_local_wire = false;
      }
    }

    /* TODO: Push the Verilog port the list, try to combine the ports if we can */
  }

  return local_wire_ports;
}

/********************************************************************
 * Write a Verilog module to a file
 * This is a key function, maybe most frequently called in our Verilog writer
 * Note that file stream must be valid 
 *******************************************************************/
void writer_verilog_module_to_file(std::fstream& fp,
                                   const ModuleManager& module_manager,
                                   const ModuleId& module_id) {
  /* Ensure a valid file stream */
  check_file_handler(fp);

  /* Ensure we have a valid module_id */
  VTR_ASSERT(module_manager.valid_module_id(module_id)); 

  /* Print module declaration */
  print_verilog_module_declaration(fp, module_manager, module_id);
   
  /* TODO: Print internal wires */

  /* TODO: Print instances */
  for (ModuleId child_module : module_manager.child_modules(module_id)) {
    for (size_t instance : module_manager.child_module_instances(module_id, child_module)) {
      /* Print an instance */
    }
  }

  /* Print an end for the module */
  print_verilog_module_end(fp, module_manager.module_name(module_id)); 
}
