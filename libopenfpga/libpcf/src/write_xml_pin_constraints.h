#ifndef WRITE_XML_PIN_CONSTRAINTS_H
#define WRITE_XML_PIN_CONSTRAINTS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include "pin_constraints.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
int write_xml_pin_constraints(const char* fname,
                              const PinConstraints& pin_constraints);

#endif
