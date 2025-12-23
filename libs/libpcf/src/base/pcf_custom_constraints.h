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

  std::string custom_constraint_pb_type(
    const PcfCustomConstraintId& custom_constraint_id) const;

  std::vector<std::string> custom_constraint_mode(
    const PcfCustomConstraintId& custom_constraint_id) const;
  std::vector<int> custom_constraint_mode_offset(
    const PcfCustomConstraintId& custom_constraint_id) const;
  /* Check if there are any io constraints */
  bool empty() const;

 public: /* Public Mutators */
  /* Reserve a number of design constraints to be memory efficent */

  PcfCustomConstraintId create_custom_constraint();

  void set_custom_constraint_pin_mode(
    const PcfCustomConstraintId& costum_constraint_id, const std::string& mode);
  void set_custom_constraint_pin_mode_offset(
    const PcfCustomConstraintId& custom_constraint_id, const int& mode_offset);
  void set_custom_constraint_command(
    const PcfCustomConstraintId& custom_constraint_id,
    const std::string& command_name);

  void set_custom_constraint_pin(
    const PcfCustomConstraintId& costum_constraint_id, const std::string& pin);

  void set_custom_constraint_pb_type(
    const PcfCustomConstraintId& custom_constraint_id,
    const std::string& pb_type);

 public: /* Public invalidators/validators */
  /* Show if the constraint id is a valid for data queries */
  bool valid_custom_constraint_id(
    const PcfCustomConstraintId& custom_constraint_id) const;

 private: /* Internal data */
  vtr::vector<PcfCustomConstraintId, PcfCustomConstraintId>
    custom_constraint_ids_;

  vtr::vector<PcfCustomConstraintId, openfpga::BasicPort>
    custom_constraint_pins_;
  vtr::vector<PcfCustomConstraintId, std::string> custom_constraint_pb_type_;

  vtr::vector<PcfCustomConstraintId, std::string>
    custom_constraint_command_name_;

  vtr::vector<PcfCustomConstraintId, std::vector<std::string>>
    custom_constraint_pin_mode_;
  vtr::vector<PcfCustomConstraintId, std::vector<int>>
    custom_constraint_pin_mode_offset_;
};

} /* End namespace openfpga*/

#endif
