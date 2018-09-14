Link circuit modules
====================
Each defined SPICE model should be linked to a FPGA module defined in the original part of architecture descriptions. It helps FPGA-SPICE creating the SPICE netlists for logic/routing blocks. Since the original part lacks such support, we create a few XML properties to link to SPICE models.

SRAM
====

To link the defined SPICE model of SRAM into the FPGA architecture description, a new line in XML format should be added under the XML node device. The new XML node is named as sram, which defines the area of a SRAM and the name of SPICE model to be linked. And example is shown as follows:

.. code-block:: xml

  <sram area=”int” spice_model_name=”string”>

  <sram>
    <spice organization="string" spice_model_name="scff"/>
    <verilog organization="string" spice_model_name="scff"/>
  </sram>

* area is expressed in terms of the number of minimum width transistors. The SRAM area defined in this line is used in the area estimation of global routing multiplexers. spice_model_name should match the name of SPICE model that have been defined under XML node module_spice_model. The type of the linked SPICE model should be sram.

* organization: the type of configuration circuits. available options: [scan-chain|memory_bank|standalone]. :numref:`fig_sram` illustrates an exmample where a memory organiation using memory decoders and 6-transistor SRAMs.

.. _fig_sram:

.. figure:: figures/sram.png
   :scale: 100%
   :alt: map to buried treasure
 
   Example of a memory organization using memory decoders 

.. note:: Currently SPICE only supports standalone memory organization.

.. note:: Currently RRAM-based FPGA only supports memory-bank organization for Verilog Generator.

Here is an example.

.. code-block:: xml

  <sram area=”4” spice_model_name=”sram6T”>


Switch Blocks
=============

Original VPR architecture description contains a XML node called switchlist under which all the multiplexers of switch blocks are described.
To link a defined SPICE model to a multiplexer in the switch blocks, a new XML property spice_model_name should added to the descriptions.

Here is an example:

.. code-block:: xml

  <switchlist>
    <switch type=”mux” name=”string” R=”float” Cin=”float” Cout=”float” Tdel=”float” mux_trans_size=”float” buf_size=”float” spice_model_name=”string”/>
  </switchlist>

* spice_model_name: should match a SPICE model whose type is mux defined under module_spice_models.


Connection Boxes
================

To link the defined SPICE model of multiplexer to the Connection Boxes, a spice_model_name should be added to the definition of Connection Boxes switches.  However, the original architecture descriptions do not offer a switch description for connection boxes as they do for the switch blocks.
Therefore, FPGA-SPICE requires a new XML node called cblock under the root XML node architecture, where a switch for connection boxes can be defined.

Here is the example:

.. code-block:: xml

  <cblock>
    <switch type=”mux” name=”string” R=”float” Cin=”float” Cout=”float” Tdel=”float” mux_trans_size=”float” buf_size=”float” spice_model_name=”string”/>
  </cblock>

* spice_model_name: should match a SPICE model whose type is mux defined under module_spice_models.

Channel Wire Segments
=====================

Simliar to the SB and CB, the channel wire segments in the original architecture descriptions can be adapted to provide a link to the defined SPICE model.

.. code-block:: xml

  <segmentlist>
    <segment freq=”float” length=”int” type=”string” Rmetal=”float” Cmetal=”float” spice_model_name=”string”/>
  </segmentlist>

* spice_model_name: should match a SPICE model whose type is chan_wire defined under module_spice_models.

Primitive Blocks inside Configurable Logic Blocks
=================================================

The architecture description employs a hierarchy of pb_types to depicting the sub modules and complex interconnections inside logic blocks. Each leaf node and interconnection in pb_type hierarchy should be linked to a SPICE model.

.. code-block:: xml

  <pb_type name="clb" idle_mode_name="ble" physical_mode_name="ble">
    <pb_type name="ble">
      <pb_type name="lut" spice_model_name="lut6">
      <pb_type name="ff" spice_model_name="dff">
      <interconnect>
        <mux input="lut.out ff.Q" output="ble.out" spice_model_name="mux_1level">
      </interconnect>
    </pb_type>
    <interconnect>
    <mux input="ble.out clb.in" output="ble.in" spice_model_name="mux_2level">
    </interconnect>
  </pb_type>

* spice_model_name: should match a SPICE model defined under module_spice_models. For the interconnection type direct, the type of the linked SPICE model should be wire. For mux, the type of the linked SPICE model should be mux. For complete, the type of the linked SPICE model can be either mux or wire, depending on the case.

* idle_mode_name: tell the name of the mode that the pb_type is configured to be by default. This is critical in building SPICE netlists for unused logic blocks.

* physical_mode_name: tell the name of the mode that describes the physical implementation of the block. This is critical in modeling actual circuit designs and architecture of a FPGA.


