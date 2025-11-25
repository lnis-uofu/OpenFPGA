#ifndef PCF_CONFIG_H
#define PCF_CONFIG_H

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
class PcfCustomConstraint {
 public: /* Types */
  typedef vtr::vector<PcfCustomConstraintId,
                      PcfCustomConstraintId>::const_iterator
    pcf_custom_constraint_iterator;
  /* Create range */
  typedef vtr::Range<pcf_custom_constraint_iterator>
    pcf_custom_constraint_range;

 public: /* Constructors */
  PcfCustomConstraint();

 public: /* Accessors: aggregates */
  pcf_custom_constraint_range custom_constraints() const;

 public: /* Public Accessors: Basic data query */
  /* Get the pin to be constrained */

  openfpga::BasicPort custom_constraint_pin(
    const PcfCustomConstraintId& custom_constraint_id) const;
  /* Check if there are any io constraints */
  bool empty() const;

 public: /* Public Mutators */
  /* Reserve a number of design constraints to be memory efficent */

  PcfCustomConstraintId create_custom_constraint();

  void set_custom_constraint_value(
    const PcfCustomConstraintId& costum_constraint_id,
    const std::string& value);

  void set_custom_constraint_option(
    const PcfCustomConstraintId& costum_constraint_id,
    const std::string& option);

  void set_custom_constraint_pin(
    const PcfCustomConstraintId& costum_constraint_id, const std::string& pin);

 public: /* Public invalidators/validators */
  /* Show if the constraint id is a valid for data queries */
  bool valid_custom_constraint_id(
    const PcfCustomConstraintId& custom_constraint_id) const;

 private: /* Internal data */
  vtr::vector<PcfCustomConstraintId, PcfCustomConstraintId>
    custom_constraint_ids_;

  vtr::vector<PcfCustomConstraintId, openfpga::BasicPort>
    custom_constraint_pins_;

  vtr::vector<PcfCustomConstraintId, std::string> custom_constraint_options_;

  vtr::vector<PcfCustomConstraintId, std::string> custom_constraint_values_;
};

} /* End namespace openfpga*/

#endif
