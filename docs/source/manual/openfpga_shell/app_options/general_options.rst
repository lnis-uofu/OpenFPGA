General Options
----------------

.. option:: num_workers [int]

    Number of worker threads to use for parallel execution. Default is 1 (no parallelism).

.. option:: timing_analysis [bool]

    Enable timing analysis during packing, placement, and routing. Default is True.

.. option:: timing_update_type [auto|full|incremental]

    Method to update timing information during packing, placement, and routing.
    Acceptable values are: ``auto`` (automatically determine the best method based on the changes),
    ``full`` (full timing update), and ``incremental`` (only update timing for modified parts). Default is ``auto``.

.. option:: device_layout

.. option:: target_device_utilization

.. option:: constant_net_method

.. option:: clock_modeling

.. option:: two_stage_clock_routing

.. option:: disable_errors

.. option:: suppress_warnings

.. option:: allow_dangling_combinational_nodes

.. option:: terminate_if_timing_fails
