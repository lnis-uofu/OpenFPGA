#ifndef VPR_MAIN_H
#define VPR_MAIN_H

#include "command.h"
#include "command_context.h"

/* Begin namespace vpr */
namespace vpr {

int vpr_wrapper(int argc, char** argv);

int vpr_standalone_wrapper(int argc, char** argv);

int read_vpr_arch_template(const openfpga::Command& cmd,
                           const openfpga::CommandContext& cmd_context);

} /* End namespace vpr */

#endif
