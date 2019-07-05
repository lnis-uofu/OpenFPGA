Command-line Options for FPGA-Verilog Generator
=================================================

All the command line options of FPGA-Verilog can be shown by calling the help menu of VPR. Here are all the FPGA-Verilog-related options that you can find:

FPGA-Verilog Supported Options::	
	
	--fpga_verilog
	--fpga_verilog_dir <directory_path_of_dumped_verilog_files>
	--fpga_verilog_include_timing
	--fpga_verilog_include_signal_init
	--fpga_verilog_print_modelsim_autodeck <modelsim_ini_path>
	--fpga_verilog_print_top_testbench 
	--fpga_verilog_print_autocheck_top_testbench <reference_verilog_file_path>
	--fpga_verilog_print_formal_verification_top_netlist
	--fpga_verilog_include_icarus_simulator


.. csv-table:: Commmand-line Options of FPGA-Verilog
   :header: "Command Options", "Description"
   :widths: 15, 30

   "--fpga_verilog", "Turn on the FPGA-Verilog."
   "--fpga_verilog_dir <dir_path>", "Specify the directory where all the Verilog files will be outputted to. <dir_path> is the destination directory."
   "--fpga_verilog_include_timing", "Includes the timings found in the XML architecture description file."
   "--fpga_verilog_include_signal_init", "Set all nets to random value to be close of a real power-on case"
   "--fpga_verilog_print_modelsim_autodeck <modelsim_ini_path>", "Generates the scripts necessary to the ModelSim simulation and specify the path to modelsim.ini file."
   "--fpga_verilog_print_top_testbench", "Prints the full-chip-level testbench for the FPGA, which includes programming phase and operationg phase (random patterns)."
   "--fpga_verilog_print_autocheck_top_testbench \
   <reference_verilog_file_path>", "Prints a testbench stimulating the generated FPGA and the initial benchmark to compare stimuli responses, which includes programming phase and operationg phase (random patterns)"
   "--fpga_verilog_print_formal_verification_top_netlist", "Prints a Verilog top file compliant with formal verification tools. With this top file the FPGA is initialy programmed. It also prints a testbench with random patterns, which can be manually or automatically check regarding previous options."
   "--fpga_verilog_include_icarus_simulator", "Activates waveforms .vcd file generation and simulation timeout, which are required for Icarus Verilog simulator"

.. note:: The selected directory will contain the *Verilog top file* and three other folders. The folders are: 

	* **sub_module:** contains each module verilog file and is more detailed in the next part *Verilog Output File Format*. 
	* **routing:** contains the Verilog for the connection blocks and the switch boxes. 
	* **lb:** contains the grids Verilog files.



