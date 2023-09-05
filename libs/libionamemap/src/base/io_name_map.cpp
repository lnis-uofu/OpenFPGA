/******************************************************************************
 * Memember functions for data structure IoLocationMap
 ******************************************************************************/
/* Headers from vtrutil library */
#include "io_name_map.h"

#include <algorithm>

#include "command_exit_codes.h"
#include "openfpga_port_parser.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

IoNameMap::IoNameMap() {
  DUMMY_PORT_DIR_STRING_ = {"input", "output", "inout"};
}

/**************************************************
 * Public Accessors
 *************************************************/
std::vector<BasicPort> IoNameMap::fpga_top_ports() const {
  std::vector<BasicPort> ports;

  for (auto it = top2core_io_name_map_.begin();
       it != top2core_io_name_map_.end(); ++it) {
    ports.push_back(str2port(it->first));
  }

  return ports;
}

BasicPort IoNameMap::fpga_core_port(const BasicPort& fpga_top_port) const {
  BasicPort core_port;
  /* First, find the pin name matching */
  auto result_key = top2core_io_name_keys_.find(fpga_top_port.get_name());
  if (result_key == top2core_io_name_keys_.end()) {
    return core_port; /* Not found, return invalid port */
  }
  /* Second, find the exact key */
  std::string top_port_key;
  for (std::string cand : result_key->second) {
    BasicPort cand_port = str2port(cand);
    /* if the top port is part of the cand port, e.g., clk[1] vs. clk[0:2], the
     * candidate is the key that we want! */
    if (cand_port.contained(fpga_top_port)) {
      top_port_key = cand;
      break;
    }
  }
  if (top_port_key.empty()) {
    return core_port; /* Not found, return invalid port */
  }
  auto result = top2core_io_name_map_.find(top_port_key);
  if (result != top2core_io_name_map_.end() && result->second.is_valid()) {
    BasicPort top_port_pool = str2port(top_port_key);
    BasicPort fpga_top_port_lsb(fpga_top_port.get_name(),
                                fpga_top_port.get_lsb(),
                                fpga_top_port.get_lsb());
    BasicPort fpga_top_port_msb(fpga_top_port.get_name(),
                                fpga_top_port.get_msb(),
                                fpga_top_port.get_msb());
    size_t ipin_anchor_lsb = top_port_pool.find_ipin(fpga_top_port_lsb);
    size_t ipin_anchor_msb = top_port_pool.find_ipin(fpga_top_port_msb);
    /* Now find the exact pin and spot the core port with pin index */
    if (ipin_anchor_lsb < top_port_pool.get_width() &&
        ipin_anchor_msb < top_port_pool.get_width()) {
      core_port.set_name(result->second.get_name());
      core_port.set_lsb(result->second.pins()[ipin_anchor_lsb]);
      core_port.set_msb(result->second.pins()[ipin_anchor_msb]);
    }
  }
  return core_port;
}

BasicPort IoNameMap::fpga_top_port(const BasicPort& fpga_core_port) const {
  BasicPort top_port;
  /* First, find the pin name matching */
  auto result_key = core2top_io_name_keys_.find(fpga_core_port.get_name());
  if (result_key == core2top_io_name_keys_.end()) {
    return top_port; /* Not found, return invalid port */
  }
  /* Second, find the exact key */
  std::string core_port_key;
  for (std::string cand : result_key->second) {
    BasicPort cand_port = str2port(cand);
    /* if the top port is part of the cand port, e.g., clk[1] vs. clk[0:2], the
     * candidate is the key that we want! */
    if (cand_port.contained(fpga_core_port)) {
      core_port_key = cand;
      break;
    }
  }
  if (core_port_key.empty()) {
    return top_port; /* Not found, return invalid port */
  }
  auto result = core2top_io_name_map_.find(core_port_key);
  if (result != core2top_io_name_map_.end() && result->second.is_valid()) {
    BasicPort core_port_pool = str2port(core_port_key);
    BasicPort fpga_core_port_lsb(fpga_core_port.get_name(),
                                 fpga_core_port.get_lsb(),
                                 fpga_core_port.get_lsb());
    BasicPort fpga_core_port_msb(fpga_core_port.get_name(),
                                 fpga_core_port.get_msb(),
                                 fpga_core_port.get_msb());
    size_t ipin_anchor_lsb = core_port_pool.find_ipin(fpga_core_port);
    size_t ipin_anchor_msb = core_port_pool.find_ipin(fpga_core_port);
    /* Now find the exact pin and spot the core port with pin index */
    if (ipin_anchor_lsb < core_port_pool.get_width() &&
        ipin_anchor_msb < core_port_pool.get_width()) {
      top_port.set_name(result->second.get_name());
      top_port.set_lsb(result->second.pins()[ipin_anchor_lsb]);
      top_port.set_msb(result->second.pins()[ipin_anchor_msb]);
    }
  }
  return top_port;
}

IoNameMap::e_port_mapping_status IoNameMap::fpga_core_port_mapping_status(
  const BasicPort& fpga_core_port, const bool& verbose) const {
  /* First, find the pin name matching */
  auto result_key = core2top_io_name_keys_.find(fpga_core_port.get_name());
  if (result_key == core2top_io_name_keys_.end()) {
    return IoNameMap::e_port_mapping_status::NONE;
  }
  /* Second, find the exact port. Create a scoreboard and check every pin.
   * Expect only one hit per pin. Error on any pin which has been hit twice
   * (indicate overlapped ports). Error on any pin which has no hit (indicate
   * partially unmapped) */
  std::vector<int8_t> scoreboard(fpga_core_port.get_width(), 0);
  for (std::string cand : result_key->second) {
    BasicPort cand_port = str2port(cand);
    for (auto pin : cand_port.pins()) {
      scoreboard[pin - fpga_core_port.get_lsb()]++;
    }
  }
  for (int8_t bit : scoreboard) {
    if (bit == 0) {
      VTR_LOGV_ERROR(verbose,
                     "Unmapped pin '%lu' of fpga_core port '%s'! Partially "
                     "mapping is not allowed\n",
                     fpga_core_port.pins()[bit + fpga_core_port.get_lsb()],
                     fpga_core_port.to_verilog_string().c_str());
      return IoNameMap::e_port_mapping_status::PARTIAL;
    }
    if (bit > 1) {
      VTR_LOGV_ERROR(verbose,
                     "Overlapped %d times on pin '%lu' of fpga_core port '%s' "
                     "when mapping!\n",
                     bit, fpga_core_port.pins()[bit + fpga_core_port.get_lsb()],
                     fpga_core_port.to_verilog_string().c_str());
      return IoNameMap::e_port_mapping_status::OVERLAPPED;
    }
  }
  return IoNameMap::e_port_mapping_status::FULL;
}

bool IoNameMap::fpga_top_port_is_dummy(const BasicPort& fpga_top_port) const {
  return !fpga_core_port(fpga_top_port).is_valid();
}

IoNameMap::e_dummy_port_direction IoNameMap::fpga_top_dummy_port_direction(
  const BasicPort& fpga_top_port) const {
  for (auto& kv : dummy_port_direction_) {
    BasicPort cand = str2port(kv.first);
    if (cand.contained(fpga_top_port)) {
      return kv.second;
    }
  }
  /* Return an invalid port type */
  return IoNameMap::e_dummy_port_direction::NUM_TYPES;
}

bool IoNameMap::empty() const {
  return top2core_io_name_keys_.empty() && top2core_io_name_map_.empty() &&
         core2top_io_name_keys_.empty() && core2top_io_name_map_.empty() &&
         dummy_port_direction_.empty();
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
  /* Register in the key first, and then add to the exact name mapping */
  {
    std::string top_port_str = port2str(fpga_top_port);
    auto result_key = top2core_io_name_keys_.find(fpga_top_port.get_name());
    if (result_key == top2core_io_name_keys_.end()) {
      /* Add to the key registery */
      top2core_io_name_keys_[fpga_top_port.get_name()].push_back(top_port_str);
      top2core_io_name_map_[top_port_str] = fpga_core_port;
    } else {
      /* Ensure that the key is not duplicated */
      if (std::find(result_key->second.begin(), result_key->second.end(),
                    top_port_str) == result_key->second.end()) {
        top2core_io_name_keys_[fpga_top_port.get_name()].push_back(
          top_port_str);
        top2core_io_name_map_[top_port_str] = fpga_core_port;
      } else {
        /* Throw a warning since we have to overwrite */
        VTR_LOG_WARN(
          "Overwrite the top-to-core pin mapping: top pin '%s' to core pin "
          "'%s' (previously was '%s')!\n",
          top_port_str.c_str(), port2str(fpga_core_port).c_str(),
          port2str(top2core_io_name_map_[top_port_str]).c_str());
        top2core_io_name_map_[top_port_str] = fpga_core_port;
      }
    }
  }
  /* Now, do similar to the core port */
  {
    std::string core_port_str = port2str(fpga_core_port);
    auto result_key = core2top_io_name_keys_.find(fpga_core_port.get_name());
    if (result_key == core2top_io_name_keys_.end()) {
      /* Add to the key registery */
      core2top_io_name_keys_[fpga_core_port.get_name()].push_back(
        core_port_str);
      core2top_io_name_map_[core_port_str] = fpga_top_port;
    } else {
      /* Ensure that the key is not duplicated */
      if (std::find(result_key->second.begin(), result_key->second.end(),
                    core_port_str) == result_key->second.end()) {
        core2top_io_name_keys_[fpga_core_port.get_name()].push_back(
          core_port_str);
        core2top_io_name_map_[core_port_str] = fpga_top_port;
      } else {
        /* Throw a warning since we have to overwrite */
        VTR_LOG_WARN(
          "Overwrite the core-to-top pin mapping: core pin '%s' to top pin "
          "'%s' (previously was '%s')!\n",
          core_port_str.c_str(), port2str(fpga_top_port).c_str(),
          port2str(core2top_io_name_map_[core_port_str]).c_str());
        core2top_io_name_map_[core_port_str] = fpga_top_port;
      }
    }
  }
  return CMD_EXEC_SUCCESS;
}

int IoNameMap::set_dummy_io(const BasicPort& fpga_top_port,
                            const e_dummy_port_direction& direction) {
  /* Must be a true dummy port, none of its pins have been paired! */
  std::string top_port_str = port2str(fpga_top_port);
  /* First, find the pin name matching */
  auto result_key = top2core_io_name_keys_.find(fpga_top_port.get_name());
  if (result_key == top2core_io_name_keys_.end()) {
    /* Add to the key registery */
    top2core_io_name_keys_[fpga_top_port.get_name()].push_back(top_port_str);
    top2core_io_name_map_[top_port_str] = BasicPort();
  } else {
    /* Ensure that the key is not duplicated */
    if (std::find(result_key->second.begin(), result_key->second.end(),
                  top_port_str) == result_key->second.end()) {
      top2core_io_name_keys_[fpga_top_port.get_name()].push_back(top_port_str);
      top2core_io_name_map_[top_port_str] = BasicPort();
    } else {
      /* Throw a error because the dummy pin should NOT be mapped before! */
      VTR_LOG_ERROR(
        "Dummy port '%s' of fpga_top is already mapped "
        "to a valid pin '%s' of fpga_core!\n",
        port2str(fpga_top_port).c_str(),
        port2str(top2core_io_name_map_[top_port_str]).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  /* Add the direction list */
  bool dir_defined = false;
  for (auto& kv : dummy_port_direction_) {
    BasicPort cand = str2port(kv.first);
    if (cand.contained(fpga_top_port)) {
      if (kv.second != direction) {
        /* Throw a error because the dummy pin should NOT be mapped before! */
        VTR_LOG_ERROR(
          "Dummy port '%s' of fpga_top is already assigned to a different "
          "direction through another dummy port definition '%s'!\n",
          port2str(fpga_top_port).c_str(), port2str(cand).c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      dir_defined = true;
      break;
    }
  }
  if (!dir_defined) {
    dummy_port_direction_[top_port_str] = direction;
  }
  return CMD_EXEC_SUCCESS;
}

std::string IoNameMap::port2str(const BasicPort& port) const {
  return port.to_verilog_string();
}

BasicPort IoNameMap::str2port(const std::string& port_str) const {
  return PortParser(port_str).port();
}

std::string IoNameMap::dummy_port_dir_all2str() const {
  std::string full_types = "[";
  for (int itype = size_t(IoNameMap::e_dummy_port_direction::INPUT);
       itype != size_t(IoNameMap::e_dummy_port_direction::NUM_TYPES); ++itype) {
    full_types += std::string(DUMMY_PORT_DIR_STRING_[itype]) + std::string("|");
  }
  full_types.pop_back();
  full_types += "]";
  return full_types;
}

IoNameMap::e_dummy_port_direction IoNameMap::str2dummy_port_dir(
  const std::string& dir_str, const bool& verbose) const {
  for (int itype = size_t(IoNameMap::e_dummy_port_direction::INPUT);
       itype != size_t(IoNameMap::e_dummy_port_direction::NUM_TYPES); ++itype) {
    if (dir_str == std::string(DUMMY_PORT_DIR_STRING_[itype])) {
      return static_cast<IoNameMap::e_dummy_port_direction>(itype);
    }
  }
  VTR_LOGV_ERROR(verbose, "Invalid direction for dummy port! Expect %s\n",
                 dummy_port_dir_all2str().c_str());
  return IoNameMap::e_dummy_port_direction::NUM_TYPES;
}

std::string IoNameMap::dummy_port_dir2str(const e_dummy_port_direction& dir,
                                          const bool& verbose) const {
  if (!valid_dummy_port_direction(dir)) {
    VTR_LOGV_ERROR(verbose, "Invalid direction for dummy port! Expect %s\n",
                   dummy_port_dir_all2str().c_str());
    return std::string();
  }
  return std::string(DUMMY_PORT_DIR_STRING_[size_t(dir)]);
}

bool IoNameMap::valid_dummy_port_direction(
  const IoNameMap::e_dummy_port_direction& direction) const {
  return direction != IoNameMap::e_dummy_port_direction::NUM_TYPES;
}

} /* end namespace openfpga */
