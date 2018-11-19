*****************************
*     FPGA SPICE Netlist    *
* Description: Parameters for Stimulations *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:08 2018
 *
*****************************
***** Frequency *****
.param clock_period=4.10047e-10
***** Parameters For Input Stimulations *****
.param input_slew_pct_rise='2.5e-11/clock_period'
.param input_slew_pct_fall='2.5e-11/clock_period'
***** Parameters For Clock Stimulations *****
***** Slew *****
.param clock_slew_pct_rise='2e-11/clock_period'
.param clock_slew_pct_fall='2e-11/clock_period'
