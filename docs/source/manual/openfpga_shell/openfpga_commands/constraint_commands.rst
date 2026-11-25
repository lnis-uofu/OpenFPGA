.. _openfpga_constraint_commands:

Constraint Commands
-------------------

The constraints of a design are in one file, an ODC file (OpenFPGA Design Constraints, ``.odc``, ``docs/spec_odc.md``), as an XDC file holds them for Xilinx devices: timing, synthesis, floorplanning, routing and pins. The flow reads the file once, and each tool gets the part it needs:

- VPR gets an SDC (``filename.sdc_file``) and a VPR constraints XML (``filename.read_vpr_constraints_file``);
- the synthesis (``synthesize``, ``read_design``) gets Yosys commands at the line ``${ODC_SYNTHESIS}`` of its script;
- the OpenFPGA commands that take ``--pin_constraints_file`` get a pin constraints XML when they are not given one.

These files are written in the folder ``odc/`` of the current directory. Every command of the file is checked against the netlist and the device as far as they are read, and the rest when they are read: a command that VPR would skip, drop without a word, or fail on with a message that does not say why, is reported with its file and line.

The commands are commands of the plugin ``ui``, see :ref:`openfpga_shell_plugins`.

.. code-block:: tcl

  odc_version 1
  create_clock -period 5.0 -name clk [get_ports clk]
  set_input_delay -clock clk -max 1.0 [get_ports {din[*]}]
  set_property KEEP true [get_nets debug_state]
  create_pblock pb_ctrl
  resize_pblock pb_ctrl -add X1Y1:X2Y3
  add_cells_to_pblock pb_ctrl [get_cells -hierarchical ctrl_*]
  set_global_signal -route_model ideal [get_nets clk]
  set_property PACKAGE_PIN LED0 [get_ports dout[0]]
  set_property LOC X0Y2/4 [get_ports rst]

An SDC file that VPR reads is an ODC file, with a warning for the missing first line ``odc_version 1``. Locations are those of the VPR grid: ``X3Y2`` a tile, ``X3Y2/1`` its subtile 1, ``X1Y1:X4Y6`` a rectangle. ``PACKAGE_PIN`` names a pin of the I/O map of the device package (``customer_pin_alias`` or ``netlist_name``).

read_odc
~~~~~~~~

  Read the constraints of the design from an ODC file. They replace the ones of the session. The default constraints of the device package (role ``odc_defaults``) are read first, and a command of the file on the same port or cell wins.

  A file with an error is not read, and the session keeps its constraints. An unknown command is reported with the nearest command, and a query that matches nothing with the nearest names. A change of the timing, floorplanning, routing or pin commands drops the stages from ``DEVICE``; a change of the synthesis commands drops the circuit. A raw SDC or VPR constraints file and the ODC cannot set the same thing: the read is refused, with the command that takes the other one away.

  .. option:: --file or -f <string>

    The ODC file

  .. option:: --lenient

    An unknown command is a warning, and is skipped, instead of an error

  .. code-block::

    read_odc --file design.odc

write_odc
~~~~~~~~~

  Write the ODC of the session, one command per line, in the order of the file and of the files it includes, without the comments and without the defaults of the device package. The file reads back to the same commands.

  .. option:: --file or -f <string>

    The file to write

clear_odc
~~~~~~~~~

  Take the ODC of the session away, with the files it gave to VPR. It drops what reading it dropped.

report_constraints
~~~~~~~~~~~~~~~~~~

  Answer with the constraints of the session: kind ``constraints`` (``openfpga_flow/schemas/constraints.schema.json``), not read from a stage. The checks run again on what is read now.

  - ``file``, ``defaults``, ``files``: the ODC file (``null`` when the session has none), the defaults of the package, and the files read
  - ``design``: the netlist the objects are matched in, whether the device is known, the I/O map
  - ``commands``: each command with its ``file``, ``line``, ``text``, ``kind``, ``stage``, ``consumer``, whether it is ``checked`` yet, and its ``queries``: the objects each matched (at most ``--limit``), or the ``nearest`` names when it matched nothing
  - ``diagnostics``: the errors and warnings, with their file and line
  - ``timing``: the clocks and their periods, the clocks of the netlist that no ``create_clock`` names, and the input and output ports without a delay
  - ``outputs``: the files given to the tools

  .. option:: --limit <int>

    The most objects listed for a query (default: 50)
