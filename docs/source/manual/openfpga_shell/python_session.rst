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

The queries of :ref:`openfpga_query_commands` are methods that return objects (immutable dataclasses) instead of JSON: ``timing_summary()`` (``TimingSummary``), ``critical_paths(count)`` (``CriticalPaths``), ``utilization()`` (``Utilization``), ``placement(block_type, prefix, limit)`` (``Placement``), ``net_route(net)`` (``NetRoute``), ``congestion(region_size)`` (``Congestion``) and ``cluster_report(query, stats, limit)`` (``ClusterReport``). The fields are the ones of the answers, listed with the commands, and ``stage`` tells which stage the answer was read from. Values that are not finite, e.g., the Fmax of a circuit without path, are ``None``.

An answer of another kind, or of a version of its schema that the module does not know, raises ``AnswerError``. A field that an answer gains without a new version is skipped.

A query raises ``OpenfpgaError`` when the stage it reads is not valid, e.g., after an option that was set since, and names the command to run first.

Threads
~~~~~~~

A command that runs for long, e.g., ``place`` or ``route``, does not hold the interpreter: the GIL is released while it runs, so that the other threads go on, e.g., to show a progress, to read a queue of requests or to cancel a wait.

The commands run one at a time in the process, whichever session or shell they are sent to, because VPR keeps its data in global variables and the output of a command is captured for the whole process. A command that is sent from another thread waits for its turn. This makes concurrent calls safe, but it does not make them parallel, and two sessions that run a flow in turns still share the data of VPR (the last one to read an input wins). For flows that run at the same time, or that must not affect each other, use one process for each.

The output that another thread prints (``print``, ``sys.stdout``) while a command runs is part of the log of that command, as the capture is at the level of the process.
