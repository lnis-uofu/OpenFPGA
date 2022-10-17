/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from fabric key */
#include "blif_head_reader.h"
#include "io_net_place.h"
#include "pcf2place.h"
#include "pcf_reader.h"
#include "read_csv_io_pin_table.h"
#include "read_xml_io_location_map.h"

int main(int argc, const char** argv) {
  /* Ensure we have the following arguments:
   * 1. Input - Users Design Constraints (.pcf)
   * 2. Input - Netlist (.blif)
   * 3. Input - Fabic I/O location map (.xml)
   * 4. Input - Chip pin table (.csv)
   * 5. Output - I/O placement (.place)
   */
  VTR_ASSERT(6 == argc);

  /* Parse the input files */
  openfpga::PcfData pcf_data;
  openfpga::read_pcf(argv[1], pcf_data);
  VTR_LOG("Read the design constraints from a pcf file: %s.\n", argv[1]);

  blifparse::BlifHeadReader callback;
  blifparse::blif_parse_filename(argv[2], callback);
  VTR_LOG("Read the blif from a file: %s.\n", argv[2]);
  if (callback.had_error()) {
    VTR_LOG("Read the blif ends with errors\n", argv[2]);
    return 1;
  }

  openfpga::IoLocationMap io_location_map =
    openfpga::read_xml_io_location_map(argv[3]);
  VTR_LOG("Read the I/O location map from an XML file: %s.\n", argv[3]);

  openfpga::IoPinTable io_pin_table = openfpga::read_csv_io_pin_table(
    argv[4], openfpga::e_pin_table_direction_convention::QUICKLOGIC);
  VTR_LOG("Read the I/O pin table from a csv file: %s.\n", argv[4]);

  /* Convert */
  openfpga::IoNetPlace io_net_place;
  int status =
    pcf2place(pcf_data, callback.input_pins(), callback.output_pins(),
              io_pin_table, io_location_map, io_net_place);
  if (status) {
    return status;
  }

  /* Output */
  status = io_net_place.write_to_place_file(argv[5], true, true);

  return status;
}
