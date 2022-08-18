#ifndef READ_XML_REPACK_DESIGN_CONSTRAINTS_H
#define READ_XML_REPACK_DESIGN_CONSTRAINTS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "pugixml_util.hpp"
#include "pugixml.hpp"
#include "repack_design_constraints.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
RepackDesignConstraints read_xml_repack_design_constraints(const char* design_constraint_fname);

#endif
