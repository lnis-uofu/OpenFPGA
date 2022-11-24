/********************************************************************
 * Build the OpenFPGA shell interface
 *******************************************************************/
#include "openfpga_shell.h"

/********************************************************************
 * Main function to start OpenFPGA shell interface
 *******************************************************************/
int main(int argc, char** argv) {
  OpenfpgaShell openfpga_shell;
  return openfpga_shell.start(argc, argv);
}
