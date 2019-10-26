/********************************************************************
 * Header file for bitstream_writer.cpp
 *******************************************************************/
#ifndef BITSTREAM_WRITER_H
#define BITSTREAM_WRITER_H

#include <string>
#include "bitstream_manager.h"

void write_arch_independent_bitstream_to_xml_file(const BitstreamManager& bitstream_manager,
                                                  const std::string& fname);

#endif
