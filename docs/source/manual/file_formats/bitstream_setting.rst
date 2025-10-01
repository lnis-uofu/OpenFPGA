.. _file_formats_bitstream_setting:

Bitstream Setting (.xml)
------------------------

An example of bitstream settings is shown as follows.
This can define a hard-coded bitstream for a reconfigurable resource in FPGA fabrics.

.. warning:: Bitstream setting is a feature for power-users. It may cause wrong bitstream to be generated. For example, the hard-coded bitstream is not compatible with LUTs whose nets may be swapped during routing stage (cause a change on the truth table as well as bitstream). It is users's responsibility to ensure correct bitstream.

.. code-block:: xml

  <openfpga_bitstream_setting>
    <pb_type name="<string>" source="eblif" content=".param LUT" is_mode_select_bistream="true" bitstream_offset="1"/>
    <default_mode_bits name="<string>" mode_bits="<string>"/>
    <interconnect name="<string>" default_path="<string>"/>
    <clock_routing network="<string>" pin="<string>"/>
    <non_fabric name="<string>" file="<string>">
      <pb name="<string>" type="<string>" content="<string>"/>
    </non_fabric>
    <overwrite_bitstream>
      <bit value="<0 or 1>" path="<string>"/>
    </overwrite_bitstream>
  </openfpga_bitstream_setting>

pb_type-related Settings
^^^^^^^^^^^^^^^^^^^^^^^^

The following syntax are applicable to the XML definition tagged by ``pb_type`` in bitstream setting files.

.. option:: name="<string>"

  The ``pb_type`` name to be constrained, which should be the full path of a ``pb_type`` consistent with VPR's architecture description. For example, 

  .. code-block:: xml

    pb_type="clb.fle[arithmetic].soft_adder.adder_lut4"

.. option:: source="<string>"

  The source of the ``pb_type`` bitstream, which could be from a ``.eblif`` file. For example, 

  .. code-block:: xml

    source="eblif"

.. option:: content="<string>"

  The content of the ``pb_type`` bitstream, which could be a keyword in a ``.eblif`` file. For example, ``content=".attr LUT"`` means that the bitstream will be extracted from the ``.attr LUT`` line which is defined under the ``.blif model`` (that is defined under the ``pb_type`` in VPR architecture file).

.. option:: is_mode_select_bitstream="<bool>"

  Can be either ``true`` or ``false``. When set ``true``, the bitstream is considered as mode-selection bitstream, which may overwrite ``mode_bits`` definition in ``pb_type_annotation`` of OpenFPGA architecture description. (See details in :ref:`annotate_vpr_arch_pb_type_annotation`)

.. option:: bitstream_offset="<int>"

  Specify the offset to be applied when overloading the bitstream to a target. For example, a LUT may have a 16-bit bitstream. When ``offset=1``, bitstream overloading will skip the first bit and start from the second bit of the 16-bit bitstream.


.. _file_formats_bitstream_setting_mode_bit:

Default Mode Bits-related Settings
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following syntax are applicable to the XML definition tagged by ``default_mode_bits`` in bitstream setting files.

.. option:: name="<string>"

  The ``pb_type`` name to be constrained, which should be the full path of a ``pb_type`` consistent with VPR's architecture description. For example, 

  .. note:: This must be a valid primitive pb_type (one has zero leaf nodes)!

  .. code-block:: xml

    pb_type="clb.fle[arithmetic].soft_adder.adder_lut4"

.. option:: mode_bits="<string>"

  The default mode bits when the ``pb_type`` is not mapped. Note that the size of mode bits must comply with the definition in the OpenFPGA architecture description (See details in :ref:`annotate_vpr_arch_pb_type_annotation`). For example, 

  .. note:: Bitstream setting has a higher priority than the ``mode_bits`` definition in the OpenFPGA architecture description!

  .. note:: Mode bits are default in big-endian format!

  .. note:: For operating modes, mode bits in binary format can contain dont care bit ``x``. This allows operating mode to overwrite only part of the mode bits . Dont care bits will be replaced by the deterministic bit ``1`` or ``0`` by mode bits from other operating modes. If the dont care bit remains after all the operating modes are applied, it will be corrected by the mode bits from physical mode.

  .. warning:: Hex format does not support any dont care bits!

  In binary format with a few available options: 

  .. code-block:: xml

    <!-- The following are equivalent in functionality -->
    <!-- BIN in big endian defined implicitedly -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="010011"/>
    <!-- BIN in big endian -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="6B'010011"/>
    <!-- BIN in big endian with splitter -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="6B'01_0011"/>
    <!-- BIN in little endian -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="6b'110010"/>
    <!-- BIN in little endian with splitter -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="6b'11_0010"/>

  Binary format can contain dont care bit ``x`` with a few available options: 

  .. code-block:: xml

    <!-- The following are equivalent in functionality -->
    <!-- BIN in big endian defined implicitedly -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="01x0x1"/>
    <!-- BIN in big endian -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="6B'01x0x1"/>
    <!-- BIN in big endian with splitter -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="6B'01_x0x1"/>
    <!-- BIN in little endian -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="6b'1x0x10"/>
    <!-- BIN in little endian with splitter -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="6b'1x_0x10"/>

  In hexadecimal format with a few available options: 

  .. code-block:: xml

    <!-- The following are equivalent in functionality -->
    <!-- HEX in big endian -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="6H'13/>
    <!-- HEX in big endian with splitter -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="6H'1_3/>
    <!-- HEX in little endian -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="6h'32/>
    <!-- HEX in little endian with splitter -->
    <default_mode_bits name="clb.fle[arithmetic].soft_adder.adder_lut4" mode_bits="6h'3_2/>
 


Interconnection-related Settings
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following syntax are applicable to the XML definition tagged by ``interconnect`` in bitstream setting files.

.. option:: name="<string>"

  The ``interconnect`` name to be constrained, which should be the full path of a ``pb_type`` consistent with VPR's architecture description. For example, 
  
  .. code-block:: xml

    pb_type="clb.fle[arithmetic].mux1"

.. option:: default_path="<string>"

  The default path denotes an input name that is consistent with VPR's architecture description. For example, in VPR architecture, there is a mux defined as 

  .. code-block:: xml

    <mux name="mux1" input="iopad.inpad ff.Q" output="io.inpad"/>

  The default path can be either ``iopad.inpad`` or ``ff.Q`` which corresponds to the first input and the second input respectively.

Clock Routing-related Settings
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following syntax are applicable to the XML definition tagged by ``clock_routing`` in bitstream setting files.
This is to force the routing for clock tap multiplexers (green line in :numref:`fig_prog_clock_network_example_2x2`) even when they are not used/mapped. If no specified, only the used clock tap multiplexers will be configured to propagate clock signals.

.. note:: This requires the benchmark has at least 1 global signal. Otherwise, the clock routing will be skipped, and there is no impact from this setting.
 
.. option:: network="<string>"

  The ``network`` name to be constrained, which should be a valid name defined in the clock network file (See details in :ref:`file_formats_clock_network`). For example, 

.. code-block:: xml

  <clock_routing network="clk_tree_2lvl" pin="clk[0:0]"/>
  <clock_routing network="rst_tree_2lvl" pin="rst[1:1]"/>

The network and pin correspond to the clock network name and a valid pin of ``global_port`` in the clock network description.

.. code-block:: xml

  <clock_network name="clk_tree_2lvl" global_port="clk[0:7]"/>
  <clock_network name="rst_tree_2lvl" global_port="rst[0:7]"/>  

.. option:: pin="<string>"

  The pin should be a valid pin of the ``global_port`` that is defined in the clock network description under the selected clock network.


non_fabric-related Settings
^^^^^^^^^^^^^^^^^^^^^^^^^^^

This is special syntax to extract PB defined parameter or attribute and save the data into dedicated JSON file outside of fabric bitstream

The following syntax are applicable to the XML definition tagged by ``non_fabric`` in bitstream setting files.

.. option:: name="<string: pb_type top level name>"

  The ``pb_type`` top level name that the data to be extracted. For example, 

  .. code-block:: xml

    name="bram"

.. option:: file="<string: JSON filepath>"

  The filepath the data is saved to. For example, 

  .. code-block:: xml

    file="bram.json"

.. option:: pb child element name="<string: pb_type child name>"

  Together with ``pb_type`` top level name, that is the source of the ``pb_type`` bitstream

  The final ``pb_type`` name is "<pb_type top level name>" + "<pb_type child name>"

  For example,

  .. code-block:: xml  
    <non_fabric name="bram" file="bram_bitstream.json">
      <pb name=".bram_lr[mem_36K_tdp].mem_36K" content=".param INIT_i"/>
    </non_fabric>

  The final ``pb_type`` name is "bram.bram_lr[mem_36K_tdp].mem_36K"

.. option:: pb child element content="<string>"

  The content of the ``pb_type`` data to be extracted. For example, ``content=".param INIT_i"`` means that the data will be extracted from the ``.param INIT_i`` line defined under the ``.blif model``.

overwrite_bitstream-related Settings
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This is to allow user to set value of a list of bits which is represented using full path in the hierarchy of FPGA fabric

This ``overwrite_bitstream`` settings has the highest priority than loading any external bitstream file

Each bit to overwrite is represented by one ``bit`` child node/tag

The following syntax are applicable to the XML definition tagged by ``bit`` node under ``overwrite_bitstream`` setting.

.. option:: value="<0 or 1>"

  The boolean ``0`` or ``1`` that will be set. For example, 

  .. code-block:: xml

    value="0"
    
.. option:: path="<string>"

  ``path`` represents the location of this block in FPGA fabric, i.e., the full path in the hierarchy of FPGA fabric.

  .. code-block:: xml

    path="fpga_top.grid_clb_1__2_.logical_tile_clb_mode_clb__0.mem_fle_9_in_5[0]"
