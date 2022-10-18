/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from fabric key */
#include "read_csv_io_pin_table.h"
#include "write_csv_io_pin_table.h"

int main(int argc, const char** argv) {
  /* Ensure we have only one or two argument */
  VTR_ASSERT((2 == argc) || (3 == argc));

  /* Parse the fabric key from an XML file */
  openfpga::IoPinTable io_pin_table = openfpga::read_csv_io_pin_table(
    argv[1], openfpga::e_pin_table_direction_convention::QUICKLOGIC);
  VTR_LOG("Read the I/O pin table from a csv file: %s.\n", argv[1]);

  /* Output to an XML file
   * This is optional only used when there is a second argument
   */
  if (3 <= argc) {
    write_csv_io_pin_table(argv[2], io_pin_table);
    VTR_LOG("Echo the I/O pin table to a csv file: %s.\n", argv[2]);
  }
}
