# Naming convention for OpenFPGA architecture files
Please reveal the following architecture features in the names to help quickly spot architecture files.
Note that an OpenFPGA architecture can be applied to multiple VPR architecture files.

- k<lut\_size>: Look-Up Table (LUT) size of FPGA. If you have fracturable LUTs or multiple LUT circuits, this should be largest input size.
- frac: If fracturable LUT is used or not.
- N<le\_size>: Number of logic elements for a CLB. If you have multiple CLB architectures, this should be largest number.
- adder\_chain: If hard adder/carry chain is used inside CLBs
- register\_chain: If shift register chain is used inside CLBs
- scan\_chain: If scan chain testing infrastructure is used inside CLBs
- mem<mem\_size>: If block RAM (BRAM) is used or not. If used, the memory size should be clarified here. The keyword wide is to specify if the BRAM spanns more than 1 column.
- aib: If the Advanced Interface Bus (AIB) is used in place of some I/Os.
- <bank\|cc\|frame\|standalone>: specify the type of configuration protocol used in the architecture.
  - `bank` refers to the memory bank
  - `cc` refers to the configuration chain
  - `frame` refers to the frame-based organization
  - `standalone` referes to the vanilla organization
- fixed\_sim: fixed clock frequencies in simulation settings. If auto clock frequencies are used, there is no need to appear in the naming
- intermediate buffer: If intermediate buffers are used in LUT designs.
- behavioral: If behavioral Verilog modeling is specified
- local\_encoder: If local encoders are used in routing multiplexer design
- spyio/spypad: If spy I/Os are used
- stdcell: If circuit designs are built with standard cells only
- tree\_mux: If routing multiplexers are built with a tree-like structure
- <feature_size>: The technology node which the delay numbers are extracted from.

Other features are used in naming should be listed here.
