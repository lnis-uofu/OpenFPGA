#ifndef WRITE_XML_FABRIC_BITSTREAM_H
#define WRITE_XML_FABRIC_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "bitstream_manager.h"
#include "fabric_bitstream.h"
#include "config_protocol.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int write_fabric_bitstream_to_xml_file(const BitstreamManager& bitstream_manager,
                                       const FabricBitstream& fabric_bitstream,
                                       const ConfigProtocol& config_protocol,
                                       const std::string& fname,
                                       const bool& verbose);

} /* end namespace openfpga */

#endif
