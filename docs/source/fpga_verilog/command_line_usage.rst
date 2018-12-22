Command-line Options for FPGA-Verilog Generator
=================================================

All the command line options of FPGA-Verilog can be shown by calling the help menu of VPR. Here are all the FPGA-Verilog-related options that you can find:

FPGA-Verilog Supported Options::	
	
	--fpga_verilog
	--fpga_verilog_dir <directory_path_of_dumped_verilog_files>
	--fpga_verilog_include_timing
	--fpga_verilog_init_sim
	--fpga_verilog_print_modelsim_autodeck
	--fpga_verilog_modelsim_ini_path <string>
	--fpga_verilog_print_top_testbench 
	--fpga_verilog_print_top_auto_testbench <path_to_the_verilog_benchmark>


.. csv-table:: Commmand-line Options of FPGA-Verilog
   :header: "Command Options", "Description"
   :widths: 15, 30

   "--fpga_verilog", "Turn on the FPGA-Verilog."
   "--fpga_verilog_dir <dir_path>", "Specify the directory that all the Verilog files will be outputted to. <dir_path> is the destination directory."
   "--fpga_verilog_include_timing", "Includes the timings found in the XML file."
   "--fpga_verilog_init_sim", "Initializes the simulation for ModelSim."
   "--fpga_verilog_print_modelsim_autodeck", "Generates the scripts necessary to the ModelSim simulation."
   "--fpga_verilog_modelsim_ini_path <string>", "Gives the path for the .ini necessary to ModelSim."
   "--fpga_verilog_print_top_testbench", "Print the full-chip-level testbench for the FPGA. Determines the type of autodeck."
   "--fpga_verilog_print_top_auto_testbench <path_to_the_verilog_benchmark>", "Prints the testbench associated with the given benchmark. Determines the type of autodeck."

.. note:: The selected directory will contain the *Verilog top file* and three other folders. The folders are: 

	* **sub_module:** contains each module verilog file and is more detailed in the next part *Verilog Output File Format*. 
	* **routing:** contains the Verilog routing files. 
	* **lb:** contains the grids Verilog files.



