#ifndef BUILD_TOP_MODULE_MEMORY_BANK_H
#define BUILD_TOP_MODULE_MEMORY_BANK_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <vector>
#include <map>
#include <array>
#include "vtr_vector.h"
#include "vtr_ndmatrix.h"
#include "module_manager.h"
#include "config_protocol.h"
#include "circuit_library.h"
#include "decoder_library.h"
#include "memory_bank_shift_register_banks.h"
#include "build_top_module_memory_utils.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void add_top_module_nets_cmos_ql_memory_bank_config_bus(ModuleManager& module_manager,
                                                        DecoderLibrary& decoder_lib,
                                                        MemoryBankShiftRegisterBanks& blwl_sr_banks,
                                                        const ModuleId& top_module,
                                                        const CircuitLibrary& circuit_lib,
                                                        const ConfigProtocol& config_protocol,
                                                        const TopModuleNumConfigBits& num_config_bits);

void add_top_module_ql_memory_bank_sram_ports(ModuleManager& module_manager, 
                                              const ModuleId& module_id,
                                              const CircuitLibrary& circuit_lib,
                                              const ConfigProtocol& config_protocol,
                                              const TopModuleNumConfigBits& num_config_bits);

int load_top_module_shift_register_banks_from_fabric_key(const FabricKey& fabric_key,
                                                         MemoryBankShiftRegisterBanks& blwl_sr_banks);

void sync_memory_bank_shift_register_banks_with_config_protocol_settings(ModuleManager& module_manager,
                                                                         MemoryBankShiftRegisterBanks& blwl_sr_banks,
                                                                         const ConfigProtocol& config_protocol,
                                                                         const ModuleId& top_module,
                                                                         const CircuitLibrary& circuit_lib);

} /* end namespace openfpga */

#endif
