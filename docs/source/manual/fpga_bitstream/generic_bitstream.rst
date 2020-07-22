Generic Bitstream
~~~~~~~~~~~~~~~~~

Usage
`````

Generic bitstream is a fabric-independent bitstream where configuration bits are organized out-of-order in a database.
This can be regarded as a raw bitstream used for 

  - ``debugging``: Hardware engineers can validate if their configuration memories across the FPGA fabric are assigned to expected values 

  - ``an exchangeable file format for bitstream assembler``: Software engineers can use the raw bitstream to build a bitstream assembler which organize the bitstream in the loadable formate to FPGA chips.

  - ``creation of artificial bitstream``: Test engineers can craft artificial bitstreams to test each element of the FPGA fabric, which is typically not synthesizable by VPR. Use the ``--read_file`` option to load the artifical bitsteam to OpenFPGA (see details in :ref:`openfpga_bitstream_commands`). 

.. warning:: The fabric-independent bitstream cannot be directly loaded to FPGA fabrics

File Format
```````````

OpenFPGA can output the generic bitstream to an XML format, which is easy to debug. As shown in the following XML code, configuration bits are organized block by block, where each block could be a LUT, a routing multiplexer `etc`. Each ``bitstream_block`` includes the following information: 
  - ``name`` represents the instance name which you can find in the fabric netlists

  - ``hierarchy_level`` represents the depth of this block in the hierarchy of the FPGA fabric. It always starts from 0 as the root.

  - ``hierarchy`` represents the location of this block in FPGA fabric.
    The hierachy includes the full hierarchy of this block

    - ``instance`` denotes the instance name which you can find in the fabric netlists

    - ``level`` denotes the depth of the block in the hierarchy

  - ``input_nets`` represents the path ids and net names that are mapped to the inputs of block. Unused inputs will be tagged as ``unmapped`` which is a reserved word of OpenFPGA. Path id corresponds the selected ``path_id`` in the ``<bitstream>`` node.

  - ``output_nets`` represents the path ids and net names that are mapped to the outputs of block. Unused outputs will be tagged as ``unmapped`` which is a reserved word OpenFPGA. 

  - ``bitstream`` represents the configuration bits affiliated to this block.

    - ``path_id`` denotes the index of inputs which is propagated to the output. Note that smallest valid index starts from zero. Only routing multiplexers have the path index. Unused routing multiplexer will not have a ``path_id`` of ``-1``, which allows bitstream assembler to freely find the best path in terms of Quality of Results (QoR). A used routing multiplexer should have a zero or positive ``path_id``.

    - ``bit`` denotes a single configuration bit under this block. It contains \

      - ``memory_port`` the memory port name which you can find in the fabric netlists by following the hierarchy.
     
      - ``value`` a binary value which is the configuration bit assigned to the memory port.

.. code-block:: xml

  <bitstream_block name="fpga_top" hierarchy_level="0">
    <!-- Bitstream block of a 4-input Look-Up Table in a Configurable Logic Block (CLB) -->
    <bitstream_block name="grid_clb_1_1" hierarchy_level="1">
      <bitstream_block name="logical_tile_clb_mode_clb__0" hierarchy_level="2">
        <bitstream_block name="logical_tile_clb_mode_default__fle_0" hierarchy_level="3">
          <bitstream_block name="logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0" hierarchy_level="4">
            <bitstream_block name="logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_mode_default__lut4_0" hierarchy_level="5">
              <bitstream_block name="lut4_config_latch_mem" hierarchy_level="6">
                <hierarchy>
                  <instance level="0" name="fpga_top"/>
                  <instance level="1" name="grid_clb_1_1"/>
                  <instance level="2" name="logical_tile_clb_mode_clb__0"/>
                  <instance level="3" name="logical_tile_clb_mode_default__fle_0"/>
                  <instance level="4" name="logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_0"/>
                  <instance level="5" name="logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_mode_default__lut4_0"/>
                  <instance level="6" name="lut4_config_latch_mem"/>
                </hierarchy>
                <bitstream>
                  <bit memory_port="mem_out[0]" value="0"/>
                  <bit memory_port="mem_out[1]" value="0"/>
                  <bit memory_port="mem_out[2]" value="0"/>
                  <bit memory_port="mem_out[3]" value="0"/>
                  <bit memory_port="mem_out[4]" value="0"/>
                  <bit memory_port="mem_out[5]" value="0"/>
                  <bit memory_port="mem_out[6]" value="0"/>
                  <bit memory_port="mem_out[7]" value="0"/>
                  <bit memory_port="mem_out[8]" value="0"/>
                  <bit memory_port="mem_out[9]" value="0"/>
                  <bit memory_port="mem_out[10]" value="0"/>
                  <bit memory_port="mem_out[11]" value="0"/>
                  <bit memory_port="mem_out[12]" value="0"/>
                  <bit memory_port="mem_out[13]" value="0"/>
                  <bit memory_port="mem_out[14]" value="0"/>
                  <bit memory_port="mem_out[15]" value="0"/>
                </bitstream>
              </bitstream_block>
            </bitstream_block>
          </bitstream_block>
        </bitstream_block>
      </bitstream_block>
    </bitstream_block>

    <!-- More bitstream blocks -->

    <!-- Bitstream block of a 2-input routing multiplexer in a Switch Block (SB) -->
    <bitstream_block name="sb_0__2_" hierarchy_level="1">
      <bitstream_block name="mem_right_track_0" hierarchy_level="2">
        <hierarchy>
          <instance level="0" name="fpga_top"/>
          <instance level="1" name="sb_0__2_"/>
          <instance level="2" name="mem_right_track_0"/>
        </hierarchy>
        <input_nets>
          <path id="0" net_name="unmapped"/>
          <path id="1" net_name="unmapped"/>
        </input_nets>
        <output_nets>
          <path id="0" net_name="unmapped"/>
        </output_nets>
        <bitstream path_id="-1">
          <bit memory_port="mem_out[0]" value="0"/>
          <bit memory_port="mem_out[1]" value="0"/>
        </bitstream>
      </bitstream_block>
    </bitstream_block>
  </bitstream_block>
