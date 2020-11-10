/************************************************************************
 * Member functions for class TileAnnotation
 ***********************************************************************/
#include "vtr_assert.h"

#include "tile_annotation.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/
TileAnnotation::TileAnnotation() {
  return;
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
TileAnnotation::global_port_range TileAnnotation::global_ports() const {
  return vtr::make_range(global_port_ids_.begin(), global_port_ids_.end());
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
std::string TileAnnotation::global_port_name(const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_names_[global_port_id];
}

std::string TileAnnotation::global_port_tile_name(const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_tile_names_[global_port_id];
}

BasicPort TileAnnotation::global_port_tile_port(const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_tile_ports_[global_port_id];
}

bool TileAnnotation::global_port_is_clock(const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_is_clock_[global_port_id];
}

bool TileAnnotation::global_port_is_set(const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_is_set_[global_port_id];
}

bool TileAnnotation::global_port_is_reset(const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_is_reset_[global_port_id];
}

size_t TileAnnotation::global_port_default_value(const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_default_values_[global_port_id];
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
TileGlobalPortId TileAnnotation::create_global_port(const std::string& port_name,
                                                    const std::string& tile_name,
                                                    const BasicPort& tile_port) {
  /* This is a legal name. we can create a new id */
  TileGlobalPortId port_id = TileGlobalPortId(global_port_ids_.size());
  global_port_ids_.push_back(port_id);
  global_port_names_.push_back(port_name);
  global_port_tile_names_.push_back(tile_name);
  global_port_tile_ports_.push_back(tile_port);
  global_port_is_clock_.push_back(false);
  global_port_is_set_.push_back(false);
  global_port_is_reset_.push_back(false);
  global_port_default_values_.push_back(0);

  return port_id;
}

void TileAnnotation::set_global_port_is_clock(const TileGlobalPortId& global_port_id,
                                              const bool& is_clock) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_is_clock_[global_port_id] = is_clock;
}

void TileAnnotation::set_global_port_is_set(const TileGlobalPortId& global_port_id,
                                            const bool& is_set) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_is_set_[global_port_id] = is_set;
}

void TileAnnotation::set_global_port_is_reset(const TileGlobalPortId& global_port_id,
                                              const bool& is_reset) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_is_reset_[global_port_id] = is_reset;
}

void TileAnnotation::set_global_port_default_value(const TileGlobalPortId& global_port_id,
                                                   const size_t& default_value) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_default_values_[global_port_id] = default_value;
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool TileAnnotation::valid_global_port_id(const TileGlobalPortId& global_port_id) const {
  return ( size_t(global_port_id) < global_port_ids_.size() ) && ( global_port_id == global_port_ids_[global_port_id] ); 
}

} /* namespace openfpga ends */
