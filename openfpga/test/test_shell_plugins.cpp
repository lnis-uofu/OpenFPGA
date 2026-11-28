/********************************************************************
 * Unit test of what the shell offers to plugins: the registrar of each
 * command, a name defined twice, overriding a command, adding options to it,
 * removing a command, and the hooks before and after a command
 *******************************************************************/
#include <cstdio>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

#include "command_exit_codes.h"
#include "shell.h"

using openfpga::Command;
using openfpga::CommandContext;
using openfpga::CommandOptionId;
using openfpga::Shell;
using openfpga::ShellCommandId;
using openfpga::ShellRegistrationError;

static int num_failures = 0;

#define CHECK(cond)                                                         \
  do {                                                                      \
    if (!(cond)) {                                                          \
      std::fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, __LINE__, \
                   #cond);                                                  \
      ++num_failures;                                                       \
    }                                                                       \
  } while (false)

/* The message of the ShellRegistrationError that f throws, or "" */
static std::string refusal(const std::function<void()>& f) {
  try {
    f();
  } catch (const ShellRegistrationError& error) {
    return error.what();
  }
  return "";
}

static bool has(const std::string& text, const std::string& part) {
  return std::string::npos != text.find(part);
}

/* The data the commands share: what ran, in order */
struct Context {
  std::vector<std::string> log;
  int value = 0;
};

using TestShell = Shell<Context>;

/* A command that records its name, or the value of its option --value */
static ShellCommandId add(TestShell& shell, const std::string& name,
                          bool with_value = false) {
  Command cmd(name.c_str());
  if (with_value) {
    CommandOptionId opt = cmd.add_option("value", false, "a number");
    cmd.set_option_require_value(opt, openfpga::OPT_INT);
  }
  ShellCommandId id = shell.add_command(cmd, "a command of the test");
  shell.set_command_execute_function(
    id, [name](Context& context, const Command& command,
               const CommandContext& parsed) {
      context.log.push_back(name);
      CommandOptionId opt = command.option("value");
      if (CommandOptionId::INVALID() != opt &&
          parsed.option_enable(command, opt)) {
        context.value = std::stoi(parsed.option_value(command, opt));
      }
      return openfpga::CMD_EXEC_SUCCESS;
    });
  return id;
}

static void test_registrar_and_duplicates() {
  TestShell shell;
  shell.set_name("Test");
  ShellCommandId core = add(shell, "read");
  CHECK(shell.command_owner(core).empty());
  shell.set_registrar("ui");
  ShellCommandId own = add(shell, "query");
  CHECK("ui" == shell.command_owner(own));
  CHECK(shell.registrar() == "ui");

  /* A name that is there: the error names who defined it */
  std::string why = refusal([&] { add(shell, "read"); });
  CHECK(has(why, "plugin 'ui'") && has(why, "'read'") &&
        has(why, "defined by Test"));
  shell.set_registrar("other");
  why = refusal([&] { add(shell, "query"); });
  CHECK(has(why, "defined by plugin 'ui'"));
  shell.set_registrar("");
  why = refusal([&] { add(shell, "query"); });
  CHECK(has(why, "Test cannot add") && has(why, "plugin 'ui'"));
}

static void test_override() {
  TestShell shell;
  ShellCommandId read = add(shell, "read", true);
  ShellCommandId route = add(shell, "route");
  shell.set_command_dependency(route, {read});

  /* The shell itself does not override, and a plugin may not change a
   * command it did not override */
  CHECK(has(refusal([&] { shell.override_command("read"); }), "Only a plugin"));
  shell.set_registrar("fast");
  CHECK(has(refusal([&] {
              shell.set_command_execute_function(
                read, [](Context&) { return openfpga::CMD_EXEC_SUCCESS; });
            }),
            "without overriding it"));
  CHECK(
    has(refusal([&] { shell.override_command("nothing"); }), "not defined"));

  ShellCommandId overridden = shell.override_command("read");
  CHECK(overridden == read);
  CHECK("fast" == shell.command_overrider(read));
  CHECK(shell.command_owner(read).empty());
  shell.set_command_execute_function(
    read,
    [](Context& context, const Command& command, const CommandContext& parsed) {
      context.log.push_back("fast read");
      CommandOptionId opt = command.option("value");
      if (parsed.option_enable(command, opt)) {
        context.value = 10 * std::stoi(parsed.option_value(command, opt));
      }
      return openfpga::CMD_EXEC_SUCCESS;
    });

  /* Its options and its dependents stay */
  Context context;
  CHECK(openfpga::CMD_EXEC_FATAL_ERROR ==
        shell.execute_command("route", context));
  CHECK(openfpga::CMD_EXEC_SUCCESS ==
        shell.execute_command("read --value 3", context));
  CHECK(30 == context.value);
  CHECK(openfpga::CMD_EXEC_SUCCESS == shell.execute_command("route", context));
  CHECK((std::vector<std::string>{"fast read", "route"}) == context.log);

  /* Once only, and never one's own */
  shell.set_registrar("slow");
  CHECK(has(refusal([&] { shell.override_command("read"); }),
            "overridden by plugin 'fast'"));
  add(shell, "mine");
  CHECK(has(refusal([&] { shell.override_command("mine"); }), "its own"));
}

static void test_options_are_only_added() {
  TestShell shell;
  ShellCommandId read = add(shell, "read", true);
  shell.set_registrar("ui");
  CHECK(
    has(refusal([&] { shell.extend_command_options(read, [](Command&) {}); }),
        "without overriding it"));
  shell.override_command("read");
  shell.extend_command_options(
    read, [](Command& cmd) { cmd.add_option("verbose", false, "say more"); });
  Context context;
  CHECK(openfpga::CMD_EXEC_SUCCESS ==
        shell.execute_command("read --value 2 --verbose", context));
  CHECK(2 == context.value);

  /* An option that changes, or a new name, is refused, and the command is
   * left as it was */
  std::string why = refusal([&] {
    shell.extend_command_options(read, [](Command& cmd) {
      cmd.set_option_require_value(cmd.option("value"), openfpga::OPT_STRING);
    });
  });
  CHECK(has(why, "'value'") && has(why, "can only be added"));
  CHECK(openfpga::OPT_INT == shell.command(read).option_require_value_type(
                               shell.command(read).option("value")));
  CHECK(has(refusal([&] {
              shell.extend_command_options(
                read, [](Command& cmd) { cmd = Command("renamed"); });
            }),
            "cannot be renamed"));
}

static void test_deregister() {
  TestShell shell;
  shell.set_name("Test");
  ShellCommandId read = add(shell, "read");
  ShellCommandId route = add(shell, "route");
  ShellCommandId vpr = add(shell, "vpr");
  ShellCommandId pack = add(shell, "pack");
  ShellCommandId help = add(shell, "help");
  shell.set_command_dependency(route, {read});
  shell.add_command_alternative(vpr, pack);
  shell.protect_command(help);

  CHECK(
    has(refusal([&] { shell.deregister_command("read"); }), "Only a plugin"));
  shell.set_registrar("lean");
  /* Not while a command that stays needs it */
  CHECK(has(refusal([&] { shell.deregister_command("read"); }),
            "'route' depends on it"));
  CHECK(has(refusal([&] { shell.deregister_command("pack"); }),
            "'vpr' has it as an alternative"));
  CHECK(
    has(refusal([&] { shell.deregister_command("help"); }), "the shell needs"));

  /* Removing the dependent first lets the other go */
  shell.deregister_command("route");
  shell.deregister_command("read");
  CHECK(shell.command_removed(read) && "lean" == shell.command_remover(read));
  CHECK(!shell.command_removed(pack));

  Context context;
  CHECK(openfpga::CMD_EXEC_FATAL_ERROR ==
        shell.execute_command("read", context));
  CHECK(context.log.empty());
  CHECK(has(refusal([&] { shell.deregister_command("read"); }),
            "removed by plugin 'lean'"));
  /* Its name is not free again, and nothing may lean on it */
  CHECK(has(refusal([&] { add(shell, "read"); }),
            "a command removed by plugin 'lean'"));
  ShellCommandId extra = add(shell, "extra");
  CHECK(has(refusal([&] { shell.set_command_dependency(extra, {read}); }),
            "removed by plugin 'lean'"));
  CHECK(has(refusal([&] { shell.override_command("read"); }),
            "removed by plugin 'lean'"));
  CHECK(has(refusal([&] {
              shell.on_before(
                "read", [](Context&, const Command&, const CommandContext&) {
                  return std::string();
                });
            }),
            "removed by plugin 'lean'"));
}

static void test_hooks() {
  TestShell shell;
  shell.set_name("Test");
  add(shell, "read");
  ShellCommandId failing = shell.add_command(Command("fail"), "fails");
  shell.set_command_execute_function(failing, [](Context& context) {
    context.log.push_back("fail");
    return openfpga::CMD_EXEC_FATAL_ERROR;
  });
  ShellCommandId throwing = shell.add_command(Command("throw"), "throws");
  shell.set_command_execute_function(
    throwing, [](Context&) -> int { throw std::runtime_error("boom"); });
  ShellCommandId macro = shell.add_command(Command("macro"), "a macro");
  shell.set_command_execute_function(macro, [](int argc, char** argv) {
    return (2 == argc && std::string("x") == argv[1])
             ? openfpga::CMD_EXEC_SUCCESS
             : openfpga::CMD_EXEC_FATAL_ERROR;
  });

  bool refuse = false;
  shell.set_registrar("a");
  shell.on_before("read", [&](Context& context, const Command& command,
                              const CommandContext&) {
    context.log.push_back("a before " + command.name());
    return refuse ? std::string("not now") : std::string();
  });
  shell.on_after("read", [](Context& context, const Command&,
                            const CommandContext&, int status) {
    context.log.push_back("a after " + std::to_string(status));
  });
  shell.set_registrar("b");
  shell.on_before("read",
                  [](Context& context, const Command&, const CommandContext&) {
                    context.log.push_back("b before");
                    return std::string();
                  });
  shell.on_after_any([](Context& context, const Command& command,
                        const CommandContext&, int status) {
    context.log.push_back("any " + command.name() + " " +
                          std::to_string(status));
  });
  CHECK(has(refusal([&] {
              shell.on_after("nothing", [](Context&, const Command&,
                                           const CommandContext&, int) {});
            }),
            "not defined"));

  ShellCommandId read = shell.command("read");
  CHECK((std::vector<std::string>{"a", "b"}) ==
        shell.command_before_hook_owners(read));
  CHECK((std::vector<std::string>{"a"}) ==
        shell.command_after_hook_owners(read));
  CHECK((std::vector<std::string>{"b"}) == shell.after_any_hook_owners());

  const std::string ok = std::to_string(openfpga::CMD_EXEC_SUCCESS);
  const std::string fatal = std::to_string(openfpga::CMD_EXEC_FATAL_ERROR);

  /* In order: the hooks before, the command, the hooks after */
  Context context;
  CHECK(openfpga::CMD_EXEC_SUCCESS == shell.execute_command("read", context));
  CHECK((std::vector<std::string>{"a before read", "b before", "read",
                                  "a after " + ok, "any read " + ok}) ==
        context.log);

  /* A refusal: the command does not run, the later hooks before it neither,
   * and the hooks after it see the failure */
  refuse = true;
  context.log.clear();
  CHECK(openfpga::CMD_EXEC_FATAL_ERROR ==
        shell.execute_command("read", context));
  CHECK((std::vector<std::string>{"a before read", "a after " + fatal,
                                  "any read " + fatal}) == context.log);
  refuse = false;

  /* A failed command and a command that throws are seen by the hooks; the
   * exception goes on to the caller */
  context.log.clear();
  CHECK(openfpga::CMD_EXEC_FATAL_ERROR ==
        shell.execute_command("fail", context));
  CHECK((std::vector<std::string>{"fail", "any fail " + fatal}) == context.log);
  context.log.clear();
  bool thrown = false;
  try {
    shell.execute_command("throw", context);
  } catch (const std::runtime_error&) {
    thrown = true;
  }
  CHECK(thrown);
  CHECK((std::vector<std::string>{"any throw " + fatal}) == context.log);

  /* A macro command has its hooks too */
  context.log.clear();
  CHECK(openfpga::CMD_EXEC_SUCCESS ==
        shell.execute_command("macro x", context));
  CHECK((std::vector<std::string>{"any macro " + ok}) == context.log);

  /* A hook that throws: the command fails, with no exception to the caller */
  shell.set_registrar("c");
  shell.on_before(
    "fail", [](Context&, const Command&, const CommandContext&) -> std::string {
      throw std::runtime_error("broken hook");
    });
  context.log.clear();
  CHECK(openfpga::CMD_EXEC_FATAL_ERROR ==
        shell.execute_command("fail", context));
  CHECK((std::vector<std::string>{"any fail " + fatal}) == context.log);
  shell.on_after("macro", [](Context&, const Command&, const CommandContext&,
                             int) { throw std::runtime_error("late"); });
  CHECK(openfpga::CMD_EXEC_FATAL_ERROR ==
        shell.execute_command("macro x", context));
}

int main() {
  test_registrar_and_duplicates();
  test_override();
  test_options_are_only_added();
  test_deregister();
  test_hooks();
  if (0 == num_failures) {
    std::printf("test_shell_plugins: all checks passed\n");
  }
  return 0 == num_failures ? 0 : 1;
}
