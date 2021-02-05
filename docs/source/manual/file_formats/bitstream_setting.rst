.. _file_formats_bitstream_setting:

Bitstream Setting (.xml)
------------------------

An example of bitstream settings is shown as follows.
This can define a hard-coded bitstream for a reconfigurable resource in FPGA fabrics.

.. code-block:: xml

  <openfpga_bitstream_setting>
    <pb_type name="<string>" source="eblif" content=".param LUT"/>
  </openfpga_bitstream_setting>

.. option:: pb_type="<string>"

  The ``pb_type`` name to be constrained, which should be the full path of a ``pb_type`` consistent with VPR's architecture description. For example, ``pb_type="clb.fle[arithmetic].soft_adder.adder_lut4"``

.. option:: source="<string>"

  The source of the ``pb_type`` bitstream, which could be from a ``.eblif`` file. For example, ``source="eblif"``.

.. option:: content="<string>"

  The content of the ``pb_type`` bitstream, which could be a keyword in a ``.eblif`` file. For example, ``content=".attr LUT"`` means that the bitstream will be extracted from the ``.attr LUT`` line which is defined under the ``.blif model`` (that is defined under the ``pb_type`` in VPR architecture file).
  
.. warning:: Bitstream is a feature for power-users. It may cause wrong bitstream to be generated. For example, the hard-coded bitstream is not compatible with LUTs whose nets may be swapped during routing stage (cause a change on the truth table as well as bitstream). It is users's responsibility to ensure correct bitstream.
