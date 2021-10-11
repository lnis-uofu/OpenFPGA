#ifndef BUILD_TOP_MODULE_MEMORY_H
#define BUILD_TOP_MODULE_MEMORY_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <vector>
#include <map>
#include "vtr_vector.h"
#include "vtr_ndmatrix.h"
#include "module_manager.h"
#include "circuit_types.h"
#include "circuit_library.h"
#include "config_protocol.h"
#include "decoder_library.h"
#include "device_grid.h"
#include "device_rr_gsb.h"
#include "fabric_key.h"
#include "config_protocol.h"
#include "memory_bank_shift_register_banks.h"
#include "build_top_module_memory_utils.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void organize_top_module_memory_modules(ModuleManager& module_manager, 
                                        const ModuleId& top_module,
                                        const CircuitLibrary& circuit_lib,
                                        const ConfigProtocol& config_protocol,
                                        const CircuitModelId& sram_model,
                                        const DeviceGrid& grids,
                                        const vtr::Matrix<size_t>& grid_instance_ids,
                                        const DeviceRRGSB& device_rr_gsb,
                                        const vtr::Matrix<size_t>& sb_instance_ids,
                                        const std::map<t_rr_type, vtr::Matrix<size_t>>& cb_instance_ids,
                                        const bool& compact_routing_hierarchy);

void shuffle_top_module_configurable_children(ModuleManager& module_manager, 
                                              const ModuleId& top_module,
                                              const ConfigProtocol& config_protocol);

int load_top_module_memory_modules_from_fabric_key(ModuleManager& module_manager,
                                                   const ModuleId& top_module,
                                                   const CircuitLibrary& circuit_lib,
                                                   const ConfigProtocol& config_protocol,
                                                   const FabricKey& fabric_key); 

TopModuleNumConfigBits find_top_module_regional_num_config_bit(const ModuleManager& module_manager,
                                                               const ModuleId& top_module,
                                                               const CircuitLibrary& circuit_lib,
                                                               const CircuitModelId& sram_model,
                                                               const e_config_protocol_type& config_protocol_type);

void add_top_module_sram_ports(ModuleManager& module_manager, 
                               const ModuleId& module_id,
                               const CircuitLibrary& circuit_lib,
                               const CircuitModelId& sram_model,
                               const ConfigProtocol& config_protocol,
                               const MemoryBankShiftRegisterBanks& blwl_sr_banks,
                               const TopModuleNumConfigBits& num_config_bits);

void add_top_module_nets_memory_config_bus(ModuleManager& module_manager,
                                           DecoderLibrary& decoder_lib,
                                           MemoryBankShiftRegisterBanks& blwl_sr_banks,
                                           const ModuleId& parent_module,
                                           const CircuitLibrary& circuit_lib,
                                           const ConfigProtocol& config_protocol, 
                                           const e_circuit_model_design_tech& mem_tech,
                                           const TopModuleNumConfigBits& num_config_bits);

} /* end namespace openfpga */

#endif
