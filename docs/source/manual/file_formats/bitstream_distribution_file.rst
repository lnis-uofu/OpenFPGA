.. _file_format_bitstream_distribution_file:

Bitstream Distribution File (.xml)
----------------------------------

The bitstream distribution file aims to show 

- region-level bitstream distribution
  - The total number of configuration bits under each region

- block-level bitstream distribution
  - The total number of configuration bits under each block
  - The number of configuration bits per block 

An example of the file is shown as follows.

.. code-block:: xml

  <bitstream_distribution>
    <regions>
      <region id="0" number_of_bits="2250">
      </region>
    </regions>
    <blocks>
      <block name="fpga_top" number_of_bits="2250">
        <block name="grid_clb_1__1_" number_of_bits="1700">
        </block>
        <block name="grid_io_top_1__2_" number_of_bits="8">
        </block>
        <block name="grid_io_right_2__1_" number_of_bits="8">
        </block>
        <block name="grid_io_bottom_1__0_" number_of_bits="8">
        </block>
        <block name="grid_io_left_0__1_" number_of_bits="8">
        </block>
        <block name="sb_0__0_" number_of_bits="40">
        </block>
        <block name="sb_0__1_" number_of_bits="40">
        </block>
        <block name="sb_1__0_" number_of_bits="40">
        </block>
        <block name="sb_1__1_" number_of_bits="40">
        </block>
        <block name="cbx_1__0_" number_of_bits="88">
        </block>
        <block name="cbx_1__1_" number_of_bits="94">
        </block>
        <block name="cby_0__1_" number_of_bits="88">
        </block>
        <block name="cby_1__1_" number_of_bits="88">
        </block>
      </block>
    </blocks>
  </bitstream_distribution>

Region-Level Bitstream Distribution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Region-level bitstream distribution is shown under the ``<regions>`` code block
  
.. option:: id="<string>"

  The unique index of the region, which can be found in the :ref:`file_formats_fabric_key`

.. option:: number_of_bits="<string>"

  The total number of configuration bits in this region

Block-Level Bitstream Distribution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Block-level bitstream distribution is shown under the ``<blocks>`` code block
  
.. option:: name="<string>"

  The block name represents the instance name which you can find in the fabric netlists

.. option:: number_of_bits="<string>"

  The total number of configuration bits in this block
