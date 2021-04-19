Integrating Custom Verilog Modules with user_defined_template.v
================================================================
Introduction and Setup
~~~~~~~~~~~~~~~~~~~~~~
**In this tutorial, we will**
   - Provide the motivation for generating the user_defined_template.v verilog file
   - Go through a generated user_defined_template.v file to demonstrate how to use it
Through this tutorial, we will show how and when to use the :ref:`cmdoption-arg-user_defined_templates.v` file.

To begin the tutorial, we start with a modified version of the hard adder task that comes with OpenFPGA.
To follow along, go to the root directory of OpenFPGA and enter: 

.. code-block:: bash

   vi openfpga_flow/openfpga_arch/k6_frac_N10_adder_chain_40nm_openfpga.xml 

Go to **LINE187** and replace **LINE187** with:

.. code-block:: XML

   <circuit_model type="hard_logic" name="ADDF" prefix="ADDF" is_default="true" spice_netlist="${OPENFPGA_PATH}/openfpga_flow/openfpga_cell_library/spice/adder.sp"     verilog_netlist="">

Motivation
~~~~~~~~~~
From the OpenFPGA root directory, run the command:

.. code-block:: bash

   python3 openfpga_flow/scripts_run_fpga_task.py fpga_verilog/adder/hard_adder --debug --show_thread_logs

Running this command should fail and produce the following errors:

.. code-block:: bash
   
   ERROR - iverilog_verification run failed with returncode 21
   ERROR - command iverilog -o compiled_and2 ./SRC/and2_include_netlists.v -s and2_top_formal_verification_random_tb
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>././SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v:50: error: Unknown module type: ADDF
   ERROR - -->>21 error(s) during elaboration.
   ERROR - Current working directory :    /research/ece/lnis/USERS/leaptrot/OpenFPGA/openfpga_flow/tasks/fpga_verilog/adder/hard_adder/run019/k6_frac_N10_tileable_adder_chain_40nm/and2/MIN_ROUTE_CHAN_WIDTH
   ERROR - Failed to run iverilog_verification task
   ERROR - Exiting . . . . . .
This error log can also be found by running the following command from the root directory:

.. code-block:: bash

   cat openfpga_flow/tasks/fpga_verilog/adder/hard_adder/latest/00_and2_MIN_ROUTE_CHAN_WIDTH_out.log

This command failed during the verification step because the path to the module definition for **ADDF** is missing. In our architecture file, user-defined verilog modules are those ``<circuit_model>`` with the key term `verilog_netlist`. The ``user_defined_template.v`` file provides a module template for incorporating Hard IPs without external library into the architecture. 

Fixing the Error
~~~~~~~~~~~~~~~~
This error can be resolved by replacing the **LINE187** of ``k6_frac_N10_adder_chain_40nm_openfpga.xml`` with the following:

.. code-block:: XML

   <circuit_model type="hard_logic" name="ADDF" prefix="ADDF" is_default="true" spice_netlist="${OPENFPGA_PATH}/openfpga_flow/openfpga_cell_library/spice/adder.sp"     verilog_netlist="${OPENFPGA_PATH}/openfpga_flow/openfpga_cell_library/verilog/adder.v">

The above line provides a path to generate the ``user_defined_template.v`` file. 
Now we can return to the root directory and run this command again:

.. code-block:: bash

   python3 openfpga_flow/scripts_run_fpga_task.py fpga_verilog/adder/hard_adder --debug --show_thread_logs
   
The task should now complete without any errors.

Fixing the Error with user_defined_template.v
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The :ref:`cmdoption-arg-user_defined_templates.v` file can be found starting from the root directory and entering:

.. code-block:: bash

   vi openfpga_flow/tasks/fpga_verilog/adder/hard_adder/latest/k6_frac_N10_tileable_adder_chain_40nm/and2/MIN_ROUTE_CHAN_WIDTH/SRC/sub_module/user_defined_template.v

.. note:: The ``user_defined_template.v`` file contains user-defined verilog modules that are found in the openfpga_cell_library with ports declaration (compatible with other netlists that are auto-generated by OpenFPGA) but without functionality. ``user_defined_template.v`` is used as a reference for engineers to check what is the port sequence required by top-level verilog netlists. ``user_defined_template.v`` can be included in simulation only if there are modifications to the ``user_defined_template.v``.

To implement our own **ADDF** module, we need to remove all other module definitions (they are already defined elsewhere and will cause an error if left in). Replace the ``user_defined_template.v`` file with the following:

.. code-block:: Verilog

   //-------------------------------------------
   //      FPGA Synthesizable Verilog Netlist
   //      Description: Template for user-defined Verilog modules
   //      Author: Xifan TANG
   //      Organization: University of Utah
   //      Date: Fri Mar 19 10:05:32 2021
   //-------------------------------------------
   //----- Time scale -----
   `timescale 1ns / 1ps
   
   
   
   // ----- Template Verilog module for ADDF -----
   //----- Default net type -----
   `default_nettype none

   // ----- Verilog module for ADDF -----
   module ADDF(A,
               B,
               CI,
               SUM,
               CO);
   //----- INPUT PORTS -----
   input [0:0] A;
   //----- INPUT PORTS -----
   input [0:0] B;
   //----- INPUT PORTS -----
   input [0:0] CI;
   //----- OUTPUT PORTS -----
   output [0:0] SUM;
   //----- OUTPUT PORTS -----
   output [0:0] CO;

   //----- BEGIN wire-connection ports -----
   //----- END wire-connection ports -----


   //----- BEGIN Registered ports -----
   //----- END Registered ports -----

   // ----- Internal logic should start here -----
      assign SUM = A ^ B ^ CI;
      assign CO  = (A & B) | (A & CI) | (B & CI);
   // ----- Internal logic should end here -----
   endmodule
   // ----- END Verilog module for ADDF -----

We can now link this ``user_defined_template.v`` into ``k6_frac_N10_adder_chain_40nm_openfpga.xml``.

.. note:: Be sure to select the run where you modified the ``user_defined_template.v``!

From the OpenFPGA root directory, run:

.. code-block:: bash

   vi openfpga_flow/openfpga_arch/k6_frac_N10_adder_chain_40nm_openfpga.xml

At **LINE187** in verilog_netlist, put in:

.. code-block:: XML

   ${OPENFPGA_PATH}/openfpga_flow/tasks/fpga_verilog/adder/hard_adder/**YOUR_RUN_NUMBER**/k6_frac_N10_tileable_adder_chain_40nm/and2/MIN_ROUTE_CHAN_WIDTH/SRC/sub_module/user_defined_template.v

Finally, rerun this command from the OpenFPGA root directory to ensure it is working:

.. code-block:: bash

   python3 openfpga_flow/scripts_run_fpga_task.py fpga_verilog/adder/hard_adder --debug --show_thread_logs


