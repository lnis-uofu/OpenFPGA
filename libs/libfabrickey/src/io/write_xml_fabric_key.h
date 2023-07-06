#ifndef WRITE_XML_FABRIC_KEY_H
#define WRITE_XML_FABRIC_KEY_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>

#include "fabric_key.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

namespace openfpga {  // Begin namespace openfpga

int write_xml_fabric_key(const char* fname, const FabricKey& fabric_key);

}  // End of namespace openfpga

#endif
