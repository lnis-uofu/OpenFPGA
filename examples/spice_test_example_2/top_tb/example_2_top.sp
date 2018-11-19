*****************************
*     FPGA SPICE Netlist    *
* Description: FPGA SPICE Netlist for Design: example_2 *
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
****** Include subckt netlists: Logic Blocks *****
.include './spice_test_example_2/subckt/grid_header.sp'
****** Include subckt netlists: Routing structures (Switch Boxes, Channels, Connection Boxes) *****
.include './spice_test_example_2/subckt/routing_header.sp'
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
.global gvdd_sram_cbs
.global gvdd_sram_sbs
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
.global 
+ gvdd_iopad[0]
+ gvdd_iopad[1]
+ gvdd_iopad[2]
+ gvdd_iopad[3]
+ gvdd_iopad[4]
+ gvdd_iopad[5]
+ gvdd_iopad[6]
+ gvdd_iopad[7]
+ gvdd_iopad[8]
+ gvdd_iopad[9]
+ gvdd_iopad[10]
+ gvdd_iopad[11]
+ gvdd_iopad[12]
+ gvdd_iopad[13]
+ gvdd_iopad[14]
+ gvdd_iopad[15]
+ gvdd_iopad[16]
+ gvdd_iopad[17]
+ gvdd_iopad[18]
+ gvdd_iopad[19]
+ gvdd_iopad[20]
+ gvdd_iopad[21]
+ gvdd_iopad[22]
+ gvdd_iopad[23]
+ gvdd_iopad[24]
+ gvdd_iopad[25]
+ gvdd_iopad[26]
+ gvdd_iopad[27]
+ gvdd_iopad[28]
+ gvdd_iopad[29]
+ gvdd_iopad[30]
+ gvdd_iopad[31]

***** Global VDD ports of hard_logic *****
.global 

***** Global Vdds for Switch Boxes *****
.global gvdd_sb[0][0] gvdd_sb[0][1] gvdd_sb[1][0] gvdd_sb[1][1] 
***** Global Vdds for Connection Blocks - X channels *****
.global gvdd_cbx[1][0] gvdd_cbx[1][1] 
***** Global Vdds for Connection Blocks - Y channels *****
.global gvdd_cby[0][1] gvdd_cby[1][1] 
***** Global input/output ports of I/O Pads *****
.global 
+ gfpga_pad_iopad[0]
+ gfpga_pad_iopad[1]
+ gfpga_pad_iopad[2]
+ gfpga_pad_iopad[3]
+ gfpga_pad_iopad[4]
+ gfpga_pad_iopad[5]
+ gfpga_pad_iopad[6]
+ gfpga_pad_iopad[7]
+ gfpga_pad_iopad[8]
+ gfpga_pad_iopad[9]
+ gfpga_pad_iopad[10]
+ gfpga_pad_iopad[11]
+ gfpga_pad_iopad[12]
+ gfpga_pad_iopad[13]
+ gfpga_pad_iopad[14]
+ gfpga_pad_iopad[15]
+ gfpga_pad_iopad[16]
+ gfpga_pad_iopad[17]
+ gfpga_pad_iopad[18]
+ gfpga_pad_iopad[19]
+ gfpga_pad_iopad[20]
+ gfpga_pad_iopad[21]
+ gfpga_pad_iopad[22]
+ gfpga_pad_iopad[23]
+ gfpga_pad_iopad[24]
+ gfpga_pad_iopad[25]
+ gfpga_pad_iopad[26]
+ gfpga_pad_iopad[27]
+ gfpga_pad_iopad[28]
+ gfpga_pad_iopad[29]
+ gfpga_pad_iopad[30]
+ gfpga_pad_iopad[31]

***** Link Blif Benchmark inputs to FPGA IOPADs *****
***** Blif Benchmark inout I0 is mapped to FPGA IOPAD gfpga_pad_[17] *****
RI0_gfpga_pad_[17] I0_gfpga_pad_[17]  gfpga_pad_iopad[17]  0
***** Blif Benchmark inout clk is mapped to FPGA IOPAD gfpga_pad_[1] *****
Rclk_gfpga_pad_[1] clk_gfpga_pad_[1]  gfpga_pad_iopad[1]  0
***** Blif Benchmark inout out_Q0 is mapped to FPGA IOPAD gfpga_pad_[30] *****
Rout_Q0_gfpga_pad_[30] out_Q0_gfpga_pad_[30]  gfpga_pad_iopad[30]  0
.temp 25
.option fast
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
Xgrid[0][1] 
+ grid[0][1]_pin[0][1][0] 
+ grid[0][1]_pin[0][1][1] 
+ grid[0][1]_pin[0][1][2] 
+ grid[0][1]_pin[0][1][3] 
+ grid[0][1]_pin[0][1][4] 
+ grid[0][1]_pin[0][1][5] 
+ grid[0][1]_pin[0][1][6] 
+ grid[0][1]_pin[0][1][7] 
+ grid[0][1]_pin[0][1][8] 
+ grid[0][1]_pin[0][1][9] 
+ grid[0][1]_pin[0][1][10] 
+ grid[0][1]_pin[0][1][11] 
+ grid[0][1]_pin[0][1][12] 
+ grid[0][1]_pin[0][1][13] 
+ grid[0][1]_pin[0][1][14] 
+ grid[0][1]_pin[0][1][15] 
+ gvdd_io 0 grid[0][1]
Xgrid[2][1] 
+ grid[2][1]_pin[0][3][0] 
+ grid[2][1]_pin[0][3][1] 
+ grid[2][1]_pin[0][3][2] 
+ grid[2][1]_pin[0][3][3] 
+ grid[2][1]_pin[0][3][4] 
+ grid[2][1]_pin[0][3][5] 
+ grid[2][1]_pin[0][3][6] 
+ grid[2][1]_pin[0][3][7] 
+ grid[2][1]_pin[0][3][8] 
+ grid[2][1]_pin[0][3][9] 
+ grid[2][1]_pin[0][3][10] 
+ grid[2][1]_pin[0][3][11] 
+ grid[2][1]_pin[0][3][12] 
+ grid[2][1]_pin[0][3][13] 
+ grid[2][1]_pin[0][3][14] 
+ grid[2][1]_pin[0][3][15] 
+ gvdd_io 0 grid[2][1]
Xgrid[1][0] 
+ grid[1][0]_pin[0][0][0] 
+ grid[1][0]_pin[0][0][1] 
+ grid[1][0]_pin[0][0][2] 
+ grid[1][0]_pin[0][0][3] 
+ grid[1][0]_pin[0][0][4] 
+ grid[1][0]_pin[0][0][5] 
+ grid[1][0]_pin[0][0][6] 
+ grid[1][0]_pin[0][0][7] 
+ grid[1][0]_pin[0][0][8] 
+ grid[1][0]_pin[0][0][9] 
+ grid[1][0]_pin[0][0][10] 
+ grid[1][0]_pin[0][0][11] 
+ grid[1][0]_pin[0][0][12] 
+ grid[1][0]_pin[0][0][13] 
+ grid[1][0]_pin[0][0][14] 
+ grid[1][0]_pin[0][0][15] 
+ gvdd_io 0 grid[1][0]
Xgrid[1][2] 
+ grid[1][2]_pin[0][2][0] 
+ grid[1][2]_pin[0][2][1] 
+ grid[1][2]_pin[0][2][2] 
+ grid[1][2]_pin[0][2][3] 
+ grid[1][2]_pin[0][2][4] 
+ grid[1][2]_pin[0][2][5] 
+ grid[1][2]_pin[0][2][6] 
+ grid[1][2]_pin[0][2][7] 
+ grid[1][2]_pin[0][2][8] 
+ grid[1][2]_pin[0][2][9] 
+ grid[1][2]_pin[0][2][10] 
+ grid[1][2]_pin[0][2][11] 
+ grid[1][2]_pin[0][2][12] 
+ grid[1][2]_pin[0][2][13] 
+ grid[1][2]_pin[0][2][14] 
+ grid[1][2]_pin[0][2][15] 
+ gvdd_io 0 grid[1][2]
Rdangling_grid[1][1]_pin[0][2][50] grid[1][1]_pin[0][2][50] 0 0
.nodeset V(grid[1][1]_pin[0][2][50]) 0
Xchanx[1][0] 
+ chanx[1][0]_out[0] 
+ chanx[1][0]_in[1] 
+ chanx[1][0]_out[2] 
+ chanx[1][0]_in[3] 
+ chanx[1][0]_out[4] 
+ chanx[1][0]_in[5] 
+ chanx[1][0]_out[6] 
+ chanx[1][0]_in[7] 
+ chanx[1][0]_out[8] 
+ chanx[1][0]_in[9] 
+ chanx[1][0]_out[10] 
+ chanx[1][0]_in[11] 
+ chanx[1][0]_out[12] 
+ chanx[1][0]_in[13] 
+ chanx[1][0]_out[14] 
+ chanx[1][0]_in[15] 
+ chanx[1][0]_out[16] 
+ chanx[1][0]_in[17] 
+ chanx[1][0]_out[18] 
+ chanx[1][0]_in[19] 
+ chanx[1][0]_out[20] 
+ chanx[1][0]_in[21] 
+ chanx[1][0]_out[22] 
+ chanx[1][0]_in[23] 
+ chanx[1][0]_out[24] 
+ chanx[1][0]_in[25] 
+ chanx[1][0]_out[26] 
+ chanx[1][0]_in[27] 
+ chanx[1][0]_out[28] 
+ chanx[1][0]_in[29] 
+ chanx[1][0]_out[30] 
+ chanx[1][0]_in[31] 
+ chanx[1][0]_out[32] 
+ chanx[1][0]_in[33] 
+ chanx[1][0]_out[34] 
+ chanx[1][0]_in[35] 
+ chanx[1][0]_out[36] 
+ chanx[1][0]_in[37] 
+ chanx[1][0]_out[38] 
+ chanx[1][0]_in[39] 
+ chanx[1][0]_out[40] 
+ chanx[1][0]_in[41] 
+ chanx[1][0]_out[42] 
+ chanx[1][0]_in[43] 
+ chanx[1][0]_out[44] 
+ chanx[1][0]_in[45] 
+ chanx[1][0]_out[46] 
+ chanx[1][0]_in[47] 
+ chanx[1][0]_out[48] 
+ chanx[1][0]_in[49] 
+ chanx[1][0]_out[50] 
+ chanx[1][0]_in[51] 
+ chanx[1][0]_out[52] 
+ chanx[1][0]_in[53] 
+ chanx[1][0]_out[54] 
+ chanx[1][0]_in[55] 
+ chanx[1][0]_out[56] 
+ chanx[1][0]_in[57] 
+ chanx[1][0]_out[58] 
+ chanx[1][0]_in[59] 
+ chanx[1][0]_out[60] 
+ chanx[1][0]_in[61] 
+ chanx[1][0]_out[62] 
+ chanx[1][0]_in[63] 
+ chanx[1][0]_out[64] 
+ chanx[1][0]_in[65] 
+ chanx[1][0]_out[66] 
+ chanx[1][0]_in[67] 
+ chanx[1][0]_out[68] 
+ chanx[1][0]_in[69] 
+ chanx[1][0]_out[70] 
+ chanx[1][0]_in[71] 
+ chanx[1][0]_out[72] 
+ chanx[1][0]_in[73] 
+ chanx[1][0]_out[74] 
+ chanx[1][0]_in[75] 
+ chanx[1][0]_out[76] 
+ chanx[1][0]_in[77] 
+ chanx[1][0]_out[78] 
+ chanx[1][0]_in[79] 
+ chanx[1][0]_out[80] 
+ chanx[1][0]_in[81] 
+ chanx[1][0]_out[82] 
+ chanx[1][0]_in[83] 
+ chanx[1][0]_out[84] 
+ chanx[1][0]_in[85] 
+ chanx[1][0]_out[86] 
+ chanx[1][0]_in[87] 
+ chanx[1][0]_out[88] 
+ chanx[1][0]_in[89] 
+ chanx[1][0]_out[90] 
+ chanx[1][0]_in[91] 
+ chanx[1][0]_out[92] 
+ chanx[1][0]_in[93] 
+ chanx[1][0]_out[94] 
+ chanx[1][0]_in[95] 
+ chanx[1][0]_out[96] 
+ chanx[1][0]_in[97] 
+ chanx[1][0]_out[98] 
+ chanx[1][0]_in[99] 
+ chanx[1][0]_in[0] 
+ chanx[1][0]_out[1] 
+ chanx[1][0]_in[2] 
+ chanx[1][0]_out[3] 
+ chanx[1][0]_in[4] 
+ chanx[1][0]_out[5] 
+ chanx[1][0]_in[6] 
+ chanx[1][0]_out[7] 
+ chanx[1][0]_in[8] 
+ chanx[1][0]_out[9] 
+ chanx[1][0]_in[10] 
+ chanx[1][0]_out[11] 
+ chanx[1][0]_in[12] 
+ chanx[1][0]_out[13] 
+ chanx[1][0]_in[14] 
+ chanx[1][0]_out[15] 
+ chanx[1][0]_in[16] 
+ chanx[1][0]_out[17] 
+ chanx[1][0]_in[18] 
+ chanx[1][0]_out[19] 
+ chanx[1][0]_in[20] 
+ chanx[1][0]_out[21] 
+ chanx[1][0]_in[22] 
+ chanx[1][0]_out[23] 
+ chanx[1][0]_in[24] 
+ chanx[1][0]_out[25] 
+ chanx[1][0]_in[26] 
+ chanx[1][0]_out[27] 
+ chanx[1][0]_in[28] 
+ chanx[1][0]_out[29] 
+ chanx[1][0]_in[30] 
+ chanx[1][0]_out[31] 
+ chanx[1][0]_in[32] 
+ chanx[1][0]_out[33] 
+ chanx[1][0]_in[34] 
+ chanx[1][0]_out[35] 
+ chanx[1][0]_in[36] 
+ chanx[1][0]_out[37] 
+ chanx[1][0]_in[38] 
+ chanx[1][0]_out[39] 
+ chanx[1][0]_in[40] 
+ chanx[1][0]_out[41] 
+ chanx[1][0]_in[42] 
+ chanx[1][0]_out[43] 
+ chanx[1][0]_in[44] 
+ chanx[1][0]_out[45] 
+ chanx[1][0]_in[46] 
+ chanx[1][0]_out[47] 
+ chanx[1][0]_in[48] 
+ chanx[1][0]_out[49] 
+ chanx[1][0]_in[50] 
+ chanx[1][0]_out[51] 
+ chanx[1][0]_in[52] 
+ chanx[1][0]_out[53] 
+ chanx[1][0]_in[54] 
+ chanx[1][0]_out[55] 
+ chanx[1][0]_in[56] 
+ chanx[1][0]_out[57] 
+ chanx[1][0]_in[58] 
+ chanx[1][0]_out[59] 
+ chanx[1][0]_in[60] 
+ chanx[1][0]_out[61] 
+ chanx[1][0]_in[62] 
+ chanx[1][0]_out[63] 
+ chanx[1][0]_in[64] 
+ chanx[1][0]_out[65] 
+ chanx[1][0]_in[66] 
+ chanx[1][0]_out[67] 
+ chanx[1][0]_in[68] 
+ chanx[1][0]_out[69] 
+ chanx[1][0]_in[70] 
+ chanx[1][0]_out[71] 
+ chanx[1][0]_in[72] 
+ chanx[1][0]_out[73] 
+ chanx[1][0]_in[74] 
+ chanx[1][0]_out[75] 
+ chanx[1][0]_in[76] 
+ chanx[1][0]_out[77] 
+ chanx[1][0]_in[78] 
+ chanx[1][0]_out[79] 
+ chanx[1][0]_in[80] 
+ chanx[1][0]_out[81] 
+ chanx[1][0]_in[82] 
+ chanx[1][0]_out[83] 
+ chanx[1][0]_in[84] 
+ chanx[1][0]_out[85] 
+ chanx[1][0]_in[86] 
+ chanx[1][0]_out[87] 
+ chanx[1][0]_in[88] 
+ chanx[1][0]_out[89] 
+ chanx[1][0]_in[90] 
+ chanx[1][0]_out[91] 
+ chanx[1][0]_in[92] 
+ chanx[1][0]_out[93] 
+ chanx[1][0]_in[94] 
+ chanx[1][0]_out[95] 
+ chanx[1][0]_in[96] 
+ chanx[1][0]_out[97] 
+ chanx[1][0]_in[98] 
+ chanx[1][0]_out[99] 
+ chanx[1][0]_midout[0] 
+ chanx[1][0]_midout[1] 
+ chanx[1][0]_midout[2] 
+ chanx[1][0]_midout[3] 
+ chanx[1][0]_midout[4] 
+ chanx[1][0]_midout[5] 
+ chanx[1][0]_midout[6] 
+ chanx[1][0]_midout[7] 
+ chanx[1][0]_midout[8] 
+ chanx[1][0]_midout[9] 
+ chanx[1][0]_midout[10] 
+ chanx[1][0]_midout[11] 
+ chanx[1][0]_midout[12] 
+ chanx[1][0]_midout[13] 
+ chanx[1][0]_midout[14] 
+ chanx[1][0]_midout[15] 
+ chanx[1][0]_midout[16] 
+ chanx[1][0]_midout[17] 
+ chanx[1][0]_midout[18] 
+ chanx[1][0]_midout[19] 
+ chanx[1][0]_midout[20] 
+ chanx[1][0]_midout[21] 
+ chanx[1][0]_midout[22] 
+ chanx[1][0]_midout[23] 
+ chanx[1][0]_midout[24] 
+ chanx[1][0]_midout[25] 
+ chanx[1][0]_midout[26] 
+ chanx[1][0]_midout[27] 
+ chanx[1][0]_midout[28] 
+ chanx[1][0]_midout[29] 
+ chanx[1][0]_midout[30] 
+ chanx[1][0]_midout[31] 
+ chanx[1][0]_midout[32] 
+ chanx[1][0]_midout[33] 
+ chanx[1][0]_midout[34] 
+ chanx[1][0]_midout[35] 
+ chanx[1][0]_midout[36] 
+ chanx[1][0]_midout[37] 
+ chanx[1][0]_midout[38] 
+ chanx[1][0]_midout[39] 
+ chanx[1][0]_midout[40] 
+ chanx[1][0]_midout[41] 
+ chanx[1][0]_midout[42] 
+ chanx[1][0]_midout[43] 
+ chanx[1][0]_midout[44] 
+ chanx[1][0]_midout[45] 
+ chanx[1][0]_midout[46] 
+ chanx[1][0]_midout[47] 
+ chanx[1][0]_midout[48] 
+ chanx[1][0]_midout[49] 
+ chanx[1][0]_midout[50] 
+ chanx[1][0]_midout[51] 
+ chanx[1][0]_midout[52] 
+ chanx[1][0]_midout[53] 
+ chanx[1][0]_midout[54] 
+ chanx[1][0]_midout[55] 
+ chanx[1][0]_midout[56] 
+ chanx[1][0]_midout[57] 
+ chanx[1][0]_midout[58] 
+ chanx[1][0]_midout[59] 
+ chanx[1][0]_midout[60] 
+ chanx[1][0]_midout[61] 
+ chanx[1][0]_midout[62] 
+ chanx[1][0]_midout[63] 
+ chanx[1][0]_midout[64] 
+ chanx[1][0]_midout[65] 
+ chanx[1][0]_midout[66] 
+ chanx[1][0]_midout[67] 
+ chanx[1][0]_midout[68] 
+ chanx[1][0]_midout[69] 
+ chanx[1][0]_midout[70] 
+ chanx[1][0]_midout[71] 
+ chanx[1][0]_midout[72] 
+ chanx[1][0]_midout[73] 
+ chanx[1][0]_midout[74] 
+ chanx[1][0]_midout[75] 
+ chanx[1][0]_midout[76] 
+ chanx[1][0]_midout[77] 
+ chanx[1][0]_midout[78] 
+ chanx[1][0]_midout[79] 
+ chanx[1][0]_midout[80] 
+ chanx[1][0]_midout[81] 
+ chanx[1][0]_midout[82] 
+ chanx[1][0]_midout[83] 
+ chanx[1][0]_midout[84] 
+ chanx[1][0]_midout[85] 
+ chanx[1][0]_midout[86] 
+ chanx[1][0]_midout[87] 
+ chanx[1][0]_midout[88] 
+ chanx[1][0]_midout[89] 
+ chanx[1][0]_midout[90] 
+ chanx[1][0]_midout[91] 
+ chanx[1][0]_midout[92] 
+ chanx[1][0]_midout[93] 
+ chanx[1][0]_midout[94] 
+ chanx[1][0]_midout[95] 
+ chanx[1][0]_midout[96] 
+ chanx[1][0]_midout[97] 
+ chanx[1][0]_midout[98] 
+ chanx[1][0]_midout[99] 
+ gvdd 0 chanx[1][0]
Xchanx[1][1] 
+ chanx[1][1]_out[0] 
+ chanx[1][1]_in[1] 
+ chanx[1][1]_out[2] 
+ chanx[1][1]_in[3] 
+ chanx[1][1]_out[4] 
+ chanx[1][1]_in[5] 
+ chanx[1][1]_out[6] 
+ chanx[1][1]_in[7] 
+ chanx[1][1]_out[8] 
+ chanx[1][1]_in[9] 
+ chanx[1][1]_out[10] 
+ chanx[1][1]_in[11] 
+ chanx[1][1]_out[12] 
+ chanx[1][1]_in[13] 
+ chanx[1][1]_out[14] 
+ chanx[1][1]_in[15] 
+ chanx[1][1]_out[16] 
+ chanx[1][1]_in[17] 
+ chanx[1][1]_out[18] 
+ chanx[1][1]_in[19] 
+ chanx[1][1]_out[20] 
+ chanx[1][1]_in[21] 
+ chanx[1][1]_out[22] 
+ chanx[1][1]_in[23] 
+ chanx[1][1]_out[24] 
+ chanx[1][1]_in[25] 
+ chanx[1][1]_out[26] 
+ chanx[1][1]_in[27] 
+ chanx[1][1]_out[28] 
+ chanx[1][1]_in[29] 
+ chanx[1][1]_out[30] 
+ chanx[1][1]_in[31] 
+ chanx[1][1]_out[32] 
+ chanx[1][1]_in[33] 
+ chanx[1][1]_out[34] 
+ chanx[1][1]_in[35] 
+ chanx[1][1]_out[36] 
+ chanx[1][1]_in[37] 
+ chanx[1][1]_out[38] 
+ chanx[1][1]_in[39] 
+ chanx[1][1]_out[40] 
+ chanx[1][1]_in[41] 
+ chanx[1][1]_out[42] 
+ chanx[1][1]_in[43] 
+ chanx[1][1]_out[44] 
+ chanx[1][1]_in[45] 
+ chanx[1][1]_out[46] 
+ chanx[1][1]_in[47] 
+ chanx[1][1]_out[48] 
+ chanx[1][1]_in[49] 
+ chanx[1][1]_out[50] 
+ chanx[1][1]_in[51] 
+ chanx[1][1]_out[52] 
+ chanx[1][1]_in[53] 
+ chanx[1][1]_out[54] 
+ chanx[1][1]_in[55] 
+ chanx[1][1]_out[56] 
+ chanx[1][1]_in[57] 
+ chanx[1][1]_out[58] 
+ chanx[1][1]_in[59] 
+ chanx[1][1]_out[60] 
+ chanx[1][1]_in[61] 
+ chanx[1][1]_out[62] 
+ chanx[1][1]_in[63] 
+ chanx[1][1]_out[64] 
+ chanx[1][1]_in[65] 
+ chanx[1][1]_out[66] 
+ chanx[1][1]_in[67] 
+ chanx[1][1]_out[68] 
+ chanx[1][1]_in[69] 
+ chanx[1][1]_out[70] 
+ chanx[1][1]_in[71] 
+ chanx[1][1]_out[72] 
+ chanx[1][1]_in[73] 
+ chanx[1][1]_out[74] 
+ chanx[1][1]_in[75] 
+ chanx[1][1]_out[76] 
+ chanx[1][1]_in[77] 
+ chanx[1][1]_out[78] 
+ chanx[1][1]_in[79] 
+ chanx[1][1]_out[80] 
+ chanx[1][1]_in[81] 
+ chanx[1][1]_out[82] 
+ chanx[1][1]_in[83] 
+ chanx[1][1]_out[84] 
+ chanx[1][1]_in[85] 
+ chanx[1][1]_out[86] 
+ chanx[1][1]_in[87] 
+ chanx[1][1]_out[88] 
+ chanx[1][1]_in[89] 
+ chanx[1][1]_out[90] 
+ chanx[1][1]_in[91] 
+ chanx[1][1]_out[92] 
+ chanx[1][1]_in[93] 
+ chanx[1][1]_out[94] 
+ chanx[1][1]_in[95] 
+ chanx[1][1]_out[96] 
+ chanx[1][1]_in[97] 
+ chanx[1][1]_out[98] 
+ chanx[1][1]_in[99] 
+ chanx[1][1]_in[0] 
+ chanx[1][1]_out[1] 
+ chanx[1][1]_in[2] 
+ chanx[1][1]_out[3] 
+ chanx[1][1]_in[4] 
+ chanx[1][1]_out[5] 
+ chanx[1][1]_in[6] 
+ chanx[1][1]_out[7] 
+ chanx[1][1]_in[8] 
+ chanx[1][1]_out[9] 
+ chanx[1][1]_in[10] 
+ chanx[1][1]_out[11] 
+ chanx[1][1]_in[12] 
+ chanx[1][1]_out[13] 
+ chanx[1][1]_in[14] 
+ chanx[1][1]_out[15] 
+ chanx[1][1]_in[16] 
+ chanx[1][1]_out[17] 
+ chanx[1][1]_in[18] 
+ chanx[1][1]_out[19] 
+ chanx[1][1]_in[20] 
+ chanx[1][1]_out[21] 
+ chanx[1][1]_in[22] 
+ chanx[1][1]_out[23] 
+ chanx[1][1]_in[24] 
+ chanx[1][1]_out[25] 
+ chanx[1][1]_in[26] 
+ chanx[1][1]_out[27] 
+ chanx[1][1]_in[28] 
+ chanx[1][1]_out[29] 
+ chanx[1][1]_in[30] 
+ chanx[1][1]_out[31] 
+ chanx[1][1]_in[32] 
+ chanx[1][1]_out[33] 
+ chanx[1][1]_in[34] 
+ chanx[1][1]_out[35] 
+ chanx[1][1]_in[36] 
+ chanx[1][1]_out[37] 
+ chanx[1][1]_in[38] 
+ chanx[1][1]_out[39] 
+ chanx[1][1]_in[40] 
+ chanx[1][1]_out[41] 
+ chanx[1][1]_in[42] 
+ chanx[1][1]_out[43] 
+ chanx[1][1]_in[44] 
+ chanx[1][1]_out[45] 
+ chanx[1][1]_in[46] 
+ chanx[1][1]_out[47] 
+ chanx[1][1]_in[48] 
+ chanx[1][1]_out[49] 
+ chanx[1][1]_in[50] 
+ chanx[1][1]_out[51] 
+ chanx[1][1]_in[52] 
+ chanx[1][1]_out[53] 
+ chanx[1][1]_in[54] 
+ chanx[1][1]_out[55] 
+ chanx[1][1]_in[56] 
+ chanx[1][1]_out[57] 
+ chanx[1][1]_in[58] 
+ chanx[1][1]_out[59] 
+ chanx[1][1]_in[60] 
+ chanx[1][1]_out[61] 
+ chanx[1][1]_in[62] 
+ chanx[1][1]_out[63] 
+ chanx[1][1]_in[64] 
+ chanx[1][1]_out[65] 
+ chanx[1][1]_in[66] 
+ chanx[1][1]_out[67] 
+ chanx[1][1]_in[68] 
+ chanx[1][1]_out[69] 
+ chanx[1][1]_in[70] 
+ chanx[1][1]_out[71] 
+ chanx[1][1]_in[72] 
+ chanx[1][1]_out[73] 
+ chanx[1][1]_in[74] 
+ chanx[1][1]_out[75] 
+ chanx[1][1]_in[76] 
+ chanx[1][1]_out[77] 
+ chanx[1][1]_in[78] 
+ chanx[1][1]_out[79] 
+ chanx[1][1]_in[80] 
+ chanx[1][1]_out[81] 
+ chanx[1][1]_in[82] 
+ chanx[1][1]_out[83] 
+ chanx[1][1]_in[84] 
+ chanx[1][1]_out[85] 
+ chanx[1][1]_in[86] 
+ chanx[1][1]_out[87] 
+ chanx[1][1]_in[88] 
+ chanx[1][1]_out[89] 
+ chanx[1][1]_in[90] 
+ chanx[1][1]_out[91] 
+ chanx[1][1]_in[92] 
+ chanx[1][1]_out[93] 
+ chanx[1][1]_in[94] 
+ chanx[1][1]_out[95] 
+ chanx[1][1]_in[96] 
+ chanx[1][1]_out[97] 
+ chanx[1][1]_in[98] 
+ chanx[1][1]_out[99] 
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
+ gvdd 0 chanx[1][1]
Xchany[0][1] 
+ chany[0][1]_out[0] 
+ chany[0][1]_in[1] 
+ chany[0][1]_out[2] 
+ chany[0][1]_in[3] 
+ chany[0][1]_out[4] 
+ chany[0][1]_in[5] 
+ chany[0][1]_out[6] 
+ chany[0][1]_in[7] 
+ chany[0][1]_out[8] 
+ chany[0][1]_in[9] 
+ chany[0][1]_out[10] 
+ chany[0][1]_in[11] 
+ chany[0][1]_out[12] 
+ chany[0][1]_in[13] 
+ chany[0][1]_out[14] 
+ chany[0][1]_in[15] 
+ chany[0][1]_out[16] 
+ chany[0][1]_in[17] 
+ chany[0][1]_out[18] 
+ chany[0][1]_in[19] 
+ chany[0][1]_out[20] 
+ chany[0][1]_in[21] 
+ chany[0][1]_out[22] 
+ chany[0][1]_in[23] 
+ chany[0][1]_out[24] 
+ chany[0][1]_in[25] 
+ chany[0][1]_out[26] 
+ chany[0][1]_in[27] 
+ chany[0][1]_out[28] 
+ chany[0][1]_in[29] 
+ chany[0][1]_out[30] 
+ chany[0][1]_in[31] 
+ chany[0][1]_out[32] 
+ chany[0][1]_in[33] 
+ chany[0][1]_out[34] 
+ chany[0][1]_in[35] 
+ chany[0][1]_out[36] 
+ chany[0][1]_in[37] 
+ chany[0][1]_out[38] 
+ chany[0][1]_in[39] 
+ chany[0][1]_out[40] 
+ chany[0][1]_in[41] 
+ chany[0][1]_out[42] 
+ chany[0][1]_in[43] 
+ chany[0][1]_out[44] 
+ chany[0][1]_in[45] 
+ chany[0][1]_out[46] 
+ chany[0][1]_in[47] 
+ chany[0][1]_out[48] 
+ chany[0][1]_in[49] 
+ chany[0][1]_out[50] 
+ chany[0][1]_in[51] 
+ chany[0][1]_out[52] 
+ chany[0][1]_in[53] 
+ chany[0][1]_out[54] 
+ chany[0][1]_in[55] 
+ chany[0][1]_out[56] 
+ chany[0][1]_in[57] 
+ chany[0][1]_out[58] 
+ chany[0][1]_in[59] 
+ chany[0][1]_out[60] 
+ chany[0][1]_in[61] 
+ chany[0][1]_out[62] 
+ chany[0][1]_in[63] 
+ chany[0][1]_out[64] 
+ chany[0][1]_in[65] 
+ chany[0][1]_out[66] 
+ chany[0][1]_in[67] 
+ chany[0][1]_out[68] 
+ chany[0][1]_in[69] 
+ chany[0][1]_out[70] 
+ chany[0][1]_in[71] 
+ chany[0][1]_out[72] 
+ chany[0][1]_in[73] 
+ chany[0][1]_out[74] 
+ chany[0][1]_in[75] 
+ chany[0][1]_out[76] 
+ chany[0][1]_in[77] 
+ chany[0][1]_out[78] 
+ chany[0][1]_in[79] 
+ chany[0][1]_out[80] 
+ chany[0][1]_in[81] 
+ chany[0][1]_out[82] 
+ chany[0][1]_in[83] 
+ chany[0][1]_out[84] 
+ chany[0][1]_in[85] 
+ chany[0][1]_out[86] 
+ chany[0][1]_in[87] 
+ chany[0][1]_out[88] 
+ chany[0][1]_in[89] 
+ chany[0][1]_out[90] 
+ chany[0][1]_in[91] 
+ chany[0][1]_out[92] 
+ chany[0][1]_in[93] 
+ chany[0][1]_out[94] 
+ chany[0][1]_in[95] 
+ chany[0][1]_out[96] 
+ chany[0][1]_in[97] 
+ chany[0][1]_out[98] 
+ chany[0][1]_in[99] 
+ chany[0][1]_in[0] 
+ chany[0][1]_out[1] 
+ chany[0][1]_in[2] 
+ chany[0][1]_out[3] 
+ chany[0][1]_in[4] 
+ chany[0][1]_out[5] 
+ chany[0][1]_in[6] 
+ chany[0][1]_out[7] 
+ chany[0][1]_in[8] 
+ chany[0][1]_out[9] 
+ chany[0][1]_in[10] 
+ chany[0][1]_out[11] 
+ chany[0][1]_in[12] 
+ chany[0][1]_out[13] 
+ chany[0][1]_in[14] 
+ chany[0][1]_out[15] 
+ chany[0][1]_in[16] 
+ chany[0][1]_out[17] 
+ chany[0][1]_in[18] 
+ chany[0][1]_out[19] 
+ chany[0][1]_in[20] 
+ chany[0][1]_out[21] 
+ chany[0][1]_in[22] 
+ chany[0][1]_out[23] 
+ chany[0][1]_in[24] 
+ chany[0][1]_out[25] 
+ chany[0][1]_in[26] 
+ chany[0][1]_out[27] 
+ chany[0][1]_in[28] 
+ chany[0][1]_out[29] 
+ chany[0][1]_in[30] 
+ chany[0][1]_out[31] 
+ chany[0][1]_in[32] 
+ chany[0][1]_out[33] 
+ chany[0][1]_in[34] 
+ chany[0][1]_out[35] 
+ chany[0][1]_in[36] 
+ chany[0][1]_out[37] 
+ chany[0][1]_in[38] 
+ chany[0][1]_out[39] 
+ chany[0][1]_in[40] 
+ chany[0][1]_out[41] 
+ chany[0][1]_in[42] 
+ chany[0][1]_out[43] 
+ chany[0][1]_in[44] 
+ chany[0][1]_out[45] 
+ chany[0][1]_in[46] 
+ chany[0][1]_out[47] 
+ chany[0][1]_in[48] 
+ chany[0][1]_out[49] 
+ chany[0][1]_in[50] 
+ chany[0][1]_out[51] 
+ chany[0][1]_in[52] 
+ chany[0][1]_out[53] 
+ chany[0][1]_in[54] 
+ chany[0][1]_out[55] 
+ chany[0][1]_in[56] 
+ chany[0][1]_out[57] 
+ chany[0][1]_in[58] 
+ chany[0][1]_out[59] 
+ chany[0][1]_in[60] 
+ chany[0][1]_out[61] 
+ chany[0][1]_in[62] 
+ chany[0][1]_out[63] 
+ chany[0][1]_in[64] 
+ chany[0][1]_out[65] 
+ chany[0][1]_in[66] 
+ chany[0][1]_out[67] 
+ chany[0][1]_in[68] 
+ chany[0][1]_out[69] 
+ chany[0][1]_in[70] 
+ chany[0][1]_out[71] 
+ chany[0][1]_in[72] 
+ chany[0][1]_out[73] 
+ chany[0][1]_in[74] 
+ chany[0][1]_out[75] 
+ chany[0][1]_in[76] 
+ chany[0][1]_out[77] 
+ chany[0][1]_in[78] 
+ chany[0][1]_out[79] 
+ chany[0][1]_in[80] 
+ chany[0][1]_out[81] 
+ chany[0][1]_in[82] 
+ chany[0][1]_out[83] 
+ chany[0][1]_in[84] 
+ chany[0][1]_out[85] 
+ chany[0][1]_in[86] 
+ chany[0][1]_out[87] 
+ chany[0][1]_in[88] 
+ chany[0][1]_out[89] 
+ chany[0][1]_in[90] 
+ chany[0][1]_out[91] 
+ chany[0][1]_in[92] 
+ chany[0][1]_out[93] 
+ chany[0][1]_in[94] 
+ chany[0][1]_out[95] 
+ chany[0][1]_in[96] 
+ chany[0][1]_out[97] 
+ chany[0][1]_in[98] 
+ chany[0][1]_out[99] 
+ chany[0][1]_midout[0] 
+ chany[0][1]_midout[1] 
+ chany[0][1]_midout[2] 
+ chany[0][1]_midout[3] 
+ chany[0][1]_midout[4] 
+ chany[0][1]_midout[5] 
+ chany[0][1]_midout[6] 
+ chany[0][1]_midout[7] 
+ chany[0][1]_midout[8] 
+ chany[0][1]_midout[9] 
+ chany[0][1]_midout[10] 
+ chany[0][1]_midout[11] 
+ chany[0][1]_midout[12] 
+ chany[0][1]_midout[13] 
+ chany[0][1]_midout[14] 
+ chany[0][1]_midout[15] 
+ chany[0][1]_midout[16] 
+ chany[0][1]_midout[17] 
+ chany[0][1]_midout[18] 
+ chany[0][1]_midout[19] 
+ chany[0][1]_midout[20] 
+ chany[0][1]_midout[21] 
+ chany[0][1]_midout[22] 
+ chany[0][1]_midout[23] 
+ chany[0][1]_midout[24] 
+ chany[0][1]_midout[25] 
+ chany[0][1]_midout[26] 
+ chany[0][1]_midout[27] 
+ chany[0][1]_midout[28] 
+ chany[0][1]_midout[29] 
+ chany[0][1]_midout[30] 
+ chany[0][1]_midout[31] 
+ chany[0][1]_midout[32] 
+ chany[0][1]_midout[33] 
+ chany[0][1]_midout[34] 
+ chany[0][1]_midout[35] 
+ chany[0][1]_midout[36] 
+ chany[0][1]_midout[37] 
+ chany[0][1]_midout[38] 
+ chany[0][1]_midout[39] 
+ chany[0][1]_midout[40] 
+ chany[0][1]_midout[41] 
+ chany[0][1]_midout[42] 
+ chany[0][1]_midout[43] 
+ chany[0][1]_midout[44] 
+ chany[0][1]_midout[45] 
+ chany[0][1]_midout[46] 
+ chany[0][1]_midout[47] 
+ chany[0][1]_midout[48] 
+ chany[0][1]_midout[49] 
+ chany[0][1]_midout[50] 
+ chany[0][1]_midout[51] 
+ chany[0][1]_midout[52] 
+ chany[0][1]_midout[53] 
+ chany[0][1]_midout[54] 
+ chany[0][1]_midout[55] 
+ chany[0][1]_midout[56] 
+ chany[0][1]_midout[57] 
+ chany[0][1]_midout[58] 
+ chany[0][1]_midout[59] 
+ chany[0][1]_midout[60] 
+ chany[0][1]_midout[61] 
+ chany[0][1]_midout[62] 
+ chany[0][1]_midout[63] 
+ chany[0][1]_midout[64] 
+ chany[0][1]_midout[65] 
+ chany[0][1]_midout[66] 
+ chany[0][1]_midout[67] 
+ chany[0][1]_midout[68] 
+ chany[0][1]_midout[69] 
+ chany[0][1]_midout[70] 
+ chany[0][1]_midout[71] 
+ chany[0][1]_midout[72] 
+ chany[0][1]_midout[73] 
+ chany[0][1]_midout[74] 
+ chany[0][1]_midout[75] 
+ chany[0][1]_midout[76] 
+ chany[0][1]_midout[77] 
+ chany[0][1]_midout[78] 
+ chany[0][1]_midout[79] 
+ chany[0][1]_midout[80] 
+ chany[0][1]_midout[81] 
+ chany[0][1]_midout[82] 
+ chany[0][1]_midout[83] 
+ chany[0][1]_midout[84] 
+ chany[0][1]_midout[85] 
+ chany[0][1]_midout[86] 
+ chany[0][1]_midout[87] 
+ chany[0][1]_midout[88] 
+ chany[0][1]_midout[89] 
+ chany[0][1]_midout[90] 
+ chany[0][1]_midout[91] 
+ chany[0][1]_midout[92] 
+ chany[0][1]_midout[93] 
+ chany[0][1]_midout[94] 
+ chany[0][1]_midout[95] 
+ chany[0][1]_midout[96] 
+ chany[0][1]_midout[97] 
+ chany[0][1]_midout[98] 
+ chany[0][1]_midout[99] 
+ gvdd 0 chany[0][1]
Xchany[1][1] 
+ chany[1][1]_out[0] 
+ chany[1][1]_in[1] 
+ chany[1][1]_out[2] 
+ chany[1][1]_in[3] 
+ chany[1][1]_out[4] 
+ chany[1][1]_in[5] 
+ chany[1][1]_out[6] 
+ chany[1][1]_in[7] 
+ chany[1][1]_out[8] 
+ chany[1][1]_in[9] 
+ chany[1][1]_out[10] 
+ chany[1][1]_in[11] 
+ chany[1][1]_out[12] 
+ chany[1][1]_in[13] 
+ chany[1][1]_out[14] 
+ chany[1][1]_in[15] 
+ chany[1][1]_out[16] 
+ chany[1][1]_in[17] 
+ chany[1][1]_out[18] 
+ chany[1][1]_in[19] 
+ chany[1][1]_out[20] 
+ chany[1][1]_in[21] 
+ chany[1][1]_out[22] 
+ chany[1][1]_in[23] 
+ chany[1][1]_out[24] 
+ chany[1][1]_in[25] 
+ chany[1][1]_out[26] 
+ chany[1][1]_in[27] 
+ chany[1][1]_out[28] 
+ chany[1][1]_in[29] 
+ chany[1][1]_out[30] 
+ chany[1][1]_in[31] 
+ chany[1][1]_out[32] 
+ chany[1][1]_in[33] 
+ chany[1][1]_out[34] 
+ chany[1][1]_in[35] 
+ chany[1][1]_out[36] 
+ chany[1][1]_in[37] 
+ chany[1][1]_out[38] 
+ chany[1][1]_in[39] 
+ chany[1][1]_out[40] 
+ chany[1][1]_in[41] 
+ chany[1][1]_out[42] 
+ chany[1][1]_in[43] 
+ chany[1][1]_out[44] 
+ chany[1][1]_in[45] 
+ chany[1][1]_out[46] 
+ chany[1][1]_in[47] 
+ chany[1][1]_out[48] 
+ chany[1][1]_in[49] 
+ chany[1][1]_out[50] 
+ chany[1][1]_in[51] 
+ chany[1][1]_out[52] 
+ chany[1][1]_in[53] 
+ chany[1][1]_out[54] 
+ chany[1][1]_in[55] 
+ chany[1][1]_out[56] 
+ chany[1][1]_in[57] 
+ chany[1][1]_out[58] 
+ chany[1][1]_in[59] 
+ chany[1][1]_out[60] 
+ chany[1][1]_in[61] 
+ chany[1][1]_out[62] 
+ chany[1][1]_in[63] 
+ chany[1][1]_out[64] 
+ chany[1][1]_in[65] 
+ chany[1][1]_out[66] 
+ chany[1][1]_in[67] 
+ chany[1][1]_out[68] 
+ chany[1][1]_in[69] 
+ chany[1][1]_out[70] 
+ chany[1][1]_in[71] 
+ chany[1][1]_out[72] 
+ chany[1][1]_in[73] 
+ chany[1][1]_out[74] 
+ chany[1][1]_in[75] 
+ chany[1][1]_out[76] 
+ chany[1][1]_in[77] 
+ chany[1][1]_out[78] 
+ chany[1][1]_in[79] 
+ chany[1][1]_out[80] 
+ chany[1][1]_in[81] 
+ chany[1][1]_out[82] 
+ chany[1][1]_in[83] 
+ chany[1][1]_out[84] 
+ chany[1][1]_in[85] 
+ chany[1][1]_out[86] 
+ chany[1][1]_in[87] 
+ chany[1][1]_out[88] 
+ chany[1][1]_in[89] 
+ chany[1][1]_out[90] 
+ chany[1][1]_in[91] 
+ chany[1][1]_out[92] 
+ chany[1][1]_in[93] 
+ chany[1][1]_out[94] 
+ chany[1][1]_in[95] 
+ chany[1][1]_out[96] 
+ chany[1][1]_in[97] 
+ chany[1][1]_out[98] 
+ chany[1][1]_in[99] 
+ chany[1][1]_in[0] 
+ chany[1][1]_out[1] 
+ chany[1][1]_in[2] 
+ chany[1][1]_out[3] 
+ chany[1][1]_in[4] 
+ chany[1][1]_out[5] 
+ chany[1][1]_in[6] 
+ chany[1][1]_out[7] 
+ chany[1][1]_in[8] 
+ chany[1][1]_out[9] 
+ chany[1][1]_in[10] 
+ chany[1][1]_out[11] 
+ chany[1][1]_in[12] 
+ chany[1][1]_out[13] 
+ chany[1][1]_in[14] 
+ chany[1][1]_out[15] 
+ chany[1][1]_in[16] 
+ chany[1][1]_out[17] 
+ chany[1][1]_in[18] 
+ chany[1][1]_out[19] 
+ chany[1][1]_in[20] 
+ chany[1][1]_out[21] 
+ chany[1][1]_in[22] 
+ chany[1][1]_out[23] 
+ chany[1][1]_in[24] 
+ chany[1][1]_out[25] 
+ chany[1][1]_in[26] 
+ chany[1][1]_out[27] 
+ chany[1][1]_in[28] 
+ chany[1][1]_out[29] 
+ chany[1][1]_in[30] 
+ chany[1][1]_out[31] 
+ chany[1][1]_in[32] 
+ chany[1][1]_out[33] 
+ chany[1][1]_in[34] 
+ chany[1][1]_out[35] 
+ chany[1][1]_in[36] 
+ chany[1][1]_out[37] 
+ chany[1][1]_in[38] 
+ chany[1][1]_out[39] 
+ chany[1][1]_in[40] 
+ chany[1][1]_out[41] 
+ chany[1][1]_in[42] 
+ chany[1][1]_out[43] 
+ chany[1][1]_in[44] 
+ chany[1][1]_out[45] 
+ chany[1][1]_in[46] 
+ chany[1][1]_out[47] 
+ chany[1][1]_in[48] 
+ chany[1][1]_out[49] 
+ chany[1][1]_in[50] 
+ chany[1][1]_out[51] 
+ chany[1][1]_in[52] 
+ chany[1][1]_out[53] 
+ chany[1][1]_in[54] 
+ chany[1][1]_out[55] 
+ chany[1][1]_in[56] 
+ chany[1][1]_out[57] 
+ chany[1][1]_in[58] 
+ chany[1][1]_out[59] 
+ chany[1][1]_in[60] 
+ chany[1][1]_out[61] 
+ chany[1][1]_in[62] 
+ chany[1][1]_out[63] 
+ chany[1][1]_in[64] 
+ chany[1][1]_out[65] 
+ chany[1][1]_in[66] 
+ chany[1][1]_out[67] 
+ chany[1][1]_in[68] 
+ chany[1][1]_out[69] 
+ chany[1][1]_in[70] 
+ chany[1][1]_out[71] 
+ chany[1][1]_in[72] 
+ chany[1][1]_out[73] 
+ chany[1][1]_in[74] 
+ chany[1][1]_out[75] 
+ chany[1][1]_in[76] 
+ chany[1][1]_out[77] 
+ chany[1][1]_in[78] 
+ chany[1][1]_out[79] 
+ chany[1][1]_in[80] 
+ chany[1][1]_out[81] 
+ chany[1][1]_in[82] 
+ chany[1][1]_out[83] 
+ chany[1][1]_in[84] 
+ chany[1][1]_out[85] 
+ chany[1][1]_in[86] 
+ chany[1][1]_out[87] 
+ chany[1][1]_in[88] 
+ chany[1][1]_out[89] 
+ chany[1][1]_in[90] 
+ chany[1][1]_out[91] 
+ chany[1][1]_in[92] 
+ chany[1][1]_out[93] 
+ chany[1][1]_in[94] 
+ chany[1][1]_out[95] 
+ chany[1][1]_in[96] 
+ chany[1][1]_out[97] 
+ chany[1][1]_in[98] 
+ chany[1][1]_out[99] 
+ chany[1][1]_midout[0] 
+ chany[1][1]_midout[1] 
+ chany[1][1]_midout[2] 
+ chany[1][1]_midout[3] 
+ chany[1][1]_midout[4] 
+ chany[1][1]_midout[5] 
+ chany[1][1]_midout[6] 
+ chany[1][1]_midout[7] 
+ chany[1][1]_midout[8] 
+ chany[1][1]_midout[9] 
+ chany[1][1]_midout[10] 
+ chany[1][1]_midout[11] 
+ chany[1][1]_midout[12] 
+ chany[1][1]_midout[13] 
+ chany[1][1]_midout[14] 
+ chany[1][1]_midout[15] 
+ chany[1][1]_midout[16] 
+ chany[1][1]_midout[17] 
+ chany[1][1]_midout[18] 
+ chany[1][1]_midout[19] 
+ chany[1][1]_midout[20] 
+ chany[1][1]_midout[21] 
+ chany[1][1]_midout[22] 
+ chany[1][1]_midout[23] 
+ chany[1][1]_midout[24] 
+ chany[1][1]_midout[25] 
+ chany[1][1]_midout[26] 
+ chany[1][1]_midout[27] 
+ chany[1][1]_midout[28] 
+ chany[1][1]_midout[29] 
+ chany[1][1]_midout[30] 
+ chany[1][1]_midout[31] 
+ chany[1][1]_midout[32] 
+ chany[1][1]_midout[33] 
+ chany[1][1]_midout[34] 
+ chany[1][1]_midout[35] 
+ chany[1][1]_midout[36] 
+ chany[1][1]_midout[37] 
+ chany[1][1]_midout[38] 
+ chany[1][1]_midout[39] 
+ chany[1][1]_midout[40] 
+ chany[1][1]_midout[41] 
+ chany[1][1]_midout[42] 
+ chany[1][1]_midout[43] 
+ chany[1][1]_midout[44] 
+ chany[1][1]_midout[45] 
+ chany[1][1]_midout[46] 
+ chany[1][1]_midout[47] 
+ chany[1][1]_midout[48] 
+ chany[1][1]_midout[49] 
+ chany[1][1]_midout[50] 
+ chany[1][1]_midout[51] 
+ chany[1][1]_midout[52] 
+ chany[1][1]_midout[53] 
+ chany[1][1]_midout[54] 
+ chany[1][1]_midout[55] 
+ chany[1][1]_midout[56] 
+ chany[1][1]_midout[57] 
+ chany[1][1]_midout[58] 
+ chany[1][1]_midout[59] 
+ chany[1][1]_midout[60] 
+ chany[1][1]_midout[61] 
+ chany[1][1]_midout[62] 
+ chany[1][1]_midout[63] 
+ chany[1][1]_midout[64] 
+ chany[1][1]_midout[65] 
+ chany[1][1]_midout[66] 
+ chany[1][1]_midout[67] 
+ chany[1][1]_midout[68] 
+ chany[1][1]_midout[69] 
+ chany[1][1]_midout[70] 
+ chany[1][1]_midout[71] 
+ chany[1][1]_midout[72] 
+ chany[1][1]_midout[73] 
+ chany[1][1]_midout[74] 
+ chany[1][1]_midout[75] 
+ chany[1][1]_midout[76] 
+ chany[1][1]_midout[77] 
+ chany[1][1]_midout[78] 
+ chany[1][1]_midout[79] 
+ chany[1][1]_midout[80] 
+ chany[1][1]_midout[81] 
+ chany[1][1]_midout[82] 
+ chany[1][1]_midout[83] 
+ chany[1][1]_midout[84] 
+ chany[1][1]_midout[85] 
+ chany[1][1]_midout[86] 
+ chany[1][1]_midout[87] 
+ chany[1][1]_midout[88] 
+ chany[1][1]_midout[89] 
+ chany[1][1]_midout[90] 
+ chany[1][1]_midout[91] 
+ chany[1][1]_midout[92] 
+ chany[1][1]_midout[93] 
+ chany[1][1]_midout[94] 
+ chany[1][1]_midout[95] 
+ chany[1][1]_midout[96] 
+ chany[1][1]_midout[97] 
+ chany[1][1]_midout[98] 
+ chany[1][1]_midout[99] 
+ gvdd 0 chany[1][1]
Xcbx[1][0] 
+ chanx[1][0]_midout[0] 
+ chanx[1][0]_midout[1] 
+ chanx[1][0]_midout[2] 
+ chanx[1][0]_midout[3] 
+ chanx[1][0]_midout[4] 
+ chanx[1][0]_midout[5] 
+ chanx[1][0]_midout[6] 
+ chanx[1][0]_midout[7] 
+ chanx[1][0]_midout[8] 
+ chanx[1][0]_midout[9] 
+ chanx[1][0]_midout[10] 
+ chanx[1][0]_midout[11] 
+ chanx[1][0]_midout[12] 
+ chanx[1][0]_midout[13] 
+ chanx[1][0]_midout[14] 
+ chanx[1][0]_midout[15] 
+ chanx[1][0]_midout[16] 
+ chanx[1][0]_midout[17] 
+ chanx[1][0]_midout[18] 
+ chanx[1][0]_midout[19] 
+ chanx[1][0]_midout[20] 
+ chanx[1][0]_midout[21] 
+ chanx[1][0]_midout[22] 
+ chanx[1][0]_midout[23] 
+ chanx[1][0]_midout[24] 
+ chanx[1][0]_midout[25] 
+ chanx[1][0]_midout[26] 
+ chanx[1][0]_midout[27] 
+ chanx[1][0]_midout[28] 
+ chanx[1][0]_midout[29] 
+ chanx[1][0]_midout[30] 
+ chanx[1][0]_midout[31] 
+ chanx[1][0]_midout[32] 
+ chanx[1][0]_midout[33] 
+ chanx[1][0]_midout[34] 
+ chanx[1][0]_midout[35] 
+ chanx[1][0]_midout[36] 
+ chanx[1][0]_midout[37] 
+ chanx[1][0]_midout[38] 
+ chanx[1][0]_midout[39] 
+ chanx[1][0]_midout[40] 
+ chanx[1][0]_midout[41] 
+ chanx[1][0]_midout[42] 
+ chanx[1][0]_midout[43] 
+ chanx[1][0]_midout[44] 
+ chanx[1][0]_midout[45] 
+ chanx[1][0]_midout[46] 
+ chanx[1][0]_midout[47] 
+ chanx[1][0]_midout[48] 
+ chanx[1][0]_midout[49] 
+ chanx[1][0]_midout[50] 
+ chanx[1][0]_midout[51] 
+ chanx[1][0]_midout[52] 
+ chanx[1][0]_midout[53] 
+ chanx[1][0]_midout[54] 
+ chanx[1][0]_midout[55] 
+ chanx[1][0]_midout[56] 
+ chanx[1][0]_midout[57] 
+ chanx[1][0]_midout[58] 
+ chanx[1][0]_midout[59] 
+ chanx[1][0]_midout[60] 
+ chanx[1][0]_midout[61] 
+ chanx[1][0]_midout[62] 
+ chanx[1][0]_midout[63] 
+ chanx[1][0]_midout[64] 
+ chanx[1][0]_midout[65] 
+ chanx[1][0]_midout[66] 
+ chanx[1][0]_midout[67] 
+ chanx[1][0]_midout[68] 
+ chanx[1][0]_midout[69] 
+ chanx[1][0]_midout[70] 
+ chanx[1][0]_midout[71] 
+ chanx[1][0]_midout[72] 
+ chanx[1][0]_midout[73] 
+ chanx[1][0]_midout[74] 
+ chanx[1][0]_midout[75] 
+ chanx[1][0]_midout[76] 
+ chanx[1][0]_midout[77] 
+ chanx[1][0]_midout[78] 
+ chanx[1][0]_midout[79] 
+ chanx[1][0]_midout[80] 
+ chanx[1][0]_midout[81] 
+ chanx[1][0]_midout[82] 
+ chanx[1][0]_midout[83] 
+ chanx[1][0]_midout[84] 
+ chanx[1][0]_midout[85] 
+ chanx[1][0]_midout[86] 
+ chanx[1][0]_midout[87] 
+ chanx[1][0]_midout[88] 
+ chanx[1][0]_midout[89] 
+ chanx[1][0]_midout[90] 
+ chanx[1][0]_midout[91] 
+ chanx[1][0]_midout[92] 
+ chanx[1][0]_midout[93] 
+ chanx[1][0]_midout[94] 
+ chanx[1][0]_midout[95] 
+ chanx[1][0]_midout[96] 
+ chanx[1][0]_midout[97] 
+ chanx[1][0]_midout[98] 
+ chanx[1][0]_midout[99] 
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
+ grid[1][1]_pin[0][2][50] 
+ grid[1][0]_pin[0][0][0] 
+ grid[1][0]_pin[0][0][2] 
+ grid[1][0]_pin[0][0][4] 
+ grid[1][0]_pin[0][0][6] 
+ grid[1][0]_pin[0][0][8] 
+ grid[1][0]_pin[0][0][10] 
+ grid[1][0]_pin[0][0][12] 
+ grid[1][0]_pin[0][0][14] 
+ gvdd_cbx[1][0] 0 cbx[1][0]
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
Xcby[0][1] 
+ chany[0][1]_midout[0] 
+ chany[0][1]_midout[1] 
+ chany[0][1]_midout[2] 
+ chany[0][1]_midout[3] 
+ chany[0][1]_midout[4] 
+ chany[0][1]_midout[5] 
+ chany[0][1]_midout[6] 
+ chany[0][1]_midout[7] 
+ chany[0][1]_midout[8] 
+ chany[0][1]_midout[9] 
+ chany[0][1]_midout[10] 
+ chany[0][1]_midout[11] 
+ chany[0][1]_midout[12] 
+ chany[0][1]_midout[13] 
+ chany[0][1]_midout[14] 
+ chany[0][1]_midout[15] 
+ chany[0][1]_midout[16] 
+ chany[0][1]_midout[17] 
+ chany[0][1]_midout[18] 
+ chany[0][1]_midout[19] 
+ chany[0][1]_midout[20] 
+ chany[0][1]_midout[21] 
+ chany[0][1]_midout[22] 
+ chany[0][1]_midout[23] 
+ chany[0][1]_midout[24] 
+ chany[0][1]_midout[25] 
+ chany[0][1]_midout[26] 
+ chany[0][1]_midout[27] 
+ chany[0][1]_midout[28] 
+ chany[0][1]_midout[29] 
+ chany[0][1]_midout[30] 
+ chany[0][1]_midout[31] 
+ chany[0][1]_midout[32] 
+ chany[0][1]_midout[33] 
+ chany[0][1]_midout[34] 
+ chany[0][1]_midout[35] 
+ chany[0][1]_midout[36] 
+ chany[0][1]_midout[37] 
+ chany[0][1]_midout[38] 
+ chany[0][1]_midout[39] 
+ chany[0][1]_midout[40] 
+ chany[0][1]_midout[41] 
+ chany[0][1]_midout[42] 
+ chany[0][1]_midout[43] 
+ chany[0][1]_midout[44] 
+ chany[0][1]_midout[45] 
+ chany[0][1]_midout[46] 
+ chany[0][1]_midout[47] 
+ chany[0][1]_midout[48] 
+ chany[0][1]_midout[49] 
+ chany[0][1]_midout[50] 
+ chany[0][1]_midout[51] 
+ chany[0][1]_midout[52] 
+ chany[0][1]_midout[53] 
+ chany[0][1]_midout[54] 
+ chany[0][1]_midout[55] 
+ chany[0][1]_midout[56] 
+ chany[0][1]_midout[57] 
+ chany[0][1]_midout[58] 
+ chany[0][1]_midout[59] 
+ chany[0][1]_midout[60] 
+ chany[0][1]_midout[61] 
+ chany[0][1]_midout[62] 
+ chany[0][1]_midout[63] 
+ chany[0][1]_midout[64] 
+ chany[0][1]_midout[65] 
+ chany[0][1]_midout[66] 
+ chany[0][1]_midout[67] 
+ chany[0][1]_midout[68] 
+ chany[0][1]_midout[69] 
+ chany[0][1]_midout[70] 
+ chany[0][1]_midout[71] 
+ chany[0][1]_midout[72] 
+ chany[0][1]_midout[73] 
+ chany[0][1]_midout[74] 
+ chany[0][1]_midout[75] 
+ chany[0][1]_midout[76] 
+ chany[0][1]_midout[77] 
+ chany[0][1]_midout[78] 
+ chany[0][1]_midout[79] 
+ chany[0][1]_midout[80] 
+ chany[0][1]_midout[81] 
+ chany[0][1]_midout[82] 
+ chany[0][1]_midout[83] 
+ chany[0][1]_midout[84] 
+ chany[0][1]_midout[85] 
+ chany[0][1]_midout[86] 
+ chany[0][1]_midout[87] 
+ chany[0][1]_midout[88] 
+ chany[0][1]_midout[89] 
+ chany[0][1]_midout[90] 
+ chany[0][1]_midout[91] 
+ chany[0][1]_midout[92] 
+ chany[0][1]_midout[93] 
+ chany[0][1]_midout[94] 
+ chany[0][1]_midout[95] 
+ chany[0][1]_midout[96] 
+ chany[0][1]_midout[97] 
+ chany[0][1]_midout[98] 
+ chany[0][1]_midout[99] 
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
+ grid[0][1]_pin[0][1][0] 
+ grid[0][1]_pin[0][1][2] 
+ grid[0][1]_pin[0][1][4] 
+ grid[0][1]_pin[0][1][6] 
+ grid[0][1]_pin[0][1][8] 
+ grid[0][1]_pin[0][1][10] 
+ grid[0][1]_pin[0][1][12] 
+ grid[0][1]_pin[0][1][14] 
+ gvdd_cby[0][1] 0 cby[0][1]
Xcby[1][1] 
+ chany[1][1]_midout[0] 
+ chany[1][1]_midout[1] 
+ chany[1][1]_midout[2] 
+ chany[1][1]_midout[3] 
+ chany[1][1]_midout[4] 
+ chany[1][1]_midout[5] 
+ chany[1][1]_midout[6] 
+ chany[1][1]_midout[7] 
+ chany[1][1]_midout[8] 
+ chany[1][1]_midout[9] 
+ chany[1][1]_midout[10] 
+ chany[1][1]_midout[11] 
+ chany[1][1]_midout[12] 
+ chany[1][1]_midout[13] 
+ chany[1][1]_midout[14] 
+ chany[1][1]_midout[15] 
+ chany[1][1]_midout[16] 
+ chany[1][1]_midout[17] 
+ chany[1][1]_midout[18] 
+ chany[1][1]_midout[19] 
+ chany[1][1]_midout[20] 
+ chany[1][1]_midout[21] 
+ chany[1][1]_midout[22] 
+ chany[1][1]_midout[23] 
+ chany[1][1]_midout[24] 
+ chany[1][1]_midout[25] 
+ chany[1][1]_midout[26] 
+ chany[1][1]_midout[27] 
+ chany[1][1]_midout[28] 
+ chany[1][1]_midout[29] 
+ chany[1][1]_midout[30] 
+ chany[1][1]_midout[31] 
+ chany[1][1]_midout[32] 
+ chany[1][1]_midout[33] 
+ chany[1][1]_midout[34] 
+ chany[1][1]_midout[35] 
+ chany[1][1]_midout[36] 
+ chany[1][1]_midout[37] 
+ chany[1][1]_midout[38] 
+ chany[1][1]_midout[39] 
+ chany[1][1]_midout[40] 
+ chany[1][1]_midout[41] 
+ chany[1][1]_midout[42] 
+ chany[1][1]_midout[43] 
+ chany[1][1]_midout[44] 
+ chany[1][1]_midout[45] 
+ chany[1][1]_midout[46] 
+ chany[1][1]_midout[47] 
+ chany[1][1]_midout[48] 
+ chany[1][1]_midout[49] 
+ chany[1][1]_midout[50] 
+ chany[1][1]_midout[51] 
+ chany[1][1]_midout[52] 
+ chany[1][1]_midout[53] 
+ chany[1][1]_midout[54] 
+ chany[1][1]_midout[55] 
+ chany[1][1]_midout[56] 
+ chany[1][1]_midout[57] 
+ chany[1][1]_midout[58] 
+ chany[1][1]_midout[59] 
+ chany[1][1]_midout[60] 
+ chany[1][1]_midout[61] 
+ chany[1][1]_midout[62] 
+ chany[1][1]_midout[63] 
+ chany[1][1]_midout[64] 
+ chany[1][1]_midout[65] 
+ chany[1][1]_midout[66] 
+ chany[1][1]_midout[67] 
+ chany[1][1]_midout[68] 
+ chany[1][1]_midout[69] 
+ chany[1][1]_midout[70] 
+ chany[1][1]_midout[71] 
+ chany[1][1]_midout[72] 
+ chany[1][1]_midout[73] 
+ chany[1][1]_midout[74] 
+ chany[1][1]_midout[75] 
+ chany[1][1]_midout[76] 
+ chany[1][1]_midout[77] 
+ chany[1][1]_midout[78] 
+ chany[1][1]_midout[79] 
+ chany[1][1]_midout[80] 
+ chany[1][1]_midout[81] 
+ chany[1][1]_midout[82] 
+ chany[1][1]_midout[83] 
+ chany[1][1]_midout[84] 
+ chany[1][1]_midout[85] 
+ chany[1][1]_midout[86] 
+ chany[1][1]_midout[87] 
+ chany[1][1]_midout[88] 
+ chany[1][1]_midout[89] 
+ chany[1][1]_midout[90] 
+ chany[1][1]_midout[91] 
+ chany[1][1]_midout[92] 
+ chany[1][1]_midout[93] 
+ chany[1][1]_midout[94] 
+ chany[1][1]_midout[95] 
+ chany[1][1]_midout[96] 
+ chany[1][1]_midout[97] 
+ chany[1][1]_midout[98] 
+ chany[1][1]_midout[99] 
+ grid[2][1]_pin[0][3][0] 
+ grid[2][1]_pin[0][3][2] 
+ grid[2][1]_pin[0][3][4] 
+ grid[2][1]_pin[0][3][6] 
+ grid[2][1]_pin[0][3][8] 
+ grid[2][1]_pin[0][3][10] 
+ grid[2][1]_pin[0][3][12] 
+ grid[2][1]_pin[0][3][14] 
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
+ gvdd_cby[1][1] 0 cby[1][1]
Xsb[0][0] 
+ chany[0][1]_out[0] chany[0][1]_in[1] chany[0][1]_out[2] chany[0][1]_in[3] chany[0][1]_out[4] chany[0][1]_in[5] chany[0][1]_out[6] chany[0][1]_in[7] chany[0][1]_out[8] chany[0][1]_in[9] chany[0][1]_out[10] chany[0][1]_in[11] chany[0][1]_out[12] chany[0][1]_in[13] chany[0][1]_out[14] chany[0][1]_in[15] chany[0][1]_out[16] chany[0][1]_in[17] chany[0][1]_out[18] chany[0][1]_in[19] chany[0][1]_out[20] chany[0][1]_in[21] chany[0][1]_out[22] chany[0][1]_in[23] chany[0][1]_out[24] chany[0][1]_in[25] chany[0][1]_out[26] chany[0][1]_in[27] chany[0][1]_out[28] chany[0][1]_in[29] chany[0][1]_out[30] chany[0][1]_in[31] chany[0][1]_out[32] chany[0][1]_in[33] chany[0][1]_out[34] chany[0][1]_in[35] chany[0][1]_out[36] chany[0][1]_in[37] chany[0][1]_out[38] chany[0][1]_in[39] chany[0][1]_out[40] chany[0][1]_in[41] chany[0][1]_out[42] chany[0][1]_in[43] chany[0][1]_out[44] chany[0][1]_in[45] chany[0][1]_out[46] chany[0][1]_in[47] chany[0][1]_out[48] chany[0][1]_in[49] chany[0][1]_out[50] chany[0][1]_in[51] chany[0][1]_out[52] chany[0][1]_in[53] chany[0][1]_out[54] chany[0][1]_in[55] chany[0][1]_out[56] chany[0][1]_in[57] chany[0][1]_out[58] chany[0][1]_in[59] chany[0][1]_out[60] chany[0][1]_in[61] chany[0][1]_out[62] chany[0][1]_in[63] chany[0][1]_out[64] chany[0][1]_in[65] chany[0][1]_out[66] chany[0][1]_in[67] chany[0][1]_out[68] chany[0][1]_in[69] chany[0][1]_out[70] chany[0][1]_in[71] chany[0][1]_out[72] chany[0][1]_in[73] chany[0][1]_out[74] chany[0][1]_in[75] chany[0][1]_out[76] chany[0][1]_in[77] chany[0][1]_out[78] chany[0][1]_in[79] chany[0][1]_out[80] chany[0][1]_in[81] chany[0][1]_out[82] chany[0][1]_in[83] chany[0][1]_out[84] chany[0][1]_in[85] chany[0][1]_out[86] chany[0][1]_in[87] chany[0][1]_out[88] chany[0][1]_in[89] chany[0][1]_out[90] chany[0][1]_in[91] chany[0][1]_out[92] chany[0][1]_in[93] chany[0][1]_out[94] chany[0][1]_in[95] chany[0][1]_out[96] chany[0][1]_in[97] chany[0][1]_out[98] chany[0][1]_in[99] 
+ grid[0][1]_pin[0][1][1] grid[0][1]_pin[0][1][3] grid[0][1]_pin[0][1][5] grid[0][1]_pin[0][1][7] grid[0][1]_pin[0][1][9] grid[0][1]_pin[0][1][11] grid[0][1]_pin[0][1][13] grid[0][1]_pin[0][1][15] grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][47] 
+ chanx[1][0]_out[0] chanx[1][0]_in[1] chanx[1][0]_out[2] chanx[1][0]_in[3] chanx[1][0]_out[4] chanx[1][0]_in[5] chanx[1][0]_out[6] chanx[1][0]_in[7] chanx[1][0]_out[8] chanx[1][0]_in[9] chanx[1][0]_out[10] chanx[1][0]_in[11] chanx[1][0]_out[12] chanx[1][0]_in[13] chanx[1][0]_out[14] chanx[1][0]_in[15] chanx[1][0]_out[16] chanx[1][0]_in[17] chanx[1][0]_out[18] chanx[1][0]_in[19] chanx[1][0]_out[20] chanx[1][0]_in[21] chanx[1][0]_out[22] chanx[1][0]_in[23] chanx[1][0]_out[24] chanx[1][0]_in[25] chanx[1][0]_out[26] chanx[1][0]_in[27] chanx[1][0]_out[28] chanx[1][0]_in[29] chanx[1][0]_out[30] chanx[1][0]_in[31] chanx[1][0]_out[32] chanx[1][0]_in[33] chanx[1][0]_out[34] chanx[1][0]_in[35] chanx[1][0]_out[36] chanx[1][0]_in[37] chanx[1][0]_out[38] chanx[1][0]_in[39] chanx[1][0]_out[40] chanx[1][0]_in[41] chanx[1][0]_out[42] chanx[1][0]_in[43] chanx[1][0]_out[44] chanx[1][0]_in[45] chanx[1][0]_out[46] chanx[1][0]_in[47] chanx[1][0]_out[48] chanx[1][0]_in[49] chanx[1][0]_out[50] chanx[1][0]_in[51] chanx[1][0]_out[52] chanx[1][0]_in[53] chanx[1][0]_out[54] chanx[1][0]_in[55] chanx[1][0]_out[56] chanx[1][0]_in[57] chanx[1][0]_out[58] chanx[1][0]_in[59] chanx[1][0]_out[60] chanx[1][0]_in[61] chanx[1][0]_out[62] chanx[1][0]_in[63] chanx[1][0]_out[64] chanx[1][0]_in[65] chanx[1][0]_out[66] chanx[1][0]_in[67] chanx[1][0]_out[68] chanx[1][0]_in[69] chanx[1][0]_out[70] chanx[1][0]_in[71] chanx[1][0]_out[72] chanx[1][0]_in[73] chanx[1][0]_out[74] chanx[1][0]_in[75] chanx[1][0]_out[76] chanx[1][0]_in[77] chanx[1][0]_out[78] chanx[1][0]_in[79] chanx[1][0]_out[80] chanx[1][0]_in[81] chanx[1][0]_out[82] chanx[1][0]_in[83] chanx[1][0]_out[84] chanx[1][0]_in[85] chanx[1][0]_out[86] chanx[1][0]_in[87] chanx[1][0]_out[88] chanx[1][0]_in[89] chanx[1][0]_out[90] chanx[1][0]_in[91] chanx[1][0]_out[92] chanx[1][0]_in[93] chanx[1][0]_out[94] chanx[1][0]_in[95] chanx[1][0]_out[96] chanx[1][0]_in[97] chanx[1][0]_out[98] chanx[1][0]_in[99] 
+ grid[1][1]_pin[0][2][42] grid[1][1]_pin[0][2][46] grid[1][0]_pin[0][0][1] grid[1][0]_pin[0][0][3] grid[1][0]_pin[0][0][5] grid[1][0]_pin[0][0][7] grid[1][0]_pin[0][0][9] grid[1][0]_pin[0][0][11] grid[1][0]_pin[0][0][13] grid[1][0]_pin[0][0][15] 
+ 
+ 
+ 
+ 
+  gvdd_sb[0][0] 0 sb[0][0]
Xsb[0][1] 
+ 
+ 
+ chanx[1][1]_out[0] chanx[1][1]_in[1] chanx[1][1]_out[2] chanx[1][1]_in[3] chanx[1][1]_out[4] chanx[1][1]_in[5] chanx[1][1]_out[6] chanx[1][1]_in[7] chanx[1][1]_out[8] chanx[1][1]_in[9] chanx[1][1]_out[10] chanx[1][1]_in[11] chanx[1][1]_out[12] chanx[1][1]_in[13] chanx[1][1]_out[14] chanx[1][1]_in[15] chanx[1][1]_out[16] chanx[1][1]_in[17] chanx[1][1]_out[18] chanx[1][1]_in[19] chanx[1][1]_out[20] chanx[1][1]_in[21] chanx[1][1]_out[22] chanx[1][1]_in[23] chanx[1][1]_out[24] chanx[1][1]_in[25] chanx[1][1]_out[26] chanx[1][1]_in[27] chanx[1][1]_out[28] chanx[1][1]_in[29] chanx[1][1]_out[30] chanx[1][1]_in[31] chanx[1][1]_out[32] chanx[1][1]_in[33] chanx[1][1]_out[34] chanx[1][1]_in[35] chanx[1][1]_out[36] chanx[1][1]_in[37] chanx[1][1]_out[38] chanx[1][1]_in[39] chanx[1][1]_out[40] chanx[1][1]_in[41] chanx[1][1]_out[42] chanx[1][1]_in[43] chanx[1][1]_out[44] chanx[1][1]_in[45] chanx[1][1]_out[46] chanx[1][1]_in[47] chanx[1][1]_out[48] chanx[1][1]_in[49] chanx[1][1]_out[50] chanx[1][1]_in[51] chanx[1][1]_out[52] chanx[1][1]_in[53] chanx[1][1]_out[54] chanx[1][1]_in[55] chanx[1][1]_out[56] chanx[1][1]_in[57] chanx[1][1]_out[58] chanx[1][1]_in[59] chanx[1][1]_out[60] chanx[1][1]_in[61] chanx[1][1]_out[62] chanx[1][1]_in[63] chanx[1][1]_out[64] chanx[1][1]_in[65] chanx[1][1]_out[66] chanx[1][1]_in[67] chanx[1][1]_out[68] chanx[1][1]_in[69] chanx[1][1]_out[70] chanx[1][1]_in[71] chanx[1][1]_out[72] chanx[1][1]_in[73] chanx[1][1]_out[74] chanx[1][1]_in[75] chanx[1][1]_out[76] chanx[1][1]_in[77] chanx[1][1]_out[78] chanx[1][1]_in[79] chanx[1][1]_out[80] chanx[1][1]_in[81] chanx[1][1]_out[82] chanx[1][1]_in[83] chanx[1][1]_out[84] chanx[1][1]_in[85] chanx[1][1]_out[86] chanx[1][1]_in[87] chanx[1][1]_out[88] chanx[1][1]_in[89] chanx[1][1]_out[90] chanx[1][1]_in[91] chanx[1][1]_out[92] chanx[1][1]_in[93] chanx[1][1]_out[94] chanx[1][1]_in[95] chanx[1][1]_out[96] chanx[1][1]_in[97] chanx[1][1]_out[98] chanx[1][1]_in[99] 
+ grid[1][2]_pin[0][2][1] grid[1][2]_pin[0][2][3] grid[1][2]_pin[0][2][5] grid[1][2]_pin[0][2][7] grid[1][2]_pin[0][2][9] grid[1][2]_pin[0][2][11] grid[1][2]_pin[0][2][13] grid[1][2]_pin[0][2][15] grid[1][1]_pin[0][0][40] grid[1][1]_pin[0][0][44] grid[1][1]_pin[0][0][48] 
+ chany[0][1]_in[0] chany[0][1]_out[1] chany[0][1]_in[2] chany[0][1]_out[3] chany[0][1]_in[4] chany[0][1]_out[5] chany[0][1]_in[6] chany[0][1]_out[7] chany[0][1]_in[8] chany[0][1]_out[9] chany[0][1]_in[10] chany[0][1]_out[11] chany[0][1]_in[12] chany[0][1]_out[13] chany[0][1]_in[14] chany[0][1]_out[15] chany[0][1]_in[16] chany[0][1]_out[17] chany[0][1]_in[18] chany[0][1]_out[19] chany[0][1]_in[20] chany[0][1]_out[21] chany[0][1]_in[22] chany[0][1]_out[23] chany[0][1]_in[24] chany[0][1]_out[25] chany[0][1]_in[26] chany[0][1]_out[27] chany[0][1]_in[28] chany[0][1]_out[29] chany[0][1]_in[30] chany[0][1]_out[31] chany[0][1]_in[32] chany[0][1]_out[33] chany[0][1]_in[34] chany[0][1]_out[35] chany[0][1]_in[36] chany[0][1]_out[37] chany[0][1]_in[38] chany[0][1]_out[39] chany[0][1]_in[40] chany[0][1]_out[41] chany[0][1]_in[42] chany[0][1]_out[43] chany[0][1]_in[44] chany[0][1]_out[45] chany[0][1]_in[46] chany[0][1]_out[47] chany[0][1]_in[48] chany[0][1]_out[49] chany[0][1]_in[50] chany[0][1]_out[51] chany[0][1]_in[52] chany[0][1]_out[53] chany[0][1]_in[54] chany[0][1]_out[55] chany[0][1]_in[56] chany[0][1]_out[57] chany[0][1]_in[58] chany[0][1]_out[59] chany[0][1]_in[60] chany[0][1]_out[61] chany[0][1]_in[62] chany[0][1]_out[63] chany[0][1]_in[64] chany[0][1]_out[65] chany[0][1]_in[66] chany[0][1]_out[67] chany[0][1]_in[68] chany[0][1]_out[69] chany[0][1]_in[70] chany[0][1]_out[71] chany[0][1]_in[72] chany[0][1]_out[73] chany[0][1]_in[74] chany[0][1]_out[75] chany[0][1]_in[76] chany[0][1]_out[77] chany[0][1]_in[78] chany[0][1]_out[79] chany[0][1]_in[80] chany[0][1]_out[81] chany[0][1]_in[82] chany[0][1]_out[83] chany[0][1]_in[84] chany[0][1]_out[85] chany[0][1]_in[86] chany[0][1]_out[87] chany[0][1]_in[88] chany[0][1]_out[89] chany[0][1]_in[90] chany[0][1]_out[91] chany[0][1]_in[92] chany[0][1]_out[93] chany[0][1]_in[94] chany[0][1]_out[95] chany[0][1]_in[96] chany[0][1]_out[97] chany[0][1]_in[98] chany[0][1]_out[99] 
+ grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][47] grid[0][1]_pin[0][1][1] grid[0][1]_pin[0][1][3] grid[0][1]_pin[0][1][5] grid[0][1]_pin[0][1][7] grid[0][1]_pin[0][1][9] grid[0][1]_pin[0][1][11] grid[0][1]_pin[0][1][13] grid[0][1]_pin[0][1][15] 
+ 
+ 
+  gvdd_sb[0][1] 0 sb[0][1]
Xsb[1][0] 
+ chany[1][1]_out[0] chany[1][1]_in[1] chany[1][1]_out[2] chany[1][1]_in[3] chany[1][1]_out[4] chany[1][1]_in[5] chany[1][1]_out[6] chany[1][1]_in[7] chany[1][1]_out[8] chany[1][1]_in[9] chany[1][1]_out[10] chany[1][1]_in[11] chany[1][1]_out[12] chany[1][1]_in[13] chany[1][1]_out[14] chany[1][1]_in[15] chany[1][1]_out[16] chany[1][1]_in[17] chany[1][1]_out[18] chany[1][1]_in[19] chany[1][1]_out[20] chany[1][1]_in[21] chany[1][1]_out[22] chany[1][1]_in[23] chany[1][1]_out[24] chany[1][1]_in[25] chany[1][1]_out[26] chany[1][1]_in[27] chany[1][1]_out[28] chany[1][1]_in[29] chany[1][1]_out[30] chany[1][1]_in[31] chany[1][1]_out[32] chany[1][1]_in[33] chany[1][1]_out[34] chany[1][1]_in[35] chany[1][1]_out[36] chany[1][1]_in[37] chany[1][1]_out[38] chany[1][1]_in[39] chany[1][1]_out[40] chany[1][1]_in[41] chany[1][1]_out[42] chany[1][1]_in[43] chany[1][1]_out[44] chany[1][1]_in[45] chany[1][1]_out[46] chany[1][1]_in[47] chany[1][1]_out[48] chany[1][1]_in[49] chany[1][1]_out[50] chany[1][1]_in[51] chany[1][1]_out[52] chany[1][1]_in[53] chany[1][1]_out[54] chany[1][1]_in[55] chany[1][1]_out[56] chany[1][1]_in[57] chany[1][1]_out[58] chany[1][1]_in[59] chany[1][1]_out[60] chany[1][1]_in[61] chany[1][1]_out[62] chany[1][1]_in[63] chany[1][1]_out[64] chany[1][1]_in[65] chany[1][1]_out[66] chany[1][1]_in[67] chany[1][1]_out[68] chany[1][1]_in[69] chany[1][1]_out[70] chany[1][1]_in[71] chany[1][1]_out[72] chany[1][1]_in[73] chany[1][1]_out[74] chany[1][1]_in[75] chany[1][1]_out[76] chany[1][1]_in[77] chany[1][1]_out[78] chany[1][1]_in[79] chany[1][1]_out[80] chany[1][1]_in[81] chany[1][1]_out[82] chany[1][1]_in[83] chany[1][1]_out[84] chany[1][1]_in[85] chany[1][1]_out[86] chany[1][1]_in[87] chany[1][1]_out[88] chany[1][1]_in[89] chany[1][1]_out[90] chany[1][1]_in[91] chany[1][1]_out[92] chany[1][1]_in[93] chany[1][1]_out[94] chany[1][1]_in[95] chany[1][1]_out[96] chany[1][1]_in[97] chany[1][1]_out[98] chany[1][1]_in[99] 
+ grid[1][1]_pin[0][1][41] grid[1][1]_pin[0][1][45] grid[1][1]_pin[0][1][49] grid[2][1]_pin[0][3][1] grid[2][1]_pin[0][3][3] grid[2][1]_pin[0][3][5] grid[2][1]_pin[0][3][7] grid[2][1]_pin[0][3][9] grid[2][1]_pin[0][3][11] grid[2][1]_pin[0][3][13] grid[2][1]_pin[0][3][15] 
+ 
+ 
+ 
+ 
+ chanx[1][0]_in[0] chanx[1][0]_out[1] chanx[1][0]_in[2] chanx[1][0]_out[3] chanx[1][0]_in[4] chanx[1][0]_out[5] chanx[1][0]_in[6] chanx[1][0]_out[7] chanx[1][0]_in[8] chanx[1][0]_out[9] chanx[1][0]_in[10] chanx[1][0]_out[11] chanx[1][0]_in[12] chanx[1][0]_out[13] chanx[1][0]_in[14] chanx[1][0]_out[15] chanx[1][0]_in[16] chanx[1][0]_out[17] chanx[1][0]_in[18] chanx[1][0]_out[19] chanx[1][0]_in[20] chanx[1][0]_out[21] chanx[1][0]_in[22] chanx[1][0]_out[23] chanx[1][0]_in[24] chanx[1][0]_out[25] chanx[1][0]_in[26] chanx[1][0]_out[27] chanx[1][0]_in[28] chanx[1][0]_out[29] chanx[1][0]_in[30] chanx[1][0]_out[31] chanx[1][0]_in[32] chanx[1][0]_out[33] chanx[1][0]_in[34] chanx[1][0]_out[35] chanx[1][0]_in[36] chanx[1][0]_out[37] chanx[1][0]_in[38] chanx[1][0]_out[39] chanx[1][0]_in[40] chanx[1][0]_out[41] chanx[1][0]_in[42] chanx[1][0]_out[43] chanx[1][0]_in[44] chanx[1][0]_out[45] chanx[1][0]_in[46] chanx[1][0]_out[47] chanx[1][0]_in[48] chanx[1][0]_out[49] chanx[1][0]_in[50] chanx[1][0]_out[51] chanx[1][0]_in[52] chanx[1][0]_out[53] chanx[1][0]_in[54] chanx[1][0]_out[55] chanx[1][0]_in[56] chanx[1][0]_out[57] chanx[1][0]_in[58] chanx[1][0]_out[59] chanx[1][0]_in[60] chanx[1][0]_out[61] chanx[1][0]_in[62] chanx[1][0]_out[63] chanx[1][0]_in[64] chanx[1][0]_out[65] chanx[1][0]_in[66] chanx[1][0]_out[67] chanx[1][0]_in[68] chanx[1][0]_out[69] chanx[1][0]_in[70] chanx[1][0]_out[71] chanx[1][0]_in[72] chanx[1][0]_out[73] chanx[1][0]_in[74] chanx[1][0]_out[75] chanx[1][0]_in[76] chanx[1][0]_out[77] chanx[1][0]_in[78] chanx[1][0]_out[79] chanx[1][0]_in[80] chanx[1][0]_out[81] chanx[1][0]_in[82] chanx[1][0]_out[83] chanx[1][0]_in[84] chanx[1][0]_out[85] chanx[1][0]_in[86] chanx[1][0]_out[87] chanx[1][0]_in[88] chanx[1][0]_out[89] chanx[1][0]_in[90] chanx[1][0]_out[91] chanx[1][0]_in[92] chanx[1][0]_out[93] chanx[1][0]_in[94] chanx[1][0]_out[95] chanx[1][0]_in[96] chanx[1][0]_out[97] chanx[1][0]_in[98] chanx[1][0]_out[99] 
+ grid[1][1]_pin[0][2][42] grid[1][1]_pin[0][2][46] grid[1][0]_pin[0][0][1] grid[1][0]_pin[0][0][3] grid[1][0]_pin[0][0][5] grid[1][0]_pin[0][0][7] grid[1][0]_pin[0][0][9] grid[1][0]_pin[0][0][11] grid[1][0]_pin[0][0][13] grid[1][0]_pin[0][0][15] 
+  gvdd_sb[1][0] 0 sb[1][0]
Xsb[1][1] 
+ 
+ 
+ 
+ 
+ chany[1][1]_in[0] chany[1][1]_out[1] chany[1][1]_in[2] chany[1][1]_out[3] chany[1][1]_in[4] chany[1][1]_out[5] chany[1][1]_in[6] chany[1][1]_out[7] chany[1][1]_in[8] chany[1][1]_out[9] chany[1][1]_in[10] chany[1][1]_out[11] chany[1][1]_in[12] chany[1][1]_out[13] chany[1][1]_in[14] chany[1][1]_out[15] chany[1][1]_in[16] chany[1][1]_out[17] chany[1][1]_in[18] chany[1][1]_out[19] chany[1][1]_in[20] chany[1][1]_out[21] chany[1][1]_in[22] chany[1][1]_out[23] chany[1][1]_in[24] chany[1][1]_out[25] chany[1][1]_in[26] chany[1][1]_out[27] chany[1][1]_in[28] chany[1][1]_out[29] chany[1][1]_in[30] chany[1][1]_out[31] chany[1][1]_in[32] chany[1][1]_out[33] chany[1][1]_in[34] chany[1][1]_out[35] chany[1][1]_in[36] chany[1][1]_out[37] chany[1][1]_in[38] chany[1][1]_out[39] chany[1][1]_in[40] chany[1][1]_out[41] chany[1][1]_in[42] chany[1][1]_out[43] chany[1][1]_in[44] chany[1][1]_out[45] chany[1][1]_in[46] chany[1][1]_out[47] chany[1][1]_in[48] chany[1][1]_out[49] chany[1][1]_in[50] chany[1][1]_out[51] chany[1][1]_in[52] chany[1][1]_out[53] chany[1][1]_in[54] chany[1][1]_out[55] chany[1][1]_in[56] chany[1][1]_out[57] chany[1][1]_in[58] chany[1][1]_out[59] chany[1][1]_in[60] chany[1][1]_out[61] chany[1][1]_in[62] chany[1][1]_out[63] chany[1][1]_in[64] chany[1][1]_out[65] chany[1][1]_in[66] chany[1][1]_out[67] chany[1][1]_in[68] chany[1][1]_out[69] chany[1][1]_in[70] chany[1][1]_out[71] chany[1][1]_in[72] chany[1][1]_out[73] chany[1][1]_in[74] chany[1][1]_out[75] chany[1][1]_in[76] chany[1][1]_out[77] chany[1][1]_in[78] chany[1][1]_out[79] chany[1][1]_in[80] chany[1][1]_out[81] chany[1][1]_in[82] chany[1][1]_out[83] chany[1][1]_in[84] chany[1][1]_out[85] chany[1][1]_in[86] chany[1][1]_out[87] chany[1][1]_in[88] chany[1][1]_out[89] chany[1][1]_in[90] chany[1][1]_out[91] chany[1][1]_in[92] chany[1][1]_out[93] chany[1][1]_in[94] chany[1][1]_out[95] chany[1][1]_in[96] chany[1][1]_out[97] chany[1][1]_in[98] chany[1][1]_out[99] 
+ grid[2][1]_pin[0][3][1] grid[2][1]_pin[0][3][3] grid[2][1]_pin[0][3][5] grid[2][1]_pin[0][3][7] grid[2][1]_pin[0][3][9] grid[2][1]_pin[0][3][11] grid[2][1]_pin[0][3][13] grid[2][1]_pin[0][3][15] grid[1][1]_pin[0][1][41] grid[1][1]_pin[0][1][45] grid[1][1]_pin[0][1][49] 
+ chanx[1][1]_in[0] chanx[1][1]_out[1] chanx[1][1]_in[2] chanx[1][1]_out[3] chanx[1][1]_in[4] chanx[1][1]_out[5] chanx[1][1]_in[6] chanx[1][1]_out[7] chanx[1][1]_in[8] chanx[1][1]_out[9] chanx[1][1]_in[10] chanx[1][1]_out[11] chanx[1][1]_in[12] chanx[1][1]_out[13] chanx[1][1]_in[14] chanx[1][1]_out[15] chanx[1][1]_in[16] chanx[1][1]_out[17] chanx[1][1]_in[18] chanx[1][1]_out[19] chanx[1][1]_in[20] chanx[1][1]_out[21] chanx[1][1]_in[22] chanx[1][1]_out[23] chanx[1][1]_in[24] chanx[1][1]_out[25] chanx[1][1]_in[26] chanx[1][1]_out[27] chanx[1][1]_in[28] chanx[1][1]_out[29] chanx[1][1]_in[30] chanx[1][1]_out[31] chanx[1][1]_in[32] chanx[1][1]_out[33] chanx[1][1]_in[34] chanx[1][1]_out[35] chanx[1][1]_in[36] chanx[1][1]_out[37] chanx[1][1]_in[38] chanx[1][1]_out[39] chanx[1][1]_in[40] chanx[1][1]_out[41] chanx[1][1]_in[42] chanx[1][1]_out[43] chanx[1][1]_in[44] chanx[1][1]_out[45] chanx[1][1]_in[46] chanx[1][1]_out[47] chanx[1][1]_in[48] chanx[1][1]_out[49] chanx[1][1]_in[50] chanx[1][1]_out[51] chanx[1][1]_in[52] chanx[1][1]_out[53] chanx[1][1]_in[54] chanx[1][1]_out[55] chanx[1][1]_in[56] chanx[1][1]_out[57] chanx[1][1]_in[58] chanx[1][1]_out[59] chanx[1][1]_in[60] chanx[1][1]_out[61] chanx[1][1]_in[62] chanx[1][1]_out[63] chanx[1][1]_in[64] chanx[1][1]_out[65] chanx[1][1]_in[66] chanx[1][1]_out[67] chanx[1][1]_in[68] chanx[1][1]_out[69] chanx[1][1]_in[70] chanx[1][1]_out[71] chanx[1][1]_in[72] chanx[1][1]_out[73] chanx[1][1]_in[74] chanx[1][1]_out[75] chanx[1][1]_in[76] chanx[1][1]_out[77] chanx[1][1]_in[78] chanx[1][1]_out[79] chanx[1][1]_in[80] chanx[1][1]_out[81] chanx[1][1]_in[82] chanx[1][1]_out[83] chanx[1][1]_in[84] chanx[1][1]_out[85] chanx[1][1]_in[86] chanx[1][1]_out[87] chanx[1][1]_in[88] chanx[1][1]_out[89] chanx[1][1]_in[90] chanx[1][1]_out[91] chanx[1][1]_in[92] chanx[1][1]_out[93] chanx[1][1]_in[94] chanx[1][1]_out[95] chanx[1][1]_in[96] chanx[1][1]_out[97] chanx[1][1]_in[98] chanx[1][1]_out[99] 
+ grid[1][2]_pin[0][2][1] grid[1][2]_pin[0][2][3] grid[1][2]_pin[0][2][5] grid[1][2]_pin[0][2][7] grid[1][2]_pin[0][2][9] grid[1][2]_pin[0][2][11] grid[1][2]_pin[0][2][13] grid[1][2]_pin[0][2][15] grid[1][1]_pin[0][0][40] grid[1][1]_pin[0][0][44] grid[1][1]_pin[0][0][48] 
+  gvdd_sb[1][1] 0 sb[1][1]
***** BEGIN CLB to CLB Direct Connections *****
***** END CLB to CLB Direct Connections *****
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
***** Global VDD for I/O pads *****
Vgvdd_io gvdd_io 0 vsp
***** Global VDD for I/O pads SRAMs *****
Vgvdd_sram_io gvdd_sram_io 0 vsp
***** Global VDD for Local Interconnection *****
Vgvdd_local_interc gvdd_local_interc 0 vsp
***** Global VDD for CLB to CLB direct connection *****
Vgvdd_direct_interc gvdd_direct_interc 0 vsp
***** Global VDD for local routing SRAMs *****
Vgvdd_sram_local_routing gvdd_sram_local_routing 0 vsp
***** Global VDD for LUTs SRAMs *****
Vgvdd_sram_luts gvdd_sram_luts 0 vsp
***** Global VDD for Connection Boxes SRAMs *****
Vgvdd_sram_cbs gvdd_sram_cbs 0 vsp
***** Global VDD for Switch Boxes SRAMs *****
Vgvdd_sram_sbs gvdd_sram_sbs 0 vsp
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
***** Global VDD for Flip-flops (FFs) *****
Vgvdd_iopad[0] gvdd_iopad[0] 0 vsp
Rgvdd_iopad[0]_huge gvdd_iopad[0] 0 'vsp/10e-15'
Vgvdd_iopad[1] gvdd_iopad[1] 0 vsp
Rgvdd_iopad[1]_huge gvdd_iopad[1] 0 'vsp/10e-15'
Vgvdd_iopad[2] gvdd_iopad[2] 0 vsp
Rgvdd_iopad[2]_huge gvdd_iopad[2] 0 'vsp/10e-15'
Vgvdd_iopad[3] gvdd_iopad[3] 0 vsp
Rgvdd_iopad[3]_huge gvdd_iopad[3] 0 'vsp/10e-15'
Vgvdd_iopad[4] gvdd_iopad[4] 0 vsp
Rgvdd_iopad[4]_huge gvdd_iopad[4] 0 'vsp/10e-15'
Vgvdd_iopad[5] gvdd_iopad[5] 0 vsp
Rgvdd_iopad[5]_huge gvdd_iopad[5] 0 'vsp/10e-15'
Vgvdd_iopad[6] gvdd_iopad[6] 0 vsp
Rgvdd_iopad[6]_huge gvdd_iopad[6] 0 'vsp/10e-15'
Vgvdd_iopad[7] gvdd_iopad[7] 0 vsp
Rgvdd_iopad[7]_huge gvdd_iopad[7] 0 'vsp/10e-15'
Vgvdd_iopad[8] gvdd_iopad[8] 0 vsp
Rgvdd_iopad[8]_huge gvdd_iopad[8] 0 'vsp/10e-15'
Vgvdd_iopad[9] gvdd_iopad[9] 0 vsp
Rgvdd_iopad[9]_huge gvdd_iopad[9] 0 'vsp/10e-15'
Vgvdd_iopad[10] gvdd_iopad[10] 0 vsp
Rgvdd_iopad[10]_huge gvdd_iopad[10] 0 'vsp/10e-15'
Vgvdd_iopad[11] gvdd_iopad[11] 0 vsp
Rgvdd_iopad[11]_huge gvdd_iopad[11] 0 'vsp/10e-15'
Vgvdd_iopad[12] gvdd_iopad[12] 0 vsp
Rgvdd_iopad[12]_huge gvdd_iopad[12] 0 'vsp/10e-15'
Vgvdd_iopad[13] gvdd_iopad[13] 0 vsp
Rgvdd_iopad[13]_huge gvdd_iopad[13] 0 'vsp/10e-15'
Vgvdd_iopad[14] gvdd_iopad[14] 0 vsp
Rgvdd_iopad[14]_huge gvdd_iopad[14] 0 'vsp/10e-15'
Vgvdd_iopad[15] gvdd_iopad[15] 0 vsp
Rgvdd_iopad[15]_huge gvdd_iopad[15] 0 'vsp/10e-15'
Vgvdd_iopad[16] gvdd_iopad[16] 0 vsp
Rgvdd_iopad[16]_huge gvdd_iopad[16] 0 'vsp/10e-15'
Vgvdd_iopad[17] gvdd_iopad[17] 0 vsp
Rgvdd_iopad[17]_huge gvdd_iopad[17] 0 'vsp/10e-15'
Vgvdd_iopad[18] gvdd_iopad[18] 0 vsp
Rgvdd_iopad[18]_huge gvdd_iopad[18] 0 'vsp/10e-15'
Vgvdd_iopad[19] gvdd_iopad[19] 0 vsp
Rgvdd_iopad[19]_huge gvdd_iopad[19] 0 'vsp/10e-15'
Vgvdd_iopad[20] gvdd_iopad[20] 0 vsp
Rgvdd_iopad[20]_huge gvdd_iopad[20] 0 'vsp/10e-15'
Vgvdd_iopad[21] gvdd_iopad[21] 0 vsp
Rgvdd_iopad[21]_huge gvdd_iopad[21] 0 'vsp/10e-15'
Vgvdd_iopad[22] gvdd_iopad[22] 0 vsp
Rgvdd_iopad[22]_huge gvdd_iopad[22] 0 'vsp/10e-15'
Vgvdd_iopad[23] gvdd_iopad[23] 0 vsp
Rgvdd_iopad[23]_huge gvdd_iopad[23] 0 'vsp/10e-15'
Vgvdd_iopad[24] gvdd_iopad[24] 0 vsp
Rgvdd_iopad[24]_huge gvdd_iopad[24] 0 'vsp/10e-15'
Vgvdd_iopad[25] gvdd_iopad[25] 0 vsp
Rgvdd_iopad[25]_huge gvdd_iopad[25] 0 'vsp/10e-15'
Vgvdd_iopad[26] gvdd_iopad[26] 0 vsp
Rgvdd_iopad[26]_huge gvdd_iopad[26] 0 'vsp/10e-15'
Vgvdd_iopad[27] gvdd_iopad[27] 0 vsp
Rgvdd_iopad[27]_huge gvdd_iopad[27] 0 'vsp/10e-15'
Vgvdd_iopad[28] gvdd_iopad[28] 0 vsp
Rgvdd_iopad[28]_huge gvdd_iopad[28] 0 'vsp/10e-15'
Vgvdd_iopad[29] gvdd_iopad[29] 0 vsp
Rgvdd_iopad[29]_huge gvdd_iopad[29] 0 'vsp/10e-15'
Vgvdd_iopad[30] gvdd_iopad[30] 0 vsp
Rgvdd_iopad[30]_huge gvdd_iopad[30] 0 'vsp/10e-15'
Vgvdd_iopad[31] gvdd_iopad[31] 0 vsp
Rgvdd_iopad[31]_huge gvdd_iopad[31] 0 'vsp/10e-15'
***** Global VDD for Switch Boxes(SBs) *****
Vgvdd_sb[0][0] gvdd_sb[0][0] 0 vsp
Vgvdd_sb[0][1] gvdd_sb[0][1] 0 vsp
Vgvdd_sb[1][0] gvdd_sb[1][0] 0 vsp
Vgvdd_sb[1][1] gvdd_sb[1][1] 0 vsp
***** Global VDD for Connection Boxes(CBs) *****
Vgvdd_cbx[1][0] gvdd_cbx[1][0] 0 vsp
Vgvdd_cbx[1][1] gvdd_cbx[1][1] 0 vsp
Vgvdd_cby[0][1] gvdd_cby[0][1] 0 vsp
Vgvdd_cby[1][1] gvdd_cby[1][1] 0 vsp
Vgfpga_pad_iopad[0] gfpga_pad_iopad[0] 0 0
Vgfpga_pad_iopad[1] gfpga_pad_iopad[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4888*10.02*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.02*clock_period')
Vgfpga_pad_iopad[2] gfpga_pad_iopad[2] 0 0
Vgfpga_pad_iopad[3] gfpga_pad_iopad[3] 0 0
Vgfpga_pad_iopad[4] gfpga_pad_iopad[4] 0 0
Vgfpga_pad_iopad[5] gfpga_pad_iopad[5] 0 0
Vgfpga_pad_iopad[6] gfpga_pad_iopad[6] 0 0
Vgfpga_pad_iopad[7] gfpga_pad_iopad[7] 0 0
Vgfpga_pad_iopad[8] gfpga_pad_iopad[8] 0 0
Vgfpga_pad_iopad[9] gfpga_pad_iopad[9] 0 0
Vgfpga_pad_iopad[10] gfpga_pad_iopad[10] 0 0
Vgfpga_pad_iopad[11] gfpga_pad_iopad[11] 0 0
Vgfpga_pad_iopad[12] gfpga_pad_iopad[12] 0 0
Vgfpga_pad_iopad[13] gfpga_pad_iopad[13] 0 0
Vgfpga_pad_iopad[14] gfpga_pad_iopad[14] 0 0
Vgfpga_pad_iopad[15] gfpga_pad_iopad[15] 0 0
Vgfpga_pad_iopad[16] gfpga_pad_iopad[16] 0 0
Vgfpga_pad_iopad[17] gfpga_pad_iopad[17] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgfpga_pad_iopad[18] gfpga_pad_iopad[18] 0 0
Vgfpga_pad_iopad[19] gfpga_pad_iopad[19] 0 0
Vgfpga_pad_iopad[20] gfpga_pad_iopad[20] 0 0
Vgfpga_pad_iopad[21] gfpga_pad_iopad[21] 0 0
Vgfpga_pad_iopad[22] gfpga_pad_iopad[22] 0 0
Vgfpga_pad_iopad[23] gfpga_pad_iopad[23] 0 0
Vgfpga_pad_iopad[24] gfpga_pad_iopad[24] 0 0
Vgfpga_pad_iopad[25] gfpga_pad_iopad[25] 0 0
Vgfpga_pad_iopad[26] gfpga_pad_iopad[26] 0 0
Vgfpga_pad_iopad[27] gfpga_pad_iopad[27] 0 0
Vgfpga_pad_iopad[28] gfpga_pad_iopad[28] 0 0
Vgfpga_pad_iopad[29] gfpga_pad_iopad[29] 0 0
Vgfpga_pad_iopad[31] gfpga_pad_iopad[31] 0 0
***** 6 Clock Simulation, accuracy=1e-13 *****
.tran 1e-13  '6*clock_period'
***** Generic Measurements for Circuit Parameters *****
.measure tran leakage_power_sram_local_routing avg p(Vgvdd_sram_local_routing) from=0 to='clock_period'
.measure tran leakage_power_sram_luts avg p(Vgvdd_sram_luts) from=0 to='clock_period'
.measure tran leakage_power_sram_cbs avg p(Vgvdd_sram_cbs) from=0 to='clock_period'
.measure tran leakage_power_sram_sbs avg p(Vgvdd_sram_sbs) from=0 to='clock_period'
.measure tran leakage_power_io avg p(Vgvdd_io) from=0 to='clock_period'
.measure tran leakage_power_local_interc avg p(Vgvdd_local_interc) from=0 to='clock_period'
.measure tran leakage_power_direct_interc avg p(Vgvdd_direct_interc) from=0 to='clock_period'
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
.measure tran leakage_power_iopad[0] avg p(Vgvdd_iopad[0]) from=0 to='clock_period'
.measure tran leakage_power_iopad[1] avg p(Vgvdd_iopad[1]) from=0 to='clock_period'
.measure tran leakage_power_iopad[2] avg p(Vgvdd_iopad[2]) from=0 to='clock_period'
.measure tran leakage_power_iopad[3] avg p(Vgvdd_iopad[3]) from=0 to='clock_period'
.measure tran leakage_power_iopad[4] avg p(Vgvdd_iopad[4]) from=0 to='clock_period'
.measure tran leakage_power_iopad[5] avg p(Vgvdd_iopad[5]) from=0 to='clock_period'
.measure tran leakage_power_iopad[6] avg p(Vgvdd_iopad[6]) from=0 to='clock_period'
.measure tran leakage_power_iopad[7] avg p(Vgvdd_iopad[7]) from=0 to='clock_period'
.measure tran leakage_power_iopad[8] avg p(Vgvdd_iopad[8]) from=0 to='clock_period'
.measure tran leakage_power_iopad[9] avg p(Vgvdd_iopad[9]) from=0 to='clock_period'
.measure tran leakage_power_iopad[10] avg p(Vgvdd_iopad[10]) from=0 to='clock_period'
.measure tran leakage_power_iopad[11] avg p(Vgvdd_iopad[11]) from=0 to='clock_period'
.measure tran leakage_power_iopad[12] avg p(Vgvdd_iopad[12]) from=0 to='clock_period'
.measure tran leakage_power_iopad[13] avg p(Vgvdd_iopad[13]) from=0 to='clock_period'
.measure tran leakage_power_iopad[14] avg p(Vgvdd_iopad[14]) from=0 to='clock_period'
.measure tran leakage_power_iopad[15] avg p(Vgvdd_iopad[15]) from=0 to='clock_period'
.measure tran leakage_power_iopad[16] avg p(Vgvdd_iopad[16]) from=0 to='clock_period'
.measure tran leakage_power_iopad[17] avg p(Vgvdd_iopad[17]) from=0 to='clock_period'
.measure tran leakage_power_iopad[18] avg p(Vgvdd_iopad[18]) from=0 to='clock_period'
.measure tran leakage_power_iopad[19] avg p(Vgvdd_iopad[19]) from=0 to='clock_period'
.measure tran leakage_power_iopad[20] avg p(Vgvdd_iopad[20]) from=0 to='clock_period'
.measure tran leakage_power_iopad[21] avg p(Vgvdd_iopad[21]) from=0 to='clock_period'
.measure tran leakage_power_iopad[22] avg p(Vgvdd_iopad[22]) from=0 to='clock_period'
.measure tran leakage_power_iopad[23] avg p(Vgvdd_iopad[23]) from=0 to='clock_period'
.measure tran leakage_power_iopad[24] avg p(Vgvdd_iopad[24]) from=0 to='clock_period'
.measure tran leakage_power_iopad[25] avg p(Vgvdd_iopad[25]) from=0 to='clock_period'
.measure tran leakage_power_iopad[26] avg p(Vgvdd_iopad[26]) from=0 to='clock_period'
.measure tran leakage_power_iopad[27] avg p(Vgvdd_iopad[27]) from=0 to='clock_period'
.measure tran leakage_power_iopad[28] avg p(Vgvdd_iopad[28]) from=0 to='clock_period'
.measure tran leakage_power_iopad[29] avg p(Vgvdd_iopad[29]) from=0 to='clock_period'
.measure tran leakage_power_iopad[30] avg p(Vgvdd_iopad[30]) from=0 to='clock_period'
.measure tran leakage_power_iopad[31] avg p(Vgvdd_iopad[31]) from=0 to='clock_period'
.measure tran leakage_power_iopad[0to0] 
+ param = 'leakage_power_iopad[0]'
.measure tran leakage_power_iopad[0to1] 
+ param = 'leakage_power_iopad[1]+leakage_power_iopad[0to0]'
.measure tran leakage_power_iopad[0to2] 
+ param = 'leakage_power_iopad[2]+leakage_power_iopad[0to1]'
.measure tran leakage_power_iopad[0to3] 
+ param = 'leakage_power_iopad[3]+leakage_power_iopad[0to2]'
.measure tran leakage_power_iopad[0to4] 
+ param = 'leakage_power_iopad[4]+leakage_power_iopad[0to3]'
.measure tran leakage_power_iopad[0to5] 
+ param = 'leakage_power_iopad[5]+leakage_power_iopad[0to4]'
.measure tran leakage_power_iopad[0to6] 
+ param = 'leakage_power_iopad[6]+leakage_power_iopad[0to5]'
.measure tran leakage_power_iopad[0to7] 
+ param = 'leakage_power_iopad[7]+leakage_power_iopad[0to6]'
.measure tran leakage_power_iopad[0to8] 
+ param = 'leakage_power_iopad[8]+leakage_power_iopad[0to7]'
.measure tran leakage_power_iopad[0to9] 
+ param = 'leakage_power_iopad[9]+leakage_power_iopad[0to8]'
.measure tran leakage_power_iopad[0to10] 
+ param = 'leakage_power_iopad[10]+leakage_power_iopad[0to9]'
.measure tran leakage_power_iopad[0to11] 
+ param = 'leakage_power_iopad[11]+leakage_power_iopad[0to10]'
.measure tran leakage_power_iopad[0to12] 
+ param = 'leakage_power_iopad[12]+leakage_power_iopad[0to11]'
.measure tran leakage_power_iopad[0to13] 
+ param = 'leakage_power_iopad[13]+leakage_power_iopad[0to12]'
.measure tran leakage_power_iopad[0to14] 
+ param = 'leakage_power_iopad[14]+leakage_power_iopad[0to13]'
.measure tran leakage_power_iopad[0to15] 
+ param = 'leakage_power_iopad[15]+leakage_power_iopad[0to14]'
.measure tran leakage_power_iopad[0to16] 
+ param = 'leakage_power_iopad[16]+leakage_power_iopad[0to15]'
.measure tran leakage_power_iopad[0to17] 
+ param = 'leakage_power_iopad[17]+leakage_power_iopad[0to16]'
.measure tran leakage_power_iopad[0to18] 
+ param = 'leakage_power_iopad[18]+leakage_power_iopad[0to17]'
.measure tran leakage_power_iopad[0to19] 
+ param = 'leakage_power_iopad[19]+leakage_power_iopad[0to18]'
.measure tran leakage_power_iopad[0to20] 
+ param = 'leakage_power_iopad[20]+leakage_power_iopad[0to19]'
.measure tran leakage_power_iopad[0to21] 
+ param = 'leakage_power_iopad[21]+leakage_power_iopad[0to20]'
.measure tran leakage_power_iopad[0to22] 
+ param = 'leakage_power_iopad[22]+leakage_power_iopad[0to21]'
.measure tran leakage_power_iopad[0to23] 
+ param = 'leakage_power_iopad[23]+leakage_power_iopad[0to22]'
.measure tran leakage_power_iopad[0to24] 
+ param = 'leakage_power_iopad[24]+leakage_power_iopad[0to23]'
.measure tran leakage_power_iopad[0to25] 
+ param = 'leakage_power_iopad[25]+leakage_power_iopad[0to24]'
.measure tran leakage_power_iopad[0to26] 
+ param = 'leakage_power_iopad[26]+leakage_power_iopad[0to25]'
.measure tran leakage_power_iopad[0to27] 
+ param = 'leakage_power_iopad[27]+leakage_power_iopad[0to26]'
.measure tran leakage_power_iopad[0to28] 
+ param = 'leakage_power_iopad[28]+leakage_power_iopad[0to27]'
.measure tran leakage_power_iopad[0to29] 
+ param = 'leakage_power_iopad[29]+leakage_power_iopad[0to28]'
.measure tran leakage_power_iopad[0to30] 
+ param = 'leakage_power_iopad[30]+leakage_power_iopad[0to29]'
.measure tran leakage_power_iopad[0to31] 
+ param = 'leakage_power_iopad[31]+leakage_power_iopad[0to30]'
.measure tran total_leakage_power_iopad 
+ param = 'leakage_power_iopad[0to31]'
***** Measure Leakage Power for Connection Boxes(CBs) *****
.measure tran leakage_power_cbx[1][0] avg p(Vgvdd_cbx[1][0]) from=0 to='clock_period'
.measure tran leakage_power_cbx[1][1] avg p(Vgvdd_cbx[1][1]) from=0 to='clock_period'
.measure tran leakage_power_cby[0][1] avg p(Vgvdd_cby[0][1]) from=0 to='clock_period'
.measure tran leakage_power_cby[1][1] avg p(Vgvdd_cby[1][1]) from=0 to='clock_period'
***** Measure Total Leakage Power for Connection Boxes(CBs) *****
.measure tran leakage_power_cbx[1to1][0to0] 
+ param='leakage_power_cbx[1][0]'
.measure tran leakage_power_cbx[1to1][0to1] 
+ param='leakage_power_cbx[1][1]+leakage_power_cbx[1to1][0to0]'
.measure tran leakage_power_cby[0to0][1to1] 
+ param='leakage_power_cby[0][1]'
.measure tran leakage_power_cby[0to1][1to1] 
+ param='leakage_power_cby[1][1]+leakage_power_cby[0to0][1to1]'
.measure tran leakage_power_cbs 
+ param='leakage_power_cbx[1to1][0to1]+leakage_power_cby[0to1][1to1]' 
***** Measure Leakage Power for Switch Boxes(SBs) *****
.measure tran leakage_power_sb[0][0] avg p(Vgvdd_sb[0][0]) from=0 to='clock_period'
.measure tran leakage_power_sb[0][1] avg p(Vgvdd_sb[0][1]) from=0 to='clock_period'
.measure tran leakage_power_sb[1][0] avg p(Vgvdd_sb[1][0]) from=0 to='clock_period'
.measure tran leakage_power_sb[1][1] avg p(Vgvdd_sb[1][1]) from=0 to='clock_period'
***** Measure Total Leakage Power for Switch Boxes(SBs) *****
.measure tran leakage_power_sb[0to0][0to0] 
+ param='leakage_power_sb[0][0]'
.measure tran leakage_power_sb[0to0][0to1] 
+ param='leakage_power_sb[0][1]+leakage_power_sb[0to0][0to0]'
.measure tran leakage_power_sb[0to1][0to0] 
+ param='leakage_power_sb[1][0]+leakage_power_sb[0to0][0to1]'
.measure tran leakage_power_sb[0to1][0to1] 
+ param='leakage_power_sb[1][1]+leakage_power_sb[0to1][0to0]'
.measure tran leakage_power_sbs 
+ param='leakage_power_sb[0to1][0to1]' 
.measure tran dynamic_power_sram_local_routing avg p(Vgvdd_sram_local_routing) from='clock_period' to='6*clock_period'
.measure tran energy_per_cycle_sram_local_routing 
 + param='dynamic_power_sram_local_routing*clock_period'
.measure tran dynamic_power_sram_luts avg p(Vgvdd_sram_luts) from='clock_period' to='6*clock_period'
.measure tran energy_per_cycle_sram_luts 
 + param='dynamic_power_sram_luts*clock_period'
.measure tran dynamic_power_sram_cbs avg p(Vgvdd_sram_cbs) from='clock_period' to='6*clock_period'
.measure tran energy_per_cycle_sram_cbs 
 + param='dynamic_power_sram_cbs*clock_period'
.measure tran dynamic_power_sram_sbs avg p(Vgvdd_sram_sbs) from='clock_period' to='6*clock_period'
.measure tran energy_per_cycle_sram_sbs 
 + param='dynamic_power_sram_sbs*clock_period'
.measure tran dynamic_power_iopad[0] avg p(Vgvdd_iopad[0]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[1] avg p(Vgvdd_iopad[1]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[2] avg p(Vgvdd_iopad[2]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[3] avg p(Vgvdd_iopad[3]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[4] avg p(Vgvdd_iopad[4]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[5] avg p(Vgvdd_iopad[5]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[6] avg p(Vgvdd_iopad[6]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[7] avg p(Vgvdd_iopad[7]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[8] avg p(Vgvdd_iopad[8]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[9] avg p(Vgvdd_iopad[9]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[10] avg p(Vgvdd_iopad[10]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[11] avg p(Vgvdd_iopad[11]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[12] avg p(Vgvdd_iopad[12]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[13] avg p(Vgvdd_iopad[13]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[14] avg p(Vgvdd_iopad[14]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[15] avg p(Vgvdd_iopad[15]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[16] avg p(Vgvdd_iopad[16]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[17] avg p(Vgvdd_iopad[17]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[18] avg p(Vgvdd_iopad[18]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[19] avg p(Vgvdd_iopad[19]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[20] avg p(Vgvdd_iopad[20]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[21] avg p(Vgvdd_iopad[21]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[22] avg p(Vgvdd_iopad[22]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[23] avg p(Vgvdd_iopad[23]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[24] avg p(Vgvdd_iopad[24]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[25] avg p(Vgvdd_iopad[25]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[26] avg p(Vgvdd_iopad[26]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[27] avg p(Vgvdd_iopad[27]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[28] avg p(Vgvdd_iopad[28]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[29] avg p(Vgvdd_iopad[29]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[30] avg p(Vgvdd_iopad[30]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[31] avg p(Vgvdd_iopad[31]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_iopad[0to0] 
+ param = 'dynamic_power_iopad[0]'
.measure tran dynamic_power_iopad[0to1] 
+ param = 'dynamic_power_iopad[1]+dynamic_power_iopad[0to0]'
.measure tran dynamic_power_iopad[0to2] 
+ param = 'dynamic_power_iopad[2]+dynamic_power_iopad[0to1]'
.measure tran dynamic_power_iopad[0to3] 
+ param = 'dynamic_power_iopad[3]+dynamic_power_iopad[0to2]'
.measure tran dynamic_power_iopad[0to4] 
+ param = 'dynamic_power_iopad[4]+dynamic_power_iopad[0to3]'
.measure tran dynamic_power_iopad[0to5] 
+ param = 'dynamic_power_iopad[5]+dynamic_power_iopad[0to4]'
.measure tran dynamic_power_iopad[0to6] 
+ param = 'dynamic_power_iopad[6]+dynamic_power_iopad[0to5]'
.measure tran dynamic_power_iopad[0to7] 
+ param = 'dynamic_power_iopad[7]+dynamic_power_iopad[0to6]'
.measure tran dynamic_power_iopad[0to8] 
+ param = 'dynamic_power_iopad[8]+dynamic_power_iopad[0to7]'
.measure tran dynamic_power_iopad[0to9] 
+ param = 'dynamic_power_iopad[9]+dynamic_power_iopad[0to8]'
.measure tran dynamic_power_iopad[0to10] 
+ param = 'dynamic_power_iopad[10]+dynamic_power_iopad[0to9]'
.measure tran dynamic_power_iopad[0to11] 
+ param = 'dynamic_power_iopad[11]+dynamic_power_iopad[0to10]'
.measure tran dynamic_power_iopad[0to12] 
+ param = 'dynamic_power_iopad[12]+dynamic_power_iopad[0to11]'
.measure tran dynamic_power_iopad[0to13] 
+ param = 'dynamic_power_iopad[13]+dynamic_power_iopad[0to12]'
.measure tran dynamic_power_iopad[0to14] 
+ param = 'dynamic_power_iopad[14]+dynamic_power_iopad[0to13]'
.measure tran dynamic_power_iopad[0to15] 
+ param = 'dynamic_power_iopad[15]+dynamic_power_iopad[0to14]'
.measure tran dynamic_power_iopad[0to16] 
+ param = 'dynamic_power_iopad[16]+dynamic_power_iopad[0to15]'
.measure tran dynamic_power_iopad[0to17] 
+ param = 'dynamic_power_iopad[17]+dynamic_power_iopad[0to16]'
.measure tran dynamic_power_iopad[0to18] 
+ param = 'dynamic_power_iopad[18]+dynamic_power_iopad[0to17]'
.measure tran dynamic_power_iopad[0to19] 
+ param = 'dynamic_power_iopad[19]+dynamic_power_iopad[0to18]'
.measure tran dynamic_power_iopad[0to20] 
+ param = 'dynamic_power_iopad[20]+dynamic_power_iopad[0to19]'
.measure tran dynamic_power_iopad[0to21] 
+ param = 'dynamic_power_iopad[21]+dynamic_power_iopad[0to20]'
.measure tran dynamic_power_iopad[0to22] 
+ param = 'dynamic_power_iopad[22]+dynamic_power_iopad[0to21]'
.measure tran dynamic_power_iopad[0to23] 
+ param = 'dynamic_power_iopad[23]+dynamic_power_iopad[0to22]'
.measure tran dynamic_power_iopad[0to24] 
+ param = 'dynamic_power_iopad[24]+dynamic_power_iopad[0to23]'
.measure tran dynamic_power_iopad[0to25] 
+ param = 'dynamic_power_iopad[25]+dynamic_power_iopad[0to24]'
.measure tran dynamic_power_iopad[0to26] 
+ param = 'dynamic_power_iopad[26]+dynamic_power_iopad[0to25]'
.measure tran dynamic_power_iopad[0to27] 
+ param = 'dynamic_power_iopad[27]+dynamic_power_iopad[0to26]'
.measure tran dynamic_power_iopad[0to28] 
+ param = 'dynamic_power_iopad[28]+dynamic_power_iopad[0to27]'
.measure tran dynamic_power_iopad[0to29] 
+ param = 'dynamic_power_iopad[29]+dynamic_power_iopad[0to28]'
.measure tran dynamic_power_iopad[0to30] 
+ param = 'dynamic_power_iopad[30]+dynamic_power_iopad[0to29]'
.measure tran dynamic_power_iopad[0to31] 
+ param = 'dynamic_power_iopad[31]+dynamic_power_iopad[0to30]'
.measure tran total_dynamic_power_iopad 
+ param = 'dynamic_power_iopad[0to31]'
.measure tran total_energy_per_cycle_iopad 
+ param = 'dynamic_power_iopad[0to31]*clock_period'
.measure tran dynamic_power_local_interc avg p(Vgvdd_local_interc) from='clock_period' to='6*clock_period'
.measure tran energy_per_cycle_local_routing 
 + param='dynamic_power_local_interc*clock_period'
.measure tran dynamic_power_direct_interc avg p(Vgvdd_direct_interc) from='clock_period' to='6*clock_period'
.measure tran energy_per_cycle_direct_interc 
 + param='dynamic_power_direct_interc*clock_period'
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
***** Measure Dynamic Power for Connection Boxes(CBs) *****
.measure tran dynamic_power_cbx[1][0] avg p(Vgvdd_cbx[1][0]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_cbx[1][1] avg p(Vgvdd_cbx[1][1]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_cby[0][1] avg p(Vgvdd_cby[0][1]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_cby[1][1] avg p(Vgvdd_cby[1][1]) from='clock_period' to='6*clock_period'
***** Measure Total Dynamic Power for Connection Boxes(CBs) *****
.measure tran dynamic_power_cbx[1to1][0to0] 
+ param='dynamic_power_cbx[1][0]'
.measure tran dynamic_power_cbx[1to1][0to1] 
+ param='dynamic_power_cbx[1][1]+dynamic_power_cbx[1to1][0to0]'
.measure tran dynamic_power_cby[0to0][1to1] 
+ param='dynamic_power_cby[0][1]'
.measure tran dynamic_power_cby[0to1][1to1] 
+ param='dynamic_power_cby[1][1]+dynamic_power_cby[0to0][1to1]'
.measure tran dynamic_power_cbs 
+ param='dynamic_power_cbx[1to1][0to1]+dynamic_power_cby[0to1][1to1]' 
.measure tran energy_per_cycle_cbs 
 + param='dynamic_power_cbs*clock_period'
***** Measure Dynamic Power for Switch Boxes(SBs) *****
.measure tran dynamic_power_sb[0][0] avg p(Vgvdd_sb[0][0]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_sb[0][1] avg p(Vgvdd_sb[0][1]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_sb[1][0] avg p(Vgvdd_sb[1][0]) from='clock_period' to='6*clock_period'
.measure tran dynamic_power_sb[1][1] avg p(Vgvdd_sb[1][1]) from='clock_period' to='6*clock_period'
***** Measure Total Dynamic Power for Switch Boxes(SBs) *****
.measure tran dynamic_power_sb[0to0][0to0] 
+ param='dynamic_power_sb[0][0]'
.measure tran dynamic_power_sb[0to0][0to1] 
+ param='dynamic_power_sb[0][1]+dynamic_power_sb[0to0][0to0]'
.measure tran dynamic_power_sb[0to1][0to0] 
+ param='dynamic_power_sb[1][0]+dynamic_power_sb[0to0][0to1]'
.measure tran dynamic_power_sb[0to1][0to1] 
+ param='dynamic_power_sb[1][1]+dynamic_power_sb[0to1][0to0]'
.measure tran dynamic_power_sbs 
+ param='dynamic_power_sb[0to1][0to1]' 
.measure tran energy_per_cycle_sbs 
 + param='dynamic_power_sbs*clock_period'
.end
