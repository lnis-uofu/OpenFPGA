/********************************************************************
 * This file includes functions that are used to create
 * an auto-check top-level testbench for a FPGA fabric
 *******************************************************************/
#include <fstream>
#include <iomanip>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"
#include "openfpga_digest.h"

#include "bitstream_manager_utils.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"
#include "simulation_utils.h"
#include "openfpga_atom_netlist_utils.h"

#include "fast_configuration.h"
#include "fabric_bitstream_utils.h"
#include "fabric_global_port_info_utils.h"

#include "verilog_constants.h"
#include "verilog_writer_utils.h"
#include "verilog_testbench_utils.h"
#include "verilog_top_testbench_memory_bank.h"

#include "verilog_top_testbench_constants.h"

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_full_testbench_ql_memory_bank_bitstream(std::fstream& fp,
                                                           const std::string& bitstream_file,
                                                           const bool& fast_configuration,
                                                           const bool& bit_value_to_skip,
                                                           const ModuleManager& module_manager,
                                                           const ModuleId& top_module,
                                                           const FabricBitstream& fabric_bitstream) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Reorganize the fabric bitstream by the same address across regions */
  MemoryBankFabricBitstream fabric_bits_by_addr = build_memory_bank_fabric_bitstream_by_address(fabric_bitstream);

  /* For fast configuration, identify the final bitstream size to be used */
  size_t num_bits_to_skip = 0;
  if (true == fast_configuration) {
    num_bits_to_skip = fabric_bits_by_addr.size() - find_memory_bank_fast_configuration_fabric_bitstream_size(fabric_bitstream, bit_value_to_skip);
  }
  VTR_ASSERT(num_bits_to_skip < fabric_bits_by_addr.size());

  /* Feed address and data input pair one by one
   * Note: the first cycle is reserved for programming reset
   * We should give dummy values
   */
  ModulePortId bl_addr_port_id = module_manager.find_module_port(top_module,
                                                                 std::string(DECODER_BL_ADDRESS_PORT_NAME));
  BasicPort bl_addr_port = module_manager.module_port(top_module, bl_addr_port_id);
  std::vector<size_t> initial_bl_addr_values(bl_addr_port.get_width(), 0);

  ModulePortId wl_addr_port_id = module_manager.find_module_port(top_module,
                                                                 std::string(DECODER_WL_ADDRESS_PORT_NAME));
  BasicPort wl_addr_port = module_manager.module_port(top_module, wl_addr_port_id);
  std::vector<size_t> initial_wl_addr_values(wl_addr_port.get_width(), 0);

  ModulePortId din_port_id = module_manager.find_module_port(top_module,
                                                             std::string(DECODER_DATA_IN_PORT_NAME));
  BasicPort din_port = module_manager.module_port(top_module, din_port_id);
  std::vector<size_t> initial_din_values(din_port.get_width(), 0);

  /* Define a constant for the bitstream length */
  print_verilog_define_flag(fp, std::string(TOP_TB_BITSTREAM_LENGTH_VARIABLE), fabric_bits_by_addr.size() - num_bits_to_skip); 
  print_verilog_define_flag(fp, std::string(TOP_TB_BITSTREAM_WIDTH_VARIABLE), bl_addr_port.get_width() + wl_addr_port.get_width() + din_port.get_width()); 

  /* Declare local variables for bitstream loading in Verilog */
  print_verilog_comment(fp, "----- Virtual memory to store the bitstream from external file -----");
  fp << "reg [0:`" << TOP_TB_BITSTREAM_WIDTH_VARIABLE << " - 1] ";
  fp << TOP_TB_BITSTREAM_MEM_REG_NAME << "[0:`" << TOP_TB_BITSTREAM_LENGTH_VARIABLE << " - 1];";
  fp << std::endl;

  fp << "reg [$clog2(`" << TOP_TB_BITSTREAM_LENGTH_VARIABLE << "):0] " << TOP_TB_BITSTREAM_INDEX_REG_NAME << ";" << std::endl;

  print_verilog_comment(fp, "----- Preload bitstream file to a virtual memory -----");
  fp << "initial begin" << std::endl;
  fp << "\t";
  fp << "$readmemb(\"" << bitstream_file << "\", " << TOP_TB_BITSTREAM_MEM_REG_NAME << ");";
  fp << std::endl;

  print_verilog_comment(fp, "----- Bit-Line Address port default input -----");
  fp << "\t";
  fp << generate_verilog_port_constant_values(bl_addr_port, initial_bl_addr_values);
  fp << ";";
  fp << std::endl;

  print_verilog_comment(fp, "----- Word-Line Address port default input -----");
  fp << "\t";
  fp << generate_verilog_port_constant_values(wl_addr_port, initial_wl_addr_values);
  fp << ";";
  fp << std::endl;

  print_verilog_comment(fp, "----- Data-input port default input -----");
  fp << "\t";
  fp << generate_verilog_port_constant_values(din_port, initial_din_values);
  fp << ";";
  fp << std::endl;

  fp << "\t";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME << " <= 0";
  fp << ";";
  fp << std::endl;

  fp << "end";
  fp << std::endl;

  print_verilog_comment(fp, "----- Begin bitstream loading during configuration phase -----");
  BasicPort prog_clock_port(std::string(TOP_TB_PROG_CLOCK_PORT_NAME) + std::string(TOP_TB_CLOCK_REG_POSTFIX), 1);
  fp << "always";
  fp << " @(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ")"; 
  fp << " begin";
  fp << std::endl;

  fp << "\t";
  fp << "if (";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME;
  fp << " >= ";
  fp << "`" << TOP_TB_BITSTREAM_LENGTH_VARIABLE;
  fp << ") begin";
  fp << std::endl;

  BasicPort config_done_port(std::string(TOP_TB_CONFIG_DONE_PORT_NAME), 1);
  fp << "\t\t";
  std::vector<size_t> config_done_final_values(config_done_port.get_width(), 1);
  fp << generate_verilog_port_constant_values(config_done_port, config_done_final_values, true);
  fp << ";" << std::endl;

  fp << "\t";
  fp << "end else begin";
  fp << std::endl;

  fp << "\t\t";
  fp << "{";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, bl_addr_port); 
  fp << ", ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, wl_addr_port); 
  fp << ", ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, din_port); 
  fp << "}";
  fp << " <= ";
  fp << TOP_TB_BITSTREAM_MEM_REG_NAME << "[" << TOP_TB_BITSTREAM_INDEX_REG_NAME << "]";
  fp << ";" << std::endl;

  fp << "\t\t";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME;
  fp << " <= ";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME << " + 1";
  fp << ";" << std::endl;

  fp << "\t";
  fp << "end";
  fp << std::endl;

  fp << "end";
  fp << std::endl;

  print_verilog_comment(fp, "----- End bitstream loading during configuration phase -----");
}

} /* end namespace openfpga */
