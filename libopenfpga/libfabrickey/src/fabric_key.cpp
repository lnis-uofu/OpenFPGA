#include <algorithm>

#include "vtr_assert.h"
#include "vtr_log.h"

#include "fabric_key.h"

/************************************************************************
 * Member functions for class FabricKey
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
FabricKey::FabricKey() {
  return;
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
FabricKey::fabric_key_range FabricKey::keys() const {
  return vtr::make_range(key_ids_.begin(), key_ids_.end());
}

FabricKey::fabric_region_range FabricKey::regions() const {
  return vtr::make_range(region_ids_.begin(), region_ids_.end());
}

FabricKey::fabric_bit_line_bank_range FabricKey::bl_banks(const FabricRegionId& region_id) const {
  VTR_ASSERT(valid_region_id(region_id));
  return vtr::make_range(bl_bank_ids_[region_id].begin(), bl_bank_ids_[region_id].end());
}

FabricKey::fabric_word_line_bank_range FabricKey::wl_banks(const FabricRegionId& region_id) const {
  VTR_ASSERT(valid_region_id(region_id));
  return vtr::make_range(wl_bank_ids_[region_id].begin(), wl_bank_ids_[region_id].end());
}

/************************************************************************
 * Public Accessors : Basic data query 
 ***********************************************************************/
std::vector<FabricKeyId> FabricKey::region_keys(const FabricRegionId& region_id) const {
  /* validate the region_id */
  VTR_ASSERT(valid_region_id(region_id));
  return region_key_ids_[region_id]; 
}

std::string FabricKey::key_name(const FabricKeyId& key_id) const {
  /* validate the key_id */
  VTR_ASSERT(valid_key_id(key_id));
  return key_names_[key_id]; 
}

size_t FabricKey::key_value(const FabricKeyId& key_id) const {
  /* validate the key_id */
  VTR_ASSERT(valid_key_id(key_id));
  return key_values_[key_id]; 
}

std::string FabricKey::key_alias(const FabricKeyId& key_id) const {
  /* validate the key_id */
  VTR_ASSERT(valid_key_id(key_id));
  return key_alias_[key_id]; 
}

vtr::Point<int> FabricKey::key_coordinate(const FabricKeyId& key_id) const {
  /* validate the key_id */
  VTR_ASSERT(valid_key_id(key_id));
  return key_coordinates_[key_id]; 
}

bool FabricKey::empty() const {
  return 0 == key_ids_.size();
}

std::vector<openfpga::BasicPort> FabricKey::bl_bank_data_ports(const FabricRegionId& region_id, const FabricBitLineBankId& bank_id) const {
  VTR_ASSERT(valid_bl_bank_id(region_id, bank_id));
  return bl_bank_data_ports_[region_id][bank_id];
}

std::vector<openfpga::BasicPort> FabricKey::wl_bank_data_ports(const FabricRegionId& region_id, const FabricWordLineBankId& bank_id) const {
  VTR_ASSERT(valid_wl_bank_id(region_id, bank_id));
  return wl_bank_data_ports_[region_id][bank_id];
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/

void FabricKey::reserve_regions(const size_t& num_regions) {
  region_ids_.reserve(num_regions);
  region_key_ids_.reserve(num_regions);
}

FabricRegionId FabricKey::create_region() {
  /* Create a new id */
  FabricRegionId region = FabricRegionId(region_ids_.size());
  region_ids_.push_back(region);
  region_key_ids_.emplace_back();
  
  return region;
}

void FabricKey::reserve_region_keys(const FabricRegionId& region_id,
                                    const size_t& num_keys) {
  /* validate the region_id */
  VTR_ASSERT(valid_region_id(region_id));

  region_key_ids_[region_id].reserve(num_keys);
}

void FabricKey::add_key_to_region(const FabricRegionId& region_id,
                                  const FabricKeyId& key_id) {
  /* validate the key_id */
  VTR_ASSERT(valid_key_id(key_id));
  /* validate the region_id */
  VTR_ASSERT(valid_region_id(region_id));

  /* Check if the key is already in the region */
  if (region_key_ids_[region_id].end() != std::find(region_key_ids_[region_id].begin(),
                                                    region_key_ids_[region_id].end(),
                                                    key_id)) {
    VTR_LOG_WARN("Try to add a key '%s' which is already in the region '%lu'!\n",
                 key_name(key_id).c_str(),
                 size_t(region_id));
    VTR_ASSERT(region_id == key_regions_[key_id]);
    return; /* Nothing to do but leave a warning! */
  }

  /* Register the key in the region */
  region_key_ids_[region_id].push_back(key_id);

  /* If the key is already in another region, we will error out */
  if ( (true == valid_region_id(key_regions_[key_id])) 
    && (region_id != key_regions_[key_id])) { 
    VTR_LOG_ERROR("Try to add a key '%s' to region '%lu' but it is already in another region '%lu'!\n",
                 key_name(key_id).c_str(),
                 size_t(key_regions_[key_id]),
                 size_t(region_id));
    exit(1);
  }

  key_regions_[key_id] = region_id;
}

void FabricKey::reserve_keys(const size_t& num_keys) {
  key_ids_.reserve(num_keys);
  key_names_.reserve(num_keys);
  key_values_.reserve(num_keys);
  key_regions_.reserve(num_keys);
  key_alias_.reserve(num_keys);
  key_coordinates_.reserve(num_keys);
}

FabricKeyId FabricKey::create_key() {
  /* Create a new id */
  FabricKeyId key = FabricKeyId(key_ids_.size());
  key_ids_.push_back(key);
  key_names_.emplace_back();
  key_values_.emplace_back();
  key_regions_.emplace_back(FabricRegionId::INVALID());
  key_alias_.emplace_back();
  key_coordinates_.emplace_back(vtr::Point<int>(-1, -1));
  
  return key;
}

void FabricKey::set_key_name(const FabricKeyId& key_id,
                             const std::string& name) {
  /* validate the key_id */
  VTR_ASSERT(valid_key_id(key_id));

  key_names_[key_id] = name;
}

void FabricKey::set_key_value(const FabricKeyId& key_id,
                             const size_t& value) {
  /* validate the key_id */
  VTR_ASSERT(valid_key_id(key_id));

  key_values_[key_id] = value;
}

void FabricKey::set_key_alias(const FabricKeyId& key_id,
                              const std::string& alias) {
  /* validate the key_id */
  VTR_ASSERT(valid_key_id(key_id));

  key_alias_[key_id] = alias;
}

void FabricKey::set_key_coordinate(const FabricKeyId& key_id,
                                   const vtr::Point<int>& coord) {
  /* validate the key_id */
  VTR_ASSERT(valid_key_id(key_id));

  key_coordinates_[key_id] = coord;
}

FabricBitLineBankId FabricKey::create_bl_shift_register_bank(const FabricRegionId& region_id) {
  VTR_ASSERT(valid_region_id(region_id));
  
  /* Create a new id */
  FabricBitLineBankId bank = FabricBitLineBankId(bl_bank_ids_[region_id].size());
  bl_bank_ids_[region_id].push_back(bank);
  bl_bank_data_ports_[region_id].emplace_back();

  return bank;
}

void FabricKey::add_data_port_to_bl_shift_register_bank(const FabricRegionId& region_id,
                                                        const FabricBitLineBankId& bank_id,
                                                        const openfpga::BasicPort& data_port) {
  VTR_ASSERT(valid_bl_bank_id(region_id, bank_id));
  bl_bank_data_ports_[region_id][bank_id].push_back(data_port);
}

FabricWordLineBankId FabricKey::create_wl_shift_register_bank(const FabricRegionId& region_id) {
  VTR_ASSERT(valid_region_id(region_id));
  
  /* Create a new id */
  FabricWordLineBankId bank = FabricWordLineBankId(wl_bank_ids_[region_id].size());
  wl_bank_ids_[region_id].push_back(bank);
  wl_bank_data_ports_[region_id].emplace_back();

  return bank;
}

void FabricKey::add_data_port_to_wl_shift_register_bank(const FabricRegionId& region_id,
                                                        const FabricWordLineBankId& bank_id,
                                                        const openfpga::BasicPort& data_port) {
  VTR_ASSERT(valid_wl_bank_id(region_id, bank_id));
  wl_bank_data_ports_[region_id][bank_id].push_back(data_port);
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool FabricKey::valid_region_id(const FabricRegionId& region_id) const {
  return ( size_t(region_id) < region_ids_.size() ) && ( region_id == region_ids_[region_id] ); 
}

bool FabricKey::valid_key_id(const FabricKeyId& key_id) const {
  return ( size_t(key_id) < key_ids_.size() ) && ( key_id == key_ids_[key_id] ); 
}

bool FabricKey::valid_key_coordinate(const vtr::Point<int>& coord) const {
  return coord.x() > -1 && coord.y() > -1;
}

bool FabricKey::valid_bl_bank_id(const FabricRegionId& region_id, const FabricBitLineBankId& bank_id) const {
  if (!valid_region_id(region_id)) {
    return false;
  }
  return ( size_t(bank_id) < bl_bank_ids_[region_id].size() ) && ( bank_id == bl_bank_ids_[region_id][bank_id] ); 
}

bool FabricKey::valid_wl_bank_id(const FabricRegionId& region_id, const FabricWordLineBankId& bank_id) const {
  if (!valid_region_id(region_id)) {
    return false;
  }
  return ( size_t(bank_id) < wl_bank_ids_[region_id].size() ) && ( bank_id == wl_bank_ids_[region_id][bank_id] ); 
}
