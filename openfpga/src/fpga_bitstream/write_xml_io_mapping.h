#ifndef WRITE_XML_IO_MAPPING_H
#define WRITE_XML_IO_MAPPING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "vpr_context.h"
#include "io_map.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int write_io_mapping_to_xml_file(const IoMap& io_map,
                                 const std::string& fname,
                                 const bool& include_time_stamp,
                                 const bool& verbose);

} /* end namespace openfpga */

#endif
