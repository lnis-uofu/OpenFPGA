#ifndef READ_XML_TILE_CONFIG_H
#define READ_XML_TILE_CONFIG_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "tile_config.h"
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/********************************************************************
 * Function declaration
 *******************************************************************/

namespace openfpga {  // Begin namespace openfpga

int read_xml_tile_config(const char* fname, TileConfig& tile_config);

}  // End of namespace openfpga

#endif
