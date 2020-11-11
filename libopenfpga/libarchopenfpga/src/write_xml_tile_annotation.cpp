/********************************************************************
 * This file includes functions that outputs tile annotations to XML format
 *******************************************************************/
/* Headers from system goes first */
#include <string>
#include <algorithm>

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "openfpga_digest.h"

/* Headers from readarchopenfpga library */
#include "write_xml_utils.h" 
#include "write_xml_tile_annotation.h"

/* namespace openfpga begins */
namespace openfpga {

/********************************************************************
 * FIXME: Use a common function to output ports
 * Generate the full hierarchy name for a operating pb_type
 *******************************************************************/
static 
std::string generate_tile_port_name(const BasicPort& pb_port) {
  std::string port_name;

  /* Output format: <port_name>[<LSB>:<MSB>] */
  port_name += pb_port.get_name();
  port_name += std::string("[");
  port_name += std::to_string(pb_port.get_lsb());
  port_name += std::string(":");
  port_name += std::to_string(pb_port.get_msb());
  port_name += std::string("]");

  return port_name;
}

/********************************************************************
 * A writer to output a device variation in a technology library to XML format
 *******************************************************************/
static 
void write_xml_tile_annotation_global_port(std::fstream& fp,
                                           const char* fname,
                                           const openfpga::TileAnnotation& tile_annotation,
                                           const TileGlobalPortId& global_port_id) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t\t" << "<global_port ";

  write_xml_attribute(fp, "name", tile_annotation.global_port_name(global_port_id).c_str());

  std::string tile_port_attr = tile_annotation.global_port_tile_name(global_port_id)
                             + "."
                             + generate_tile_port_name(tile_annotation.global_port_tile_port(global_port_id));
  write_xml_attribute(fp, "tile_port", tile_port_attr.c_str()); 

  write_xml_attribute(fp, "is_clock", tile_annotation.global_port_is_clock(global_port_id)); 

  write_xml_attribute(fp, "is_set", tile_annotation.global_port_is_set(global_port_id)); 

  write_xml_attribute(fp, "is_reset", tile_annotation.global_port_is_reset(global_port_id)); 

  write_xml_attribute(fp, "default_value", tile_annotation.global_port_default_value(global_port_id)); 
  
  fp << "/>" << "\n";
}

/********************************************************************
 * A writer to output tile annotations to XML format
 *******************************************************************/
void write_xml_tile_annotations(std::fstream& fp,
                                const char* fname,
                                const TileAnnotation& tile_annotation) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node for pb_type annotations, 
   * we apply a tab becuase pb_type annotations is just a subnode 
   * under the root node <openfpga_arch>
   */
  fp << "\t" << "<tile_annotations>" << "\n";

  /* Write device model one by one */ 
  for (const TileGlobalPortId& global_port_id : tile_annotation.global_ports()) {
    write_xml_tile_annotation_global_port(fp, fname, tile_annotation, global_port_id);
  }

  /* Write the root node for pb_type annotations */
  fp << "\t" << "</tile_annotations>" << "\n";
}

} /* namespace openfpga ends */
