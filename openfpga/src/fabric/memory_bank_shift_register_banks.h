#ifndef MEMORY_BANK_SHIFT_REGISTER_BANKS_H
#define MEMORY_BANK_SHIFT_REGISTER_BANKS_H

#include <map>
#include <vector>
#include "vtr_vector.h"
#include "module_manager.h"

/* begin namespace openfpga */
namespace openfpga {

/******************************************************************************
 * This files includes data structures that stores detailed information about 
 * shift register banks for each configuration region in the top-level module, including
 * - Module id of each shift register bank
 * - Instance id of each shift register bank 
 * - The connectivity of of each shift register bank to reconfigurable child under a configuration region
 *   - The ids of each child to be connected 
 *   - The Bit Line (BL) or Word Line (WL) pin id of each child

 * @note This data structure is mainly used as a database for adding connections around shift register banks in top-level module
 ******************************************************************************/
class MemoryBankShiftRegisterBanks {
  public: /* Accessors */
    /* @brief Return a list of modules of unique shift register banks across all the regions */
    std::vector<ModuleId> shift_register_bank_unique_modules() const; 

    /* @brief Return a list of modules of shift register banks under a specific configuration region of top-level module */
    std::vector<ModuleId> shift_register_bank_modules(const ConfigRegionId& region) const; 

    /* @brief Return a list of instances of shift register banks under a specific configuration region of top-level module */
    std::vector<size_t> shift_register_bank_instances(const ConfigRegionId& region) const; 

    /* @brief Return a list of ids of reconfigurable children for a given instance of shift register bank 
     * under a specific configuration region of top-level module
     */
    std::vector<size_t> shift_register_bank_sink_child_ids(const ConfigRegionId& region,
                                                           const ModuleId& sr_module,
                                                           const size_t& sr_instance) const; 

    /* @brief Return a list of BL/WL ids of reconfigurable children for a given instance of shift register bank 
     * under a specific configuration region of top-level module
     */
    std::vector<size_t> shift_register_bank_sink_pin_ids(const ConfigRegionId& region,
                                                         const ModuleId& sr_module,
                                                         const size_t& sr_instance) const; 
  public: /* Mutators */
    void resize_regions(const size_t& num_regions);

    /* @brief Add the module id and instance id of a shift register under a specific configuration region of top-level module */
    void add_shift_register_instance(const ConfigRegionId& region,
                                     const ModuleId& sr_module,
                                     const size_t& sr_instance); 

    /* @brief Add the module id and instance id of a shift register under a specific configuration region of top-level module */
    void add_shift_register_sink_nodes(const ConfigRegionId& region,
                                       const ModuleId& sr_module,
                                       const size_t& sr_instance,
                                       const size_t& sink_child_id,
                                       const size_t& sink_child_pin_id); 
  public:  /* Validators */
    bool valid_region_id(const ConfigRegionId& region) const;

  private: /* Internal data */
    /* [config_region][(shift_register_module, shift_register_instance)][i] = (reconfigurable_child_id, blwl_port_pin_index)*/
    vtr::vector<ConfigRegionId, std::map<std::pair<ModuleId, size_t>, std::vector<std::pair<size_t, size_t>>>> sr_instance_info_;
};

} /* end namespace openfpga */

#endif
