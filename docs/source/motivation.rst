Motivation
==========

The built-in timing and power analysis engines of VPR are based on analytical models [1]. Analytical model-based analysis can promise accuracy only on a limited number of circuit designs for which the model is valid. As the technology advancements create more opportunities on circuit designs and FPGA architectures, the analytical power model require to be updated to follow the new trends. However, without referring to simulation results, the analytical power models cannot prove their accuracy. SPICE simulators have the advantages on generality and accuracy over analytical models. For this reason, SPICE simulation results are often selected to check the accuracy of analytical models. Therefore, there is a strong need for a simulation-based power analysis approach for FPGAs, which can support general circuit designs.

It motivates us to develop FPGA-SPICE, an add-on for the current State-of-Art FPGA architecture exploration tools, VPR [2]. FPGA-SPICE aims at generating SPICE netlists and testbenches for the FPGA architectures supported by VPR. The SPICE netlists and testbenches are generated according to the placement and routing results of VPR. As a result, SPICE simulator can be used to perform accurate delay and power analysis. The SPICE simulation results are useful in three aspects: (1) it can provide accurate power analysis; (2) it helps improving the accuracy of built-in analytical models; and moreover (3) it creates opportunities in developing novel analytical models.

SPICE modeling for FPGA architectures requires a detailed transistor-level modeling for all the circuit elements within the considered FPGA architecture. However, current VPR architectural description language [3] does not offer enough transistor-level parameters to model the most common circuit modules, such as multiplexers and LUTs. Therefore, we develop an extension on the VPR architectural description language in order to model the transistor-level circuit designs.

In this manual, we will introduce how to use FPGA-SPICE to conduct accuracy power analysis. First, we give an overview on the design flow of FPGA-SPICE-based tool suites. Then, we show the command-line options of FPGA-SPICE. Afterwards, we introduce the extension on architectural language and the transistor-level design supports. Finally, we present how to simulate the generated SPICE netlists and testbenches. 

In the appendix, we introduce the hierarchy of the generated SPICE netlists and testbenches, to help you customize the SPICE netlists. We also attach an example of architecture XML file for your interest.

The technical details can be found in our ICCD’15 paper [4].

