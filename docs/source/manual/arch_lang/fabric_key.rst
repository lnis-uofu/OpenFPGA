.. _fabric_key:

Fabric Key
~~~~~~~~~~

Fabric key is a secure key for users to generate bitstream for a specific FPGA fabric. 
With this key, OpenFPGA can generate correct bitstreams for the FPGA.
Using a wrong key, OpenFPGA may error out or generate wrong bitstreams.
The fabric key support allows users to build secured/classified FPGA chips even with an open-source tool.

.. figure:: figures/fabric_key_motivation.png
   :scale: 60%
   :alt: map to buried treasure
 
   The use of fabric key to secure the FPGA chip design

.. note:: Users are the only owner of the key. OpenFPGA will not store or replicate the key.

Key Generation
``````````````
A fabric key can be achieved in the following ways:

- OpenFPGA can auto-generate a fabric key using random algorithms (see detail in :ref:`cmd_build_fabric`)

- Users can craft a fabric key based on auto-generated file by following the file format description.

File Format
```````````

See details in :ref:`file_formats_fabric_key`
