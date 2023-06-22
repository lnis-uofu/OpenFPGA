/******************************************************************************
 * Memember functions for data structure IoLocationMap
 ******************************************************************************/
/* Headers from vtrutil library */
#include "io_name_map.h"

#include <algorithm>

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Accessors
 *************************************************/
std::vector<BasicPort> IoNameMap::fpga_top_ports() const {
  std::vector<BasicPort> ports;

  for (auto it = top2core_io_name_map_.begin();
       it != top2core_io_name_map_.end(); ++it) {
    ports.push_back(it->first);
  }

  return ports;
}

BasicPort IoNameMap::fpga_core_port(const BasicPort& fpga_top_port) const {
  BasicPort core_port;
  auto result = top2core_io_name_map_.find(fpga_top_port);
  if (result != top2core_io_name_map_.end()) {
    core_port = result->second;
  }
  return core_port;
}

BasicPort IoNameMap::fpga_top_port(const BasicPort& fpga_core_port) const {
  BasicPort top_port;
  auto result = core2top_io_name_map_.find(fpga_core_port);
  if (result != core2top_io_name_map_.end()) {
    top_port = result->second;
  }
  return top_port;
}

bool IoNameMap::fpga_top_port_is_dummy(const BasicPort& fpga_top_port) const {
  return !fpga_core_port(fpga_top_port).is_valid();
}

int IoNameMap::set_io_pair(const BasicPort& fpga_top_port,
                           const BasicPort& fpga_core_port) {
  /* Ensure the two ports are matching in size */
  if (fpga_top_port.get_width() != fpga_core_port.get_width()) {
    VTR_LOG_ERROR(
      "Unable to pair two ports 'fpga_top.%s[%lu:%lu]' and "
      "'fpga_core.%s[%lu:%lu]' which are in the same size!\n",
      fpga_top_port.get_name().c_str(), fpga_top_port.get_lsb(),
      fpga_top_port.get_msb(), fpga_core_port.get_name().c_str(),
      fpga_core_port.get_lsb(), fpga_core_port.get_msb());
    return CMD_EXEC_FATAL_ERROR;
  }
  VTR_ASSERT_SAFE(fpga_top_port.get_width() != fpga_core_port.get_width());
  for (size_t ipin = 0; ipin < fpga_top_port.pins().size(); ++ipin) {
    BasicPort top_pin(fpga_top_port.get_name(), fpga_top_port.pins()[ipin],
                      fpga_top_port.pins()[ipin]);
    BasicPort core_pin(fpga_core_port.get_name(), fpga_core_port.pins()[ipin],
                       fpga_core_port.pins()[ipin]);
    top2core_io_name_map_[top_pin] = core_pin;
    core2top_io_name_map_[core_pin] = top_pin;
  }
  return CMD_EXEC_SUCCESS;
}

int IoNameMap::set_dummy_io(const BasicPort& fpga_top_port) {
  /* Must be a true dummy port, none of its pins have been paired! */
  for (size_t ipin = 0; ipin < fpga_top_port.pins().size(); ++ipin) {
    BasicPort top_pin(fpga_top_port.get_name(), fpga_top_port.pins()[ipin],
                      fpga_top_port.pins()[ipin]);
    auto result = top2core_io_name_map_.find(top_pin);
    if (result != top2core_io_name_map_.end() && result->second.is_valid()) {
      VTR_LOG_ERROR(
        "Pin '%lu' in a dummy port '%s[%lu:%lu]' of fpga_top is already mapped "
        "to a valid pin '%s[%lu:%lu]' of fpga_core!\n",
        top_pin.get_lsb(), fpga_top_port.get_name().c_str(),
        fpga_top_port.get_lsb(), fpga_top_port.get_msb(),
        result->second.get_name().c_str(), result->second.get_lsb(),
        result->second.get_msb());
      return CMD_EXEC_FATAL_ERROR;
    }
    top2core_io_name_map_[top_pin] = BasicPort();
  }
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
