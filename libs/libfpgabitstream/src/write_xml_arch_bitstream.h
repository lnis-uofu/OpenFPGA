#pragma once

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

int write_xml_architecture_bitstream(const BitstreamManager& bitstream_manager,
                                      const std::string& fname,
                                      const bool& include_time_stamp);

} /* end namespace openfpga */
