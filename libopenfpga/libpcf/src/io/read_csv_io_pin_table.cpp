/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML of pin constraints to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_time.h"

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


  return io_pin_table;
}

} /* End namespace openfpga*/



