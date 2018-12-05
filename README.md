# Getting Started with FPGA-SPICE

## Introduction

FPGA-SPICE is an extension to VPR. It is an IP Verilog Generator allowing reliable and fast testing of heterogeneous architectures.

## Compilation

The different ways of compiling can be found in the **./compilation** folder. 

We currently implemented it for:

1. Ubuntu 18.04
2. Red Hat 7.5
3. MacOS High Sierra 10.13.4

Please note that those were the versions we tested the software for. It might work with earlier versions and other distributions.

## Examples

You can find in the folder **./examples**. This will help you get in touch with the software and test different configurations to see how FPGA-SPICE reacts to them. 

./example_x.sh allows to launch the script linked to example_x.xml and .blif.

In all the examples, the CLBs are composed of LUTs, FFs and MUXs as a base. 

Example 1 shows a very basic design with only 4 inputs on the LUTs, a FF and a MUX in the CLB (only 1). It implements an inverter and allows the user to see the very core of the .xml file.

Example 2 increases the complexity by having 3x3 CLBs and 4 slices per CLB. The slices provide a feedback to the input structure and input MUXs are added.




