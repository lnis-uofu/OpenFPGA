/********************************************************************
 * The ui plugin: the queries of the flow (report_session, timing_summary,
 * placement, ...) and the commands of the application options
 * (set_app_option, reset_app_option, report_app_option), and the
 * constraints of ODC (read_odc, write_odc, clear_odc, report_constraints),
 * and the data of the views (export_view, view_tile).
 * The Python module, the MCP server and the app read the flow through them
 *******************************************************************/
#include "app_options_commands.h"
#include "odc_commands.h"
#include "openfpga_plugins.h"
#include "query_commands.h"
#include "view_commands.h"

/* begin namespace openfpga */
namespace openfpga {

/* Declared for the list of the plugins of the build, which calls it */
void register_plugin_ui(PluginApi& api);

void register_plugin_ui(PluginApi& api) {
  add_app_options_commands(api.shell());
  add_query_commands(api.shell());
  add_odc_commands(api.shell());
  add_view_commands(api.shell());
}

} /* end namespace openfpga */
