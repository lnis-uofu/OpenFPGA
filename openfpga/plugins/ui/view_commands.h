#ifndef VIEW_COMMANDS_H
#define VIEW_COMMANDS_H

/********************************************************************
 * The commands that write the data of the views of a design
 * (docs/plan_design_views.md, 3.1), see view_export.h:
 * - export_view --kind <kind> --file <path> [--module, --cone, --depth,
 *   --blocks, --limit, --paths]: a whole export
 * - view_tile --kind <kind> --level <l> --x <i> --y <j> --file <path>: a
 *   tile of the device
 * They are queries: they write a file and answer JSON (kind view_export)
 * with the file, its version, its size and the rows of its tables
 *******************************************************************/
#include "openfpga_context.h"
#include "shell.h"

/* begin namespace openfpga */
namespace openfpga {

void add_view_commands(Shell<OpenfpgaContext>& shell);

} /* end namespace openfpga */

#endif
