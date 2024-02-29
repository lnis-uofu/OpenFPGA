/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML modeling OpenFPGA architecture to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_ql_memory_bank_config_setting.h"
#include "read_xml_util.h"

/********************************************************************
 * Parse XML codes about <ql_memory_bank_config_setting> to
 *QLMemoryBankConfigSetting
 *******************************************************************/
void read_xml_ql_memory_bank_config_setting(
  QLMemoryBankConfigSetting& setting, pugi::xml_node& Node,
  const pugiutil::loc_data& loc_data) {
  /* Parse configuration protocol root node */
  pugi::xml_node config_setting =
    get_single_child(Node, "ql_memory_bank_config_setting", loc_data,
                     pugiutil::ReqOpt::OPTIONAL);

  if (config_setting) {
    /* Add to ql_memory_bank_config_setting_ */
    for (pugi::xml_node xml_child : config_setting.children()) {
      if (xml_child.name() != std::string("pb_type")) {
        bad_tag(xml_child, loc_data, config_setting, {"pb_type"});
      }
      const std::string& name_attr =
        get_attribute(xml_child, "name", loc_data).as_string();
      uint32_t num_wl = get_attribute(xml_child, "num_wl", loc_data).as_uint();
      setting.add_pb_setting(name_attr, num_wl);
    }
  }
}
