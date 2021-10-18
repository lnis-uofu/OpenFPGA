OpenFPGA shortcuts
------------------

OpenFPGA provides bash/zsh shell-based shortcuts to perform all essential functions and navigating through the directories. Go to the OpenFPGA directory and source ``openfpga.sh``

.. code-block:: bash

   cd ${OPENFPGA_PATH} && source openfpga.sh

.. note::
    The OpenFPGA shortcuts are designed to work with only bash based shells. e.g. bash/zsh/fish etc.

Shortcut Commands
^^^^^^^^^^^^^^^^^

Once the ``openfpga.sh`` script is sourced, you can run any of the following commands directly in the terminal.

.. option:: list-tasks

   This command lists all the OpenFPGA tasks from the current task directory.
   default task directory is considered as ``${OPENFPGA_PATH}/openfpga_flow/tasks``

.. option:: run-task <task_name> **kwarags

   This command runs the specified task listed from the ``list-task`` command or from the existing directory. The command name is relative to the ``TASK_DIRECTORY``. Users can provide any additional arguments which are listed `here <_openfpga_task_args>`_ to this command.

.. option:: run-modelsim

   This command runs the verification using ModelSim.
   The test benches are generated during the OpenFPGA run.
   **Note**: users need to have ``VSIM`` installed and configured


.. option:: run-regression-local

   This script runs the regression test locally using the current version of OpenFPGA.
   **NOTE** Important before making a pull request to the master

.. option:: unset-openfpga

   Unregisters all the shortcuts and commands from the current shell session

