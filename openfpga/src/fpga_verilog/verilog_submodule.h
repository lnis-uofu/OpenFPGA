#ifndef VERILOG_SUBMODULE_H
#define VERILOG_SUBMODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "decoder_library.h"
#include "fabric_verilog_options.h"
#include "memory_bank_shift_register_banks.h"
#include "module_manager.h"
#include "module_name_map.h"
#include "mux_library.h"
#include "netlist_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule(
  ModuleManager& module_manager, NetlistManager& netlist_manager,
  const MemoryBankShiftRegisterBanks& blwl_sr_banks, const MuxLibrary& mux_lib,
  const DecoderLibrary& decoder_lib, const CircuitLibrary& circuit_lib,
  const ModuleNameMap& module_name_map, const std::string& submodule_dir,
  const std::string& submodule_dir_name,
  const FabricVerilogOption& fpga_verilog_opts);

} /* end namespace openfpga */

#endif
