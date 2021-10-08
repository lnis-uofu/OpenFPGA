#include <algorithm>
#include "vtr_assert.h"
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

void MemoryBankShiftRegisterBanks::reserve_wl_shift_register_banks(const ConfigRegionId& region_id, const size_t& num_banks) {
  VTR_ASSERT(valid_region_id(region_id));
  wl_bank_ids_[region_id].reserve(num_banks);
  wl_bank_data_ports_[region_id].reserve(num_banks);
}

FabricBitLineBankId MemoryBankShiftRegisterBanks::create_bl_shift_register_bank(const ConfigRegionId& region_id) {
  VTR_ASSERT(valid_region_id(region_id));
  
  /* Create a new id */
  FabricBitLineBankId bank = FabricBitLineBankId(bl_bank_ids_[region_id].size());
  bl_bank_ids_[region_id].push_back(bank);
  bl_bank_data_ports_[region_id].emplace_back();

  return bank;
}

void MemoryBankShiftRegisterBanks::add_data_port_to_bl_shift_register_bank(const ConfigRegionId& region_id,
                                                        const FabricBitLineBankId& bank_id,
                                                        const openfpga::BasicPort& data_port) {
  VTR_ASSERT(valid_bl_bank_id(region_id, bank_id));
  bl_bank_data_ports_[region_id][bank_id].push_back(data_port);
}

FabricWordLineBankId MemoryBankShiftRegisterBanks::create_wl_shift_register_bank(const ConfigRegionId& region_id) {
  VTR_ASSERT(valid_region_id(region_id));
  
  /* Create a new id */
  FabricWordLineBankId bank = FabricWordLineBankId(wl_bank_ids_[region_id].size());
  wl_bank_ids_[region_id].push_back(bank);
  wl_bank_data_ports_[region_id].emplace_back();

  return bank;
}

void MemoryBankShiftRegisterBanks::add_data_port_to_wl_shift_register_bank(const ConfigRegionId& region_id,
                                                        const FabricWordLineBankId& bank_id,
                                                        const openfpga::BasicPort& data_port) {
  VTR_ASSERT(valid_wl_bank_id(region_id, bank_id));
  wl_bank_data_ports_[region_id][bank_id].push_back(data_port);
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

} /* end namespace openfpga */
