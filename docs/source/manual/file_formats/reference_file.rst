.. _file_format_reference_file:

Reference File (.yaml)
----------------------------------------

This file is generated by command :ref:`openfpga_setup_commands_report_reference`


The reference file aims to the show reference number of each child module of given parent module

By using the options of the command :ref:`openfpga_setup_commands_report_reference`, user can selectively output the reference info under the given parent module on their needs.

An example of the file is shown as follows.

.. code-block:: yaml

  Date: Mon Sep  9 16:41:53 2024

  #the instance names are given during netlist generation

  references:
    - module: grid_io_top
      count: 1
      instances:
        - grid_io_top_1__2_
    - module: grid_io_right
      count: 1
      instances:
        - grid_io_right_2__1_
    - module: grid_io_bottom
      count: 1
      instances:
        - grid_io_bottom_1__0_
    - module: grid_io_left
      count: 1
      instances:
        - grid_io_left_0__1_
    - module: grid_clb
      count: 1
      instances:
        - grid_clb_1__1_
    - module: sb_0__0_
      count: 1
      instances:
        - sb_0__0_
    - module: sb_0__1_
      count: 1
      instances:
        - sb_0__1_
    - module: sb_1__0_
      count: 1
      instances:
        - sb_1__0_
    - module: sb_1__1_
      count: 1
      instances:
        - sb_1__1_
    - module: cbx_1__0_
      count: 1
      instances:
        - cbx_1__0_
    - module: cbx_1__1_
      count: 1
      instances:
        - cbx_1__1_
    - module: cby_0__1_
      count: 1
      instances:
        - cby_0__1_
    - module: cby_1__1_
      count: 1
      instances:
        - cby_1__1_

In this example, the parent module is ``fpga_top``.
The child modules under ``fpga_top`` are ``grid_io_top``, ``grid_io_right``, and etc.

The instance of the child module ``grid_io_top`` is shown as a list as below:
    - grid_io_top_1__2_

