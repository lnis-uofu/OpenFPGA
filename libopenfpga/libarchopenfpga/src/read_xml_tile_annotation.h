#ifndef READ_XML_TILE_ANNOTATION_H
#define READ_XML_TILE_ANNOTATION_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "pugixml_util.hpp"
#include "pugixml.hpp"
#include "tile_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
openfpga::TileAnnotation read_xml_tile_annotations(pugi::xml_node& Node,
                                                   const pugiutil::loc_data& loc_data);

#endif
