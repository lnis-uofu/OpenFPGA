#ifndef READ_OPENFPGA_XML_H
#define READ_OPENFPGA_XML_H

#include <string>
#include "circuit_library.h"

void read_xml_openfpga_arch(const char* arch_file_name,
                            CircuitLibrary& circuit_lib);

#endif
