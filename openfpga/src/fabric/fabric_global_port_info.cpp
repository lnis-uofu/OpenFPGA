/************************************************************************
 * Member functions for class FabricGlobalPortInfo
 ***********************************************************************/
#include "vtr_assert.h"

#include "fabric_global_port_info.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/
FabricGlobalPortInfo::FabricGlobalPortInfo() {
  return;
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
FabricGlobalPortInfo::global_port_range FabricGlobalPortInfo::global_ports() const {
  return vtr::make_range(global_port_ids_.begin(), global_port_ids_.end());
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
ModulePortId FabricGlobalPortInfo::global_module_port(const FabricGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_module_ports_[global_port_id];
}

bool FabricGlobalPortInfo::global_port_is_clock(const FabricGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_is_clock_[global_port_id];
}

bool FabricGlobalPortInfo::global_port_is_set(const FabricGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_is_set_[global_port_id];
}

bool FabricGlobalPortInfo::global_port_is_reset(const FabricGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_is_reset_[global_port_id];
}

bool FabricGlobalPortInfo::global_port_is_prog(const FabricGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_is_prog_[global_port_id];
}

bool FabricGlobalPortInfo::global_port_is_config_enable(const FabricGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_is_config_enable_[global_port_id];
}

bool FabricGlobalPortInfo::global_port_is_io(const FabricGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_is_io_[global_port_id];
}

size_t FabricGlobalPortInfo::global_port_default_value(const FabricGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_default_values_[global_port_id];
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
FabricGlobalPortId FabricGlobalPortInfo::create_global_port(const ModulePortId& module_port) {
  /* This is a legal name. we can create a new id */
  FabricGlobalPortId port_id = FabricGlobalPortId(global_port_ids_.size());
  global_port_ids_.push_back(port_id);
  global_module_ports_.push_back(module_port);
  global_port_is_clock_.push_back(false);
  global_port_is_set_.push_back(false);
  global_port_is_reset_.push_back(false);
  global_port_is_prog_.push_back(false);
  global_port_is_io_.push_back(false);
  global_port_is_config_enable_.push_back(false);
  global_port_default_values_.push_back(0);

  return port_id;
}

void FabricGlobalPortInfo::set_global_port_is_clock(const FabricGlobalPortId& global_port_id,
                                                    const bool& is_clock) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_is_clock_[global_port_id] = is_clock;
}

void FabricGlobalPortInfo::set_global_port_is_set(const FabricGlobalPortId& global_port_id,
                                                  const bool& is_set) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_is_set_[global_port_id] = is_set;
}

void FabricGlobalPortInfo::set_global_port_is_reset(const FabricGlobalPortId& global_port_id,
                                                    const bool& is_reset) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_is_reset_[global_port_id] = is_reset;
}

void FabricGlobalPortInfo::set_global_port_is_prog(const FabricGlobalPortId& global_port_id,
                                                   const bool& is_prog) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_is_prog_[global_port_id] = is_prog;
}

void FabricGlobalPortInfo::set_global_port_is_config_enable(const FabricGlobalPortId& global_port_id,
                                                            const bool& is_config_enable) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_is_config_enable_[global_port_id] = is_config_enable;
}

void FabricGlobalPortInfo::set_global_port_is_io(const FabricGlobalPortId& global_port_id,
                                                 const bool& is_io) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_is_io_[global_port_id] = is_io;
}

void FabricGlobalPortInfo::set_global_port_default_value(const FabricGlobalPortId& global_port_id,
                                                         const size_t& default_value) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_default_values_[global_port_id] = default_value;
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool FabricGlobalPortInfo::valid_global_port_id(const FabricGlobalPortId& global_port_id) const {
  return ( size_t(global_port_id) < global_port_ids_.size() ) && ( global_port_id == global_port_ids_[global_port_id] ); 
}

} /* namespace openfpga ends */
