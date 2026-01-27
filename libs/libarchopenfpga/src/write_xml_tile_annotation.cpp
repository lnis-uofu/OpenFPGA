/********************************************************************
 * This file includes functions that outputs tile annotations to XML format
 *******************************************************************/
/* Headers from system goes first */
#include <algorithm>
#include <string>

/* Headers from vtr util library */
#include "openfpga_digest.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from readarchopenfpga library */
#include "tile_annotation_xml_constants.h"
#include "write_xml_tile_annotation.h"
#include "write_xml_utils.h"

/* namespace openfpga begins */
namespace openfpga {

/********************************************************************
 * A writer to output a device variation in a technology library to XML format
 *******************************************************************/
static void write_xml_tile_annotation_global_port(
  std::fstream& fp, const char* fname,
  const openfpga::TileAnnotation& tile_annotation,
  const TileGlobalPortId& global_port_id) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t"
     << "<" << XML_TILE_ANNOTATIONS_GLOBAL_PORT_NODE_NAME << " ";

  write_xml_attribute(fp, XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_NAME,
                      tile_annotation.global_port_name(global_port_id).c_str());

  write_xml_attribute(fp, XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_IS_CLOCK,
                      tile_annotation.global_port_is_clock(global_port_id));

  if (tile_annotation.global_port_is_clock(global_port_id) &&
      !tile_annotation.global_port_clock_arch_tree_name(global_port_id)
         .empty()) {
    write_xml_attribute(
      fp, XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_CLOCK_ARCH_TREE_NAME,
      tile_annotation.global_port_clock_arch_tree_name(global_port_id).c_str());
  }

  write_xml_attribute(fp, XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_IS_SET,
                      tile_annotation.global_port_is_set(global_port_id));

  write_xml_attribute(fp, XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_IS_RESET,
                      tile_annotation.global_port_is_reset(global_port_id));

  write_xml_attribute(
    fp, XML_TILE_ANNOTATIONS_GLOBAL_PORT_ATTR_DEFAULT_VAL,
    tile_annotation.global_port_default_value(global_port_id));

  fp << ">"
     << "\n";

  for (size_t tile_info_id = 0;
       tile_info_id <
       tile_annotation.global_port_tile_names(global_port_id).size();
       ++tile_info_id) {
    fp << "\t\t\t"
       << "<" << XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_NODE_NAME << " ";
    write_xml_attribute(
      fp, XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_ATTR_NAME,
      tile_annotation.global_port_tile_names(global_port_id)[tile_info_id]
        .c_str());
    write_xml_attribute(
      fp, XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_ATTR_PORT,
      generate_xml_port_name(
        tile_annotation.global_port_tile_ports(global_port_id)[tile_info_id])
        .c_str());
    write_xml_attribute(
      fp, XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_ATTR_X,
      tile_annotation.global_port_tile_coordinates(global_port_id)[tile_info_id]
        .x());
    write_xml_attribute(
      fp, XML_TILE_ANNOTATIONS_GLOBAL_PORT_TILE_ATTR_Y,
      tile_annotation.global_port_tile_coordinates(global_port_id)[tile_info_id]
        .y());
    fp << "/>";
  }

  fp << "\t\t"
     << "</" << XML_TILE_ANNOTATIONS_GLOBAL_PORT_NODE_NAME << ">";
}

/********************************************************************
 * A writer to output a device variation in a technology library to XML format
 *******************************************************************/
static void write_xml_tile_annotation_subtile_port_to_merge(
  std::fstream& fp, const char* fname, const std::string& tile_name,
  const std::string& port_name) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t"
     << "<" << XML_TILE_ANNOTATIONS_MERGE_SUBTILE_PORTS_NODE_NAME << " ";

  write_xml_attribute(fp, XML_TILE_ANNOTATIONS_MERGE_SUBTILE_PORTS_ATTR_TILE,
                      tile_name.c_str());
  write_xml_attribute(fp, XML_TILE_ANNOTATIONS_MERGE_SUBTILE_PORTS_ATTR_PORT,
                      port_name.c_str());

  fp << "/>";
}

/********************************************************************
 * A writer to output a device variation in a technology library to XML format
 *******************************************************************/
static void write_xml_tile_annotation_physical_equivalent_site(
  std::fstream& fp, const char* fname, const std::string& tile_name,
  const std::string& subtile_name, const std::string& site_name) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t"
     << "<" << XML_TILE_ANNOTATIONS_PHYSICAL_EQUIVALENT_SITE_NODE_NAME << " ";

  write_xml_attribute(fp,
                      XML_TILE_ANNOTATIONS_PHYSICAL_EQUIVALENT_SITE_ATTR_TILE,
                      tile_name.c_str());
  write_xml_attribute(
    fp, XML_TILE_ANNOTATIONS_PHYSICAL_EQUIVALENT_SITE_ATTR_SUBTILE,
    subtile_name.c_str());
  write_xml_attribute(fp,
                      XML_TILE_ANNOTATIONS_PHYSICAL_EQUIVALENT_SITE_ATTR_SITE,
                      site_name.c_str());

  fp << "/>";
}

/********************************************************************
 * A writer to output tile annotations to XML format
 *******************************************************************/
void write_xml_tile_annotations(std::fstream& fp, const char* fname,
                                const TileAnnotation& tile_annotation) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node for pb_type annotations,
   * we apply a tab becuase pb_type annotations is just a subnode
   * under the root node <openfpga_arch>
   */
  fp << "\t"
     << "<" << XML_TILE_ANNOTATIONS_NODE_NAME << ">"
     << "\n";

  /* Write device model one by one */
  for (const TileGlobalPortId& global_port_id :
       tile_annotation.global_ports()) {
    write_xml_tile_annotation_global_port(fp, fname, tile_annotation,
                                          global_port_id);
  }
  for (std::string tile_name : tile_annotation.tiles_to_merge_ports()) {
    for (std::string port_name :
         tile_annotation.tile_ports_to_merge(tile_name)) {
      write_xml_tile_annotation_subtile_port_to_merge(fp, fname, tile_name,
                                                      port_name);
    }
  }
  for (auto tile_info : tile_annotation.physical_equivalent_sites()) {
    write_xml_tile_annotation_physical_equivalent_site(
      fp, fname, tile_info.first, tile_info.second,
      tile_annotation.physical_equivalent_site(tile_info.first,
                                               tile_info.second));
  }

  /* Write the root node for pb_type annotations */
  fp << "\t"
     << "</" << XML_TILE_ANNOTATIONS_NODE_NAME << ">"
     << "\n";
}

}  // namespace openfpga
