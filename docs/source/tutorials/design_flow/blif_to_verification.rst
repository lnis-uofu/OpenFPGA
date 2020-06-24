.. _from_blif_to_verification:

From BLIF to Verification
-------------------------

This tutorial will show an example how to 
  - generate Verilog netlists for a FPGA fabric
  - generate Verilog testbenches for a RTL design
  - run HDL simulation to verify the functional correctness of the implemented FPGA fabric

Netlist Generation
~~~~~~~~~~~~~~~~~~
We will use the openfpga_flow scripts (see details in :ref:`run_fpga_task`) to generate the Verilog netlists and testbenches.
Here, we consider a representative but fairly simple FPGA architecture, which is based on 4-input LUTs.
We will map a 2-input AND gate to the FPGA fabric, and run a full testbench (see details in :ref:`fpga_verilog_testbench`)

We will simply execute the following openfpga task-run by 

.. code-block:: shell

  python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/full_testbench/configuration_chain

Detailed settings, such as architecture XML files and RTL designs, can be found at ``${OPENFPGA_PATH}/openfpga_flow/tasks/openfpga_shell/full_testbench/configuration_chain/config/task.conf``.

.. note:: ``${OPENFPGA_PATH}`` is the root directory of OpenFPGA 

After this task-run, you can find all the generated netlists and testbenches at  

.. code-block:: shell

  ${OPENFPGA_PATH}/openfpga_flow/tasks/openfpga_shell/full_testbench/configuration_chain/latest/k4_N4_tileable_40nm/and2/MIN_ROUTE_CHAN_WIDTH/SRC/
   
.. note:: See :ref:`fabric_netlists` and :ref:`fpga_verilog_testbench` for the netlist details. 

Run icarus iVerilog Simulation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Through OpenFPGA Scripts
^^^^^^^^^^^^^^^^^^^^^^^^

By default, the ``configuration_chain`` task-run will execute iVerilog simulation automatically.
The simulation results are logged in 

.. code-block:: shell

  ${OPENFPGA_PATH}/openfpga_flow/tasks/openfpga_shell/full_testbench/configuration_chain/latest/k4_N4_tileable_40nm/and2/MIN_ROUTE_CHAN_WIDTH/vvp_sim_output.txt

If the verification passed, you should be able to see ``Simulation Succeed`` in the log file.

All the waveforms are stored in the ``and2_formal.vcd`` file.
To visualize the waveforms, you can use the `GTKWave
<http://gtkwave.sourceforge.net/>`_.

.. code-block:: shell

  gtkwave ${OPENFPGA_PATH}/openfpga_flow/tasks/openfpga_shell/full_testbench/configuration_chain/latest/k4_N4_tileable_40nm/and2/MIN_ROUTE_CHAN_WIDTH/and2_formal.vcd &

Manual Method
^^^^^^^^^^^^^

If you want to run iVerilog simulation manually, you can follow these steps:

.. code-block:: shell

  cd ${OPENFPGA_PATH}/openfpga_flow/tasks/openfpga_shell/full_testbench/configuration_chain/latest/k4_N4_tileable_40nm/and2/MIN_ROUTE_CHAN_WIDTH

  source iverilog_output.txt
  
  vvp compiled_and2

Debugging Tips
^^^^^^^^^^^^^^

If you want to apply full visibility to the signals, you need to change the following line in 

.. code-block:: shell 

  ${OPENFPGA_PATH}/openfpga_flow/tasks/openfpga_shell/full_testbench/configuration_chain/latest/k4_N4_tileable_40nm/and2/MIN_ROUTE_CHAN_WIDTH/SRC/and2_autocheck_top_tb.v
   
from 

.. code-block:: shell

  $dumpvars (1, and2_autocheck_top_tb);

to 

.. code-block:: shell

  $dumpvars (12, and2_autocheck_top_tb);
   

Run Modelsim Simulation
~~~~~~~~~~~~~~~~~~~~~~~
Alternatively, you can run Modelsim simulations through openfpga_flow scripts or manually.

Through OpenFPGA Scripts
^^^^^^^^^^^^^^^^^^^^^^^^
You can simply call the python script in the following line:

.. code-block:: shell

  python3 openfpga_flow/scripts/run_modelsim.py openfpga_shell/full_testbench/configuration_chain --run_sim

The script will automatically create a Modelsim project at  

.. code-block:: shell

  ${OPENFPGA_PATH}/openfpga_flow/tasks/openfpga_shell/full_testbench/configuration_chain/latest/k4_N4_tileable_40nm/and2/MIN_ROUTE_CHAN_WIDTH/MSIM2/

and run the simulation.

You may open the project and visualize the simulation results.

Manual Method
^^^^^^^^^^^^^

Modify the ``fpga_defines.v`` (see details in :ref:`fabric_netlists`) at 

.. code-block:: shell

  ${OPENFPGA_PATH}/openfpga_flow/tasks/openfpga_shellfull_testbench//configuration_chain/latest/k4_N4_tileable_40nm/and2/MIN_ROUTE_CHAN_WIDTH/SRC/

by **deleting** the line 

.. code-block:: shell

  `define ICARUS_SIMULATOR 1

Create a folder ``MSIM`` under

.. code-block:: shell

  ${OPENFPGA_PATH}/openfpga_flow/tasks/openfpga_shell/full_testbench/configuration_chain/latest/k4_N4_tileable_40nm/and2/MIN_ROUTE_CHAN_WIDTH/

Under the ``MSIM`` folder, create symbolic links to ``SRC`` folder and reference benchmarks by

.. code-block:: shell

  ln -s ../SRC ./ 

  ln -s ../and2_output_verilog.v ./

.. note:: Depending on the operating system, you may use other ways to create the symbolic links

Launch ModelSim under the ``MSIM`` folder and create a project by following Modelsim user manuals.

Add the following file to your project:

.. code-block:: shell

  ${OPENFPGA_PATH}/openfpga_flow/tasks/openfpga_shell/full_testbench/configuration_chain/latest/k4_N4_tileable_40nm/and2/MIN_ROUTE_CHAN_WIDTH/SRC/and2_include_netlists.v

Compile the netlists, create a simulation configuration and specify ``and2_autocheck_top_tb`` at the top unit.

Execute simulation with ``run -all``
You should see ``Simulation Succeed`` in the output log.
