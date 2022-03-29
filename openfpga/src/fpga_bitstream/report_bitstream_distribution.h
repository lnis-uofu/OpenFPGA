#ifndef REPORT_BITSTREAM_DISTRIBUTION_H
#define REPORT_BITSTREAM_DISTRIBUTION_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "fabric_bitstream.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int report_bitstream_distribution(const std::string& fname,
                                  const BitstreamManager& bitstream_manager,
                                  const FabricBitstream& fabric_bitstream,
                                  const bool& include_time_stamp,
                                  const size_t& max_hierarchy_level = 1);

} /* end namespace openfpga */

#endif
