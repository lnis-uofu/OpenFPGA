.. _developer_naming_convention:

Naming Convention
=================

.. _developer_naming_convention_cell_names:

Cell Names
----------

.. warning:: This is a different concept than the cell names in :ref:`developer_naming_convention_ff_model_names`!

.. note:: we refer to standard cell wrapper here. Wrappers are built to make netlists portable between PDKs as well as across standard cell libraries in a PDK.

For code readability, the cell name should follow the convention
::
  <Cell_Function><Set_Features><Reset_Features><Output_Features><Drive_Strength>_<Wrapper>

.. option:: Cell_Function

  Name of logic function, e.g., AND2, XNOR3, etc.

.. option:: Set_Features

  This is mainly for sequential cells, e.g., D-type flip-flops. If a cell contains a set signal, its existence and polarity must be inferreable by the cell name. The available options are 
  
  - S: Asynchronous active-high set 
  - SYNS: Synchronous active-hight set
  - SN: Asynchronous active-low set
  - SYNSN: Synchronous active-low set

  .. note:: For cells without set, this keyword should be empty

.. option:: Reset_Features

  This is mainly for sequential cells, e.g., D-type flip-flops. If a cell contains a reset signal, its existence and polarity must be inferreable by the cell name. The available options are 
  
  - R: Asynchronous active-high reset 
  - SYNR: Synchronous active-hight reset
  - RN: Asynchronous active-low reset
  - SYNRN: Synchronous active-low reset

  .. note:: For cells without reset, this keyword should be empty

.. option:: Output_Features

  This is mainly for sequential cells, e.g., D-type flip-flops.

  - If not specified, the sequential cell contains a pair of differential outputs, e.g., ``Q`` and ``QN``
  - If specified, the sequential cell only contains single output, e.g., ``Q`` 

  The available options are
  
  - Q: single output which is positive
  - QN: single ouput which is negative

  .. note:: For cells without reset, this keyword should be empty

.. option:: Drive_Strength

  This is to specify the drive strength of a cell

  - If not specified, we assume minimum drive strength, i.e., ``D0``.
  - If specified, we expect a format of ``D<int>``, where the integer indicates the drive strength

.. option:: Wrapper

  This is to specify if the cell is a wrapper of an existing standard cell

  - If not specified, we assume this cell contains RTL
  - If specified, we assume this cell is a wrapper of an existing standard cell

A quick example
::
  NAND2D4_WRAPPER

represents a wrapper for a standard cell that is a 2-input NAND gate with a drive strength of 4

Another example
::
  SDFFSSYNRNQ

represents a scan-chain flip-flop which contains
 
  - Asynchronous active-high set
  - Synchronous active-low reset
  - Single output

Pin Names
---------

.. note:: Please use lowercase as much as you can

For code readability, the pin name should follow the convention
::
  <Pin_Name>_<Polarity><Direction>


.. option:: Pin_Name

  Represents the pin name

.. option:: Polarity

  Represents polarity of the pin, it can be 

  - ``n`` denotes a negative-enable (active_low) signal 

  .. note:: When not specified, by default we assume this is a postive-enable (active-high) signal

.. option:: Direction

  Represents the direction of a pin, it can be 

  - ``i`` denotes an input signal
  - ``o`` denotes an output signal

A quick example
::
  clk_ni

represents an input clock signal which is negative-enable

Another example
::
  q_no

represents an output Q signal which is negative to the input

.. _developer_naming_convention_ff_model_names:

Flip-flop Model Names
---------------------

.. warning:: This is a different concept than the cell names in :ref:`developer_naming_convention_cell_names`!

.. note:: we refer to virtual cell model (used by VPR and Yosys for cell mapping) here.

For code readability, D-type flip-flop model names should follow the convention
::
  <Sync_Type>dff<Trigger_Type><Set_Type><Reset_Type>

.. option:: Sync_Features

  Represents if the reset/set is synchronous or asynchronous to the clock, it can be 

  - ``s`` denotes a synchronous behavior
  - an empty string "" denotes an asynchronous behavior, e.g., ``ffr``

.. option:: Trigger_Type

  Represents if the flip-flop is triggered by rising edge or falling edge of a clock, it can be 

  - ``n`` means triggered by failling edge 
  - an empty string "" means triggered by rising edge, e.g., ``ff``

.. option:: Set_Type

  Represents if the flip-flop has a set and the polarity of the set, it can be 

  - ``s`` means that the flip-flop has an active-high set pin
  - ``sn`` means that the flip-flop has an active-low set pin
  - an empty string "" means the flip-flop does not have a set pin, e.g., ``ff``

.. option:: Reset_Type

  Represents if the flip-flop has a reset and the polarity of the reset, it can be 

  - ``r`` means that the flip-flop has an active-high reset pin
  - ``rn`` means that the flip-flop has an active-low reset pin
  - an empty string "" means the flip-flop does not have a reset pin, e.g., ``ff``


A quick example
::
  ffnrn

represents a flip-flop 

- triggered by falling edge
- with an asynchronous active-low reset

Another example
::
  sffs

represents a flip-flop 

- triggered by rising edge
- with a synchronous active-high set

.. _developer_naming_convention_mux_model_names:

Multiplexer Model Names
-----------------------

.. warning:: This is a different concept than the cell names in :ref:`developer_naming_convention_cell_names`!

.. note:: Here, we refer to the circuit model name used in OpenFPGA architecture file.

For code readability, a routing multiplexer circuit model name should follow the convention
::
  <Location>_mux_<Load>

.. option:: Location

  Represents the location of the routing multiplexers, it can be 

  - ``cb`` denotes a routing multiplexer in a connection block
  - ``sb`` denotes a routing multiplexer in a switch block
  - ``pb`` denotes a routing multiplexer in a programmable block

.. option:: Load

  Represents the output load condition of the routing multiplexers, it can be 

  - ``highload`` means that the routing multiplexer has to drive a very high capacitive load, which potentially requires a big buffer at output
  - an empty string "" means the routing multiplexer requires only a typical buffer size.

A quick example
::
  pb_mux_highload

represents a routing multiplexer used in a programmable block which drives a high capacitive load
