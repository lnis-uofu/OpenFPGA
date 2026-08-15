#pragma once

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "bitstream_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
/* begin namespace openfpga */
namespace openfpga {

int read_xml_architecture_bitstream(const char* fname,
                                    BitstreamManager& bitstream_manager,
                                    const bool& verbose);

} /* end namespace openfpga */
