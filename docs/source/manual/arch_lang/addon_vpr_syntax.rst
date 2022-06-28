.. _addon_vpr_syntax:

Additional Syntax to Original VPR XML
-------------------------------------

.. warning:: Note this is only applicable to VPR8!

Models, Complex blocks and Physical Tiles
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
Each ``<pb_type>`` should contain a ``<mode>`` that describes the physical implementation of the ``<pb_type>``. Note that this is fully compatible to the VPR architecture XML syntax.
  
.. note:: ``<model>`` should include the models that describe the primitive ``<pb_type>`` in physical mode.

.. note:: Currently, OpenFPGA only supports 1 ``<equivalent_sites>`` to be defined under each ``<tile>``

.. option:: <mode disable_packing="<bool">/>

  OpenFPGA allows users to define it a mode is disabled for VPR packer.
  By default, the ``disable_packing`` is set to ``false``. 
  This is mainly used for the mode that describes the physical implementation, which is typically not packable. Disable it in the packing and signficantly accelerate the packing runtime.

  .. note:: Once a mode is disabled in packing, its child modes will be disabled as well.

Layout
~~~~~~

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

  .. warning:: Do NOT enable ``through_channel`` if you are not using the tileable routing resource graph generator!
  
  .. warning:: You cannot use ``spread`` pin location for the ``height > 1`` or ``width >1`` tiles when using the tileable routing resource graph!!! Otherwise, it will cause undriven pins in your device!!!


A quick example to show tileable routing is enabled and through channels are disabled:

.. code-block:: xml

  <layout tileable="true" through_channel="false">
  </layout>

Switch Block
~~~~~~~~~~~~

``<switch_block>`` may include addition syntax to enable different connectivity for pass tracks

.. option:: sub_type="<string>"
  
  Connecting type for pass tracks in each switch block
  The supported connecting patterns are ``subset``, ``universal`` and ``wilton``, being the same as VPR capability
  If not specified, the pass tracks will the same connecting patterns as start/end tracks, which are defined in ``type``

.. option:: sub_Fs="<int>"

  Connectivity parameter for pass tracks in each switch block. Must be a multiple of 3.
  If not specified, the pass tracks will the same connectivity as start/end tracks, which are defined in ``fs``

A quick example which defines a switch block
  - Starting/ending routing tracks are connected in the ``wilton`` pattern
  - Each starting/ending routing track can drive 3 other starting/ending routing tracks
  - Passing routing tracks are connected in the ``subset`` pattern
  - Each passing routing track can drive 6 other starting/ending routing tracks

.. code-block:: xml

  <device>
    <switch_block type="wilton" fs="3" sub_type="subset" sub_fs="6"/>
  </device>

Routing Segments
~~~~~~~~~~~~~~~~

OpenFPGA suggests users to give explicit names for each routing segement in ``<segmentlist>`` 
This is used to link ``circuit_model`` to routing segments.

A quick example which defines a length-4 uni-directional routing segment called ``L4`` :

.. code-block:: xml

  <segmentlist>
    <segment name="L4" freq="1" length="4" type="undir"/>
  </segmentlist>

.. note:: Currently, OpenFPGA only supports uni-directional routing architectures

