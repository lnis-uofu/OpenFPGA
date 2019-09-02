.. _run_fpga_flow:

OpenFPGA Flow
---------------

This python script executes the supported OpenFPGA flow for a
single benchmark and architecture file for given script parameters.

The script is located at::

    ${OPENFPGA_PATH}/openfpga_flow/scripts/run_fpga_flow.py

.. program:: run_fpga_flow.py

Basic Usage
~~~~~~~~~~~

At a minimum ``open_fpga_flow.py`` requires following command-line arguments::

    open_fpga_flow.py <architecture_file> <benchmark_files> --top_module <top_module_name>

where:

  * ``<architecture_file>`` is the target :ref:`FPGA architecture <fpga_architecture_description>`
  * ``<circuit_file>`` The list of files in the benchmark (Supports ../directory/\*.v)
  * ``<top_module_name>`` The name of the top level module in Verilog project

.. note::
    The script will create a ``tmp`` run directory in base OpenFPGA path, unless otherwise specified with the :option:`--run_dir` option.
    All stages of the flow will be run within run directory.
    Several intermediate files will be generated and maintian in run directory.
    The path variables declared in architecture XML file will be resolved with absolute path and copied to the ``tmp/arch`` directory before executing flow.
    All the benchmark files provided will be copied to ``tmp/bench`` directory without maintaining any directory structure.
    **Users should ensure that no important files are kept in this directory as script will clear directory before each execution**

.. _openfpga-variables:

OpenFPGA Variables
~~~~~~~~~~~~~~~~~~
Frequently, while running OpenFPGA flow User is suppose to refer external files.
To avoid long names and referencing errors user can use
following openfpga variables.
These variables are resolved with absolute path while execution making
each run independent of launch directory.


  * ``<OPENFPGA_PATH>`` Path to the base OpenFPGA directory
  * ``<OPENFPGA_FLOW_PATH>`` Path to the run_fpga_flow script directory
  * ``<SPICENETLIST_PATH>`` Path where spice netlists are saved
  * ``<VERILOG_PATH>`` Path where Verilog modules are saved
  * ``<TECH_PATH>`` Path where all characterized XML files are stored

For example in architecture file path vairable can be used as follows::

    .... lib_path="${TECH_PATH}/PTM_45nm/45nm.pm" ....

Output
~~~~~~
Based on which flow is executed, resulting in intermediate files are generated in run_directory

The output log of the script provides the status of each stage to the user.
If any stage failed to execute, the output log would indicate the stage at which execution failed, and execution traceback.

In case of successful execution, The OpenFPGA flow script will parse
parameters listed in configuration from different result files and will create
``vpr_stat.txt``, ``vpr_stat_power.txt`` \(optional\) file in run_directory.

Advanced Usage
~~~~~~~~~~~~~~

User can pass additional *optional* command arguments to ``run_fpga_flow.py`` script::

    run_fpga_flow.py <architecture_file> <benchmark_files> [<options>] [<vpr_options>] [<fpga-verilog_options>] [<fpga-spice_options>] [<fpga-bitstream_options>] [<ace_options>]


where:

  * ``<options>`` are additional arguments passed to ``run_fpga_flow.py`` (described below),
  * ``<vpr_options>`` Any argument prefixed with ``--vpr-*`` will be forwarded to vpr script as it is. The detail of supported vpr argument is available ``Add corrrect reference``
  * ``<fpga-verilog_options>`` are any arguments not recognized by ``run_vtr_flow.pl``. These will be forwarded to VPR.
  * ``<ace_options>`` these arguments will be passed to ACE activity estimator program

For example::

   run_fpga_flow.py my_circuit.v my_arch.xml -track_memory_usage --pack --place

will run the VTR flow to map the circuit ``my_circuit.v`` onto the architecture ``my_arch.xml``; the arguments ``--pack`` and ``--place`` will be passed to VPR (since they are unrecognized arguments to ``run_vtr_flow.pl``).
They will cause VPR to perform only :ref:`packing and placement <general_options>`.

Detailed Command-line Options
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 .. Note:: All the commnadline arguments starting with ``vpr_*`` , ``fpga-verilog_*`` , ``fpga-spice_*`` or ``fpga-bitstream_*`` will be passed to VPR without suffix

General Arguments
^^^^^^^^^^^^^^^^^

.. option:: --top_module <name>

    Provide top module name of the benchmark. Default ``top``

.. option:: --run_dir <directory_path>

    Using this option user can provide a custom path as a run directory. Default is  ``tmp`` directory in OpenFPGA root path.

.. option:: --K <lut_inputs>

      This option defines the number of inputs to the LUT. By default, the script parses provided architecture file and finds out inputs to the biggest LUT.

.. option:: --yosys_tmpl <yosys_template_file>

    This option allows the user to provide a custom Yosys template
    While running a yosys_vpr flow. Default template is stored in a directory ``open_fpga_flow\misc\ys_tmpl_yosys_vpr_flow.ys``. Yosys template script supports ``TOP_MODULE`` ``READ_VERILOG_FILE`` ``LUT_SIZE`` & ``OUTPUT_BLIF`` variables, which can be used as ``${var_name}``. Alternately, user can create a copy and modify according to their need.

.. option:: --debug

    To enable detail logs printing.

.. option:: --flow_config

    User can provide option flow configuration file to override some of the default script parameters.
    for detail information refer :ref:`OpenFPGA Flow Configuration <OpenFPGA_Conf_File>`

ACE Arguments
^^^^^^^^^^^^^
.. option:: --black_box_ace

    Performs ACE simulation on the black box [deprecated]

VPR RUN Arguments
^^^^^^^^^^^^^^^^^

.. option:: --fix_route_chan_width <channel_number>

    Performs VPR implementation for a fixed number of channels defined as the 'channel_number'

.. option:: --min_route_chan_width <percentage_slack>

    Performs VPR implementation to get minimum channel width and then perform fixed channel rerouting with ``percentage_slack`` increase in the channel width.

.. option:: --max_route_width_retry <max_retry_count>

    Number of times  the channel width should be increased and attempt VPR implementation, while performing ``min_route_chan_width``

.. option:: --power
.. option:: --power_tech


blif_vpr_flow Arguments
^^^^^^^^^^^^^^^^^^^^^^^^

.. option:: --activity_file

    Activity to be used for the given benchmark while running ``blif_vpr_flow``

.. option:: --base_verilog

    Verilog benchmark file to perform verification while running ``bliff_vpr_flow``



.. _OpenFPGA_Conf_File:
OpenFPGA Flow Configuration file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The OpenFPGA Flow configuration file consists of following sections

    * ``CAD_TOOLS_PATH``
        Lists executable file path for different CAD tools used in the script

    * ``FLOW_SCRIPT_CONFIG``
        Lists the supported flows by the script.

    * ``DEFAULT_PARSE_RESULT_VPR``
        List of default parameters to be parsed from Place, Pack, and Route output

    * ``DEFAULT_PARSE_RESULT_POWER``
        List of default parameters to be parsed from VPR power analysis output

    * ``INTERMIDIATE_FILE_PREFIX``
        [Not implemented yet]

Default OpenFPGA_flow Configuration file is located in ``open_fpga_flow\misc\fpgaflow_default_tool_path.conf``.
User-supplied configuration file overrides or extends the default configuration.
