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

int report_architecture_bitstream_distribution(
  std::fstream& fp, const BitstreamManager& bitstream_manager,
  const size_t& max_hierarchy_level, const size_t& hierarchy_level);

} /* end namespace openfpga */

#endif
