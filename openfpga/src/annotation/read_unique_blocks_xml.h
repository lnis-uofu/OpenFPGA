#ifndef READ_XML_UNIQUE_BLOCKS_XML_H
#define READ_XML_UNIQUE_BLOCKS_XML_H

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
#include "device_rr_gsb_utils.h"
#include "unique_blocks_uxsdcxx.capnp.h"
/********************************************************************
 * This file includes the top-level functions of this library
 * which includes:
 *  -- reads an XML file of unique blocks to the associated
 * data structures: device_rr_gsb
 *******************************************************************/
namespace openfpga {

std::vector<vtr::Point<size_t>> read_xml_unique_instance_coords(
  const pugi::xml_node& xml_block_info, const pugiutil::loc_data& loc_data);

vtr::Point<size_t> read_xml_unique_block_coord(
  const pugi::xml_node& xml_block_info, const pugiutil::loc_data& loc_data);

void report_unique_module_status_read(const DeviceRRGSB& device_rr_gsb,
                                      bool verbose_output);

int read_xml_unique_blocks(DeviceRRGSB& device_rr_gsb, const char* file_name,
                           bool verbose_output);
}  // namespace openfpga

#endif
