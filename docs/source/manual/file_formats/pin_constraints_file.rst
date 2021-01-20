.. _file_format_pin_constraints_file:

Pin Constraints File
--------------------

The *Pin Constraints File* (PCF) aims to create pin binding between an implementation and an FPGA fabric

An example of design constraints is shown as follows.

.. code-block:: xml

  <pin_constraints>
    <set_io pin="clk[0]" net="clk0"/>
    <set_io pin="clk[1]" net="clk1"/>
    <set_io pin="clk[2]" net="OPEN"/>
    <set_io pin="clk[3]" net="OPEN"/>
  </pin_constraints>

.. option:: pin="<string>"

  The pin name of the FPGA fabric to be constrained, which should be a valid pin defined in OpenFPGA architecture description. Explicit index is required, e.g., ``clk[1:1]``. Otherwise, default index ``0`` will be considered, e.g., ``clk`` will be translated as ``clk[0:0]``.

.. option:: net="<string>"

  The net name of the pin to be mapped, which should be consistent with net definition in your ``.blif`` file. The reserved word ``OPEN`` means that no net should be mapped to a given pin. Please ensure that it is not conflicted with any net names in your ``.blif`` file.
