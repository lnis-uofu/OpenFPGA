.. _openfpga_shell_plugins:

Plugins of the shell
--------------------

A plugin adds commands to the OpenFPGA shell. It may also take a command of the shell away, replace how one runs, or run code before or after a command (a hook). Plugins are C++ libraries chosen when OpenFPGA is built: a build has the same plugins in the ``openfpga`` executable, the Tcl module and the Python module.

Choosing the plugins
~~~~~~~~~~~~~~~~~~~~

The CMake option ``OPENFPGA_PLUGINS`` lists the plugins to build in, separated by ``;``, in the order they register. It is ``ui`` by default:

.. code-block:: shell

  make all CMAKE_FLAGS="-DOPENFPGA_PLUGINS='ui;example'"

A plugin is a folder of ``openfpga/plugins``, or of a folder given in ``OPENFPGA_PLUGIN_DIRS``. A name that is not found, or given twice, stops the configuration.

The plugins of the repository:

- ``ui``: the queries (see :ref:`openfpga_query_commands`) and the commands of the application options (:ref:`set_app_option <set_app_option>`, :ref:`reset_app_option <reset_app_option>`, :ref:`report_app_option <report_app_option>`). The Python module, the MCP server and the web app read the flow through them, so ``OPENFPGA_SWIG_PYTHON`` needs it.
- ``example``: a plugin that shows each thing a plugin can do, with a test. It is not for use: it takes ``write_sdc_disable_timing_configure_ports`` away and makes ``write_fabric_spice`` write nothing.

What a build has
~~~~~~~~~~~~~~~~

- At the start, the shell prints a line for each plugin, with what it added, replaced, took away and hooked.
- ``version`` prints ``Plugins: ui 1.0``, or ``Plugins: none``.
- ``help`` shows ``(plugin ui)`` after a command of a plugin, ``(overridden by plugin x)`` after a command that a plugin replaced, and lists apart the commands that plugins took away. Calling one of those fails and names the plugin.

report_plugins
^^^^^^^^^^^^^^

  Answer with the plugins of the build, in the order they registered: kind ``plugins``, not read from a stage (schema ``openfpga_flow/schemas/plugins.schema.json``). For each plugin: ``name``, ``version``, ``commands`` (added), ``overrides`` (commands of others it replaced), ``removes`` (commands it took away), ``hooks`` (``command`` and ``when``: ``before`` or ``after``) and ``every_command`` (a hook after every command). In Python: ``Session.plugins()``.

A checkpoint and a journal of a :ref:`Python Session <openfpga_python_session>` record the plugins of the build that made them. A build with other plugins refuses to restore the checkpoint, or to replay the journal, as their commands may not do the same.

Writing a plugin
~~~~~~~~~~~~~~~~

A plugin is a folder ``<name>`` (``[a-z][a-z0-9_]*``) with a ``CMakeLists.txt``:

.. code-block:: cmake

  openfpga_add_plugin(NAME myplugin
                      VERSION 1.0
                      SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/myplugin.cpp)

and a function that registers it, in the namespace ``openfpga``:

.. code-block:: cpp

  #include "openfpga_plugins.h"

  namespace openfpga {

  void register_plugin_myplugin(PluginApi& api);

  void register_plugin_myplugin(PluginApi& api) {
    Shell<OpenfpgaContext>& shell = api.shell();
    ShellCommandId id = shell.add_command(Command("my_command"), "What it does");
    shell.set_command_execute_function(id, my_command);
  }

  }

The plugins register after the commands of the shell and before the basic ones (``help``, ``exit``, ...). What a plugin does through ``api.shell()`` is recorded under its name:

- ``add_command()`` and the functions that set a command up, for its own commands. A name that is defined already stops the start.
- ``override_command(name)`` replaces how a command of the shell or of another plugin runs: the plugin then sets its execute function. The command keeps its name, options and dependencies; ``extend_command_options()`` may add options, not change those it has. A command is overridden by one plugin at most.
- ``deregister_command(name)`` takes a command away. It is refused for the basic commands and ``report_plugins``, and for a command that another one depends on.
- ``on_before(name, hook)`` runs before a command, once its options are read, and may refuse it by returning the reason; ``on_after(name, hook)`` runs after it with its status, also when it failed; ``on_after_any(hook)`` runs after every command.
- ``context.extension<T>()`` is a place in the data of the shell for the data of the plugin: one object of each type ``T``, made on first use.

A plugin that cannot register (a name defined twice, a removal that is refused) stops the start of the shell with the reason. ``openfpga/plugins/example`` does each of these, and its test (``test_example_plugin``) is built with it.

Plugins are built with the tree: the headers of the shell are not a stable interface for plugins built apart.
