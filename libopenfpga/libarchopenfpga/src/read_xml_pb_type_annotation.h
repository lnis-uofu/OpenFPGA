#ifndef READ_XML_PB_TYPE_ANNOTATION_H
#define READ_XML_PB_TYPE_ANNOTATION_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "pugixml_util.hpp"
#include "pugixml.hpp"
#include "pb_type_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
std::vector<openfpga::PbTypeAnnotation> read_xml_pb_type_annotations(pugi::xml_node& Node,
                                                                     const pugiutil::loc_data& loc_data);

#endif
