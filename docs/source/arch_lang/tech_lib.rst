Technology library Declaration
==============================
.. note:: <tech_lib lib_type=”string” transistor_type=”string” lib_path=”string” nominal_vdd=”float”/>

  - lib_type: can be either industry or academia [industry|academia]. For the industry library, a number of transistor types are available and the type of transistor should be declared in the property transistor_type. 

  - transistor_type: This XML property specify the transistors to be used in the industry library. For example, the type of transistors can be “TT”, “FF” etc.

  - lib_path: specify the path of the library. For example: lib_path=/home/tech/45nm.pm.

  - nominal_vdd: specify the working voltage for the technology. The voltage will be used as the supply voltage in all the SPICE netlist.

.. note:: <transistors pn_ratio=”float” model_ref=”string”/>

  - pn_ratio: specify the ratio between p-type transistors and n-type transistors. The ratio will be used when building circuit structures such as inverters, buffers etc.
    
  - model_ref: specify the reference of in calling a transistor model. In SPICE netlist, define a transistor follows the convention: <model_ref><trans_name> <ports> <model_name>. The reference depends on the technology and the type of library. For example, PTM bulk model use “M” as the reference while PTM FinFET model use “X” as the reference.

  - <nmos model_name=”string” chan_length=”float” min_width=”float”/>

  - <pmos model_name=”string” chan_length=”float” min_width=”float”/>

  - model_name:  specify the name of the p/n type transistor, which can be found in the manual of the technology provider.
   
  - chan_length: specify the channel length of p/n type transistor.
  
  - min_width: specify the minimum width of p/n type transistor. This parameter will be used in building inverter, buffer and etc. as a base number for transistor sizing. 
  
