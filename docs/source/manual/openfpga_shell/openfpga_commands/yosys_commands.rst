.. _openfpga_yosys_commands:

Yosys Commands
--------------

yosys
~~~~~
  
  OpenFPGA allows users to call ``yosys`` in the script mode. Note that the interactive mode and other command-line options are not supported. Refer to yosys_project_.

  .. note:: This command will run Yosys in a standalone way, whose results will not be kept and used by other commands. 

  .. option:: --file <path>

     Specify the path to the yosys script to be run

report_design_hierarchy
~~~~~~~~~~~~~~~~~~~~~~~

  Elaborate the RTL of a design from its top with Yosys, in a script of its own, and answer the tree of its instances and its modules as JSON (kind ``design_hierarchy``, schema ``openfpga_flow/schemas/design_hierarchy.schema.json``). The RTL is read as the synthesis reads it: the options of ``read_verilog`` of the device package of the session, the defines, include folders and options of the design, the top and the defines of the ODC of the session, and the cell library of the package (role ``yosys_cell_sim``) as blackboxes. The script is ``read_verilog``, ``hierarchy -top``, ``proc`` and ``write_json``. The flow of the session is not changed.

  The answer has:

  - ``tree``: the top and its instances, each with ``instance``, ``module`` (the name of the RTL), ``parameters`` (``W=8`` for a module that ``#(...)`` derives), ``kind`` (``module``; ``library``, a cell of the device package; or ``unknown``, a module that no file has), ``src`` (the file and line that instantiate it) and ``children``. The tree is breadth first and has ``--limit`` instances at most; a node whose children are not all there says how many are left out in ``more``, and ``truncated`` is true.
  - ``modules``: each module of the tree with its ``instances`` in the whole tree, its ``cells`` (the cells of Yosys in one instance: its logic, not its instances) and its ``src``, the most instantiated first.
  - ``nodes`` (in ``tree``), ``instances`` (in the whole design, the top included), ``design``, ``top``, ``files``, ``script`` and ``json`` (the file that Yosys wrote).

  .. option:: --file <path>

     The design directory (``docs/spec_design_directory.md``). A design with only a netlist is refused

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
