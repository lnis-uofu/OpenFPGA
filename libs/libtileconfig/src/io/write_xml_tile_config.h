#ifndef WRITE_XML_TILE_CONFIG_H
#define WRITE_XML_TILE_CONFIG_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>

#include "tile_config.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
namespace openfpga {  // Begin namespace openfpga

int write_xml_tile_config(const char* fname, const TileConfig& tile_config);

}  // End of namespace openfpga

#endif
