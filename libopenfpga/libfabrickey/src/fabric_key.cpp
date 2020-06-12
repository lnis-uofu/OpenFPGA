#include "vtr_assert.h"

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

/************************************************************************
 * Public Accessors : Basic data query 
 ***********************************************************************/
/* Access the name of a key */
std::string FabricKey::key_name(const FabricKeyId& key_id) const {
  /* validate the key_id */
  VTR_ASSERT(valid_key_id(key_id));
  return key_names_[key_id]; 
}

/* Access the value of a key */
size_t FabricKey::key_value(const FabricKeyId& key_id) const {
  /* validate the key_id */
  VTR_ASSERT(valid_key_id(key_id));
  return key_values_[key_id]; 
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void FabricKey::reserve_keys(const size_t& num_keys) {
  key_ids_.reserve(num_keys);
  key_names_.reserve(num_keys);
  key_values_.reserve(num_keys);
}

/* Create a new key and add it to the library, return an id */
FabricKeyId FabricKey::create_key() {
  /* Create a new id */
  FabricKeyId key = FabricKeyId(key_ids_.size());
  key_ids_.push_back(key);
  key_names_.emplace_back();
  key_values_.emplace_back();
  
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

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool FabricKey::valid_key_id(const FabricKeyId& key_id) const {
  return ( size_t(key_id) < key_ids_.size() ) && ( key_id == key_ids_[key_id] ); 
}
