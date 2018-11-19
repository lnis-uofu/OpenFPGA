*****************************
*     FPGA SPICE Netlist    *
* Description: FPGA SPICE Connection Box Testbench Bench for Design: example_2 *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:08 2018
 *
*****************************
****** Include Header file: circuit design parameters *****
.include './spice_test_example_2/include/design_params.sp'
****** Include Header file: measurement parameters *****
.include './spice_test_example_2/include/meas_params.sp'
****** Include Header file: stimulation parameters *****
.include './spice_test_example_2/include/stimulate_params.sp'
****** Include subckt netlists: NMOS and PMOS *****
.include './spice_test_example_2/subckt/nmos_pmos.sp'
****** Include subckt netlists: Inverters, Buffers *****
.include './spice_test_example_2/subckt/inv_buf_trans_gate.sp'
****** Include subckt netlists: Multiplexers *****
.include './spice_test_example_2/subckt/muxes.sp'
****** Include subckt netlists: Wires *****
.include './spice_test_example_2/subckt/wires.sp'
.include '/research/ece/lnis/USERS/tang/tangxifan-eda-tools/branches/vpr7_rram/vpr/SpiceNetlists/ff.sp'
.include '/research/ece/lnis/USERS/tang/tangxifan-eda-tools/branches/vpr7_rram/vpr/SpiceNetlists/sram.sp'
.include '/research/ece/lnis/USERS/tang/tangxifan-eda-tools/branches/vpr7_rram/vpr/SpiceNetlists/io.sp'
.temp 25
.option fast
***** Generic global ports ***** 
***** VDD, GND  ***** 
.global gvdd
.global ggnd
***** Global set ports ***** 
.global gset gset_inv 
***** Global reset ports ***** 
.global greset greset_inv
***** Configuration done ports ***** 
.global gconfig_done gconfig_done_inv
***** Global SRAM input ***** 
.global sram->in
***** Global Clock Signals *****
.global gclock
.global gclock_inv
***** User-defined global ports ****** 
.global 

***** BEGIN Global ports *****
+  zin[0]  clk[0]  Reset[0]  Set[0] 
***** END Global ports *****
.global gvdd_sram_cbs
****** Include subckt netlists: Connection Box X-channel  [1][1] *****
.include './spice_test_example_2/subckt/cbx_1_1.sp'
***** Call defined Connection Box[1][1] *****
Xcbx[1][1] 
+ chanx[1][1]_midout[0] 
+ chanx[1][1]_midout[1] 
+ chanx[1][1]_midout[2] 
+ chanx[1][1]_midout[3] 
+ chanx[1][1]_midout[4] 
+ chanx[1][1]_midout[5] 
+ chanx[1][1]_midout[6] 
+ chanx[1][1]_midout[7] 
+ chanx[1][1]_midout[8] 
+ chanx[1][1]_midout[9] 
+ chanx[1][1]_midout[10] 
+ chanx[1][1]_midout[11] 
+ chanx[1][1]_midout[12] 
+ chanx[1][1]_midout[13] 
+ chanx[1][1]_midout[14] 
+ chanx[1][1]_midout[15] 
+ chanx[1][1]_midout[16] 
+ chanx[1][1]_midout[17] 
+ chanx[1][1]_midout[18] 
+ chanx[1][1]_midout[19] 
+ chanx[1][1]_midout[20] 
+ chanx[1][1]_midout[21] 
+ chanx[1][1]_midout[22] 
+ chanx[1][1]_midout[23] 
+ chanx[1][1]_midout[24] 
+ chanx[1][1]_midout[25] 
+ chanx[1][1]_midout[26] 
+ chanx[1][1]_midout[27] 
+ chanx[1][1]_midout[28] 
+ chanx[1][1]_midout[29] 
+ chanx[1][1]_midout[30] 
+ chanx[1][1]_midout[31] 
+ chanx[1][1]_midout[32] 
+ chanx[1][1]_midout[33] 
+ chanx[1][1]_midout[34] 
+ chanx[1][1]_midout[35] 
+ chanx[1][1]_midout[36] 
+ chanx[1][1]_midout[37] 
+ chanx[1][1]_midout[38] 
+ chanx[1][1]_midout[39] 
+ chanx[1][1]_midout[40] 
+ chanx[1][1]_midout[41] 
+ chanx[1][1]_midout[42] 
+ chanx[1][1]_midout[43] 
+ chanx[1][1]_midout[44] 
+ chanx[1][1]_midout[45] 
+ chanx[1][1]_midout[46] 
+ chanx[1][1]_midout[47] 
+ chanx[1][1]_midout[48] 
+ chanx[1][1]_midout[49] 
+ chanx[1][1]_midout[50] 
+ chanx[1][1]_midout[51] 
+ chanx[1][1]_midout[52] 
+ chanx[1][1]_midout[53] 
+ chanx[1][1]_midout[54] 
+ chanx[1][1]_midout[55] 
+ chanx[1][1]_midout[56] 
+ chanx[1][1]_midout[57] 
+ chanx[1][1]_midout[58] 
+ chanx[1][1]_midout[59] 
+ chanx[1][1]_midout[60] 
+ chanx[1][1]_midout[61] 
+ chanx[1][1]_midout[62] 
+ chanx[1][1]_midout[63] 
+ chanx[1][1]_midout[64] 
+ chanx[1][1]_midout[65] 
+ chanx[1][1]_midout[66] 
+ chanx[1][1]_midout[67] 
+ chanx[1][1]_midout[68] 
+ chanx[1][1]_midout[69] 
+ chanx[1][1]_midout[70] 
+ chanx[1][1]_midout[71] 
+ chanx[1][1]_midout[72] 
+ chanx[1][1]_midout[73] 
+ chanx[1][1]_midout[74] 
+ chanx[1][1]_midout[75] 
+ chanx[1][1]_midout[76] 
+ chanx[1][1]_midout[77] 
+ chanx[1][1]_midout[78] 
+ chanx[1][1]_midout[79] 
+ chanx[1][1]_midout[80] 
+ chanx[1][1]_midout[81] 
+ chanx[1][1]_midout[82] 
+ chanx[1][1]_midout[83] 
+ chanx[1][1]_midout[84] 
+ chanx[1][1]_midout[85] 
+ chanx[1][1]_midout[86] 
+ chanx[1][1]_midout[87] 
+ chanx[1][1]_midout[88] 
+ chanx[1][1]_midout[89] 
+ chanx[1][1]_midout[90] 
+ chanx[1][1]_midout[91] 
+ chanx[1][1]_midout[92] 
+ chanx[1][1]_midout[93] 
+ chanx[1][1]_midout[94] 
+ chanx[1][1]_midout[95] 
+ chanx[1][1]_midout[96] 
+ chanx[1][1]_midout[97] 
+ chanx[1][1]_midout[98] 
+ chanx[1][1]_midout[99] 
+ grid[1][2]_pin[0][2][0] 
+ grid[1][2]_pin[0][2][2] 
+ grid[1][2]_pin[0][2][4] 
+ grid[1][2]_pin[0][2][6] 
+ grid[1][2]_pin[0][2][8] 
+ grid[1][2]_pin[0][2][10] 
+ grid[1][2]_pin[0][2][12] 
+ grid[1][2]_pin[0][2][14] 
+ grid[1][1]_pin[0][0][0] 
+ grid[1][1]_pin[0][0][4] 
+ grid[1][1]_pin[0][0][8] 
+ grid[1][1]_pin[0][0][12] 
+ grid[1][1]_pin[0][0][16] 
+ grid[1][1]_pin[0][0][20] 
+ grid[1][1]_pin[0][0][24] 
+ grid[1][1]_pin[0][0][28] 
+ grid[1][1]_pin[0][0][32] 
+ grid[1][1]_pin[0][0][36] 
+ gvdd_cbx[1][1] 0 cbx[1][1]
***** Signal chanx[1][1]_midout[0] density = 0, probability=0.*****
Vchanx[1][1]_midout[0] chanx[1][1]_midout[0] 0 
+  0
***** Signal chanx[1][1]_midout[1] density = 0, probability=0.*****
Vchanx[1][1]_midout[1] chanx[1][1]_midout[1] 0 
+  0
***** Signal chanx[1][1]_midout[2] density = 0, probability=0.*****
Vchanx[1][1]_midout[2] chanx[1][1]_midout[2] 0 
+  0
***** Signal chanx[1][1]_midout[3] density = 0, probability=0.*****
Vchanx[1][1]_midout[3] chanx[1][1]_midout[3] 0 
+  0
***** Signal chanx[1][1]_midout[4] density = 0, probability=0.*****
Vchanx[1][1]_midout[4] chanx[1][1]_midout[4] 0 
+  0
***** Signal chanx[1][1]_midout[5] density = 0, probability=0.*****
Vchanx[1][1]_midout[5] chanx[1][1]_midout[5] 0 
+  0
***** Signal chanx[1][1]_midout[6] density = 0, probability=0.*****
Vchanx[1][1]_midout[6] chanx[1][1]_midout[6] 0 
+  0
***** Signal chanx[1][1]_midout[7] density = 0, probability=0.*****
Vchanx[1][1]_midout[7] chanx[1][1]_midout[7] 0 
+  0
***** Signal chanx[1][1]_midout[8] density = 0, probability=0.*****
Vchanx[1][1]_midout[8] chanx[1][1]_midout[8] 0 
+  0
***** Signal chanx[1][1]_midout[9] density = 0, probability=0.*****
Vchanx[1][1]_midout[9] chanx[1][1]_midout[9] 0 
+  0
***** Signal chanx[1][1]_midout[10] density = 0, probability=0.*****
Vchanx[1][1]_midout[10] chanx[1][1]_midout[10] 0 
+  0
***** Signal chanx[1][1]_midout[11] density = 0, probability=0.*****
Vchanx[1][1]_midout[11] chanx[1][1]_midout[11] 0 
+  0
***** Signal chanx[1][1]_midout[12] density = 0, probability=0.*****
Vchanx[1][1]_midout[12] chanx[1][1]_midout[12] 0 
+  0
***** Signal chanx[1][1]_midout[13] density = 0, probability=0.*****
Vchanx[1][1]_midout[13] chanx[1][1]_midout[13] 0 
+  0
***** Signal chanx[1][1]_midout[14] density = 0, probability=0.*****
Vchanx[1][1]_midout[14] chanx[1][1]_midout[14] 0 
+  0
***** Signal chanx[1][1]_midout[15] density = 0, probability=0.*****
Vchanx[1][1]_midout[15] chanx[1][1]_midout[15] 0 
+  0
***** Signal chanx[1][1]_midout[16] density = 0, probability=0.*****
Vchanx[1][1]_midout[16] chanx[1][1]_midout[16] 0 
+  0
***** Signal chanx[1][1]_midout[17] density = 0, probability=0.*****
Vchanx[1][1]_midout[17] chanx[1][1]_midout[17] 0 
+  0
***** Signal chanx[1][1]_midout[18] density = 0, probability=0.*****
Vchanx[1][1]_midout[18] chanx[1][1]_midout[18] 0 
+  0
***** Signal chanx[1][1]_midout[19] density = 0, probability=0.*****
Vchanx[1][1]_midout[19] chanx[1][1]_midout[19] 0 
+  0
***** Signal chanx[1][1]_midout[20] density = 0.2026, probability=0.4982.*****
Vchanx[1][1]_midout[20] chanx[1][1]_midout[20] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
***** Signal chanx[1][1]_midout[21] density = 0, probability=0.*****
Vchanx[1][1]_midout[21] chanx[1][1]_midout[21] 0 
+  0
***** Signal chanx[1][1]_midout[22] density = 0, probability=0.*****
Vchanx[1][1]_midout[22] chanx[1][1]_midout[22] 0 
+  0
***** Signal chanx[1][1]_midout[23] density = 0, probability=0.*****
Vchanx[1][1]_midout[23] chanx[1][1]_midout[23] 0 
+  0
***** Signal chanx[1][1]_midout[24] density = 0, probability=0.*****
Vchanx[1][1]_midout[24] chanx[1][1]_midout[24] 0 
+  0
***** Signal chanx[1][1]_midout[25] density = 0, probability=0.*****
Vchanx[1][1]_midout[25] chanx[1][1]_midout[25] 0 
+  0
***** Signal chanx[1][1]_midout[26] density = 0, probability=0.*****
Vchanx[1][1]_midout[26] chanx[1][1]_midout[26] 0 
+  0
***** Signal chanx[1][1]_midout[27] density = 0, probability=0.*****
Vchanx[1][1]_midout[27] chanx[1][1]_midout[27] 0 
+  0
***** Signal chanx[1][1]_midout[28] density = 0, probability=0.*****
Vchanx[1][1]_midout[28] chanx[1][1]_midout[28] 0 
+  0
***** Signal chanx[1][1]_midout[29] density = 0, probability=0.*****
Vchanx[1][1]_midout[29] chanx[1][1]_midout[29] 0 
+  0
***** Signal chanx[1][1]_midout[30] density = 0, probability=0.*****
Vchanx[1][1]_midout[30] chanx[1][1]_midout[30] 0 
+  0
***** Signal chanx[1][1]_midout[31] density = 0, probability=0.*****
Vchanx[1][1]_midout[31] chanx[1][1]_midout[31] 0 
+  0
***** Signal chanx[1][1]_midout[32] density = 0, probability=0.*****
Vchanx[1][1]_midout[32] chanx[1][1]_midout[32] 0 
+  0
***** Signal chanx[1][1]_midout[33] density = 0, probability=0.*****
Vchanx[1][1]_midout[33] chanx[1][1]_midout[33] 0 
+  0
***** Signal chanx[1][1]_midout[34] density = 0, probability=0.*****
Vchanx[1][1]_midout[34] chanx[1][1]_midout[34] 0 
+  0
***** Signal chanx[1][1]_midout[35] density = 0, probability=0.*****
Vchanx[1][1]_midout[35] chanx[1][1]_midout[35] 0 
+  0
***** Signal chanx[1][1]_midout[36] density = 0, probability=0.*****
Vchanx[1][1]_midout[36] chanx[1][1]_midout[36] 0 
+  0
***** Signal chanx[1][1]_midout[37] density = 0, probability=0.*****
Vchanx[1][1]_midout[37] chanx[1][1]_midout[37] 0 
+  0
***** Signal chanx[1][1]_midout[38] density = 0, probability=0.*****
Vchanx[1][1]_midout[38] chanx[1][1]_midout[38] 0 
+  0
***** Signal chanx[1][1]_midout[39] density = 0, probability=0.*****
Vchanx[1][1]_midout[39] chanx[1][1]_midout[39] 0 
+  0
***** Signal chanx[1][1]_midout[40] density = 0, probability=0.*****
Vchanx[1][1]_midout[40] chanx[1][1]_midout[40] 0 
+  0
***** Signal chanx[1][1]_midout[41] density = 0, probability=0.*****
Vchanx[1][1]_midout[41] chanx[1][1]_midout[41] 0 
+  0
***** Signal chanx[1][1]_midout[42] density = 0, probability=0.*****
Vchanx[1][1]_midout[42] chanx[1][1]_midout[42] 0 
+  0
***** Signal chanx[1][1]_midout[43] density = 0, probability=0.*****
Vchanx[1][1]_midout[43] chanx[1][1]_midout[43] 0 
+  0
***** Signal chanx[1][1]_midout[44] density = 0, probability=0.*****
Vchanx[1][1]_midout[44] chanx[1][1]_midout[44] 0 
+  0
***** Signal chanx[1][1]_midout[45] density = 0, probability=0.*****
Vchanx[1][1]_midout[45] chanx[1][1]_midout[45] 0 
+  0
***** Signal chanx[1][1]_midout[46] density = 0, probability=0.*****
Vchanx[1][1]_midout[46] chanx[1][1]_midout[46] 0 
+  0
***** Signal chanx[1][1]_midout[47] density = 0, probability=0.*****
Vchanx[1][1]_midout[47] chanx[1][1]_midout[47] 0 
+  0
***** Signal chanx[1][1]_midout[48] density = 0, probability=0.*****
Vchanx[1][1]_midout[48] chanx[1][1]_midout[48] 0 
+  0
***** Signal chanx[1][1]_midout[49] density = 0, probability=0.*****
Vchanx[1][1]_midout[49] chanx[1][1]_midout[49] 0 
+  0
***** Signal chanx[1][1]_midout[50] density = 0, probability=0.*****
Vchanx[1][1]_midout[50] chanx[1][1]_midout[50] 0 
+  0
***** Signal chanx[1][1]_midout[51] density = 0, probability=0.*****
Vchanx[1][1]_midout[51] chanx[1][1]_midout[51] 0 
+  0
***** Signal chanx[1][1]_midout[52] density = 0, probability=0.*****
Vchanx[1][1]_midout[52] chanx[1][1]_midout[52] 0 
+  0
***** Signal chanx[1][1]_midout[53] density = 0, probability=0.*****
Vchanx[1][1]_midout[53] chanx[1][1]_midout[53] 0 
+  0
***** Signal chanx[1][1]_midout[54] density = 0, probability=0.*****
Vchanx[1][1]_midout[54] chanx[1][1]_midout[54] 0 
+  0
***** Signal chanx[1][1]_midout[55] density = 0, probability=0.*****
Vchanx[1][1]_midout[55] chanx[1][1]_midout[55] 0 
+  0
***** Signal chanx[1][1]_midout[56] density = 0, probability=0.*****
Vchanx[1][1]_midout[56] chanx[1][1]_midout[56] 0 
+  0
***** Signal chanx[1][1]_midout[57] density = 0, probability=0.*****
Vchanx[1][1]_midout[57] chanx[1][1]_midout[57] 0 
+  0
***** Signal chanx[1][1]_midout[58] density = 0, probability=0.*****
Vchanx[1][1]_midout[58] chanx[1][1]_midout[58] 0 
+  0
***** Signal chanx[1][1]_midout[59] density = 0, probability=0.*****
Vchanx[1][1]_midout[59] chanx[1][1]_midout[59] 0 
+  0
***** Signal chanx[1][1]_midout[60] density = 0, probability=0.*****
Vchanx[1][1]_midout[60] chanx[1][1]_midout[60] 0 
+  0
***** Signal chanx[1][1]_midout[61] density = 0, probability=0.*****
Vchanx[1][1]_midout[61] chanx[1][1]_midout[61] 0 
+  0
***** Signal chanx[1][1]_midout[62] density = 0, probability=0.*****
Vchanx[1][1]_midout[62] chanx[1][1]_midout[62] 0 
+  0
***** Signal chanx[1][1]_midout[63] density = 0, probability=0.*****
Vchanx[1][1]_midout[63] chanx[1][1]_midout[63] 0 
+  0
***** Signal chanx[1][1]_midout[64] density = 0, probability=0.*****
Vchanx[1][1]_midout[64] chanx[1][1]_midout[64] 0 
+  0
***** Signal chanx[1][1]_midout[65] density = 0, probability=0.*****
Vchanx[1][1]_midout[65] chanx[1][1]_midout[65] 0 
+  0
***** Signal chanx[1][1]_midout[66] density = 0, probability=0.*****
Vchanx[1][1]_midout[66] chanx[1][1]_midout[66] 0 
+  0
***** Signal chanx[1][1]_midout[67] density = 0, probability=0.*****
Vchanx[1][1]_midout[67] chanx[1][1]_midout[67] 0 
+  0
***** Signal chanx[1][1]_midout[68] density = 0, probability=0.*****
Vchanx[1][1]_midout[68] chanx[1][1]_midout[68] 0 
+  0
***** Signal chanx[1][1]_midout[69] density = 0, probability=0.*****
Vchanx[1][1]_midout[69] chanx[1][1]_midout[69] 0 
+  0
***** Signal chanx[1][1]_midout[70] density = 0, probability=0.*****
Vchanx[1][1]_midout[70] chanx[1][1]_midout[70] 0 
+  0
***** Signal chanx[1][1]_midout[71] density = 0, probability=0.*****
Vchanx[1][1]_midout[71] chanx[1][1]_midout[71] 0 
+  0
***** Signal chanx[1][1]_midout[72] density = 0, probability=0.*****
Vchanx[1][1]_midout[72] chanx[1][1]_midout[72] 0 
+  0
***** Signal chanx[1][1]_midout[73] density = 0, probability=0.*****
Vchanx[1][1]_midout[73] chanx[1][1]_midout[73] 0 
+  0
***** Signal chanx[1][1]_midout[74] density = 0, probability=0.*****
Vchanx[1][1]_midout[74] chanx[1][1]_midout[74] 0 
+  0
***** Signal chanx[1][1]_midout[75] density = 0, probability=0.*****
Vchanx[1][1]_midout[75] chanx[1][1]_midout[75] 0 
+  0
***** Signal chanx[1][1]_midout[76] density = 0, probability=0.*****
Vchanx[1][1]_midout[76] chanx[1][1]_midout[76] 0 
+  0
***** Signal chanx[1][1]_midout[77] density = 0, probability=0.*****
Vchanx[1][1]_midout[77] chanx[1][1]_midout[77] 0 
+  0
***** Signal chanx[1][1]_midout[78] density = 0, probability=0.*****
Vchanx[1][1]_midout[78] chanx[1][1]_midout[78] 0 
+  0
***** Signal chanx[1][1]_midout[79] density = 0, probability=0.*****
Vchanx[1][1]_midout[79] chanx[1][1]_midout[79] 0 
+  0
***** Signal chanx[1][1]_midout[80] density = 0, probability=0.*****
Vchanx[1][1]_midout[80] chanx[1][1]_midout[80] 0 
+  0
***** Signal chanx[1][1]_midout[81] density = 0, probability=0.*****
Vchanx[1][1]_midout[81] chanx[1][1]_midout[81] 0 
+  0
***** Signal chanx[1][1]_midout[82] density = 0, probability=0.*****
Vchanx[1][1]_midout[82] chanx[1][1]_midout[82] 0 
+  0
***** Signal chanx[1][1]_midout[83] density = 0, probability=0.*****
Vchanx[1][1]_midout[83] chanx[1][1]_midout[83] 0 
+  0
***** Signal chanx[1][1]_midout[84] density = 0, probability=0.*****
Vchanx[1][1]_midout[84] chanx[1][1]_midout[84] 0 
+  0
***** Signal chanx[1][1]_midout[85] density = 0, probability=0.*****
Vchanx[1][1]_midout[85] chanx[1][1]_midout[85] 0 
+  0
***** Signal chanx[1][1]_midout[86] density = 0, probability=0.*****
Vchanx[1][1]_midout[86] chanx[1][1]_midout[86] 0 
+  0
***** Signal chanx[1][1]_midout[87] density = 0, probability=0.*****
Vchanx[1][1]_midout[87] chanx[1][1]_midout[87] 0 
+  0
***** Signal chanx[1][1]_midout[88] density = 0, probability=0.*****
Vchanx[1][1]_midout[88] chanx[1][1]_midout[88] 0 
+  0
***** Signal chanx[1][1]_midout[89] density = 0, probability=0.*****
Vchanx[1][1]_midout[89] chanx[1][1]_midout[89] 0 
+  0
***** Signal chanx[1][1]_midout[90] density = 0, probability=0.*****
Vchanx[1][1]_midout[90] chanx[1][1]_midout[90] 0 
+  0
***** Signal chanx[1][1]_midout[91] density = 0, probability=0.*****
Vchanx[1][1]_midout[91] chanx[1][1]_midout[91] 0 
+  0
***** Signal chanx[1][1]_midout[92] density = 0, probability=0.*****
Vchanx[1][1]_midout[92] chanx[1][1]_midout[92] 0 
+  0
***** Signal chanx[1][1]_midout[93] density = 0, probability=0.*****
Vchanx[1][1]_midout[93] chanx[1][1]_midout[93] 0 
+  0
***** Signal chanx[1][1]_midout[94] density = 0, probability=0.*****
Vchanx[1][1]_midout[94] chanx[1][1]_midout[94] 0 
+  0
***** Signal chanx[1][1]_midout[95] density = 0, probability=0.*****
Vchanx[1][1]_midout[95] chanx[1][1]_midout[95] 0 
+  0
***** Signal chanx[1][1]_midout[96] density = 0, probability=0.*****
Vchanx[1][1]_midout[96] chanx[1][1]_midout[96] 0 
+  0
***** Signal chanx[1][1]_midout[97] density = 0, probability=0.*****
Vchanx[1][1]_midout[97] chanx[1][1]_midout[97] 0 
+  0
***** Signal chanx[1][1]_midout[98] density = 0, probability=0.*****
Vchanx[1][1]_midout[98] chanx[1][1]_midout[98] 0 
+  0
***** Signal chanx[1][1]_midout[99] density = 0, probability=0.*****
Vchanx[1][1]_midout[99] chanx[1][1]_midout[99] 0 
+  0
******* IO_TYPE loads *******
******* END loads *******

******* IO_TYPE loads *******
******* END loads *******

******* IO_TYPE loads *******
******* END loads *******

******* IO_TYPE loads *******
******* END loads *******

******* IO_TYPE loads *******
******* END loads *******

******* IO_TYPE loads *******
******* END loads *******

******* IO_TYPE loads *******
******* END loads *******

******* IO_TYPE loads *******
******* END loads *******

******* Normal TYPE loads *******
Xload_inv[0]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[0] gvdd_load 0 inv size=1
Xload_inv[1]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[1] gvdd_load 0 inv size=1
Xload_inv[2]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[2] gvdd_load 0 inv size=1
Xload_inv[3]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[3] gvdd_load 0 inv size=1
Xload_inv[4]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[4] gvdd_load 0 inv size=1
Xload_inv[5]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[5] gvdd_load 0 inv size=1
Xload_inv[6]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[6] gvdd_load 0 inv size=1
Xload_inv[7]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[7] gvdd_load 0 inv size=1
Xload_inv[8]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[8] gvdd_load 0 inv size=1
Xload_inv[9]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[9] gvdd_load 0 inv size=1
Xload_inv[10]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[10] gvdd_load 0 inv size=1
Xload_inv[11]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[11] gvdd_load 0 inv size=1
Xload_inv[12]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[12] gvdd_load 0 inv size=1
Xload_inv[13]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[13] gvdd_load 0 inv size=1
Xload_inv[14]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[14] gvdd_load 0 inv size=1
Xload_inv[15]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[15] gvdd_load 0 inv size=1
Xload_inv[16]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[16] gvdd_load 0 inv size=1
Xload_inv[17]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[17] gvdd_load 0 inv size=1
Xload_inv[18]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[18] gvdd_load 0 inv size=1
Xload_inv[19]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[19] gvdd_load 0 inv size=1
Xload_inv[20]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[20] gvdd_load 0 inv size=1
Xload_inv[21]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[21] gvdd_load 0 inv size=1
Xload_inv[22]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[22] gvdd_load 0 inv size=1
Xload_inv[23]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[23] gvdd_load 0 inv size=1
Xload_inv[24]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[24] gvdd_load 0 inv size=1
Xload_inv[25]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[25] gvdd_load 0 inv size=1
Xload_inv[26]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[26] gvdd_load 0 inv size=1
Xload_inv[27]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[27] gvdd_load 0 inv size=1
Xload_inv[28]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[28] gvdd_load 0 inv size=1
Xload_inv[29]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[29] gvdd_load 0 inv size=1
Xload_inv[30]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[30] gvdd_load 0 inv size=1
Xload_inv[31]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[31] gvdd_load 0 inv size=1
Xload_inv[32]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[32] gvdd_load 0 inv size=1
Xload_inv[33]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[33] gvdd_load 0 inv size=1
Xload_inv[34]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[34] gvdd_load 0 inv size=1
Xload_inv[35]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[35] gvdd_load 0 inv size=1
Xload_inv[36]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[36] gvdd_load 0 inv size=1
Xload_inv[37]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[37] gvdd_load 0 inv size=1
Xload_inv[38]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[38] gvdd_load 0 inv size=1
Xload_inv[39]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[39] gvdd_load 0 inv size=1
Xload_inv[40]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[40] gvdd_load 0 inv size=1
Xload_inv[41]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[41] gvdd_load 0 inv size=1
Xload_inv[42]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[42] gvdd_load 0 inv size=1
Xload_inv[43]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[43] gvdd_load 0 inv size=1
Xload_inv[44]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[44] gvdd_load 0 inv size=1
Xload_inv[45]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[45] gvdd_load 0 inv size=1
Xload_inv[46]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[46] gvdd_load 0 inv size=1
Xload_inv[47]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[47] gvdd_load 0 inv size=1
Xload_inv[48]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[48] gvdd_load 0 inv size=1
Xload_inv[49]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[49] gvdd_load 0 inv size=1
Xload_inv[50]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[50] gvdd_load 0 inv size=1
Xload_inv[51]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[51] gvdd_load 0 inv size=1
Xload_inv[52]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[52] gvdd_load 0 inv size=1
Xload_inv[53]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[53] gvdd_load 0 inv size=1
Xload_inv[54]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[54] gvdd_load 0 inv size=1
Xload_inv[55]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[55] gvdd_load 0 inv size=1
Xload_inv[56]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[56] gvdd_load 0 inv size=1
Xload_inv[57]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[57] gvdd_load 0 inv size=1
Xload_inv[58]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[58] gvdd_load 0 inv size=1
Xload_inv[59]_no0 grid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0]_out[59] gvdd_load 0 inv size=1
******* END loads *******

******* Normal TYPE loads *******
Xload_inv[60]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[0] gvdd_load 0 inv size=1
Xload_inv[61]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[1] gvdd_load 0 inv size=1
Xload_inv[62]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[2] gvdd_load 0 inv size=1
Xload_inv[63]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[3] gvdd_load 0 inv size=1
Xload_inv[64]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[4] gvdd_load 0 inv size=1
Xload_inv[65]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[5] gvdd_load 0 inv size=1
Xload_inv[66]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[6] gvdd_load 0 inv size=1
Xload_inv[67]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[7] gvdd_load 0 inv size=1
Xload_inv[68]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[8] gvdd_load 0 inv size=1
Xload_inv[69]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[9] gvdd_load 0 inv size=1
Xload_inv[70]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[10] gvdd_load 0 inv size=1
Xload_inv[71]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[11] gvdd_load 0 inv size=1
Xload_inv[72]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[12] gvdd_load 0 inv size=1
Xload_inv[73]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[13] gvdd_load 0 inv size=1
Xload_inv[74]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[14] gvdd_load 0 inv size=1
Xload_inv[75]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[15] gvdd_load 0 inv size=1
Xload_inv[76]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[16] gvdd_load 0 inv size=1
Xload_inv[77]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[17] gvdd_load 0 inv size=1
Xload_inv[78]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[18] gvdd_load 0 inv size=1
Xload_inv[79]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[19] gvdd_load 0 inv size=1
Xload_inv[80]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[20] gvdd_load 0 inv size=1
Xload_inv[81]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[21] gvdd_load 0 inv size=1
Xload_inv[82]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[22] gvdd_load 0 inv size=1
Xload_inv[83]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[23] gvdd_load 0 inv size=1
Xload_inv[84]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[24] gvdd_load 0 inv size=1
Xload_inv[85]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[25] gvdd_load 0 inv size=1
Xload_inv[86]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[26] gvdd_load 0 inv size=1
Xload_inv[87]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[27] gvdd_load 0 inv size=1
Xload_inv[88]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[28] gvdd_load 0 inv size=1
Xload_inv[89]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[29] gvdd_load 0 inv size=1
Xload_inv[90]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[30] gvdd_load 0 inv size=1
Xload_inv[91]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[31] gvdd_load 0 inv size=1
Xload_inv[92]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[32] gvdd_load 0 inv size=1
Xload_inv[93]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[33] gvdd_load 0 inv size=1
Xload_inv[94]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[34] gvdd_load 0 inv size=1
Xload_inv[95]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[35] gvdd_load 0 inv size=1
Xload_inv[96]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[36] gvdd_load 0 inv size=1
Xload_inv[97]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[37] gvdd_load 0 inv size=1
Xload_inv[98]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[38] gvdd_load 0 inv size=1
Xload_inv[99]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[39] gvdd_load 0 inv size=1
Xload_inv[100]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[40] gvdd_load 0 inv size=1
Xload_inv[101]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[41] gvdd_load 0 inv size=1
Xload_inv[102]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[42] gvdd_load 0 inv size=1
Xload_inv[103]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[43] gvdd_load 0 inv size=1
Xload_inv[104]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[44] gvdd_load 0 inv size=1
Xload_inv[105]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[45] gvdd_load 0 inv size=1
Xload_inv[106]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[46] gvdd_load 0 inv size=1
Xload_inv[107]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[47] gvdd_load 0 inv size=1
Xload_inv[108]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[48] gvdd_load 0 inv size=1
Xload_inv[109]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[49] gvdd_load 0 inv size=1
Xload_inv[110]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[50] gvdd_load 0 inv size=1
Xload_inv[111]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[51] gvdd_load 0 inv size=1
Xload_inv[112]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[52] gvdd_load 0 inv size=1
Xload_inv[113]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[53] gvdd_load 0 inv size=1
Xload_inv[114]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[54] gvdd_load 0 inv size=1
Xload_inv[115]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[55] gvdd_load 0 inv size=1
Xload_inv[116]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[56] gvdd_load 0 inv size=1
Xload_inv[117]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[57] gvdd_load 0 inv size=1
Xload_inv[118]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[58] gvdd_load 0 inv size=1
Xload_inv[119]_no0 grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[59] gvdd_load 0 inv size=1
******* END loads *******

******* Normal TYPE loads *******
Xload_inv[120]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[0] gvdd_load 0 inv size=1
Xload_inv[121]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[1] gvdd_load 0 inv size=1
Xload_inv[122]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[2] gvdd_load 0 inv size=1
Xload_inv[123]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[3] gvdd_load 0 inv size=1
Xload_inv[124]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[4] gvdd_load 0 inv size=1
Xload_inv[125]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[5] gvdd_load 0 inv size=1
Xload_inv[126]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[6] gvdd_load 0 inv size=1
Xload_inv[127]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[7] gvdd_load 0 inv size=1
Xload_inv[128]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[8] gvdd_load 0 inv size=1
Xload_inv[129]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[9] gvdd_load 0 inv size=1
Xload_inv[130]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[10] gvdd_load 0 inv size=1
Xload_inv[131]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[11] gvdd_load 0 inv size=1
Xload_inv[132]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[12] gvdd_load 0 inv size=1
Xload_inv[133]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[13] gvdd_load 0 inv size=1
Xload_inv[134]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[14] gvdd_load 0 inv size=1
Xload_inv[135]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[15] gvdd_load 0 inv size=1
Xload_inv[136]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[16] gvdd_load 0 inv size=1
Xload_inv[137]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[17] gvdd_load 0 inv size=1
Xload_inv[138]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[18] gvdd_load 0 inv size=1
Xload_inv[139]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[19] gvdd_load 0 inv size=1
Xload_inv[140]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[20] gvdd_load 0 inv size=1
Xload_inv[141]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[21] gvdd_load 0 inv size=1
Xload_inv[142]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[22] gvdd_load 0 inv size=1
Xload_inv[143]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[23] gvdd_load 0 inv size=1
Xload_inv[144]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[24] gvdd_load 0 inv size=1
Xload_inv[145]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[25] gvdd_load 0 inv size=1
Xload_inv[146]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[26] gvdd_load 0 inv size=1
Xload_inv[147]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[27] gvdd_load 0 inv size=1
Xload_inv[148]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[28] gvdd_load 0 inv size=1
Xload_inv[149]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[29] gvdd_load 0 inv size=1
Xload_inv[150]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[30] gvdd_load 0 inv size=1
Xload_inv[151]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[31] gvdd_load 0 inv size=1
Xload_inv[152]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[32] gvdd_load 0 inv size=1
Xload_inv[153]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[33] gvdd_load 0 inv size=1
Xload_inv[154]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[34] gvdd_load 0 inv size=1
Xload_inv[155]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[35] gvdd_load 0 inv size=1
Xload_inv[156]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[36] gvdd_load 0 inv size=1
Xload_inv[157]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[37] gvdd_load 0 inv size=1
Xload_inv[158]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[38] gvdd_load 0 inv size=1
Xload_inv[159]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[39] gvdd_load 0 inv size=1
Xload_inv[160]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[40] gvdd_load 0 inv size=1
Xload_inv[161]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[41] gvdd_load 0 inv size=1
Xload_inv[162]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[42] gvdd_load 0 inv size=1
Xload_inv[163]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[43] gvdd_load 0 inv size=1
Xload_inv[164]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[44] gvdd_load 0 inv size=1
Xload_inv[165]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[45] gvdd_load 0 inv size=1
Xload_inv[166]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[46] gvdd_load 0 inv size=1
Xload_inv[167]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[47] gvdd_load 0 inv size=1
Xload_inv[168]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[48] gvdd_load 0 inv size=1
Xload_inv[169]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[49] gvdd_load 0 inv size=1
Xload_inv[170]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[50] gvdd_load 0 inv size=1
Xload_inv[171]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[51] gvdd_load 0 inv size=1
Xload_inv[172]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[52] gvdd_load 0 inv size=1
Xload_inv[173]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[53] gvdd_load 0 inv size=1
Xload_inv[174]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[54] gvdd_load 0 inv size=1
Xload_inv[175]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[55] gvdd_load 0 inv size=1
Xload_inv[176]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[56] gvdd_load 0 inv size=1
Xload_inv[177]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[57] gvdd_load 0 inv size=1
Xload_inv[178]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[58] gvdd_load 0 inv size=1
Xload_inv[179]_no0 grid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8]_out[59] gvdd_load 0 inv size=1
******* END loads *******

******* Normal TYPE loads *******
Xload_inv[180]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[0] gvdd_load 0 inv size=1
Xload_inv[181]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[1] gvdd_load 0 inv size=1
Xload_inv[182]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[2] gvdd_load 0 inv size=1
Xload_inv[183]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[3] gvdd_load 0 inv size=1
Xload_inv[184]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[4] gvdd_load 0 inv size=1
Xload_inv[185]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[5] gvdd_load 0 inv size=1
Xload_inv[186]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[6] gvdd_load 0 inv size=1
Xload_inv[187]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[7] gvdd_load 0 inv size=1
Xload_inv[188]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[8] gvdd_load 0 inv size=1
Xload_inv[189]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[9] gvdd_load 0 inv size=1
Xload_inv[190]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[10] gvdd_load 0 inv size=1
Xload_inv[191]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[11] gvdd_load 0 inv size=1
Xload_inv[192]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[12] gvdd_load 0 inv size=1
Xload_inv[193]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[13] gvdd_load 0 inv size=1
Xload_inv[194]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[14] gvdd_load 0 inv size=1
Xload_inv[195]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[15] gvdd_load 0 inv size=1
Xload_inv[196]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[16] gvdd_load 0 inv size=1
Xload_inv[197]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[17] gvdd_load 0 inv size=1
Xload_inv[198]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[18] gvdd_load 0 inv size=1
Xload_inv[199]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[19] gvdd_load 0 inv size=1
Xload_inv[200]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[20] gvdd_load 0 inv size=1
Xload_inv[201]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[21] gvdd_load 0 inv size=1
Xload_inv[202]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[22] gvdd_load 0 inv size=1
Xload_inv[203]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[23] gvdd_load 0 inv size=1
Xload_inv[204]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[24] gvdd_load 0 inv size=1
Xload_inv[205]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[25] gvdd_load 0 inv size=1
Xload_inv[206]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[26] gvdd_load 0 inv size=1
Xload_inv[207]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[27] gvdd_load 0 inv size=1
Xload_inv[208]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[28] gvdd_load 0 inv size=1
Xload_inv[209]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[29] gvdd_load 0 inv size=1
Xload_inv[210]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[30] gvdd_load 0 inv size=1
Xload_inv[211]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[31] gvdd_load 0 inv size=1
Xload_inv[212]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[32] gvdd_load 0 inv size=1
Xload_inv[213]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[33] gvdd_load 0 inv size=1
Xload_inv[214]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[34] gvdd_load 0 inv size=1
Xload_inv[215]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[35] gvdd_load 0 inv size=1
Xload_inv[216]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[36] gvdd_load 0 inv size=1
Xload_inv[217]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[37] gvdd_load 0 inv size=1
Xload_inv[218]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[38] gvdd_load 0 inv size=1
Xload_inv[219]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[39] gvdd_load 0 inv size=1
Xload_inv[220]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[40] gvdd_load 0 inv size=1
Xload_inv[221]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[41] gvdd_load 0 inv size=1
Xload_inv[222]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[42] gvdd_load 0 inv size=1
Xload_inv[223]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[43] gvdd_load 0 inv size=1
Xload_inv[224]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[44] gvdd_load 0 inv size=1
Xload_inv[225]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[45] gvdd_load 0 inv size=1
Xload_inv[226]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[46] gvdd_load 0 inv size=1
Xload_inv[227]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[47] gvdd_load 0 inv size=1
Xload_inv[228]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[48] gvdd_load 0 inv size=1
Xload_inv[229]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[49] gvdd_load 0 inv size=1
Xload_inv[230]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[50] gvdd_load 0 inv size=1
Xload_inv[231]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[51] gvdd_load 0 inv size=1
Xload_inv[232]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[52] gvdd_load 0 inv size=1
Xload_inv[233]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[53] gvdd_load 0 inv size=1
Xload_inv[234]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[54] gvdd_load 0 inv size=1
Xload_inv[235]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[55] gvdd_load 0 inv size=1
Xload_inv[236]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[56] gvdd_load 0 inv size=1
Xload_inv[237]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[57] gvdd_load 0 inv size=1
Xload_inv[238]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[58] gvdd_load 0 inv size=1
Xload_inv[239]_no0 grid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12]_out[59] gvdd_load 0 inv size=1
******* END loads *******

******* Normal TYPE loads *******
Xload_inv[240]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[0] gvdd_load 0 inv size=1
Xload_inv[241]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[1] gvdd_load 0 inv size=1
Xload_inv[242]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[2] gvdd_load 0 inv size=1
Xload_inv[243]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[3] gvdd_load 0 inv size=1
Xload_inv[244]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[4] gvdd_load 0 inv size=1
Xload_inv[245]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[5] gvdd_load 0 inv size=1
Xload_inv[246]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[6] gvdd_load 0 inv size=1
Xload_inv[247]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[7] gvdd_load 0 inv size=1
Xload_inv[248]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[8] gvdd_load 0 inv size=1
Xload_inv[249]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[9] gvdd_load 0 inv size=1
Xload_inv[250]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[10] gvdd_load 0 inv size=1
Xload_inv[251]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[11] gvdd_load 0 inv size=1
Xload_inv[252]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[12] gvdd_load 0 inv size=1
Xload_inv[253]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[13] gvdd_load 0 inv size=1
Xload_inv[254]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[14] gvdd_load 0 inv size=1
Xload_inv[255]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[15] gvdd_load 0 inv size=1
Xload_inv[256]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[16] gvdd_load 0 inv size=1
Xload_inv[257]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[17] gvdd_load 0 inv size=1
Xload_inv[258]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[18] gvdd_load 0 inv size=1
Xload_inv[259]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[19] gvdd_load 0 inv size=1
Xload_inv[260]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[20] gvdd_load 0 inv size=1
Xload_inv[261]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[21] gvdd_load 0 inv size=1
Xload_inv[262]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[22] gvdd_load 0 inv size=1
Xload_inv[263]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[23] gvdd_load 0 inv size=1
Xload_inv[264]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[24] gvdd_load 0 inv size=1
Xload_inv[265]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[25] gvdd_load 0 inv size=1
Xload_inv[266]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[26] gvdd_load 0 inv size=1
Xload_inv[267]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[27] gvdd_load 0 inv size=1
Xload_inv[268]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[28] gvdd_load 0 inv size=1
Xload_inv[269]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[29] gvdd_load 0 inv size=1
Xload_inv[270]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[30] gvdd_load 0 inv size=1
Xload_inv[271]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[31] gvdd_load 0 inv size=1
Xload_inv[272]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[32] gvdd_load 0 inv size=1
Xload_inv[273]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[33] gvdd_load 0 inv size=1
Xload_inv[274]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[34] gvdd_load 0 inv size=1
Xload_inv[275]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[35] gvdd_load 0 inv size=1
Xload_inv[276]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[36] gvdd_load 0 inv size=1
Xload_inv[277]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[37] gvdd_load 0 inv size=1
Xload_inv[278]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[38] gvdd_load 0 inv size=1
Xload_inv[279]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[39] gvdd_load 0 inv size=1
Xload_inv[280]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[40] gvdd_load 0 inv size=1
Xload_inv[281]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[41] gvdd_load 0 inv size=1
Xload_inv[282]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[42] gvdd_load 0 inv size=1
Xload_inv[283]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[43] gvdd_load 0 inv size=1
Xload_inv[284]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[44] gvdd_load 0 inv size=1
Xload_inv[285]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[45] gvdd_load 0 inv size=1
Xload_inv[286]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[46] gvdd_load 0 inv size=1
Xload_inv[287]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[47] gvdd_load 0 inv size=1
Xload_inv[288]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[48] gvdd_load 0 inv size=1
Xload_inv[289]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[49] gvdd_load 0 inv size=1
Xload_inv[290]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[50] gvdd_load 0 inv size=1
Xload_inv[291]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[51] gvdd_load 0 inv size=1
Xload_inv[292]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[52] gvdd_load 0 inv size=1
Xload_inv[293]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[53] gvdd_load 0 inv size=1
Xload_inv[294]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[54] gvdd_load 0 inv size=1
Xload_inv[295]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[55] gvdd_load 0 inv size=1
Xload_inv[296]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[56] gvdd_load 0 inv size=1
Xload_inv[297]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[57] gvdd_load 0 inv size=1
Xload_inv[298]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[58] gvdd_load 0 inv size=1
Xload_inv[299]_no0 grid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16]_out[59] gvdd_load 0 inv size=1
******* END loads *******

******* Normal TYPE loads *******
Xload_inv[300]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[0] gvdd_load 0 inv size=1
Xload_inv[301]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[1] gvdd_load 0 inv size=1
Xload_inv[302]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[2] gvdd_load 0 inv size=1
Xload_inv[303]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[3] gvdd_load 0 inv size=1
Xload_inv[304]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[4] gvdd_load 0 inv size=1
Xload_inv[305]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[5] gvdd_load 0 inv size=1
Xload_inv[306]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[6] gvdd_load 0 inv size=1
Xload_inv[307]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[7] gvdd_load 0 inv size=1
Xload_inv[308]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[8] gvdd_load 0 inv size=1
Xload_inv[309]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[9] gvdd_load 0 inv size=1
Xload_inv[310]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[10] gvdd_load 0 inv size=1
Xload_inv[311]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[11] gvdd_load 0 inv size=1
Xload_inv[312]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[12] gvdd_load 0 inv size=1
Xload_inv[313]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[13] gvdd_load 0 inv size=1
Xload_inv[314]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[14] gvdd_load 0 inv size=1
Xload_inv[315]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[15] gvdd_load 0 inv size=1
Xload_inv[316]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[16] gvdd_load 0 inv size=1
Xload_inv[317]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[17] gvdd_load 0 inv size=1
Xload_inv[318]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[18] gvdd_load 0 inv size=1
Xload_inv[319]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[19] gvdd_load 0 inv size=1
Xload_inv[320]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[20] gvdd_load 0 inv size=1
Xload_inv[321]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[21] gvdd_load 0 inv size=1
Xload_inv[322]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[22] gvdd_load 0 inv size=1
Xload_inv[323]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[23] gvdd_load 0 inv size=1
Xload_inv[324]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[24] gvdd_load 0 inv size=1
Xload_inv[325]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[25] gvdd_load 0 inv size=1
Xload_inv[326]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[26] gvdd_load 0 inv size=1
Xload_inv[327]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[27] gvdd_load 0 inv size=1
Xload_inv[328]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[28] gvdd_load 0 inv size=1
Xload_inv[329]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[29] gvdd_load 0 inv size=1
Xload_inv[330]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[30] gvdd_load 0 inv size=1
Xload_inv[331]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[31] gvdd_load 0 inv size=1
Xload_inv[332]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[32] gvdd_load 0 inv size=1
Xload_inv[333]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[33] gvdd_load 0 inv size=1
Xload_inv[334]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[34] gvdd_load 0 inv size=1
Xload_inv[335]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[35] gvdd_load 0 inv size=1
Xload_inv[336]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[36] gvdd_load 0 inv size=1
Xload_inv[337]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[37] gvdd_load 0 inv size=1
Xload_inv[338]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[38] gvdd_load 0 inv size=1
Xload_inv[339]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[39] gvdd_load 0 inv size=1
Xload_inv[340]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[40] gvdd_load 0 inv size=1
Xload_inv[341]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[41] gvdd_load 0 inv size=1
Xload_inv[342]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[42] gvdd_load 0 inv size=1
Xload_inv[343]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[43] gvdd_load 0 inv size=1
Xload_inv[344]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[44] gvdd_load 0 inv size=1
Xload_inv[345]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[45] gvdd_load 0 inv size=1
Xload_inv[346]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[46] gvdd_load 0 inv size=1
Xload_inv[347]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[47] gvdd_load 0 inv size=1
Xload_inv[348]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[48] gvdd_load 0 inv size=1
Xload_inv[349]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[49] gvdd_load 0 inv size=1
Xload_inv[350]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[50] gvdd_load 0 inv size=1
Xload_inv[351]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[51] gvdd_load 0 inv size=1
Xload_inv[352]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[52] gvdd_load 0 inv size=1
Xload_inv[353]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[53] gvdd_load 0 inv size=1
Xload_inv[354]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[54] gvdd_load 0 inv size=1
Xload_inv[355]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[55] gvdd_load 0 inv size=1
Xload_inv[356]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[56] gvdd_load 0 inv size=1
Xload_inv[357]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[57] gvdd_load 0 inv size=1
Xload_inv[358]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[58] gvdd_load 0 inv size=1
Xload_inv[359]_no0 grid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20]_out[59] gvdd_load 0 inv size=1
******* END loads *******

******* Normal TYPE loads *******
Xload_inv[360]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[0] gvdd_load 0 inv size=1
Xload_inv[361]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[1] gvdd_load 0 inv size=1
Xload_inv[362]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[2] gvdd_load 0 inv size=1
Xload_inv[363]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[3] gvdd_load 0 inv size=1
Xload_inv[364]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[4] gvdd_load 0 inv size=1
Xload_inv[365]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[5] gvdd_load 0 inv size=1
Xload_inv[366]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[6] gvdd_load 0 inv size=1
Xload_inv[367]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[7] gvdd_load 0 inv size=1
Xload_inv[368]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[8] gvdd_load 0 inv size=1
Xload_inv[369]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[9] gvdd_load 0 inv size=1
Xload_inv[370]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[10] gvdd_load 0 inv size=1
Xload_inv[371]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[11] gvdd_load 0 inv size=1
Xload_inv[372]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[12] gvdd_load 0 inv size=1
Xload_inv[373]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[13] gvdd_load 0 inv size=1
Xload_inv[374]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[14] gvdd_load 0 inv size=1
Xload_inv[375]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[15] gvdd_load 0 inv size=1
Xload_inv[376]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[16] gvdd_load 0 inv size=1
Xload_inv[377]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[17] gvdd_load 0 inv size=1
Xload_inv[378]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[18] gvdd_load 0 inv size=1
Xload_inv[379]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[19] gvdd_load 0 inv size=1
Xload_inv[380]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[20] gvdd_load 0 inv size=1
Xload_inv[381]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[21] gvdd_load 0 inv size=1
Xload_inv[382]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[22] gvdd_load 0 inv size=1
Xload_inv[383]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[23] gvdd_load 0 inv size=1
Xload_inv[384]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[24] gvdd_load 0 inv size=1
Xload_inv[385]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[25] gvdd_load 0 inv size=1
Xload_inv[386]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[26] gvdd_load 0 inv size=1
Xload_inv[387]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[27] gvdd_load 0 inv size=1
Xload_inv[388]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[28] gvdd_load 0 inv size=1
Xload_inv[389]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[29] gvdd_load 0 inv size=1
Xload_inv[390]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[30] gvdd_load 0 inv size=1
Xload_inv[391]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[31] gvdd_load 0 inv size=1
Xload_inv[392]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[32] gvdd_load 0 inv size=1
Xload_inv[393]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[33] gvdd_load 0 inv size=1
Xload_inv[394]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[34] gvdd_load 0 inv size=1
Xload_inv[395]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[35] gvdd_load 0 inv size=1
Xload_inv[396]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[36] gvdd_load 0 inv size=1
Xload_inv[397]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[37] gvdd_load 0 inv size=1
Xload_inv[398]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[38] gvdd_load 0 inv size=1
Xload_inv[399]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[39] gvdd_load 0 inv size=1
Xload_inv[400]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[40] gvdd_load 0 inv size=1
Xload_inv[401]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[41] gvdd_load 0 inv size=1
Xload_inv[402]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[42] gvdd_load 0 inv size=1
Xload_inv[403]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[43] gvdd_load 0 inv size=1
Xload_inv[404]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[44] gvdd_load 0 inv size=1
Xload_inv[405]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[45] gvdd_load 0 inv size=1
Xload_inv[406]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[46] gvdd_load 0 inv size=1
Xload_inv[407]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[47] gvdd_load 0 inv size=1
Xload_inv[408]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[48] gvdd_load 0 inv size=1
Xload_inv[409]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[49] gvdd_load 0 inv size=1
Xload_inv[410]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[50] gvdd_load 0 inv size=1
Xload_inv[411]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[51] gvdd_load 0 inv size=1
Xload_inv[412]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[52] gvdd_load 0 inv size=1
Xload_inv[413]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[53] gvdd_load 0 inv size=1
Xload_inv[414]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[54] gvdd_load 0 inv size=1
Xload_inv[415]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[55] gvdd_load 0 inv size=1
Xload_inv[416]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[56] gvdd_load 0 inv size=1
Xload_inv[417]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[57] gvdd_load 0 inv size=1
Xload_inv[418]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[58] gvdd_load 0 inv size=1
Xload_inv[419]_no0 grid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24]_out[59] gvdd_load 0 inv size=1
******* END loads *******

******* Normal TYPE loads *******
Xload_inv[420]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[0] gvdd_load 0 inv size=1
Xload_inv[421]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[1] gvdd_load 0 inv size=1
Xload_inv[422]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[2] gvdd_load 0 inv size=1
Xload_inv[423]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[3] gvdd_load 0 inv size=1
Xload_inv[424]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[4] gvdd_load 0 inv size=1
Xload_inv[425]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[5] gvdd_load 0 inv size=1
Xload_inv[426]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[6] gvdd_load 0 inv size=1
Xload_inv[427]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[7] gvdd_load 0 inv size=1
Xload_inv[428]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[8] gvdd_load 0 inv size=1
Xload_inv[429]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[9] gvdd_load 0 inv size=1
Xload_inv[430]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[10] gvdd_load 0 inv size=1
Xload_inv[431]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[11] gvdd_load 0 inv size=1
Xload_inv[432]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[12] gvdd_load 0 inv size=1
Xload_inv[433]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[13] gvdd_load 0 inv size=1
Xload_inv[434]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[14] gvdd_load 0 inv size=1
Xload_inv[435]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[15] gvdd_load 0 inv size=1
Xload_inv[436]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[16] gvdd_load 0 inv size=1
Xload_inv[437]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[17] gvdd_load 0 inv size=1
Xload_inv[438]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[18] gvdd_load 0 inv size=1
Xload_inv[439]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[19] gvdd_load 0 inv size=1
Xload_inv[440]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[20] gvdd_load 0 inv size=1
Xload_inv[441]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[21] gvdd_load 0 inv size=1
Xload_inv[442]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[22] gvdd_load 0 inv size=1
Xload_inv[443]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[23] gvdd_load 0 inv size=1
Xload_inv[444]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[24] gvdd_load 0 inv size=1
Xload_inv[445]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[25] gvdd_load 0 inv size=1
Xload_inv[446]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[26] gvdd_load 0 inv size=1
Xload_inv[447]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[27] gvdd_load 0 inv size=1
Xload_inv[448]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[28] gvdd_load 0 inv size=1
Xload_inv[449]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[29] gvdd_load 0 inv size=1
Xload_inv[450]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[30] gvdd_load 0 inv size=1
Xload_inv[451]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[31] gvdd_load 0 inv size=1
Xload_inv[452]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[32] gvdd_load 0 inv size=1
Xload_inv[453]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[33] gvdd_load 0 inv size=1
Xload_inv[454]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[34] gvdd_load 0 inv size=1
Xload_inv[455]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[35] gvdd_load 0 inv size=1
Xload_inv[456]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[36] gvdd_load 0 inv size=1
Xload_inv[457]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[37] gvdd_load 0 inv size=1
Xload_inv[458]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[38] gvdd_load 0 inv size=1
Xload_inv[459]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[39] gvdd_load 0 inv size=1
Xload_inv[460]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[40] gvdd_load 0 inv size=1
Xload_inv[461]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[41] gvdd_load 0 inv size=1
Xload_inv[462]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[42] gvdd_load 0 inv size=1
Xload_inv[463]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[43] gvdd_load 0 inv size=1
Xload_inv[464]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[44] gvdd_load 0 inv size=1
Xload_inv[465]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[45] gvdd_load 0 inv size=1
Xload_inv[466]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[46] gvdd_load 0 inv size=1
Xload_inv[467]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[47] gvdd_load 0 inv size=1
Xload_inv[468]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[48] gvdd_load 0 inv size=1
Xload_inv[469]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[49] gvdd_load 0 inv size=1
Xload_inv[470]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[50] gvdd_load 0 inv size=1
Xload_inv[471]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[51] gvdd_load 0 inv size=1
Xload_inv[472]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[52] gvdd_load 0 inv size=1
Xload_inv[473]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[53] gvdd_load 0 inv size=1
Xload_inv[474]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[54] gvdd_load 0 inv size=1
Xload_inv[475]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[55] gvdd_load 0 inv size=1
Xload_inv[476]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[56] gvdd_load 0 inv size=1
Xload_inv[477]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[57] gvdd_load 0 inv size=1
Xload_inv[478]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[58] gvdd_load 0 inv size=1
Xload_inv[479]_no0 grid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28]_out[59] gvdd_load 0 inv size=1
******* END loads *******

******* Normal TYPE loads *******
Xload_inv[480]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[0] gvdd_load 0 inv size=1
Xload_inv[481]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[1] gvdd_load 0 inv size=1
Xload_inv[482]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[2] gvdd_load 0 inv size=1
Xload_inv[483]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[3] gvdd_load 0 inv size=1
Xload_inv[484]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[4] gvdd_load 0 inv size=1
Xload_inv[485]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[5] gvdd_load 0 inv size=1
Xload_inv[486]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[6] gvdd_load 0 inv size=1
Xload_inv[487]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[7] gvdd_load 0 inv size=1
Xload_inv[488]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[8] gvdd_load 0 inv size=1
Xload_inv[489]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[9] gvdd_load 0 inv size=1
Xload_inv[490]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[10] gvdd_load 0 inv size=1
Xload_inv[491]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[11] gvdd_load 0 inv size=1
Xload_inv[492]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[12] gvdd_load 0 inv size=1
Xload_inv[493]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[13] gvdd_load 0 inv size=1
Xload_inv[494]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[14] gvdd_load 0 inv size=1
Xload_inv[495]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[15] gvdd_load 0 inv size=1
Xload_inv[496]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[16] gvdd_load 0 inv size=1
Xload_inv[497]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[17] gvdd_load 0 inv size=1
Xload_inv[498]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[18] gvdd_load 0 inv size=1
Xload_inv[499]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[19] gvdd_load 0 inv size=1
Xload_inv[500]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[20] gvdd_load 0 inv size=1
Xload_inv[501]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[21] gvdd_load 0 inv size=1
Xload_inv[502]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[22] gvdd_load 0 inv size=1
Xload_inv[503]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[23] gvdd_load 0 inv size=1
Xload_inv[504]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[24] gvdd_load 0 inv size=1
Xload_inv[505]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[25] gvdd_load 0 inv size=1
Xload_inv[506]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[26] gvdd_load 0 inv size=1
Xload_inv[507]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[27] gvdd_load 0 inv size=1
Xload_inv[508]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[28] gvdd_load 0 inv size=1
Xload_inv[509]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[29] gvdd_load 0 inv size=1
Xload_inv[510]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[30] gvdd_load 0 inv size=1
Xload_inv[511]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[31] gvdd_load 0 inv size=1
Xload_inv[512]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[32] gvdd_load 0 inv size=1
Xload_inv[513]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[33] gvdd_load 0 inv size=1
Xload_inv[514]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[34] gvdd_load 0 inv size=1
Xload_inv[515]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[35] gvdd_load 0 inv size=1
Xload_inv[516]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[36] gvdd_load 0 inv size=1
Xload_inv[517]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[37] gvdd_load 0 inv size=1
Xload_inv[518]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[38] gvdd_load 0 inv size=1
Xload_inv[519]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[39] gvdd_load 0 inv size=1
Xload_inv[520]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[40] gvdd_load 0 inv size=1
Xload_inv[521]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[41] gvdd_load 0 inv size=1
Xload_inv[522]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[42] gvdd_load 0 inv size=1
Xload_inv[523]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[43] gvdd_load 0 inv size=1
Xload_inv[524]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[44] gvdd_load 0 inv size=1
Xload_inv[525]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[45] gvdd_load 0 inv size=1
Xload_inv[526]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[46] gvdd_load 0 inv size=1
Xload_inv[527]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[47] gvdd_load 0 inv size=1
Xload_inv[528]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[48] gvdd_load 0 inv size=1
Xload_inv[529]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[49] gvdd_load 0 inv size=1
Xload_inv[530]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[50] gvdd_load 0 inv size=1
Xload_inv[531]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[51] gvdd_load 0 inv size=1
Xload_inv[532]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[52] gvdd_load 0 inv size=1
Xload_inv[533]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[53] gvdd_load 0 inv size=1
Xload_inv[534]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[54] gvdd_load 0 inv size=1
Xload_inv[535]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[55] gvdd_load 0 inv size=1
Xload_inv[536]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[56] gvdd_load 0 inv size=1
Xload_inv[537]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[57] gvdd_load 0 inv size=1
Xload_inv[538]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[58] gvdd_load 0 inv size=1
Xload_inv[539]_no0 grid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32]_out[59] gvdd_load 0 inv size=1
******* END loads *******

******* Normal TYPE loads *******
Xload_inv[540]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[0] gvdd_load 0 inv size=1
Xload_inv[541]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[1] gvdd_load 0 inv size=1
Xload_inv[542]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[2] gvdd_load 0 inv size=1
Xload_inv[543]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[3] gvdd_load 0 inv size=1
Xload_inv[544]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[4] gvdd_load 0 inv size=1
Xload_inv[545]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[5] gvdd_load 0 inv size=1
Xload_inv[546]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[6] gvdd_load 0 inv size=1
Xload_inv[547]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[7] gvdd_load 0 inv size=1
Xload_inv[548]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[8] gvdd_load 0 inv size=1
Xload_inv[549]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[9] gvdd_load 0 inv size=1
Xload_inv[550]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[10] gvdd_load 0 inv size=1
Xload_inv[551]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[11] gvdd_load 0 inv size=1
Xload_inv[552]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[12] gvdd_load 0 inv size=1
Xload_inv[553]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[13] gvdd_load 0 inv size=1
Xload_inv[554]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[14] gvdd_load 0 inv size=1
Xload_inv[555]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[15] gvdd_load 0 inv size=1
Xload_inv[556]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[16] gvdd_load 0 inv size=1
Xload_inv[557]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[17] gvdd_load 0 inv size=1
Xload_inv[558]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[18] gvdd_load 0 inv size=1
Xload_inv[559]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[19] gvdd_load 0 inv size=1
Xload_inv[560]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[20] gvdd_load 0 inv size=1
Xload_inv[561]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[21] gvdd_load 0 inv size=1
Xload_inv[562]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[22] gvdd_load 0 inv size=1
Xload_inv[563]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[23] gvdd_load 0 inv size=1
Xload_inv[564]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[24] gvdd_load 0 inv size=1
Xload_inv[565]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[25] gvdd_load 0 inv size=1
Xload_inv[566]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[26] gvdd_load 0 inv size=1
Xload_inv[567]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[27] gvdd_load 0 inv size=1
Xload_inv[568]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[28] gvdd_load 0 inv size=1
Xload_inv[569]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[29] gvdd_load 0 inv size=1
Xload_inv[570]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[30] gvdd_load 0 inv size=1
Xload_inv[571]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[31] gvdd_load 0 inv size=1
Xload_inv[572]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[32] gvdd_load 0 inv size=1
Xload_inv[573]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[33] gvdd_load 0 inv size=1
Xload_inv[574]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[34] gvdd_load 0 inv size=1
Xload_inv[575]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[35] gvdd_load 0 inv size=1
Xload_inv[576]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[36] gvdd_load 0 inv size=1
Xload_inv[577]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[37] gvdd_load 0 inv size=1
Xload_inv[578]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[38] gvdd_load 0 inv size=1
Xload_inv[579]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[39] gvdd_load 0 inv size=1
Xload_inv[580]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[40] gvdd_load 0 inv size=1
Xload_inv[581]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[41] gvdd_load 0 inv size=1
Xload_inv[582]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[42] gvdd_load 0 inv size=1
Xload_inv[583]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[43] gvdd_load 0 inv size=1
Xload_inv[584]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[44] gvdd_load 0 inv size=1
Xload_inv[585]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[45] gvdd_load 0 inv size=1
Xload_inv[586]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[46] gvdd_load 0 inv size=1
Xload_inv[587]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[47] gvdd_load 0 inv size=1
Xload_inv[588]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[48] gvdd_load 0 inv size=1
Xload_inv[589]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[49] gvdd_load 0 inv size=1
Xload_inv[590]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[50] gvdd_load 0 inv size=1
Xload_inv[591]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[51] gvdd_load 0 inv size=1
Xload_inv[592]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[52] gvdd_load 0 inv size=1
Xload_inv[593]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[53] gvdd_load 0 inv size=1
Xload_inv[594]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[54] gvdd_load 0 inv size=1
Xload_inv[595]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[55] gvdd_load 0 inv size=1
Xload_inv[596]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[56] gvdd_load 0 inv size=1
Xload_inv[597]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[57] gvdd_load 0 inv size=1
Xload_inv[598]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[58] gvdd_load 0 inv size=1
Xload_inv[599]_no0 grid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36]_out[59] gvdd_load 0 inv size=1
******* END loads *******

***** Voltage supplies *****
***** Voltage supplies *****
Vgvdd_cb[1][1] gvdd_cbx[1][1] 0 vsp
Vgvdd_sram_cbs gvdd_sram_cbs 0 vsp
***** 6 Clock Simulation, accuracy=1e-13 *****
.tran 1e-13  '6*clock_period'
***** Generic Measurements for Circuit Parameters *****
***** Measurements *****
***** Leakage Power Measurement *****
.meas tran leakage_power_cb avg p(Vgvdd_cb[1][1]) from=0 to='clock_period'
.meas tran leakage_power_sram_cb avg p(Vgvdd_sram_cbs) from=0 to='clock_period'
***** Dynamic Power Measurement *****
.meas tran dynamic_power_cb avg p(Vgvdd_cb[1][1]) from='clock_period' to='6*clock_period'
.meas tran energy_per_cycle_cb param='dynamic_power_cb*clock_period'
.meas tran dynamic_power_sram_cb avg p(Vgvdd_sram_cbs) from='clock_period' to='6*clock_period'
.meas tran energy_per_cycle_sram_cb param='dynamic_power_sram_cb*clock_period'
***** Global VDD port *****
Vgvdd gvdd 0 vsp
***** Global GND port *****
Vggnd ggnd 0 0
***** Global Net for reset signal *****
Vgreset greset 0 0
Vgreset_inv greset_inv 0 vsp
***** Global Net for set signal *****
Vgset gset 0 0
Vgset_inv gset_inv 0 vsp
***** Global Net for configuration done signal *****
Vgconfig_done gconfig_done 0 0
Vgconfig_done_inv gconfig_done_inv 0 vsp
***** Global Clock signal *****
***** pulse(vlow vhigh tdelay trise tfall pulse_width period *****
Vgclock gclock 0 pulse(0 vsp 'clock_period'
+                      'clock_slew_pct_rise*clock_period' 'clock_slew_pct_fall*clock_period'
+                      '0.5*(1-clock_slew_pct_rise-clock_slew_pct_fall)*clock_period' 'clock_period')

***** pulse(vlow vhigh tdelay trise tfall pulse_width period *****
Vgclock_inv gclock_inv 0 pulse(0 vsp 'clock_period'
+                              'clock_slew_pct_rise*clock_period' 'clock_slew_pct_fall*clock_period'
+                              '0.5*(1-clock_slew_pct_rise-clock_slew_pct_fall)*clock_period' 'clock_period')
***** Connecting Global ports *****
Rzin[0] zin[0]  ggnd 0
Rshortwireclk[0] clk[0]  gclock 0
RshortwireReset[0] Reset[0]  greset 0
RshortwireSet[0] Set[0]  gset 0
***** End Connecting Global ports *****
***** Global Inputs for SRAMs *****
***** Global Inputs for SRAMs *****
Vsram->in sram->in 0 0
.nodeset V(sram->in) 0
***** Global VDD for SRAMs *****
Vgvdd_sram gvdd_sram 0 vsp
***** Global VDD for load inverters *****
Vgvdd_load gvdd_load 0 vsp
.end
