.. _openfpga_script_format:

OpenFPGA Script Format
----------------------

OpenFPGA accepts a simplified tcl-like script format.

.. option:: Comments
   
   Any content after a ``#`` will be treated as comments.
   Comments will not be executed.

  .. note:: comments can be added inline or as a new line. See the example below

.. option:: Continued line
  
   Lines to be continued should be finished with ``\``.
   Continued lines will be conjuncted and executed as one line
 
  .. note:: please ensure necessary spaces. Otherwise it may cause command parser fail.

The following is an example.

.. code-block:: python

  # Run VPR for the s298 design
  vpr ./test_vpr_arch/k6_frac_N10_40nm.xml ./test_blif/and.blif --clock_modeling route #--write_rr_graph example_rr_graph.xml
  
  # Read OpenFPGA architecture definition
  read_openfpga_arch -f ./test_openfpga_arch/k6_frac_N10_40nm_openfpga.xml
  
  # Write out the architecture XML as a proof
  #write_openfpga_arch -f ./arch_echo.xml
  
  # Annotate the OpenFPGA architecture to VPR data base
  link_openfpga_arch --activity_file ./test_blif/and.act \
                     --sort_gsb_chan_node_in_edges #--verbose
  
  # Check and correct any naming conflicts in the BLIF netlist
  check_netlist_naming_conflict --fix --report ./netlist_renaming.xml
  
  # Apply fix-up to clustering nets based on routing results
  pb_pin_fixup --verbose
  
  # Apply fix-up to Look-Up Table truth tables based on packing results
  lut_truth_table_fixup #--verbose
  
  # Build the module graph 
  #  - Enabled compression on routing architecture modules
  #  - Enable pin duplication on grid modules 
  build_fabric --compress_routing \
               --duplicate_grid_pin #--verbose
  
  # Repack the netlist to physical pbs
  # This must be done before bitstream generator and testbench generation
  # Strongly recommend it is done after all the fix-up have been applied
  repack #--verbose
  
  # Build the bitstream 
  #  - Output the fabric-independent bitstream to a file
  build_architecture_bitstream --verbose \
                               --write_file /var/tmp/xtang/openfpga_test_src/fabric_indepenent_bitstream.xml
  
  # Build fabric-dependent bitstream
  build_fabric_bitstream --verbose
  
  # Write the Verilog netlist for FPGA fabric
  #  - Enable the use of explicit port mapping in Verilog netlist
  write_fabric_verilog --file /var/tmp/xtang/openfpga_test_src/SRC \
                       --explicit_port_mapping \
                       --include_timing \
                       --include_signal_init \
                       --support_icarus_simulator \
                       --print_user_defined_template \
                       --verbose
  
  # Write the Verilog testbench for FPGA fabric
  #  - We suggest the use of same output directory as fabric Verilog netlists
  #  - Must specify the reference benchmark file if you want to output any testbenches
  #  - Enable top-level testbench which is a full verification including programming circuit and core logic of FPGA
  #  - Enable pre-configured top-level testbench which is a fast verification skipping programming phase
  #  - Simulation ini file is optional and is needed only when you need to interface different HDL simulators using openfpga flow-run scripts
  write_verilog_testbench --file /var/tmp/xtang/openfpga_test_src/SRC \
                          --reference_benchmark_file_path /var/tmp/xtang/and.v \
                          --print_top_testbench \
                          --print_preconfig_top_testbench \
                          --print_simulation_ini /var/tmp/xtang/openfpga_test_src/simulation_deck.ini
  
  # Write the SDC files for PnR backend
  #  - Turn on every options here 
  write_pnr_sdc --file /var/tmp/xtang/openfpga_test_src/SDC 
  
  # Write the SDC to run timing analysis for a mapped FPGA fabric
  write_analysis_sdc --file /var/tmp/xtang/openfpga_test_src/SDC_analysis
  
  # Finish and exit OpenFPGA
  exit
