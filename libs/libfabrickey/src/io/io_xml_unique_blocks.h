#ifndef IO_XML_UNIQUE_BLOCKS_H
#define IO_XML_UNIQUE_BLOCKS_H

#include <fstream>

#include "pugixml.hpp"
#include "pugixml_util.hpp"
#include "vtr_geometry.h"
/********************************************************************
 * Function declaration
 *******************************************************************/

namespace openfpga {  // Begin namespace openfpga

vtr::Point<size_t> read_xml_unique_instance_info(
  pugi::xml_node& xml_instance_info, const pugiutil::loc_data& loc_data);

int write_xml_block(
  std::map<int, vtr::Point<size_t>>& id_unique_block_map,
  std::map<int, std::vector<vtr::Point<size_t>>>& id_instance_map,
  std::fstream& fp, std::string type);

}  // End of namespace openfpga

#endif
