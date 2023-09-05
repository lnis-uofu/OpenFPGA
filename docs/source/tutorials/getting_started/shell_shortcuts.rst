OpenFPGA Shell Commands
-----------------------

OpenFPGA provides `bash`/`zsh` shell-based shortcuts to perform all essential functions and navigate through the directories. Go to the OpenFPGA directory and source ``openfpga.sh``,

.. code-block:: bash

   export OPENFPGA_PATH=<path-to-openfpga-repository-root>
   cd ${OPENFPGA_PATH} && source openfpga.sh

.. note::
    The OpenFPGA shortcut works with only a bash-like shell. e.g., `bash`/`zsh`/`fish,` etc.

Commands
^^^^^^^^

Once the ``openfpga.sh`` script is sourced, you can run any following commands directly in the terminal.

.. option:: list-tasks

   This command lists all the OpenFPGA tasks from the current task directory.
   default task directory is considered as ``${OPENFPGA_PATH}/openfpga_flow/tasks``

.. option:: run-task <task_name> **kwarags

   This command runs the specified task. The script will first look for the task in the current working directory.
   If it is not in the current directory, it will then search in ``TASK_DIRECTORY`` (relative to task directory).
   You can also provide a path as a task_name, for example, ``run-task basic_tests/generate_fabric``
   The valid arguments listed here <_openfpga_task_args>`_, you can also run `run-task run-task` to get the list of command-line arguments.


.. option:: create-task <task_name> <template>

   It creates a template task in the current directory with the given task_name.
   the template is an optional argument; there are two templates currently configured
   - ``vpr_blif``: A template task for running flow with `.blif` file as an input (VPR + Netlist generation)
   - ``yosys_vpr``: A template task for running flow with `.v` file as an input (Synthesis + VPR + Netlist generation)
   you can also use this command to copy any example project; use a ``list-tasks`` command to get the list of example projects
   for example  ``create-task _my_task_copy basic_tests/generate_fabric`` create a copy of the ``basic_tests/generate_fabric`` task in
   the current directory with  ``_my_task_copy`` name.

.. option:: goto_task <task_name> <run_num[default 0]>

   This command navigate shell to specific run-directory of the given task.
   For example `goto_task lab1 2` will change directory to `run002` runt directory of `lab2`

.. option:: clear-task-run <task_name>

   Clears all run directories of the given task

.. option:: run-modelsim <task_name>

   This command runs the verification using ModelSim.
   The test benches are generated during the OpenFPGA run.
   **Note**: users need to have ``VSIM`` installed and configured

.. option:: run-regression-local

   This script runs the regression test locally using the current version of OpenFPGA.
   **NOTE** Important before making a pull request to the master

.. option:: unset-openfpga

   Unregisters all the shortcuts and commands from the current shell session

