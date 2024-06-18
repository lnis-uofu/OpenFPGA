#include "arch_direct.h"

#include "vtr_assert.h"

/************************************************************************
 * Member functions for class ArchDirect
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
ArchDirect::ArchDirect() { return; }

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
ArchDirect::arch_direct_range ArchDirect::directs() const {
  return vtr::make_range(direct_ids_.begin(), direct_ids_.end());
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
ArchDirectId ArchDirect::direct(const std::string& name) const {
  if (0 < direct_name2ids_.count(name)) {
    return direct_name2ids_.at(name);
  }
  return ArchDirectId::INVALID();
}

std::string ArchDirect::name(const ArchDirectId& direct_id) const {
  /* validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  return names_[direct_id];
}

CircuitModelId ArchDirect::circuit_model(const ArchDirectId& direct_id) const {
  /* validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  return circuit_models_[direct_id];
}

e_direct_type ArchDirect::type(const ArchDirectId& direct_id) const {
  /* validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  return types_[direct_id];
}

e_direct_direction ArchDirect::x_dir(const ArchDirectId& direct_id) const {
  /* validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  return directions_[direct_id].x();
}

e_direct_direction ArchDirect::y_dir(const ArchDirectId& direct_id) const {
  /* validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  return directions_[direct_id].y();
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
ArchDirectId ArchDirect::add_direct(const std::string& name) {
  if (0 < direct_name2ids_.count(name)) {
    return ArchDirectId::INVALID();
  }

  /* This is a legal name. we can create a new id */
  ArchDirectId direct = ArchDirectId(direct_ids_.size());
  direct_ids_.push_back(direct);
  names_.push_back(name);
  circuit_models_.push_back(CircuitModelId::INVALID());
  types_.emplace_back(e_direct_type::NUM_DIRECT_TYPES);
  directions_.emplace_back(vtr::Point<e_direct_direction>(
    NUM_DIRECT_DIRECTIONS, NUM_DIRECT_DIRECTIONS));

  /* Register in the name-to-id map */
  direct_name2ids_[name] = direct;

  return direct;
}

void ArchDirect::set_circuit_model(const ArchDirectId& direct_id,
                                   const CircuitModelId& circuit_model) {
  /* validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  circuit_models_[direct_id] = circuit_model;
}

void ArchDirect::set_type(const ArchDirectId& direct_id,
                          const e_direct_type& type) {
  /* validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  types_[direct_id] = type;
}

void ArchDirect::set_direction(const ArchDirectId& direct_id,
                               const e_direct_direction& x_dir,
                               const e_direct_direction& y_dir) {
  /* validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  directions_[direct_id].set_x(x_dir);
  directions_[direct_id].set_y(y_dir);
}

/************************************************************************
 * Internal invalidators/validators
 ***********************************************************************/
/* Validators */
bool ArchDirect::valid_direct_id(const ArchDirectId& direct_id) const {
  return (size_t(direct_id) < direct_ids_.size()) &&
         (direct_id == direct_ids_[direct_id]);
}
