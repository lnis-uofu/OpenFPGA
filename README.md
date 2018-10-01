# Getting Started with FPGA-SPICE
Clone the [OpenFPGA git repository:](https://github.com/LNIS-Projects/OpenFPGA)

[//todo]: # (change to final repository location)
`git clone https://github.com/LNIS-Projects/OpenFPGA.git `

Go to the `VPR` directory and build the tool:
`cd ./vpr`
`make `

Note: if you are using macOS, the graphical interface might not be usable since it requires the X11 library. In this case, open the Makefile and change the line 10 "ENABLE_GRAPHICS = true" to false.

VPR requires a minimum of one XML file that specifies the architecture of the FPGA, and one BLIF file that specifies the logic circuit to be put on the FPGA. 

The first example we want to run is go.sh in the same folder as we already are.

`./go.sh`


