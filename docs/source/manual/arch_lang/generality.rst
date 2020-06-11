.. _arch_generality:

General Hierarchy
-----------------

For OpenFPGA using VPR7
~~~~~~~~~~~~~~~~~~~~~~~

The extension of the VPR architectural description language is developed as an independent branch of the original one. Most of the FPGA-SPICE descriptions are located under a XML node called <spice_settings>, which is a child node under the root node <architecture>. 
Under the <spice_settings>, some child node is created for describing SPICE simulation settings, technology library and transistor-level modeling of circuit modules.
In the following sub-sections, we will introduce the structures of these XML nodes and the parameters provided.

For OpenFPGA using VPR8
~~~~~~~~~~~~~~~~~~~~~~~

OpenFPGA uses separated XMLs file other than the VPR8 architecture description file.
This is to keep a loose integration to VPR8 so that OpenFPGA can easily integrate any future version of VPR with least engineering effort.
However, to implement a physical FPGA, OpenFPGA requires the original VPR XML to include full physical design details.
Full syntax can be found in :ref:`addon_vpr_syntax`.

The OpenFPGA requires two XML files: an architecture description file and a simulation setting description file.

OpenFPGA Architecture Description File
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This file contains device-level and circuit-level details as well as annotations to the original VPR architecture.
It contains a root node called ``<openfpga_architecture>`` under which architecture-level information, such as device-level description, circuit-level and architecture annotations to original VPR architecture XML are defined.

It consists of the following code blocks

    - ``<circuit_library>`` includes a number of ``circuit_model``, each of which describe a primitive block in FPGA architecture, such as Look-Up Tables and multiplexers. Full syntax can be found in :ref:`circuit_library`.
    - ``<technology_library>`` includes transistor-level parameters, where users can specify which transistor models are going to be used when building the ``circuit models``.  Full syntax can be found in :ref:`technology_library`.
    - ``<configuration_protocol>`` includes detailed description on the configuration protocols to be used in FPGA fabric. Full syntax can be found in :ref:`config_protocol`.
    - ``<connection_block>`` includes annotation on the connection block definition ``<connection_block>`` in original VPR XML. Full syntax can be found in :ref:`annotate_vpr_arch`.
    - ``<switch_block>`` includes annotation on the switch block definition ``<switchlist>`` in original VPR XML. Full syntax can be found in :ref:`annotate_vpr_arch`.
    - ``<routing_segment>`` includes annotation on the routing segment definition ``<segmentlist>`` in original VPR XML. Full syntax can be found in :ref:`annotate_vpr_arch`.
    - ``<direct_connection>`` includes annotation on the inter-tile direct connection definitioin ``<directlist>`` in original VPR XML. Full syntax can be found in :ref:`direct_interconnect`.
    - ``<pb_type_annotation>`` includes annotation on the programmable block architecture ``<complexblocklist>`` in original VPR XML. Full syntax can be found in :ref:`annotate_vpr_arch`.

.. note:: ``<technology_library>`` will be applied to ``circuit_model`` when running FPGA-SPICE. It will not impact FPGA-Verilog, FPGA-Bitstream, FPGA-SDC.


OpenFPGA Simulation Setting File
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This file contains parameters required by testbench generators.
It contains a root node ``<openfpga_simulation_setting>``, under which all the parameters to be used in generate testbenches in simulation purpose are defined.

It consists of the following code blocks

    - ``<clock_setting>`` defines the clock-related settings in simulation, such as clock frequency and number of clock cycles to be used.
    - ``<simulator_option>`` defines universal options available in both HDL and SPICE simulators. This is mainly used by :ref:`fpga_spice`.
    - ``<monte_carlo>`` defines critical parameters to be used in monte-carlo simulations. This is used by  :ref:`fpga_spice`.
    - ``<measurement_setting>`` defines the parameters used to measure signal slew and delays. This is used by :ref:`fpga_spice`.
    - ``<stimulus>`` defines the parameters used to generate voltage stimuli in testbenches. This is used by :ref:`fpga_spice`.

Full syntax can be found in :ref:`simulation_setting`.

.. note:: the parameters in ``<clock_setting>`` will be applied to both FPGA-Verilog and FPGA-SPICE simulations


