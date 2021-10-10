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
    ModuleManager::region_range regions() const;
    FabricKey::fabric_bit_line_bank_range bl_banks(const ConfigRegionId& region_id) const;
    FabricKey::fabric_word_line_bank_range wl_banks(const ConfigRegionId& region_id) const;
  public: /* Accessors */
    /* @brief Return a list of unique sizes of shift register banks for BL protocol */
    std::vector<size_t> bl_bank_unique_sizes() const;

    /* @brief Return a list of unique modules of shift register banks for BL protocol */
    std::vector<ModuleId> bl_bank_unique_modules() const;

    /* @brief Return the size of a BL shift register bank */
    size_t bl_bank_size(const ConfigRegionId& region_id, const FabricBitLineBankId& bank_id) const;

    /* @brief Return a list of data ports which will be driven by a BL shift register bank */
    std::vector<BasicPort> bl_bank_data_ports(const ConfigRegionId& region_id, const FabricBitLineBankId& bank_id) const;

    /** @brief find the BL shift register bank id to which a BL port is connected to */
    FabricBitLineBankId find_bl_shift_register_bank_id(const ConfigRegionId& region, const BasicPort& bl_port) const;

    /** @brief find the data port of a BL shift register bank id to which a BL port is connected to */
    BasicPort find_bl_shift_register_bank_data_port(const ConfigRegionId& region, const BasicPort& bl_port) const;

    /** @brief Return the module id of a BL shift register bank */
    ModuleId bl_shift_register_bank_module(const ConfigRegionId& region_id,
                                           const FabricBitLineBankId& bank_id) const;

    /** @brief Return the instance id of a BL shift register bank */
    size_t bl_shift_register_bank_instance(const ConfigRegionId& region_id,
                                           const FabricBitLineBankId& bank_id) const;

    /** @brief return the child ids at top-level module to which a data port (1-bit) of a BL shift register bank is connected to 
     *  @note a BL may drive multiple children (children on the same column share the same BLs)
     */
    std::vector<size_t> bl_shift_register_bank_sink_child_ids(const ConfigRegionId& region_id,
                                                              const FabricBitLineBankId& bank_id,
                                                              const BasicPort& src_port) const;

    /** @brief return the child pin id of the child module at top-level module 
     *  to which a data port (1-bit) of a BL shift register bank is connected to
     *  @note a BL may drive multiple children (children on the same column share the same BLs)
     */
    std::vector<size_t> bl_shift_register_bank_sink_child_pin_ids(const ConfigRegionId& region_id,
                                                                  const FabricBitLineBankId& bank_id,
                                                                  const BasicPort& src_port) const;

    /** @brief Return a list of single-bit ports which are the data ports of a BL shift register bank */
    std::vector<BasicPort> bl_shift_register_bank_source_ports(const ConfigRegionId& region_id,
                                                               const FabricBitLineBankId& bank_id) const;

    /* @brief Return a list of unique sizes of shift register banks for WL protocol */
    std::vector<size_t> wl_bank_unique_sizes() const;

    /* @brief Return a list of unique modules of shift register banks for WL protocol */
    std::vector<ModuleId> wl_bank_unique_modules() const;

    /* @brief Return the size of a WL shift register bank */
    size_t wl_bank_size(const ConfigRegionId& region_id, const FabricWordLineBankId& bank_id) const;

    /* @brief Return a list of data ports which will be driven by a WL shift register bank */
    std::vector<BasicPort> wl_bank_data_ports(const ConfigRegionId& region_id, const FabricWordLineBankId& bank_id) const;

    /** @brief find the WL shift register bank id to which a BL port is connected to */
    FabricWordLineBankId find_wl_shift_register_bank_id(const ConfigRegionId& region, const BasicPort& wl_port) const;

    /** @brief find the data port of a WL shift register bank id to which a BL port is connected to */
    BasicPort find_wl_shift_register_bank_data_port(const ConfigRegionId& region, const BasicPort& wl_port) const;

    /** @brief Return the module id of a WL shift register bank */
    ModuleId wl_shift_register_bank_module(const ConfigRegionId& region_id,
                                           const FabricWordLineBankId& bank_id) const;

    /** @brief Return the instance id of a WL shift register bank */
    size_t wl_shift_register_bank_instance(const ConfigRegionId& region_id,
                                           const FabricWordLineBankId& bank_id) const;

    /** @brief return the child id at top-level module to which a data port (1-bit) of a WL shift register bank is connected to
     *  @note a WL may drive multiple children (children on the same row share the same WLs)
     */
    std::vector<size_t> wl_shift_register_bank_sink_child_ids(const ConfigRegionId& region,
                                                              const FabricWordLineBankId& bank_id,
                                                              const BasicPort& src_port) const;

    /** @brief return the child pin id of the child module at top-level module 
     *  to which a data port (1-bit) of a WL shift register bank is connected to
     *  @note a WL may drive multiple children (children on the same row share the same WLs)
     */
    std::vector<size_t> wl_shift_register_bank_sink_child_pin_ids(const ConfigRegionId& region,
                                                                  const FabricWordLineBankId& bank_id,
                                                                  const BasicPort& src_port) const;

    /** @brief Return a list of single-bit ports which are the data ports of a WL shift register bank */
    std::vector<BasicPort> wl_shift_register_bank_source_ports(const ConfigRegionId& region_id,
                                                               const FabricWordLineBankId& bank_id) const;

  public: /* Mutators */
    void resize_regions(const size_t& num_regions);

    /* Reserve a number of banks to be memory efficent */
    void reserve_bl_shift_register_banks(const FabricRegionId& region_id, const size_t& num_banks);
    void reserve_bl_shift_register_banks(const ConfigRegionId& region_id, const size_t& num_banks);

    /* Create a new shift register bank for BLs and return an id */
    FabricBitLineBankId create_bl_shift_register_bank(const FabricRegionId& region_id);
    FabricBitLineBankId create_bl_shift_register_bank(const ConfigRegionId& region_id);

    /* Add a data port to a given BL shift register bank */
    void add_data_port_to_bl_shift_register_bank(const FabricRegionId& region_id,
                                                 const FabricBitLineBankId& bank_id,
                                                 const openfpga::BasicPort& data_port);
    void add_data_port_to_bl_shift_register_bank(const ConfigRegionId& region_id,
                                                 const FabricBitLineBankId& bank_id,
                                                 const openfpga::BasicPort& data_port);

    /* Link a BL shift register bank to a module id */
    void link_bl_shift_register_bank_to_module(const ConfigRegionId& region_id,
                                               const FabricBitLineBankId& bank_id,
                                               const ModuleId& module_id);

    /* Link a BL shift register bank to a instance id */
    void link_bl_shift_register_bank_to_instance(const ConfigRegionId& region_id,
                                                 const FabricBitLineBankId& bank_id,
                                                 const size_t& instance_id);

    /* @brief Add the child id and pin id of BL to which a shift register is connected to under a specific configuration region of top-level module */
    void add_bl_shift_register_bank_sink_node(const ConfigRegionId& region,
                                              const FabricBitLineBankId& bank,
                                              const BasicPort& src_port,
                                              const size_t& sink_child_id,
                                              const size_t& sink_child_pin_id); 

    /* Reserve a number of banks to be memory efficent */
    void reserve_wl_shift_register_banks(const FabricRegionId& region_id, const size_t& num_banks);
    void reserve_wl_shift_register_banks(const ConfigRegionId& region_id, const size_t& num_banks);

    /* Create a new shift register bank for WLs and return an id */
    FabricWordLineBankId create_wl_shift_register_bank(const FabricRegionId& region_id);
    FabricWordLineBankId create_wl_shift_register_bank(const ConfigRegionId& region_id);

    /* Add a data port to a given WL shift register bank */
    void add_data_port_to_wl_shift_register_bank(const FabricRegionId& region_id,
                                                 const FabricWordLineBankId& bank_id,
                                                 const openfpga::BasicPort& data_port);
    void add_data_port_to_wl_shift_register_bank(const ConfigRegionId& region_id,
                                                 const FabricWordLineBankId& bank_id,
                                                 const openfpga::BasicPort& data_port);

    /* Link a WL shift register bank to a module id */
    void link_wl_shift_register_bank_to_module(const ConfigRegionId& region_id,
                                               const FabricWordLineBankId& bank_id,
                                               const ModuleId& module_id);

    /* Link a WL shift register bank to a instance id */
    void link_wl_shift_register_bank_to_instance(const ConfigRegionId& region_id,
                                                 const FabricWordLineBankId& bank_id,
                                                 const size_t& instance_id);

    /* @brief Add the child id and pin id of WL to which a shift register is connected to under a specific configuration region of top-level module */
    void add_wl_shift_register_bank_sink_node(const ConfigRegionId& region,
                                              const FabricWordLineBankId& bank,
                                              const BasicPort& src_port,
                                              const size_t& sink_child_id,
                                              const size_t& sink_child_pin_id); 

  public:  /* Validators */
    bool valid_region_id(const ConfigRegionId& region) const;
    bool valid_bl_bank_id(const ConfigRegionId& region_id, const FabricBitLineBankId& bank_id) const;
    bool valid_wl_bank_id(const ConfigRegionId& region_id, const FabricWordLineBankId& bank_id) const;
    bool empty() const;

  private: /* Internal Mutators */
    /** @brief Build the mapping from a BL/WL port to shift register bank and assoicated pins 
     *  @note we use const here because the caller functions, e.g., find_bl_shift_register_bank_id(), is const
     *        even though it does modify internal data
     */
    void build_bl_port_fast_lookup() const;
    void build_wl_port_fast_lookup() const;

  private: /* Internal data */
    vtr::vector<ConfigRegionId, ConfigRegionId> config_region_ids_;

    /* General information about the BL shift register bank */
    vtr::vector<ConfigRegionId, vtr::vector<FabricBitLineBankId, FabricBitLineBankId>> bl_bank_ids_;
    vtr::vector<ConfigRegionId, vtr::vector<FabricBitLineBankId, std::vector<BasicPort>>> bl_bank_data_ports_;
    vtr::vector<ConfigRegionId, vtr::vector<FabricBitLineBankId, ModuleId>> bl_bank_modules_;
    vtr::vector<ConfigRegionId, vtr::vector<FabricBitLineBankId, size_t>> bl_bank_instances_;
    vtr::vector<ConfigRegionId, vtr::vector<FabricBitLineBankId, std::map<BasicPort, std::vector<size_t>>>> bl_bank_sink_child_ids_;
    vtr::vector<ConfigRegionId, vtr::vector<FabricBitLineBankId, std::map<BasicPort, std::vector<size_t>>>> bl_bank_sink_child_pin_ids_;

    /* General information about the WL shift register bank */
    vtr::vector<ConfigRegionId, vtr::vector<FabricWordLineBankId, FabricWordLineBankId>> wl_bank_ids_;
    vtr::vector<ConfigRegionId, vtr::vector<FabricWordLineBankId, std::vector<BasicPort>>> wl_bank_data_ports_;
    vtr::vector<ConfigRegionId, vtr::vector<FabricWordLineBankId, ModuleId>> wl_bank_modules_;
    vtr::vector<ConfigRegionId, vtr::vector<FabricWordLineBankId, size_t>> wl_bank_instances_;
    vtr::vector<ConfigRegionId, vtr::vector<FabricWordLineBankId, std::map<BasicPort, std::vector<size_t>>>> wl_bank_sink_child_ids_;
    vtr::vector<ConfigRegionId, vtr::vector<FabricWordLineBankId, std::map<BasicPort, std::vector<size_t>>>> wl_bank_sink_child_pin_ids_;

    /* Fast look-up: given a BL/Wl port, e.g., bl[i], find out
     * - the shift register bank id
     * - the output pin id of the shift register bank
     */
    mutable vtr::vector<ConfigRegionId, std::map<BasicPort, FabricBitLineBankId>> bl_ports_to_sr_bank_ids_;
    mutable vtr::vector<ConfigRegionId, std::map<BasicPort, BasicPort>> bl_ports_to_sr_bank_ports_;
    mutable vtr::vector<ConfigRegionId, std::map<BasicPort, FabricWordLineBankId>> wl_ports_to_sr_bank_ids_;
    mutable vtr::vector<ConfigRegionId, std::map<BasicPort, BasicPort>> wl_ports_to_sr_bank_ports_;

    /* A flag to indicate that the general information of the shift register banks have been modified, fast look-up has to be updated */
    mutable bool is_bl_bank_dirty_ = false;
    mutable bool is_wl_bank_dirty_ = false;
};

} /* end namespace openfpga */

#endif
