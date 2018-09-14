Link circuit modules
====================
Each defined SPICE model should be linked to a FPGA module defined in the original part of architecture descriptions. It helps FPGA-SPICE creating the SPICE netlists for logic/routing blocks. Since the original part lacks such support, we create a few XML properties to link to SPICE models.

5.1.	SRAM
To link the defined SPICE model of SRAM into the FPGA architecture description, a new line in XML format should be added under the XML node device. The new XML node is named as sram, which defines the area of a SRAM and the name of SPICE model to be linked. And example is shown as follows:
<sram area=”int” spice_model_name=”string”>
area is expressed in terms of the number of minimum width transistors. The SRAM area defined in this line is used in the area estimation of global routing multiplexers. spice_model_name should match the name of SPICE model that have been defined under XML node module_spice_model. The type of the linked SPICE model should be sram.
Here is an example.
<sram area=”4” spice_model_name=”sram6T”>


5.2.	Switch Blocks
Original VPR architecture description contains a XML node called switchlist under which all the multiplexers of switch blocks are described.
To link a defined SPICE model to a multiplexer in the switch blocks, a new XML property spice_model_name should added to the descriptions.

Here is an example:

<switchlist>
      <switch type=”mux” name=”string” R=”float” Cin=”float” Cout=”float” Tdel=”float” mux_trans_size=”float” buf_size=”float” spice_model_name=”string”/>
    </switchlist>
spice_model_name: should match a SPICE model whose type is mux defined under module_spice_models.


5.3.	Connection Boxes
To link the defined SPICE model of multiplexer to the Connection Boxes, a spice_model_name should be added to the definition of Connection Boxes switches.  However, the original architecture descriptions do not offer a switch description for connection boxes as they do for the switch blocks.
Therefore, FPGA-SPICE requires a new XML node called cblock under the root XML node architecture, where a switch for connection boxes can be defined.
Here is the example:
<cblock>
  <switch type=”mux” name=”string” R=”float” Cin=”float” Cout=”float” Tdel=”float” mux_trans_size=”float” buf_size=”float” spice_model_name=”string”/>
</cblock>
spice_model_name: should match a SPICE model whose type is mux defined under module_spice_models.

5.4.	Channel Wire Segments
Simliar to the SB and CB, the channel wire segments in the original architecture descriptions can be adapted to provide a link to the defined SPICE model.
<segmentlist>
  <segment freq=”float” length=”int” type=”string” Rmetal=”float” Cmetal=”float” spice_model_name=”string”/>
</segmentlist>
spice_model_name: should match a SPICE model whose type is chan_wire defined under module_spice_models.

5.5.	Modules inside logic blocks (pb_type)
The architecture description employs a hierarchy of pb_types to depicting the sub modules and complex interconnections inside logic blocks. Each leaf node and interconnection in pb_type hierarchy should be linked to a SPICE model.

<pb_type name=”string” blif_model=”string” spice_model_name=”string” idle_mode_name=”string”>
</pb_type>
<interconnect>
  <direct name=”string” input=”string” output=”string” spice_model_name=”string”/>
  <complete name=”string” input=”string” output=”string” spice_model_name=”string”/>
  <mux name=”string” input=”string” output=”string” spice_model_name=”string”/>
</interconnect>
spice_model_name: should match a SPICE model defined under module_spice_models. For the interconnection type direct, the type of the linked SPICE model should be wire. For mux, the type of the linked SPICE model should be mux. For complete, the type of the linked SPICE model can be either mux or wire, depending on the case.

idle_mode_name: tell the name of the mode that the pb_type is configured to be by default. This is critical in building SPICE netlists for unused logic blocks.


