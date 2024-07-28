#ifndef OVERWRITE_BITSTREAM_H
#define OVERWRITE_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include "openfpga_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void overwrite_bitstream(openfpga::BitstreamManager& bitstream_manager,
                         const openfpga::BitstreamSetting& bitstream_setting,
                         const bool& verbose);

} /* end namespace openfpga */

#endif
