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
#include "command_exit_codes.h"
#include "read_xml_boundary_timing.h"
#include "read_xml_util.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Parse XML codes of a <set_io> to an object of PinConstraint
 *******************************************************************/
static void read_xml_one_boundary_timing(pugi::xml_node& xml_timing,
                                         const pugiutil::loc_data& loc_data,
                                         BoundaryTiming& boundary_timing) {
  openfpga::PortParser port_parser(
    get_attribute(xml_timing, "name", loc_data).as_string());

  std::string min_delay =
    get_attribute(xml_timing, "min", loc_data).as_string();
  std::string max_delay =
    get_attribute(xml_timing, "max", loc_data).as_string();

  /* Sanity checks */
  boundary_timing.create_pin_boundary_timing(port_parser.port(), max_delay,
                                             min_delay);
}

int read_xml_boundary_timing(const char* fname, BoundaryTiming& bdy_timing,
                             const bool& append, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Read boundary timing file");
  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  if (!append) {
    VTR_LOGV(verbose,
             "Start with a clean boundary timing as required by user\n");
    bdy_timing.clear();
  }

  try {
    loc_data = pugiutil::load_xml(doc, fname);

    pugi::xml_node xml_root =
      get_single_child(doc, "boundary_timing", loc_data);

    for (pugi::xml_node xml_timing : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_timing.name() != std::string("pin")) {
        bad_tag(xml_timing, loc_data, xml_root, {"pin"});
      }
      read_xml_one_boundary_timing(xml_timing, loc_data, bdy_timing);
    }
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(fname, e.line(), "%s", e.what());
  }
  return CMD_EXEC_SUCCESS;
}

} /* End namespace openfpga*/
