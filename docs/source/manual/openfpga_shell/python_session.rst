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
  print(session.timing_summary().setup.fmax_mhz)

The files of the flow are written in the current directory of the process.

Installation
~~~~~~~~~~~~

The module is built from the source of OpenFPGA (a checkout with its submodules) and installed with ``pip``, in a virtual environment for instance:

.. code-block:: shell

  python3 -m venv ~/openfpga-env
  ~/openfpga-env/bin/pip install .
  ~/openfpga-env/bin/python3 openfpga_flow/openfpga_python_scripts/session_quickstart.py

The build uses ``scikit-build-core``, which ``pip`` fetches, and builds only the Python module (not the ``openfpga`` executable, Yosys or the tests). It needs a C++17 compiler, CMake, SWIG, the Python development headers and the Tcl development headers (``tcl-dev`` or ``tcl-devel``, which VPR's SDC parser uses). CMake takes the compiler that is first in the ``PATH``: set ``CC`` and ``CXX`` for another one, e.g., ``CC=gcc-12 CXX=g++-12 pip install .``. The result is ``import openfpga``. The build is kept in ``build/python``, so that the next ``pip install .`` only builds what changed.

``session_quickstart.py`` runs a small design through the four stages in a worker process, and prints the stages reached, the time of each, the Fmax, the utilization and the worst path. Its ``--in-process`` option runs a ``Session`` instead, and ``--help`` lists the files that it takes.

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
  * - ``no_vpr_data``
    - A query was asked before any flow
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
  * - ``vpr_<type>``
    - An error that VPR gave a type, e.g., ``vpr_architecture_file``, that no rule knows
  * - ``error``, ``warning``
    - Anything else

A new failure of a known kind needs a rule in ``openfpga_python_diagnostics.py`` and a test with the text that it comes from.

State and checkpoints
~~~~~~~~~~~~~~~~~~~~~

``state()`` gives what the flow has done (``SessionState``, from the query ``report_session``): the stages that are ``reached``, the ``inputs`` (the files they were read from) and the ``results`` (the files of the packing, the placement and the routing).

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

Queries
~~~~~~~

The queries of :ref:`openfpga_query_commands` are methods that return objects (immutable dataclasses) instead of JSON: ``timing_summary()`` (``TimingSummary``), ``critical_paths(count)`` (``CriticalPaths``), ``utilization()`` (``Utilization``), ``placement(block_type, prefix, limit)`` (``Placement``), ``net_route(net)`` (``NetRoute``), ``congestion(region_size, hotspots)`` (``Congestion``), ``cluster_report(query, stats, limit)`` (``ClusterReport``), ``device(tile_type, limit)`` (``Device``), ``find(pattern, kind, limit)`` (``Found``), ``block_info(block)`` (``BlockInfo``) and ``net_info(net)`` (``NetInfo``). The fields are the ones of the answers, listed with the commands, and ``stage`` tells which stage the answer was read from. Values that are not finite, e.g., the Fmax of a circuit without path, are ``None``.

An answer of another kind, or of a version of its schema that the module does not know, raises ``AnswerError``. A field that an answer gains without a new version is skipped.

A query raises ``OpenfpgaError`` when the stage it reads is not valid, e.g., after an option that was set since, and names the command to run first.

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

It has the methods of ``Session`` (``pack()``, ``timing_summary()``, ``options``, ``checkpoint()``, ...), which run in the worker and give back the same results. The failure of a command is the same ``OpenfpgaError``, and the worker goes on. ``shell`` is not available: the commands of the shell that have no method are run with ``command(name, **options)``. ``cwd`` is the directory of the worker, where VPR writes its files. ``RemoteSession.restore(directory)`` starts a worker that is restored from a checkpoint.

The failures of the worker itself are ``WorkerError``:

- ``WorkerCrashed``: the process ended during a call, e.g., an abort of VPR or a kill by the system. It has the ``exitcode``, the ``signal_name`` (e.g., ``SIGABRT``) and the last lines the worker wrote outside of its commands (``log_tail``)
- ``WorkerTimeout``: the call took longer than its ``timeout_s`` (an argument of the call, or an attribute of the object), and the worker was killed
- ``WorkerCancelled``: ``cancel()`` was called, from another thread, while the call was running, and the worker was killed. A command cannot be interrupted any other way
- ``WorkerNotRunning``: a call after one of them, or after ``close()``

``restart()`` starts another worker. With a ``checkpoint_dir`` the worker makes a checkpoint after each stage, and the new worker is restored from the last one, so that the flow loses the stage that was running and nothing else; without one it starts from nothing. It returns the state of the new session. A worker ends when its caller ends, even in the middle of a command (Linux), and ``close()``, or the end of a ``with`` block, ends it.

The calls are made one at a time, and a call is not a way to share data: results are copied from the worker. The output of the commands is in their results, and a worker's own output outside of them goes to a log that is not kept.

Threads
~~~~~~~

A command that runs for long, e.g., ``place`` or ``route``, does not hold the interpreter: the GIL is released while it runs, so that the other threads go on, e.g., to show a progress, to read a queue of requests or to cancel a wait.

The commands run one at a time in the process, whichever session or shell they are sent to, because VPR keeps its data in global variables and the output of a command is captured for the whole process. A command that is sent from another thread waits for its turn. This makes concurrent calls safe, but it does not make them parallel, and two sessions that run a flow in turns still share the data of VPR (the last one to read an input wins). For flows that run at the same time, or that must not affect each other, use one process for each.

The output that another thread prints (``print``, ``sys.stdout``) while a command runs is part of the log of that command, as the capture is at the level of the process.
