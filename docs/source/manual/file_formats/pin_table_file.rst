.. _file_format_pin_table_file:

Pin Table File (.csv)
---------------------

.. note:: This file is typically a spreadsheet provided by FPGA vendors. Please contact your vendor for the exact file.

.. note:: OpenFPGA will not include or guarantee the correctness of the file!!!

The pin table file is the file which describes the pin mapping between a chip and an FPGA inside the chip. 

An example of the file is shown as follows.

.. code-block:: xml

  orientation,row,col,pin_num_in_cell,port_name,mapped_pin,GPIO_type,Associated Clock,Clock Edge
  TOP,,,,gfpga_pad_IO_A2F[0],pad_fpga_io[0],in,,
  TOP,,,,gfpga_pad_IO_F2A[0],pad_fpga_io[0],out,,
  TOP,,,,gfpga_pad_IO_A2F[4],pad_fpga_io[1],in,,
  TOP,,,,gfpga_pad_IO_F2A[4],pad_fpga_io[1],out,,
  TOP,,,,gfpga_pad_IO_A2F[8],pad_fpga_io[2],in,,
  TOP,,,,gfpga_pad_IO_F2A[8],pad_fpga_io[2],out,,
  TOP,,,,gfpga_pad_IO_A2F[31],pad_fpga_io[3],in,,
  TOP,,,,gfpga_pad_IO_F2A[31],pad_fpga_io[3],out,,
  RIGHT,,,,gfpga_pad_IO_A2F[32],pad_fpga_io[4],in,,
  RIGHT,,,,gfpga_pad_IO_F2A[32],pad_fpga_io[4],out,,
  RIGHT,,,,gfpga_pad_IO_A2F[40],pad_fpga_io[5],in,,
  RIGHT,,,,gfpga_pad_IO_F2A[40],pad_fpga_io[5],out,,
  BOTTOM,,,,gfpga_pad_IO_A2F[64],pad_fpga_io[6],in,,
  BOTTOM,,,,gfpga_pad_IO_F2A[64],pad_fpga_io[6],out,,
  LEFT,,,,gfpga_pad_IO_F2A[127],pad_fpga_io[7],in,,
  LEFT,,,,gfpga_pad_IO_A2F[127],pad_fpga_io[7],out,,

An pin table may serve in various purposes. However, for OpenFPGA, the following attributes are required

.. option:: orientation

  Specify on which side the pin locates

.. option:: port_name

  Specify the port name of the FPGA fabric

.. option:: mapped_pin

  Specify the pin name of the FPGA chip

.. option:: GPIO_type

  Specify the pin direction. Can be [``in``|``out``].

  .. note:: This column can be left as empty if users follow quicklogic style. See details in :ref:`openfpga_setup_commands_pcf2place`
