Link circuit modules
--------------------
Each defined circuit model should be linked to an FPGA module defined in the original part of architecture descriptions. It helps FPGA-circuit creating the circuit netlists for logic/routing blocks. Since the original part lacks such support, we create a few XML properties to link to Circuit models.

SRAM
====

To link the defined circuit model of SRAM into the FPGA architecture description, a new line in XML format should be added under the XML node device. The new XML node is named as sram, which defines the area of an SRAM and the name of the circuit model to be linked. An example is shown as follows:

.. code-block:: xml

  <sram area=”int” circuit_model_name=”string”>

  <sram>
    <spice organization="string" circuit_model_name="scff"/>
    <verilog organization="string" circuit_model_name="scff"/>
  </sram>

* **area:** is expressed in terms of the number of minimum width transistors. The SRAM area defined in this line is used in the area estimation of global routing multiplexers. circuit_model_name should match the name of the circuit model that has been defined under XML node module_circuit_model. The type of the linked circuit model should be sram.

* **organization:** [scan-chain|memory_bank|standalone], is the type of configuration circuits.

:numref:`fig_sram` illustrates an example where a memory organization using memory decoders and 6-transistor SRAMs.

.. _fig_sram:

.. figure:: figures/sram.png
   :scale: 100%
   :alt: map to buried treasure
 
   Example of a memory organization using memory decoders 

.. note:: Currently circuit only supports standalone memory organization.

.. note:: Currently RRAM-based FPGA only supports memory-bank organization for Verilog Generator.

Here is an example.

.. code-block:: xml

  <sram area=”4” circuit_model_name=”sram6T”>


Switch Boxes
=============

Original VPR architecture description contains an XML node called switchlist under which all the multiplexers of switch blocks are described.
To link a defined circuit model to a multiplexer in the switch blocks, a new XML property circuit_model_name should be added to the descriptions.

Here is an example:

.. code-block:: xml

  <switchlist>
    <switch type=”mux” name=”string” R=”float” Cin=”float” Cout=”float” Tdel=”float” mux_trans_size=”float” buf_size=”float” circuit_model_name=”string”/>
  </switchlist>

* **circuit_model_name:** should match a circuit model whose type is mux defined under module_circuit_models.


Connection Blocks
==================

To link the defined circuit model of the multiplexer to the Connection Blocks, a circuit_model_name should be added to the definition of Connection Blocks switches.  However, the original architecture descriptions do not offer a switch description for connection boxes as they do for the switch blocks.
Therefore, FPGA-circuit requires a new XML node called **cblock** under the root XML node architecture, where a switch for connection blocks can be defined.

Here is the example:

.. code-block:: xml

  <cblock>
    <switch type=”mux” name=”string” R=”float” Cin=”float” Cout=”float” Tdel=”float” mux_trans_size=”float” buf_size=”float” circuit_model_name=”string”/>
  </cblock>

* **circuit_model_name:** should match a circuit model whose type is mux defined under module_circuit_models.

Channel Wire Segments
=====================

Similar to the Switch Boxes and Connection Blocks, the channel wire segments in the original architecture descriptions can be adapted to provide a link to the defined circuit model.

.. code-block:: xml

  <segmentlist>
    <segment freq=”float” length=”int” type=”string” Rmetal=”float” Cmetal=”float” circuit_model_name=”string”/>
  </segmentlist>

* circuit_model_name: should match a circuit model whose type is chan_wire defined under module_circuit_models.

Primitive Blocks inside Multi-mode Configurable Logic Blocks
=============================================================

The architecture description employs a hierarchy of ``pb_types`` to depict the sub-modules and complex interconnections inside logic blocks. Each leaf node and interconnection in the pb_type hierarchy should be linked to a circuit model.
Each primitive block, i.e., the leaf ``pb_types``, should be linked to a valid circuit model, using the XML syntax ``circuit_model_name``.
The ``circuit_model_name`` should match the given name of a ``circuit_model`` defined by users.

.. code-block:: xml
  
  <!-- Multi-mode BLE -->
  <pb_type name="ble" num_pb="10" physical_mode_name="ble_phy"/>
    <!-- Physical implementation of BLE shown in Fig. :ref:`` --> 
    <mode name="ble_phy" disabled_in_packing="true"/>
      <!-- Define a 6-input LUT in BLE and link it to circuit model -->
      <pb_type name="flut6_phy" circuit_model_name="frac_lut6">
        <input name="in" num_pins="6"/>
        <output name="lut4_out" num_pins="4"/>
        <output name="lut5_out" num_pins="2"/>
        <output name="lut6_out" num_pins="1"/>
      </pb_type>
      <pb_type name="lut4_phy" circuit_model_name="lut4">
        <input name="in" num_pins="4"/>
        <output name="out" num_pins="1"/>
      </pb_type>
      <pb_type name="adder_phy" num_pb="2" circuit_model_name="adder">
        <input name="a" num_pins="1"/>
        <input name="b" num_pins="1"/>
        <input name="cin" num_pins="1"/>
        <output name="cout" num_pins="1"/>
        <output name="sumout" num_pins="1"/>
      </pb_type>
      <pb_type name="ff_phy" num_pb="2" circuit_model_name="dff">
        <input name="D" num_pins="1"/>
        <output name="Q" num_pins="1"/>
        <clock name="clk" num_pins="1"/>
      </pb_type>
      <interconnect>
      <!-- Routing multiplexers are omitted in this example. -->
      </interconnect>
    </mode>
    <!-- Arithmetic mode of BLE shown in Fig. 2(b)-->
    <mode name="flut4_arithmetic"/>
      <pb_type name="flut4_arith" num_pb="4"/>
        <!-- Define a virtual 4-input LUT in BLE and link it to physical 6-input LUT defined at LINE 6 -->
        <pb_type name="lut4" mode_bits="01" physical_pb_type_name="flut6_phy">
          <!-- Define an input port and link it to its physical port defined at LINE 7 -->
          <input name="in" num_pins="4" physical_mode_pin="in[3:0]"/>
          <!-- Define an output port and link it to its physical port defined at LINE 8 -->
          <output name="out" num_pins="1" physical_mode_pin="lut4_out"/>
        </pb_type>
        <pb_type name="adder" num_pb="2" physical_pb_type_name="adder_phy">
          <input name="a" num_pins="1" physical_mode_pin="a"/>
          <input name="b" num_pins="1" physical_mode_pin="b"/>
          <input name="cin" num_pins="1" physical_mode_pin="cin"/>
          <output name="cout" num_pins="1" physical_mode_pin="cout"/>
          <output name="sumout" num_pins="1" physical_mode_pin="sumout"/>
        </pb_type>
        <pb_type name="ff" num_pb="2" physical_pb_type_name="ff_phy">
          <input name="D" num_pins="1" physical_mode_pin="D"/>
          <output name="Q" num_pins="1" physical_mode_pin="Q"/>
          <clock name="clk" num_pins="1" physical_mode_pin="clk"/>
        </pb_type>
        <interconnect>
        <!-- Routing multiplexers are omitted in this example. Full details can be found in [21] -->
        </interconnect>
      </pb_type>
    </mode>
  <!-- More operating modes can be defined -->
  </pb_type>

* **physical_mode_name:** tell the name of the mode that describes the physical implementation of the configurable block. This is critical in modeling actual circuit designs and architecture of an FPGA. Typically, only one physical_mode should be specified for each multi-mode ``pb_type``.

* **idle_mode_name:** tell the name of the mode that the ``pb_type`` is configured to be by default. This is critical in building circuit netlists for unused logic blocks.

* **circuit_model_name:** should match a circuit model defined under ``module_circuit_models``. The ``circuit_model_name`` is mandatory for every leaf ``pb_type`` in a physical_mode ``pb_type``. For the interconnection type direct, the type of the linked circuit model should be wire. For multiplexers, the type of linked circuit model should be ``mux``. For complete, the type of the linked circuit model can be either ``mux`` or ``wire``, depending on the case.

* **mode_bits** specifies the configuration bits for the ``circuit_model`` when operating at an operating mode. The length of ``mode_bits`` should match the ``port`` size defined in ``circuit_model``. The ``mode_bits`` should be derived from circuit designs while users are responsible for its correctness. FPGA-Bitstreamm will add the ``mode_bits`` during bitstream generation.

* **physical_pb_type_name** creates the link on ``pb_type`` between operating and physical modes. This syntax is mandatory for every leaf ``pb_type`` in an operating mode ``pb_type``. It should be a valid name of leaf ``pb_type`` in physical mode.   

* **physical_pb_type_index_factor** aims to align the indices for ``pb_type`` between operating and physical modes, especially when an operating mode contains multiple ``pb_type`` (``num_pb``>1) that are linked to the same physical ``pb_type``. When ``physical_pb_type_name`` is larger than 1, the  index of ``pb_type`` will be multipled by the given factor. 

* **physical_pb_type_index_offset** aims to align the indices for ``pb_type`` between operating and physical modes, especially when an operating mode contains multiple ``pb_type`` (``num_pb``>1) that are linked to the same physical ``pb_type``. When ``physical_pb_type_name`` is larger than 1, the  index of ``pb_type`` will be shifted by the given factor. 

* **physical_mode_pin** creates the linke on ``port`` of ``pb_type`` between operating and physical modes. This syntax is mandatory for every leaf ``pb_type`` in an operating mode ``pb_type``. It should be a valid ``port`` name of leaf ``pb_type`` in physical mode and the port size should also match. 

* **physical_mode_pin_rotate_offset** aims to align the pin indices for ``port`` of ``pb_type`` between operating and physical modes, especially when an operating mode contains multiple ``pb_type`` (``num_pb``>1) that are linked to the same physical ``pb_type``. When ``physical_mode_pin_rotate_offset`` is larger than zero, the pin index of ``pb_type`` (whose index is large than 1) will be shifted by the given offset. 

.. note::
  It is highly recommended that only one physical mode is defined for a multi-mode configurable block. Try not to use nested physical mode definition. This will ease the debugging and lead to clean XML description. 

.. note::
  Be careful in using ``physical_pb_type_index_factor``, ``physical_pb_type_index_offset`` and ``physical_mode_pin_rotate_offset``! Try to avoid using them unless for highly complex configuration blocks with very deep hierarchy. 


