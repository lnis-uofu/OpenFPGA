.. _file_format_bus_group_file:

Bus Group File (.xml)
=====================

The bus group file aims to show 

- How bus ports are flatten by EDA engines, e.g., synthesis.
- What are the pins in post-routing corresponding to the bus ports before synthesis

An example of file is shown as follows.

.. code-block:: xml

  <bus_group>
    <bus name="i_addr[0:3]" big_endian="false">
      <pin id="0" name="i_addr_0_"/>
      <pin id="1" name="i_addr_1_"/>
      <pin id="2" name="i_addr_2_"/>
      <pin id="3" name="i_addr_3_"/>
    </bus>
  </bus_group>

Bus-related Syntax
------------------

.. option:: name="<string>"

  The bus port defined before synthesis, e.g., addr[0:3]

.. option:: big_endian="<bool>"

  Specify if this port should follow big endian or little endian in Verilog netlist. By default, big endian is assumed, e.g., addr[0:3].

Pin-related Syntax
------------------

.. option:: id="<int>"

  The index of the current pin in a bus port. The index must be the range of **[LSB, MSB-1]** that are defined in the bus.

.. option:: name="<string>"

  The pin name after bus flatten in synthesis results
