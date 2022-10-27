.. _file_format_pcf_file:

Pin Constraints File (.pcf)
---------------------------

.. note:: This file is in a different usage than the Pin Constraints File in XML format (see details in :ref:`file_format_pin_constraints_file`)

The PCF file is the file which **users** should craft to assign their I/O constraints

An example of the file is shown as follows.

.. code-block:: xml

  set_io a pad_fpga_io[0]
  set_io b[0] pad_fpga_io[4]
  set_io c[1] pad_fpga_io[6]

.. option:: set_io <net> <pin>

  Assign a net (defined as an input or output in users' HDL design) to a specific pin of an FPGA device (typically a packaged chip).

  .. note:: The net should be single-bit and match the port declaration of the top-module in users' HDL design

  .. note:: FPGA devices have different pin names, depending their naming rules. Please contact your vendor about details.
