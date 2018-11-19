*****************************
*     FPGA SPICE Netlist    *
* Description: FPGA SPICE Routing MUX Test Bench for Design: example_1 *
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
Xmux_2level_size5[0] mux_2level_size5[0]->in[0] mux_2level_size5[0]->in[1] mux_2level_size5[0]->in[2] mux_2level_size5[0]->in[3] mux_2level_size5[0]->in[4] mux_2level_size5[0]->out sram[0]->out sram[0]->outb sram[1]->outb sram[1]->out sram[2]->out sram[2]->outb sram[3]->outb sram[3]->out sram[4]->out sram[4]->outb sram[5]->out sram[5]->outb gvdd_mux_2level_size5[0] 0 mux_2level_size5
***** SRAM bits for MUX[0], level=2, select_path_id=3. *****
*****010100*****
Xsram[0] sram->in sram[0]->out sram[0]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[0]->out) 0
.nodeset V(sram[0]->outb) vsp
Xsram[1] sram->in sram[1]->out sram[1]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[1]->out) 0
.nodeset V(sram[1]->outb) vsp
Xsram[2] sram->in sram[2]->out sram[2]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[2]->out) 0
.nodeset V(sram[2]->outb) vsp
Xsram[3] sram->in sram[3]->out sram[3]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[3]->out) 0
.nodeset V(sram[3]->outb) vsp
Xsram[4] sram->in sram[4]->out sram[4]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[4]->out) 0
.nodeset V(sram[4]->outb) vsp
Xsram[5] sram->in sram[5]->out sram[5]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[5]->out) 0
.nodeset V(sram[5]->outb) vsp
***** Signal mux_2level_size5[0]->in[0] density = 0, probability=0.*****
Vmux_2level_size5[0]->in[0] mux_2level_size5[0]->in[0] 0 
+  0
***** Signal mux_2level_size5[0]->in[1] density = 0, probability=0.*****
Vmux_2level_size5[0]->in[1] mux_2level_size5[0]->in[1] 0 
+  0
***** Signal mux_2level_size5[0]->in[2] density = 0, probability=0.*****
Vmux_2level_size5[0]->in[2] mux_2level_size5[0]->in[2] 0 
+  0
***** Signal mux_2level_size5[0]->in[3] density = 0.1906, probability=0.4782.*****
Vmux_2level_size5[0]->in[3] mux_2level_size5[0]->in[3] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4782*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
***** Signal mux_2level_size5[0]->in[4] density = 0.1906, probability=0.5218.*****
Vmux_2level_size5[0]->in[4] mux_2level_size5[0]->in[4] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5218*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
Vgvdd_mux_2level_size5[0] gvdd_mux_2level_size5[0] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar trig v(mux_2level_size5[0]->in[3]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_size5[0]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar trig v(mux_2level_size5[0]->in[3]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_size5[0]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar when v(mux_2level_size5[0]->in[3])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar trig v(mux_2level_size5[0]->in[3]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_size5[0]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar when v(mux_2level_size5[0]->in[3])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar trig v(mux_2level_size5[0]->in[3]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_size5[0]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_size5[0]_leakage_power avg p(Vgvdd_mux_2level_size5[0]) from=0 to='clock_period'
.meas tran leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar param='mux_2level_size5[0]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_size5[0]_dynamic_power avg p(Vgvdd_mux_2level_size5[0]) from='clock_period' to='7*clock_period'
.meas tran mux_2level_size5[0]_energy_per_cycle param='mux_2level_size5[0]_dynamic_power*clock_period'
.meas tran dynamic_power_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar  param='mux_2level_size5[0]_dynamic_power'
.meas tran energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar  param='dynamic_power_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar*clock_period'
.meas tran dynamic_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar avg p(Vgvdd_mux_2level_size5[0]) from='start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar' to='start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar+switch_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar'
.meas tran dynamic_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar avg p(Vgvdd_mux_2level_size5[0]) from='start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar' to='start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar+switch_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar'
.meas tran sum_leakage_power_mux[0to0] 
+          param='leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar'
.meas tran sum_energy_per_cycle_mux[0to0] 
+          param='energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar'
Xload_inv[0]_no0 mux_2level_size5[0]->out mux_2level_size5[0]->out_out[0] gvdd_load 0 inv size=5
.meas tran sum_leakage_power_pb_mux[0to0] 
+          param='leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar'
.meas tran sum_energy_per_cycle_pb_mux[0to0] 
+          param='energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[0]_crossbar'
Xmux_2level_size5[1] mux_2level_size5[1]->in[0] mux_2level_size5[1]->in[1] mux_2level_size5[1]->in[2] mux_2level_size5[1]->in[3] mux_2level_size5[1]->in[4] mux_2level_size5[1]->out sram[6]->outb sram[6]->out sram[7]->out sram[7]->outb sram[8]->out sram[8]->outb sram[9]->outb sram[9]->out sram[10]->out sram[10]->outb sram[11]->out sram[11]->outb gvdd_mux_2level_size5[1] 0 mux_2level_size5
***** SRAM bits for MUX[1], level=2, select_path_id=0. *****
*****100100*****
Xsram[6] sram->in sram[6]->out sram[6]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[6]->out) 0
.nodeset V(sram[6]->outb) vsp
Xsram[7] sram->in sram[7]->out sram[7]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[7]->out) 0
.nodeset V(sram[7]->outb) vsp
Xsram[8] sram->in sram[8]->out sram[8]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[8]->out) 0
.nodeset V(sram[8]->outb) vsp
Xsram[9] sram->in sram[9]->out sram[9]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[9]->out) 0
.nodeset V(sram[9]->outb) vsp
Xsram[10] sram->in sram[10]->out sram[10]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[10]->out) 0
.nodeset V(sram[10]->outb) vsp
Xsram[11] sram->in sram[11]->out sram[11]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[11]->out) 0
.nodeset V(sram[11]->outb) vsp
***** Signal mux_2level_size5[1]->in[0] density = 0, probability=0.*****
Vmux_2level_size5[1]->in[0] mux_2level_size5[1]->in[0] 0 
+  0
***** Signal mux_2level_size5[1]->in[1] density = 0, probability=0.*****
Vmux_2level_size5[1]->in[1] mux_2level_size5[1]->in[1] 0 
+  0
***** Signal mux_2level_size5[1]->in[2] density = 0, probability=0.*****
Vmux_2level_size5[1]->in[2] mux_2level_size5[1]->in[2] 0 
+  0
***** Signal mux_2level_size5[1]->in[3] density = 0.1906, probability=0.4782.*****
Vmux_2level_size5[1]->in[3] mux_2level_size5[1]->in[3] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4782*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
***** Signal mux_2level_size5[1]->in[4] density = 0.1906, probability=0.5218.*****
Vmux_2level_size5[1]->in[4] mux_2level_size5[1]->in[4] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5218*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
Vgvdd_mux_2level_size5[1] gvdd_mux_2level_size5[1] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar trig v(mux_2level_size5[1]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_size5[1]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar trig v(mux_2level_size5[1]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_size5[1]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar when v(mux_2level_size5[1]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar trig v(mux_2level_size5[1]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_size5[1]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar when v(mux_2level_size5[1]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar trig v(mux_2level_size5[1]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_size5[1]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_size5[1]_leakage_power avg p(Vgvdd_mux_2level_size5[1]) from=0 to='clock_period'
.meas tran leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar param='mux_2level_size5[1]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_size5[1]_dynamic_power avg p(Vgvdd_mux_2level_size5[1]) from='clock_period' to='7*clock_period'
.meas tran mux_2level_size5[1]_energy_per_cycle param='mux_2level_size5[1]_dynamic_power*clock_period'
.meas tran dynamic_power_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar  param='mux_2level_size5[1]_dynamic_power'
.meas tran energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar  param='dynamic_power_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar*clock_period'
.meas tran dynamic_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar avg p(Vgvdd_mux_2level_size5[1]) from='start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar' to='start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar+switch_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar'
.meas tran dynamic_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar avg p(Vgvdd_mux_2level_size5[1]) from='start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar' to='start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar+switch_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar'
.meas tran sum_leakage_power_mux[0to1] 
+          param='sum_leakage_power_mux[0to0]+leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar'
.meas tran sum_energy_per_cycle_mux[0to1] 
+          param='sum_energy_per_cycle_mux[0to0]+energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar'
Xload_inv[1]_no0 mux_2level_size5[1]->out mux_2level_size5[1]->out_out[0] gvdd_load 0 inv size=5
.meas tran sum_leakage_power_pb_mux[0to1] 
+          param='sum_leakage_power_pb_mux[0to0]+leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar'
.meas tran sum_energy_per_cycle_pb_mux[0to1] 
+          param='sum_energy_per_cycle_pb_mux[0to0]+energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[1]_crossbar'
Xmux_2level_size5[2] mux_2level_size5[2]->in[0] mux_2level_size5[2]->in[1] mux_2level_size5[2]->in[2] mux_2level_size5[2]->in[3] mux_2level_size5[2]->in[4] mux_2level_size5[2]->out sram[12]->outb sram[12]->out sram[13]->out sram[13]->outb sram[14]->out sram[14]->outb sram[15]->outb sram[15]->out sram[16]->out sram[16]->outb sram[17]->out sram[17]->outb gvdd_mux_2level_size5[2] 0 mux_2level_size5
***** SRAM bits for MUX[2], level=2, select_path_id=0. *****
*****100100*****
Xsram[12] sram->in sram[12]->out sram[12]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[12]->out) 0
.nodeset V(sram[12]->outb) vsp
Xsram[13] sram->in sram[13]->out sram[13]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[13]->out) 0
.nodeset V(sram[13]->outb) vsp
Xsram[14] sram->in sram[14]->out sram[14]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[14]->out) 0
.nodeset V(sram[14]->outb) vsp
Xsram[15] sram->in sram[15]->out sram[15]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[15]->out) 0
.nodeset V(sram[15]->outb) vsp
Xsram[16] sram->in sram[16]->out sram[16]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[16]->out) 0
.nodeset V(sram[16]->outb) vsp
Xsram[17] sram->in sram[17]->out sram[17]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[17]->out) 0
.nodeset V(sram[17]->outb) vsp
***** Signal mux_2level_size5[2]->in[0] density = 0, probability=0.*****
Vmux_2level_size5[2]->in[0] mux_2level_size5[2]->in[0] 0 
+  0
***** Signal mux_2level_size5[2]->in[1] density = 0, probability=0.*****
Vmux_2level_size5[2]->in[1] mux_2level_size5[2]->in[1] 0 
+  0
***** Signal mux_2level_size5[2]->in[2] density = 0, probability=0.*****
Vmux_2level_size5[2]->in[2] mux_2level_size5[2]->in[2] 0 
+  0
***** Signal mux_2level_size5[2]->in[3] density = 0.1906, probability=0.4782.*****
Vmux_2level_size5[2]->in[3] mux_2level_size5[2]->in[3] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4782*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
***** Signal mux_2level_size5[2]->in[4] density = 0.1906, probability=0.5218.*****
Vmux_2level_size5[2]->in[4] mux_2level_size5[2]->in[4] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5218*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
Vgvdd_mux_2level_size5[2] gvdd_mux_2level_size5[2] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar trig v(mux_2level_size5[2]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_size5[2]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar trig v(mux_2level_size5[2]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_size5[2]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar when v(mux_2level_size5[2]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar trig v(mux_2level_size5[2]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_size5[2]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar when v(mux_2level_size5[2]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar trig v(mux_2level_size5[2]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_size5[2]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_size5[2]_leakage_power avg p(Vgvdd_mux_2level_size5[2]) from=0 to='clock_period'
.meas tran leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar param='mux_2level_size5[2]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_size5[2]_dynamic_power avg p(Vgvdd_mux_2level_size5[2]) from='clock_period' to='7*clock_period'
.meas tran mux_2level_size5[2]_energy_per_cycle param='mux_2level_size5[2]_dynamic_power*clock_period'
.meas tran dynamic_power_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar  param='mux_2level_size5[2]_dynamic_power'
.meas tran energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar  param='dynamic_power_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar*clock_period'
.meas tran dynamic_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar avg p(Vgvdd_mux_2level_size5[2]) from='start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar' to='start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar+switch_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar'
.meas tran dynamic_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar avg p(Vgvdd_mux_2level_size5[2]) from='start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar' to='start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar+switch_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar'
.meas tran sum_leakage_power_mux[0to2] 
+          param='sum_leakage_power_mux[0to1]+leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar'
.meas tran sum_energy_per_cycle_mux[0to2] 
+          param='sum_energy_per_cycle_mux[0to1]+energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar'
Xload_inv[2]_no0 mux_2level_size5[2]->out mux_2level_size5[2]->out_out[0] gvdd_load 0 inv size=5
.meas tran sum_leakage_power_pb_mux[0to2] 
+          param='sum_leakage_power_pb_mux[0to1]+leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar'
.meas tran sum_energy_per_cycle_pb_mux[0to2] 
+          param='sum_energy_per_cycle_pb_mux[0to1]+energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[2]_crossbar'
Xmux_2level_size5[3] mux_2level_size5[3]->in[0] mux_2level_size5[3]->in[1] mux_2level_size5[3]->in[2] mux_2level_size5[3]->in[3] mux_2level_size5[3]->in[4] mux_2level_size5[3]->out sram[18]->outb sram[18]->out sram[19]->out sram[19]->outb sram[20]->out sram[20]->outb sram[21]->outb sram[21]->out sram[22]->out sram[22]->outb sram[23]->out sram[23]->outb gvdd_mux_2level_size5[3] 0 mux_2level_size5
***** SRAM bits for MUX[3], level=2, select_path_id=0. *****
*****100100*****
Xsram[18] sram->in sram[18]->out sram[18]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[18]->out) 0
.nodeset V(sram[18]->outb) vsp
Xsram[19] sram->in sram[19]->out sram[19]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[19]->out) 0
.nodeset V(sram[19]->outb) vsp
Xsram[20] sram->in sram[20]->out sram[20]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[20]->out) 0
.nodeset V(sram[20]->outb) vsp
Xsram[21] sram->in sram[21]->out sram[21]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[21]->out) 0
.nodeset V(sram[21]->outb) vsp
Xsram[22] sram->in sram[22]->out sram[22]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[22]->out) 0
.nodeset V(sram[22]->outb) vsp
Xsram[23] sram->in sram[23]->out sram[23]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[23]->out) 0
.nodeset V(sram[23]->outb) vsp
***** Signal mux_2level_size5[3]->in[0] density = 0, probability=0.*****
Vmux_2level_size5[3]->in[0] mux_2level_size5[3]->in[0] 0 
+  0
***** Signal mux_2level_size5[3]->in[1] density = 0, probability=0.*****
Vmux_2level_size5[3]->in[1] mux_2level_size5[3]->in[1] 0 
+  0
***** Signal mux_2level_size5[3]->in[2] density = 0, probability=0.*****
Vmux_2level_size5[3]->in[2] mux_2level_size5[3]->in[2] 0 
+  0
***** Signal mux_2level_size5[3]->in[3] density = 0.1906, probability=0.4782.*****
Vmux_2level_size5[3]->in[3] mux_2level_size5[3]->in[3] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4782*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
***** Signal mux_2level_size5[3]->in[4] density = 0.1906, probability=0.5218.*****
Vmux_2level_size5[3]->in[4] mux_2level_size5[3]->in[4] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5218*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
Vgvdd_mux_2level_size5[3] gvdd_mux_2level_size5[3] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar trig v(mux_2level_size5[3]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_size5[3]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar trig v(mux_2level_size5[3]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_size5[3]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar when v(mux_2level_size5[3]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar trig v(mux_2level_size5[3]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_size5[3]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar when v(mux_2level_size5[3]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar trig v(mux_2level_size5[3]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_size5[3]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_size5[3]_leakage_power avg p(Vgvdd_mux_2level_size5[3]) from=0 to='clock_period'
.meas tran leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar param='mux_2level_size5[3]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_size5[3]_dynamic_power avg p(Vgvdd_mux_2level_size5[3]) from='clock_period' to='7*clock_period'
.meas tran mux_2level_size5[3]_energy_per_cycle param='mux_2level_size5[3]_dynamic_power*clock_period'
.meas tran dynamic_power_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar  param='mux_2level_size5[3]_dynamic_power'
.meas tran energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar  param='dynamic_power_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar*clock_period'
.meas tran dynamic_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar avg p(Vgvdd_mux_2level_size5[3]) from='start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar' to='start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar+switch_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar'
.meas tran dynamic_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar avg p(Vgvdd_mux_2level_size5[3]) from='start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar' to='start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar+switch_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar'
.meas tran sum_leakage_power_mux[0to3] 
+          param='sum_leakage_power_mux[0to2]+leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar'
.meas tran sum_energy_per_cycle_mux[0to3] 
+          param='sum_energy_per_cycle_mux[0to2]+energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar'
Xload_inv[3]_no0 mux_2level_size5[3]->out mux_2level_size5[3]->out_out[0] gvdd_load 0 inv size=5
.meas tran sum_leakage_power_pb_mux[0to3] 
+          param='sum_leakage_power_pb_mux[0to2]+leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar'
.meas tran sum_energy_per_cycle_pb_mux[0to3] 
+          param='sum_energy_per_cycle_pb_mux[0to2]+energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_in[3]_crossbar'
Xmux_1level_tapbuf_size2[4] mux_1level_tapbuf_size2[4]->in[0] mux_1level_tapbuf_size2[4]->in[1] mux_1level_tapbuf_size2[4]->out sram[24]->outb sram[24]->out gvdd_mux_1level_tapbuf_size2[4] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[4], level=1, select_path_id=0. *****
*****1*****
Xsram[24] sram->in sram[24]->out sram[24]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[24]->out) 0
.nodeset V(sram[24]->outb) vsp
***** Signal mux_1level_tapbuf_size2[4]->in[0] density = 0.1906, probability=0.5218.*****
Vmux_1level_tapbuf_size2[4]->in[0] mux_1level_tapbuf_size2[4]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5218*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
***** Signal mux_1level_tapbuf_size2[4]->in[1] density = 0.099455, probability=0.5218.*****
Vmux_1level_tapbuf_size2[4]->in[1] mux_1level_tapbuf_size2[4]->in[1] 0 
+  pulse(vsp 0 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5218*20.1096*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '20.1096*clock_period')
Vgvdd_mux_1level_tapbuf_size2[4] gvdd_mux_1level_tapbuf_size2[4] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1 trig v(mux_1level_tapbuf_size2[4]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[4]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1 trig v(mux_1level_tapbuf_size2[4]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[4]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1 when v(mux_1level_tapbuf_size2[4]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1 trig v(mux_1level_tapbuf_size2[4]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[4]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1 when v(mux_1level_tapbuf_size2[4]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1 trig v(mux_1level_tapbuf_size2[4]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[4]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[4]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[4]) from=0 to='clock_period'
.meas tran leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1 param='mux_1level_tapbuf_size2[4]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[4]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[4]) from='clock_period' to='7*clock_period'
.meas tran mux_1level_tapbuf_size2[4]_energy_per_cycle param='mux_1level_tapbuf_size2[4]_dynamic_power*clock_period'
.meas tran dynamic_power_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1  param='mux_1level_tapbuf_size2[4]_dynamic_power'
.meas tran energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1  param='dynamic_power_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1*clock_period'
.meas tran dynamic_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1 avg p(Vgvdd_mux_1level_tapbuf_size2[4]) from='start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1' to='start_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1+switch_rise_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1'
.meas tran dynamic_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1 avg p(Vgvdd_mux_1level_tapbuf_size2[4]) from='start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1' to='start_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1+switch_fall_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1'
.meas tran sum_leakage_power_mux[0to4] 
+          param='sum_leakage_power_mux[0to3]+leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1'
.meas tran sum_energy_per_cycle_mux[0to4] 
+          param='sum_energy_per_cycle_mux[0to3]+energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1'
Xload_inv[4]_no0 mux_1level_tapbuf_size2[4]->out mux_1level_tapbuf_size2[4]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[5]_no0 mux_1level_tapbuf_size2[4]->out mux_1level_tapbuf_size2[4]->out_out[1] gvdd_load 0 inv size=1
Xload_inv[6]_no0 mux_1level_tapbuf_size2[4]->out mux_1level_tapbuf_size2[4]->out_out[2] gvdd_load 0 inv size=1
Xload_inv[7]_no0 mux_1level_tapbuf_size2[4]->out mux_1level_tapbuf_size2[4]->out_out[3] gvdd_load 0 inv size=1
Xload_inv[8]_no0 mux_1level_tapbuf_size2[4]->out load_inv[8]_out gvdd_load 0 inv size=1
Xload_inv[9]_no0 mux_1level_tapbuf_size2[4]->out load_inv[9]_out gvdd_load 0 inv size=1
Xload_inv[10]_no0 mux_1level_tapbuf_size2[4]->out load_inv[10]_out gvdd_load 0 inv size=1
Xload_inv[11]_no0 mux_1level_tapbuf_size2[4]->out load_inv[11]_out gvdd_load 0 inv size=1
.meas tran sum_leakage_power_pb_mux[0to4] 
+          param='sum_leakage_power_pb_mux[0to3]+leakage_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1'
.meas tran sum_energy_per_cycle_pb_mux[0to4] 
+          param='sum_energy_per_cycle_pb_mux[0to3]+energy_per_cycle_grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_out[0]_mux1'
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
***** 7 Clock Simulation, accuracy=1e-13 *****
.tran 1e-13  '7*clock_period'
***** Generic Measurements for Circuit Parameters *****
.meas tran total_leakage_srams avg p(Vgvdd_sram) from=0 to='clock_period'
.meas tran total_dynamic_srams avg p(Vgvdd_sram) from='clock_period' to='7*clock_period'
.meas tran total_energy_per_cycle_srams param='total_dynamic_srams*clock_period'
.meas tran total_leakage_power_mux[0to4] 
+          param='sum_leakage_power_mux[0to4]'
.meas tran total_energy_per_cycle_mux[0to4] 
+          param='sum_energy_per_cycle_mux[0to4]'
.meas tran total_leakage_power_pb_mux 
+          param='sum_leakage_power_pb_mux[0to4]'
.meas tran total_energy_per_cycle_pb_mux 
+          param='sum_energy_per_cycle_pb_mux[0to4]'
.end
