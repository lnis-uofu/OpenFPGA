.. _openfpga_python_session:

Python Session
--------------

The module ``openfpga`` (see the build option ``OPENFPGA_SWIG_PYTHON`` in :ref:`tutorial_compile`) has the class ``Session`` to run the flow from Python. It wraps an ``OpenfpgaShell``, which stays available as ``session.shell`` for the commands that have no method.

.. code-block:: python

  import openfpga

  session = openfpga.Session()
  session.options["router.route_chan_width"] = 100
  session.read_vpr_arch("k6_frac_N10_tileable_40nm.xml", layout="2x2")
  session.read_openfpga_arch("k6_frac_N10_40nm_openfpga.xml")
  session.read_circuit("and2.blif")
  session.run_flow()

The files of the flow are written in the current directory of the process.

Installation
~~~~~~~~~~~~

The module is built from the source of OpenFPGA (a checkout with its submodules) and installed with ``pip``, in a virtual environment for instance:

.. code-block:: shell

  python3 -m venv ~/openfpga-env
  ~/openfpga-env/bin/pip install .
  ~/openfpga-env/bin/python3 openfpga_flow/openfpga_python_scripts/session_quickstart.py

The build uses ``scikit-build-core``, which ``pip`` fetches, and builds only the Python module (not the ``openfpga`` executable, Yosys or the tests). It needs a C++17 compiler, CMake, SWIG, the Python development headers and the Tcl development headers (``tcl-dev`` or ``tcl-devel``, which VPR's SDC parser uses). CMake takes the compiler that is first in the ``PATH``: set ``CC`` and ``CXX`` for another one, e.g., ``CC=gcc-12 CXX=g++-12 pip install .``. The result is ``import openfpga``. The build is kept in ``build/python``, so that the next ``pip install .`` only builds what changed.

``session_quickstart.py`` runs a small design through the four stages in a worker process, and prints the stages reached, the time of each, the critical path delay, the wirelength and the blocks of each type (from ``run_report()``), and the files of the results. Its ``--in-process`` option runs a ``Session`` instead, and ``--help`` lists the files that it takes.

Commands
~~~~~~~~

A step returns a ``StepResult`` with the ``command`` that was sent, its ``log`` and the time it took (``elapsed_s``). A step that does not succeed **raises** ``OpenfpgaError``, with the status, the log and the message of the failure, and the session stays usable.

- Inputs: ``read_vpr_arch(file, layout)``, ``read_openfpga_arch(file)``, ``read_circuit(file)``
- Stages: ``pack()``, ``place()``, ``route()``, ``analysis()``, and ``run_flow()`` for the four
- Results made elsewhere: ``read_packing(file)``, ``read_placement(file)``, ``read_routing(file)``
- Any other command of the shell: ``command(name, *args, **options)``. ``key=value`` is written ``--key value``, ``key=True`` is the switch ``--key``

Options
~~~~~~~

``session.options`` gives the application options (see :ref:`openfpga_basic_commands`). ``options["place.seed"]`` is the value, with the type of the option (``str``, ``int``, ``float`` or ``bool``), and ``options["place.seed"] = 3`` sets it. A ``bool`` is written ``true`` or ``false``, and a path is accepted for a file. ``get(name)`` gives an ``AppOption`` with the ``default``, the ``help``, the ``choices`` of a selection and the ``stage`` that the option changes. ``list()`` and ``changed()`` give all the options, and those that do not have their default value, and ``reset(name)`` gives an option its default. An option that does not exist, or a value that it refuses, raises ``OpenfpgaError``.

What went wrong
~~~~~~~~~~~~~~~

A failure is text in the log of a command. ``OpenfpgaError.diagnostics`` (and ``errors``, which keeps the errors) reads it into ``Diagnostic`` objects, so that a program does not have to:

.. code-block:: python

  try:
      session.read_vpr_arch("bad.xml", layout="2x2")
  except openfpga.OpenfpgaError as error:
      found = error.errors[0]
      found.kind      # 'xml_syntax'
      found.file      # 'bad.xml'
      found.line      # 1
      found.hint      # "Fix the XML of 'bad.xml' at the line that the message gives"

A ``Diagnostic`` has a ``severity`` (``error`` or ``warning``; the errors come first, and at most ten warnings are kept), a ``kind``, the ``message`` as it is in the log, the ``file`` and ``line`` of the user that the message names (``None`` if it names none), the ``source`` (where VPR raised an error, for a report of a bug), a ``hint`` on what to try (``None`` when there is nothing to say), and ``data``, the facts that the message contains. The kinds do not change with the wording of a message:

.. list-table::
  :header-rows: 1

  * - ``kind``
    - Meaning, and its ``data``
  * - ``input_not_read``
    - An input is missing: ``input``, ``command`` to run
  * - ``stage_not_reached``
    - The stage that a command needs is not valid: ``stage``, ``command`` to run
  * - ``stage_failed``
    - The stage that a command needs ran and failed (see :ref:`failed_stages`): ``stage``, ``command`` to run again, ``reason``
  * - ``no_vpr_data``
    - A report of the results of the flow was asked before any flow
  * - ``file_not_found``
    - A file cannot be opened; ``file`` is set when the log says which
  * - ``xml_syntax``
    - An XML file cannot be parsed, with its ``file`` and ``line``
  * - ``option_unknown``, ``option_value_invalid``
    - An application option that does not exist, or a value it refuses: ``option``, ``type``, ``reason``
  * - ``command_unknown``, ``command_option_missing``
    - A command that does not exist, or an option that it requires: ``command`` or ``option``
  * - ``layout_not_found``
    - The device layout is not in the architecture: ``layout``, ``valid_layouts``
  * - ``route_failed``, ``route_unroutable``
    - The routing failed; for ``route_unroutable``, ``channel_width`` is the width that did not route
  * - ``worker_crashed``, ``worker_timeout``, ``worker_cancelled``, ``worker_not_running``
    - Failures of a ``RemoteSession`` worker (``WorkerError.diagnostics``); ``worker_crashed`` has the ``exitcode`` and the ``signal``, and its diagnostics go on with what the worker wrote before it ended
  * - ``tcl_error``, ``tcl_refused``
    - A Tcl script failed (``TclError.diagnostics``, see :ref:`session_tcl`); ``where`` is the line of the script. When a command of the shell failed, its own diagnostics follow
  * - ``vpr_<type>``
    - An error that VPR gave a type, e.g., ``vpr_architecture_file``, that no rule knows
  * - ``error``, ``warning``
    - Anything else

A new failure of a known kind needs a rule in ``openfpga_python_diagnostics.py`` and a test with the text that it comes from.

.. _session_constraints:

Constraints and options that change a flow
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The inputs that VPR takes besides the architecture and the circuit are options of the flow, and ``Session`` has a method for each. They return a ``Change``: the ``option``, its value ``before`` and ``after``, and the stages that were reached and are not any more (``dropped``, in the order of the flow). The next command of a stage that was dropped runs with the new value.

.. code-block:: python

  session.read_vpr_constraints("floorplan.xml")   # Change(option='filename.read_vpr_constraints_file', ..., dropped=['DEVICE', 'PACKED', 'PLACED'])
  session.read_sdc("timing.sdc")
  session.set_route_chan_width(24)
  session.set_option("place.seed", 3)             # any option; dropped=['PLACED', 'ROUTED', 'ANALYZED']
  session.read_sdc(None)                          # take the timing constraints away
  session.clear_constraints()                     # the floorplan and the timing constraints

- ``read_vpr_constraints(file)``: the floorplan of VPR (XML), i.e., the atoms of a partition and the regions where they must be packed and placed. ``None`` takes it away
- ``read_sdc(file)``: the timing constraints of VPR, i.e., the clocks and their periods, and the input and output delays. They set what a path requires (the ``data required time`` of the timing reports of VPR). ``None`` takes them away
- ``set_route_chan_width(width)``: route with this channel width instead of looking for the smallest. ``fixed_width_refusal(command, flat_routing=True)`` says why a flow that only routes at a fixed width (one that a program or a model drives) does not run a command now, or ``None``: ``vpr`` (the whole flow in one command), ``route`` while no width is set, and, with ``flat_routing=False`` (a device that does not support flat routing), ``route`` while ``router.flat_routing`` is set
- ``set_option(name, value)``: any option, as ``options[name] = value``, with its ``Change``

The placer is the option ``place.placer``: ``classic`` (``pack``, then ``place``) or ``analytical``, the analytical placement of VPR (``vpr --analytical_place``), which packs and places at once. With it, ``pack()`` and ``place()`` both run it and reach ``PACKED`` and ``PLACED`` together. The two are then one result: a change that drops the placement (an option ``place.*`` or ``aplace.*``) drops the packing too, and ``read_placement`` is refused on its packing. The stages that are run again on fresh data run it again. ``run_report().placement`` has ``placer`` and ``global_placement`` (its estimated wirelength and critical path delay).

Flat routing is the option ``router.flat_routing``: ``route()`` and ``analysis()`` route the atoms and not the clusters, with the port equivalences turned off, as ``vpr --flat_routing on`` does, and give the same files and results. ``run_report().routing.flat_routing`` says it; the wires and the wirelength are those of the channels.

VPR reads these when the device is made, so the options belong to the stage ``DEVICE`` and the whole flow runs again. A file is written as a whole path in the option, and a file that does not exist raises ``FileNotFoundError`` before anything changes. The constraints are not checked until VPR reads them: a constraint that VPR refuses makes the next command fail, with the message of VPR.

The pin constraints of OpenFPGA are not inputs of a stage: they are options of the commands that follow the stages, e.g., ``route_clock_rr_graph`` and ``pcf2place``. Give them with ``session.command("route_clock_rr_graph", pin_constraints_file="pcf.xml")``, which the journal records.

.. _journal_and_replay:

Journal and replay
~~~~~~~~~~~~~~~~~~

``Session(journal="run.journal")`` writes the commands that changed the flow, in the order they ran, in two files of that directory:

- ``replay.py``: a Python script, ``session.options["router.route_chan_width"] = 20``, ``session.command("pack")``, ...
- ``replay.openfpga``: a script of the shell, ``set_app_option --name router.route_chan_width --value 20``, ``pack``, ... It ends with ``exit``, as the shell does not stop at the end of a script, and runs with ``openfpga -f replay.openfpga``

.. code-block:: python

  session = openfpga.Session(journal="run.journal")
  session.options["router.route_chan_width"] = 20
  session.read_vpr_arch("arch.xml", layout="2x2")
  ...

  # Later, in another directory or process, or by hand: python run.journal/replay.py
  session = openfpga.Session.replay("run.journal")

What is written:

- the commands that the Session runs: the options, the inputs, the stages, the files of the results that are read (``read_packing``, ...), and any command that is run with ``session.command()``. Files are written as whole paths, so that a replay in another directory reads the same files. The files of the flow are written in the current directory of the replay
- not the commands that only answer (``Session.READ_ONLY_COMMANDS``: ``help``, ``report_session``, ``report_app_option``, ...), nor what is run with ``session.shell``: that is the way to ask, and to run what has no method, without a mark in the journal. A subclass of ``Session`` that adds commands that only answer extends the set: ``READ_ONLY_COMMANDS = Session.READ_ONLY_COMMANDS | {...}``
- a command that failed is a comment, ``# Failed, not replayed: route (...)``, with the reason
- the files are written again when a command ends, so that a run that stops leaves scripts that run. A new Session with the same ``journal`` starts the files again, unless ``resume_journal=True``: it goes on with what is there
- the script of a ``yosys`` command is copied into the directory (``scripts/001.ys``, ...) and the journal names the copy, as the file that was run may be written again for the next run: a replay runs the synthesis that was run
- ``Session.restore(directory, journal=...)`` writes the commands that restore does: the options, the inputs and the results that are read from the checkpoint

``Session.replay(path)`` takes the directory of the journal or one of its two files, runs the commands of ``replay.openfpga`` (a person can edit it: blank lines and ``#`` comments are skipped) and returns the Session, which goes on from there. The other arguments are those of ``Session``, e.g., ``journal=``, to journal the replay. A command that fails raises ``ReplayError``, with its ``line`` and its ``number``. ``RemoteSession(journal=directory)`` has the worker write the journal of its session, see :ref:`worker_processes`.

.. _failed_stages:

Stages that fail
~~~~~~~~~~~~~~~~

When the packing, the placement or the routing fails (the circuit does not fit the device, the router finds no legal routing), the stage is not reached, and what VPR made until then is kept, so that it can be read to tell why it failed:

.. code-block:: python

  session.options["router.route_chan_width"] = 8
  try:
      session.route()
  except openfpga.OpenfpgaError:
      pass
  session.state().failed        # StageFailure(stage='ROUTED', message='Routing failed: ...')

- ``state().failed`` has the ``stage`` and the ``message``. It is ``None`` once the stage is run again, or an option or an input that made it out of date is set
- the results of the stages before it stay: ``state().results`` lists their files, and the commands that read their data work as they did before
- the commands that need the stage that failed, e.g., ``repack`` after a failed route, raise ``OpenfpgaError`` with the diagnostic ``stage_failed``, which has the stage, the reason and the command to run again
- the next command of a stage starts again from fresh data of VPR, and gives the results it gives without the failure. ``checkpoint()`` does not save the stage that failed

A router that cannot reach a pin at all, e.g., with a channel width that is too small for the architecture, stops at once, and its routing has no shared wires: the log of VPR says ``Cannot route from ...``.

State and checkpoints
~~~~~~~~~~~~~~~~~~~~~

``state()`` gives what the flow has done (``SessionState``, from ``report_session``, see :ref:`openfpga_session_report_commands`): the stages that are ``reached``, the ``inputs`` (the files they were read from), the ``results`` (the files of the packing, the placement and the routing) and ``failed``, the stage that failed last and left its data, see :ref:`failed_stages`.

``checkpoint(directory)`` saves the flow, and ``Session.restore(directory)`` makes a session that is at the same point, in this process or in another one:

.. code-block:: python

  session.pack()
  session.place()
  session.checkpoint("run1.ckpt")

  # Later, e.g., in a new process
  session = openfpga.Session.restore("run1.ckpt")
  session.route()

The directory gets ``checkpoint.json`` and the files of the results of the stages that are reached (``.net``, ``.place`` and ``.route``). ``checkpoint.json`` lists the stages, the options that do not have their default value, and the files of the inputs, with the checksum of every file. The inputs are not copied: they must be where they were when ``restore()`` is called. Restoring sets the options, reads the inputs, reads the results of the stages instead of running them, and runs ``analysis`` again if it was reached. The flow then goes on with the same results as the one that was saved. The stages after ``ANALYZED`` (``repack`` and the bitstream) are not part of a checkpoint: run ``link_openfpga_arch``, ``build_fabric`` and the rest again after a restore.

A new checkpoint in the same directory replaces the old one, and the manifest is the last file to change. A file of the results that does not match its checksum, a missing file or an input that changed raises ``CheckpointError`` (``check_inputs=False`` accepts an input that changed, and VPR still refuses a packing that is not of the circuit). ``Session(checkpoint_dir=directory)`` makes a checkpoint after each stage.

Answers
~~~~~~~

A command that answers in JSON (see :ref:`query_answers`) gives its answer to the session as an object (an immutable dataclass) instead of JSON. The session reports of :ref:`openfpga_session_report_commands` are methods: ``state()`` (``SessionState``, from ``report_session``) , ``run_report()`` (``RunReport``, from ``report_run``) and ``stage_info(stage, changed, query, limit)`` (``StageReport``, from ``report_stage``). The fields are the ones of the answer, and ``stage`` tells which stage the answer was read from, for the classes that have it. Values that are not finite are ``None``.

An answer of another kind, or of a version of its schema that the module does not know, raises ``AnswerError``. A field that an answer gains without a new version is skipped. A command that a plugin adds can answer with a kind of its own: ``openfpga.register_answer_kind(kind, version)`` makes the module read it, and a subclass of ``Session`` reads the answer with ``self._query(command, answer_kind, cls, **options)``.

.. _session_tcl:

Tcl scripts
~~~~~~~~~~~

``tcl(script)`` runs a Tcl script in an interpreter that the session keeps, and returns a ``TclResult``: the ``result`` (the value of its last command), the ``output`` (what ``puts`` printed and the logs of the commands of the shell, in their order), ``printed`` (``puts`` alone), the ``commands`` of the shell that it ran, and ``elapsed_s``. Every command of the shell is a Tcl command, written as on its command line, so a loop or a sweep is one call:

.. code-block:: python

    session.tcl("""
    read_vpr_arch --file arch.xml --layout 2x2
    read_openfpga_arch --file openfpga_arch.xml
    read_circuit --file design.blif
    foreach seed {1 2 3} {
        set_app_option --name place.seed --value $seed
        place
        route
        puts "$seed [dict get [report_run] routing critical_path_delay_ns]"
    }
    """)

- The commands run through ``command()``, so the journal has those that change the flow, as if they had been run from Python. The Tcl around them (loops, procedures) is not in the journal.
- A command that answers in JSON (see :ref:`query_answers`) returns its answer as a Tcl dict: an object is a dict, a list is a list, ``true`` and ``false`` are ``1`` and ``0``, and ``null`` is the empty string. The JSON that the command prints is not in the output. Like in the shell, ``report_app_option`` answers as data only with ``--json``.
- A command that fails is an error of Tcl, which ``catch`` takes, with the message of the first error and ``-errorcode {OPENFPGA <kind>}``. A script that fails raises ``TclError``, with the ``output`` so far, the trace of Tcl (``error_info``), the ``commands`` and, when a command failed, its diagnostics.
- A command whose name is one of Tcl keeps it: ``source`` is the one of Tcl, and the one of the shell is ``openfpga::source``. Every command is also in the namespace ``openfpga``. ``exit`` and ``cd`` are not available, as the process and its directory are those of the session.
- The variables and the procedures stay for the next script, until the process ends: a worker that starts again (``restart()``) has a new interpreter.

``tcl(script, safe=True)`` runs the script in a safe interpreter (``interp create -safe``), kept apart from the other one: no files, no programs, no ``exit``. It has the commands of the shell but those that run another program or script (``ext_exec``, ``source``, ``yosys``), and an argument of a command that is a path must be in the current directory, links followed. What it refuses raises ``TclRefused``, before the command runs. It is meant for scripts that a program or a model writes.

``tcl(script, fixed_width=True, flat_routing=True)`` refuses, with ``TclRefused``, the commands that ``fixed_width_refusal()`` refuses (see :ref:`session_constraints`), before they run.

Tcl comes with ``tkinter``, which is part of most Python installations: a Python without it raises ``TclError``. In a ``RemoteSession`` the script runs in the worker, so that the timeout of the call and ``cancel()`` stop a script that does not end.

.. _worker_processes:

Worker processes
~~~~~~~~~~~~~~~~

VPR keeps its data in global variables, stops the process when an assertion fails, and exits on some bad options. A ``Session`` therefore shares its fate with the interpreter that runs it, and two sessions of a process share the data of VPR. ``RemoteSession`` runs a ``Session`` in a process of its own, a *worker*: whatever happens to the worker, the interpreter of the caller goes on, and workers share nothing, so flows can run at the same time.

.. code-block:: python

  with openfpga.RemoteSession(cwd="run1", checkpoint_dir="run1.ckpt") as session:
      session.options["router.route_chan_width"] = 100
      session.read_vpr_arch("arch.xml", layout="2x2")
      session.read_openfpga_arch("openfpga_arch.xml")
      session.read_circuit("design.blif")
      session.pack()
      session.place()
      try:
          session.route(timeout_s=600)
      except openfpga.WorkerError:
          session.restart()      # the worker is restored at the end of place
          session.route()

It has the methods of ``Session`` (``pack()``, ``state()``, ``options``, ``checkpoint()``, ...), which run in the worker and give back the same results. The failure of a command is the same ``OpenfpgaError``, and the worker goes on. ``shell`` is not available: the commands of the shell that have no method are run with ``command(name, **options)``. ``cwd`` is the directory of the worker, where VPR writes its files. ``RemoteSession.restore(directory)`` starts a worker that is restored from a checkpoint.

The failures of the worker itself are ``WorkerError``:

- ``WorkerCrashed``: the process ended during a call, e.g., an abort of VPR or a kill by the system. It has the ``exitcode``, the ``signal_name`` (e.g., ``SIGABRT``) and the last lines the worker wrote outside of its commands (``log_tail``)
- ``WorkerTimeout``: the call took longer than its ``timeout_s`` (an argument of the call, or an attribute of the object), and the worker was killed
- ``WorkerCancelled``: ``cancel()`` was called, from another thread, while the call was running, and the worker was killed. A command cannot be interrupted any other way
- ``WorkerNotRunning``: a call after one of them, or after ``close()``

``restart()`` starts another worker. With a ``checkpoint_dir`` the worker makes a checkpoint after each stage, and the new worker is restored from the last one, so that the flow loses the stage that was running and nothing else; without one it starts from nothing. It returns the state of the new session. A worker ends when its caller ends, even in the middle of a command (Linux), and ``close()``, or the end of a ``with`` block, ends it.

``RemoteSession(journal=directory)`` has the worker write the journal of its session in the directory (see :ref:`journal_and_replay`). A worker that starts again (``restart()``) goes on with it: the commands of the restore are not written twice, and a call that ends with the worker is not in it. ``memory_limit_mb`` limits the address space of the worker (``RLIMIT_AS``, Linux), for it and the workers that start again: a command that needs more fails with the error of the allocation, e.g., ``std::bad_alloc``, and the worker goes on. VPR and Yosys map much more than they use, so a worker needs a few GB to start; a limit that is too small for it raises ``WorkerError``.

The calls are made one at a time, and a call is not a way to share data: results are copied from the worker. The output of the commands is in their results, and a worker's own output outside of them goes to a log that is not kept.

Threads
~~~~~~~

A command that runs for long, e.g., ``place`` or ``route``, does not hold the interpreter: the GIL is released while it runs, so that the other threads go on, e.g., to show a progress, to read a queue of requests or to cancel a wait.

The commands run one at a time in the process, whichever session or shell they are sent to, because VPR keeps its data in global variables and the output of a command is captured for the whole process. A command that is sent from another thread waits for its turn. This makes concurrent calls safe, but it does not make them parallel, and two sessions that run a flow in turns still share the data of VPR (the last one to read an input wins). For flows that run at the same time, or that must not affect each other, use one process for each.

The output that another thread prints (``print``, ``sys.stdout``) while a command runs is part of the log of that command, as the capture is at the level of the process.
