# FPGA Flow
This tutorial will help the user to understand how to use the OpenFPGA flow.<br />
During this tutorial, we consider that the user starts in the OpenFPGA folder and we will use tips and information provided in [tutorial index](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/tutorial_index.md#tips-and-informations). Details on how the folder is organized are available [here](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/fpga_flow/folder_organization.md).

## Running fpga_flow.pl
A script example can be found at OPENFPGAPATHKEYWORD/fpga_flow/tuto_fpga_flow.sh.

### Experiment
cd fpga_flow<br />
./tuto_fpga_flow.sh<br />

### Explanation
The *fpga_flow.pl* script takes an architecture description file (.xml), generates its netlists and generates a bitstream to implement a benchmark on the FPGA fabric and verifis its correct implementation.<br />
When you open the perl script, you can see that 2 scripts are called. The first one is **rewrite_path_in_file.pl** which allows us to make this tutorial generic by generating full path to the dependencies.<br />
The second one is **fpga_flow.pl**. This script launches the OpenFPGA flow and can be used with many different [options](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/fpga_flow/options.md).<br />
There are 3 important things to observe here:
- All the FPGA-Verilog options have been activated
- fpga_flow.pl calls a configuration file through the "config_file" variable
- fpga_flow.pl calls a list of benchmark to be implemented and tested through the "bench_txt" variable

### Configuration File
In this file, paths have to be defined as **absolute** paths as relative paths could lead to errors.<br />
The file is organized in 3 parts: 
* **dir_path**: provides all the tool and repository paths
* **flow_conf**: provides information on how the flow runs
* **csv_tags**: *to be completed*

When empty, the file is as follow:

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

### Benchmark List
The benchmark folder contains 3 sub-folders:
* **Blif**: contains .blif and .act of benchmarks
* **List**: contains all benchmark list files
* **Verilog**: contains Verilog designs

Blif and Verilog folders are organized by folders using the name of the projects. **The folder, top module and top module file must share the same name.**<br />
The benchmark list file can contain as many benchmarks as available in the same folder targetted by the "benchmark_dir" variable from the configuration file. It's written as:<br />
top_module/*.v,<int_value>; where <int_value> is the number of channel/wire between each block.

*This example file can be found at OPENFPGAPATHKEYWORD/fpga_flow/benchmarks/List/tuto_benchmark.txt*


## Modifying the Flow
Once the dependencies are understood, the flow can be modified by changing the architecture file and the route channel width.

### Experiment
* cd OPENFPGAPATHKEYWORD/fpga_flow/configs/tutorial
* replace the architecture "k6_N10_sram_chain_HC_template.xml" and "k6_N10_sram_chain_HC.xml" respectively with "k8_N10_sram_chain_FC_template.xml" and "k8_N10_sram_chain_FC.xml" in tuto.conf
* cd OPENFPGAPATHKEYWORD/fpga_flow/benchmarks/List
* replace "200" with "300" in tuto_benchmark.txt
* cd OPENFPGAPATHKEYWORD/fpga_flow
* replace the architecture "k6_N10_sram_chain_HC_template.xml" and "k6_N10_sram_chain_HC.xml" respectively with "k8_N10_sram_chain_FC_template.xml" and "k8_N10_sram_chain_FC.xml" in tuto_fpga_flow.sh
* ./tuto_fpga_flow.sh

### Explanations
With this last experiment, the [**K6 architecture**](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/images/architectures_schematics/frac_lut6.pdf) was replaced by a [**K8 architecture**](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/images/architectures_schematics/frac_lut8.pdf), which means that an 8-input fracturable LUT (implemented by LUT6 and LUT4 with 2 shared inputs) is used. This architecture provides more modes for the CLB and the crossbar which is changed from a half-connected to a fully connected, implying bigger multiplexors between the CLB and LUT inputs. These requirements in term of interconnection will lead an increase in the routing channel width. Indeed, if the routing channel is too low, it could be impossible to route a benchmark or the FPGA output could be delayed.
