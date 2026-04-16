#ifndef VPR_MAIN_H
#define VPR_MAIN_H

#include "command.h"
#include "command_context.h"
#include "openfpga_context.h"
#include "shell.h"

/* Begin namespace vpr */
namespace vpr {

int place_template(openfpga::Shell<OpenfpgaContext>* shell,
                   OpenfpgaContext& openfpga_ctx, const openfpga::Command& cmd,
                   const openfpga::CommandContext& cmd_context);

int route_template(openfpga::Shell<OpenfpgaContext>* shell,
                   OpenfpgaContext& openfpga_ctx, const openfpga::Command& cmd,
                   const openfpga::CommandContext& cmd_context);

int analysis_template(openfpga::Shell<OpenfpgaContext>* shell,
                      OpenfpgaContext& openfpga_ctx,
                      const openfpga::Command& cmd,
                      const openfpga::CommandContext& cmd_context);

int vpr_wrapper(int argc, char** argv);

int vpr_standalone_wrapper(int argc, char** argv);

int read_vpr_arch_template(openfpga::Shell<OpenfpgaContext>* shell,
                           OpenfpgaContext& openfpga_ctx,
                           const openfpga::Command& cmd,
                           const openfpga::CommandContext& cmd_context);

int show_vpr_setup_template(openfpga::Shell<OpenfpgaContext>* shell,
                            OpenfpgaContext& openfpga_ctx,
                            const openfpga::Command& cmd,
                            const openfpga::CommandContext& cmd_context);

int read_circuit_template(openfpga::Shell<OpenfpgaContext>* shell,
                          OpenfpgaContext& openfpga_ctx,
                          const openfpga::Command& cmd,
                          const openfpga::CommandContext& cmd_context);

int pack_template(openfpga::Shell<OpenfpgaContext>* shell,
                  OpenfpgaContext& openfpga_ctx, const openfpga::Command& cmd,
                  const openfpga::CommandContext& cmd_context);

} /* End namespace vpr */

#endif
