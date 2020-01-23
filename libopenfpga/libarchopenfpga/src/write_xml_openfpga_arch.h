#ifndef WRITE_XML_OPENFPGA_ARCH_H
#define WRITE_XML_OPENFPGA_ARCH_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "openfpga_arch.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
void write_xml_openfpga_arch(const char* xml_fname, 
                             const openfpga::Arch& openfpga_arch);

#endif
