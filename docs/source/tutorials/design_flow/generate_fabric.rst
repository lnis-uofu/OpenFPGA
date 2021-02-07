.. _tutorial_generate_fabric:

Generate Fabric Netlists
------------------------

.. note:: You may watch the video representation of this tutorial

.. youtube:: aJ0OkZ1uh68

This tutorial will show an example how to 
  - generate Verilog netlists for a FPGA fabric

.. note:: Before running any design flows, please checkout the tutorial :ref:`tutorial_compile`, to ensure that you have an operating copy of OpenFPGA installed on your computer.


Prepare Task Configuration File
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

OpenFPGA provides push-button scripts for users to run design flows (see details in :ref:`run_fpga_task`). Users can customize their flow-run by crafting a task configuration file.

Here, we consider an existing test case ``generate_fabric``.
In the `task configuration file <https://github.com/lnis-uofu/OpenFPGA/blob/master/openfpga_flow/tasks/basic_tests/generate_fabric/config/task.conf>`_, you can specify the XML-based architecture files in ``LINE 21`` and ``LINE 25``  that describe the architecture of the FPGA fabric. In this example, we are using a low-cost FPGA architecture similar to the lattice ICE40 series

Also, in ``LINE 20``, you can specify the openfpga shell script to be executed. Here, we are using an example script which is golden reference to generate Verilog netlists

.. note:: You can use text editor to customize the configuration file. Here, we use it as is.


Run OpenFPGA Task
~~~~~~~~~~~~~~~~~

After finalizing your configuration file, you can run the task by calling the python script with the given path to task configuration file.

.. code-block:: shell

  python3 openfpga_flow/scripts/run_fpga_task.py basic_tests/generate_fabric 

When the flow run is executed, you can visit the runtime directory and check the Verilog netlists.

Note that your task-run outcomes are stored in the directory called ``latest`` in the same level of your task configuration file.

The Verilog netlists are generated in the following directory

.. code-block:: shell

  ${OPENFPGA_PATH}/openfpga_flow/tasks/basic_tests/generate_fabric/latest/k6_frac_N10_tileable_40nm/and2/MIN_ROUTE_CHAN_WIDTH/SRC

.. note:: ``${OPENFPGA_PATH}`` is the root directory of OpenFPGA 
   
.. note:: See :ref:`fabric_netlists` for the netlist details. 

In the Verilog files, you can validate if the Verilog description is consistent as your definition in the architecture file. The Verilog files can be then used to drive different tools, such as layout generation *etc*.

Run icarus iVerilog Compilation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Go to the directory 

.. code-block:: shell

   cd ${OPENFPGA_PATH}/openfpga_flow/tasks/basic_tests/generate_fabric/latest/k6_frac_N10_tileable_40nm/and2/MIN_ROUTE_CHAN_WIDTH

Compile with iVerilog command:

.. code-block:: shell

  iverilog SRC/fabric_netlists.v

.. note:: Please ensure that iVerilog is installed correctly on your computer

If compilation is successful, you can see a file ``a.out`` in the directory.
