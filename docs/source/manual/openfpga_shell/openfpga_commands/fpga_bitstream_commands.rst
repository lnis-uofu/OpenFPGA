.. _openfpga_bitstream_commands:

FPGA-Bitstream
--------------

repack
~~~~~~

  Repack the netlist to physical pbs

  .. note:: This must be done before bitstream generator and testbench generation. Strongly recommend it is done after all the fix-up have been applied
   
  - ``--verbose`` Show verbose log

build_architecture_bitstream
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Decode VPR implementing results to an fabric-independent bitstream database 
  
  - ``--read_file`` Read the fabric-independent bitstream from an XML file. When this is enabled, bitstream generation will NOT consider VPR results.

  - ``--write_file`` Output the fabric-independent bitstream to an XML file
  
  - ``--verbose`` Show verbose log

build_fabric_bitstream
~~~~~~~~~~~~~~~~~~~~~~

  Build a sequence for every configuration bits in the bitstream database for a specific FPGA fabric

  - ``--verbose`` Show verbose log

write_fabric_bitstream
~~~~~~~~~~~~~~~~~~~~~~

  Output the fabric bitstream database to a specific file format

  - ``--file`` or ``-f`` Output the fabric bitstream to an plain text file (only 0 or 1)

  - ``--format`` Specify the file format [``plain_text`` | ``xml``]. By default is ``plain_text``.

  - ``--verbose`` Show verbose log
