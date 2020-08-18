.. _annotate_vpr_arch:

Bind circuit modules to VPR architecture 
----------------------------------------
Each defined circuit model should be linked to an FPGA module defined in the original part of architecture descriptions. It helps FPGA-circuit creating the circuit netlists for logic/routing blocks. Since the original part lacks such support, we create a few XML properties to link to Circuit models.

Switch Blocks
~~~~~~~~~~~~~

Original VPR architecture description contains an XML node called switchlist under which all the multiplexers of switch blocks are described.
To link a defined circuit model to a multiplexer in the switch blocks, a new XML property circuit_model_name should be added to the descriptions.

Here is an example:

.. code-block:: xml

  <switch_block>
    <switch type="mux" name="<string>" circuit_model_name="<string>"/>
  </switch_block>

- ``circuit_model_name="<string>"`` should match a circuit model whose type is ``mux`` defined in :ref:`circuit_library`.


Connection Blocks
~~~~~~~~~~~~~~~~~

To link the defined circuit model of the multiplexer to the Connection Blocks, a ``circuit_model_name`` should be annotated to the definition of Connection Blocks switches.  

Here is the example:

.. code-block:: xml

  <connection_block>
    <switch type="ipin_cblock" name="<string>" circuit_model_name="<string>"/>
  </connection_block>

- ``circuit_model_name="<string>"`` should match a circuit model whose type is ``mux`` defined in :ref:`circuit_library`.

Channel Wire Segments
~~~~~~~~~~~~~~~~~~~~~

Similar to the Switch Boxes and Connection Blocks, the channel wire segments in the original architecture descriptions can be adapted to provide a link to the defined circuit model.

.. code-block:: xml

  <segmentlist>
    <segment name="<string>" circuit_model_name="<string>"/>
  </segmentlist>

- ``circuit_model_name="<string>"`` should match a circuit model whose type is ``chan_wire`` defined in :ref:`circuit_library`.

Primitive Blocks inside Multi-mode Configurable Logic Blocks
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The architecture description employs a hierarchy of ``pb_types`` to depict the sub-modules and complex interconnections inside logic blocks. Each leaf node and interconnection in the pb_type hierarchy should be linked to a circuit model.
Each primitive block, i.e., the leaf ``pb_types``, should be linked to a valid circuit model, using the XML syntax ``circuit_model_name``.
The ``circuit_model_name`` should match the given name of a ``circuit_model`` defined by users.

.. code-block:: xml

  <pb_type_annotations>
    <!-- physical pb_type binding in complex block IO -->
    <pb_type name="io" physical_mode_name="physical"/>
    <pb_type name="io[physical].iopad" circuit_model_name="iopad" mode_bits="1"/> 
    <pb_type name="io[inpad].inpad" physical_pb_type_name="io[physical].iopad" mode_bits="1"/> 
    <pb_type name="io[outpad].outpad" physical_pb_type_name="io[physical].iopad" mode_bits="0"/> 
    <!-- End physical pb_type binding in complex block IO -->

    <!-- physical pb_type binding in complex block CLB -->
    <!-- physical mode will be the default mode if not specified -->
    <pb_type name="clb">
      <!-- Binding interconnect to circuit models as their physical implementation, if not defined, we use the default model -->
      <interconnect name="crossbar" circuit_model_name="mux_2level"/>
    </pb_type>
    <pb_type name="clb.fle" physical_mode_name="physical"/>
    <pb_type name="clb.fle[physical].fabric.frac_logic.frac_lut6" circuit_model_name="frac_lut6" mode_bits="0"/>
    <pb_type name="clb.fle[physical].fabric.ff" circuit_model_name="static_dff"/>
    <!-- Binding operating pb_type to physical pb_type -->
    <pb_type name="clb.fle[n2_lut5].lut5inter.ble5.lut5" physical_pb_type_name="clb.fle[physical].fabric.frac_logic.frac_lut6" mode_bits="1" physical_pb_type_index_factor="0.5">
      <!-- Binding the lut5 to the first 5 inputs of fracturable lut6 -->
      <port name="in" physical_mode_port="in[0:4]"/>
      <port name="out" physical_mode_port="lut5_out" physical_mode_pin_rotate_offset="1"/>
    </pb_type>
    <pb_type name="clb.fle[n2_lut5].lut5inter.ble5.ff" physical_pb_type_name="clb.fle[physical].fabric.ff"/>
    <pb_type name="clb.fle[n1_lut6].ble6.lut6" physical_pb_type_name="clb.fle[physical].fabric.frac_logic.frac_lut6" mode_bits="0">
      <!-- Binding the lut6 to the first 6 inputs of fracturable lut6 -->
      <port name="in" physical_mode_port="in[0:5]"/>
      <port name="out" physical_mode_port="lut6_out"/>
    </pb_type>
    <pb_type name="clb.fle[n1_lut6].ble6.ff" physical_pb_type_name="clb.fle[physical].fabric.ff" physical_pb_type_index_factor="2" physical_pb_type_index_offset="0"/>
    <!-- End physical pb_type binding in complex block IO -->
  </pb_type_annotations>
  
.. option:: <pb_type name="<string>" physical_mode_name="<string>">

  Specify a physical mode for multi-mode ``pb_type`` defined in VPR architecture.

  .. note:: This should be applied to non-primitive ``pb_type``, i.e., ``pb_type`` have child ``pb_type``.

  - ``name="<string>"`` specifiy the full name of a ``pb_type`` in the hierarchy of VPR architecture.

  - ``physical_mode_name="<string>"`` Specify the name of the mode that describes the physical implementation of the configurable block. This is critical in modeling actual circuit designs and architecture of an FPGA. Typically, only one ``physical_mode`` should be specified for each multi-mode ``pb_type``.

.. note:: OpenFPGA will infer the physical mode for a single-mode ``pb_type`` defined in VPR architecture

.. option:: <pb_type name="<string>" physical_pb_type_name="<string>" circuit_model_name="<string>" 
  mode_bits="<int>" physical_pb_type_index_factor="<float>" physical_pb_type_index_offset="<int>">

  Specify the physical implementation for a primitive ``pb_type`` in VPR architecture

  .. note:: This should be applied to primitive ``pb_type``, i.e., ``pb_type`` have no children.

  - ``name="<string>"`` specifiy the full name of a ``pb_type`` in the hierarchy of VPR architecture.

  - ``physical_pb_type_name=<string>`` creates the link on ``pb_type`` between operating and physical modes. This syntax is mandatory for every primitive ``pb_type`` in an operating mode ``pb_type``. It should be a valid name of primitive ``pb_type`` in physical mode.   

  - ``circuit_model_name="<string>"`` Specify a circuit model to implement a ``pb_type`` in VPR architecture. The ``circuit_model_name`` is mandatory for every primitive``pb_type`` in a physical_mode ``pb_type``.

  - ``mode_bits="<int>"`` Specify the configuration bits for the ``circuit_model`` when operating at an operating mode. The length of ``mode_bits`` should match the ``port`` size defined in ``circuit_model``. The ``mode_bits`` should be derived from circuit designs while users are responsible for its correctness. FPGA-Bitstreamm will add the ``mode_bits`` during bitstream generation.

  - ``physical_pb_type_index_factor="<float>"`` aims to align the indices for ``pb_type`` between operating and physical modes, especially when an operating mode contains multiple ``pb_type`` (``num_pb``>1) that are linked to the same physical ``pb_type``. When ``physical_pb_type_name`` is larger than 1, the  index of ``pb_type`` will be multipled by the given factor. 

  - ``physical_pb_type_index_offset=<int>`` aims to align the indices for ``pb_type`` between operating and physical modes, especially when an operating mode contains multiple ``pb_type`` (``num_pb``>1) that are linked to the same physical ``pb_type``. When ``physical_pb_type_name`` is larger than 1, the  index of ``pb_type`` will be shifted by the given factor. 

.. option:: <interconnect name="<string>" circuit_model_name="<string>">

  - ``name="<string>"`` specifiy the name of a ``interconnect`` in VPR architecture. Different from ``pb_type``, hierarchical name is not required here.

  - ``circuit_model_name="<string>"`` For the interconnection type direct, the type of the linked circuit model should be wire. For multiplexers, the type of linked circuit model should be ``mux``. For complete, the type of the linked circuit model can be either ``mux`` or ``wire``, depending on the case.

.. option:: <port name="<string>" physical_mode_port="<string>" physical_mode_pin_rotate_offset="<int>"/>

   Link a port of an operating ``pb_type`` to a port of a physical ``pb_type``

  - ``name="<string>"`` specifiy the name of a ``port`` in VPR architecture. Different from ``pb_type``, hierarchical name is not required here.

  - ``physical_mode_pin="<string>" creates the link of ``port`` of ``pb_type`` between operating and physical modes. This syntax is mandatory for every primitive ``pb_type`` in an operating mode ``pb_type``. It should be a valid ``port`` name of leaf ``pb_type`` in physical mode and the port size should also match. 

    .. note:: Users can define multiple ports. For example: ``physical_mode_pin="a[0:1] b[2:2]"``. When multiple ports are used, the ``physical_mode_pin_rotate_offset`` should also be adapt. For example: ``physical_mode_pin_rotate_offset="1 0"``)

  - ``physical_mode_pin_rotate_offset="<int>"`` aims to align the pin indices for ``port`` of ``pb_type`` between operating and physical modes, especially when an operating mode contains multiple ``pb_type`` (``num_pb``>1) that are linked to the same physical ``pb_type``. When ``physical_mode_pin_rotate_offset`` is larger than zero, the pin index of ``pb_type`` (whose index is large than 1) will be shifted by the given offset. 

    .. note:: If not defined, the default value of ``physical_mode_pin_rotate_offset`` is set to ``0``.

.. note::
  It is highly recommended that only one physical mode is defined for a multi-mode configurable block. Try not to use nested physical mode definition. This will ease the debugging and lead to clean XML description. 

.. note::
  Be careful in using ``physical_pb_type_index_factor``, ``physical_pb_type_index_offset`` and ``physical_mode_pin_rotate_offset``! Try to avoid using them unless for highly complex configuration blocks with very deep hierarchy. 


