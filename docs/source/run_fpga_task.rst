.. _run_fpga_task:

OpenFPGA Task
---------------

Tasks provide a framework for running the :ref:`run_fpga_flow` on
multiple benchmarks, architectures and set of OpenFPGA parameters.
The structure of the framework is very similar to
`VTR-Tasks <https://docs.verilogtorouting.org/en/latest/vtr/tasks/>`_
implementation with additional functionality and minor file extention changes.

Task Directory
~~~~~~~~~~~~~~

The tasks are store in a ``TASK_DIRECTORY``, which by default points to
``${OPENFPGA_PATH}/openfpga_flow/tasks``. Every directory or sub-directory in
task directory consisting of ``../config/task.conf`` file can be reffered as a
task.

To create as task name called ``basic_flow`` following directory has to exist::

   ${TASK_DIRECTORY}/basic_flow/conf/task.conf

Similarly  ``regression/regression_quick`` expect following structure::

   ${TASK_DIRECTORY}/regression/regression_quick/conf/task.conf


Running OpenFPGA Task:
~~~~~~~~~~~~~~~~~~~~~~

At a minimum ``open_fpga_flow.py`` requires following command-line arguments::

    open_fpga_flow.py <task1_name> <task2_name> ...

where:

  * ``<task_name>`` is the name of the task to run


Craeating A New OpenFPGA Task:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Create the folder ``${TASK_DIRECTORY}/<task_name>`` and create a file called
``${TASK_DIRECTORY}/<task_name>/config/task.conf`` in it.



Configuring a New Task
~~~~~~~~~~~~~~~~~~~~~~

The task configuration file ``task.conf`` consists of ``GENERAL``,
``ARCHITECTURES``, ``BENCHMARKS``, ``SYNTHESIS_PARAM`` and
``SCRIPT_PARAM_<var_name>`` sections.
Declaring all the above sections are mandatory.

.. note::
    Configuration file supports all the OpenFPGA Variables refer
    :ref:`openfpga-variables` section to know more. Variables in configuration
    file is declares as ``${PATH:<variable_name>}``

General Section
^^^^^^^^^^^^^^^

.. option:: fpga_flow==<yosys_vpr|vpr_blif>

    Defines which OpenFPGA flow to run. By default ``yosys_vpr`` is executed.

.. option:: power_analysis=<true|false>

    Specifies whether to perform power analysis or not.

.. option:: power_tech_file=<path_to_tech_XML_file>

    Declares which tech XML file to be used while perforing Power Analysis.

.. option:: spice_output=<true|false>

    Setting up this variable generates Spice Netlist at the end of the flow.
    Equivalent of passing ``--vpr_fpga_spice`` command to :ref:`run_fpga_flow`

.. option:: verilog_output=<true|false>

    Setting up this variable generates Verilog Netlist at the end of the flow.
    Equivalent of passing ``--vpr_fpga_spice`` command to :ref:`run_fpga_flow`

.. option:: timeout_each_job=<true|false>

    Specifies the the timeout for each :ref:`run_fpga_flow` execution. Default
    is set to ``20 min``


Architectures Sections
^^^^^^^^^^^^^^^^^^^^^^

    User can define the list of architecure files in this section.

.. option:: arch<arch_label>=<xml_architecture_file_path>

    The ``arch_label`` variable can be any number of string without
    white-spaces. ``xml_architecture_file_path`` is path to the actual XML
    architecture file

.. note::

    In final OpenFPGA Task result the architecture will be referred by its
    ``arch_label``.

Benchmarks Sections
^^^^^^^^^^^^^^^^^^^

    User can define the list of benchmarks files in this section.

.. option:: bench<bench_label>=<list_of_files_in_benchmark>

    The ``bench_label`` variable can be any number of string without
    white-spaces. ``xml_architecture_file_path`` is path to the actual XML
    architecture file

    For Example following code shows how to define a benchmarks,
    with single file multiple files and files added from specific directory.

    .. code-block:: text

        [BENCHMARKS]
        # To declare single benchmark file
        bench_design1=${BENCH_PATH}/design/top.v

        # To declare multiple benchmark file
        bench_design2=${BENCH_PATH}/design/top.v,${BENCH_PATH}/design/sub_module.v

        # To add all files in specific directory to the benchmark
        bench_design3=${BENCH_PATH}/design/top.v,${BENCH_PATH}/design/lib/*.v

.. note::
    ``bench_label`` is referred again in ``Synthesis_Param`` section to
    provide addional information about benchmark

Synthesis Parameter Sections
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    User can define extra parameters for each benchmark defined in the
    ``BENCHMARKS`` sections.

.. option:: bench<bench_label>_top=<Top_Module_Name>

    This defines the Top Level module name for ``bench_label`` benchmark.
    By default, the top level module name is cosidereed as a ``top``.

.. option:: bench<bench_label>_yosys_tmpl=<yosys_template_file>

   [TODO]

.. option:: bench<bench_label>_chan_width=<chan_width_to_use>

    In case of running fixed channel width routing for each benchmark,
    this option defines the channel width to be used for ``bench_label``
    benchmark

.. option:: bench<bench_label>_act=<activity_file_path>

    In case of running ``blif_vpr_flow`` this option provides the activity files
    to be used to generate testbench for ``bench_label`` benchmark

.. option:: bench<bench_label>_verilog=<source_verilog_file_path>

    In case of running ``blif_vpr_flow`` with verification this option provides
    the source verilog design for ``bench_label`` benchmark to be used
    while verification.

Script Parameter Sections
^^^^^^^^^^^^^^^^^^^^^^^^^
The script parameter section lists set of commnad line pararmeters to be passed to :ref:`run_fpga_flow` script. The section name is defines as ``SCRIPT_PARAM_<parameter_set_label>`` where `parameter_set_label` can be any word without white spaces.
The section is referred with ``parameter_set_label`` in final result file.

For example following code Specifies the two sets (``Fixed_Routing_30`` and ``Fixed_Routing_50``) of :ref:`run_fpga_flow` arguments.

.. code-block:: text

    [SCRIPT_PARAM_Fixed_Routing_30]
    # Execute fixed routing with channel with 30
    fix_route_chan_width=30

    [SCRIPT_PARAM_Fixed_Routing_50]
    # Execute fixed routing with channel with 50
    fix_route_chan_width=50

Example Task Configuration File
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. code-block:: text

    [GENERAL]
    spice_output=false
    verilog_output=false
    power_analysis = true
    power_tech_file = ${PATH:TECH_PATH}/winbond90nm/winbond90nm_power_properties.xml
    timeout_each_job = 20*60

    [ARCHITECTURES]
    arch0=${PATH:ARCH_PATH}/winbond90/k6_N10_rram_memory_bank_SC_winbond90.xml

    [BENCHMARKS]
    bench0=${PATH:BENCH_PATH}/MCNC_Verilog/s298/s298.v
    bench1=${PATH:BENCH_PATH}/MCNC_Verilog/elliptic/elliptic.v

    [SYNTHESIS_PARAM]
    bench0_top = s298
    bench1_top = elliptic

    [SCRIPT_PARAM_Slack_30]
    min_route_chan_width=1.3

    [SCRIPT_PARAM_Slack_80]
    min_route_chan_width=1.8

