#ifndef REPACK_DESIGN_CONSTRAINTS_H
#define REPACK_DESIGN_CONSTRAINTS_H

/********************************************************************
 * This file include the declaration of repack design constraints
 *******************************************************************/
#include <string>
#include <map>
#include <array>

/* Headers from vtrutil library */
#include "vtr_vector.h"
#include "vtr_geometry.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"

#include "repack_design_constraints_fwd.h"

/********************************************************************
 * A data structure to describe the design constraints for repacking tools
 * This data structure may include a number of design constraints
 * each of which may constrain:
 * - pin assignment, for instance, force a net to be mapped to specific pin
 *
 * Typical usage:
 * --------------
 *   // Create an object of design constraints
 *   RepackDesignConstraints repack_design_constraints;
 *   // Add a pin assignment
 *   RepackDesignConstraintId repack_dc_id = fabric_key.create_design_constraint(RepackDesignConstraints::PIN_ASSIGNMENT);
 *
 *******************************************************************/
class RepackDesignConstraints {
  public: /* Type of design constraints */
    enum e_design_constraint_type {
      PIN_ASSIGNMENT,
      NUM_DESIGN_CONSTRAINT_TYPES
    };
  public: /* Types */
    typedef vtr::vector<RepackDesignConstraintId, RepackDesignConstraintId>::const_iterator repack_design_constraint_iterator;
    /* Create range */
    typedef vtr::Range<repack_design_constraint_iterator> repack_design_constraint_range;
  public:  /* Constructors */
    RepackDesignConstraints();
  public: /* Accessors: aggregates */
    repack_design_constraint_range design_constraints() const;
  public: /* Public Accessors: Basic data query */
    /* Get the type of constraint */
    e_design_constraint_type type(const RepackDesignConstraintId& repack_design_constraint_id) const;

    /* Get the tile name to be constrained */
    std::string tile(const RepackDesignConstraintId& repack_design_constraint_id) const;

    /* Get the pin to be constrained */
    openfpga::BasicPort pin(const RepackDesignConstraintId& repack_design_constraint_id) const;

    /* Get the net to be constrained */
    std::string net(const RepackDesignConstraintId& repack_design_constraint_id) const;

    /* Check if there are any design constraints */
    bool empty() const;

  public: /* Public Mutators */

    /* Reserve a number of design constraints to be memory efficent */
    void reserve_design_constraints(const size_t& num_design_constraints);

    /* Add a design constraint to storage */
    RepackDesignConstraintId create_design_constraint(const e_design_constraint_type& repack_design_constraint_type);

    /* Set the tile name to be constrained */
    void set_tile(const RepackDesignConstraintId& repack_design_constraint_id,
                  const std::string& tile);

    /* Set the pin to be constrained */
    void set_pin(const RepackDesignConstraintId& repack_design_constraint_id,
                 const openfpga::BasicPort& pin);

    /* Set the net to be constrained */
    void set_net(const RepackDesignConstraintId& repack_design_constraint_id,
                 const std::string& net);

  public: /* Public invalidators/validators */
    bool valid_design_constraint_id(const RepackDesignConstraintId& repack_design_constraint_id) const;
  private: /* Internal data */
    /* Unique ids for each design constraint */
    vtr::vector<RepackDesignConstraintId, RepackDesignConstraintId> repack_design_constraint_ids_;

    /* Type for each design constraint */
    vtr::vector<RepackDesignConstraintId, e_design_constraint_type> repack_design_constraint_types_;

    /* Tiles to constraint */
    vtr::vector<RepackDesignConstraintId, std::string> repack_design_constraint_tiles_;

    /* Pins to constraint */
    vtr::vector<RepackDesignConstraintId, openfpga::BasicPort> repack_design_constraint_pins_;

    /* Nets to constraint */
    vtr::vector<RepackDesignConstraintId, std::string> repack_design_constraint_nets_;
};

#endif
