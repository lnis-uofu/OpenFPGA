/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML of clock network file to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "command_exit_codes.h"
#include "module_name_map_xml_constants.h"
#include "read_xml_module_name_map.h"
#include "read_xml_util.h"

namespace openfpga {  // Begin namespace openfpga

/********************************************************************
 * Parse XML codes of a <port> to an object of I/O naming
 *******************************************************************/
static int read_xml_module_name_binding(pugi::xml_node& xml_binding,
                                        const pugiutil::loc_data& loc_data,
                                        ModuleNameMap& module_name_map) {
  std::string default_name =
    get_attribute(xml_binding, XML_MODULE_NAME_ATTRIBUTE_DEFAULT, loc_data)
      .as_string();
  std::string given_name =
    get_attribute(xml_binding, XML_MODULE_NAME_ATTRIBUTE_GIVEN, loc_data)
      .as_string();

  return module_name_map.set_tag_to_name_pair(default_name, given_name);
}

/********************************************************************
 * Parse XML codes about <ports> to an object of ClockNetwork
 *******************************************************************/
int read_xml_module_name_map(const char* fname,
                             ModuleNameMap& module_name_map) {
  vtr::ScopedStartFinishTimer timer("Read module rename rules");

  int status = CMD_EXEC_SUCCESS;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, fname);

    pugi::xml_node xml_root =
      get_single_child(doc, XML_MODULE_NAMES_ROOT_NAME, loc_data);

    for (pugi::xml_node xml_binding : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_binding.name() != std::string(XML_MODULE_NAME_NODE_NAME)) {
        bad_tag(xml_binding, loc_data, xml_root, {XML_MODULE_NAME_NODE_NAME});
      }
      status =
        read_xml_module_name_binding(xml_binding, loc_data, module_name_map);
      if (status != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(fname, e.line(), "%s", e.what());
  }

  return status;
}

}  // End of namespace openfpga
