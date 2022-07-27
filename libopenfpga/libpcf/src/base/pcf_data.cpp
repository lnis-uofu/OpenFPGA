#include <algorithm>

#include "vtr_assert.h"
#include "vtr_log.h"

#include "openfpga_port_parser.h"
#include "pcf_data.h"

/* Begin namespace openfpga */
namespace openfpga {

/************************************************************************
 * Member functions for class PcfData
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
PcfData::PcfData() {
  return;
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
PcfData::pcf_io_constraint_range PcfData::io_constraints() const {
  return vtr::make_range(io_constraint_ids_.begin(), io_constraint_ids_.end());
}

/************************************************************************
 * Public Accessors : Basic data query 
 ***********************************************************************/
openfpga::BasicPort PcfData::io_pin(const PcfIoConstraintId& io_id) const {
  /* validate the io_id */
  VTR_ASSERT(valid_io_constraint_id(io_id));
  return io_constraint_pins_[io_id]; 
}

std::string PcfData::io_net(const PcfIoConstraintId& io_id) const {
  /* validate the io_id */
  VTR_ASSERT(valid_io_constraint_id(io_id));
  return io_constraint_nets_[io_id]; 
}

bool PcfData::empty() const {
  return 0 == io_constraint_ids_.size();
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void PcfData::reserve_io_constraints(const size_t& num_io_constraints) {
  io_constraint_ids_.reserve(num_io_constraints);
  io_constraint_pins_.reserve(num_io_constraints);
  io_constraint_nets_.reserve(num_io_constraints);
}

PcfIoConstraintId PcfData::create_io_constraint() {
  /* Create a new id */
  PcfIoConstraintId io_id = PcfIoConstraintId(io_constraint_ids_.size());
  
  io_constraint_ids_.push_back(io_id);
  io_constraint_pins_.emplace_back();
  io_constraint_nets_.emplace_back();
  
  return io_id;
}

void PcfData::set_io_net(const PcfIoConstraintId& io_id,
                         const std::string& net) {
  VTR_ASSERT(valid_io_constraint_id(io_id));
  io_constraint_nets_[io_id] = net;
}

void PcfData::set_io_pin(const PcfIoConstraintId& io_id,
                         const std::string& pin) {
  VTR_ASSERT(valid_io_constraint_id(io_id));
  PortParser port_parser(pin);
  io_constraint_pins_[io_id] = port_parser.port();
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool PcfData::valid_io_constraint_id(const PcfIoConstraintId& io_id) const {
  return ( size_t(io_id) < io_constraint_ids_.size() ) && ( io_id == io_constraint_ids_[io_id] ); 
}

} /* End namespace openfpga*/

