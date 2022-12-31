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
