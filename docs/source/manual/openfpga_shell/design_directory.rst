.. _openfpga_design_directory:

Design directories
------------------

A *design directory* holds everything that belongs to one design: the RTL, a netlist that was synthesized already, the timing and other constraints, the testbench and the memory files, with a manifest, ``design.json``, that says what each file is. The command ``read_design`` (see :ref:`openfpga_yosys_commands`) reads one, and ``report_design_hierarchy`` elaborates its RTL. A design directory names no device: the same design runs on any device, which the user of the design chooses.

Layout
~~~~~~

.. code-block:: text

  <design_name>/
    design.json        the manifest (required)
    README.md          free text (optional)
    LICENSE            the licence text, when the license is a LicenseRef (optional)
    rtl/               Verilog and SystemVerilog files (.v, .sv)
      include/         headers read by `include (.vh, .svh)
    netlist/           netlists that were synthesized already: .blif or .eblif (optional)
    tb/                testbench (optional)
    mem/               memory initialization files: .hex, .bin, .mem (optional)
    constraints/       SDC, constraints, pin and placement constraints (optional)

The paths of the manifest are relative to the design directory, and the manifest decides what a file is: the folder names are a convention. A run never writes into a design directory: the script, the netlist and the files of the flow go to the directory of the run.

Manifest
~~~~~~~~

.. code-block:: json

  {
    "format_version": "1.0",
    "name": "chip",
    "license": "MIT",
    "top": "chip",
    "files": [
      { "role": "rtl", "path": "rtl/chip.v" },
      { "role": "rtl_include", "path": "rtl/include/defs.vh" },
      { "role": "sdc", "path": "constraints/chip.sdc", "stage": "implementation" },
      { "role": "netlist", "path": "netlist/chip.blif", "label": "yosys" }
    ],
    "rtl": {
      "include_dirs": ["rtl/include"],
      "defines": { "WIDTH": "16" },
      "read_options": ["-nolatches"]
    }
  }

- ``format_version``: ``major.minor``, ``1.0``
- ``name``: the name of the design (the name of the directory when it is not given)
- ``license``: an SPDX identifier, or ``LicenseRef-<name>`` with the text in ``LICENSE``
- ``top``: the top module of the RTL
- ``files``: each file with its ``role`` and ``path`` (see below)
- ``rtl``: how the RTL is read: ``include_dirs`` (folders of the design), ``defines`` (``NAME: value``, letters, digits and ``_``) and ``read_options`` (options of ``read_verilog`` such as ``-nolatches``; ``-sv`` is added for SystemVerilog). Nothing else of the manifest is written into a synthesis script

The roles of the files:

.. list-table::
  :header-rows: 1
  :widths: 20 80

  * - Role
    - File
  * - ``rtl``
    - a Verilog (``.v``, ``.vh``) or SystemVerilog (``.sv``) design file; one or more
  * - ``rtl_include``
    - a header read by `` `include``
  * - ``netlist``
    - a synthesized netlist, ``.blif`` or ``.eblif``, with a ``label`` when there are several
  * - ``sdc``
    - timing constraints; ``stage`` is ``synthesis``, ``implementation`` or both (the default)
  * - ``constraints``
    - the constraints of the design in a format that a plugin reads (one at most), see below
  * - ``testbench``, ``stimulus``, ``mem_init``, ``pin_constraints``, ``placement_constraints``, ``repack_constraints``, ``activity``, ``doc``
    - the other files of a design; ``read_design`` does not use them yet
  * - ``x_<name>``
    - a role of one's own, which tools leave out

Rules
~~~~~

``read_design`` checks what it reads, and refuses a design that breaks a rule, with the reason (``Synthesis refused: ...``):

- every path of the manifest is relative, stays inside the design directory (no ``..``, no absolute path, no link that leaves it) and exists
- a design has RTL or a netlist. A design with RTL is synthesized, even when it has a netlist: a netlist is read only when it is asked for by its label (``--netlist``), or when the design has no RTL (its first netlist)
- a source file takes no file from outside the design directory: an `` `include`` or a ``$readmemh``/``$readmemb`` of a name that is absolute, starts with ``~`` or has ``..`` is refused
- at most 20 RTL files, of 1 MiB each at most
- the defines, include folders and read options are made of characters that a Yosys script takes

What read_design does
~~~~~~~~~~~~~~~~~~~~~

#. It reads the manifest and checks the design.
#. It reads the file of the role ``constraints`` with the command that a plugin of the build names for it (see :ref:`openfpga_shell_plugins`). A design with such a file is refused by a build that has no such command.
#. It synthesizes the RTL as ``synthesize`` does, with the top, the include folders, the defines and the read options of the manifest, or it reads the netlist; the result is read as the circuit when the architectures are read.
#. It gives the SDC of the stage ``implementation`` (or of both stages) to VPR, as the option ``filename.sdc_file``. When the constraints of the session give the timing already, a design with an SDC is refused.

``read_design --check`` checks the design and says what would be synthesized, without writing anything.
