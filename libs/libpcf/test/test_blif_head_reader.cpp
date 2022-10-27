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

int main(int argc, const char** argv) {
  /* Ensure we have only one or two argument */
  VTR_ASSERT(2 == argc);

  /* Parse the blif */
  blifparse::BlifHeadReader callback;
  blifparse::blif_parse_filename(argv[1], callback);
  VTR_LOG("Read the blif from a file: %s.\n", argv[1]);

  if (callback.had_error()) {
    VTR_LOG("Read the blif ends with errors\n", argv[1]);
    return 1;
  }

  /* Output */
  VTR_LOG("Input pins: \n");
  for (const std::string& pin : callback.input_pins()) {
    VTR_LOG("%s\n", pin.c_str());
  }
  VTR_LOG("Output pins: \n");
  for (const std::string& pin : callback.output_pins()) {
    VTR_LOG("%s\n", pin.c_str());
  }

  return 0;
}
