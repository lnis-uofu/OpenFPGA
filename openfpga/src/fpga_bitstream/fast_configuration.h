#ifndef FAST_CONFIGURATION_H
#define FAST_CONFIGURATION_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "fabric_global_port_info.h"
#include "config_protocol.h"
#include "bitstream_manager.h"
#include "fabric_bitstream.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

bool is_fast_configuration_applicable(const FabricGlobalPortInfo& global_ports);

bool find_bit_value_to_skip_for_fast_configuration(const e_config_protocol_type& config_protocol_type,  
                                                   const FabricGlobalPortInfo& global_ports,
                                                   const BitstreamManager& bitstream_manager,
                                                   const FabricBitstream& fabric_bitstream);

} /* end namespace openfpga */

#endif
