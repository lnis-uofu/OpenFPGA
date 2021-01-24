/********************************************************************
 * This file includes functions that outputs a repack design constraint object to XML format
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

/* Headers from fabrickey library */
#include "write_xml_repack_design_constraints.h"

/********************************************************************
 * A writer to output a pin constraint to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 *******************************************************************/
static 
int write_xml_pin_constraint(std::fstream& fp,
                             const RepackDesignConstraints& repack_design_constraints,
                             const RepackDesignConstraintId& design_constraint) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  openfpga::write_tab_to_file(fp, 1);
  fp << "<pin_constraint";

  if (false == repack_design_constraints.valid_design_constraint_id(design_constraint)) {
    return 1;
  }

  write_xml_attribute(fp, "pb_type", repack_design_constraints.pb_type(design_constraint).c_str());
  write_xml_attribute(fp, "pin", generate_xml_port_name(repack_design_constraints.pin(design_constraint)).c_str());
  write_xml_attribute(fp, "net", repack_design_constraints.net(design_constraint).c_str());

  fp << "/>" << "\n";

  return 0;
}

/********************************************************************
 * A writer to output a repack design constraint object to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 *******************************************************************/
int write_xml_repack_design_constraints(const char* fname,
                                        const RepackDesignConstraints& repack_design_constraints) {

  vtr::ScopedStartFinishTimer timer("Write Repack Design Constraints");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node */
  fp << "<repack_design_constraints>" << "\n";

  int err_code = 0;

  /* Write region by region */ 
  for (const RepackDesignConstraintId& design_constraint : repack_design_constraints.design_constraints()) {
    /* Write constraint by constraint */ 
    if (RepackDesignConstraints::PIN_ASSIGNMENT == repack_design_constraints.type(design_constraint)) {
      err_code = write_xml_pin_constraint(fp, repack_design_constraints, design_constraint);
      if (0 != err_code) {
        return err_code;
      }
    }
  }

  /* Finish writing the root node */
  fp << "</repack_design_constraints>" << "\n";

  /* Close the file stream */
  fp.close();

  return err_code;
}
