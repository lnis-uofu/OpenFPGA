# Getting Started with OpenFPGA

[![Build Status](https://travis-ci.org/LNIS-Projects/OpenFPGA.svg?branch=master)](https://travis-ci.org/LNIS-Projects/OpenFPGA)
[![Documentation Status](https://readthedocs.org/projects/openfpga/badge/?version=master)](https://openfpga.readthedocs.io/en/master/?badge=master)

## Introduction

The OpenFPGA framework is the **first open-source FPGA IP generator** supporting highly-customizable homogeneous FPGA architectures. OpenFPGA provides a full set of EDA support for customized FPGAs, including Verilog-to-bitstream generation and self-testing verification testbenches/scripts. OpenFPGA opens the door to democratizing FPGA technology and EDA techniques, with agile prototyping approaches and constantly evolving EDA tools for chip designers and researchers.<br />

## Compilation

Dependancies and help using docker can be found at [**./tutorials/building.md**](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/building.md).

**Compilation steps:**
1. git clone https://github.com/LNIS-Projects/OpenFPGA.git && cd OpenFPGA # *Clone the repository and go into it*
2. mkdir build && cd build # *Create a folder named build in OpenPFGA repository*
3. cmake ..  -DCMAKE_BUILD_TYPE=debug # *Create Makefile in this folder using cmake*
4. make # *Compile the tool and its dependencies*

*We currently implemented OpenFPGA for:*<br />
*1. Ubuntu 16.04*<br />
*2. Red Hat 7.5*<br />
*3. MacOS Mojiva 10.13.4*<br /><br />
*Please note that those were the versions we tested the software for. It might work with earlier versions and other distributions.*

## Documentation
OpenFPGA's [full documentation](https://openfpga.readthedocs.io/en/master/) includes tutorials, descriptions of the design flow, and tool options.

## Tutorial

You can find in the folder [**./tutorials**](https://github.com/LNIS-Projects/OpenFPGA/tree/master/tutorials). This will help you get in touch with the software and test different configurations to see how OpenFPGA reacts to them. 

Through this tutorial users can learn how to use the flow and set the dependancies.<br />
The [tutorial index](https://github.com/LNIS-Projects/OpenFPGA/blob/master/tutorials/tutorial_index.md) will guide you through trainings and explain folder oraganization as well as referencing tips and used keywords.

