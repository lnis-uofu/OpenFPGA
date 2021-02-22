# Regression tests for OpenFPGA
The regression tests are grouped in category of OpenFPGA tools as well as integrated flows.
The principle is that each OpenFPGA tool should have a set of regression tests.

- compilation\_verfication: a quicktest after compilation

- Basic regression tests should focus on fundamental flow integration, such as

  - Yosys + VPR + OpenFPGA for a Verilog-to-Verification flow-run

- FPGA-Verilog regression tests should focus on testing fabric correctness, such as 

  - VPR + OpenFPGA integration for a BLIF-to-Verification flow-run


- FPGA-Bitstream regression tests should focus on testing bitstream correctness and runtime on large devices and benchmark suites

- FPGA-SDC regression test should focus on SDC file generation and necessary syntax check

- FPGA-SPICE regression test should focus on SPICE netlist generation / compilation and SPICE simulations qwith QoR checks.

- Quicklogic regression test is to ensure working flows for QuickLogic's devices and variants

- Benchmark sweep regression test should focus on testing mainly the bitstream generation for a wide range of benchmark suites

Please keep this README up-to-date on the OpenFPGA tools
