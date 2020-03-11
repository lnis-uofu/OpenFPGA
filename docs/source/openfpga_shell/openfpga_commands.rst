.. _openfpga_commands:

Commands
--------

As OpenFPGA integrates various tools, the commands are categorized into different classes:

Basic Commands
~~~~~~~~~~~~~~

.. option:: help

  Show help desk to list all the available commands

.. option:: exit

  Exit OpenFPGA shell

VPR
~~~

.. option:: vpr
  
  OpenFPGA allows users to call ``vpr`` in the standard way as documented in vtr project.

Setup OpenFPGA
~~~~~~~~~~~~~~

.. option:: read_openfpga_arch

  Read the XML architecture file required by OpenFPGA

  - ``--file`` or ``-f`` Specify the file name 

  - ``--verbose`` Show verbose log

.. option:: write_openfpga_arch

  Write the OpenFPGA XML architecture file to a file

  - ``--file`` or ``-f`` Specify the file name 

  - ``--verbose`` Show verbose log

.. option:: link_openfpga_arch

  Annotate the OpenFPGA architecture to VPR data base

  - ``--activity_file`` Specify the signal activity file

  - ``--sort_gsb_chan_node_in_edges`` Sort the edges for the routing tracks in General Switch Blocks (GSBs). Strongly recommand to turn this on for uniquifying the routing modules

  - ``--verbose`` Show verbose log

.. option:: check_netlist_naming_conflict 

  Check and correct any naming conflicts in the BLIF netlist
  This is strongly recommended. Otherwise, the outputted Verilog netlists may not be compiled successfully.

  .. note:: This command may be deprecated in future when merged to VPR upstream
  
  - ``--fix`` Apply fix-up to the names that violate the syntax

  - ``--report <.xml>`` Report the naming fix-up to a log file

.. option:: pb_pin_fixup

  Apply fix-up to clustering nets based on routing results
  This is strongly recommended. Otherwise, the bitstream generation may be wrong

  .. note:: This command may be deprecated in future when merged to VPR upstream
  
  - ``--verbose`` Show verbose log
   
.. option:: lut_truth_table_fixup

  Apply fix-up to Look-Up Table truth tables based on packing results

  .. note:: This command may be deprecated in future when merged to VPR upstream

  - ``--verbose`` Show verbose log
  
.. option:: build_fabric

  Build the module graph. This is a must-run command before launching FPGA-Verilog, FPGA-Bitstream, FPGA-SDC and FPGA-SPICE

  - ``--compress_routing`` Enable compression on routing architecture modules. Strongly recommend this as it will minimize the number of routing modules to be outputted. It can reduce the netlist size significantly.
  
  - ``--duplicate_grid_pin`` Enable pin duplication on grid modules. This is optional unless ultra-dense layout generation is needed

  - ``--verbose`` Show verbose log
  
FPGA-Bitstream
~~~~~~~~~~~~~~

.. option:: repack

  Repack the netlist to physical pbs
  This must be done before bitstream generator and testbench generation
  Strongly recommend it is done after all the fix-up have been applied
   
  - ``--verbose`` Show verbose log

.. option:: build_architecture_bitstream

  Decode VPR implementing results to an fabric-independent bitstream database 
  
  - ``--file`` or ``-f`` Output the fabric-independent bitstream to an XML file
  
  - ``--verbose`` Show verbose log

.. option:: build_fabric_bitstream

  Reorganize the bitstream database for a specific FPGA fabric

  - ``--verbose`` Show verbose log
  
FPGA-Verilog
~~~~~~~~~~~~

.. option:: write_fabric_verilog

  Write the Verilog netlist for FPGA fabric based on module graph

  - ``--file`` or ``-f`` Specify the output directory for the Verilog netlists

  - ``--explict_port_mapping`` Use explict port mapping when writing the Verilog netlists

  - ``--include_timing`` Output timing information to Verilog netlists for primitive modules
 
  - ``--include_signal_init`` Output signal initialization to Verilog netlists for primitive modules

  - ``--support_icarus_simulator`` Output Verilog netlists with syntax that iVerilog simulatorcan accept

  - ``--print_user_defined_template`` Output a template Verilog netlist for all the user-defined ``circuit models`` in :ref:`circuit_library`. This aims to help engineers to check what is the port sequence required by top-level Verilog netlists

  - ``--verbose`` Show verbose log

.. option:: write_verilog_testbench
 
  Write the Verilog testbench for FPGA fabric

  - ``--file`` or ``-f`` The output directory for all the testbench netlists. We suggest the use of same output directory as fabric Verilog netlists

  - ``--reference_benchmark_file_path`` Must specify the reference benchmark Verilog file if you want to output any testbenches

  - ``--print_top_testbench`` Enable top-level testbench which is a full verification including programming circuit and core logic of FPGA

  - ``--print_formal_verification_top_netlist`` Generate a top-level module which can be used in formal verification

  - ``--print_preconfig_top_testbench`` Enable pre-configured top-level testbench which is a fast verification skipping programming phase

  - ``--print_simulation_ini`` Output an exchangeable simulation ini file, which is needed only when you need to interface different HDL simulators using openfpga flow-run scripts

FPGA-SDC
~~~~~~~~

.. option:: write_pnr_sdc
 
  Write the SDC files for PnR backend
  
  - ``--file`` or ``-f`` Specify the output directory for SDC files

  - ``--constrain_global_port`` Constrain all the global ports of FPGA fabric

  - ``--constrain_grid`` Constrain all the grids of FPGA fabric

  - `--constrain_sb`` Constrain all the switch blocks of FPGA fabric

  - ``--constrain_cb`` Constrain all the connection blocks of FPGA fabric

  - ``--constrain_configurable_memory_outputs`` Constrain all the outputs of configurable memories of FPGA fabric

  - ``--constrain_routing_multiplexer_outputs`` Constrain all the outputs of routing multiplexer of FPGA fabric

  - ``--constrain_switch_block_outputs`` Constrain all the outputs of switch blocks of FPGA fabric
  
  - ``--verbose`` Enable verbose output

.. option:: write_analysis_sdc

  Write the SDC to run timing analysis for a mapped FPGA fabric

  - ``--file`` or ``-f`` Specify the output directory for SDC files
