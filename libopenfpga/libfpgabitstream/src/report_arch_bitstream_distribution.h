#ifndef REPORT_ARCH_BITSTREAM_DISTRIBUTION_H
#define REPORT_ARCH_BITSTREAM_DISTRIBUTION_H

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

void report_architecture_bitstream_distribution(const BitstreamManager& bitstream_manager,
                                                const std::string& fname,
                                                const size_t& max_hierarchy_level = 1);

} /* end namespace openfpga */

#endif
