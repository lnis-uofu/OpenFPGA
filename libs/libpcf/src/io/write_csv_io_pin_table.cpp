/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML of pin constraints to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from libopenfpga util library */
#include "openfpga_port_parser.h"
/* Headers from arch openfpga library */
#include "write_csv_io_pin_table.h"
#include "write_xml_utils.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Write I/O pin table to a csv file
 *
 * Return 0 if successful
 * Return 1 if there are more serious errors
 * Return 2 if fail when creating files
 *******************************************************************/
int write_csv_io_pin_table(const char* fname, const IoPinTable& io_pin_table) {
  vtr::ScopedStartFinishTimer timer("Write I/O Pin Table");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* TODO: Move this to constants header file */
  std::array<std::string, IoPinTable::NUM_IO_DIRECTIONS> IO_DIRECTION_STRING = {
    "input", "output"};

  /* Print head row */
  std::vector<std::string> head_row_str(
    {"orientation", "port_name", "mapped_pin", "direction"});
  for (size_t icol = 0; icol < head_row_str.size(); icol++) {
    fp << head_row_str[icol];
    if (icol < head_row_str.size() - 1) {
      fp << ",";
    }
  }
  fp << "\n";

  /* Print data */
  for (const IoPinTableId& pin_id : io_pin_table.pins()) {
    std::vector<std::string> data_row_str;
    data_row_str.push_back(TOTAL_2D_SIDE_STRINGS[io_pin_table.pin_side(pin_id)]);
    data_row_str.push_back(
      generate_xml_port_name(io_pin_table.internal_pin(pin_id)));
    data_row_str.push_back(
      generate_xml_port_name(io_pin_table.external_pin(pin_id)));
    data_row_str.push_back(
      IO_DIRECTION_STRING[io_pin_table.pin_direction(pin_id)]);
    for (size_t icol = 0; icol < head_row_str.size(); icol++) {
      fp << data_row_str[icol];
      if (icol < data_row_str.size() - 1) {
        fp << ",";
      }
    }
    fp << "\n";
  }

  return 0;
}

} /* End namespace openfpga*/
