# Getting Started with OpenFPGA

[![Build Status](https://travis-ci.org/LNIS-Projects/OpenFPGA.svg?branch=master)](https://travis-ci.org/LNIS-Projects/OpenFPGA)
[![Documentation Status](https://readthedocs.org/projects/openfpga/badge/?version=master)](https://openfpga.readthedocs.io/en/master/?badge=master)

## Introduction

OpenFPGA is an extension to VPR. It is an IP Verilog Generator allowing reliable and fast testing of homogeneous architectures.

## Compilation

The different ways of compiling can be found in the **./compilation** folder.<br />
Dependancies and help using docker can be found at [**./tutorials/building.md**](https://github.com/LNIS-Projects/OpenFPGA/blob/documentation/tutorials/building.md).

**Compilation steps:**
1. Clone the repository (git clone https://github.com/LNIS-Projects/OpenFPGA.git && cd OpenFPGA)
2. Create a folder named build in OpenPFGA repository (mkdir build && cd build)
3. Create Makefile in this folder using cmake (cmake ..  -DCMAKE_BUILD_TYPE=debug)
4. Compile the tool and its dependencies (make)

*We currently implemented OpenFPGA for:*

*1. Ubuntu 18.04*
*2. Red Hat 7.5*
*3. MacOS High Sierra 10.13.4*

*Please note that those were the versions we tested the software for. It might work with earlier versions and other distributions.*

## Documentation
OpenFPGA's [full documentation](https://openfpga.readthedocs.io/en/master/) includes tutorials, descriptions of the design flow, and tool options.

## Tutorial

You can find in the folder **./tutorials**. This will help you get in touch with the software and test different configurations to see how OpenFPGA reacts to them. 

Through this tutorial users can learn how to use the flow and set the dependancies.



