*****************************
*     FPGA SPICE Netlist    *
* Description: Parameters for measurement *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
***** Parameters For Slew Measurement *****
***** Rising Edge *****
.param slew_upper_thres_pct_rise=0.95
.param slew_lower_thres_pct_rise=0.05
***** Falling Edge *****
.param slew_upper_thres_pct_fall=0.05
.param slew_lower_thres_pct_fall=0.95
***** Parameters For Delay Measurement *****
***** Rising Edge *****
.param input_thres_pct_rise=0.5
.param output_thres_pct_rise=0.5
***** Falling Edge *****
.param input_thres_pct_fall=0.5
.param output_thres_pct_fall=0.5
