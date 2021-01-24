.. _openfpga_bitstream_commands:

FPGA-Bitstream
--------------

repack
~~~~~~

Repack the netlist to physical pbs
Repack is an essential procedure before building a bitstream, which aims to packing each programmable blocks by considering **only** the physical modes.
Repack's functionality are in the following aspects:

- It annotates the net mapping results from operating modes (considered by VPR) to the physical modes (considered by OpenFPGA)

- It re-routes all the nets by considering the programmable interconnects in physical modes **only**.

.. note:: This must be done before bitstream generator and testbench generation. Strongly recommend it is done after all the fix-up have been applied

.. option:: --design_constraints 

  Apply design constraints from an external file. 
  Normally, repack takes the net mapping from VPR packing and routing results. 
  Alternatively, repack can accept the design constraints, in particular, net remapping, from an XML-based design constraint description.
  See details in :ref:`fpga_bitstream_repack_design_constraints`.

.. warning:: Design constraints are designed to help repacker to identify which clock net to be mapped to which pin, so that multi-clock benchmarks can be correctly implemented, in the case that VPR may not have sufficient vision on clock net mapping. **Try not to use design constraints to remap any other types of nets!!!**
   
.. option:: --verbose 

  Show verbose log

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
