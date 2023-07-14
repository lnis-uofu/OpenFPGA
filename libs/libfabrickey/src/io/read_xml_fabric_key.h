#ifndef READ_XML_FABRIC_KEY_H
#define READ_XML_FABRIC_KEY_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "fabric_key.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

namespace openfpga {  // Begin namespace openfpga

FabricKey read_xml_fabric_key(const char* key_fname);

}  // End of namespace openfpga

#endif
