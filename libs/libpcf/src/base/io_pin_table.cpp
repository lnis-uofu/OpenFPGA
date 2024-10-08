#include "io_pin_table.h"

#include <algorithm>

#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/************************************************************************
 * Member functions for class IoPinTable
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
IoPinTable::IoPinTable() { return; }

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
IoPinTable::io_pin_table_range IoPinTable::pins() const {
  return vtr::make_range(pin_ids_.begin(), pin_ids_.end());
}

/************************************************************************
 * Public Accessors : Basic data query
 ***********************************************************************/
BasicPort IoPinTable::internal_pin(const IoPinTableId& pin_id) const {
  /* validate the pin_id */
  VTR_ASSERT(valid_pin_id(pin_id));
  return internal_pins_[pin_id];
}

BasicPort IoPinTable::external_pin(const IoPinTableId& pin_id) const {
  /* validate the pin_id */
  VTR_ASSERT(valid_pin_id(pin_id));
  return external_pins_[pin_id];
}

e_side IoPinTable::pin_side(const IoPinTableId& pin_id) const {
  /* validate the pin_id */
  VTR_ASSERT(valid_pin_id(pin_id));
  return pin_sides_[pin_id];
}

IoPinTable::e_io_direction IoPinTable::pin_direction(
  const IoPinTableId& pin_id) const {
  /* validate the pin_id */
  VTR_ASSERT(valid_pin_id(pin_id));
  return pin_directions_[pin_id];
}

std::vector<IoPinTableId> IoPinTable::find_internal_pin(
  const BasicPort& ext_pin, const e_io_direction& pin_direction) const {
  std::vector<IoPinTableId> int_pin_ids;
  for (auto pin_id : pin_ids_) {
    if ((external_pins_[pin_id] == ext_pin) &&
        (pin_directions_[pin_id] == pin_direction)) {
      int_pin_ids.push_back(pin_id);
    }
  }
  return int_pin_ids;
}

bool IoPinTable::empty() const { return 0 == pin_ids_.size(); }

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
  pin_sides_.emplace_back(NUM_2D_SIDES);
  pin_directions_.emplace_back(NUM_IO_DIRECTIONS);

  return pin_id;
}

void IoPinTable::set_internal_pin(const IoPinTableId& pin_id,
                                  const BasicPort& pin) {
  VTR_ASSERT(valid_pin_id(pin_id));
  internal_pins_[pin_id] = pin;
}

void IoPinTable::set_external_pin(const IoPinTableId& pin_id,
                                  const BasicPort& pin) {
  VTR_ASSERT(valid_pin_id(pin_id));
  external_pins_[pin_id] = pin;
}

void IoPinTable::set_pin_side(const IoPinTableId& pin_id, const e_side& side) {
  VTR_ASSERT(valid_pin_id(pin_id));
  pin_sides_[pin_id] = side;
}

void IoPinTable::set_pin_direction(const IoPinTableId& pin_id,
                                   const e_io_direction& direction) {
  VTR_ASSERT(valid_pin_id(pin_id));
  pin_directions_[pin_id] = direction;
}

/************************************************************************
 * Internal invalidators/validators
 ***********************************************************************/
/* Validators */
bool IoPinTable::valid_pin_id(const IoPinTableId& pin_id) const {
  return (size_t(pin_id) < pin_ids_.size()) && (pin_id == pin_ids_[pin_id]);
}

} /* end namespace openfpga */
