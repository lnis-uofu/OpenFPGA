Command-line Options for FPGA-Verilog Generator
=================================================

All the command line options of FPGA-Verilog can be shown by calling the help menu of VPR. Here are all the FPGA-Verilog-related options that you can find:

FPGA-Verilog Supported Options::

	--fpga_verilog
	--fpga_verilog_dir <directory_path_of_dumped_verilog_files>

.. csv-table:: Commmand-line Options of FPGA-Verilog
   :header: "Command Options", "Description"
   :widths: 15, 30

   "--fpga_verilog", "Turn on the FPGA-Verilog."
   "--fpga_verilog_dir <dir_path>", "Specify the directory that all the Verilog files will be outputted to. <dir_path> is the destination directory."

.. note:: The selected directory will contain the *Verilog top file* and three other folders. The folders are: 

	* **sub_module:** contains each module verilog file and is more detailed in the next part *Verilog Output File Format*. 
	* **routing:** contains the Verilog routing files. 
	* **lib:** contains the grids Verilog files.



