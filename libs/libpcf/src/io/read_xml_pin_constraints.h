#ifndef READ_XML_PIN_CONSTRAINTS_H
#define READ_XML_PIN_CONSTRAINTS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "pugixml_util.hpp"
#include "pugixml.hpp"
#include "pin_constraints.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
PinConstraints read_xml_pin_constraints(const char* pin_constraint_fname);

#endif
