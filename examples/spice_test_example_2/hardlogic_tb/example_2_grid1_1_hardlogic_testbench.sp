*****************************
*     FPGA SPICE Netlist    *
* Description: FPGA Hard Logic Testbench for Design: example_2 *
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
.global gvdd_load
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

***** Global VDD ports of hard_logic *****

***** Global VDD ports of iopad *****

.global gvdd_sram_io
***** Hardlogic[10]: logical_block_index[-1], gvdd_index[-1]*****
Xhardlogic_dff[0] 

***** BEGIN Global ports of SPICE_MODEL(static_dff) *****
+  Set[0]  Reset[0]  clk[0] 
***** END Global ports of SPICE_MODEL(static_dff) *****
+ hardlogic_dff[0]->D[0] 
+ hardlogic_dff[0]->Q[0] 
+ gvdd_dff[0] ggnd 
+ static_dff
Vhardlogic_dff[0]->D[0] hardlogic_dff[0]->D[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Xload_inv[0]_no0 hardlogic_dff[0]->Q[0] hardlogic_dff[0]->Q[0]_out[0] gvdd_load 0 inv size=1
***** Hardlogic[10]: logical_block_index[-1], gvdd_index[-1]*****
Xhardlogic_dff[1] 

***** BEGIN Global ports of SPICE_MODEL(static_dff) *****
+  Set[0]  Reset[0]  clk[0] 
***** END Global ports of SPICE_MODEL(static_dff) *****
+ hardlogic_dff[1]->D[0] 
+ hardlogic_dff[1]->Q[0] 
+ gvdd_dff[1] ggnd 
+ static_dff
Vhardlogic_dff[1]->D[0] hardlogic_dff[1]->D[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Xload_inv[1]_no0 hardlogic_dff[1]->Q[0] hardlogic_dff[1]->Q[0]_out[0] gvdd_load 0 inv size=1
***** Hardlogic[10]: logical_block_index[-1], gvdd_index[-1]*****
Xhardlogic_dff[2] 

***** BEGIN Global ports of SPICE_MODEL(static_dff) *****
+  Set[0]  Reset[0]  clk[0] 
***** END Global ports of SPICE_MODEL(static_dff) *****
+ hardlogic_dff[2]->D[0] 
+ hardlogic_dff[2]->Q[0] 
+ gvdd_dff[2] ggnd 
+ static_dff
Vhardlogic_dff[2]->D[0] hardlogic_dff[2]->D[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Xload_inv[2]_no0 hardlogic_dff[2]->Q[0] hardlogic_dff[2]->Q[0]_out[0] gvdd_load 0 inv size=1
***** Hardlogic[10]: logical_block_index[-1], gvdd_index[-1]*****
Xhardlogic_dff[3] 

***** BEGIN Global ports of SPICE_MODEL(static_dff) *****
+  Set[0]  Reset[0]  clk[0] 
***** END Global ports of SPICE_MODEL(static_dff) *****
+ hardlogic_dff[3]->D[0] 
+ hardlogic_dff[3]->Q[0] 
+ gvdd_dff[3] ggnd 
+ static_dff
Vhardlogic_dff[3]->D[0] hardlogic_dff[3]->D[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Xload_inv[3]_no0 hardlogic_dff[3]->Q[0] hardlogic_dff[3]->Q[0]_out[0] gvdd_load 0 inv size=1
***** Hardlogic[10]: logical_block_index[-1], gvdd_index[-1]*****
Xhardlogic_dff[4] 

***** BEGIN Global ports of SPICE_MODEL(static_dff) *****
+  Set[0]  Reset[0]  clk[0] 
***** END Global ports of SPICE_MODEL(static_dff) *****
+ hardlogic_dff[4]->D[0] 
+ hardlogic_dff[4]->Q[0] 
+ gvdd_dff[4] ggnd 
+ static_dff
Vhardlogic_dff[4]->D[0] hardlogic_dff[4]->D[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Xload_inv[4]_no0 hardlogic_dff[4]->Q[0] hardlogic_dff[4]->Q[0]_out[0] gvdd_load 0 inv size=1
***** Hardlogic[10]: logical_block_index[-1], gvdd_index[-1]*****
Xhardlogic_dff[5] 

***** BEGIN Global ports of SPICE_MODEL(static_dff) *****
+  Set[0]  Reset[0]  clk[0] 
***** END Global ports of SPICE_MODEL(static_dff) *****
+ hardlogic_dff[5]->D[0] 
+ hardlogic_dff[5]->Q[0] 
+ gvdd_dff[5] ggnd 
+ static_dff
Vhardlogic_dff[5]->D[0] hardlogic_dff[5]->D[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Xload_inv[5]_no0 hardlogic_dff[5]->Q[0] hardlogic_dff[5]->Q[0]_out[0] gvdd_load 0 inv size=1
***** Hardlogic[10]: logical_block_index[-1], gvdd_index[-1]*****
Xhardlogic_dff[6] 

***** BEGIN Global ports of SPICE_MODEL(static_dff) *****
+  Set[0]  Reset[0]  clk[0] 
***** END Global ports of SPICE_MODEL(static_dff) *****
+ hardlogic_dff[6]->D[0] 
+ hardlogic_dff[6]->Q[0] 
+ gvdd_dff[6] ggnd 
+ static_dff
Vhardlogic_dff[6]->D[0] hardlogic_dff[6]->D[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Xload_inv[6]_no0 hardlogic_dff[6]->Q[0] hardlogic_dff[6]->Q[0]_out[0] gvdd_load 0 inv size=1
***** Hardlogic[10]: logical_block_index[-1], gvdd_index[-1]*****
Xhardlogic_dff[7] 

***** BEGIN Global ports of SPICE_MODEL(static_dff) *****
+  Set[0]  Reset[0]  clk[0] 
***** END Global ports of SPICE_MODEL(static_dff) *****
+ hardlogic_dff[7]->D[0] 
+ hardlogic_dff[7]->Q[0] 
+ gvdd_dff[7] ggnd 
+ static_dff
Vhardlogic_dff[7]->D[0] hardlogic_dff[7]->D[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Xload_inv[7]_no0 hardlogic_dff[7]->Q[0] hardlogic_dff[7]->Q[0]_out[0] gvdd_load 0 inv size=1
***** Hardlogic[10]: logical_block_index[-1], gvdd_index[-1]*****
Xhardlogic_dff[8] 

***** BEGIN Global ports of SPICE_MODEL(static_dff) *****
+  Set[0]  Reset[0]  clk[0] 
***** END Global ports of SPICE_MODEL(static_dff) *****
+ hardlogic_dff[8]->D[0] 
+ hardlogic_dff[8]->Q[0] 
+ gvdd_dff[8] ggnd 
+ static_dff
Vhardlogic_dff[8]->D[0] hardlogic_dff[8]->D[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Xload_inv[8]_no0 hardlogic_dff[8]->Q[0] hardlogic_dff[8]->Q[0]_out[0] gvdd_load 0 inv size=1
***** Hardlogic[10]: logical_block_index[3], gvdd_index[9]*****
Xhardlogic_dff[9] 

***** BEGIN Global ports of SPICE_MODEL(static_dff) *****
+  Set[0]  Reset[0]  clk[0] 
***** END Global ports of SPICE_MODEL(static_dff) *****
+ hardlogic_dff[9]->D[0] 
+ hardlogic_dff[9]->Q[0] 
+ gvdd_dff[9] ggnd 
+ static_dff
Vhardlogic_dff[9]->D[0] hardlogic_dff[9]->D[0] 0 
+  pulse(vsp 0 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*19.8147*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '19.8147*clock_period')
Xload_inv[9]_no0 hardlogic_dff[9]->Q[0] hardlogic_dff[9]->Q[0]_out[0] gvdd_load 0 inv size=1
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
***** Global VDD for FFs *****
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
***** Global VDD for Hardlogics *****
***** 6 Clock Simulation, accuracy=1e-13 *****
.tran 1e-13  '6*clock_period'
***** Generic Measurements for Circuit Parameters *****
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
