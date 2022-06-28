#ifndef REPORT_FABRIC_BITSTREAM_DISTRIBUTION_H
#define REPORT_FABRIC_BITSTREAM_DISTRIBUTION_H

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

int report_fabric_bitstream_distribution(std::fstream& fp,
                                         const FabricBitstream& fabric_bitstream,
                                         const int& hierarchy_level);

} /* end namespace openfpga */

#endif
