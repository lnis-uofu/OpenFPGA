# Plugins of the OpenFPGA shell: plan

C++ plugins, chosen when OpenFPGA is built, add commands to the shell. A
plugin may also:
- take a default command away;
- replace how a default command runs;
- run code before or after a command (hooks).

The first plugin, `ui`, holds the commands that the Python module, the MCP
server and the app use to read the flow: the queries, the app options and
the constraints.

## Decisions (owner, 2026-11-24)

- Plugins are chosen at build time.
- A plugin may deregister or override a default command.
- Hooks before and after a command are part of the design.
- Only the query, option and constraint commands move to the `ui` plugin.
  The rest stays in the core:
  - the stage commands (`read_vpr_arch`, `read_circuit`, `pack`, `place`,
    `route`, `analysis`);
  - the session and its stages;
  - `read_device_package` and `read_design` (M15), which are inputs of the
    flow like `read_vpr_arch`.

## What is there today

- **Registration:**
  - `OpenfpgaShell()` (openfpga/src/base/openfpga_shell.cpp) calls fixed
    group functions: `add_vpr_commands`, the Yosys, setup, Verilog,
    bitstream, SPICE and SDC groups, `add_app_options_commands`,
    `add_stage_commands`, `add_query_commands`.
  - `add_basic_commands` comes last, because `help` must be registered
    after every other command.
- **`Shell<T>`** (libs/libopenfpgashell/src/base/shell.h, shell.tpp):
  - commands are dense ids in `vtr::vector`s;
  - dependencies and alternatives point at those ids;
  - there is no way to remove a command or replace its function;
  - `add_command` with a name that exists returns `INVALID` and says
    nothing.
- **Shared state:** every command shares one `OpenfpgaContext`. It has the
  session and the slot of the JSON answer of a query (`query_result`), which
  `execute()` hands to Python as `CommandResult.data`.
- **The build:** `openfpga/CMakeLists.txt` globs `src/*/*.cpp` into
  libopenfpga, so any file under `openfpga/src` is always built. Plugins must
  live outside it.
- **What moves:**

| Group | Commands | Files |
|---|---|---|
| Queries | `report_session`, `report_stage`, `report_run`, `timing_summary`, `critical_paths`, `device`, `utilization`, `placement`, `net_route`, `congestion`, `report_cluster`, `find`, `block_info`, `net_info` | `query_commands`, `query_design`, `query_run`, `query_stage`, `query_timing`, `session_json` |
| Options | `set_app_option`, `reset_app_option`, `report_app_option` | `app_options_commands`, `app_options_json` |
| Constraints | `read_odc`, `write_odc`, `clear_odc`, `report_constraints`, `write_io_map` (M15, not written yet) | `libodc` stays a library; the commands go to the plugin |

- **What stays in the core**, because the core needs it:
  - the registry of the options (`app_options`, `app_option_table`,
    `vpr_option_table`, `vpr_app_options`, `app_option_stage`), since the
    session drops stages when an option changes and the stage commands give
    the options to VPR;
  - the JSON writer (`utils/json_writer`, `json_response`);
  - the `query_result` slot of the context, a general "structured answer of
    a command".

## Design

### Build

- **Choosing plugins:** `cmake -DOPENFPGA_PLUGINS="ui;other"`.
  - Each plugin is a directory with a `CMakeLists.txt` that makes a static
    library, and one function `void register_plugin_<name>(PluginApi& api)`.
  - The plugins of the repository are in `openfpga/plugins/<name>/`. A
    plugin from elsewhere is a directory given by
    `-DOPENFPGA_PLUGIN_DIRS=...`.
- **Registration:** CMake writes `openfpga_plugins.cpp`, which calls the
  register functions in the order of the list. No registration happens
  through static constructors: the linker drops unreferenced objects of a
  static library, and the order of static initialisation is not defined.
- **When:** `OpenfpgaShell()` runs the default groups, then the plugins in
  order, then `add_basic_commands`, so that `help` still comes last.
- **Defaults:**
  - `ui` is on by default;
  - `OPENFPGA_SWIG_PYTHON=ON` requires it, and CMake stops with the reason if
    it is off, because `Session` and the MCP server read the flow through its
    commands;
  - the Tcl module and the `openfpga` binary are built with the same list.
- **No `dlopen`:** the headers of the shell are not a stable interface, and
  a plugin built with the tree has no ABI problem.

### The API of a plugin (`PluginApi`)

A plugin sees the shell and the context only through this API, with its own
name attached to everything it does.

- **`add_command_class(name)`, `add_command(cmd, descr)`**, and the setters
  of the execute function and the dependencies, as `Shell<T>` has them. A
  name that exists is an error that stops the start, naming both owners.
- **`override_command(name, fn)`**:
  - replaces how a command runs, and keeps its id, name, class and options,
    so the commands that depend on it, and the scripts, still work;
  - `add_option` on an overridden command may add options, but not take any
    away or rename them;
  - two plugins that override the same command are an error.
- **`deregister_command(name)`**:
  - the command is marked removed, and keeps its id so the other ids stay
    stable;
  - calling it fails with "removed by plugin X";
  - it is an error when a command that stays depends on it, or names it as
    an alternative;
  - `help`, `exit` and the other basic commands cannot be removed.
- **Hooks: `on_before(name, fn)` and `on_after(name, fn)`**:
  - `on_before` gets the command and its parsed options, and may refuse the
    command with a message (the command does not run, and its status is an
    error);
  - `on_after` gets the exit code, and runs even when the command failed;
  - hooks run in the order of the plugins;
  - a hook that throws makes the command fail with the plugin's name;
  - `on_after_any(fn)` is for observers of every command (a journal, for
    example).
- **`context.extension<T>()`**: a slot of the context where a plugin keeps
  its own data, by type. Data that is part of the flow must say how it is
  cleared on a reset and how a checkpoint saves it (the session's reset and
  checkpoints call these).

### What is active, and where it is seen

- **At start, and in `version`:** the plugins (name and version) and every
  command that a plugin overrode, removed or hooked.
- **In `help`:** each command shows the plugin it comes from, and the commands
  that were removed are listed apart.
- **`report_plugins --json`** (core, with a schema), which the Python module
  reads as `Session.plugins()`.
- **Journals and checkpoints** record the plugins. A checkpoint or a journal
  replayed with a different set fails with the difference, so a command does
  not mean one thing in one build and another thing in the next.

### Tests

- **libopenfpgashell:** a duplicate name, an override, a removal blocked by
  a dependency, hooks in order, a hook that refuses, a hook that throws.
- **A test plugin** (`openfpga/plugins/example`), built only in CI:
  - it adds a command, overrides one, removes one and hooks one;
  - a shell test checks each, and what `help`, `version` and
    `report_plugins` say.
- **The `ui` plugin:** the whole Python suite, the MCP tests and the app's
  tests pass unchanged with it.
- **CI:**
  - a build without plugins runs the regression tests of the flow, which
    need none of the moved commands;
  - a build with `ui` runs the Python tests.

## Phases

### Phase 1: the shell (libopenfpgashell)

1. Duplicate names become an error.
2. Override, deregister (marked removed), hooks and `on_after_any`.
3. Where each command comes from, for `help`.
4. Unit tests.

None of this changes what an existing script does.

Status: done (`Shell<T>` in shell.h and shell.tpp; test_shell_plugins.cpp).

- **Registrar:** `set_registrar(name)` says who registers from now on: empty
  for the shell's own commands, or a plugin. `command_owner`,
  `command_overrider`, `command_remover` and the owners of the hooks are kept
  for each command.
- **Errors:** a name defined twice throws `ShellRegistrationError`, naming
  both registrars, as do the other refusals below. Before, `add_command`
  returned INVALID and said nothing.
- **`override_command(name)`:**
  - for a plugin only; never its own command, and once per command;
  - the plugin then sets the new execute function with
    `set_command_execute_function`.
- **Changes to a command:** a registrar may change a command only if it owns
  it or overrode it. This covers the execute function, the dependencies, an
  alternative and the options; otherwise the change throws.
- **`extend_command_options(id, fn)`:** adds options to a command. Changing
  an existing option (its short name, whether it is required, whether it
  takes a value and of which type) throws, and so does renaming the command.
- **`deregister_command(name)`:**
  - for a plugin only; the command is marked removed and keeps its id;
  - it is refused when the command is protected (`protect_command`: the
    basic commands `exit`, `version`, `hidden_version`, `source`,
    `ext_exec`, `help`), or when a command that stays depends on it or has
    it as an alternative;
  - calling a removed command fails with the plugin's name, and its name
    cannot be added again.
- **Hooks:**
  - `on_before` returns "" to let the command run, or a reason to refuse it;
  - `on_after` and `on_after_any` get the status;
  - hooks run only once the options are parsed, including for macro
    commands;
  - a refusal or a hook that throws fails the command, without an exception
    to the caller;
  - a command that throws is seen by the hooks after it, and the exception
    goes on.
- **`help`:** shows `(plugin x)` and `(overridden by plugin x)`, and lists
  removed commands apart. The output of the shell's own commands is
  unchanged.
- **Found on the way:** two C++ tests were stale since earlier commits:
  - test_json_writer did not expect `vpr_flag` after `stage`;
  - test_vpr_app_options did not list `place.placer` as an option of
    OpenFPGA.

  Both are fixed. The C++ tests (ctest) were not run when those commits were
  made.

### Phase 2: plugins in the build

1. `OPENFPGA_PLUGINS`, `OPENFPGA_PLUGIN_DIRS`, the generated registration
   file, and `PluginApi`.
2. `context.extension<T>()`.
3. `report_plugins --json`, the start line, `version`, and the plugins in
   journals and checkpoints.
4. The example plugin and its tests. CI builds with and without plugins.

Status: done, except CI (not committed).

- **Build:**
  - `OPENFPGA_PLUGINS` and `OPENFPGA_PLUGIN_DIRS` are cache variables (top
    CMakeLists.txt).
  - `openfpga_add_plugin(NAME VERSION SOURCES)` (openfpga/CMakeLists.txt)
    makes the static library `openfpga_plugin_<name>`, linked both ways with
    libopenfpga, with `-Werror` under strict compile.
  - A plugin is the folder `<name>/` in openfpga/plugins or in a folder of
    `OPENFPGA_PLUGIN_DIRS`. A bad name, a name given twice, a missing folder,
    or a folder that does not call the function stops the configuration.
  - CMake writes `openfpga_plugin_list.cpp` (from
    openfpga/plugins/openfpga_plugin_list.cpp.in), with each plugin's name,
    version and `register_plugin_<name>`, in order. With no plugins, it is
    an empty list.
- **Core** (openfpga_plugins.h/.cpp):
  - `PluginApi` gives a plugin its name, its version and the shell, with the
    plugin as the registrar.
  - `register_openfpga_plugins` runs the plugins in order, after the
    shell's own commands and before the basic ones. Any exception becomes a
    `ShellRegistrationError` with the plugin's name, and `main` prints it and
    exits with 1.
  - `report_plugins` (class Plugins, protected) answers the kind `plugins`
    (schema `plugins.schema.json`): each plugin with the commands it added,
    overrode and removed, its hooks, and `every_command`.
  - The title shown at start has a line per plugin, and `version` says
    `Plugins: none` or `Plugins: example 1.0`.
- **`OpenfpgaContext::extension<E>()`** and `find_extension<E>()`: one
  object per type, made on first use. Not done: clearing it on a reset and
  saving it in a checkpoint. The context has no reset as a whole, and
  checkpoints are written by Python. A plugin with data of the flow handles
  both itself, and the `ui` plugin has none.
- **Python:**
  - `Session.plugins()` returns `Plugin` and `PluginHook`.
  - `checkpoint.json` has `plugins`, and `restore` refuses a different set
    with `CheckpointError`. A checkpoint without the key counts as made with
    none.
  - The journal's scripts have `# Plugins of the build: ...`, and `replay`
    refuses a different set with `ReplayError` on that line. A script
    without the line is replayed as before.
  - `shell_commands()` reads the name before the plugin note of `help`, and
    stops at the removed commands.
- **Example plugin** (openfpga/plugins/example):
  - it adds `example_report`, overrides `write_fabric_spice`, removes
    `write_sdc_disable_timing_configure_ports`, hooks `read_vpr_arch`
    (after) and `read_circuit` (before, refusing a name with
    `refused_by_example`) and every command, and keeps its data in
    `extension()`;
  - `test_example_plugin` (ctest) is built only with the plugin.
- **Tests:**
  - test_plugins.py: 3 tests in any build, plus 3 with the example plugin;
  - built without plugins: 905 Python tests pass (the 3 of the example
    plugin are skipped), and the 12 ctest tests;
  - built with `OPENFPGA_PLUGINS=example`: the full Python suite (908) and
    `test_example_plugin` pass.
- **CI, not done:**
  - CI runs no ctest today, and the `openfpga` artifact of the Python job is
    the one the regression jobs use, so the example plugin cannot go into
    that build.
  - A build with plugins needs a job of its own, a full build that runs the
    ctest tests. This is left to the owner.

### Phase 3: the `ui` plugin

1. Move the query and option commands and their files into
   `openfpga/plugins/ui/`. The registry of the options and the JSON writer
   stay in the core.
2. The Python, MCP and app suites pass unchanged. `Session` checks that the
   plugin is there and says so if it is not.
3. Docs: the shell's command reference (each command's plugin), a page on
   writing a plugin, python_session.rst.

Status: done (not committed).

- **The move:** the query and option commands and their files moved, with
  their history, from openfpga/src/base to openfpga/plugins/ui:
  - query_commands, query_design, query_run, query_stage and query_timing;
  - session_json, app_options_commands and app_options_json.

  `register_plugin_ui` adds the option commands, then the queries.
  openfpga_shell.cpp no longer adds them. The registry of the options, the
  JSON writer and the session stay in the core.
- **Order:** the ui commands now come after `report_plugins` in `help`. No
  command of the core refers to them by id.
- **Build:**
  - `OPENFPGA_PLUGINS` is `ui` by default, and pyproject.toml sets it too;
  - `OPENFPGA_SWIG_PYTHON` without `ui` stops the configuration with the
    reason;
  - the plugin's tests are in plugins/ui/test: the part of
    test_json_writer about `app_options_json` moved there, as
    test_app_options_json.
- **Python:** `Session()` raises `RuntimeError` when the build has no ui
  plugin.
- **Checkpoints:** a checkpoint without `plugins` is now restored by any
  build. It was made before the plugins were recorded, when the ui commands
  were the shell's own, so saved sessions of the app keep working.
- **Found on the way, fixed:**
  - `run_report_json` had an unused parameter `run` since the report_run
    commit. Its name is left out now (`-Wunused-parameter`, an error under
    strict compile).
  - query_design.cpp had two lines that were not clang-format clean, from
    the flat routing commit.
- **Docs:**
  - manual/openfpga_shell/plugins.rst: the options, the ui and example
    plugins, `report_plugins`, and writing a plugin;
  - notes in query_commands.rst and basic_commands.rst;
  - python_session.rst: the ui plugin, `plugins()`, and the plugins in
    journals and checkpoints.
- **Tests:**
  - build with ui: 906 Python tests pass, and 3 skipped (example); 13
    ctest tests; the browser test `e2e:session`;
  - build with ui and example: test_example_plugin (its version check now
    allows other plugins), test_app_options_json, test_plugins.py,
    test_session.py and test_checkpoint.py;
  - not tried: `pip install .` with the new define.

### Phase 4: constraints in the plugin

The commands of ODC (M15: `read_odc`, `write_odc`, `clear_odc`,
`report_constraints`, `write_io_map`) are written in the `ui` plugin from
the start. If M15 starts before phase 3, they go next to the query commands
and move with them.

### Later

- Moving the hold of the session in the default commands onto hooks:
  - the session updates of `read_openfpga_arch`;
  - the stage checks (`stage_guard`) of the setup and bitstream commands;
  - the logs of the stages in `openfpga_shell.cpp`.

  The core commands would then not know the session. This is only worth it
  if the maintainers ask for it.
- A stable plugin interface for plugins outside the repository: not
  promised. A plugin from elsewhere is built with the tree, and breaks when
  the headers change.

## Upstream

The query and option commands are in open PRs of the stack (#2840, #2859,
and later ones). Moving them into a plugin changes those PRs. Ask the
maintainers on issue #2860 first:
- whether they prefer the shell's core to stay as it is, with an optional
  plugin that is off by default;
- or the commands in the core, as the PRs have them.

The answer decides whether phase 3 rewrites the stack or comes as a PR of
its own after it.
