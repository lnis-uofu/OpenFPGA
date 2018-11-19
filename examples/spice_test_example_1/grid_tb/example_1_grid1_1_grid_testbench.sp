*****************************
*     FPGA SPICE Netlist    *
* Description: FPGA Grid Testbench for Design: example_1 *
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
.global gvdd_local_interc gvdd_io gvdd_hardlogic
.global gvdd_sram_local_routing
.global gvdd_sram_luts
.global gvdd_sram_io
***** Global VDD ports of Look-Up Table *****
.global 
+ gvdd_lut4[0]

***** Global VDD ports of Flip-flop *****
.global 
+ gvdd_dff[0]

***** Global VDD ports of iopad *****

***** Global VDD ports of hard_logic *****

Xgrid[1][1] 
+ grid[1][1]_pin[0][0][0] 
+ grid[1][1]_pin[0][0][4] 
+ grid[1][1]_pin[0][1][1] 
+ grid[1][1]_pin[0][1][5] 
+ grid[1][1]_pin[0][2][2] 
+ grid[1][1]_pin[0][3][3] 
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
Vgvdd_lut4[0] gvdd_lut4[0] 0 vsp
Rgvdd_lut4[0]_huge gvdd_lut4[0] 0 'vsp/10e-15'
***** Global VDD for Flip-flops (FFs) *****
Vgvdd_dff[0] gvdd_dff[0] 0 vsp
Rgvdd_dff[0]_huge gvdd_dff[0] 0 'vsp/10e-15'
Vgrid[1][1]_pin[0][0][0] grid[1][1]_pin[0][0][0] 0 
+  0
Xgrid[1][1]_pin[0][0][4]_inv[0] grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[0] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][4]_inv[1] grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[1] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][4]_inv[2] grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[2] gvdd_load 0 inv size=1
Xgrid[1][1]_pin[0][0][4]_inv[3] grid[1][1]_pin[0][0][4] grid[1][1]_pin[0][0][4]_out[3] gvdd_load 0 inv size=1
Vgrid[1][1]_pin[0][1][1] grid[1][1]_pin[0][1][1] 0 
+  0
Vgrid[1][1]_pin[0][1][5] grid[1][1]_pin[0][1][5] 0 
+  0
Vgrid[1][1]_pin[0][2][2] grid[1][1]_pin[0][2][2] 0 
+  0
Vgrid[1][1]_pin[0][3][3] grid[1][1]_pin[0][3][3] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4782*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
***** 7 Clock Simulation, accuracy=1e-13 *****
.tran 1e-13  '7*clock_period'
***** Generic Measurements for Circuit Parameters *****
.measure tran leakage_power_sram_local_routing avg p(Vgvdd_sram_local_routing) from=0 to='clock_period'
.measure tran leakage_power_sram_luts avg p(Vgvdd_sram_luts) from=0 to='clock_period'
.measure tran leakage_power_local_routing avg p(Vgvdd_local_interc) from=0 to='clock_period'
.measure tran leakage_power_lut4[0] avg p(Vgvdd_lut4[0]) from=0 to='clock_period'
.measure tran leakage_power_lut4[0to0] 
+ param = 'leakage_power_lut4[0]'
.measure tran total_leakage_power_lut4 
+ param = 'leakage_power_lut4[0to0]'
.measure tran leakage_power_dff[0] avg p(Vgvdd_dff[0]) from=0 to='clock_period'
.measure tran leakage_power_dff[0to0] 
+ param = 'leakage_power_dff[0]'
.measure tran total_leakage_power_dff 
+ param = 'leakage_power_dff[0to0]'
.measure tran dynamic_power_sram_local_routing avg p(Vgvdd_sram_local_routing) from='clock_period' to='7*clock_period'
.measure tran total_energy_per_cycle_sram_local_routing param='dynamic_power_sram_local_routing*clock_period'
.measure tran dynamic_power_sram_luts avg p(Vgvdd_sram_luts) from='clock_period' to='7*clock_period'
.measure tran total_energy_per_cycle_sram_luts param='dynamic_power_sram_luts*clock_period'
.measure tran dynamic_power_local_interc avg p(Vgvdd_local_interc) from='clock_period' to='7*clock_period'
.measure tran total_energy_per_cycle_local_routing param='dynamic_power_local_interc*clock_period'
.measure tran dynamic_power_lut4[0] avg p(Vgvdd_lut4[0]) from='clock_period' to='7*clock_period'
.measure tran dynamic_power_lut4[0to0] 
+ param = 'dynamic_power_lut4[0]'
.measure tran total_dynamic_power_lut4 
+ param = 'dynamic_power_lut4[0to0]'
.measure tran total_energy_per_cycle_lut4 
+ param = 'dynamic_power_lut4[0to0]*clock_period'
.measure tran dynamic_power_dff[0] avg p(Vgvdd_dff[0]) from='clock_period' to='7*clock_period'
.measure tran dynamic_power_dff[0to0] 
+ param = 'dynamic_power_dff[0]'
.measure tran total_dynamic_power_dff 
+ param = 'dynamic_power_dff[0to0]'
.measure tran total_energy_per_cycle_dff 
+ param = 'dynamic_power_dff[0to0]*clock_period'
.end
