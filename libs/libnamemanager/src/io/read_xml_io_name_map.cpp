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

/* Headers from libopenfpga util library */
#include "openfpga_port_parser.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "command_exit_codes.h"
#include "io_name_map_xml_constants.h"
#include "read_xml_io_name_map.h"
#include "read_xml_util.h"

namespace openfpga {  // Begin namespace openfpga

/********************************************************************
 * Parse XML codes of a <port> to an object of I/O naming
 *******************************************************************/
static int read_xml_io_map_port(pugi::xml_node& xml_port,
                                const pugiutil::loc_data& loc_data,
                                IoNameMap& io_name_map) {
  /* Parse fpga top port information */
  std::string top_name =
    get_attribute(xml_port, XML_IO_NAME_MAP_ATTRIBUTE_TOP_NAME, loc_data)
      .as_string();
  BasicPort top_port = openfpga::PortParser(top_name).port();

  /* For dummy port, create the dummy io */
  bool is_dummy = get_attribute(xml_port, XML_IO_NAME_MAP_ATTRIBUTE_IS_DUMMY,
                                loc_data, pugiutil::ReqOpt::OPTIONAL)
                    .as_bool(false);
  if (is_dummy) {
    std::string dir_str =
      get_attribute(xml_port, XML_IO_NAME_MAP_ATTRIBUTE_DIRECTION, loc_data)
        .as_string();
    IoNameMap::e_dummy_port_direction dummy_port_dir =
      io_name_map.str2dummy_port_dir(dir_str, true);
    if (!io_name_map.valid_dummy_port_direction(dummy_port_dir)) {
      VTR_LOG_ERROR("Invalid direction for dummy port '%s'!\n",
                    top_port.to_verilog_string().c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    return io_name_map.set_dummy_io(top_port,
                                    dummy_port_dir); /* Early return */
  }

  /* This is not a dummy io, create the io mapping */
  std::string core_name =
    get_attribute(xml_port, XML_IO_NAME_MAP_ATTRIBUTE_CORE_NAME, loc_data)
      .as_string();
  BasicPort core_port = openfpga::PortParser(core_name).port();

  return io_name_map.set_io_pair(top_port, core_port);
}

/********************************************************************
 * Parse XML codes about <ports> to an object of ClockNetwork
 *******************************************************************/
int read_xml_io_name_map(const char* fname, IoNameMap& io_name_map) {
  vtr::ScopedStartFinishTimer timer("Read I/O naming rules");

  int status = CMD_EXEC_SUCCESS;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, fname);

    pugi::xml_node xml_root =
      get_single_child(doc, XML_IO_NAME_MAP_ROOT_NAME, loc_data);

    for (pugi::xml_node xml_port : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_port.name() != std::string(XML_IO_NAME_MAP_NODE_NAME)) {
        bad_tag(xml_port, loc_data, xml_root, {XML_IO_NAME_MAP_NODE_NAME});
      }
      status = read_xml_io_map_port(xml_port, loc_data, io_name_map);
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
