Command-line Options for FPGA SPICE Generator
=================================================
All the command line options of FPGA-SPICE can be shown by calling the help menu of VPR. Here are all the FPGA-SPICE-related options that you can find:

FPGA-SPICE Supported Options::

	--fpga_spice
	--fpga_spice_dir <directory_path_output_spice_netlists>
	--fpga_spice_print_top_testbench
	--fpga_spice_print_lut_testbench
	--fpga_spice_print_hardlogic_testbench
	--fpga_spice_print_pb_mux_testbench
	--fpga_spice_print_cb_mux_testbench
	--fpga_spice_print_sb_mux_testbench
	--fpga_spice_print_cb_testbench
	--fpga_spice_print_sb_testbench
	--fpga_spice_print_grid_testbench
	--fpga_spice_rename_illegal_port
	--fpga_spice_signal_density_weight <float>
	--fpga_spice_sim_window_size <float>
	--fpga_spice_leakage_only
	--fpga_spice_parasitic_net_estimation_off
	--fpga_spice_testbench_load_extraction_off
	--fpga_spice_sim_mt_num <int>

.. note:: FPGA-SPICE requires the input of activity estimation results (\*.act file) from ACE2. 
   Remember to use the option --activity_file <act_file> to read the activity file. 

.. note::  To dump full-chip-level testbenches, the option –-fpga_spice_print_top_testbench should be enabled. 
   
.. note:: To dump grid-level testbenches, the options -- fpga_spice_print_grid_testbench, -- fpga_spice_print_cb_testbench and -- fpga_spice_print_sb_testbench should be enabled. 
  
.. note::  To dump component-level testbenches, the options –fpga_spice_print_lut_testbench, --fpga_spice_print_hardlogic_testbench, --fpga_spice_print_pb_mux_testbench, --fpga_spice_print_cb_mux_testbench and --fpga_spice_print_sb_mux_testbench should be enabled. 

.. csv-table:: Commmand-line Options of FPGA-SPICE
   :header: "Command Options", "Description"
   :widths: 15, 30

   "--fpga_spice", "Turn on the FPGA-SPICE."
   "--fpga_spice_dir <dir_path>", "Specify the directory that all the SPICE netlists will be outputted to. <dir_path> is the destination directory."
   "--fpga_spice_print_top_testbench", "Print the full-chip-level testbench for the FPGA."
   "--fpga_spice_print_lut_testbench", "Print the testbenches for all the LUTs."
   "--fpga_spice_print_hardlogic_testbench", "Print the test benches for all the hardlogics."
   "--fpga_spice_print_pb_mux_testbench", "Print the testbenches for all the multiplexers in the logic blocks."
   "--fpga_spice_print_cb_mux_testbench", "Print the testbenches for all the multiplexers in Connection Boxes."
   "-- fpga_spice_print_sb_mux_testbench", "Print the testbenches for all the multiplexers in Switch Blocks."
   "--fpga_spice_print_cb_testbench", "Print the testbenches for all the CBs."
   "--fpga_spice_print_sb_testbench", "Print the testbenches for all the SBs."
   "--fpga_spice_print_grid_testbench", "Print the testbenches for the logic blocks."
   "--fpga_spice_rename_illegal_port", "Rename illegal port names"
   "--fpga_spice_signal_density_weight <float>", "Set the weight of signal density."
   "--fpga_spice_sim_window_size <float>", "Set the window size in determining the number of clock cycles in simulation."
   "--fpga_spice_leakage_only", "FPGA-SPICE conduct power analysis on the leakage power only."
   "--fpga_spice_parasitic_net_estimation_off", "Turn off the parasitic net estimation technique."
   "--fpga_spice_testbench_load_extraction_off", "Turn off the load effect on net estimation technique."
   "--fpga_spice_sim_mt_num <int>", "Set the number of multi-thread used in simulation"

.. note:: The parasitic net estimation technique is used to analyze the parasitic net activities which improves the accuracy of power analysis. When turned off, the errors between the full-chip-level and grid/component-level testbenches will increase."
