# FPGA flow

This tutorial will help the user to understand how to use OpenFPGA flow.<br />
During this tutorial we consider the user start in the OpenFPGA folder and we'll use tips and informations provided in [tutorial index](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/tutorial_index.md#tips-and-informations). Details on how the folder is organized are available [here](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/fpga_flow/folder_organization.md).

## Running fpga_flow.pl

A script example can be found at OPENFPGAPATHKEYWORD/fpga_flow/tuto_fpga_flow.sh.

### Experiment

cd fpga_flow<br />
./tuto_fpga_flow.sh<br />

### Explanation

By running this script we took an architecture description file, generated its netlist, generated a bitstream to implement a benchmark on it and verified this implementation.<br />
When you open this file you can see that 2 scripts are called. The first one is **rewrite_path_in_file.pl** which allow us to make this tutorial generic by generating full path to dependancies.<br />
The second one is **fpga_flow.pl**. This script launch OpenFPGA flow and can be used with a lot of [options](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/fpga_flow/options.md).<br />
There is 3 important things to see:
- All FPGA-Verilog options have been activated
- fpga_flow.pl calls a configuration file through "config_file" variable
- fpga_flow.pl calls a list of benchmark to implement and test through "bench_txt" variable

## Configuration file

In this file paths have to be full path. Relative path could lead to errors.<br />
The file is organized in 3 parts: 
* **dir_path**: provides all the tools and repository path
* **flow_conf**: provides information on how the flow run
* **csv_tags**: *to complete*

While empty the file is as follow:

[dir_path]<br />
script_base = OPENFPGAPATHKEYWORD/fpga_flow/scripts<br />
benchmark_dir = *<Path to the folder containing all sources of the design>*<br />
yosys_path = OPENFPGAPATHKEYWORD/yosys<br />
odin2_path = not_used<br />
cirkit_path = not_used<br />
abc_path = OPENFPGAPATHKEYWORD/abc<br />
abc_mccl_path = OPENFPGAPATHKEYWORD/abc<br />
abc_with_bb_support_path = OPENFPGAPATHKEYWORD/abc<br />
mpack1_path = not_used<br />
m2net_path = not_used<br />
mpack2_path = not_used<br />
vpr_path = OPENFPGAPATHKEYWORD/vpr7_x2p/vpr<br />
rpt_dir = *<wherever you want logs to be saved>*<br />
ace_path = OPENFPGAPATHKEYWORD/ace2<br />

[flow_conf]<br />
flow_type = yosys_vpr *to use verilog input*<br />
vpr_arch = *<wherever the architecture file is saved>*<br />
mpack1_abc_stdlib = DRLC7T_SiNWFET.genlib # Use relative path under ABC folder is OK<br />
m2net_conf = not_used<br />
mpack2_arch = not_used<br />
power_tech_xml = *<wherever the xml tech file is saved>*<br />

[csv_tags]<br />
mpack1_tags = Global mapping efficiency:|efficiency:|occupancy wo buf:|efficiency wo buf:<br />
mpack2_tags = BLE Number:|BLE Fill Rate: <br />
vpr_tags = Netlist clb blocks:|Final critical path:|Total logic delay:|total net delay:|Total routing area:|Total used logic block area:|Total wirelength:|Packing took|Placement took|Routing took|Average net density:|Median net density:|Recommend no. of clock cycles:<br />
vpr_power_tags = PB Types|Routing|Switch Box|Connection Box|Primitives|Interc Structures|lut6|ff<br />

*This example file can be found at OPENFPGAPATHKEYWORD/fpga_flow/configs/tutorial/tuto.conf*

## Benchmark list

The benchmark folder contains 3 sub-folders:
* **Blif**: contains .blif and .act of benchmarks
* **List**: contains all benchmark list files
* **Verilog**: contains Verilog designs

Blif and Verilog folders are organized by folders with the name of projects. **Folder, top module and top module file must share the same name.**<br />
The benchmark list file can contain as many benchmarks as available in the same folder targetted by "benchmark_dir" variable from the configuration file. It's written as:<br />
top_module/*.v,<int_value>; where <int_value> is the number of channel/wire between each block.

*This example file can be found at OPENFPGAPATHKEYWORD/fpga_flow/benchmarks/List/tuto_benchmark.txt*
