/********************************************************************
 * This file includes functions that outputs a pin constraint object to XML format
 *******************************************************************/
/* Headers from system goes first */
#include <string>
#include <algorithm>

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpga util library */
#include "openfpga_digest.h"

/* Headers from arch openfpga library */
#include "write_xml_utils.h" 

/* Headers from pin constraint library */
#include "write_xml_pin_constraints.h"

/********************************************************************
 * A writer to output a pin constraint to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 *******************************************************************/
static 
int write_xml_pin_constraint(std::fstream& fp,
                             const PinConstraints& pin_constraints,
                             const PinConstraintId& pin_constraint) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  openfpga::write_tab_to_file(fp, 1);
  fp << "<set_io";

  if (false == pin_constraints.valid_pin_constraint_id(pin_constraint)) {
    return 1;
  }

  write_xml_attribute(fp, "pin", generate_xml_port_name(pin_constraints.pin(pin_constraint)).c_str());
  write_xml_attribute(fp, "net", pin_constraints.net(pin_constraint).c_str());
  write_xml_attribute(fp, "default_value", pin_constraints.net_default_value_to_string(pin_constraint).c_str());

  fp << "/>" << "\n";

  return 0;
}

/********************************************************************
 * A writer to output a repack pin constraint object to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 *******************************************************************/
int write_xml_pin_constraints(const char* fname,
                              const PinConstraints& pin_constraints) {

  vtr::ScopedStartFinishTimer timer("Write Pin Constraints");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node */
  fp << "<pin_constraints>" << "\n";

  int err_code = 0;

  /* Write region by region */ 
  for (const PinConstraintId& pin_constraint : pin_constraints.pin_constraints()) {
    /* Write constraint by constraint */ 
    err_code = write_xml_pin_constraint(fp, pin_constraints, pin_constraint);
    if (0 != err_code) {
      return err_code;
    }
  }

  /* Finish writing the root node */
  fp << "</pin_constraints>" << "\n";

  /* Close the file stream */
  fp.close();

  return err_code;
}
