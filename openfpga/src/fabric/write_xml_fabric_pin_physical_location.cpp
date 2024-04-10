/***************************************************************************************
 * Output internal structure of DeviceRRGSB to XML format
 ***************************************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
#include "fabric_pin_physical_location_xml_constants.h"
#include "write_xml_fabric_pin_physical_location.h"

/* begin namespace openfpga */
namespace openfpga {

int write_xml_fabric_pin_physical_location(
  const char* fname, const std::string& module_name,
  const ModuleGraph& module_manager,
  const bool& include_time_stamp,
  const bool& verbose) {

  vtr::ScopedStartFinishTimer timer("Write fabric pin physical location");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  write_xml_fabric_pin_physical_location_file_head(fp, include_time_stamp);

  /* Write the root node */
  fp << "<" << XML_PINLOC_ROOT_NAME;
  fp << ">"
     << "\n";

  int err_code = 0;

  /* Write each port */



  /* Finish writing the root node */
  fp << "</" << XML_PINLOC_ROOT_NAME << ">"
     << "\n";

  /* Close the file stream */
  fp.close();

  VTR_LOGV(verbose, "Outputted %lu modules with pin physical location.\n", cnt);

  return err_code;
} /* end namespace openfpga */
