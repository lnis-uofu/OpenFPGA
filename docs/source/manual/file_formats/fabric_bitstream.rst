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

  Multiple lines will be included, each of which is organized as <bl_address><wl_address><bits>.
  The size of address line and data input bits are shown as a comment in the bitstream file, which eases the development of bitstream downloader.
  For example 
  
  .. code-block:: verilog

    // Bitstream width (LSB -> MSB): <bl_address 5 bits><wl_address 5 bits><data input 1 bits>

  The first part represents the Bit-Line address.
  The second part represents the Word-Line address.
  The third part represents the configuration bit.
  For example
   
  .. code-block:: xml
     
     <bitline_address><wordline_address><bit_value> 
     <bitline_address><wordline_address><bit_value> 
     ...
     <bitline_address><wordline_address><bit_value> 

  .. note:: When there are multiple configuration regions, each ``<bit_value>`` may consist of multiple bits. For example, ``0110`` represents the bits for 4 configuration regions, where the 4 digits correspond to the bits from region ``0, 1, 2, 3`` respectively.

.. option:: ql_memory_bank using decoders

  Multiple lines will be included, each of which is organized as <bl_address><wl_address><bits>.
  The size of address line and data input bits are shown as a comment in the bitstream file, which eases the development of bitstream downloader.
  For example 
  
  .. code-block:: verilog

    // Bitstream width (LSB -> MSB): <bl_address 5 bits><wl_address 5 bits><data input 1 bits>

  The first part represents the Bit-Line address.
  The second part represents the Word-Line address.
  The third part represents the configuration bit.
  For example
   
  .. code-block:: xml
     
     <bitline_address><wordline_address><bit_value> 
     <bitline_address><wordline_address><bit_value> 
     ...
     <bitline_address><wordline_address><bit_value> 

  .. note:: When there are multiple configuration regions, each ``<bit_value>`` may consist of multiple bits. For example, ``0110`` represents the bits for 4 configuration regions, where the 4 digits correspond to the bits from region ``0, 1, 2, 3`` respectively.

.. option:: ql_memory_bank using flatten BL and WLs

  Multiple lines will be included, each of which is organized as <bl_data><wl_data>.
  The size of data are shown as a comment in the bitstream file, which eases the development of bitstream downloader.
  For example 
  
  .. code-block:: verilog

    // Bitstream width (LSB -> MSB): <Region 1: bl_data 5 bits><Region 2: bl_data 4 bits><Region 1: wl_data 5 bits><Region 2: wl_data 6 bits>

  The first part represents the Bit-Line data from multiple configuration regions.
  The second part represents the Word-Line data from multiple configuration regions.
  For example
   
  .. code-block:: xml
     
     <bitline_data_region1><bitline_data_region2><wordline_data_region1><wordline_data_region2> 
     <bitline_data_region1><bitline_data_region2><wordline_data_region1><wordline_data_region2> 
     ...
     <bitline_data_region1><bitline_data_region2><wordline_data_region1><wordline_data_region2> 

  .. note:: The WL data of region is one-hot.

.. option:: ql_memory_bank using shift registers

  Multiple lines will be included, each of which is organized as <bl_data> or <wl_data>.
  The size of data are shown as a comment in the bitstream file, which eases the development of bitstream downloader.
  For example 
  
  .. code-block:: verilog

    // Bitstream word count: 36
    // Bitstream bl word size: 39
    // Bitstream wl word size: 37
    // Bitstream width (LSB -> MSB): <bl shift register heads 1 bits><wl shift register heads 1 bits>

  The bitstream data are organized by words. Each word consists of two parts, BL data to be loaded to BL shift register chains and WL data to be loaded to WL shift register chains 
  For example
   
  .. code-block:: xml
     
     // Word 0
     // BL Part
     <bitline_shift_register_data@clock_0>  ----
     <bitline_shift_register_data@clock_1>   ^
     <bitline_shift_register_data@clock_1>   |
     ...                                   BL word size
     <bitline_shift_register_data@clock_n-2> |
     <bitline_shift_register_data@clock_n-1> v
     <bitline_shift_register_data@clock_n>  ----
     // Word 0
     // WL Part
     <wordline_shift_register_data@clock_0>  ----
     <wordline_shift_register_data@clock_1>   ^
     <wordline_shift_register_data@clock_1>   |
     ...                                   WL word size
     <wordline_shift_register_data@clock_n-2> |
     <wordline_shift_register_data@clock_n-1> v
     <wordline_shift_register_data@clock_n>  ----
     // Word 1
     // BL Part
     <bitline_shift_register_data@clock_0>  ----
     <bitline_shift_register_data@clock_1>   ^
     <bitline_shift_register_data@clock_1>   |
     ...                                   BL word size
     <bitline_shift_register_data@clock_n-2> |
     <bitline_shift_register_data@clock_n-1> v
     <bitline_shift_register_data@clock_n>  ----
     // Word 1
     // WL Part
     <wordline_shift_register_data@clock_0>  ----
     <wordline_shift_register_data@clock_1>   ^
     <wordline_shift_register_data@clock_1>   |
     ...                                   WL word size
     <wordline_shift_register_data@clock_n-2> |
     <wordline_shift_register_data@clock_n-1> v
     <wordline_shift_register_data@clock_n>  ----
     ... // More words

  .. note:: The BL/WL data may be multi-bit, while each bit corresponds to a configuration region
  .. note:: The WL data of region is one-hot.

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
