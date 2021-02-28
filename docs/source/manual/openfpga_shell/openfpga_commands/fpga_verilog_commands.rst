.. _openfpga_verilog_commands:

FPGA-Verilog
------------

write_fabric_verilog
~~~~~~~~~~~~~~~~~~~~

  Write the Verilog netlist for FPGA fabric based on module graph

  .. option:: --file <string> or -f <string> 

    Specify the output directory for the Verilog netlists. For example, ``--file /temp/fabric_netlist/``

  .. option:: --default_net_type <string>

    Specify the default net type for the Verilog netlists. Currently, supported types are ``none`` and ``wire``. Default value: ``none``.

  .. option:: --explicit_port_mapping

    Use explicit port mapping when writing the Verilog netlists

  .. option:: --include_timing

    Output timing information to Verilog netlists for primitive modules
 
  .. option:: --include_signal_init

    Output signal initialization to Verilog netlists for primitive modules

  .. option:: --support_icarus_simulator
     
    Output Verilog netlists with syntax that iVerilog simulatorcan accept

  .. option:: --print_user_defined_template

    Output a template Verilog netlist for all the user-defined ``circuit models`` in :ref:`circuit_library`. This aims to help engineers to check what is the port sequence required by top-level Verilog netlists

  .. option:: --verbose

    Show verbose log

write_verilog_testbench
~~~~~~~~~~~~~~~~~~~~~~~
 
  Write the Verilog testbench for FPGA fabric

  .. option:: --file <string> or -f <string>
     
    The output directory for all the testbench netlists. We suggest the use of same output directory as fabric Verilog netlists. For example, ``--file /temp/testbench``

  .. option:: --fabric_netlist_file_path <string>

    Specify the fabric Verilog file if they are not in the same directory as the testbenches to be generated. If not specified, OpenFPGA will assume that the fabric netlists are the in the same directory as testbenches and assign default names. For example, ``--file /temp/fabric/fabric_netlists.v``

  .. option:: --reference_benchmark_file_path <string>

    Must specify the reference benchmark Verilog file if you want to output any testbenches. For example, ``--reference_benchmark_file_path /temp/benchmark/counter_post_synthesis.v``

  .. option:: --pin_constraints_file <string> or -pcf <string>

    Specify the *Pin Constraints File* (PCF) if you want to custom stimulus in testbenches. For example, ``-pin_constraints_file pin_constraints.xml``
    Strongly recommend for multi-clock simulations. See detailed file format about :ref:`file_format_pin_constraints_file`.

  .. option:: --fast_configuration

    Enable fast configuration phase for the top-level testbench in order to reduce runtime of simulations. It is applicable to configuration chain, memory bank and frame-based configuration protocols. For configuration chain, when enabled, the zeros at the head of the bitstream will be skipped. For memory bank and frame-based, when enabled, all the zero configuration bits will be skipped. So ensure that your memory cells can be correctly reset to zero with a reset signal. 

    .. note:: If both reset and set ports are defined in the circuit modeling for programming, OpenFPGA will pick the one that will bring largest benefit in speeding up configuration.

  .. option:: --print_top_testbench

    Enable top-level testbench which is a full verification including programming circuit and core logic of FPGA

  .. option:: --print_formal_verification_top_netlist

    Generate a top-level module which can be used in formal verification

  .. option:: --print_preconfig_top_testbench

    Enable pre-configured top-level testbench which is a fast verification skipping programming phase

  .. option:: --print_simulation_ini <string>

    Output an exchangeable simulation ini file, which is needed only when you need to interface different HDL simulators using openfpga flow-run scripts. For example, ``--print_simulation_ini /temp/testbench/sim.ini``

  .. option:: --explicit_port_mapping

    Use explicit port mapping when writing the Verilog netlists
