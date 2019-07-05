# Getting Started with OpenFPGA

[![Build Status](https://travis-ci.org/LNIS-Projects/OpenFPGA.svg?branch=master)](https://travis-ci.org/LNIS-Projects/OpenFPGA)
[![Documentation Status](https://readthedocs.org/projects/openfpga/badge/?version=master)](https://openfpga.readthedocs.io/en/master/?badge=master)

## Introduction

OpenFPGA is an extension to VPR. It is an IP Verilog Generator allowing reliable and fast testing of homogeneous architectures.

## Compilation

The different ways of compiling can be found in the **./compilation** folder.

**Compilation steps:**
1. Create a folder named build in OpenPFGA repository (mkdir build && cd build)
2. Create Makefile in this folder using cmake (cmake ..)
3. Compile the tool and its dependencies (make)

*We currently implemented OpenFPGA for:*

*1. Ubuntu 18.04*
*2. Red Hat 7.5*
*3. MacOS High Sierra 10.13.4*

*Please note that those were the versions we tested the software for. It might work with earlier versions and other distributions.*

## Documentation
OpenFPGA's [full documentation](https://openfpga.readthedocs.io/en/master/) includes tutorials, descriptions of the design flow, and tool options.

## Examples

You can find in the folder **./examples**. This will help you get in touch with the software and test different configurations to see how FPGA-SPICE reacts to them. 

./example_x.sh allows to launch the script linked to example_x.xml and .blif.

In all the examples, the CLBs are composed of LUTs, FFs and MUXs as a base. 

Example 1 shows a very basic design with only 4 inputs on the LUTs, a FF and a MUX in the CLB (only 1). It implements an inverter and allows the user to see the very core of the .xml file.

Example 2 increases the complexity by having 3x3 CLBs and 4 slices per CLB. The slices provide a feedback to the input structure and input MUXs are added.




