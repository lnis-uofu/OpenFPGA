Fabric-dependent Bitstream
~~~~~~~~~~~~~~~~~~~~~~~~~~

Usage
`````

Fabric-dependent bitstream is design to be loadable to the configuration protocols of FPGAs. 
The bitstream just sets an order to the configuration bits in the database, without duplicating the database.
OpenFPGA framework provides a fabric-dependent bitstream generator which is aligned to our Verilog netlists.
The fabric-dependent bitstream can be found in the pre-configured Verilog testbenches.
The fabric bitsteam can be outputted in different file format in terms of usage.

Plain Text File Format
```````````````````````

See details in :ref:`file_formats_fabric_bitstream_plain_text`

XML File Format
```````````````

See details in :ref:`file_formats_fabric_bitstream_xml`
