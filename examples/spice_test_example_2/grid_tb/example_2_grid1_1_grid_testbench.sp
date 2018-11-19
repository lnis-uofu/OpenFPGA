*****************************
*     FPGA SPICE Netlist    *
* Description: FPGA Grid Testbench for Design: example_2 *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:09 2018
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
****** Include subckt netlists: Look-Up Tables (LUTs) *****
.include './spice_test_example_2/subckt/luts.sp'
****** Include subckt netlists: Grid[1][1] *****
.include './spice_test_example_2/subckt/grid_1_1.sp'
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
.global gvdd_local_interc gvdd_io gvdd_hardlogic
.global gvdd_sram_local_routing
.global gvdd_sram_luts
.global gvdd_sram_io
***** Global VDD ports of Look-Up Table *****
.global 
+ gvdd_lut6[0]
+ gvdd_lut6[1]
+ gvdd_lut6[2]
+ gvdd_lut6[3]
+ gvdd_lut6[4]
+ gvdd_lut6[5]
+ gvdd_lut6[6]
+ gvdd_lut6[7]
+ gvdd_lut6[8]
+ gvdd_lut6[9]

***** Global VDD ports of Flip-flop *****
.global 
+ gvdd_dff[0]
+ gvdd_dff[1]
+ gvdd_dff[2]
+ gvdd_dff[3]
+ gvdd_dff[4]
+ gvdd_dff[5]
+ gvdd_dff[6]
+ gvdd_dff[7]
+ gvdd_dff[8]
+ gvdd_dff[9]

***** Global VDD ports of iopad *****

***** Global VDD ports of hard_logic *****

Xgrid[1][1] 
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
+ grid[1][1]_pin[0][0][40] 
+ grid[1][1]_pin[0][0][44] 
+ grid[1][1]_pin[0][0][48] 
+ grid[1][1]_pin[0][1][1] 
+ grid[1][1]_pin[0][1][5] 
+ grid[1][1]_pin[0][1][9] 
+ grid[1][1]_pin[0][1][13] 
+ grid[1][1]_pin[0][1][17] 
+ grid[1][1]_pin[0][1][21] 
+ grid[1][1]_pin[0][1][25] 
+ grid[1][1]_pin[0][1][29] 
+ grid[1][1]_pin[0][1][33] 
+ grid[1][1]_pin[0][1][37] 
+ grid[1][1]_pin[0][1][41] 
+ grid[1][1]_pin[0][1][45] 
+ grid[1][1]_pin[0][1][49] 
+ grid[1][1]_pin[0][2][2] 
+ grid[1][1]_pin[0][2][6] 
+ grid[1][1]_pin[0][2][10] 
+ grid[1][1]_pin[0][2][14] 
+ grid[1][1]_pin[0][2][18] 
+ grid[1][1]_pin[0][2][22] 
+ grid[1][1]_pin[0][2][26] 
+ grid[1][1]_pin[0][2][30] 
+ grid[1][1]_pin[0][2][34] 
+ grid[1][1]_pin[0][2][38] 
+ grid[1][1]_pin[0][2][42] 
+ grid[1][1]_pin[0][2][46] 
+ grid[1][1]_pin[0][2][50] 
+ grid[1][1]_pin[0][3][3] 
+ grid[1][1]_pin[0][3][7] 
+ grid[1][1]_pin[0][3][11] 
+ grid[1][1]_pin[0][3][15] 
+ grid[1][1]_pin[0][3][19] 
+ grid[1][1]_pin[0][3][23] 
+ grid[1][1]_pin[0][3][27] 
+ grid[1][1]_pin[0][3][31] 
+ grid[1][1]_pin[0][3][35] 
+ grid[1][1]_pin[0][3][39] 
+ grid[1][1]_pin[0][3][43] 
+ grid[1][1]_pin[0][3][47] 
+ gvdd 0 grid[1][1]
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
Vgvdd_local_interc gvdd_local_interc 0 vsp
Vgvdd_sram_luts gvdd_sram_luts 0 vsp
Vgvdd_sram_local_routing gvdd_sram_local_routing 0 vsp
Vgvdd_sram_io gvdd_sram_io 0 vsp
***** Global VDD for load inverters *****
Vgvdd_load gvdd_load 0 vsp
***** Global VDD for Hard Logics *****
***** Global VDD for Look-Up Tables (LUTs) *****
Vgvdd_lut6[0] gvdd_lut6[0] 0 vsp
Rgvdd_lut6[0]_huge gvdd_lut6[0] 0 'vsp/10e-15'
Vgvdd_lut6[1] gvdd_lut6[1] 0 vsp
Rgvdd_lut6[1]_huge gvdd_lut6[1] 0 'vsp/10e-15'
Vgvdd_lut6[2] gvdd_lut6[2] 0 vsp
Rgvdd_lut6[2]_huge gvdd_lut6[2] 0 'vsp/10e-15'
Vgvdd_lut6[3] gvdd_lut6[3] 0 vsp
Rgvdd_lut6[3]_huge gvdd_lut6[3] 0 'vsp/10e-15'
Vgvdd_lut6[4] gvdd_lut6[4] 0 vsp
Rgvdd_lut6[4]_huge gvdd_lut6[4] 0 'vsp/10e-15'
Vgvdd_lut6[5] gvdd_lut6[5] 0 vsp
Rgvdd_lut6[5]_huge gvdd_lut6[5] 0 'vsp/10e-15'
Vgvdd_lut6[6] gvdd_lut6[6] 0 vsp
Rgvdd_lut6[6]_huge gvdd_lut6[6] 0 'vsp/10e-15'
Vgvdd_lut6[7] gvdd_lut6[7] 0 vsp
Rgvdd_lut6[7]_huge gvdd_lut6[7] 0 'vsp/10e-15'
Vgvdd_lut6[8] gvdd_lut6[8] 0 vsp
Rgvdd_lut6[8]_huge gvdd_lut6[8] 0 'vsp/10e-15'
Vgvdd_lut6[9] gvdd_lut6[9] 0 vsp
Rgvdd_lut6[9]_huge gvdd_lut6[9] 0 'vsp/10e-15'
***** Global VDD for Flip-flops (FFs) *****
Vgvdd_dff[0] gvdd_dff[0] 0 vsp
Rgvdd_dff[0]_huge gvdd_dff[0] 0 'vsp/10e-15'
Vgvdd_dff[1] gvdd_dff[1] 0 vsp
Rgvdd_dff[1]_huge gvdd_dff[1] 0 'vsp/10e-15'
Vgvdd_dff[2] gvdd_dff[2] 0 vsp
Rgvdd_dff[2]_huge gvdd_dff[2] 0 'vsp/10e-15'
Vgvdd_dff[3] gvdd_dff[3] 0 vsp
Rgvdd_dff[3]_huge gvdd_dff[3] 0 'vsp/10e-15'
Vgvdd_dff[4] gvdd_dff[4] 0 vsp
Rgvdd_dff[4]_huge gvdd_dff[4] 0 'vsp/10e-15'
Vgvdd_dff[5] gvdd_dff[5] 0 vsp
Rgvdd_dff[5]_huge gvdd_dff[5] 0 'vsp/10e-15'
Vgvdd_dff[6] gvdd_dff[6] 0 vsp
Rgvdd_dff[6]_huge gvdd_dff[6] 0 'vsp/10e-15'
Vgvdd_dff[7] gvdd_dff[7] 0 vsp
Rgvdd_dff[7]_huge gvdd_dff[7] 0 'vsp/10e-15'
Vgvdd_dff[8] gvdd_dff[8] 0 vsp
Rgvdd_dff[8]_huge gvdd_dff[8] 0 'vsp/10e-15'
Vgvdd_dff[9] gvdd_dff[9] 0 vsp
Rgvdd_dff[9]_huge gvdd_dff[9] 0 'vsp/10e-15'
Vgrid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0] 0 
+  0
Vgrid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4] 0 
+  0
Vgrid[1][1]_pin[0][0][8] grid[1][1]_pin[0][0][8] 0 
+  0
Vgrid[1][1]_pin[0][0][12] grid[1][1]_pin[0][0][12] 0 
+  0
Vgrid[1][1]_pin[0][0][16] grid[1][1]_pin[0][0][16] 0 
+  0
Vgrid[1][1]_pin[0][0][20] grid[1][1]_pin[0][0][20] 0 
+  0
Vgrid[1][1]_pin[0][0][24] grid[1][1]_pin[0][0][24] 0 
+  0
Vgrid[1][1]_pin[0][0][28] grid[1][1]_pin[0][0][28] 0 
+  0
Vgrid[1][1]_pin[0][0][32] grid[1][1]_pin[0][0][32] 0 
+  0
Vgrid[1][1]_pin[0][0][36] grid[1][1]_pin[0][0][36] 0 
+  0
Xgrid[1][1]_pin[0][0][40]_inv[0] grid[1][1]_pin[0][0][40] grid[1][1]_pin[0][0][40]_out[0] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][40]_inv[1] grid[1][1]_pin[0][0][40] grid[1][1]_pin[0][0][40]_out[1] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][40]_inv[2] grid[1][1]_pin[0][0][40] grid[1][1]_pin[0][0][40]_out[2] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][40]_inv[3] grid[1][1]_pin[0][0][40] grid[1][1]_pin[0][0][40]_out[3] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][40]_inv[4] grid[1][1]_pin[0][0][40] grid[1][1]_pin[0][0][40]_out[4] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][40]_inv[5] grid[1][1]_pin[0][0][40] grid[1][1]_pin[0][0][40]_out[5] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][40]_inv[6] grid[1][1]_pin[0][0][40] grid[1][1]_pin[0][0][40]_out[6] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][40]_inv[7] grid[1][1]_pin[0][0][40] grid[1][1]_pin[0][0][40]_out[7] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][40]_inv[8] grid[1][1]_pin[0][0][40] grid[1][1]_pin[0][0][40]_out[8] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][40]_inv[9] grid[1][1]_pin[0][0][40] grid[1][1]_pin[0][0][40]_out[9] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][44]_inv[0] grid[1][1]_pin[0][0][44] grid[1][1]_pin[0][0][44]_out[0] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][44]_inv[1] grid[1][1]_pin[0][0][44] grid[1][1]_pin[0][0][44]_out[1] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][44]_inv[2] grid[1][1]_pin[0][0][44] grid[1][1]_pin[0][0][44]_out[2] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][44]_inv[3] grid[1][1]_pin[0][0][44] grid[1][1]_pin[0][0][44]_out[3] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][44]_inv[4] grid[1][1]_pin[0][0][44] grid[1][1]_pin[0][0][44]_out[4] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][44]_inv[5] grid[1][1]_pin[0][0][44] grid[1][1]_pin[0][0][44]_out[5] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][44]_inv[6] grid[1][1]_pin[0][0][44] grid[1][1]_pin[0][0][44]_out[6] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][44]_inv[7] grid[1][1]_pin[0][0][44] grid[1][1]_pin[0][0][44]_out[7] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][44]_inv[8] grid[1][1]_pin[0][0][44] grid[1][1]_pin[0][0][44]_out[8] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][44]_inv[9] grid[1][1]_pin[0][0][44] grid[1][1]_pin[0][0][44]_out[9] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][48]_inv[0] grid[1][1]_pin[0][0][48] grid[1][1]_pin[0][0][48]_out[0] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][48]_inv[1] grid[1][1]_pin[0][0][48] grid[1][1]_pin[0][0][48]_out[1] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][48]_inv[2] grid[1][1]_pin[0][0][48] grid[1][1]_pin[0][0][48]_out[2] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][48]_inv[3] grid[1][1]_pin[0][0][48] grid[1][1]_pin[0][0][48]_out[3] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][48]_inv[4] grid[1][1]_pin[0][0][48] grid[1][1]_pin[0][0][48]_out[4] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][48]_inv[5] grid[1][1]_pin[0][0][48] grid[1][1]_pin[0][0][48]_out[5] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][48]_inv[6] grid[1][1]_pin[0][0][48] grid[1][1]_pin[0][0][48]_out[6] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][48]_inv[7] grid[1][1]_pin[0][0][48] grid[1][1]_pin[0][0][48]_out[7] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][48]_inv[8] grid[1][1]_pin[0][0][48] grid[1][1]_pin[0][0][48]_out[8] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][48]_inv[9] grid[1][1]_pin[0][0][48] grid[1][1]_pin[0][0][48]_out[9] gvdd_load 0 inv size=1
Vgrid[1][1]_pin[0][1][1] grid[1][1]_pin[0][1][1] 0 
+  0
Vgrid[1][1]_pin[0][1][5] grid[1][1]_pin[0][1][5] 0 
+  0
Vgrid[1][1]_pin[0][1][9] grid[1][1]_pin[0][1][9] 0 
+  0
Vgrid[1][1]_pin[0][1][13] grid[1][1]_pin[0][1][13] 0 
+  0
Vgrid[1][1]_pin[0][1][17] grid[1][1]_pin[0][1][17] 0 
+  0
Vgrid[1][1]_pin[0][1][21] grid[1][1]_pin[0][1][21] 0 
+  0
Vgrid[1][1]_pin[0][1][25] grid[1][1]_pin[0][1][25] 0 
+  0
Vgrid[1][1]_pin[0][1][29] grid[1][1]_pin[0][1][29] 0 
+  0
Vgrid[1][1]_pin[0][1][33] grid[1][1]_pin[0][1][33] 0 
+  0
Vgrid[1][1]_pin[0][1][37] grid[1][1]_pin[0][1][37] 0 
+  0
Xgrid[1][1]_pin[0][1][41]_inv[0] grid[1][1]_pin[0][1][41] grid[1][1]_pin[0][1][41]_out[0] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][41]_inv[1] grid[1][1]_pin[0][1][41] grid[1][1]_pin[0][1][41]_out[1] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][41]_inv[2] grid[1][1]_pin[0][1][41] grid[1][1]_pin[0][1][41]_out[2] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][41]_inv[3] grid[1][1]_pin[0][1][41] grid[1][1]_pin[0][1][41]_out[3] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][41]_inv[4] grid[1][1]_pin[0][1][41] grid[1][1]_pin[0][1][41]_out[4] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][41]_inv[5] grid[1][1]_pin[0][1][41] grid[1][1]_pin[0][1][41]_out[5] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][41]_inv[6] grid[1][1]_pin[0][1][41] grid[1][1]_pin[0][1][41]_out[6] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][41]_inv[7] grid[1][1]_pin[0][1][41] grid[1][1]_pin[0][1][41]_out[7] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][41]_inv[8] grid[1][1]_pin[0][1][41] grid[1][1]_pin[0][1][41]_out[8] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][41]_inv[9] grid[1][1]_pin[0][1][41] grid[1][1]_pin[0][1][41]_out[9] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][45]_inv[0] grid[1][1]_pin[0][1][45] grid[1][1]_pin[0][1][45]_out[0] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][45]_inv[1] grid[1][1]_pin[0][1][45] grid[1][1]_pin[0][1][45]_out[1] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][45]_inv[2] grid[1][1]_pin[0][1][45] grid[1][1]_pin[0][1][45]_out[2] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][45]_inv[3] grid[1][1]_pin[0][1][45] grid[1][1]_pin[0][1][45]_out[3] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][45]_inv[4] grid[1][1]_pin[0][1][45] grid[1][1]_pin[0][1][45]_out[4] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][45]_inv[5] grid[1][1]_pin[0][1][45] grid[1][1]_pin[0][1][45]_out[5] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][45]_inv[6] grid[1][1]_pin[0][1][45] grid[1][1]_pin[0][1][45]_out[6] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][45]_inv[7] grid[1][1]_pin[0][1][45] grid[1][1]_pin[0][1][45]_out[7] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][45]_inv[8] grid[1][1]_pin[0][1][45] grid[1][1]_pin[0][1][45]_out[8] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][45]_inv[9] grid[1][1]_pin[0][1][45] grid[1][1]_pin[0][1][45]_out[9] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][49]_inv[0] grid[1][1]_pin[0][1][49] grid[1][1]_pin[0][1][49]_out[0] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][49]_inv[1] grid[1][1]_pin[0][1][49] grid[1][1]_pin[0][1][49]_out[1] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][49]_inv[2] grid[1][1]_pin[0][1][49] grid[1][1]_pin[0][1][49]_out[2] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][49]_inv[3] grid[1][1]_pin[0][1][49] grid[1][1]_pin[0][1][49]_out[3] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][49]_inv[4] grid[1][1]_pin[0][1][49] grid[1][1]_pin[0][1][49]_out[4] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][49]_inv[5] grid[1][1]_pin[0][1][49] grid[1][1]_pin[0][1][49]_out[5] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][49]_inv[6] grid[1][1]_pin[0][1][49] grid[1][1]_pin[0][1][49]_out[6] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][49]_inv[7] grid[1][1]_pin[0][1][49] grid[1][1]_pin[0][1][49]_out[7] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][49]_inv[8] grid[1][1]_pin[0][1][49] grid[1][1]_pin[0][1][49]_out[8] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][1][49]_inv[9] grid[1][1]_pin[0][1][49] grid[1][1]_pin[0][1][49]_out[9] gvdd_load 0 inv size=1
Vgrid[1][1]_pin[0][2][2] grid[1][1]_pin[0][2][2] 0 
+  0
Vgrid[1][1]_pin[0][2][6] grid[1][1]_pin[0][2][6] 0 
+  0
Vgrid[1][1]_pin[0][2][10] grid[1][1]_pin[0][2][10] 0 
+  0
Vgrid[1][1]_pin[0][2][14] grid[1][1]_pin[0][2][14] 0 
+  0
Vgrid[1][1]_pin[0][2][18] grid[1][1]_pin[0][2][18] 0 
+  0
Vgrid[1][1]_pin[0][2][22] grid[1][1]_pin[0][2][22] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgrid[1][1]_pin[0][2][26] grid[1][1]_pin[0][2][26] 0 
+  0
Vgrid[1][1]_pin[0][2][30] grid[1][1]_pin[0][2][30] 0 
+  0
Vgrid[1][1]_pin[0][2][34] grid[1][1]_pin[0][2][34] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgrid[1][1]_pin[0][2][38] grid[1][1]_pin[0][2][38] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Xgrid[1][1]_pin[0][2][42]_inv[0] grid[1][1]_pin[0][2][42] grid[1][1]_pin[0][2][42]_out[0] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][42]_inv[1] grid[1][1]_pin[0][2][42] grid[1][1]_pin[0][2][42]_out[1] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][42]_inv[2] grid[1][1]_pin[0][2][42] grid[1][1]_pin[0][2][42]_out[2] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][42]_inv[3] grid[1][1]_pin[0][2][42] grid[1][1]_pin[0][2][42]_out[3] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][42]_inv[4] grid[1][1]_pin[0][2][42] grid[1][1]_pin[0][2][42]_out[4] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][42]_inv[5] grid[1][1]_pin[0][2][42] grid[1][1]_pin[0][2][42]_out[5] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][42]_inv[6] grid[1][1]_pin[0][2][42] grid[1][1]_pin[0][2][42]_out[6] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][42]_inv[7] grid[1][1]_pin[0][2][42] grid[1][1]_pin[0][2][42]_out[7] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][42]_inv[8] grid[1][1]_pin[0][2][42] grid[1][1]_pin[0][2][42]_out[8] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][42]_inv[9] grid[1][1]_pin[0][2][42] grid[1][1]_pin[0][2][42]_out[9] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][46]_inv[0] grid[1][1]_pin[0][2][46] grid[1][1]_pin[0][2][46]_out[0] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][46]_inv[1] grid[1][1]_pin[0][2][46] grid[1][1]_pin[0][2][46]_out[1] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][46]_inv[2] grid[1][1]_pin[0][2][46] grid[1][1]_pin[0][2][46]_out[2] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][46]_inv[3] grid[1][1]_pin[0][2][46] grid[1][1]_pin[0][2][46]_out[3] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][46]_inv[4] grid[1][1]_pin[0][2][46] grid[1][1]_pin[0][2][46]_out[4] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][46]_inv[5] grid[1][1]_pin[0][2][46] grid[1][1]_pin[0][2][46]_out[5] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][46]_inv[6] grid[1][1]_pin[0][2][46] grid[1][1]_pin[0][2][46]_out[6] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][46]_inv[7] grid[1][1]_pin[0][2][46] grid[1][1]_pin[0][2][46]_out[7] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][46]_inv[8] grid[1][1]_pin[0][2][46] grid[1][1]_pin[0][2][46]_out[8] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][2][46]_inv[9] grid[1][1]_pin[0][2][46] grid[1][1]_pin[0][2][46]_out[9] gvdd_load 0 inv size=1
Vgrid[1][1]_pin[0][2][50] grid[1][1]_pin[0][2][50] 0 
+  0
Vgrid[1][1]_pin[0][3][3] grid[1][1]_pin[0][3][3] 0 
+  0
Vgrid[1][1]_pin[0][3][7] grid[1][1]_pin[0][3][7] 0 
+  0
Vgrid[1][1]_pin[0][3][11] grid[1][1]_pin[0][3][11] 0 
+  0
Vgrid[1][1]_pin[0][3][15] grid[1][1]_pin[0][3][15] 0 
+  0
Vgrid[1][1]_pin[0][3][19] grid[1][1]_pin[0][3][19] 0 
+  0
Vgrid[1][1]_pin[0][3][23] grid[1][1]_pin[0][3][23] 0 
+  0
Vgrid[1][1]_pin[0][3][27] grid[1][1]_pin[0][3][27] 0 
+  0
Vgrid[1][1]_pin[0][3][31] grid[1][1]_pin[0][3][31] 0 
+  0
Vgrid[1][1]_pin[0][3][35] grid[1][1]_pin[0][3][35] 0 
+  0
Vgrid[1][1]_pin[0][3][39] grid[1][1]_pin[0][3][39] 0 
+  0
Xgrid[1][1]_pin[0][3][43]_inv[0] grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][43]_out[0] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][43]_inv[1] grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][43]_out[1] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][43]_inv[2] grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][43]_out[2] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][43]_inv[3] grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][43]_out[3] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][43]_inv[4] grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][43]_out[4] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][43]_inv[5] grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][43]_out[5] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][43]_inv[6] grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][43]_out[6] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][43]_inv[7] grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][43]_out[7] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][43]_inv[8] grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][43]_out[8] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][43]_inv[9] grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][43]_out[9] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][47]_inv[0] grid[1][1]_pin[0][3][47] grid[1][1]_pin[0][3][47]_out[0] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][47]_inv[1] grid[1][1]_pin[0][3][47] grid[1][1]_pin[0][3][47]_out[1] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][47]_inv[2] grid[1][1]_pin[0][3][47] grid[1][1]_pin[0][3][47]_out[2] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][47]_inv[3] grid[1][1]_pin[0][3][47] grid[1][1]_pin[0][3][47]_out[3] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][47]_inv[4] grid[1][1]_pin[0][3][47] grid[1][1]_pin[0][3][47]_out[4] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][47]_inv[5] grid[1][1]_pin[0][3][47] grid[1][1]_pin[0][3][47]_out[5] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][47]_inv[6] grid[1][1]_pin[0][3][47] grid[1][1]_pin[0][3][47]_out[6] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][47]_inv[7] grid[1][1]_pin[0][3][47] grid[1][1]_pin[0][3][47]_out[7] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][47]_inv[8] grid[1][1]_pin[0][3][47] grid[1][1]_pin[0][3][47]_out[8] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][3][47]_inv[9] grid[1][1]_pin[0][3][47] grid[1][1]_pin[0][3][47]_out[9] gvdd_load 0 inv size=1
***** 6 Clock Simulation, accuracy=1e-13 *****
.tran 1e-13  '6*clock_period'
***** Generic Measurements for Circuit Parameters *****
.measure tran leakage_power_sram_local_routing avg p(Vgvdd_sram_local_routing) from=0 to='clock_period'
.measure tran leakage_power_sram_luts avg p(Vgvdd_sram_luts) from=0 to='clock_period'
.measure tran leakage_power_local_routing avg p(Vgvdd_local_interc) from=0 to='clock_period'
.measure tran leakage_power_lut6[0] avg p(Vgvdd_lut6[0]) from=0 to='clock_period'
.measure tran leakage_power_lut6[1] avg p(Vgvdd_lut6[1]) from=0 to='clock_period'
.measure tran leakage_power_lut6[2] avg p(Vgvdd_lut6[2]) from=0 to='clock_period'
.measure tran leakage_power_lut6[3] avg p(Vgvdd_lut6[3]) from=0 to='clock_period'
.measure tran leakage_power_lut6[4] avg p(Vgvdd_lut6[4]) from=0 to='clock_period'
.measure tran leakage_power_lut6[5] avg p(Vgvdd_lut6[5]) from=0 to='clock_period'
.measure tran leakage_power_lut6[6] avg p(Vgvdd_lut6[6]) from=0 to='clock_period'
.measure tran leakage_power_lut6[7] avg p(Vgvdd_lut6[7]) from=0 to='clock_period'
.measure tran leakage_power_lut6[8] avg p(Vgvdd_lut6[8]) from=0 to='clock_period'
.measure tran leakage_power_lut6[9] avg p(Vgvdd_lut6[9]) from=0 to='clock_period'
.measure tran leakage_power_lut6[0to0] 
+ param = 'leakage_power_lut6[0]'
.measure tran leakage_power_lut6[0to1] 
+ param = 'leakage_power_lut6[1]+leakage_power_lut6[0to0]'
.measure tran leakage_power_lut6[0to2] 
+ param = 'leakage_power_lut6[2]+leakage_power_lut6[0to1]'
.measure tran leakage_power_lut6[0to3] 
+ param = 'leakage_power_lut6[3]+leakage_power_lut6[0to2]'
.measure tran leakage_power_lut6[0to4] 
+ param = 'leakage_power_lut6[4]+leakage_power_lut6[0to3]'
.measure tran leakage_power_lut6[0to5] 
+ param = 'leakage_power_lut6[5]+leakage_power_lut6[0to4]'
.measure tran leakage_power_lut6[0to6] 
+ param = 'leakage_power_lut6[6]+leakage_power_lut6[0to5]'
.measure tran leakage_power_lut6[0to7] 
+ param = 'leakage_power_lut6[7]+leakage_power_lut6[0to6]'
.measure tran leakage_power_lut6[0to8] 
+ param = 'leakage_power_lut6[8]+leakage_power_lut6[0to7]'
.measure tran leakage_power_lut6[0to9] 
+ param = 'leakage_power_lut6[9]+leakage_power_lut6[0to8]'
.measure tran total_leakage_power_lut6 
+ param = 'leakage_power_lut6[0to9]'
.measure tran leakage_power_dff[0] avg p(Vgvdd_dff[0]) from=0 to='clock_period'
.measure tran leakage_power_dff[1] avg p(Vgvdd_dff[1]) from=0 to='clock_period'
.measure tran leakage_power_dff[2] avg p(Vgvdd_dff[2]) from=0 to='clock_period'
.measure tran leakage_power_dff[3] avg p(Vgvdd_dff[3]) from=0 to='clock_period'
.measure tran leakage_power_dff[4] avg p(Vgvdd_dff[4]) from=0 to='clock_period'
.measure tran leakage_power_dff[5] avg p(Vgvdd_dff[5]) from=0 to='clock_period'
.measure tran leakage_power_dff[6] avg p(Vgvdd_dff[6]) from=0 to='clock_period'
.measure tran leakage_power_dff[7] avg p(Vgvdd_dff[7]) from=0 to='clock_period'
.measure tran leakage_power_dff[8] avg p(Vgvdd_dff[8]) from=0 to='clock_period'
.measure tran leakage_power_dff[9] avg p(Vgvdd_dff[9]) from=0 to='clock_period'
.measure tran leakage_power_dff[0to0] 
+ param = 'leakage_power_dff[0]'
.measure tran leakage_power_dff[0to1] 
+ param = 'leakage_power_dff[1]+leakage_power_dff[0to0]'
.measure tran leakage_power_dff[0to2] 
+ param = 'leakage_power_dff[2]+leakage_power_dff[0to1]'
.measure tran leakage_power_dff[0to3] 
+ param = 'leakage_power_dff[3]+leakage_power_dff[0to2]'
.measure tran leakage_power_dff[0to4] 
+ param = 'leakage_power_dff[4]+leakage_power_dff[0to3]'
.measure tran leakage_power_dff[0to5] 
+ param = 'leakage_power_dff[5]+leakage_power_dff[0to4]'
.measure tran leakage_power_dff[0to6] 
+ param = 'leakage_power_dff[6]+leakage_power_dff[0to5]'
.measure tran leakage_power_dff[0to7] 
+ param = 'leakage_power_dff[7]+leakage_power_dff[0to6]'
.measure tran leakage_power_dff[0to8] 
+ param = 'leakage_power_dff[8]+leakage_power_dff[0to7]'
.measure tran leakage_power_dff[0to9] 
+ param = 'leakage_power_dff[9]+leakage_power_dff[0to8]'
.measure tran total_leakage_power_dff 
+ param = 'leakage_power_dff[0to9]'
.measure tran dynamic_power_sram_local_routing avg p(Vgvdd_sram_local_routing) from='clock_period' to='6*clock_period'
.measure tran total_energy_per_cycle_sram_local_routing param='dynamic_power_sram_local_routing*clock_period'
.measure tran dynamic_power_sram_luts avg p(Vgvdd_sram_luts) from='clock_period' to='6*clock_period'
.measure tran total_energy_per_cycle_sram_luts param='dynamic_power_sram_luts*clock_period'
.measure tran dynamic_power_local_interc avg p(Vgvdd_local_interc) from='clock_period' to='6*clock_period'
.measure tran total_energy_per_cycle_local_routing param='dynamic_power_local_interc*clock_period'
.measure tran dynamic_power_lut6[0] avg p(Vgvdd_lut6[0]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_lut6[1] avg p(Vgvdd_lut6[1]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_lut6[2] avg p(Vgvdd_lut6[2]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_lut6[3] avg p(Vgvdd_lut6[3]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_lut6[4] avg p(Vgvdd_lut6[4]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_lut6[5] avg p(Vgvdd_lut6[5]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_lut6[6] avg p(Vgvdd_lut6[6]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_lut6[7] avg p(Vgvdd_lut6[7]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_lut6[8] avg p(Vgvdd_lut6[8]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_lut6[9] avg p(Vgvdd_lut6[9]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_lut6[0to0] 
+ param = 'dynamic_power_lut6[0]'
.measure tran dynamic_power_lut6[0to1] 
+ param = 'dynamic_power_lut6[1]+dynamic_power_lut6[0to0]'
.measure tran dynamic_power_lut6[0to2] 
+ param = 'dynamic_power_lut6[2]+dynamic_power_lut6[0to1]'
.measure tran dynamic_power_lut6[0to3] 
+ param = 'dynamic_power_lut6[3]+dynamic_power_lut6[0to2]'
.measure tran dynamic_power_lut6[0to4] 
+ param = 'dynamic_power_lut6[4]+dynamic_power_lut6[0to3]'
.measure tran dynamic_power_lut6[0to5] 
+ param = 'dynamic_power_lut6[5]+dynamic_power_lut6[0to4]'
.measure tran dynamic_power_lut6[0to6] 
+ param = 'dynamic_power_lut6[6]+dynamic_power_lut6[0to5]'
.measure tran dynamic_power_lut6[0to7] 
+ param = 'dynamic_power_lut6[7]+dynamic_power_lut6[0to6]'
.measure tran dynamic_power_lut6[0to8] 
+ param = 'dynamic_power_lut6[8]+dynamic_power_lut6[0to7]'
.measure tran dynamic_power_lut6[0to9] 
+ param = 'dynamic_power_lut6[9]+dynamic_power_lut6[0to8]'
.measure tran total_dynamic_power_lut6 
+ param = 'dynamic_power_lut6[0to9]'
.measure tran total_energy_per_cycle_lut6 
+ param = 'dynamic_power_lut6[0to9]*clock_period'
.measure tran dynamic_power_dff[0] avg p(Vgvdd_dff[0]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_dff[1] avg p(Vgvdd_dff[1]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_dff[2] avg p(Vgvdd_dff[2]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_dff[3] avg p(Vgvdd_dff[3]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_dff[4] avg p(Vgvdd_dff[4]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_dff[5] avg p(Vgvdd_dff[5]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_dff[6] avg p(Vgvdd_dff[6]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_dff[7] avg p(Vgvdd_dff[7]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_dff[8] avg p(Vgvdd_dff[8]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_dff[9] avg p(Vgvdd_dff[9]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_dff[0to0] 
+ param = 'dynamic_power_dff[0]'
.measure tran dynamic_power_dff[0to1] 
+ param = 'dynamic_power_dff[1]+dynamic_power_dff[0to0]'
.measure tran dynamic_power_dff[0to2] 
+ param = 'dynamic_power_dff[2]+dynamic_power_dff[0to1]'
.measure tran dynamic_power_dff[0to3] 
+ param = 'dynamic_power_dff[3]+dynamic_power_dff[0to2]'
.measure tran dynamic_power_dff[0to4] 
+ param = 'dynamic_power_dff[4]+dynamic_power_dff[0to3]'
.measure tran dynamic_power_dff[0to5] 
+ param = 'dynamic_power_dff[5]+dynamic_power_dff[0to4]'
.measure tran dynamic_power_dff[0to6] 
+ param = 'dynamic_power_dff[6]+dynamic_power_dff[0to5]'
.measure tran dynamic_power_dff[0to7] 
+ param = 'dynamic_power_dff[7]+dynamic_power_dff[0to6]'
.measure tran dynamic_power_dff[0to8] 
+ param = 'dynamic_power_dff[8]+dynamic_power_dff[0to7]'
.measure tran dynamic_power_dff[0to9] 
+ param = 'dynamic_power_dff[9]+dynamic_power_dff[0to8]'
.measure tran total_dynamic_power_dff 
+ param = 'dynamic_power_dff[0to9]'
.measure tran total_energy_per_cycle_dff 
+ param = 'dynamic_power_dff[0to9]*clock_period'
.end
