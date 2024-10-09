#include <capnp/message.h>
#include <capnp/serialize.h>
#include <kj/io.h>

#include <string>
/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "command_exit_codes.h"
#include "device_rr_gsb_utils.h"
#include "mmap_file.h"
#include "openfpga_digest.h"
#include "read_unique_blocks_bin.h"
#include "read_unique_blocks_xml.h"
#include "read_xml_util.h"
#include "rr_gsb.h"
#include "unique_blocks_uxsdcxx.capnp.h"
#include "write_xml_utils.h"

/********************************************************************
 * This file includes the top-level functions of this library
 * which includes:
 *  -- reads a bin file of unique blocks to the associated
 * data structures: device_rr_gsb
 *******************************************************************/
namespace openfpga {

/*read the instances' coordinate of a unique block from a bin file*/
std::vector<vtr::Point<size_t>> read_bin_unique_instance_coords(
  const ucap::Block::Reader& unique_block) {
  std::vector<vtr::Point<size_t>> instance_coords;
  if (unique_block.hasInstances()) {
    auto instance_list = unique_block.getInstances();
    for (auto instance : instance_list) {
      int instance_x = instance.getX();
      int instance_y = instance.getY();
      vtr::Point<size_t> instance_coordinate(instance_x, instance_y);
      instance_coords.push_back(instance_coordinate);
    }
  }
  return instance_coords;
}

/*read the unique block coordinate from a bin file */
vtr::Point<size_t> read_bin_unique_block_coord(
  const ucap::Block::Reader& unique_block, ucap::Type& type) {
  int block_x = unique_block.getX();
  int block_y = unique_block.getY();
  type = unique_block.getType();
  vtr::Point<size_t> block_coordinate(block_x, block_y);
  return block_coordinate;
}

/*top-level function to read unique blocks from bin file*/
int read_bin_unique_blocks(DeviceRRGSB& device_rr_gsb, const char* file_name,
                           bool verbose_output) {
  /* clear unique modules & reserve memory to relavant vectors */
  device_rr_gsb.clear_unique_modules();
  device_rr_gsb.reserve_unique_modules();
  MmapFile f(file_name);
  ::capnp::FlatArrayMessageReader reader(f.getData());
  auto root = reader.getRoot<ucap::UniqueBlocks>();
  if (root.hasBlocks()) {
    auto block_list = root.getBlocks();
    for (auto unique_block : block_list) {
      ucap::Type type;
      vtr::Point<size_t> block_coordinate = read_bin_unique_block_coord(
        unique_block, type); /*get block coordinate and type*/
      std::vector<vtr::Point<size_t>> instance_coords =
        read_bin_unique_instance_coords(
          unique_block); /* get a list of instance coordinates*/
      /* get block coordinate and instance coordinate, try to setup
       * device_rr_gsb */
      if (type == ucap::Type::SB) {
        device_rr_gsb.preload_unique_sb_module(block_coordinate,
                                               instance_coords);
      } else if (type == ucap::Type::CBY) {
        device_rr_gsb.preload_unique_cby_module(block_coordinate,
                                                instance_coords);
      } else if (type == ucap::Type::CBX) {
        device_rr_gsb.preload_unique_cbx_module(block_coordinate,
                                                instance_coords);
      } else if (type == ucap::Type::UXSD_INVALID) {
        VTR_LOG_ERROR("Invalid block type!");
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }
  device_rr_gsb.build_gsb_unique_module();
  if (verbose_output) {
    report_unique_module_status_read(device_rr_gsb, true);
  }
  return CMD_EXEC_SUCCESS;
}
}  // namespace openfpga
