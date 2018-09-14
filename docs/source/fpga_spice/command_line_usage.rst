Command-line Options for FPGA SPICE Generator
=================================================
All the command line options of FPGA-SPICE can be shown by calling the help menu of VPR. Here are all the FPGA-SPICE-related options that you can find:

FPGA Spice Support Options::

--fpga_spice
--fpga_spice_dir <directory_path_output_spice_netlists>
--fpga_print_spice_top_testbench
--fpga_print_spice_lut_testbench
--fpga_print_spice_hardlogic_testbench
--fpga_print_spice_pb_mux_testbench
--fpga_print_spice_cb_mux_testbench
--fpga_print_spice_sb_mux_testbench
--fpga_print_spice_grid_testbench
--fpga_print_spice_cb_testbench
--fpga_print_spice_sb_testbench
--fpga_spice_leakage_only
--fpga_spice_parasitic_net_estimation <on|off>

.. note:: FPGA-SPICE requires the input of activity estimation results (\*.act file) from ACE2. 
   Remember to use the option --activity_file <act_file> to read the activity file. 

.. note::  To dump full-chip-level testbenches, the option –-fpga_print_spice_top_testbench should be enabled. 
   
.. note:: To dump grid-level testbenches, the options -- fpga_print_spice_grid_testbench, -- fpga_print_spice_cb_testbench and -- fpga_print_spice_sb_testbench should be enabled. 
  
.. note::  To dump component-level testbenches, the options –fpga_print_spice_lut_testbench, --fpga_print_spice_hardlogic_testbench, --fpga_print_spice_pb_mux_testbench, --fpga_print_spice_cb_mux_testbench and --fpga_print_spice_sb_mux_testbench should be enabled. 

.. csv-table:: Commmand-line Options of FPGA-SPICE
   :header: "Command Options", "Description"
   :widths: 15, 30

   "--fpga_spice", "Turn on the FPGA-SPICE."
   "--fpga_spice_dir <dir_path>", "Specify the directory that all the SPICE netlists will be outputted to. <dir_path> is the destination directory."
   "--fpga_print_spice_top_testbench", "Print the full-chip-level testbench for the FPGA."
   "--fpga_print_spice_lut_testbench", "Print the testbenches for all the LUTs."
   "--fpga_print_spice_dff_testbench", "Print the testbenches for all the FFs."
   "--fpga_print_spice_pb_mux_testbench", "Print the testbenches for all the multiplexers in the logic blocks."
   "--fpga_print_spice_cb_mux_testbench", "Print the testbenches for all the multiplexers in Connection Boxes."
   "-- fpga_print_spice_sb_mux_testbench", "Print the testbenches for all the multiplexers in Switch Blocks."
   "--fpga_print_spice_grid_testbench", "Print the testbenches for the logic blocks."
   "--fpga_spice_leakage_only", "FPGA-SPICE conduct power analysis on the leakage power only."
   "--fpga_spice_parasitic_net_estimation <on/off>", "Default: on. Turn on or off the parasitic net estimation technique."

.. note:: The parasitic net estimation technique is used to analyze the parasitic net activities which improves the accuracy of power analysis. When turned off, the errors between the full-chip-level and grid/component-level testbenches will increase."

