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

openfpga::BasicPort PinConstraints::net_pin(const std::string& net) const {
  openfpga::BasicPort constrained_pin;
  for (const PinConstraintId& pin_constraint : pin_constraints()) {
    if (net == pin_constraint_nets_[pin_constraint]) {
      constrained_pin = pin(pin_constraint); 
      break;
    }
  }
  return constrained_pin;
} 

PinConstraints::e_logic_level PinConstraints::net_default_value(const std::string& net) const {
  PinConstraints::e_logic_level logic_level = PinConstraints::NUM_LOGIC_LEVELS;
  for (const PinConstraintId& pin_constraint : pin_constraints()) {
    if (net == pin_constraint_nets_[pin_constraint]) {
      logic_level = pin_constraint_net_default_values_[pin_constraint]; 
      break;
    }
  }
  return logic_level;
}

std::string PinConstraints::net_default_value_to_string(const PinConstraintId& pin_constraint) const {
  VTR_ASSERT(valid_pin_constraint_id(pin_constraint));
  if (PinConstraints::LOGIC_HIGH == pin_constraint_net_default_values_[pin_constraint]) {
    return std::string("1");
  } else if (PinConstraints::LOGIC_LOW == pin_constraint_net_default_values_[pin_constraint]) {
    return std::string("0");
  }
  return std::string();
} 

size_t PinConstraints::net_default_value_to_int(const std::string& net) const {
  if (PinConstraints::LOGIC_HIGH == net_default_value(net)) {
    return 1;
  } else if (PinConstraints::LOGIC_LOW == net_default_value(net)) {
    return 0;
  }
  return -1;
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
  pin_constraint_net_default_values_.reserve(num_pin_constraints);
}

PinConstraintId PinConstraints::create_pin_constraint(const openfpga::BasicPort& pin,
                                                      const std::string& net) {
  /* Create a new id */
  PinConstraintId pin_constraint_id = PinConstraintId(pin_constraint_ids_.size());
  
  pin_constraint_ids_.push_back(pin_constraint_id);
  pin_constraint_pins_.push_back(pin);
  pin_constraint_nets_.push_back(net);
  pin_constraint_net_default_values_.push_back(PinConstraints::NUM_LOGIC_LEVELS);
  
  return pin_constraint_id;
}

void PinConstraints::set_net_default_value(const PinConstraintId& pin_constraint,
                                           const std::string& default_value) {
  VTR_ASSERT(valid_pin_constraint_id(pin_constraint));
  if (default_value == std::string("1")) {
    pin_constraint_net_default_values_[pin_constraint] = PinConstraints::LOGIC_HIGH;
  } else if (default_value == std::string("0")) {
    pin_constraint_net_default_values_[pin_constraint] = PinConstraints::LOGIC_LOW;
  }
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool PinConstraints::valid_pin_constraint_id(const PinConstraintId& pin_constraint_id) const {
  return ( size_t(pin_constraint_id) < pin_constraint_ids_.size() ) && ( pin_constraint_id == pin_constraint_ids_[pin_constraint_id] ); 
}

bool PinConstraints::unconstrained_net(const std::string& net) const {
  return net.empty();
}

bool PinConstraints::unmapped_net(const std::string& net) const {
  return std::string(PIN_CONSTRAINT_OPEN_NET) == net;
}

bool PinConstraints::valid_net_default_value(const PinConstraintId& pin_constraint) const {
  VTR_ASSERT(valid_pin_constraint_id(pin_constraint));
  return PinConstraints::NUM_LOGIC_LEVELS != pin_constraint_net_default_values_[pin_constraint]; 
}

bool PinConstraints::valid_net_default_value(const std::string& net) const {
  return PinConstraints::NUM_LOGIC_LEVELS != net_default_value(net); 
}
