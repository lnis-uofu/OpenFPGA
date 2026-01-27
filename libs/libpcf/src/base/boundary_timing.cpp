#include "boundary_timing.h"

#include <algorithm>

#include "vtr_assert.h"
#include "vtr_log.h"

/************************************************************************
 * Member functions for class PinConstraints
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
namespace openfpga {
BoundaryTiming::BoundaryTiming() { return; }

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
BoundaryTiming::pin_constraint_range BoundaryTiming::pin_constraints() const {
  return vtr::make_range(pin_constraint_ids_.begin(),
                         pin_constraint_ids_.end());
}

/************************************************************************
 * Public Accessors : Basic data query
 ***********************************************************************/
openfpga::BasicPort BoundaryTiming::pin(
  const PinConstraintId& pin_constraint_id) const {
  /* validate the pin_constraint_id */
  VTR_ASSERT(valid_pin_constraint_id(pin_constraint_id));
  return pin_constraint_pins_[pin_constraint_id];
}

std::string BoundaryTiming::max_delay(
  const PinConstraintId& pin_constraint_id) const {
  /* validate the pin_constraint_id */
  VTR_ASSERT(valid_pin_constraint_id(pin_constraint_id));
  return pin_constraint_max_delay_[pin_constraint_id];
}

std::string BoundaryTiming::min_delay(
  const PinConstraintId& pin_constraint_id) const {
  /* validate the pin_constraint_id */
  VTR_ASSERT(valid_pin_constraint_id(pin_constraint_id));
  return pin_constraint_min_delay_[pin_constraint_id];
}

std::string BoundaryTiming::pin_max_delay(
  const openfpga::BasicPort& pin) const {
  std::string max = "";
  for (const PinConstraintId& pin_constraint : pin_constraints()) {
    if (pin == pin_constraint_pins_[pin_constraint]) {
      max = max_delay(pin_constraint);
      break;
    }
  }
  return max;
}

std::string BoundaryTiming::pin_min_delay(
  const openfpga::BasicPort& pin) const {
  std::string min = "";
  for (const PinConstraintId& pin_constraint : pin_constraints()) {
    if (pin == pin_constraint_pins_[pin_constraint]) {
      min = min_delay(pin_constraint);
      break;
    }
  }
  return min;
}

bool BoundaryTiming::pin_delay_constrained(
  const openfpga::BasicPort& pin) const {
  std::string max = pin_max_delay(pin);
  std::string min = pin_min_delay(pin);
  if (!max.empty() && !min.empty()) {
    return true;
  }
  return false;
}

bool BoundaryTiming::empty() const { return 0 == pin_constraint_ids_.size(); }

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void BoundaryTiming::reserve_pin_constraints(
  const size_t& num_pin_constraints) {
  pin_constraint_ids_.reserve(num_pin_constraints);
  pin_constraint_pins_.reserve(num_pin_constraints);
  pin_constraint_max_delay_.reserve(num_pin_constraints);
  pin_constraint_min_delay_.reserve(num_pin_constraints);
}

PinConstraintId BoundaryTiming::create_pin_boundary_timing(
  const openfpga::BasicPort& pin, const std::string& max_delay,
  const std::string min_delay) {
  /* Create a new id */
  PinConstraintId pin_constraint_id =
    PinConstraintId(pin_constraint_ids_.size());

  pin_constraint_ids_.push_back(pin_constraint_id);
  pin_constraint_pins_.push_back(pin);
  pin_constraint_max_delay_.push_back(max_delay);
  pin_constraint_min_delay_.push_back(min_delay);
  return pin_constraint_id;
}

/************************************************************************
 * Internal invalidators/validators
 ***********************************************************************/
/* Validators */
bool BoundaryTiming::valid_pin_constraint_id(
  const PinConstraintId& pin_constraint_id) const {
  return (size_t(pin_constraint_id) < pin_constraint_ids_.size()) &&
         (pin_constraint_id == pin_constraint_ids_[pin_constraint_id]);
}
}  // namespace openfpga