Parameters for SPICE simulation settings
========================================
All the parameters that need to be defined in the HSPICE simulations are located under a child node called <parameters>, which is under its father node <spice_settings>. 
The parameters are divided into three categories and can be defined in three XML nodes, <options>, <measure> and <stimulate>, respectively. 

* The XML node <options>

.. code-block:: xml

   <options sim_temp=”int” post=”string”captab=”string” fast=”string”/> 

These properties define the options that will be printed in the top SPICE netlists.

* **sim_temp:** specify the temperature which will be defined in SPICE netlists. In the top SPICE netlists, it will show as .temp <int>.

* **post:** [on|off]. Specify if the simulation waveforms should be printed out after SPICE simulations. In all the SPICE netlists, it will show as .option POST when turned on.

.. note:: when the SPICE netlists are large or a long simulation time period is defined, the post option is recommended to be off. If not, huge disk space will be occupied by the waveform files.

* **captab:** [on|off]. Specify if the capacitances of all the nodes in the SPICE netlists will be printed out. In the top SPICE netlists, it will show as .option CAPTAB when turned on. When turned on, the SPICE simulation runtime may increase.

* The XML node <stimulate>

.. code-block:: xml

    <stimulate>
      <clock op_freq=”auto|float” sim_slack=”float” prog_freq=”float”>
        <rise slew_time=”float” slew_type=”string”/>
        <fall slew_time=”float” slew_type=”string”/>
      </clock>
    </stimulate>

Define stimulates for the clock signal.

* **op_freq:** either auto or a float number (unit:[Hz])  Specify the operation clock frequency that are used in SPICE simulations. This frequency is used in testbenches for operation phase simulation. Note that this is a mandatory option. Users have to specify either this frequency is automatically determined by assigning “auto”, or give an exact number. If this clock frequency is specified. The sim_slack option is disgarded.

* **sim_slack:** add a slack to the critical path delay in the SPICE simulation. For example, sim_slack=0.2 implies that the clock period in SPICE simulations is 1.2 of the critical path delay reported by VPR. **Only valid when option op_freq is not specified.**

* **prog_freq:** Specify the programming clock frequency that are used in SPICE simulations. This frequency is used in testbenches for programming phase simulation.

* **slew_type & slew_time:** define the slew of clock signals at the rising/falling edge. Property slew_type can be either absolute or fractional [abs|frac]. 

	* The type of **absolute** implies that the slew time is the absolute value. For example, slew_time=20e-12, slew_type=abs means that the slew of a clock signal is  20ps. 
	* The type of **fractional** means that the slew time is related to the time period (frequency) of the clock signal. For example, slew_time=0.05, slew_type=frac means that the slew of a clock signal takes 5% of the time period of the clock.

:numref:`fig_meas_edge` depicts the definition of the slew and delays of signals and the parameters that can be supported by FPGA-SPICE.

.. code-block:: xml

     <stimulate>
       <input>
         <rise slew_time=”float” slew_type=”string”/>
         <fall slew_time=”float” slew_type=”string”/>
       </input>
     </stimulate>

Define the slew of input signals at the rising/falling edge.

* **slew_type & slew_time:** define the slew of all the input signals at the rising/falling edge. Property slew_type can be either absolute or fractional [abs|frac]. 

	* The type of **absolute** implies that the slew time is the absolute value. For example, slew_time=20e-12, slew_type=abs means that the slew of a clock signal is  20ps. 

	* The type of **fractional** means that the slew time is related to the time period (frequency) of the clock signal. For example, slew_time=0.05, slew_type=frac means that the slew of a clock signal takes 5% of the time period of the clock.

.. note:: These slew settings are valid for all the input signals of the testbenches in different complexity levels.

.. _fig_meas_edge:

.. figure:: figures/meas_edge.png 
   :scale: 100%
   :alt: map to buried traesure
  
   Parameter in measuring the slew and delay of signals

* The XML node <measure>

.. code-block:: xml
    
   <measure sim_num_clock_cycle=”int”accuracy=”float”accuracy_type=”string”/>

* **sim_num_clock_cycle:** can be either “auto” or an integer. By setting to “auto”, FPGA-SPICE automatically determines the number of clock cycles to simulate, which is related to the average of all the signal density in ACE2 results. When set to an integer, FPGA-SPICE will use the given number of clock cycles in the SPICE netlists.
    
* **accuracy_type:** [abs|frac]. Specify the type of transient step in SPICE simulation. 

	* When **abs** is selected, the accuracy should be the absolute value, such as 1e-12. 

	* When **frac** is selected, the accuracy is the number of simulation points in a clock cycle time period, for example, 100.
    
* **accuracy:** specify the transient step in SPICE simulation. Typically, the smaller the step is, the higher accuracy can be reached while the long simulation runtime is. The recommended accuracy is between 0.1ps and 0.01ps, which generates good accuracy and runtime is not significantly long. 
    
.. note:: Users can define the parameters in measuring the slew of signals, under a child node <slew> of the node <measure>.

.. code-block:: xml
    
    <rise upper_thres_pct=”float” lower_thres_pct=”float”/>

Define the starting and ending point in measuring the slew of a rising edge of a signal.
    
* **upper_thres_pct:** the ending point in measuring the slew of a rising edge. It is expressed as a percentage of the maximum voltage of a signal. For example, the meaning of upper_thres_pct=0.95 is depicted in Figure 2. 
    
* **lower_thres_pct:** the starting point in measuring the slew of a rising edge. It is expressed as a percentage of the maximum voltage of a signal. For example, the meaning of lower_thres_pct=0.05 is depicted in Figure 2.
    
.. code-block:: xml
    
    <fall upper_thres_pct=”float” lower_thres_pct=”float”/>

* **upper_thres_pct:** the ending point in measuring the slew of a falling edge. It is expressed as a percentage of the maximum voltage of a signal. For example, the meaning of upper_thres_pct=0.05 is depicted in Figure 2.
    
 * **lower_thres_pct:** the starting point in measuring the slew of a falling edge. It is expressed as a percentage of the maximum voltage of a signal. For example, the meaning of lower_thres_pct=0.95 is depicted in Figure 2.
    
    
.. note:: Users can define the parameters related to measurements of delays between signals, under a child node <delay> of the node <measure>.

.. code-block:: xml
    
    <rise input_thres_pct=”float” output_thres_pct=”float”/>

Define the starting and ending point in measuring the delay between two signals when they are both at a rising edge.
    
* **input_thres_pct:** the starting point in measuring the delay of a rising edge. It is expressed as a percentage of the maximum voltage of a signal. For example, the meaning of input_thres_pct=0.5 is depicted in Figure 2.     

* **output_thres_pct:** the ending point in measuring the delay of a rising edge. It is expressed as a percentage of the maximum voltage of a signal. For example, the meaning of output_thres_pct=0.5 is depicted in Figure 2.
    
.. code-block:: xml
    
    <fall input_thres_pct=”float” output_thres_pct=”float”/>

Define the starting and ending point in measuring the delay between two signals when they are both at a falling edge.

* **input_thres_pct:** the starting point in measuring the delay of a falling edge. It is expressed as a percentage of the maximum voltage of a signal. For example, upper_thres_pct=0.5 is depicted in :numref:`fig_meas_edge`. 
    
* **output_thres_pct:** the ending point in measuring the delay of a falling edge. It is expressed as a percentage of the maximum voltage of a signal. For example, lower_thres_pct=0. 5 is depicted in :numref:`fig_meas_edge`.
    
    
