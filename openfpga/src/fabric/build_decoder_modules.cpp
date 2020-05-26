/***************************************************************************************
 * This file includes functions that are used to build modules for decoders, including:
 * 1. Local decoders used by multiplexers ONLY
 * 2. Decoders used by grid/routing/top-level module for memory address decoding
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"
#include "decoder_library_utils.h"
#include "module_manager_utils.h"

#include "build_decoder_modules.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Create a module for a decoder with a given output size
 *
 *                     Data input
 *                   | | ... | 
 *                   v v     v
 *                 +-----------+
 *                /             \
 *      enable-->/    Decoder    \
 *              +-----------------+
 *                | | | ... | | |
 *                v v v     v v v
 *                  Data Outputs
 *               
 *  The outputs are assumes to be one-hot codes (at most only one '1' exist)
 *  Considering this fact, there are only num_of_outputs conditions to be encoded.
 *  Therefore, the number of inputs is ceil(log(num_of_outputs)/log(2))
 ***************************************************************************************/
ModuleId build_frame_memory_decoder_module(ModuleManager& module_manager,
                                           const DecoderLibrary& decoder_lib,
                                           const DecoderId& decoder) {
  /* Get the number of inputs */
  size_t addr_size = decoder_lib.addr_size(decoder);
  size_t data_size = decoder_lib.data_size(decoder);

  /* Create a name for the local encoder */
  std::string module_name = generate_frame_memory_decoder_subckt_name(addr_size, data_size);

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = module_manager.add_module(module_name); 
  VTR_ASSERT(true == module_manager.valid_module_id(module_id));

  /* Add enable port */
  BasicPort en_port(std::string(DECODER_ENABLE_PORT_NAME), 1);
  module_manager.add_port(module_id, en_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add each input port */
  BasicPort addr_port(std::string(DECODER_ADDRESS_PORT_NAME), addr_size);
  module_manager.add_port(module_id, addr_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add each output port */
  BasicPort data_port(std::string(DECODER_DATA_PORT_NAME), data_size);
  module_manager.add_port(module_id, data_port, ModuleManager::MODULE_OUTPUT_PORT);

  /* Data port is registered. It should be outputted as 
   *   output reg [lsb:msb] data 
   */
  module_manager.set_port_is_register(module_id, data_port.get_name(), true);
  /* Add data_in port */
  if (true == decoder_lib.use_data_inv_port(decoder)) {
    BasicPort data_inv_port(std::string(DECODER_DATA_INV_PORT_NAME), data_size);
    module_manager.add_port(module_id, data_inv_port, ModuleManager::MODULE_OUTPUT_PORT);
  }

  return module_id;
}

/***************************************************************************************
 * Create a module for a decoder with a given output size
 *
 *                     Inputs 
 *                   | | ... | 
 *                   v v     v
 *                 +-----------+
 *                /             \
 *               /    Decoder    \
 *              +-----------------+
 *                | | | ... | | |
 *                v v v     v v v
 *                    Outputs
 *               
 *  The outputs are assumes to be one-hot codes (at most only one '1' exist)
 *  Considering this fact, there are only num_of_outputs conditions to be encoded.
 *  Therefore, the number of inputs is ceil(log(num_of_outputs)/log(2))
 ***************************************************************************************/
static 
void build_mux_local_decoder_module(ModuleManager& module_manager,
                                    const DecoderLibrary& decoder_lib,
                                    const DecoderId& decoder) {
  /* Get the number of inputs */
  size_t addr_size = decoder_lib.addr_size(decoder);
  size_t data_size = decoder_lib.data_size(decoder);

  /* Create a name for the local encoder */
  std::string module_name = generate_mux_local_decoder_subckt_name(addr_size, data_size);

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = module_manager.add_module(module_name); 
  VTR_ASSERT(true == module_manager.valid_module_id(module_id));
  /* Add module ports */
  /* Add each input port */
  BasicPort addr_port(generate_mux_local_decoder_addr_port_name(), addr_size);
  module_manager.add_port(module_id, addr_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add each output port */
  BasicPort data_port(generate_mux_local_decoder_data_port_name(), data_size);
  module_manager.add_port(module_id, data_port, ModuleManager::MODULE_OUTPUT_PORT);
  /* Data port is registered. It should be outputted as 
   *   output reg [lsb:msb] data 
   */
  module_manager.set_port_is_register(module_id, data_port.get_name(), true);
  /* Add data_in port */
  BasicPort data_inv_port(generate_mux_local_decoder_data_inv_port_name(), data_size);
  VTR_ASSERT(true == decoder_lib.use_data_inv_port(decoder));
  module_manager.add_port(module_id, data_inv_port, ModuleManager::MODULE_OUTPUT_PORT);
}

/***************************************************************************************
 * This function will generate all the unique Verilog modules of local decoders for 
 * the multiplexers used in a FPGA fabric
 * It will reach the goal in two steps:
 * 1. Find the unique local decoders w.r.t. the number of inputs/outputs 
 *    We will generate the subgraphs from the multiplexing graph of each multiplexers
 *    The number of memory bits is the number of outputs.
 *    From that we can infer the number of inputs of each local decoders.
 *    Here is an illustrative example of how local decoders are interfaced with multi-level MUXes
 *
 *        +---------+         +---------+
 *        |  Local  |         |  Local  |
 *        | Decoder |         | Decoder |
 *        |    A    |         |    B    |
 *        +---------+         +---------+
 *          | ... |             | ... |
 *          v     v             v     v
 *      +--------------+    +--------------+
 *      |  MUX Level 0 |--->|  MUX Level 1 |
 *      +--------------+    +--------------+
 * 2. Generate local decoder Verilog modules using behavioral description.
 *    Note that the implementation of local decoders can be dependent on the technology
 *    and standard cell libraries.
 *    Therefore, behavioral Verilog is used and the local decoders should be synthesized 
 *    before running the back-end flow for FPGA fabric 
 *    See more details in the function print_verilog_mux_local_decoder() for more details
 ***************************************************************************************/
void build_mux_local_decoder_modules(ModuleManager& module_manager,
                                     const MuxLibrary& mux_lib,
                                     const CircuitLibrary& circuit_lib) {
  vtr::ScopedStartFinishTimer timer("Build local encoder (for multiplexers) modules");

  /* Create a library for local encoders with different sizes */
  DecoderLibrary decoder_lib;
  
  /* Find unique local decoders for unique branches shared by the multiplexers */
  for (auto mux : mux_lib.muxes()) {
    /* Local decoders are need only when users specify them */
    CircuitModelId mux_circuit_model = mux_lib.mux_circuit_model(mux); 
    /* If this MUX does not need local decoder, we skip it */
    if (false == circuit_lib.mux_use_local_encoder(mux_circuit_model)) {
      continue;
    }

    const MuxGraph& mux_graph = mux_lib.mux_graph(mux);
    /* Create a mux graph for the branch circuit */
    std::vector<MuxGraph> branch_mux_graphs = mux_graph.build_mux_branch_graphs();
    /* Add the decoder to the decoder library */
    for (auto branch_mux_graph : branch_mux_graphs) {
      /* The decoder size depends on the number of memories of a branch MUX.
       * Note that only when there are >=2 memories, a decoder is needed
       */
      size_t decoder_data_size = branch_mux_graph.num_memory_bits();
      if (0 == decoder_data_size) {
        continue;
      }
      /* Try to find if the decoder already exists in the library, 
       * If there is no such decoder, add it to the library 
       */
      add_mux_local_decoder_to_library(decoder_lib, decoder_data_size);
    }
  }

  /* Generate Verilog modules for the found unique local encoders */
  for (const auto& decoder : decoder_lib.decoders()) {
    build_mux_local_decoder_module(module_manager, decoder_lib, decoder);
  }
}

} /* end namespace openfpga */
