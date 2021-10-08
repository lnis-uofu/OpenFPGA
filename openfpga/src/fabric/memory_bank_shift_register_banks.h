#ifndef MEMORY_BANK_SHIFT_REGISTER_BANKS_H
#define MEMORY_BANK_SHIFT_REGISTER_BANKS_H

#include <map>
#include <vector>
#include "vtr_vector.h"
#include "fabric_key.h"
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
  public: /* Accessors: aggregates */
    FabricKey::fabric_bit_line_bank_range bl_banks(const ConfigRegionId& region_id) const;
    FabricKey::fabric_word_line_bank_range wl_banks(const ConfigRegionId& region_id) const;
  public: /* Accessors */
    /* @brief Return a list of unique sizes of shift register banks for BL protocol */
    std::vector<size_t> bl_bank_unique_sizes(const ConfigRegionId& region_id) const;

    /* @brief Return the size of a BL shift register bank */
    size_t bl_bank_size(const ConfigRegionId& region_id, const FabricBitLineBankId& bank_id) const;

    /* @brief Return a list of data ports which will be driven by a BL shift register bank */
    std::vector<BasicPort> bl_bank_data_ports(const ConfigRegionId& region_id, const FabricBitLineBankId& bank_id) const;

    /* @brief Return a list of unique sizes of shift register banks for WL protocol */
    std::vector<size_t> wl_bank_unique_sizes(const ConfigRegionId& region_id) const;

    /* @brief Return the size of a WL shift register bank */
    size_t wl_bank_size(const ConfigRegionId& region_id, const FabricWordLineBankId& bank_id) const;

    /* @brief Return a list of data ports which will be driven by a WL shift register bank */
    std::vector<BasicPort> wl_bank_data_ports(const ConfigRegionId& region_id, const FabricWordLineBankId& bank_id) const;

    /* @brief Return a list of modules of unique shift register banks across all the regions */
    std::vector<ModuleId> bl_shift_register_bank_unique_modules() const; 

    /* @brief Return a list of modules of shift register banks under a specific configuration region of top-level module */
    std::vector<ModuleId> bl_shift_register_bank_modules(const ConfigRegionId& region) const; 

    /* @brief Return a list of instances of shift register banks under a specific configuration region of top-level module */
    std::vector<size_t> bl_shift_register_bank_instances(const ConfigRegionId& region) const; 

    /* @brief Return a list of ids of reconfigurable children for a given instance of shift register bank 
     * under a specific configuration region of top-level module
     */
    std::vector<size_t> bl_shift_register_bank_sink_child_ids(const ConfigRegionId& region,
                                                              const ModuleId& sr_module,
                                                              const size_t& sr_instance) const; 

    /* @brief Return a list of BL ids of reconfigurable children for a given instance of shift register bank 
     * under a specific configuration region of top-level module
     */
    std::vector<size_t> bl_shift_register_bank_sink_pin_ids(const ConfigRegionId& region,
                                                            const ModuleId& sr_module,
                                                            const size_t& sr_instance) const; 

    /* @brief Return a list of BL ids of a given instance of shift register bank 
     * under a specific configuration region of top-level module
     */
    std::vector<size_t> bl_shift_register_bank_source_blwl_ids(const ConfigRegionId& region,
                                                               const ModuleId& sr_module,
                                                               const size_t& sr_instance) const; 

    /* @brief Return a list of modules of unique shift register banks across all the regions */
    std::vector<ModuleId> wl_shift_register_bank_unique_modules() const; 

    /* @brief Return a list of modules of shift register banks under a specific configuration region of top-level module */
    std::vector<ModuleId> wl_shift_register_bank_modules(const ConfigRegionId& region) const; 

    /* @brief Return a list of instances of shift register banks under a specific configuration region of top-level module */
    std::vector<size_t> wl_shift_register_bank_instances(const ConfigRegionId& region) const; 

    /* @brief Return a list of ids of reconfigurable children for a given instance of shift register bank 
     * under a specific configuration region of top-level module
     */
    std::vector<size_t> wl_shift_register_bank_sink_child_ids(const ConfigRegionId& region,
                                                              const ModuleId& sr_module,
                                                              const size_t& sr_instance) const; 

    /* @brief Return a list of WL ids of reconfigurable children for a given instance of shift register bank 
     * under a specific configuration region of top-level module
     */
    std::vector<size_t> wl_shift_register_bank_sink_pin_ids(const ConfigRegionId& region,
                                                            const ModuleId& sr_module,
                                                            const size_t& sr_instance) const; 

    /* @brief Return a list of WL ids of a given instance of shift register bank 
     * under a specific configuration region of top-level module
     */
    std::vector<size_t> wl_shift_register_bank_source_blwl_ids(const ConfigRegionId& region,
                                                               const ModuleId& sr_module,
                                                               const size_t& sr_instance) const; 

  public: /* Mutators */
    void resize_regions(const size_t& num_regions);

    /* Reserve a number of banks to be memory efficent */
    void reserve_bl_shift_register_banks(const ConfigRegionId& region_id, const size_t& num_banks);
    void reserve_wl_shift_register_banks(const ConfigRegionId& region_id, const size_t& num_banks);

    /* Create a new shift register bank for BLs and return an id */
    FabricBitLineBankId create_bl_shift_register_bank(const ConfigRegionId& region_id);

    /* Add a data port to a given BL shift register bank */
    void add_data_port_to_bl_shift_register_bank(const ConfigRegionId& region_id,
                                                 const FabricBitLineBankId& bank_id,
                                                 const openfpga::BasicPort& data_port);

    /* Create a new shift register bank for WLs and return an id */
    FabricWordLineBankId create_wl_shift_register_bank(const ConfigRegionId& region_id);

    /* Add a data port to a given WL shift register bank */
    void add_data_port_to_wl_shift_register_bank(const ConfigRegionId& region_id,
                                                 const FabricWordLineBankId& bank_id,
                                                 const openfpga::BasicPort& data_port);

    /* @brief Add the module id and instance id of a shift register under a specific configuration region of top-level module */
    void add_bl_shift_register_instance(const ConfigRegionId& region,
                                        const ModuleId& sr_module,
                                        const size_t& sr_instance); 

    /* @brief Add the child id and pin id of BL to which a shift register is connected to under a specific configuration region of top-level module */
    void add_bl_shift_register_sink_nodes(const ConfigRegionId& region,
                                          const ModuleId& sr_module,
                                          const size_t& sr_instance,
                                          const size_t& sink_child_id,
                                          const size_t& sink_child_pin_id); 

    /* @brief Add the BL id under a specific configuration region of top-level module to which a shift register is connected to */
    void add_bl_shift_register_source_blwls(const ConfigRegionId& region,
                                            const ModuleId& sr_module,
                                            const size_t& sr_instance,
                                            const size_t& sink_blwl_id); 

    /* @brief Add the module id and instance id of a shift register under a specific configuration region of top-level module */
    void add_wl_shift_register_instance(const ConfigRegionId& region,
                                        const ModuleId& sr_module,
                                        const size_t& sr_instance); 

    /* @brief Add the child id and pin id of WL to which a shift register is connected to under a specific configuration region of top-level module */
    void add_wl_shift_register_sink_nodes(const ConfigRegionId& region,
                                          const ModuleId& sr_module,
                                          const size_t& sr_instance,
                                          const size_t& sink_child_id,
                                          const size_t& sink_child_pin_id); 

    /* @brief Add the BL/WL id under a specific configuration region of top-level module to which a shift register is connected to */
    void add_wl_shift_register_source_blwls(const ConfigRegionId& region,
                                            const ModuleId& sr_module,
                                            const size_t& sr_instance,
                                            const size_t& sink_blwl_id); 

  public:  /* Validators */
    bool valid_region_id(const ConfigRegionId& region) const;
    bool valid_bl_bank_id(const ConfigRegionId& region_id, const FabricBitLineBankId& bank_id) const;
    bool valid_wl_bank_id(const ConfigRegionId& region_id, const FabricWordLineBankId& bank_id) const;

  private: /* Internal data */
    /* General information about the BL shift register bank */
    vtr::vector<ConfigRegionId, vtr::vector<FabricBitLineBankId, FabricBitLineBankId>> bl_bank_ids_;
    vtr::vector<ConfigRegionId, vtr::vector<FabricBitLineBankId, std::vector<BasicPort>>> bl_bank_data_ports_;

    /* BL: [config_region][(shift_register_module, shift_register_instance)][i] = (reconfigurable_child_id, blwl_port_pin_index)*/
    vtr::vector<ConfigRegionId, std::map<std::pair<ModuleId, size_t>, std::vector<size_t>>> bl_sr_instance_sink_child_ids_;
    vtr::vector<ConfigRegionId, std::map<std::pair<ModuleId, size_t>, std::vector<size_t>>> bl_sr_instance_sink_child_pin_ids_;
    vtr::vector<ConfigRegionId, std::map<std::pair<ModuleId, size_t>, std::vector<size_t>>> bl_sr_instance_source_blwl_ids_;

    /* General information about the WL shift register bank */
    vtr::vector<ConfigRegionId, vtr::vector<FabricWordLineBankId, FabricWordLineBankId>> wl_bank_ids_;
    vtr::vector<ConfigRegionId, vtr::vector<FabricWordLineBankId, std::vector<BasicPort>>> wl_bank_data_ports_;

    /* WL: [config_region][(shift_register_module, shift_register_instance)][i] = (reconfigurable_child_id, blwl_port_pin_index)*/
    vtr::vector<ConfigRegionId, std::map<std::pair<ModuleId, size_t>, std::vector<size_t>>> wl_sr_instance_sink_child_ids_;
    vtr::vector<ConfigRegionId, std::map<std::pair<ModuleId, size_t>, std::vector<size_t>>> wl_sr_instance_sink_child_pin_ids_;
    vtr::vector<ConfigRegionId, std::map<std::pair<ModuleId, size_t>, std::vector<size_t>>> wl_sr_instance_source_blwl_ids_;

};

} /* end namespace openfpga */

#endif
