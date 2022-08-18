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

IoPinTable read_csv_io_pin_table(const char* fname);

} /* End namespace openfpga*/

#endif
