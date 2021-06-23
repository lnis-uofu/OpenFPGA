.. _file_format_bitstream_distribution_file:

Bitstream Distribution File (.xml)
----------------------------------

The bitstream distribution file aims to show 

- The total number of configuration bits under each block
- The number of configuration bits per block 

An example of design constraints is shown as follows.

.. code-block:: xml

  <block name="fpga_top" number_of_bits="527">
  	<block name="grid_clb_1__1_" number_of_bits="136">
  	</block>
  	<block name="grid_io_top_1__2_" number_of_bits="8">
  	</block>
  	<block name="grid_io_right_2__1_" number_of_bits="8">
  	</block>
  	<block name="grid_io_bottom_1__0_" number_of_bits="8">
  	</block>
  	<block name="grid_io_left_0__1_" number_of_bits="8">
  	</block>
  	<block name="sb_0__0_" number_of_bits="58">
  	</block>
  	<block name="sb_0__1_" number_of_bits="57">
  	</block>
  	<block name="sb_1__0_" number_of_bits="59">
  	</block>
  	<block name="sb_1__1_" number_of_bits="56">
  	</block>
  	<block name="cbx_1__0_" number_of_bits="33">
  	</block>
  	<block name="cbx_1__1_" number_of_bits="33">
  	</block>
  	<block name="cby_0__1_" number_of_bits="30">
  	</block>
  	<block name="cby_1__1_" number_of_bits="33">
  	</block>
  </block>

.. option:: name="<string>"

  The block name represents the instance name which you can find in the fabric netlists

.. option:: number_of_bits="<string>"

  The total number of configuration bits in this block
