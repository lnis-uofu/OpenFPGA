# Getting Started with FPGA-SPICE
Clone the [FPGA-SPICE git repository:](https://github.com/tangxifan/tangxifan-eda-tools)

[//todo]: # (change to final repository location)
`git clone https://github.com/tangxifan/tangxifan-eda-tools.git `

There is a submodule in the repository so move to the OpenFPGA directory and clone that too:

`cd OpenFPGA`

`git submodule init`

`git submodule update`

Go to the `VPR` dirctory and build the tool:
`cd tangxifan-eda-tools/branches/vpr7_rram/vpr/`
`make `

VPR requires a minimum of one XML file that specifies the architecture of the FPGA, and one BLIF file that specifies the logic circuit to be put on the FPGA. 

[//todo]: # (make sure the circuits are available)
While in the `vpr` directory, run the tool on some example files:
`./vpr ../libarchfpga/arch/sample_arch.xml ./Circuits/s298_K6_N10_ace.blif `

If a graphic environment is available, this will bring up a display of how the circuit is being placed on the FPGA. Press the `Proceed` button to step to the final placement, press `Proceed` again to step to the routing. Press the `Exit` button to exit the display.

To run VPR without the display, use the command `-nodisp`
`./vpr ../libarchfpga/arch/sample_arch.xml ./Circuits/s298_K6_N10_ace.blif  -nodisp`

VPR creates a number of files in the same location as the BLIF file when it is run. The `place` file shows how the circuit was placed on the FPGA; the `route` file shows how the logic was routed on the FPGA; the `net` file shows the wiring.


