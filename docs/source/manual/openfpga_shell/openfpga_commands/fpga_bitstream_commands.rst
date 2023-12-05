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
    See details in :ref:`file_formats_repack_design_constraints`.
  
  .. warning:: Design constraints are designed to help repacker to identify which clock net to be mapped to which pin, so that multi-clock benchmarks can be correctly implemented, in the case that VPR may not have sufficient vision on clock net mapping. **Try not to use design constraints to remap any other types of nets!!!**

  .. option:: --ignore_global_nets_on_pins

    Specify the mapping results of global nets should be ignored on which pins of a ``pb_type``. For example, ``--ignore_global_nets_on_pins clb.I[0:11]``. Once specified, the mapping results on the pins for all the global nets, such as clock, reset *etc.*, are ignored. Routing traces will be appeneded to other pins where the same global nets are mapped to. 
  
  .. note::  - This option is designed for global nets which are applied to both data path and global networks. For example, a reset signal is mapped to both a LUT input and the reset pin of a FF. Suggest not to use the option in other purposes!
  - For repack options, the constraints specified by ``--ignore_global_nets_on_pins`` have higher priority than those set by ``ignore_net``. When the constraints from ``--ignore_global_nets_on_pins`` are satisfied, those from ``ignore_net`` will not be checked. For more information on ``ignore_net``, see :ref:`file_formats_repack_design_constraints`. 

  .. warning:: Users must specify the size/width of the pin. Currently, OpenFPGA cannot infer the pin size from the architecture!!!
     
  .. option:: --verbose 
  
    Show verbose log

build_architecture_bitstream
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Decode VPR implementing results to an fabric-independent bitstream database 
  
  .. option:: --read_file <string>

    Read the fabric-independent bitstream from an XML file. When this is enabled, bitstream generation will NOT consider VPR results. See details at :ref:`file_formats_architecture_bitstream`.

  .. option:: --write_file <string>

    Output the fabric-independent bitstream to an XML file. See details at :ref:`file_formats_architecture_bitstream`.

  .. option:: --no_time_stamp

    Do not print time stamp in bitstream files
  
  .. option:: --verbose

    Show verbose log

build_fabric_bitstream
~~~~~~~~~~~~~~~~~~~~~~

  Build a sequence for every configuration bits in the bitstream database for a specific FPGA fabric

  .. option:: --verbose

    Show verbose log

write_fabric_bitstream
~~~~~~~~~~~~~~~~~~~~~~

  Output the fabric bitstream database to a specific file format

  .. option:: --file <string> or -f <string>

    Output the fabric bitstream to an plain text file (only ``0`` or ``1``)

  .. option:: --format <string>

    Specify the file format [``plain_text`` | ``xml``]. By default is ``plain_text``.
    See file formats in :ref:`file_formats_fabric_bitstream_xml` and :ref:`file_formats_fabric_bitstream_plain_text`.

  .. option:: --filter_value <int>

    .. warning:: Value filter is only applicable to XML file format!

    Specify the value to be keep in the bitstream file. Can be [``0`` | ``1`` ]. By default is ``none``, which means no filter is applied.
    When specified, only the bit with the filter value is written to the file. 
    See file formats in :ref:`file_formats_fabric_bitstream_xml`.

  .. option:: --path_only

    .. warning:: This is only applicable to XML file format!

    Specify that only the ``path`` attribute is kept in the bitstream file. By default is ``off``.
    When specified, only the ``path`` attribute is written to the file. 
    Regarding the ``path`` attribute, See file formats in :ref:`file_formats_fabric_bitstream_xml`.

  .. option:: --value_only

    .. warning:: This is only applicable to XML file format!

    Specify that only the ``value`` attribute is kept in the bitstream file. By default is ``off``.
    When specified, only the ``value`` attribute is written to the file. 
    Regarding the ``value`` attribute, see file formats in :ref:`file_formats_fabric_bitstream_xml`.

  .. option:: --trim_path

    .. warning:: This is only applicable to XML file format!

    .. warning:: This is an option for power user! Suggest only to use when you enable the ``--group_config_block`` option when building a fabric (See details in :ref:`cmd_build_fabric`).

    Specify that the ``path`` will be trimed by 1 level in resulting bitstream file. By default is ``off``.
    When specified, the hierarchy of ``path`` will be reduced by 1. For example, the original path is ``fpga_top.tile_1__1_.config_block.sub_mem.mem_out[0]``, the path after trimming is ``fpga_top.tile_1__1_.config_block.mem_out[0]``. 
    Regarding the ``path`` attribute, see file formats in :ref:`file_formats_fabric_bitstream_xml`.

  .. option:: --fast_configuration

    Reduce the bitstream size when outputing by skipping dummy configuration bits. It is applicable to configuration chain, memory bank and frame-based configuration protocols. For configuration chain, when enabled, the zeros at the head of the bitstream will be skipped. For memory bank and frame-based, when enabled, all the zero configuration bits will be skipped. So ensure that your memory cells can be correctly reset to zero with a reset signal. 
   
    .. warning:: Fast configuration is only applicable to plain text file format!

    .. note:: If both reset and set ports are defined in the circuit modeling for programming, OpenFPGA will pick the one that will bring largest benefit in speeding up configuration.

  .. option:: --keep_dont_care_bits

    Keep don't care bits (``x``) in the outputted bitstream file. This is only applicable to plain text file format. If not enabled, the don't care bits are converted to either logic ``0`` or ``1``.

  .. option:: --no_time_stamp

    Do not print time stamp in bitstream files

  .. option:: --verbose

    Show verbose log

write_io_mapping
~~~~~~~~~~~~~~~~

  Output the I/O mapping information to a file

  .. option:: --file <string> or -f <string>

    Specify the file name where the I/O mapping will be outputted to.
    See file formats in :ref:`file_format_io_mapping_file`.

  .. option:: --no_time_stamp

    Do not print time stamp in bitstream files

  .. option:: --verbose

    Show verbose log

report_bitstream_distribution
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Output the bitstream distribution to a file

  .. option:: --file <string> or -f <string>

    Specify the file name where the bitstream distribution will be outputted to.
    See file formats in :ref:`file_format_bitstream_distribution_file`.

  .. option:: --depth <int> or -d <int>

    Specify the maximum depth of the block which should appear in the block

  .. option:: --no_time_stamp

    Do not print time stamp in bitstream files

  .. option:: --verbose

    Show verbose log


