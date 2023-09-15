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

/* Headers from openfpga util library */
#include "openfpga_digest.h"

/* Headers from arch openfpga library */
#include "write_xml_utils.h"

/* Headers from pin constraint library */
#include "io_name_map_xml_constants.h"
#include "write_xml_io_name_map.h"

namespace openfpga {  // Begin namespace openfpga

/********************************************************************
 * A writer to output a I/O name mapping to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 *******************************************************************/
static int write_xml_io_map_port(std::fstream& fp, const IoNameMap& io_name_map,
                                 const BasicPort& fpga_top_port) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  openfpga::write_tab_to_file(fp, 1);
  fp << "<" << XML_IO_NAME_MAP_NODE_NAME << "";
  write_xml_attribute(fp, XML_IO_NAME_MAP_ATTRIBUTE_TOP_NAME,
                      generate_xml_port_name(fpga_top_port).c_str());

  if (io_name_map.fpga_top_port_is_dummy(fpga_top_port)) {
    write_xml_attribute(fp, XML_IO_NAME_MAP_ATTRIBUTE_IS_DUMMY, "true");
    IoNameMap::e_dummy_port_direction dir =
      io_name_map.fpga_top_dummy_port_direction(fpga_top_port);
    write_xml_attribute(fp, XML_IO_NAME_MAP_ATTRIBUTE_DIRECTION,
                        io_name_map.dummy_port_dir2str(dir, true).c_str());
  } else {
    BasicPort fpga_core_port = io_name_map.fpga_core_port(fpga_top_port);
    write_xml_attribute(fp, XML_IO_NAME_MAP_ATTRIBUTE_CORE_NAME,
                        generate_xml_port_name(fpga_core_port).c_str());
  }
  fp << ">"
     << "\n";

  return 0;
}

/********************************************************************
 * A writer to output an object to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 *******************************************************************/
int write_xml_io_name_map(const char* fname, const IoNameMap& io_name_map) {
  vtr::ScopedStartFinishTimer timer("Write I/O naming rules");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node */
  fp << "<" << XML_IO_NAME_MAP_ROOT_NAME;
  fp << ">"
     << "\n";

  int err_code = 0;

  /* Write each port */
  for (BasicPort fpga_top_port : io_name_map.fpga_top_ports()) {
    /* Write bus */
    err_code = write_xml_io_map_port(fp, io_name_map, fpga_top_port);
    if (0 != err_code) {
      return err_code;
    }
  }

  /* Finish writing the root node */
  fp << "</" << XML_IO_NAME_MAP_ROOT_NAME << ">"
     << "\n";

  /* Close the file stream */
  fp.close();

  return err_code;
}

}  // End of namespace openfpga
