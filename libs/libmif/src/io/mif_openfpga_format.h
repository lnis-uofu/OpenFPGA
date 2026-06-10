#ifndef MIF_OPENFPGA_FORMAT_H
#define MIF_OPENFPGA_FORMAT_H

#include <ostream>

#include "mif_storage.h"

/********************************************************************
 * OpenFPGA custom MIF text format (example):
 *   # and // line comments
 *   // X <int>  // Y <int>  (optional tile coordinates)
 *   // ADDR_WIDTH <int>  // DATA_WIDTH <int>
 *   # Address Data
 *   0x2000 0x0000000a
 *   //RAM_ID <int>  (starts a new RAM block section)
 *   0x2e00 0x0000038f
 *******************************************************************/
namespace openfpga {

void serialize_openfpga_mif(const MifStorage& storage, std::ostream& os);

} /* namespace openfpga */

#endif
