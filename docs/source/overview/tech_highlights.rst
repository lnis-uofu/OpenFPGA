Technical Highlights
--------------------

The follow lists of technical features are created to help users spot their needs in customizing FPGA fabrics.(**as of February 2021**)

Supported Circuit Designs
~~~~~~~~~~~~~~~~~~~~~~~~~

+-----------------+--------------+-----------+-----------------------------------------------------+
| | Circuit Types | | Auto-      | | User-   | | Design Topologies                                 |
| |               | | generation | | Defined |                                                     |
+=================+==============+===========+=====================================================+
| Inverter        |     Yes      |   Yes     | - :ref:`circuit_model_power_gated_inverter_example` |
|                 |              |           | - :ref:`circuit_model_inverter_1x_example`          |
|                 |              |           | - :ref:`circuit_model_tapered_inv_16x_example`      |
+-----------------+--------------+-----------+-----------------------------------------------------+
| Buffer          |     Yes      |   Yes     | - :ref:`circuit_model_buffer_2x_example`            |
|                 |              |           | - :ref:`circuit_model_power_gated_buffer_example`   |
|                 |              |           | - :ref:`circuit_model_tapered_buffer_64x_example`   |
+-----------------+--------------+-----------+-----------------------------------------------------+
| AND gate        |     Yes      |   Yes     | - :ref:`circuit_model_and2_example`                 |
+-----------------+--------------+-----------+-----------------------------------------------------+
| OR gate         |     Yes      |   Yes     | - :ref:`circuit_model_or2_example`                  |
+-----------------+--------------+-----------+-----------------------------------------------------+
| MUX2 gate       |     Yes      |   Yes     | - :ref:`circuit_model_mux2_gate_example`            |
+-----------------+--------------+-----------+-----------------------------------------------------+
| Pass gate       |     Yes      |   Yes     | - :ref:`circuit_model_tgate_example`                |
|                 |              |           | - :ref:`circuit_model_pass_transistor_example`      |
+-----------------+--------------+-----------+-----------------------------------------------------+
| Look-Up Table   |     Yes      |   Yes     | - **Any size**                                      |
|                 |              |           | - :ref:`circuit_model_single_output_lut_example`    |
|                 |              |           | - :ref:`circuit_model_frac_lut_example`             |
|                 |              |           | - :ref:`circuit_model_lut_harden_logic_example`     |
+-----------------+--------------+-----------+-----------------------------------------------------+
| | Routing       |     Yes      |   No      | - **Any size**                                      |
| | Multiplexer   |              |           | - :ref:`circuit_model_mux_multilevel_example`       |
|                 |              |           | - :ref:`circuit_model_mux_1level_example`           |
|                 |              |           | - :ref:`circuit_model_mux_tree_example`             |
|                 |              |           | - :ref:`circuit_model_mux_stdcell_example`          |
|                 |              |           | - :ref:`circuit_model_mux_local_encoder_example`    |
|                 |              |           | - :ref:`circuit_model_mux_const_input_example`      |
+-----------------+--------------+-----------+-----------------------------------------------------+
| | Configurable  |     No       | Yes       | - :ref:`circuit_model_config_latch_example`         | 
| | Memory        |              |           | - :ref:`circuit_model_sram_blwl_example`            |
|                 |              |           | - :ref:`circuit_model_ccff_example`                 | 
|                 |              |           | - :ref:`circuit_model_ccff_enable_example`          | 
|                 |              |           | - :ref:`circuit_model_ccff_scanable_example`        | 
+-----------------+--------------+-----------+-----------------------------------------------------+
| Data Memory     | No           | Yes       | - **Any size**                                      |
|                 |              |           | - :ref:`circuit_model_dff_example`                  | 
|                 |              |           | - :ref:`circuit_model_multi_mode_ff_example`        | 
|                 |              |           | - Single-port Block RAM                             |
|                 |              |           | - :ref:`circuit_model_single_mode_dpram_example`    |
|                 |              |           | - :ref:`circuit_model_multi_mode_dpram_example`     |
+-----------------+--------------+-----------+-----------------------------------------------------+
| | Arithmetic    | No           | Yes       | - **Any size**                                      |
| | Units         |              |           | - Multiplier                                        |
|                 |              |           | - :ref:`circuit_model_full_adder_example`           |
+-----------------+--------------+-----------+-----------------------------------------------------+
| I/O             | No           | Yes       | - :ref:`circuit_model_gpio_example`                 |
|                 |              |           | - Bi-directional buffer                             |
|                 |              |           | - AIB                                               |
+-----------------+--------------+-----------+-----------------------------------------------------+


* The user defined netlist could come from a standard cell

Supported FPGA Architectures
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We support most FPGA architectures that VPR can support!
The following are most commonly seen architectural features:

+------------------------+----------------------------------------------+
| Block Type             | Architecture features                        |
+========================+==============================================+
| Programmable Block     | - Single-mode Configurable Logic Block (CLB) |
|                        | - Multi-mode Configurable Logic Block (CLB)  |
|                        | - Single-mode heterogeneous blocks           |
|                        | - Multi-mode heterogeneous blocks            |
|                        | - Flexible local routing architecture        |
+------------------------+----------------------------------------------+
| Routing Block          | - Tileable routing architecture              |
|                        | - Flexible connectivity                      |
|                        | - Flexible Switch Block Patterns             |
+------------------------+----------------------------------------------+
|                        | - Chain-based organization                   |
|                        | - Frame-based organization                   |
| :ref:`config_protocol` | - Memory bank organization                   |
|                        | - Flatten organization                       |
+------------------------+----------------------------------------------+

Supported Verilog Modeling
~~~~~~~~~~~~~~~~~~~~~~~~~~

OpenFPGA supports the following Verilog features in auto-generated netlists for circuit designs

- Synthesizable Behavioral Verilog

- Structural Verilog

- Implicit/Explicit port mapping

