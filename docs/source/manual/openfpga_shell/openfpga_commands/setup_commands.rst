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
  This is strongly recommended. Otherwise, the bitstream generation may be wrong

  .. warning:: This command may be deprecated in future when it is merged to VPR upstream
  
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
  
  .. option:: --duplicate_grid_pin

    Enable pin duplication on grid modules. This is optional unless ultra-dense layout generation is needed

  .. option:: --load_fabric_key <string>

    Load an external fabric key from an XML file. For example, ``--load_fabric_key fpga_2x2.xml`` See details in :ref:`file_formats_fabric_key`.

  .. option:: --generate_random_fabric_key

    Generate a fabric key in a random way

  .. option:: --write_fabric_key <string>.

    Output current fabric key to an XML file. For example, ``--write_fabric_key fpga_2x2.xml`` See details in :ref:`file_formats_fabric_key`.

  .. option:: --frame_view

    Create only frame views of the module graph. When enabled, top-level module will not include any nets. This option is made for save runtime and memory.

    .. warning:: Recommend to turn the option on when bitstream generation is the only purpose of the flow. Do not use it when you need generate netlists!

  .. option:: --verbose

    Show verbose log

  .. note:: This is a must-run command before launching FPGA-Verilog, FPGA-Bitstream, FPGA-SDC and FPGA-SPICE

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
