.. _utility_fabric_key_assistant:

Fabric Key Assistant
--------------------

Fabric Key Assistant is a tool to help users to craft fabric key files (see details in :ref:`file_formats_fabric_key`).
Note that crafting a fabric key is not an easy task for engineers, as its complexity grows exponentially with FPGA sizes.
This tool is developed to assist engineers when finalizing fabric key files.
It can apply sanity checks on hand-crafted fabric key files, helping engineers to correct and debug.

The tool can be found at ``/build/libs/libfabrickey/fabric_key_assistant``

The tool includes the following options:

.. option:: --reference <string>

  Specifiy a reference fabric key file, which has been already validated by OpenFPGA. For example, the reference fabric key can be a file which is written by OpenFPGA as a default key. The reference fabric key file is treated as the baseline, on which the input fabric key file will be compared to.

  .. note:: The reference fabric key should contain all the syntax, e.g., ``name``, ``value`` and ``alias``. 

.. option:: --input <string>

  Specify the input fabric key file, which is typically hand-crafted by users. Sanity checks will be applied to the input fabric key file by comparing the reference.

  .. note:: The input fabric key should contain only the syntax ``alias``. 

.. option:: --output <string>

  Specify the output fabric key file, which is an updated version of the input fabric key file. Difference from the input file, the output file contains ``name`` and ``value``, which is added by linking the ``alias`` from input file to reference file. For example, the reference fabric key includes a key:

.. code-block:: xml

  <key id="1" name="tile_0__0_" value="5" alias="tile_4__2_"/>

while the input fabric key includes a key:

.. code-block:: xml

  <key id="23" alias="tile_4__2_"/>

the resulting output fabric key file includes a key:

.. code-block:: xml

  <key id="23" name="tile_0__0_" value="5" alias="tile_4__2_"/>

.. option:: --verbose

  To enable verbose output

.. option:: --help

  Show help desk
