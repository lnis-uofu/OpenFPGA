*****************************
*     FPGA SPICE Netlist    *
* Description: FPGA SPICE Switch Block Testbench Bench for Design: example_1 *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
****** Include Header file: circuit design parameters *****
.include './spice_test_example_1/include/design_params.sp'
****** Include Header file: measurement parameters *****
.include './spice_test_example_1/include/meas_params.sp'
****** Include Header file: stimulation parameters *****
.include './spice_test_example_1/include/stimulate_params.sp'
****** Include subckt netlists: NMOS and PMOS *****
.include './spice_test_example_1/subckt/nmos_pmos.sp'
****** Include subckt netlists: Inverters, Buffers *****
.include './spice_test_example_1/subckt/inv_buf_trans_gate.sp'
****** Include subckt netlists: Multiplexers *****
.include './spice_test_example_1/subckt/muxes.sp'
****** Include subckt netlists: Wires *****
.include './spice_test_example_1/subckt/wires.sp'
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
.global gvdd_sram_sbs
****** Include subckt netlists: Switch Block[1][0] *****
.include './spice_test_example_1/subckt/sb_1_0.sp'
***** Call defined Switch Box[1][0] *****
Xsb[1][0] 
+ chany[1][1]_out[0] chany[1][1]_in[1] chany[1][1]_out[2] chany[1][1]_in[3] chany[1][1]_out[4] chany[1][1]_in[5] chany[1][1]_out[6] chany[1][1]_in[7] chany[1][1]_out[8] chany[1][1]_in[9] chany[1][1]_out[10] chany[1][1]_in[11] chany[1][1]_out[12] chany[1][1]_in[13] chany[1][1]_out[14] chany[1][1]_in[15] chany[1][1]_out[16] chany[1][1]_in[17] chany[1][1]_out[18] chany[1][1]_in[19] chany[1][1]_out[20] chany[1][1]_in[21] chany[1][1]_out[22] chany[1][1]_in[23] chany[1][1]_out[24] chany[1][1]_in[25] chany[1][1]_out[26] chany[1][1]_in[27] chany[1][1]_out[28] chany[1][1]_in[29] 
+ grid[2][1]_pin[0][3][1] grid[2][1]_pin[0][3][3] grid[2][1]_pin[0][3][5] grid[2][1]_pin[0][3][7] grid[2][1]_pin[0][3][9] grid[2][1]_pin[0][3][11] grid[2][1]_pin[0][3][13] grid[2][1]_pin[0][3][15] 
+ 
+ 
+ 
+ 
+ chanx[1][0]_in[0] chanx[1][0]_out[1] chanx[1][0]_in[2] chanx[1][0]_out[3] chanx[1][0]_in[4] chanx[1][0]_out[5] chanx[1][0]_in[6] chanx[1][0]_out[7] chanx[1][0]_in[8] chanx[1][0]_out[9] chanx[1][0]_in[10] chanx[1][0]_out[11] chanx[1][0]_in[12] chanx[1][0]_out[13] chanx[1][0]_in[14] chanx[1][0]_out[15] chanx[1][0]_in[16] chanx[1][0]_out[17] chanx[1][0]_in[18] chanx[1][0]_out[19] chanx[1][0]_in[20] chanx[1][0]_out[21] chanx[1][0]_in[22] chanx[1][0]_out[23] chanx[1][0]_in[24] chanx[1][0]_out[25] chanx[1][0]_in[26] chanx[1][0]_out[27] chanx[1][0]_in[28] chanx[1][0]_out[29] 
+ grid[1][0]_pin[0][0][1] grid[1][0]_pin[0][0][3] grid[1][0]_pin[0][0][5] grid[1][0]_pin[0][0][7] grid[1][0]_pin[0][0][9] grid[1][0]_pin[0][0][11] grid[1][0]_pin[0][0][13] grid[1][0]_pin[0][0][15] 
+  gvdd_sb[1][0] 0 sb[1][0]
**** Load for rr_node[227] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=0, type=5 *****
Xchan_chany[1][1]_out[0]_loadlvl[0]_out chany[1][1]_out[0] chany[1][1]_out[0]_loadlvl[0]_out chany[1][1]_out[0]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[0]_no0 chany[1][1]_out[0]_loadlvl[0]_out chany[1][1]_out[0]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[1]_no0 chany[1][1]_out[0]_loadlvl[0]_midout chany[1][1]_out[0]_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[1] density = 0, probability=0.*****
Vchany[1][1]_in[1] chany[1][1]_in[1] 0 
+  0
**** Load for rr_node[229] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=2, type=5 *****
Xchan_chany[1][1]_out[2]_loadlvl[0]_out chany[1][1]_out[2] chany[1][1]_out[2]_loadlvl[0]_out chany[1][1]_out[2]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[2]_no0 chany[1][1]_out[2]_loadlvl[0]_out chany[1][1]_out[2]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[3] density = 0, probability=0.*****
Vchany[1][1]_in[3] chany[1][1]_in[3] 0 
+  0
**** Load for rr_node[231] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=4, type=5 *****
Xchan_chany[1][1]_out[4]_loadlvl[0]_out chany[1][1]_out[4] chany[1][1]_out[4]_loadlvl[0]_out chany[1][1]_out[4]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[3]_no0 chany[1][1]_out[4]_loadlvl[0]_out chany[1][1]_out[4]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[5] density = 0, probability=0.*****
Vchany[1][1]_in[5] chany[1][1]_in[5] 0 
+  0
**** Load for rr_node[233] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=6, type=5 *****
Xchan_chany[1][1]_out[6]_loadlvl[0]_out chany[1][1]_out[6] chany[1][1]_out[6]_loadlvl[0]_out chany[1][1]_out[6]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[4]_no0 chany[1][1]_out[6]_loadlvl[0]_out chany[1][1]_out[6]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[7] density = 0, probability=0.*****
Vchany[1][1]_in[7] chany[1][1]_in[7] 0 
+  0
**** Load for rr_node[235] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=8, type=5 *****
Xchan_chany[1][1]_out[8]_loadlvl[0]_out chany[1][1]_out[8] chany[1][1]_out[8]_loadlvl[0]_out chany[1][1]_out[8]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[5]_no0 chany[1][1]_out[8]_loadlvl[0]_out chany[1][1]_out[8]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[9] density = 0, probability=0.*****
Vchany[1][1]_in[9] chany[1][1]_in[9] 0 
+  0
**** Load for rr_node[237] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=10, type=5 *****
Xchan_chany[1][1]_out[10]_loadlvl[0]_out chany[1][1]_out[10] chany[1][1]_out[10]_loadlvl[0]_out chany[1][1]_out[10]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[6]_no0 chany[1][1]_out[10]_loadlvl[0]_out chany[1][1]_out[10]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[11] density = 0, probability=0.*****
Vchany[1][1]_in[11] chany[1][1]_in[11] 0 
+  0
**** Load for rr_node[239] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=12, type=5 *****
Xchan_chany[1][1]_out[12]_loadlvl[0]_out chany[1][1]_out[12] chany[1][1]_out[12]_loadlvl[0]_out chany[1][1]_out[12]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[7]_no0 chany[1][1]_out[12]_loadlvl[0]_out chany[1][1]_out[12]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[13] density = 0, probability=0.*****
Vchany[1][1]_in[13] chany[1][1]_in[13] 0 
+  0
**** Load for rr_node[241] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=14, type=5 *****
Xchan_chany[1][1]_out[14]_loadlvl[0]_out chany[1][1]_out[14] chany[1][1]_out[14]_loadlvl[0]_out chany[1][1]_out[14]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[8]_no0 chany[1][1]_out[14]_loadlvl[0]_out chany[1][1]_out[14]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[15] density = 0, probability=0.*****
Vchany[1][1]_in[15] chany[1][1]_in[15] 0 
+  0
**** Load for rr_node[243] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=16, type=5 *****
Xchan_chany[1][1]_out[16]_loadlvl[0]_out chany[1][1]_out[16] chany[1][1]_out[16]_loadlvl[0]_out chany[1][1]_out[16]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[9]_no0 chany[1][1]_out[16]_loadlvl[0]_out chany[1][1]_out[16]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[10]_no0 chany[1][1]_out[16]_loadlvl[0]_midout chany[1][1]_out[16]_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[17] density = 0, probability=0.*****
Vchany[1][1]_in[17] chany[1][1]_in[17] 0 
+  0
**** Load for rr_node[245] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=18, type=5 *****
Xchan_chany[1][1]_out[18]_loadlvl[0]_out chany[1][1]_out[18] chany[1][1]_out[18]_loadlvl[0]_out chany[1][1]_out[18]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[11]_no0 chany[1][1]_out[18]_loadlvl[0]_out chany[1][1]_out[18]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[19] density = 0, probability=0.*****
Vchany[1][1]_in[19] chany[1][1]_in[19] 0 
+  0
**** Load for rr_node[247] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=20, type=5 *****
Xchan_chany[1][1]_out[20]_loadlvl[0]_out chany[1][1]_out[20] chany[1][1]_out[20]_loadlvl[0]_out chany[1][1]_out[20]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[12]_no0 chany[1][1]_out[20]_loadlvl[0]_out chany[1][1]_out[20]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[21] density = 0, probability=0.*****
Vchany[1][1]_in[21] chany[1][1]_in[21] 0 
+  0
**** Load for rr_node[249] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=22, type=5 *****
Xchan_chany[1][1]_out[22]_loadlvl[0]_out chany[1][1]_out[22] chany[1][1]_out[22]_loadlvl[0]_out chany[1][1]_out[22]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[13]_no0 chany[1][1]_out[22]_loadlvl[0]_out chany[1][1]_out[22]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[23] density = 0, probability=0.*****
Vchany[1][1]_in[23] chany[1][1]_in[23] 0 
+  0
**** Load for rr_node[251] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=24, type=5 *****
Xchan_chany[1][1]_out[24]_loadlvl[0]_out chany[1][1]_out[24] chany[1][1]_out[24]_loadlvl[0]_out chany[1][1]_out[24]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[14]_no0 chany[1][1]_out[24]_loadlvl[0]_out chany[1][1]_out[24]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[25] density = 0, probability=0.*****
Vchany[1][1]_in[25] chany[1][1]_in[25] 0 
+  0
**** Load for rr_node[253] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=26, type=5 *****
Xchan_chany[1][1]_out[26]_loadlvl[0]_out chany[1][1]_out[26] chany[1][1]_out[26]_loadlvl[0]_out chany[1][1]_out[26]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[15]_no0 chany[1][1]_out[26]_loadlvl[0]_out chany[1][1]_out[26]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[27] density = 0, probability=0.*****
Vchany[1][1]_in[27] chany[1][1]_in[27] 0 
+  0
**** Load for rr_node[255] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=28, type=5 *****
Xchan_chany[1][1]_out[28]_loadlvl[0]_out chany[1][1]_out[28] chany[1][1]_out[28]_loadlvl[0]_out chany[1][1]_out[28]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[16]_no0 chany[1][1]_out[28]_loadlvl[0]_out chany[1][1]_out[28]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
***** Signal chany[1][1]_in[29] density = 0, probability=0.*****
Vchany[1][1]_in[29] chany[1][1]_in[29] 0 
+  0
Vgrid[2][1]_pin[0][3][1] grid[2][1]_pin[0][3][1] 0 
+  0
Vgrid[2][1]_pin[0][3][3] grid[2][1]_pin[0][3][3] 0 
+  0
Vgrid[2][1]_pin[0][3][5] grid[2][1]_pin[0][3][5] 0 
+  0
Vgrid[2][1]_pin[0][3][7] grid[2][1]_pin[0][3][7] 0 
+  0
Vgrid[2][1]_pin[0][3][9] grid[2][1]_pin[0][3][9] 0 
+  0
Vgrid[2][1]_pin[0][3][11] grid[2][1]_pin[0][3][11] 0 
+  0
Vgrid[2][1]_pin[0][3][13] grid[2][1]_pin[0][3][13] 0 
+  0
Vgrid[2][1]_pin[0][3][15] grid[2][1]_pin[0][3][15] 0 
+  0



***** Signal chanx[1][0]_in[0] density = 0, probability=0.*****
Vchanx[1][0]_in[0] chanx[1][0]_in[0] 0 
+  0
**** Load for rr_node[138] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=1, type=4 *****
Xchan_chanx[1][0]_out[1]_loadlvl[0]_out chanx[1][0]_out[1] chanx[1][0]_out[1]_loadlvl[0]_out chanx[1][0]_out[1]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[17]_no0 chanx[1][0]_out[1]_loadlvl[0]_out chanx[1][0]_out[1]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[18]_no0 chanx[1][0]_out[1]_loadlvl[0]_midout chanx[1][0]_out[1]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[19]_no0 chanx[1][0]_out[1]_loadlvl[0]_midout chanx[1][0]_out[1]_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[2] density = 0, probability=0.*****
Vchanx[1][0]_in[2] chanx[1][0]_in[2] 0 
+  0
**** Load for rr_node[140] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=3, type=4 *****
Xchan_chanx[1][0]_out[3]_loadlvl[0]_out chanx[1][0]_out[3] chanx[1][0]_out[3]_loadlvl[0]_out chanx[1][0]_out[3]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[20]_no0 chanx[1][0]_out[3]_loadlvl[0]_out chanx[1][0]_out[3]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[21]_no0 chanx[1][0]_out[3]_loadlvl[0]_midout chanx[1][0]_out[3]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[4] density = 0, probability=0.*****
Vchanx[1][0]_in[4] chanx[1][0]_in[4] 0 
+  0
**** Load for rr_node[142] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=5, type=4 *****
Xchan_chanx[1][0]_out[5]_loadlvl[0]_out chanx[1][0]_out[5] chanx[1][0]_out[5]_loadlvl[0]_out chanx[1][0]_out[5]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[22]_no0 chanx[1][0]_out[5]_loadlvl[0]_out chanx[1][0]_out[5]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[23]_no0 chanx[1][0]_out[5]_loadlvl[0]_midout chanx[1][0]_out[5]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[6] density = 0, probability=0.*****
Vchanx[1][0]_in[6] chanx[1][0]_in[6] 0 
+  0
**** Load for rr_node[144] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=7, type=4 *****
Xchan_chanx[1][0]_out[7]_loadlvl[0]_out chanx[1][0]_out[7] chanx[1][0]_out[7]_loadlvl[0]_out chanx[1][0]_out[7]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[24]_no0 chanx[1][0]_out[7]_loadlvl[0]_out chanx[1][0]_out[7]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[25]_no0 chanx[1][0]_out[7]_loadlvl[0]_midout chanx[1][0]_out[7]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[26]_no0 chanx[1][0]_out[7]_loadlvl[0]_midout chanx[1][0]_out[7]_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[8] density = 0, probability=0.*****
Vchanx[1][0]_in[8] chanx[1][0]_in[8] 0 
+  0
**** Load for rr_node[146] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=9, type=4 *****
Xchan_chanx[1][0]_out[9]_loadlvl[0]_out chanx[1][0]_out[9] chanx[1][0]_out[9]_loadlvl[0]_out chanx[1][0]_out[9]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[27]_no0 chanx[1][0]_out[9]_loadlvl[0]_out chanx[1][0]_out[9]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[28]_no0 chanx[1][0]_out[9]_loadlvl[0]_midout chanx[1][0]_out[9]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[10] density = 0, probability=0.*****
Vchanx[1][0]_in[10] chanx[1][0]_in[10] 0 
+  0
**** Load for rr_node[148] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=11, type=4 *****
Xchan_chanx[1][0]_out[11]_loadlvl[0]_out chanx[1][0]_out[11] chanx[1][0]_out[11]_loadlvl[0]_out chanx[1][0]_out[11]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[29]_no0 chanx[1][0]_out[11]_loadlvl[0]_out chanx[1][0]_out[11]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[30]_no0 chanx[1][0]_out[11]_loadlvl[0]_midout chanx[1][0]_out[11]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[12] density = 0, probability=0.*****
Vchanx[1][0]_in[12] chanx[1][0]_in[12] 0 
+  0
**** Load for rr_node[150] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=13, type=4 *****
Xchan_chanx[1][0]_out[13]_loadlvl[0]_out chanx[1][0]_out[13] chanx[1][0]_out[13]_loadlvl[0]_out chanx[1][0]_out[13]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[31]_no0 chanx[1][0]_out[13]_loadlvl[0]_out chanx[1][0]_out[13]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[32]_no0 chanx[1][0]_out[13]_loadlvl[0]_midout chanx[1][0]_out[13]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[14] density = 0, probability=0.*****
Vchanx[1][0]_in[14] chanx[1][0]_in[14] 0 
+  0
**** Load for rr_node[152] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=15, type=4 *****
Xchan_chanx[1][0]_out[15]_loadlvl[0]_out chanx[1][0]_out[15] chanx[1][0]_out[15]_loadlvl[0]_out chanx[1][0]_out[15]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[33]_no0 chanx[1][0]_out[15]_loadlvl[0]_out chanx[1][0]_out[15]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[34]_no0 chanx[1][0]_out[15]_loadlvl[0]_midout chanx[1][0]_out[15]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[16] density = 0, probability=0.*****
Vchanx[1][0]_in[16] chanx[1][0]_in[16] 0 
+  0
**** Load for rr_node[154] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=17, type=4 *****
Xchan_chanx[1][0]_out[17]_loadlvl[0]_out chanx[1][0]_out[17] chanx[1][0]_out[17]_loadlvl[0]_out chanx[1][0]_out[17]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[35]_no0 chanx[1][0]_out[17]_loadlvl[0]_out chanx[1][0]_out[17]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[36]_no0 chanx[1][0]_out[17]_loadlvl[0]_midout chanx[1][0]_out[17]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[18] density = 0, probability=0.*****
Vchanx[1][0]_in[18] chanx[1][0]_in[18] 0 
+  0
**** Load for rr_node[156] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=19, type=4 *****
Xchan_chanx[1][0]_out[19]_loadlvl[0]_out chanx[1][0]_out[19] chanx[1][0]_out[19]_loadlvl[0]_out chanx[1][0]_out[19]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[37]_no0 chanx[1][0]_out[19]_loadlvl[0]_out chanx[1][0]_out[19]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[38]_no0 chanx[1][0]_out[19]_loadlvl[0]_midout chanx[1][0]_out[19]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[20] density = 0, probability=0.*****
Vchanx[1][0]_in[20] chanx[1][0]_in[20] 0 
+  0
**** Load for rr_node[158] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=21, type=4 *****
Xchan_chanx[1][0]_out[21]_loadlvl[0]_out chanx[1][0]_out[21] chanx[1][0]_out[21]_loadlvl[0]_out chanx[1][0]_out[21]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[39]_no0 chanx[1][0]_out[21]_loadlvl[0]_out chanx[1][0]_out[21]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[40]_no0 chanx[1][0]_out[21]_loadlvl[0]_midout chanx[1][0]_out[21]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[22] density = 0, probability=0.*****
Vchanx[1][0]_in[22] chanx[1][0]_in[22] 0 
+  0
**** Load for rr_node[160] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=23, type=4 *****
Xchan_chanx[1][0]_out[23]_loadlvl[0]_out chanx[1][0]_out[23] chanx[1][0]_out[23]_loadlvl[0]_out chanx[1][0]_out[23]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[41]_no0 chanx[1][0]_out[23]_loadlvl[0]_out chanx[1][0]_out[23]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[42]_no0 chanx[1][0]_out[23]_loadlvl[0]_midout chanx[1][0]_out[23]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[43]_no0 chanx[1][0]_out[23]_loadlvl[0]_midout chanx[1][0]_out[23]_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[24] density = 0, probability=0.*****
Vchanx[1][0]_in[24] chanx[1][0]_in[24] 0 
+  0
**** Load for rr_node[162] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=25, type=4 *****
Xchan_chanx[1][0]_out[25]_loadlvl[0]_out chanx[1][0]_out[25] chanx[1][0]_out[25]_loadlvl[0]_out chanx[1][0]_out[25]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[44]_no0 chanx[1][0]_out[25]_loadlvl[0]_out chanx[1][0]_out[25]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[45]_no0 chanx[1][0]_out[25]_loadlvl[0]_midout chanx[1][0]_out[25]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[26] density = 0, probability=0.*****
Vchanx[1][0]_in[26] chanx[1][0]_in[26] 0 
+  0
**** Load for rr_node[164] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=27, type=4 *****
Xchan_chanx[1][0]_out[27]_loadlvl[0]_out chanx[1][0]_out[27] chanx[1][0]_out[27]_loadlvl[0]_out chanx[1][0]_out[27]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[46]_no0 chanx[1][0]_out[27]_loadlvl[0]_out chanx[1][0]_out[27]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[47]_no0 chanx[1][0]_out[27]_loadlvl[0]_midout chanx[1][0]_out[27]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
***** Signal chanx[1][0]_in[28] density = 0, probability=0.*****
Vchanx[1][0]_in[28] chanx[1][0]_in[28] 0 
+  0
**** Load for rr_node[166] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=29, type=4 *****
Xchan_chanx[1][0]_out[29]_loadlvl[0]_out chanx[1][0]_out[29] chanx[1][0]_out[29]_loadlvl[0]_out chanx[1][0]_out[29]_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[48]_no0 chanx[1][0]_out[29]_loadlvl[0]_out chanx[1][0]_out[29]_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[49]_no0 chanx[1][0]_out[29]_loadlvl[0]_midout chanx[1][0]_out[29]_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Vgrid[1][0]_pin[0][0][1] grid[1][0]_pin[0][0][1] 0 
+  0
Vgrid[1][0]_pin[0][0][3] grid[1][0]_pin[0][0][3] 0 
+  0
Vgrid[1][0]_pin[0][0][5] grid[1][0]_pin[0][0][5] 0 
+  0
Vgrid[1][0]_pin[0][0][7] grid[1][0]_pin[0][0][7] 0 
+  0
Vgrid[1][0]_pin[0][0][9] grid[1][0]_pin[0][0][9] 0 
+  0
Vgrid[1][0]_pin[0][0][11] grid[1][0]_pin[0][0][11] 0 
+  0
Vgrid[1][0]_pin[0][0][13] grid[1][0]_pin[0][0][13] 0 
+  0
Vgrid[1][0]_pin[0][0][15] grid[1][0]_pin[0][0][15] 0 
+  0

***** Voltage supplies *****
Vgvdd_sb[1][0] gvdd_sb[1][0] 0 vsp
Vgvdd_sram_sbs gvdd_sram_sbs 0 vsp
***** 2 Clock Simulation, accuracy=1e-13 *****
.tran 1e-13  '2*clock_period'
***** Generic Measurements for Circuit Parameters *****
***** Measurements *****
***** Leakage Power Measurement *****
.meas tran leakage_power_sb avg p(Vgvdd_sb[1][0]) from=0 to='clock_period'
.meas tran leakage_power_sram_sb avg p(Vgvdd_sram_sbs) from=0 to='clock_period'
***** Dynamic Power Measurement *****
.meas tran dynamic_power_sb avg p(Vgvdd_sb[1][0]) from='clock_period' to='2*clock_period'
.meas tran energy_per_cycle_sb param='dynamic_power_sb*clock_period'
.meas tran dynamic_power_sram_sb avg p(Vgvdd_sram_sbs) from='clock_period' to='2*clock_period'
.meas tran energy_per_cycle_sram_sb param='dynamic_power_sram_sb*clock_period'
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
