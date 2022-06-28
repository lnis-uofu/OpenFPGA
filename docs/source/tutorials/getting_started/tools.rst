.. _openfpga_tools:

Supported Tools
---------------

Internal Tools
^^^^^^^^^^^^^^

To enable various design purposes, OpenFPGA integrates several tools to i.e., FPGA-Verilog, FPGA-SDC and FPGA-bitstream (highlighted green in :ref:`fig_openfpga_tools`, with other popular open-source EDA tools, i.e., VPR and Yosys.

.. _fig_openfpga_tools:

.. figure:: figures/openfpga_tools.svg
   :width: 100%
   :alt: map to buried treasure

   OpenFPGA tool suites and design flows 

Third-Party Tools
^^^^^^^^^^^^^^^^^

OpenFPGA accepts and outputs in standard file formats, and therefore can 
interface a wide range of commercial and open-source tools.

+--------------+-------------------------+---------------------+
| Usage        | Tools                   | Version Requirement |
+==============+=========================+=====================+
| Backend      | Synopsys IC Compiler II | v2019.03 or later   |
|              |                         |                     |
|              | Cadence Innovus         | v19.1 or later      |
+--------------+-------------------------+---------------------+
| Timing       | Synopsys PrimeTime      | v2019.03 or later   |
| Analyzer     |                         |                     |
|              | Cadence Tempus          | v19.15 or later     |
+--------------+-------------------------+---------------------+
| Verification | Synopsys VCS            | v2019.06 or later   |
|              |                         |                     |
|              | Synopsys Formality      | v2019.03 or later   |
|              |                         |                     |
|              | Mentor ModelSim         | v10.6 or later      |
|              |                         |                     |
|              | Mentor QuestaSim        | v2019.3 or later    |
|              |                         |                     |
|              | Cadence NCSim           | v15.2 or later      |
|              |                         |                     |
|              | Icarus iVerilog         | v10.1 or later      |
+--------------+-------------------------+---------------------+

* The version requirements is based on our local tests. Older versions may work.
