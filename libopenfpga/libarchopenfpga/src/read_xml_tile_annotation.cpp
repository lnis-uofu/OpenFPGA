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
  /* We have mandatory XML attributes: 
   * - name of the port 
   */
  const std::string& name_attr = get_attribute(xml_tile, "name", loc_data).as_string();

  TileGlobalPortId tile_global_port_id = tile_annotation.create_global_port(name_attr);

  /* Report any duplicated port names */
  if (TileGlobalPortId::INVALID() == tile_global_port_id) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_tile),
                   "Invalid port name '%s' which is defined more than once in the global port list!\n",
                   name_attr.c_str());
  }

  /* Iterate over the children under this node,
   * each child should be named after <pb_type>
   */
  for (pugi::xml_node xml_tile_port : xml_tile.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_tile_port.name() != std::string("tile")) {
      bad_tag(xml_tile_port, loc_data, xml_tile, {"tile"});
    }
    /* Parse the name of the tiles and ports */
    const std::string& tile_name_attr = get_attribute(xml_tile_port, "name", loc_data).as_string();
    const std::string& port_name_attr = get_attribute(xml_tile_port, "port", loc_data).as_string();

    /* Extract the tile port information */
    openfpga::PortParser tile_port_parser(port_name_attr);
    
    /* Parse tile coordinates */
    vtr::Point<size_t> tile_coord(get_attribute(xml_tile_port, "x", loc_data, pugiutil::ReqOpt::OPTIONAL).as_int(-1), 
                                  get_attribute(xml_tile_port, "y", loc_data, pugiutil::ReqOpt::OPTIONAL).as_int(-1));

    /* Add tile port information */ 
    tile_annotation.add_global_port_tile_information(tile_global_port_id,
                                                     tile_name_attr, 
                                                     tile_port_parser.port(),
                                                     tile_coord);
  } 

  /* Check: Must have at least one global port tile information */
  if (true == tile_annotation.global_port_tile_names(tile_global_port_id).empty()) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_tile),
                   "Invalid tile annotation for global port '%s'! At least 1 tile port definition is expected!\n",
                   name_attr.c_str());
  }

  /* Get is_clock attributes */
  tile_annotation.set_global_port_is_clock(tile_global_port_id, get_attribute(xml_tile, "is_clock", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));

  /* Get is_set attributes */
  tile_annotation.set_global_port_is_set(tile_global_port_id, get_attribute(xml_tile, "is_set", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));

  /* Get is_reset attributes */
  tile_annotation.set_global_port_is_reset(tile_global_port_id, get_attribute(xml_tile, "is_reset", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));

  /* Get default_value attributes */
  tile_annotation.set_global_port_default_value(tile_global_port_id, get_attribute(xml_tile, "default_val", loc_data, pugiutil::ReqOpt::OPTIONAL).as_int(0));

  /* Ensure valid port attributes */
  if (false == tile_annotation.valid_global_port_attributes(tile_global_port_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_tile),
                   "Invalid port attributes for '%s'! A port can only be clock or set or reset.\n",
                   name_attr.c_str());
  }
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
