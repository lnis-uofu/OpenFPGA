.. _tutorial_compile:

Installation
------------

Compiling Source
~~~~~~~~~~~~~~~~

.. note:: We recommend you to watch a tutorial `video <https://youtu.be/F9sMRmDewM0>`_ about how-to-compile before getting started

.. only:: html

  .. youtube:: F9sMRmDewM0

Supported Operating Systems
^^^^^^^^^^^^^^^^^^^^^^^^^^^

OpenFPGA is continously tested with Ubuntu 20.04 and partially on Ubuntu 22.04
It might work with earlier versions and other distributions.

In addition to continous integration, our community users have tested OpenFPGA on their local machines using the following operating systems:

- CentOS 7.8
- CentOS 8
- Ubuntu 18.04
- Ubuntu 21.04
- Ubuntu 22.04

Build Steps
^^^^^^^^^^^

OpenFPGA uses CMake to generate the Makefile scripts.
In general, please follow the steps to compile

.. code-block:: shell

  git clone https://github.com/LNIS-Projects/OpenFPGA.git
  cd OpenFPGA
  make all

.. note:: OpenFPGA requires gcc/g++ version > 7 and clang version > 6.

.. note:: cmake3.12+ is recommended to compile OpenFPGA with GUI

.. note:: Recommend using ``make -j<int>`` to accelerate the compilation, where ``<int>`` denotes the number of cores to be used in compilation.

.. note:: VPR's GUI requires gtk-3, and can be enabled with ``make .. CMAKE_FLAGS="-DVPR_USE_EZGL=on"``

**Quick Compilation Verification**

.. note:: Ensure that you install python dependences in :ref:`tutorial_compile_dependencies`.

To quickly verify the tool is well compiled, users can run the following command from OpenFPGA root repository

.. code-block:: shell

  python3 openfpga_flow/scripts/run_fpga_task.py compilation_verification --debug --show_thread_logs

.. _tutorial_compile_build_options:

Build Options
^^^^^^^^^^^^^

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
  - ``DOPENFPGA_WITH_VERSION=[ON|OFF]``: Enable/Disable the build of version number. When disabled, version number will be displayed as an empty string.
  - ``DOPENFPGA_WITH_SWIG=[ON|OFF]``: Enable/Disable the build of SWIG, which is required for integrating to high-level interface.
  - ``OPENFPGA_ENABLE_STRICT_COMPILE=[ON|OFF]``: Specifies whether compiler warnings should be treated as errors (e.g. -Werror)

.. warning:: By default, only required modules in *Verilog-to-Routing* (VTR) is enabled. On other words, ``abc``, ``odin``, ``yosys`` and other add-ons inside VTR are not built. If you want to enable them, please look into the dedicated options of CMake scripts.

.. option:: CMAKE_GOALS=<string>

  Specify the build target for CMake system. For example, ``cmake_goals=openfpga`` indicates that only openfpga binary will be compiled. For a detailed list of targets, use ``make list_cmake_targets`` to show. By default, all the build targets will be included.

.. _tutorial_compile_dependencies:

Dependencies
^^^^^^^^^^^^

Dependencies can be installed upon the use of OpenFPGA on different systems
In general, OpenFPGA requires specific versions for the following dependencies:

:cmake:
  version >3.12 for graphical interface

:iverilog:
  version 10.3+ is required to run Verilog-to-Verification flow


Ubuntu 20.04
^^^^^^^^^^^^

- Dependencies required to build the code base

.. include:: ubuntu20p04_dependencies.sh
  :code: shell

- Dependencies required to run regression tests

.. include:: regtest_dependencies.sh
  :code: shell

.. note:: Python packages are also required

.. code-block::

  python3 -m pip install -r requirements.txt

- Dependencies required to build documentation

.. include:: doc_dependencies.sh
  :code: shell

Ubuntu 22.04
^^^^^^^^^^^^

- Dependencies required to build the code base

.. include:: ubuntu22p04_dependencies.sh
  :code: shell

- Dependencies required to run regression tests

.. include:: regtest_dependencies.sh
  :code: shell

.. note:: Python packages are also required

.. code-block::

  python3 -m pip install -r requirements.txt

- Dependencies required to build documentation

.. include:: doc_dependencies.sh
  :code: shell


Running Docker Image
~~~~~~~~~~~~~~~~~~~~

Users can skip the traditional installation process by using the Dockerized version
of the OpenFPGA tool. T
he OpenFPGA project maintains the docker image/Github package of every commit pushed to the `master` branch.
You can find full list of avaialble packages
`openfpga-master <https://github.com/orgs/lnis-uofu/packages/container/package/openfpga-master>`_.
To use most recent version please use :latest tag (as shown in the following source) or you can pin to specific version by using tag `ghcr.io/lnis-uofu/openfpga-master:v1.1.541` or to specific master commit using short first 8 characters of commit SHA `ghcr.io/lnis-uofu/openfpga-master:62ec82c1`.
This image contains precompiled OpenFPGA binaries with all prerequisites already installed.

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



Running on Binder
~~~~~~~~~~~~~~~~~

If user want to quickly evaluate the OpenFPGA functionality without any local installation, you can use the Binder service to quickly provision a low-end compute unit with docker and visual studio code.
Please click the following link to launch the Binder instance.

.. image:: https://mybinder.org/badge_logo.svg
   :target: https://mybinder.org/v2/gh/lnis-uofu/OpenFPGA/master?urlpath=vscode

.. note:: The storage on Binder session is not permanent and the session is completely removed if inactive. Make sure yopu download or commit files if there are any important changes.
