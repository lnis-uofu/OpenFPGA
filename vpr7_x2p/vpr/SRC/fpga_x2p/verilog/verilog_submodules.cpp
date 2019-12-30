/*********************************************************************
 * This file includes top-level function to generate Verilog primitive modules
 * and print them to files
 ********************************************************************/
/* Standard header files */

/* External library header files */
#include "util.h"

/* FPGA-Verilog header files */
#include "verilog_submodule_utils.h"
#include "verilog_essential_gates.h"
#include "verilog_decoders.h"
#include "verilog_mux.h"
#include "verilog_lut.h"
#include "verilog_wire.h"
#include "verilog_memory.h"
#include "verilog_writer_utils.h"

/* Header file for this source file */
#include "verilog_submodules.h"

/*********************************************************************
 * Top-level function to generate primitive modules:
 * 1. Logic gates: AND/OR, inverter, buffer and transmission-gate/pass-transistor
 * 2. Routing multiplexers
 * 3. Local encoders for routing multiplexers
 * 4. Wires
 * 5. Configuration memory blocks
 * 6. Verilog template
 ********************************************************************/
void print_verilog_submodules(ModuleManager& module_manager, 
                              const MuxLibrary& mux_lib,
                              t_sram_orgz_info* cur_sram_orgz_info,
                              const char* verilog_dir, 
                              const char* submodule_dir, 
                              const t_arch& Arch, 
                              const t_syn_verilog_opts& fpga_verilog_opts) {

  /* Register all the user-defined modules in the module manager
   * This should be done prior to other steps in this function, 
   * because they will be instanciated by other primitive modules 
   */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Registering user-defined modules...\n");

  /* Create a vector to contain all the Verilog netlist names that have been generated in this function */
  std::vector<std::string> netlist_names;

  add_user_defined_verilog_modules(module_manager, Arch.spice->circuit_lib);

  print_verilog_submodule_essentials(module_manager, 
                                     netlist_names,
                                     std::string(verilog_dir), 
                                     std::string(submodule_dir),
                                     Arch.spice->circuit_lib);

  /* Routing multiplexers */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating modules for routing multiplexers...\n");

  /* NOTE: local decoders generation must go before the MUX generation!!! 
   *       because local decoders modules will be instanciated in the MUX modules 
   */
  print_verilog_submodule_mux_local_decoders(module_manager, netlist_names, 
                                             mux_lib, Arch.spice->circuit_lib, 
                                             std::string(verilog_dir), std::string(submodule_dir));
  print_verilog_submodule_muxes(module_manager, netlist_names, mux_lib, Arch.spice->circuit_lib, cur_sram_orgz_info, 
                                std::string(verilog_dir), std::string(submodule_dir),
                                fpga_verilog_opts.dump_explicit_verilog);

 
  /* LUTes */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating modules for LUTs...\n");
  print_verilog_submodule_luts(module_manager, netlist_names, Arch.spice->circuit_lib, std::string(verilog_dir), std::string(submodule_dir),
                               fpga_verilog_opts.dump_explicit_verilog);

  /* Hard wires */
  print_verilog_submodule_wires(module_manager, netlist_names, Arch.spice->circuit_lib, std::string(verilog_dir), std::string(submodule_dir));

  /* 4. Memories */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating modules for configuration memory blocks...\n");
  print_verilog_submodule_memories(module_manager, netlist_names,
                                   mux_lib, Arch.spice->circuit_lib, 
                                   std::string(verilog_dir), std::string(submodule_dir),
                                   fpga_verilog_opts.dump_explicit_verilog);

  /* 5. Dump template for all the modules */
  if (TRUE == fpga_verilog_opts.print_user_defined_template) { 
    print_verilog_submodule_templates(module_manager, Arch.spice->circuit_lib, std::string(verilog_dir), std::string(submodule_dir));
  }

  /* Create a header file to include all the subckts */
  vpr_printf(TIO_MESSAGE_INFO,
             "Generating header file for primitive modules...\n");
  print_verilog_netlist_include_header_file(netlist_names,
                                            submodule_dir,
                                            submodule_verilog_file_name);
}

