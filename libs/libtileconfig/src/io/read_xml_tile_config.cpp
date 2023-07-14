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
#include "read_xml_tile_config.h"
#include "read_xml_util.h"
#include "tile_config_xml_constants.h"

namespace openfpga {  // Begin namespace openfpga

/********************************************************************
 * Parse XML codes about <ports> to an object of ClockNetwork
 *******************************************************************/
int read_xml_tile_config(const char* fname, TileConfig& tile_config) {
  vtr::ScopedStartFinishTimer timer("Read tile configuration rules");

  int status = CMD_EXEC_SUCCESS;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, fname);

    pugi::xml_node xml_root =
      get_single_child(doc, XML_TILE_CONFIG_ROOT_NAME, loc_data);

    std::string style =
      get_attribute(xml_root, XML_TILE_CONFIG_ATTRIBUTE_STYLE_NAME, loc_data)
        .as_string();
    tile_config.set_style(style);
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(fname, e.line(), "%s", e.what());
  }

  return status;
}

}  // End of namespace openfpga
