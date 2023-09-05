.. _openfpga_setup_commands:

Setup OpenFPGA
--------------

read_openfpga_arch
~~~~~~~~~~~~~~~~~~

  Read the XML file about architecture description (see details in :ref:`arch_generality`)

  .. option:: --file <string> or -f <string>
     
    Specify the file name. For example, ``--file openfpga_arch.xml`` 

  .. option:: --verbose

    Show verbose log

write_openfpga_arch
~~~~~~~~~~~~~~~~~~~

  Write the OpenFPGA XML architecture file to a file

  .. option:: --file <string> or -f <string>
     
    Specify the file name. For example, ``--file arch_echo.xml`` 

  .. option:: --verbose

    Show verbose log

read_openfpga_simulation_setting
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Read the XML file about simulation settings (see details in :ref:`simulation_setting`)

  .. option:: --file <string> or -f <string>
     
    Specify the file name. For example, ``--file auto_simulation_setting.xml`` 

  .. option:: --verbose

    Show verbose log

write_openfpga_simulation_setting
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Write the OpenFPGA XML simulation settings to a file

  .. option:: --file <string> or -f <string>
     
    Specify the file name. For example, ``--file auto_simulation_setting_echo.xml``.
    See details about file format at :ref:`simulation_setting`.

  .. option:: --verbose

    Show verbose log

read_openfpga_bitstream_setting
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Read the XML file about bitstream settings (see details in :ref:`file_formats_bitstream_setting`)

  .. option:: --file <string> or -f <string>
     
    Specify the file name. For example, ``--file bitstream_setting.xml`` 

  .. option:: --verbose

    Show verbose log

write_openfpga_bitstream_setting
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Write the OpenFPGA XML bitstream settings to a file

  .. option:: --file <string> or -f <string>
     
    Specify the file name. For example, ``--file auto_bitstream_setting_echo.xml``.
    See details about file format at :ref:`file_formats_bitstream_setting`.

  .. option:: --verbose

    Show verbose log

.. _openfpga_setup_command_read_openfpga_clock_arch:

read_openfpga_clock_arch
~~~~~~~~~~~~~~~~~~~~~~~~

  Read the XML file about programmable clock network (see details in :ref:`file_formats_clock_network`)

  .. option:: --file <string> or -f <string>
     
    Specify the file name. For example, ``--file clock_network.xml`` 

  .. option:: --verbose

    Show verbose log

write_openfpga_clock_arch
~~~~~~~~~~~~~~~~~~~~~~~~~

  Write the OpenFPGA programmable clock network to an XML file

  .. option:: --file <string> or -f <string>
     
    Specify the file name. For example, ``--file clock_network_echo.xml``.
    See details about file format at :ref:`file_formats_clock_network`.

  .. option:: --verbose

    Show verbose log

append_clock_rr_graph
~~~~~~~~~~~~~~~~~~~~~

Build the routing resource graph based on an defined programmable clock network, and append it to the existing routing resource graph built by VPR.
Use command :ref:`openfpga_setup_command_read_openfpga_clock_arch`` to load the clock network.

  .. option:: --verbose

    Show verbose log

route_clock_rr_graph
~~~~~~~~~~~~~~~~~~~~

Route clock signals on the built routing resource graph which contains a programmable clock network.
Clock signals will be auto-detected and routed based on pin constraints which are provided by users.

  .. option:: --pin_constraints_file <string> or -pcf <string>

    Specify the *Pin Constraints File* (PCF) when the clock network contains multiple clock pins. For example, ``-pin_constraints_file pin_constraints.xml``
    Strongly recommend for multi-clock network. See detailed file format about :ref:`file_format_pin_constraints_file`.

  .. option:: --verbose

    Show verbose log

link_openfpga_arch
~~~~~~~~~~~~~~~~~~

  Annotate the OpenFPGA architecture to VPR data base

  .. option:: --activity_file <string>

    Specify the signal activity file. For example, ``--activity_file counter.act``.
    This is required when users wants OpenFPGA to automatically find the number of clocks in simulations. See details at :ref:`simulation_setting`.

  .. option:: --sort_gsb_chan_node_in_edges

    Sort the edges for the routing tracks in General Switch Blocks (GSBs). Strongly recommand to turn this on for uniquifying the routing modules

  .. option:: --verbose

    Show verbose log

write_gsb_to_xml
~~~~~~~~~~~~~~~~

  Write the internal structure of General Switch Blocks (GSBs) across a FPGA fabric, including the interconnection between the nodes and node-level details, to XML files

  .. option:: --file <string> or -f <string>

    Specify the output directory of the XML files. Each GSB will be written to an indepedent XML file
    For example, ``--file /temp/gsb_output``

  .. option:: --unique

    Only output unique GSBs to XML files

  .. option:: --exclude_rr_info

    Exclude routing resource graph information from output files, e.g., node id as well as other attributes. This is useful to check the connection inside GSBs purely.

  .. option:: --exclude <string>

    Exclude part of the GSB data to be outputted. Can be [``sb``|``cbx``|``cby``]. Users can exclude multiple parts by using a splitter ``,``.
    For example, 

      - ``--exclude sb``
      - ``--exclude sb,cbx``

  .. option:: --gsb_names <string>

    Specify the name of GSB to be outputted. Users can specify multiple GSBs by using a splitter ``,``.
    When specified, only the GSBs whose names match the list will be outputted to files.
    If not specified, all the GSBs will be outputted.

    .. note:: When option ``--unique`` is enable, the given name of GSBs should match the unique modules! 

    For example,

      - ``--gsb_names gsb_2__4_,gsb_3__2_``
      - ``--gsb_names gsb_2__4_``

  .. option:: --verbose

    Show verbose log

  .. note:: This command is used to help users to study the difference between GSBs

check_netlist_naming_conflict 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Check and correct any naming conflicts in the BLIF netlist
  This is strongly recommended. Otherwise, the outputted Verilog netlists may not be compiled successfully.

  .. warning:: This command may be deprecated in future when it is merged to VPR upstream
  
  .. option:: --fix

    Apply fix-up to the names that violate the syntax

  .. option:: --report <string>

    Report the naming fix-up to an XML-based log file. For example, ``--report rename.xml``

pb_pin_fixup
~~~~~~~~~~~~

  Apply fix-up to clustering nets based on routing results

  .. note:: Suggest to skip the similar fix-up applied by VPR through options ``--skip_sync_clustering_and_routing_results on`` when calling vpr in openfpga shell.

  .. warning:: This feature has been integrated into VPR to provide accurate timing analysis results at post-routing stage. However, this command provides a light fix-up (not as thorough as the one in VPR) but bring more flexibility in support some architecture without local routing. Suggest to enable it when your architecture does not have local routing for *Look-Up Tables* (LUTs) but you want to enable logic equivalent for input pins of LUTs

  .. warning:: This command may be deprecated in future
  
  .. option:: --verbose

    Show verbose log
   
lut_truth_table_fixup
~~~~~~~~~~~~~~~~~~~~~

  Apply fix-up to Look-Up Table truth tables based on packing results

  .. warning:: This command may be deprecated in future when it is merged to VPR upstream

  .. option:: --verbose

    Show verbose log

.. _cmd_build_fabric:
  
build_fabric
~~~~~~~~~~~~

  Build the module graph.

  .. option:: --compress_routing

    Enable compression on routing architecture modules. Strongly recommend this as it will minimize the number of routing modules to be outputted. It can reduce the netlist size significantly.

  .. option:: --group_tile <string>

    Group fine-grained programmable blocks, connection blocks and switch blocks into tiles. Once enabled, tiles will be added to the top-level module. Otherwise, the top-level module consists of programmable blocks, connection blocks and switch blocks. The tile style can be customized through a file. See details in :ref:`file_formats_tile_config_file`. When enabled, the Verilog netlists will contain additional netlists that model tiles (see details in :ref:`fabric_netlists_tiles`). 

    .. warning:: This option does not support ``--duplicate_grid_pin``!
   
    .. warning:: This option requires ``--compress_routing`` to be enabled!

  .. option:: --group_config_block

    Group configuration memory blocks under each CLB/SB/CB etc. into a centralized configuration memory blocks, as depicted in :numref:`fig_group_config_block_overview`. When disabled, the configuration memory blocks are placed in a distributed way under CLB/SB/CB etc. For example, each programming resource, e.g., LUT, has a dedicated configuration memory block, being placed in the same module. When enabled, as illustrated in :numref:`fig_group_config_block_hierarchy`, the physical memory block locates under a CLB, driving a number of logical memory blocks which are close to the programmable resources. The logical memory blocks contain only pass-through wires which can be optimized out during physical design phase.

  .. _fig_group_config_block_overview:
  
  .. figure:: ./figures/group_config_block_overview.png
     :width: 100%
  
     Impact on grouping configuable blocks: before and after
  
  .. _fig_group_config_block_hierarchy:
  
  .. figure:: ./figures/group_config_block_hierarchy.png
     :width: 100%
  
     Netlist hierarchy on grouped configuable blocks

 
  .. option:: --duplicate_grid_pin

    Enable pin duplication on grid modules. This is optional unless ultra-dense layout generation is needed

  .. option:: --load_fabric_key <string>

    Load an external fabric key from an XML file. For example, ``--load_fabric_key fpga_2x2.xml`` See details in :ref:`file_formats_fabric_key`.

  .. option:: --generate_random_fabric_key

    Generate a fabric key in a random way

  .. option:: --write_fabric_key <string>.

    Output current fabric key to an XML file. For example, ``--write_fabric_key fpga_2x2.xml`` See details in :ref:`file_formats_fabric_key`.

    .. warning:: This option will be deprecated. Use :ref:`cmd_write_fabric_key` as a replacement.

  .. option:: --frame_view

    Create only frame views of the module graph. When enabled, top-level module will not include any nets. This option is made for save runtime and memory.

    .. warning:: Recommend to turn the option on when bitstream generation is the only purpose of the flow. Do not use it when you need generate netlists!

  .. option:: --verbose

    Show verbose log

  .. note:: This is a must-run command before launching FPGA-Verilog, FPGA-Bitstream, FPGA-SDC and FPGA-SPICE

.. _cmd_write_fabric_key:

write_fabric_key
~~~~~~~~~~~~~~~~

  Output current fabric key to an XML file. For example, ``write_fabric_key --file fpga_2x2.xml`` See details in :ref:`file_formats_fabric_key`.

  .. note:: This command can output module-level keys while the ``--write_fabric_key`` option in command ``build_fabric`` does NOT support! Strongly recommend to use this command to obtain fabric key.

  .. option:: --file <string> or -f <string>
     
    Specify the file name. For example, ``--file fabric_key_echo.xml``.

  .. option:: --include_module_keys

    Output module-level keys to the file.

  .. option:: --verbose

    Show verbose log

.. _cmd_add_fpga_core_to_fabric:
  
add_fpga_core_to_fabric
~~~~~~~~~~~~~~~~~~~~~~~

  Add a wrapper module ``fpga_core`` as an intermediate layer to FPGA fabric. After this command, the existing module ``fpga_top`` will remain the top-level module while there is a new module ``fpga_core`` under it. Under fpga_core, there will be the detailed building blocks.

  .. option:: --io_naming <string>

    This is optional. Specify the I/O naming rules when connecting I/Os of ``fpga_core`` module to the top-level module ``fpga_top``. If not defined, the ``fpga_top`` will be the same as ``fpga_core`` w.r.t. ports. See details about the file format of I/O naming rules in :ref:`file_formats_io_naming_file`.

  .. option:: --instance_name <string>

    This is optional. Specify the instance name to be used when instanciate the ``fpga_core`` module under the top-level module ``fpga_top``. If not defined, by default it is ``fpga_core_inst``.

  .. option:: --frame_view

    Create only frame views of the module graph. When enabled, top-level module will not include any nets. This option is made for save runtime and memory.

    .. warning:: Recommend to turn the option on when bitstream generation is the only purpose of the flow. Do not use it when you need generate netlists!

  .. option:: --verbose

    Show verbose log


write_fabric_hierarchy
~~~~~~~~~~~~~~~~~~~~~~

  Write the hierarchy of FPGA fabric graph to a plain-text file
  
  .. option:: --file <string> or -f <string>
  
    Specify the file name to write the hierarchy. 

  .. option:: --depth <int>

    Specify at which depth of the fabric module graph should the writer stop outputting. The root module start from depth 0. For example, if you want a two-level hierarchy, you should specify depth as 1. 

  .. option:: --verbose

    Show verbose log

  .. note:: This file is designed for hierarchical PnR flow, which requires the tree of Multiple-Instanced-Blocks (MIBs).

.. _openfpga_setup_commands_write_fabric_io_info:

write_fabric_io_info
~~~~~~~~~~~~~~~~~~~~

  Write the I/O information of FPGA fabric to an XML file
  
  .. option:: --file <string> or -f <string>
  
    Specify the file name to write the I/O information

  .. option:: --no_time_stamp

    Do not print time stamp in bitstream files

  .. option:: --verbose

    Show verbose log

  .. note:: This file is designed for pin constraint file conversion.

.. _openfpga_setup_commands_pcf2place:

pcf2place
~~~~~~~~~

  Convert a Pin Constraint File (.pcf, see details in :ref:`file_format_pcf_file`) to a `placement file <https://docs.verilogtorouting.org/en/latest/vpr/file_formats/#placement-file-format-place>`_)
  
  .. option:: --pcf <string>
  
    Specify the path to the users' pin constraint file

  .. option:: --blif <string>

    Specify the path to the users' post-synthesis netlist

  .. option:: --fpga_io_map <string>

    Specify the path to the FPGA I/O location. Achieved by the command :ref:`openfpga_setup_commands_write_fabric_io_info`

  .. option:: --pin_table <string>

    Specify the path to the pin table file, which describes the pin mapping between chip I/Os and FPGA I/Os. See details in :ref:`file_format_pin_table_file`

  .. option:: --fpga_fix_pins <string>

    Specify the path to the placement file which will be outputted by running this command 

  .. option:: --pin_table_direction_convention <string>

    Specify the naming convention for ports in pin table files from which pin direction can be inferred. Can be [``explicit``|``quicklogic``]. When ``explicit`` is selected, pin direction is inferred based on the explicit definition in a column of pin table file, e.g., GPIO direction (see details in :ref:`file_format_pin_table_file`). When ``quicklogic`` is selected, pin direction is inferred by port name: a port whose postfix is ``_A2F`` is an input, while a port whose postfix is ``_A2F`` is an output. By default, it is ``explicit``.

  .. option:: --no_time_stamp

    Do not print time stamp in bitstream files

  .. option:: --verbose

    Show verbose log
