.. _openfpga_vpr_commands:

VPR Commands
------------

vpr
~~~
  
  OpenFPGA allows users to call ``vpr`` in the standard way as documented in the vtr_project_.

  .. note:: This command will run vpr in a standalone way, whose results will be kept and used by other commands. Suggest to use when this is the final run of VPR.

  For example, vpr commands may be called in the following way:

.. code-block:: shell

  # VPR standalone runs, no results will be kept for downstream commands
  vpr_standalone <some_options> 
  vpr_standalone <some_options> 
  # More standalone runs may be expected
  vpr_standalone <some_options> 
  # Final VPR run, results are kept for downstream commands
  vpr <some_options>
  # Other commands that use VPR results

.. _vtr_project: https://github.com/verilog-to-routing/vtr-verilog-to-routing

vpr_standalone
~~~~~~~~~~~~~~
  
  OpenFPGA allows users to call ``vpr`` in the standard way as documented in the vtr_project_.
  
  .. note:: This command will run vpr in a standalone way, whose results will **not** be kept and **not** used by other commands. Suggest to use when only some stages of VPR are needed.

.. _vtr_project: https://github.com/verilog-to-routing/vtr-verilog-to-routing

Stage-by-stage flow
~~~~~~~~~~~~~~~~~~~

  Instead of running VPR in one command, the inputs can be read first, and the stages run one at a time with the settings of the application options (see :ref:`set_app_option <set_app_option>`). The device must have a fixed layout. The commands are:

  - ``read_vpr_arch --file <string> --layout <string>`` reads and checks the VPR architecture file, with the name of the fixed layout of the device (for example ``2x2``). It starts the flow over: the results of the stages and the circuit are dropped.
  - ``read_openfpga_arch --file <string>`` reads the OpenFPGA architecture. It needs the VPR architecture to have been read.
  - ``read_circuit --file <string>`` reads the circuit netlist against the VPR architecture. VPR sets itself up from the architecture and the circuit together, so the architecture is read again.
  - ``pack``, ``place`` and ``route`` run the stages. A stage needs the one before it: ``pack`` the device, ``place`` the packing, ``route`` the placement.

  The device (the grid and, if the channel width is fixed with ``router.route_chan_width``, the routing resource graph) is created once the three inputs have been read. A layout that is not fixed (``auto``) is refused, since it is sized from the result of packing.

  A stage that is run again, or run after an option changed, runs the stages before it again on fresh data, since VPR cannot run a stage twice on the same data. The results are those of ``vpr``: the same packing, placement and routing for the same options. The command ``vpr`` replaces the data of this flow, which has to be read again.

  .. code-block:: shell

    set_app_option --name router.route_chan_width --value 20
    read_vpr_arch --file vpr_arch.xml --layout 2x2
    read_openfpga_arch --file openfpga_arch.xml
    read_circuit --file and2.blif
    pack
    place
    route
    # Try another seed: the packing stays valid, and placement and routing run again
    set_app_option --name place.seed --value 3
    place
    route
