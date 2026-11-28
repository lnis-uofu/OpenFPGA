.. _openfpga_yosys_commands:

Yosys Commands
--------------

yosys
~~~~~
  
  OpenFPGA allows users to call ``yosys`` in the script mode. Note that the interactive mode and other command-line options are not supported. Refer to yosys_project_.

  .. note:: This command will run Yosys in a standalone way, whose results will not be kept and used by other commands. 

  .. option:: --file <path>

     Specify the path to the yosys script to be run

synthesize
~~~~~~~~~~

  Synthesize Verilog files into a netlist of the LUTs of the device with Yosys (it needs a build with Yosys), and read the netlist as the circuit when the architectures are read. It answers in JSON (kind ``synthesis``, schema ``openfpga_flow/schemas/synthesis.schema.json``): ``top``, ``netlist``, ``script``, ``target``, ``lut_size``, ``files``, ``constraints``, ``synthesized`` and ``circuit_read``.

  The script is a generic one (the LUT mapping of ABC, as the flow scripts do it), with the LUT size of ``--lut_size``. A plugin may give a *synthesis target* for the session (see :ref:`openfpga_shell_plugins`): its script template, the values of its ``${NAME}`` placeholders, its LUT size, its cell libraries, and whether its netlists are extended BLIF (``.eblif``). The placeholders of a script are ``${TOP_MODULE}``, ``${VERILOG_FILES}``, ``${READ_VERILOG_OPTIONS}``, ``${LUT_SIZE}``, ``${OUTPUT_BLIF}``, ``${CONSTRAINT_SYNTHESIS}`` (the Yosys commands that the constraints of the session give, right after the hierarchy is elaborated) and those of the target; a placeholder that nothing fills is refused.

  What goes into a script is checked before Yosys runs: the top module, plain file names, the read options and defines, and the values of the target. A source that takes a file from outside its folder (`` `include "/etc/passwd"``) is refused. A refusal says ``Synthesis refused: <reason>``.

  .. option:: --top <name>

     The top module

  .. option:: --files <paths>

     The Verilog files (``.v``, ``.sv``, ``.vh``), separated by commas

  .. option:: --lut_size <number>

     The size of the LUTs (2 to 8), unless the synthesis target gives it

  .. option:: --include_dirs <paths>

     The folders of `` `include``, separated by commas

  .. option:: --defines <NAME=value,...>

     The defines, separated by commas

  .. option:: --read_options <options>

     Options of ``read_verilog`` (``-nolatches``), separated by commas

  .. option:: --output_dir <path>

     The folder of the script (``synth.ys``) and of the netlist. Default ``synth``

  .. option:: --no_run

     Write the script and answer the plan, without running Yosys

  .. option:: --no_read

     Do not read the netlist as the circuit

  .. code-block:: shell

     synthesize --top and2 --files and2.v --lut_size 6

read_design
~~~~~~~~~~~

  Read a design directory (see :ref:`openfpga_design_directory`): read its constraints file with the command that a plugin names, synthesize its RTL as ``synthesize`` does (or read its netlist), and give its SDC to VPR. It answers as ``synthesize`` does, with ``design``, ``design_folder``, ``sdc`` and ``constraints_file``.

  .. option:: --file <path>

     The design directory

  .. option:: --netlist <label>

     Read the netlist of this label instead of synthesizing

  .. option:: --lut_size <number>

     The size of the LUTs (2 to 8), unless the synthesis target gives it

  .. option:: --output_dir <path>

     The folder of the script and the netlist. Default ``synth``

  .. option:: --check

     Check the design and answer what would be synthesized, without writing or reading anything

  .. option:: --no_run

     Write the script and answer the plan, without running Yosys

  .. option:: --no_read

     Do not read the netlist as the circuit

  .. code-block:: shell

     read_design --file designs/chip --lut_size 6

report_design_hierarchy
~~~~~~~~~~~~~~~~~~~~~~~

  Elaborate the RTL of a design from its top with Yosys, in a script of its own, and answer the tree of its instances and its modules as JSON (kind ``design_hierarchy``, schema ``openfpga_flow/schemas/design_hierarchy.schema.json``). The RTL is read as the synthesis reads it: the options of ``read_verilog`` of the synthesis target of the session, the defines, include folders and options of the design, the top and the defines of the constraints of the session, and the cell libraries of the target as blackboxes. The script is ``read_verilog``, ``hierarchy -top``, ``proc`` and ``write_json``. The flow of the session is not changed.

  The answer has:

  - ``tree``: the top and its instances, each with ``instance``, ``module`` (the name of the RTL), ``parameters`` (``W=8`` for a module that ``#(...)`` derives), ``kind`` (``module``; ``library``, a cell of a cell library of the synthesis target; or ``unknown``, a module that no file has), ``src`` (the file and line that instantiate it) and ``children``. The tree is breadth first and has ``--limit`` instances at most; a node whose children are not all there says how many are left out in ``more``, and ``truncated`` is true.
  - ``modules``: each module of the tree with its ``instances`` in the whole tree, its ``cells`` (the cells of Yosys in one instance: its logic, not its instances) and its ``src``, the most instantiated first.
  - ``nodes`` (in ``tree``), ``instances`` (in the whole design, the top included), ``design``, ``top``, ``files``, ``script`` and ``json`` (the file that Yosys wrote).

  .. option:: --file <path>

     The design directory (see :ref:`openfpga_design_directory`). A design with only a netlist is refused

  .. option:: --top <name>

     The top module, without a design directory (with ``--files``)

  .. option:: --files <paths>

     The Verilog files (``.v``, ``.sv``, ``.vh``), separated by commas

  .. option:: --include_dirs <paths>

     The folders of `` `include``, separated by commas

  .. option:: --defines <NAME=value,...>

     The defines, separated by commas

  .. option:: --read_options <options>

     Options of ``read_verilog`` (``-nolatches``), separated by commas

  .. option:: --output_dir <path>

     The folder of the script and of the JSON of Yosys. Default ``hierarchy``

  .. option:: --limit <number>

     The instances of the tree at most, from 1 to 100000. Default 500

  .. code-block:: shell

     report_design_hierarchy --file designs/my_design --limit 1000

.. _yosys_project: https://github.com/YosysHQ/yosys
