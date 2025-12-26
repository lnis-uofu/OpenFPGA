.. _file_format_pcf_file:

Pin Constraints File (.pcf)
---------------------------

.. note:: This file is in a different usage than the Pin Constraints File in XML format (see details in :ref:`file_format_pin_constraints_file`)

The PCF file is the file which **users** should craft to assign their I/O constraints.

Built-in Commands
~~~~~~~~~~~~~~~~~
An example of a standard PCF file is shown as follows:

.. code-block:: xml

  set_io a pad_fpga_io[0]
  set_io b[0] pad_fpga_io[4]
  set_io c[1] pad_fpga_io[6]

.. option:: set_io <net> <pin>

  Assign a net (defined as an input or output in users' HDL design) to a specific pin of an FPGA device (typically a packaged chip).

  .. note:: The net should be single-bit and match the port declaration of the top-module in users' HDL design

  .. note:: FPGA devices have different pin names, depending on their naming rules. Please contact your vendor for details.

  .. note:: Built-in PCF commands cannot be overloaded. 
    
Custom Commands
~~~~~~~~~~~~~~~


In addition to standard pin assignment commands, the PCF file supports custom commands defined in :ref:`file_format_pcf_custom_command_config_file`. These commands provide a structured way for users to configure platform-specific hardware features through PCF.

.. note:: 
   Custom PCF commands allow users to specify platform-specific configuration options (such as delay chains or peripheral modes) without dealing with low-level bitstream encodings. 
   The detailed behavior, valid options, and encoding rules of each custom command are defined in :ref:`file_format_pcf_custom_command_config_file` and automatically interpreted by the PCF parser.

An example of PCF file using custom commands is shown as follows:

.. code-block:: xml

  # Configure a delay chain on input pad pad_fpga_i[0]
  # The detailed step and range are defined in the configuration file.
  # The user only needs to specify the delay value.
  # The PCF parser automatically encodes this into configuration bits.
  set_delay_chain -pad pad_fpga_i[0] -delay 18446744073709551614

  # Configure a watchdog peripheral on output pad pad_fpga_o[0]
  # The user specifies the operating mode by name.
  # The PCF parser automatically maps the mode to configuration bits.
  set_watch_dog -pad pad_fpga_o[0] -mode NORMAL

.. option:: <custom_command_name> <option> <value> ...

  .. note:: Each option must match the definitions in :ref:`file_format_pcf_custom_command_config_file`. 
     The value provided must be valid according to the configuration file. The PCF parser will automatically validate and encode the values.
