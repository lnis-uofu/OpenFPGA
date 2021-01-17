#include <algorithm>

#include "vtr_assert.h"
#include "vtr_log.h"

#include "repack_design_constraints.h"

/************************************************************************
 * Member functions for class RepackDesignConstraints
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
RepackDesignConstraints::RepackDesignConstraints() {
  return;
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
RepackDesignConstraints::repack_design_constraint_range RepackDesignConstraints::design_constraints() const {
  return vtr::make_range(repack_design_constraint_ids_.begin(), repack_design_constraint_ids_.end());
}

/************************************************************************
 * Public Accessors : Basic data query 
 ***********************************************************************/
RepackDesignConstraints::e_design_constraint_type RepackDesignConstraints::type(const RepackDesignConstraintId& repack_design_constraint_id) const {
  /* validate the design_constraint_id */
  VTR_ASSERT(valid_design_constraint_id(repack_design_constraint_id));
  return repack_design_constraint_types_[repack_design_constraint_id]; 
}

std::string RepackDesignConstraints::tile(const RepackDesignConstraintId& repack_design_constraint_id) const {
  /* validate the design_constraint_id */
  VTR_ASSERT(valid_design_constraint_id(repack_design_constraint_id));
  return repack_design_constraint_tiles_[repack_design_constraint_id]; 
}

openfpga::BasicPort RepackDesignConstraints::pin(const RepackDesignConstraintId& repack_design_constraint_id) const {
  /* validate the design_constraint_id */
  VTR_ASSERT(valid_design_constraint_id(repack_design_constraint_id));
  return repack_design_constraint_pins_[repack_design_constraint_id]; 
}

std::string RepackDesignConstraints::net(const RepackDesignConstraintId& repack_design_constraint_id) const {
  /* validate the design_constraint_id */
  VTR_ASSERT(valid_design_constraint_id(repack_design_constraint_id));
  return repack_design_constraint_nets_[repack_design_constraint_id]; 
}

bool RepackDesignConstraints::empty() const {
  return 0 == repack_design_constraint_ids_.size();
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void RepackDesignConstraints::reserve_design_constraints(const size_t& num_design_constraints) {
  repack_design_constraint_ids_.reserve(num_design_constraints);
  repack_design_constraint_types_.reserve(num_design_constraints);
  repack_design_constraint_tiles_.reserve(num_design_constraints);
  repack_design_constraint_pins_.reserve(num_design_constraints);
  repack_design_constraint_nets_.reserve(num_design_constraints);
}

RepackDesignConstraintId RepackDesignConstraints::create_design_constraint(const RepackDesignConstraints::e_design_constraint_type& repack_design_constraint_type) {
  /* Create a new id */
  RepackDesignConstraintId repack_design_constraint_id = RepackDesignConstraintId(repack_design_constraint_ids_.size());
  
  repack_design_constraint_ids_.push_back(repack_design_constraint_id);
  repack_design_constraint_types_.push_back(repack_design_constraint_type);
  repack_design_constraint_tiles_.emplace_back();
  repack_design_constraint_pins_.emplace_back();
  repack_design_constraint_nets_.emplace_back();
  
  return repack_design_constraint_id;
}

void RepackDesignConstraints::set_tile(const RepackDesignConstraintId& repack_design_constraint_id,
                                       const std::string& tile) {
  /* validate the design_constraint_id */
  VTR_ASSERT(valid_design_constraint_id(repack_design_constraint_id));
  repack_design_constraint_tiles_[repack_design_constraint_id] = tile;
}

void RepackDesignConstraints::set_pin(const RepackDesignConstraintId& repack_design_constraint_id,
                                      const openfpga::BasicPort& pin) {
  /* validate the design_constraint_id */
  VTR_ASSERT(valid_design_constraint_id(repack_design_constraint_id));
  repack_design_constraint_pins_[repack_design_constraint_id] = pin;
}

void RepackDesignConstraints::set_net(const RepackDesignConstraintId& repack_design_constraint_id,
                                      const std::string& net) {
  /* validate the design_constraint_id */
  VTR_ASSERT(valid_design_constraint_id(repack_design_constraint_id));
  repack_design_constraint_nets_[repack_design_constraint_id] = net;
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool RepackDesignConstraints::valid_design_constraint_id(const RepackDesignConstraintId& design_constraint_id) const {
  return ( size_t(design_constraint_id) < repack_design_constraint_ids_.size() ) && ( design_constraint_id == repack_design_constraint_ids_[design_constraint_id] ); 
}
