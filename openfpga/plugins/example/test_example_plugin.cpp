/********************************************************************
 * Test of the plugins of the shell with the example plugin built in
 * (OPENFPGA_PLUGINS=example): what it added, overrode, removed and hooked is
 * there, and is said by report_plugins, version and help
 *******************************************************************/
#include <cstdio>
#include <string>

#include "openfpga_shell.h"

static int num_failures = 0;

#define CHECK(cond)                                                         \
  do {                                                                      \
    if (!(cond)) {                                                          \
      std::fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, __LINE__, \
                   #cond);                                                  \
      ++num_failures;                                                       \
    }                                                                       \
  } while (false)

static bool has(const std::string& text, const std::string& part) {
  return std::string::npos != text.find(part);
}

int main() {
  OpenfpgaShell shell;

  const CommandResult plugins = shell.execute("report_plugins");
  CHECK(0 == plugins.status);
  CHECK(has(plugins.data, "\"kind\":\"plugins\""));
  CHECK(has(plugins.data, "\"name\":\"example\",\"version\":\"1.0\""));
  CHECK(has(plugins.data, "\"commands\":[\"example_report\"]"));
  CHECK(has(plugins.data, "\"overrides\":[\"write_fabric_spice\"]"));
  CHECK(has(plugins.data,
            "\"removes\":[\"write_sdc_disable_timing_configure_ports\"]"));
  CHECK(
    has(plugins.data, "{\"command\":\"read_vpr_arch\",\"when\":\"after\"}"));
  CHECK(
    has(plugins.data, "{\"command\":\"read_circuit\",\"when\":\"before\"}"));
  CHECK(has(plugins.data, "\"every_command\":true"));

  /* The build may have other plugins too */
  const std::string version = shell.execute("version").log;
  CHECK(has(version, "Plugins: ") && has(version, "example 1.0"));
  const std::string help = shell.execute("help").log;
  CHECK(has(help, "example_report (plugin example)"));
  CHECK(has(help, "write_fabric_spice (overridden by plugin example)"));
  CHECK(has(help, "Removed by plugins:"));
  CHECK(has(help, "write_sdc_disable_timing_configure_ports (plugin example)"));

  /* The removed command fails and says why */
  const CommandResult removed =
    shell.execute("write_sdc_disable_timing_configure_ports --file x.sdc");
  CHECK(0 != removed.status);
  CHECK(has(removed.log, "removed by plugin 'example'"));

  /* The hooks: the architecture is seen, a circuit is refused */
  const std::string arch =
    std::string(OPENFPGA_SOURCE_DIR) +
    "/openfpga_flow/vpr_arch/k6_frac_N10_tileable_40nm.xml";
  CHECK(
    0 ==
    shell.execute(("read_vpr_arch --file " + arch + " --layout auto").c_str())
      .status);
  const CommandResult refused =
    shell.execute("read_circuit --file refused_by_example.blif");
  CHECK(0 != refused.status);
  CHECK(has(refused.log, "refused by plugin 'example'"));
  /* The option of the plugin: its owner, and its value */
  const CommandResult options =
    shell.execute("report_app_option --name example.greeting --json");
  CHECK(0 == options.status);
  CHECK(has(options.data, "\"owner\":\"example\""));
  CHECK(has(options.data, "\"stage\":\"ANALYZED\""));
  CHECK(0 ==
        shell.execute("set_app_option --name example.greeting --value Bonjour")
          .status);
  const std::string report = shell.execute("example_report").log;
  CHECK(has(report, "Bonjour"));
  CHECK(has(report, "Architecture read: " + arch));
  CHECK(!has(report, "Commands seen by the example plugin: 0"));

  if (0 == num_failures) {
    std::printf("test_example_plugin: all checks passed\n");
  }
  return 0 == num_failures ? 0 : 1;
}
