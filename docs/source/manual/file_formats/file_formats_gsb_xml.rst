.. _file_formats_gsb_xml:

General Switch Blocks (GSB)
---------------------------

The XML-based representation of General Switch Blocks (GSBs) describes the internal connectivity of drivers in connection blocks (CBs) and switch blocks (SBs) within the FPGA routing architecture. Connection blocks are represented using the ``rr_cb`` tag, while switch blocks are represented using the ``rr_sb`` tag.

Connection blocks:
``````````````````

An example connection block representation is shown below:

.. code-block:: text

                 ┌─────────────────────────────┐
                 │   Connection Block (CB)     │
                 │                             │
                 │        CHANX/CHANY          │(CHANX Right)
    <------------│---------●-------------------│------>
    (CHANX Right)│---------●-------------------│
                 │         │                   │
                 │         │ IPIN              │
                 └─────────●───────────────────┘
                           │
                     IPIN_DRIVER_SIDE
                     IPIN_DRIVER_INDEX
               (Opposite to grid PIN side)


.. code-block:: xml

  <rr_cb x="<X_LOC>" y="<Y_LOC>" num_sides="<NUM_SIDES>">
    <!-- This is IPIN drivers representation -->
    <IPIN side="<IPIN_DRIVER_SIDE>" index="<IPIN_INDEX>" mux_size="<MUX_SIZE>">
      <driver_node type="<CHANNEL_TYPE>" side="<DRIVER_SIDE>" index="<DRIVER_INDEX>" segment_id="<SEGMENT_ID>" tap="<TAP_DISTANCE>"/>
      ....
    </IPIN>
    ....
  </rr_cb>


The IPIN tag represents a single drivers in the current location. A file contains multiple IPIN tags to represent all the drivers in the current location.
Each IPIN tag contains multiple driver_node tags to represent the drivers connected to the current IPIN and properties of the driver.

The attributes of the IPIN tag are as follows:

.. option:: x="<X_LOC>"

   The x-coordinate of the connection block in the FPGA grid.

.. option:: y="<Y_LOC>"

    The y-coordinate of the connection block in the FPGA grid.

.. option:: num_sides="<NUM_SIDES>"

    Total number of sides of the connection block (Generally 4).

.. option:: driver_side="<IPIN_DRIVER_SIDE>"

    The side of the connection block where the driver is located (e.g., "TOP", "BOTTOM", "LEFT", "RIGHT").
    Generally, the driver side is opposite to the side of the PIN on the grid.

.. option:: index="<IPIN_INDEX>"

   The index of the IPIN in the RRGSB datastrcuture.


.. option:: mux_size="<MUX_SIZE>"

   The size of the multiplexer for the IPIN. Count of number of drivers connected to this IPIN.


The attributes of the driver_node tag are as follows:


.. option:: type="<CHANNEL_TYPE>"

    The type of the driver channel (e.g., "CHANX", "CHANY"). IPIN can be driven by both CHANX and CHANY channels.

.. option:: side="<DRIVER_SIDE>"

    The direction of the channel/routing wire (e.g., "TOP", "BOTTOM", "LEFT", "RIGHT").


.. option:: index="<DRIVER_INDEX>"

    The index/ptc number of the driver channel/routing wire in the current grid location.


.. option:: segment_id="<SEGMENT_ID>"

    The segment ID of the driver channel/routing wire. This is used to identify the length/type of the driver channel/routing wire.

.. option:: tap="<TAP_DISTANCE>"

    The tap distance of the driver channel/routing wire to the current IPIN. This is used to identify the relative position of the driver channel/routing wire to the current IPIN.
    tap = abs(x1-x2) + abs(y1-y2), where (x1,y1) and (x2,y2) are the coordinates of the driver channel/routing wire and the current IPIN respectively.

.. code-block:: text

    Driver
    (x1,y1)
      ●---------------●--------
                      | Destination
                        (x2,y2)


Switch Block GSB Format
```````````````````````

An example switch block representation is shown below:

.. code-block:: text

                 CHANY
                │BOTTOM
            ┌───▼──────┐
      CHANX │          │
      RIGHT │          │
     ───────►  Switch  ├────►
       ┌────►  Block   │ DRIVER_DIRECTION
       │    │          │ CHANNEL_INDEX
       │    └────▲─────┘
       │         │CHANY
       │OPIN     │TOP


.. code-block:: xml
  <rr_sb>
    <!-- This is Switch block vertical drivers representation -->
    <CHANY side="<DRIVER_DIRECTION>" index="<CHANNEL_INDEX>" mux_size="<MUX_SIZE>">
      <driver_node type="<CHANNEL_TYPE>" side="<DRIVER_SIDE>" index="<DRIVER_INDEX>" segment_id="<SEGMENT_ID>" tap="<TAP_DISTANCE>"/>
      ....
    </CHANY>
    ....
    <!-- This is Switch block horizontal drivers representation -->
    <CHANX side="<DRIVER_DIRECTION>" index="<CHANNEL_INDEX>" mux_size="<MUX_SIZE>">
      <driver_node type="<CHANNEL_TYPE>" side="<DRIVER_SIDE>" index="<DRIVER_INDEX>" segment_id="<SEGMENT_ID>" tap="<TAP_DISTANCE>"/>
      ....
    </CHANX>
    ....
  </rr_sb>


The CHANX and CHANY tags represent the drivers in the switch block for horizontal and vertical channels respectively. A file contains multiple CHANX and CHANY tags to represent all the drivers in the current grid location. Each CHANX and CHANY tag contains multiple driver_node tags to represent the drivers connected to the current channel and properties of the driver.

The attributes of the CHANX and CHANY tags are as follows:
.. option:: side="<DRIVER_DIRECTION>"

    The direction of the channel/routing wire (e.g., "TOP", "BOTTOM", "LEFT", "RIGHT").

.. option:: index="<CHANNEL_INDEX>"

    The starting index of the driver channel/routing wire from RRGSB datastructure.

.. option:: mux_size="<MUX_SIZE>"

    The size of the multiplexer for the channel. Count of number of drivers connected to this channel/routing wire.


The attributes of the driver_node tag are the same as described in the connection block section above.


.. option:: type="<CHANNEL_TYPE>"

    The type of the driver channel. Switch blocks can have CHANX, CHANY and OPIN drivers.

.. option:: side="<DRIVER_SIDE>"

    The direction of the channel/routing wire (e.g., "TOP", "BOTTOM", "LEFT", "RIGHT").
    In case of OPIN drivers, the driver side is opposite to the side of the OPIN on the grid.

.. option:: index="<DRIVER_INDEX>"

    The index/ptc number of the driver channel/routing wire in the current grid location.
    In case of OPIN drivers, the driver index is the same as the OPIN index on the grid.

.. option:: tap="<TAP_DISTANCE>"

    The tap distance of the driver channel/routing wire to the current channel. This is used to identify the relative position of the driver channel/routing wire to the current channel. In case of OPIN drivers, the tap distance is set to 0. This is used to identify the relative position of the driver channel/routing wire to the current IPIN.
    tap = abs(x1-x2) + abs(y1-y2), where (x1,y1) and (x2,y2) are the coordinates of the driver channel/routing wire and the current IPIN respectively.

.. code-block:: text

    Driver
    (x1,y1)
      ●---------------●--------
                      | Destination
                        (x2,y2)