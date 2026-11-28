.. _openfpga_shell_plugins:

Plugins of the shell
--------------------

A plugin adds commands to the OpenFPGA shell. It may also take a command of the shell away, replace how one runs, or run code before or after a command (a hook). Plugins are C++ libraries chosen when OpenFPGA is built: a build has the same plugins in the ``openfpga`` executable, the Tcl module and the Python module.

Choosing the plugins
~~~~~~~~~~~~~~~~~~~~

The CMake option ``OPENFPGA_PLUGINS`` lists the plugins to build in, separated by ``;``, in the order they register. It is empty by default: a build has no plugin unless it is asked for.

.. code-block:: shell

  make all CMAKE_FLAGS="-DOPENFPGA_PLUGINS=example"
  make all CMAKE_FLAGS="-DOPENFPGA_PLUGIN_DIRS=/path/to/plugins -DOPENFPGA_PLUGINS='example;myplugin'"

A plugin is a folder of ``openfpga/plugins``, or of a folder given in ``OPENFPGA_PLUGIN_DIRS``. A name that is not found, or given twice, stops the configuration.

The repository has one plugin, ``example``: it shows each thing a plugin can do, with a test. It is not for use: it takes ``write_sdc_disable_timing_configure_ports`` away and makes ``write_fabric_spice`` write nothing.

What a build has
~~~~~~~~~~~~~~~~

- At the start, the shell prints a line for each plugin, with what it added, replaced, took away and hooked.
- ``version`` prints ``Plugins: example 1.0``, or ``Plugins: none``.
- ``help`` shows ``(plugin example)`` after a command of a plugin, ``(overridden by plugin x)`` after a command that a plugin replaced, and lists apart the commands that plugins took away. Calling one of those fails and names the plugin.

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

- ``api.register_app_option(name, default, stage)`` adds an application option (see :ref:`set_app_option <set_app_option>`), whose name starts with ``<plugin name>.``, e.g., ``myplugin.level``; changing it drops ``stage`` and the stages after it, and ``report_app_option --json`` gives it the ``owner`` ``myplugin``. A name that is taken, or that does not start with the name of the plugin, stops the start.

What a plugin can use of the core, besides the shell:

- ``query_answer.h``, for commands that answer in JSON like the reports of the shell (:ref:`openfpga_session_report_commands`): ``answer_query(ctx, stage, command, query)`` checks the stage, runs ``query(run, json)`` on the data of VPR and gives the answer to the caller; ``stage_data(ctx, stage)``, ``count_option()``, ``string_option()``, ``add_value_option()``, ``add_query_command()`` (a read-only command) and ``QUERY_LIST_DEFAULT_LIMIT``. The answers use ``json_writer.h`` and ``json_response.h``.
- The attachments of the session (``OpenFpgaSession::set_attachment(key, json)``, ``attachment(key)``, ``clear_attachment(key)``, ``attachment_keys()``): data of the plugin, as JSON, kept with the architectures under a key that starts with its name. Reading an architecture, or failing to, clears them; ``report_session`` lists them (``attachments``), and the checkpoints of a Python ``Session`` keep them. The command ``set_attachment`` sets one from a script.
- ``OpenFpgaSession::set_fixed_channel_width_owner(owner)`` makes the channel width that was just set read only until an architecture is read: ``set_app_option`` and ``reset_app_option`` refuse another value of ``router.route_chan_width``, with a message that names the owner (``fixed_option(name)``). ``set_cache_options(names)`` names the options that were set to read cached files of the device, e.g., ``filename.read_rr_graph_file``: they go back to their default when an architecture is read (``reset_cache_options()`` does it at once).
- ``design_synthesis.h``: ``set_synthesis_target_provider(provider)`` gives the synthesis target of a session (``SynthesisTarget``: ``name``, ``lut_size``, ``yosys_script``, ``values`` of its placeholders, ``cell_libraries``, ``vpr_options``) to ``synthesize``, ``read_design`` and ``report_design_hierarchy``; the provider returns false for none, and a ``std::exception`` that it throws makes the command refuse with its message. ``set_design_constraints_command(name)`` names the command that ``read_design`` reads the constraints file of a design with.
- ``OpenFpgaSession::set_constraints(SessionConstraints)``: what constraints that a plugin read give to the core: the Yosys commands of ``${CONSTRAINT_SYNTHESIS}``, the top and the defines of the synthesis, and a pin constraints file for the commands that take ``--pin_constraints_file`` and are not given one. ``flow_input_file_changed(session)`` drops the device when a file that VPR reads with it (an SDC, VPR constraints) changed, and ``drop_input()`` drops an input, e.g., the circuit when the synthesis constraints change.
- ``OpenFpgaSession::set_bitstream_recipe(BitstreamRecipe)``: how ``build_bitstream`` makes the bitstream of the architectures that the plugin read (``supported``, ``owner``, ``link_options``, ``fixups``, ``fabric_options``). Reading an architecture gives the defaults back.
- ``make_flow_device(session)`` (``staged_flow.h``) makes the device at once, e.g., after a plugin read the architectures in its own way; ``OpenFpgaSession::stage_generation(stage)`` changes each time a stage completes, to know when what was made from it is out of date.

A plugin that cannot register (a name defined twice, a removal that is refused) stops the start of the shell with the reason. ``openfpga/plugins/example`` does each of these, and its test (``test_example_plugin``) is built with it.

Plugins are built with the tree: the headers of the shell are not a stable interface for plugins built apart.
