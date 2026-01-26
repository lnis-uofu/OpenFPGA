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
#include "read_xml_util.h"

/* Headers from libopenfpgautil */
#include "openfpga_port_parser.h"
#include "openfpga_tokenizer.h"
#include "read_xml_tile_annotation.h"

/********************************************************************
 * Parse XML description for an interconnection annotation
 * under a <global_port> XML node
 *******************************************************************/
static void read_xml_tile_global_port_annotation(
  pugi::xml_node& xml_tile, const pugiutil::loc_data& loc_data,
  openfpga::TileAnnotation& tile_annotation) {
  /* We have mandatory XML attributes:
   * - name of the port
   */
  const std::string& name_attr =
    get_attribute(xml_tile, XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_NAME, loc_data).as_string();

  TileGlobalPortId tile_global_port_id =
    tile_annotation.create_global_port(name_attr);

  /* Report any duplicated port names */
  if (TileGlobalPortId::INVALID() == tile_global_port_id) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_tile),
                   "Invalid port name '%s' which is defined more than once in "
                   "the global port list!\n",
                   name_attr.c_str());
  }

  /* Iterate over the children under this node,
   * each child should be named after <pb_type>
   */
  for (pugi::xml_node xml_tile_port : xml_tile.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_tile_port.name() != std::string(XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_NODE_NAME)) {
      bad_tag(xml_tile_port, loc_data, xml_tile, {XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_NODE_NAME});
    }
    /* Parse the name of the tiles and ports */
    const std::string& tile_name_attr =
      get_attribute(xml_tile_port, XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_ATTR_NAME, loc_data).as_string();
    const std::string& port_name_attr =
      get_attribute(xml_tile_port, XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_ATTR_PORT, loc_data).as_string();

    /* Extract the tile port information */
    openfpga::PortParser tile_port_parser(port_name_attr);

    /* Parse tile coordinates */
    vtr::Point<size_t> tile_coord(
      get_attribute(xml_tile_port, XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_ATTR_X, loc_data, pugiutil::ReqOpt::OPTIONAL)
        .as_int(-1),
      get_attribute(xml_tile_port, XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_ATTR_Y, loc_data, pugiutil::ReqOpt::OPTIONAL)
        .as_int(-1));

    /* Add tile port information */
    tile_annotation.add_global_port_tile_information(
      tile_global_port_id, tile_name_attr, tile_port_parser.port(), tile_coord);
  }

  /* Check: Must have at least one global port tile information */
  if (true ==
      tile_annotation.global_port_tile_names(tile_global_port_id).empty()) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_tile),
                   "Invalid tile annotation for global port '%s'! At least 1 "
                   "tile port definition is expected!\n",
                   name_attr.c_str());
  }

  /* Get is_clock attributes */
  tile_annotation.set_global_port_is_clock(
    tile_global_port_id,
    get_attribute(xml_tile, XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_IS_CLOCK, loc_data, pugiutil::ReqOpt::OPTIONAL)
      .as_bool(false));

  /* Get is_set attributes */
  tile_annotation.set_global_port_is_set(
    tile_global_port_id,
    get_attribute(xml_tile, XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_IS_SET, loc_data, pugiutil::ReqOpt::OPTIONAL)
      .as_bool(false));

  /* Get is_reset attributes */
  tile_annotation.set_global_port_is_reset(
    tile_global_port_id,
    get_attribute(xml_tile, XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_IS_RESET, loc_data, pugiutil::ReqOpt::OPTIONAL)
      .as_bool(false));

  /* Get clock tree attributes if this is a clock, reset or set */
  if (tile_annotation.global_port_is_clock(tile_global_port_id) ||
      tile_annotation.global_port_is_reset(tile_global_port_id) ||
      tile_annotation.global_port_is_set(tile_global_port_id)) {
    tile_annotation.set_global_port_clock_arch_tree_name(
      tile_global_port_id, get_attribute(xml_tile, XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_CLOCK_ARCH_TREE_NAME,
                                         loc_data, pugiutil::ReqOpt::OPTIONAL)
                             .as_string());
  }

  /* Get default_value attributes */
  tile_annotation.set_global_port_default_value(
    tile_global_port_id,
    get_attribute(xml_tile, XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_DEFAULT_VAL, loc_data, pugiutil::ReqOpt::OPTIONAL)
      .as_int(0));

  /* Ensure valid port attributes */
  if (false ==
      tile_annotation.valid_global_port_attributes(tile_global_port_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_tile),
                   "Invalid port attributes for '%s'! A port can only be clock "
                   "or set or reset.\n",
                   name_attr.c_str());
  }
}

/********************************************************************
 * Parse XML description for an interconnection annotation
 * under a <global_port> XML node
 *******************************************************************/
static void read_xml_tile_merge_port_annotation(
  pugi::xml_node& xml_tile, const pugiutil::loc_data& loc_data,
  openfpga::TileAnnotation& tile_annotation) {
  const std::string& tile_attr =
    get_attribute(xml_tile, XML_TILE_ANNOTATIONS_MERGE_SUBTILE_PORTS_ATTR_TILE, loc_data).as_string();

  const std::string& port_attr =
    get_attribute(xml_tile, XML_TILE_ANNOTATIONS_MERGE_SUBTILE_PORTS_ATTR_PORT, loc_data).as_string();

  tile_annotation.add_merge_subtile_ports(tile_attr, port_attr);
}

/********************************************************************
 * Parse XML description for an interconnection annotation
 * under a <global_port> XML node
 *******************************************************************/
static int read_xml_tile_physical_equivalent_site_annotation(
  pugi::xml_node& xml_tile, const pugiutil::loc_data& loc_data,
  openfpga::TileAnnotation& tile_annotation) {
  const std::string& tile_attr =
    get_attribute(xml_tile, XML_TILE_ANNOTATIONS_PHYSICAL_EQUIVALENT_SITE_ATTR_TILE, loc_data).as_string();

  const std::string& stile_attr =
    get_attribute(xml_tile, XML_TILE_ANNOTATIONS_PHYSICAL_EQUIVALENT_SITE_ATTR_SUBTILE, loc_data).as_string();

  const std::string& site_attr =
    get_attribute(xml_tile, XML_TILE_ANNOTATIONS_PHYSICAL_EQUIVALENT_SITE_ATTR_SITE, loc_data).as_string();

  tile_annotation.set_physical_equivalent_site(tile_attr, stile_attr, site_attr);
}

/********************************************************************
 * Top function to parse XML description about tile annotation
 *******************************************************************/
openfpga::TileAnnotation read_xml_tile_annotations(
  pugi::xml_node& Node, const pugiutil::loc_data& loc_data) {
  openfpga::TileAnnotation tile_annotations;

  /* Parse configuration protocol root node */
  pugi::xml_node xml_annotations = get_single_child(
    Node, XML_TILE_ANNOTATIONS_NODE_NAME, loc_data, pugiutil::ReqOpt::OPTIONAL);

  /* Not found, we can return */
  if (!xml_annotations) {
    return tile_annotations;
  }

  /* Iterate over the children under this node,
   * each child should be named after <pb_type>
   */
  for (pugi::xml_node xml_tile_global_port : xml_annotations.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_tile_global_port.name() == std::string(XML_TILE_ANNOTATIONS_GLOBAL_PORT_NODE_NAME)) {
      read_xml_tile_global_port_annotation(xml_tile_global_port, loc_data,
                                           tile_annotations);
    } else if (xml_tile_global_port.name() ==
               std::string(XML_TILE_ANNOTATIONS_MERGE_SUBTILE_PORTS_NODE_NAME)) {
      read_xml_tile_merge_port_annotation(xml_tile_global_port, loc_data,
                                          tile_annotations);
    } else if (xml_tile_global_port.name() ==
               std::string(XML_TILE_ANNOTATIONS_PHYSICAL_EQUIVALENT_SITE_NODE_NAME)) {
      read_xml_tile_physical_equivalent_site_annotation(xml_tile_global_port, loc_data,
                                          tile_annotations);
    } else {
      bad_tag(xml_tile_global_port, loc_data, xml_annotations,
              {XML_TILE_ANNOTATIONS_GLOBAL_PORT_NODE_NAME, XML_TILE_ANNOTATIONS_MERGE_SUBTILE_PORTS_NODE_NAME, XML_TILE_ANNOTATIONS_PHYSICAL_EQUIVALENT_SITE_NODE_NAME});
    }
  }

  return tile_annotations;
}
