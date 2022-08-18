#ifndef PCF_READER_H
#define PCF_READER_H
/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <stddef.h>
#include <array>
#include <map>
#include <fstream>
#include "pcf_data.h"

/* Begin namespace openfpga */
namespace openfpga {

/* Parse a .pcf file through a stream, return an object which contains all the data */
int read_pcf(const char* fname,
             PcfData& pcf_data);

} /* End namespace openfpga*/

#endif
