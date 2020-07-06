Fabric Key
~~~~~~~~~~

Fabric key is a secure key for users to generate bitstream for a specific FPGA fabric. 
With this key, OpenFPGA can generate correct bitstreams for the FPGA.
Using a wrong key, OpenFPGA may error out or generate wrong bitstreams.
The fabric key support allows users to build secured/classified FPGA chips even with an open-source tool.

.. note:: Users are the only owner of the key. OpenFPGA will not store or replicate the key.

Key Generation
``````````````
A fabric key can be achieved in the following ways:

- OpenFPGA can auto-generate a fabric key using random algorithms (see detail in :ref:`cmd_build_fabric`)

- Users can craft a fabric key based on auto-generated file by following the file format description.

File Format
```````````

A fabric key follows an XML format. As shown in the following XML code, the key file includes the organization of configurable memory blocks in the top-level FPGA fabric: 

  - ``id`` indicates the sequence of the configurable memory block in the top-level FPGA fabric.

  - ``name`` indicates the module name of the configurable memory block.

  - ``value`` indicates the instance id of the configurable memory block in the top-level FPGA fabric.

  - ``alias`` indicates the instance name of the configurable memory block in the top-level FPGA fabric. If a valid alias is specified, the ``value`` will not be considered.

The following is an example of a fabric key generate by OpenFPGA for a 2 :math:`\times` 2 FPGA.

.. code-block:: xml

    <fabric_key>
    	<key id="0" name="sb_2__2_" value="0" alias="sb_2__2_"/>
    	<key id="1" name="grid_clb" value="3" alias="grid_clb_2_2"/>
    	<key id="2" name="sb_0__1_" value="0" alias="sb_0__1_"/>
    	<key id="3" name="cby_0__1_" value="0" alias="cby_0__1_"/>
    	<key id="4" name="grid_clb" value="2" alias="grid_clb_2_1"/>
    	<key id="5" name="grid_io_left" value="0" alias="grid_io_left_0_1"/>
    	<key id="6" name="sb_1__0_" value="0" alias="sb_1__0_"/>
    	<key id="7" name="sb_1__1_" value="0" alias="sb_1__1_"/>
    	<key id="8" name="cbx_1__1_" value="1" alias="cbx_2__1_"/>
    	<key id="9" name="cby_1__1_" value="1" alias="cby_1__2_"/>
    	<key id="10" name="grid_io_right" value="1" alias="grid_io_right_3_2"/>
    	<key id="11" name="cbx_1__0_" value="1" alias="cbx_2__0_"/>
    	<key id="12" name="cby_1__1_" value="0" alias="cby_1__1_"/>
    	<key id="13" name="grid_io_right" value="0" alias="grid_io_right_3_1"/>
    	<key id="14" name="grid_io_bottom" value="0" alias="grid_io_bottom_1_0"/>
    	<key id="15" name="cby_2__1_" value="0" alias="cby_2__1_"/>
    	<key id="16" name="sb_2__1_" value="0" alias="sb_2__1_"/>
    	<key id="17" name="cbx_1__0_" value="0" alias="cbx_1__0_"/>
    	<key id="18" name="grid_clb" value="1" alias="grid_clb_1_2"/>
    	<key id="19" name="cbx_1__2_" value="0" alias="cbx_1__2_"/>
    	<key id="20" name="cbx_1__2_" value="1" alias="cbx_2__2_"/>
    	<key id="21" name="sb_2__0_" value="0" alias="sb_2__0_"/>
    	<key id="22" name="sb_1__2_" value="0" alias="sb_1__2_"/>
    	<key id="23" name="cby_0__1_" value="1" alias="cby_0__2_"/>
    	<key id="24" name="sb_0__0_" value="0" alias="sb_0__0_"/>
    	<key id="25" name="grid_clb" value="0" alias="grid_clb_1_1"/>
    	<key id="26" name="cby_2__1_" value="1" alias="cby_2__2_"/>
    	<key id="27" name="grid_io_top" value="1" alias="grid_io_top_2_3"/>
    	<key id="28" name="sb_0__2_" value="0" alias="sb_0__2_"/>
    	<key id="29" name="grid_io_bottom" value="1" alias="grid_io_bottom_2_0"/>
    	<key id="30" name="cbx_1__1_" value="0" alias="cbx_1__1_"/>
    	<key id="31" name="grid_io_top" value="0" alias="grid_io_top_1_3"/>
    	<key id="32" name="grid_io_left" value="1" alias="grid_io_left_0_2"/>
    </fabric_key>
