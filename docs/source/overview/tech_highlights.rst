Technical Highlights
--------------------

The follow lists of technical features are created to help users spot their needs in customizing FPGA fabrics.(**as of October 2020**)

Supported Circuit Designs
~~~~~~~~~~~~~~~~~~~~~~~~~

+---------------+-----------------+--------------+-------------------------+
| Circuit Types | Auto-generation | User-Defined | Design Topologies       |
+===============+=================+==============+=========================+
| Inverter      |     Yes         |   Yes        | - Power-gating          |
+---------------+-----------------+--------------+-------------------------+
| Buffer        |     Yes         |   Yes        | - Tapered buffers       |
|               |                 |              | - Power-gating          |
+---------------+-----------------+--------------+-------------------------+
| AND gate      |     Yes         |   Yes        | - 2-input               |
+---------------+-----------------+--------------+-------------------------+
| OR gate       |     Yes         |   Yes        | - 2-input               |
+---------------+-----------------+--------------+-------------------------+
| MUX2 gate     |     Yes         |   Yes        | - 2-input               |
+---------------+-----------------+--------------+-------------------------+
| Pass gate     |     Yes         |   Yes        | - Transmission gate     |
|               |                 |              | - Pass transistor       |
+---------------+-----------------+--------------+-------------------------+
| Look-Up Table |     Yes         |   Yes        | - **Any size**          |
|               |                 |              | - Single-output LUT     |
|               |                 |              | - Fracturable LUT       |
|               |                 |              | - Buffer location       |
+---------------+-----------------+--------------+-------------------------+
| Routing       |     Yes         |   No         | - **Any size**          |
| Multiplexer   |                 |              | - Buffer location       |
|               |                 |              | - One-level structure   |
|               |                 |              | - Treee structure       |
|               |                 |              | - Multi-level structure |
|               |                 |              | - Local encoders        |
|               |                 |              | - Constant inputs       |
+---------------+-----------------+--------------+-------------------------+
| Configurable  |     No          | Yes          | - Latch                 | 
| Memory        |                 |              | - SRAM                  |
|               |                 |              | - D-type flip-flop      | 
+---------------+-----------------+--------------+-------------------------+
| Block RAM     | No              | Yes          | - Single-port           |
|               |                 |              | - Dual-port             |
|               |                 |              | - Fracturable           |
|               |                 |              | - **Any size**          |
+---------------+-----------------+--------------+-------------------------+
| Arithmetic    | No              | Yes          | - **Any size**          |
| Units         |                 |              | - Multiplier            |
|               |                 |              | - Adder                 |
+---------------+-----------------+--------------+-------------------------+
| I/O           | No              | Yes          | - General purpose I/O   |
|               |                 |              | - Bi-directional buffer |
|               |                 |              | - AIB                   |
+---------------+-----------------+--------------+-------------------------+


* The user defined netlist could come from a standard cell

Supported FPGA Architectures
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We support most FPGA architectures that VPR can support!
The following are most commonly seen architectural features:

+--------------------+----------------------------------------------+
| Block Type         | Architecture features                        |
+====================+==============================================+
| Programmable Block | - Single-mode Configurable Logic Block (CLB) |
|                    | - Multi-mode Configurable Logic Block (CLB)  |
|                    | - Single-mode heterogeneous blocks           |
|                    | - Multi-mode heterogeneous blocks            |
|                    | - Flexible local routing architecture        |
+--------------------+----------------------------------------------+
| Routing Block      | - Tileable routing architecture              |
|                    | - Flexible connectivity                      |
|                    | - Flexible Switch Block Patterns             |
+--------------------+----------------------------------------------+

Supported Verilog Modeling
~~~~~~~~~~~~~~~~~~~~~~~~~~

OpenFPGA supports the following Verilog features in auto-generated netlists for circuit designs

- Synthesizable Behavioral Verilog

- Structural Verilog

- Implicit/Explicit port mapping

