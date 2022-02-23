/********************************************************************
 * Build the OpenFPGA shell interface 
 *******************************************************************/

/********************************************************************
 * Main function to start OpenFPGA shell interface
 *******************************************************************/
#include "openfpga_invoker.h"

int main(int argc, char** argv) {

  std::vector<std::string> cmd_opts;
  for (int iarg = 1; iarg < argc; ++iarg) {
    cmd_opts.push_back(std::string(argv[iarg]));
  }

  int return_status = mode_invoker(cmd_opts,false);
  return return_status;

}

