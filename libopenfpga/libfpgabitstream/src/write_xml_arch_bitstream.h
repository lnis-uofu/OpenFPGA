#ifndef WRITE_XML_ARCH_BITSTREAM_H
#define WRITE_XML_ARCH_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "bitstream_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void write_xml_architecture_bitstream(const BitstreamManager& bitstream_manager,
                                      const std::string& fname,
                                      const bool& include_time_stamp);

} /* end namespace openfpga */

#endif
