#pragma once

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "pugixml.hpp"
#include "bitstream_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int report_architecture_bitstream_distribution(
  pugi::xml_node& parent_node, const BitstreamManager& bitstream_manager,
  const size_t& max_hierarchy_level, const size_t& hierarchy_level);

} /* end namespace openfpga */
