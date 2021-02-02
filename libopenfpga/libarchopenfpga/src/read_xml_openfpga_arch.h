#ifndef READ_XML_OPENFPGA_ARCH_H
#define READ_XML_OPENFPGA_ARCH_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "openfpga_arch.h"
#include "simulation_setting.h"
#include "bitstream_setting.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
openfpga::Arch read_xml_openfpga_arch(const char* arch_file_name);

openfpga::SimulationSetting read_xml_openfpga_simulation_settings(const char* sim_setting_file_name);

openfpga::BitstreamSetting read_xml_openfpga_bitstream_settings(const char* bitstream_setting_file_name);

#endif
