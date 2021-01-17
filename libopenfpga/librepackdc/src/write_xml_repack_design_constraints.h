#ifndef WRITE_XML_REPACK_DESIGN_CONSTRAINTS_H
#define WRITE_XML_REPACK_DESIGN_CONSTRAINTS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include "repack_design_constraints.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
int write_xml_repack_design_constraints(const char* fname,
                                        const RepackDesignConstraints& repack_design_constraints);

#endif
