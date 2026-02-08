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
#include "pugixml.hpp"
#include "pugixml_util.hpp"
/* Begin namespace openfpga */
namespace openfpga {

/* Parse a .pcf file through a stream, return an object which contains all the
 * data */
int read_pcf(const char* fname, PcfData& pcf_data,
             const PcfCustomCommand& pcf_custom_command,
             bool reduce_error_to_warning, const bool& verbose);
int read_pcf_config(const std::string& pcf_config_file,
                    PcfCustomCommand& pcf_custom_command);

} /* End namespace openfpga*/

#endif
