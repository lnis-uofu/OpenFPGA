.. _openfpga_verilog_commands:

FPGA-Verilog
------------

write_fabric_verilog
~~~~~~~~~~~~~~~~~~~~

  Write the Verilog netlist for FPGA fabric based on module graph. See details in :ref:`fabric_netlists`.

  .. option:: --file <string> or -f <string> 

    Specify the output directory for the Verilog netlists. For example, ``--file /temp/fabric_netlist/``

  .. option:: --default_net_type <string>

    Specify the default net type for the Verilog netlists. Currently, supported types are ``none`` and ``wire``. Default value: ``none``.

  .. option:: --explicit_port_mapping

    Use explicit port mapping when writing the Verilog netlists

  .. option:: --include_timing

    Output timing information to Verilog netlists for primitive modules

  .. option:: --use_relative_path

    Force to use relative path in netlists when including other netlists. By default, this is off, which means that netlists use absolute paths when including other netlists
 
  .. option:: --print_user_defined_template

    Output a template Verilog netlist for all the user-defined ``circuit models`` in :ref:`circuit_library`. This aims to help engineers to check what is the port sequence required by top-level Verilog netlists

  .. option:: --no_time_stamp

    Do not print time stamp in Verilog netlists

  .. option:: --verbose

    Show verbose log

write_full_testbench
~~~~~~~~~~~~~~~~~~~~~~~
 
  Write the full testbench for FPGA fabric in Verilog format. See details in :ref:`fpga_verilog_testbench`.

  .. option:: --file <string> or -f <string>
     
    The output directory for all the testbench netlists. We suggest the use of same output directory as fabric Verilog netlists. For example, ``--file /temp/testbench``

  .. option:: --bitstream <string>     

    The bitstream file to be loaded to the full testbench, which should be in the same file format that OpenFPGA can outputs (See detailes in :ref:`file_formats_fabric_bitstream_plain_text`). For example, ``--bitstream and2.bit``

  .. option:: --fabric_netlist_file_path <string>

    Specify the fabric Verilog file if they are not in the same directory as the testbenches to be generated. If not specified, OpenFPGA will assume that the fabric netlists are the in the same directory as testbenches and assign default names. For example, ``--file /temp/fabric/fabric_netlists.v``

  .. option:: --reference_benchmark_file_path <string>

    Specify the reference benchmark Verilog file if you want to output any self-checking testbench. For example, ``--reference_benchmark_file_path /temp/benchmark/counter_post_synthesis.v``
   
    .. note:: If not specified, the testbench will not include any self-checking feature!

  .. option:: --pin_constraints_file <string> or -pcf <string>

    Specify the *Pin Constraints File* (PCF) if you want to custom stimulus in testbenches. For example, ``-pin_constraints_file pin_constraints.xml``
    Strongly recommend for multi-clock simulations. See detailed file format about :ref:`file_format_pin_constraints_file`.

  .. option:: --bus_group_file <string> or -bgf <string>

    Specify the *Bus Group File* (BGF) if you want to group pins to buses. For example, ``-bgf bus_group.xml``
    Strongly recommend when input HDL contains bus ports. See detailed file format about :ref:`file_format_bus_group_file`.

  .. option:: --fast_configuration

    Enable fast configuration phase for the top-level testbench in order to reduce runtime of simulations. It is applicable to configuration chain, memory bank and frame-based configuration protocols. For configuration chain, when enabled, the zeros at the head of the bitstream will be skipped. For memory bank and frame-based, when enabled, all the zero configuration bits will be skipped. So ensure that your memory cells can be correctly reset to zero with a reset signal. 

    .. note:: If both reset and set ports are defined in the circuit modeling for programming, OpenFPGA will pick the one that will bring largest benefit in speeding up configuration.

  .. option:: --explicit_port_mapping

    Use explicit port mapping when writing the Verilog netlists

  .. option:: --default_net_type <string>

    Specify the default net type for the Verilog netlists. Currently, supported types are ``none`` and ``wire``. Default value: ``none``.

  .. option:: --include_signal_init

    Output signal initialization to Verilog testbench to smooth convergence in HDL simulation

    .. note:: We strongly recommend users to turn on this flag as it can help simulators to converge quickly.

   .. warning:: Signal initialization is only applied to the datapath inputs of routing multiplexers (considering the fact that they are indispensible cells of FPGAs)! If your FPGA does not contain any multiplexer cells, signal initialization is not applicable.

  .. option:: --no_time_stamp

    Do not print time stamp in Verilog netlists

  .. option:: --use_relative_path

    Force to use relative path in netlists when including other netlists. By default, this is off, which means that netlists use absolute paths when including other netlists

  .. option:: --verbose

    Show verbose log

write_preconfigured_fabric_wrapper
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
  Write the Verilog wrapper for a preconfigured FPGA fabric. See details in :ref:`fpga_verilog_testbench`.

  .. option:: --file <string> or -f <string>
     
    The output directory for the netlists. We suggest the use of same output directory as fabric Verilog netlists. For example, ``--file /temp/testbench``

  .. option:: --fabric_netlist_file_path <string>

    Specify the fabric Verilog file if they are not in the same directory as the testbenches to be generated. If not specified, OpenFPGA will assume that the fabric netlists are the in the same directory as testbenches and assign default names. For example, ``--file /temp/fabric/fabric_netlists.v``

  .. option:: --pin_constraints_file <string> or -pcf <string>

    Specify the *Pin Constraints File* (PCF) if you want to custom stimulus in testbenches. For example, ``-pin_constraints_file pin_constraints.xml``
    Strongly recommend for multi-clock simulations. See detailed file format about :ref:`file_format_pin_constraints_file`.

  .. option:: --bus_group_file <string> or -bgf <string>

    Specify the *Bus Group File* (BGF) if you want to group pins to buses. For example, ``-bgf bus_group.xml``
    Strongly recommend when input HDL contains bus ports. See detailed file format about :ref:`file_format_bus_group_file`.

  .. option:: --explicit_port_mapping

    Use explicit port mapping when writing the Verilog netlists

  .. option:: --default_net_type <string>

    Specify the default net type for the Verilog netlists. Currently, supported types are ``none`` and ``wire``. Default value: ``none``.

  .. option:: --embed_bitstream <string>
     
    Specify if the bitstream should be embedded to the Verilog netlists in HDL codes. Available options are ``none``, ``iverilog`` and ``modelsim``. Default value: ``modelsim``.

    .. warning:: If the option ``none`` is selected, bitstream will not be embedded. Users should force the bitstream through HDL simulator commands. Otherwise, functionality of the wrapper netlist is wrong!

   .. warning:: Please specify ``iverilog`` if you are using icarus iVerilog simulator.

__ iverilog_website_

.. _iverilog_website: http://iverilog.icarus.com/

  .. option:: --include_signal_init

    Output signal initialization to Verilog testbench to smooth convergence in HDL simulation

    .. note:: We strongly recommend users to turn on this flag as it can help simulators to converge quickly.

   .. warning:: Signal initialization is only applied to the datapath inputs of routing multiplexers (considering the fact that they are indispensible cells of FPGAs)! If your FPGA does not contain any multiplexer cells, signal initialization is not applicable.

  .. option:: --no_time_stamp

    Do not print time stamp in Verilog netlists

  .. option:: --verbose

    Show verbose log

write_mock_fpga_wrapper
~~~~~~~~~~~~~~~~~~~~~~~
 
  Write the Verilog wrapper which mockes a mapped FPGA fabric. See details in :ref:`fpga_verilog_mock_fpga_wrapper`.

  .. option:: --file <string> or -f <string>
     
    The output directory for the netlists. We suggest the use of same output directory as fabric Verilog netlists. For example, ``--file /temp/testbench``

  .. option:: --pin_constraints_file <string> or -pcf <string>

    Specify the *Pin Constraints File* (PCF) if you want to custom stimulus in testbenches. For example, ``-pin_constraints_file pin_constraints.xml``
    Strongly recommend for multi-clock simulations. See detailed file format about :ref:`file_format_pin_constraints_file`.

  .. option:: --bus_group_file <string> or -bgf <string>

    Specify the *Bus Group File* (BGF) if you want to group pins to buses. For example, ``-bgf bus_group.xml``
    Strongly recommend when input HDL contains bus ports. See detailed file format about :ref:`file_format_bus_group_file`.

  .. option:: --explicit_port_mapping

    Use explicit port mapping when writing the Verilog netlists

  .. option:: --use_relative_path

    Force to use relative path in netlists when including other netlists. By default, this is off, which means that netlists use absolute paths when including other netlists

  .. option:: --default_net_type <string>

    Specify the default net type for the Verilog netlists. Currently, supported types are ``none`` and ``wire``. Default value: ``none``.

  .. option:: --no_time_stamp

    Do not print time stamp in Verilog netlists

  .. option:: --verbose

    Show verbose log

write_preconfigured_testbench
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
  Write the Verilog testbench for a preconfigured FPGA fabric. See details in :ref:`fpga_verilog_testbench`.

  .. option:: --file <string> or -f <string>
     
    The output directory for all the testbench netlists. We suggest the use of same output directory as fabric Verilog netlists. For example, ``--file /temp/testbench``

  .. option:: --fabric_netlist_file_path <string>

    Specify the fabric Verilog file if they are not in the same directory as the testbenches to be generated. If not specified, OpenFPGA will assume that the fabric netlists are the in the same directory as testbenches and assign default names. For example, ``--file /temp/fabric/fabric_netlists.v``

  .. option:: --reference_benchmark_file_path <string>

    Specify the reference benchmark Verilog file if you want to output any self-checking testbench. For example, ``--reference_benchmark_file_path /temp/benchmark/counter_post_synthesis.v``

    .. note:: If not specified, the testbench will not include any self-checking feature!

  .. option:: --pin_constraints_file <string> or -pcf <string>

    Specify the *Pin Constraints File* (PCF) if you want to custom stimulus in testbenches. For example, ``-pin_constraints_file pin_constraints.xml``
    Strongly recommend for multi-clock simulations. See detailed file format about :ref:`file_format_pin_constraints_file`.

  .. option:: --bus_group_file <string> or -bgf <string>

    Specify the *Bus Group File* (BGF) if you want to group pins to buses. For example, ``-bgf bus_group.xml``
    Strongly recommend when input HDL contains bus ports. See detailed file format about :ref:`file_format_bus_group_file`.

  .. option:: --explicit_port_mapping

    Use explicit port mapping when writing the Verilog netlists

  .. option:: --default_net_type <string>

    Specify the default net type for the Verilog netlists. Currently, supported types are ``none`` and ``wire``. Default value: ``none``.

  .. option:: --no_time_stamp

    Do not print time stamp in Verilog netlists

  .. option:: --use_relative_path

    Force to use relative path in netlists when including other netlists. By default, this is off, which means that netlists use absolute paths when including other netlists

  .. option:: --verbose

    Show verbose log

write_simulation_task_info
~~~~~~~~~~~~~~~~~~~~~~~~~~

  Write an interchangeable file in ``.ini`` format to interface HDL simulators, such as iVerilog and Modelsim.

  .. option:: --file <string> or -f <string>

    Specify the file path to output simulation-related information. For example, ``--file simulation.ini``

  .. option:: --hdl_dir <string>

    Specify the directory path where HDL netlists are created. For example, ``--hdl_dir ./SRC``

  .. option:: --reference_benchmark_file_path <string>

    Must specify the reference benchmark Verilog file if you want to output any testbenches. For example, ``--reference_benchmark_file_path /temp/benchmark/counter_post_synthesis.v``

  .. option:: --testbench_type <string>

    Specify the type of testbenches [``preconfigured_testbench``|``full_testbench``]. By default, it is the ``preconfigured_testbench``.

  .. option:: --time_unit <string>
      
    Specify a time unit to be used in SDC files. Acceptable values are string: ``as`` | ``fs`` | ``ps`` | ``ns`` | ``us`` | ``ms`` | ``ks`` | ``Ms``. By default, we will consider second (``ms``).

  .. option:: --verbose

    Show verbose log
 
