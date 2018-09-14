EDA flow
========

As illustrated in :numref:`fig_eda_flow` , FPGA-SPICE creates a modified VTR flow. All the input files for VPR do not need modifications except the architecture description XML. As simulation-based power analysis needs the transistor-level netlists, we extend the architecture description language to support transistor-level modeling (See details in Section V). FPGA-SPICE, embedded in VPR, outputs the SPICE netlists and testbenches according to placement and routing results, when enabled by command-line options. (See Section IV for details about command-line options.) Besides automatically generating all the SPICE netlists, FPGA-SPICE supports user-defined SPICE netlists for modules. We believe the support on user-defined SPICE netlists allows FPGA-SPICE to be general enough to support novel circuit designs and even technologies. (See Section VII for guidelines in customize your FPGA-SPICE compatible SPICE netlists.) With the dumped SPICE netlists and testbenches, a SPICE simulator, i.e. HSPICE, can be called to conduct power analysis. FPGA-SPICE automatically generates a shell script, which brings convenience for users to run all the simulations (See Section VIII).

.. _fig_eda_flow:

.. figure:: figures/eda_flow.png
   :scale: 50%
   :alt: map to buried treasure

   Detailed EDA flows based on FPGA-SPICE/Verilog/Bitstream in the purpose of : (a) prototyping and area analysis (b) power analysis; and (c) functionality verificaiton.


How to compile
==============
Running the Makefile in the released package can compile all the source codes. 
The released package includes a version of VPR with FPGA-SPICE support, ABC with black box support and Activity Estimator.
