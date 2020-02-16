/********************************************************************
 * This file include top-level function of FPGA-Verilog
 ********************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

#include "circuit_library_utils.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "device_rr_gsb.h"
#include "verilog_constants.h"
#include "verilog_auxiliary_netlists.h"
#include "verilog_submodule.h"
#include "verilog_routing.h"
//#include "verilog_grid.h"
//#include "verilog_top_module.h"

/* Header file for this source file */
#include "verilog_api.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Top-level function of FPGA-Verilog
 * This function will generate
 * 1. primitive modules required by the full fabric
 *    which are LUTs, routing multiplexer, logic gates, transmission-gates etc.
 * 2. Routing modules, which are Switch Blocks (SBs) and Connection Blocks (CBs)
 * 3. Logic block modules, which are Configuration Logic Blocks (CLBs)
 * 4. FPGA module, which are the full FPGA fabric with configuration protocol
 * 5. A wrapper module, which encapsulate the FPGA module in a Verilog module which have the same port as the input benchmark 
 * 6. Testbench, where a FPGA module is configured with a bitstream and then driven by input vectors
 * 7. Pre-configured testbench, which can skip the configuration phase and pre-configure the FPGA module. This testbench is created for quick verification and formal verification purpose.
 * 8. Verilog netlist including preprocessing flags and all the Verilog netlists that have been generated
 *
 * TODO: We should use module manager as a constant here. 
 * All the modification should be done before this writer!
 * The only exception now is the user-defined modules.
 * We should think clearly about how to handle them for both Verilog and SPICE generators!
 ********************************************************************/
void fpga_fabric_verilog(ModuleManager& module_manager,
                         const CircuitLibrary& circuit_lib,
                         const MuxLibrary& mux_lib,
                         const DeviceGrid& grids, 
                         const DeviceRRGSB& device_rr_gsb,
                         const FabricVerilogOption& options) {

  vtr::ScopedStartFinishTimer timer("Write Verilog netlists for FPGA fabric\n");

  std::string src_dir_path = format_dir_path(options.output_directory());

  /* Create directories */
  create_dir_path(src_dir_path.c_str());

  /* Sub directory under SRC directory to contain all the primitive block netlists */
  std::string submodule_dir_path = src_dir_path + std::string(DEFAULT_SUBMODULE_DIR_NAME);
  create_dir_path(submodule_dir_path.c_str());

  /* Sub directory under SRC directory to contain all the logic block netlists */
  std::string lb_dir_path = src_dir_path + std::string(DEFAULT_LB_DIR_NAME);
  create_dir_path(lb_dir_path.c_str());

  /* Sub directory under SRC directory to contain all the routing block netlists */
  std::string rr_dir_path = src_dir_path + std::string(DEFAULT_RR_DIR_NAME);
  create_dir_path(rr_dir_path.c_str());

  /* Print Verilog files containing preprocessing flags */
  print_verilog_preprocessing_flags_netlist(std::string(src_dir_path),
                                            options);

  print_verilog_simulation_preprocessing_flags(std::string(src_dir_path),
                                               options);

  /* Generate primitive Verilog modules, which are corner stones of FPGA fabric
   * Note that this function MUST be called before Verilog generation of
   * core logic (i.e., logic blocks and routing resources) !!!
   * This is because that this function will add the primitive Verilog modules to 
   * the module manager.
   * Without the modules in the module manager, core logic generation is not possible!!!
   */
  print_verilog_submodule(module_manager, mux_lib, circuit_lib,
                          src_dir_path, submodule_dir_path, 
                          options);

  /* Generate routing blocks */
  if (true == options.compress_routing()) {
    print_verilog_unique_routing_modules(const_cast<const ModuleManager&>(module_manager),
                                         device_rr_gsb,  
                                         src_dir_path, rr_dir_path,
                                         options.explicit_port_mapping());
  } else {
    VTR_ASSERT(false == options.compress_routing());
    print_verilog_flatten_routing_modules(const_cast<const ModuleManager&>(module_manager),
                                          device_rr_gsb, 
                                          src_dir_path, rr_dir_path,
                                          options.explicit_port_mapping());
  }

  /* Generate grids */
  //print_verilog_grids(module_manager, 
  //                    src_dir_path, lb_dir_path,
  //                    dump_explicit_verilog);

  /* Generate FPGA fabric */
  //print_verilog_top_module(module_manager, 
  //                         std::string(vpr_setup.FileNameOpts.ArchFile), 
  //                         src_dir_path,
  //                         dump_explicit_verilog);

  /* Given a brief stats on how many Verilog modules have been written to files */
  VTR_LOGV(options.verbose_output(), 
           "Outputted %lu Verilog modules in total\n", 
           module_manager.num_modules());  
}

} /* end namespace openfpga */
