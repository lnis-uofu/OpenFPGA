#ifndef SESSION_PIN_CONSTRAINTS_H
#define SESSION_PIN_CONSTRAINTS_H
/********************************************************************
 * The pin constraints file of a command that takes --pin_constraints_file:
 * the one of its option or, when it is not given, the one that the
 * constraints of the session give (SessionConstraints, which a plugin sets)
 *******************************************************************/
#include <string>

#include "command.h"
#include "command_context.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

template <class T>
std::string pin_constraints_file(const T& openfpga_ctx, const Command& cmd,
                                 const CommandContext& cmd_context,
                                 const CommandOptionId& opt_pcf) {
  if (cmd_context.option_enable(cmd, opt_pcf)) {
    return cmd_context.option_value(cmd, opt_pcf);
  }
  const std::string& file =
    openfpga_ctx.session().constraints().pin_constraints_file;
  if (!file.empty()) {
    VTR_LOG("Use the pin constraints of the constraints of the session: '%s'\n",
            file.c_str());
  }
  return file;
}

} /* end namespace openfpga */

#endif
