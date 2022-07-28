/******************************************************************************
 * Inspired from https://github.com/genbtc/VerilogPCFparser
 ******************************************************************************/
#include <sstream>

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
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
              const blifparse::BlifHeadReader& blif_head,
              const IoPinTable& io_pin_table,
              const IoLocationMap& io_location_map,
              IoNetPlace& io_net_place) {
  vtr::ScopedStartFinishTimer timer("Convert PCF data to VPR I/O place data");

  int num_err = 0;

  /* TODO: Validate pcf data, blif_head and io_pin_table
   * - there are no duplicated pin assignment in pcf
   * - the pin direction in io_pin_table matches the pin type defined in blif
   */
  if (!pcf_data.validate()) {
    VTR_LOG_ERROR("Invalid PCF data!\n"); 
    return 1;
  }

  /* Build the I/O place */
  for (const PcfIoConstraintId& io_id : pcf_data.io_constraints()) {
    /* Find the net name */
    std::string net = pcf_data.io_net(io_id);
    /* Find the external pin name */
    BasicPort ext_pin = pcf_data.io_pin(io_id);
    /* Find the pin direction from blif reader */
    IoPinTable::e_io_direction pin_direction = NUM_IO_DIRECTIONS;
    std::vector<std::string> input_nets = blif_head.input_pins();
    std::vector<std::string> output_nets = blif_head.input_pins();
    if (input_pins.end() != std::find(input_nets.begin(), input_nets.end(), net)) {
      pin_direction = IoPinTable::INPUT;
    } else if (output_pins.end() != std::find(output_nets.begin(), output_nets.end(), net)) {
      pin_direction = IoPinTable::OUTPUT;
    } else {
      /* Cannot find the pin, error out! */
      VTR_LOG_ERROR("Net '%s' from .pcf is neither defined as input nor output in .blif!\n",
                    net.c_str()); 
      num_err++;
      return 1;
    }
    /* Find the internal pin name from pin table */
    BasicPort int_pin = io_pin_table.find_internal_pin(ext_pin, pin_direction);
    /* Find the coordinate from io location map */
    size_t x = io_location_map.io_x(int_pin); 
    size_t y = io_location_map.io_y(int_pin); 
    size_t z = io_location_map.io_z(int_pin); 
    /* Add a fixed prefix to net namei, this is hard coded by VPR */
    if (OUTPUT == pin_direction) {
      net = "out:" + net; 
    }
    /* Add the information to I/O place data */
    io_net_place.set_net_coord(net, x, y, z);
  }

  return num_err;
}

} /* end namespace openfpga */
