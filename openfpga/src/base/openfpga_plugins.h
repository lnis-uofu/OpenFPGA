#ifndef OPENFPGA_PLUGINS_H
#define OPENFPGA_PLUGINS_H
/********************************************************************
 * The plugins of the shell, chosen when OpenFPGA is built
 * (cmake -DOPENFPGA_PLUGINS="a;b"; see docs/plan_shell_plugins.md).
 *
 * A plugin is a static library with one function
 *   void openfpga::register_plugin_<name>(openfpga::PluginApi& api);
 * which adds commands to the shell, overrides or removes some of the
 * shell's own, and hooks commands, through api.shell(). Everything it does
 * is recorded under its name. CMake writes the list of the plugins of the
 * build (openfpga_plugin_list.cpp), in the order of OPENFPGA_PLUGINS.
 *******************************************************************/
#include <string>
#include <vector>

#include "openfpga_context.h"
#include "shell.h"

/* begin namespace openfpga */
namespace openfpga {

/* A plugin of the build: its name and its version */
struct PluginInfo {
  std::string name;
  std::string version;
};

/* What a plugin is given to register itself */
class PluginApi {
 public:
  PluginApi(Shell<OpenfpgaContext>& shell, const PluginInfo& info)
    : shell_(shell), info_(info) {}
  const std::string& name() const { return info_.name; }
  const std::string& version() const { return info_.version; }
  /* The shell, whose registrar is this plugin while it registers */
  Shell<OpenfpgaContext>& shell() { return shell_; }

 private:
  Shell<OpenfpgaContext>& shell_;
  PluginInfo info_;
};

typedef void (*PluginRegisterFunction)(PluginApi&);

struct PluginEntry {
  PluginInfo info;
  PluginRegisterFunction register_function;
};

/* The plugins of the build, in order (written by CMake) */
const std::vector<PluginEntry>& openfpga_plugin_entries();

/* Their names and versions */
std::vector<PluginInfo> openfpga_plugins();

/* Let each plugin of the build register itself in a shell, in order. A
 * plugin that cannot (a name defined twice, a removal that is refused, an
 * exception) throws ShellRegistrationError with its name */
void register_openfpga_plugins(Shell<OpenfpgaContext>& shell);

/* One line per plugin, with what it added, overrode, removed and hooked;
 * empty when the build has no plugin */
std::string plugins_summary(const Shell<OpenfpgaContext>& shell);

/* The command report_plugins */
void add_plugin_commands(Shell<OpenfpgaContext>& shell);

} /* end namespace openfpga */

#endif
