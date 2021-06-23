.. _file_format_io_mapping_file:

I/O Mapping File (.xml)
-----------------------

The I/O mapping file aims to show 

- What nets have been mapped to each I/O
- What is the directionality of each mapped I/O 

An example of design constraints is shown as follows.

.. code-block:: xml

  <io_mapping>
      <io name="gfpga_pad_GPIO_PAD[6:6]" net="a" dir="input"/>
      <io name="gfpga_pad_GPIO_PAD[1:1]" net="b" dir="input"/>
      <io name="gfpga_pad_GPIO_PAD[9:9]" net="out_c" dir="output"/>
  </io_mapping>

.. option:: name="<string>"

  The pin name of the FPGA fabric which has been mapped, which should be a valid pin defined in OpenFPGA architecture description.

  .. note:: You should be find the exact pin in the top-level module of FPGA fabric if you output the Verilog netlists.

.. option:: net="<string>"

  The net name which is actually mapped to a pin, which should be consistent with net definition in your ``.blif`` file.

.. option:: dir="<string>"

  The direction of an I/O, which can be either ``input`` or ``output``.
