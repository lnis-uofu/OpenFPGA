.. dev_manual_regression_tests::

Regression Tests
================

Regression tests are designed to cover various technical features of the OpenFPGA projects, including but not limited to

- Netlist generation
- Netlist verification
- Bitstream generation

Considering the large number of technical features, regression tests are categorized into several groups, which can be found at ``openfpga_flow/regression_test_scripts/``

Run a Test
----------

.. note:: Make sure you have compiled OpenFPGA and set up your environment before reaching this step. See details in :ref:`getting_started_tutorials`.

To run a regression test, users can execute a shell script (assume you are under the root directory of the project), for example, 

.. code-block:: shell

  ./openfpga_flow/regression_test_scripts/basic_reg_test.sh [OPTIONS]

.. note:: ``basic_reg_test`` can be replaced by other tests which are under ``openfpga_flow/regression_test_scripts/``

Test Options
------------

There are a few options available when running the tests.

.. option:: --debug

  This option can turn on debug mode when running regression tests. By default it is ``off``.

.. option:: --show_thread_logs

  This option can enable verbose output when running regression tests. By default it is ``off``.

.. note:: To avoid massive outputs, suggest to run the tests with default options. In CI, always recommend to turn on the debug and verbose options

.. option:: --remove_run_dir all

  This option is to remove all the previous run results for a specific regression test. Suggest to use when there are limited disk space.

  .. note:: Be careful before using this option! It may cause permanent loss on test results.


