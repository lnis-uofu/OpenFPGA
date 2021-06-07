.. _file_formats_bitstream_setting:

Bitstream Setting (.xml)
------------------------

An example of bitstream settings is shown as follows.
This can define a hard-coded bitstream for a reconfigurable resource in FPGA fabrics.

.. warning:: Bitstream setting is a feature for power-users. It may cause wrong bitstream to be generated. For example, the hard-coded bitstream is not compatible with LUTs whose nets may be swapped during routing stage (cause a change on the truth table as well as bitstream). It is users's responsibility to ensure correct bitstream.

.. code-block:: xml

  <openfpga_bitstream_setting>
    <pb_type name="<string>" source="eblif" content=".param LUT" is_mode_select_bistream="true" bitstream_offset="1"/>
    <interconnect name="<string>" default_path="<string>"/>
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
