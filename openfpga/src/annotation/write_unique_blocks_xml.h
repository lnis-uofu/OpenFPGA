#ifndef WRITE_XML_UNIQUE_BLOCKS_XML_H
#define WRITE_XML_UNIQUE_BLOCKS_XML_H

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
 * This file includes the top-level functions of this library
 * which includes:
 *  -- write the unique blocks' information in the associated data structures:
 *device_rr_gsb to a XML file
 *******************************************************************/

namespace openfpga {
int write_xml_atom_block(std::fstream& fp,
                         const std::vector<vtr::Point<size_t>>& instance_map,
                         const vtr::Point<size_t>& unique_block_coord,
                         std::string type);
void report_unique_module_status_write(
  const DeviceRRGSB& device_rr_gsb,
  bool verbose_output); /*report status of written info*/
int write_xml_unique_blocks(const DeviceRRGSB& device_rr_gsb, const char* fname,
                            bool verbose_output);
}  // namespace openfpga
#endif
