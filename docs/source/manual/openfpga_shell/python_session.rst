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

Queries
~~~~~~~

The queries of :ref:`openfpga_query_commands` are methods that return objects (immutable dataclasses) instead of JSON: ``timing_summary()`` (``TimingSummary``), ``critical_paths(count)`` (``CriticalPaths``), ``utilization()`` (``Utilization``), ``placement(block_type, prefix, limit)`` (``Placement``), ``net_route(net)`` (``NetRoute``), ``congestion(region_size)`` (``Congestion``) and ``cluster_report(query, stats, limit)`` (``ClusterReport``). The fields are the ones of the answers, listed with the commands, and ``stage`` tells which stage the answer was read from. Values that are not finite, e.g., the Fmax of a circuit without path, are ``None``.

An answer of another kind, or of a version of its schema that the module does not know, raises ``AnswerError``. A field that an answer gains without a new version is skipped.

A query raises ``OpenfpgaError`` when the stage it reads is not valid, e.g., after an option that was set since, and names the command to run first.
