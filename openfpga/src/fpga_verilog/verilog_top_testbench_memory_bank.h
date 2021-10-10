#ifndef VERILOG_TOP_TESTBENCH_MEMORY_BANK
#define VERILOG_TOP_TESTBENCH_MEMORY_BANK

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "module_manager.h"
#include "bitstream_manager.h"
#include "fabric_bitstream.h"
#include "circuit_library.h"
#include "config_protocol.h"
#include "vpr_context.h"
#include "pin_constraints.h"
#include "io_location_map.h"
#include "fabric_global_port_info.h"
#include "vpr_netlist_annotation.h"
#include "simulation_setting.h"
#include "memory_bank_shift_register_banks.h"
#include "verilog_testbench_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/**
 * @brief Print local wires for memory bank configuration protocols
 */
void print_verilog_top_testbench_ql_memory_bank_port(std::fstream& fp,
                                                     const ModuleManager& module_manager,
                                                     const ModuleId& top_module,
                                                     const ConfigProtocol& config_protocol);

/**
 * @brief Generate the Verilog codes that connect shift register clock stimuli to FPGA ports
 */
void print_verilog_top_testbench_global_shift_register_clock_ports_stimuli(std::fstream& fp,
                                                                           const ModuleManager& module_manager,
                                                                           const ModuleId& top_module,
                                                                           const FabricGlobalPortInfo& fabric_global_port_info);

/**
 * @brief Generate the Verilog codes that generate stimuli waveforms for BL/WL protocols
 */
int print_verilog_top_testbench_configuration_protocol_ql_memory_bank_stimulus(std::fstream& fp,
                                                                               const ConfigProtocol& config_protocol, 
                                                                               const SimulationSetting& sim_settings, 
                                                                               const ModuleManager& module_manager,
                                                                               const ModuleId& top_module,
                                                                               const bool& fast_configuration,
                                                                               const bool& bit_value_to_skip,
                                                                               const FabricBitstream& fabric_bitstream,
                                                                               const MemoryBankShiftRegisterBanks& blwl_sr_banks,
                                                                               const float& prog_clock_period,
                                                                               const float& timescale);

/**
 * @brief Print stimulus for a FPGA fabric with a memory bank configuration protocol
 *        where configuration bits are programming in serial (one by one)
 */
void print_verilog_full_testbench_ql_memory_bank_bitstream(std::fstream& fp,
                                                           const std::string& bitstream_file,
                                                           const ConfigProtocol& config_protocol,
                                                           const bool& fast_configuration,
                                                           const bool& bit_value_to_skip,
                                                           const ModuleManager& module_manager,
                                                           const ModuleId& top_module,
                                                           const FabricBitstream& fabric_bitstream,
                                                           const MemoryBankShiftRegisterBanks& blwl_sr_banks);

} /* end namespace openfpga */

#endif
