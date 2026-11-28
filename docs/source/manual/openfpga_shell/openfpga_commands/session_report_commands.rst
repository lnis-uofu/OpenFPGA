.. _openfpga_session_report_commands:

Session Report Commands
-----------------------

The session report commands tell what the flow that is run stage by stage has done, so that a script or a program can look at it before it decides what to do next. Each of them prints its answer as JSON and gives it to the program that ran the command, see :ref:`query_answers`. In Python, the answer is the ``data`` of the result:

.. code-block:: python

  shell.report_session().data["data"]["reached"]

In Python, the ``Session`` class gives the answers as objects, see :ref:`openfpga_python_session`.

The reports read, and never change, the data of the session. ``report_run`` needs the inputs to be read (the stage ``DEVICE``), and names the commands to run first when they are not. The schema of each answer is in ``openfpga_flow/schemas``.

report_session
~~~~~~~~~~~~~~

  Answer with what the flow that is run stage by stage has done: kind ``session``, not read from a stage. It works at any time, and needs no stage.

  - ``reached``: the stages that are valid, in the order of the flow (``DEVICE``, ``PACKED``, ``PLACED``, ``ROUTED``, ``ANALYZED``, ...). A stage that an option or an input made out of date is not in it. The list is empty when the data of VPR that the session worked with was replaced, e.g., by ``vpr`` or by another shell
  - ``inputs``: ``vpr_arch`` (``file`` and ``layout``), ``openfpga_arch`` and ``circuit`` (``file``): where each was read from, as a whole path. ``null`` for an input that is not read
  - ``failed``: ``stage`` and ``message`` of the stage (``PACKED``, ``PLACED`` or ``ROUTED``) that failed last and left what it made, e.g., a route that found no legal routing; ``null`` otherwise, and once the stage runs again or is dropped
  - ``results``: ``packing``, ``placement`` and ``routing``: the file of the results of a stage that is reached, which is the one that VPR wrote, or the one that ``read_packing``, ``read_placement`` or ``read_routing`` read instead of running the stage. ``null`` for the other stages. The path is whole, from the current directory of the process
  - ``attachments``: the data that plugins keep with the architectures of the session, by key (a key starts with the name of its plugin, e.g., ``myplugin.data``), as JSON. Reading an architecture clears them, see :ref:`openfpga_shell_plugins`

  .. code-block::

    report_session

report_stage
~~~~~~~~~~~~

  Answer with what can be changed for each stage of the flow: kind ``stage_info``, from the table of the application options. It is a bounded list, for the question "what can I change after ``place``?". It reads the session only, so it works at any time and needs no data of VPR.

  For each stage (``DEVICE``, ``PACKED``, ``PLACED``, ``ROUTED``, ``ANALYZED``, ``REPACKED``, ``BITSTREAM``):

  - ``requires``: the stage before it in the flow, and ``commands``: the commands that reach it
  - ``reached`` and ``failed`` (the message, if the stage failed and left its data): as in ``report_session``
  - ``dropped_by_change``: the stages that are dropped when an option of this stage changes, i.e., this one and the ones that depend on it. The earlier stages are kept
  - ``options``: ``total`` (the options of the stage), ``matched`` (by the filters), ``returned``, ``truncated`` and ``items``, which are the entries of ``report_app_option --json`` (name, type, value, default, ``is_default``, help, choices and stage)

  .. option:: --stage <string>

    Only this stage: its name in any case (``PLACED``), or a command that reaches it (``place``, ``read_routing``). An unknown stage is refused, with the names that exist. Default: all

  .. option:: --changed

    Only the options that do not have their default value

  .. option:: --query <string>

    Only the options whose name matches this shell-style pattern, e.g., ``place.*``

  .. option:: --limit <int>

    Most options to list for each stage. Default: 1000. The list says that it stopped at the limit with ``truncated``, so that the answer cannot grow without bounds

  .. code-block::

    report_stage --stage place --query "place.*" --limit 20
    report_stage --changed

report_run
~~~~~~~~~~

  Answer with what each stage of the flow did, in one place: kind ``run_report``, read from the stage ``DEVICE`` (it works as soon as the two architectures are read, and the parts of the stages that are not reached are ``null``). It reads the data of VPR, and for what only the log of VPR says, what the stage printed when a command of the session ran it.

  - ``stages``: the stages that are reached (``PACKED``, ``PLACED``, ``ROUTED``, ``ANALYZED``) with the ``seconds`` that the command took, ``null`` if the results were read from a file
  - ``packing``: the number of ``clusters``, ``atoms`` (the blocks of the circuit) and ``nets``, and the ``blocks`` of each type
  - ``placement``: ``half_perimeter``, the sum over the nets of the half perimeter of the box around their blocks, from the data. From the log of the placement: ``cost``, ``bb_cost``, ``td_cost``, the ``estimated_critical_path_delay_ns`` and ``estimated_fmax_mhz`` that the placer ends with, ``num_temperatures`` and ``swap_attempts``. ``placer`` is the option ``place.placer`` (``classic`` or ``analytical``); with the analytical placer the figures are those of its detailed placer, and ``global_placement`` has the ``estimated_wirelength`` and ``estimated_critical_path_delay_ns`` of its global placement (``null`` with the classic placer)
  - ``routing``: the ``channel_width``, the ``wirelength`` (the tiles that the wires span, which is what VPR adds up) and the number of ``wires``, from the data. From the log of the routing: ``iterations`` and ``critical_path_delay_ns``. ``flat_routing`` is true when the routing is flat (``router.flat_routing``): the wires and the wirelength are those of the channels, without the routing inside the clusters
  - ``failed``: the stage that failed last, as in ``report_session``

  What comes from a log is ``null`` when the stage was not run by a command (e.g., ``read_placement``), and the tests compare each number with the log of VPR, so that a change of the log is noticed.

  .. code-block::

    report_run

set_attachment
~~~~~~~~~~~~~~

  Keep the data of a plugin with the architectures of the session (an attachment, listed by ``report_session``), e.g., when a checkpoint is restored. Reading an architecture clears the attachments.

  .. option:: --key <string>

    The key: the name of the plugin, a dot and a name, e.g., ``myplugin.data``

  .. option:: --value <string>

    The value, as JSON. A value with double quotes, which a command line does not take, is given with ``--file``

  .. option:: --file <string>

    A file whose text is the value, as JSON

  Without ``--value`` and ``--file``, the attachment is dropped. In Python: ``Session.set_attachment(key, value)``.

  .. code-block::

    set_attachment --key myplugin.level --value 3
