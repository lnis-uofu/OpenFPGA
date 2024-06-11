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

CI/CD setup
-----------

OpenFPGA implements CI/CD system using Github actions.
The following figure shows the Actions implements flow.
The source building is skipped if there are changes only in ``openfpga_flow`` or ``docs`` directory,
in which case the docker image compiled for the latest master branch is used for running a regression.


.. graphviz::
   :align: center

   digraph G {
       node [fontname = "Handlee"];
       edge [fontname = "Handlee"];
       Trigger [
           label = "Action triggered"
       ];

       masterCompare [
           label = "Diff with current master"
       ];

       Build [
           label = "Changes only in\n openfpga_flow/doc?"
           shape = diamond
       ];

       BuildDocker [
           label = "Run build regression test\nBuild docker images"
           shape = box
       ];

       PushDockersCond [
           label = "Is merge\non master?"
           shape = diamond
       ];

       PushDockers [
           label = "Push docker Images\n(maintain compiled binary\nin docker + Example tasks)"
           shape = box
       ];

       RunRegression [
           label = "Run functional regression test"
           shape = box
       ];

     Trigger ->masterCompare;
     masterCompare ->Build;
     Build -> BuildDocker [ label = "No" ];
     BuildDocker -> PushDockersCond;
     edge[weight=0.5] Build -> RunRegression [ label = "Yes" ];
     edge[weight=10] PushDockersCond -> RunRegression [ label = "No" ];
     PushDockersCond -> PushDockers [ label = "Yes" ];
     edge[weight=2] PushDockers -> RunRegression;

     {
       rank=same;
       PushDockersCond PushDockers;
     };
   }


|


.. option:: Build regression test

    The OpenFPGA source is compiled with the following set of compilers.

      #. gcc-7
      #. gcc-8
      #. gcc-9
      #. gcc-10
      #. gcc-11
      #. clang-6
      #. clang-7
      #. clang-8
      #. clang-10

    The docker images for these build environment are available on `github packages <https://github.com/orgs/lnis-uofu/packages>`_.

.. option:: Functional regression test

    OpenFPGA maintains a set of functional tests to validate the different functionality.
    The test are broadly catagories into ``basic_reg_test``, ``fpga_verilog_reg_test``,
    ``fpga_bitstream_reg_test``, ``fpga_sdc_reg_test``, and ``fpga_spice_reg_test``.
    A functional regression test is run for every commit on every branch.


How to debug failed regression test
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
In case the ``functional regression test`` fails,
the actions script will collect all ``.log`` files from
the task directory and upload as a artifacts on github storage.
These artifacts can be downloaded from the github website actions tab, for more reference follow `this <https://docs.github.com/en/actions/managing-workflow-runs/downloading-workflow-artifacts>`_ article.

**NOTE** : The retention time of these artifacts is 1 day,
so in case user want to reserve the failure log for longer duration back it up locally

Release Docker Images
^^^^^^^^^^^^^^^^^^^^^^

.. option:: ghcr.io/lnis-uofu/openfpga-master:latest

    This is a bleeding-edge release from the current master branch of OpenFPGA.
    It is updated automatically whenever there is activity on the master branch.
    Due to high development activity, we recommend the user to use the bleeding-edge version to get access to all new features and report an issue in case there are any bugs.


CI after cloning repository
^^^^^^^^^^^^^^^^^^^^^^^^^^^
If you clone the repository the CI setup will still function, except the based images are  still pulled from "lnis-uofu" repository and the master branch
of cloned repo will not push final docker image to any repository .

**In case you want to host your own copies of OpenFPGA base images** and final release create a github secret variable with name  ``DOCKER_REPO`` and set it to ``true``. This will make ci script to download base images from your own repo packages, and upload final release to the same.

**If you don not want to use docker images based regression test** and like to compile all the binaries for each CI run. You can set ``IGNORE_DOCKER_TEST`` secrete variable to ``true``.

.. note:: Once you add ``DOCKER_REPO`` variable, you need to generate base images. To do this trigger manual workflow ``Build docker CI images``
