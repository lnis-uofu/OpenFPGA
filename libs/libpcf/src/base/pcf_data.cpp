#include "pcf_data.h"

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
PcfData::PcfData() { return; }

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

bool PcfData::empty() const { return 0 == io_constraint_ids_.size(); }

bool PcfData::validate() const {
  size_t num_err = 0;
  /* In principle, we do not expect duplicated assignment: 1 net -> 2 pins */
  std::map<std::string, BasicPort> net2pin;
  for (const PcfIoConstraintId& io_id : io_constraints()) {
    std::string curr_net = io_constraint_nets_[io_id];
    BasicPort curr_pin = io_constraint_pins_[io_id];
    auto result = net2pin.find(curr_net);
    if (result != net2pin.end()) {
      /* Found one nets assigned to two pins, throw warning  */
      VTR_LOG_WARN(
        "Net '%s' is assigned to two pins '%s[%lu]' and '%s[%lu]'!\n",
        curr_net.c_str(), curr_pin.get_name().c_str(), curr_pin.get_lsb(),
        result->second.get_name().c_str(), result->second.get_lsb());
    }
    net2pin[curr_net] = curr_pin;
  }
  /* We should not have duplicated pins in assignment: 1 pin -> 2 nets */
  /* Caution: must use constant pointer here, otherwise you may see duplicated
   * key on BasicPort with different content! */
  std::map<const BasicPort*, std::string> pin2net;
  for (const PcfIoConstraintId& io_id : io_constraints()) {
    std::string curr_net = io_constraint_nets_[io_id];
    const BasicPort& curr_pin = io_constraint_pins_[io_id];
    auto result = pin2net.find(&curr_pin);
    if (result != pin2net.end()) {
      /* Found one pin assigned to two nets, this is definitely an error  */
      VTR_LOG_ERROR("Pin '%s[%lu]' is assigned to two nets '%s' and '%s'!\n",
                    curr_pin.get_name().c_str(), curr_pin.get_lsb(),
                    result->second.c_str(), curr_net.c_str());
      num_err++;
    }
    pin2net[&curr_pin] = curr_net;
  }
  if (num_err) {
    return false;
  }
  return true;
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
  return (size_t(io_id) < io_constraint_ids_.size()) &&
         (io_id == io_constraint_ids_[io_id]);
}

} /* End namespace openfpga*/
