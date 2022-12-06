.. dev_manual_backward_compatibility::

Backward compatibility
======================

OpenFPGA v1.1
-------------

OpenFPGA v1.2 is a major upgrade over v1.1, which upgrades the internal VPR engine.
The (VPR) architecture files used with v1.1 may not be compatible with v1.2.

You can upgrade your architecture files with script

.. code-block:: bash

  python3 openfpga_flow/scripts/arch_file_updater.py \
      --input_file ${v1.1_arch_file} \
      --output_file ${v1.2_compatible_arch_file}

Or, If you want to stay with v1.1, the final build was (tag: `OpenFPGA:v1.1.541 <https://github.com/lnis-uofu/OpenFPGA/tree/v1.1.541>`_))

.. code-block:: bash

  https://github.com/lnis-uofu/OpenFPGA/tree/v1.1.541

or you can download the docker image

.. code-block:: bash

  docker pull ghcr.io/lnis-uofu/openfpga-master:v1.1.541
