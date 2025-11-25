#include "pcf_custom_constraints.h"

#include <algorithm>

#include "openfpga_port_parser.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* Begin namespace openfpga */
namespace openfpga {

/************************************************************************
 * Member functions for class PcfData
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
PcfCustomConstraint::PcfCustomConstraint() { return; }

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
PcfCustomConstraint::pcf_custom_constraint_range
PcfCustomConstraint::custom_constraints() const {
  return vtr::make_range(custom_constraint_ids_.begin(),
                         custom_constraint_ids_.end());
}

/************************************************************************
 * Public Accessors : Basic data query
 ***********************************************************************/
openfpga::BasicPort PcfCustomConstraint::custom_constraint_pin(
  const PcfCustomConstraintId& custom_constraint_id) const {
  /* validate the io_id */
  VTR_ASSERT(valid_custom_constraint_id(custom_constraint_id));
  return custom_constraint_pins_[custom_constraint_id];
}

bool PcfCustomConstraint::empty() const {
  return 0 == custom_constraint_ids_.size();
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/

PcfCustomConstraintId PcfCustomConstraint::create_custom_constraint() {
  /* Create a new id */
  PcfCustomConstraintId custom_id =
    PcfCustomConstraintId(custom_constraint_ids_.size());

  custom_constraint_ids_.push_back(custom_id);
  custom_constraint_pins_.emplace_back();
  custom_constraint_pin_mode_.emplace_back();
  custom_constraint_command_name_.emplace_back();

  return custom_id;
}

void PcfCustomConstraint::set_custom_constraint_pin_mode(
  const PcfCustomConstraintId& custom_constraint_id, const std::string& mode) {
  VTR_ASSERT(valid_custom_constraint_id(custom_constraint_id));
  custom_constraint_pin_mode_[custom_constraint_id] = mode;
}

void PcfCustomConstraint::set_custom_constraint_command(
  const PcfCustomConstraintId& custom_constraint_id,
  const std::string& command_name) {
  VTR_ASSERT(valid_custom_constraint_id(custom_constraint_id));
  custom_constraint_command_name_[custom_constraint_id] = command_name;
}

void PcfCustomConstraint::set_custom_constraint_pin(
  const PcfCustomConstraintId& custom_constraint_id, const std::string& pin) {
  VTR_ASSERT(valid_custom_constraint_id(custom_constraint_id));
  PortParser port_parser(pin);
  custom_constraint_pins_[custom_constraint_id] = port_parser.port();
}

/************************************************************************
 * Internal invalidators/validators
 ***********************************************************************/
/* Validators */
bool PcfCustomConstraint::valid_custom_constraint_id(
  const PcfCustomConstraintId& custom_constraint_id) const {
  return (size_t(custom_constraint_id) < custom_constraint_ids_.size()) &&
         (custom_constraint_id == custom_constraint_ids_[custom_constraint_id]);
}

} /* End namespace openfpga*/
