.. _openfpga_setup_commands:

Setup OpenFPGA
--------------

read_openfpga_arch
~~~~~~~~~~~~~~~~~~

  Read the XML file about architecture description (see details in :ref:`arch_generality`)

  - ``--file`` or ``-f`` Specify the file name 

  - ``--verbose`` Show verbose log

write_openfpga_arch
~~~~~~~~~~~~~~~~~~~

  Write the OpenFPGA XML architecture file to a file

  - ``--file`` or ``-f`` Specify the file name 

  - ``--verbose`` Show verbose log

read_openfpga_simulation_setting
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Read the XML file about simulation settings (see details in :ref:`simulation_setting`)

  - ``--file`` or ``-f`` Specify the file name 

  - ``--verbose`` Show verbose log

write_openfpga_simulation_setting
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Write the OpenFPGA XML simulation settings to a file

  - ``--file`` or ``-f`` Specify the file name 

  - ``--verbose`` Show verbose log

link_openfpga_arch
~~~~~~~~~~~~~~~~~~

  Annotate the OpenFPGA architecture to VPR data base

  - ``--activity_file`` Specify the signal activity file

  - ``--sort_gsb_chan_node_in_edges`` Sort the edges for the routing tracks in General Switch Blocks (GSBs). Strongly recommand to turn this on for uniquifying the routing modules

  - ``--verbose`` Show verbose log

write_gsb_to_xml
~~~~~~~~~~~~~~~~

  Write the internal structure of General Switch Blocks (GSBs) across a FPGA fabric, including the interconnection between the nodes and node-level details, to XML files

  - ``--file`` or ``-f`` Specify the output directory of the XML files. Each GSB will be written to an indepedent XML file

  - ``--verbose`` Show verbose log

  .. note:: This command is used to help users to study the difference between GSBs

check_netlist_naming_conflict 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Check and correct any naming conflicts in the BLIF netlist
  This is strongly recommended. Otherwise, the outputted Verilog netlists may not be compiled successfully.

  .. warning:: This command may be deprecated in future when it is merged to VPR upstream
  
  - ``--fix`` Apply fix-up to the names that violate the syntax

  - ``--report <.xml>`` Report the naming fix-up to a log file

pb_pin_fixup
~~~~~~~~~~~~

  Apply fix-up to clustering nets based on routing results
  This is strongly recommended. Otherwise, the bitstream generation may be wrong

  .. warning:: This command may be deprecated in future when it is merged to VPR upstream
  
  - ``--verbose`` Show verbose log
   
lut_truth_table_fixup
~~~~~~~~~~~~~~~~~~~~~

  Apply fix-up to Look-Up Table truth tables based on packing results

  .. warning:: This command may be deprecated in future when it is merged to VPR upstream

  - ``--verbose`` Show verbose log

.. _cmd_build_fabric:
  
build_fabric
~~~~~~~~~~~~

  Build the module graph.

  - ``--compress_routing`` Enable compression on routing architecture modules. Strongly recommend this as it will minimize the number of routing modules to be outputted. It can reduce the netlist size significantly.
  
  - ``--duplicate_grid_pin`` Enable pin duplication on grid modules. This is optional unless ultra-dense layout generation is needed

  - ``--load_fabric_key <xml_file>`` Load an external fabric key from an XML file.

  - ``--generate_fabric_key`` Generate a fabric key in a random way

  - ``--write_fabric_key <xml_file>`` Output current fabric key to an XML file

  - ``--verbose`` Show verbose log

  .. note:: This is a must-run command before launching FPGA-Verilog, FPGA-Bitstream, FPGA-SDC and FPGA-SPICE

write_fabric_hierarchy
~~~~~~~~~~~~~~~~~~~~~~

  Write the hierarchy of FPGA fabric graph to a plain-text file
  
  - ``--file`` or ``-f`` Specify the file name to write the hierarchy. 

  - ``--depth`` Specify at which depth of the fabric module graph should the writer stop outputting. The root module start from depth 0. For example, if you want a two-level hierarchy, you should specify depth as 1. 

  - ``--verbose`` Show verbose log

  .. note:: This file is designed for hierarchical PnR flow, which requires the tree of Multiple-Instanced-Blocks (MIBs).
