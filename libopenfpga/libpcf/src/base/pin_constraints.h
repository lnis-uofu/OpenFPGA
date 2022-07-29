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
    /* Logic value */
    enum e_logic_level {
      LOGIC_HIGH,
      LOGIC_LOW,
      NUM_LOGIC_LEVELS
    };
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

    /* Find the pin that a net is constrained to
     * If not found, the return port will be an invalid BasicPort
     * TODO: this function will only return the first pin found in the constraint list    
     */
    openfpga::BasicPort net_pin(const std::string& net) const; 

    /* Find the default value that a net is constrained to
     * If not found, return an invalid value
     */
    e_logic_level net_default_value(const std::string& net) const; 

    /* Generate the string of the default value
     * If not found, return an empty string
     */
    std::string net_default_value_to_string(const PinConstraintId& pin_constraint) const; 

    /* Generate the integer representation of the default value
     * If not found, return -1
     */
    size_t net_default_value_to_int(const std::string& net) const;

    /* Check if there are any pin constraints */
    bool empty() const;

  public: /* Public Mutators */
    /* Reserve a number of design constraints to be memory efficent */
    void reserve_pin_constraints(const size_t& num_pin_constraints);

    /* Add a pin constraint to storage */
    PinConstraintId create_pin_constraint(const openfpga::BasicPort& pin,
                                          const std::string& net);

    /* Set the default value for the net under a given pin constraint */
    void set_net_default_value(const PinConstraintId& pin_constraint,
                               const std::string& default_value);

  public: /* Public invalidators/validators */
    /* Show if the pin constraint id is a valid for data queries */
    bool valid_pin_constraint_id(const PinConstraintId& pin_constraint_id) const;

    /* Show if the net has no constraints (free to map to any pin) 
     * This function is used to identify the net name returned by APIs:
     * - pin_net()
     * - net()
     */
    bool unconstrained_net(const std::string& net) const;

    /* Show if the net is defined specifically not to map to any pin 
     * This function is used to identify the net name returned by APIs:
     * - pin_net()
     * - net()
     */
    bool unmapped_net(const std::string& net) const;

    /* Check if default value is a valid one or not
     * This is to check if the default value is constrained or not 
     */
    bool valid_net_default_value(const PinConstraintId& pin_constraint) const;

    /* Check if default value is a valid one or not
     * This is to check if the default value is constrained or not 
     */
    bool valid_net_default_value(const std::string& net) const;
  private: /* Internal data */
    /* Unique ids for each design constraint */
    vtr::vector<PinConstraintId, PinConstraintId> pin_constraint_ids_;

    /* Pins to constraint */
    vtr::vector<PinConstraintId, openfpga::BasicPort> pin_constraint_pins_;

    /* Nets to constraint */
    vtr::vector<PinConstraintId, std::string> pin_constraint_nets_;

    /* Default value of the nets to constraint */
    vtr::vector<PinConstraintId, e_logic_level> pin_constraint_net_default_values_;
};

#endif
