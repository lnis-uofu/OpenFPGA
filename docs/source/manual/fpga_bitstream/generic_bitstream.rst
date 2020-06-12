Generic Bitstream
~~~~~~~~~~~~~~~~~

Usage
`````

Generic bitstream is a fabric-independent bitstream where configuration bits are organized out-of-order in a database.
This can be regarded as a raw bitstream used for 
  - ``debugging``: Hardware engineers can validate if their configuration memories across the FPGA fabric are assigned to expected values 
  - ``an exchangeable file format for bitstream assembler``: Software engineers can use the raw bitstream to build a bitstream assembler which organize the bitstream in the loadable formate to FPGA chips.
  - ``creation of artificial bitstream``: Test engineers can craft artificial bitstreams to test each element of the FPGA fabric, which is typically not synthesizable by VPR. 

.. note:: The fabric-independent bitstream cannot be directly loaded to FPGA fabrics

File Format
```````````

OpenFPGA can output the generic bitstream to an XML format, which is easy to debug. As shown in the following XML code, configuration bits are organized block by block, where each block could be a LUT, a routing multiplexer `etc`. Each ``bitstream_block`` includes two sets of information: 

  - ``hierarchy`` represents the location of this block in FPGA fabric.

  - ``bitstream`` represents the configuration bits affiliated to this block.

.. code-block:: xml

  <bitstream_block index="0">
      <hierarchy>
          <instance level="0" name="fpga_top"/>
          <instance level="1" name="grid_clb_1_1"/>
          <instance level="2" name="fle_0"/>
          <instance level="3" name="lut4_0"/>
      </hierarchy>
      <bitstream>
          <bit memory_port="mem_out[0]" value="1"/>
          <bit memory_port="mem_out[1]" value="0"/>
          <bit memory_port="mem_out[2]" value="1"/>
          <bit memory_port="mem_out[3]" value="0"/>
          <bit memory_port="mem_out[4]" value="1"/>
          <bit memory_port="mem_out[5]" value="0"/>
          <bit memory_port="mem_out[6]" value="1"/>
          <bit memory_port="mem_out[7]" value="0"/>
          <bit memory_port="mem_out[8]" value="1"/>
          <bit memory_port="mem_out[9]" value="0"/>
          <bit memory_port="mem_out[10]" value="1"/>
          <bit memory_port="mem_out[11]" value="0"/>
          <bit memory_port="mem_out[12]" value="1"/>
          <bit memory_port="mem_out[13]" value="0"/>
          <bit memory_port="mem_out[14]" value="1"/>
          <bit memory_port="mem_out[15]" value="0"/>
      </bitstream>
  </bitstream_block>
