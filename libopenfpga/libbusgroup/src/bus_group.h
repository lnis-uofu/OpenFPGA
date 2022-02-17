#ifndef BUS_GROUP_H
#define BUS_GROUP_H

/********************************************************************
 * This file include the declaration of pin constraints
 *******************************************************************/
#include <string>
#include <map>
#include <array>

/* Headers from vtrutil library */
#include "vtr_vector.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"

#include "bus_group_fwd.h"

/********************************************************************
 * A data structure to describe the bus-to-pin mapping
 * This data structure may include a number of buses
 * each of which has:
 * - a unique id and name
 * - a number of pins with names and index which are flatten from the bus
 *
 * Typical usage:
 * --------------
 *   // Create an object of bus group
 *   BusGroup bus_group;
 *   // Create a new port
 *   BasicPort bus_a("BusA", 0, 3)
 *   // Add a bus
 *   BusGroupId bus_group_id = bus_group.create_bus(bus_a);
 *
 *******************************************************************/
class BusGroup {
  public: /* Types */
    typedef vtr::vector<BusGroupId, BusGroupId>::const_iterator bus_group_iterator;
    /* Create range */
    typedef vtr::Range<bus_group_iterator> bus_group_range;
  public:  /* Constructors */
    BusGroup();
  public: /* Accessors: aggregates */
    bus_group_range buses() const;
  public: /* Public Accessors: Basic data query */
    /** Get port information of a bus with a given id */
    openfpga::BasicPort BusGroup::bus_port(const BusGroupId& bus_id) const;

    /* Check if there are any buses */
    bool empty() const;

  public: /* Public Mutators */
    /* Reserve a number of buses to be memory efficent */
    void reserve_buses(const size_t& num_buses);

    /* Add a bus to storage */
    BusGroupId create_bus(const openfpga::BasicPort& bus_port);

  public: /* Public invalidators/validators */
    /* Show if the pin constraint id is a valid for data queries */
    bool valid_bus_id(const BusGroupId& bus_id) const;

  private: /* Internal data */
    /* Unique ids for each bus */
    vtr::vector<BusGroupId, BusGroupId> bus_ids_;

    /* Port information of each bus */
    vtr::vector<BusGroupId, openfpga::BasicPort> bus_ports_;

    /* Indices of each pin under each bus */
    vtr::vector<BusGroupId, std::vector<int>> bus_pin_indices_;

    /* Name of each pin under each bus */
    vtr::vector<BusGroupId, std::vector<std::string>> bus_pin_names_;
};

#endif
