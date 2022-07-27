/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML of pin constraints to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from libopenfpga util library */
#include "openfpga_port_parser.h"

#include "csv.hpp"

#include "read_csv_io_pin_table.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Parse XML codes about <pin_constraints> to an object of PinConstraints
 *******************************************************************/
IoPinTable read_csv_io_pin_table(const char* fname) {
  vtr::ScopedStartFinishTimer timer("Read I/O Pin Table");

  IoPinTable io_pin_table;

  csv::CSVFormat format;
  format.delimiter(',');
  format.quote('~');
  format.trim({'\t', ' '});
  format.header_row(0);

  csv::CSVReader reader(fname, format);

  /* TODO: Move this to constants */
  std::map<std::string, e_side> side_str_map { {"TOP", TOP}, {"RIGHT", RIGHT}, {"LEFT", LEFT}, {"BOTTOM", BOTTOM} };

  for (csv::CSVRow& row : reader) {
    IoPinTableId pin_id = io_pin_table.create_pin();
    /* Fill pin-level information */
    PortParser internal_pin_parser(row["port_name"].get<std::string>());
    io_pin_table.set_internal_pin(pin_id, internal_pin_parser.port());

    PortParser external_pin_parser(row["mapped_pin"].get<std::string>());
    io_pin_table.set_external_pin(pin_id, external_pin_parser.port());

    std::string pin_side_str = row["orientation"].get<std::string>();
    if (side_str_map.end() == side_str_map.find(pin_side_str)) {
      VTR_LOG("Invalid side defintion! Expect [TOP|RIGHT|LEFT|BOTTOM]\n");
      exit(1);
    } else {
      io_pin_table.set_pin_side(pin_id, side_str_map[pin_side_str]);
    }

    /*This is not general purpose: we should have an explicit attribute in the csv file to decalare direction */
    if (internal_pin_parser.port().get_name().find("A2F") != std::string::npos) {
      io_pin_table.set_pin_direction(pin_id, IoPinTable::INPUT);
    } else if (internal_pin_parser.port().get_name().find("F2A") != std::string::npos) {
      io_pin_table.set_pin_direction(pin_id, IoPinTable::OUTPUT);
    } else {
      VTR_LOG("Invalid direction defintion! Expect [A2F|F2A] in the pin name\n");
      exit(1);
    }
  }

  return io_pin_table;
}

} /* End namespace openfpga*/



