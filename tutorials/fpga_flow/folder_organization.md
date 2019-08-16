# fpga_flow folder organization

The fpga_flow folder is organized as follow:
* **arch**: contains architectures description files
* **benchmarks**: contains Verilog and blif benchmarks + lists
* **configs**: contains configuration files to run fpga_flow.pl
* **scripts**: contains all the scripts required to run the flow
* **tech**: contains xml tech files for power estimation

## arch
In this folder are saved the architecture files. These files are Hardware description for the FPGA written in XML. This folder contains 3 sub-folders:
- **fpga_spice**: contains existing architecture ready to use.
- **template**: contains template architecture which contain keyword to replace
- **generated**: empty at the beginning, will host rewritten template

## benchmarks
This folder contains benchmarks to implement in the FPGA. it's divided in 3 folders:
- **Blif**: Contains .blif and .act file to use in OpenFPGA. Benchmarks are divided in folder with the same name as the top module
- **Verilog**: Contains Verilog netlist of benchmarks to use in OpenFPGA. Each project is saved in a folder with the same name as the top module.
- **List**: Contains files with a list of benchmarks to run in one flow. More details are available in [fpga_flow tutorial](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/fpga_flow/how2use.md#benchmark-list)

## configs
This folder contains configuration files required by openFPGA flow. They specify path to tools and benchmarks as well as flow utilization mode. More details are available in [fpga_flow tutorial](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/fpga_flow/how2use.md#configuration-file)

## scripts
This folder contains scripts call by OpenFPGA flow. Some of them can be used out of the flow as **pro_blif.pl** and **rewrite_path_in_file.pl** which respectively rewrite a blif file with 3 members on a ".latch" module to let it have 5 and replace a keyword in a file.<br />
Any script provide help if call without argument.

## tech
This folder contains XML files describing the technology used. These files are used during power analysis.
