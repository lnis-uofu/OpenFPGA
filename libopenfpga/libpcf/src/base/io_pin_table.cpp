#include <algorithm>

#include "vtr_assert.h"
#include "vtr_log.h"

#include "io_pin_table.h"

/* begin namespace openfpga */
namespace openfpga {

/************************************************************************
 * Member functions for class IoPinTable
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
IoPinTable::IoPinTable() {
  return;
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
IoPinTable::io_pin_table_range IoPinTable::internal_pins() const {
  return vtr::make_range(pin_ids_.begin(), pin_ids_.end());
}

/************************************************************************
 * Public Accessors : Basic data query 
 ***********************************************************************/
openfpga::BasicPort IoPinTable::internal_pin(const IoPinTableId& pin_id) const {
  /* validate the pin_id */
  VTR_ASSERT(valid_pin_id(pin_id));
  return internal_pins_[pin_id]; 
}

bool IoPinTable::empty() const {
  return 0 == pin_ids_.size();
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void IoPinTable::reserve_pins(const size_t& num_pins) {
  pin_ids_.reserve(num_pins);
  internal_pins_.reserve(num_pins);
  external_pins_.reserve(num_pins);
  pin_sides_.reserve(num_pins);
  pin_directions_.reserve(num_pins);
}

IoPinTableId IoPinTable::create_pin() {
  /* Create a new id */
  IoPinTableId pin_id = IoPinTableId(pin_ids_.size());
  
  pin_ids_.push_back(pin_id);
  internal_pins_.emplace_back(); 
  external_pins_.emplace_back(); 
  pin_sides_.emplace_back(NUM_SIDES); 
  pin_directions_.emplace_back(NUM_IO_DIRECTIONS); 
  
  return pin_id;
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool IoPinTable::valid_pin_id(const IoPinTableId& pin_id) const {
  return ( size_t(pin_id) < pin_ids_.size() ) && ( pin_id == pin_ids_[pin_id] ); 
}

} /* end namespace openfpga */
