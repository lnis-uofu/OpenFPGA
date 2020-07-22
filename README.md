# Getting Started with OpenFPGA <img src="./docs/source/figures/OpenFPGA_logo.png" width="200" align="right">
[![Build Status](https://travis-ci.org/LNIS-Projects/OpenFPGA.svg?branch=master)](https://travis-ci.org/LNIS-Projects/OpenFPGA)
[![Documentation Status](https://readthedocs.org/projects/openfpga/badge/?version=master)](https://openfpga.readthedocs.io/en/master/?badge=master)

## Introduction
The OpenFPGA framework is the **first open-source FPGA IP generator** supporting highly-customizable homogeneous FPGA architectures. OpenFPGA provides a full set of EDA support for customized FPGAs, including Verilog-to-bitstream generation and self-testing verification. OpenFPGA opens the door to democratizing FPGA technology and EDA techniques, with agile prototyping approaches and constantly evolving EDA tools for chip designers and researchers.

## Compilation
Dependencies and help using docker can be found [**here**](./docs/source/tutorials/compile.rst).

**Compilation Steps:**
```bash
# Clone the repository and go inside it
git clone https://github.com/LNIS-Projects/OpenFPGA.git && cd OpenFPGA
mkdir build && cd build            # Create a folder named build in the OpenPFGA repository
cmake ..                           # Create a Makefile in this folder using cmake
make                               # Compile the tool and its dependencies
```
*cmake3.12 is recommended to compile OpenFPGA with GUI*

**Quick Compilation Verification**
To quickly verify the tool is well compiled, user can run the following command from OpenFPGA root repository.
```bash
python3 openfpga_flow/scripts/run_fpga_task.py compilation_verification --debug --show_thread_logs
```

*Python3 and iVerilog v10.1+ are required. GUI will pop-up if enabled during compilation.*


**Supported Operating Systems**
We currently target OpenFPGA for:
 1. Ubuntu 18.04
 2. Red Hat 7.5

*The tool was tested with these operating systems. It might work with earlier versions and other distributions.*

## Documentation
OpenFPGA's [full documentation](https://openfpga.readthedocs.io/en/master/) includes tutorials, descriptions of the design flow, and tool options.

## Tutorials
You can find some tutorials in the [**./tutorials**](./docs/source/tutorials/) folder. This will help you get more familiar with the tool and use OpenFPGA under different configurations. 
