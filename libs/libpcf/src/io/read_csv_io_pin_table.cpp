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
#include "rapidcsv.h"
#include "read_csv_io_pin_table.h"

/* Begin namespace openfpga */
namespace openfpga {

/* Constants for io pin table csv parser */
constexpr const int ROW_INDEX_INTERNAL_PIN = 4;
constexpr const int ROW_INDEX_EXTERNAL_PIN = 5;
constexpr const int ROW_INDEX_DIRECTION = 6;
constexpr const int ROW_INDEX_SIDE = 0;
constexpr const char* DIRECTION_INPUT = "in";
constexpr const char* DIRECTION_OUTPUT = "out";

/********************************************************************
 * Parse XML codes about <pin_constraints> to an object of PinConstraints
 *******************************************************************/
IoPinTable read_csv_io_pin_table(
  const char* fname,
  const e_pin_table_direction_convention& pin_dir_convention) {
  vtr::ScopedStartFinishTimer timer("Read I/O Pin Table");

  IoPinTable io_pin_table;

  rapidcsv::Document doc(fname, rapidcsv::LabelParams(-1, -1),
                         rapidcsv::SeparatorParams(','));

  /* TODO: Move this to constants */
  std::map<std::string, e_side> side_str_map{
    {"TOP", TOP}, {"RIGHT", RIGHT}, {"LEFT", LEFT}, {"BOTTOM", BOTTOM}};

  int num_rows = doc.GetRowCount();
  io_pin_table.reserve_pins(num_rows);

  for (int irow = 1; irow < num_rows; irow++) {
    std::vector<std::string> row_vec = doc.GetRow<std::string>(irow);
    IoPinTableId pin_id = io_pin_table.create_pin();
    /* Fill pin-level information */
    PortParser internal_pin_parser(row_vec.at(ROW_INDEX_INTERNAL_PIN));
    io_pin_table.set_internal_pin(pin_id, internal_pin_parser.port());

    PortParser external_pin_parser(row_vec.at(ROW_INDEX_EXTERNAL_PIN));
    io_pin_table.set_external_pin(pin_id, external_pin_parser.port());

    std::string pin_side_str = row_vec.at(ROW_INDEX_SIDE);
    if (side_str_map.end() == side_str_map.find(pin_side_str)) {
      VTR_LOG(
        "Invalid side defintion (='%s')! Expect [TOP|RIGHT|LEFT|BOTTOM]\n",
        pin_side_str.c_str());
      exit(1);
    } else {
      io_pin_table.set_pin_side(pin_id, side_str_map[pin_side_str]);
    }

    /*This is not general purpose: we should have an explicit attribute in the
     * csv file to decalare direction */
    if (pin_dir_convention == e_pin_table_direction_convention::QUICKLOGIC) {
      if (internal_pin_parser.port().get_name().find("A2F") !=
          std::string::npos) {
        io_pin_table.set_pin_direction(pin_id, IoPinTable::INPUT);
      } else if (internal_pin_parser.port().get_name().find("F2A") !=
                 std::string::npos) {
        io_pin_table.set_pin_direction(pin_id, IoPinTable::OUTPUT);
      } else {
        VTR_LOG(
          "Invalid direction defintion! Expect [A2F|F2A] in the pin name\n");
        exit(1);
      }
    }

    /* Parse pin direction from a specific column, this has a higher priority
     * than inferring from pin names */
    std::string port_dir_str = row_vec.at(ROW_INDEX_DIRECTION);
    if (port_dir_str == std::string(DIRECTION_INPUT)) {
      io_pin_table.set_pin_direction(pin_id, IoPinTable::INPUT);
    } else if (port_dir_str == std::string(DIRECTION_OUTPUT)) {
      io_pin_table.set_pin_direction(pin_id, IoPinTable::OUTPUT);
    } else if (pin_dir_convention ==
               e_pin_table_direction_convention::EXPLICIT) {
      /* Error out only when we need explicit port direction */
      VTR_LOG(
        "Invalid direction defintion! Expect [%s|%s] in the GPIO direction\n",
        DIRECTION_INPUT, DIRECTION_OUTPUT);
      exit(1);
    }
  }

  return io_pin_table;
}

} /* End namespace openfpga*/
