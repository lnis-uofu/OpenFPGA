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

vtr::Point<size_t> RepackDesignConstraints::tile_coordinate(const RepackDesignConstraintId& repack_design_constraint_id) const {
  /* validate the design_constraint_id */
  VTR_ASSERT(valid_design_constraint_id(repack_design_constraint_id));
  return vtr::Point<size_t>(repack_design_constraint_tiles_x_[repack_design_constraint_id],
                            repack_design_constraint_tiles_y_[repack_design_constraint_id]);
}

BasicPort RepackDesignConstraints::pin(const RepackDesignConstraintId& repack_design_constraint_id) const {
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
  return 0 == design_constraint_ids_.size();
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void RepackDesignConstraints::reserve_design_constraints(const size_t& num_design_constraints) {
  repack_design_constraint_ids_.reserve(num_design_constraints);
  repack_design_constraint_tile_types_.reserve(num_design_constraints);
  repack_design_constraint_tiles_.reserve(num_design_constraints);
  repack_design_constraint_tiles_x_.reserve(num_design_constraints);
  repack_design_constraint_tiles_y_.reserve(num_design_constraints);
  repack_design_constraint_pins_.reserve(num_design_constraints);
  repack_design_constraint_nets_.reserve(num_design_constraints);
}

RepackDesignConstraintId create_design_constraint(const RepackDesignConstraints::e_design_constraint_type& repack_design_constraint_type) {
  /* Create a new id */
  RepackDesignConstraintId repack_design_constraint_id = RepackDesignConstraintId(design_constraint_ids_.size());
  
  repack_design_constraint_ids_.push_back(repack_design_constraint_id);
  repack_design_constraint_types_.push_back(repack_design_constraint_type);
  repack_design_constraint_tiles_.emplace_back();
  repack_design_constraint_tiles_x_.push_back(size_t(-1));
  repack_design_constraint_tiles_y_.push_back(size_t(-1));
  repack_design_constraint_tile_pins_.emplace_back();
  repack_design_constraint_tile_nets_.emplace_back();
  
  return repack_design_constraint_id;
}

void RepackDesignConstraints::set_tile(const RepackDesignConstraintId& repack_design_constraint_id,
                                       const std::string& tile) {
  /* validate the design_constraint_id */
  VTR_ASSERT(valid_design_constraint_id(repack_design_constraint_id));
  repack_design_constraint_tiles_[repack_design_constaint_id] = tile;
}

void RepackDesignConstraints::set_tile_coordinate(const RepackDesignConstraintId& repack_design_constraint_id,
                                                  const vtr::Point<size_t>& tile_coordinate) {
  /* validate the design_constraint_id */
  VTR_ASSERT(valid_design_constraint_id(repack_design_constraint_id));
  repack_design_constraint_tiles_x_[repack_design_constaint_id] = tile_coordinate.x();
  repack_design_constraint_tiles_y_[repack_design_constaint_id] = tile_coordinate.y();
}

void RepackDesignConstraints::set_pin(const RepackDesignConstraintId& repack_design_constraint_id,
                                      const BasicPort& pin) {
  /* validate the design_constraint_id */
  VTR_ASSERT(valid_design_constraint_id(repack_design_constraint_id));
  repack_design_constraint_pins_[repack_design_constaint_id] = pin;
}

void RepackDesignConstraints::set_net(const RepackDesignConstraintId& repack_design_constraint_id,
                                      const std::string& net) {
  /* validate the design_constraint_id */
  VTR_ASSERT(valid_design_constraint_id(repack_design_constraint_id));
  repack_design_constraint_nets_[repack_design_constaint_id] = net;
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool RepackDesignConstraintId::valid_design_constraint_id(const RepackDesignConstraintId& design_constraint_id) const {
  return ( size_t(design_constraint_id) < repack_design_constraint_ids_.size() ) && ( design_constraint_id == repack_design_constraint_ids_[design_constraint_id] ); 
}
