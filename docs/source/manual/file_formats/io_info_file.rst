.. _file_format_io_info_file:

I/O Information File (.xml)
---------------------------

.. note:: This file is in a different usage than the I/O mapping file (see details in :ref:`file_format_io_mapping_file`)

The I/O information file aims to show 

- The number of I/O in an FPGA fabric
- The name of each I/O in an FPGA fabric
- The coordinate (in VPR domain) of each I/O in an FPGA fabric

An example of the file is shown as follows.

.. code-block:: xml

  <io_coordinates>
    <io pad="gfpga_pad_GPIO_PAD[0]" x="1" y="2" z="0"/>
    <io pad="gfpga_pad_GPIO_PAD[1]" x="1" y="2" z="1"/>
    <io pad="gfpga_pad_GPIO_PAD[2]" x="1" y="2" z="2"/>
    <io pad="gfpga_pad_GPIO_PAD[3]" x="1" y="2" z="3"/>
    <io pad="gfpga_pad_GPIO_PAD[4]" x="1" y="2" z="4"/>
    <io pad="gfpga_pad_GPIO_PAD[5]" x="1" y="2" z="5"/>
    <io pad="gfpga_pad_GPIO_PAD[6]" x="1" y="2" z="6"/>
    <io pad="gfpga_pad_GPIO_PAD[7]" x="1" y="2" z="7"/>
    <io pad="gfpga_pad_GPIO_PAD[8]" x="2" y="1" z="0"/>
    <io pad="gfpga_pad_GPIO_PAD[9]" x="2" y="1" z="1"/>
    <io pad="gfpga_pad_GPIO_PAD[10]" x="2" y="1" z="2"/>
    <io pad="gfpga_pad_GPIO_PAD[11]" x="2" y="1" z="3"/>
    <io pad="gfpga_pad_GPIO_PAD[12]" x="2" y="1" z="4"/>
    <io pad="gfpga_pad_GPIO_PAD[13]" x="2" y="1" z="5"/>
    <io pad="gfpga_pad_GPIO_PAD[14]" x="2" y="1" z="6"/>
    <io pad="gfpga_pad_GPIO_PAD[15]" x="2" y="1" z="7"/>
  </io_coordinates>

.. option:: pad="<string>"

  The port name of the I/O in FPGA fabric, which should be a valid port defined in output Verilog netlist.

  .. note:: You should be find the exact pin in the top-level module of FPGA fabric if you output the Verilog netlists.

.. option:: x="<int>"

  The x coordinate of the I/O in VPR coordinate system.

.. option:: y="<int>"

  The y coordinate of the I/O in VPR coordinate system.

.. option:: z="<int>"

  The z coordinate of the I/O in VPR coordinate system.
