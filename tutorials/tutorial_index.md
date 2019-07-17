# Tutorial Introduction
OpenFPGA is an IP Verilog Generator allowing reliable and fast testing of homogeneous FPGA architectures.<br />
Its main goal is to easily and efficiently generated a complete customizable FPGA and uses a semi-custom design flow.<br /><br />
In order to help you get in touch with the software, we provide few tutorials which are organized as follow:
* [Building the tool and his dependencies](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/building.md)
* [Launching the flow and understand how it works](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/fpga_flow/how2use.md)
* Architecture modification

## Folder Organization
OpenFPGA repository is organized as follow:
* **abc**: open source synthesys tool
* **ace2**: abc extension generating .act files
* **ace2**: abc extension generating activity files (.act)
* **vpr7_x2p**: sources of modified vpr
* **yosys**: opensource synthesys tool
* **fpga_flow**: scripts and dependencies to run the complete flow

## Tips and Information
Some keywords will be used during in the tutorials:
* OPENFPGAPATHKEYWORD: refers to OpenFPGA folder full path
