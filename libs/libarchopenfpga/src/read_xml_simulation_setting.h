#ifndef READ_XML_SIMULATION_SETTING_H
#define READ_XML_SIMULATION_SETTING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "pugixml_util.hpp"
#include "pugixml.hpp"
#include "simulation_setting.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

openfpga::SimulationSetting read_xml_simulation_setting(pugi::xml_node& Node,
                                                        const pugiutil::loc_data& loc_data);

#endif
