.. _openfpga_verilog_commands:

FPGA-Verilog
------------

write_fabric_verilog
~~~~~~~~~~~~~~~~~~~~

  Write the Verilog netlist for FPGA fabric based on module graph

  - ``--file`` or ``-f`` Specify the output directory for the Verilog netlists

  - ``--explicit_port_mapping`` Use explicit port mapping when writing the Verilog netlists

  - ``--include_timing`` Output timing information to Verilog netlists for primitive modules
 
  - ``--include_signal_init`` Output signal initialization to Verilog netlists for primitive modules

  - ``--support_icarus_simulator`` Output Verilog netlists with syntax that iVerilog simulatorcan accept

  - ``--print_user_defined_template`` Output a template Verilog netlist for all the user-defined ``circuit models`` in :ref:`circuit_library`. This aims to help engineers to check what is the port sequence required by top-level Verilog netlists

  - ``--verbose`` Show verbose log

write_verilog_testbench
~~~~~~~~~~~~~~~~~~~~~~~
 
  Write the Verilog testbench for FPGA fabric

  - ``--file`` or ``-f`` The output directory for all the testbench netlists. We suggest the use of same output directory as fabric Verilog netlists

  - ``--reference_benchmark_file_path`` Must specify the reference benchmark Verilog file if you want to output any testbenches

  - ``--fast_configuration`` Enable fast configuration phase for the top-level testbench in order to reduce runtime of simulations. It is applicable to configuration chain, memory bank and frame-based configuration protocols. For configuration chain, when enabled, the zeros at the head of the bitstream will be skipped. For memory bank and frame-based, when enabled, all the zero configuration bits will be skipped. So ensure that your memory cells can be correctly reset to zero with a reset signal. 

  - ``--print_top_testbench`` Enable top-level testbench which is a full verification including programming circuit and core logic of FPGA

  - ``--print_formal_verification_top_netlist`` Generate a top-level module which can be used in formal verification

  - ``--print_preconfig_top_testbench`` Enable pre-configured top-level testbench which is a fast verification skipping programming phase

  - ``--print_simulation_ini`` Output an exchangeable simulation ini file, which is needed only when you need to interface different HDL simulators using openfpga flow-run scripts

  - ``--explicit_port_mapping`` Use explicit port mapping when writing the Verilog netlists
