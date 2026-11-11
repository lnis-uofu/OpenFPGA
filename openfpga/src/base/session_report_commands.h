#ifndef SESSION_REPORT_COMMANDS_H
#define SESSION_REPORT_COMMANDS_H

#include "openfpga_context.h"
#include "shell.h"

/********************************************************************
 * Commands that report what the flow that is run stage by stage did. Each
 * prints its answer as JSON and gives it as the data of the result, see
 * json_response.h. They do not change anything:
 * - report_session: the stages that are reached, the inputs and the files of
 *   the results, see session_json.h. Needs no stage
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

void add_session_report_commands(openfpga::Shell<OpenfpgaContext>& shell);

} /* End namespace openfpga */

#endif
