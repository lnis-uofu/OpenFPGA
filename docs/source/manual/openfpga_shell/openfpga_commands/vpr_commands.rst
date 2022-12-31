.. _openfpga_vpr_commands:

VPR Commands
------------

vpr
~~~
  
  OpenFPGA allows users to call ``vpr`` in the standard way as documented in the vtr_project_.

  .. note:: This command will run vpr in a standalone way, whose results will be kept and used by other commands. Suggest to use when this is the final run of VPR.

.. _vtr_project: https://github.com/verilog-to-routing/vtr-verilog-to-routing

vpr_standalone
~~~~~~~~~~~~~~
  
  OpenFPGA allows users to call ``vpr`` in the standard way as documented in the vtr_project_.
  
  .. note:: This command will run vpr in a standalone way, whose results will **not** be kept and **not** used by other commands. Suggest to use when only some stages of VPR are needed.

.. _vtr_project: https://github.com/verilog-to-routing/vtr-verilog-to-routing
