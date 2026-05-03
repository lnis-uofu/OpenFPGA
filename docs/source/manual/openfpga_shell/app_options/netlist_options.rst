Netlist Options
----------------



.. option:: absorb_buffer_luts [bool]

    Enable absorption of buffer LUTs into their fanout logic. Default is True.

.. option:: const_gen_inference [bool]

    Enable inference of constant generators during netlist processing. Default is True.

.. option:: sweep_dangling_primary_ios [bool]

    Enable sweeping of dangling primary inputs and outputs from the netlist. Default is True.

.. option:: sweep_dangling_nets [bool]

    Enable sweeping of dangling nets (nets that are not connected to any logic) from the netlist. Default is True.

.. option:: sweep_dangling_blocks [bool]

    Enable sweeping of dangling blocks (blocks that are not connected to any nets) from the netlist. Default is True.

.. option:: sweep_constant_primary_outputs [bool]

    Enable sweeping of constant primary outputs (primary outputs that are driven by constant values) from the netlist. Default is True.
