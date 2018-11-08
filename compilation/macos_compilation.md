MacOS compilation
==================

*This tutorial has been tested under MacOS High Sierra 10.13.4*

Clone the [OpenFPGA git repository:](https://github.com/LNIS-Projects/OpenFPGA)

`git clone --recurse-submodules https://github.com/LNIS-Projects/OpenFPGA.git `

[//todo]: # (There is a submodule in the repository so move to the OpenFPGA directory and clone that too:)

[//]: # (`git submodule init`)

[//todo]: # (`git submodule update`)

Go to the `VPR` directory and build the tool:

`cd ./OpenFPGA/vpr7_x2p/vpr/`

Note: the graphical interface might not be usable since it requires the X11 library. In this case, open the Makefile and change the line 10 "ENABLE_GRAPHICS = true" to false.

`make `

This will generate vpr and a libvpr.a file.

Enhancements of VPR were made.
In order to see them just type:

`./vpr`

This will show the different options that can be used. Our modifications concern the options starting with fpga_spice and fpga_verilog.

A script is already prepared in the folder to test FPGA-SPICE:

`./go.sh`

This script uses the enhanced version of vpr with some new options such as --fpga_spice_print_top_testbench which automatically generates a testbench for the full FPGA and --fpga_verilog_dir which allows us to choose the destination directory for the verilog output we generate.
For more informations on how the new commands work, please visit [OpenFPGA Options FPGA-SPICE](https://openfpga.readthedocs.io/en/latest/fpga_spice/command_line_usage.html).

As a result, we get a new folder, /verilog_test, which contains the verilog code. The name_top.v contains the full FPGA we just created. Three other folders are created, *lb*, *routing* and *sub_modules*. *lb* contains the different CLBs used in the architecture. *routing* contains the different connection blocks, the switch boxes and the wires. *sub_modules* contains the different modules needed in the architecture.