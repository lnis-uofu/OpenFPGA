# Tutorial

This tutorial purpose it to clarify how to use:
1. The full flow using fpga_flow.pl script
2. Architecture customization

Some keywords will be used during this tutorial:
* OPENFPGAPATHKEYWORD: refers to OpenFPGA folder full path

### Folder organization

OpenFPGA repository is organized as follow:
* **abc**: open source synthesys tool
* **ace2**: abc extension generating .act files
* **vpr7_x2p**: sources of modified vpr
* **yosys**: opensource synthesys tool
* **fpga_flow**: scripts and dependencies to run the complete flow

## 1. FPGA flow

The folder is organized as follow:
* **arch**: contains architectures description files
* **benchmarks**: contains Verilog and blif benchmarks + lists
* **configs**: contains configuration files to run fpga_flow.pl
* **scripts**: contains all the scripts required to run the flow
* **tech**: contains xml tech files for power estimation

fpga_flow.pl is saved in OPENFPGAPATHKEYWORD/fpga_flow/scripts. If we look in this folder, we can find some other scripts as:
* pro_blif.pl: rewrite a blif which has only 3 members in a .latch module
* rewrite_path_in_file.pl: target a keyword in a file and replace it

*Any script provides a help if launch without argument*

fpga_flow.pl has dependencies which need to be configured. They are:
* configuration file, which provides dependencies path and flow type
* benchmark list file

### Configuration file

In this file paths have to be full path. Relative path could lead to errors.<br />
The file is organized in 3 parts: 
* dir_path: provides all the tools and repository path
* flow_conf: provides information on how the flow run
* csv_tags: *to complete*

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

[csv_tags] *to complete*<br />
mpack1_tags = Global mapping efficiency:|efficiency:|occupancy wo buf:|efficiency wo buf:<br />
mpack2_tags = BLE Number:|BLE Fill Rate: <br />
vpr_tags = Netlist clb blocks:|Final critical path:|Total logic delay:|total net delay:|Total routing area:|Total used logic block area:|Total wirelength:|Packing took|Placement took|Routing took|Average net density:|Median net density:|Recommend no. of clock cycles:<br />
vpr_power_tags = PB Types|Routing|Switch Box|Connection Box|Primitives|Interc Structures|lut6|ff<br />

*An example of this file can be found at OPENFPGAPATHKEYWORD/fpga_flow/configs/tutorial/tuto.conf*

### Benchmark list

The benchmark folder contains 3 sub-folders:
* Blif: contains .blif and .act of benchmarks
* List: contains all benchmark list files
* Verilog: contains Verilog designs

Blif and Verilog folders are organized by folders with name of projects. **Folder, top module and top module file must share the same name.**<br />
The benchmark list file can contain as many benchmark as available in a same folder targetted by "benchmark_dir" variable from the configuration file. It's written as:<br />
top_module/*.v,<int_value>; where <int_value> is the number ofchannel/wire between each blocks.

*An example of this file can be found at OPENFPGAPATHKEYWORD/fpga_flow/benchmarks/List/tuto_benchmark.txt*
