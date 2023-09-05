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

.. note:: The following syntax is only available in OpenFPGA!

We allow more flexible pin location assignment when a ``<tile>`` has a capacity > 1.
User can specify the location using the index of instance, e.g.,

.. code-block:: xml

    <tile name="io_bottom" capacity="6" area="0">
      <equivalent_sites>
        <site pb_type="io"/>
      </equivalent_sites>
      <input name="outpad" num_pins="1"/>
      <output name="inpad" num_pins="1"/>
      <fc in_type="frac" in_val="0.15" out_type="frac" out_val="0.10"/>
      <pinlocations pattern="custom">
        <loc side="top">io_bottom[0:1].outpad io_bottom[0:3].inpad io_bottom[2:5].outpad io_bottom[4:5].inpad</loc>
      </pinlocations>
    </tile>

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
  By default, it is ``false``.

  .. _fig_thru_channel:
  
  .. figure:: ./figures/thru_channel.png
     :width: 100%
     :alt: Impact of through channel
  
     Impact on routing architecture when through channel in multi-width and multi-height programmable blocks: (a) disabled; (b) enabled.

  .. warning:: Do NOT enable ``through_channel`` if you are not using the tileable routing resource graph generator!
  
  .. warning:: You cannot use ``spread`` pin location for the ``height > 1`` or ``width >1`` tiles when using the tileable routing resource graph!!! Otherwise, it will cause undriven pins in your device!!!

.. option:: shrink_boundary="<bool>"
  
  Remove all the routing wires in empty regions. This is mainly used in non-rectangle FPGAs to avoid redundant routing wires in blank area, as illustrated in :numref:`fig_shrink_boundary`.
  By default, it is ``false``.

  .. _fig_shrink_boundary:
  
  .. figure:: ./figures/shrink_boundary.png
     :width: 100%
     :alt: Impact of shrink boundary
  
     Impact on routing architecture when shrink-boundary: (a) disabled; (b) enabled.

  .. warning:: Do NOT enable ``shrink_boundary`` if you are not using the tileable routing resource graph generator!

A quick example to show tileable routing is enabled, other options, e.g., through channels are disabled:

.. code-block:: xml

  <layout tileable="true" through_channel="false" shrink_boundary="false">
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

