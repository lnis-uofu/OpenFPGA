#ifndef WRITE_XML_TILE_ANNOTATION_H
#define WRITE_XML_TILE_ANNOTATION_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include "tile_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
/* namespace openfpga begins */
namespace openfpga {

void write_xml_tile_annotations(std::fstream& fp,
                                const char* fname,
                                const TileAnnotation& tile_annotation);

} /* namespace openfpga ends */

#endif
