#pragma once

/********************************************************************
 * This file include the declaration of boundary timing
 *******************************************************************/
#include <array>
#include <map>
#include <string>

/* Headers from vtrutil library */
#include "vtr_geometry.h"
#include "vtr_vector.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"
#include "pin_constraints_fwd.h"
/********************************************************************
 * A data structure to describe the boundary timing of pins.
 * The boundary timing are the maximum and minimum input/output delays outside
the eFPGA. It shows the external timing condition which must be considered when
running STA on a mapped eFPGA.
 *******************************************************************/
namespace openfpga {
class BoundaryTiming {
  /********************************************************************
  PinConstraintId is a strong identifier that is shared across multiple data
  structures. Developers must ensure that PinConstraintId values are globally
  consistent and interchangeable between these data structures within a single
  run.

  In particular, a run must be based on exactly one pin table and one PCF file.
  Providing multiple pin tables or multiple PCF files in the same run will break
  the consistency assumptions of PinConstraintId and may lead to undefined
  behavior.
  *******************************************************************/
 public: /* Types */
  typedef vtr::vector<PinConstraintId, PinConstraintId>::const_iterator
    pin_constraint_iterator;
  /* Create range */
  typedef vtr::Range<pin_constraint_iterator> pin_constraint_range;
  /* Logic value */

 public: /* Constructors */
  BoundaryTiming();

 public: /* Accessors: aggregates */
  pin_constraint_range pin_constraints() const;

 public: /* Public Accessors: Basic data query */
  /* Get the pin to be constrained */
  openfpga::BasicPort pin(const PinConstraintId& pin_constraint_id) const;

  std::string max_delay(const PinConstraintId& pin_constraint_id) const;

  std::string min_delay(const PinConstraintId& pin_constraint_id) const;

  std::string pin_max_delay(const openfpga::BasicPort& pin) const;

  std::string pin_min_delay(const openfpga::BasicPort& pin) const;

  bool pin_delay_constrained(const openfpga::BasicPort& pin) const;
  /* Check if there are any pin constraints */
  bool empty() const;

 public: /* Public Mutators */
  /* Reserve a number of design constraints to be memory efficent */
  void reserve_pin_constraints(const size_t& num_pin_constraints);

  /* Add a pin constraint to storage */
  PinConstraintId create_pin_boundary_timing(const openfpga::BasicPort& pin,
                                             const std::string& max_delay,
                                             const std::string min_delay);

 public: /* Public invalidators/validators */
  /* Show if the pin constraint id is a valid for data queries */
  bool valid_pin_constraint_id(const PinConstraintId& pin_constraint_id) const;

 private: /* Internal data */
  /* Unique ids for each design constraint */
  vtr::vector<PinConstraintId, PinConstraintId> pin_constraint_ids_;

  /* Pins to constraint */
  vtr::vector<PinConstraintId, openfpga::BasicPort> pin_constraint_pins_;

  /* Nets to constraint */
  vtr::vector<PinConstraintId, std::string> pin_constraint_max_delay_;
  vtr::vector<PinConstraintId, std::string> pin_constraint_min_delay_;
};
}  // namespace openfpga