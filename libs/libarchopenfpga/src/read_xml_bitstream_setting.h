#ifndef READ_XML_BITSTREAM_SETTING_H
#define READ_XML_BITSTREAM_SETTING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "bitstream_setting.h"
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/********************************************************************
 * Function declaration
 *******************************************************************/

openfpga::BitstreamSetting read_xml_bitstream_setting(
  pugi::xml_node& Node, const pugiutil::loc_data& loc_data);

#endif
