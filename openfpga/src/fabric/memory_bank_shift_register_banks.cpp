#include <algorithm>
#include "vtr_assert.h"
#include "openfpga_reserved_words.h"
#include "memory_bank_shift_register_banks.h" 

/* begin namespace openfpga */
namespace openfpga {

std::vector<size_t> MemoryBankShiftRegisterBanks::bl_bank_unique_sizes(const ConfigRegionId& region_id) const {
  std::vector<size_t> unique_sizes;
  for (const auto& bank_id : bl_banks(region_id)) {
    size_t cur_bank_size = bl_bank_size(region_id, bank_id);
    if (unique_sizes.end() == std::find(unique_sizes.begin(), unique_sizes.end(), cur_bank_size)) {
      unique_sizes.push_back(cur_bank_size);
    }
  }
  return unique_sizes;
}

size_t MemoryBankShiftRegisterBanks::bl_bank_size(const ConfigRegionId& region_id, const FabricBitLineBankId& bank_id) const {
  size_t cur_bank_size = 0;
  for (const auto& data_port : bl_bank_data_ports(region_id, bank_id)) {
    cur_bank_size += data_port.get_width();   
  }
  return cur_bank_size;
}

FabricKey::fabric_bit_line_bank_range MemoryBankShiftRegisterBanks::bl_banks(const ConfigRegionId& region_id) const {
  VTR_ASSERT(valid_region_id(region_id));
  return vtr::make_range(bl_bank_ids_[region_id].begin(), bl_bank_ids_[region_id].end());
}

std::vector<size_t> MemoryBankShiftRegisterBanks::wl_bank_unique_sizes(const ConfigRegionId& region_id) const {
  std::vector<size_t> unique_sizes;
  for (const auto& bank_id : wl_banks(region_id)) {
    size_t cur_bank_size = wl_bank_size(region_id, bank_id);
    if (unique_sizes.end() == std::find(unique_sizes.begin(), unique_sizes.end(), cur_bank_size)) {
      unique_sizes.push_back(cur_bank_size);
    }
  }
  return unique_sizes;
}

size_t MemoryBankShiftRegisterBanks::wl_bank_size(const ConfigRegionId& region_id, const FabricWordLineBankId& bank_id) const {
  size_t cur_bank_size = 0;
  for (const auto& data_port : wl_bank_data_ports(region_id, bank_id)) {
    cur_bank_size += data_port.get_width();   
  }
  return cur_bank_size;
}

FabricKey::fabric_word_line_bank_range MemoryBankShiftRegisterBanks::wl_banks(const ConfigRegionId& region_id) const {
  VTR_ASSERT(valid_region_id(region_id));
  return vtr::make_range(wl_bank_ids_[region_id].begin(), wl_bank_ids_[region_id].end());
}

std::vector<ModuleId> MemoryBankShiftRegisterBanks::bl_shift_register_bank_unique_modules() const {
  std::vector<ModuleId> sr_bank_modules;
  for (const auto& region : bl_sr_instance_sink_child_ids_) {
    for (const auto& pair : region) {
      if (sr_bank_modules.end() == std::find(sr_bank_modules.begin(), sr_bank_modules.end(), pair.first.first)) {
        sr_bank_modules.push_back(pair.first.first);
      }
    }
  }
  return sr_bank_modules;
} 

std::vector<ModuleId> MemoryBankShiftRegisterBanks::bl_shift_register_bank_modules(const ConfigRegionId& region) const {
  std::vector<ModuleId> sr_bank_modules;
  VTR_ASSERT(valid_region_id(region));
  for (const auto& pair : bl_sr_instance_sink_child_ids_[region]) {
    sr_bank_modules.push_back(pair.first.first);
  }
  return sr_bank_modules;
} 

std::vector<size_t> MemoryBankShiftRegisterBanks::bl_shift_register_bank_instances(const ConfigRegionId& region) const {
  std::vector<size_t> sr_bank_instances;
  VTR_ASSERT(valid_region_id(region));
  for (const auto& pair : bl_sr_instance_sink_child_ids_[region]) {
    sr_bank_instances.push_back(pair.first.second);
  }
  return sr_bank_instances;
} 

std::vector<size_t> MemoryBankShiftRegisterBanks::bl_shift_register_bank_sink_child_ids(const ConfigRegionId& region,
                                                                                        const ModuleId& sr_module,
                                                                                        const size_t& sr_instance) const {
  VTR_ASSERT(valid_region_id(region));

  auto result = bl_sr_instance_sink_child_ids_[region].find(std::make_pair(sr_module, sr_instance));
  /* Return an empty vector if not found */
  if (result != bl_sr_instance_sink_child_ids_[region].end()) {
    return result->second;
  }
  return std::vector<size_t>();
} 

std::vector<size_t> MemoryBankShiftRegisterBanks::bl_shift_register_bank_sink_pin_ids(const ConfigRegionId& region,
                                                                                      const ModuleId& sr_module,
                                                                                      const size_t& sr_instance) const {
  VTR_ASSERT(valid_region_id(region));

  auto result = bl_sr_instance_sink_child_pin_ids_[region].find(std::make_pair(sr_module, sr_instance));
  /* Return an empty vector if not found */
  if (result != bl_sr_instance_sink_child_pin_ids_[region].end()) {
    return result->second;
  }
  return std::vector<size_t>();
}

std::vector<size_t> MemoryBankShiftRegisterBanks::bl_shift_register_bank_source_blwl_ids(const ConfigRegionId& region,
                                                                                         const ModuleId& sr_module,
                                                                                         const size_t& sr_instance) const {
  VTR_ASSERT(valid_region_id(region));

  auto result = bl_sr_instance_source_blwl_ids_[region].find(std::make_pair(sr_module, sr_instance));
  /* Return an empty vector if not found */
  if (result != bl_sr_instance_source_blwl_ids_[region].end()) {
    return result->second;
  }
  return std::vector<size_t>();
}

std::vector<ModuleId> MemoryBankShiftRegisterBanks::wl_shift_register_bank_unique_modules() const {
  std::vector<ModuleId> sr_bank_modules;
  for (const auto& region : wl_sr_instance_sink_child_ids_) {
    for (const auto& pair : region) {
      if (sr_bank_modules.end() == std::find(sr_bank_modules.begin(), sr_bank_modules.end(), pair.first.first)) {
        sr_bank_modules.push_back(pair.first.first);
      }
    }
  }
  return sr_bank_modules;
} 

std::vector<ModuleId> MemoryBankShiftRegisterBanks::wl_shift_register_bank_modules(const ConfigRegionId& region) const {
  std::vector<ModuleId> sr_bank_modules;
  VTR_ASSERT(valid_region_id(region));
  for (const auto& pair : wl_sr_instance_sink_child_ids_[region]) {
    sr_bank_modules.push_back(pair.first.first);
  }
  return sr_bank_modules;
} 

std::vector<size_t> MemoryBankShiftRegisterBanks::wl_shift_register_bank_instances(const ConfigRegionId& region) const {
  std::vector<size_t> sr_bank_instances;
  VTR_ASSERT(valid_region_id(region));
  for (const auto& pair : wl_sr_instance_sink_child_ids_[region]) {
    sr_bank_instances.push_back(pair.first.second);
  }
  return sr_bank_instances;
} 

std::vector<size_t> MemoryBankShiftRegisterBanks::wl_shift_register_bank_sink_child_ids(const ConfigRegionId& region,
                                                                                        const ModuleId& sr_module,
                                                                                        const size_t& sr_instance) const {
  VTR_ASSERT(valid_region_id(region));

  auto result = wl_sr_instance_sink_child_ids_[region].find(std::make_pair(sr_module, sr_instance));
  /* Return an empty vector if not found */
  if (result != wl_sr_instance_sink_child_ids_[region].end()) {
    return result->second;
  }
  return std::vector<size_t>();
} 

std::vector<size_t> MemoryBankShiftRegisterBanks::wl_shift_register_bank_sink_pin_ids(const ConfigRegionId& region,
                                                                                      const ModuleId& sr_module,
                                                                                      const size_t& sr_instance) const {
  VTR_ASSERT(valid_region_id(region));

  auto result = wl_sr_instance_sink_child_pin_ids_[region].find(std::make_pair(sr_module, sr_instance));
  /* Return an empty vector if not found */
  if (result != wl_sr_instance_sink_child_pin_ids_[region].end()) {
    return result->second;
  }
  return std::vector<size_t>();
}

std::vector<size_t> MemoryBankShiftRegisterBanks::wl_shift_register_bank_source_blwl_ids(const ConfigRegionId& region,
                                                                                         const ModuleId& sr_module,
                                                                                         const size_t& sr_instance) const {
  VTR_ASSERT(valid_region_id(region));

  auto result = wl_sr_instance_source_blwl_ids_[region].find(std::make_pair(sr_module, sr_instance));
  /* Return an empty vector if not found */
  if (result != wl_sr_instance_source_blwl_ids_[region].end()) {
    return result->second;
  }
  return std::vector<size_t>();
}

std::vector<BasicPort> MemoryBankShiftRegisterBanks::bl_bank_data_ports(const ConfigRegionId& region_id, const FabricBitLineBankId& bank_id) const {
  VTR_ASSERT(valid_bl_bank_id(region_id, bank_id));
  return bl_bank_data_ports_[region_id][bank_id];
}

std::vector<BasicPort> MemoryBankShiftRegisterBanks::wl_bank_data_ports(const ConfigRegionId& region_id, const FabricWordLineBankId& bank_id) const {
  VTR_ASSERT(valid_wl_bank_id(region_id, bank_id));
  return wl_bank_data_ports_[region_id][bank_id];
}

FabricBitLineBankId MemoryBankShiftRegisterBanks::find_bl_shift_register_bank_id(const ConfigRegionId& region,
                                                                                 const BasicPort& bl_port) const {
  if (is_bl_bank_dirty_) {
    build_bl_port_fast_lookup(); 
  }
  
  VTR_ASSERT(valid_region_id(region));
  const auto& result = bl_ports_to_sr_bank_ids_[region].find(bl_port);
  if (result == bl_ports_to_sr_bank_ids_[region].end()) {
    return FabricBitLineBankId::INVALID();
  }
  return result->second;
}

BasicPort MemoryBankShiftRegisterBanks::find_bl_shift_register_bank_data_port(const ConfigRegionId& region,
                                                                              const BasicPort& bl_port) const {
  if (is_bl_bank_dirty_) {
    build_bl_port_fast_lookup(); 
  }
  
  VTR_ASSERT(valid_region_id(region));
  const auto& result = bl_ports_to_sr_bank_ports_[region].find(bl_port);
  if (result == bl_ports_to_sr_bank_ports_[region].end()) {
    return BasicPort();
  }
  return result->second;
}

FabricWordLineBankId MemoryBankShiftRegisterBanks::find_wl_shift_register_bank_id(const ConfigRegionId& region,
                                                                                  const BasicPort& wl_port) const {
  if (is_wl_bank_dirty_) {
    build_wl_port_fast_lookup(); 
  }
  
  VTR_ASSERT(valid_region_id(region));
  const auto& result = wl_ports_to_sr_bank_ids_[region].find(wl_port);
  if (result == wl_ports_to_sr_bank_ids_[region].end()) {
    return FabricWordLineBankId::INVALID();
  }
  return result->second;
}

BasicPort MemoryBankShiftRegisterBanks::find_wl_shift_register_bank_data_port(const ConfigRegionId& region,
                                                                              const BasicPort& wl_port) const {
  if (is_wl_bank_dirty_) {
    build_wl_port_fast_lookup(); 
  }
  
  VTR_ASSERT(valid_region_id(region));
  const auto& result = wl_ports_to_sr_bank_ports_[region].find(wl_port);
  if (result == wl_ports_to_sr_bank_ports_[region].end()) {
    return BasicPort();
  }
  return result->second;
}

void MemoryBankShiftRegisterBanks::resize_regions(const size_t& num_regions) {
  bl_bank_ids_.resize(num_regions);
  bl_bank_data_ports_.resize(num_regions);
  bl_sr_instance_sink_child_ids_.resize(num_regions);
  bl_sr_instance_sink_child_pin_ids_.resize(num_regions);
  bl_sr_instance_source_blwl_ids_.resize(num_regions);

  wl_bank_ids_.resize(num_regions);
  wl_bank_data_ports_.resize(num_regions);
  wl_sr_instance_sink_child_ids_.resize(num_regions);
  wl_sr_instance_sink_child_pin_ids_.resize(num_regions);
  wl_sr_instance_source_blwl_ids_.resize(num_regions);
}

void MemoryBankShiftRegisterBanks::add_bl_shift_register_instance(const ConfigRegionId& region,
                                                                  const ModuleId& sr_module,
                                                                  const size_t& sr_instance) {
  VTR_ASSERT(valid_region_id(region));
  bl_sr_instance_sink_child_ids_[region][std::make_pair(sr_module, sr_instance)]; 
  bl_sr_instance_sink_child_pin_ids_[region][std::make_pair(sr_module, sr_instance)]; 
  bl_sr_instance_source_blwl_ids_[region][std::make_pair(sr_module, sr_instance)]; 
}

void MemoryBankShiftRegisterBanks::add_bl_shift_register_sink_nodes(const ConfigRegionId& region,
                                                                    const ModuleId& sr_module,
                                                                    const size_t& sr_instance,
                                                                    const size_t& sink_child_id,
                                                                    const size_t& sink_child_pin_id) { 
  VTR_ASSERT(valid_region_id(region));
  bl_sr_instance_sink_child_ids_[region][std::make_pair(sr_module, sr_instance)].push_back(sink_child_id); 
  bl_sr_instance_sink_child_pin_ids_[region][std::make_pair(sr_module, sr_instance)].push_back(sink_child_pin_id); 
}

void MemoryBankShiftRegisterBanks::add_bl_shift_register_source_blwls(const ConfigRegionId& region,
                                                                      const ModuleId& sr_module,
                                                                      const size_t& sr_instance,
                                                                      const size_t& sink_blwl_id) {
  VTR_ASSERT(valid_region_id(region));
  bl_sr_instance_source_blwl_ids_[region][std::make_pair(sr_module, sr_instance)].push_back(sink_blwl_id); 
} 

void MemoryBankShiftRegisterBanks::add_wl_shift_register_instance(const ConfigRegionId& region,
                                                                  const ModuleId& sr_module,
                                                                  const size_t& sr_instance) {
  VTR_ASSERT(valid_region_id(region));
  wl_sr_instance_sink_child_ids_[region][std::make_pair(sr_module, sr_instance)]; 
  wl_sr_instance_sink_child_pin_ids_[region][std::make_pair(sr_module, sr_instance)]; 
  wl_sr_instance_source_blwl_ids_[region][std::make_pair(sr_module, sr_instance)]; 
}

void MemoryBankShiftRegisterBanks::add_wl_shift_register_sink_nodes(const ConfigRegionId& region,
                                                                    const ModuleId& sr_module,
                                                                    const size_t& sr_instance,
                                                                    const size_t& sink_child_id,
                                                                    const size_t& sink_child_pin_id) { 
  VTR_ASSERT(valid_region_id(region));
  wl_sr_instance_sink_child_ids_[region][std::make_pair(sr_module, sr_instance)].push_back(sink_child_id); 
  wl_sr_instance_sink_child_pin_ids_[region][std::make_pair(sr_module, sr_instance)].push_back(sink_child_pin_id); 
}

void MemoryBankShiftRegisterBanks::add_wl_shift_register_source_blwls(const ConfigRegionId& region,
                                                                      const ModuleId& sr_module,
                                                                      const size_t& sr_instance,
                                                                      const size_t& sink_blwl_id) {
  VTR_ASSERT(valid_region_id(region));
  wl_sr_instance_source_blwl_ids_[region][std::make_pair(sr_module, sr_instance)].push_back(sink_blwl_id); 
} 

void MemoryBankShiftRegisterBanks::reserve_bl_shift_register_banks(const ConfigRegionId& region_id, const size_t& num_banks) {
  VTR_ASSERT(valid_region_id(region_id));
  bl_bank_ids_[region_id].reserve(num_banks);
  bl_bank_data_ports_[region_id].reserve(num_banks);
}

void MemoryBankShiftRegisterBanks::reserve_bl_shift_register_banks(const FabricRegionId& region_id, const size_t& num_banks) {
  ConfigRegionId config_region_id = ConfigRegionId(size_t(region_id));
  reserve_bl_shift_register_banks(config_region_id, num_banks);
}

void MemoryBankShiftRegisterBanks::reserve_wl_shift_register_banks(const ConfigRegionId& region_id, const size_t& num_banks) {
  VTR_ASSERT(valid_region_id(region_id));
  wl_bank_ids_[region_id].reserve(num_banks);
  wl_bank_data_ports_[region_id].reserve(num_banks);
}

void MemoryBankShiftRegisterBanks::reserve_wl_shift_register_banks(const FabricRegionId& region_id, const size_t& num_banks) {
  ConfigRegionId config_region_id = ConfigRegionId(size_t(region_id));
  reserve_wl_shift_register_banks(config_region_id, num_banks);
}

FabricBitLineBankId MemoryBankShiftRegisterBanks::create_bl_shift_register_bank(const ConfigRegionId& region_id) {
  VTR_ASSERT(valid_region_id(region_id));
  
  /* Create a new id */
  FabricBitLineBankId bank = FabricBitLineBankId(bl_bank_ids_[region_id].size());
  bl_bank_ids_[region_id].push_back(bank);
  bl_bank_data_ports_[region_id].emplace_back();

  return bank;
}

FabricBitLineBankId MemoryBankShiftRegisterBanks::create_bl_shift_register_bank(const FabricRegionId& region_id) {
  ConfigRegionId config_region_id = ConfigRegionId(size_t(region_id));
  return create_bl_shift_register_bank(config_region_id);
}

void MemoryBankShiftRegisterBanks::add_data_port_to_bl_shift_register_bank(const FabricRegionId& region_id,
                                                                           const FabricBitLineBankId& bank_id,
                                                                           const openfpga::BasicPort& data_port) {
  ConfigRegionId config_region_id = ConfigRegionId(size_t(region_id));
  add_data_port_to_bl_shift_register_bank(config_region_id, bank_id, data_port);
}

void MemoryBankShiftRegisterBanks::add_data_port_to_bl_shift_register_bank(const ConfigRegionId& region_id,
                                                                           const FabricBitLineBankId& bank_id,
                                                                           const openfpga::BasicPort& data_port) {
  VTR_ASSERT(valid_bl_bank_id(region_id, bank_id));
  bl_bank_data_ports_[region_id][bank_id].push_back(data_port);
  is_bl_bank_dirty_ = true;
}

FabricWordLineBankId MemoryBankShiftRegisterBanks::create_wl_shift_register_bank(const FabricRegionId& region_id) {
  ConfigRegionId config_region_id = ConfigRegionId(size_t(region_id));
  return create_wl_shift_register_bank(config_region_id);
}

FabricWordLineBankId MemoryBankShiftRegisterBanks::create_wl_shift_register_bank(const ConfigRegionId& region_id) {
  VTR_ASSERT(valid_region_id(region_id));
  
  /* Create a new id */
  FabricWordLineBankId bank = FabricWordLineBankId(wl_bank_ids_[region_id].size());
  wl_bank_ids_[region_id].push_back(bank);
  wl_bank_data_ports_[region_id].emplace_back();

  return bank;
}

void MemoryBankShiftRegisterBanks::add_data_port_to_wl_shift_register_bank(const FabricRegionId& region_id,
                                                                           const FabricWordLineBankId& bank_id,
                                                                           const openfpga::BasicPort& data_port) {
  ConfigRegionId config_region_id = ConfigRegionId(size_t(region_id));
  add_data_port_to_wl_shift_register_bank(config_region_id, bank_id, data_port);
}

void MemoryBankShiftRegisterBanks::add_data_port_to_wl_shift_register_bank(const ConfigRegionId& region_id,
                                                                           const FabricWordLineBankId& bank_id,
                                                                           const openfpga::BasicPort& data_port) {
  VTR_ASSERT(valid_wl_bank_id(region_id, bank_id));
  wl_bank_data_ports_[region_id][bank_id].push_back(data_port);
  is_wl_bank_dirty_ = true;
}

bool MemoryBankShiftRegisterBanks::valid_region_id(const ConfigRegionId& region) const {
  return size_t(region) < bl_sr_instance_sink_child_ids_.size();
}

bool MemoryBankShiftRegisterBanks::valid_bl_bank_id(const ConfigRegionId& region_id, const FabricBitLineBankId& bank_id) const {
  if (!valid_region_id(region_id)) {
    return false;
  }
  return ( size_t(bank_id) < bl_bank_ids_[region_id].size() ) && ( bank_id == bl_bank_ids_[region_id][bank_id] ); 
}

bool MemoryBankShiftRegisterBanks::valid_wl_bank_id(const ConfigRegionId& region_id, const FabricWordLineBankId& bank_id) const {
  if (!valid_region_id(region_id)) {
    return false;
  }
  return ( size_t(bank_id) < wl_bank_ids_[region_id].size() ) && ( bank_id == wl_bank_ids_[region_id][bank_id] ); 
}

bool MemoryBankShiftRegisterBanks::empty() const {
  return bl_bank_ids_.empty() && wl_bank_ids_.empty(); 
}

void MemoryBankShiftRegisterBanks::build_bl_port_fast_lookup() const {
  bl_ports_to_sr_bank_ids_.resize(bl_bank_data_ports_.size());
  for (const auto& region : bl_bank_data_ports_) {
    size_t bl_index = 0;
    for (const auto& bank : region) { 
      for (const auto& port : bank) {
        for (const auto& pin : port.pins()) {
          BasicPort bl_port(std::string(MEMORY_BL_PORT_NAME), bl_index, bl_index);
          BasicPort sr_bl_port(std::string(MEMORY_BL_PORT_NAME), pin, pin);
          ConfigRegionId region_id = ConfigRegionId(&region - &bl_bank_data_ports_[ConfigRegionId(0)]);
          FabricBitLineBankId bank_id = FabricBitLineBankId(&bank - &region[FabricBitLineBankId(0)]);
          bl_ports_to_sr_bank_ids_[region_id][bl_port] = bank_id; 
          bl_ports_to_sr_bank_ports_[region_id][bl_port] = sr_bl_port; 
          bl_index++;
        }
      }
    }
  }
  /* Clear the flag, now fast look-up is synchronized */
  is_bl_bank_dirty_ = false;
}

void MemoryBankShiftRegisterBanks::build_wl_port_fast_lookup() const {
  wl_ports_to_sr_bank_ids_.resize(wl_bank_data_ports_.size());
  for (const auto& region : wl_bank_data_ports_) {
    size_t wl_index = 0;
    for (const auto& bank : region) { 
      for (const auto& port : bank) {
        for (const auto& pin : port.pins()) {
          BasicPort wl_port(std::string(MEMORY_WL_PORT_NAME), wl_index, wl_index);
          BasicPort sr_wl_port(std::string(MEMORY_WL_PORT_NAME), pin, pin);
          ConfigRegionId region_id = ConfigRegionId(&region - &wl_bank_data_ports_[ConfigRegionId(0)]);
          FabricWordLineBankId bank_id = FabricWordLineBankId(&bank - &region[FabricWordLineBankId(0)]);
          wl_ports_to_sr_bank_ids_[region_id][wl_port] = bank_id; 
          wl_ports_to_sr_bank_ports_[region_id][wl_port] = sr_wl_port; 
          wl_index++;
        }
      }
    }
  }
  /* Clear the flag, now fast look-up is synchronized */
  is_wl_bank_dirty_ = false;
}

} /* end namespace openfpga */
