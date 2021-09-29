#include "vtr_assert.h"
#include "memory_bank_shift_register_banks.h" 

/* begin namespace openfpga */
namespace openfpga {

std::vector<ModuleId> MemoryBankShiftRegisterBanks::shift_register_bank_modules(const ConfigRegionId& region) const {
  std::vector<ModuleId> sr_bank_modules;
  VTR_ASSERT(valid_region_id(region));
  for (const auto& pair : sr_instance_info_[region]) {
    sr_bank_modules.push_back(pair.first.first);
  }
  return sr_bank_modules;
} 

std::vector<size_t> MemoryBankShiftRegisterBanks::shift_register_bank_instances(const ConfigRegionId& region) const {
  std::vector<size_t> sr_bank_instances;
  VTR_ASSERT(valid_region_id(region));
  for (const auto& pair : sr_instance_info_[region]) {
    sr_bank_instances.push_back(pair.first.second);
  }
  return sr_bank_instances;
} 

std::vector<size_t> MemoryBankShiftRegisterBanks::shift_register_bank_sink_child_ids(const ConfigRegionId& region,
                                                                                     const ModuleId& sr_module,
                                                                                     const size_t& sr_instance) const {
  VTR_ASSERT(valid_region_id(region));
  std::vector<size_t> sink_child_ids;

  auto result = sr_instance_info_[region].find(std::make_pair(sr_module, sr_instance));
  /* Return an empty vector if not found */
  if (result != sr_instance_info_[region].end()) {
    for (const auto& sink_child : result->second) {
      sink_child_ids.push_back(sink_child.first);
    }
  }
  return sink_child_ids;
} 

std::vector<size_t> MemoryBankShiftRegisterBanks::shift_register_bank_sink_pin_ids(const ConfigRegionId& region,
                                                                                   const ModuleId& sr_module,
                                                                                   const size_t& sr_instance) const {
  VTR_ASSERT(valid_region_id(region));
  std::vector<size_t> sink_child_pin_ids;

  auto result = sr_instance_info_[region].find(std::make_pair(sr_module, sr_instance));
  /* Return an empty vector if not found */
  if (result != sr_instance_info_[region].end()) {
    for (const auto& sink_child : result->second) {
      sink_child_pin_ids.push_back(sink_child.second);
    }
  }
  return sink_child_pin_ids;
}

void MemoryBankShiftRegisterBanks::resize_regions(const size_t& num_regions) {
  sr_instance_info_.resize(num_regions);
}

void MemoryBankShiftRegisterBanks::add_shift_register_instance(const ConfigRegionId& region,
                                                               const ModuleId& sr_module,
                                                               const size_t& sr_instance) {
  VTR_ASSERT(valid_region_id(region));
  sr_instance_info_[region][std::make_pair(sr_module, sr_instance)]; 
}

void MemoryBankShiftRegisterBanks::add_shift_register_sink_nodes(const ConfigRegionId& region,
                                                                 const ModuleId& sr_module,
                                                                 const size_t& sr_instance,
                                                                 const size_t& sink_child_id,
                                                                 const size_t& sink_child_pin_id) { 
  VTR_ASSERT(valid_region_id(region));
  sr_instance_info_[region][std::make_pair(sr_module, sr_instance)].push_back(std::make_pair(sink_child_id, sink_child_pin_id)); 
}

bool MemoryBankShiftRegisterBanks::valid_region_id(const ConfigRegionId& region) const {
  return size_t(region) < sr_instance_info_.size();
}

} /* end namespace openfpga */
