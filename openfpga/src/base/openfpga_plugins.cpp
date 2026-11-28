/********************************************************************
 * The plugins of the shell: their registration, what each one changed, and
 * the command report_plugins (see openfpga_plugins.h)
 *******************************************************************/
#include "openfpga_plugins.h"

#include <algorithm>
#include <exception>
#include <utility>

#include "command_exit_codes.h"
#include "json_response.h"
#include "json_writer.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

namespace {

/* What a plugin did to a shell */
struct PluginChanges {
  std::vector<std::string> commands;  /* added, and not removed since */
  std::vector<std::string> overrides; /* commands of others it overrode */
  std::vector<std::string> removes;   /* commands it removed */
  /* hooks on a command: the command, and "before" or "after" */
  std::vector<std::pair<std::string, std::string>> hooks;
  bool every_command = false; /* a hook after every command */
};

PluginChanges changes_of(const Shell<OpenfpgaContext>& shell,
                         const std::string& plugin) {
  PluginChanges changes;
  for (const ShellCommandId& id : shell.commands()) {
    const std::string name = shell.command(id).name();
    if (plugin == shell.command_owner(id) && !shell.command_removed(id)) {
      changes.commands.push_back(name);
    }
    if (plugin == shell.command_overrider(id)) {
      changes.overrides.push_back(name);
    }
    if (plugin == shell.command_remover(id)) {
      changes.removes.push_back(name);
    }
    for (const std::string& owner : shell.command_before_hook_owners(id)) {
      if (plugin == owner) {
        changes.hooks.emplace_back(name, "before");
      }
    }
    for (const std::string& owner : shell.command_after_hook_owners(id)) {
      if (plugin == owner) {
        changes.hooks.emplace_back(name, "after");
      }
    }
  }
  const std::vector<std::string> every = shell.after_any_hook_owners();
  changes.every_command =
    every.end() != std::find(every.begin(), every.end(), plugin);
  return changes;
}

std::string joined(const std::vector<std::string>& words) {
  std::string text;
  for (const std::string& word : words) {
    text += (text.empty() ? "" : ", ") + word;
  }
  return text;
}

void write_names(JsonWriter& json, const std::string& key,
                 const std::vector<std::string>& names) {
  json.key(key).begin_array();
  for (const std::string& name : names) {
    json.value(name);
  }
  json.end_array();
}

/* The answer of report_plugins */
std::string plugins_json(const Shell<OpenfpgaContext>& shell) {
  JsonWriter json;
  begin_response(json, "plugins", 1);
  json.begin_object();
  json.key("plugins").begin_array();
  for (const PluginInfo& plugin : openfpga_plugins()) {
    const PluginChanges changes = changes_of(shell, plugin.name);
    json.begin_object();
    json.key("name").value(plugin.name);
    json.key("version").value(plugin.version);
    write_names(json, "commands", changes.commands);
    write_names(json, "overrides", changes.overrides);
    write_names(json, "removes", changes.removes);
    json.key("hooks").begin_array();
    for (const auto& hook : changes.hooks) {
      json.begin_object();
      json.key("command").value(hook.first);
      json.key("when").value(hook.second);
      json.end_object();
    }
    json.end_array();
    json.key("every_command").value(changes.every_command);
    json.end_object();
  }
  json.end_array();
  json.end_object();
  end_response(json);
  return json.str();
}

int report_plugins_command(Shell<OpenfpgaContext>* shell,
                           OpenfpgaContext& openfpga_ctx, const Command&,
                           const CommandContext&) {
  const std::string json = plugins_json(*shell);
  openfpga_ctx.set_query_result(json);
  VTR_LOG("%s\n", json.c_str());
  return CMD_EXEC_SUCCESS;
}

} /* end anonymous namespace */

std::vector<PluginInfo> openfpga_plugins() {
  std::vector<PluginInfo> plugins;
  for (const PluginEntry& entry : openfpga_plugin_entries()) {
    plugins.push_back(entry.info);
  }
  return plugins;
}

void register_openfpga_plugins(Shell<OpenfpgaContext>& shell) {
  for (const PluginEntry& entry : openfpga_plugin_entries()) {
    shell.set_registrar(entry.info.name);
    PluginApi api(shell, entry.info);
    try {
      entry.register_function(api);
    } catch (const ShellRegistrationError&) {
      shell.set_registrar("");
      throw;
    } catch (const std::exception& error) {
      shell.set_registrar("");
      throw ShellRegistrationError("plugin '" + entry.info.name +
                                   "' failed to register: " + error.what());
    }
  }
  shell.set_registrar("");
}

std::string plugins_summary(const Shell<OpenfpgaContext>& shell) {
  std::string summary;
  for (const PluginInfo& plugin : openfpga_plugins()) {
    const PluginChanges changes = changes_of(shell, plugin.name);
    std::string line = "Plugin " + plugin.name + " " + plugin.version;
    std::vector<std::string> parts;
    if (!changes.commands.empty()) {
      parts.push_back("adds " + joined(changes.commands));
    }
    if (!changes.overrides.empty()) {
      parts.push_back("overrides " + joined(changes.overrides));
    }
    if (!changes.removes.empty()) {
      parts.push_back("removes " + joined(changes.removes));
    }
    std::vector<std::string> hooked;
    for (const auto& hook : changes.hooks) {
      hooked.push_back(hook.second + " " + hook.first);
    }
    if (changes.every_command) {
      hooked.push_back("after every command");
    }
    if (!hooked.empty()) {
      parts.push_back("hooks " + joined(hooked));
    }
    for (size_t i = 0; i < parts.size(); ++i) {
      line += (0 == i ? ": " : "; ") + parts[i];
    }
    summary += line + "\n";
  }
  return summary;
}

void add_plugin_commands(Shell<OpenfpgaContext>& shell) {
  ShellCommandClassId cmd_class = shell.add_command_class("Plugins");
  Command cmd("report_plugins");
  ShellCommandId id = shell.add_command(
    cmd,
    "Report the plugins of the build and what each one added, overrode, "
    "removed and hooked, as JSON");
  shell.set_command_class(id, cmd_class);
  shell.set_command_execute_function(id, report_plugins_command);
  shell.protect_command(id);
}

} /* end namespace openfpga */
