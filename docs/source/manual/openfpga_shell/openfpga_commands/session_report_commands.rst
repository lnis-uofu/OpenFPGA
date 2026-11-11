.. _openfpga_session_report_commands:

Session Report Commands
-----------------------

The session report commands tell what the flow that is run stage by stage has done, so that a script or a program can look at it before it decides what to do next. Each of them prints its answer as JSON and gives it to the program that ran the command, see :ref:`query_answers`. In Python, the answer is the ``data`` of the result:

.. code-block:: python

  shell.report_session().data["data"]["reached"]

In Python, the ``Session`` class gives the answers as objects, see :ref:`openfpga_python_session`.

The reports read, and never change, the data of the session. The schema of each answer is in ``openfpga_flow/schemas``.

report_session
~~~~~~~~~~~~~~

  Answer with what the flow that is run stage by stage has done: kind ``session``, not read from a stage. It works at any time, and needs no stage.

  - ``reached``: the stages that are valid, in the order of the flow (``DEVICE``, ``PACKED``, ``PLACED``, ``ROUTED``, ``ANALYZED``, ...). A stage that an option or an input made out of date is not in it. The list is empty when the data of VPR that the session worked with was replaced, e.g., by ``vpr`` or by another shell
  - ``inputs``: ``vpr_arch`` (``file`` and ``layout``), ``openfpga_arch`` and ``circuit`` (``file``): where each was read from, as a whole path. ``null`` for an input that is not read
  - ``results``: ``packing``, ``placement`` and ``routing``: the file of the results of a stage that is reached, which is the one that VPR wrote, or the one that ``read_packing``, ``read_placement`` or ``read_routing`` read instead of running the stage. ``null`` for the other stages. The path is whole, from the current directory of the process

  .. code-block::

    report_session
