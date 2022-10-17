#ifndef READ_CSV_IO_PIN_TABLE_H
#define READ_CSV_IO_PIN_TABLE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

#include "io_pin_table.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

/* Option to read csv */
enum class e_pin_table_direction_convention {
  EXPLICIT = 0,
  QUICKLOGIC,
  NUM_PIN_DIRECTION_CONVENTION
};
constexpr std::array<const char*, NUM_PIN_DIRECTION_CONVENTION> PIN_TABLE_DIRECTION_CONVENTION_STRING = {{"explicit", "quicklogic"}}; //String versions of side orientations

IoPinTable read_csv_io_pin_table(const char* fname, const e_pin_table_direction_convention& pin_dir_convention);

} /* End namespace openfpga*/

#endif
