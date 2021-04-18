#include <algorithm>

#include "vtr_assert.h"
#include "vtr_log.h"

#include "pin_constraints.h"

/************************************************************************
 * Member functions for class PinConstraints
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
PinConstraints::PinConstraints() {
  return;
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
PinConstraints::pin_constraint_range PinConstraints::pin_constraints() const {
  return vtr::make_range(pin_constraint_ids_.begin(), pin_constraint_ids_.end());
}

/************************************************************************
 * Public Accessors : Basic data query 
 ***********************************************************************/
openfpga::BasicPort PinConstraints::pin(const PinConstraintId& pin_constraint_id) const {
  /* validate the pin_constraint_id */
  VTR_ASSERT(valid_pin_constraint_id(pin_constraint_id));
  return pin_constraint_pins_[pin_constraint_id]; 
}

std::string PinConstraints::net(const PinConstraintId& pin_constraint_id) const {
  /* validate the pin_constraint_id */
  VTR_ASSERT(valid_pin_constraint_id(pin_constraint_id));
  return pin_constraint_nets_[pin_constraint_id]; 
}

std::string PinConstraints::pin_net(const openfpga::BasicPort& pin) const {
  std::string constrained_net_name;
  for (const PinConstraintId& pin_constraint : pin_constraints()) {
    if (pin == pin_constraint_pins_[pin_constraint]) {
      constrained_net_name = net(pin_constraint); 
      break;
    }
  }
  return constrained_net_name;
}

bool PinConstraints::empty() const {
  return 0 == pin_constraint_ids_.size();
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void PinConstraints::reserve_pin_constraints(const size_t& num_pin_constraints) {
  pin_constraint_ids_.reserve(num_pin_constraints);
  pin_constraint_pins_.reserve(num_pin_constraints);
  pin_constraint_nets_.reserve(num_pin_constraints);
}

PinConstraintId PinConstraints::create_pin_constraint(const openfpga::BasicPort& pin,
                                                      const std::string& net) {
  /* Create a new id */
  PinConstraintId pin_constraint_id = PinConstraintId(pin_constraint_ids_.size());
  
  pin_constraint_ids_.push_back(pin_constraint_id);
  pin_constraint_pins_.push_back(pin);
  pin_constraint_nets_.push_back(net);
  
  return pin_constraint_id;
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool PinConstraints::valid_pin_constraint_id(const PinConstraintId& pin_constraint_id) const {
  return ( size_t(pin_constraint_id) < pin_constraint_ids_.size() ) && ( pin_constraint_id == pin_constraint_ids_[pin_constraint_id] ); 
}
