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
 * @brief Precompute the number of word lines required by each tile under a specific configuration region
 * @note 
 *   Not every index in the range computed by the compute_memory_bank_regional_configurable_child_x_range() function has a postive number of word lines
 *   If an empty entry is found (e.g., std::map::find(y) is empty), it means there are not word lines required in that tile
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
