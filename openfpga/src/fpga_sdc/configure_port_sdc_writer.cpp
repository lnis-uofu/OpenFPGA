/********************************************************************
 * This file includes functions that print SDC (Synopsys Design Constraint) 
 * files in physical design tools, i.e., Place & Route (PnR) tools
 * The SDC files are used to constrain the timing of configuration chain 
 *
 * Note that this is different from the SDC to constrain VPR Place&Route
 * engine! These SDCs are designed for PnR to generate FPGA layouts!!!
 *******************************************************************/
#include <ctime>
#include <fstream>
#include <iomanip>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from openfpgautil library */
#include "openfpga_scale.h"
#include "openfpga_port.h"
#include "openfpga_digest.h"

#include "openfpga_naming.h"

#include "sdc_writer_naming.h"
#include "sdc_writer_utils.h"
#include "sdc_mux_utils.h"
#include "configure_port_sdc_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Break combinational loops in FPGA fabric, which mainly come from 
 * non-MUX programmable modules
 * To handle this, we disable the timing at configuration ports
 *
 * Return code:
 *   0: success
 *   1: fatal error occurred
 *******************************************************************/
static 
int print_sdc_disable_non_mux_circuit_configure_ports(std::fstream& fp,
                                                      const bool& flatten_names,
                                                      const CircuitLibrary& circuit_lib,
                                                      const ModuleManager& module_manager,
                                                      const ModuleId& top_module) {

  if (false == valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Iterate over the MUX modules */
  for (const CircuitModelId& model : circuit_lib.models()) {
    
    /* Skip MUXes, they are handled in another function */
    if (CIRCUIT_MODEL_MUX == circuit_lib.model_type(model)) {
      continue;
    }

    /* We care programmable circuit models only */
    if (0 == circuit_lib.model_ports_by_type(model, CIRCUIT_MODEL_PORT_SRAM).size()) {
      continue;
    }

    std::string programmable_module_name = circuit_lib.model_name(model);

    /* Find the module name in module manager */
    ModuleId programmable_module = module_manager.find_module(programmable_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(programmable_module));

    /* Go recursively in the module manager, 
     * starting from the top-level module: instance id of the top-level module is 0 by default 
     * Disable all the outputs of child modules that matches the mux_module id
     */
    for (const CircuitPortId& sram_port : circuit_lib.model_ports_by_type(model, CIRCUIT_MODEL_PORT_SRAM)) {
      const std::string& sram_port_name = circuit_lib.port_lib_name(sram_port);
      if (CMD_EXEC_FATAL_ERROR == 
            rec_print_sdc_disable_timing_for_module_ports(fp,
                                                          flatten_names,
                                                          module_manager,
                                                          top_module,
                                                          programmable_module,
                                                          format_dir_path(module_manager.module_name(top_module)),
                                                          sram_port_name)) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Break combinational loops in FPGA fabric, which mainly come from
 * the configure ports of each programmable module. 
 * To handle this, we disable the configure ports of 
 *   - routing multiplexers
 *   - other circuit model that has SRAM ports
 *******************************************************************/
int print_sdc_disable_timing_configure_ports(const std::string& sdc_fname,
                                             const bool& flatten_names,
                                             const MuxLibrary& mux_lib,
                                             const CircuitLibrary& circuit_lib,
                                             const ModuleManager& module_manager) {
  /* Create the directory */
  create_directory(find_path_dir_name(sdc_fname));

  /* Start time count */
  std::string timer_message = std::string("Write SDC to disable timing on configuration outputs of programmable cells for P&R flow '") + sdc_fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Disable configuration outputs of all the programmable cells for PnR"));

  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));
 
  /* Disable timing for the configure ports of all the routing multiplexer */
  VTR_LOG("Write disable timing for routing multiplexers...");
  if (CMD_EXEC_FATAL_ERROR == print_sdc_disable_routing_multiplexer_configure_ports(fp,
                                                                                    flatten_names,
                                                                                    mux_lib,
                                                                                    circuit_lib,
                                                                                    module_manager,
                                                                                    top_module)) {
    VTR_LOG("Fatal errors occurred\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  VTR_LOG("Done\n");

  /* Disable timing for the other programmable circuit models */
  VTR_LOG("Write disable timing for other programmable modules...");
  if (CMD_EXEC_FATAL_ERROR == print_sdc_disable_non_mux_circuit_configure_ports(fp,
                                                                                flatten_names,
                                                                                circuit_lib,
                                                                                module_manager,
                                                                                top_module)) {
    VTR_LOG("Fatal errors occurred\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  VTR_LOG("Done\n");

  /* Close file handler */
  fp.close();

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
