# Getting Started with FPGA-SPICE
FPGA-SPICE is an extension on VPR7 release. 
This is the formal release for ICCD 2015 paper: 

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

X. Tang, P. Gaillardon and G. De Micheli, "FPGA-SPICE: A simulation-based power estimation framework for FPGAs," 2015 33rd IEEE International Conference on Computer Design (ICCD), New York, NY, 2015, pp. 696-703.
doi: 10.1109/ICCD.2015.7357183

If you have questions or comments, please e-mail OpenFPGA team:

General questions:

pierre-emmanuel.gaillardon@utah.edu

Technical issues:

xifan.tang@utah.edu



