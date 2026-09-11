.. _file_format_mif_location_map_file:

MIF Location Map File (.xml)
----------------------------

The *Memory Initialization File* (MIF) location map aims to show the detailed information for each MIF data bus at the top-level of FPGA fabric

- The primitive pb_type that the data bus is assoicated to
- For each primitive pb_type, the data width and offset of the data bus at the top-level, based on which MSB ad LSB can be inferred
- The coordinate (in VPR domain) of each memory block with MIF feature in an FPGA fabric

An example of the file is shown as follows.

.. code-block:: xml
  :caption: Example MIF location map file
  :name: simple-mif-loc-file

  <mif_coordinates>
  	<mif port="gfpga_pad_frac_mem_256_preload_mem_init_data" pb_type="memory[0][mem_16x16_phy]/mem_16x16_phy[0]" data_offset="16" data_width="16" x="2" y="1" z="0" />
  	<mif port="gfpga_pad_frac_mem_256_preload_mem_init_data" pb_type="memory[0][mem_16x16_phy]/mem_16x16_phy[0]" data_offset="0" data_width="16" x="2" y="2" z="0" />
  </mif_coordinates>


.. option:: port="<string>"

  The port name of the MIF data bus at the top-level FPGA fabric, which should be a valid port defined in output Verilog netlist.

  .. note:: You should be find the exact pin in the top-level module of FPGA fabric if you output the Verilog netlists.

.. option:: pb_type="<string>"

  The hierarchical name of primitive pb_type (a memory block with MIF feature) in the VPR architecture definition

.. option:: data_offset="<int>"

  The offset in the MIF data bus at the top-level FPGA fabric, which the MIF bus of the primitive pb_type will take. 

.. option:: data_width="<int>" 

  The width in the MIF data bus at the top-level FPGA fabric, which the MIF bus of the primitive pb_type will take. 

.. note:: Combined with the ``data_width`` and ``offset``, the *Most Significant Bit* (MSB) and *Least Signaificant Bit* (LSB) can be inferred. Take the example in :numref:`simple-mif-loc-file`, the MIF data bus of the top-level FPGA fabric is ``gfpga_pad_frac_mem_256_preload_mem_init_data[0:31]``, an offset of ``16`` and data width of ``16`` indicates that the memory block at (x=2, y=2, z=0) takes the ``gfpga_pad_frac_mem_256_preload_mem_init_data[16:31]``

.. option:: x="<int>"

  The x coordinate of the primitive memory block in VPR coordinate system.

.. option:: y="<int>"

  The y coordinate of the primitive memory block in VPR coordinate system.

.. option:: z="<int>"

  The z coordinate of the primitive memory block in VPR coordinate system.
