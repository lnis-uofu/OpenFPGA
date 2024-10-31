.. _file_formats_unique_blocks:

Unique Blocks (.xml)
--------------------

A unique blocks file is formatted in XML. The unique blocks can be of type ``cbx``, ``cby``, or ``sb``. As illustrated by the XML code below, the file includes the type and coordinates of these unique blocks, as well as the coordinates of their corresponding instances.

Configurable Block
~~~~~~~~~~~~~~~~~~

Unique blocks can be applied to various blocks, each of which can be of type ``cbx``, ``cby``, or ``sb``, and may have different coordinates.

.. note::

   For each block, a set of keys can be defined. For unique blocks, both keys and instances can be specified. However, if a unique block does not have an instance, only keys are permitted.

   - ``type`` specifies the type of the unique block in the FPGA fabric. Valid values for ``type`` are ``cbx``, ``cby``, or ``sb``.
   - ``x`` represents the x-coordinate of the unique block.
   - ``y`` represents the y-coordinate of the unique block.

Configurable Instance
~~~~~~~~~~~~~~~~~~~~~

A specific unique block can have multiple instances, where each instance is a mirrored version of the unique block. Each instance shares the same type as its parent block and includes information about its coordinates.

.. note::

   - ``x`` specifies the x-coordinate of the instance.
   - ``y`` specifies the y-coordinate of the instance.

The following content provides an example of a unique block file:

.. code-block:: xml

   <unique_blocks>
       <block type="sb" x="0" y="0"/>
       <block type="sb" x="0" y="1"/>
       <block type="sb" x="1" y="0"/>
       <block type="sb" x="1" y="1"/>
       <block type="cbx" x="1" y="0">
           <instance x="0" y="0"/>
           <instance x="0" y="1"/>
       </block>
       <block type="cbx" x="1" y="1"/>
       <block type="cby" x="0" y="1">
           <instance x="0" y="0"/>
           <instance x="1" y="0"/>
       </block>
       <block type="cby" x="1" y="1"/>
   </unique_blocks>
