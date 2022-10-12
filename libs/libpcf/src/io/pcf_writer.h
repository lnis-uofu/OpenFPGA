#ifndef PCF_WRITER_H
#define PCF_WRITER_H
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

/* Write a .pcf file from an object */
int write_pcf(const char* fname, const PcfData& pcf_data);

} /* End namespace openfpga*/

#endif
