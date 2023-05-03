.. _file_formats_clock_network:

Clock Network (.xml)
--------------------

The XML-based clock network description language is used to describe 

- One or more programmable clock networks constaining programmable switches for routing clock signals
- The routing for clock signals on the programmable clock network

Using the clock network description language, users can define multiple clock networks, each of which consists:

- A number of clock spines which can propagate clock signals from one point to another. See details in :ref:`file_formats_clock_network_clock_spine`.
- A number of switch points which interconnects clock spines using programmable routing switches. See details in :ref:`file_formats_clock_network_switch_point`.
- A number of tap points which connect the clock spines to programmable blocks, e.g., CLBs. See details in :ref:`file_formats_clock_network_tap_point`.

.. note:: Please note that the levels of a clock network will be automatically inferred from the clock spines and switch points. Clock network will be **only** built based on the width and the number of levels, as well as the tap points.

.. note:: The switch points and clock spines will be used to route a clock network. The switch points will not impact the physical clock network but only impact the configuration of the programmable routing switches in the physical clock network.

.. warning:: Clock network is a feature for power-users. It requires additional EDA support to leverage the best performance of the clock network, as timing analysis and convergence is more challenging.

.. code-block:: xml

  <clock_networks default_segment="<string>" default_switch="<string>"> 
    <clock_network name="<string>" width="<int>"> 
      <spine name="<string>" start_x="<int>" start_y="<int>" end_x="<int>" end_y="<int>"> 
        <switch_point tap="<string>" x="<int>" y="<int>"/> 
      </spine>  
      <taps>
        <tap tile_pin="<string>"/>
      </taps>
    </clock_network>  
  </clock_networks> 

General Settings
^^^^^^^^^^^^^^^^

The following syntax are applicable to the XML definition under the root node ``clock_networks``

.. option:: default_segment="<string>"

  Define the default routing segment to be used when building the routing tracks for the clock network. Must be a valid routing segment defined in the VPR architecture file.  For example, 

  .. code-block:: xml

   default_segment="L1"

where the segment is defined in the VPR architecture file:

.. code-block:: xml

  <segmentlist>
    <segment name="L1" freq="1" length="1" type="undir"/>
  </segmentlist>

.. note:: Currently, clock network requires only length-1 wire segment to be used!

.. option:: default_switch="<string>"

  Define the default routing switch to be used when interconnects the routing tracks in the clock network. Must be a valid routing switch defined in the VPR architecture file. For example, 

  .. code-block:: xml

    default_switch="clk_mux"

where the switch is defined in the VPR architecture file:

.. code-block:: xml

  <switchlist>
    <switch type="mux" name="clk_mux" R="551" Cin=".77e-15" Cout="4e-15" Tdel="58e-12" mux_trans_size="2.630740" buf_size="27.645901"/>
  </switchlist>

.. note:: Currently, clock network only supports one type of routing switch, which means all the programmable routing switch in the clock network will be in the same type and circuit design topology.

Clock Network Settings
^^^^^^^^^^^^^^^^^^^^^^

The following syntax are applicable to the XML definition tagged by ``clock_network``.
Note that a number of clock networks can be defined under the root node ``clock_networks``.

.. option:: name="<string>"

  The unique name of the clock network. It will be used to link the clock network to a specific global port in :ref:`annotate_vpr_arch_physical_tile_annotation`. For example, 
  
  .. code-block:: xml

    name="clk_tree_0"

where the clock network is used to drive the global clock pin ``clk0`` in OpenFPGA's architecture description file:

.. code-block:: xml

  <tile_annotations>
    <global_port name="clk0" is_clock="true" clock_arch_tree_name="clk_tree_0" default_val="0">
      <tile name="clb" port="clk[0:1]"
    </global_port>
  </tile_annotations>

.. option:: width="<int>"

  The maximum number of clock pins that a clock network can drive.

.. _file_formats_clock_network_clock_spine:

Clock Spine Settings
^^^^^^^^^^^^^^^^^^^^

The following syntax are applicable to the XML definition tagged by ``spine``.
Note that a number of clock spines can be defined under the node ``clock_network``.

.. option:: name="<string>"

  The unique name of the clock spine. It will be used to build switch points between other clock spines.

.. option:: start_x="<int>"

  The coordinate X of the starting point of the clock spine.

.. option:: start_y="<int>"

  The coordinate Y of the starting point of the clock spine.

.. option:: end_x="<int>"

  The coordinate X of the ending point of the clock spine.

.. option:: end_y="<int>"

  The coordinate Y of the ending point of the clock spine.

For example, 

.. code-block:: xml

  <spine name="spine0" start_x="1" start_y="1" end_x="2" end_y="1"/>

where a horizental clock spine ``spine0`` is defined which spans from (1, 1) to (2, 1)

.. note:: We only support clock spines in horizental and vertical directions. Diagonal clock spine is not supported!

.. _file_formats_clock_network_switch_point:

Switch Point Settings
^^^^^^^^^^^^^^^^^^^^^

The following syntax are applicable to the XML definition tagged by ``switch_point``.
Note that a number of switch points can be defined under each clock spine ``spine``.

.. option:: tap="<string>"

  Define which clock spine will be tapped from the current clock spine.

.. option:: x="<int>"

  The coordinate X of the switch point. Must be a valid coordinate within the range of the current clock spine and the clock spine to be tapped.

.. option:: y="<int>"

  The coordinate Y of the switch point. Must be a valid coordinate within the range of the current clock spine and the clock spine to be tapped.

For example, 

.. code-block:: xml

  <spine name="spine0" start_x="1" start_y="1" end_x="2" end_y="1">
    <switch_point tap="spine1" x="1" y="1"/>
  <spine>

where clock spine ``spine0`` will drive another clock spine ``spine1`` at (1, 1). 

.. _file_formats_clock_network_tap_point:

Tap Point Settings
^^^^^^^^^^^^^^^^^^

The following syntax are applicable to the XML definition tagged by ``tap``.
Note that a number of tap points can be defined under the node ``taps``.

.. option:: tile_pin="<string>"

  Define the pin of a programmable block to be tapped by a clock network. The pin must be a valid pin defined in the VPR architecture description file.

.. note:: Only the leaf clock spine (not switch points to drive other clock spine) can tap pins of programmable blocks.

For example,

.. code-block:: xml

  <clock_network name="clk_tree_0" width="1">
    <!-- Some clock spines -->
    <taps>
      <tap tile_pin="clb.clk"/>
    </taps>
  </clock_network>

where all the clock spines of the clock network ``clk_tree_0`` tap the clock pins ``clk`` of tile ``clb`` in a VPR architecture description file:

.. code-block:: xml

  <tile name="clb">
   <sub_tile name="clb">
     <clock name="clk" num_pins="1"/>
   </sub_tile>
  </tile>

