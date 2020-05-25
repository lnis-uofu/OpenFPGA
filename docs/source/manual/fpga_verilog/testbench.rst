Testbench
---------

In this part, we will introduce the hierarchy, dependency and functionality of each Verilog testbench, which are generated to verify a FPGA fabric implemented with an application.

OpenFPGA can auto-generate two types of Verilog testbenches to validate the correctness of the fabric: full and formal-oriented.
Both testbenches share the same organization, as depicted in :numref:`fig_verilog_testbench_organization` (a).
To enable self-testing, the FPGA and user's RTL design (simulate using an HDL simulator) are driven by the same input stimuli, and any mismatch on their outputs will raise an error flag.

.. _fig_verilog_testbench_organization:

.. figure:: figures/verilog_testbench_organization.png
   :scale: 50%
   :alt: Functional Verification using ModelSim

   Principles of Verilog testbenches organization: (a) block diagram and (b) waveforms. 

Full Testbench
~~~~~~~~~~~~~~
Full testbench aims at simulating an entire FPGA operating period, consisting of two phases: 

  - the **Configuration Phase**, where the synthesized design bitstream is loaded to the programmable fabric, as highlighted by the green rectangle of :numref:`fig_verilog_testbench_organization` (b);

  - the **Operating Phase**, where random input vectors are auto-generated to drive both Devices Under Test (DUTs), as highlighted by the red rectangle of :numref:`fig_verilog_testbench_organization` (b). Using the full testbench, users can validate both the configuration circuits and programming fabric of an FPGA.

Formal-oriented Testbench
~~~~~~~~~~~~~~~~~~~~~~~~~
The formal-oriented testbench aims to test a programmed FPGA is instantiated with the user's bitstream.
The module of the programmed FPGA is encapsulated with the same port mapping as the user's RTL design and thus can be fed to a formal tool for a 100% coverage formal verification. Compared to the full testbench, this skips the time-consuming configuration phase, reducing the simulation time, potentially also significantly accelerating the functional verification, especially for large FPGAs.

.. warning:: Formal-oriented testbenches do not validate the configuration protocol of FPGAs. It is used to  validate FPGA with a wide range of benchmarks.

General Usage
~~~~~~~~~~~~~

All the generated Verilog testbenches are located in the directory as you specify in the OpenFPGA command ``write_fabric_verilog``.
Inside the directory, the Verilog testbenches are organized as illustrated in :numref:`fig_verilog_testbench_hierarchy`.

.. _fig_verilog_testbench_hierarchy:

.. figure:: ./figures/verilog_testbench_hierarchy.png
   :scale: 90%

   Hierarchy of Verilog testbenches for a FPGA fabric implemented with an application

.. note:: ``<bench_name>`` is the module name of users' RTL design. 

.. option:: <bench_name>_include_netlist.v

   This file includes all the related Verilog netlists that are used by the testbenches, including both full and formal oriented testbenches.
   This file is created to simplify the netlist addition for HDL simulator.
   This is the only file you need to add to a simulator.

   .. note:: Fabric Verilog netlists are included in this file.

.. option:: define_simulation.v

   This file includes pre-processing flags required by the testbenches, to smooth HDL simulation.
   It will include the folliwng pre-procesing flags:
  
   - ```define AUTOCHECK_SIMULATION`` When enabled, testbench will include self-testing features. The FPGA and user's RTL design (simulate using an HDL simulator) are driven by the same input stimuli, and any mismatch on their outputs will raise an error flag.

   .. note:: OpenFPGA always enable the self-testing feature. Users can disable it by commenting out the associated line in the ``define_simulation.v``.

   - ```define ENABLE_FORMAL_VERFICATION`` When enabled, the ``<bench_name>_include_netlist.v`` will include the pre-configured FPGA netlist for formal verification usage. This flag is added when ``--print_formal_verification_top_netlist`` option is enabled when calling the ``write_verilog_testbench`` command. 

   - ```define ENABLE_FORMAL_SIMULATION`` When enabled, the ``<bench_name>_include_netlist.v`` will include the testbench netlist for formal-oriented simulation. This flag is added when ``--print_preconfig_top_testbench`` option is enabled when calling the ``write_verilog_testbench`` command. 

   .. note:: To run full testbenches, both flags ``ENABLE_FORMAL_VERIFICATION`` and ``ENABLE_FORMAL_SIMULATION`` must be disabled!

.. option:: <bench_name>_autocheck_top_tb.v

  This is the netlist for full testbench.

.. option:: <bench_name>_formal_random_top_tb.v

  This is the netlist for formal-oriented testbench.

.. option:: <bench_name>_top_formal_verification.v

  This netlist includes a Verilog module of a pre-configured FPGA fabric, which is a wrapper on top of the ``fpga_top.v`` netlist.
  The wrapper module has the same port map as the top-level module of user's RTL design, which be directly def to formal verification tools to validate FPGA's functional equivalence. 
  :numref:`fig_preconfig_module` illustrates the organization of a pre-configured module, which consists of a FPGA fabric (see :ref:`fabric_netlists`) and a hard-coded bitstream.
  Only used I/Os of FPGA fabric will appear in the port list of the pre-configured module. 

.. _fig_preconfig_module:

.. figure:: ./figures/preconfig_module.png
   :scale: 100%

   Internal structure of a pre-configured FPGA module

