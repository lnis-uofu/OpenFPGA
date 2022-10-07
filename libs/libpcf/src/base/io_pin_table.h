#ifndef IO_PIN_TABLE_H
#define IO_PIN_TABLE_H

/********************************************************************
 * This file include the declaration of pin constraints
 *******************************************************************/
#include <array>
#include <map>
#include <string>

/* Headers from vtrutil library */
#include "vtr_geometry.h"
#include "vtr_vector.h"

/* Headers from libarchfpga library */
#include "physical_types.h"

/* Headers from openfpgautil library */
#include "io_pin_table_fwd.h"
#include "openfpga_port.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A data structure to describe the I/O pin table for FPGA fabrics
 * This data structure may include a number of I/O pins
 * each of which contains the following information
 * - side: the side which this I/O locates on FPGA perimeter
 * - external_pin_name: the name of the external I/O pin (typically on a
 *packaged chip), which is exposed to end-users
 * - internal_pin_name: the name of the internal I/O pin (typically inside the
 *chip but on an FPGA fabric), which is defined in FPGA netlists
 * - direction: the direction of the internal pin, can be input, output or inout
 *
 * The following figure illustrates the relationship between external and
 *internal pins. FPGA Chip
 *                  +----------------------------------------
 *                  |                  FPGA fabric
 *                  |                +----------------------
 *                  |    +----- +    |
 *  CHIP_IO_TOP --->|--->| I/O  |--->| FPGA_IN[0]
 * (External pin)   |    | Ctrl |    | (internal pin as input)
 *                  |    |      |<---| FPGA_OUT[1]
 *                  |    +------+    | (internal pin as output)
 *
 * Typical usage:
 * --------------
 *   // Create an object
 *   IoPinTable io_pin_table;
 *   // Add a pin
 *   openfpga::BasicPort ext_pin_info("CHIP_IO_TOP", 1);
 *   openfpga::BasicPort int_pin_info("FPGA_IN", 1, 1);
 *   IoPinTableId pin_id = io_pin_table.create_io_pin(int_pin_info,
 *ext_pin_info, TOP, INPUT);
 *
 *******************************************************************/
class IoPinTable {
 public: /* Types */
  typedef vtr::vector<IoPinTableId, IoPinTableId>::const_iterator
    io_pin_table_iterator;
  /* Create range */
  typedef vtr::Range<io_pin_table_iterator> io_pin_table_range;
  /* Logic value */
  enum e_io_direction { INPUT, OUTPUT, NUM_IO_DIRECTIONS };

 public: /* Constructors */
  IoPinTable();

 public: /* Accessors: aggregates */
  /* Walk through the internal pins. We do not walk through external pins
   * because they are not unique in the table. An external pin may be accessible
   * by two internal pins
   */
  io_pin_table_range pins() const;

 public: /* Public Accessors: Basic data query */
  /* Get the basic information for a pin */
  BasicPort internal_pin(const IoPinTableId& pin_id) const;
  BasicPort external_pin(const IoPinTableId& pin_id) const;
  e_side pin_side(const IoPinTableId& pin_id) const;
  e_io_direction pin_direction(const IoPinTableId& pin_id) const;
  /* Given an external pin, find all the internal pin that is mapped */
  std::vector<IoPinTableId> find_internal_pin(
    const BasicPort& ext_pin, const e_io_direction& pin_direction) const;
  /* Check if there are any pins */
  bool empty() const;

 public: /* Public Mutators */
  /* Reserve to be memory efficent */
  void reserve_pins(const size_t& num_pins);
  /* Add a pin to storage */
  IoPinTableId create_pin();
  /* Set pin attributes */
  void set_internal_pin(const IoPinTableId& pin_id, const BasicPort& pin);
  void set_external_pin(const IoPinTableId& pin_id, const BasicPort& pin);
  void set_pin_side(const IoPinTableId& pin_id, const e_side& side);
  void set_pin_direction(const IoPinTableId& pin_id,
                         const e_io_direction& direction);

 public: /* Public invalidators/validators */
  /* Show if the pin id is a valid for data queries */
  bool valid_pin_id(const IoPinTableId& pin_id) const;

 private: /* Internal data */
  /* Unique ids for each design constraint */
  vtr::vector<IoPinTableId, IoPinTableId> pin_ids_;

  /* Pin information*/
  vtr::vector<IoPinTableId, BasicPort> internal_pins_;
  vtr::vector<IoPinTableId, BasicPort> external_pins_;
  vtr::vector<IoPinTableId, e_side> pin_sides_;
  vtr::vector<IoPinTableId, e_io_direction> pin_directions_;
};

} /* end namespace openfpga */

#endif
