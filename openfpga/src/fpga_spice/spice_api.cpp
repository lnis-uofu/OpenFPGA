/********************************************************************
 * This file include top-level function of FPGA-SPICE
 ********************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
#include "openfpga_reserved_words.h"

#include "spice_constants.h"
#include "spice_submodule.h"
#include "spice_routing.h"

/* Header file for this source file */
#include "spice_api.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A top-level function of FPGA-SPICE which focuses on fabric Spice generation
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
 *    All the testbench generation should be in the function fpga_testbench_spice()
 ********************************************************************/
int fpga_fabric_spice(const ModuleManager& module_manager,
                      NetlistManager& netlist_manager,
                      const Arch& openfpga_arch,
                      const MuxLibrary& mux_lib,
                      const DeviceRRGSB &device_rr_gsb,
                      const FabricSpiceOption& options) {

  vtr::ScopedStartFinishTimer timer("Write SPICE netlists for FPGA fabric\n");

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

  /* Generate primitive Spice modules, which are corner stones of FPGA fabric
   * Note that this function MUST be called before Spice generation of
   * core logic (i.e., logic blocks and routing resources) !!!
   * This is because that this function will add the primitive Spice modules to
   * the module manager.
   * Without the modules in the module manager, core logic generation is not possible!!!
   */
  int status = CMD_EXEC_SUCCESS;

  status = print_spice_submodule(netlist_manager,
                                 module_manager,
                                 openfpga_arch,
                                 mux_lib,
                                 submodule_dir_path);
 
  if (CMD_EXEC_SUCCESS != status) {
    return status;
  }

  /* Generate routing blocks */
  if (true == options.compress_routing()) {
    print_spice_unique_routing_modules(netlist_manager,
                                       module_manager,
                                       device_rr_gsb,
                                       rr_dir_path);
  } else {
    VTR_ASSERT(false == options.compress_routing());
    print_spice_flatten_routing_modules(netlist_manager,
                                        module_manager,
                                        device_rr_gsb,
                                        rr_dir_path);
  }

  /* Given a brief stats on how many Spice modules have been written to files */
  VTR_LOGV(options.verbose_output(),
           "Written %lu SPICE modules in total\n",
           module_manager.num_modules());

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
