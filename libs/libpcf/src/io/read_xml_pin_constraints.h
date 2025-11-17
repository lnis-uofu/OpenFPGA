#ifndef READ_XML_PIN_CONSTRAINTS_H
#define READ_XML_PIN_CONSTRAINTS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "pin_constraints.h"
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/********************************************************************
 * Function declaration
 *******************************************************************/
PinConstraints read_xml_pin_constraints(const char* pin_constraint_fname);
int read_xml_pcf_command(pugi::xml_node& xml_pcf_command,
                         const pugiutil::loc_data& loc_data);
#endif
