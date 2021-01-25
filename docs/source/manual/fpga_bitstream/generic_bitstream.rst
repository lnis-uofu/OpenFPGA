Generic Bitstream
~~~~~~~~~~~~~~~~~

Usage
`````

Generic bitstream is a fabric-independent bitstream where configuration bits are organized out-of-order in a database.
This can be regarded as a raw bitstream used for 

  - ``debugging``: Hardware engineers can validate if their configuration memories across the FPGA fabric are assigned to expected values 

  - ``an exchangeable file format for bitstream assembler``: Software engineers can use the raw bitstream to build a bitstream assembler which organize the bitstream in the loadable formate to FPGA chips.

  - ``creation of artificial bitstream``: Test engineers can craft artificial bitstreams to test each element of the FPGA fabric, which is typically not synthesizable by VPR. Use the ``--read_file`` option to load the artifical bitsteam to OpenFPGA (see details in :ref:`openfpga_bitstream_commands`). 

.. warning:: The fabric-independent bitstream cannot be directly loaded to FPGA fabrics

File Format
```````````

See details in :ref:`file_formats_architecture_bitstream`
