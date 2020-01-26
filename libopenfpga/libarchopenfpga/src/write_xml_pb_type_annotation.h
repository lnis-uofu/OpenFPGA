#ifndef WRITE_XML_PB_TYPE_ANNOTATION_H
#define WRITE_XML_PB_TYPE_ANNOTATION_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include "pb_type_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
/* namespace openfpga begins */
namespace openfpga {

void write_xml_pb_type_annotations(std::fstream& fp,
                                   const char* fname,
                                   const std::vector<PbTypeAnnotation>& pb_type_annotations);

} /* namespace openfpga ends */

#endif
