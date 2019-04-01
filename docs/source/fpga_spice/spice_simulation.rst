Run SPICE simulation
====================

* Simulation results 

The HSPICE simulator creates an LIS file (\*.lis) to store the results. In each LIS file, you can find the leakage power and dynamic power of each module, as well the total leakage power and the total dynamic power of all the modules in a SPICE netlist.

The following is an example of simulation results of a pb_mux testbench.::

 total_leakage_srams= -16.4425u  

 total_dynamic_srams=  83.0480u 

 total_energy_per_cycle_srams= 269.7773f

 total_leakage_power_mux[0to76]=-140.1750u

 total_energy_per_cycle_mux[0to76]= -37.5871p

 total_leakage_power_pb_mux=-140.1750u

 total_energy_per_cycle_pb_mux= -37.5871p

.. note:: total_energy_per_cycle_srams represents the total energy per cycle of all the SRAMs of the multiplexers in this testbench, while total_energy_per_cycle_pb_mux is the total energy per cycle of all the multiplexer structures in this testbench.
  
  Therefore, the total energy per cycle of all the multiplexers in this testbench should be the sum of total_energy_per_cycle_srams and total_energy_per_cycle_pb_mux.

  Similarly, the total leakage power of all the multiplexers in this testbench should be the sum of total_leakage_srams and  total_leakage_power_pb_mux.

  The leakage power is measured for the first clock cycle, where FPGA-SPICE set all the voltage stimuli in constant voltage levels.

  The total energy per cycle is measured for the rest of clock cycles (the 1st clock cycle is not included). 
 
The total power can be calculated by, 

:math:`total\_energy\_per\_cycle \cdot clock\_freq`

where clock_freq is the clock frequency used in SPICE simulations.

