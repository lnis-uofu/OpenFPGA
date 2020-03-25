.. _addon_vpr_syntax:

Additional Syntax to Original VPR XML
-------------------------------------

.. warning:: Note this is only applicable to VPR8!

  
Each ``<pb_type>`` should contain a ``<mode>`` that describe the physical implementation of the ``<pb_type>``. Note that this is fully compatible to the VPR architecture XML syntax.
  
``<model>`` should include the models that describe the primitive ``<pb_type>`` in physical mode.
  
``<layout>`` may include additioinal syntax to enable tileable routing resource graph generation

.. option:: tileable="<bool>"

  Turn ``on``/``off`` tileable routing resource graph generator.
  
  Tileable routing architecture can minimize the number of unique modules in FPGA fabric to be physically implemented.

  Technical details can be found in :cite:`XTang_FPT_2019`. 

  .. note:: Strongly recommend to enable the tileable routing architecture when you want to PnR large FPGA fabrics, which can effectively reduce the runtime.

.. option:: through_channel="<bool>"
  
  Allow routing channels to pass through multi-width and multi-height programable blocks. This is mainly used in heterogeneous FPGAs to increase routability, as illustrated in :numref:`fig_thru_channel`.
  By default, it is ``off``.

  .. _fig_thru_channel:
  
  .. figure:: ./figures/thru_channel.png
     :scale: 80%
     :alt: Impact of through channel
  
     Impact on routing architecture when through channel in multi-width and multi-height programmable blocks: (a) disabled; (b) enabled.

  .. warning:: Do NOT enable if you are not using the tileable routing resource graph generator!

``<switch_block>`` may include addition syntax to enable different connectivity for pass tracks

.. option:: sub_type="<string>"
  
  Connecting type for pass tracks in each switch block
  If not specified, the pass tracks will the same connecting patterns as start/end tracks, which are defined in ``type``

.. option:: sub_Fs="<int>"

  Connectivity parameter for pass tracks in each switch block. Must be a multiple of 3.
  If not specified, the pass tracks will the same connectivity as start/end tracks, which are defined in ``fs``

.. note:: Currently, OpenFPGA only supports uni-directional routing architectures

.. note:: Currently, OpenFPGA only supports 1 ``<equivalent_sites>`` to be defined under each ``<tile>``

.. note:: OpenFPGA require explicit names to be defined for each routing segement in ``<segmentlist>`` 


