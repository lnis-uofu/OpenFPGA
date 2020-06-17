Generic Bitstream
~~~~~~~~~~~~~~~~~

Usage
`````

Generic bitstream is a fabric-independent bitstream where configuration bits are organized out-of-order in a database.
This can be regarded as a raw bitstream used for 

  - ``debugging``: Hardware engineers can validate if their configuration memories across the FPGA fabric are assigned to expected values 

  - ``an exchangeable file format for bitstream assembler``: Software engineers can use the raw bitstream to build a bitstream assembler which organize the bitstream in the loadable formate to FPGA chips.

  - ``creation of artificial bitstream``: Test engineers can craft artificial bitstreams to test each element of the FPGA fabric, which is typically not synthesizable by VPR. 

.. warning:: The fabric-independent bitstream cannot be directly loaded to FPGA fabrics

File Format
```````````

OpenFPGA can output the generic bitstream to an XML format, which is easy to debug. As shown in the following XML code, configuration bits are organized block by block, where each block could be a LUT, a routing multiplexer `etc`. Each ``bitstream_block`` includes the follwoing information: 

  - ``hierarchy`` represents the location of this block in FPGA fabric.
    The hierachy includes the full hierarchy of this block

    - ``instance`` denotes the instance name which you can find in the fabric netlists

    - ``level`` denotes the depth of the block in the hierarchy

  - ``input_nets`` represents the net names that are mapped to the inputs of block. Unused inputs will be tagged as ``unmapped`` which is a reserved word of OpenFPGA. 

  - ``output_nets`` represents the net names that are mapped to the outputs of block. Unused outputs will be tagged as ``unmapped`` which is a reserved word OpenFPGA. 

  - ``bitstream`` represents the configuration bits affiliated to this block.

    - ``path_id`` denotes the index of inputs which is propagated to the output. Note that smallest valid index starts from zero. Only routing multiplexers have the path index. Unused routing multiplexer will not have a ``path_id`` of ``-1``, which allows bitstream assembler to freely find the best path in terms of Quality of Results (QoR). A used routing multiplexer should have a zero or positive ``path_id``.

    - ``bit`` denotes a single configuration bit under this block. It contains \

      - ``memory_port`` the memory port name which you can find in the fabric netlists by following the hierarchy.
     
      - ``value`` a binary value which is the configuration bit assigned to the memory port.

.. code-block:: xml

  <!-- Bitstream block of a 4-input Look-Up Table in a Configurable Logic Block (CLB) -->
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

  <!-- More bitstream blocks -->

  <!-- Bitstream block of a 2-input routing multiplexer in a Switch Block (SB) -->
  <bitstream_block index="2462">
  	<hierarchy>
  		<instance level="0" name="fpga_top"/>
  		<instance level="1" name="sb_0__3_"/>
  		<instance level="2" name="mem_right_track_22"/>
  	</hierarchy>
  	<input_nets>
  			SAP_out[1]	SAP_out[1]
  	</input_nets>
  	<output_nets>
  			SAP_out[1]
  	</output_nets>
  	<bitstream path_id="1">
  		<bit memory_port="mem_out[0]" value="0"/>
  		<bit memory_port="mem_out[1]" value="1"/>
  	</bitstream>
  </bitstream_block>

