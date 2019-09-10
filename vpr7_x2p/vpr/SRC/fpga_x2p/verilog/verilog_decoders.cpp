/***************************************************************************************
 * This file includes functions to generate Verilog modules of decoders
 ***************************************************************************************/
/* TODO: merge verilog_decoder.c to this source file and rename to verilog_decoder.cpp */
#include <string>

#include "util.h"
#include "vtr_assert.h"

/* Device-level header files */
#include "decoder_library_utils.h"
#include "module_manager.h"

/* FPGA-X2P context header files */
#include "spice_types.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"

/* FPGA-Verilog context header files */
#include "verilog_global.h"
#include "verilog_writer_utils.h"
#include "verilog_decoders.h"

/***************************************************************************************
 * Create a Verilog module for a decoder with a given output size
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
void print_verilog_mux_local_decoder_module(std::fstream& fp, 
                                            ModuleManager& module_manager,
                                            const DecoderLibrary& decoder_lib,
                                            const DecoderId& decoder) {
  /* Get the number of inputs */
  size_t addr_size = decoder_lib.addr_size(decoder);
  size_t data_size = decoder_lib.data_size(decoder);

  /* Validate the FILE handler */
  check_file_handler(fp);

  /* TODO: create a name for the local encoder */
  std::string module_name = generate_mux_local_decoder_subckt_name(addr_size, data_size);

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = module_manager.add_module(module_name); 
  VTR_ASSERT(ModuleId::INVALID() != module_id);
  /* Add module ports */
  /* Add each input port */
  BasicPort addr_port("addr", addr_size);
  module_manager.add_port(module_id, addr_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add each output port */
  BasicPort data_port("data", data_size);
  module_manager.add_port(module_id, data_port, ModuleManager::MODULE_OUTPUT_PORT);
  /* Data port is registered. It should be outputted as 
   *   output reg [lsb:msb] data 
   */
  module_manager.set_port_is_register(module_id, data_port.get_name(), true);
  /* Add data_in port */
  BasicPort data_inv_port("data_inv", data_size);
  VTR_ASSERT(true == decoder_lib.use_data_inv_port(decoder));
  module_manager.add_port(module_id, data_inv_port, ModuleManager::MODULE_OUTPUT_PORT);

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id);
  /* Finish dumping ports */

  print_verilog_comment(fp, std::string("----- BEGIN Verilog codes for Decoder convert " + std::to_string(addr_size) + "-bit addr to " + std::to_string(data_size) + "-bit data -----"));

  /* Print the truth table of this decoder */
  /* Internal logics */
  /* We use a magic number -1 as the addr=1 should be mapped to ...1
   * Otherwise addr will map addr=1 to ..10 
   * Note that there should be a range for the shift operators
   * We should narrow the encoding to be applied to a given set of data
   * This will lead to that any addr which falls out of the op code of data
   * will give a all-zero code
   * For example: 
   * data is 5-bit while addr is 3-bit 
   * data=8'b0_0000 will be encoded to addr=3'b001;
   * data=8'b0_0001 will be encoded to addr=3'b010;
   * data=8'b0_0010 will be encoded to addr=3'b011;
   * data=8'b0_0100 will be encoded to addr=3'b100;
   * data=8'b0_1000 will be encoded to addr=3'b101;
   * data=8'b1_0000 will be encoded to addr=3'b110;
   * The rest of addr codes 3'b110, 3'b111 will be decoded to data=8'b0_0000;
   */

  fp << "\t" << "always@(" << generate_verilog_port(VERILOG_PORT_CONKT, addr_port) << ")" << std::endl;
  fp << "\t" << "case (" << generate_verilog_port(VERILOG_PORT_CONKT, addr_port) << ")" << std::endl;
  /* Create a string for addr and data */
  for (size_t i = 0; i < data_size; ++i) {
    /* TODO: give a namespace to the itobin function */
    fp << "\t\t" << generate_verilog_constant_values(my_itobin_vec(i, addr_size)); 
    fp << " : ";
    fp << generate_verilog_port_constant_values(data_port, my_ito1hot_vec(i, data_size)); 
    fp << ";" << std::endl;
  }
  fp << "\t\t" << "default : ";
  fp << generate_verilog_port_constant_values(data_port, my_ito1hot_vec(data_size - 1, data_size)); 
  fp << ";" << std::endl;
  fp << "\t" << "endcase" << std::endl;

  print_verilog_wire_connection(fp, data_inv_port, data_port, true);
  
  print_verilog_comment(fp, std::string("----- END Verilog codes for Decoder convert " + std::to_string(addr_size) + "-bit addr to " + std::to_string(data_size) + "-bit data -----"));

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, module_name);

  return;
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
void print_verilog_submodule_mux_local_decoders(ModuleManager& module_manager,
                                                const MuxLibrary& mux_lib,
                                                const CircuitLibrary& circuit_lib,
                                                const std::string& verilog_dir,
                                                const std::string& submodule_dir) {
  std::string verilog_fname(submodule_dir + local_encoder_verilog_file_name);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  /* Print out debugging information for if the file is not opened/created properly */
  vpr_printf(TIO_MESSAGE_INFO,
             "Creating Verilog netlist for local decoders for multiplexers (%s)...\n",
             verilog_fname.c_str()); 

  print_verilog_file_header(fp, "Local Decoders for Multiplexers"); 

  print_verilog_include_defines_preproc_file(fp, verilog_dir);

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
      /* Try to find if the decoder already exists in the library, 
       * If there is no such decoder, add it to the library 
       */
      add_mux_local_decoder_to_library(decoder_lib, branch_mux_graph.num_inputs());
    }
  }

  /* Generate Verilog modules for the found unique local encoders */
  for (const auto& decoder : decoder_lib.decoders()) {
    print_verilog_mux_local_decoder_module(fp, module_manager, decoder_lib, decoder);
  }

  /* Close the file steam */
  fp.close();

  /* Add fname to the linked list when debugging is finished */
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_fname.c_str());  
}

