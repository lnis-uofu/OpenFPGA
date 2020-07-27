Fabric-dependent Bitstream
~~~~~~~~~~~~~~~~~~~~~~~~~~

Usage
`````

Fabric-dependent bitstream is design to be loadable to the configuration protocols of FPGAs. 
The bitstream just sets an order to the configuration bits in the database, without duplicating the database.
OpenFPGA framework provides a fabric-dependent bitstream generator which is aligned to our Verilog netlists.
The fabric-dependent bitstream can be found in the pre-configured Verilog testbenches.
The fabric bitsteam can be outputted in different file format in terms of usage.

Plain Text File Format
```````````````````````

This file format is designed to be directly loaded to an FPGA fabric.
It does not include any comments but only bitstream.

The information depends on the type of configuration procotol.

.. option:: vanilla

  A line consisting of ``0``|``1``

.. option:: scan_chain

  A line consisting of ``0``|``1``

.. option:: memory_bank

  Multiple lines will be included, each of which is organized as <address><space><bit>.
  Note that due to the use of Bit-Line and Word-Line decoders, every two lines are paired.
  The first line represents the Bit-Line address and configuration bit.
  The second line represents the Word-Line address and configuration bit.
  For example
   
  .. code_block:: 
     
     <bitline_address> <bit_value> 
     <wordline_address> <bit_value> 
     <bitline_address> <bit_value> 
     <wordline_address> <bit_value> 
     ...
     <bitline_address> <bit_value> 
     <wordline_address> <bit_value> 

.. option:: frame_based 
  Multiple lines will be included, each of which is organized as <address><space><bit>.
  For example
   
  .. code_block:: 
     
     <frame_address> <bit_value> 
     <frame_address> <bit_value> 
     ...
     <frame_address> <bit_value> 

XML File Format
```````````````

This file format is designed to generate testbenches using external tools, e.g., CocoTB.

In principle, the file consist a number of XML node ``<bit>``, each bit contains the following attributes:

- ``id``: The unique id of the configuration bit in the fabric bitstream.

- ``value``: The configuration bit value.

  - ``hierarchy`` represents the location of this block in FPGA fabric.
    The hierachy includes the full hierarchy of this block

    - ``instance`` denotes the instance name which you can find in the fabric netlists

    - ``level`` denotes the depth of the block in the hierarchy

A quick example:

.. code_block:: xml

  <bit id="0" value="1">
    <hierarchy>
      <instance level="0" name="fpga_top"/>
      <instance level="1" name="grid_clb_1__2_"/>
      <instance level="2" name="logical_tile_clb_mode_clb__0"/>
      <instance level="3" name="mem_fle_9_in_5"/>
    </hierarchy>
  </bit>

Other information may depend on the type of configuration procotol.

.. option:: memory_bank

  - ``bl``: Bit line address information 

  - ``wl``: Word line address information 

  A quick example:

  .. code_block:: xml

  <bit id="0" value="1">
    <hierarchy>
      <instance level="0" name="fpga_top"/>
      <instance level="1" name="grid_io_bottom_1__0_"/>
      <instance level="2" name="logical_tile_io_mode_io__0"/>
      <instance level="3" name="logical_tile_io_mode_physical__iopad_0"/>
      <instance level="4" name="iopad_sram_blwl_mem"/>
    </hierarchy>
    <bl address="000000"/>
    <wl address="000000"/>
  </bit>

.. option:: frame_based 

  - ``frame``: frame address information 

  A quick example:

  .. code_block:: xml

  <bit id="0" value="1">
    <hierarchy>
      <instance level="0" name="fpga_top"/>
      <instance level="1" name="grid_io_bottom_1__0_"/>
      <instance level="2" name="logical_tile_io_mode_io__0"/>
      <instance level="3" name="logical_tile_io_mode_physical__iopad_0"/>
      <instance level="4" name="iopad_config_latch_mem"/>
    </hierarchy>
    <frame address="0000000000000000"/>
  </bit>
