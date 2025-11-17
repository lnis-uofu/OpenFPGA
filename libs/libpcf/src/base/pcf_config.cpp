#include "pcf_config.h"

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
PcfConfig::PcfConfig() { return; }

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
PcfConfig::pcf_custom_constraint_range PcfConfig::custom_constraints() const {
  return vtr::make_range(custom_constraint_ids_.begin(),
                         custom_constraint_ids_.end());
}

/************************************************************************
 * Public Accessors : Basic data query
 ***********************************************************************/
openfpga::BasicPort PcfConfig::custom_constraint_pin(
  const PcfCustomConstraintId& custom_constraint_id) const {
  /* validate the io_id */
  VTR_ASSERT(valid_custom_constraint_id(custom_constraint_id));
  return custom_constraint_pins_[custom_constraint_id];
}

bool PcfConfig::empty() const { return 0 == custom_constraint_ids_.size(); }

/************************************************************************
 * Public Mutators
 ***********************************************************************/

PcfCustomConstraintId PcfConfig::create_custom_constraint() {
  /* Create a new id */
  PcfCustomConstraintId custom_id =
    PcfCustomConstraintId(custom_constraint_ids_.size());

  custom_constraint_ids_.push_back(custom_id);
  custom_constraint_pins_.emplace_back();
  custom_constraint_options_.emplace_back();
  custom_constraint_values_.emplace_back();

  return custom_id;
}

void PcfConfig::set_custom_constraint_option(
  const PcfCustomConstraintId& custom_constraint_id,
  const std::string& option) {
  VTR_ASSERT(valid_custom_constraint_id(custom_constraint_id));
  custom_constraint_options_[custom_constraint_id] = option;
}

void PcfConfig::set_custom_constraint_value(
  const PcfCustomConstraintId& custom_constraint_id, const std::string& value) {
  VTR_ASSERT(valid_custom_constraint_id(custom_constraint_id));
  custom_constraint_values_[custom_constraint_id] = value;
}

void PcfConfig::set_custom_constraint_pin(
  const PcfCustomConstraintId& custom_constraint_id, const std::string& pin) {
  VTR_ASSERT(valid_custom_constraint_id(custom_constraint_id));
  PortParser port_parser(pin);
  custom_constraint_pins_[custom_constraint_id] = port_parser.port();
}

/************************************************************************
 * Internal invalidators/validators
 ***********************************************************************/
/* Validators */
bool PcfConfig::valid_custom_constraint_id(
  const PcfCustomConstraintId& custom_constraint_id) const {
  return (size_t(custom_constraint_id) < custom_constraint_ids_.size()) &&
         (custom_constraint_id == custom_constraint_ids_[custom_constraint_id]);
}
} /* End namespace openfpga*/
