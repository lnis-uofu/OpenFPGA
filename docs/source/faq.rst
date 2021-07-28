.. _faq:

Frequently Asked Questions
==========================

Where is the best place to get help with OpenFPGA?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Currently, we have an active github issues page found `here <https://github.com/lnis-uofu/OpenFPGA/issues>`_. Users can see if their 
questions have already been answered by searching the open or closed issues, and users are recommended to post questions there first. 
Asking questions on the github issues page allows us to answer the question for everyone who may be experiencing similar problems as 
well.

What should I do if check-in tests failed when first installing OpenFPGA?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

First, check to make sure all dependencies for OpenFPGA and Python have been installed and are up-to-date on the desired device. To see the full
list of depenencies, please visit 
`our github dependencies page <https://github.com/lnis-uofu/OpenFPGA/blob/master/.github/workflows/install_dependencies_build.sh>`_. 
This issue has been discussed `in issue 280 <https://github.com/lnis-uofu/OpenFPGA/issues/280>`_.


How to sweep design parameters in a task run of OpenFPGA design flow?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Testing multiple script parameters for a variable is possible by modifying the task.conf file. Doing so will create a job for 
each combination of the variables. A solution is discussed `in issue 228 <https://github.com/lnis-uofu/OpenFPGA/issues/228>`_.


How do I setup OpenFPGA to be used by multiple users on a single device?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

OpenFPGA can support multiple users on a shared device using the environment variable ``OPENFPGA_ROOT``. The OpenFPGA script for 
running tasks needs ``OPENFPGA_ROOT`` to be the path to the OpenFPGA root directory. Users can then run the script on a task in the
current working directory. A solution is discussed `in issue 209 <https://github.com/lnis-uofu/OpenFPGA/issues/209>`_.


How do I contribute to OpenFPGA?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Users of OpenFPGA that are interested in contributing must complete the following: 

    - Create a branch. For external collaborators, please fork the repository first and create a branch in the fork.
    - Creatre a pull request and fill out our pull request template. It is easy for us to acknowledge and review your pull request.	
    - Wait or keep debugging until all the CI tests pass.	
    - Request for a review. You may expect several rounds of review and discussion before the pull request is approved.

