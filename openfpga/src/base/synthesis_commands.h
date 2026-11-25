#ifndef SYNTHESIS_COMMANDS_H
#define SYNTHESIS_COMMANDS_H
/********************************************************************
 * The commands of synthesis: synthesize (Verilog files and a top module) and
 * read_design (a design directory, docs/spec_design_directory.md R-34)
 *******************************************************************/
#include "openfpga_context.h"
#include "shell.h"

/* begin namespace openfpga */
namespace openfpga {

void add_synthesis_commands(Shell<OpenfpgaContext>& shell);

} /* end namespace openfpga */

#endif
