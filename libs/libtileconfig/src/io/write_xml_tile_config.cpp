/********************************************************************
 * This file includes functions that outputs a clock network object to XML
 *format
 *******************************************************************/
/* Headers from system goes first */
#include <algorithm>
#include <string>

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from arch openfpga library */
#include "openfpga_digest.h"
#include "write_xml_utils.h"

/* Headers from pin constraint library */
#include "tile_config_xml_constants.h"
#include "write_xml_tile_config.h"

namespace openfpga {  // Begin namespace openfpga

/********************************************************************
 * A writer to output an object to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 *******************************************************************/
int write_xml_tile_config(const char* fname, const TileConfig& tile_config) {
  vtr::ScopedStartFinishTimer timer("Write tile configuration rules");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  int err_code = 0;

  /* Write the root node */
  fp << "<" << XML_TILE_CONFIG_ROOT_NAME;

  write_xml_attribute(fp, XML_TILE_CONFIG_ATTRIBUTE_STYLE_NAME,
                      tile_config.style_to_string().c_str());
  /* Finish writing the root node */
  fp << "/>"
     << "\n";

  /* Close the file stream */
  fp.close();

  return err_code;
}

}  // End of namespace openfpga
