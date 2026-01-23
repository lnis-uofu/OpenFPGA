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
#include <fstream>

#include "openfpga_port_parser.h"
/* Headers from libarchfpga */
#include "arch_error.h"
#include "command_exit_codes.h"
#include "openfpga_digest.h"
#include "read_xml_boundary_timing.h"
#include "read_xml_util.h"
#include "write_xml_utils.cpp"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Parse XML codes of a <pin> to an object of BoundaryTiming
 *******************************************************************/
static int write_xml_one_boundary_timing(
  std::fstream& fp, const BoundaryTiming& boundary_timing,
  const PinConstraintId& pin_constraint) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  openfpga::write_tab_to_file(fp, 1);
  fp << "<pin";

  if (false == boundary_timing.valid_pin_constraint_id(pin_constraint)) {
    return 1;
  }
  auto pin = boundary_timing.pin(pin_constraint);
  write_xml_attribute(
    fp, "name",
    generate_xml_port_name(boundary_timing.pin(pin_constraint)).c_str());
  write_xml_attribute(fp, "min", boundary_timing.pin_min_delay(pin).c_str());
  write_xml_attribute(fp, "max", boundary_timing.pin_max_delay(pin).c_str());

  fp << "/>"
     << "\n";

  return 0;
}

/********************************************************************
 * Parse XML codes about <boundary_timing> to an object of BoundaryTiming
 *******************************************************************/
int write_xml_boundary_timing(const char* fname,
                              const BoundaryTiming& bdy_timing) {
  vtr::ScopedStartFinishTimer timer("Write Boundary Timing File");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node */
  fp << "<boundary_timing>"
     << "\n";

  int err_code = 0;

  /* Write region by region */
  for (const PinConstraintId& pin_constraint : bdy_timing.pin_constraints()) {
    /* Write constraint by constraint */
    err_code = write_xml_one_boundary_timing(fp, bdy_timing, pin_constraint);
    if (0 != err_code) {
      return err_code;
    }
  }

  /* Finish writing the root node */
  fp << "</boundary_timing>"
     << "\n";

  /* Close the file stream */
  fp.close();

  return err_code;
}

} /* End namespace openfpga*/
