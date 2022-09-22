#ifndef WRITE_XML_SIMULATION_SETTING_H
#define WRITE_XML_SIMULATION_SETTING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include "simulation_setting.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
void write_xml_simulation_setting(std::fstream& fp,
                                  const char* fname,
                                  const openfpga::SimulationSetting& sim_setting);

#endif
