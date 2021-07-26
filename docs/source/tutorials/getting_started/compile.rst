.. _tutorial_compile:

How to Compile
--------------

.. note:: We recommend you to watch a tutorial `video <https://youtu.be/F9sMRmDewM0>`_ about how-to-compile before getting started

.. only:: html

  .. youtube:: F9sMRmDewM0

General Guidelines
~~~~~~~~~~~~~~~~~~
OpenFPGA uses CMake to generate the Makefile scripts.
In general, please follow the steps to compile

.. code-block:: shell

  git clone https://github.com/LNIS-Projects/OpenFPGA.git
  cd OpenFPGA
  make all

.. note:: OpenFPGA requires gcc/g++ version >5

.. note:: cmake3.12+ is recommended to compile OpenFPGA with GUI

.. note:: Recommend using ``make -j<int>`` to accelerate the compilation, where ``<int>`` denotes the number of cores to be used in compilation.

.. note:: VPR's GUI requires gtk-3, and can be enabled with ``cmake .. -DVPR_USE_EZGL=on``

**Quick Compilation Verification**

To quickly verify the tool is well compiled, users can run the following command from OpenFPGA root repository

.. code-block:: shell

  python3 openfpga_flow/scripts/run_fpga_task.py compilation_verification --debug --show_thread_logs

Dependencies
~~~~~~~~~~~~
Full list of dependencies can be found at install_dependencies_build_.
In particular, OpenFPGA requires specific versions for the following dependencies:

:cmake:
  version >3.12 for graphical interface

:iverilog:
  version 10.1+ is required to run Verilog-to-Verification flow
  
:envyaml:
  python package envyaml is required:
  
  python3 -m pip install envyaml

.. _install_dependencies_build: https://github.com/lnis-uofu/OpenFPGA/blob/master/.github/workflows/install_dependencies_build.sh


Running with the docker image
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Users can skip the traditional installation process by using the Dockerized version
of the OpenFPGA tool. The OpenFPGA project maintains the docker image/Github package of
the latest stable version of OpenFPGA in the following repository
`openfpga-master <https://github.com/orgs/lnis-uofu/packages/container/package/openfpga-master>`_.
This image contains precompiled OpenFPGA binaries with all prerequisites installed.

.. code-block:: bash

   # To get the docker image from the repository, docker pull ghcr.io/lnis-uofu/openfpga-master:latest

   # To invoke openfpga_shell
   docker run -it ghcr.io/lnis-uofu/openfpga-master:latest openfpga/openfpga -i

   # To run the task that already exists in the repository.
   docker run -it ghcr.io/lnis-uofu/openfpga-master:latest bash -c "source openfpga.sh && run-task compilation_verification"

   # To run a task from a local machine
   mkdir <<task_name>>/config
   vi <<task_name>>/config/task.config # Create your task configuration
   TASK_NAME=<<task_name>> docker run -it -v ${PWD}/${TASK_NAME}:/opt/openfpga/openfpga_flow/tasks/${TASK_NAME} ghcr.io/lnis-uofu/openfpga-master:latest bash -c "source openfpga.sh && run-task ${TASK_NAME}"

.. note::
   While running local task using docker, make sure all the additional files
   are maintained in the task_directory and reference using variable ${TASK_DIR}
