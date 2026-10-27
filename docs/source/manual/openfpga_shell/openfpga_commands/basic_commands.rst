.. _openfpga_basic_commands:

Basic Commands
--------------

version
~~~~~~~

  Show OpenFPGA version information

help
~~~~

  Show help desk to list all the available commands

source
~~~~~~

  Run a set of existing commands from a string stream or a file

  .. option:: --command_stream <string>

    A string/file stream which contains the commands to be executed. Use quote(``"``) to group command and semicolumn(``;``) to split between commands. For example,

  .. code-block::

     source --command_stream "help;exit;"

  .. option:: --from_file

    Specify the command stream comes from a file. When selected, the file will be parsed as a regular script following the OpenFPGA script file format. See details in :ref:`openfpga_script_format`

  .. option:: --batch_mode

    Enable batch mode when executing the script from a file. Valid only when ``--from_file`` is enabled.

    .. note:: If you are sourcing a file when running OpenFPGA in script mode, please turn on the batch mode here. See details in :ref:`launch_openfpga_shell`

ext_exec
~~~~~~~~

  Run a system call for a command which is not in OpenFPGA shell

  .. option:: --command <string>

    A string stream which contains the command to be executed. Use quote(``"``) to group command. For example,

  .. code-block::

    ext_exec --command "ls -all"

.. _set_app_option:

set_app_option
~~~~~~~~~~~~~~

  Set an application option. Application options tune the engines of the flow, e.g., the seed of the placer. Every option has a name made of a group and an option, e.g., ``place.seed``, a type and a default value. The value is checked against the type: an invalid value is an error, and the option keeps its value.

  The options are the ones of VPR, with the same defaults, plus a few of OpenFPGA. The groups are ``filename`` (files and the architecture and circuit to read), ``general``, ``atom`` (atom netlist), ``cluster`` (packing), ``aplace`` (analytical placement), ``place`` and ``tplace`` (placement and timing-driven placement), ``router`` and ``trouter`` (routing and timing-driven routing), ``analysis``, ``power``, ``crr`` and ``build_fabric`` (OpenFPGA only). Options that only make sense on the command line of VPR, e.g., stage switches, graphics and verbosity, are not available. Use :ref:`report_app_option <report_app_option>` to list the options, their values and the values a selection accepts.

  Every option tunes one stage of the flow: ``filename``, ``general``, ``atom`` and ``crr`` the device, ``cluster`` packing, ``aplace``, ``place`` and ``tplace`` placement, ``router`` and ``trouter`` routing, ``analysis`` and ``power`` the analysis, and ``build_fabric`` the repacking. A few options belong to another stage than their group: the ``filename`` options belong to the stage of the file they name, e.g., ``filename.place_file`` to placement, and the channel width (``router.route_chan_width``, ``place.place_chan_width``) to the device, which includes the routing resource graph. Changing the value of an option makes the results of its stage, and of the stages after it, out of date, so that the next run of these stages uses the new value. The earlier stages are kept. Setting the value an option already has changes nothing.

  .. option:: --name <string>

    Name of the option, e.g., ``place.seed``

  .. option:: --value <string>

    Value of the option. It is read according to the type of the option:

    - string: any text. Use quotes (``"``) if it has spaces
    - int: a whole number, e.g., ``10`` or ``-2``
    - float: a number, e.g., ``0.5`` or ``1e-3``
    - bool: ``true``, ``false``, ``on``, ``off``, ``yes``, ``no``, ``1`` or ``0``
    - selection: one of the values the option accepts. They are case sensitive

  .. code-block::

    set_app_option --name place.seed --value 5
    set_app_option --name router.flat_routing --value true
    set_app_option --name place.place_algorithm --value bounding_box

.. _report_app_option:

report_app_option
~~~~~~~~~~~~~~~~~

  Report the value of one application option, or of all of them. Each option is shown as ``name = value``. A selection also shows the values it accepts, in brackets.

  .. option:: --name <string>

    Name of the option to report. All the options are reported when it is not set

  .. code-block::

    report_app_option --name place.seed

exit
~~~~

  Exit OpenFPGA shell

