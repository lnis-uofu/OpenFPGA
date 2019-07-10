# fpga_flow folder organization

The fpga_flow folder is organized as follow:
* [**arch**]: contains architectures description files
* [**benchmarks**]: contains Verilog and blif benchmarks + lists
* [**configs**]: contains configuration files to run fpga_flow.pl
* [**scripts**]: contains all the scripts required to run the flow
* [**tech**]: contains xml tech files for power estimation

## arch
In this folder are saved the architecture files. These files are Hardware description for the FPGA written in XML. This folder contains 3 sub-folders:
- **fpga_spice**: contains existing architecture ready to use.
- **template**: contains template architecture which contain keyword to replace
- **generated**: empty at the beginning, will host rewritten template

## benchmarks
This folder contains benchmarks to implement in the FPGA.

## configs

## scripts

## tech
