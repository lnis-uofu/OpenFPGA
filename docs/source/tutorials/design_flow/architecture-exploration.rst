.. _architecture_exploration:

Architecture Exploration
------------------------

Architecture exploration is generally the first stage of any architecture design, and there are several ways to explore FPGA architectures.
This tutorial will explain how to explore FPGA architecture using application benchmarking.
This tutorial will detail how to evaluate the performance of different architectures for a given set of applications to understand which one is the most suitable.

.. note:: We are not exploring the architecture of the application/design for FPGA implementation, but the architecture of the FPGA itself.


Application Benchmarks
^^^^^^^^^^^^^^^^^^^^^^

The architecture exploration process starts with a set of applications that expects FPGA architecture customization.
In the case of generic FPGA design, this benchmark suit may consist of applications from various domains.
In this tutorial, we selected the following few designs from the `VTR benchmarks`.

#. **ch_intrinsics** : Memory Init
#. **diffeq1** : Arithematic Unit
#. **diffeq2** : Arithematic Unit
#. **sha** : Cryptography

You will find these benchamrks in ``openfpga_flow/benchmarks/vtr_benchmark/`` directory.

Candidate Architectures
^^^^^^^^^^^^^^^^^^^^^^^

The base candidate architectures are selected for evaluating the performance of the above applications and later iteratively analyzed and modified to improve overall performance. These base architectures are (in the case of OpenFPGA) modeled in an architecture XML file. The XML-based description enables the modeling of hundreds of different architectures in a human-readable format. In this tutorial, we will modify the `Flexible Logic Block` (FLE) of the FPGA to understand its impact on the application performance.
The arch0 uses ten 6-input LUTs in each CLB, whereas the arch1 replaces the a fixed LUT-6 with fracturable LUT which can be operated in 1x LUT6 or 2x LUT5 mode.

#. **arch0:** k6_N10_tileable.xml
#. **arch1:** k6_frac_N10_tileable.xml

Create OpenFPGA Task
^^^^^^^^^^^^^^^^^^^^

OpenFPGA works on the task-based structure, where you create a directory with a name of your choice, which consists of the file ``config/task.conf``.
This file contains all the configuration variables (such as architecture, benchmarks, synthesis parameters, flow type etc.).
A ``task`` is executed using the ``run-task`` command

Lets create the OpenFPGA task from the template with above settings. Following code with create a directory/task named ``lab1`` from ``frac-lut-arch-explore`` template.

.. code-block:: bash

    # create-task <new_task_dir_name> <template_name>
    create-task lab1 frac-lut-arch-explore
    tree lab1  # To check content of the copied task
    # lab1/
    # ├── config
    # │   └── task.conf
    # ├── k6_frac_N10_tileable.xml
    # ├── k6_N10_tileable.xml
    # └── vtr_benchmark_template_script.openfpga


task.conf
~~~~~~~~~

``OpenFPGA_SHELL`` section defines common template script used in this flow.

.. code-block:: bash

    [OpenFPGA_SHELL]
    openfpga_shell_template=${PATH:TASK_DIR}/vtr_benchmark_template_script.openfpga
    openfpga_arch_file=${PATH:OPENFPGA_PATH}/openfpga_flow/openfpga_arch/k6_frac_N10_adder_chain_dpram8K_dsp36_40nm_openfpga.xml
    openfpga_sim_setting_file=${PATH:OPENFPGA_PATH}/openfpga_flow/openfpga_simulation_settings/fixed_sim_openfpga.xml
    vpr_route_chan_width=300


.. code-block:: bash

    [ARCHITECTURES]
    arch0=${PATH:TASK_DIR}/k6_N10_tileable_dpram8K_dsp36_40nm.xml
    arch1=${PATH:TASK_DIR}/k6_frac_N10_tileable_adder_chain_dpram8K_dsp36_40nm.xml


.. code-block:: bash

    [BENCHMARKS]
    bench1=${PATH:OPENFPGA_PATH}/openfpga_flow/benchmarks/vtr_benchmark/ch_intrinsics.v
    bench2=${PATH:OPENFPGA_PATH}/openfpga_flow/benchmarks/vtr_benchmark/diffeq1.v
    bench3=${PATH:OPENFPGA_PATH}/openfpga_flow/benchmarks/vtr_benchmark/diffeq2.v
    bench4=${PATH:OPENFPGA_PATH}/openfpga_flow/benchmarks/vtr_benchmark/sha.v


OpenFPGA shell script
~~~~~~~~~~~~~~~~~~~~~

``*.openfpgashell`` script is simialr to TCL script for traditional FPGA CAD tools
If you notice content of ``vtr_benchmark_template_script.openfpga`` file,
it simply excutes the VPR tool. More commands are avaialble :ref:`_openfpga_commands`


.. code-block:: bash

    # Execute VPR for architecture exploration
    vpr ${VPR_ARCH_FILE} ${VPR_TESTBENCH_BLIF} \
        --route_chan_width ${VPR_ROUTE_CHAN_WIDTH} \
        --constant_net_method route
    exit

Architecture Files
~~~~~~~~~~~~~~~~~~


Run OpenFPGA Task
^^^^^^^^^^^^^^^^^

.. code-block:: bash

    run-task lab1 # To excute the task


Execute OpenFPGA Task
^^^^^^^^^^^^^^^^^^^^^


Analyze Results
^^^^^^^^^^^^^^^

.. code-block:: bash

    column -t -s, lab1/latest/task_result.csv | less -S


.. code-block:: csv

   name                   ,TotalRunTime , clb_blocks ,total_wire_length
   00_memset_             ,           1 ,         31 ,             2217
   01_memset_             ,           6 ,         25 ,             2120
   00_diffeq_paj_convert_ ,          16 ,        368 ,            43526
   01_diffeq_paj_convert_ ,          45 ,        276 ,            38465
   00_diffeq_f_systemC_   ,          16 ,        354 ,            37245
   01_diffeq_f_systemC_   ,          42 ,        262 ,            32722
   00_sha1_               ,           9 ,        168 ,            16099
   01_sha1_               ,          14 ,        153 ,            15274
