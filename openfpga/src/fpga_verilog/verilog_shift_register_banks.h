#ifndef VERILOG_SHIFT_REGISTER_BANKS_H
#define VERILOG_SHIFT_REGISTER_BANKS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>

#include "memory_bank_shift_register_banks.h"
#include "module_manager.h"
#include "netlist_manager.h"
#include "fabric_verilog_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_shift_register_banks(const ModuleManager& module_manager,
                                                  NetlistManager& netlist_manager,
                                                  const std::array<MemoryBankShiftRegisterBanks, 2>& blwl_sr_banks,
                                                  const std::string& submodule_dir,
                                                  const FabricVerilogOption& options);

} /* end namespace openfpga */

#endif
