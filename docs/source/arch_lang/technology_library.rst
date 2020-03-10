.. _technology_library:

Technology library
------------------

For OpenFPGA using VPR7
~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: xml

  <tech_lib lib_type=”string” transistor_type=”string” lib_path=”string” nominal_vdd=”float”/>

* **lib_type:** can be either industry or academia [industry|academia]. For the industry library, some transistor types are available, and the type of transistor should be declared in the property transistor_type. 

* **transistor_type:** This XML property specify the transistors to be used in the industry library. For example, the type of transistors can be “TT”, “FF” etc.

* **lib_path:** specify the path of the library. For example: lib_path=/home/tech/45nm.pm.

* **nominal_vdd:** specify the working voltage for the technology. The voltage will be used as the supply voltage in all the SPICE netlist.

.. code-block:: xml

   <transistors pn_ratio=”float” model_ref=”string”/>

* **pn_ratio:** specify the ratio between p-type transistors and n-type transistors. The ratio will be used when building circuit structures such as inverters, buffers, etc.
    
* **model_ref:** specify the reference of in calling a transistor model. In SPICE netlist, define a transistor follows the convention: <model_ref><trans_name> <ports> <model_name>. The reference depends on the technology and the type of library. For example, the PTM bulk model uses “M” as the reference while the PTM FinFET model uses “X” as the reference.

.. code-block:: xml

   <nmos model_name=”string” chan_length=”float” min_width=”float”/>
   <pmos model_name=”string” chan_length=”float” min_width=”float”/>

* **model_name:**  specify the name of the p/n type transistor, which can be found in the manual of the technology provider.
   
* **chan_length:** specify the channel length of p/n type transistor.
  
* **min_width:** specify the minimum width of p/n type transistor. This parameter will be used in building inverter, buffer, etc. as a base number for transistor sizing. 
  
For OpenFPGA using VPR8
~~~~~~~~~~~~~~~~~~~~~~~

Technology library aims to describe transistor-level parameters to be applied to the physical design of FPGAs. In addition to transistor models, technology library also supports the definition of process variations on any transistor models. 
General organization is as follows.

.. code-block:: xml

  <technology_library>
    <device_library>
      <device_model name="<string>" type="<string>">
        <lib type="<string>" corner="<string>" ref="<string>" path="<string>"/>
        <design vdd="<float>" pn_ratio="<float>"/>
        <pmos name="<string>" chan_length="<float>" min_width="<float>" variation="<string>"/>
        <nmos name="<string>" chan_length="<float>" min_width="<float>" variation="<string>"/>
        <rram rlrs="<float>" rhrs="<float>" variation="<string>"/> 
      </device_model>
    </device_library>
    <variation_library>
      <variation name="<string>" abs_deviation="<float>" num_sigma="<int>"/>
    </variation_library>
  </technology_library>

Device Library
^^^^^^^^^^^^^^
Device library contains detailed description on device models, such as transistors and Resistive Random Access Memories (RRAMs).
A device library may consist of a number of ``<device_model>`` and each of them denotes a different transistor model.

A device model represents a transistor/RRAM model available in users' technology library.

.. option:: <device_model name="<string>" type="<string>">
  
  Specify the name and type of a device model
  
  - ``name="<string>"`` is the unique name of the device model in the context of ``<device_library>``. 
  - ``type="transistor|rram"`` is the type of device model in terms of functionality
    Currently, OpenFPGA supports two types: transistor and RRAM.

.. note:: the name of ``<device_model>`` may not be the name in users' technology library.

.. option:: <lib type="<string>" corner="<string>" ref="<string>" path="<string>"/>
  Specify the technology library that defines the device model

  - ``type="academia|industry"``  For the industry library, FPGA-SPICE will use ``.lib <lib_file_path>`` to include the library file in SPICE netlists. For academia library, FPGA-SPICE will use ``.include <lib_file_path>`` to include the library file in SPICE netlists

  - ``corner="<string>"`` is the process corner name available in technology library. 
    For example, the type of transistors can be ``TT``, ``SS`` and ``FF`` *etc*.

  - ``ref="<string>"`` specify the reference of in calling a transistor model. In SPICE netlists, define a transistor follows the convention: 

  .. code-block:: xml

    <model_ref><trans_name> <ports> <model_name>

  The reference depends on the technology and the type of library. For example, the PTM bulk model uses “M” as the reference while the PTM FinFET model uses “X” as the reference.

  - ``path="<string>"`` specify the path of the technology library file. For example: 

  .. code-block:: xml 

    lib_path=/home/tech/45nm.pm.

.. option:: <design vdd="<float>" pn_ratio="<float>"/>

   Specify transistor-level design parameters

   - ``vdd="<float>"`` specify the working voltage for the technology. The voltage will be used as the supply voltage in all the SPICE netlists.
 
   - ``pn_ratio="<float>"`` specify the ratio between *p*-type and *n*-type transistors. The ratio will be used when building circuit structures such as inverters, buffers, etc.

.. option:: <pmos|nmos name="<string>" chan_length="<float>" min_width="<float>" variation="<string>"/>
  
  Specify device-level parameters for transistors

  - ``name="<string>"`` specify the name of the p/n type transistor, which can be found in the manual of the technology provider.

  - ``chan_length="<float>"`` specify the channel length of *p/n* type transistor.
  
  - ``min_width="<float>"`` specify the minimum width of *p/n* type transistor. This parameter will be used in building inverter, buffer, *etc*. as a base number for transistor sizing. 

  - ``variation="<string>"`` specify the variation name defined in the ``<variation_library>`` 

.. option:: <rram rlrs="<float>" rhrs="<float>" variation="<string>"/> 

  Specify device-level parameters for RRAMs

  - ``rlrs="<float>"`` specify the resistance of Low Resistance State (LRS) of a RRAM device

  - ``rhrs="<float>"`` specify the resistance of High Resistance State (HRS) of a RRAM device 

  - ``variation="<string>"`` specify the variation name defined in the ``<variation_library>`` 

Variation Library
^^^^^^^^^^^^^^^^^
Variation library contains detailed description on device variations specified by users.
A variation library may consist of a number of ``<variation>`` and each of them denotes a different variation parameter.

.. option:: <variation name="<string>" abs_deviation="<float>" num_sigma="<int>"/>
  
  Specify detail variation parameters

  - ``name="<string>"`` is the unique name of the device variation in the context of ``<variation_library>``.  The name will be used in ``<device_model>`` to bind variations
  
  - ``abs_variation="<float>"`` is the absolute deviation of a variation

  - ``num_sigma="<int>"`` is the standard deviation of a variation
