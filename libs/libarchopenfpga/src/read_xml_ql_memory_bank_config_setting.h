#ifndef READ_XML_QL_MEMORY_BANK_CONFIG_SETTING_H
#define READ_XML_QL_MEMORY_BANK_CONFIG_SETTING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "pugixml.hpp"
#include "pugixml_util.hpp"
#include "ql_memory_bank_config_setting.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
void read_xml_ql_memory_bank_config_setting(QLMemoryBankConfigSetting& setting,
                                            pugi::xml_node& Node,
                                            const pugiutil::loc_data& loc_data);

#endif
