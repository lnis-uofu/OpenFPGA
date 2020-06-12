#ifndef FABRIC_BITSTREAM_WRITER_H
#define FABRIC_BITSTREAM_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "bitstream_manager.h"
#include "fabric_bitstream.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void write_fabric_bitstream_to_text_file(const BitstreamManager& bitstream_manager,
                                         const FabricBitstream& fabric_bitstream,
                                         const std::string& fname);

} /* end namespace openfpga */

#endif
