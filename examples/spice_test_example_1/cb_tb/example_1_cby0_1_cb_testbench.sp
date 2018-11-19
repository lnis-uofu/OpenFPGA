*****************************
*     FPGA SPICE Netlist    *
* Description: FPGA SPICE Connection Box Testbench Bench for Design: example_1 *
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
.global gvdd_sram_cbs
****** Include subckt netlists: Connection Box Y-channel  [0][1] *****
.include './spice_test_example_1/subckt/cby_0_1.sp'
***** Call defined Connection Box[0][1] *****
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
+ grid[1][1]_pin[0][3][3] 
+ grid[0][1]_pin[0][1][0] 
+ grid[0][1]_pin[0][1][2] 
+ grid[0][1]_pin[0][1][4] 
+ grid[0][1]_pin[0][1][6] 
+ grid[0][1]_pin[0][1][8] 
+ grid[0][1]_pin[0][1][10] 
+ grid[0][1]_pin[0][1][12] 
+ grid[0][1]_pin[0][1][14] 
+ gvdd_cby[0][1] 0 cby[0][1]
***** Signal chany[0][1]_midout[0] density = 0, probability=0.*****
Vchany[0][1]_midout[0] chany[0][1]_midout[0] 0 
+  0
***** Signal chany[0][1]_midout[1] density = 0, probability=0.*****
Vchany[0][1]_midout[1] chany[0][1]_midout[1] 0 
+  0
***** Signal chany[0][1]_midout[2] density = 0, probability=0.*****
Vchany[0][1]_midout[2] chany[0][1]_midout[2] 0 
+  0
***** Signal chany[0][1]_midout[3] density = 0, probability=0.*****
Vchany[0][1]_midout[3] chany[0][1]_midout[3] 0 
+  0
***** Signal chany[0][1]_midout[4] density = 0, probability=0.*****
Vchany[0][1]_midout[4] chany[0][1]_midout[4] 0 
+  0
***** Signal chany[0][1]_midout[5] density = 0, probability=0.*****
Vchany[0][1]_midout[5] chany[0][1]_midout[5] 0 
+  0
***** Signal chany[0][1]_midout[6] density = 0, probability=0.*****
Vchany[0][1]_midout[6] chany[0][1]_midout[6] 0 
+  0
***** Signal chany[0][1]_midout[7] density = 0, probability=0.*****
Vchany[0][1]_midout[7] chany[0][1]_midout[7] 0 
+  0
***** Signal chany[0][1]_midout[8] density = 0, probability=0.*****
Vchany[0][1]_midout[8] chany[0][1]_midout[8] 0 
+  0
***** Signal chany[0][1]_midout[9] density = 0, probability=0.*****
Vchany[0][1]_midout[9] chany[0][1]_midout[9] 0 
+  0
***** Signal chany[0][1]_midout[10] density = 0, probability=0.*****
Vchany[0][1]_midout[10] chany[0][1]_midout[10] 0 
+  0
***** Signal chany[0][1]_midout[11] density = 0, probability=0.*****
Vchany[0][1]_midout[11] chany[0][1]_midout[11] 0 
+  0
***** Signal chany[0][1]_midout[12] density = 0, probability=0.*****
Vchany[0][1]_midout[12] chany[0][1]_midout[12] 0 
+  0
***** Signal chany[0][1]_midout[13] density = 0, probability=0.*****
Vchany[0][1]_midout[13] chany[0][1]_midout[13] 0 
+  0
***** Signal chany[0][1]_midout[14] density = 0, probability=0.*****
Vchany[0][1]_midout[14] chany[0][1]_midout[14] 0 
+  0
***** Signal chany[0][1]_midout[15] density = 0, probability=0.*****
Vchany[0][1]_midout[15] chany[0][1]_midout[15] 0 
+  0
***** Signal chany[0][1]_midout[16] density = 0, probability=0.*****
Vchany[0][1]_midout[16] chany[0][1]_midout[16] 0 
+  0
***** Signal chany[0][1]_midout[17] density = 0, probability=0.*****
Vchany[0][1]_midout[17] chany[0][1]_midout[17] 0 
+  0
***** Signal chany[0][1]_midout[18] density = 0, probability=0.*****
Vchany[0][1]_midout[18] chany[0][1]_midout[18] 0 
+  0
***** Signal chany[0][1]_midout[19] density = 0, probability=0.*****
Vchany[0][1]_midout[19] chany[0][1]_midout[19] 0 
+  0
***** Signal chany[0][1]_midout[20] density = 0, probability=0.*****
Vchany[0][1]_midout[20] chany[0][1]_midout[20] 0 
+  0
***** Signal chany[0][1]_midout[21] density = 0, probability=0.*****
Vchany[0][1]_midout[21] chany[0][1]_midout[21] 0 
+  0
***** Signal chany[0][1]_midout[22] density = 0, probability=0.*****
Vchany[0][1]_midout[22] chany[0][1]_midout[22] 0 
+  0
***** Signal chany[0][1]_midout[23] density = 0, probability=0.*****
Vchany[0][1]_midout[23] chany[0][1]_midout[23] 0 
+  0
***** Signal chany[0][1]_midout[24] density = 0, probability=0.*****
Vchany[0][1]_midout[24] chany[0][1]_midout[24] 0 
+  0
***** Signal chany[0][1]_midout[25] density = 0, probability=0.*****
Vchany[0][1]_midout[25] chany[0][1]_midout[25] 0 
+  0
***** Signal chany[0][1]_midout[26] density = 0, probability=0.*****
Vchany[0][1]_midout[26] chany[0][1]_midout[26] 0 
+  0
***** Signal chany[0][1]_midout[27] density = 0.1906, probability=0.4782.*****
Vchany[0][1]_midout[27] chany[0][1]_midout[27] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4782*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
***** Signal chany[0][1]_midout[28] density = 0, probability=0.*****
Vchany[0][1]_midout[28] chany[0][1]_midout[28] 0 
+  0
***** Signal chany[0][1]_midout[29] density = 0, probability=0.*****
Vchany[0][1]_midout[29] chany[0][1]_midout[29] 0 
+  0
******* Normal TYPE loads *******
Xload_inv[0]_no0 grid[1][1]_pin[0][3][3] grid[1][1]_pin[0][3][3]_out[0] gvdd_load 0 inv size=1
Xload_inv[1]_no0 grid[1][1]_pin[0][3][3] grid[1][1]_pin[0][3][3]_out[1] gvdd_load 0 inv size=1
Xload_inv[2]_no0 grid[1][1]_pin[0][3][3] grid[1][1]_pin[0][3][3]_out[2] gvdd_load 0 inv size=1
Xload_inv[3]_no0 grid[1][1]_pin[0][3][3] grid[1][1]_pin[0][3][3]_out[3] gvdd_load 0 inv size=1
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

******* IO_TYPE loads *******
******* END loads *******

***** Voltage supplies *****
***** Voltage supplies *****
Vgvdd_cb[0][1] gvdd_cby[0][1] 0 vsp
Vgvdd_sram_cbs gvdd_sram_cbs 0 vsp
***** 7 Clock Simulation, accuracy=1e-13 *****
.tran 1e-13  '7*clock_period'
***** Generic Measurements for Circuit Parameters *****
***** Measurements *****
***** Leakage Power Measurement *****
.meas tran leakage_power_cb avg p(Vgvdd_cb[0][1]) from=0 to='clock_period'
.meas tran leakage_power_sram_cb avg p(Vgvdd_sram_cbs) from=0 to='clock_period'
***** Dynamic Power Measurement *****
.meas tran dynamic_power_cb avg p(Vgvdd_cb[0][1]) from='clock_period' to='7*clock_period'
.meas tran energy_per_cycle_cb param='dynamic_power_cb*clock_period'
.meas tran dynamic_power_sram_cb avg p(Vgvdd_sram_cbs) from='clock_period' to='7*clock_period'
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
