#ifndef WRITE_XML_UNIQUE_BLOCKS_H
#define WRITE_XML_UNIQUE_BLOCKS_H

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

int write_xml_block(
  std::map<int, vtr::Point<size_t>>& id_unique_block_map,
  std::map<int, std::vector<vtr::Point<size_t>>>& id_instance_map,
  std::fstream& fp, std::string type);
void report_unique_module_status_write(const DeviceRRGSB& device_rr_gsb,
                                       bool verbose_output);
int write_xml_unique_blocks(const DeviceRRGSB& device_rr_gsb, const char* fname,
                            bool verbose_output);
}  // namespace openfpga
#endif
