Motivation
==========

FPGA-SPICE
----------

The built-in timing and power analysis engines of VPR are based on analytical models :cite:`VBetz_Book_1999,JGoeders_FPT_2012`. Analytical model-based analysis can promise accuracy only on a limited number of circuit designs for which the model is valid. As the technology advancements create more opportunities on circuit designs and FPGA architectures, the analytical power model require to be updated to follow the new trends. However, without referring to simulation results, the analytical power models cannot prove their accuracy. SPICE simulators have the advantages on generality and accuracy over analytical models. For this reason, SPICE simulation results are often selected to check the accuracy of analytical models. Therefore, there is a strong need for a simulation-based power analysis approach for FPGAs, which can support general circuit designs.

It motivates us to develop FPGA-SPICE, an add-on for the current State-of-Art FPGA architecture exploration tools, VPR :cite:`JRose_FPGA_2012`.
FPGA-SPICE aims at generating SPICE netlists and testbenches for the FPGA architectures supported by VPR. The SPICE netlists and testbenches are generated according to the placement and routing results of VPR. As a result, SPICE simulator can be used to perform accurate delay and power analysis. The SPICE simulation results are useful in three aspects: (1) it can provide accurate power analysis; (2) it helps improving the accuracy of built-in analytical models; and moreover (3) it creates opportunities in developing novel analytical models.

SPICE modeling for FPGA architectures requires a detailed transistor-level modeling for all the circuit elements within the considered FPGA architecture. However, current VPR architectural description language :cite:`JLuu_FPGA_2011` does not offer enough transistor-level parameters to model the most common circuit modules, such as multiplexers and LUTs. Therefore, we develop an extension on the VPR architectural description language in order to model the transistor-level circuit designs.

In this manual, we will introduce how to use FPGA-SPICE to conduct accuracy power analysis. First, we give an overview on the design flow of FPGA-SPICE-based tool suites. Then, we show the command-line options of FPGA-SPICE. Afterwards, we introduce the extension on architectural language and the transistor-level design supports. Finally, we present how to simulate the generated SPICE netlists and testbenches. 

In the appendix, we introduce the hierarchy of the generated SPICE netlists and testbenches, to help you customize the SPICE netlists. We also attach an example of architecture XML file for your interest.

The technical details can be found in our ICCD’15 paper :cite:`XTang_ICCD_2015`.

FPGA-Verilog
------------

On a second note, it is becoming more and more necessary to have a fast access to the Verilog code of the structures and architectures researchers want to study. We think that some issues cannot be studies through VPR only and a more complete overview is possible through a more extensive workflow. One of the prerequites for this is the generation of the Verilog which enables Place & Route and Signoff analysis. While VPR enables the researcher to have access to fast results if the characteristics of the system are well known by the user, it is quite limited otherwise. In the same way, it is quite hard to study the same architecture accross multiple technology nodes without strong knowledge of it. 

This motivates us to generate the Verilog code of the architecture to enable a second level of research concerning the architectures to be explored. This Verilog code encompasses the whole design and is divided into multiple sub-directories for targetted analysis or a global one. This is left to the choice of the user. 

In this manual, we present FPGA-Verilog. This extension enables the generation of a fully functional Verilog code enabling deeper understanding of the architectures of the FPGAs. We introduce different options to this module to do the verification of the system. This will be presented in more depth in the FPGA-Bitstream section

FPGA-Bitstream
--------------

In order to have the right functionality on top of the FPGA generated, it is necessary to have a Bitstream generation which programs the FPGA and gets the right functionality on top of it. For this reason, we generate a Bitstream and some testbenches in parallel which allow the user to do some functional verification of the system. This includes three different testbenches. First, the FPGA is configured then the clock runs with random patterns are generated to test the functionality. Secondly, the FPGA can be configured in parallel to the testbench itself to do a comparison of the signals and check the validity. Finally, the configuration can be skipped to directly have access to the functioning of the system and reduce the processing time.

This will be explained in more depth in the FPGA-Bitstream section.
