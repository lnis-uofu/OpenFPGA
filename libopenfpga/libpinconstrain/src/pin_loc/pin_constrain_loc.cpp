#include "pin_location.h"
#include "cmd_line.h"
#include "pin_constrain_loc.h"
#include <vector>
#include <string>

// wraper function to do the real job, it is used by openfpga shell as well
// this gurantees same behavior
int pin_constrain_location (vector<string> args) {
  if (args.size() != PIN_C_ARGUMENT_NUMBER ) {
    return 1;
  }
  const char* pin_c_args [PIN_C_ARGUMENT_NUMBER];
  for (int i = 0; i < PIN_C_ARGUMENT_NUMBER; i++) {
    pin_c_args[i] = const_cast<char*> (args[i].c_str());
  }
  cmd_line pin_c_cmd (PIN_C_ARGUMENT_NUMBER, pin_c_args);
  return pin_constrain_location_cmd_line(pin_c_cmd);
}

// base function for both openfpga wrapper and pin_c executable
int pin_constrain_location_cmd_line (cmd_line& cmd) {
  pin_location pl (cmd);
  //pl.get_cmd().print_options();
  if (!pl.reader_and_writer()) {
    return 1;
  }
  return 0;
}

