.. _addon_vpr_syntax:

Additional Syntax to Original VPR XML
-------------------------------------

.. warning:: Note this is only applicable to VPR8!

  
Each ``<pb_type>`` should contain a ``<mode>`` that describe the physical implementation of the ``<pb_type>``. Note that this is fully compatible to the VPR architecture XML syntax.
  
``<model>`` should include the models that describe the primitive ``<pb_type>`` in physical mode.
  
``<layout>`` may include additioinal syntax to enable tileable routing resource graph generation

.. option:: tileable="<bool>"

  Turn on/off tileable routing resource graph generator

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


