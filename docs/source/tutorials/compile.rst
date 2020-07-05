.. _compile:

How to Compile
--------------

General Guidelines
~~~~~~~~~~~~~~~~~~
OpenFPGA uses CMake to generate the Makefile scripts
In general, please follow the steps to compile

.. code-block:: shell

  git clone https://github.com/LNIS-Projects/OpenFPGA.git
  cd OpenFPGA
  mkdir build
  cd build            
  cmake ..
  make                             

.. note:: OpenFPGA requires gcc/g++ version >5

.. note:: cmake3.12+ is recommended to compile OpenFPGA with GUI

.. note:: recommand to use ``make -j`` to accelerate the compilation

**Quick Compilation Verification**

To quickly verify the tool is well compiled, user can run the following command from OpenFPGA root repository

.. code-block:: shell

  python3 openfpga_flow/scripts/run_fpga_task.py compilation_verification --debug --show_thread_logs

Dependencies
~~~~~~~~~~~~
Full list of dependencies can be found at travis_setup_link_
In particular, OpenFPGA requires specific versions for the following dependencies:

:cmake:
  version >3.12 for graphical interface

:iverilog:
  version 10.1+ is required to run Verilog-to-Verification flow

.. _travis_setup_link: https://github.com/LNIS-Projects/OpenFPGA/blob/0cfb88a49f152aab0a06f309ff160f222bb51ed7/.travis.yml#L34

Docker
~~~~~~
If some of these dependencies are not installed on your machine, you can choose to use a Docker (the Docker tool needs to be installed).
For the ease of the customer first experience, a Dockerfile is provided in the OpenFPGA folder. A container ready to use can be created with the following command

.. code-block:: shell

  docker run lnis/open_fpga:release

.. note:: This command is for quick testing. If you want to conserve your work, you should certainly use other options, such as ``-v``.

Otherwise, a container where you can build OpenFPGA yourself can be created with the following commands

.. code-block:: shell

  docker build . -t open_fpga
  docker run -it --rm -v $PWD:/localfile/OpenFPGA -w="/localfile/OpenFPGA" open_fpga bash

For more information about dock, see dock_download_link_

.. _dock_download_link: https://www.docker.com/products/docker-desktop

To build the tool, go in the OpenFPGA folder and follow the compilation steps

.. note:: Using docker, you cannot use ``make -j``, errors will happen
