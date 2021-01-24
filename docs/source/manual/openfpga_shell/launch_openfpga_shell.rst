.. _launch_openfpga_shell:

Launch OpenFPGA Shell
---------------------

OpenFPGA employs a shell-like user interface, in order to integrate all the tools in a well-modularized way.
Currently, OpenFPGA shell is an unified platform to call ``vpr``, ``FPGA-Verilog``, ``FPGA-Bitstream``, ``FPGA-SDC`` and ``FPGA-SPICE``.
To launch OpenFPGA shell, users can choose two modes.

.. option::	--interactive or -i

  Launch OpenFPGA in interactive mode where users type-in command by command and get runtime results

.. option::	--file or -f

  Launch OpenFPGA in script mode where users write commands in scripts and FPGA will execute them

.. option::	--batch_execution or -batch

  Execute OpenFPGA script in batch mode. This option is only valid for script mode.

  - If in batch mode, OpenFPGA will abort immediately when fatal errors occurred.
  - If not in batch mode, OpenFPGA will enter interactive mode when fatal errors occurred.

.. option::	--help or -h
	
  Show the help desk

