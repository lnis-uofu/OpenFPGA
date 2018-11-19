*****************************
*     FPGA SPICE Netlist    *
* Description: FPGA LUT Testbench for Design: example_2 *
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
.global gvdd_sram_luts
.global gvdd_load
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

***** LUT[0]: logical_block_index[-1], gvdd_index[-1]*****
Xlut[0] lut[0]->in[0] lut[0]->in[1] lut[0]->in[2] lut[0]->in[3] lut[0]->in[4] lut[0]->in[5] lut[0]->out gvdd 0 grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut6]_ble6[0]_mode[ble6]_lut6[0]
Vlut[0]->in[0] lut[0]->in[0] 0 
+  0
Vlut[0]->in[1] lut[0]->in[1] 0 
+  0
Vlut[0]->in[2] lut[0]->in[2] 0 
+  0
Vlut[0]->in[3] lut[0]->in[3] 0 
+  0
Vlut[0]->in[4] lut[0]->in[4] 0 
+  0
Vlut[0]->in[5] lut[0]->in[5] 0 
+  0
Xload_inv[0]_no0 lut[0]->out lut[0]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[1]_no0 lut[0]->out lut[0]->out_out[1] gvdd_load 0 inv size=1
***** LUT[1]: logical_block_index[-1], gvdd_index[-1]*****
Xlut[1] lut[1]->in[0] lut[1]->in[1] lut[1]->in[2] lut[1]->in[3] lut[1]->in[4] lut[1]->in[5] lut[1]->out gvdd 0 grid[1][1]_clb[0]_mode[clb]_fle[1]_mode[n1_lut6]_ble6[0]_mode[ble6]_lut6[0]
Vlut[1]->in[0] lut[1]->in[0] 0 
+  0
Vlut[1]->in[1] lut[1]->in[1] 0 
+  0
Vlut[1]->in[2] lut[1]->in[2] 0 
+  0
Vlut[1]->in[3] lut[1]->in[3] 0 
+  0
Vlut[1]->in[4] lut[1]->in[4] 0 
+  0
Vlut[1]->in[5] lut[1]->in[5] 0 
+  0
Xload_inv[2]_no0 lut[1]->out lut[1]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[3]_no0 lut[1]->out lut[1]->out_out[1] gvdd_load 0 inv size=1
***** LUT[2]: logical_block_index[-1], gvdd_index[-1]*****
Xlut[2] lut[2]->in[0] lut[2]->in[1] lut[2]->in[2] lut[2]->in[3] lut[2]->in[4] lut[2]->in[5] lut[2]->out gvdd 0 grid[1][1]_clb[0]_mode[clb]_fle[2]_mode[n1_lut6]_ble6[0]_mode[ble6]_lut6[0]
Vlut[2]->in[0] lut[2]->in[0] 0 
+  0
Vlut[2]->in[1] lut[2]->in[1] 0 
+  0
Vlut[2]->in[2] lut[2]->in[2] 0 
+  0
Vlut[2]->in[3] lut[2]->in[3] 0 
+  0
Vlut[2]->in[4] lut[2]->in[4] 0 
+  0
Vlut[2]->in[5] lut[2]->in[5] 0 
+  0
Xload_inv[4]_no0 lut[2]->out lut[2]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[5]_no0 lut[2]->out lut[2]->out_out[1] gvdd_load 0 inv size=1
***** LUT[3]: logical_block_index[-1], gvdd_index[-1]*****
Xlut[3] lut[3]->in[0] lut[3]->in[1] lut[3]->in[2] lut[3]->in[3] lut[3]->in[4] lut[3]->in[5] lut[3]->out gvdd 0 grid[1][1]_clb[0]_mode[clb]_fle[3]_mode[n1_lut6]_ble6[0]_mode[ble6]_lut6[0]
Vlut[3]->in[0] lut[3]->in[0] 0 
+  0
Vlut[3]->in[1] lut[3]->in[1] 0 
+  0
Vlut[3]->in[2] lut[3]->in[2] 0 
+  0
Vlut[3]->in[3] lut[3]->in[3] 0 
+  0
Vlut[3]->in[4] lut[3]->in[4] 0 
+  0
Vlut[3]->in[5] lut[3]->in[5] 0 
+  0
Xload_inv[6]_no0 lut[3]->out lut[3]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[7]_no0 lut[3]->out lut[3]->out_out[1] gvdd_load 0 inv size=1
***** LUT[4]: logical_block_index[-1], gvdd_index[-1]*****
Xlut[4] lut[4]->in[0] lut[4]->in[1] lut[4]->in[2] lut[4]->in[3] lut[4]->in[4] lut[4]->in[5] lut[4]->out gvdd 0 grid[1][1]_clb[0]_mode[clb]_fle[4]_mode[n1_lut6]_ble6[0]_mode[ble6]_lut6[0]
Vlut[4]->in[0] lut[4]->in[0] 0 
+  0
Vlut[4]->in[1] lut[4]->in[1] 0 
+  0
Vlut[4]->in[2] lut[4]->in[2] 0 
+  0
Vlut[4]->in[3] lut[4]->in[3] 0 
+  0
Vlut[4]->in[4] lut[4]->in[4] 0 
+  0
Vlut[4]->in[5] lut[4]->in[5] 0 
+  0
Xload_inv[8]_no0 lut[4]->out lut[4]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[9]_no0 lut[4]->out lut[4]->out_out[1] gvdd_load 0 inv size=1
***** LUT[5]: logical_block_index[-1], gvdd_index[-1]*****
Xlut[5] lut[5]->in[0] lut[5]->in[1] lut[5]->in[2] lut[5]->in[3] lut[5]->in[4] lut[5]->in[5] lut[5]->out gvdd 0 grid[1][1]_clb[0]_mode[clb]_fle[5]_mode[n1_lut6]_ble6[0]_mode[ble6]_lut6[0]
Vlut[5]->in[0] lut[5]->in[0] 0 
+  0
Vlut[5]->in[1] lut[5]->in[1] 0 
+  0
Vlut[5]->in[2] lut[5]->in[2] 0 
+  0
Vlut[5]->in[3] lut[5]->in[3] 0 
+  0
Vlut[5]->in[4] lut[5]->in[4] 0 
+  0
Vlut[5]->in[5] lut[5]->in[5] 0 
+  0
Xload_inv[10]_no0 lut[5]->out lut[5]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[11]_no0 lut[5]->out lut[5]->out_out[1] gvdd_load 0 inv size=1
***** LUT[6]: logical_block_index[-1], gvdd_index[-1]*****
Xlut[6] lut[6]->in[0] lut[6]->in[1] lut[6]->in[2] lut[6]->in[3] lut[6]->in[4] lut[6]->in[5] lut[6]->out gvdd 0 grid[1][1]_clb[0]_mode[clb]_fle[6]_mode[n1_lut6]_ble6[0]_mode[ble6]_lut6[0]
Vlut[6]->in[0] lut[6]->in[0] 0 
+  0
Vlut[6]->in[1] lut[6]->in[1] 0 
+  0
Vlut[6]->in[2] lut[6]->in[2] 0 
+  0
Vlut[6]->in[3] lut[6]->in[3] 0 
+  0
Vlut[6]->in[4] lut[6]->in[4] 0 
+  0
Vlut[6]->in[5] lut[6]->in[5] 0 
+  0
Xload_inv[12]_no0 lut[6]->out lut[6]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[13]_no0 lut[6]->out lut[6]->out_out[1] gvdd_load 0 inv size=1
***** LUT[7]: logical_block_index[-1], gvdd_index[-1]*****
Xlut[7] lut[7]->in[0] lut[7]->in[1] lut[7]->in[2] lut[7]->in[3] lut[7]->in[4] lut[7]->in[5] lut[7]->out gvdd 0 grid[1][1]_clb[0]_mode[clb]_fle[7]_mode[n1_lut6]_ble6[0]_mode[ble6]_lut6[0]
Vlut[7]->in[0] lut[7]->in[0] 0 
+  0
Vlut[7]->in[1] lut[7]->in[1] 0 
+  0
Vlut[7]->in[2] lut[7]->in[2] 0 
+  0
Vlut[7]->in[3] lut[7]->in[3] 0 
+  0
Vlut[7]->in[4] lut[7]->in[4] 0 
+  0
Vlut[7]->in[5] lut[7]->in[5] 0 
+  0
Xload_inv[14]_no0 lut[7]->out lut[7]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[15]_no0 lut[7]->out lut[7]->out_out[1] gvdd_load 0 inv size=1
***** LUT[8]: logical_block_index[-1], gvdd_index[-1]*****
Xlut[8] lut[8]->in[0] lut[8]->in[1] lut[8]->in[2] lut[8]->in[3] lut[8]->in[4] lut[8]->in[5] lut[8]->out gvdd 0 grid[1][1]_clb[0]_mode[clb]_fle[8]_mode[n1_lut6]_ble6[0]_mode[ble6]_lut6[0]
Vlut[8]->in[0] lut[8]->in[0] 0 
+  0
Vlut[8]->in[1] lut[8]->in[1] 0 
+  0
Vlut[8]->in[2] lut[8]->in[2] 0 
+  0
Vlut[8]->in[3] lut[8]->in[3] 0 
+  0
Vlut[8]->in[4] lut[8]->in[4] 0 
+  0
Vlut[8]->in[5] lut[8]->in[5] 0 
+  0
Xload_inv[16]_no0 lut[8]->out lut[8]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[17]_no0 lut[8]->out lut[8]->out_out[1] gvdd_load 0 inv size=1
***** LUT[9]: logical_block_index[4], gvdd_index[9]*****
Xlut[9] lut[9]->in[0] lut[9]->in[1] lut[9]->in[2] lut[9]->in[3] lut[9]->in[4] lut[9]->in[5] lut[9]->out gvdd 0 grid[1][1]_clb[0]_mode[clb]_fle[9]_mode[n1_lut6]_ble6[0]_mode[ble6]_lut6[0]
Vlut[9]->in[0] lut[9]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vlut[9]->in[1] lut[9]->in[1] 0 
+  0
Vlut[9]->in[2] lut[9]->in[2] 0 
+  0
Vlut[9]->in[3] lut[9]->in[3] 0 
+  0
Vlut[9]->in[4] lut[9]->in[4] 0 
+  0
Vlut[9]->in[5] lut[9]->in[5] 0 
+  0
Xload_inv[18]_no0 lut[9]->out lut[9]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[19]_no0 lut[9]->out lut[9]->out_out[1] gvdd_load 0 inv size=1
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
***** Global VDD for LUTs SRAMs *****
Vgvdd_sram_luts gvdd_sram_luts 0 vsp
***** Global VDD for SRAMs *****
Vgvdd_sram gvdd_sram 0 vsp
***** Global VDD for load inverters *****
Vgvdd_load gvdd_load 0 vsp
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
***** 6 Clock Simulation, accuracy=1e-13 *****
.tran 1e-13  '6*clock_period'
***** Generic Measurements for Circuit Parameters *****
.measure tran leakage_power_sram_luts avg p(Vgvdd_sram_luts) from=0 to='clock_period'
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
.measure tran dynamic_power_sram_luts avg p(Vgvdd_sram_luts) from='clock_period' to='6*clock_period'
.measure tran energy_per_cycle_sram_luts param='dynamic_power_sram_luts*clock_period'
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
.end
