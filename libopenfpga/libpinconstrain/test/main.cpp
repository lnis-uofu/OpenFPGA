
/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "cmd_line.h"
#include "pin_location.h"
#include "pin_constrain_loc.h"

int main(int argc, const char** argv) {
  VTR_ASSERT(argc == PIN_C_ARGUMENT_NUMBER);
  string command_line;
  for (int i = 0; i < argc; i++) {
     if (i > 0) {
       command_line += " ";
     }
     command_line += argv[i];
  }
  VTR_LOG("Created command line <%s> for test.\n", command_line);
  cmd_line pin_c_cmd (argc, argv);
  VTR_LOG("Testing reader and writer.\n");
  int status = pin_constrain_location_cmd_line(pin_c_cmd);
  VTR_LOG("Test result: %s.\n", status == 0 ? "PASS" : "FAIL");
  return status;
}

