/********************************************************************
 * Build the OpenFPGA shell interface
 *******************************************************************/
#include <cstdio>

#include "openfpga_shell.h"

/********************************************************************
 * Main function to start OpenFPGA shell interface
 *******************************************************************/
int main(int argc, char** argv) {
  /* A plugin of the build that cannot register (a name defined twice, a
   * command it cannot remove) stops the start, with the reason */
  try {
    OpenfpgaShell openfpga_shell;
    return openfpga_shell.start(argc, argv);
  } catch (const openfpga::ShellRegistrationError& error) {
    std::fprintf(stderr, "OpenFPGA cannot start: %s\n", error.what());
    return 1;
  }
}
