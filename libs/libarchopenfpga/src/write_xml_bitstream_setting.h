#ifndef WRITE_XML_BITSTREAM_SETTING_H
#define WRITE_XML_BITSTREAM_SETTING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include "bitstream_setting.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
void write_xml_bitstream_setting(std::fstream& fp,
                                 const char* fname,
                                 const openfpga::BitstreamSetting& bitstream_setting);

#endif
