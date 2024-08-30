#ifndef READ_XML_UNIQUE_BLOCKS_H
#define READ_XML_UNIQUE_BLOCKS_H

/********************************************************************
 * This file includes the top-level functions of this library
 * which includes:
 *  -- reads an XML file of unique blocks to the associated
 * data structures: device_rr_gsb
 * -- write device__rr_gsb's info about unique blocks to a xml file
 *******************************************************************/

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

/********************************************************************
 * Parse XML codes of a <instance> to an object of device_rr_gsb
 * instance is the mirror of unique module.
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
