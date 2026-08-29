.. _file_formats_bitstream_remap:

Bitstream Remap (.xml)
======================

Overview
--------

The bitstream remap format is used to reorganize individual bitstream bit locations after place and route. It enables:

- Converting a 1D bitstream sequence to another 1D sequence with remapped bit positions
- Converting a 1D bitstream to a 2D matrix representation with remapped addresses

.. note:: This feature is currently supported only for CCFF-style configuration bitstreams.

Example
-------

The following example remaps a 33-bit bitstream to both:

- A 1D bitstream with reordered bits
- An 8×5 matrix (5 words of 8 bits), with padding for unused locations

.. image:: ./figures/bitstream_remap_example.svg
  :alt: Bitstream remap example
  :width: 100%

.. note:: A 1D bitstream remap is a special case of 2D remapping where width is set to 1.

Format Specification
--------------------

The bitstream remapping process operates region-by-region on CCFF bitstreams. Bits in different regions cannot be remapped across regions. The XML format contains two main sections:

**<region>**
  Defines how the original bitstream is divided into repeatable tiles, each containing a specified number of configuration bits (cbits).

**<tile_bitmap>**
  Defines how bits within each tile are remapped to new positions in the output bitstream.

.. note:: Tile definitions in <region> differ from tile names used during Verilog netlist generation or physical design. A tile is a logical grouping of bits that repeats in the region; bit reordering is possible within a tile but not across tiles.

Example Remap XML File
----------------------

.. code-block:: xml

  <bitstream_remap>
    <regions>
     <region id="0" total_cbits="100" width="1">
      <tile id="0" name="section1" />
      <tile id="1" name="section2" />
     </region>
     <region id="1" ...>
      ...
     </region>
    </regions>
    <tile_bitmaps>
     <tile_bitmap name="section1" cbits="120" length="120" width="1">
      <bit index="0">0</bit>
      <bit index="1">1</bit>
      ...
     </tile_bitmap>
     <tile_bitmap name="section2" cbits="120" length="120" width="1">
      <bit index="0">0</bit>
      <bit index="1">1</bit>
      ...
     </tile_bitmap>
    </tile_bitmaps>
  </bitstream_remap>

Description of XML Elements
----------------------------

- **<bitstream_remap>**: Root element containing all remapping information.
- **<regions>**: Container for region definitions.
  - **<region>**: Defines a region of the bitstream with attributes:
    - **id**: Unique identifier for the region, matching the fabric key in the bitstream.
    - **total_cbits**: Total number of configuration bits in the region.
    - **width**: Width of the output bitstream (1 for 1D, >1 for 2D).
  - Contains one or more **<tile>** elements defining tiles within the region. The sum of length×width for all tiles in a region must equal (in 1D case) or exceed (in 2D case) the total_cbits of the region.

- **<tile_bitmaps>**: Container for tile bitmap definitions.
  - **<tile_bitmap>**: Defines a bitmap for a specific tile with attributes:
    - **name**: Name of the tile, referring to the tile name in the region definition.
    - **cbits**: Number of configuration bits in the tile.
    - **length**: Length of the bitmap, equal to the number of bits in the tile (cbits) in 1D remapping.
    - **width**: Width of the bitmap (1 for 1D, >1 for 2D).
  - Contains one or more **<bit>** elements defining the mapping of individual bits. Only remapped bits need to be defined; undefined bits remain at the same location in the output bitstream.

- **<bit>**: Defines the mapping of a single bit in the tile with attributes:
    - **index**: Original index of the bit in the tile.
    - Text content: New index of the bit in the output bitstream.

Error Conditions
----------------

The command will raise an error if:

- The configuration bitstream is not in CCFF style.
- The total number of cbits defined in the tile bitmaps does not match the total_cbits specified for each region.
- Any new bit index exceeds the total number of bits in the output bitstream (considering width and length)
