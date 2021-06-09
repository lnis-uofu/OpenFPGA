/********************************************************************
 * This file include top-level function of FPGA-Verilog
 ********************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

#include "command_exit_codes.h"

#include "circuit_library_utils.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
#include "openfpga_reserved_words.h"

#include "device_rr_gsb.h"
#include "verilog_constants.h"
#include "verilog_auxiliary_netlists.h"
#include "verilog_submodule.h"
#include "verilog_routing.h"
#include "verilog_grid.h"
#include "verilog_top_module.h"

#include "verilog_preconfig_top_module.h"
#include "verilog_formal_random_top_testbench.h"
#include "verilog_top_testbench.h"
#include "verilog_simulation_info_writer.h"

/* Header file for this source file */
#include "verilog_api.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A top-level function of FPGA-Verilog which focuses on fabric Verilog generation
 * This function will generate
 *  - primitive modules required by the full fabric
 *  - which are LUTs, routing multiplexer, logic gates, transmission-gates etc.
 *  - Routing modules, which are Switch Blocks (SBs) and Connection Blocks (CBs)
 *  - Logic block modules, which are Configuration Logic Blocks (CLBs)
 *  - FPGA module, which are the full FPGA fabric with configuration protocol
 *
 * Note:
 *  - Please do NOT include ANY testbench generation in this function!!!
 *    It is about the fabric itself, independent from any implementation
 *    All the testbench generation should be in the function fpga_testbench_verilog()
 *
 * TODO: We should use module manager as a constant here.
 * All the modification should be done before this writer!
 * The only exception now is the user-defined modules.
 * We should think clearly about how to handle them for both Verilog and SPICE generators!
 ********************************************************************/
void fpga_fabric_verilog(ModuleManager &module_manager,
                         NetlistManager &netlist_manager,
                         const CircuitLibrary &circuit_lib,
                         const MuxLibrary &mux_lib,
                         const DecoderLibrary &decoder_lib,
                         const DeviceContext &device_ctx,
                         const VprDeviceAnnotation &device_annotation,
                         const DeviceRRGSB &device_rr_gsb,
                         const FabricVerilogOption &options) {

  vtr::ScopedStartFinishTimer timer("Write Verilog netlists for FPGA fabric\n");

  std::string src_dir_path = format_dir_path(options.output_directory());

  /* Create directories */
  create_directory(src_dir_path);

  /* Sub directory under SRC directory to contain all the primitive block netlists */
  std::string submodule_dir_path = src_dir_path + std::string(DEFAULT_SUBMODULE_DIR_NAME);
  create_directory(submodule_dir_path);

  /* Sub directory under SRC directory to contain all the logic block netlists */
  std::string lb_dir_path = src_dir_path + std::string(DEFAULT_LB_DIR_NAME);
  create_directory(lb_dir_path);

  /* Sub directory under SRC directory to contain all the routing block netlists */
  std::string rr_dir_path = src_dir_path + std::string(DEFAULT_RR_DIR_NAME);
  create_directory(rr_dir_path);

  /* Print Verilog files containing preprocessing flags */
  print_verilog_preprocessing_flags_netlist(std::string(src_dir_path),
                                            options);

  /* Generate primitive Verilog modules, which are corner stones of FPGA fabric
   * Note that this function MUST be called before Verilog generation of
   * core logic (i.e., logic blocks and routing resources) !!!
   * This is because that this function will add the primitive Verilog modules to
   * the module manager.
   * Without the modules in the module manager, core logic generation is not possible!!!
   */
  print_verilog_submodule(module_manager, netlist_manager,
                          mux_lib, decoder_lib, circuit_lib,
                          submodule_dir_path,
                          options);

  /* Generate routing blocks */
  if (true == options.compress_routing()) {
    print_verilog_unique_routing_modules(netlist_manager,
                                         const_cast<const ModuleManager &>(module_manager),
                                         device_rr_gsb,
                                         rr_dir_path,
                                         options);
  } else {
    VTR_ASSERT(false == options.compress_routing());
    print_verilog_flatten_routing_modules(netlist_manager,
                                          const_cast<const ModuleManager &>(module_manager),
                                          device_rr_gsb,
                                          rr_dir_path,
                                          options);
  }

  /* Generate grids */
  print_verilog_grids(netlist_manager,
                      const_cast<const ModuleManager &>(module_manager),
                      device_ctx, device_annotation,
                      lb_dir_path,
                      options,
                      options.verbose_output());

  /* Generate FPGA fabric */
  print_verilog_top_module(netlist_manager,
                           const_cast<const ModuleManager &>(module_manager),
                           src_dir_path,
                           options);

  /* Generate an netlist including all the fabric-related netlists */
  print_verilog_fabric_include_netlist(const_cast<const NetlistManager &>(netlist_manager),
                                       src_dir_path,
                                       circuit_lib);

  /* Given a brief stats on how many Verilog modules have been written to files */
  VTR_LOGV(options.verbose_output(),
           "Written %lu Verilog modules in total\n",
           module_manager.num_modules());
}

/********************************************************************
 * A top-level function of FPGA-Verilog which focuses on fabric Verilog generation
 * This function will generate
 *  - A wrapper module, which encapsulate the FPGA module in a Verilog module which have the same port as the input benchmark
 *  - Testbench, where a FPGA module is configured with a bitstream and then driven by input vectors
 *  - Pre-configured testbench, which can skip the configuration phase and pre-configure the FPGA module.
 *    This testbench is created for quick verification and formal verification purpose.
 *  - Verilog netlist including preprocessing flags and all the Verilog netlists that have been generated
 ********************************************************************/
int fpga_verilog_testbench(const ModuleManager &module_manager,
                           const BitstreamManager &bitstream_manager,
                           const FabricBitstream &fabric_bitstream,
                           const AtomContext &atom_ctx,
                           const PlacementContext &place_ctx,
                           const PinConstraints& pin_constraints,
                           const IoLocationMap &io_location_map,
                           const FabricGlobalPortInfo &fabric_global_port_info,
                           const VprNetlistAnnotation &netlist_annotation,
                           const CircuitLibrary &circuit_lib,
                           const SimulationSetting &simulation_setting,
                           const ConfigProtocol &config_protocol,
                           const VerilogTestbenchOption &options) {

  vtr::ScopedStartFinishTimer timer("Write Verilog testbenches for FPGA fabric\n");

  std::string src_dir_path = format_dir_path(options.output_directory());

  std::string netlist_name = atom_ctx.nlist.netlist_name();

  int status = CMD_EXEC_SUCCESS;

  /* Create directories */
  create_directory(src_dir_path);

  /* Output preprocessing flags for HDL simulations */
  print_verilog_simulation_preprocessing_flags(std::string(src_dir_path),
                                               options);

  /* Generate wrapper module for FPGA fabric (mapped by the input benchmark and pre-configured testbench for verification */
  if (true == options.print_formal_verification_top_netlist()) {
    std::string formal_verification_top_netlist_file_path = src_dir_path + netlist_name + std::string(FORMAL_VERIFICATION_VERILOG_FILE_POSTFIX);
    status = print_verilog_preconfig_top_module(module_manager, bitstream_manager,
                                                config_protocol,
                                                circuit_lib, fabric_global_port_info,
                                                atom_ctx, place_ctx,
                                                pin_constraints,
                                                io_location_map,
                                                netlist_annotation,
                                                netlist_name,
                                                formal_verification_top_netlist_file_path,
                                                options.explicit_port_mapping());
    if (status == CMD_EXEC_FATAL_ERROR) {
      return status;
    }
  }

  if (true == options.print_preconfig_top_testbench()) {
    /* Generate top-level testbench using random vectors */
    std::string random_top_testbench_file_path = src_dir_path + netlist_name + std::string(RANDOM_TOP_TESTBENCH_VERILOG_FILE_POSTFIX);
    print_verilog_random_top_testbench(netlist_name,
                                       random_top_testbench_file_path,
                                       atom_ctx,
                                       netlist_annotation,
                                       module_manager,
                                       fabric_global_port_info,
                                       pin_constraints,
                                       simulation_setting,
                                       options.explicit_port_mapping());
  }

  /* Generate full testbench for verification, including configuration phase and operating phase */
  if (true == options.print_top_testbench()) {
    std::string top_testbench_file_path = src_dir_path + netlist_name + std::string(AUTOCHECK_TOP_TESTBENCH_VERILOG_FILE_POSTFIX);
    print_verilog_top_testbench(module_manager,
                                bitstream_manager, fabric_bitstream,
                                circuit_lib,
                                config_protocol,
                                fabric_global_port_info,
                                atom_ctx, place_ctx,
                                pin_constraints,
                                io_location_map,
                                netlist_annotation,
                                netlist_name,
                                top_testbench_file_path,
                                simulation_setting,
                                options);
  }

  /* Generate exchangeable files which contains simulation settings */
  if (true == options.print_simulation_ini()) {
    std::string simulation_ini_file_name = options.simulation_ini_path();
    VTR_ASSERT(true != options.simulation_ini_path().empty());
    print_verilog_simulation_info(simulation_ini_file_name,
                                  netlist_name,
                                  src_dir_path,
                                  atom_ctx, place_ctx, io_location_map,
                                  module_manager,
                                  config_protocol.type(),
                                  bitstream_manager.num_bits(),
                                  simulation_setting.num_clock_cycles(),
                                  simulation_setting.programming_clock_frequency(),
                                  simulation_setting.default_operating_clock_frequency());
  }

  /* Generate a Verilog file including all the netlists that have been generated */
  print_verilog_testbench_include_netlists(src_dir_path,
                                           netlist_name,
                                           options.fabric_netlist_file_path(),
                                           options.reference_benchmark_file_path());

  return status;
}

/********************************************************************
 * A top-level function of FPGA-Verilog which focuses on full testbench generation
 * This function will generate
 *  - Verilog netlist including preprocessing flags and all the Verilog netlists that have been generated
 ********************************************************************/
int fpga_verilog_full_testbench(const ModuleManager &module_manager,
                                const BitstreamManager &bitstream_manager,
                                const FabricBitstream &fabric_bitstream,
                                const AtomContext &atom_ctx,
                                const PlacementContext &place_ctx,
                                const PinConstraints& pin_constraints,
                                const std::string& bitstream_file,
                                const IoLocationMap &io_location_map,
                                const FabricGlobalPortInfo &fabric_global_port_info,
                                const VprNetlistAnnotation &netlist_annotation,
                                const CircuitLibrary &circuit_lib,
                                const SimulationSetting &simulation_setting,
                                const ConfigProtocol &config_protocol,
                                const VerilogTestbenchOption &options) {

  vtr::ScopedStartFinishTimer timer("Write Verilog full testbenches for FPGA fabric\n");

  std::string src_dir_path = format_dir_path(options.output_directory());

  std::string netlist_name = atom_ctx.nlist.netlist_name();

  int status = CMD_EXEC_SUCCESS;

  /* Create directories */
  create_directory(src_dir_path);

  /* Output preprocessing flags for HDL simulations */
  print_verilog_simulation_preprocessing_flags(std::string(src_dir_path),
                                               options);

  /* Generate full testbench for verification, including configuration phase and operating phase */
  std::string top_testbench_file_path = src_dir_path + netlist_name + std::string(AUTOCHECK_TOP_TESTBENCH_VERILOG_FILE_POSTFIX);
  print_verilog_full_testbench(module_manager,
                              bitstream_manager, fabric_bitstream,
                              circuit_lib,
                              config_protocol,
                              fabric_global_port_info,
                              atom_ctx, place_ctx,
                              pin_constraints,
                              bitstream_file,
                              io_location_map,
                              netlist_annotation,
                              netlist_name,
                              top_testbench_file_path,
                              simulation_setting,
                              options);

  /* Generate a Verilog file including all the netlists that have been generated */
  print_verilog_testbench_include_netlists(src_dir_path,
                                           netlist_name,
                                           options.fabric_netlist_file_path(),
                                           options.reference_benchmark_file_path());

  return status;
}

/********************************************************************
 * A top-level function of FPGA-Verilog which focuses on full testbench generation
 * This function will generate
 *  - A wrapper module, which encapsulate the FPGA module in a Verilog module which have the same port as the input benchmark
 ********************************************************************/
int fpga_verilog_preconfigured_fabric_wrapper(const ModuleManager &module_manager,
                                              const BitstreamManager &bitstream_manager,
                                              const AtomContext &atom_ctx,
                                              const PlacementContext &place_ctx,
                                              const PinConstraints& pin_constraints,
                                              const IoLocationMap &io_location_map,
                                              const FabricGlobalPortInfo &fabric_global_port_info,
                                              const VprNetlistAnnotation &netlist_annotation,
                                              const CircuitLibrary &circuit_lib,
                                              const ConfigProtocol &config_protocol,
                                              const VerilogTestbenchOption &options) {

  vtr::ScopedStartFinishTimer timer("Write a wrapper module for a preconfigured FPGA fabric\n");

  std::string src_dir_path = format_dir_path(options.output_directory());

  std::string netlist_name = atom_ctx.nlist.netlist_name();

  int status = CMD_EXEC_SUCCESS;

  /* Create directories */
  create_directory(src_dir_path);

  /* Generate wrapper module for FPGA fabric (mapped by the input benchmark and pre-configured testbench for verification */
  std::string formal_verification_top_netlist_file_path = src_dir_path + netlist_name + std::string(FORMAL_VERIFICATION_VERILOG_FILE_POSTFIX);
  status = print_verilog_preconfig_top_module(module_manager, bitstream_manager,
                                              config_protocol,
                                              circuit_lib, fabric_global_port_info,
                                              atom_ctx, place_ctx,
                                              pin_constraints,
                                              io_location_map,
                                              netlist_annotation,
                                              netlist_name,
                                              formal_verification_top_netlist_file_path,
                                              options.explicit_port_mapping());

  return status;
}


} /* end namespace openfpga */
