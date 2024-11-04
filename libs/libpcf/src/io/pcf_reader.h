#ifndef PCF_READER_H
#define PCF_READER_H
/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <stddef.h>

#include <array>
#include <fstream>
#include <map>

#include "pcf_data.h"

/* Begin namespace openfpga */
namespace openfpga {

/* Parse a .pcf file through a stream, return an object which contains all the
 * data */
int read_pcf(const char* fname, PcfData& pcf_data,
             bool reduce_error_to_warning = false);

} /* End namespace openfpga*/

#endif
