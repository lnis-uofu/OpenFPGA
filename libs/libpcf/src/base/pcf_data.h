#ifndef PCF_DATA_H
#define PCF_DATA_H

/********************************************************************
 * This file include the declaration of pcf data
 *******************************************************************/
#include <array>
#include <map>
#include <string>

/* Headers from vtrutil library */
#include "vtr_geometry.h"
#include "vtr_vector.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"
#include "pcf_data_fwd.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A data structure to constain PCF data
 * This data structure may include a number of design constraints
 * - I/O constraint, for instance, force a net to be mapped to specific pin
 *
 * Typical usage:
 * --------------
 *   // Create an object
 *   PcfData pcf_data;
 *   // Add a constraint
 *   PcfIoConstraintId io_id = pcf_data.create_io_constraint();
 *   pcf_data.set_io_net(io_id, net_name);
 *   pcf_data.set_io_pin(io_id, pin_name);
 *
 *******************************************************************/
class PcfData {
 public: /* Types */
  typedef vtr::vector<PcfIoConstraintId, PcfIoConstraintId>::const_iterator
    pcf_io_constraint_iterator;
  /* Create range */
  typedef vtr::Range<pcf_io_constraint_iterator> pcf_io_constraint_range;

 public: /* Constructors */
  PcfData();

 public: /* Accessors: aggregates */
  pcf_io_constraint_range io_constraints() const;

 public: /* Public Accessors: Basic data query */
  /* Get the pin to be constrained */
  openfpga::BasicPort io_pin(const PcfIoConstraintId& io_id) const;

  /* Get the net to be constrained */
  std::string io_net(const PcfIoConstraintId& io_id) const;

  /* Check if there are any io constraints */
  bool empty() const;

  /* Check if the data is valid: each pin can only be mapped to one net */
  bool validate() const;

 public: /* Public Mutators */
  /* Reserve a number of design constraints to be memory efficent */
  void reserve_io_constraints(const size_t& num_io_constraints);

  /* Add a pin constraint to storage */
  PcfIoConstraintId create_io_constraint();

  /* Set the net for an io constraint */
  void set_io_net(const PcfIoConstraintId& io_id, const std::string& net);

  /* Set the net for an io constraint */
  void set_io_pin(const PcfIoConstraintId& io_id, const std::string& pin);

 public: /* Public invalidators/validators */
  /* Show if the constraint id is a valid for data queries */
  bool valid_io_constraint_id(const PcfIoConstraintId& io_id) const;

 private: /* Internal data */
  /* Unique ids for each design constraint */
  vtr::vector<PcfIoConstraintId, PcfIoConstraintId> io_constraint_ids_;

  /* Pins to constraint */
  vtr::vector<PcfIoConstraintId, openfpga::BasicPort> io_constraint_pins_;

  /* Nets to constraint */
  vtr::vector<PcfIoConstraintId, std::string> io_constraint_nets_;
};

} /* End namespace openfpga*/

#endif
