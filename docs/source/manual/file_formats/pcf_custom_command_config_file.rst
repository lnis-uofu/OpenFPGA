.. _file_format_pcf_custom_command_config_file:

PCF Custom Command Configuration File (.xml)
--------------------------------------------

.. note:: This file defines custom PCF commands that can be used in a Pin Constraints File (PCF).  
   It specifies how user-facing commands are translated into FPGA configuration bits.

The custom command configuration file aims to show:

- The list of available custom commands in the platform  
- The options associated with each command  
- The encoding rules (explicit mode or decimal calculation) for each option  

An example of the file is shown as follows.

.. code-block:: xml

  <pcf_config>
    <command name="set_delay_chain" type="delay_chain">
      <option name="pad" type="pin"/>
      <pb_type name="gp_inpad.inpad"/>
      <option name="delay" type="decimal" num_bits="64" max="18446744073709551615" little_endian="false" offset="0"/>
    </command>

    <command name="set_watch_dog" type="peripheral">
      <option name="pad" type="pin"/>
      <pb_type name="gp_outpad.outpad"/>
      <option name="mode" type="mode" offset="0">
        <mode name="NORMAL" value="0001"/>
        <mode name="LOW_SPEED" value="0010"/>
        <mode name="HIGH_SPEED" value="0100"/>
      </option>
    </command>
  </pcf_config>

.. option:: command name="<string>"

  The name of the custom command as used in the PCF file.

.. option:: type="<string>"

  The category of the custom command used internally by the parser.

.. option:: option name="<string>" type="<pin|mode|decimal>" [additional attributes]

  Defines a configurable parameter for the custom command.

  - **pin**:  
    Specifies an I/O pad. Users provide a valid pad name from the pin table.  
    The parser validates the name.

  - **mode**:  
    Users select a predefined mode by name (e.g., NORMAL, LOW_SPEED, HIGH_SPEED).  
    Each mode has an explicit bit pattern defined in the configuration file.

  - **decimal**:  
    Users provide a numeric value (e.g., delay). The parser computes the corresponding bit pattern or mode bits.

    Attributes include:

    - **num_bits**: Number of bits used for encoding
    - **max**: Maximum allowed value
    - **little_endian**: Bit ordering
    - **offset**: Bit offset applied when overwriting the bitstream

.. option:: pb_type name="<string>"

  Specifies the programmable block associated with this command, whose configuration bits will be modified.

.. option:: mode name="<string>" value="<bit pattern>"

  Used only under a mode-type option. Defines the mapping between a mode name and its explicit bit pattern.
