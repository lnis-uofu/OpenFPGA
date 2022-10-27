#ifndef READ_XML_ARCH_BITSTREAM_H
#define READ_XML_ARCH_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "bitstream_manager.h"
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/********************************************************************
 * Function declaration
 *******************************************************************/
/* begin namespace openfpga */
namespace openfpga {

BitstreamManager read_xml_architecture_bitstream(const char* fname);

} /* end namespace openfpga */

#endif
