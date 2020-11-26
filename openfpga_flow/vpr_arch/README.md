# Naming convention for VPR architecture files
Please reveal the following architecture features in the names to help quickly spot architecture files.

- k<lut\_size>\_<frac><Native>: Look-Up Table (LUT) size of FPGA. If you have fracturable LUTs or multiple LUT circuits, this should be largest input size. 
  * The keyword 'frac' is to specify if fracturable LUT is used or not.
  * The keyword 'Native' is to specify if fracturable LUT design is a native one (without mode switch) or a standard one (with mode switch).
- N<le\_size>: Number of logic elements for a CLB. If you have multiple CLB architectures, this should be largest number.
- tileable: If the routing architecture is tileable or not.
- adder\_chain: If hard adder/carry chain is used inside CLBs
- register\_chain: If shift register chain is used inside CLBs
- scan\_chain: If scan chain testing infrastructure is used inside CLBs
- <wide>\_<frac>\_mem<mem\_size>: If block RAM (BRAM) is used or not. If used, the memory size should be clarified here. The keyword 'wide' is to specify if the BRAM spans more than 1 column. The keyword 'frac' is to specify if the BRAM is fracturable to operate in different modes.
- <wide>\_<frac>\_dsp<dsp\_size>: If Digital Signal Processor (DSP) is used or not. If used, the input size should be clarified here. The keyword 'wide' is to specify if the DSP spans more than 1 column. The keyword 'frac' is to specify if the DSP is fracturable to operate in different modes.
- aib: If the Advanced Interface Bus (AIB) is used in place of some I/Os.
- multi\_io\_capacity: If I/O capacity is different on each side of FPGAs.
- reduced\_io: If I/Os only appear a certain or multiple sides of FPGAs 
- <feature\_size>: The technology node which the delay numbers are extracted from.
- TileOrgz<Type>: How tile is organized. 
  * Top-left (Tl): the pins of a tile are placed on the top side and left side only
  * Top-right (Tr): the pins of a tile are placed on the top side and right side only
  * Bottom-right (Br): the pins of a tile are placed on the bottom side and right side only

Other features are used in naming should be listed here.
