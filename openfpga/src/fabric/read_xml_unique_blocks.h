#ifndef READ_XML_UNIQUE_BLOCKS_H
#define READ_XML_UNIQUE_BLOCKS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/********************************************************************
 * Function declaration
 *******************************************************************/
int read_xml_unique_blocks(const char* file_name, const char* file_type,
                           bool verbose);

#endif
