#pragma once

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

#include "fabric_bitstream.h"
#include "pugixml.hpp"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int report_fabric_bitstream_distribution(
  pugi::xml_node& parent_node, const FabricBitstream& fabric_bitstream);

} /* end namespace openfpga */
