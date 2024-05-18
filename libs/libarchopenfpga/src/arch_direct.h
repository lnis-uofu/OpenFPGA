#ifndef ARCH_DIRECT_H
#define ARCH_DIRECT_H

#include <array>
#include <map>

#include "arch_direct_fwd.h"
#include "circuit_library_fwd.h"
#include "vtr_geometry.h"
#include "vtr_vector.h"

/********************************************************************
 * Define the types of point to point connection between CLBs
 * These types are supplementary to the original VPR direct connections
 * Here we extend to the cross-row and cross-column connections
 ********************************************************************/
enum class e_direct_type {
  INNER_COLUMN_OR_ROW,
  PART_OF_CB,
  INTER_COLUMN,
  INTER_ROW,
  NUM_DIRECT_TYPES
};
constexpr std::array<const char*, (size_t)e_direct_type::NUM_DIRECT_TYPES>
  DIRECT_TYPE_STRING = {
    {"inner_column_or_row", "part_of_cb", "inter_column", "inter_row"}};

enum e_direct_direction { POSITIVE_DIR, NEGATIVE_DIR, NUM_DIRECT_DIRECTIONS };
constexpr std::array<const char*, NUM_DIRECT_DIRECTIONS>
  DIRECT_DIRECTION_STRING = {{"positive", "negative"}};

/********************************************************************
 * A data base to describe the direct connection in OpenFPGA architecture
 * For each direct connection, it will include
 * - name: the identifier to annotate the original direct connection in VPR
 *architecture
 * - circuit model: the circuit model to used to implement this connection
 * - type: if this connection should be cross-column or cross-row
 * - x-direction: how this connection is going to be applied to adjacent columns
 *                a positive x-direction means that column A will be connected
 *                to the column B on the right side of column A
 * - y-direction: how this connection is going to be applied to adjacent rows
 *                a positive y-direction means that row A will be connected
 *                to the row B on the bottom side of row A
 *
 * Note that: this is the data structure to be used when parsing the XML
 *            this is NOT the data structure to be use in core engine
 ********************************************************************/
class ArchDirect {
 public: /* Types */
  typedef vtr::vector<ArchDirectId, ArchDirectId>::const_iterator
    arch_direct_iterator;
  /* Create range */
  typedef vtr::Range<arch_direct_iterator> arch_direct_range;

 public: /* Constructors */
  ArchDirect();

 public: /* Accessors: aggregates */
  arch_direct_range directs() const;

 public: /* Public Accessors: Basic data query on directs */
  ArchDirectId direct(const std::string& name) const;
  std::string name(const ArchDirectId& direct_id) const;
  CircuitModelId circuit_model(const ArchDirectId& direct_id) const;
  e_direct_type type(const ArchDirectId& direct_id) const;
  e_direct_direction x_dir(const ArchDirectId& direct_id) const;
  e_direct_direction y_dir(const ArchDirectId& direct_id) const;

 public: /* Public Mutators */
  ArchDirectId add_direct(const std::string& name);
  void set_circuit_model(const ArchDirectId& direct_id,
                         const CircuitModelId& circuit_model);
  void set_type(const ArchDirectId& direct_id, const e_direct_type& type);
  void set_direction(const ArchDirectId& direct_id,
                     const e_direct_direction& x_dir,
                     const e_direct_direction& y_dir);

 public: /* Public invalidators/validators */
  bool valid_direct_id(const ArchDirectId& direct_id) const;

 private: /* Internal data */
  vtr::vector<ArchDirectId, ArchDirectId> direct_ids_;

  /* Unique name: the identifier to annotate the original direct connection in
   * VPR architecture */
  vtr::vector<ArchDirectId, std::string> names_;

  /* circuit model: the circuit model to used to implement this connection */
  vtr::vector<ArchDirectId, CircuitModelId> circuit_models_;

  /* type: if this connection should be cross-column or cross-row */
  vtr::vector<ArchDirectId, e_direct_type> types_;

  /*
   * x-direction: how this connection is going to be applied to adjacent columns
   *              a positive x-direction means that column A will be connected
   *              to the column B on the right side of column A
   * y-direction: how this connection is going to be applied to adjacent rows
   *              a positive y-direction means that row A will be connected
   *              to the row B on the bottom side of row A
   */
  vtr::vector<ArchDirectId, vtr::Point<e_direct_direction>> directions_;

  /* Fast look-up */
  std::map<std::string, ArchDirectId> direct_name2ids_;
};

#endif
