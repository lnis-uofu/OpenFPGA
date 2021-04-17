#ifndef PIN_CONSTRAINTS_H
#define PIN_CONSTRAINTS_H

/********************************************************************
 * This file include the declaration of pin constraints
 *******************************************************************/
#include <string>
#include <map>
#include <array>

/* Headers from vtrutil library */
#include "vtr_vector.h"
#include "vtr_geometry.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"

#include "pin_constraints_fwd.h"

/* Constants */
constexpr char* PIN_CONSTRAINT_OPEN_NET = "OPEN";

/********************************************************************
 * A data structure to describe the pin constraints for FPGA fabrics
 * This data structure may include a number of pin constraints
 * each of which may constrain:
 * - pin assignment, for instance, force a net to be mapped to specific pin
 *
 * Typical usage:
 * --------------
 *   // Create an object of pin constraints
 *   PinConstraints pin_constraints;
 *   // Add a pin assignment
 *   openfpga::BasicPort pin_info(clk, 1);
 *   std::string net_info("top_clock");
 *   PinConstraintId pin_constraint_id = pin_constraints.create_pin_constraint(pin_info, net_info);
 *
 *******************************************************************/
class PinConstraints {
  public: /* Types */
    typedef vtr::vector<PinConstraintId, PinConstraintId>::const_iterator pin_constraint_iterator;
    /* Create range */
    typedef vtr::Range<pin_constraint_iterator> pin_constraint_range;
  public:  /* Constructors */
    PinConstraints();
  public: /* Accessors: aggregates */
    pin_constraint_range pin_constraints() const;
  public: /* Public Accessors: Basic data query */
    /* Get the pin to be constrained */
    openfpga::BasicPort pin(const PinConstraintId& pin_constraint_id) const;

    /* Get the net to be constrained */
    std::string net(const PinConstraintId& pin_constraint_id) const;

    /* Find the net that is constrained on a pin
     * TODO: this function will only return the first net found in the constraint list    
     */
    std::string pin_net(const openfpga::BasicPort& pin) const;

    /* Check if there are any pin constraints */
    bool empty() const;

  public: /* Public Mutators */

    /* Reserve a number of design constraints to be memory efficent */
    void reserve_pin_constraints(const size_t& num_pin_constraints);

    /* Add a pin constraint to storage */
    PinConstraintId create_pin_constraint(const openfpga::BasicPort& pin,
                                          const std::string& net);

  public: /* Public invalidators/validators */
    bool valid_pin_constraint_id(const PinConstraintId& pin_constraint_id) const;
  private: /* Internal data */
    /* Unique ids for each design constraint */
    vtr::vector<PinConstraintId, PinConstraintId> pin_constraint_ids_;

    /* Pins to constraint */
    vtr::vector<PinConstraintId, openfpga::BasicPort> pin_constraint_pins_;

    /* Nets to constraint */
    vtr::vector<PinConstraintId, std::string> pin_constraint_nets_;
};

#endif
