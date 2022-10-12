#ifndef WRITE_CSV_IO_PIN_TABLE_H
#define WRITE_CSV_IO_PIN_TABLE_H

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

int write_csv_io_pin_table(const char* fname, const IoPinTable& io_pin_table);

} /* End namespace openfpga*/

#endif
