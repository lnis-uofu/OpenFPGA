/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML of I/O location to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from libopenfpga util library */
#include "openfpga_port_parser.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_io_location_map.h"
#include "read_xml_util.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Parse XML codes of a <set_io> to an object of PinConstraint
 *******************************************************************/
static void read_xml_one_io_location(pugi::xml_node& xml_io,
                                     const pugiutil::loc_data& loc_data,
                                     IoLocationMap& io_location_map) {
  openfpga::PortParser port_parser(
    get_attribute(xml_io, "pad", loc_data).as_string());

  int x_coord = get_attribute(xml_io, "x", loc_data).as_int();
  int y_coord = get_attribute(xml_io, "y", loc_data).as_int();
  int z_coord = get_attribute(xml_io, "z", loc_data).as_int();

  /* Sanity checks */
  if (x_coord < 0 || y_coord < 0 || z_coord < 0) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_io),
                   "Invalid coordinate (x, y, z) = (%d, %d, %d)! Expect zero "
                   "or a positive integer!\n",
                   x_coord, y_coord, z_coord);
  }
  if (port_parser.port().get_width() != 1) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_io),
                   "I/O (%s) does not have a port size of 1!\n",
                   get_attribute(xml_io, "pad", loc_data).as_string());
  }
  io_location_map.set_io_index(size_t(x_coord), size_t(y_coord),
                               size_t(z_coord), port_parser.port().get_name(),
                               port_parser.port().get_lsb());
}

/********************************************************************
 * Parse XML codes about <io_coordinates> to an object of PinConstraints
 *******************************************************************/
IoLocationMap read_xml_io_location_map(const char* fname) {
  vtr::ScopedStartFinishTimer timer("Read I/O Location Map");

  IoLocationMap io_location_map;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, fname);

    pugi::xml_node xml_root = get_single_child(doc, "io_coordinates", loc_data);

    /*size_t num_children = std::distance(xml_root.children().begin(),
     * xml_root.children().end());
     * TODO: Reserve memory space for efficiency */

    for (pugi::xml_node xml_io : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_io.name() != std::string("io")) {
        bad_tag(xml_io, loc_data, xml_root, {"io"});
      }
      read_xml_one_io_location(xml_io, loc_data, io_location_map);
    }
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(fname, e.line(), "%s", e.what());
  }

  return io_location_map;
}

} /* End namespace openfpga*/
