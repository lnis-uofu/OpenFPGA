.. _tutorial_compile:

How to Compile
--------------

.. note:: We recommend you to watch a tutorial `video <https://youtu.be/F9sMRmDewM0>`_ about how-to-compile before getting started

.. only:: html

  .. youtube:: F9sMRmDewM0

Supported Operating Systems
~~~~~~~~~~~~~~~~~~~~~~~~~~~

OpenFPGA is continously tested with Ubuntu 22.04, Ubuntu 24.04 and CentOS 9
It might work with earlier versions and other distributions.

In addition to continous integration, our community users have tested OpenFPGA on their local machines using the following operating systems:

- CentOS 7.8
- CentOS 7.9
- CentOS 8
- Ubuntu 18.04
- Ubuntu 21.04
- Ubuntu 20.04 

Build Steps
~~~~~~~~~~~
OpenFPGA uses CMake to generate the Makefile scripts.
In general, please follow the steps to compile

.. code-block:: shell

  git clone https://github.com/LNIS-Projects/OpenFPGA.git
  cd OpenFPGA
  make all

.. note:: OpenFPGA requires gcc/g++ version >= 11 and clang version >= 17.

.. note:: cmake3.12+ is recommended to compile OpenFPGA with GUI

.. note:: Recommend using ``make -j<int>`` to accelerate the compilation, where ``<int>`` denotes the number of cores to be used in compilation.

.. note:: VPR's GUI requires gtk-3, and can be enabled with ``make .. CMAKE_FLAGS="-DVPR_USE_EZGL=on"``

**Quick Compilation Verification**

.. note:: Ensure that you install python dependences in :ref:`tutorial_compile_dependencies`.

To quickly verify the tool is well compiled, users can run the following command from OpenFPGA root repository

.. code-block:: shell

  python3 openfpga_flow/scripts/run_fpga_task.py compilation_verification --debug --show_thread_logs

If you would like to create an installer, you may run

.. code-block:: shell

  # Refer to build options for installer customization
  make installer 

.. _tutorial_compile_build_options:

Build Options
~~~~~~~~~~~~~

General build targets are available in the top-level makefile. Call help desk to see details

.. code-block:: shell

  make help

The following options are available for a custom build

.. option:: BUILD_TYPE=<string>

  Specify the type of build. Can be either ``release`` or ``debug``. By default, release mode is selected (full optimization on runtime)

.. option:: PACK_INSTALLER=[ON|OFF]

  Specify if the installer packages should be included when compiling. When enabled, installer packaging will be enabled in CPack format. Use ``make installer`` to create the final package.

.. option:: INSTALL_DOC=[ON|OFF]

  Specify if the installer packages should include the documentation. By default, it is ON. Ensure that you have build the doc locally before creating the installer 

.. option:: INSTALLER_TYPE=[STGZ|DEB|IFW]

  Specify type of installer. By default, it is ``STGZ``. If you prefer an installer with graphical user interface, please choose ``IFW``.

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
~~~~~~~~~~~~

Dependencies can be installed upon the use of OpenFPGA on different systems
In general, OpenFPGA requires specific versions for the following dependencies:

:cmake:
  version >3.12 for graphical interface

:iverilog:
  version 10.3+ is required to run Verilog-to-Verification flow
  

Ubuntu
^^^^^^

If your OS is Ubuntu 20.04, 22.04 and 24.04, we offer unified scripts to install all the dependencies.

- Dependencies required to build the code base

.. include:: ubuntu_dependencies.sh
  :code: shell

- Dependencies required to run regression tests

.. include:: ubuntu_regtest_dependencies.sh
  :code: shell
  
.. note:: Python packages are also required. If you are using Ubuntu 24.04, the required Python version is <= 3.12.3
  
.. code-block::

  python3 -m pip install -r requirements.txt

- Dependencies required to build documentation

.. include:: doc_dependencies.sh
  :code: shell

CentOS
^^^^^^

If your OS is CentOS 7.9, we offer the script to install all the dependencies.

.. include:: centos7p9_dependencies.sh
  :code: shell

If your OS is CentOS 9, we offer the script to install all the dependencies.

.. include:: centos9_dependencies.sh
  :code: shell

Running with pre-built docker image
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

