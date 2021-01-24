.. _fpga_bitstream_repack_design_constraints:

Repack Design Constraints
-------------------------

An example of design constraints is shown as follows.

.. code-block:: xml

  <repack_design_constraints>
    <pin_constraint pb_type="clb" pin="clk[0]" net="clk0"/>
    <pin_constraint pb_type="clb" pin="clk[1]" net="clk1"/>
    <pin_constraint pb_type="clb" pin="clk[2]" net="OPEN"/>
    <pin_constraint pb_type="clb" pin="clk[3]" net="OPEN"/>
  </repack_design_constraints>

.. option:: pb_type="<string>"

  The pb_type name to be constrained, which should be consistent with VPR's architecture description.

.. option:: pin="<string>"

  The pin name of the ``pb_type`` to be constrained, which should be consistent with VPR's architecture description.

.. option:: net="<string>"

  The net name of the pin to be mapped, which should be consistent with net definition in your ``.blif`` file. The reserved word ``OPEN`` means that no net should be mapped to a given pin. Please ensure that it is not conflicted with any net names in your ``.blif`` file.
  
.. warning:: Design constraints is a feature for power-users. It may cause repack to fail. It is users's responsibility to ensure proper design constraints
