#Tutorial

This tutorial purpose it to clarify how to use:
1. The full flow using fpga_flow.pl script
2. Architecture customization

Some keywords will be used during this tutorial:
- OPENFPGAPATHKEYWORD: it refer to OpenFPGA full path

##1. FPGA flow

OpenFPGA repository is organized as follow:
  - abc: open source synthesys tool
  - ace2: abc extension generating .act files
  - vpr7_x2p: source of modified vpr
  - yosys: opensource synthesys tool

fpga_flow.pl is saved in OPENFPGAPATHKEYWORD/fpga_flow/scripts. If we look in this folder, we can find some other scripts as:
  - pro_blif.pl: rewrite a blif which has only 3 members in a .latch module
  - rewrite_path_in_file.pl: target a keyword in a file and replace it

*Any script provides a help if launch without argument*

fpga_flow.pl has dependencies which need to be configured. They are:
  - configuration file, which provides dependencies path and flow type
  - benchmark list file

###Configuration file

In this file paths have to be full path. Relative path could lead to errors.
The file is organized in 3 parts: 
  - dir_path: provides all the tools and repository path
  - flow_conf: provides information on how the flow run
  - csv_tags: *to complete*

While empty the file is as follow:

[dir_path]
script_base = OPENFPGAPATHKEYWORD/fpga_flow/scripts
benchmark_dir = *Path to the folder containing all sources of the design*
yosys_path = OPENFPGAPATHKEYWORD/yosys
odin2_path = not_used
cirkit_path = not_used
abc_path = OPENFPGAPATHKEYWORD/abc
abc_mccl_path = OPENFPGAPATHKEYWORD/abc
abc_with_bb_support_path = OPENFPGAPATHKEYWORD/abc
mpack1_path = not_used
m2net_path = not_used
mpack2_path = not_used
vpr_path = OPENFPGAPATHKEYWORD/vpr7_x2p/vpr
rpt_dir = *wherever tou want logs to be saved*
ace_path = OPENFPGAPATHKEYWORD/ace2

[flow_conf]
flow_type = yosys_vpr *to use verilog input*
vpr_arch = *wherever the architecture file is saved*
mpack1_abc_stdlib = DRLC7T_SiNWFET.genlib # Use relative path under ABC folder is OK
m2net_conf = not_used
mpack2_arch = not_used
power_tech_xml = *wherever the xml tech file is saved*

[csv_tags] *to complete*
mpack1_tags = Global mapping efficiency:|efficiency:|occupancy wo buf:|efficiency wo buf:
mpack2_tags = BLE Number:|BLE Fill Rate: 
vpr_tags = Netlist clb blocks:|Final critical path:|Total logic delay:|total net delay:|Total routing area:|Total used logic block area:|Total wirelength:|Packing took|Placement took|Routing took|Average net density:|Median net density:|Recommend no. of clock cycles:
vpr_power_tags = PB Types|Routing|Switch Box|Connection Box|Primitives|Interc Structures|lut6|ff
