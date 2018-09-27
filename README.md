# Getting Started with FPGA-SPICE
Clone the [OpenFPGA git repository:](https://github.com/LNIS-Projects/OpenFPGA)

[//todo]: # (change to final repository location)
`git clone https://github.com/LNIS-Projects/OpenFPGA.git `

There is a submodule in the repository so move to the OpenFPGA directory and clone that too:

`cd OpenFPGA`

`git submodule init`

`git submodule update`

Go to the `VPR` directory and build the tool:
`cd ./vpr7_x2p/vpr/`
`make `

Note: if you are using macOS, the graphical interface might not be usable since it requires the X11 library. In this case, open the Makefile and change the line 10 "ENABLE_GRAPHICS = true" to false.

VPR requires a minimum of one XML file that specifies the architecture of the FPGA, and one BLIF file that specifies the logic circuit to be put on the FPGA. 

[//todo]: # (make sure the circuits are available)
While in the `vpr` directory, run the tool on some example files:
`./vpr ../libarchfpga/arch/sample_arch.xml ./Circuits/s298_K6_N10_ace.blif `

If a graphic environment is available, this will bring up a display of how the circuit is being placed on the FPGA. Press the `Proceed` button to step to the final placement, press `Proceed` again to step to the routing. Press the `Exit` button to exit the display.

To run VPR without the display, use the command `-nodisp`
`./vpr ../libarchfpga/arch/sample_arch.xml ./Circuits/s298_K6_N10_ace.blif  -nodisp`

VPR creates a number of files in the same location as the BLIF file when it is run. The `place` file shows how the circuit was placed on the FPGA; the `route` file shows how the logic was routed on the FPGA; the `net` file shows the wiring.

Now that we know that VPR is working, we can move on to FPGA-SPICE. 

The first example we want to run is go.sh in the same folder as we already are.

`./go.sh`

By calling this script (if not modified), we call FPGA-SPICE on an architecture built on TSMC 40 nm node in typical conditions. By modifying the script, we can do different corners at the same time.

This script creates the folder verily_test containing the bitstream and other informations on the circuit we implemented. 