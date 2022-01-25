/********************************************************************
 * Most utilized function used to constrain routing multiplexers in FPGA
 * fabric using SDC commands
 *******************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"

#include "mux_utils.h"
#include "circuit_library_utils.h"

#include "sdc_writer_naming.h"
#include "sdc_writer_utils.h"

#include "sdc_mux_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Break combinational loops in FPGA fabric, which mainly come from 
 * loops of multiplexers.
 * To handle this, we disable the timing at outputs of routing multiplexers
 *******************************************************************/
void print_sdc_disable_routing_multiplexer_outputs(const std::string& sdc_dir,
                                                   const bool& flatten_names,
                                                   const bool& include_time_stamp,
                                                   const MuxLibrary& mux_lib,
                                                   const CircuitLibrary& circuit_lib,
                                                   const ModuleManager& module_manager,
                                                   const ModuleId& top_module) {
  /* Create the file name for Verilog netlist */
  std::string sdc_fname(sdc_dir + std::string(SDC_DISABLE_MUX_OUTPUTS_FILE_NAME));

  /* Start time count */
  std::string timer_message = std::string("Write SDC to disable routing multiplexer outputs for P&R flow '") + sdc_fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(sdc_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(sdc_fname.c_str(), fp);

  /* Generate the descriptions*/
  print_sdc_file_header(fp, std::string("Disable routing multiplexer outputs for PnR"), include_time_stamp);

  /* Iterate over the MUX modules */
  for (const MuxId& mux_id : mux_lib.muxes()) {
    const CircuitModelId& mux_model = mux_lib.mux_circuit_model(mux_id);
    
    /* Skip LUTs, we only care about multiplexers here */
    if (CIRCUIT_MODEL_MUX != circuit_lib.model_type(mux_model)) {
      continue;
    }

    const MuxGraph& mux_graph = mux_lib.mux_graph(mux_id);
    std::string mux_module_name = generate_mux_subckt_name(circuit_lib, mux_model, 
                                                           find_mux_num_datapath_inputs(circuit_lib, mux_model, mux_graph.num_inputs()), 
                                                           std::string(""));

    /* Find the module name in module manager */
    ModuleId mux_module = module_manager.find_module(mux_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(mux_module));

    /* Go recursively in the module manager, 
     * starting from the top-level module: instance id of the top-level module is 0 by default 
     * Disable all the outputs of child modules that matches the mux_module id
     */
    for (const BasicPort& output_port : module_manager.module_ports_by_type(mux_module, ModuleManager::MODULE_OUTPUT_PORT)) {
      rec_print_sdc_disable_timing_for_module_ports(fp,
                                                    flatten_names,
                                                    module_manager,
                                                    top_module,
                                                    mux_module,
                                                    format_dir_path(module_manager.module_name(top_module)),
                                                    output_port.get_name());
    }
  }

  /* Close file handler */
  fp.close();
}

/********************************************************************
 * Break combinational loops in FPGA fabric, which mainly come from 
 * loops of multiplexers.
 * To handle this, we disable the timing at configuration ports of routing multiplexers
 *
 * Return code:
 *   0: success
 *   1: fatal error occurred
 *******************************************************************/
int print_sdc_disable_routing_multiplexer_configure_ports(std::fstream& fp,
                                                          const bool& flatten_names,
                                                          const MuxLibrary& mux_lib,
                                                          const CircuitLibrary& circuit_lib,
                                                          const ModuleManager& module_manager,
                                                          const ModuleId& top_module) {

  if (false == valid_file_stream(fp)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Iterate over the MUX modules */
  for (const MuxId& mux_id : mux_lib.muxes()) {
    const CircuitModelId& mux_model = mux_lib.mux_circuit_model(mux_id);

    /* Skip LUTs, we only care about multiplexers here */
    if (CIRCUIT_MODEL_MUX != circuit_lib.model_type(mux_model)) {
      continue;
    }
    
    const MuxGraph& mux_graph = mux_lib.mux_graph(mux_id);
    std::string mux_module_name = generate_mux_subckt_name(circuit_lib, mux_model, 
                                                           find_mux_num_datapath_inputs(circuit_lib, mux_model, mux_graph.num_inputs()), 
                                                           std::string(""));

    /* Find the module name in module manager */
    ModuleId mux_module = module_manager.find_module(mux_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(mux_module));

    /* Go recursively in the module manager, 
     * starting from the top-level module: instance id of the top-level module is 0 by default 
     * Disable all the outputs of child modules that matches the mux_module id
     */
    for (const CircuitPortId& mux_sram_port : find_circuit_regular_sram_ports(circuit_lib, mux_model)) {
      const std::string& mux_sram_port_name = circuit_lib.port_lib_name(mux_sram_port);
      VTR_ASSERT(true == module_manager.valid_module_port_id(mux_module, module_manager.find_module_port(mux_module, mux_sram_port_name)));
      if (CMD_EXEC_FATAL_ERROR == 
            rec_print_sdc_disable_timing_for_module_ports(fp,
                                                          flatten_names,
                                                          module_manager,
                                                          top_module,
                                                          mux_module,
                                                          format_dir_path(module_manager.module_name(top_module)),
                                                          mux_sram_port_name)) {
        return CMD_EXEC_FATAL_ERROR;
      }

      const std::string& mux_sram_inv_port_name = circuit_lib.port_lib_name(mux_sram_port) + INV_PORT_POSTFIX;
      VTR_ASSERT(true == module_manager.valid_module_port_id(mux_module, module_manager.find_module_port(mux_module, mux_sram_inv_port_name)));
      if (CMD_EXEC_FATAL_ERROR == 
            rec_print_sdc_disable_timing_for_module_ports(fp,
                                                          flatten_names,
                                                          module_manager,
                                                          top_module,
                                                          mux_module,
                                                          format_dir_path(module_manager.module_name(top_module)),
                                                          mux_sram_inv_port_name)) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }

  }

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
