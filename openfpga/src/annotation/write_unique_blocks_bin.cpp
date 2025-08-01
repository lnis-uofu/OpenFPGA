
#include <capnp/message.h>

#include <string>
/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"
#include "serdes_utils.h"
/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "command_exit_codes.h"
#include "device_rr_gsb_utils.h"
#include "openfpga_digest.h"
#include "read_xml_util.h"
#include "rr_gsb.h"
#include "unique_blocks_uxsdcxx.capnp.h"
#include "write_unique_blocks_bin.h"
#include "write_unique_blocks_xml.h"
#include "write_xml_utils.h"

/********************************************************************
 * This file includes the top-level functions of this library
 * which includes:
 *  -- write the unique blocks' information in the associated data structures:
 *device_rr_gsb to a bin file
 *******************************************************************/
namespace openfpga {
/* write each unique block (including a single unique block info and its mirror
 * instances' info)into capnp builder */
int write_bin_atom_block(const std::vector<vtr::Point<size_t>>& instance_map,
                         const vtr::Point<size_t>& unique_block_coord,
                         const ucap::Type type, ucap::Block::Builder& root) {
  root.setX(unique_block_coord.x());
  root.setY(unique_block_coord.y());
  root.setType(type);
  if (instance_map.size() > 0) {
    auto instance_list = root.initInstances(instance_map.size());
    for (size_t instance_id = 0; instance_id < instance_map.size();
         instance_id++) {
      auto instance = instance_list[instance_id];
      instance.setX(instance_map[instance_id].x());
      instance.setY(instance_map[instance_id].y());
    }
  }
  return openfpga::CMD_EXEC_SUCCESS;
}

/* Top-level function to write bin file of unique blocks */
int write_bin_unique_blocks(const DeviceRRGSB& device_rr_gsb, const char* fname,
                            bool verbose_output) {
  ::capnp::MallocMessageBuilder builder;
  auto unique_blocks = builder.initRoot<ucap::UniqueBlocks>();
  int num_unique_blocks =
    device_rr_gsb.get_num_sb_unique_module() +
    device_rr_gsb.get_num_cb_unique_module(e_rr_type::CHANX) +
    device_rr_gsb.get_num_cb_unique_module(e_rr_type::CHANY);
  auto block_list = unique_blocks.initBlocks(num_unique_blocks);

  /*write switch blocks into bin file */
  for (size_t id = 0; id < device_rr_gsb.get_num_sb_unique_module(); ++id) {
    const auto unique_block_coord = device_rr_gsb.get_sb_unique_block_coord(id);
    const std::vector<vtr::Point<size_t>> instance_map =
      device_rr_gsb.get_sb_unique_block_instance_coord(unique_block_coord);
    auto unique_block = block_list[id];
    int status_code = write_bin_atom_block(instance_map, unique_block_coord,
                                           ucap::Type::SB, unique_block);
    if (status_code != 0) {
      VTR_LOG_ERROR("write sb unique blocks into bin file failed!");
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  /*write cbx blocks into bin file */
  for (size_t id = 0;
       id < device_rr_gsb.get_num_cb_unique_module(e_rr_type::CHANX); ++id) {
    const auto unique_block_coord =
      device_rr_gsb.get_cbx_unique_block_coord(id);
    const std::vector<vtr::Point<size_t>> instance_map =
      device_rr_gsb.get_cbx_unique_block_instance_coord(unique_block_coord);
    int block_id = id + device_rr_gsb.get_num_sb_unique_module();
    auto unique_block = block_list[block_id];
    int status_code = write_bin_atom_block(instance_map, unique_block_coord,
                                           ucap::Type::CBX, unique_block);
    if (status_code != 0) {
      VTR_LOG_ERROR("write cbx unique blocks into bin file failed!");
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  /*write cby blocks into bin file */
  for (size_t id = 0;
       id < device_rr_gsb.get_num_cb_unique_module(e_rr_type::CHANY); ++id) {
    const auto unique_block_coord =
      device_rr_gsb.get_cby_unique_block_coord(id);
    const std::vector<vtr::Point<size_t>> instance_map =
      device_rr_gsb.get_cby_unique_block_instance_coord(unique_block_coord);
    int block_id = id + device_rr_gsb.get_num_sb_unique_module() +
                   device_rr_gsb.get_num_cb_unique_module(e_rr_type::CHANX);
    auto unique_block = block_list[block_id];
    int status_code = write_bin_atom_block(instance_map, unique_block_coord,
                                           ucap::Type::CBY, unique_block);
    if (status_code != 0) {
      VTR_LOG_ERROR("write cby unique blocks into bin file failed!");
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  writeMessageToFile(fname, &builder);
  if (verbose_output) {
    report_unique_module_status_write(device_rr_gsb, true);
  }
  return openfpga::CMD_EXEC_SUCCESS;
}

}  // namespace openfpga
