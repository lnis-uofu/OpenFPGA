#ifndef READ_OPENFPGA_XML_H
#define READ_OPENFPGA_XML_H

#include <string>
#include "circuit_library.h"
#include "circuit_settings.h"

CircuitSettings read_xml_openfpga_arch(const char* arch_file_name);

#endif
