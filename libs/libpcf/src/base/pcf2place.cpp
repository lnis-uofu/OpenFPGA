/******************************************************************************
 * Inspired from https://github.com/genbtc/VerilogPCFparser
 ******************************************************************************/
#include <sstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
#include "pcf2place.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Generate a .place file with the a few inputs
 *
 * Return 0 if successful
 * Return 1 if there are serious errors
 *******************************************************************/
int pcf2place(const PcfData& pcf_data,
              const std::vector<std::string>& input_nets,
              const std::vector<std::string>& output_nets,
              const IoPinTable& io_pin_table,
              const IoLocationMap& io_location_map, IoNetPlace& io_net_place) {
  vtr::ScopedStartFinishTimer timer("Convert PCF data to VPR I/O place data");

  int num_err = 0;

  /* TODO: Validate pcf data, blif_head and io_pin_table
   * - there are no duplicated pin assignment in pcf
   * - the pin direction in io_pin_table matches the pin type defined in blif
   */
  if (!pcf_data.validate()) {
    VTR_LOG_ERROR("PCF contains invalid I/O assignment!\n");
    return 1;
  } else {
    VTR_LOG("PCF basic check passed\n");
  }

  /* Map from location to net */
  std::map<std::array<size_t, 3>, std::string> net_map;
  /* Build the I/O place */
  for (const PcfIoConstraintId& io_id : pcf_data.io_constraints()) {
    /* Find the net name */
    std::string net = pcf_data.io_net(io_id);
    /* Find the external pin name */
    BasicPort ext_pin = pcf_data.io_pin(io_id);
    /* Find the pin direction from blif reader */
    IoPinTable::e_io_direction pin_direction = IoPinTable::NUM_IO_DIRECTIONS;
    if (input_nets.end() !=
        std::find(input_nets.begin(), input_nets.end(), net)) {
      pin_direction = IoPinTable::INPUT;
    } else if (output_nets.end() !=
               std::find(output_nets.begin(), output_nets.end(), net)) {
      pin_direction = IoPinTable::OUTPUT;
    } else {
      /* Cannot find the pin, error out! */
      VTR_LOG_ERROR(
        "Net '%s' from .pcf is neither defined as input nor output in .blif!\n",
        net.c_str());
      num_err++;
      continue;
    }
    /* Find the internal pin name from pin table, currently we only support
     * 1-to-1 mapping */
    auto int_pin_ids = io_pin_table.find_internal_pin(ext_pin, pin_direction);
    if (0 == int_pin_ids.size()) {
      VTR_LOG_ERROR(
        "Cannot find any internal pin that net '%s' is mapped through an "
        "external pin '%s[%lu]'!\n",
        net.c_str(), ext_pin.get_name().c_str(), ext_pin.get_lsb());
      num_err++;
      continue;
    } else if (1 < int_pin_ids.size()) {
      VTR_LOG_ERROR(
        "Found multiple internal pins that net '%s' is mapped through an "
        "external pin '%s[%lu]'! Please double check your pin table!\n",
        net.c_str(), ext_pin.get_name().c_str(), ext_pin.get_lsb());
      for (auto int_pin_id : int_pin_ids) {
        VTR_LOG("%s[%ld]\n",
                io_pin_table.internal_pin(int_pin_id).get_name().c_str(),
                io_pin_table.internal_pin(int_pin_id).get_lsb());
      }
      num_err++;
      continue;
    }
    VTR_ASSERT(1 == int_pin_ids.size());
    BasicPort int_pin = io_pin_table.internal_pin(int_pin_ids[0]);
    /* Find the coordinate from io location map */
    size_t x = io_location_map.io_x(int_pin);
    size_t y = io_location_map.io_y(int_pin);
    size_t z = io_location_map.io_z(int_pin);
    /* Sanity check */
    if (size_t(-1) == x || size_t(-1) == y || size_t(-1) == z) {
      VTR_LOG_ERROR(
        "Invalid coordinate (%ld, %ld, %ld) found for net '%s' mapped to an "
        "external pin '%s[%lu]' through an internal pin '%s[%lu]'!\n",
        x, y, z, net.c_str(), ext_pin.get_name().c_str(), ext_pin.get_lsb(),
        int_pin.get_name().c_str(), int_pin.get_lsb());
      continue;
    }

    std::array<size_t, 3> loc = {x, y, z};
    auto itr = net_map.find(loc);
    if (itr == net_map.end()) {
      net_map.insert({loc, net});
    } else {
      VTR_LOG_ERROR(
        "Illegal pin constraint: Two nets '%s' and '%s' are mapped to the I/O "
        "pin '%s[%lu]' which belongs to the same coordinate (%ld, %ld, %ld)!\n",
        itr->second.c_str(), net.c_str(), int_pin.get_name().c_str(),
        int_pin.get_lsb(), x, y, z);
      num_err++;
      continue;
    }

    /* Add a fixed prefix to net namei, this is hard coded by VPR */
    if (IoPinTable::OUTPUT == pin_direction) {
      net = "out:" + net;
    }
    /* Add the information to I/O place data */
    io_net_place.set_net_coord(net, x, y, z);
  }

  return num_err;
}

} /* end namespace openfpga */
