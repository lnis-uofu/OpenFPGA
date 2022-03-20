# Naming convention for OpenFPGA architecture files
Please reveal the following architecture features in the names to help quickly spot architecture files.
Note that an OpenFPGA architecture can be applied to multiple VPR architecture files.

- k<lut\_size>\_<frac><Native>: Look-Up Table (LUT) size of FPGA. If you have fracturable LUTs or multiple LUT circuits, this should be largest input size.
  * The keyword 'frac' is to specify if fracturable LUT is used or not.
  * The keyword 'Native' is to specify if fracturable LUT design is a native one (without mode switch) or a standard one (with mode switch).
- N<le\_size>: Number of logic elements for a CLB. If you have multiple CLB architectures, this should be largest number.
- fracdff: Use multi-mode DFF model, where reset/set/clock polarity is configurable
- adder\_chain: If hard adder/carry chain is used inside CLBs
- register\_chain: If shift register chain is used inside CLBs
- scan\_chain: If scan chain testing infrastructure is used inside CLBs
- <wide>\_<frac>\_dsp<dsp\_size>reg: If Digital Signal Processor (DSP) is used or not. If used, the input size should be clarified here.
  - The keyword 'wide' is to specify if the DSP spans more than 1 column. 
  - The keyword 'frac' is to specify if the DSP is fracturable to operate in different modes.
  - The keyword 'reg' is to specify if the DSP has input and output registers. If only input or output registers are used, the keyword will be 'regin' or 'regout' respectively.
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
- registerable\_io: If I/Os are registerable (can be either combinational or sequential)
- stdcell: If circuit designs are built with standard cells only
- tree\_mux: If routing multiplexers are built with a tree-like structure
- <feature_size>: The technology node which the delay numbers are extracted from.
- powergate : The FPGA has power-gating techniques applied. If not defined, there is no power-gating.
- GlobalTile<Int>Clk<Pin>: How many clocks are defined through global ports from physical tiles.
  * <Int> is the number of clocks 
  * <Pin> When specified, multiple clocks are in separated pins with different names

Other features are used in naming should be listed here.
