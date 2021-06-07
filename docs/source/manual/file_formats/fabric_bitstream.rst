.. _file_formats_fabric_bitstream:

Fabric-dependent Bitstream
--------------------------

.. _file_formats_fabric_bitstream_plain_text:

Plain text (.bit)
~~~~~~~~~~~~~~~~~

This file format is designed to be directly loaded to an FPGA fabric.
It does not include any comments but only bitstream.

The information depends on the type of configuration procotol.

.. option:: vanilla

  A line consisting of ``0`` | ``1``

.. option:: scan_chain

  Multiple lines consisting of ``0`` | ``1``

  For example, a bitstream for 1 configuration regions:

  .. code-block:: xml 

     0
     1
     0
     0

  For example, a bitstream for 4 configuration regions:

  .. code-block:: xml 

     0000
     1010
     0110
     0120

  .. note:: When there are multiple configuration regions, each line may consist of multiple bits. For example, ``0110`` represents the bits for 4 configuration regions, where the 4 digits correspond to the bits from region ``0, 1, 2, 3`` respectively.

.. option:: memory_bank

  Multiple lines will be included, each of which is organized as <address><space><bits>.
  Note that due to the use of Bit-Line and Word-Line decoders, every two lines are paired.
  The first line represents the Bit-Line address and configuration bit.
  The second line represents the Word-Line address and configuration bit.
  For example
   
  .. code-block:: xml
     
     <bitline_address> <bit_value> 
     <wordline_address> <bit_value> 
     <bitline_address> <bit_value> 
     <wordline_address> <bit_value> 
     ...
     <bitline_address> <bit_value> 
     <wordline_address> <bit_value> 

  .. note:: When there are multiple configuration regions, each ``<bit_value>`` may consist of multiple bits. For example, ``0110`` represents the bits for 4 configuration regions, where the 4 digits correspond to the bits from region ``0, 1, 2, 3`` respectively.

.. option:: frame_based 

  Multiple lines will be included, each of which is organized as ``<address><data_input_bits>``.
  The size of address line and data input bits are shown as a comment in the bitstream file, which eases the development of bitstream downloader.
  For example 
  
  .. code-block:: verilog

    // Bitstream width (LSB -> MSB): <address 14 bits><data input 1 bits>

  Note that the address may include don't care bit which is denoted as ``x``.

  .. note:: OpenFPGA automatically convert don't care bit to logic ``0`` when generating testbenches.

  For example
   
  .. code-block:: xml 
     
     <frame_address><bit_value> 
     <frame_address><bit_value> 
     ...
     <frame_address><bit_value> 

  .. note:: When there are multiple configuration regions, each ``<bit_value>`` may consist of multiple bits. For example, ``0110`` represents the bits for 4 configuration regions, where the 4 digits correspond to the bits from region ``0, 1, 2, 3`` respectively.

.. _file_formats_fabric_bitstream_xml:

XML (.xml)
~~~~~~~~~~

This file format is designed to generate testbenches using external tools, e.g., CocoTB.

In principle, the file consist a number of XML node ``<region>``, each region has a unique id, and contains a number of XML nodes ``<bit>``.

- ``id``: The unique id of a configuration region in the fabric bitstream.

A quick example:

.. code-block:: xml

  <region id="0">
    <bit id="0" value="1" path="fpga_top.grid_clb_1__2_.logical_tile_clb_mode_clb__0.mem_fle_9_in_5.mem_out[0]"/>
    </bit>
  </region>


Each XML node ``<bit>`` contains the following attributes:

- ``id``: The unique id of the configuration bit in the fabric bitstream.

- ``value``: The configuration bit value.

- ``path`` represents the location of this block in FPGA fabric, i.e., the full path in the hierarchy of FPGA fabric.

A quick example:

.. code-block:: xml

  <bit id="0" value="1" path="fpga_top.grid_clb_1__2_.logical_tile_clb_mode_clb__0.mem_fle_9_in_5.mem_out[0]"/>
  </bit>

Other information may depend on the type of configuration procotol.

.. option:: memory_bank

  - ``bl``: Bit line address information 

  - ``wl``: Word line address information 

  A quick example:

  .. code-block:: xml

    <bit id="0" value="1" path="fpga_top.grid_clb_1__2_.logical_tile_clb_mode_clb__0.mem_fle_9_in_5.mem_out[0]"/>
      <bl address="000000"/>
      <wl address="000000"/>
    </bit>

.. option:: frame_based 

  - ``frame``: frame address information 

  .. note:: Frame address may include don't care bit which is denoted as ``x``.

  A quick example:

  .. code-block:: xml

    <bit id="0" value="1" path="fpga_top.grid_clb_1__2_.logical_tile_clb_mode_clb__0.mem_fle_9_in_5.mem_out[0]"/>
      <frame address="0001000x00000x01"/>
    </bit>
