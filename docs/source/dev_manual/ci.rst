.. _developer_ci:

Continous Integration
=====================

Motivation
----------

Continous Integration (CI) systems are built to ensure that input and output files of each teams are

- Correct
- Reproducable
- Consistent with other teams

CI system is automatically triggered on 

- Main branch: the master branch of the codebase
- A pull request on main branch

Workflows
---------

Principles
^^^^^^^^^^

Continous Integration system consists a number of workflows, each of which is designed to validate a specific aspect of the codebase.
For the work of each team, there is at least 1 dedicated workflow.

Workflows can categorized in two types

.. option:: Generation flow

  Such type of workflow is designed to ensure that golden files (netlists, bitstreams, etc.) are reproduciable.
  A generation workflow consists of three steps:

  - Detect changes on input files, e.g., architecture files, IPs and related scripts. 
  
    - If no changes detected, the workflow ends, since the golden outputs are not changed in a pull request
    - If any changes are detected, the workflow will continue to the next steps
  
  - Regenerate golden files by calling scripts. By the end of this step, it will compare the newly generated files with the golden reference (current branch)
    - If there are no changes, the workflow ends.
    - If any changes on golden reference are detected, this will error out. It means that the current golden reference are not reproduciable. 
  
  .. warning:: If any changes on golden references are detected, code review has to be enforced. Ensure that all the teams impacted agree on the changes.

.. option:: Validation flow

  Such type of workflow is designed to verify the correctness of golden files
  A validation workflow consists of three steps:

  - Detect changes on golden reference (some pull requests update golden references) 
  
    - If no changes detected, the workflow ends. There is no need to validate the correctness of the golden reference (previous pull request should already do so).
    - If any changes are detected, the workflow will continue to the next steps
  
  - Run validation by calling scripts. For example, verification may call HDL simulations to verify the correctness of netlists.
    - If the new golden reference passes all the tests, this will end. 
    - If the new golden reference fails any test, this will error out. It means that the current golden reference can not meet basic requirements. 
  
  .. warning:: If any validation flow failed, the pull request cannot be merged in general.

.. _developer_ci_workflow_check_tool_version:

Check Tool Version
^^^^^^^^^^^^^^^^^^

The workflow aims to validate the following:

- All the tools meet the expected versions as documented

.. warning:: **This workflow is essential!** If it fails, there is a problem in infrastructure.

.. _developer_ci_workflow_netlist_generation:

Netlist Generation
^^^^^^^^^^^^^^^^^^

As illustrated in Fig. :numref:`fig_ci_workflows_netlist_generation`, the workflow aims to validate the following:

- RTL netlists are reproduciable by OpenFPGA and architecture files
- Gate-level netlists are reproduciable by OpenFPGA, architecture files and related scripts


.. _fig_ci_workflows_netlist_generation:

.. figure:: ./figures/ci_workflows_netlist_generation.svg
  :width: 100%

  Decision tree of netlist generation workflow

.. _developer_ci_workflow_bitstream_generation:

Bitstream Generation
^^^^^^^^^^^^^^^^^^^^

As illustrated in Fig. :numref:`fig_ci_workflows_bitstream_generation`, the workflow aims to validate the following:

- Bitstream files are reproduciable by OpenFPGA, benchmarks and architecture files

.. _fig_ci_workflows_bitstream_generation:

.. figure:: ./figures/ci_workflows_bitstream_generation.svg
  :width: 100%

  Decision tree of bitstream generation workflow

.. _developer_ci_workflow_testbench_generation:

Testbench Generation
^^^^^^^^^^^^^^^^^^^^

As illustrated in Fig. :numref:`fig_ci_workflows_testbench_generation`, the workflow aims to validate the following:

- Testbench files are reproduciable by OpenFPGA, benchmarks and architecture files

.. _fig_ci_workflows_testbench_generation:

.. figure:: ./figures/ci_workflows_testbench_generation.svg
  :width: 100%

  Decision tree of testbench generation workflow

.. _developer_ci_workflow_rtl_verification:

RTL Verification
^^^^^^^^^^^^^^^^

As illustrated in Fig. :numref:`fig_ci_workflows_rtl_verification`, the workflow aims to validate the following:

- RTL netlists can pass all the design verification tests.

.. _fig_ci_workflows_rtl_verification:

.. figure:: ./figures/ci_workflows_rtl_verification.svg
  :width: 100%

  Decision tree of RTL verification workflow


Useful Labels of Pull Requests
------------------------------

Continous integration is triggered conditionally to avoid high traffic in computing machines.
Users can add the following labels in pull requests, to force running some tests:

.. option:: force_netlist_generation

  Force the run of netlist generation workflow. See details in :ref:`developer_ci_workflow_netlist_generation`

.. option:: force_bitstream_generation

  Force the run of bitstream generation workflow. See details in :ref:`developer_ci_workflow_bitstream_generation`

.. option:: force_testbench_generation

  Force the run of testbench generation workflow. See details in :ref:`developer_ci_workflow_testbench_generation`

.. option:: force_rtl_full_simulation

  Force the run of full testbench simulation for RTL netlists. See details in :ref:`developer_ci_workflow_rtl_verification`

.. option:: force_rtl_preconfig_simulation

  Force the run of preconfigured testbench simulation for RTL netlists. See details in :ref:`developer_ci_workflow_rtl_verification`

.. option:: force_gl_full_simulation

  Force the run of full testbench simulation for gate-level netlists. See details in :ref:`developer_ci_workflow_rtl_verification`

.. option:: force_gl_preconfig_simulation

  Force the run of preconfigured testbench simulation for gate-level netlists. See details in :ref:`developer_ci_workflow_rtl_verification`

CI Runners
----------

Workflows are executed on two type of runners (computers)

- Github-hosted runners

- Self-hosted runners

Github-Hosted Runners
^^^^^^^^^^^^^^^^^^^^^

All the detect-changes parts of workflow are executed here because they do not require in-house tools

Self-Hosted Runners
^^^^^^^^^^^^^^^^^^^

Most generation/validation workflow are executed here because they require in-house tools

Currently, the self-hosted runners are on the ``eda01``, ``eda02`` and ``eda03`` workstation
