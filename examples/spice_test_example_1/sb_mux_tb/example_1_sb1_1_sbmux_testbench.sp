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
Xmux_1level_tapbuf_size3[0] mux_1level_tapbuf_size3[0]->in[0] mux_1level_tapbuf_size3[0]->in[1] mux_1level_tapbuf_size3[0]->in[2] mux_1level_tapbuf_size3[0]->out sram[0]->outb sram[0]->out sram[1]->out sram[1]->outb sram[2]->out sram[2]->outb gvdd_mux_1level_tapbuf_size3[0] 0 mux_1level_tapbuf_size3
***** SRAM bits for MUX[0], level=1, select_path_id=0. *****
*****100*****
Xsram[0] sram->in sram[0]->out sram[0]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[0]->out) 0
.nodeset V(sram[0]->outb) vsp
Xsram[1] sram->in sram[1]->out sram[1]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[1]->out) 0
.nodeset V(sram[1]->outb) vsp
Xsram[2] sram->in sram[2]->out sram[2]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[2]->out) 0
.nodeset V(sram[2]->outb) vsp
***** Signal mux_1level_tapbuf_size3[0]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[0]->in[0] mux_1level_tapbuf_size3[0]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size3[0]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[0]->in[1] mux_1level_tapbuf_size3[0]->in[1] 0 
+  0
***** Signal mux_1level_tapbuf_size3[0]->in[2] density = 0.1906, probability=0.5218.*****
Vmux_1level_tapbuf_size3[0]->in[2] mux_1level_tapbuf_size3[0]->in[2] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5218*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
Vgvdd_mux_1level_tapbuf_size3[0] gvdd_mux_1level_tapbuf_size3[0] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[228] trig v(mux_1level_tapbuf_size3[0]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[0]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[228] trig v(mux_1level_tapbuf_size3[0]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[0]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[228] when v(mux_1level_tapbuf_size3[0]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[228] trig v(mux_1level_tapbuf_size3[0]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[0]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[228] when v(mux_1level_tapbuf_size3[0]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[228] trig v(mux_1level_tapbuf_size3[0]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[0]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[0]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[0]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[228] param='mux_1level_tapbuf_size3[0]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[0]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[0]) from='clock_period' to='7*clock_period'
.meas tran mux_1level_tapbuf_size3[0]_energy_per_cycle param='mux_1level_tapbuf_size3[0]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[228]  param='mux_1level_tapbuf_size3[0]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[228]  param='dynamic_power_sb_mux[1][1]_rrnode[228]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[228] avg p(Vgvdd_mux_1level_tapbuf_size3[0]) from='start_rise_sb_mux[1][1]_rrnode[228]' to='start_rise_sb_mux[1][1]_rrnode[228]+switch_rise_sb_mux[1][1]_rrnode[228]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[228] avg p(Vgvdd_mux_1level_tapbuf_size3[0]) from='start_fall_sb_mux[1][1]_rrnode[228]' to='start_fall_sb_mux[1][1]_rrnode[228]+switch_fall_sb_mux[1][1]_rrnode[228]'
.meas tran sum_leakage_power_mux[0to0] 
+          param='leakage_sb_mux[1][1]_rrnode[228]'
.meas tran sum_energy_per_cycle_mux[0to0] 
+          param='energy_per_cycle_sb_mux[1][1]_rrnode[228]'
***** Load for rr_node[228] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=1, type=5 *****
Xchan_mux_1level_tapbuf_size3[0]->out_loadlvl[0]_out mux_1level_tapbuf_size3[0]->out mux_1level_tapbuf_size3[0]->out_loadlvl[0]_out mux_1level_tapbuf_size3[0]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[0]_no0 mux_1level_tapbuf_size3[0]->out_loadlvl[0]_out mux_1level_tapbuf_size3[0]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[1]_no0 mux_1level_tapbuf_size3[0]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[0]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to0] 
+          param='leakage_sb_mux[1][1]_rrnode[228]'
.meas tran sum_energy_per_cycle_sb_mux[0to0] 
+          param='energy_per_cycle_sb_mux[1][1]_rrnode[228]'
Xmux_1level_tapbuf_size2[1] mux_1level_tapbuf_size2[1]->in[0] mux_1level_tapbuf_size2[1]->in[1] mux_1level_tapbuf_size2[1]->out sram[3]->outb sram[3]->out gvdd_mux_1level_tapbuf_size2[1] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[1], level=1, select_path_id=0. *****
*****1*****
Xsram[3] sram->in sram[3]->out sram[3]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[3]->out) 0
.nodeset V(sram[3]->outb) vsp
***** Signal mux_1level_tapbuf_size2[1]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[1]->in[0] mux_1level_tapbuf_size2[1]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[1]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[1]->in[1] mux_1level_tapbuf_size2[1]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[1] gvdd_mux_1level_tapbuf_size2[1] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[230] trig v(mux_1level_tapbuf_size2[1]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[1]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[230] trig v(mux_1level_tapbuf_size2[1]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[1]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[230] when v(mux_1level_tapbuf_size2[1]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[230] trig v(mux_1level_tapbuf_size2[1]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[1]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[230] when v(mux_1level_tapbuf_size2[1]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[230] trig v(mux_1level_tapbuf_size2[1]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[1]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[1]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[1]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[230] param='mux_1level_tapbuf_size2[1]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[1]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[1]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[1]_energy_per_cycle param='mux_1level_tapbuf_size2[1]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[230]  param='mux_1level_tapbuf_size2[1]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[230]  param='dynamic_power_sb_mux[1][1]_rrnode[230]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[230] avg p(Vgvdd_mux_1level_tapbuf_size2[1]) from='start_rise_sb_mux[1][1]_rrnode[230]' to='start_rise_sb_mux[1][1]_rrnode[230]+switch_rise_sb_mux[1][1]_rrnode[230]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[230] avg p(Vgvdd_mux_1level_tapbuf_size2[1]) from='start_fall_sb_mux[1][1]_rrnode[230]' to='start_fall_sb_mux[1][1]_rrnode[230]+switch_fall_sb_mux[1][1]_rrnode[230]'
.meas tran sum_leakage_power_mux[0to1] 
+          param='sum_leakage_power_mux[0to0]+leakage_sb_mux[1][1]_rrnode[230]'
.meas tran sum_energy_per_cycle_mux[0to1] 
+          param='sum_energy_per_cycle_mux[0to0]+energy_per_cycle_sb_mux[1][1]_rrnode[230]'
***** Load for rr_node[230] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=3, type=5 *****
Xchan_mux_1level_tapbuf_size2[1]->out_loadlvl[0]_out mux_1level_tapbuf_size2[1]->out mux_1level_tapbuf_size2[1]->out_loadlvl[0]_out mux_1level_tapbuf_size2[1]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[2]_no0 mux_1level_tapbuf_size2[1]->out_loadlvl[0]_out mux_1level_tapbuf_size2[1]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to1] 
+          param='sum_leakage_power_sb_mux[0to0]+leakage_sb_mux[1][1]_rrnode[230]'
.meas tran sum_energy_per_cycle_sb_mux[0to1] 
+          param='sum_energy_per_cycle_sb_mux[0to0]+energy_per_cycle_sb_mux[1][1]_rrnode[230]'
Xmux_1level_tapbuf_size2[2] mux_1level_tapbuf_size2[2]->in[0] mux_1level_tapbuf_size2[2]->in[1] mux_1level_tapbuf_size2[2]->out sram[4]->outb sram[4]->out gvdd_mux_1level_tapbuf_size2[2] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[2], level=1, select_path_id=0. *****
*****1*****
Xsram[4] sram->in sram[4]->out sram[4]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[4]->out) 0
.nodeset V(sram[4]->outb) vsp
***** Signal mux_1level_tapbuf_size2[2]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[2]->in[0] mux_1level_tapbuf_size2[2]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[2]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[2]->in[1] mux_1level_tapbuf_size2[2]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[2] gvdd_mux_1level_tapbuf_size2[2] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[232] trig v(mux_1level_tapbuf_size2[2]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[2]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[232] trig v(mux_1level_tapbuf_size2[2]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[2]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[232] when v(mux_1level_tapbuf_size2[2]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[232] trig v(mux_1level_tapbuf_size2[2]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[2]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[232] when v(mux_1level_tapbuf_size2[2]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[232] trig v(mux_1level_tapbuf_size2[2]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[2]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[2]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[2]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[232] param='mux_1level_tapbuf_size2[2]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[2]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[2]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[2]_energy_per_cycle param='mux_1level_tapbuf_size2[2]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[232]  param='mux_1level_tapbuf_size2[2]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[232]  param='dynamic_power_sb_mux[1][1]_rrnode[232]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[232] avg p(Vgvdd_mux_1level_tapbuf_size2[2]) from='start_rise_sb_mux[1][1]_rrnode[232]' to='start_rise_sb_mux[1][1]_rrnode[232]+switch_rise_sb_mux[1][1]_rrnode[232]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[232] avg p(Vgvdd_mux_1level_tapbuf_size2[2]) from='start_fall_sb_mux[1][1]_rrnode[232]' to='start_fall_sb_mux[1][1]_rrnode[232]+switch_fall_sb_mux[1][1]_rrnode[232]'
.meas tran sum_leakage_power_mux[0to2] 
+          param='sum_leakage_power_mux[0to1]+leakage_sb_mux[1][1]_rrnode[232]'
.meas tran sum_energy_per_cycle_mux[0to2] 
+          param='sum_energy_per_cycle_mux[0to1]+energy_per_cycle_sb_mux[1][1]_rrnode[232]'
***** Load for rr_node[232] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=5, type=5 *****
Xchan_mux_1level_tapbuf_size2[2]->out_loadlvl[0]_out mux_1level_tapbuf_size2[2]->out mux_1level_tapbuf_size2[2]->out_loadlvl[0]_out mux_1level_tapbuf_size2[2]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[3]_no0 mux_1level_tapbuf_size2[2]->out_loadlvl[0]_out mux_1level_tapbuf_size2[2]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to2] 
+          param='sum_leakage_power_sb_mux[0to1]+leakage_sb_mux[1][1]_rrnode[232]'
.meas tran sum_energy_per_cycle_sb_mux[0to2] 
+          param='sum_energy_per_cycle_sb_mux[0to1]+energy_per_cycle_sb_mux[1][1]_rrnode[232]'
Xmux_1level_tapbuf_size2[3] mux_1level_tapbuf_size2[3]->in[0] mux_1level_tapbuf_size2[3]->in[1] mux_1level_tapbuf_size2[3]->out sram[5]->outb sram[5]->out gvdd_mux_1level_tapbuf_size2[3] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[3], level=1, select_path_id=0. *****
*****1*****
Xsram[5] sram->in sram[5]->out sram[5]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[5]->out) 0
.nodeset V(sram[5]->outb) vsp
***** Signal mux_1level_tapbuf_size2[3]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[3]->in[0] mux_1level_tapbuf_size2[3]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[3]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[3]->in[1] mux_1level_tapbuf_size2[3]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[3] gvdd_mux_1level_tapbuf_size2[3] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[234] trig v(mux_1level_tapbuf_size2[3]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[3]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[234] trig v(mux_1level_tapbuf_size2[3]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[3]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[234] when v(mux_1level_tapbuf_size2[3]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[234] trig v(mux_1level_tapbuf_size2[3]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[3]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[234] when v(mux_1level_tapbuf_size2[3]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[234] trig v(mux_1level_tapbuf_size2[3]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[3]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[3]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[3]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[234] param='mux_1level_tapbuf_size2[3]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[3]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[3]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[3]_energy_per_cycle param='mux_1level_tapbuf_size2[3]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[234]  param='mux_1level_tapbuf_size2[3]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[234]  param='dynamic_power_sb_mux[1][1]_rrnode[234]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[234] avg p(Vgvdd_mux_1level_tapbuf_size2[3]) from='start_rise_sb_mux[1][1]_rrnode[234]' to='start_rise_sb_mux[1][1]_rrnode[234]+switch_rise_sb_mux[1][1]_rrnode[234]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[234] avg p(Vgvdd_mux_1level_tapbuf_size2[3]) from='start_fall_sb_mux[1][1]_rrnode[234]' to='start_fall_sb_mux[1][1]_rrnode[234]+switch_fall_sb_mux[1][1]_rrnode[234]'
.meas tran sum_leakage_power_mux[0to3] 
+          param='sum_leakage_power_mux[0to2]+leakage_sb_mux[1][1]_rrnode[234]'
.meas tran sum_energy_per_cycle_mux[0to3] 
+          param='sum_energy_per_cycle_mux[0to2]+energy_per_cycle_sb_mux[1][1]_rrnode[234]'
***** Load for rr_node[234] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=7, type=5 *****
Xchan_mux_1level_tapbuf_size2[3]->out_loadlvl[0]_out mux_1level_tapbuf_size2[3]->out mux_1level_tapbuf_size2[3]->out_loadlvl[0]_out mux_1level_tapbuf_size2[3]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[4]_no0 mux_1level_tapbuf_size2[3]->out_loadlvl[0]_out mux_1level_tapbuf_size2[3]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to3] 
+          param='sum_leakage_power_sb_mux[0to2]+leakage_sb_mux[1][1]_rrnode[234]'
.meas tran sum_energy_per_cycle_sb_mux[0to3] 
+          param='sum_energy_per_cycle_sb_mux[0to2]+energy_per_cycle_sb_mux[1][1]_rrnode[234]'
Xmux_1level_tapbuf_size2[4] mux_1level_tapbuf_size2[4]->in[0] mux_1level_tapbuf_size2[4]->in[1] mux_1level_tapbuf_size2[4]->out sram[6]->outb sram[6]->out gvdd_mux_1level_tapbuf_size2[4] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[4], level=1, select_path_id=0. *****
*****1*****
Xsram[6] sram->in sram[6]->out sram[6]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[6]->out) 0
.nodeset V(sram[6]->outb) vsp
***** Signal mux_1level_tapbuf_size2[4]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[4]->in[0] mux_1level_tapbuf_size2[4]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[4]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[4]->in[1] mux_1level_tapbuf_size2[4]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[4] gvdd_mux_1level_tapbuf_size2[4] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[236] trig v(mux_1level_tapbuf_size2[4]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[4]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[236] trig v(mux_1level_tapbuf_size2[4]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[4]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[236] when v(mux_1level_tapbuf_size2[4]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[236] trig v(mux_1level_tapbuf_size2[4]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[4]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[236] when v(mux_1level_tapbuf_size2[4]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[236] trig v(mux_1level_tapbuf_size2[4]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[4]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[4]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[4]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[236] param='mux_1level_tapbuf_size2[4]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[4]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[4]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[4]_energy_per_cycle param='mux_1level_tapbuf_size2[4]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[236]  param='mux_1level_tapbuf_size2[4]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[236]  param='dynamic_power_sb_mux[1][1]_rrnode[236]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[236] avg p(Vgvdd_mux_1level_tapbuf_size2[4]) from='start_rise_sb_mux[1][1]_rrnode[236]' to='start_rise_sb_mux[1][1]_rrnode[236]+switch_rise_sb_mux[1][1]_rrnode[236]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[236] avg p(Vgvdd_mux_1level_tapbuf_size2[4]) from='start_fall_sb_mux[1][1]_rrnode[236]' to='start_fall_sb_mux[1][1]_rrnode[236]+switch_fall_sb_mux[1][1]_rrnode[236]'
.meas tran sum_leakage_power_mux[0to4] 
+          param='sum_leakage_power_mux[0to3]+leakage_sb_mux[1][1]_rrnode[236]'
.meas tran sum_energy_per_cycle_mux[0to4] 
+          param='sum_energy_per_cycle_mux[0to3]+energy_per_cycle_sb_mux[1][1]_rrnode[236]'
***** Load for rr_node[236] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=9, type=5 *****
Xchan_mux_1level_tapbuf_size2[4]->out_loadlvl[0]_out mux_1level_tapbuf_size2[4]->out mux_1level_tapbuf_size2[4]->out_loadlvl[0]_out mux_1level_tapbuf_size2[4]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[5]_no0 mux_1level_tapbuf_size2[4]->out_loadlvl[0]_out mux_1level_tapbuf_size2[4]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to4] 
+          param='sum_leakage_power_sb_mux[0to3]+leakage_sb_mux[1][1]_rrnode[236]'
.meas tran sum_energy_per_cycle_sb_mux[0to4] 
+          param='sum_energy_per_cycle_sb_mux[0to3]+energy_per_cycle_sb_mux[1][1]_rrnode[236]'
Xmux_1level_tapbuf_size2[5] mux_1level_tapbuf_size2[5]->in[0] mux_1level_tapbuf_size2[5]->in[1] mux_1level_tapbuf_size2[5]->out sram[7]->outb sram[7]->out gvdd_mux_1level_tapbuf_size2[5] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[5], level=1, select_path_id=0. *****
*****1*****
Xsram[7] sram->in sram[7]->out sram[7]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[7]->out) 0
.nodeset V(sram[7]->outb) vsp
***** Signal mux_1level_tapbuf_size2[5]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[5]->in[0] mux_1level_tapbuf_size2[5]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[5]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[5]->in[1] mux_1level_tapbuf_size2[5]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[5] gvdd_mux_1level_tapbuf_size2[5] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[238] trig v(mux_1level_tapbuf_size2[5]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[5]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[238] trig v(mux_1level_tapbuf_size2[5]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[5]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[238] when v(mux_1level_tapbuf_size2[5]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[238] trig v(mux_1level_tapbuf_size2[5]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[5]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[238] when v(mux_1level_tapbuf_size2[5]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[238] trig v(mux_1level_tapbuf_size2[5]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[5]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[5]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[5]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[238] param='mux_1level_tapbuf_size2[5]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[5]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[5]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[5]_energy_per_cycle param='mux_1level_tapbuf_size2[5]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[238]  param='mux_1level_tapbuf_size2[5]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[238]  param='dynamic_power_sb_mux[1][1]_rrnode[238]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[238] avg p(Vgvdd_mux_1level_tapbuf_size2[5]) from='start_rise_sb_mux[1][1]_rrnode[238]' to='start_rise_sb_mux[1][1]_rrnode[238]+switch_rise_sb_mux[1][1]_rrnode[238]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[238] avg p(Vgvdd_mux_1level_tapbuf_size2[5]) from='start_fall_sb_mux[1][1]_rrnode[238]' to='start_fall_sb_mux[1][1]_rrnode[238]+switch_fall_sb_mux[1][1]_rrnode[238]'
.meas tran sum_leakage_power_mux[0to5] 
+          param='sum_leakage_power_mux[0to4]+leakage_sb_mux[1][1]_rrnode[238]'
.meas tran sum_energy_per_cycle_mux[0to5] 
+          param='sum_energy_per_cycle_mux[0to4]+energy_per_cycle_sb_mux[1][1]_rrnode[238]'
***** Load for rr_node[238] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=11, type=5 *****
Xchan_mux_1level_tapbuf_size2[5]->out_loadlvl[0]_out mux_1level_tapbuf_size2[5]->out mux_1level_tapbuf_size2[5]->out_loadlvl[0]_out mux_1level_tapbuf_size2[5]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[6]_no0 mux_1level_tapbuf_size2[5]->out_loadlvl[0]_out mux_1level_tapbuf_size2[5]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to5] 
+          param='sum_leakage_power_sb_mux[0to4]+leakage_sb_mux[1][1]_rrnode[238]'
.meas tran sum_energy_per_cycle_sb_mux[0to5] 
+          param='sum_energy_per_cycle_sb_mux[0to4]+energy_per_cycle_sb_mux[1][1]_rrnode[238]'
Xmux_1level_tapbuf_size2[6] mux_1level_tapbuf_size2[6]->in[0] mux_1level_tapbuf_size2[6]->in[1] mux_1level_tapbuf_size2[6]->out sram[8]->outb sram[8]->out gvdd_mux_1level_tapbuf_size2[6] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[6], level=1, select_path_id=0. *****
*****1*****
Xsram[8] sram->in sram[8]->out sram[8]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[8]->out) 0
.nodeset V(sram[8]->outb) vsp
***** Signal mux_1level_tapbuf_size2[6]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[6]->in[0] mux_1level_tapbuf_size2[6]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[6]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[6]->in[1] mux_1level_tapbuf_size2[6]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[6] gvdd_mux_1level_tapbuf_size2[6] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[240] trig v(mux_1level_tapbuf_size2[6]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[6]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[240] trig v(mux_1level_tapbuf_size2[6]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[6]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[240] when v(mux_1level_tapbuf_size2[6]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[240] trig v(mux_1level_tapbuf_size2[6]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[6]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[240] when v(mux_1level_tapbuf_size2[6]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[240] trig v(mux_1level_tapbuf_size2[6]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[6]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[6]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[6]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[240] param='mux_1level_tapbuf_size2[6]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[6]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[6]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[6]_energy_per_cycle param='mux_1level_tapbuf_size2[6]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[240]  param='mux_1level_tapbuf_size2[6]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[240]  param='dynamic_power_sb_mux[1][1]_rrnode[240]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[240] avg p(Vgvdd_mux_1level_tapbuf_size2[6]) from='start_rise_sb_mux[1][1]_rrnode[240]' to='start_rise_sb_mux[1][1]_rrnode[240]+switch_rise_sb_mux[1][1]_rrnode[240]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[240] avg p(Vgvdd_mux_1level_tapbuf_size2[6]) from='start_fall_sb_mux[1][1]_rrnode[240]' to='start_fall_sb_mux[1][1]_rrnode[240]+switch_fall_sb_mux[1][1]_rrnode[240]'
.meas tran sum_leakage_power_mux[0to6] 
+          param='sum_leakage_power_mux[0to5]+leakage_sb_mux[1][1]_rrnode[240]'
.meas tran sum_energy_per_cycle_mux[0to6] 
+          param='sum_energy_per_cycle_mux[0to5]+energy_per_cycle_sb_mux[1][1]_rrnode[240]'
***** Load for rr_node[240] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=13, type=5 *****
Xchan_mux_1level_tapbuf_size2[6]->out_loadlvl[0]_out mux_1level_tapbuf_size2[6]->out mux_1level_tapbuf_size2[6]->out_loadlvl[0]_out mux_1level_tapbuf_size2[6]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[7]_no0 mux_1level_tapbuf_size2[6]->out_loadlvl[0]_out mux_1level_tapbuf_size2[6]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to6] 
+          param='sum_leakage_power_sb_mux[0to5]+leakage_sb_mux[1][1]_rrnode[240]'
.meas tran sum_energy_per_cycle_sb_mux[0to6] 
+          param='sum_energy_per_cycle_sb_mux[0to5]+energy_per_cycle_sb_mux[1][1]_rrnode[240]'
Xmux_1level_tapbuf_size2[7] mux_1level_tapbuf_size2[7]->in[0] mux_1level_tapbuf_size2[7]->in[1] mux_1level_tapbuf_size2[7]->out sram[9]->outb sram[9]->out gvdd_mux_1level_tapbuf_size2[7] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[7], level=1, select_path_id=0. *****
*****1*****
Xsram[9] sram->in sram[9]->out sram[9]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[9]->out) 0
.nodeset V(sram[9]->outb) vsp
***** Signal mux_1level_tapbuf_size2[7]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[7]->in[0] mux_1level_tapbuf_size2[7]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[7]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[7]->in[1] mux_1level_tapbuf_size2[7]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[7] gvdd_mux_1level_tapbuf_size2[7] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[242] trig v(mux_1level_tapbuf_size2[7]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[7]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[242] trig v(mux_1level_tapbuf_size2[7]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[7]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[242] when v(mux_1level_tapbuf_size2[7]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[242] trig v(mux_1level_tapbuf_size2[7]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[7]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[242] when v(mux_1level_tapbuf_size2[7]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[242] trig v(mux_1level_tapbuf_size2[7]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[7]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[7]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[7]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[242] param='mux_1level_tapbuf_size2[7]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[7]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[7]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[7]_energy_per_cycle param='mux_1level_tapbuf_size2[7]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[242]  param='mux_1level_tapbuf_size2[7]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[242]  param='dynamic_power_sb_mux[1][1]_rrnode[242]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[242] avg p(Vgvdd_mux_1level_tapbuf_size2[7]) from='start_rise_sb_mux[1][1]_rrnode[242]' to='start_rise_sb_mux[1][1]_rrnode[242]+switch_rise_sb_mux[1][1]_rrnode[242]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[242] avg p(Vgvdd_mux_1level_tapbuf_size2[7]) from='start_fall_sb_mux[1][1]_rrnode[242]' to='start_fall_sb_mux[1][1]_rrnode[242]+switch_fall_sb_mux[1][1]_rrnode[242]'
.meas tran sum_leakage_power_mux[0to7] 
+          param='sum_leakage_power_mux[0to6]+leakage_sb_mux[1][1]_rrnode[242]'
.meas tran sum_energy_per_cycle_mux[0to7] 
+          param='sum_energy_per_cycle_mux[0to6]+energy_per_cycle_sb_mux[1][1]_rrnode[242]'
***** Load for rr_node[242] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=15, type=5 *****
Xchan_mux_1level_tapbuf_size2[7]->out_loadlvl[0]_out mux_1level_tapbuf_size2[7]->out mux_1level_tapbuf_size2[7]->out_loadlvl[0]_out mux_1level_tapbuf_size2[7]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[8]_no0 mux_1level_tapbuf_size2[7]->out_loadlvl[0]_out mux_1level_tapbuf_size2[7]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to7] 
+          param='sum_leakage_power_sb_mux[0to6]+leakage_sb_mux[1][1]_rrnode[242]'
.meas tran sum_energy_per_cycle_sb_mux[0to7] 
+          param='sum_energy_per_cycle_sb_mux[0to6]+energy_per_cycle_sb_mux[1][1]_rrnode[242]'
Xmux_1level_tapbuf_size2[8] mux_1level_tapbuf_size2[8]->in[0] mux_1level_tapbuf_size2[8]->in[1] mux_1level_tapbuf_size2[8]->out sram[10]->outb sram[10]->out gvdd_mux_1level_tapbuf_size2[8] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[8], level=1, select_path_id=0. *****
*****1*****
Xsram[10] sram->in sram[10]->out sram[10]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[10]->out) 0
.nodeset V(sram[10]->outb) vsp
***** Signal mux_1level_tapbuf_size2[8]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[8]->in[0] mux_1level_tapbuf_size2[8]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[8]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[8]->in[1] mux_1level_tapbuf_size2[8]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[8] gvdd_mux_1level_tapbuf_size2[8] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[244] trig v(mux_1level_tapbuf_size2[8]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[8]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[244] trig v(mux_1level_tapbuf_size2[8]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[8]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[244] when v(mux_1level_tapbuf_size2[8]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[244] trig v(mux_1level_tapbuf_size2[8]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[8]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[244] when v(mux_1level_tapbuf_size2[8]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[244] trig v(mux_1level_tapbuf_size2[8]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[8]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[8]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[8]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[244] param='mux_1level_tapbuf_size2[8]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[8]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[8]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[8]_energy_per_cycle param='mux_1level_tapbuf_size2[8]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[244]  param='mux_1level_tapbuf_size2[8]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[244]  param='dynamic_power_sb_mux[1][1]_rrnode[244]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[244] avg p(Vgvdd_mux_1level_tapbuf_size2[8]) from='start_rise_sb_mux[1][1]_rrnode[244]' to='start_rise_sb_mux[1][1]_rrnode[244]+switch_rise_sb_mux[1][1]_rrnode[244]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[244] avg p(Vgvdd_mux_1level_tapbuf_size2[8]) from='start_fall_sb_mux[1][1]_rrnode[244]' to='start_fall_sb_mux[1][1]_rrnode[244]+switch_fall_sb_mux[1][1]_rrnode[244]'
.meas tran sum_leakage_power_mux[0to8] 
+          param='sum_leakage_power_mux[0to7]+leakage_sb_mux[1][1]_rrnode[244]'
.meas tran sum_energy_per_cycle_mux[0to8] 
+          param='sum_energy_per_cycle_mux[0to7]+energy_per_cycle_sb_mux[1][1]_rrnode[244]'
***** Load for rr_node[244] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=17, type=5 *****
Xchan_mux_1level_tapbuf_size2[8]->out_loadlvl[0]_out mux_1level_tapbuf_size2[8]->out mux_1level_tapbuf_size2[8]->out_loadlvl[0]_out mux_1level_tapbuf_size2[8]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[9]_no0 mux_1level_tapbuf_size2[8]->out_loadlvl[0]_out mux_1level_tapbuf_size2[8]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[10]_no0 mux_1level_tapbuf_size2[8]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[8]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to8] 
+          param='sum_leakage_power_sb_mux[0to7]+leakage_sb_mux[1][1]_rrnode[244]'
.meas tran sum_energy_per_cycle_sb_mux[0to8] 
+          param='sum_energy_per_cycle_sb_mux[0to7]+energy_per_cycle_sb_mux[1][1]_rrnode[244]'
Xmux_1level_tapbuf_size2[9] mux_1level_tapbuf_size2[9]->in[0] mux_1level_tapbuf_size2[9]->in[1] mux_1level_tapbuf_size2[9]->out sram[11]->outb sram[11]->out gvdd_mux_1level_tapbuf_size2[9] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[9], level=1, select_path_id=0. *****
*****1*****
Xsram[11] sram->in sram[11]->out sram[11]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[11]->out) 0
.nodeset V(sram[11]->outb) vsp
***** Signal mux_1level_tapbuf_size2[9]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[9]->in[0] mux_1level_tapbuf_size2[9]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[9]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[9]->in[1] mux_1level_tapbuf_size2[9]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[9] gvdd_mux_1level_tapbuf_size2[9] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[246] trig v(mux_1level_tapbuf_size2[9]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[9]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[246] trig v(mux_1level_tapbuf_size2[9]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[9]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[246] when v(mux_1level_tapbuf_size2[9]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[246] trig v(mux_1level_tapbuf_size2[9]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[9]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[246] when v(mux_1level_tapbuf_size2[9]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[246] trig v(mux_1level_tapbuf_size2[9]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[9]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[9]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[9]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[246] param='mux_1level_tapbuf_size2[9]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[9]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[9]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[9]_energy_per_cycle param='mux_1level_tapbuf_size2[9]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[246]  param='mux_1level_tapbuf_size2[9]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[246]  param='dynamic_power_sb_mux[1][1]_rrnode[246]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[246] avg p(Vgvdd_mux_1level_tapbuf_size2[9]) from='start_rise_sb_mux[1][1]_rrnode[246]' to='start_rise_sb_mux[1][1]_rrnode[246]+switch_rise_sb_mux[1][1]_rrnode[246]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[246] avg p(Vgvdd_mux_1level_tapbuf_size2[9]) from='start_fall_sb_mux[1][1]_rrnode[246]' to='start_fall_sb_mux[1][1]_rrnode[246]+switch_fall_sb_mux[1][1]_rrnode[246]'
.meas tran sum_leakage_power_mux[0to9] 
+          param='sum_leakage_power_mux[0to8]+leakage_sb_mux[1][1]_rrnode[246]'
.meas tran sum_energy_per_cycle_mux[0to9] 
+          param='sum_energy_per_cycle_mux[0to8]+energy_per_cycle_sb_mux[1][1]_rrnode[246]'
***** Load for rr_node[246] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=19, type=5 *****
Xchan_mux_1level_tapbuf_size2[9]->out_loadlvl[0]_out mux_1level_tapbuf_size2[9]->out mux_1level_tapbuf_size2[9]->out_loadlvl[0]_out mux_1level_tapbuf_size2[9]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[11]_no0 mux_1level_tapbuf_size2[9]->out_loadlvl[0]_out mux_1level_tapbuf_size2[9]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to9] 
+          param='sum_leakage_power_sb_mux[0to8]+leakage_sb_mux[1][1]_rrnode[246]'
.meas tran sum_energy_per_cycle_sb_mux[0to9] 
+          param='sum_energy_per_cycle_sb_mux[0to8]+energy_per_cycle_sb_mux[1][1]_rrnode[246]'
Xmux_1level_tapbuf_size2[10] mux_1level_tapbuf_size2[10]->in[0] mux_1level_tapbuf_size2[10]->in[1] mux_1level_tapbuf_size2[10]->out sram[12]->outb sram[12]->out gvdd_mux_1level_tapbuf_size2[10] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[10], level=1, select_path_id=0. *****
*****1*****
Xsram[12] sram->in sram[12]->out sram[12]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[12]->out) 0
.nodeset V(sram[12]->outb) vsp
***** Signal mux_1level_tapbuf_size2[10]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[10]->in[0] mux_1level_tapbuf_size2[10]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[10]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[10]->in[1] mux_1level_tapbuf_size2[10]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[10] gvdd_mux_1level_tapbuf_size2[10] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[248] trig v(mux_1level_tapbuf_size2[10]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[10]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[248] trig v(mux_1level_tapbuf_size2[10]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[10]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[248] when v(mux_1level_tapbuf_size2[10]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[248] trig v(mux_1level_tapbuf_size2[10]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[10]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[248] when v(mux_1level_tapbuf_size2[10]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[248] trig v(mux_1level_tapbuf_size2[10]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[10]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[10]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[10]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[248] param='mux_1level_tapbuf_size2[10]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[10]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[10]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[10]_energy_per_cycle param='mux_1level_tapbuf_size2[10]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[248]  param='mux_1level_tapbuf_size2[10]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[248]  param='dynamic_power_sb_mux[1][1]_rrnode[248]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[248] avg p(Vgvdd_mux_1level_tapbuf_size2[10]) from='start_rise_sb_mux[1][1]_rrnode[248]' to='start_rise_sb_mux[1][1]_rrnode[248]+switch_rise_sb_mux[1][1]_rrnode[248]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[248] avg p(Vgvdd_mux_1level_tapbuf_size2[10]) from='start_fall_sb_mux[1][1]_rrnode[248]' to='start_fall_sb_mux[1][1]_rrnode[248]+switch_fall_sb_mux[1][1]_rrnode[248]'
.meas tran sum_leakage_power_mux[0to10] 
+          param='sum_leakage_power_mux[0to9]+leakage_sb_mux[1][1]_rrnode[248]'
.meas tran sum_energy_per_cycle_mux[0to10] 
+          param='sum_energy_per_cycle_mux[0to9]+energy_per_cycle_sb_mux[1][1]_rrnode[248]'
***** Load for rr_node[248] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=21, type=5 *****
Xchan_mux_1level_tapbuf_size2[10]->out_loadlvl[0]_out mux_1level_tapbuf_size2[10]->out mux_1level_tapbuf_size2[10]->out_loadlvl[0]_out mux_1level_tapbuf_size2[10]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[12]_no0 mux_1level_tapbuf_size2[10]->out_loadlvl[0]_out mux_1level_tapbuf_size2[10]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to10] 
+          param='sum_leakage_power_sb_mux[0to9]+leakage_sb_mux[1][1]_rrnode[248]'
.meas tran sum_energy_per_cycle_sb_mux[0to10] 
+          param='sum_energy_per_cycle_sb_mux[0to9]+energy_per_cycle_sb_mux[1][1]_rrnode[248]'
Xmux_1level_tapbuf_size2[11] mux_1level_tapbuf_size2[11]->in[0] mux_1level_tapbuf_size2[11]->in[1] mux_1level_tapbuf_size2[11]->out sram[13]->outb sram[13]->out gvdd_mux_1level_tapbuf_size2[11] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[11], level=1, select_path_id=0. *****
*****1*****
Xsram[13] sram->in sram[13]->out sram[13]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[13]->out) 0
.nodeset V(sram[13]->outb) vsp
***** Signal mux_1level_tapbuf_size2[11]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[11]->in[0] mux_1level_tapbuf_size2[11]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[11]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[11]->in[1] mux_1level_tapbuf_size2[11]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[11] gvdd_mux_1level_tapbuf_size2[11] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[250] trig v(mux_1level_tapbuf_size2[11]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[11]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[250] trig v(mux_1level_tapbuf_size2[11]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[11]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[250] when v(mux_1level_tapbuf_size2[11]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[250] trig v(mux_1level_tapbuf_size2[11]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[11]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[250] when v(mux_1level_tapbuf_size2[11]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[250] trig v(mux_1level_tapbuf_size2[11]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[11]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[11]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[11]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[250] param='mux_1level_tapbuf_size2[11]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[11]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[11]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[11]_energy_per_cycle param='mux_1level_tapbuf_size2[11]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[250]  param='mux_1level_tapbuf_size2[11]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[250]  param='dynamic_power_sb_mux[1][1]_rrnode[250]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[250] avg p(Vgvdd_mux_1level_tapbuf_size2[11]) from='start_rise_sb_mux[1][1]_rrnode[250]' to='start_rise_sb_mux[1][1]_rrnode[250]+switch_rise_sb_mux[1][1]_rrnode[250]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[250] avg p(Vgvdd_mux_1level_tapbuf_size2[11]) from='start_fall_sb_mux[1][1]_rrnode[250]' to='start_fall_sb_mux[1][1]_rrnode[250]+switch_fall_sb_mux[1][1]_rrnode[250]'
.meas tran sum_leakage_power_mux[0to11] 
+          param='sum_leakage_power_mux[0to10]+leakage_sb_mux[1][1]_rrnode[250]'
.meas tran sum_energy_per_cycle_mux[0to11] 
+          param='sum_energy_per_cycle_mux[0to10]+energy_per_cycle_sb_mux[1][1]_rrnode[250]'
***** Load for rr_node[250] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=23, type=5 *****
Xchan_mux_1level_tapbuf_size2[11]->out_loadlvl[0]_out mux_1level_tapbuf_size2[11]->out mux_1level_tapbuf_size2[11]->out_loadlvl[0]_out mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[13]_no0 mux_1level_tapbuf_size2[11]->out_loadlvl[0]_out mux_1level_tapbuf_size2[11]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to11] 
+          param='sum_leakage_power_sb_mux[0to10]+leakage_sb_mux[1][1]_rrnode[250]'
.meas tran sum_energy_per_cycle_sb_mux[0to11] 
+          param='sum_energy_per_cycle_sb_mux[0to10]+energy_per_cycle_sb_mux[1][1]_rrnode[250]'
Xmux_1level_tapbuf_size2[12] mux_1level_tapbuf_size2[12]->in[0] mux_1level_tapbuf_size2[12]->in[1] mux_1level_tapbuf_size2[12]->out sram[14]->outb sram[14]->out gvdd_mux_1level_tapbuf_size2[12] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[12], level=1, select_path_id=0. *****
*****1*****
Xsram[14] sram->in sram[14]->out sram[14]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[14]->out) 0
.nodeset V(sram[14]->outb) vsp
***** Signal mux_1level_tapbuf_size2[12]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[12]->in[0] mux_1level_tapbuf_size2[12]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[12]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[12]->in[1] mux_1level_tapbuf_size2[12]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[12] gvdd_mux_1level_tapbuf_size2[12] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[252] trig v(mux_1level_tapbuf_size2[12]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[12]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[252] trig v(mux_1level_tapbuf_size2[12]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[12]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[252] when v(mux_1level_tapbuf_size2[12]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[252] trig v(mux_1level_tapbuf_size2[12]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[12]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[252] when v(mux_1level_tapbuf_size2[12]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[252] trig v(mux_1level_tapbuf_size2[12]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[12]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[12]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[12]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[252] param='mux_1level_tapbuf_size2[12]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[12]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[12]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[12]_energy_per_cycle param='mux_1level_tapbuf_size2[12]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[252]  param='mux_1level_tapbuf_size2[12]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[252]  param='dynamic_power_sb_mux[1][1]_rrnode[252]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[252] avg p(Vgvdd_mux_1level_tapbuf_size2[12]) from='start_rise_sb_mux[1][1]_rrnode[252]' to='start_rise_sb_mux[1][1]_rrnode[252]+switch_rise_sb_mux[1][1]_rrnode[252]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[252] avg p(Vgvdd_mux_1level_tapbuf_size2[12]) from='start_fall_sb_mux[1][1]_rrnode[252]' to='start_fall_sb_mux[1][1]_rrnode[252]+switch_fall_sb_mux[1][1]_rrnode[252]'
.meas tran sum_leakage_power_mux[0to12] 
+          param='sum_leakage_power_mux[0to11]+leakage_sb_mux[1][1]_rrnode[252]'
.meas tran sum_energy_per_cycle_mux[0to12] 
+          param='sum_energy_per_cycle_mux[0to11]+energy_per_cycle_sb_mux[1][1]_rrnode[252]'
***** Load for rr_node[252] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=25, type=5 *****
Xchan_mux_1level_tapbuf_size2[12]->out_loadlvl[0]_out mux_1level_tapbuf_size2[12]->out mux_1level_tapbuf_size2[12]->out_loadlvl[0]_out mux_1level_tapbuf_size2[12]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[14]_no0 mux_1level_tapbuf_size2[12]->out_loadlvl[0]_out mux_1level_tapbuf_size2[12]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to12] 
+          param='sum_leakage_power_sb_mux[0to11]+leakage_sb_mux[1][1]_rrnode[252]'
.meas tran sum_energy_per_cycle_sb_mux[0to12] 
+          param='sum_energy_per_cycle_sb_mux[0to11]+energy_per_cycle_sb_mux[1][1]_rrnode[252]'
Xmux_1level_tapbuf_size2[13] mux_1level_tapbuf_size2[13]->in[0] mux_1level_tapbuf_size2[13]->in[1] mux_1level_tapbuf_size2[13]->out sram[15]->outb sram[15]->out gvdd_mux_1level_tapbuf_size2[13] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[13], level=1, select_path_id=0. *****
*****1*****
Xsram[15] sram->in sram[15]->out sram[15]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[15]->out) 0
.nodeset V(sram[15]->outb) vsp
***** Signal mux_1level_tapbuf_size2[13]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[13]->in[0] mux_1level_tapbuf_size2[13]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[13]->in[1] density = 0.1906, probability=0.4782.*****
Vmux_1level_tapbuf_size2[13]->in[1] mux_1level_tapbuf_size2[13]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4782*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
Vgvdd_mux_1level_tapbuf_size2[13] gvdd_mux_1level_tapbuf_size2[13] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[254] trig v(mux_1level_tapbuf_size2[13]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[13]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[254] trig v(mux_1level_tapbuf_size2[13]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[13]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[254] when v(mux_1level_tapbuf_size2[13]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[254] trig v(mux_1level_tapbuf_size2[13]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[13]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[254] when v(mux_1level_tapbuf_size2[13]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[254] trig v(mux_1level_tapbuf_size2[13]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[13]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[13]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[13]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[254] param='mux_1level_tapbuf_size2[13]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[13]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[13]) from='clock_period' to='7*clock_period'
.meas tran mux_1level_tapbuf_size2[13]_energy_per_cycle param='mux_1level_tapbuf_size2[13]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[254]  param='mux_1level_tapbuf_size2[13]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[254]  param='dynamic_power_sb_mux[1][1]_rrnode[254]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[254] avg p(Vgvdd_mux_1level_tapbuf_size2[13]) from='start_rise_sb_mux[1][1]_rrnode[254]' to='start_rise_sb_mux[1][1]_rrnode[254]+switch_rise_sb_mux[1][1]_rrnode[254]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[254] avg p(Vgvdd_mux_1level_tapbuf_size2[13]) from='start_fall_sb_mux[1][1]_rrnode[254]' to='start_fall_sb_mux[1][1]_rrnode[254]+switch_fall_sb_mux[1][1]_rrnode[254]'
.meas tran sum_leakage_power_mux[0to13] 
+          param='sum_leakage_power_mux[0to12]+leakage_sb_mux[1][1]_rrnode[254]'
.meas tran sum_energy_per_cycle_mux[0to13] 
+          param='sum_energy_per_cycle_mux[0to12]+energy_per_cycle_sb_mux[1][1]_rrnode[254]'
***** Load for rr_node[254] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=27, type=5 *****
Xchan_mux_1level_tapbuf_size2[13]->out_loadlvl[0]_out mux_1level_tapbuf_size2[13]->out mux_1level_tapbuf_size2[13]->out_loadlvl[0]_out mux_1level_tapbuf_size2[13]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[15]_no0 mux_1level_tapbuf_size2[13]->out_loadlvl[0]_out mux_1level_tapbuf_size2[13]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to13] 
+          param='sum_leakage_power_sb_mux[0to12]+leakage_sb_mux[1][1]_rrnode[254]'
.meas tran sum_energy_per_cycle_sb_mux[0to13] 
+          param='sum_energy_per_cycle_sb_mux[0to12]+energy_per_cycle_sb_mux[1][1]_rrnode[254]'
Xmux_1level_tapbuf_size2[14] mux_1level_tapbuf_size2[14]->in[0] mux_1level_tapbuf_size2[14]->in[1] mux_1level_tapbuf_size2[14]->out sram[16]->outb sram[16]->out gvdd_mux_1level_tapbuf_size2[14] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[14], level=1, select_path_id=0. *****
*****1*****
Xsram[16] sram->in sram[16]->out sram[16]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[16]->out) 0
.nodeset V(sram[16]->outb) vsp
***** Signal mux_1level_tapbuf_size2[14]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[14]->in[0] mux_1level_tapbuf_size2[14]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[14]->in[1] density = 0.1906, probability=0.5218.*****
Vmux_1level_tapbuf_size2[14]->in[1] mux_1level_tapbuf_size2[14]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5218*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
Vgvdd_mux_1level_tapbuf_size2[14] gvdd_mux_1level_tapbuf_size2[14] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[256] trig v(mux_1level_tapbuf_size2[14]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[14]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[256] trig v(mux_1level_tapbuf_size2[14]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[14]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[256] when v(mux_1level_tapbuf_size2[14]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[256] trig v(mux_1level_tapbuf_size2[14]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[14]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[256] when v(mux_1level_tapbuf_size2[14]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[256] trig v(mux_1level_tapbuf_size2[14]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[14]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[14]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[14]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[256] param='mux_1level_tapbuf_size2[14]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[14]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[14]) from='clock_period' to='7*clock_period'
.meas tran mux_1level_tapbuf_size2[14]_energy_per_cycle param='mux_1level_tapbuf_size2[14]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[256]  param='mux_1level_tapbuf_size2[14]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[256]  param='dynamic_power_sb_mux[1][1]_rrnode[256]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[256] avg p(Vgvdd_mux_1level_tapbuf_size2[14]) from='start_rise_sb_mux[1][1]_rrnode[256]' to='start_rise_sb_mux[1][1]_rrnode[256]+switch_rise_sb_mux[1][1]_rrnode[256]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[256] avg p(Vgvdd_mux_1level_tapbuf_size2[14]) from='start_fall_sb_mux[1][1]_rrnode[256]' to='start_fall_sb_mux[1][1]_rrnode[256]+switch_fall_sb_mux[1][1]_rrnode[256]'
.meas tran sum_leakage_power_mux[0to14] 
+          param='sum_leakage_power_mux[0to13]+leakage_sb_mux[1][1]_rrnode[256]'
.meas tran sum_energy_per_cycle_mux[0to14] 
+          param='sum_energy_per_cycle_mux[0to13]+energy_per_cycle_sb_mux[1][1]_rrnode[256]'
***** Load for rr_node[256] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=29, type=5 *****
Xchan_mux_1level_tapbuf_size2[14]->out_loadlvl[0]_out mux_1level_tapbuf_size2[14]->out mux_1level_tapbuf_size2[14]->out_loadlvl[0]_out mux_1level_tapbuf_size2[14]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[16]_no0 mux_1level_tapbuf_size2[14]->out_loadlvl[0]_out mux_1level_tapbuf_size2[14]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to14] 
+          param='sum_leakage_power_sb_mux[0to13]+leakage_sb_mux[1][1]_rrnode[256]'
.meas tran sum_energy_per_cycle_sb_mux[0to14] 
+          param='sum_energy_per_cycle_sb_mux[0to13]+energy_per_cycle_sb_mux[1][1]_rrnode[256]'
Xmux_1level_tapbuf_size3[15] mux_1level_tapbuf_size3[15]->in[0] mux_1level_tapbuf_size3[15]->in[1] mux_1level_tapbuf_size3[15]->in[2] mux_1level_tapbuf_size3[15]->out sram[17]->out sram[17]->outb sram[18]->outb sram[18]->out sram[19]->out sram[19]->outb gvdd_mux_1level_tapbuf_size3[15] 0 mux_1level_tapbuf_size3
***** SRAM bits for MUX[15], level=1, select_path_id=1. *****
*****010*****
Xsram[17] sram->in sram[17]->out sram[17]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[17]->out) 0
.nodeset V(sram[17]->outb) vsp
Xsram[18] sram->in sram[18]->out sram[18]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[18]->out) 0
.nodeset V(sram[18]->outb) vsp
Xsram[19] sram->in sram[19]->out sram[19]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[19]->out) 0
.nodeset V(sram[19]->outb) vsp
***** Signal mux_1level_tapbuf_size3[15]->in[0] density = 0.1906, probability=0.5218.*****
Vmux_1level_tapbuf_size3[15]->in[0] mux_1level_tapbuf_size3[15]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5218*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
***** Signal mux_1level_tapbuf_size3[15]->in[1] density = 0.1906, probability=0.4782.*****
Vmux_1level_tapbuf_size3[15]->in[1] mux_1level_tapbuf_size3[15]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4782*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
***** Signal mux_1level_tapbuf_size3[15]->in[2] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[15]->in[2] mux_1level_tapbuf_size3[15]->in[2] 0 
+  0
Vgvdd_mux_1level_tapbuf_size3[15] gvdd_mux_1level_tapbuf_size3[15] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[168] trig v(mux_1level_tapbuf_size3[15]->in[1]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[15]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[168] trig v(mux_1level_tapbuf_size3[15]->in[1]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[15]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[168] when v(mux_1level_tapbuf_size3[15]->in[1])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[168] trig v(mux_1level_tapbuf_size3[15]->in[1]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[15]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[168] when v(mux_1level_tapbuf_size3[15]->in[1])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[168] trig v(mux_1level_tapbuf_size3[15]->in[1]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[15]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[15]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[15]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[168] param='mux_1level_tapbuf_size3[15]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[15]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[15]) from='clock_period' to='7*clock_period'
.meas tran mux_1level_tapbuf_size3[15]_energy_per_cycle param='mux_1level_tapbuf_size3[15]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[168]  param='mux_1level_tapbuf_size3[15]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[168]  param='dynamic_power_sb_mux[1][1]_rrnode[168]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[168] avg p(Vgvdd_mux_1level_tapbuf_size3[15]) from='start_rise_sb_mux[1][1]_rrnode[168]' to='start_rise_sb_mux[1][1]_rrnode[168]+switch_rise_sb_mux[1][1]_rrnode[168]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[168] avg p(Vgvdd_mux_1level_tapbuf_size3[15]) from='start_fall_sb_mux[1][1]_rrnode[168]' to='start_fall_sb_mux[1][1]_rrnode[168]+switch_fall_sb_mux[1][1]_rrnode[168]'
.meas tran sum_leakage_power_mux[0to15] 
+          param='sum_leakage_power_mux[0to14]+leakage_sb_mux[1][1]_rrnode[168]'
.meas tran sum_energy_per_cycle_mux[0to15] 
+          param='sum_energy_per_cycle_mux[0to14]+energy_per_cycle_sb_mux[1][1]_rrnode[168]'
***** Load for rr_node[168] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=1, type=4 *****
Xchan_mux_1level_tapbuf_size3[15]->out_loadlvl[0]_out mux_1level_tapbuf_size3[15]->out mux_1level_tapbuf_size3[15]->out_loadlvl[0]_out mux_1level_tapbuf_size3[15]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[17]_no0 mux_1level_tapbuf_size3[15]->out_loadlvl[0]_out mux_1level_tapbuf_size3[15]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[18]_no0 mux_1level_tapbuf_size3[15]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[15]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to15] 
+          param='sum_leakage_power_sb_mux[0to14]+leakage_sb_mux[1][1]_rrnode[168]'
.meas tran sum_energy_per_cycle_sb_mux[0to15] 
+          param='sum_energy_per_cycle_sb_mux[0to14]+energy_per_cycle_sb_mux[1][1]_rrnode[168]'
Xmux_1level_tapbuf_size3[16] mux_1level_tapbuf_size3[16]->in[0] mux_1level_tapbuf_size3[16]->in[1] mux_1level_tapbuf_size3[16]->in[2] mux_1level_tapbuf_size3[16]->out sram[20]->outb sram[20]->out sram[21]->out sram[21]->outb sram[22]->out sram[22]->outb gvdd_mux_1level_tapbuf_size3[16] 0 mux_1level_tapbuf_size3
***** SRAM bits for MUX[16], level=1, select_path_id=0. *****
*****100*****
Xsram[20] sram->in sram[20]->out sram[20]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[20]->out) 0
.nodeset V(sram[20]->outb) vsp
Xsram[21] sram->in sram[21]->out sram[21]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[21]->out) 0
.nodeset V(sram[21]->outb) vsp
Xsram[22] sram->in sram[22]->out sram[22]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[22]->out) 0
.nodeset V(sram[22]->outb) vsp
***** Signal mux_1level_tapbuf_size3[16]->in[0] density = 0.1906, probability=0.5218.*****
Vmux_1level_tapbuf_size3[16]->in[0] mux_1level_tapbuf_size3[16]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5218*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
***** Signal mux_1level_tapbuf_size3[16]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[16]->in[1] mux_1level_tapbuf_size3[16]->in[1] 0 
+  0
***** Signal mux_1level_tapbuf_size3[16]->in[2] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[16]->in[2] mux_1level_tapbuf_size3[16]->in[2] 0 
+  0
Vgvdd_mux_1level_tapbuf_size3[16] gvdd_mux_1level_tapbuf_size3[16] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[170] trig v(mux_1level_tapbuf_size3[16]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[16]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[170] trig v(mux_1level_tapbuf_size3[16]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[16]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[170] when v(mux_1level_tapbuf_size3[16]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[170] trig v(mux_1level_tapbuf_size3[16]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[16]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[170] when v(mux_1level_tapbuf_size3[16]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[170] trig v(mux_1level_tapbuf_size3[16]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[16]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[16]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[16]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[170] param='mux_1level_tapbuf_size3[16]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[16]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[16]) from='clock_period' to='7*clock_period'
.meas tran mux_1level_tapbuf_size3[16]_energy_per_cycle param='mux_1level_tapbuf_size3[16]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[170]  param='mux_1level_tapbuf_size3[16]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[170]  param='dynamic_power_sb_mux[1][1]_rrnode[170]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[170] avg p(Vgvdd_mux_1level_tapbuf_size3[16]) from='start_rise_sb_mux[1][1]_rrnode[170]' to='start_rise_sb_mux[1][1]_rrnode[170]+switch_rise_sb_mux[1][1]_rrnode[170]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[170] avg p(Vgvdd_mux_1level_tapbuf_size3[16]) from='start_fall_sb_mux[1][1]_rrnode[170]' to='start_fall_sb_mux[1][1]_rrnode[170]+switch_fall_sb_mux[1][1]_rrnode[170]'
.meas tran sum_leakage_power_mux[0to16] 
+          param='sum_leakage_power_mux[0to15]+leakage_sb_mux[1][1]_rrnode[170]'
.meas tran sum_energy_per_cycle_mux[0to16] 
+          param='sum_energy_per_cycle_mux[0to15]+energy_per_cycle_sb_mux[1][1]_rrnode[170]'
***** Load for rr_node[170] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=3, type=4 *****
Xchan_mux_1level_tapbuf_size3[16]->out_loadlvl[0]_out mux_1level_tapbuf_size3[16]->out mux_1level_tapbuf_size3[16]->out_loadlvl[0]_out mux_1level_tapbuf_size3[16]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[19]_no0 mux_1level_tapbuf_size3[16]->out_loadlvl[0]_out mux_1level_tapbuf_size3[16]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[20]_no0 mux_1level_tapbuf_size3[16]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[16]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to16] 
+          param='sum_leakage_power_sb_mux[0to15]+leakage_sb_mux[1][1]_rrnode[170]'
.meas tran sum_energy_per_cycle_sb_mux[0to16] 
+          param='sum_energy_per_cycle_sb_mux[0to15]+energy_per_cycle_sb_mux[1][1]_rrnode[170]'
Xmux_1level_tapbuf_size3[17] mux_1level_tapbuf_size3[17]->in[0] mux_1level_tapbuf_size3[17]->in[1] mux_1level_tapbuf_size3[17]->in[2] mux_1level_tapbuf_size3[17]->out sram[23]->outb sram[23]->out sram[24]->out sram[24]->outb sram[25]->out sram[25]->outb gvdd_mux_1level_tapbuf_size3[17] 0 mux_1level_tapbuf_size3
***** SRAM bits for MUX[17], level=1, select_path_id=0. *****
*****100*****
Xsram[23] sram->in sram[23]->out sram[23]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[23]->out) 0
.nodeset V(sram[23]->outb) vsp
Xsram[24] sram->in sram[24]->out sram[24]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[24]->out) 0
.nodeset V(sram[24]->outb) vsp
Xsram[25] sram->in sram[25]->out sram[25]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[25]->out) 0
.nodeset V(sram[25]->outb) vsp
***** Signal mux_1level_tapbuf_size3[17]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[17]->in[0] mux_1level_tapbuf_size3[17]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size3[17]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[17]->in[1] mux_1level_tapbuf_size3[17]->in[1] 0 
+  0
***** Signal mux_1level_tapbuf_size3[17]->in[2] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[17]->in[2] mux_1level_tapbuf_size3[17]->in[2] 0 
+  0
Vgvdd_mux_1level_tapbuf_size3[17] gvdd_mux_1level_tapbuf_size3[17] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[172] trig v(mux_1level_tapbuf_size3[17]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[17]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[172] trig v(mux_1level_tapbuf_size3[17]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[17]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[172] when v(mux_1level_tapbuf_size3[17]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[172] trig v(mux_1level_tapbuf_size3[17]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[17]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[172] when v(mux_1level_tapbuf_size3[17]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[172] trig v(mux_1level_tapbuf_size3[17]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[17]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[17]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[17]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[172] param='mux_1level_tapbuf_size3[17]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[17]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[17]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size3[17]_energy_per_cycle param='mux_1level_tapbuf_size3[17]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[172]  param='mux_1level_tapbuf_size3[17]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[172]  param='dynamic_power_sb_mux[1][1]_rrnode[172]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[172] avg p(Vgvdd_mux_1level_tapbuf_size3[17]) from='start_rise_sb_mux[1][1]_rrnode[172]' to='start_rise_sb_mux[1][1]_rrnode[172]+switch_rise_sb_mux[1][1]_rrnode[172]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[172] avg p(Vgvdd_mux_1level_tapbuf_size3[17]) from='start_fall_sb_mux[1][1]_rrnode[172]' to='start_fall_sb_mux[1][1]_rrnode[172]+switch_fall_sb_mux[1][1]_rrnode[172]'
.meas tran sum_leakage_power_mux[0to17] 
+          param='sum_leakage_power_mux[0to16]+leakage_sb_mux[1][1]_rrnode[172]'
.meas tran sum_energy_per_cycle_mux[0to17] 
+          param='sum_energy_per_cycle_mux[0to16]+energy_per_cycle_sb_mux[1][1]_rrnode[172]'
***** Load for rr_node[172] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=5, type=4 *****
Xchan_mux_1level_tapbuf_size3[17]->out_loadlvl[0]_out mux_1level_tapbuf_size3[17]->out mux_1level_tapbuf_size3[17]->out_loadlvl[0]_out mux_1level_tapbuf_size3[17]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[21]_no0 mux_1level_tapbuf_size3[17]->out_loadlvl[0]_out mux_1level_tapbuf_size3[17]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[22]_no0 mux_1level_tapbuf_size3[17]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[17]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to17] 
+          param='sum_leakage_power_sb_mux[0to16]+leakage_sb_mux[1][1]_rrnode[172]'
.meas tran sum_energy_per_cycle_sb_mux[0to17] 
+          param='sum_energy_per_cycle_sb_mux[0to16]+energy_per_cycle_sb_mux[1][1]_rrnode[172]'
Xmux_1level_tapbuf_size2[18] mux_1level_tapbuf_size2[18]->in[0] mux_1level_tapbuf_size2[18]->in[1] mux_1level_tapbuf_size2[18]->out sram[26]->outb sram[26]->out gvdd_mux_1level_tapbuf_size2[18] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[18], level=1, select_path_id=0. *****
*****1*****
Xsram[26] sram->in sram[26]->out sram[26]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[26]->out) 0
.nodeset V(sram[26]->outb) vsp
***** Signal mux_1level_tapbuf_size2[18]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[18]->in[0] mux_1level_tapbuf_size2[18]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[18]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[18]->in[1] mux_1level_tapbuf_size2[18]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[18] gvdd_mux_1level_tapbuf_size2[18] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[174] trig v(mux_1level_tapbuf_size2[18]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[18]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[174] trig v(mux_1level_tapbuf_size2[18]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[18]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[174] when v(mux_1level_tapbuf_size2[18]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[174] trig v(mux_1level_tapbuf_size2[18]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[18]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[174] when v(mux_1level_tapbuf_size2[18]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[174] trig v(mux_1level_tapbuf_size2[18]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[18]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[18]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[18]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[174] param='mux_1level_tapbuf_size2[18]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[18]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[18]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[18]_energy_per_cycle param='mux_1level_tapbuf_size2[18]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[174]  param='mux_1level_tapbuf_size2[18]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[174]  param='dynamic_power_sb_mux[1][1]_rrnode[174]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[174] avg p(Vgvdd_mux_1level_tapbuf_size2[18]) from='start_rise_sb_mux[1][1]_rrnode[174]' to='start_rise_sb_mux[1][1]_rrnode[174]+switch_rise_sb_mux[1][1]_rrnode[174]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[174] avg p(Vgvdd_mux_1level_tapbuf_size2[18]) from='start_fall_sb_mux[1][1]_rrnode[174]' to='start_fall_sb_mux[1][1]_rrnode[174]+switch_fall_sb_mux[1][1]_rrnode[174]'
.meas tran sum_leakage_power_mux[0to18] 
+          param='sum_leakage_power_mux[0to17]+leakage_sb_mux[1][1]_rrnode[174]'
.meas tran sum_energy_per_cycle_mux[0to18] 
+          param='sum_energy_per_cycle_mux[0to17]+energy_per_cycle_sb_mux[1][1]_rrnode[174]'
***** Load for rr_node[174] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=7, type=4 *****
Xchan_mux_1level_tapbuf_size2[18]->out_loadlvl[0]_out mux_1level_tapbuf_size2[18]->out mux_1level_tapbuf_size2[18]->out_loadlvl[0]_out mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[23]_no0 mux_1level_tapbuf_size2[18]->out_loadlvl[0]_out mux_1level_tapbuf_size2[18]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[24]_no0 mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[25]_no0 mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to18] 
+          param='sum_leakage_power_sb_mux[0to17]+leakage_sb_mux[1][1]_rrnode[174]'
.meas tran sum_energy_per_cycle_sb_mux[0to18] 
+          param='sum_energy_per_cycle_sb_mux[0to17]+energy_per_cycle_sb_mux[1][1]_rrnode[174]'
Xmux_1level_tapbuf_size2[19] mux_1level_tapbuf_size2[19]->in[0] mux_1level_tapbuf_size2[19]->in[1] mux_1level_tapbuf_size2[19]->out sram[27]->outb sram[27]->out gvdd_mux_1level_tapbuf_size2[19] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[19], level=1, select_path_id=0. *****
*****1*****
Xsram[27] sram->in sram[27]->out sram[27]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[27]->out) 0
.nodeset V(sram[27]->outb) vsp
***** Signal mux_1level_tapbuf_size2[19]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[19]->in[0] mux_1level_tapbuf_size2[19]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[19]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[19]->in[1] mux_1level_tapbuf_size2[19]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[19] gvdd_mux_1level_tapbuf_size2[19] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[176] trig v(mux_1level_tapbuf_size2[19]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[19]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[176] trig v(mux_1level_tapbuf_size2[19]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[19]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[176] when v(mux_1level_tapbuf_size2[19]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[176] trig v(mux_1level_tapbuf_size2[19]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[19]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[176] when v(mux_1level_tapbuf_size2[19]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[176] trig v(mux_1level_tapbuf_size2[19]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[19]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[19]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[19]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[176] param='mux_1level_tapbuf_size2[19]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[19]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[19]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[19]_energy_per_cycle param='mux_1level_tapbuf_size2[19]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[176]  param='mux_1level_tapbuf_size2[19]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[176]  param='dynamic_power_sb_mux[1][1]_rrnode[176]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[176] avg p(Vgvdd_mux_1level_tapbuf_size2[19]) from='start_rise_sb_mux[1][1]_rrnode[176]' to='start_rise_sb_mux[1][1]_rrnode[176]+switch_rise_sb_mux[1][1]_rrnode[176]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[176] avg p(Vgvdd_mux_1level_tapbuf_size2[19]) from='start_fall_sb_mux[1][1]_rrnode[176]' to='start_fall_sb_mux[1][1]_rrnode[176]+switch_fall_sb_mux[1][1]_rrnode[176]'
.meas tran sum_leakage_power_mux[0to19] 
+          param='sum_leakage_power_mux[0to18]+leakage_sb_mux[1][1]_rrnode[176]'
.meas tran sum_energy_per_cycle_mux[0to19] 
+          param='sum_energy_per_cycle_mux[0to18]+energy_per_cycle_sb_mux[1][1]_rrnode[176]'
***** Load for rr_node[176] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=9, type=4 *****
Xchan_mux_1level_tapbuf_size2[19]->out_loadlvl[0]_out mux_1level_tapbuf_size2[19]->out mux_1level_tapbuf_size2[19]->out_loadlvl[0]_out mux_1level_tapbuf_size2[19]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[26]_no0 mux_1level_tapbuf_size2[19]->out_loadlvl[0]_out mux_1level_tapbuf_size2[19]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[27]_no0 mux_1level_tapbuf_size2[19]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[19]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to19] 
+          param='sum_leakage_power_sb_mux[0to18]+leakage_sb_mux[1][1]_rrnode[176]'
.meas tran sum_energy_per_cycle_sb_mux[0to19] 
+          param='sum_energy_per_cycle_sb_mux[0to18]+energy_per_cycle_sb_mux[1][1]_rrnode[176]'
Xmux_1level_tapbuf_size2[20] mux_1level_tapbuf_size2[20]->in[0] mux_1level_tapbuf_size2[20]->in[1] mux_1level_tapbuf_size2[20]->out sram[28]->outb sram[28]->out gvdd_mux_1level_tapbuf_size2[20] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[20], level=1, select_path_id=0. *****
*****1*****
Xsram[28] sram->in sram[28]->out sram[28]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[28]->out) 0
.nodeset V(sram[28]->outb) vsp
***** Signal mux_1level_tapbuf_size2[20]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[20]->in[0] mux_1level_tapbuf_size2[20]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[20]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[20]->in[1] mux_1level_tapbuf_size2[20]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[20] gvdd_mux_1level_tapbuf_size2[20] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[178] trig v(mux_1level_tapbuf_size2[20]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[20]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[178] trig v(mux_1level_tapbuf_size2[20]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[20]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[178] when v(mux_1level_tapbuf_size2[20]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[178] trig v(mux_1level_tapbuf_size2[20]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[20]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[178] when v(mux_1level_tapbuf_size2[20]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[178] trig v(mux_1level_tapbuf_size2[20]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[20]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[20]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[20]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[178] param='mux_1level_tapbuf_size2[20]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[20]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[20]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[20]_energy_per_cycle param='mux_1level_tapbuf_size2[20]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[178]  param='mux_1level_tapbuf_size2[20]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[178]  param='dynamic_power_sb_mux[1][1]_rrnode[178]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[178] avg p(Vgvdd_mux_1level_tapbuf_size2[20]) from='start_rise_sb_mux[1][1]_rrnode[178]' to='start_rise_sb_mux[1][1]_rrnode[178]+switch_rise_sb_mux[1][1]_rrnode[178]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[178] avg p(Vgvdd_mux_1level_tapbuf_size2[20]) from='start_fall_sb_mux[1][1]_rrnode[178]' to='start_fall_sb_mux[1][1]_rrnode[178]+switch_fall_sb_mux[1][1]_rrnode[178]'
.meas tran sum_leakage_power_mux[0to20] 
+          param='sum_leakage_power_mux[0to19]+leakage_sb_mux[1][1]_rrnode[178]'
.meas tran sum_energy_per_cycle_mux[0to20] 
+          param='sum_energy_per_cycle_mux[0to19]+energy_per_cycle_sb_mux[1][1]_rrnode[178]'
***** Load for rr_node[178] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=11, type=4 *****
Xchan_mux_1level_tapbuf_size2[20]->out_loadlvl[0]_out mux_1level_tapbuf_size2[20]->out mux_1level_tapbuf_size2[20]->out_loadlvl[0]_out mux_1level_tapbuf_size2[20]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[28]_no0 mux_1level_tapbuf_size2[20]->out_loadlvl[0]_out mux_1level_tapbuf_size2[20]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[29]_no0 mux_1level_tapbuf_size2[20]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[20]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to20] 
+          param='sum_leakage_power_sb_mux[0to19]+leakage_sb_mux[1][1]_rrnode[178]'
.meas tran sum_energy_per_cycle_sb_mux[0to20] 
+          param='sum_energy_per_cycle_sb_mux[0to19]+energy_per_cycle_sb_mux[1][1]_rrnode[178]'
Xmux_1level_tapbuf_size2[21] mux_1level_tapbuf_size2[21]->in[0] mux_1level_tapbuf_size2[21]->in[1] mux_1level_tapbuf_size2[21]->out sram[29]->outb sram[29]->out gvdd_mux_1level_tapbuf_size2[21] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[21], level=1, select_path_id=0. *****
*****1*****
Xsram[29] sram->in sram[29]->out sram[29]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[29]->out) 0
.nodeset V(sram[29]->outb) vsp
***** Signal mux_1level_tapbuf_size2[21]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[21]->in[0] mux_1level_tapbuf_size2[21]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[21]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[21]->in[1] mux_1level_tapbuf_size2[21]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[21] gvdd_mux_1level_tapbuf_size2[21] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[180] trig v(mux_1level_tapbuf_size2[21]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[21]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[180] trig v(mux_1level_tapbuf_size2[21]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[21]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[180] when v(mux_1level_tapbuf_size2[21]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[180] trig v(mux_1level_tapbuf_size2[21]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[21]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[180] when v(mux_1level_tapbuf_size2[21]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[180] trig v(mux_1level_tapbuf_size2[21]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[21]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[21]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[21]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[180] param='mux_1level_tapbuf_size2[21]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[21]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[21]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[21]_energy_per_cycle param='mux_1level_tapbuf_size2[21]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[180]  param='mux_1level_tapbuf_size2[21]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[180]  param='dynamic_power_sb_mux[1][1]_rrnode[180]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[180] avg p(Vgvdd_mux_1level_tapbuf_size2[21]) from='start_rise_sb_mux[1][1]_rrnode[180]' to='start_rise_sb_mux[1][1]_rrnode[180]+switch_rise_sb_mux[1][1]_rrnode[180]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[180] avg p(Vgvdd_mux_1level_tapbuf_size2[21]) from='start_fall_sb_mux[1][1]_rrnode[180]' to='start_fall_sb_mux[1][1]_rrnode[180]+switch_fall_sb_mux[1][1]_rrnode[180]'
.meas tran sum_leakage_power_mux[0to21] 
+          param='sum_leakage_power_mux[0to20]+leakage_sb_mux[1][1]_rrnode[180]'
.meas tran sum_energy_per_cycle_mux[0to21] 
+          param='sum_energy_per_cycle_mux[0to20]+energy_per_cycle_sb_mux[1][1]_rrnode[180]'
***** Load for rr_node[180] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=13, type=4 *****
Xchan_mux_1level_tapbuf_size2[21]->out_loadlvl[0]_out mux_1level_tapbuf_size2[21]->out mux_1level_tapbuf_size2[21]->out_loadlvl[0]_out mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[30]_no0 mux_1level_tapbuf_size2[21]->out_loadlvl[0]_out mux_1level_tapbuf_size2[21]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[31]_no0 mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[32]_no0 mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[33]_no0 mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to21] 
+          param='sum_leakage_power_sb_mux[0to20]+leakage_sb_mux[1][1]_rrnode[180]'
.meas tran sum_energy_per_cycle_sb_mux[0to21] 
+          param='sum_energy_per_cycle_sb_mux[0to20]+energy_per_cycle_sb_mux[1][1]_rrnode[180]'
Xmux_1level_tapbuf_size2[22] mux_1level_tapbuf_size2[22]->in[0] mux_1level_tapbuf_size2[22]->in[1] mux_1level_tapbuf_size2[22]->out sram[30]->outb sram[30]->out gvdd_mux_1level_tapbuf_size2[22] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[22], level=1, select_path_id=0. *****
*****1*****
Xsram[30] sram->in sram[30]->out sram[30]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[30]->out) 0
.nodeset V(sram[30]->outb) vsp
***** Signal mux_1level_tapbuf_size2[22]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[22]->in[0] mux_1level_tapbuf_size2[22]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[22]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[22]->in[1] mux_1level_tapbuf_size2[22]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[22] gvdd_mux_1level_tapbuf_size2[22] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[182] trig v(mux_1level_tapbuf_size2[22]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[22]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[182] trig v(mux_1level_tapbuf_size2[22]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[22]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[182] when v(mux_1level_tapbuf_size2[22]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[182] trig v(mux_1level_tapbuf_size2[22]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[22]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[182] when v(mux_1level_tapbuf_size2[22]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[182] trig v(mux_1level_tapbuf_size2[22]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[22]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[22]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[22]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[182] param='mux_1level_tapbuf_size2[22]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[22]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[22]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[22]_energy_per_cycle param='mux_1level_tapbuf_size2[22]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[182]  param='mux_1level_tapbuf_size2[22]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[182]  param='dynamic_power_sb_mux[1][1]_rrnode[182]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[182] avg p(Vgvdd_mux_1level_tapbuf_size2[22]) from='start_rise_sb_mux[1][1]_rrnode[182]' to='start_rise_sb_mux[1][1]_rrnode[182]+switch_rise_sb_mux[1][1]_rrnode[182]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[182] avg p(Vgvdd_mux_1level_tapbuf_size2[22]) from='start_fall_sb_mux[1][1]_rrnode[182]' to='start_fall_sb_mux[1][1]_rrnode[182]+switch_fall_sb_mux[1][1]_rrnode[182]'
.meas tran sum_leakage_power_mux[0to22] 
+          param='sum_leakage_power_mux[0to21]+leakage_sb_mux[1][1]_rrnode[182]'
.meas tran sum_energy_per_cycle_mux[0to22] 
+          param='sum_energy_per_cycle_mux[0to21]+energy_per_cycle_sb_mux[1][1]_rrnode[182]'
***** Load for rr_node[182] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=15, type=4 *****
Xchan_mux_1level_tapbuf_size2[22]->out_loadlvl[0]_out mux_1level_tapbuf_size2[22]->out mux_1level_tapbuf_size2[22]->out_loadlvl[0]_out mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[34]_no0 mux_1level_tapbuf_size2[22]->out_loadlvl[0]_out mux_1level_tapbuf_size2[22]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[35]_no0 mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to22] 
+          param='sum_leakage_power_sb_mux[0to21]+leakage_sb_mux[1][1]_rrnode[182]'
.meas tran sum_energy_per_cycle_sb_mux[0to22] 
+          param='sum_energy_per_cycle_sb_mux[0to21]+energy_per_cycle_sb_mux[1][1]_rrnode[182]'
Xmux_1level_tapbuf_size2[23] mux_1level_tapbuf_size2[23]->in[0] mux_1level_tapbuf_size2[23]->in[1] mux_1level_tapbuf_size2[23]->out sram[31]->outb sram[31]->out gvdd_mux_1level_tapbuf_size2[23] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[23], level=1, select_path_id=0. *****
*****1*****
Xsram[31] sram->in sram[31]->out sram[31]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[31]->out) 0
.nodeset V(sram[31]->outb) vsp
***** Signal mux_1level_tapbuf_size2[23]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[23]->in[0] mux_1level_tapbuf_size2[23]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[23]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[23]->in[1] mux_1level_tapbuf_size2[23]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[23] gvdd_mux_1level_tapbuf_size2[23] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[184] trig v(mux_1level_tapbuf_size2[23]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[23]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[184] trig v(mux_1level_tapbuf_size2[23]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[23]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[184] when v(mux_1level_tapbuf_size2[23]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[184] trig v(mux_1level_tapbuf_size2[23]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[23]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[184] when v(mux_1level_tapbuf_size2[23]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[184] trig v(mux_1level_tapbuf_size2[23]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[23]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[23]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[23]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[184] param='mux_1level_tapbuf_size2[23]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[23]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[23]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[23]_energy_per_cycle param='mux_1level_tapbuf_size2[23]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[184]  param='mux_1level_tapbuf_size2[23]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[184]  param='dynamic_power_sb_mux[1][1]_rrnode[184]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[184] avg p(Vgvdd_mux_1level_tapbuf_size2[23]) from='start_rise_sb_mux[1][1]_rrnode[184]' to='start_rise_sb_mux[1][1]_rrnode[184]+switch_rise_sb_mux[1][1]_rrnode[184]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[184] avg p(Vgvdd_mux_1level_tapbuf_size2[23]) from='start_fall_sb_mux[1][1]_rrnode[184]' to='start_fall_sb_mux[1][1]_rrnode[184]+switch_fall_sb_mux[1][1]_rrnode[184]'
.meas tran sum_leakage_power_mux[0to23] 
+          param='sum_leakage_power_mux[0to22]+leakage_sb_mux[1][1]_rrnode[184]'
.meas tran sum_energy_per_cycle_mux[0to23] 
+          param='sum_energy_per_cycle_mux[0to22]+energy_per_cycle_sb_mux[1][1]_rrnode[184]'
***** Load for rr_node[184] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=17, type=4 *****
Xchan_mux_1level_tapbuf_size2[23]->out_loadlvl[0]_out mux_1level_tapbuf_size2[23]->out mux_1level_tapbuf_size2[23]->out_loadlvl[0]_out mux_1level_tapbuf_size2[23]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[36]_no0 mux_1level_tapbuf_size2[23]->out_loadlvl[0]_out mux_1level_tapbuf_size2[23]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[37]_no0 mux_1level_tapbuf_size2[23]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[23]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to23] 
+          param='sum_leakage_power_sb_mux[0to22]+leakage_sb_mux[1][1]_rrnode[184]'
.meas tran sum_energy_per_cycle_sb_mux[0to23] 
+          param='sum_energy_per_cycle_sb_mux[0to22]+energy_per_cycle_sb_mux[1][1]_rrnode[184]'
Xmux_1level_tapbuf_size2[24] mux_1level_tapbuf_size2[24]->in[0] mux_1level_tapbuf_size2[24]->in[1] mux_1level_tapbuf_size2[24]->out sram[32]->outb sram[32]->out gvdd_mux_1level_tapbuf_size2[24] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[24], level=1, select_path_id=0. *****
*****1*****
Xsram[32] sram->in sram[32]->out sram[32]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[32]->out) 0
.nodeset V(sram[32]->outb) vsp
***** Signal mux_1level_tapbuf_size2[24]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[24]->in[0] mux_1level_tapbuf_size2[24]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[24]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[24]->in[1] mux_1level_tapbuf_size2[24]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[24] gvdd_mux_1level_tapbuf_size2[24] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[186] trig v(mux_1level_tapbuf_size2[24]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[24]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[186] trig v(mux_1level_tapbuf_size2[24]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[24]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[186] when v(mux_1level_tapbuf_size2[24]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[186] trig v(mux_1level_tapbuf_size2[24]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[24]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[186] when v(mux_1level_tapbuf_size2[24]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[186] trig v(mux_1level_tapbuf_size2[24]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[24]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[24]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[24]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[186] param='mux_1level_tapbuf_size2[24]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[24]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[24]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[24]_energy_per_cycle param='mux_1level_tapbuf_size2[24]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[186]  param='mux_1level_tapbuf_size2[24]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[186]  param='dynamic_power_sb_mux[1][1]_rrnode[186]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[186] avg p(Vgvdd_mux_1level_tapbuf_size2[24]) from='start_rise_sb_mux[1][1]_rrnode[186]' to='start_rise_sb_mux[1][1]_rrnode[186]+switch_rise_sb_mux[1][1]_rrnode[186]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[186] avg p(Vgvdd_mux_1level_tapbuf_size2[24]) from='start_fall_sb_mux[1][1]_rrnode[186]' to='start_fall_sb_mux[1][1]_rrnode[186]+switch_fall_sb_mux[1][1]_rrnode[186]'
.meas tran sum_leakage_power_mux[0to24] 
+          param='sum_leakage_power_mux[0to23]+leakage_sb_mux[1][1]_rrnode[186]'
.meas tran sum_energy_per_cycle_mux[0to24] 
+          param='sum_energy_per_cycle_mux[0to23]+energy_per_cycle_sb_mux[1][1]_rrnode[186]'
***** Load for rr_node[186] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=19, type=4 *****
Xchan_mux_1level_tapbuf_size2[24]->out_loadlvl[0]_out mux_1level_tapbuf_size2[24]->out mux_1level_tapbuf_size2[24]->out_loadlvl[0]_out mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[38]_no0 mux_1level_tapbuf_size2[24]->out_loadlvl[0]_out mux_1level_tapbuf_size2[24]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[39]_no0 mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to24] 
+          param='sum_leakage_power_sb_mux[0to23]+leakage_sb_mux[1][1]_rrnode[186]'
.meas tran sum_energy_per_cycle_sb_mux[0to24] 
+          param='sum_energy_per_cycle_sb_mux[0to23]+energy_per_cycle_sb_mux[1][1]_rrnode[186]'
Xmux_1level_tapbuf_size2[25] mux_1level_tapbuf_size2[25]->in[0] mux_1level_tapbuf_size2[25]->in[1] mux_1level_tapbuf_size2[25]->out sram[33]->outb sram[33]->out gvdd_mux_1level_tapbuf_size2[25] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[25], level=1, select_path_id=0. *****
*****1*****
Xsram[33] sram->in sram[33]->out sram[33]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[33]->out) 0
.nodeset V(sram[33]->outb) vsp
***** Signal mux_1level_tapbuf_size2[25]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[25]->in[0] mux_1level_tapbuf_size2[25]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[25]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[25]->in[1] mux_1level_tapbuf_size2[25]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[25] gvdd_mux_1level_tapbuf_size2[25] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[188] trig v(mux_1level_tapbuf_size2[25]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[25]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[188] trig v(mux_1level_tapbuf_size2[25]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[25]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[188] when v(mux_1level_tapbuf_size2[25]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[188] trig v(mux_1level_tapbuf_size2[25]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[25]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[188] when v(mux_1level_tapbuf_size2[25]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[188] trig v(mux_1level_tapbuf_size2[25]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[25]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[25]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[25]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[188] param='mux_1level_tapbuf_size2[25]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[25]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[25]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[25]_energy_per_cycle param='mux_1level_tapbuf_size2[25]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[188]  param='mux_1level_tapbuf_size2[25]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[188]  param='dynamic_power_sb_mux[1][1]_rrnode[188]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[188] avg p(Vgvdd_mux_1level_tapbuf_size2[25]) from='start_rise_sb_mux[1][1]_rrnode[188]' to='start_rise_sb_mux[1][1]_rrnode[188]+switch_rise_sb_mux[1][1]_rrnode[188]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[188] avg p(Vgvdd_mux_1level_tapbuf_size2[25]) from='start_fall_sb_mux[1][1]_rrnode[188]' to='start_fall_sb_mux[1][1]_rrnode[188]+switch_fall_sb_mux[1][1]_rrnode[188]'
.meas tran sum_leakage_power_mux[0to25] 
+          param='sum_leakage_power_mux[0to24]+leakage_sb_mux[1][1]_rrnode[188]'
.meas tran sum_energy_per_cycle_mux[0to25] 
+          param='sum_energy_per_cycle_mux[0to24]+energy_per_cycle_sb_mux[1][1]_rrnode[188]'
***** Load for rr_node[188] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=21, type=4 *****
Xchan_mux_1level_tapbuf_size2[25]->out_loadlvl[0]_out mux_1level_tapbuf_size2[25]->out mux_1level_tapbuf_size2[25]->out_loadlvl[0]_out mux_1level_tapbuf_size2[25]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[40]_no0 mux_1level_tapbuf_size2[25]->out_loadlvl[0]_out mux_1level_tapbuf_size2[25]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[41]_no0 mux_1level_tapbuf_size2[25]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[25]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to25] 
+          param='sum_leakage_power_sb_mux[0to24]+leakage_sb_mux[1][1]_rrnode[188]'
.meas tran sum_energy_per_cycle_sb_mux[0to25] 
+          param='sum_energy_per_cycle_sb_mux[0to24]+energy_per_cycle_sb_mux[1][1]_rrnode[188]'
Xmux_1level_tapbuf_size2[26] mux_1level_tapbuf_size2[26]->in[0] mux_1level_tapbuf_size2[26]->in[1] mux_1level_tapbuf_size2[26]->out sram[34]->outb sram[34]->out gvdd_mux_1level_tapbuf_size2[26] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[26], level=1, select_path_id=0. *****
*****1*****
Xsram[34] sram->in sram[34]->out sram[34]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[34]->out) 0
.nodeset V(sram[34]->outb) vsp
***** Signal mux_1level_tapbuf_size2[26]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[26]->in[0] mux_1level_tapbuf_size2[26]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[26]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[26]->in[1] mux_1level_tapbuf_size2[26]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[26] gvdd_mux_1level_tapbuf_size2[26] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[190] trig v(mux_1level_tapbuf_size2[26]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[26]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[190] trig v(mux_1level_tapbuf_size2[26]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[26]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[190] when v(mux_1level_tapbuf_size2[26]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[190] trig v(mux_1level_tapbuf_size2[26]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[26]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[190] when v(mux_1level_tapbuf_size2[26]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[190] trig v(mux_1level_tapbuf_size2[26]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[26]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[26]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[26]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[190] param='mux_1level_tapbuf_size2[26]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[26]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[26]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[26]_energy_per_cycle param='mux_1level_tapbuf_size2[26]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[190]  param='mux_1level_tapbuf_size2[26]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[190]  param='dynamic_power_sb_mux[1][1]_rrnode[190]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[190] avg p(Vgvdd_mux_1level_tapbuf_size2[26]) from='start_rise_sb_mux[1][1]_rrnode[190]' to='start_rise_sb_mux[1][1]_rrnode[190]+switch_rise_sb_mux[1][1]_rrnode[190]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[190] avg p(Vgvdd_mux_1level_tapbuf_size2[26]) from='start_fall_sb_mux[1][1]_rrnode[190]' to='start_fall_sb_mux[1][1]_rrnode[190]+switch_fall_sb_mux[1][1]_rrnode[190]'
.meas tran sum_leakage_power_mux[0to26] 
+          param='sum_leakage_power_mux[0to25]+leakage_sb_mux[1][1]_rrnode[190]'
.meas tran sum_energy_per_cycle_mux[0to26] 
+          param='sum_energy_per_cycle_mux[0to25]+energy_per_cycle_sb_mux[1][1]_rrnode[190]'
***** Load for rr_node[190] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=23, type=4 *****
Xchan_mux_1level_tapbuf_size2[26]->out_loadlvl[0]_out mux_1level_tapbuf_size2[26]->out mux_1level_tapbuf_size2[26]->out_loadlvl[0]_out mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[42]_no0 mux_1level_tapbuf_size2[26]->out_loadlvl[0]_out mux_1level_tapbuf_size2[26]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[43]_no0 mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to26] 
+          param='sum_leakage_power_sb_mux[0to25]+leakage_sb_mux[1][1]_rrnode[190]'
.meas tran sum_energy_per_cycle_sb_mux[0to26] 
+          param='sum_energy_per_cycle_sb_mux[0to25]+energy_per_cycle_sb_mux[1][1]_rrnode[190]'
Xmux_1level_tapbuf_size2[27] mux_1level_tapbuf_size2[27]->in[0] mux_1level_tapbuf_size2[27]->in[1] mux_1level_tapbuf_size2[27]->out sram[35]->outb sram[35]->out gvdd_mux_1level_tapbuf_size2[27] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[27], level=1, select_path_id=0. *****
*****1*****
Xsram[35] sram->in sram[35]->out sram[35]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[35]->out) 0
.nodeset V(sram[35]->outb) vsp
***** Signal mux_1level_tapbuf_size2[27]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[27]->in[0] mux_1level_tapbuf_size2[27]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[27]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[27]->in[1] mux_1level_tapbuf_size2[27]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[27] gvdd_mux_1level_tapbuf_size2[27] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[192] trig v(mux_1level_tapbuf_size2[27]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[27]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[192] trig v(mux_1level_tapbuf_size2[27]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[27]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[192] when v(mux_1level_tapbuf_size2[27]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[192] trig v(mux_1level_tapbuf_size2[27]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[27]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[192] when v(mux_1level_tapbuf_size2[27]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[192] trig v(mux_1level_tapbuf_size2[27]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[27]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[27]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[27]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[192] param='mux_1level_tapbuf_size2[27]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[27]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[27]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[27]_energy_per_cycle param='mux_1level_tapbuf_size2[27]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[192]  param='mux_1level_tapbuf_size2[27]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[192]  param='dynamic_power_sb_mux[1][1]_rrnode[192]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[192] avg p(Vgvdd_mux_1level_tapbuf_size2[27]) from='start_rise_sb_mux[1][1]_rrnode[192]' to='start_rise_sb_mux[1][1]_rrnode[192]+switch_rise_sb_mux[1][1]_rrnode[192]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[192] avg p(Vgvdd_mux_1level_tapbuf_size2[27]) from='start_fall_sb_mux[1][1]_rrnode[192]' to='start_fall_sb_mux[1][1]_rrnode[192]+switch_fall_sb_mux[1][1]_rrnode[192]'
.meas tran sum_leakage_power_mux[0to27] 
+          param='sum_leakage_power_mux[0to26]+leakage_sb_mux[1][1]_rrnode[192]'
.meas tran sum_energy_per_cycle_mux[0to27] 
+          param='sum_energy_per_cycle_mux[0to26]+energy_per_cycle_sb_mux[1][1]_rrnode[192]'
***** Load for rr_node[192] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=25, type=4 *****
Xchan_mux_1level_tapbuf_size2[27]->out_loadlvl[0]_out mux_1level_tapbuf_size2[27]->out mux_1level_tapbuf_size2[27]->out_loadlvl[0]_out mux_1level_tapbuf_size2[27]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[44]_no0 mux_1level_tapbuf_size2[27]->out_loadlvl[0]_out mux_1level_tapbuf_size2[27]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[45]_no0 mux_1level_tapbuf_size2[27]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[27]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to27] 
+          param='sum_leakage_power_sb_mux[0to26]+leakage_sb_mux[1][1]_rrnode[192]'
.meas tran sum_energy_per_cycle_sb_mux[0to27] 
+          param='sum_energy_per_cycle_sb_mux[0to26]+energy_per_cycle_sb_mux[1][1]_rrnode[192]'
Xmux_1level_tapbuf_size2[28] mux_1level_tapbuf_size2[28]->in[0] mux_1level_tapbuf_size2[28]->in[1] mux_1level_tapbuf_size2[28]->out sram[36]->outb sram[36]->out gvdd_mux_1level_tapbuf_size2[28] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[28], level=1, select_path_id=0. *****
*****1*****
Xsram[36] sram->in sram[36]->out sram[36]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[36]->out) 0
.nodeset V(sram[36]->outb) vsp
***** Signal mux_1level_tapbuf_size2[28]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[28]->in[0] mux_1level_tapbuf_size2[28]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[28]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[28]->in[1] mux_1level_tapbuf_size2[28]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[28] gvdd_mux_1level_tapbuf_size2[28] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[194] trig v(mux_1level_tapbuf_size2[28]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[28]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[194] trig v(mux_1level_tapbuf_size2[28]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[28]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[194] when v(mux_1level_tapbuf_size2[28]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[194] trig v(mux_1level_tapbuf_size2[28]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[28]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[194] when v(mux_1level_tapbuf_size2[28]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[194] trig v(mux_1level_tapbuf_size2[28]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[28]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[28]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[28]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[194] param='mux_1level_tapbuf_size2[28]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[28]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[28]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[28]_energy_per_cycle param='mux_1level_tapbuf_size2[28]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[194]  param='mux_1level_tapbuf_size2[28]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[194]  param='dynamic_power_sb_mux[1][1]_rrnode[194]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[194] avg p(Vgvdd_mux_1level_tapbuf_size2[28]) from='start_rise_sb_mux[1][1]_rrnode[194]' to='start_rise_sb_mux[1][1]_rrnode[194]+switch_rise_sb_mux[1][1]_rrnode[194]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[194] avg p(Vgvdd_mux_1level_tapbuf_size2[28]) from='start_fall_sb_mux[1][1]_rrnode[194]' to='start_fall_sb_mux[1][1]_rrnode[194]+switch_fall_sb_mux[1][1]_rrnode[194]'
.meas tran sum_leakage_power_mux[0to28] 
+          param='sum_leakage_power_mux[0to27]+leakage_sb_mux[1][1]_rrnode[194]'
.meas tran sum_energy_per_cycle_mux[0to28] 
+          param='sum_energy_per_cycle_mux[0to27]+energy_per_cycle_sb_mux[1][1]_rrnode[194]'
***** Load for rr_node[194] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=27, type=4 *****
Xchan_mux_1level_tapbuf_size2[28]->out_loadlvl[0]_out mux_1level_tapbuf_size2[28]->out mux_1level_tapbuf_size2[28]->out_loadlvl[0]_out mux_1level_tapbuf_size2[28]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[46]_no0 mux_1level_tapbuf_size2[28]->out_loadlvl[0]_out mux_1level_tapbuf_size2[28]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[47]_no0 mux_1level_tapbuf_size2[28]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[28]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to28] 
+          param='sum_leakage_power_sb_mux[0to27]+leakage_sb_mux[1][1]_rrnode[194]'
.meas tran sum_energy_per_cycle_sb_mux[0to28] 
+          param='sum_energy_per_cycle_sb_mux[0to27]+energy_per_cycle_sb_mux[1][1]_rrnode[194]'
Xmux_1level_tapbuf_size2[29] mux_1level_tapbuf_size2[29]->in[0] mux_1level_tapbuf_size2[29]->in[1] mux_1level_tapbuf_size2[29]->out sram[37]->outb sram[37]->out gvdd_mux_1level_tapbuf_size2[29] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[29], level=1, select_path_id=0. *****
*****1*****
Xsram[37] sram->in sram[37]->out sram[37]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[37]->out) 0
.nodeset V(sram[37]->outb) vsp
***** Signal mux_1level_tapbuf_size2[29]->in[0] density = 0.1906, probability=0.4782.*****
Vmux_1level_tapbuf_size2[29]->in[0] mux_1level_tapbuf_size2[29]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4782*10.4932*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '10.4932*clock_period')
***** Signal mux_1level_tapbuf_size2[29]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[29]->in[1] mux_1level_tapbuf_size2[29]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[29] gvdd_mux_1level_tapbuf_size2[29] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[196] trig v(mux_1level_tapbuf_size2[29]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[29]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[196] trig v(mux_1level_tapbuf_size2[29]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[29]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[196] when v(mux_1level_tapbuf_size2[29]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[196] trig v(mux_1level_tapbuf_size2[29]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[29]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[196] when v(mux_1level_tapbuf_size2[29]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[196] trig v(mux_1level_tapbuf_size2[29]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[29]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[29]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[29]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[196] param='mux_1level_tapbuf_size2[29]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[29]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[29]) from='clock_period' to='7*clock_period'
.meas tran mux_1level_tapbuf_size2[29]_energy_per_cycle param='mux_1level_tapbuf_size2[29]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[196]  param='mux_1level_tapbuf_size2[29]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[196]  param='dynamic_power_sb_mux[1][1]_rrnode[196]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[196] avg p(Vgvdd_mux_1level_tapbuf_size2[29]) from='start_rise_sb_mux[1][1]_rrnode[196]' to='start_rise_sb_mux[1][1]_rrnode[196]+switch_rise_sb_mux[1][1]_rrnode[196]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[196] avg p(Vgvdd_mux_1level_tapbuf_size2[29]) from='start_fall_sb_mux[1][1]_rrnode[196]' to='start_fall_sb_mux[1][1]_rrnode[196]+switch_fall_sb_mux[1][1]_rrnode[196]'
.meas tran sum_leakage_power_mux[0to29] 
+          param='sum_leakage_power_mux[0to28]+leakage_sb_mux[1][1]_rrnode[196]'
.meas tran sum_energy_per_cycle_mux[0to29] 
+          param='sum_energy_per_cycle_mux[0to28]+energy_per_cycle_sb_mux[1][1]_rrnode[196]'
***** Load for rr_node[196] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=29, type=4 *****
Xchan_mux_1level_tapbuf_size2[29]->out_loadlvl[0]_out mux_1level_tapbuf_size2[29]->out mux_1level_tapbuf_size2[29]->out_loadlvl[0]_out mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[48]_no0 mux_1level_tapbuf_size2[29]->out_loadlvl[0]_out mux_1level_tapbuf_size2[29]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[49]_no0 mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to29] 
+          param='sum_leakage_power_sb_mux[0to28]+leakage_sb_mux[1][1]_rrnode[196]'
.meas tran sum_energy_per_cycle_sb_mux[0to29] 
+          param='sum_energy_per_cycle_sb_mux[0to28]+energy_per_cycle_sb_mux[1][1]_rrnode[196]'
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
.meas tran total_leakage_power_mux[0to29] 
+          param='sum_leakage_power_mux[0to29]'
.meas tran total_energy_per_cycle_mux[0to29] 
+          param='sum_energy_per_cycle_mux[0to29]'
.meas tran total_leakage_power_sb_mux 
+          param='sum_leakage_power_sb_mux[0to29]'
.meas tran total_energy_per_cycle_sb_mux 
+          param='sum_energy_per_cycle_sb_mux[0to29]'
.end
