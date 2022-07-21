#include "pin_location.h"
#include "cmd_line.h"
#include <vector>
#include <string>

// wraper function to do the real job, it is used by openfpga shell as well
// this gurantees same behavior
int pin_constrain_location (std::vector<std::string> args);

// base function for both openfpga wrapper and pin_c executable
int pin_constrain_location_cmd_line (cmd_line& cmd); 

