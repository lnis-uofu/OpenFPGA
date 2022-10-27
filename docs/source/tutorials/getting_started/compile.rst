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

.. note:: Ensure that you install python dependences in :ref:`tutorial_compile_dependencies`.

To quickly verify the tool is well compiled, users can run the following command from OpenFPGA root repository

.. code-block:: shell

  python3 openfpga_flow/scripts/run_fpga_task.py compilation_verification --debug --show_thread_logs

.. _tutorial_compile_build_options:

Build Options
~~~~~~~~~~~~~

General build targets are available in the top-level makefile. Call help desk to see details

.. code-block:: shell

  make help

The following options are available for a custom build

.. option:: BUILD_TYPE=<string>

  Specify the type of build. Can be either ``release`` or ``debug``. By default, release mode is selected (full optimization on runtime)

.. option:: CMAKE_FLAGS=<string>

  Force build flags to CMake. The following flags are available

  - ``DOPENFPGA_WITH_TEST=[ON|OFF]``: Enable/Disable the test build
  - ``DOPENFPGA_WITH_YOSYS=[ON|OFF]``: Enable/Disable the build of yosys. Note that when disabled, the build of yosys-plugin is also disabled
  - ``DOPENFPGA_WITH_YOSYS_PLUGIN=[ON|OFF]``: Enable/Disable the build of yosys-plugin.

.. warning:: By default, only required modules in *Verilog-to-Routing* (VTR) is enabled. On other words, ``abc``, ``odin``, ``yosys`` and other add-ons inside VTR are not built. If you want to enable them, please look into the dedicated options of CMake scripts.  

.. _tutorial_compile_dependencies:

Dependencies
~~~~~~~~~~~~
Full list of dependencies can be found at install_dependencies_build_.
In particular, OpenFPGA requires specific versions for the following dependencies:

:cmake:
  version >3.12 for graphical interface

:iverilog:
  version 10.1+ is required to run Verilog-to-Verification flow
  
:python dependencies:
  python packages are also required:
  
.. code-block::

  python3 -m pip install -r requirements.txt


.. _install_dependencies_build: https://github.com/lnis-uofu/OpenFPGA/blob/master/.github/workflows/install_dependencies_build.sh


Running with the docker image
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Users can skip the traditional installation process by using the Dockerized version
of the OpenFPGA tool. The OpenFPGA project maintains the docker image/Github package of
the latest stable version of OpenFPGA in the following repository
`openfpga-master <https://github.com/orgs/lnis-uofu/packages/container/package/openfpga-master>`_.
This image contains precompiled OpenFPGA binaries with all prerequisites installed.

.. code-block:: bash

   # To get the docker image from the repository, 
   docker pull ghcr.io/lnis-uofu/openfpga-master:latest

   # To invoke openfpga_shell
   docker run -it ghcr.io/lnis-uofu/openfpga-master:latest openfpga/openfpga bash

   # To run the task that already exists in the repository.
   docker run -it ghcr.io/lnis-uofu/openfpga-master:latest bash -c "source openfpga.sh && run-task compilation_verification"

   # To link the local directory wihth docker
   mkdir work

   docker run -it -v work:/opt/openfpga/ ghcr.io/lnis-uofu/openfpga-master:latest bash
   # Inside container 
   source openfpga.sh
   cd work 
   create_task _my_task yosys_vpr

