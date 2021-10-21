
CI/CD setup
------------

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
           label = "Run functional regeression test"
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

    The OpenFPGA soure is compiled with the following set of compilers.

      #. gcc-5
      #. gcc-6
      #. gcc-7
      #. gcc-8
      #. gcc-9
      #. clang-6.0
      #. clang-8

    The docker images for these build enviroment are available on `github packages <https://github.com/orgs/lnis-uofu/packages>`_.

.. option:: Functional regeression test

    OpenFPGA maintains a set of functional tests to validate the different functionality.
    The test are broadly catagories into ``basic_reg_test``, ``fpga_verilog_reg_test``,
    ``fpga_bitstream_reg_test``, ``fpga_sdc_reg_test``, and ``fpga_spice_reg_test``.
    A functional regression test is run for every commit on every branch.


How to debug failed regression test
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
In case the ``funtional regression test`` fails,
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
If you clone the repository the CI setup will still function, except the based images are  still pullled from "lnis-uofu" repsitory and the master branch
of cloned repo will not push final docker image to any repository .

**In case you want to host your own copies of OpenFPGA base images** and final release create a githib secret variable with name  ``DOCKER_REPO`` and set it to ``true``. This will make ci script to download base images from your own repo pakcages, and upload final realse to the same.

**If you don not want to use docker images based regression test** and like to compile all the bianries for each CI run. You can set ``IGNORE_DOCKER_TEST`` secrete variable to ``true``.

.. note:: Once you add ``DOCKER_REPO`` variable, you need to genrerate base images. To do this trigger mannual workflow ``Build docker CI images``
