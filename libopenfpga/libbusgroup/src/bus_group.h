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
    typedef vtr::vector<BusPinId, BusPinId>::const_iterator bus_pin_iterator;
    /* Create range */
    typedef vtr::Range<bus_group_iterator> bus_group_range;
    typedef vtr::Range<bus_pin_iterator> bus_pin_range;
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

    /* Reserve a number of pins to be memory efficent */
    void reserve_pins(const size_t& num_pins);

    /* Add a bus to storage */
    BusGroupId create_bus(const openfpga::BasicPort& bus_port);

    /* Add a pin to a bus */
    BusPinId create_pin(const BusGroupId& bus_id);

    /* Set the index for a pin */
    void set_pin_index(const BusPinId& pin_id, const int& index);

    /* Set the name for a pin */
    void set_pin_index(const BusPinId& pin_id, const std::string& name);

  public: /* Public invalidators/validators */
    /* Show if the bus id is a valid for data queries */
    bool valid_bus_id(const BusGroupId& bus_id) const;

    /* Show if the pin id is a valid for data queries */
    bool valid_pin_id(const BusPinId& pin_id) const;

  private: /* Internal data */
    /* Unique ids for each bus */
    vtr::vector<BusGroupId, BusGroupId> bus_ids_;

    /* Port information of each bus */
    vtr::vector<BusGroupId, openfpga::BasicPort> bus_ports_;

    /* Indices of each pin under each bus */
    vtr::vector<BusGroupId, std::vector<BusPinId>> bus_pin_ids_;

    /* Unique ids for each pin */
    vtr::vector<BusPinId, BusPinId> pin_ids_;

    /* Index for each pin */
    vtr::vector<BusPinId, int> pin_indices_;

    /* Name of each pin under each bus */
    vtr::vector<BusPinId, std::string> pin_names_;
};

#endif
