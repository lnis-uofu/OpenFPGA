#ifndef MEMORY_BANK_UTILS_H
#define MEMORY_BANK_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <vector>
#include <map>
#include "vtr_vector.h"
#include "vtr_ndmatrix.h"
#include "module_manager.h"
#include "circuit_library.h"
#include "decoder_library.h"
#include "build_top_module_memory_utils.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/**
 * @brief Precompute the range of x coordinates of all the configurable children under a specific configuration region
 * The lower bound is stored in the first element of the return struct
 * The upper bound is stored in the second element of the return struct
 */
std::pair<int, int> compute_memory_bank_regional_configurable_child_x_range(const ModuleManager& module_manager,
                                                                            const ModuleId& top_module,
                                                                            const ConfigRegionId& config_region);

/**
 * @brief Precompute the range of y coordinates of all the configurable children under a specific configuration region
 * The lower bound is stored in the first element of the return struct
 * The upper bound is stored in the second element of the return struct
 */
std::pair<int, int> compute_memory_bank_regional_configurable_child_y_range(const ModuleManager& module_manager,
                                                                            const ModuleId& top_module,
                                                                            const ConfigRegionId& config_region);

/**
 * @brief Find the size of BL or WL ports for a given module
 */
size_t find_module_ql_memory_bank_num_blwls(const ModuleManager& module_manager,
                                            const ModuleId& module_id,
                                            const CircuitLibrary& circuit_lib,
                                            const CircuitModelId& sram_model,
                                            const e_config_protocol_type& sram_orgz_type,
										    const e_circuit_model_port_type& circuit_port_type);
/**
 * @brief Precompute the total number of bit lines required by a specific configuration region
 */
size_t compute_memory_bank_regional_num_bls(const ModuleManager& module_manager,
                                            const ModuleId& top_module,
                                            const ConfigRegionId& config_region,
                                            const CircuitLibrary& circuit_lib,
                                            const CircuitModelId& sram_model);

/**
 * @brief Precompute the number of bit lines required by each tile under a specific configuration region
 * @note 
 *   Not every index in the range computed by the compute_memory_bank_regional_configurable_child_x_range() function has a postive number of bit lines
 *   If an empty entry is found (e.g., std::map::find(x) is empty), it means there are not bit lines required in that tile
 */
std::map<int, size_t> compute_memory_bank_regional_bitline_numbers_per_tile(const ModuleManager& module_manager,
                                                                            const ModuleId& top_module,
                                                                            const ConfigRegionId& config_region,
                                                                            const CircuitLibrary& circuit_lib,
                                                                            const CircuitModelId& sram_model);
/**
 * @brief Precompute the total number of word lines required by a specific configuration region
 */
size_t compute_memory_bank_regional_num_wls(const ModuleManager& module_manager,
                                            const ModuleId& top_module,
                                            const ConfigRegionId& config_region,
                                            const CircuitLibrary& circuit_lib,
                                            const CircuitModelId& sram_model);

/**
 * @brief Precompute the number of word lines required by each tile under a specific configuration region
 * @note 
 *   Not every index in the range computed by the compute_memory_bank_regional_configurable_child_y_range() function has a postive number of word lines
 *   If an empty entry is found (e.g., std::map::find(y) is empty), it means there are not word lines required in that tile
 * @note
 *   This function requires an input argument which describes number of bitlines per tile. Base on the information, the number of word lines are inferred
 *   by total number of memores / number of bit lines at a given tile location 
 *   This strategy is chosen because in each column, the number of bit lines are bounded by the tile which consumes most configuation bits. It may reduces
 *   the use of word lines. For example, a tile[0][0] has only 8 bits, from which we may infer 3 BLs and 3 WLs. However, when tile[0][1] contains 100 bits, 
 *   which will force the number of BLs to be 10. In such case, tile[0][0] only requires 1 WL
 */
std::map<int, size_t> compute_memory_bank_regional_wordline_numbers_per_tile(const ModuleManager& module_manager,
                                                                             const ModuleId& top_module,
                                                                             const ConfigRegionId& config_region,
                                                                             const CircuitLibrary& circuit_lib,
                                                                             const CircuitModelId& sram_model);

/**
 * @brief Precompute the BLs and WLs distribution across the FPGA fabric
 * The distribution is a matrix which contains the starting index of BL/WL for each column or row
 */
std::map<int, size_t> compute_memory_bank_regional_blwl_start_index_per_tile(const std::pair<int, int>& child_xy_range,
                                                                             const std::map<int, size_t>& num_blwls_per_tile);

} /* end namespace openfpga */

#endif
