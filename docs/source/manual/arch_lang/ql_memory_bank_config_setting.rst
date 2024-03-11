.. _ql_memory_bank_config_setting:

QL Memory Bank Config Setting
-----------------------------

This is optional for QL Memory Bank configuration protocol.
In QL Memory Bank configuration protocol, configuration bits are organized as BitLine (BL) x WordLine (WL)
By default, OpenFPGA will keep BL and WL in square shape if possible where BL might be one bit longer than WL in some cases
  For example: 
    - If the configuration bits of a PB is 9 bits, then BL=3 and WL=3
    - If the configuration bits of a PB is 11 bits, then BL=4 and WL=3 (where there is one extra bit as phantom bit)
    - If the configuration bits of a PB is 14 bits, then BL=4 and WL=4 (where there is two extra bits as phantom bits)
    
This setting allow OpenFPGA to use a fixed WL size, instead of default approach

Template
~~~~~~~~

.. code-block:: xml

  <ql_memory_bank_config_setting>
    <pb_type name="<string>" num_wl="<int>"/>
  </ql_memory_bank_config_setting>

.. option:: name="<string>"

  Specify the name of PB type, for example: clb, dsp, bram and etc

.. option:: num_wl="<int>"

  Fix the size of WL
  
  For example: 
    Considered that the configuration bits of a PB is 400 bits.
    
    If num_wl is not defined, then 
     - BL will be 20 [=ceiling(square_root(400))]
     - WL will be 20 [=ceiling(400/20)]
    
    If num_wl is defined as 10, then 
     - WL will be fixed as 10
     - BL will be 40 [=ceiling(400/10)]

    If num_wl is defined as 32, then 
     - WL will be fixed as 32
     - BL will be 13 [=ceiling(400/32)]
     - There will be 16 bits [=(32x13)-400] as phantom bits. 
