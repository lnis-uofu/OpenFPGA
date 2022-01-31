/*********************************************************************
 * This file includes functions to generate Verilog submodules for 
 * the memories that are affiliated to multiplexers and other programmable
 * circuit models, such as IOPADs, LUTs, etc.
 ********************************************************************/
#include <string>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "mux_graph.h"
#include "module_manager.h"
#include "circuit_library_utils.h"
#include "mux_utils.h"

#include "openfpga_naming.h"

#include "verilog_constants.h"
#include "verilog_writer_utils.h"
#include "verilog_module_writer.h"
#include "verilog_memory.h"

/* begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Generate Verilog modules for the memories that are used
 * by multiplexers  
 *
 *            +----------------+
 * mem_in --->|  Memory Module |---> mem_out
 *            +----------------+
 *              |  |  ... |  | 
 *              v  v      v  v SRAM ports of multiplexer
 *          +---------------------+
 *    in--->|  Multiplexer Module |---> out
 *          +---------------------+
 ********************************************************************/
static 
void print_verilog_mux_memory_module(const ModuleManager& module_manager,
                                     const CircuitLibrary& circuit_lib,
                                     std::fstream& fp,
                                     const CircuitModelId& mux_model,
                                     const MuxGraph& mux_graph,
                                     const FabricVerilogOption& options) {
  /* Multiplexers built with different technology is in different organization */
  switch (circuit_lib.design_tech_type(mux_model)) {
  case CIRCUIT_MODEL_DESIGN_CMOS: {
    /* Generate module name */
    std::string module_name = generate_mux_subckt_name(circuit_lib, mux_model, 
                                                       find_mux_num_datapath_inputs(circuit_lib, mux_model, mux_graph.num_inputs()), 
                                                       std::string(VERILOG_MEM_POSTFIX));
    ModuleId mem_module = module_manager.find_module(module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(mem_module));
    /* Write the module content in Verilog format */
    write_verilog_module_to_file(fp, module_manager, mem_module, 
                                 options.explicit_port_mapping() || circuit_lib.dump_explicit_port_map(mux_model),
                                 options.default_net_type());

    /* Add an empty line as a splitter */
    fp << std::endl;
    break;
  }
  case CIRCUIT_MODEL_DESIGN_RRAM:
    /* We do not need a memory submodule for RRAM MUX,
     * RRAM are embedded in the datapath  
     * TODO: generate local encoders for RRAM-based multiplexers here!!!
     */ 
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid design technology of multiplexer '%s'\n",
                   circuit_lib.model_name(mux_model).c_str()); 
    exit(1);
  }
}


/*********************************************************************
 * Generate Verilog modules for 
 * the memories that are affiliated to multiplexers and other programmable
 * circuit models, such as IOPADs, LUTs, etc.
 *
 * We keep the memory modules separated from the multiplexers and other 
 * programmable circuit models, for the sake of supporting
 * various configuration schemes.
 * By following such organiztion, the Verilog modules of the circuit models
 * implements the functionality (circuit logic) only, while the memory Verilog
 * modules implements the memory circuits as well as configuration protocols.
 * For example, the local decoders of multiplexers are implemented in the
 * memory modules. 
 * Take another example, the memory circuit can implement the scan-chain or 
 * memory-bank organization for the memories.
 ********************************************************************/
void print_verilog_submodule_memories(const ModuleManager& module_manager,
                                      NetlistManager& netlist_manager,
                                      const MuxLibrary& mux_lib,
                                      const CircuitLibrary& circuit_lib,
                                      const std::string& submodule_dir,
                                      const std::string& submodule_dir_name,
                                      const FabricVerilogOption& options) {
  /* Plug in with the mux subckt */
  std::string verilog_fname(MEMORIES_VERILOG_FILE_NAME);
  std::string verilog_fpath(submodule_dir + verilog_fname);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fpath, std::fstream::out | std::fstream::trunc);

  check_file_stream(verilog_fpath.c_str(), fp);

  /* Print out debugging information for if the file is not opened/created properly */
  VTR_LOG("Writing Verilog netlist for memories '%s' ...",
          verilog_fpath.c_str()); 

  print_verilog_file_header(fp, "Memories used in FPGA", options.time_stamp()); 

  /* Create the memory circuits for the multiplexer */
  for (auto mux : mux_lib.muxes()) {
    const MuxGraph& mux_graph = mux_lib.mux_graph(mux);
    CircuitModelId mux_model = mux_lib.mux_circuit_model(mux); 
    /* Bypass the non-MUX circuit models (i.e., LUTs). 
     * They should be handled in a different way 
     * Memory circuits of LUT includes both regular and mode-select ports
     */
    if (CIRCUIT_MODEL_MUX != circuit_lib.model_type(mux_model)) {
      continue;
    }
    /* Create a Verilog module for the memories used by the multiplexer */
    print_verilog_mux_memory_module(module_manager,
                                    circuit_lib,
                                    fp,
                                    mux_model,
                                    mux_graph,
                                    options);
  }

  /* Create the memory circuits for non-MUX circuit models.
   * In this case, the memory modules are designed to interface
   * the mode-select ports 
   */
  for (const auto& model : circuit_lib.models()) {
    /* Bypass MUXes, they have already been considered */
    if (CIRCUIT_MODEL_MUX == circuit_lib.model_type(model)) {
      continue;
    }
    /* Bypass those modules without any SRAM ports */
    std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(model, CIRCUIT_MODEL_PORT_SRAM, true);
    if (0 == sram_ports.size()) {
      continue;
    }
    /* Find the name of memory module */
    /* Get the total number of SRAMs */
    size_t num_mems = 0; 
    for (const auto& port : sram_ports) {
      num_mems += circuit_lib.port_size(port);
    }
    /* Get the circuit model for the memory circuit used by the multiplexer */
    std::vector<CircuitModelId> sram_models;
    for (const auto& port : sram_ports) {
      CircuitModelId sram_model = circuit_lib.port_tri_state_model(port);
      VTR_ASSERT(CircuitModelId::INVALID() != sram_model);
      /* Found in the vector of sram_models, do not update and go to the next */
      if (sram_models.end() != std::find(sram_models.begin(), sram_models.end(), sram_model)) {
        continue;
      }
      /* sram_model not found in the vector, update the sram_models */
      sram_models.push_back(sram_model);
    }
    /* Should have only 1 SRAM model */
    VTR_ASSERT( 1 == sram_models.size() );
  
    /* Create the module name for the memory block */
    std::string module_name = generate_memory_module_name(circuit_lib, model, sram_models[0], std::string(VERILOG_MEM_POSTFIX));

    ModuleId mem_module = module_manager.find_module(module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(mem_module));
    /* Write the module content in Verilog format */
    write_verilog_module_to_file(fp, module_manager, mem_module, 
                                 options.explicit_port_mapping() || circuit_lib.dump_explicit_port_map(model),
                                 options.default_net_type());

    /* Add an empty line as a splitter */
    fp << std::endl;
  }

  /* Close the file stream */
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = NetlistId::INVALID();
  if (options.use_relative_path()) {
    nlist_id = netlist_manager.add_netlist(submodule_dir_name + verilog_fname);
  } else {
    nlist_id = netlist_manager.add_netlist(verilog_fpath);
  }
  VTR_ASSERT(nlist_id);
  netlist_manager.set_netlist_type(nlist_id, NetlistManager::SUBMODULE_NETLIST);

  VTR_LOG("Done\n");
}

} /* end namespace openfpga */
