/********************************************************************
 * Unit test functions to validate the correctness of
 * 1. parser of data structures
 * 2. writer of data structures
 *******************************************************************/
/* Headers from vtrutils */
#include <iostream>

#include "read_blif_clock_info.h"

int main(int argc, const char** argv) {
  /* Ensure we have the following arguments:
   * 1. Input - arch file
   * 2. Input - blif file
   * 3. Output - All clocks in the blif
   */
  std::vector<std::string> clock_names = read_blif_clock_info(argv[1], argv[2]);
  VTR_LOG("clock size is: %d \n", clock_names.size());
  for (auto i : clock_names) {
    VTR_LOG("clock name: %s \n", i.c_str());
  }
  return 0;
}
