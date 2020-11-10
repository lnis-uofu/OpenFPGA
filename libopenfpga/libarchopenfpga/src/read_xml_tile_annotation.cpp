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
#include "openfpga_tokenizer.h"
#include "openfpga_port_parser.h"

#include "read_xml_tile_annotation.h"

/********************************************************************
 * Parse XML description for an interconnection annotation 
 * under a <global_port> XML node
 *******************************************************************/
static 
void read_xml_tile_global_port_annotation(pugi::xml_node& xml_tile,
                                          const pugiutil::loc_data& loc_data,
                                          openfpga::TileAnnotation& tile_annotation) {
  /* We have two mandatory XML attributes
   * 1. name of the port
   * 2. name of the tile and ports in the format of <tile_name>.<tile_port_name>
   */
  const std::string& name_attr = get_attribute(xml_tile, "name", loc_data).as_string();
  const std::string& tile_port_name_attr = get_attribute(xml_tile, "tile_port", loc_data).as_string();

  /* Extract the tile name */
  openfpga::StringToken tokenizer(tile_port_name_attr);
  std::vector<std::string> tile_port_tokens = tokenizer.split('.');
  if (2 != tile_port_tokens.size()) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_tile),
                   "Invalid tile_port attribute '%s'! Valid format is <tile_name>.<port_name>\n",
                   tile_port_name_attr);
  }
  /* Extract the tile port information */
  openfpga::PortParser tile_port_parser(tile_port_tokens[1]);

  TileGlobalPortId tile_global_port_id = tile_annotation.create_global_port(name_attr, tile_port_tokens[0], tile_port_parser.port());

  /* Get is_clock attributes */
  tile_annotation.set_global_port_is_clock(tile_global_port_id, get_attribute(xml_tile, "is_clock", loc_data).as_bool(false));

  /* Get is_set attributes */
  tile_annotation.set_global_port_is_set(tile_global_port_id, get_attribute(xml_tile, "is_set", loc_data).as_bool(false));

  /* Get is_reset attributes */
  tile_annotation.set_global_port_is_reset(tile_global_port_id, get_attribute(xml_tile, "is_reset", loc_data).as_bool(false));

  /* Get default_value attributes */
  tile_annotation.set_global_port_default_value(tile_global_port_id, get_attribute(xml_tile, "default_value", loc_data).as_int(0));
}

/********************************************************************
 * Top function to parse XML description about tile annotation 
 *******************************************************************/
openfpga::TileAnnotation read_xml_tile_annotations(pugi::xml_node& Node,
                                                     const pugiutil::loc_data& loc_data) {
  openfpga::TileAnnotation tile_annotations;

  /* Parse configuration protocol root node */
  pugi::xml_node xml_annotations = get_single_child(Node, "tile_annotations", loc_data, pugiutil::ReqOpt::OPTIONAL);

  /* Not found, we can return */
  if (!xml_annotations) {
    return tile_annotations;
  }

  /* Iterate over the children under this node,
   * each child should be named after <pb_type>
   */
  for (pugi::xml_node xml_tile_global_port : xml_annotations.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_tile_global_port.name() != std::string("global_port")) {
      bad_tag(xml_tile_global_port, loc_data, xml_annotations, {"global_port"});
    }
    read_xml_tile_global_port_annotation(xml_tile_global_port, loc_data, tile_annotations);
  } 

  return tile_annotations;
}
