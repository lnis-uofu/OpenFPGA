*****************************
*     FPGA SPICE Netlist    *
* Description: FPGA LUT Testbench for Design: example_1 *
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
****** Include subckt netlists: Look-Up Tables (LUTs) *****
.include './spice_test_example_1/subckt/luts.sp'
****** Include subckt netlists: Grid[1][1] *****
.include './spice_test_example_1/subckt/grid_1_1.sp'
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
+ gvdd_lut4[0]

***** LUT[0]: logical_block_index[4], gvdd_index[0]*****
Xlut[0] lut[0]->in[0] lut[0]->in[1] lut[0]->in[2] lut[0]->in[3] lut[0]->out gvdd 0 grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_lut4[0]
Vlut[0]->in[0] lut[0]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4782*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
Vlut[0]->in[1] lut[0]->in[1] 0 
+  0
Vlut[0]->in[2] lut[0]->in[2] 0 
+  0
Vlut[0]->in[3] lut[0]->in[3] 0 
+  0
Xload_inv[0]_no0 lut[0]->out lut[0]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[1]_no0 lut[0]->out lut[0]->out_out[1] gvdd_load 0 inv size=1
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
Vgvdd_lut4[0] gvdd_lut4[0] 0 vsp
Rgvdd_lut4[0]_huge gvdd_lut4[0] 0 'vsp/10e-15'
***** 7 Clock Simulation, accuracy=1e-13 *****
.tran 1e-13  '7*clock_period'
***** Generic Measurements for Circuit Parameters *****
.measure tran leakage_power_sram_luts avg p(Vgvdd_sram_luts) from=0 to='clock_period'
.measure tran leakage_power_lut4[0] avg p(Vgvdd_lut4[0]) from=0 to='clock_period'
.measure tran leakage_power_lut4[0to0] 
+ param = 'leakage_power_lut4[0]'
.measure tran total_leakage_power_lut4 
+ param = 'leakage_power_lut4[0to0]'
.measure tran dynamic_power_sram_luts avg p(Vgvdd_sram_luts) from='clock_period' to='7*clock_period'
.measure tran energy_per_cycle_sram_luts param='dynamic_power_sram_luts*clock_period'
.measure tran dynamic_power_lut4[0] avg p(Vgvdd_lut4[0]) from='clock_period' to='7*clock_period'
.measure tran dynamic_power_lut4[0to0] 
+ param = 'dynamic_power_lut4[0]'
.measure tran total_dynamic_power_lut4 
+ param = 'dynamic_power_lut4[0to0]'
.measure tran total_energy_per_cycle_lut4 
+ param = 'dynamic_power_lut4[0to0]*clock_period'
.end
