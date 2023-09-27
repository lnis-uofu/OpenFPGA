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
     << "<global_port ";

  write_xml_attribute(fp, "name",
                      tile_annotation.global_port_name(global_port_id).c_str());

  write_xml_attribute(fp, "is_clock",
                      tile_annotation.global_port_is_clock(global_port_id));

  if (tile_annotation.global_port_is_clock(global_port_id) &&
      !tile_annotation.global_port_clock_arch_tree_name(global_port_id)
         .empty()) {
    write_xml_attribute(
      fp, "clock_arch_tree_name",
      tile_annotation.global_port_clock_arch_tree_name(global_port_id).c_str());
  }

  write_xml_attribute(fp, "is_set",
                      tile_annotation.global_port_is_set(global_port_id));

  write_xml_attribute(fp, "is_reset",
                      tile_annotation.global_port_is_reset(global_port_id));

  write_xml_attribute(
    fp, "default_value",
    tile_annotation.global_port_default_value(global_port_id));

  fp << ">"
     << "\n";

  for (size_t tile_info_id = 0;
       tile_info_id <
       tile_annotation.global_port_tile_names(global_port_id).size();
       ++tile_info_id) {
    fp << "\t\t\t"
       << "<tile ";
    write_xml_attribute(
      fp, "name",
      tile_annotation.global_port_tile_names(global_port_id)[tile_info_id]
        .c_str());
    write_xml_attribute(
      fp, "port",
      generate_xml_port_name(
        tile_annotation.global_port_tile_ports(global_port_id)[tile_info_id])
        .c_str());
    write_xml_attribute(
      fp, "x",
      tile_annotation.global_port_tile_coordinates(global_port_id)[tile_info_id]
        .x());
    write_xml_attribute(
      fp, "y",
      tile_annotation.global_port_tile_coordinates(global_port_id)[tile_info_id]
        .y());
    fp << "/>";
  }

  fp << "\t\t"
     << "</global_port>";
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
     << "<merge_subtile_ports ";

  write_xml_attribute(fp, "tile", tile_name.c_str());
  write_xml_attribute(fp, "port", port_name.c_str());

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
     << "<tile_annotations>"
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

  /* Write the root node for pb_type annotations */
  fp << "\t"
     << "</tile_annotations>"
     << "\n";
}

}  // namespace openfpga
