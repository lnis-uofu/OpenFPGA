#ifndef SYNTHESIS_COMMANDS_H
#define SYNTHESIS_COMMANDS_H
/********************************************************************
 * The commands of synthesis: synthesize (Verilog files and a top module),
 * read_design (a design directory) and report_design_hierarchy (the tree of
 * the instances of a design), see design_synthesis.h
 *******************************************************************/
#include "openfpga_context.h"
#include "shell.h"

/* begin namespace openfpga */
namespace openfpga {

void add_synthesis_commands(Shell<OpenfpgaContext>& shell);

} /* end namespace openfpga */

#endif
