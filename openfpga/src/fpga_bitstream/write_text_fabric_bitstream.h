#ifndef WRITE_TEXT_FABRIC_BITSTREAM_H
#define WRITE_TEXT_FABRIC_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "bitstream_manager.h"
#include "fabric_bitstream.h"
#include "config_protocol.h"
#include "fabric_global_port_info.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int write_fabric_bitstream_to_text_file(const BitstreamManager& bitstream_manager,
                                        const FabricBitstream& fabric_bitstream,
                                        const ConfigProtocol& config_protocol,
                                        const FabricGlobalPortInfo& global_ports,
                                        const std::string& fname,
                                        const bool& fast_configuration,
                                        const bool& keep_dont_care_bits,
                                        const bool& verbose);

} /* end namespace openfpga */

#endif
