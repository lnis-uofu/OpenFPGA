*****************************
*     FPGA SPICE Netlist    *
* Description: FPGA SPICE Routing MUX Test Bench for Design: example_2 *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:08 2018
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
Xmux_1level_tapbuf_size2[0] mux_1level_tapbuf_size2[0]->in[0] mux_1level_tapbuf_size2[0]->in[1] mux_1level_tapbuf_size2[0]->out sram[0]->outb sram[0]->out gvdd_mux_1level_tapbuf_size2[0] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[0], level=1, select_path_id=0. *****
*****1*****
Xsram[0] sram->in sram[0]->out sram[0]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[0]->out) 0
.nodeset V(sram[0]->outb) vsp
***** Signal mux_1level_tapbuf_size2[0]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[0]->in[0] mux_1level_tapbuf_size2[0]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[0]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[0]->in[1] mux_1level_tapbuf_size2[0]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[0] gvdd_mux_1level_tapbuf_size2[0] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[391] trig v(mux_1level_tapbuf_size2[0]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[0]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[391] trig v(mux_1level_tapbuf_size2[0]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[0]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[391] when v(mux_1level_tapbuf_size2[0]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[391] trig v(mux_1level_tapbuf_size2[0]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[0]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[391] when v(mux_1level_tapbuf_size2[0]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[391] trig v(mux_1level_tapbuf_size2[0]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[0]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[0]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[0]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[391] param='mux_1level_tapbuf_size2[0]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[0]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[0]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[0]_energy_per_cycle param='mux_1level_tapbuf_size2[0]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[391]  param='mux_1level_tapbuf_size2[0]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[391]  param='dynamic_power_sb_mux[0][0]_rrnode[391]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[391] avg p(Vgvdd_mux_1level_tapbuf_size2[0]) from='start_rise_sb_mux[0][0]_rrnode[391]' to='start_rise_sb_mux[0][0]_rrnode[391]+switch_rise_sb_mux[0][0]_rrnode[391]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[391] avg p(Vgvdd_mux_1level_tapbuf_size2[0]) from='start_fall_sb_mux[0][0]_rrnode[391]' to='start_fall_sb_mux[0][0]_rrnode[391]+switch_fall_sb_mux[0][0]_rrnode[391]'
.meas tran sum_leakage_power_mux[0to0] 
+          param='leakage_sb_mux[0][0]_rrnode[391]'
.meas tran sum_energy_per_cycle_mux[0to0] 
+          param='energy_per_cycle_sb_mux[0][0]_rrnode[391]'
***** Load for rr_node[391] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=0, type=5 *****
Xchan_mux_1level_tapbuf_size2[0]->out_loadlvl[0]_out mux_1level_tapbuf_size2[0]->out mux_1level_tapbuf_size2[0]->out_loadlvl[0]_out mux_1level_tapbuf_size2[0]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[0]_no0 mux_1level_tapbuf_size2[0]->out_loadlvl[0]_out mux_1level_tapbuf_size2[0]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[1]_no0 mux_1level_tapbuf_size2[0]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[0]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[2]_no0 mux_1level_tapbuf_size2[0]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[0]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[3]_no0 mux_1level_tapbuf_size2[0]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[0]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to0] 
+          param='leakage_sb_mux[0][0]_rrnode[391]'
.meas tran sum_energy_per_cycle_sb_mux[0to0] 
+          param='energy_per_cycle_sb_mux[0][0]_rrnode[391]'
Xmux_1level_tapbuf_size2[1] mux_1level_tapbuf_size2[1]->in[0] mux_1level_tapbuf_size2[1]->in[1] mux_1level_tapbuf_size2[1]->out sram[1]->outb sram[1]->out gvdd_mux_1level_tapbuf_size2[1] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[1], level=1, select_path_id=0. *****
*****1*****
Xsram[1] sram->in sram[1]->out sram[1]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[1]->out) 0
.nodeset V(sram[1]->outb) vsp
***** Signal mux_1level_tapbuf_size2[1]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[1]->in[0] mux_1level_tapbuf_size2[1]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[1]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[1]->in[1] mux_1level_tapbuf_size2[1]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[1] gvdd_mux_1level_tapbuf_size2[1] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[393] trig v(mux_1level_tapbuf_size2[1]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[1]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[393] trig v(mux_1level_tapbuf_size2[1]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[1]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[393] when v(mux_1level_tapbuf_size2[1]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[393] trig v(mux_1level_tapbuf_size2[1]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[1]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[393] when v(mux_1level_tapbuf_size2[1]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[393] trig v(mux_1level_tapbuf_size2[1]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[1]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[1]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[1]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[393] param='mux_1level_tapbuf_size2[1]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[1]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[1]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[1]_energy_per_cycle param='mux_1level_tapbuf_size2[1]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[393]  param='mux_1level_tapbuf_size2[1]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[393]  param='dynamic_power_sb_mux[0][0]_rrnode[393]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[393] avg p(Vgvdd_mux_1level_tapbuf_size2[1]) from='start_rise_sb_mux[0][0]_rrnode[393]' to='start_rise_sb_mux[0][0]_rrnode[393]+switch_rise_sb_mux[0][0]_rrnode[393]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[393] avg p(Vgvdd_mux_1level_tapbuf_size2[1]) from='start_fall_sb_mux[0][0]_rrnode[393]' to='start_fall_sb_mux[0][0]_rrnode[393]+switch_fall_sb_mux[0][0]_rrnode[393]'
.meas tran sum_leakage_power_mux[0to1] 
+          param='sum_leakage_power_mux[0to0]+leakage_sb_mux[0][0]_rrnode[393]'
.meas tran sum_energy_per_cycle_mux[0to1] 
+          param='sum_energy_per_cycle_mux[0to0]+energy_per_cycle_sb_mux[0][0]_rrnode[393]'
***** Load for rr_node[393] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=2, type=5 *****
Xchan_mux_1level_tapbuf_size2[1]->out_loadlvl[0]_out mux_1level_tapbuf_size2[1]->out mux_1level_tapbuf_size2[1]->out_loadlvl[0]_out mux_1level_tapbuf_size2[1]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[4]_no0 mux_1level_tapbuf_size2[1]->out_loadlvl[0]_out mux_1level_tapbuf_size2[1]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[5]_no0 mux_1level_tapbuf_size2[1]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[1]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[6]_no0 mux_1level_tapbuf_size2[1]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[1]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[7]_no0 mux_1level_tapbuf_size2[1]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[1]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to1] 
+          param='sum_leakage_power_sb_mux[0to0]+leakage_sb_mux[0][0]_rrnode[393]'
.meas tran sum_energy_per_cycle_sb_mux[0to1] 
+          param='sum_energy_per_cycle_sb_mux[0to0]+energy_per_cycle_sb_mux[0][0]_rrnode[393]'
Xmux_1level_tapbuf_size2[2] mux_1level_tapbuf_size2[2]->in[0] mux_1level_tapbuf_size2[2]->in[1] mux_1level_tapbuf_size2[2]->out sram[2]->outb sram[2]->out gvdd_mux_1level_tapbuf_size2[2] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[2], level=1, select_path_id=0. *****
*****1*****
Xsram[2] sram->in sram[2]->out sram[2]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[2]->out) 0
.nodeset V(sram[2]->outb) vsp
***** Signal mux_1level_tapbuf_size2[2]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[2]->in[0] mux_1level_tapbuf_size2[2]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[2]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[2]->in[1] mux_1level_tapbuf_size2[2]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[2] gvdd_mux_1level_tapbuf_size2[2] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[395] trig v(mux_1level_tapbuf_size2[2]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[2]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[395] trig v(mux_1level_tapbuf_size2[2]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[2]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[395] when v(mux_1level_tapbuf_size2[2]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[395] trig v(mux_1level_tapbuf_size2[2]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[2]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[395] when v(mux_1level_tapbuf_size2[2]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[395] trig v(mux_1level_tapbuf_size2[2]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[2]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[2]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[2]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[395] param='mux_1level_tapbuf_size2[2]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[2]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[2]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[2]_energy_per_cycle param='mux_1level_tapbuf_size2[2]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[395]  param='mux_1level_tapbuf_size2[2]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[395]  param='dynamic_power_sb_mux[0][0]_rrnode[395]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[395] avg p(Vgvdd_mux_1level_tapbuf_size2[2]) from='start_rise_sb_mux[0][0]_rrnode[395]' to='start_rise_sb_mux[0][0]_rrnode[395]+switch_rise_sb_mux[0][0]_rrnode[395]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[395] avg p(Vgvdd_mux_1level_tapbuf_size2[2]) from='start_fall_sb_mux[0][0]_rrnode[395]' to='start_fall_sb_mux[0][0]_rrnode[395]+switch_fall_sb_mux[0][0]_rrnode[395]'
.meas tran sum_leakage_power_mux[0to2] 
+          param='sum_leakage_power_mux[0to1]+leakage_sb_mux[0][0]_rrnode[395]'
.meas tran sum_energy_per_cycle_mux[0to2] 
+          param='sum_energy_per_cycle_mux[0to1]+energy_per_cycle_sb_mux[0][0]_rrnode[395]'
***** Load for rr_node[395] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=4, type=5 *****
Xchan_mux_1level_tapbuf_size2[2]->out_loadlvl[0]_out mux_1level_tapbuf_size2[2]->out mux_1level_tapbuf_size2[2]->out_loadlvl[0]_out mux_1level_tapbuf_size2[2]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[8]_no0 mux_1level_tapbuf_size2[2]->out_loadlvl[0]_out mux_1level_tapbuf_size2[2]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[9]_no0 mux_1level_tapbuf_size2[2]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[2]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[10]_no0 mux_1level_tapbuf_size2[2]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[2]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[11]_no0 mux_1level_tapbuf_size2[2]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[2]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to2] 
+          param='sum_leakage_power_sb_mux[0to1]+leakage_sb_mux[0][0]_rrnode[395]'
.meas tran sum_energy_per_cycle_sb_mux[0to2] 
+          param='sum_energy_per_cycle_sb_mux[0to1]+energy_per_cycle_sb_mux[0][0]_rrnode[395]'
Xmux_1level_tapbuf_size2[3] mux_1level_tapbuf_size2[3]->in[0] mux_1level_tapbuf_size2[3]->in[1] mux_1level_tapbuf_size2[3]->out sram[3]->outb sram[3]->out gvdd_mux_1level_tapbuf_size2[3] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[3], level=1, select_path_id=0. *****
*****1*****
Xsram[3] sram->in sram[3]->out sram[3]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[3]->out) 0
.nodeset V(sram[3]->outb) vsp
***** Signal mux_1level_tapbuf_size2[3]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[3]->in[0] mux_1level_tapbuf_size2[3]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[3]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[3]->in[1] mux_1level_tapbuf_size2[3]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[3] gvdd_mux_1level_tapbuf_size2[3] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[397] trig v(mux_1level_tapbuf_size2[3]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[3]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[397] trig v(mux_1level_tapbuf_size2[3]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[3]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[397] when v(mux_1level_tapbuf_size2[3]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[397] trig v(mux_1level_tapbuf_size2[3]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[3]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[397] when v(mux_1level_tapbuf_size2[3]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[397] trig v(mux_1level_tapbuf_size2[3]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[3]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[3]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[3]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[397] param='mux_1level_tapbuf_size2[3]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[3]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[3]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[3]_energy_per_cycle param='mux_1level_tapbuf_size2[3]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[397]  param='mux_1level_tapbuf_size2[3]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[397]  param='dynamic_power_sb_mux[0][0]_rrnode[397]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[397] avg p(Vgvdd_mux_1level_tapbuf_size2[3]) from='start_rise_sb_mux[0][0]_rrnode[397]' to='start_rise_sb_mux[0][0]_rrnode[397]+switch_rise_sb_mux[0][0]_rrnode[397]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[397] avg p(Vgvdd_mux_1level_tapbuf_size2[3]) from='start_fall_sb_mux[0][0]_rrnode[397]' to='start_fall_sb_mux[0][0]_rrnode[397]+switch_fall_sb_mux[0][0]_rrnode[397]'
.meas tran sum_leakage_power_mux[0to3] 
+          param='sum_leakage_power_mux[0to2]+leakage_sb_mux[0][0]_rrnode[397]'
.meas tran sum_energy_per_cycle_mux[0to3] 
+          param='sum_energy_per_cycle_mux[0to2]+energy_per_cycle_sb_mux[0][0]_rrnode[397]'
***** Load for rr_node[397] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=6, type=5 *****
Xchan_mux_1level_tapbuf_size2[3]->out_loadlvl[0]_out mux_1level_tapbuf_size2[3]->out mux_1level_tapbuf_size2[3]->out_loadlvl[0]_out mux_1level_tapbuf_size2[3]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[12]_no0 mux_1level_tapbuf_size2[3]->out_loadlvl[0]_out mux_1level_tapbuf_size2[3]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[13]_no0 mux_1level_tapbuf_size2[3]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[3]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[14]_no0 mux_1level_tapbuf_size2[3]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[3]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to3] 
+          param='sum_leakage_power_sb_mux[0to2]+leakage_sb_mux[0][0]_rrnode[397]'
.meas tran sum_energy_per_cycle_sb_mux[0to3] 
+          param='sum_energy_per_cycle_sb_mux[0to2]+energy_per_cycle_sb_mux[0][0]_rrnode[397]'
Xmux_1level_tapbuf_size2[4] mux_1level_tapbuf_size2[4]->in[0] mux_1level_tapbuf_size2[4]->in[1] mux_1level_tapbuf_size2[4]->out sram[4]->outb sram[4]->out gvdd_mux_1level_tapbuf_size2[4] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[4], level=1, select_path_id=0. *****
*****1*****
Xsram[4] sram->in sram[4]->out sram[4]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[4]->out) 0
.nodeset V(sram[4]->outb) vsp
***** Signal mux_1level_tapbuf_size2[4]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[4]->in[0] mux_1level_tapbuf_size2[4]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[4]->in[1] density = 0.2026, probability=0.5018.*****
Vmux_1level_tapbuf_size2[4]->in[1] mux_1level_tapbuf_size2[4]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgvdd_mux_1level_tapbuf_size2[4] gvdd_mux_1level_tapbuf_size2[4] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[399] trig v(mux_1level_tapbuf_size2[4]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[4]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[399] trig v(mux_1level_tapbuf_size2[4]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[4]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[399] when v(mux_1level_tapbuf_size2[4]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[399] trig v(mux_1level_tapbuf_size2[4]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[4]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[399] when v(mux_1level_tapbuf_size2[4]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[399] trig v(mux_1level_tapbuf_size2[4]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[4]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[4]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[4]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[399] param='mux_1level_tapbuf_size2[4]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[4]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[4]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[4]_energy_per_cycle param='mux_1level_tapbuf_size2[4]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[399]  param='mux_1level_tapbuf_size2[4]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[399]  param='dynamic_power_sb_mux[0][0]_rrnode[399]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[399] avg p(Vgvdd_mux_1level_tapbuf_size2[4]) from='start_rise_sb_mux[0][0]_rrnode[399]' to='start_rise_sb_mux[0][0]_rrnode[399]+switch_rise_sb_mux[0][0]_rrnode[399]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[399] avg p(Vgvdd_mux_1level_tapbuf_size2[4]) from='start_fall_sb_mux[0][0]_rrnode[399]' to='start_fall_sb_mux[0][0]_rrnode[399]+switch_fall_sb_mux[0][0]_rrnode[399]'
.meas tran sum_leakage_power_mux[0to4] 
+          param='sum_leakage_power_mux[0to3]+leakage_sb_mux[0][0]_rrnode[399]'
.meas tran sum_energy_per_cycle_mux[0to4] 
+          param='sum_energy_per_cycle_mux[0to3]+energy_per_cycle_sb_mux[0][0]_rrnode[399]'
***** Load for rr_node[399] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=8, type=5 *****
Xchan_mux_1level_tapbuf_size2[4]->out_loadlvl[0]_out mux_1level_tapbuf_size2[4]->out mux_1level_tapbuf_size2[4]->out_loadlvl[0]_out mux_1level_tapbuf_size2[4]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[15]_no0 mux_1level_tapbuf_size2[4]->out_loadlvl[0]_out mux_1level_tapbuf_size2[4]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[16]_no0 mux_1level_tapbuf_size2[4]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[4]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[17]_no0 mux_1level_tapbuf_size2[4]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[4]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[18]_no0 mux_1level_tapbuf_size2[4]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[4]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[19]_no0 mux_1level_tapbuf_size2[4]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[4]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to4] 
+          param='sum_leakage_power_sb_mux[0to3]+leakage_sb_mux[0][0]_rrnode[399]'
.meas tran sum_energy_per_cycle_sb_mux[0to4] 
+          param='sum_energy_per_cycle_sb_mux[0to3]+energy_per_cycle_sb_mux[0][0]_rrnode[399]'
Xmux_1level_tapbuf_size2[5] mux_1level_tapbuf_size2[5]->in[0] mux_1level_tapbuf_size2[5]->in[1] mux_1level_tapbuf_size2[5]->out sram[5]->outb sram[5]->out gvdd_mux_1level_tapbuf_size2[5] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[5], level=1, select_path_id=0. *****
*****1*****
Xsram[5] sram->in sram[5]->out sram[5]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[5]->out) 0
.nodeset V(sram[5]->outb) vsp
***** Signal mux_1level_tapbuf_size2[5]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[5]->in[0] mux_1level_tapbuf_size2[5]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[5]->in[1] density = 0.2026, probability=0.5018.*****
Vmux_1level_tapbuf_size2[5]->in[1] mux_1level_tapbuf_size2[5]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgvdd_mux_1level_tapbuf_size2[5] gvdd_mux_1level_tapbuf_size2[5] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[401] trig v(mux_1level_tapbuf_size2[5]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[5]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[401] trig v(mux_1level_tapbuf_size2[5]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[5]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[401] when v(mux_1level_tapbuf_size2[5]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[401] trig v(mux_1level_tapbuf_size2[5]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[5]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[401] when v(mux_1level_tapbuf_size2[5]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[401] trig v(mux_1level_tapbuf_size2[5]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[5]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[5]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[5]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[401] param='mux_1level_tapbuf_size2[5]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[5]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[5]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[5]_energy_per_cycle param='mux_1level_tapbuf_size2[5]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[401]  param='mux_1level_tapbuf_size2[5]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[401]  param='dynamic_power_sb_mux[0][0]_rrnode[401]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[401] avg p(Vgvdd_mux_1level_tapbuf_size2[5]) from='start_rise_sb_mux[0][0]_rrnode[401]' to='start_rise_sb_mux[0][0]_rrnode[401]+switch_rise_sb_mux[0][0]_rrnode[401]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[401] avg p(Vgvdd_mux_1level_tapbuf_size2[5]) from='start_fall_sb_mux[0][0]_rrnode[401]' to='start_fall_sb_mux[0][0]_rrnode[401]+switch_fall_sb_mux[0][0]_rrnode[401]'
.meas tran sum_leakage_power_mux[0to5] 
+          param='sum_leakage_power_mux[0to4]+leakage_sb_mux[0][0]_rrnode[401]'
.meas tran sum_energy_per_cycle_mux[0to5] 
+          param='sum_energy_per_cycle_mux[0to4]+energy_per_cycle_sb_mux[0][0]_rrnode[401]'
***** Load for rr_node[401] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=10, type=5 *****
Xchan_mux_1level_tapbuf_size2[5]->out_loadlvl[0]_out mux_1level_tapbuf_size2[5]->out mux_1level_tapbuf_size2[5]->out_loadlvl[0]_out mux_1level_tapbuf_size2[5]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[20]_no0 mux_1level_tapbuf_size2[5]->out_loadlvl[0]_out mux_1level_tapbuf_size2[5]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[21]_no0 mux_1level_tapbuf_size2[5]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[5]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[22]_no0 mux_1level_tapbuf_size2[5]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[5]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to5] 
+          param='sum_leakage_power_sb_mux[0to4]+leakage_sb_mux[0][0]_rrnode[401]'
.meas tran sum_energy_per_cycle_sb_mux[0to5] 
+          param='sum_energy_per_cycle_sb_mux[0to4]+energy_per_cycle_sb_mux[0][0]_rrnode[401]'
Xmux_1level_tapbuf_size2[6] mux_1level_tapbuf_size2[6]->in[0] mux_1level_tapbuf_size2[6]->in[1] mux_1level_tapbuf_size2[6]->out sram[6]->outb sram[6]->out gvdd_mux_1level_tapbuf_size2[6] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[6], level=1, select_path_id=0. *****
*****1*****
Xsram[6] sram->in sram[6]->out sram[6]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[6]->out) 0
.nodeset V(sram[6]->outb) vsp
***** Signal mux_1level_tapbuf_size2[6]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[6]->in[0] mux_1level_tapbuf_size2[6]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[6]->in[1] density = 0.2026, probability=0.5018.*****
Vmux_1level_tapbuf_size2[6]->in[1] mux_1level_tapbuf_size2[6]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgvdd_mux_1level_tapbuf_size2[6] gvdd_mux_1level_tapbuf_size2[6] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[403] trig v(mux_1level_tapbuf_size2[6]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[6]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[403] trig v(mux_1level_tapbuf_size2[6]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[6]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[403] when v(mux_1level_tapbuf_size2[6]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[403] trig v(mux_1level_tapbuf_size2[6]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[6]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[403] when v(mux_1level_tapbuf_size2[6]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[403] trig v(mux_1level_tapbuf_size2[6]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[6]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[6]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[6]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[403] param='mux_1level_tapbuf_size2[6]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[6]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[6]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[6]_energy_per_cycle param='mux_1level_tapbuf_size2[6]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[403]  param='mux_1level_tapbuf_size2[6]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[403]  param='dynamic_power_sb_mux[0][0]_rrnode[403]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[403] avg p(Vgvdd_mux_1level_tapbuf_size2[6]) from='start_rise_sb_mux[0][0]_rrnode[403]' to='start_rise_sb_mux[0][0]_rrnode[403]+switch_rise_sb_mux[0][0]_rrnode[403]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[403] avg p(Vgvdd_mux_1level_tapbuf_size2[6]) from='start_fall_sb_mux[0][0]_rrnode[403]' to='start_fall_sb_mux[0][0]_rrnode[403]+switch_fall_sb_mux[0][0]_rrnode[403]'
.meas tran sum_leakage_power_mux[0to6] 
+          param='sum_leakage_power_mux[0to5]+leakage_sb_mux[0][0]_rrnode[403]'
.meas tran sum_energy_per_cycle_mux[0to6] 
+          param='sum_energy_per_cycle_mux[0to5]+energy_per_cycle_sb_mux[0][0]_rrnode[403]'
***** Load for rr_node[403] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=12, type=5 *****
Xchan_mux_1level_tapbuf_size2[6]->out_loadlvl[0]_out mux_1level_tapbuf_size2[6]->out mux_1level_tapbuf_size2[6]->out_loadlvl[0]_out mux_1level_tapbuf_size2[6]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[23]_no0 mux_1level_tapbuf_size2[6]->out_loadlvl[0]_out mux_1level_tapbuf_size2[6]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[24]_no0 mux_1level_tapbuf_size2[6]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[6]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[25]_no0 mux_1level_tapbuf_size2[6]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[6]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[26]_no0 mux_1level_tapbuf_size2[6]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[6]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to6] 
+          param='sum_leakage_power_sb_mux[0to5]+leakage_sb_mux[0][0]_rrnode[403]'
.meas tran sum_energy_per_cycle_sb_mux[0to6] 
+          param='sum_energy_per_cycle_sb_mux[0to5]+energy_per_cycle_sb_mux[0][0]_rrnode[403]'
Xmux_1level_tapbuf_size2[7] mux_1level_tapbuf_size2[7]->in[0] mux_1level_tapbuf_size2[7]->in[1] mux_1level_tapbuf_size2[7]->out sram[7]->outb sram[7]->out gvdd_mux_1level_tapbuf_size2[7] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[7], level=1, select_path_id=0. *****
*****1*****
Xsram[7] sram->in sram[7]->out sram[7]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[7]->out) 0
.nodeset V(sram[7]->outb) vsp
***** Signal mux_1level_tapbuf_size2[7]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[7]->in[0] mux_1level_tapbuf_size2[7]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[7]->in[1] density = 0.2026, probability=0.5018.*****
Vmux_1level_tapbuf_size2[7]->in[1] mux_1level_tapbuf_size2[7]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgvdd_mux_1level_tapbuf_size2[7] gvdd_mux_1level_tapbuf_size2[7] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[405] trig v(mux_1level_tapbuf_size2[7]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[7]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[405] trig v(mux_1level_tapbuf_size2[7]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[7]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[405] when v(mux_1level_tapbuf_size2[7]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[405] trig v(mux_1level_tapbuf_size2[7]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[7]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[405] when v(mux_1level_tapbuf_size2[7]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[405] trig v(mux_1level_tapbuf_size2[7]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[7]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[7]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[7]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[405] param='mux_1level_tapbuf_size2[7]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[7]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[7]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[7]_energy_per_cycle param='mux_1level_tapbuf_size2[7]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[405]  param='mux_1level_tapbuf_size2[7]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[405]  param='dynamic_power_sb_mux[0][0]_rrnode[405]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[405] avg p(Vgvdd_mux_1level_tapbuf_size2[7]) from='start_rise_sb_mux[0][0]_rrnode[405]' to='start_rise_sb_mux[0][0]_rrnode[405]+switch_rise_sb_mux[0][0]_rrnode[405]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[405] avg p(Vgvdd_mux_1level_tapbuf_size2[7]) from='start_fall_sb_mux[0][0]_rrnode[405]' to='start_fall_sb_mux[0][0]_rrnode[405]+switch_fall_sb_mux[0][0]_rrnode[405]'
.meas tran sum_leakage_power_mux[0to7] 
+          param='sum_leakage_power_mux[0to6]+leakage_sb_mux[0][0]_rrnode[405]'
.meas tran sum_energy_per_cycle_mux[0to7] 
+          param='sum_energy_per_cycle_mux[0to6]+energy_per_cycle_sb_mux[0][0]_rrnode[405]'
***** Load for rr_node[405] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=14, type=5 *****
Xchan_mux_1level_tapbuf_size2[7]->out_loadlvl[0]_out mux_1level_tapbuf_size2[7]->out mux_1level_tapbuf_size2[7]->out_loadlvl[0]_out mux_1level_tapbuf_size2[7]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[27]_no0 mux_1level_tapbuf_size2[7]->out_loadlvl[0]_out mux_1level_tapbuf_size2[7]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[28]_no0 mux_1level_tapbuf_size2[7]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[7]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[29]_no0 mux_1level_tapbuf_size2[7]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[7]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[30]_no0 mux_1level_tapbuf_size2[7]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[7]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to7] 
+          param='sum_leakage_power_sb_mux[0to6]+leakage_sb_mux[0][0]_rrnode[405]'
.meas tran sum_energy_per_cycle_sb_mux[0to7] 
+          param='sum_energy_per_cycle_sb_mux[0to6]+energy_per_cycle_sb_mux[0][0]_rrnode[405]'
Xmux_1level_tapbuf_size2[8] mux_1level_tapbuf_size2[8]->in[0] mux_1level_tapbuf_size2[8]->in[1] mux_1level_tapbuf_size2[8]->out sram[8]->outb sram[8]->out gvdd_mux_1level_tapbuf_size2[8] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[8], level=1, select_path_id=0. *****
*****1*****
Xsram[8] sram->in sram[8]->out sram[8]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[8]->out) 0
.nodeset V(sram[8]->outb) vsp
***** Signal mux_1level_tapbuf_size2[8]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[8]->in[0] mux_1level_tapbuf_size2[8]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[8]->in[1] density = 0.2026, probability=0.5018.*****
Vmux_1level_tapbuf_size2[8]->in[1] mux_1level_tapbuf_size2[8]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgvdd_mux_1level_tapbuf_size2[8] gvdd_mux_1level_tapbuf_size2[8] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[407] trig v(mux_1level_tapbuf_size2[8]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[8]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[407] trig v(mux_1level_tapbuf_size2[8]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[8]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[407] when v(mux_1level_tapbuf_size2[8]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[407] trig v(mux_1level_tapbuf_size2[8]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[8]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[407] when v(mux_1level_tapbuf_size2[8]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[407] trig v(mux_1level_tapbuf_size2[8]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[8]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[8]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[8]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[407] param='mux_1level_tapbuf_size2[8]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[8]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[8]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[8]_energy_per_cycle param='mux_1level_tapbuf_size2[8]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[407]  param='mux_1level_tapbuf_size2[8]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[407]  param='dynamic_power_sb_mux[0][0]_rrnode[407]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[407] avg p(Vgvdd_mux_1level_tapbuf_size2[8]) from='start_rise_sb_mux[0][0]_rrnode[407]' to='start_rise_sb_mux[0][0]_rrnode[407]+switch_rise_sb_mux[0][0]_rrnode[407]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[407] avg p(Vgvdd_mux_1level_tapbuf_size2[8]) from='start_fall_sb_mux[0][0]_rrnode[407]' to='start_fall_sb_mux[0][0]_rrnode[407]+switch_fall_sb_mux[0][0]_rrnode[407]'
.meas tran sum_leakage_power_mux[0to8] 
+          param='sum_leakage_power_mux[0to7]+leakage_sb_mux[0][0]_rrnode[407]'
.meas tran sum_energy_per_cycle_mux[0to8] 
+          param='sum_energy_per_cycle_mux[0to7]+energy_per_cycle_sb_mux[0][0]_rrnode[407]'
***** Load for rr_node[407] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=16, type=5 *****
Xchan_mux_1level_tapbuf_size2[8]->out_loadlvl[0]_out mux_1level_tapbuf_size2[8]->out mux_1level_tapbuf_size2[8]->out_loadlvl[0]_out mux_1level_tapbuf_size2[8]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[31]_no0 mux_1level_tapbuf_size2[8]->out_loadlvl[0]_out mux_1level_tapbuf_size2[8]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[32]_no0 mux_1level_tapbuf_size2[8]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[8]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[33]_no0 mux_1level_tapbuf_size2[8]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[8]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[34]_no0 mux_1level_tapbuf_size2[8]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[8]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to8] 
+          param='sum_leakage_power_sb_mux[0to7]+leakage_sb_mux[0][0]_rrnode[407]'
.meas tran sum_energy_per_cycle_sb_mux[0to8] 
+          param='sum_energy_per_cycle_sb_mux[0to7]+energy_per_cycle_sb_mux[0][0]_rrnode[407]'
Xmux_1level_tapbuf_size2[9] mux_1level_tapbuf_size2[9]->in[0] mux_1level_tapbuf_size2[9]->in[1] mux_1level_tapbuf_size2[9]->out sram[9]->outb sram[9]->out gvdd_mux_1level_tapbuf_size2[9] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[9], level=1, select_path_id=0. *****
*****1*****
Xsram[9] sram->in sram[9]->out sram[9]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[9]->out) 0
.nodeset V(sram[9]->outb) vsp
***** Signal mux_1level_tapbuf_size2[9]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[9]->in[0] mux_1level_tapbuf_size2[9]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[9]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[9]->in[1] mux_1level_tapbuf_size2[9]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[9] gvdd_mux_1level_tapbuf_size2[9] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[409] trig v(mux_1level_tapbuf_size2[9]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[9]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[409] trig v(mux_1level_tapbuf_size2[9]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[9]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[409] when v(mux_1level_tapbuf_size2[9]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[409] trig v(mux_1level_tapbuf_size2[9]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[9]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[409] when v(mux_1level_tapbuf_size2[9]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[409] trig v(mux_1level_tapbuf_size2[9]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[9]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[9]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[9]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[409] param='mux_1level_tapbuf_size2[9]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[9]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[9]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[9]_energy_per_cycle param='mux_1level_tapbuf_size2[9]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[409]  param='mux_1level_tapbuf_size2[9]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[409]  param='dynamic_power_sb_mux[0][0]_rrnode[409]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[409] avg p(Vgvdd_mux_1level_tapbuf_size2[9]) from='start_rise_sb_mux[0][0]_rrnode[409]' to='start_rise_sb_mux[0][0]_rrnode[409]+switch_rise_sb_mux[0][0]_rrnode[409]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[409] avg p(Vgvdd_mux_1level_tapbuf_size2[9]) from='start_fall_sb_mux[0][0]_rrnode[409]' to='start_fall_sb_mux[0][0]_rrnode[409]+switch_fall_sb_mux[0][0]_rrnode[409]'
.meas tran sum_leakage_power_mux[0to9] 
+          param='sum_leakage_power_mux[0to8]+leakage_sb_mux[0][0]_rrnode[409]'
.meas tran sum_energy_per_cycle_mux[0to9] 
+          param='sum_energy_per_cycle_mux[0to8]+energy_per_cycle_sb_mux[0][0]_rrnode[409]'
***** Load for rr_node[409] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=18, type=5 *****
Xchan_mux_1level_tapbuf_size2[9]->out_loadlvl[0]_out mux_1level_tapbuf_size2[9]->out mux_1level_tapbuf_size2[9]->out_loadlvl[0]_out mux_1level_tapbuf_size2[9]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[35]_no0 mux_1level_tapbuf_size2[9]->out_loadlvl[0]_out mux_1level_tapbuf_size2[9]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[36]_no0 mux_1level_tapbuf_size2[9]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[9]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[37]_no0 mux_1level_tapbuf_size2[9]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[9]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[38]_no0 mux_1level_tapbuf_size2[9]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[9]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to9] 
+          param='sum_leakage_power_sb_mux[0to8]+leakage_sb_mux[0][0]_rrnode[409]'
.meas tran sum_energy_per_cycle_sb_mux[0to9] 
+          param='sum_energy_per_cycle_sb_mux[0to8]+energy_per_cycle_sb_mux[0][0]_rrnode[409]'
Xmux_1level_tapbuf_size2[10] mux_1level_tapbuf_size2[10]->in[0] mux_1level_tapbuf_size2[10]->in[1] mux_1level_tapbuf_size2[10]->out sram[10]->outb sram[10]->out gvdd_mux_1level_tapbuf_size2[10] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[10], level=1, select_path_id=0. *****
*****1*****
Xsram[10] sram->in sram[10]->out sram[10]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[10]->out) 0
.nodeset V(sram[10]->outb) vsp
***** Signal mux_1level_tapbuf_size2[10]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[10]->in[0] mux_1level_tapbuf_size2[10]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[10]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[10]->in[1] mux_1level_tapbuf_size2[10]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[10] gvdd_mux_1level_tapbuf_size2[10] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[411] trig v(mux_1level_tapbuf_size2[10]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[10]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[411] trig v(mux_1level_tapbuf_size2[10]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[10]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[411] when v(mux_1level_tapbuf_size2[10]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[411] trig v(mux_1level_tapbuf_size2[10]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[10]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[411] when v(mux_1level_tapbuf_size2[10]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[411] trig v(mux_1level_tapbuf_size2[10]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[10]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[10]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[10]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[411] param='mux_1level_tapbuf_size2[10]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[10]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[10]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[10]_energy_per_cycle param='mux_1level_tapbuf_size2[10]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[411]  param='mux_1level_tapbuf_size2[10]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[411]  param='dynamic_power_sb_mux[0][0]_rrnode[411]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[411] avg p(Vgvdd_mux_1level_tapbuf_size2[10]) from='start_rise_sb_mux[0][0]_rrnode[411]' to='start_rise_sb_mux[0][0]_rrnode[411]+switch_rise_sb_mux[0][0]_rrnode[411]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[411] avg p(Vgvdd_mux_1level_tapbuf_size2[10]) from='start_fall_sb_mux[0][0]_rrnode[411]' to='start_fall_sb_mux[0][0]_rrnode[411]+switch_fall_sb_mux[0][0]_rrnode[411]'
.meas tran sum_leakage_power_mux[0to10] 
+          param='sum_leakage_power_mux[0to9]+leakage_sb_mux[0][0]_rrnode[411]'
.meas tran sum_energy_per_cycle_mux[0to10] 
+          param='sum_energy_per_cycle_mux[0to9]+energy_per_cycle_sb_mux[0][0]_rrnode[411]'
***** Load for rr_node[411] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=20, type=5 *****
Xchan_mux_1level_tapbuf_size2[10]->out_loadlvl[0]_out mux_1level_tapbuf_size2[10]->out mux_1level_tapbuf_size2[10]->out_loadlvl[0]_out mux_1level_tapbuf_size2[10]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[39]_no0 mux_1level_tapbuf_size2[10]->out_loadlvl[0]_out mux_1level_tapbuf_size2[10]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[40]_no0 mux_1level_tapbuf_size2[10]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[10]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[41]_no0 mux_1level_tapbuf_size2[10]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[10]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to10] 
+          param='sum_leakage_power_sb_mux[0to9]+leakage_sb_mux[0][0]_rrnode[411]'
.meas tran sum_energy_per_cycle_sb_mux[0to10] 
+          param='sum_energy_per_cycle_sb_mux[0to9]+energy_per_cycle_sb_mux[0][0]_rrnode[411]'
Xmux_1level_tapbuf_size2[11] mux_1level_tapbuf_size2[11]->in[0] mux_1level_tapbuf_size2[11]->in[1] mux_1level_tapbuf_size2[11]->out sram[11]->outb sram[11]->out gvdd_mux_1level_tapbuf_size2[11] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[11], level=1, select_path_id=0. *****
*****1*****
Xsram[11] sram->in sram[11]->out sram[11]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[11]->out) 0
.nodeset V(sram[11]->outb) vsp
***** Signal mux_1level_tapbuf_size2[11]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[11]->in[0] mux_1level_tapbuf_size2[11]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[11]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[11]->in[1] mux_1level_tapbuf_size2[11]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[11] gvdd_mux_1level_tapbuf_size2[11] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[413] trig v(mux_1level_tapbuf_size2[11]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[11]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[413] trig v(mux_1level_tapbuf_size2[11]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[11]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[413] when v(mux_1level_tapbuf_size2[11]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[413] trig v(mux_1level_tapbuf_size2[11]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[11]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[413] when v(mux_1level_tapbuf_size2[11]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[413] trig v(mux_1level_tapbuf_size2[11]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[11]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[11]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[11]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[413] param='mux_1level_tapbuf_size2[11]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[11]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[11]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[11]_energy_per_cycle param='mux_1level_tapbuf_size2[11]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[413]  param='mux_1level_tapbuf_size2[11]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[413]  param='dynamic_power_sb_mux[0][0]_rrnode[413]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[413] avg p(Vgvdd_mux_1level_tapbuf_size2[11]) from='start_rise_sb_mux[0][0]_rrnode[413]' to='start_rise_sb_mux[0][0]_rrnode[413]+switch_rise_sb_mux[0][0]_rrnode[413]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[413] avg p(Vgvdd_mux_1level_tapbuf_size2[11]) from='start_fall_sb_mux[0][0]_rrnode[413]' to='start_fall_sb_mux[0][0]_rrnode[413]+switch_fall_sb_mux[0][0]_rrnode[413]'
.meas tran sum_leakage_power_mux[0to11] 
+          param='sum_leakage_power_mux[0to10]+leakage_sb_mux[0][0]_rrnode[413]'
.meas tran sum_energy_per_cycle_mux[0to11] 
+          param='sum_energy_per_cycle_mux[0to10]+energy_per_cycle_sb_mux[0][0]_rrnode[413]'
***** Load for rr_node[413] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=22, type=5 *****
Xchan_mux_1level_tapbuf_size2[11]->out_loadlvl[0]_out mux_1level_tapbuf_size2[11]->out mux_1level_tapbuf_size2[11]->out_loadlvl[0]_out mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[42]_no0 mux_1level_tapbuf_size2[11]->out_loadlvl[0]_out mux_1level_tapbuf_size2[11]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[43]_no0 mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[44]_no0 mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[45]_no0 mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[46]_no0 mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to11] 
+          param='sum_leakage_power_sb_mux[0to10]+leakage_sb_mux[0][0]_rrnode[413]'
.meas tran sum_energy_per_cycle_sb_mux[0to11] 
+          param='sum_energy_per_cycle_sb_mux[0to10]+energy_per_cycle_sb_mux[0][0]_rrnode[413]'
Xmux_1level_tapbuf_size2[12] mux_1level_tapbuf_size2[12]->in[0] mux_1level_tapbuf_size2[12]->in[1] mux_1level_tapbuf_size2[12]->out sram[12]->outb sram[12]->out gvdd_mux_1level_tapbuf_size2[12] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[12], level=1, select_path_id=0. *****
*****1*****
Xsram[12] sram->in sram[12]->out sram[12]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[12]->out) 0
.nodeset V(sram[12]->outb) vsp
***** Signal mux_1level_tapbuf_size2[12]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[12]->in[0] mux_1level_tapbuf_size2[12]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[12]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[12]->in[1] mux_1level_tapbuf_size2[12]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[12] gvdd_mux_1level_tapbuf_size2[12] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[415] trig v(mux_1level_tapbuf_size2[12]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[12]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[415] trig v(mux_1level_tapbuf_size2[12]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[12]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[415] when v(mux_1level_tapbuf_size2[12]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[415] trig v(mux_1level_tapbuf_size2[12]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[12]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[415] when v(mux_1level_tapbuf_size2[12]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[415] trig v(mux_1level_tapbuf_size2[12]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[12]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[12]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[12]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[415] param='mux_1level_tapbuf_size2[12]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[12]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[12]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[12]_energy_per_cycle param='mux_1level_tapbuf_size2[12]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[415]  param='mux_1level_tapbuf_size2[12]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[415]  param='dynamic_power_sb_mux[0][0]_rrnode[415]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[415] avg p(Vgvdd_mux_1level_tapbuf_size2[12]) from='start_rise_sb_mux[0][0]_rrnode[415]' to='start_rise_sb_mux[0][0]_rrnode[415]+switch_rise_sb_mux[0][0]_rrnode[415]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[415] avg p(Vgvdd_mux_1level_tapbuf_size2[12]) from='start_fall_sb_mux[0][0]_rrnode[415]' to='start_fall_sb_mux[0][0]_rrnode[415]+switch_fall_sb_mux[0][0]_rrnode[415]'
.meas tran sum_leakage_power_mux[0to12] 
+          param='sum_leakage_power_mux[0to11]+leakage_sb_mux[0][0]_rrnode[415]'
.meas tran sum_energy_per_cycle_mux[0to12] 
+          param='sum_energy_per_cycle_mux[0to11]+energy_per_cycle_sb_mux[0][0]_rrnode[415]'
***** Load for rr_node[415] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=24, type=5 *****
Xchan_mux_1level_tapbuf_size2[12]->out_loadlvl[0]_out mux_1level_tapbuf_size2[12]->out mux_1level_tapbuf_size2[12]->out_loadlvl[0]_out mux_1level_tapbuf_size2[12]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[47]_no0 mux_1level_tapbuf_size2[12]->out_loadlvl[0]_out mux_1level_tapbuf_size2[12]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[48]_no0 mux_1level_tapbuf_size2[12]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[12]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[49]_no0 mux_1level_tapbuf_size2[12]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[12]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[50]_no0 mux_1level_tapbuf_size2[12]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[12]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to12] 
+          param='sum_leakage_power_sb_mux[0to11]+leakage_sb_mux[0][0]_rrnode[415]'
.meas tran sum_energy_per_cycle_sb_mux[0to12] 
+          param='sum_energy_per_cycle_sb_mux[0to11]+energy_per_cycle_sb_mux[0][0]_rrnode[415]'
Xmux_1level_tapbuf_size2[13] mux_1level_tapbuf_size2[13]->in[0] mux_1level_tapbuf_size2[13]->in[1] mux_1level_tapbuf_size2[13]->out sram[13]->outb sram[13]->out gvdd_mux_1level_tapbuf_size2[13] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[13], level=1, select_path_id=0. *****
*****1*****
Xsram[13] sram->in sram[13]->out sram[13]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[13]->out) 0
.nodeset V(sram[13]->outb) vsp
***** Signal mux_1level_tapbuf_size2[13]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[13]->in[0] mux_1level_tapbuf_size2[13]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[13]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[13]->in[1] mux_1level_tapbuf_size2[13]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[13] gvdd_mux_1level_tapbuf_size2[13] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[417] trig v(mux_1level_tapbuf_size2[13]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[13]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[417] trig v(mux_1level_tapbuf_size2[13]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[13]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[417] when v(mux_1level_tapbuf_size2[13]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[417] trig v(mux_1level_tapbuf_size2[13]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[13]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[417] when v(mux_1level_tapbuf_size2[13]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[417] trig v(mux_1level_tapbuf_size2[13]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[13]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[13]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[13]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[417] param='mux_1level_tapbuf_size2[13]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[13]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[13]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[13]_energy_per_cycle param='mux_1level_tapbuf_size2[13]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[417]  param='mux_1level_tapbuf_size2[13]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[417]  param='dynamic_power_sb_mux[0][0]_rrnode[417]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[417] avg p(Vgvdd_mux_1level_tapbuf_size2[13]) from='start_rise_sb_mux[0][0]_rrnode[417]' to='start_rise_sb_mux[0][0]_rrnode[417]+switch_rise_sb_mux[0][0]_rrnode[417]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[417] avg p(Vgvdd_mux_1level_tapbuf_size2[13]) from='start_fall_sb_mux[0][0]_rrnode[417]' to='start_fall_sb_mux[0][0]_rrnode[417]+switch_fall_sb_mux[0][0]_rrnode[417]'
.meas tran sum_leakage_power_mux[0to13] 
+          param='sum_leakage_power_mux[0to12]+leakage_sb_mux[0][0]_rrnode[417]'
.meas tran sum_energy_per_cycle_mux[0to13] 
+          param='sum_energy_per_cycle_mux[0to12]+energy_per_cycle_sb_mux[0][0]_rrnode[417]'
***** Load for rr_node[417] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=26, type=5 *****
Xchan_mux_1level_tapbuf_size2[13]->out_loadlvl[0]_out mux_1level_tapbuf_size2[13]->out mux_1level_tapbuf_size2[13]->out_loadlvl[0]_out mux_1level_tapbuf_size2[13]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[51]_no0 mux_1level_tapbuf_size2[13]->out_loadlvl[0]_out mux_1level_tapbuf_size2[13]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[52]_no0 mux_1level_tapbuf_size2[13]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[13]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[53]_no0 mux_1level_tapbuf_size2[13]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[13]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to13] 
+          param='sum_leakage_power_sb_mux[0to12]+leakage_sb_mux[0][0]_rrnode[417]'
.meas tran sum_energy_per_cycle_sb_mux[0to13] 
+          param='sum_energy_per_cycle_sb_mux[0to12]+energy_per_cycle_sb_mux[0][0]_rrnode[417]'
Xmux_1level_tapbuf_size2[14] mux_1level_tapbuf_size2[14]->in[0] mux_1level_tapbuf_size2[14]->in[1] mux_1level_tapbuf_size2[14]->out sram[14]->outb sram[14]->out gvdd_mux_1level_tapbuf_size2[14] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[14], level=1, select_path_id=0. *****
*****1*****
Xsram[14] sram->in sram[14]->out sram[14]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[14]->out) 0
.nodeset V(sram[14]->outb) vsp
***** Signal mux_1level_tapbuf_size2[14]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[14]->in[0] mux_1level_tapbuf_size2[14]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[14]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[14]->in[1] mux_1level_tapbuf_size2[14]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[14] gvdd_mux_1level_tapbuf_size2[14] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[419] trig v(mux_1level_tapbuf_size2[14]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[14]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[419] trig v(mux_1level_tapbuf_size2[14]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[14]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[419] when v(mux_1level_tapbuf_size2[14]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[419] trig v(mux_1level_tapbuf_size2[14]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[14]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[419] when v(mux_1level_tapbuf_size2[14]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[419] trig v(mux_1level_tapbuf_size2[14]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[14]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[14]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[14]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[419] param='mux_1level_tapbuf_size2[14]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[14]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[14]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[14]_energy_per_cycle param='mux_1level_tapbuf_size2[14]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[419]  param='mux_1level_tapbuf_size2[14]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[419]  param='dynamic_power_sb_mux[0][0]_rrnode[419]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[419] avg p(Vgvdd_mux_1level_tapbuf_size2[14]) from='start_rise_sb_mux[0][0]_rrnode[419]' to='start_rise_sb_mux[0][0]_rrnode[419]+switch_rise_sb_mux[0][0]_rrnode[419]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[419] avg p(Vgvdd_mux_1level_tapbuf_size2[14]) from='start_fall_sb_mux[0][0]_rrnode[419]' to='start_fall_sb_mux[0][0]_rrnode[419]+switch_fall_sb_mux[0][0]_rrnode[419]'
.meas tran sum_leakage_power_mux[0to14] 
+          param='sum_leakage_power_mux[0to13]+leakage_sb_mux[0][0]_rrnode[419]'
.meas tran sum_energy_per_cycle_mux[0to14] 
+          param='sum_energy_per_cycle_mux[0to13]+energy_per_cycle_sb_mux[0][0]_rrnode[419]'
***** Load for rr_node[419] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=28, type=5 *****
Xchan_mux_1level_tapbuf_size2[14]->out_loadlvl[0]_out mux_1level_tapbuf_size2[14]->out mux_1level_tapbuf_size2[14]->out_loadlvl[0]_out mux_1level_tapbuf_size2[14]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[54]_no0 mux_1level_tapbuf_size2[14]->out_loadlvl[0]_out mux_1level_tapbuf_size2[14]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[55]_no0 mux_1level_tapbuf_size2[14]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[14]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[56]_no0 mux_1level_tapbuf_size2[14]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[14]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[57]_no0 mux_1level_tapbuf_size2[14]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[14]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to14] 
+          param='sum_leakage_power_sb_mux[0to13]+leakage_sb_mux[0][0]_rrnode[419]'
.meas tran sum_energy_per_cycle_sb_mux[0to14] 
+          param='sum_energy_per_cycle_sb_mux[0to13]+energy_per_cycle_sb_mux[0][0]_rrnode[419]'
Xmux_1level_tapbuf_size2[15] mux_1level_tapbuf_size2[15]->in[0] mux_1level_tapbuf_size2[15]->in[1] mux_1level_tapbuf_size2[15]->out sram[15]->outb sram[15]->out gvdd_mux_1level_tapbuf_size2[15] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[15], level=1, select_path_id=0. *****
*****1*****
Xsram[15] sram->in sram[15]->out sram[15]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[15]->out) 0
.nodeset V(sram[15]->outb) vsp
***** Signal mux_1level_tapbuf_size2[15]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[15]->in[0] mux_1level_tapbuf_size2[15]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[15]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[15]->in[1] mux_1level_tapbuf_size2[15]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[15] gvdd_mux_1level_tapbuf_size2[15] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[421] trig v(mux_1level_tapbuf_size2[15]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[15]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[421] trig v(mux_1level_tapbuf_size2[15]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[15]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[421] when v(mux_1level_tapbuf_size2[15]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[421] trig v(mux_1level_tapbuf_size2[15]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[15]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[421] when v(mux_1level_tapbuf_size2[15]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[421] trig v(mux_1level_tapbuf_size2[15]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[15]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[15]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[15]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[421] param='mux_1level_tapbuf_size2[15]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[15]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[15]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[15]_energy_per_cycle param='mux_1level_tapbuf_size2[15]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[421]  param='mux_1level_tapbuf_size2[15]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[421]  param='dynamic_power_sb_mux[0][0]_rrnode[421]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[421] avg p(Vgvdd_mux_1level_tapbuf_size2[15]) from='start_rise_sb_mux[0][0]_rrnode[421]' to='start_rise_sb_mux[0][0]_rrnode[421]+switch_rise_sb_mux[0][0]_rrnode[421]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[421] avg p(Vgvdd_mux_1level_tapbuf_size2[15]) from='start_fall_sb_mux[0][0]_rrnode[421]' to='start_fall_sb_mux[0][0]_rrnode[421]+switch_fall_sb_mux[0][0]_rrnode[421]'
.meas tran sum_leakage_power_mux[0to15] 
+          param='sum_leakage_power_mux[0to14]+leakage_sb_mux[0][0]_rrnode[421]'
.meas tran sum_energy_per_cycle_mux[0to15] 
+          param='sum_energy_per_cycle_mux[0to14]+energy_per_cycle_sb_mux[0][0]_rrnode[421]'
***** Load for rr_node[421] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=30, type=5 *****
Xchan_mux_1level_tapbuf_size2[15]->out_loadlvl[0]_out mux_1level_tapbuf_size2[15]->out mux_1level_tapbuf_size2[15]->out_loadlvl[0]_out mux_1level_tapbuf_size2[15]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[58]_no0 mux_1level_tapbuf_size2[15]->out_loadlvl[0]_out mux_1level_tapbuf_size2[15]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[59]_no0 mux_1level_tapbuf_size2[15]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[15]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[60]_no0 mux_1level_tapbuf_size2[15]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[15]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[61]_no0 mux_1level_tapbuf_size2[15]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[15]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to15] 
+          param='sum_leakage_power_sb_mux[0to14]+leakage_sb_mux[0][0]_rrnode[421]'
.meas tran sum_energy_per_cycle_sb_mux[0to15] 
+          param='sum_energy_per_cycle_sb_mux[0to14]+energy_per_cycle_sb_mux[0][0]_rrnode[421]'
Xmux_1level_tapbuf_size2[16] mux_1level_tapbuf_size2[16]->in[0] mux_1level_tapbuf_size2[16]->in[1] mux_1level_tapbuf_size2[16]->out sram[16]->outb sram[16]->out gvdd_mux_1level_tapbuf_size2[16] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[16], level=1, select_path_id=0. *****
*****1*****
Xsram[16] sram->in sram[16]->out sram[16]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[16]->out) 0
.nodeset V(sram[16]->outb) vsp
***** Signal mux_1level_tapbuf_size2[16]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[16]->in[0] mux_1level_tapbuf_size2[16]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[16]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[16]->in[1] mux_1level_tapbuf_size2[16]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[16] gvdd_mux_1level_tapbuf_size2[16] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[423] trig v(mux_1level_tapbuf_size2[16]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[16]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[423] trig v(mux_1level_tapbuf_size2[16]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[16]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[423] when v(mux_1level_tapbuf_size2[16]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[423] trig v(mux_1level_tapbuf_size2[16]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[16]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[423] when v(mux_1level_tapbuf_size2[16]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[423] trig v(mux_1level_tapbuf_size2[16]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[16]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[16]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[16]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[423] param='mux_1level_tapbuf_size2[16]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[16]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[16]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[16]_energy_per_cycle param='mux_1level_tapbuf_size2[16]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[423]  param='mux_1level_tapbuf_size2[16]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[423]  param='dynamic_power_sb_mux[0][0]_rrnode[423]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[423] avg p(Vgvdd_mux_1level_tapbuf_size2[16]) from='start_rise_sb_mux[0][0]_rrnode[423]' to='start_rise_sb_mux[0][0]_rrnode[423]+switch_rise_sb_mux[0][0]_rrnode[423]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[423] avg p(Vgvdd_mux_1level_tapbuf_size2[16]) from='start_fall_sb_mux[0][0]_rrnode[423]' to='start_fall_sb_mux[0][0]_rrnode[423]+switch_fall_sb_mux[0][0]_rrnode[423]'
.meas tran sum_leakage_power_mux[0to16] 
+          param='sum_leakage_power_mux[0to15]+leakage_sb_mux[0][0]_rrnode[423]'
.meas tran sum_energy_per_cycle_mux[0to16] 
+          param='sum_energy_per_cycle_mux[0to15]+energy_per_cycle_sb_mux[0][0]_rrnode[423]'
***** Load for rr_node[423] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=32, type=5 *****
Xchan_mux_1level_tapbuf_size2[16]->out_loadlvl[0]_out mux_1level_tapbuf_size2[16]->out mux_1level_tapbuf_size2[16]->out_loadlvl[0]_out mux_1level_tapbuf_size2[16]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[62]_no0 mux_1level_tapbuf_size2[16]->out_loadlvl[0]_out mux_1level_tapbuf_size2[16]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[63]_no0 mux_1level_tapbuf_size2[16]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[16]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[64]_no0 mux_1level_tapbuf_size2[16]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[16]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[65]_no0 mux_1level_tapbuf_size2[16]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[16]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to16] 
+          param='sum_leakage_power_sb_mux[0to15]+leakage_sb_mux[0][0]_rrnode[423]'
.meas tran sum_energy_per_cycle_sb_mux[0to16] 
+          param='sum_energy_per_cycle_sb_mux[0to15]+energy_per_cycle_sb_mux[0][0]_rrnode[423]'
Xmux_1level_tapbuf_size2[17] mux_1level_tapbuf_size2[17]->in[0] mux_1level_tapbuf_size2[17]->in[1] mux_1level_tapbuf_size2[17]->out sram[17]->outb sram[17]->out gvdd_mux_1level_tapbuf_size2[17] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[17], level=1, select_path_id=0. *****
*****1*****
Xsram[17] sram->in sram[17]->out sram[17]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[17]->out) 0
.nodeset V(sram[17]->outb) vsp
***** Signal mux_1level_tapbuf_size2[17]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[17]->in[0] mux_1level_tapbuf_size2[17]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[17]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[17]->in[1] mux_1level_tapbuf_size2[17]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[17] gvdd_mux_1level_tapbuf_size2[17] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[425] trig v(mux_1level_tapbuf_size2[17]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[17]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[425] trig v(mux_1level_tapbuf_size2[17]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[17]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[425] when v(mux_1level_tapbuf_size2[17]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[425] trig v(mux_1level_tapbuf_size2[17]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[17]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[425] when v(mux_1level_tapbuf_size2[17]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[425] trig v(mux_1level_tapbuf_size2[17]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[17]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[17]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[17]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[425] param='mux_1level_tapbuf_size2[17]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[17]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[17]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[17]_energy_per_cycle param='mux_1level_tapbuf_size2[17]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[425]  param='mux_1level_tapbuf_size2[17]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[425]  param='dynamic_power_sb_mux[0][0]_rrnode[425]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[425] avg p(Vgvdd_mux_1level_tapbuf_size2[17]) from='start_rise_sb_mux[0][0]_rrnode[425]' to='start_rise_sb_mux[0][0]_rrnode[425]+switch_rise_sb_mux[0][0]_rrnode[425]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[425] avg p(Vgvdd_mux_1level_tapbuf_size2[17]) from='start_fall_sb_mux[0][0]_rrnode[425]' to='start_fall_sb_mux[0][0]_rrnode[425]+switch_fall_sb_mux[0][0]_rrnode[425]'
.meas tran sum_leakage_power_mux[0to17] 
+          param='sum_leakage_power_mux[0to16]+leakage_sb_mux[0][0]_rrnode[425]'
.meas tran sum_energy_per_cycle_mux[0to17] 
+          param='sum_energy_per_cycle_mux[0to16]+energy_per_cycle_sb_mux[0][0]_rrnode[425]'
***** Load for rr_node[425] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=34, type=5 *****
Xchan_mux_1level_tapbuf_size2[17]->out_loadlvl[0]_out mux_1level_tapbuf_size2[17]->out mux_1level_tapbuf_size2[17]->out_loadlvl[0]_out mux_1level_tapbuf_size2[17]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[66]_no0 mux_1level_tapbuf_size2[17]->out_loadlvl[0]_out mux_1level_tapbuf_size2[17]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[67]_no0 mux_1level_tapbuf_size2[17]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[17]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[68]_no0 mux_1level_tapbuf_size2[17]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[17]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[69]_no0 mux_1level_tapbuf_size2[17]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[17]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to17] 
+          param='sum_leakage_power_sb_mux[0to16]+leakage_sb_mux[0][0]_rrnode[425]'
.meas tran sum_energy_per_cycle_sb_mux[0to17] 
+          param='sum_energy_per_cycle_sb_mux[0to16]+energy_per_cycle_sb_mux[0][0]_rrnode[425]'
Xmux_1level_tapbuf_size2[18] mux_1level_tapbuf_size2[18]->in[0] mux_1level_tapbuf_size2[18]->in[1] mux_1level_tapbuf_size2[18]->out sram[18]->outb sram[18]->out gvdd_mux_1level_tapbuf_size2[18] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[18], level=1, select_path_id=0. *****
*****1*****
Xsram[18] sram->in sram[18]->out sram[18]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[18]->out) 0
.nodeset V(sram[18]->outb) vsp
***** Signal mux_1level_tapbuf_size2[18]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[18]->in[0] mux_1level_tapbuf_size2[18]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[18]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[18]->in[1] mux_1level_tapbuf_size2[18]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[18] gvdd_mux_1level_tapbuf_size2[18] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[427] trig v(mux_1level_tapbuf_size2[18]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[18]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[427] trig v(mux_1level_tapbuf_size2[18]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[18]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[427] when v(mux_1level_tapbuf_size2[18]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[427] trig v(mux_1level_tapbuf_size2[18]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[18]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[427] when v(mux_1level_tapbuf_size2[18]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[427] trig v(mux_1level_tapbuf_size2[18]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[18]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[18]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[18]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[427] param='mux_1level_tapbuf_size2[18]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[18]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[18]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[18]_energy_per_cycle param='mux_1level_tapbuf_size2[18]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[427]  param='mux_1level_tapbuf_size2[18]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[427]  param='dynamic_power_sb_mux[0][0]_rrnode[427]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[427] avg p(Vgvdd_mux_1level_tapbuf_size2[18]) from='start_rise_sb_mux[0][0]_rrnode[427]' to='start_rise_sb_mux[0][0]_rrnode[427]+switch_rise_sb_mux[0][0]_rrnode[427]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[427] avg p(Vgvdd_mux_1level_tapbuf_size2[18]) from='start_fall_sb_mux[0][0]_rrnode[427]' to='start_fall_sb_mux[0][0]_rrnode[427]+switch_fall_sb_mux[0][0]_rrnode[427]'
.meas tran sum_leakage_power_mux[0to18] 
+          param='sum_leakage_power_mux[0to17]+leakage_sb_mux[0][0]_rrnode[427]'
.meas tran sum_energy_per_cycle_mux[0to18] 
+          param='sum_energy_per_cycle_mux[0to17]+energy_per_cycle_sb_mux[0][0]_rrnode[427]'
***** Load for rr_node[427] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=36, type=5 *****
Xchan_mux_1level_tapbuf_size2[18]->out_loadlvl[0]_out mux_1level_tapbuf_size2[18]->out mux_1level_tapbuf_size2[18]->out_loadlvl[0]_out mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[70]_no0 mux_1level_tapbuf_size2[18]->out_loadlvl[0]_out mux_1level_tapbuf_size2[18]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[71]_no0 mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[72]_no0 mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[73]_no0 mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to18] 
+          param='sum_leakage_power_sb_mux[0to17]+leakage_sb_mux[0][0]_rrnode[427]'
.meas tran sum_energy_per_cycle_sb_mux[0to18] 
+          param='sum_energy_per_cycle_sb_mux[0to17]+energy_per_cycle_sb_mux[0][0]_rrnode[427]'
Xmux_1level_tapbuf_size2[19] mux_1level_tapbuf_size2[19]->in[0] mux_1level_tapbuf_size2[19]->in[1] mux_1level_tapbuf_size2[19]->out sram[19]->outb sram[19]->out gvdd_mux_1level_tapbuf_size2[19] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[19], level=1, select_path_id=0. *****
*****1*****
Xsram[19] sram->in sram[19]->out sram[19]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[19]->out) 0
.nodeset V(sram[19]->outb) vsp
***** Signal mux_1level_tapbuf_size2[19]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[19]->in[0] mux_1level_tapbuf_size2[19]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[19]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[19]->in[1] mux_1level_tapbuf_size2[19]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[19] gvdd_mux_1level_tapbuf_size2[19] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[429] trig v(mux_1level_tapbuf_size2[19]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[19]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[429] trig v(mux_1level_tapbuf_size2[19]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[19]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[429] when v(mux_1level_tapbuf_size2[19]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[429] trig v(mux_1level_tapbuf_size2[19]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[19]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[429] when v(mux_1level_tapbuf_size2[19]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[429] trig v(mux_1level_tapbuf_size2[19]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[19]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[19]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[19]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[429] param='mux_1level_tapbuf_size2[19]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[19]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[19]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[19]_energy_per_cycle param='mux_1level_tapbuf_size2[19]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[429]  param='mux_1level_tapbuf_size2[19]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[429]  param='dynamic_power_sb_mux[0][0]_rrnode[429]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[429] avg p(Vgvdd_mux_1level_tapbuf_size2[19]) from='start_rise_sb_mux[0][0]_rrnode[429]' to='start_rise_sb_mux[0][0]_rrnode[429]+switch_rise_sb_mux[0][0]_rrnode[429]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[429] avg p(Vgvdd_mux_1level_tapbuf_size2[19]) from='start_fall_sb_mux[0][0]_rrnode[429]' to='start_fall_sb_mux[0][0]_rrnode[429]+switch_fall_sb_mux[0][0]_rrnode[429]'
.meas tran sum_leakage_power_mux[0to19] 
+          param='sum_leakage_power_mux[0to18]+leakage_sb_mux[0][0]_rrnode[429]'
.meas tran sum_energy_per_cycle_mux[0to19] 
+          param='sum_energy_per_cycle_mux[0to18]+energy_per_cycle_sb_mux[0][0]_rrnode[429]'
***** Load for rr_node[429] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=38, type=5 *****
Xchan_mux_1level_tapbuf_size2[19]->out_loadlvl[0]_out mux_1level_tapbuf_size2[19]->out mux_1level_tapbuf_size2[19]->out_loadlvl[0]_out mux_1level_tapbuf_size2[19]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[74]_no0 mux_1level_tapbuf_size2[19]->out_loadlvl[0]_out mux_1level_tapbuf_size2[19]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[75]_no0 mux_1level_tapbuf_size2[19]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[19]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[76]_no0 mux_1level_tapbuf_size2[19]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[19]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[77]_no0 mux_1level_tapbuf_size2[19]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[19]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to19] 
+          param='sum_leakage_power_sb_mux[0to18]+leakage_sb_mux[0][0]_rrnode[429]'
.meas tran sum_energy_per_cycle_sb_mux[0to19] 
+          param='sum_energy_per_cycle_sb_mux[0to18]+energy_per_cycle_sb_mux[0][0]_rrnode[429]'
Xmux_1level_tapbuf_size2[20] mux_1level_tapbuf_size2[20]->in[0] mux_1level_tapbuf_size2[20]->in[1] mux_1level_tapbuf_size2[20]->out sram[20]->outb sram[20]->out gvdd_mux_1level_tapbuf_size2[20] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[20], level=1, select_path_id=0. *****
*****1*****
Xsram[20] sram->in sram[20]->out sram[20]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[20]->out) 0
.nodeset V(sram[20]->outb) vsp
***** Signal mux_1level_tapbuf_size2[20]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[20]->in[0] mux_1level_tapbuf_size2[20]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[20]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[20]->in[1] mux_1level_tapbuf_size2[20]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[20] gvdd_mux_1level_tapbuf_size2[20] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[431] trig v(mux_1level_tapbuf_size2[20]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[20]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[431] trig v(mux_1level_tapbuf_size2[20]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[20]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[431] when v(mux_1level_tapbuf_size2[20]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[431] trig v(mux_1level_tapbuf_size2[20]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[20]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[431] when v(mux_1level_tapbuf_size2[20]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[431] trig v(mux_1level_tapbuf_size2[20]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[20]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[20]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[20]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[431] param='mux_1level_tapbuf_size2[20]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[20]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[20]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[20]_energy_per_cycle param='mux_1level_tapbuf_size2[20]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[431]  param='mux_1level_tapbuf_size2[20]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[431]  param='dynamic_power_sb_mux[0][0]_rrnode[431]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[431] avg p(Vgvdd_mux_1level_tapbuf_size2[20]) from='start_rise_sb_mux[0][0]_rrnode[431]' to='start_rise_sb_mux[0][0]_rrnode[431]+switch_rise_sb_mux[0][0]_rrnode[431]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[431] avg p(Vgvdd_mux_1level_tapbuf_size2[20]) from='start_fall_sb_mux[0][0]_rrnode[431]' to='start_fall_sb_mux[0][0]_rrnode[431]+switch_fall_sb_mux[0][0]_rrnode[431]'
.meas tran sum_leakage_power_mux[0to20] 
+          param='sum_leakage_power_mux[0to19]+leakage_sb_mux[0][0]_rrnode[431]'
.meas tran sum_energy_per_cycle_mux[0to20] 
+          param='sum_energy_per_cycle_mux[0to19]+energy_per_cycle_sb_mux[0][0]_rrnode[431]'
***** Load for rr_node[431] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=40, type=5 *****
Xchan_mux_1level_tapbuf_size2[20]->out_loadlvl[0]_out mux_1level_tapbuf_size2[20]->out mux_1level_tapbuf_size2[20]->out_loadlvl[0]_out mux_1level_tapbuf_size2[20]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[78]_no0 mux_1level_tapbuf_size2[20]->out_loadlvl[0]_out mux_1level_tapbuf_size2[20]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[79]_no0 mux_1level_tapbuf_size2[20]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[20]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[80]_no0 mux_1level_tapbuf_size2[20]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[20]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to20] 
+          param='sum_leakage_power_sb_mux[0to19]+leakage_sb_mux[0][0]_rrnode[431]'
.meas tran sum_energy_per_cycle_sb_mux[0to20] 
+          param='sum_energy_per_cycle_sb_mux[0to19]+energy_per_cycle_sb_mux[0][0]_rrnode[431]'
Xmux_1level_tapbuf_size2[21] mux_1level_tapbuf_size2[21]->in[0] mux_1level_tapbuf_size2[21]->in[1] mux_1level_tapbuf_size2[21]->out sram[21]->outb sram[21]->out gvdd_mux_1level_tapbuf_size2[21] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[21], level=1, select_path_id=0. *****
*****1*****
Xsram[21] sram->in sram[21]->out sram[21]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[21]->out) 0
.nodeset V(sram[21]->outb) vsp
***** Signal mux_1level_tapbuf_size2[21]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[21]->in[0] mux_1level_tapbuf_size2[21]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[21]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[21]->in[1] mux_1level_tapbuf_size2[21]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[21] gvdd_mux_1level_tapbuf_size2[21] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[433] trig v(mux_1level_tapbuf_size2[21]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[21]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[433] trig v(mux_1level_tapbuf_size2[21]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[21]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[433] when v(mux_1level_tapbuf_size2[21]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[433] trig v(mux_1level_tapbuf_size2[21]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[21]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[433] when v(mux_1level_tapbuf_size2[21]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[433] trig v(mux_1level_tapbuf_size2[21]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[21]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[21]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[21]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[433] param='mux_1level_tapbuf_size2[21]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[21]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[21]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[21]_energy_per_cycle param='mux_1level_tapbuf_size2[21]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[433]  param='mux_1level_tapbuf_size2[21]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[433]  param='dynamic_power_sb_mux[0][0]_rrnode[433]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[433] avg p(Vgvdd_mux_1level_tapbuf_size2[21]) from='start_rise_sb_mux[0][0]_rrnode[433]' to='start_rise_sb_mux[0][0]_rrnode[433]+switch_rise_sb_mux[0][0]_rrnode[433]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[433] avg p(Vgvdd_mux_1level_tapbuf_size2[21]) from='start_fall_sb_mux[0][0]_rrnode[433]' to='start_fall_sb_mux[0][0]_rrnode[433]+switch_fall_sb_mux[0][0]_rrnode[433]'
.meas tran sum_leakage_power_mux[0to21] 
+          param='sum_leakage_power_mux[0to20]+leakage_sb_mux[0][0]_rrnode[433]'
.meas tran sum_energy_per_cycle_mux[0to21] 
+          param='sum_energy_per_cycle_mux[0to20]+energy_per_cycle_sb_mux[0][0]_rrnode[433]'
***** Load for rr_node[433] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=42, type=5 *****
Xchan_mux_1level_tapbuf_size2[21]->out_loadlvl[0]_out mux_1level_tapbuf_size2[21]->out mux_1level_tapbuf_size2[21]->out_loadlvl[0]_out mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[81]_no0 mux_1level_tapbuf_size2[21]->out_loadlvl[0]_out mux_1level_tapbuf_size2[21]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[82]_no0 mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[83]_no0 mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[84]_no0 mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to21] 
+          param='sum_leakage_power_sb_mux[0to20]+leakage_sb_mux[0][0]_rrnode[433]'
.meas tran sum_energy_per_cycle_sb_mux[0to21] 
+          param='sum_energy_per_cycle_sb_mux[0to20]+energy_per_cycle_sb_mux[0][0]_rrnode[433]'
Xmux_1level_tapbuf_size2[22] mux_1level_tapbuf_size2[22]->in[0] mux_1level_tapbuf_size2[22]->in[1] mux_1level_tapbuf_size2[22]->out sram[22]->outb sram[22]->out gvdd_mux_1level_tapbuf_size2[22] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[22], level=1, select_path_id=0. *****
*****1*****
Xsram[22] sram->in sram[22]->out sram[22]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[22]->out) 0
.nodeset V(sram[22]->outb) vsp
***** Signal mux_1level_tapbuf_size2[22]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[22]->in[0] mux_1level_tapbuf_size2[22]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[22]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[22]->in[1] mux_1level_tapbuf_size2[22]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[22] gvdd_mux_1level_tapbuf_size2[22] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[435] trig v(mux_1level_tapbuf_size2[22]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[22]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[435] trig v(mux_1level_tapbuf_size2[22]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[22]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[435] when v(mux_1level_tapbuf_size2[22]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[435] trig v(mux_1level_tapbuf_size2[22]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[22]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[435] when v(mux_1level_tapbuf_size2[22]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[435] trig v(mux_1level_tapbuf_size2[22]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[22]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[22]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[22]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[435] param='mux_1level_tapbuf_size2[22]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[22]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[22]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[22]_energy_per_cycle param='mux_1level_tapbuf_size2[22]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[435]  param='mux_1level_tapbuf_size2[22]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[435]  param='dynamic_power_sb_mux[0][0]_rrnode[435]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[435] avg p(Vgvdd_mux_1level_tapbuf_size2[22]) from='start_rise_sb_mux[0][0]_rrnode[435]' to='start_rise_sb_mux[0][0]_rrnode[435]+switch_rise_sb_mux[0][0]_rrnode[435]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[435] avg p(Vgvdd_mux_1level_tapbuf_size2[22]) from='start_fall_sb_mux[0][0]_rrnode[435]' to='start_fall_sb_mux[0][0]_rrnode[435]+switch_fall_sb_mux[0][0]_rrnode[435]'
.meas tran sum_leakage_power_mux[0to22] 
+          param='sum_leakage_power_mux[0to21]+leakage_sb_mux[0][0]_rrnode[435]'
.meas tran sum_energy_per_cycle_mux[0to22] 
+          param='sum_energy_per_cycle_mux[0to21]+energy_per_cycle_sb_mux[0][0]_rrnode[435]'
***** Load for rr_node[435] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=44, type=5 *****
Xchan_mux_1level_tapbuf_size2[22]->out_loadlvl[0]_out mux_1level_tapbuf_size2[22]->out mux_1level_tapbuf_size2[22]->out_loadlvl[0]_out mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[85]_no0 mux_1level_tapbuf_size2[22]->out_loadlvl[0]_out mux_1level_tapbuf_size2[22]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[86]_no0 mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[87]_no0 mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[88]_no0 mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[89]_no0 mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to22] 
+          param='sum_leakage_power_sb_mux[0to21]+leakage_sb_mux[0][0]_rrnode[435]'
.meas tran sum_energy_per_cycle_sb_mux[0to22] 
+          param='sum_energy_per_cycle_sb_mux[0to21]+energy_per_cycle_sb_mux[0][0]_rrnode[435]'
Xmux_1level_tapbuf_size2[23] mux_1level_tapbuf_size2[23]->in[0] mux_1level_tapbuf_size2[23]->in[1] mux_1level_tapbuf_size2[23]->out sram[23]->outb sram[23]->out gvdd_mux_1level_tapbuf_size2[23] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[23], level=1, select_path_id=0. *****
*****1*****
Xsram[23] sram->in sram[23]->out sram[23]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[23]->out) 0
.nodeset V(sram[23]->outb) vsp
***** Signal mux_1level_tapbuf_size2[23]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[23]->in[0] mux_1level_tapbuf_size2[23]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[23]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[23]->in[1] mux_1level_tapbuf_size2[23]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[23] gvdd_mux_1level_tapbuf_size2[23] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[437] trig v(mux_1level_tapbuf_size2[23]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[23]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[437] trig v(mux_1level_tapbuf_size2[23]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[23]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[437] when v(mux_1level_tapbuf_size2[23]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[437] trig v(mux_1level_tapbuf_size2[23]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[23]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[437] when v(mux_1level_tapbuf_size2[23]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[437] trig v(mux_1level_tapbuf_size2[23]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[23]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[23]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[23]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[437] param='mux_1level_tapbuf_size2[23]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[23]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[23]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[23]_energy_per_cycle param='mux_1level_tapbuf_size2[23]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[437]  param='mux_1level_tapbuf_size2[23]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[437]  param='dynamic_power_sb_mux[0][0]_rrnode[437]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[437] avg p(Vgvdd_mux_1level_tapbuf_size2[23]) from='start_rise_sb_mux[0][0]_rrnode[437]' to='start_rise_sb_mux[0][0]_rrnode[437]+switch_rise_sb_mux[0][0]_rrnode[437]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[437] avg p(Vgvdd_mux_1level_tapbuf_size2[23]) from='start_fall_sb_mux[0][0]_rrnode[437]' to='start_fall_sb_mux[0][0]_rrnode[437]+switch_fall_sb_mux[0][0]_rrnode[437]'
.meas tran sum_leakage_power_mux[0to23] 
+          param='sum_leakage_power_mux[0to22]+leakage_sb_mux[0][0]_rrnode[437]'
.meas tran sum_energy_per_cycle_mux[0to23] 
+          param='sum_energy_per_cycle_mux[0to22]+energy_per_cycle_sb_mux[0][0]_rrnode[437]'
***** Load for rr_node[437] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=46, type=5 *****
Xchan_mux_1level_tapbuf_size2[23]->out_loadlvl[0]_out mux_1level_tapbuf_size2[23]->out mux_1level_tapbuf_size2[23]->out_loadlvl[0]_out mux_1level_tapbuf_size2[23]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[90]_no0 mux_1level_tapbuf_size2[23]->out_loadlvl[0]_out mux_1level_tapbuf_size2[23]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[91]_no0 mux_1level_tapbuf_size2[23]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[23]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[92]_no0 mux_1level_tapbuf_size2[23]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[23]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to23] 
+          param='sum_leakage_power_sb_mux[0to22]+leakage_sb_mux[0][0]_rrnode[437]'
.meas tran sum_energy_per_cycle_sb_mux[0to23] 
+          param='sum_energy_per_cycle_sb_mux[0to22]+energy_per_cycle_sb_mux[0][0]_rrnode[437]'
Xmux_1level_tapbuf_size2[24] mux_1level_tapbuf_size2[24]->in[0] mux_1level_tapbuf_size2[24]->in[1] mux_1level_tapbuf_size2[24]->out sram[24]->outb sram[24]->out gvdd_mux_1level_tapbuf_size2[24] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[24], level=1, select_path_id=0. *****
*****1*****
Xsram[24] sram->in sram[24]->out sram[24]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[24]->out) 0
.nodeset V(sram[24]->outb) vsp
***** Signal mux_1level_tapbuf_size2[24]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[24]->in[0] mux_1level_tapbuf_size2[24]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[24]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[24]->in[1] mux_1level_tapbuf_size2[24]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[24] gvdd_mux_1level_tapbuf_size2[24] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[439] trig v(mux_1level_tapbuf_size2[24]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[24]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[439] trig v(mux_1level_tapbuf_size2[24]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[24]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[439] when v(mux_1level_tapbuf_size2[24]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[439] trig v(mux_1level_tapbuf_size2[24]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[24]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[439] when v(mux_1level_tapbuf_size2[24]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[439] trig v(mux_1level_tapbuf_size2[24]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[24]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[24]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[24]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[439] param='mux_1level_tapbuf_size2[24]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[24]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[24]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[24]_energy_per_cycle param='mux_1level_tapbuf_size2[24]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[439]  param='mux_1level_tapbuf_size2[24]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[439]  param='dynamic_power_sb_mux[0][0]_rrnode[439]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[439] avg p(Vgvdd_mux_1level_tapbuf_size2[24]) from='start_rise_sb_mux[0][0]_rrnode[439]' to='start_rise_sb_mux[0][0]_rrnode[439]+switch_rise_sb_mux[0][0]_rrnode[439]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[439] avg p(Vgvdd_mux_1level_tapbuf_size2[24]) from='start_fall_sb_mux[0][0]_rrnode[439]' to='start_fall_sb_mux[0][0]_rrnode[439]+switch_fall_sb_mux[0][0]_rrnode[439]'
.meas tran sum_leakage_power_mux[0to24] 
+          param='sum_leakage_power_mux[0to23]+leakage_sb_mux[0][0]_rrnode[439]'
.meas tran sum_energy_per_cycle_mux[0to24] 
+          param='sum_energy_per_cycle_mux[0to23]+energy_per_cycle_sb_mux[0][0]_rrnode[439]'
***** Load for rr_node[439] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=48, type=5 *****
Xchan_mux_1level_tapbuf_size2[24]->out_loadlvl[0]_out mux_1level_tapbuf_size2[24]->out mux_1level_tapbuf_size2[24]->out_loadlvl[0]_out mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[93]_no0 mux_1level_tapbuf_size2[24]->out_loadlvl[0]_out mux_1level_tapbuf_size2[24]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[94]_no0 mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[95]_no0 mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[96]_no0 mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to24] 
+          param='sum_leakage_power_sb_mux[0to23]+leakage_sb_mux[0][0]_rrnode[439]'
.meas tran sum_energy_per_cycle_sb_mux[0to24] 
+          param='sum_energy_per_cycle_sb_mux[0to23]+energy_per_cycle_sb_mux[0][0]_rrnode[439]'
Xmux_1level_tapbuf_size2[25] mux_1level_tapbuf_size2[25]->in[0] mux_1level_tapbuf_size2[25]->in[1] mux_1level_tapbuf_size2[25]->out sram[25]->outb sram[25]->out gvdd_mux_1level_tapbuf_size2[25] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[25], level=1, select_path_id=0. *****
*****1*****
Xsram[25] sram->in sram[25]->out sram[25]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[25]->out) 0
.nodeset V(sram[25]->outb) vsp
***** Signal mux_1level_tapbuf_size2[25]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[25]->in[0] mux_1level_tapbuf_size2[25]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[25]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[25]->in[1] mux_1level_tapbuf_size2[25]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[25] gvdd_mux_1level_tapbuf_size2[25] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[441] trig v(mux_1level_tapbuf_size2[25]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[25]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[441] trig v(mux_1level_tapbuf_size2[25]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[25]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[441] when v(mux_1level_tapbuf_size2[25]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[441] trig v(mux_1level_tapbuf_size2[25]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[25]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[441] when v(mux_1level_tapbuf_size2[25]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[441] trig v(mux_1level_tapbuf_size2[25]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[25]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[25]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[25]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[441] param='mux_1level_tapbuf_size2[25]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[25]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[25]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[25]_energy_per_cycle param='mux_1level_tapbuf_size2[25]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[441]  param='mux_1level_tapbuf_size2[25]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[441]  param='dynamic_power_sb_mux[0][0]_rrnode[441]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[441] avg p(Vgvdd_mux_1level_tapbuf_size2[25]) from='start_rise_sb_mux[0][0]_rrnode[441]' to='start_rise_sb_mux[0][0]_rrnode[441]+switch_rise_sb_mux[0][0]_rrnode[441]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[441] avg p(Vgvdd_mux_1level_tapbuf_size2[25]) from='start_fall_sb_mux[0][0]_rrnode[441]' to='start_fall_sb_mux[0][0]_rrnode[441]+switch_fall_sb_mux[0][0]_rrnode[441]'
.meas tran sum_leakage_power_mux[0to25] 
+          param='sum_leakage_power_mux[0to24]+leakage_sb_mux[0][0]_rrnode[441]'
.meas tran sum_energy_per_cycle_mux[0to25] 
+          param='sum_energy_per_cycle_mux[0to24]+energy_per_cycle_sb_mux[0][0]_rrnode[441]'
***** Load for rr_node[441] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=50, type=5 *****
Xchan_mux_1level_tapbuf_size2[25]->out_loadlvl[0]_out mux_1level_tapbuf_size2[25]->out mux_1level_tapbuf_size2[25]->out_loadlvl[0]_out mux_1level_tapbuf_size2[25]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[97]_no0 mux_1level_tapbuf_size2[25]->out_loadlvl[0]_out mux_1level_tapbuf_size2[25]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[98]_no0 mux_1level_tapbuf_size2[25]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[25]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[99]_no0 mux_1level_tapbuf_size2[25]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[25]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[100]_no0 mux_1level_tapbuf_size2[25]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[25]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to25] 
+          param='sum_leakage_power_sb_mux[0to24]+leakage_sb_mux[0][0]_rrnode[441]'
.meas tran sum_energy_per_cycle_sb_mux[0to25] 
+          param='sum_energy_per_cycle_sb_mux[0to24]+energy_per_cycle_sb_mux[0][0]_rrnode[441]'
Xmux_1level_tapbuf_size2[26] mux_1level_tapbuf_size2[26]->in[0] mux_1level_tapbuf_size2[26]->in[1] mux_1level_tapbuf_size2[26]->out sram[26]->outb sram[26]->out gvdd_mux_1level_tapbuf_size2[26] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[26], level=1, select_path_id=0. *****
*****1*****
Xsram[26] sram->in sram[26]->out sram[26]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[26]->out) 0
.nodeset V(sram[26]->outb) vsp
***** Signal mux_1level_tapbuf_size2[26]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[26]->in[0] mux_1level_tapbuf_size2[26]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[26]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[26]->in[1] mux_1level_tapbuf_size2[26]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[26] gvdd_mux_1level_tapbuf_size2[26] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[443] trig v(mux_1level_tapbuf_size2[26]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[26]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[443] trig v(mux_1level_tapbuf_size2[26]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[26]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[443] when v(mux_1level_tapbuf_size2[26]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[443] trig v(mux_1level_tapbuf_size2[26]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[26]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[443] when v(mux_1level_tapbuf_size2[26]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[443] trig v(mux_1level_tapbuf_size2[26]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[26]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[26]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[26]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[443] param='mux_1level_tapbuf_size2[26]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[26]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[26]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[26]_energy_per_cycle param='mux_1level_tapbuf_size2[26]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[443]  param='mux_1level_tapbuf_size2[26]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[443]  param='dynamic_power_sb_mux[0][0]_rrnode[443]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[443] avg p(Vgvdd_mux_1level_tapbuf_size2[26]) from='start_rise_sb_mux[0][0]_rrnode[443]' to='start_rise_sb_mux[0][0]_rrnode[443]+switch_rise_sb_mux[0][0]_rrnode[443]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[443] avg p(Vgvdd_mux_1level_tapbuf_size2[26]) from='start_fall_sb_mux[0][0]_rrnode[443]' to='start_fall_sb_mux[0][0]_rrnode[443]+switch_fall_sb_mux[0][0]_rrnode[443]'
.meas tran sum_leakage_power_mux[0to26] 
+          param='sum_leakage_power_mux[0to25]+leakage_sb_mux[0][0]_rrnode[443]'
.meas tran sum_energy_per_cycle_mux[0to26] 
+          param='sum_energy_per_cycle_mux[0to25]+energy_per_cycle_sb_mux[0][0]_rrnode[443]'
***** Load for rr_node[443] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=52, type=5 *****
Xchan_mux_1level_tapbuf_size2[26]->out_loadlvl[0]_out mux_1level_tapbuf_size2[26]->out mux_1level_tapbuf_size2[26]->out_loadlvl[0]_out mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[101]_no0 mux_1level_tapbuf_size2[26]->out_loadlvl[0]_out mux_1level_tapbuf_size2[26]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[102]_no0 mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[103]_no0 mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[104]_no0 mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to26] 
+          param='sum_leakage_power_sb_mux[0to25]+leakage_sb_mux[0][0]_rrnode[443]'
.meas tran sum_energy_per_cycle_sb_mux[0to26] 
+          param='sum_energy_per_cycle_sb_mux[0to25]+energy_per_cycle_sb_mux[0][0]_rrnode[443]'
Xmux_1level_tapbuf_size2[27] mux_1level_tapbuf_size2[27]->in[0] mux_1level_tapbuf_size2[27]->in[1] mux_1level_tapbuf_size2[27]->out sram[27]->outb sram[27]->out gvdd_mux_1level_tapbuf_size2[27] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[27], level=1, select_path_id=0. *****
*****1*****
Xsram[27] sram->in sram[27]->out sram[27]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[27]->out) 0
.nodeset V(sram[27]->outb) vsp
***** Signal mux_1level_tapbuf_size2[27]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[27]->in[0] mux_1level_tapbuf_size2[27]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[27]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[27]->in[1] mux_1level_tapbuf_size2[27]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[27] gvdd_mux_1level_tapbuf_size2[27] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[445] trig v(mux_1level_tapbuf_size2[27]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[27]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[445] trig v(mux_1level_tapbuf_size2[27]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[27]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[445] when v(mux_1level_tapbuf_size2[27]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[445] trig v(mux_1level_tapbuf_size2[27]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[27]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[445] when v(mux_1level_tapbuf_size2[27]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[445] trig v(mux_1level_tapbuf_size2[27]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[27]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[27]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[27]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[445] param='mux_1level_tapbuf_size2[27]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[27]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[27]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[27]_energy_per_cycle param='mux_1level_tapbuf_size2[27]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[445]  param='mux_1level_tapbuf_size2[27]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[445]  param='dynamic_power_sb_mux[0][0]_rrnode[445]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[445] avg p(Vgvdd_mux_1level_tapbuf_size2[27]) from='start_rise_sb_mux[0][0]_rrnode[445]' to='start_rise_sb_mux[0][0]_rrnode[445]+switch_rise_sb_mux[0][0]_rrnode[445]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[445] avg p(Vgvdd_mux_1level_tapbuf_size2[27]) from='start_fall_sb_mux[0][0]_rrnode[445]' to='start_fall_sb_mux[0][0]_rrnode[445]+switch_fall_sb_mux[0][0]_rrnode[445]'
.meas tran sum_leakage_power_mux[0to27] 
+          param='sum_leakage_power_mux[0to26]+leakage_sb_mux[0][0]_rrnode[445]'
.meas tran sum_energy_per_cycle_mux[0to27] 
+          param='sum_energy_per_cycle_mux[0to26]+energy_per_cycle_sb_mux[0][0]_rrnode[445]'
***** Load for rr_node[445] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=54, type=5 *****
Xchan_mux_1level_tapbuf_size2[27]->out_loadlvl[0]_out mux_1level_tapbuf_size2[27]->out mux_1level_tapbuf_size2[27]->out_loadlvl[0]_out mux_1level_tapbuf_size2[27]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[105]_no0 mux_1level_tapbuf_size2[27]->out_loadlvl[0]_out mux_1level_tapbuf_size2[27]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[106]_no0 mux_1level_tapbuf_size2[27]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[27]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[107]_no0 mux_1level_tapbuf_size2[27]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[27]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[108]_no0 mux_1level_tapbuf_size2[27]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[27]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to27] 
+          param='sum_leakage_power_sb_mux[0to26]+leakage_sb_mux[0][0]_rrnode[445]'
.meas tran sum_energy_per_cycle_sb_mux[0to27] 
+          param='sum_energy_per_cycle_sb_mux[0to26]+energy_per_cycle_sb_mux[0][0]_rrnode[445]'
Xmux_1level_tapbuf_size2[28] mux_1level_tapbuf_size2[28]->in[0] mux_1level_tapbuf_size2[28]->in[1] mux_1level_tapbuf_size2[28]->out sram[28]->outb sram[28]->out gvdd_mux_1level_tapbuf_size2[28] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[28], level=1, select_path_id=0. *****
*****1*****
Xsram[28] sram->in sram[28]->out sram[28]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[28]->out) 0
.nodeset V(sram[28]->outb) vsp
***** Signal mux_1level_tapbuf_size2[28]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[28]->in[0] mux_1level_tapbuf_size2[28]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[28]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[28]->in[1] mux_1level_tapbuf_size2[28]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[28] gvdd_mux_1level_tapbuf_size2[28] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[447] trig v(mux_1level_tapbuf_size2[28]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[28]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[447] trig v(mux_1level_tapbuf_size2[28]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[28]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[447] when v(mux_1level_tapbuf_size2[28]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[447] trig v(mux_1level_tapbuf_size2[28]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[28]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[447] when v(mux_1level_tapbuf_size2[28]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[447] trig v(mux_1level_tapbuf_size2[28]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[28]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[28]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[28]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[447] param='mux_1level_tapbuf_size2[28]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[28]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[28]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[28]_energy_per_cycle param='mux_1level_tapbuf_size2[28]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[447]  param='mux_1level_tapbuf_size2[28]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[447]  param='dynamic_power_sb_mux[0][0]_rrnode[447]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[447] avg p(Vgvdd_mux_1level_tapbuf_size2[28]) from='start_rise_sb_mux[0][0]_rrnode[447]' to='start_rise_sb_mux[0][0]_rrnode[447]+switch_rise_sb_mux[0][0]_rrnode[447]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[447] avg p(Vgvdd_mux_1level_tapbuf_size2[28]) from='start_fall_sb_mux[0][0]_rrnode[447]' to='start_fall_sb_mux[0][0]_rrnode[447]+switch_fall_sb_mux[0][0]_rrnode[447]'
.meas tran sum_leakage_power_mux[0to28] 
+          param='sum_leakage_power_mux[0to27]+leakage_sb_mux[0][0]_rrnode[447]'
.meas tran sum_energy_per_cycle_mux[0to28] 
+          param='sum_energy_per_cycle_mux[0to27]+energy_per_cycle_sb_mux[0][0]_rrnode[447]'
***** Load for rr_node[447] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=56, type=5 *****
Xchan_mux_1level_tapbuf_size2[28]->out_loadlvl[0]_out mux_1level_tapbuf_size2[28]->out mux_1level_tapbuf_size2[28]->out_loadlvl[0]_out mux_1level_tapbuf_size2[28]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[109]_no0 mux_1level_tapbuf_size2[28]->out_loadlvl[0]_out mux_1level_tapbuf_size2[28]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[110]_no0 mux_1level_tapbuf_size2[28]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[28]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[111]_no0 mux_1level_tapbuf_size2[28]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[28]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to28] 
+          param='sum_leakage_power_sb_mux[0to27]+leakage_sb_mux[0][0]_rrnode[447]'
.meas tran sum_energy_per_cycle_sb_mux[0to28] 
+          param='sum_energy_per_cycle_sb_mux[0to27]+energy_per_cycle_sb_mux[0][0]_rrnode[447]'
Xmux_1level_tapbuf_size2[29] mux_1level_tapbuf_size2[29]->in[0] mux_1level_tapbuf_size2[29]->in[1] mux_1level_tapbuf_size2[29]->out sram[29]->outb sram[29]->out gvdd_mux_1level_tapbuf_size2[29] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[29], level=1, select_path_id=0. *****
*****1*****
Xsram[29] sram->in sram[29]->out sram[29]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[29]->out) 0
.nodeset V(sram[29]->outb) vsp
***** Signal mux_1level_tapbuf_size2[29]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[29]->in[0] mux_1level_tapbuf_size2[29]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[29]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[29]->in[1] mux_1level_tapbuf_size2[29]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[29] gvdd_mux_1level_tapbuf_size2[29] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[449] trig v(mux_1level_tapbuf_size2[29]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[29]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[449] trig v(mux_1level_tapbuf_size2[29]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[29]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[449] when v(mux_1level_tapbuf_size2[29]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[449] trig v(mux_1level_tapbuf_size2[29]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[29]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[449] when v(mux_1level_tapbuf_size2[29]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[449] trig v(mux_1level_tapbuf_size2[29]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[29]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[29]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[29]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[449] param='mux_1level_tapbuf_size2[29]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[29]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[29]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[29]_energy_per_cycle param='mux_1level_tapbuf_size2[29]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[449]  param='mux_1level_tapbuf_size2[29]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[449]  param='dynamic_power_sb_mux[0][0]_rrnode[449]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[449] avg p(Vgvdd_mux_1level_tapbuf_size2[29]) from='start_rise_sb_mux[0][0]_rrnode[449]' to='start_rise_sb_mux[0][0]_rrnode[449]+switch_rise_sb_mux[0][0]_rrnode[449]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[449] avg p(Vgvdd_mux_1level_tapbuf_size2[29]) from='start_fall_sb_mux[0][0]_rrnode[449]' to='start_fall_sb_mux[0][0]_rrnode[449]+switch_fall_sb_mux[0][0]_rrnode[449]'
.meas tran sum_leakage_power_mux[0to29] 
+          param='sum_leakage_power_mux[0to28]+leakage_sb_mux[0][0]_rrnode[449]'
.meas tran sum_energy_per_cycle_mux[0to29] 
+          param='sum_energy_per_cycle_mux[0to28]+energy_per_cycle_sb_mux[0][0]_rrnode[449]'
***** Load for rr_node[449] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=58, type=5 *****
Xchan_mux_1level_tapbuf_size2[29]->out_loadlvl[0]_out mux_1level_tapbuf_size2[29]->out mux_1level_tapbuf_size2[29]->out_loadlvl[0]_out mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[112]_no0 mux_1level_tapbuf_size2[29]->out_loadlvl[0]_out mux_1level_tapbuf_size2[29]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[113]_no0 mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[114]_no0 mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[115]_no0 mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[116]_no0 mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to29] 
+          param='sum_leakage_power_sb_mux[0to28]+leakage_sb_mux[0][0]_rrnode[449]'
.meas tran sum_energy_per_cycle_sb_mux[0to29] 
+          param='sum_energy_per_cycle_sb_mux[0to28]+energy_per_cycle_sb_mux[0][0]_rrnode[449]'
Xmux_1level_tapbuf_size2[30] mux_1level_tapbuf_size2[30]->in[0] mux_1level_tapbuf_size2[30]->in[1] mux_1level_tapbuf_size2[30]->out sram[30]->outb sram[30]->out gvdd_mux_1level_tapbuf_size2[30] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[30], level=1, select_path_id=0. *****
*****1*****
Xsram[30] sram->in sram[30]->out sram[30]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[30]->out) 0
.nodeset V(sram[30]->outb) vsp
***** Signal mux_1level_tapbuf_size2[30]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[30]->in[0] mux_1level_tapbuf_size2[30]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[30]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[30]->in[1] mux_1level_tapbuf_size2[30]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[30] gvdd_mux_1level_tapbuf_size2[30] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[451] trig v(mux_1level_tapbuf_size2[30]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[30]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[451] trig v(mux_1level_tapbuf_size2[30]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[30]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[451] when v(mux_1level_tapbuf_size2[30]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[451] trig v(mux_1level_tapbuf_size2[30]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[30]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[451] when v(mux_1level_tapbuf_size2[30]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[451] trig v(mux_1level_tapbuf_size2[30]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[30]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[30]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[30]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[451] param='mux_1level_tapbuf_size2[30]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[30]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[30]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[30]_energy_per_cycle param='mux_1level_tapbuf_size2[30]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[451]  param='mux_1level_tapbuf_size2[30]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[451]  param='dynamic_power_sb_mux[0][0]_rrnode[451]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[451] avg p(Vgvdd_mux_1level_tapbuf_size2[30]) from='start_rise_sb_mux[0][0]_rrnode[451]' to='start_rise_sb_mux[0][0]_rrnode[451]+switch_rise_sb_mux[0][0]_rrnode[451]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[451] avg p(Vgvdd_mux_1level_tapbuf_size2[30]) from='start_fall_sb_mux[0][0]_rrnode[451]' to='start_fall_sb_mux[0][0]_rrnode[451]+switch_fall_sb_mux[0][0]_rrnode[451]'
.meas tran sum_leakage_power_mux[0to30] 
+          param='sum_leakage_power_mux[0to29]+leakage_sb_mux[0][0]_rrnode[451]'
.meas tran sum_energy_per_cycle_mux[0to30] 
+          param='sum_energy_per_cycle_mux[0to29]+energy_per_cycle_sb_mux[0][0]_rrnode[451]'
***** Load for rr_node[451] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=60, type=5 *****
Xchan_mux_1level_tapbuf_size2[30]->out_loadlvl[0]_out mux_1level_tapbuf_size2[30]->out mux_1level_tapbuf_size2[30]->out_loadlvl[0]_out mux_1level_tapbuf_size2[30]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[117]_no0 mux_1level_tapbuf_size2[30]->out_loadlvl[0]_out mux_1level_tapbuf_size2[30]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[118]_no0 mux_1level_tapbuf_size2[30]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[30]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[119]_no0 mux_1level_tapbuf_size2[30]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[30]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to30] 
+          param='sum_leakage_power_sb_mux[0to29]+leakage_sb_mux[0][0]_rrnode[451]'
.meas tran sum_energy_per_cycle_sb_mux[0to30] 
+          param='sum_energy_per_cycle_sb_mux[0to29]+energy_per_cycle_sb_mux[0][0]_rrnode[451]'
Xmux_1level_tapbuf_size2[31] mux_1level_tapbuf_size2[31]->in[0] mux_1level_tapbuf_size2[31]->in[1] mux_1level_tapbuf_size2[31]->out sram[31]->outb sram[31]->out gvdd_mux_1level_tapbuf_size2[31] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[31], level=1, select_path_id=0. *****
*****1*****
Xsram[31] sram->in sram[31]->out sram[31]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[31]->out) 0
.nodeset V(sram[31]->outb) vsp
***** Signal mux_1level_tapbuf_size2[31]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[31]->in[0] mux_1level_tapbuf_size2[31]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[31]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[31]->in[1] mux_1level_tapbuf_size2[31]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[31] gvdd_mux_1level_tapbuf_size2[31] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[453] trig v(mux_1level_tapbuf_size2[31]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[31]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[453] trig v(mux_1level_tapbuf_size2[31]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[31]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[453] when v(mux_1level_tapbuf_size2[31]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[453] trig v(mux_1level_tapbuf_size2[31]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[31]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[453] when v(mux_1level_tapbuf_size2[31]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[453] trig v(mux_1level_tapbuf_size2[31]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[31]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[31]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[31]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[453] param='mux_1level_tapbuf_size2[31]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[31]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[31]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[31]_energy_per_cycle param='mux_1level_tapbuf_size2[31]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[453]  param='mux_1level_tapbuf_size2[31]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[453]  param='dynamic_power_sb_mux[0][0]_rrnode[453]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[453] avg p(Vgvdd_mux_1level_tapbuf_size2[31]) from='start_rise_sb_mux[0][0]_rrnode[453]' to='start_rise_sb_mux[0][0]_rrnode[453]+switch_rise_sb_mux[0][0]_rrnode[453]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[453] avg p(Vgvdd_mux_1level_tapbuf_size2[31]) from='start_fall_sb_mux[0][0]_rrnode[453]' to='start_fall_sb_mux[0][0]_rrnode[453]+switch_fall_sb_mux[0][0]_rrnode[453]'
.meas tran sum_leakage_power_mux[0to31] 
+          param='sum_leakage_power_mux[0to30]+leakage_sb_mux[0][0]_rrnode[453]'
.meas tran sum_energy_per_cycle_mux[0to31] 
+          param='sum_energy_per_cycle_mux[0to30]+energy_per_cycle_sb_mux[0][0]_rrnode[453]'
***** Load for rr_node[453] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=62, type=5 *****
Xchan_mux_1level_tapbuf_size2[31]->out_loadlvl[0]_out mux_1level_tapbuf_size2[31]->out mux_1level_tapbuf_size2[31]->out_loadlvl[0]_out mux_1level_tapbuf_size2[31]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[120]_no0 mux_1level_tapbuf_size2[31]->out_loadlvl[0]_out mux_1level_tapbuf_size2[31]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[121]_no0 mux_1level_tapbuf_size2[31]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[31]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[122]_no0 mux_1level_tapbuf_size2[31]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[31]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[123]_no0 mux_1level_tapbuf_size2[31]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[31]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to31] 
+          param='sum_leakage_power_sb_mux[0to30]+leakage_sb_mux[0][0]_rrnode[453]'
.meas tran sum_energy_per_cycle_sb_mux[0to31] 
+          param='sum_energy_per_cycle_sb_mux[0to30]+energy_per_cycle_sb_mux[0][0]_rrnode[453]'
Xmux_1level_tapbuf_size2[32] mux_1level_tapbuf_size2[32]->in[0] mux_1level_tapbuf_size2[32]->in[1] mux_1level_tapbuf_size2[32]->out sram[32]->outb sram[32]->out gvdd_mux_1level_tapbuf_size2[32] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[32], level=1, select_path_id=0. *****
*****1*****
Xsram[32] sram->in sram[32]->out sram[32]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[32]->out) 0
.nodeset V(sram[32]->outb) vsp
***** Signal mux_1level_tapbuf_size2[32]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[32]->in[0] mux_1level_tapbuf_size2[32]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[32]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[32]->in[1] mux_1level_tapbuf_size2[32]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[32] gvdd_mux_1level_tapbuf_size2[32] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[455] trig v(mux_1level_tapbuf_size2[32]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[32]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[455] trig v(mux_1level_tapbuf_size2[32]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[32]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[455] when v(mux_1level_tapbuf_size2[32]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[455] trig v(mux_1level_tapbuf_size2[32]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[32]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[455] when v(mux_1level_tapbuf_size2[32]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[455] trig v(mux_1level_tapbuf_size2[32]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[32]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[32]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[32]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[455] param='mux_1level_tapbuf_size2[32]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[32]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[32]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[32]_energy_per_cycle param='mux_1level_tapbuf_size2[32]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[455]  param='mux_1level_tapbuf_size2[32]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[455]  param='dynamic_power_sb_mux[0][0]_rrnode[455]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[455] avg p(Vgvdd_mux_1level_tapbuf_size2[32]) from='start_rise_sb_mux[0][0]_rrnode[455]' to='start_rise_sb_mux[0][0]_rrnode[455]+switch_rise_sb_mux[0][0]_rrnode[455]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[455] avg p(Vgvdd_mux_1level_tapbuf_size2[32]) from='start_fall_sb_mux[0][0]_rrnode[455]' to='start_fall_sb_mux[0][0]_rrnode[455]+switch_fall_sb_mux[0][0]_rrnode[455]'
.meas tran sum_leakage_power_mux[0to32] 
+          param='sum_leakage_power_mux[0to31]+leakage_sb_mux[0][0]_rrnode[455]'
.meas tran sum_energy_per_cycle_mux[0to32] 
+          param='sum_energy_per_cycle_mux[0to31]+energy_per_cycle_sb_mux[0][0]_rrnode[455]'
***** Load for rr_node[455] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=64, type=5 *****
Xchan_mux_1level_tapbuf_size2[32]->out_loadlvl[0]_out mux_1level_tapbuf_size2[32]->out mux_1level_tapbuf_size2[32]->out_loadlvl[0]_out mux_1level_tapbuf_size2[32]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[124]_no0 mux_1level_tapbuf_size2[32]->out_loadlvl[0]_out mux_1level_tapbuf_size2[32]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[125]_no0 mux_1level_tapbuf_size2[32]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[32]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[126]_no0 mux_1level_tapbuf_size2[32]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[32]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[127]_no0 mux_1level_tapbuf_size2[32]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[32]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to32] 
+          param='sum_leakage_power_sb_mux[0to31]+leakage_sb_mux[0][0]_rrnode[455]'
.meas tran sum_energy_per_cycle_sb_mux[0to32] 
+          param='sum_energy_per_cycle_sb_mux[0to31]+energy_per_cycle_sb_mux[0][0]_rrnode[455]'
Xmux_1level_tapbuf_size2[33] mux_1level_tapbuf_size2[33]->in[0] mux_1level_tapbuf_size2[33]->in[1] mux_1level_tapbuf_size2[33]->out sram[33]->outb sram[33]->out gvdd_mux_1level_tapbuf_size2[33] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[33], level=1, select_path_id=0. *****
*****1*****
Xsram[33] sram->in sram[33]->out sram[33]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[33]->out) 0
.nodeset V(sram[33]->outb) vsp
***** Signal mux_1level_tapbuf_size2[33]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[33]->in[0] mux_1level_tapbuf_size2[33]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[33]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[33]->in[1] mux_1level_tapbuf_size2[33]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[33] gvdd_mux_1level_tapbuf_size2[33] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[457] trig v(mux_1level_tapbuf_size2[33]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[33]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[457] trig v(mux_1level_tapbuf_size2[33]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[33]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[457] when v(mux_1level_tapbuf_size2[33]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[457] trig v(mux_1level_tapbuf_size2[33]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[33]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[457] when v(mux_1level_tapbuf_size2[33]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[457] trig v(mux_1level_tapbuf_size2[33]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[33]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[33]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[33]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[457] param='mux_1level_tapbuf_size2[33]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[33]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[33]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[33]_energy_per_cycle param='mux_1level_tapbuf_size2[33]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[457]  param='mux_1level_tapbuf_size2[33]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[457]  param='dynamic_power_sb_mux[0][0]_rrnode[457]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[457] avg p(Vgvdd_mux_1level_tapbuf_size2[33]) from='start_rise_sb_mux[0][0]_rrnode[457]' to='start_rise_sb_mux[0][0]_rrnode[457]+switch_rise_sb_mux[0][0]_rrnode[457]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[457] avg p(Vgvdd_mux_1level_tapbuf_size2[33]) from='start_fall_sb_mux[0][0]_rrnode[457]' to='start_fall_sb_mux[0][0]_rrnode[457]+switch_fall_sb_mux[0][0]_rrnode[457]'
.meas tran sum_leakage_power_mux[0to33] 
+          param='sum_leakage_power_mux[0to32]+leakage_sb_mux[0][0]_rrnode[457]'
.meas tran sum_energy_per_cycle_mux[0to33] 
+          param='sum_energy_per_cycle_mux[0to32]+energy_per_cycle_sb_mux[0][0]_rrnode[457]'
***** Load for rr_node[457] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=66, type=5 *****
Xchan_mux_1level_tapbuf_size2[33]->out_loadlvl[0]_out mux_1level_tapbuf_size2[33]->out mux_1level_tapbuf_size2[33]->out_loadlvl[0]_out mux_1level_tapbuf_size2[33]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[128]_no0 mux_1level_tapbuf_size2[33]->out_loadlvl[0]_out mux_1level_tapbuf_size2[33]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[129]_no0 mux_1level_tapbuf_size2[33]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[33]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[130]_no0 mux_1level_tapbuf_size2[33]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[33]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[131]_no0 mux_1level_tapbuf_size2[33]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[33]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to33] 
+          param='sum_leakage_power_sb_mux[0to32]+leakage_sb_mux[0][0]_rrnode[457]'
.meas tran sum_energy_per_cycle_sb_mux[0to33] 
+          param='sum_energy_per_cycle_sb_mux[0to32]+energy_per_cycle_sb_mux[0][0]_rrnode[457]'
Xmux_1level_tapbuf_size2[34] mux_1level_tapbuf_size2[34]->in[0] mux_1level_tapbuf_size2[34]->in[1] mux_1level_tapbuf_size2[34]->out sram[34]->outb sram[34]->out gvdd_mux_1level_tapbuf_size2[34] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[34], level=1, select_path_id=0. *****
*****1*****
Xsram[34] sram->in sram[34]->out sram[34]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[34]->out) 0
.nodeset V(sram[34]->outb) vsp
***** Signal mux_1level_tapbuf_size2[34]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[34]->in[0] mux_1level_tapbuf_size2[34]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[34]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[34]->in[1] mux_1level_tapbuf_size2[34]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[34] gvdd_mux_1level_tapbuf_size2[34] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[459] trig v(mux_1level_tapbuf_size2[34]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[34]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[459] trig v(mux_1level_tapbuf_size2[34]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[34]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[459] when v(mux_1level_tapbuf_size2[34]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[459] trig v(mux_1level_tapbuf_size2[34]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[34]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[459] when v(mux_1level_tapbuf_size2[34]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[459] trig v(mux_1level_tapbuf_size2[34]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[34]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[34]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[34]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[459] param='mux_1level_tapbuf_size2[34]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[34]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[34]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[34]_energy_per_cycle param='mux_1level_tapbuf_size2[34]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[459]  param='mux_1level_tapbuf_size2[34]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[459]  param='dynamic_power_sb_mux[0][0]_rrnode[459]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[459] avg p(Vgvdd_mux_1level_tapbuf_size2[34]) from='start_rise_sb_mux[0][0]_rrnode[459]' to='start_rise_sb_mux[0][0]_rrnode[459]+switch_rise_sb_mux[0][0]_rrnode[459]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[459] avg p(Vgvdd_mux_1level_tapbuf_size2[34]) from='start_fall_sb_mux[0][0]_rrnode[459]' to='start_fall_sb_mux[0][0]_rrnode[459]+switch_fall_sb_mux[0][0]_rrnode[459]'
.meas tran sum_leakage_power_mux[0to34] 
+          param='sum_leakage_power_mux[0to33]+leakage_sb_mux[0][0]_rrnode[459]'
.meas tran sum_energy_per_cycle_mux[0to34] 
+          param='sum_energy_per_cycle_mux[0to33]+energy_per_cycle_sb_mux[0][0]_rrnode[459]'
***** Load for rr_node[459] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=68, type=5 *****
Xchan_mux_1level_tapbuf_size2[34]->out_loadlvl[0]_out mux_1level_tapbuf_size2[34]->out mux_1level_tapbuf_size2[34]->out_loadlvl[0]_out mux_1level_tapbuf_size2[34]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[132]_no0 mux_1level_tapbuf_size2[34]->out_loadlvl[0]_out mux_1level_tapbuf_size2[34]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[133]_no0 mux_1level_tapbuf_size2[34]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[34]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[134]_no0 mux_1level_tapbuf_size2[34]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[34]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[135]_no0 mux_1level_tapbuf_size2[34]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[34]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to34] 
+          param='sum_leakage_power_sb_mux[0to33]+leakage_sb_mux[0][0]_rrnode[459]'
.meas tran sum_energy_per_cycle_sb_mux[0to34] 
+          param='sum_energy_per_cycle_sb_mux[0to33]+energy_per_cycle_sb_mux[0][0]_rrnode[459]'
Xmux_1level_tapbuf_size2[35] mux_1level_tapbuf_size2[35]->in[0] mux_1level_tapbuf_size2[35]->in[1] mux_1level_tapbuf_size2[35]->out sram[35]->outb sram[35]->out gvdd_mux_1level_tapbuf_size2[35] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[35], level=1, select_path_id=0. *****
*****1*****
Xsram[35] sram->in sram[35]->out sram[35]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[35]->out) 0
.nodeset V(sram[35]->outb) vsp
***** Signal mux_1level_tapbuf_size2[35]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[35]->in[0] mux_1level_tapbuf_size2[35]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[35]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[35]->in[1] mux_1level_tapbuf_size2[35]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[35] gvdd_mux_1level_tapbuf_size2[35] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[461] trig v(mux_1level_tapbuf_size2[35]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[35]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[461] trig v(mux_1level_tapbuf_size2[35]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[35]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[461] when v(mux_1level_tapbuf_size2[35]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[461] trig v(mux_1level_tapbuf_size2[35]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[35]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[461] when v(mux_1level_tapbuf_size2[35]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[461] trig v(mux_1level_tapbuf_size2[35]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[35]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[35]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[35]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[461] param='mux_1level_tapbuf_size2[35]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[35]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[35]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[35]_energy_per_cycle param='mux_1level_tapbuf_size2[35]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[461]  param='mux_1level_tapbuf_size2[35]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[461]  param='dynamic_power_sb_mux[0][0]_rrnode[461]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[461] avg p(Vgvdd_mux_1level_tapbuf_size2[35]) from='start_rise_sb_mux[0][0]_rrnode[461]' to='start_rise_sb_mux[0][0]_rrnode[461]+switch_rise_sb_mux[0][0]_rrnode[461]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[461] avg p(Vgvdd_mux_1level_tapbuf_size2[35]) from='start_fall_sb_mux[0][0]_rrnode[461]' to='start_fall_sb_mux[0][0]_rrnode[461]+switch_fall_sb_mux[0][0]_rrnode[461]'
.meas tran sum_leakage_power_mux[0to35] 
+          param='sum_leakage_power_mux[0to34]+leakage_sb_mux[0][0]_rrnode[461]'
.meas tran sum_energy_per_cycle_mux[0to35] 
+          param='sum_energy_per_cycle_mux[0to34]+energy_per_cycle_sb_mux[0][0]_rrnode[461]'
***** Load for rr_node[461] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=70, type=5 *****
Xchan_mux_1level_tapbuf_size2[35]->out_loadlvl[0]_out mux_1level_tapbuf_size2[35]->out mux_1level_tapbuf_size2[35]->out_loadlvl[0]_out mux_1level_tapbuf_size2[35]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[136]_no0 mux_1level_tapbuf_size2[35]->out_loadlvl[0]_out mux_1level_tapbuf_size2[35]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[137]_no0 mux_1level_tapbuf_size2[35]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[35]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[138]_no0 mux_1level_tapbuf_size2[35]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[35]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to35] 
+          param='sum_leakage_power_sb_mux[0to34]+leakage_sb_mux[0][0]_rrnode[461]'
.meas tran sum_energy_per_cycle_sb_mux[0to35] 
+          param='sum_energy_per_cycle_sb_mux[0to34]+energy_per_cycle_sb_mux[0][0]_rrnode[461]'
Xmux_1level_tapbuf_size2[36] mux_1level_tapbuf_size2[36]->in[0] mux_1level_tapbuf_size2[36]->in[1] mux_1level_tapbuf_size2[36]->out sram[36]->outb sram[36]->out gvdd_mux_1level_tapbuf_size2[36] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[36], level=1, select_path_id=0. *****
*****1*****
Xsram[36] sram->in sram[36]->out sram[36]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[36]->out) 0
.nodeset V(sram[36]->outb) vsp
***** Signal mux_1level_tapbuf_size2[36]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[36]->in[0] mux_1level_tapbuf_size2[36]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[36]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[36]->in[1] mux_1level_tapbuf_size2[36]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[36] gvdd_mux_1level_tapbuf_size2[36] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[463] trig v(mux_1level_tapbuf_size2[36]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[36]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[463] trig v(mux_1level_tapbuf_size2[36]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[36]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[463] when v(mux_1level_tapbuf_size2[36]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[463] trig v(mux_1level_tapbuf_size2[36]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[36]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[463] when v(mux_1level_tapbuf_size2[36]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[463] trig v(mux_1level_tapbuf_size2[36]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[36]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[36]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[36]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[463] param='mux_1level_tapbuf_size2[36]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[36]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[36]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[36]_energy_per_cycle param='mux_1level_tapbuf_size2[36]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[463]  param='mux_1level_tapbuf_size2[36]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[463]  param='dynamic_power_sb_mux[0][0]_rrnode[463]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[463] avg p(Vgvdd_mux_1level_tapbuf_size2[36]) from='start_rise_sb_mux[0][0]_rrnode[463]' to='start_rise_sb_mux[0][0]_rrnode[463]+switch_rise_sb_mux[0][0]_rrnode[463]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[463] avg p(Vgvdd_mux_1level_tapbuf_size2[36]) from='start_fall_sb_mux[0][0]_rrnode[463]' to='start_fall_sb_mux[0][0]_rrnode[463]+switch_fall_sb_mux[0][0]_rrnode[463]'
.meas tran sum_leakage_power_mux[0to36] 
+          param='sum_leakage_power_mux[0to35]+leakage_sb_mux[0][0]_rrnode[463]'
.meas tran sum_energy_per_cycle_mux[0to36] 
+          param='sum_energy_per_cycle_mux[0to35]+energy_per_cycle_sb_mux[0][0]_rrnode[463]'
***** Load for rr_node[463] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=72, type=5 *****
Xchan_mux_1level_tapbuf_size2[36]->out_loadlvl[0]_out mux_1level_tapbuf_size2[36]->out mux_1level_tapbuf_size2[36]->out_loadlvl[0]_out mux_1level_tapbuf_size2[36]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[139]_no0 mux_1level_tapbuf_size2[36]->out_loadlvl[0]_out mux_1level_tapbuf_size2[36]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[140]_no0 mux_1level_tapbuf_size2[36]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[36]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[141]_no0 mux_1level_tapbuf_size2[36]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[36]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[142]_no0 mux_1level_tapbuf_size2[36]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[36]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[143]_no0 mux_1level_tapbuf_size2[36]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[36]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to36] 
+          param='sum_leakage_power_sb_mux[0to35]+leakage_sb_mux[0][0]_rrnode[463]'
.meas tran sum_energy_per_cycle_sb_mux[0to36] 
+          param='sum_energy_per_cycle_sb_mux[0to35]+energy_per_cycle_sb_mux[0][0]_rrnode[463]'
Xmux_1level_tapbuf_size2[37] mux_1level_tapbuf_size2[37]->in[0] mux_1level_tapbuf_size2[37]->in[1] mux_1level_tapbuf_size2[37]->out sram[37]->outb sram[37]->out gvdd_mux_1level_tapbuf_size2[37] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[37], level=1, select_path_id=0. *****
*****1*****
Xsram[37] sram->in sram[37]->out sram[37]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[37]->out) 0
.nodeset V(sram[37]->outb) vsp
***** Signal mux_1level_tapbuf_size2[37]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[37]->in[0] mux_1level_tapbuf_size2[37]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[37]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[37]->in[1] mux_1level_tapbuf_size2[37]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[37] gvdd_mux_1level_tapbuf_size2[37] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[465] trig v(mux_1level_tapbuf_size2[37]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[37]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[465] trig v(mux_1level_tapbuf_size2[37]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[37]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[465] when v(mux_1level_tapbuf_size2[37]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[465] trig v(mux_1level_tapbuf_size2[37]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[37]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[465] when v(mux_1level_tapbuf_size2[37]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[465] trig v(mux_1level_tapbuf_size2[37]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[37]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[37]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[37]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[465] param='mux_1level_tapbuf_size2[37]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[37]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[37]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[37]_energy_per_cycle param='mux_1level_tapbuf_size2[37]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[465]  param='mux_1level_tapbuf_size2[37]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[465]  param='dynamic_power_sb_mux[0][0]_rrnode[465]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[465] avg p(Vgvdd_mux_1level_tapbuf_size2[37]) from='start_rise_sb_mux[0][0]_rrnode[465]' to='start_rise_sb_mux[0][0]_rrnode[465]+switch_rise_sb_mux[0][0]_rrnode[465]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[465] avg p(Vgvdd_mux_1level_tapbuf_size2[37]) from='start_fall_sb_mux[0][0]_rrnode[465]' to='start_fall_sb_mux[0][0]_rrnode[465]+switch_fall_sb_mux[0][0]_rrnode[465]'
.meas tran sum_leakage_power_mux[0to37] 
+          param='sum_leakage_power_mux[0to36]+leakage_sb_mux[0][0]_rrnode[465]'
.meas tran sum_energy_per_cycle_mux[0to37] 
+          param='sum_energy_per_cycle_mux[0to36]+energy_per_cycle_sb_mux[0][0]_rrnode[465]'
***** Load for rr_node[465] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=74, type=5 *****
Xchan_mux_1level_tapbuf_size2[37]->out_loadlvl[0]_out mux_1level_tapbuf_size2[37]->out mux_1level_tapbuf_size2[37]->out_loadlvl[0]_out mux_1level_tapbuf_size2[37]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[144]_no0 mux_1level_tapbuf_size2[37]->out_loadlvl[0]_out mux_1level_tapbuf_size2[37]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[145]_no0 mux_1level_tapbuf_size2[37]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[37]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[146]_no0 mux_1level_tapbuf_size2[37]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[37]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[147]_no0 mux_1level_tapbuf_size2[37]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[37]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to37] 
+          param='sum_leakage_power_sb_mux[0to36]+leakage_sb_mux[0][0]_rrnode[465]'
.meas tran sum_energy_per_cycle_sb_mux[0to37] 
+          param='sum_energy_per_cycle_sb_mux[0to36]+energy_per_cycle_sb_mux[0][0]_rrnode[465]'
Xmux_1level_tapbuf_size2[38] mux_1level_tapbuf_size2[38]->in[0] mux_1level_tapbuf_size2[38]->in[1] mux_1level_tapbuf_size2[38]->out sram[38]->out sram[38]->outb gvdd_mux_1level_tapbuf_size2[38] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[38], level=1, select_path_id=1. *****
*****0*****
Xsram[38] sram->in sram[38]->out sram[38]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[38]->out) 0
.nodeset V(sram[38]->outb) vsp
***** Signal mux_1level_tapbuf_size2[38]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[38]->in[0] mux_1level_tapbuf_size2[38]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[38]->in[1] density = 0.2026, probability=0.4982.*****
Vmux_1level_tapbuf_size2[38]->in[1] mux_1level_tapbuf_size2[38]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgvdd_mux_1level_tapbuf_size2[38] gvdd_mux_1level_tapbuf_size2[38] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[467] trig v(mux_1level_tapbuf_size2[38]->in[1]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[38]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[467] trig v(mux_1level_tapbuf_size2[38]->in[1]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[38]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[467] when v(mux_1level_tapbuf_size2[38]->in[1])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[467] trig v(mux_1level_tapbuf_size2[38]->in[1]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[38]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[467] when v(mux_1level_tapbuf_size2[38]->in[1])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[467] trig v(mux_1level_tapbuf_size2[38]->in[1]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[38]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[38]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[38]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[467] param='mux_1level_tapbuf_size2[38]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[38]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[38]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[38]_energy_per_cycle param='mux_1level_tapbuf_size2[38]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[467]  param='mux_1level_tapbuf_size2[38]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[467]  param='dynamic_power_sb_mux[0][0]_rrnode[467]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[467] avg p(Vgvdd_mux_1level_tapbuf_size2[38]) from='start_rise_sb_mux[0][0]_rrnode[467]' to='start_rise_sb_mux[0][0]_rrnode[467]+switch_rise_sb_mux[0][0]_rrnode[467]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[467] avg p(Vgvdd_mux_1level_tapbuf_size2[38]) from='start_fall_sb_mux[0][0]_rrnode[467]' to='start_fall_sb_mux[0][0]_rrnode[467]+switch_fall_sb_mux[0][0]_rrnode[467]'
.meas tran sum_leakage_power_mux[0to38] 
+          param='sum_leakage_power_mux[0to37]+leakage_sb_mux[0][0]_rrnode[467]'
.meas tran sum_energy_per_cycle_mux[0to38] 
+          param='sum_energy_per_cycle_mux[0to37]+energy_per_cycle_sb_mux[0][0]_rrnode[467]'
***** Load for rr_node[467] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=76, type=5 *****
Xchan_mux_1level_tapbuf_size2[38]->out_loadlvl[0]_out mux_1level_tapbuf_size2[38]->out mux_1level_tapbuf_size2[38]->out_loadlvl[0]_out mux_1level_tapbuf_size2[38]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[148]_no0 mux_1level_tapbuf_size2[38]->out_loadlvl[0]_out mux_1level_tapbuf_size2[38]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[149]_no0 mux_1level_tapbuf_size2[38]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[38]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[150]_no0 mux_1level_tapbuf_size2[38]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[38]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to38] 
+          param='sum_leakage_power_sb_mux[0to37]+leakage_sb_mux[0][0]_rrnode[467]'
.meas tran sum_energy_per_cycle_sb_mux[0to38] 
+          param='sum_energy_per_cycle_sb_mux[0to37]+energy_per_cycle_sb_mux[0][0]_rrnode[467]'
Xmux_1level_tapbuf_size2[39] mux_1level_tapbuf_size2[39]->in[0] mux_1level_tapbuf_size2[39]->in[1] mux_1level_tapbuf_size2[39]->out sram[39]->outb sram[39]->out gvdd_mux_1level_tapbuf_size2[39] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[39], level=1, select_path_id=0. *****
*****1*****
Xsram[39] sram->in sram[39]->out sram[39]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[39]->out) 0
.nodeset V(sram[39]->outb) vsp
***** Signal mux_1level_tapbuf_size2[39]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[39]->in[0] mux_1level_tapbuf_size2[39]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[39]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[39]->in[1] mux_1level_tapbuf_size2[39]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[39] gvdd_mux_1level_tapbuf_size2[39] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[469] trig v(mux_1level_tapbuf_size2[39]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[39]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[469] trig v(mux_1level_tapbuf_size2[39]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[39]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[469] when v(mux_1level_tapbuf_size2[39]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[469] trig v(mux_1level_tapbuf_size2[39]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[39]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[469] when v(mux_1level_tapbuf_size2[39]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[469] trig v(mux_1level_tapbuf_size2[39]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[39]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[39]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[39]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[469] param='mux_1level_tapbuf_size2[39]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[39]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[39]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[39]_energy_per_cycle param='mux_1level_tapbuf_size2[39]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[469]  param='mux_1level_tapbuf_size2[39]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[469]  param='dynamic_power_sb_mux[0][0]_rrnode[469]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[469] avg p(Vgvdd_mux_1level_tapbuf_size2[39]) from='start_rise_sb_mux[0][0]_rrnode[469]' to='start_rise_sb_mux[0][0]_rrnode[469]+switch_rise_sb_mux[0][0]_rrnode[469]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[469] avg p(Vgvdd_mux_1level_tapbuf_size2[39]) from='start_fall_sb_mux[0][0]_rrnode[469]' to='start_fall_sb_mux[0][0]_rrnode[469]+switch_fall_sb_mux[0][0]_rrnode[469]'
.meas tran sum_leakage_power_mux[0to39] 
+          param='sum_leakage_power_mux[0to38]+leakage_sb_mux[0][0]_rrnode[469]'
.meas tran sum_energy_per_cycle_mux[0to39] 
+          param='sum_energy_per_cycle_mux[0to38]+energy_per_cycle_sb_mux[0][0]_rrnode[469]'
***** Load for rr_node[469] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=78, type=5 *****
Xchan_mux_1level_tapbuf_size2[39]->out_loadlvl[0]_out mux_1level_tapbuf_size2[39]->out mux_1level_tapbuf_size2[39]->out_loadlvl[0]_out mux_1level_tapbuf_size2[39]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[151]_no0 mux_1level_tapbuf_size2[39]->out_loadlvl[0]_out mux_1level_tapbuf_size2[39]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[152]_no0 mux_1level_tapbuf_size2[39]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[39]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[153]_no0 mux_1level_tapbuf_size2[39]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[39]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[154]_no0 mux_1level_tapbuf_size2[39]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[39]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to39] 
+          param='sum_leakage_power_sb_mux[0to38]+leakage_sb_mux[0][0]_rrnode[469]'
.meas tran sum_energy_per_cycle_sb_mux[0to39] 
+          param='sum_energy_per_cycle_sb_mux[0to38]+energy_per_cycle_sb_mux[0][0]_rrnode[469]'
Xmux_1level_tapbuf_size2[40] mux_1level_tapbuf_size2[40]->in[0] mux_1level_tapbuf_size2[40]->in[1] mux_1level_tapbuf_size2[40]->out sram[40]->outb sram[40]->out gvdd_mux_1level_tapbuf_size2[40] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[40], level=1, select_path_id=0. *****
*****1*****
Xsram[40] sram->in sram[40]->out sram[40]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[40]->out) 0
.nodeset V(sram[40]->outb) vsp
***** Signal mux_1level_tapbuf_size2[40]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[40]->in[0] mux_1level_tapbuf_size2[40]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[40]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[40]->in[1] mux_1level_tapbuf_size2[40]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[40] gvdd_mux_1level_tapbuf_size2[40] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[471] trig v(mux_1level_tapbuf_size2[40]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[40]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[471] trig v(mux_1level_tapbuf_size2[40]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[40]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[471] when v(mux_1level_tapbuf_size2[40]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[471] trig v(mux_1level_tapbuf_size2[40]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[40]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[471] when v(mux_1level_tapbuf_size2[40]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[471] trig v(mux_1level_tapbuf_size2[40]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[40]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[40]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[40]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[471] param='mux_1level_tapbuf_size2[40]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[40]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[40]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[40]_energy_per_cycle param='mux_1level_tapbuf_size2[40]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[471]  param='mux_1level_tapbuf_size2[40]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[471]  param='dynamic_power_sb_mux[0][0]_rrnode[471]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[471] avg p(Vgvdd_mux_1level_tapbuf_size2[40]) from='start_rise_sb_mux[0][0]_rrnode[471]' to='start_rise_sb_mux[0][0]_rrnode[471]+switch_rise_sb_mux[0][0]_rrnode[471]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[471] avg p(Vgvdd_mux_1level_tapbuf_size2[40]) from='start_fall_sb_mux[0][0]_rrnode[471]' to='start_fall_sb_mux[0][0]_rrnode[471]+switch_fall_sb_mux[0][0]_rrnode[471]'
.meas tran sum_leakage_power_mux[0to40] 
+          param='sum_leakage_power_mux[0to39]+leakage_sb_mux[0][0]_rrnode[471]'
.meas tran sum_energy_per_cycle_mux[0to40] 
+          param='sum_energy_per_cycle_mux[0to39]+energy_per_cycle_sb_mux[0][0]_rrnode[471]'
***** Load for rr_node[471] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=80, type=5 *****
Xchan_mux_1level_tapbuf_size2[40]->out_loadlvl[0]_out mux_1level_tapbuf_size2[40]->out mux_1level_tapbuf_size2[40]->out_loadlvl[0]_out mux_1level_tapbuf_size2[40]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[155]_no0 mux_1level_tapbuf_size2[40]->out_loadlvl[0]_out mux_1level_tapbuf_size2[40]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[156]_no0 mux_1level_tapbuf_size2[40]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[40]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[157]_no0 mux_1level_tapbuf_size2[40]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[40]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[158]_no0 mux_1level_tapbuf_size2[40]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[40]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to40] 
+          param='sum_leakage_power_sb_mux[0to39]+leakage_sb_mux[0][0]_rrnode[471]'
.meas tran sum_energy_per_cycle_sb_mux[0to40] 
+          param='sum_energy_per_cycle_sb_mux[0to39]+energy_per_cycle_sb_mux[0][0]_rrnode[471]'
Xmux_1level_tapbuf_size2[41] mux_1level_tapbuf_size2[41]->in[0] mux_1level_tapbuf_size2[41]->in[1] mux_1level_tapbuf_size2[41]->out sram[41]->outb sram[41]->out gvdd_mux_1level_tapbuf_size2[41] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[41], level=1, select_path_id=0. *****
*****1*****
Xsram[41] sram->in sram[41]->out sram[41]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[41]->out) 0
.nodeset V(sram[41]->outb) vsp
***** Signal mux_1level_tapbuf_size2[41]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[41]->in[0] mux_1level_tapbuf_size2[41]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[41]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[41]->in[1] mux_1level_tapbuf_size2[41]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[41] gvdd_mux_1level_tapbuf_size2[41] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[473] trig v(mux_1level_tapbuf_size2[41]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[41]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[473] trig v(mux_1level_tapbuf_size2[41]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[41]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[473] when v(mux_1level_tapbuf_size2[41]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[473] trig v(mux_1level_tapbuf_size2[41]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[41]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[473] when v(mux_1level_tapbuf_size2[41]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[473] trig v(mux_1level_tapbuf_size2[41]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[41]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[41]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[41]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[473] param='mux_1level_tapbuf_size2[41]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[41]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[41]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[41]_energy_per_cycle param='mux_1level_tapbuf_size2[41]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[473]  param='mux_1level_tapbuf_size2[41]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[473]  param='dynamic_power_sb_mux[0][0]_rrnode[473]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[473] avg p(Vgvdd_mux_1level_tapbuf_size2[41]) from='start_rise_sb_mux[0][0]_rrnode[473]' to='start_rise_sb_mux[0][0]_rrnode[473]+switch_rise_sb_mux[0][0]_rrnode[473]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[473] avg p(Vgvdd_mux_1level_tapbuf_size2[41]) from='start_fall_sb_mux[0][0]_rrnode[473]' to='start_fall_sb_mux[0][0]_rrnode[473]+switch_fall_sb_mux[0][0]_rrnode[473]'
.meas tran sum_leakage_power_mux[0to41] 
+          param='sum_leakage_power_mux[0to40]+leakage_sb_mux[0][0]_rrnode[473]'
.meas tran sum_energy_per_cycle_mux[0to41] 
+          param='sum_energy_per_cycle_mux[0to40]+energy_per_cycle_sb_mux[0][0]_rrnode[473]'
***** Load for rr_node[473] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=82, type=5 *****
Xchan_mux_1level_tapbuf_size2[41]->out_loadlvl[0]_out mux_1level_tapbuf_size2[41]->out mux_1level_tapbuf_size2[41]->out_loadlvl[0]_out mux_1level_tapbuf_size2[41]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[159]_no0 mux_1level_tapbuf_size2[41]->out_loadlvl[0]_out mux_1level_tapbuf_size2[41]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[160]_no0 mux_1level_tapbuf_size2[41]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[41]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[161]_no0 mux_1level_tapbuf_size2[41]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[41]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[162]_no0 mux_1level_tapbuf_size2[41]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[41]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to41] 
+          param='sum_leakage_power_sb_mux[0to40]+leakage_sb_mux[0][0]_rrnode[473]'
.meas tran sum_energy_per_cycle_sb_mux[0to41] 
+          param='sum_energy_per_cycle_sb_mux[0to40]+energy_per_cycle_sb_mux[0][0]_rrnode[473]'
Xmux_1level_tapbuf_size2[42] mux_1level_tapbuf_size2[42]->in[0] mux_1level_tapbuf_size2[42]->in[1] mux_1level_tapbuf_size2[42]->out sram[42]->outb sram[42]->out gvdd_mux_1level_tapbuf_size2[42] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[42], level=1, select_path_id=0. *****
*****1*****
Xsram[42] sram->in sram[42]->out sram[42]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[42]->out) 0
.nodeset V(sram[42]->outb) vsp
***** Signal mux_1level_tapbuf_size2[42]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[42]->in[0] mux_1level_tapbuf_size2[42]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[42]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[42]->in[1] mux_1level_tapbuf_size2[42]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[42] gvdd_mux_1level_tapbuf_size2[42] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[475] trig v(mux_1level_tapbuf_size2[42]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[42]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[475] trig v(mux_1level_tapbuf_size2[42]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[42]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[475] when v(mux_1level_tapbuf_size2[42]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[475] trig v(mux_1level_tapbuf_size2[42]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[42]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[475] when v(mux_1level_tapbuf_size2[42]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[475] trig v(mux_1level_tapbuf_size2[42]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[42]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[42]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[42]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[475] param='mux_1level_tapbuf_size2[42]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[42]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[42]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[42]_energy_per_cycle param='mux_1level_tapbuf_size2[42]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[475]  param='mux_1level_tapbuf_size2[42]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[475]  param='dynamic_power_sb_mux[0][0]_rrnode[475]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[475] avg p(Vgvdd_mux_1level_tapbuf_size2[42]) from='start_rise_sb_mux[0][0]_rrnode[475]' to='start_rise_sb_mux[0][0]_rrnode[475]+switch_rise_sb_mux[0][0]_rrnode[475]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[475] avg p(Vgvdd_mux_1level_tapbuf_size2[42]) from='start_fall_sb_mux[0][0]_rrnode[475]' to='start_fall_sb_mux[0][0]_rrnode[475]+switch_fall_sb_mux[0][0]_rrnode[475]'
.meas tran sum_leakage_power_mux[0to42] 
+          param='sum_leakage_power_mux[0to41]+leakage_sb_mux[0][0]_rrnode[475]'
.meas tran sum_energy_per_cycle_mux[0to42] 
+          param='sum_energy_per_cycle_mux[0to41]+energy_per_cycle_sb_mux[0][0]_rrnode[475]'
***** Load for rr_node[475] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=84, type=5 *****
Xchan_mux_1level_tapbuf_size2[42]->out_loadlvl[0]_out mux_1level_tapbuf_size2[42]->out mux_1level_tapbuf_size2[42]->out_loadlvl[0]_out mux_1level_tapbuf_size2[42]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[163]_no0 mux_1level_tapbuf_size2[42]->out_loadlvl[0]_out mux_1level_tapbuf_size2[42]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[164]_no0 mux_1level_tapbuf_size2[42]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[42]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[165]_no0 mux_1level_tapbuf_size2[42]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[42]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[166]_no0 mux_1level_tapbuf_size2[42]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[42]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to42] 
+          param='sum_leakage_power_sb_mux[0to41]+leakage_sb_mux[0][0]_rrnode[475]'
.meas tran sum_energy_per_cycle_sb_mux[0to42] 
+          param='sum_energy_per_cycle_sb_mux[0to41]+energy_per_cycle_sb_mux[0][0]_rrnode[475]'
Xmux_1level_tapbuf_size2[43] mux_1level_tapbuf_size2[43]->in[0] mux_1level_tapbuf_size2[43]->in[1] mux_1level_tapbuf_size2[43]->out sram[43]->outb sram[43]->out gvdd_mux_1level_tapbuf_size2[43] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[43], level=1, select_path_id=0. *****
*****1*****
Xsram[43] sram->in sram[43]->out sram[43]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[43]->out) 0
.nodeset V(sram[43]->outb) vsp
***** Signal mux_1level_tapbuf_size2[43]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[43]->in[0] mux_1level_tapbuf_size2[43]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[43]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[43]->in[1] mux_1level_tapbuf_size2[43]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[43] gvdd_mux_1level_tapbuf_size2[43] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[477] trig v(mux_1level_tapbuf_size2[43]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[43]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[477] trig v(mux_1level_tapbuf_size2[43]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[43]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[477] when v(mux_1level_tapbuf_size2[43]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[477] trig v(mux_1level_tapbuf_size2[43]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[43]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[477] when v(mux_1level_tapbuf_size2[43]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[477] trig v(mux_1level_tapbuf_size2[43]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[43]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[43]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[43]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[477] param='mux_1level_tapbuf_size2[43]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[43]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[43]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[43]_energy_per_cycle param='mux_1level_tapbuf_size2[43]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[477]  param='mux_1level_tapbuf_size2[43]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[477]  param='dynamic_power_sb_mux[0][0]_rrnode[477]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[477] avg p(Vgvdd_mux_1level_tapbuf_size2[43]) from='start_rise_sb_mux[0][0]_rrnode[477]' to='start_rise_sb_mux[0][0]_rrnode[477]+switch_rise_sb_mux[0][0]_rrnode[477]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[477] avg p(Vgvdd_mux_1level_tapbuf_size2[43]) from='start_fall_sb_mux[0][0]_rrnode[477]' to='start_fall_sb_mux[0][0]_rrnode[477]+switch_fall_sb_mux[0][0]_rrnode[477]'
.meas tran sum_leakage_power_mux[0to43] 
+          param='sum_leakage_power_mux[0to42]+leakage_sb_mux[0][0]_rrnode[477]'
.meas tran sum_energy_per_cycle_mux[0to43] 
+          param='sum_energy_per_cycle_mux[0to42]+energy_per_cycle_sb_mux[0][0]_rrnode[477]'
***** Load for rr_node[477] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=86, type=5 *****
Xchan_mux_1level_tapbuf_size2[43]->out_loadlvl[0]_out mux_1level_tapbuf_size2[43]->out mux_1level_tapbuf_size2[43]->out_loadlvl[0]_out mux_1level_tapbuf_size2[43]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[167]_no0 mux_1level_tapbuf_size2[43]->out_loadlvl[0]_out mux_1level_tapbuf_size2[43]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[168]_no0 mux_1level_tapbuf_size2[43]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[43]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[169]_no0 mux_1level_tapbuf_size2[43]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[43]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[170]_no0 mux_1level_tapbuf_size2[43]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[43]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to43] 
+          param='sum_leakage_power_sb_mux[0to42]+leakage_sb_mux[0][0]_rrnode[477]'
.meas tran sum_energy_per_cycle_sb_mux[0to43] 
+          param='sum_energy_per_cycle_sb_mux[0to42]+energy_per_cycle_sb_mux[0][0]_rrnode[477]'
Xmux_1level_tapbuf_size2[44] mux_1level_tapbuf_size2[44]->in[0] mux_1level_tapbuf_size2[44]->in[1] mux_1level_tapbuf_size2[44]->out sram[44]->outb sram[44]->out gvdd_mux_1level_tapbuf_size2[44] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[44], level=1, select_path_id=0. *****
*****1*****
Xsram[44] sram->in sram[44]->out sram[44]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[44]->out) 0
.nodeset V(sram[44]->outb) vsp
***** Signal mux_1level_tapbuf_size2[44]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[44]->in[0] mux_1level_tapbuf_size2[44]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[44]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[44]->in[1] mux_1level_tapbuf_size2[44]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[44] gvdd_mux_1level_tapbuf_size2[44] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[479] trig v(mux_1level_tapbuf_size2[44]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[44]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[479] trig v(mux_1level_tapbuf_size2[44]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[44]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[479] when v(mux_1level_tapbuf_size2[44]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[479] trig v(mux_1level_tapbuf_size2[44]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[44]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[479] when v(mux_1level_tapbuf_size2[44]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[479] trig v(mux_1level_tapbuf_size2[44]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[44]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[44]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[44]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[479] param='mux_1level_tapbuf_size2[44]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[44]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[44]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[44]_energy_per_cycle param='mux_1level_tapbuf_size2[44]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[479]  param='mux_1level_tapbuf_size2[44]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[479]  param='dynamic_power_sb_mux[0][0]_rrnode[479]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[479] avg p(Vgvdd_mux_1level_tapbuf_size2[44]) from='start_rise_sb_mux[0][0]_rrnode[479]' to='start_rise_sb_mux[0][0]_rrnode[479]+switch_rise_sb_mux[0][0]_rrnode[479]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[479] avg p(Vgvdd_mux_1level_tapbuf_size2[44]) from='start_fall_sb_mux[0][0]_rrnode[479]' to='start_fall_sb_mux[0][0]_rrnode[479]+switch_fall_sb_mux[0][0]_rrnode[479]'
.meas tran sum_leakage_power_mux[0to44] 
+          param='sum_leakage_power_mux[0to43]+leakage_sb_mux[0][0]_rrnode[479]'
.meas tran sum_energy_per_cycle_mux[0to44] 
+          param='sum_energy_per_cycle_mux[0to43]+energy_per_cycle_sb_mux[0][0]_rrnode[479]'
***** Load for rr_node[479] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=88, type=5 *****
Xchan_mux_1level_tapbuf_size2[44]->out_loadlvl[0]_out mux_1level_tapbuf_size2[44]->out mux_1level_tapbuf_size2[44]->out_loadlvl[0]_out mux_1level_tapbuf_size2[44]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[171]_no0 mux_1level_tapbuf_size2[44]->out_loadlvl[0]_out mux_1level_tapbuf_size2[44]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[172]_no0 mux_1level_tapbuf_size2[44]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[44]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[173]_no0 mux_1level_tapbuf_size2[44]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[44]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[174]_no0 mux_1level_tapbuf_size2[44]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[44]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to44] 
+          param='sum_leakage_power_sb_mux[0to43]+leakage_sb_mux[0][0]_rrnode[479]'
.meas tran sum_energy_per_cycle_sb_mux[0to44] 
+          param='sum_energy_per_cycle_sb_mux[0to43]+energy_per_cycle_sb_mux[0][0]_rrnode[479]'
Xmux_1level_tapbuf_size2[45] mux_1level_tapbuf_size2[45]->in[0] mux_1level_tapbuf_size2[45]->in[1] mux_1level_tapbuf_size2[45]->out sram[45]->outb sram[45]->out gvdd_mux_1level_tapbuf_size2[45] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[45], level=1, select_path_id=0. *****
*****1*****
Xsram[45] sram->in sram[45]->out sram[45]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[45]->out) 0
.nodeset V(sram[45]->outb) vsp
***** Signal mux_1level_tapbuf_size2[45]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[45]->in[0] mux_1level_tapbuf_size2[45]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[45]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[45]->in[1] mux_1level_tapbuf_size2[45]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[45] gvdd_mux_1level_tapbuf_size2[45] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[481] trig v(mux_1level_tapbuf_size2[45]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[45]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[481] trig v(mux_1level_tapbuf_size2[45]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[45]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[481] when v(mux_1level_tapbuf_size2[45]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[481] trig v(mux_1level_tapbuf_size2[45]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[45]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[481] when v(mux_1level_tapbuf_size2[45]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[481] trig v(mux_1level_tapbuf_size2[45]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[45]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[45]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[45]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[481] param='mux_1level_tapbuf_size2[45]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[45]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[45]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[45]_energy_per_cycle param='mux_1level_tapbuf_size2[45]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[481]  param='mux_1level_tapbuf_size2[45]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[481]  param='dynamic_power_sb_mux[0][0]_rrnode[481]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[481] avg p(Vgvdd_mux_1level_tapbuf_size2[45]) from='start_rise_sb_mux[0][0]_rrnode[481]' to='start_rise_sb_mux[0][0]_rrnode[481]+switch_rise_sb_mux[0][0]_rrnode[481]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[481] avg p(Vgvdd_mux_1level_tapbuf_size2[45]) from='start_fall_sb_mux[0][0]_rrnode[481]' to='start_fall_sb_mux[0][0]_rrnode[481]+switch_fall_sb_mux[0][0]_rrnode[481]'
.meas tran sum_leakage_power_mux[0to45] 
+          param='sum_leakage_power_mux[0to44]+leakage_sb_mux[0][0]_rrnode[481]'
.meas tran sum_energy_per_cycle_mux[0to45] 
+          param='sum_energy_per_cycle_mux[0to44]+energy_per_cycle_sb_mux[0][0]_rrnode[481]'
***** Load for rr_node[481] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=90, type=5 *****
Xchan_mux_1level_tapbuf_size2[45]->out_loadlvl[0]_out mux_1level_tapbuf_size2[45]->out mux_1level_tapbuf_size2[45]->out_loadlvl[0]_out mux_1level_tapbuf_size2[45]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[175]_no0 mux_1level_tapbuf_size2[45]->out_loadlvl[0]_out mux_1level_tapbuf_size2[45]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[176]_no0 mux_1level_tapbuf_size2[45]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[45]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[177]_no0 mux_1level_tapbuf_size2[45]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[45]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to45] 
+          param='sum_leakage_power_sb_mux[0to44]+leakage_sb_mux[0][0]_rrnode[481]'
.meas tran sum_energy_per_cycle_sb_mux[0to45] 
+          param='sum_energy_per_cycle_sb_mux[0to44]+energy_per_cycle_sb_mux[0][0]_rrnode[481]'
Xmux_1level_tapbuf_size2[46] mux_1level_tapbuf_size2[46]->in[0] mux_1level_tapbuf_size2[46]->in[1] mux_1level_tapbuf_size2[46]->out sram[46]->outb sram[46]->out gvdd_mux_1level_tapbuf_size2[46] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[46], level=1, select_path_id=0. *****
*****1*****
Xsram[46] sram->in sram[46]->out sram[46]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[46]->out) 0
.nodeset V(sram[46]->outb) vsp
***** Signal mux_1level_tapbuf_size2[46]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[46]->in[0] mux_1level_tapbuf_size2[46]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[46]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[46]->in[1] mux_1level_tapbuf_size2[46]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[46] gvdd_mux_1level_tapbuf_size2[46] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[483] trig v(mux_1level_tapbuf_size2[46]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[46]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[483] trig v(mux_1level_tapbuf_size2[46]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[46]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[483] when v(mux_1level_tapbuf_size2[46]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[483] trig v(mux_1level_tapbuf_size2[46]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[46]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[483] when v(mux_1level_tapbuf_size2[46]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[483] trig v(mux_1level_tapbuf_size2[46]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[46]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[46]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[46]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[483] param='mux_1level_tapbuf_size2[46]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[46]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[46]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[46]_energy_per_cycle param='mux_1level_tapbuf_size2[46]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[483]  param='mux_1level_tapbuf_size2[46]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[483]  param='dynamic_power_sb_mux[0][0]_rrnode[483]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[483] avg p(Vgvdd_mux_1level_tapbuf_size2[46]) from='start_rise_sb_mux[0][0]_rrnode[483]' to='start_rise_sb_mux[0][0]_rrnode[483]+switch_rise_sb_mux[0][0]_rrnode[483]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[483] avg p(Vgvdd_mux_1level_tapbuf_size2[46]) from='start_fall_sb_mux[0][0]_rrnode[483]' to='start_fall_sb_mux[0][0]_rrnode[483]+switch_fall_sb_mux[0][0]_rrnode[483]'
.meas tran sum_leakage_power_mux[0to46] 
+          param='sum_leakage_power_mux[0to45]+leakage_sb_mux[0][0]_rrnode[483]'
.meas tran sum_energy_per_cycle_mux[0to46] 
+          param='sum_energy_per_cycle_mux[0to45]+energy_per_cycle_sb_mux[0][0]_rrnode[483]'
***** Load for rr_node[483] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=92, type=5 *****
Xchan_mux_1level_tapbuf_size2[46]->out_loadlvl[0]_out mux_1level_tapbuf_size2[46]->out mux_1level_tapbuf_size2[46]->out_loadlvl[0]_out mux_1level_tapbuf_size2[46]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[178]_no0 mux_1level_tapbuf_size2[46]->out_loadlvl[0]_out mux_1level_tapbuf_size2[46]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[179]_no0 mux_1level_tapbuf_size2[46]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[46]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[180]_no0 mux_1level_tapbuf_size2[46]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[46]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[181]_no0 mux_1level_tapbuf_size2[46]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[46]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to46] 
+          param='sum_leakage_power_sb_mux[0to45]+leakage_sb_mux[0][0]_rrnode[483]'
.meas tran sum_energy_per_cycle_sb_mux[0to46] 
+          param='sum_energy_per_cycle_sb_mux[0to45]+energy_per_cycle_sb_mux[0][0]_rrnode[483]'
Xmux_1level_tapbuf_size2[47] mux_1level_tapbuf_size2[47]->in[0] mux_1level_tapbuf_size2[47]->in[1] mux_1level_tapbuf_size2[47]->out sram[47]->outb sram[47]->out gvdd_mux_1level_tapbuf_size2[47] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[47], level=1, select_path_id=0. *****
*****1*****
Xsram[47] sram->in sram[47]->out sram[47]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[47]->out) 0
.nodeset V(sram[47]->outb) vsp
***** Signal mux_1level_tapbuf_size2[47]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[47]->in[0] mux_1level_tapbuf_size2[47]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[47]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[47]->in[1] mux_1level_tapbuf_size2[47]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[47] gvdd_mux_1level_tapbuf_size2[47] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[485] trig v(mux_1level_tapbuf_size2[47]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[47]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[485] trig v(mux_1level_tapbuf_size2[47]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[47]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[485] when v(mux_1level_tapbuf_size2[47]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[485] trig v(mux_1level_tapbuf_size2[47]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[47]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[485] when v(mux_1level_tapbuf_size2[47]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[485] trig v(mux_1level_tapbuf_size2[47]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[47]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[47]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[47]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[485] param='mux_1level_tapbuf_size2[47]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[47]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[47]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[47]_energy_per_cycle param='mux_1level_tapbuf_size2[47]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[485]  param='mux_1level_tapbuf_size2[47]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[485]  param='dynamic_power_sb_mux[0][0]_rrnode[485]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[485] avg p(Vgvdd_mux_1level_tapbuf_size2[47]) from='start_rise_sb_mux[0][0]_rrnode[485]' to='start_rise_sb_mux[0][0]_rrnode[485]+switch_rise_sb_mux[0][0]_rrnode[485]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[485] avg p(Vgvdd_mux_1level_tapbuf_size2[47]) from='start_fall_sb_mux[0][0]_rrnode[485]' to='start_fall_sb_mux[0][0]_rrnode[485]+switch_fall_sb_mux[0][0]_rrnode[485]'
.meas tran sum_leakage_power_mux[0to47] 
+          param='sum_leakage_power_mux[0to46]+leakage_sb_mux[0][0]_rrnode[485]'
.meas tran sum_energy_per_cycle_mux[0to47] 
+          param='sum_energy_per_cycle_mux[0to46]+energy_per_cycle_sb_mux[0][0]_rrnode[485]'
***** Load for rr_node[485] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=94, type=5 *****
Xchan_mux_1level_tapbuf_size2[47]->out_loadlvl[0]_out mux_1level_tapbuf_size2[47]->out mux_1level_tapbuf_size2[47]->out_loadlvl[0]_out mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[182]_no0 mux_1level_tapbuf_size2[47]->out_loadlvl[0]_out mux_1level_tapbuf_size2[47]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[183]_no0 mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[184]_no0 mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[185]_no0 mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[186]_no0 mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to47] 
+          param='sum_leakage_power_sb_mux[0to46]+leakage_sb_mux[0][0]_rrnode[485]'
.meas tran sum_energy_per_cycle_sb_mux[0to47] 
+          param='sum_energy_per_cycle_sb_mux[0to46]+energy_per_cycle_sb_mux[0][0]_rrnode[485]'
Xmux_1level_tapbuf_size2[48] mux_1level_tapbuf_size2[48]->in[0] mux_1level_tapbuf_size2[48]->in[1] mux_1level_tapbuf_size2[48]->out sram[48]->outb sram[48]->out gvdd_mux_1level_tapbuf_size2[48] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[48], level=1, select_path_id=0. *****
*****1*****
Xsram[48] sram->in sram[48]->out sram[48]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[48]->out) 0
.nodeset V(sram[48]->outb) vsp
***** Signal mux_1level_tapbuf_size2[48]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[48]->in[0] mux_1level_tapbuf_size2[48]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[48]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[48]->in[1] mux_1level_tapbuf_size2[48]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[48] gvdd_mux_1level_tapbuf_size2[48] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[487] trig v(mux_1level_tapbuf_size2[48]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[48]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[487] trig v(mux_1level_tapbuf_size2[48]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[48]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[487] when v(mux_1level_tapbuf_size2[48]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[487] trig v(mux_1level_tapbuf_size2[48]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[48]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[487] when v(mux_1level_tapbuf_size2[48]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[487] trig v(mux_1level_tapbuf_size2[48]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[48]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[48]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[48]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[487] param='mux_1level_tapbuf_size2[48]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[48]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[48]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[48]_energy_per_cycle param='mux_1level_tapbuf_size2[48]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[487]  param='mux_1level_tapbuf_size2[48]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[487]  param='dynamic_power_sb_mux[0][0]_rrnode[487]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[487] avg p(Vgvdd_mux_1level_tapbuf_size2[48]) from='start_rise_sb_mux[0][0]_rrnode[487]' to='start_rise_sb_mux[0][0]_rrnode[487]+switch_rise_sb_mux[0][0]_rrnode[487]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[487] avg p(Vgvdd_mux_1level_tapbuf_size2[48]) from='start_fall_sb_mux[0][0]_rrnode[487]' to='start_fall_sb_mux[0][0]_rrnode[487]+switch_fall_sb_mux[0][0]_rrnode[487]'
.meas tran sum_leakage_power_mux[0to48] 
+          param='sum_leakage_power_mux[0to47]+leakage_sb_mux[0][0]_rrnode[487]'
.meas tran sum_energy_per_cycle_mux[0to48] 
+          param='sum_energy_per_cycle_mux[0to47]+energy_per_cycle_sb_mux[0][0]_rrnode[487]'
***** Load for rr_node[487] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=96, type=5 *****
Xchan_mux_1level_tapbuf_size2[48]->out_loadlvl[0]_out mux_1level_tapbuf_size2[48]->out mux_1level_tapbuf_size2[48]->out_loadlvl[0]_out mux_1level_tapbuf_size2[48]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[187]_no0 mux_1level_tapbuf_size2[48]->out_loadlvl[0]_out mux_1level_tapbuf_size2[48]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[188]_no0 mux_1level_tapbuf_size2[48]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[48]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[189]_no0 mux_1level_tapbuf_size2[48]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[48]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to48] 
+          param='sum_leakage_power_sb_mux[0to47]+leakage_sb_mux[0][0]_rrnode[487]'
.meas tran sum_energy_per_cycle_sb_mux[0to48] 
+          param='sum_energy_per_cycle_sb_mux[0to47]+energy_per_cycle_sb_mux[0][0]_rrnode[487]'
Xmux_1level_tapbuf_size2[49] mux_1level_tapbuf_size2[49]->in[0] mux_1level_tapbuf_size2[49]->in[1] mux_1level_tapbuf_size2[49]->out sram[49]->outb sram[49]->out gvdd_mux_1level_tapbuf_size2[49] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[49], level=1, select_path_id=0. *****
*****1*****
Xsram[49] sram->in sram[49]->out sram[49]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[49]->out) 0
.nodeset V(sram[49]->outb) vsp
***** Signal mux_1level_tapbuf_size2[49]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[49]->in[0] mux_1level_tapbuf_size2[49]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[49]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[49]->in[1] mux_1level_tapbuf_size2[49]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[49] gvdd_mux_1level_tapbuf_size2[49] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[489] trig v(mux_1level_tapbuf_size2[49]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[49]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[489] trig v(mux_1level_tapbuf_size2[49]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[49]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[489] when v(mux_1level_tapbuf_size2[49]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[489] trig v(mux_1level_tapbuf_size2[49]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[49]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[489] when v(mux_1level_tapbuf_size2[49]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[489] trig v(mux_1level_tapbuf_size2[49]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[49]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[49]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[49]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[489] param='mux_1level_tapbuf_size2[49]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[49]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[49]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[49]_energy_per_cycle param='mux_1level_tapbuf_size2[49]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[489]  param='mux_1level_tapbuf_size2[49]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[489]  param='dynamic_power_sb_mux[0][0]_rrnode[489]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[489] avg p(Vgvdd_mux_1level_tapbuf_size2[49]) from='start_rise_sb_mux[0][0]_rrnode[489]' to='start_rise_sb_mux[0][0]_rrnode[489]+switch_rise_sb_mux[0][0]_rrnode[489]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[489] avg p(Vgvdd_mux_1level_tapbuf_size2[49]) from='start_fall_sb_mux[0][0]_rrnode[489]' to='start_fall_sb_mux[0][0]_rrnode[489]+switch_fall_sb_mux[0][0]_rrnode[489]'
.meas tran sum_leakage_power_mux[0to49] 
+          param='sum_leakage_power_mux[0to48]+leakage_sb_mux[0][0]_rrnode[489]'
.meas tran sum_energy_per_cycle_mux[0to49] 
+          param='sum_energy_per_cycle_mux[0to48]+energy_per_cycle_sb_mux[0][0]_rrnode[489]'
***** Load for rr_node[489] *****
**** Loads for rr_node: xlow=0, ylow=1, xhigh=0, yhigh=1, ptc_num=98, type=5 *****
Xchan_mux_1level_tapbuf_size2[49]->out_loadlvl[0]_out mux_1level_tapbuf_size2[49]->out mux_1level_tapbuf_size2[49]->out_loadlvl[0]_out mux_1level_tapbuf_size2[49]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[190]_no0 mux_1level_tapbuf_size2[49]->out_loadlvl[0]_out mux_1level_tapbuf_size2[49]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[191]_no0 mux_1level_tapbuf_size2[49]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[49]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[192]_no0 mux_1level_tapbuf_size2[49]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[49]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[193]_no0 mux_1level_tapbuf_size2[49]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[49]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to49] 
+          param='sum_leakage_power_sb_mux[0to48]+leakage_sb_mux[0][0]_rrnode[489]'
.meas tran sum_energy_per_cycle_sb_mux[0to49] 
+          param='sum_energy_per_cycle_sb_mux[0to48]+energy_per_cycle_sb_mux[0][0]_rrnode[489]'
Xmux_1level_tapbuf_size2[50] mux_1level_tapbuf_size2[50]->in[0] mux_1level_tapbuf_size2[50]->in[1] mux_1level_tapbuf_size2[50]->out sram[50]->outb sram[50]->out gvdd_mux_1level_tapbuf_size2[50] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[50], level=1, select_path_id=0. *****
*****1*****
Xsram[50] sram->in sram[50]->out sram[50]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[50]->out) 0
.nodeset V(sram[50]->outb) vsp
***** Signal mux_1level_tapbuf_size2[50]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[50]->in[0] mux_1level_tapbuf_size2[50]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[50]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[50]->in[1] mux_1level_tapbuf_size2[50]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[50] gvdd_mux_1level_tapbuf_size2[50] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[191] trig v(mux_1level_tapbuf_size2[50]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[50]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[191] trig v(mux_1level_tapbuf_size2[50]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[50]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[191] when v(mux_1level_tapbuf_size2[50]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[191] trig v(mux_1level_tapbuf_size2[50]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[50]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[191] when v(mux_1level_tapbuf_size2[50]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[191] trig v(mux_1level_tapbuf_size2[50]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[50]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[50]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[50]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[191] param='mux_1level_tapbuf_size2[50]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[50]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[50]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[50]_energy_per_cycle param='mux_1level_tapbuf_size2[50]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[191]  param='mux_1level_tapbuf_size2[50]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[191]  param='dynamic_power_sb_mux[0][0]_rrnode[191]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[191] avg p(Vgvdd_mux_1level_tapbuf_size2[50]) from='start_rise_sb_mux[0][0]_rrnode[191]' to='start_rise_sb_mux[0][0]_rrnode[191]+switch_rise_sb_mux[0][0]_rrnode[191]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[191] avg p(Vgvdd_mux_1level_tapbuf_size2[50]) from='start_fall_sb_mux[0][0]_rrnode[191]' to='start_fall_sb_mux[0][0]_rrnode[191]+switch_fall_sb_mux[0][0]_rrnode[191]'
.meas tran sum_leakage_power_mux[0to50] 
+          param='sum_leakage_power_mux[0to49]+leakage_sb_mux[0][0]_rrnode[191]'
.meas tran sum_energy_per_cycle_mux[0to50] 
+          param='sum_energy_per_cycle_mux[0to49]+energy_per_cycle_sb_mux[0][0]_rrnode[191]'
***** Load for rr_node[191] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=0, type=4 *****
Xchan_mux_1level_tapbuf_size2[50]->out_loadlvl[0]_out mux_1level_tapbuf_size2[50]->out mux_1level_tapbuf_size2[50]->out_loadlvl[0]_out mux_1level_tapbuf_size2[50]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[194]_no0 mux_1level_tapbuf_size2[50]->out_loadlvl[0]_out mux_1level_tapbuf_size2[50]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[195]_no0 mux_1level_tapbuf_size2[50]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[50]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[196]_no0 mux_1level_tapbuf_size2[50]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[50]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[197]_no0 mux_1level_tapbuf_size2[50]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[50]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[198]_no0 mux_1level_tapbuf_size2[50]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[50]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to50] 
+          param='sum_leakage_power_sb_mux[0to49]+leakage_sb_mux[0][0]_rrnode[191]'
.meas tran sum_energy_per_cycle_sb_mux[0to50] 
+          param='sum_energy_per_cycle_sb_mux[0to49]+energy_per_cycle_sb_mux[0][0]_rrnode[191]'
Xmux_1level_tapbuf_size2[51] mux_1level_tapbuf_size2[51]->in[0] mux_1level_tapbuf_size2[51]->in[1] mux_1level_tapbuf_size2[51]->out sram[51]->outb sram[51]->out gvdd_mux_1level_tapbuf_size2[51] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[51], level=1, select_path_id=0. *****
*****1*****
Xsram[51] sram->in sram[51]->out sram[51]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[51]->out) 0
.nodeset V(sram[51]->outb) vsp
***** Signal mux_1level_tapbuf_size2[51]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[51]->in[0] mux_1level_tapbuf_size2[51]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[51]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[51]->in[1] mux_1level_tapbuf_size2[51]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[51] gvdd_mux_1level_tapbuf_size2[51] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[193] trig v(mux_1level_tapbuf_size2[51]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[51]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[193] trig v(mux_1level_tapbuf_size2[51]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[51]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[193] when v(mux_1level_tapbuf_size2[51]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[193] trig v(mux_1level_tapbuf_size2[51]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[51]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[193] when v(mux_1level_tapbuf_size2[51]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[193] trig v(mux_1level_tapbuf_size2[51]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[51]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[51]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[51]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[193] param='mux_1level_tapbuf_size2[51]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[51]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[51]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[51]_energy_per_cycle param='mux_1level_tapbuf_size2[51]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[193]  param='mux_1level_tapbuf_size2[51]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[193]  param='dynamic_power_sb_mux[0][0]_rrnode[193]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[193] avg p(Vgvdd_mux_1level_tapbuf_size2[51]) from='start_rise_sb_mux[0][0]_rrnode[193]' to='start_rise_sb_mux[0][0]_rrnode[193]+switch_rise_sb_mux[0][0]_rrnode[193]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[193] avg p(Vgvdd_mux_1level_tapbuf_size2[51]) from='start_fall_sb_mux[0][0]_rrnode[193]' to='start_fall_sb_mux[0][0]_rrnode[193]+switch_fall_sb_mux[0][0]_rrnode[193]'
.meas tran sum_leakage_power_mux[0to51] 
+          param='sum_leakage_power_mux[0to50]+leakage_sb_mux[0][0]_rrnode[193]'
.meas tran sum_energy_per_cycle_mux[0to51] 
+          param='sum_energy_per_cycle_mux[0to50]+energy_per_cycle_sb_mux[0][0]_rrnode[193]'
***** Load for rr_node[193] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=2, type=4 *****
Xchan_mux_1level_tapbuf_size2[51]->out_loadlvl[0]_out mux_1level_tapbuf_size2[51]->out mux_1level_tapbuf_size2[51]->out_loadlvl[0]_out mux_1level_tapbuf_size2[51]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[199]_no0 mux_1level_tapbuf_size2[51]->out_loadlvl[0]_out mux_1level_tapbuf_size2[51]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[200]_no0 mux_1level_tapbuf_size2[51]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[51]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[201]_no0 mux_1level_tapbuf_size2[51]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[51]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to51] 
+          param='sum_leakage_power_sb_mux[0to50]+leakage_sb_mux[0][0]_rrnode[193]'
.meas tran sum_energy_per_cycle_sb_mux[0to51] 
+          param='sum_energy_per_cycle_sb_mux[0to50]+energy_per_cycle_sb_mux[0][0]_rrnode[193]'
Xmux_1level_tapbuf_size2[52] mux_1level_tapbuf_size2[52]->in[0] mux_1level_tapbuf_size2[52]->in[1] mux_1level_tapbuf_size2[52]->out sram[52]->outb sram[52]->out gvdd_mux_1level_tapbuf_size2[52] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[52], level=1, select_path_id=0. *****
*****1*****
Xsram[52] sram->in sram[52]->out sram[52]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[52]->out) 0
.nodeset V(sram[52]->outb) vsp
***** Signal mux_1level_tapbuf_size2[52]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[52]->in[0] mux_1level_tapbuf_size2[52]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[52]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[52]->in[1] mux_1level_tapbuf_size2[52]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[52] gvdd_mux_1level_tapbuf_size2[52] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[195] trig v(mux_1level_tapbuf_size2[52]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[52]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[195] trig v(mux_1level_tapbuf_size2[52]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[52]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[195] when v(mux_1level_tapbuf_size2[52]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[195] trig v(mux_1level_tapbuf_size2[52]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[52]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[195] when v(mux_1level_tapbuf_size2[52]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[195] trig v(mux_1level_tapbuf_size2[52]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[52]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[52]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[52]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[195] param='mux_1level_tapbuf_size2[52]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[52]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[52]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[52]_energy_per_cycle param='mux_1level_tapbuf_size2[52]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[195]  param='mux_1level_tapbuf_size2[52]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[195]  param='dynamic_power_sb_mux[0][0]_rrnode[195]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[195] avg p(Vgvdd_mux_1level_tapbuf_size2[52]) from='start_rise_sb_mux[0][0]_rrnode[195]' to='start_rise_sb_mux[0][0]_rrnode[195]+switch_rise_sb_mux[0][0]_rrnode[195]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[195] avg p(Vgvdd_mux_1level_tapbuf_size2[52]) from='start_fall_sb_mux[0][0]_rrnode[195]' to='start_fall_sb_mux[0][0]_rrnode[195]+switch_fall_sb_mux[0][0]_rrnode[195]'
.meas tran sum_leakage_power_mux[0to52] 
+          param='sum_leakage_power_mux[0to51]+leakage_sb_mux[0][0]_rrnode[195]'
.meas tran sum_energy_per_cycle_mux[0to52] 
+          param='sum_energy_per_cycle_mux[0to51]+energy_per_cycle_sb_mux[0][0]_rrnode[195]'
***** Load for rr_node[195] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=4, type=4 *****
Xchan_mux_1level_tapbuf_size2[52]->out_loadlvl[0]_out mux_1level_tapbuf_size2[52]->out mux_1level_tapbuf_size2[52]->out_loadlvl[0]_out mux_1level_tapbuf_size2[52]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[202]_no0 mux_1level_tapbuf_size2[52]->out_loadlvl[0]_out mux_1level_tapbuf_size2[52]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[203]_no0 mux_1level_tapbuf_size2[52]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[52]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[204]_no0 mux_1level_tapbuf_size2[52]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[52]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[205]_no0 mux_1level_tapbuf_size2[52]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[52]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to52] 
+          param='sum_leakage_power_sb_mux[0to51]+leakage_sb_mux[0][0]_rrnode[195]'
.meas tran sum_energy_per_cycle_sb_mux[0to52] 
+          param='sum_energy_per_cycle_sb_mux[0to51]+energy_per_cycle_sb_mux[0][0]_rrnode[195]'
Xmux_1level_tapbuf_size2[53] mux_1level_tapbuf_size2[53]->in[0] mux_1level_tapbuf_size2[53]->in[1] mux_1level_tapbuf_size2[53]->out sram[53]->outb sram[53]->out gvdd_mux_1level_tapbuf_size2[53] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[53], level=1, select_path_id=0. *****
*****1*****
Xsram[53] sram->in sram[53]->out sram[53]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[53]->out) 0
.nodeset V(sram[53]->outb) vsp
***** Signal mux_1level_tapbuf_size2[53]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[53]->in[0] mux_1level_tapbuf_size2[53]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[53]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[53]->in[1] mux_1level_tapbuf_size2[53]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[53] gvdd_mux_1level_tapbuf_size2[53] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[197] trig v(mux_1level_tapbuf_size2[53]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[53]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[197] trig v(mux_1level_tapbuf_size2[53]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[53]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[197] when v(mux_1level_tapbuf_size2[53]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[197] trig v(mux_1level_tapbuf_size2[53]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[53]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[197] when v(mux_1level_tapbuf_size2[53]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[197] trig v(mux_1level_tapbuf_size2[53]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[53]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[53]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[53]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[197] param='mux_1level_tapbuf_size2[53]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[53]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[53]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[53]_energy_per_cycle param='mux_1level_tapbuf_size2[53]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[197]  param='mux_1level_tapbuf_size2[53]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[197]  param='dynamic_power_sb_mux[0][0]_rrnode[197]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[197] avg p(Vgvdd_mux_1level_tapbuf_size2[53]) from='start_rise_sb_mux[0][0]_rrnode[197]' to='start_rise_sb_mux[0][0]_rrnode[197]+switch_rise_sb_mux[0][0]_rrnode[197]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[197] avg p(Vgvdd_mux_1level_tapbuf_size2[53]) from='start_fall_sb_mux[0][0]_rrnode[197]' to='start_fall_sb_mux[0][0]_rrnode[197]+switch_fall_sb_mux[0][0]_rrnode[197]'
.meas tran sum_leakage_power_mux[0to53] 
+          param='sum_leakage_power_mux[0to52]+leakage_sb_mux[0][0]_rrnode[197]'
.meas tran sum_energy_per_cycle_mux[0to53] 
+          param='sum_energy_per_cycle_mux[0to52]+energy_per_cycle_sb_mux[0][0]_rrnode[197]'
***** Load for rr_node[197] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=6, type=4 *****
Xchan_mux_1level_tapbuf_size2[53]->out_loadlvl[0]_out mux_1level_tapbuf_size2[53]->out mux_1level_tapbuf_size2[53]->out_loadlvl[0]_out mux_1level_tapbuf_size2[53]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[206]_no0 mux_1level_tapbuf_size2[53]->out_loadlvl[0]_out mux_1level_tapbuf_size2[53]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[207]_no0 mux_1level_tapbuf_size2[53]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[53]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[208]_no0 mux_1level_tapbuf_size2[53]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[53]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[209]_no0 mux_1level_tapbuf_size2[53]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[53]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to53] 
+          param='sum_leakage_power_sb_mux[0to52]+leakage_sb_mux[0][0]_rrnode[197]'
.meas tran sum_energy_per_cycle_sb_mux[0to53] 
+          param='sum_energy_per_cycle_sb_mux[0to52]+energy_per_cycle_sb_mux[0][0]_rrnode[197]'
Xmux_1level_tapbuf_size2[54] mux_1level_tapbuf_size2[54]->in[0] mux_1level_tapbuf_size2[54]->in[1] mux_1level_tapbuf_size2[54]->out sram[54]->outb sram[54]->out gvdd_mux_1level_tapbuf_size2[54] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[54], level=1, select_path_id=0. *****
*****1*****
Xsram[54] sram->in sram[54]->out sram[54]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[54]->out) 0
.nodeset V(sram[54]->outb) vsp
***** Signal mux_1level_tapbuf_size2[54]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[54]->in[0] mux_1level_tapbuf_size2[54]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[54]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[54]->in[1] mux_1level_tapbuf_size2[54]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[54] gvdd_mux_1level_tapbuf_size2[54] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[199] trig v(mux_1level_tapbuf_size2[54]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[54]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[199] trig v(mux_1level_tapbuf_size2[54]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[54]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[199] when v(mux_1level_tapbuf_size2[54]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[199] trig v(mux_1level_tapbuf_size2[54]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[54]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[199] when v(mux_1level_tapbuf_size2[54]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[199] trig v(mux_1level_tapbuf_size2[54]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[54]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[54]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[54]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[199] param='mux_1level_tapbuf_size2[54]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[54]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[54]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[54]_energy_per_cycle param='mux_1level_tapbuf_size2[54]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[199]  param='mux_1level_tapbuf_size2[54]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[199]  param='dynamic_power_sb_mux[0][0]_rrnode[199]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[199] avg p(Vgvdd_mux_1level_tapbuf_size2[54]) from='start_rise_sb_mux[0][0]_rrnode[199]' to='start_rise_sb_mux[0][0]_rrnode[199]+switch_rise_sb_mux[0][0]_rrnode[199]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[199] avg p(Vgvdd_mux_1level_tapbuf_size2[54]) from='start_fall_sb_mux[0][0]_rrnode[199]' to='start_fall_sb_mux[0][0]_rrnode[199]+switch_fall_sb_mux[0][0]_rrnode[199]'
.meas tran sum_leakage_power_mux[0to54] 
+          param='sum_leakage_power_mux[0to53]+leakage_sb_mux[0][0]_rrnode[199]'
.meas tran sum_energy_per_cycle_mux[0to54] 
+          param='sum_energy_per_cycle_mux[0to53]+energy_per_cycle_sb_mux[0][0]_rrnode[199]'
***** Load for rr_node[199] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=8, type=4 *****
Xchan_mux_1level_tapbuf_size2[54]->out_loadlvl[0]_out mux_1level_tapbuf_size2[54]->out mux_1level_tapbuf_size2[54]->out_loadlvl[0]_out mux_1level_tapbuf_size2[54]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[210]_no0 mux_1level_tapbuf_size2[54]->out_loadlvl[0]_out mux_1level_tapbuf_size2[54]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[211]_no0 mux_1level_tapbuf_size2[54]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[54]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[212]_no0 mux_1level_tapbuf_size2[54]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[54]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[213]_no0 mux_1level_tapbuf_size2[54]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[54]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to54] 
+          param='sum_leakage_power_sb_mux[0to53]+leakage_sb_mux[0][0]_rrnode[199]'
.meas tran sum_energy_per_cycle_sb_mux[0to54] 
+          param='sum_energy_per_cycle_sb_mux[0to53]+energy_per_cycle_sb_mux[0][0]_rrnode[199]'
Xmux_1level_tapbuf_size2[55] mux_1level_tapbuf_size2[55]->in[0] mux_1level_tapbuf_size2[55]->in[1] mux_1level_tapbuf_size2[55]->out sram[55]->outb sram[55]->out gvdd_mux_1level_tapbuf_size2[55] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[55], level=1, select_path_id=0. *****
*****1*****
Xsram[55] sram->in sram[55]->out sram[55]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[55]->out) 0
.nodeset V(sram[55]->outb) vsp
***** Signal mux_1level_tapbuf_size2[55]->in[0] density = 0.2026, probability=0.5018.*****
Vmux_1level_tapbuf_size2[55]->in[0] mux_1level_tapbuf_size2[55]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
***** Signal mux_1level_tapbuf_size2[55]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[55]->in[1] mux_1level_tapbuf_size2[55]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[55] gvdd_mux_1level_tapbuf_size2[55] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[201] trig v(mux_1level_tapbuf_size2[55]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[55]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[201] trig v(mux_1level_tapbuf_size2[55]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[55]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[201] when v(mux_1level_tapbuf_size2[55]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[201] trig v(mux_1level_tapbuf_size2[55]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[55]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[201] when v(mux_1level_tapbuf_size2[55]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[201] trig v(mux_1level_tapbuf_size2[55]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[55]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[55]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[55]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[201] param='mux_1level_tapbuf_size2[55]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[55]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[55]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[55]_energy_per_cycle param='mux_1level_tapbuf_size2[55]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[201]  param='mux_1level_tapbuf_size2[55]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[201]  param='dynamic_power_sb_mux[0][0]_rrnode[201]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[201] avg p(Vgvdd_mux_1level_tapbuf_size2[55]) from='start_rise_sb_mux[0][0]_rrnode[201]' to='start_rise_sb_mux[0][0]_rrnode[201]+switch_rise_sb_mux[0][0]_rrnode[201]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[201] avg p(Vgvdd_mux_1level_tapbuf_size2[55]) from='start_fall_sb_mux[0][0]_rrnode[201]' to='start_fall_sb_mux[0][0]_rrnode[201]+switch_fall_sb_mux[0][0]_rrnode[201]'
.meas tran sum_leakage_power_mux[0to55] 
+          param='sum_leakage_power_mux[0to54]+leakage_sb_mux[0][0]_rrnode[201]'
.meas tran sum_energy_per_cycle_mux[0to55] 
+          param='sum_energy_per_cycle_mux[0to54]+energy_per_cycle_sb_mux[0][0]_rrnode[201]'
***** Load for rr_node[201] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=10, type=4 *****
Xchan_mux_1level_tapbuf_size2[55]->out_loadlvl[0]_out mux_1level_tapbuf_size2[55]->out mux_1level_tapbuf_size2[55]->out_loadlvl[0]_out mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[214]_no0 mux_1level_tapbuf_size2[55]->out_loadlvl[0]_out mux_1level_tapbuf_size2[55]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[215]_no0 mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[216]_no0 mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[217]_no0 mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to55] 
+          param='sum_leakage_power_sb_mux[0to54]+leakage_sb_mux[0][0]_rrnode[201]'
.meas tran sum_energy_per_cycle_sb_mux[0to55] 
+          param='sum_energy_per_cycle_sb_mux[0to54]+energy_per_cycle_sb_mux[0][0]_rrnode[201]'
Xmux_1level_tapbuf_size2[56] mux_1level_tapbuf_size2[56]->in[0] mux_1level_tapbuf_size2[56]->in[1] mux_1level_tapbuf_size2[56]->out sram[56]->outb sram[56]->out gvdd_mux_1level_tapbuf_size2[56] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[56], level=1, select_path_id=0. *****
*****1*****
Xsram[56] sram->in sram[56]->out sram[56]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[56]->out) 0
.nodeset V(sram[56]->outb) vsp
***** Signal mux_1level_tapbuf_size2[56]->in[0] density = 0.2026, probability=0.5018.*****
Vmux_1level_tapbuf_size2[56]->in[0] mux_1level_tapbuf_size2[56]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
***** Signal mux_1level_tapbuf_size2[56]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[56]->in[1] mux_1level_tapbuf_size2[56]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[56] gvdd_mux_1level_tapbuf_size2[56] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[203] trig v(mux_1level_tapbuf_size2[56]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[56]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[203] trig v(mux_1level_tapbuf_size2[56]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[56]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[203] when v(mux_1level_tapbuf_size2[56]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[203] trig v(mux_1level_tapbuf_size2[56]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[56]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[203] when v(mux_1level_tapbuf_size2[56]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[203] trig v(mux_1level_tapbuf_size2[56]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[56]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[56]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[56]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[203] param='mux_1level_tapbuf_size2[56]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[56]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[56]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[56]_energy_per_cycle param='mux_1level_tapbuf_size2[56]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[203]  param='mux_1level_tapbuf_size2[56]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[203]  param='dynamic_power_sb_mux[0][0]_rrnode[203]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[203] avg p(Vgvdd_mux_1level_tapbuf_size2[56]) from='start_rise_sb_mux[0][0]_rrnode[203]' to='start_rise_sb_mux[0][0]_rrnode[203]+switch_rise_sb_mux[0][0]_rrnode[203]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[203] avg p(Vgvdd_mux_1level_tapbuf_size2[56]) from='start_fall_sb_mux[0][0]_rrnode[203]' to='start_fall_sb_mux[0][0]_rrnode[203]+switch_fall_sb_mux[0][0]_rrnode[203]'
.meas tran sum_leakage_power_mux[0to56] 
+          param='sum_leakage_power_mux[0to55]+leakage_sb_mux[0][0]_rrnode[203]'
.meas tran sum_energy_per_cycle_mux[0to56] 
+          param='sum_energy_per_cycle_mux[0to55]+energy_per_cycle_sb_mux[0][0]_rrnode[203]'
***** Load for rr_node[203] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=12, type=4 *****
Xchan_mux_1level_tapbuf_size2[56]->out_loadlvl[0]_out mux_1level_tapbuf_size2[56]->out mux_1level_tapbuf_size2[56]->out_loadlvl[0]_out mux_1level_tapbuf_size2[56]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[218]_no0 mux_1level_tapbuf_size2[56]->out_loadlvl[0]_out mux_1level_tapbuf_size2[56]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[219]_no0 mux_1level_tapbuf_size2[56]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[56]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[220]_no0 mux_1level_tapbuf_size2[56]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[56]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to56] 
+          param='sum_leakage_power_sb_mux[0to55]+leakage_sb_mux[0][0]_rrnode[203]'
.meas tran sum_energy_per_cycle_sb_mux[0to56] 
+          param='sum_energy_per_cycle_sb_mux[0to55]+energy_per_cycle_sb_mux[0][0]_rrnode[203]'
Xmux_1level_tapbuf_size2[57] mux_1level_tapbuf_size2[57]->in[0] mux_1level_tapbuf_size2[57]->in[1] mux_1level_tapbuf_size2[57]->out sram[57]->outb sram[57]->out gvdd_mux_1level_tapbuf_size2[57] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[57], level=1, select_path_id=0. *****
*****1*****
Xsram[57] sram->in sram[57]->out sram[57]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[57]->out) 0
.nodeset V(sram[57]->outb) vsp
***** Signal mux_1level_tapbuf_size2[57]->in[0] density = 0.2026, probability=0.5018.*****
Vmux_1level_tapbuf_size2[57]->in[0] mux_1level_tapbuf_size2[57]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
***** Signal mux_1level_tapbuf_size2[57]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[57]->in[1] mux_1level_tapbuf_size2[57]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[57] gvdd_mux_1level_tapbuf_size2[57] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[205] trig v(mux_1level_tapbuf_size2[57]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[57]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[205] trig v(mux_1level_tapbuf_size2[57]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[57]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[205] when v(mux_1level_tapbuf_size2[57]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[205] trig v(mux_1level_tapbuf_size2[57]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[57]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[205] when v(mux_1level_tapbuf_size2[57]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[205] trig v(mux_1level_tapbuf_size2[57]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[57]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[57]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[57]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[205] param='mux_1level_tapbuf_size2[57]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[57]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[57]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[57]_energy_per_cycle param='mux_1level_tapbuf_size2[57]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[205]  param='mux_1level_tapbuf_size2[57]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[205]  param='dynamic_power_sb_mux[0][0]_rrnode[205]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[205] avg p(Vgvdd_mux_1level_tapbuf_size2[57]) from='start_rise_sb_mux[0][0]_rrnode[205]' to='start_rise_sb_mux[0][0]_rrnode[205]+switch_rise_sb_mux[0][0]_rrnode[205]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[205] avg p(Vgvdd_mux_1level_tapbuf_size2[57]) from='start_fall_sb_mux[0][0]_rrnode[205]' to='start_fall_sb_mux[0][0]_rrnode[205]+switch_fall_sb_mux[0][0]_rrnode[205]'
.meas tran sum_leakage_power_mux[0to57] 
+          param='sum_leakage_power_mux[0to56]+leakage_sb_mux[0][0]_rrnode[205]'
.meas tran sum_energy_per_cycle_mux[0to57] 
+          param='sum_energy_per_cycle_mux[0to56]+energy_per_cycle_sb_mux[0][0]_rrnode[205]'
***** Load for rr_node[205] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=14, type=4 *****
Xchan_mux_1level_tapbuf_size2[57]->out_loadlvl[0]_out mux_1level_tapbuf_size2[57]->out mux_1level_tapbuf_size2[57]->out_loadlvl[0]_out mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[221]_no0 mux_1level_tapbuf_size2[57]->out_loadlvl[0]_out mux_1level_tapbuf_size2[57]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[222]_no0 mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[223]_no0 mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[224]_no0 mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[225]_no0 mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to57] 
+          param='sum_leakage_power_sb_mux[0to56]+leakage_sb_mux[0][0]_rrnode[205]'
.meas tran sum_energy_per_cycle_sb_mux[0to57] 
+          param='sum_energy_per_cycle_sb_mux[0to56]+energy_per_cycle_sb_mux[0][0]_rrnode[205]'
Xmux_1level_tapbuf_size2[58] mux_1level_tapbuf_size2[58]->in[0] mux_1level_tapbuf_size2[58]->in[1] mux_1level_tapbuf_size2[58]->out sram[58]->outb sram[58]->out gvdd_mux_1level_tapbuf_size2[58] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[58], level=1, select_path_id=0. *****
*****1*****
Xsram[58] sram->in sram[58]->out sram[58]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[58]->out) 0
.nodeset V(sram[58]->outb) vsp
***** Signal mux_1level_tapbuf_size2[58]->in[0] density = 0.2026, probability=0.5018.*****
Vmux_1level_tapbuf_size2[58]->in[0] mux_1level_tapbuf_size2[58]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
***** Signal mux_1level_tapbuf_size2[58]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[58]->in[1] mux_1level_tapbuf_size2[58]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[58] gvdd_mux_1level_tapbuf_size2[58] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[207] trig v(mux_1level_tapbuf_size2[58]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[58]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[207] trig v(mux_1level_tapbuf_size2[58]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[58]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[207] when v(mux_1level_tapbuf_size2[58]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[207] trig v(mux_1level_tapbuf_size2[58]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[58]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[207] when v(mux_1level_tapbuf_size2[58]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[207] trig v(mux_1level_tapbuf_size2[58]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[58]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[58]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[58]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[207] param='mux_1level_tapbuf_size2[58]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[58]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[58]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[58]_energy_per_cycle param='mux_1level_tapbuf_size2[58]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[207]  param='mux_1level_tapbuf_size2[58]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[207]  param='dynamic_power_sb_mux[0][0]_rrnode[207]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[207] avg p(Vgvdd_mux_1level_tapbuf_size2[58]) from='start_rise_sb_mux[0][0]_rrnode[207]' to='start_rise_sb_mux[0][0]_rrnode[207]+switch_rise_sb_mux[0][0]_rrnode[207]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[207] avg p(Vgvdd_mux_1level_tapbuf_size2[58]) from='start_fall_sb_mux[0][0]_rrnode[207]' to='start_fall_sb_mux[0][0]_rrnode[207]+switch_fall_sb_mux[0][0]_rrnode[207]'
.meas tran sum_leakage_power_mux[0to58] 
+          param='sum_leakage_power_mux[0to57]+leakage_sb_mux[0][0]_rrnode[207]'
.meas tran sum_energy_per_cycle_mux[0to58] 
+          param='sum_energy_per_cycle_mux[0to57]+energy_per_cycle_sb_mux[0][0]_rrnode[207]'
***** Load for rr_node[207] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=16, type=4 *****
Xchan_mux_1level_tapbuf_size2[58]->out_loadlvl[0]_out mux_1level_tapbuf_size2[58]->out mux_1level_tapbuf_size2[58]->out_loadlvl[0]_out mux_1level_tapbuf_size2[58]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[226]_no0 mux_1level_tapbuf_size2[58]->out_loadlvl[0]_out mux_1level_tapbuf_size2[58]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[227]_no0 mux_1level_tapbuf_size2[58]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[58]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[228]_no0 mux_1level_tapbuf_size2[58]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[58]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to58] 
+          param='sum_leakage_power_sb_mux[0to57]+leakage_sb_mux[0][0]_rrnode[207]'
.meas tran sum_energy_per_cycle_sb_mux[0to58] 
+          param='sum_energy_per_cycle_sb_mux[0to57]+energy_per_cycle_sb_mux[0][0]_rrnode[207]'
Xmux_1level_tapbuf_size2[59] mux_1level_tapbuf_size2[59]->in[0] mux_1level_tapbuf_size2[59]->in[1] mux_1level_tapbuf_size2[59]->out sram[59]->outb sram[59]->out gvdd_mux_1level_tapbuf_size2[59] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[59], level=1, select_path_id=0. *****
*****1*****
Xsram[59] sram->in sram[59]->out sram[59]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[59]->out) 0
.nodeset V(sram[59]->outb) vsp
***** Signal mux_1level_tapbuf_size2[59]->in[0] density = 0.2026, probability=0.5018.*****
Vmux_1level_tapbuf_size2[59]->in[0] mux_1level_tapbuf_size2[59]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.5018*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
***** Signal mux_1level_tapbuf_size2[59]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[59]->in[1] mux_1level_tapbuf_size2[59]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[59] gvdd_mux_1level_tapbuf_size2[59] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[209] trig v(mux_1level_tapbuf_size2[59]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[59]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[209] trig v(mux_1level_tapbuf_size2[59]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[59]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[209] when v(mux_1level_tapbuf_size2[59]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[209] trig v(mux_1level_tapbuf_size2[59]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[59]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[209] when v(mux_1level_tapbuf_size2[59]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[209] trig v(mux_1level_tapbuf_size2[59]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[59]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[59]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[59]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[209] param='mux_1level_tapbuf_size2[59]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[59]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[59]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[59]_energy_per_cycle param='mux_1level_tapbuf_size2[59]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[209]  param='mux_1level_tapbuf_size2[59]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[209]  param='dynamic_power_sb_mux[0][0]_rrnode[209]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[209] avg p(Vgvdd_mux_1level_tapbuf_size2[59]) from='start_rise_sb_mux[0][0]_rrnode[209]' to='start_rise_sb_mux[0][0]_rrnode[209]+switch_rise_sb_mux[0][0]_rrnode[209]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[209] avg p(Vgvdd_mux_1level_tapbuf_size2[59]) from='start_fall_sb_mux[0][0]_rrnode[209]' to='start_fall_sb_mux[0][0]_rrnode[209]+switch_fall_sb_mux[0][0]_rrnode[209]'
.meas tran sum_leakage_power_mux[0to59] 
+          param='sum_leakage_power_mux[0to58]+leakage_sb_mux[0][0]_rrnode[209]'
.meas tran sum_energy_per_cycle_mux[0to59] 
+          param='sum_energy_per_cycle_mux[0to58]+energy_per_cycle_sb_mux[0][0]_rrnode[209]'
***** Load for rr_node[209] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=18, type=4 *****
Xchan_mux_1level_tapbuf_size2[59]->out_loadlvl[0]_out mux_1level_tapbuf_size2[59]->out mux_1level_tapbuf_size2[59]->out_loadlvl[0]_out mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[229]_no0 mux_1level_tapbuf_size2[59]->out_loadlvl[0]_out mux_1level_tapbuf_size2[59]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[230]_no0 mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[231]_no0 mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[232]_no0 mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to59] 
+          param='sum_leakage_power_sb_mux[0to58]+leakage_sb_mux[0][0]_rrnode[209]'
.meas tran sum_energy_per_cycle_sb_mux[0to59] 
+          param='sum_energy_per_cycle_sb_mux[0to58]+energy_per_cycle_sb_mux[0][0]_rrnode[209]'
Xmux_1level_tapbuf_size2[60] mux_1level_tapbuf_size2[60]->in[0] mux_1level_tapbuf_size2[60]->in[1] mux_1level_tapbuf_size2[60]->out sram[60]->outb sram[60]->out gvdd_mux_1level_tapbuf_size2[60] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[60], level=1, select_path_id=0. *****
*****1*****
Xsram[60] sram->in sram[60]->out sram[60]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[60]->out) 0
.nodeset V(sram[60]->outb) vsp
***** Signal mux_1level_tapbuf_size2[60]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[60]->in[0] mux_1level_tapbuf_size2[60]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[60]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[60]->in[1] mux_1level_tapbuf_size2[60]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[60] gvdd_mux_1level_tapbuf_size2[60] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[211] trig v(mux_1level_tapbuf_size2[60]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[60]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[211] trig v(mux_1level_tapbuf_size2[60]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[60]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[211] when v(mux_1level_tapbuf_size2[60]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[211] trig v(mux_1level_tapbuf_size2[60]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[60]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[211] when v(mux_1level_tapbuf_size2[60]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[211] trig v(mux_1level_tapbuf_size2[60]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[60]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[60]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[60]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[211] param='mux_1level_tapbuf_size2[60]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[60]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[60]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[60]_energy_per_cycle param='mux_1level_tapbuf_size2[60]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[211]  param='mux_1level_tapbuf_size2[60]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[211]  param='dynamic_power_sb_mux[0][0]_rrnode[211]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[211] avg p(Vgvdd_mux_1level_tapbuf_size2[60]) from='start_rise_sb_mux[0][0]_rrnode[211]' to='start_rise_sb_mux[0][0]_rrnode[211]+switch_rise_sb_mux[0][0]_rrnode[211]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[211] avg p(Vgvdd_mux_1level_tapbuf_size2[60]) from='start_fall_sb_mux[0][0]_rrnode[211]' to='start_fall_sb_mux[0][0]_rrnode[211]+switch_fall_sb_mux[0][0]_rrnode[211]'
.meas tran sum_leakage_power_mux[0to60] 
+          param='sum_leakage_power_mux[0to59]+leakage_sb_mux[0][0]_rrnode[211]'
.meas tran sum_energy_per_cycle_mux[0to60] 
+          param='sum_energy_per_cycle_mux[0to59]+energy_per_cycle_sb_mux[0][0]_rrnode[211]'
***** Load for rr_node[211] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=20, type=4 *****
Xchan_mux_1level_tapbuf_size2[60]->out_loadlvl[0]_out mux_1level_tapbuf_size2[60]->out mux_1level_tapbuf_size2[60]->out_loadlvl[0]_out mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[233]_no0 mux_1level_tapbuf_size2[60]->out_loadlvl[0]_out mux_1level_tapbuf_size2[60]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[234]_no0 mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[235]_no0 mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[236]_no0 mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[237]_no0 mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to60] 
+          param='sum_leakage_power_sb_mux[0to59]+leakage_sb_mux[0][0]_rrnode[211]'
.meas tran sum_energy_per_cycle_sb_mux[0to60] 
+          param='sum_energy_per_cycle_sb_mux[0to59]+energy_per_cycle_sb_mux[0][0]_rrnode[211]'
Xmux_1level_tapbuf_size2[61] mux_1level_tapbuf_size2[61]->in[0] mux_1level_tapbuf_size2[61]->in[1] mux_1level_tapbuf_size2[61]->out sram[61]->outb sram[61]->out gvdd_mux_1level_tapbuf_size2[61] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[61], level=1, select_path_id=0. *****
*****1*****
Xsram[61] sram->in sram[61]->out sram[61]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[61]->out) 0
.nodeset V(sram[61]->outb) vsp
***** Signal mux_1level_tapbuf_size2[61]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[61]->in[0] mux_1level_tapbuf_size2[61]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[61]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[61]->in[1] mux_1level_tapbuf_size2[61]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[61] gvdd_mux_1level_tapbuf_size2[61] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[213] trig v(mux_1level_tapbuf_size2[61]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[61]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[213] trig v(mux_1level_tapbuf_size2[61]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[61]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[213] when v(mux_1level_tapbuf_size2[61]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[213] trig v(mux_1level_tapbuf_size2[61]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[61]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[213] when v(mux_1level_tapbuf_size2[61]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[213] trig v(mux_1level_tapbuf_size2[61]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[61]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[61]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[61]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[213] param='mux_1level_tapbuf_size2[61]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[61]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[61]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[61]_energy_per_cycle param='mux_1level_tapbuf_size2[61]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[213]  param='mux_1level_tapbuf_size2[61]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[213]  param='dynamic_power_sb_mux[0][0]_rrnode[213]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[213] avg p(Vgvdd_mux_1level_tapbuf_size2[61]) from='start_rise_sb_mux[0][0]_rrnode[213]' to='start_rise_sb_mux[0][0]_rrnode[213]+switch_rise_sb_mux[0][0]_rrnode[213]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[213] avg p(Vgvdd_mux_1level_tapbuf_size2[61]) from='start_fall_sb_mux[0][0]_rrnode[213]' to='start_fall_sb_mux[0][0]_rrnode[213]+switch_fall_sb_mux[0][0]_rrnode[213]'
.meas tran sum_leakage_power_mux[0to61] 
+          param='sum_leakage_power_mux[0to60]+leakage_sb_mux[0][0]_rrnode[213]'
.meas tran sum_energy_per_cycle_mux[0to61] 
+          param='sum_energy_per_cycle_mux[0to60]+energy_per_cycle_sb_mux[0][0]_rrnode[213]'
***** Load for rr_node[213] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=22, type=4 *****
Xchan_mux_1level_tapbuf_size2[61]->out_loadlvl[0]_out mux_1level_tapbuf_size2[61]->out mux_1level_tapbuf_size2[61]->out_loadlvl[0]_out mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[238]_no0 mux_1level_tapbuf_size2[61]->out_loadlvl[0]_out mux_1level_tapbuf_size2[61]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[239]_no0 mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[240]_no0 mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to61] 
+          param='sum_leakage_power_sb_mux[0to60]+leakage_sb_mux[0][0]_rrnode[213]'
.meas tran sum_energy_per_cycle_sb_mux[0to61] 
+          param='sum_energy_per_cycle_sb_mux[0to60]+energy_per_cycle_sb_mux[0][0]_rrnode[213]'
Xmux_1level_tapbuf_size2[62] mux_1level_tapbuf_size2[62]->in[0] mux_1level_tapbuf_size2[62]->in[1] mux_1level_tapbuf_size2[62]->out sram[62]->outb sram[62]->out gvdd_mux_1level_tapbuf_size2[62] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[62], level=1, select_path_id=0. *****
*****1*****
Xsram[62] sram->in sram[62]->out sram[62]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[62]->out) 0
.nodeset V(sram[62]->outb) vsp
***** Signal mux_1level_tapbuf_size2[62]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[62]->in[0] mux_1level_tapbuf_size2[62]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[62]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[62]->in[1] mux_1level_tapbuf_size2[62]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[62] gvdd_mux_1level_tapbuf_size2[62] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[215] trig v(mux_1level_tapbuf_size2[62]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[62]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[215] trig v(mux_1level_tapbuf_size2[62]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[62]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[215] when v(mux_1level_tapbuf_size2[62]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[215] trig v(mux_1level_tapbuf_size2[62]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[62]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[215] when v(mux_1level_tapbuf_size2[62]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[215] trig v(mux_1level_tapbuf_size2[62]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[62]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[62]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[62]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[215] param='mux_1level_tapbuf_size2[62]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[62]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[62]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[62]_energy_per_cycle param='mux_1level_tapbuf_size2[62]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[215]  param='mux_1level_tapbuf_size2[62]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[215]  param='dynamic_power_sb_mux[0][0]_rrnode[215]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[215] avg p(Vgvdd_mux_1level_tapbuf_size2[62]) from='start_rise_sb_mux[0][0]_rrnode[215]' to='start_rise_sb_mux[0][0]_rrnode[215]+switch_rise_sb_mux[0][0]_rrnode[215]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[215] avg p(Vgvdd_mux_1level_tapbuf_size2[62]) from='start_fall_sb_mux[0][0]_rrnode[215]' to='start_fall_sb_mux[0][0]_rrnode[215]+switch_fall_sb_mux[0][0]_rrnode[215]'
.meas tran sum_leakage_power_mux[0to62] 
+          param='sum_leakage_power_mux[0to61]+leakage_sb_mux[0][0]_rrnode[215]'
.meas tran sum_energy_per_cycle_mux[0to62] 
+          param='sum_energy_per_cycle_mux[0to61]+energy_per_cycle_sb_mux[0][0]_rrnode[215]'
***** Load for rr_node[215] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=24, type=4 *****
Xchan_mux_1level_tapbuf_size2[62]->out_loadlvl[0]_out mux_1level_tapbuf_size2[62]->out mux_1level_tapbuf_size2[62]->out_loadlvl[0]_out mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[241]_no0 mux_1level_tapbuf_size2[62]->out_loadlvl[0]_out mux_1level_tapbuf_size2[62]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[242]_no0 mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[243]_no0 mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[244]_no0 mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to62] 
+          param='sum_leakage_power_sb_mux[0to61]+leakage_sb_mux[0][0]_rrnode[215]'
.meas tran sum_energy_per_cycle_sb_mux[0to62] 
+          param='sum_energy_per_cycle_sb_mux[0to61]+energy_per_cycle_sb_mux[0][0]_rrnode[215]'
Xmux_1level_tapbuf_size2[63] mux_1level_tapbuf_size2[63]->in[0] mux_1level_tapbuf_size2[63]->in[1] mux_1level_tapbuf_size2[63]->out sram[63]->outb sram[63]->out gvdd_mux_1level_tapbuf_size2[63] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[63], level=1, select_path_id=0. *****
*****1*****
Xsram[63] sram->in sram[63]->out sram[63]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[63]->out) 0
.nodeset V(sram[63]->outb) vsp
***** Signal mux_1level_tapbuf_size2[63]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[63]->in[0] mux_1level_tapbuf_size2[63]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[63]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[63]->in[1] mux_1level_tapbuf_size2[63]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[63] gvdd_mux_1level_tapbuf_size2[63] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[217] trig v(mux_1level_tapbuf_size2[63]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[63]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[217] trig v(mux_1level_tapbuf_size2[63]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[63]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[217] when v(mux_1level_tapbuf_size2[63]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[217] trig v(mux_1level_tapbuf_size2[63]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[63]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[217] when v(mux_1level_tapbuf_size2[63]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[217] trig v(mux_1level_tapbuf_size2[63]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[63]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[63]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[63]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[217] param='mux_1level_tapbuf_size2[63]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[63]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[63]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[63]_energy_per_cycle param='mux_1level_tapbuf_size2[63]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[217]  param='mux_1level_tapbuf_size2[63]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[217]  param='dynamic_power_sb_mux[0][0]_rrnode[217]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[217] avg p(Vgvdd_mux_1level_tapbuf_size2[63]) from='start_rise_sb_mux[0][0]_rrnode[217]' to='start_rise_sb_mux[0][0]_rrnode[217]+switch_rise_sb_mux[0][0]_rrnode[217]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[217] avg p(Vgvdd_mux_1level_tapbuf_size2[63]) from='start_fall_sb_mux[0][0]_rrnode[217]' to='start_fall_sb_mux[0][0]_rrnode[217]+switch_fall_sb_mux[0][0]_rrnode[217]'
.meas tran sum_leakage_power_mux[0to63] 
+          param='sum_leakage_power_mux[0to62]+leakage_sb_mux[0][0]_rrnode[217]'
.meas tran sum_energy_per_cycle_mux[0to63] 
+          param='sum_energy_per_cycle_mux[0to62]+energy_per_cycle_sb_mux[0][0]_rrnode[217]'
***** Load for rr_node[217] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=26, type=4 *****
Xchan_mux_1level_tapbuf_size2[63]->out_loadlvl[0]_out mux_1level_tapbuf_size2[63]->out mux_1level_tapbuf_size2[63]->out_loadlvl[0]_out mux_1level_tapbuf_size2[63]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[245]_no0 mux_1level_tapbuf_size2[63]->out_loadlvl[0]_out mux_1level_tapbuf_size2[63]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[246]_no0 mux_1level_tapbuf_size2[63]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[63]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[247]_no0 mux_1level_tapbuf_size2[63]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[63]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to63] 
+          param='sum_leakage_power_sb_mux[0to62]+leakage_sb_mux[0][0]_rrnode[217]'
.meas tran sum_energy_per_cycle_sb_mux[0to63] 
+          param='sum_energy_per_cycle_sb_mux[0to62]+energy_per_cycle_sb_mux[0][0]_rrnode[217]'
Xmux_1level_tapbuf_size2[64] mux_1level_tapbuf_size2[64]->in[0] mux_1level_tapbuf_size2[64]->in[1] mux_1level_tapbuf_size2[64]->out sram[64]->outb sram[64]->out gvdd_mux_1level_tapbuf_size2[64] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[64], level=1, select_path_id=0. *****
*****1*****
Xsram[64] sram->in sram[64]->out sram[64]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[64]->out) 0
.nodeset V(sram[64]->outb) vsp
***** Signal mux_1level_tapbuf_size2[64]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[64]->in[0] mux_1level_tapbuf_size2[64]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[64]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[64]->in[1] mux_1level_tapbuf_size2[64]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[64] gvdd_mux_1level_tapbuf_size2[64] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[219] trig v(mux_1level_tapbuf_size2[64]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[64]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[219] trig v(mux_1level_tapbuf_size2[64]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[64]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[219] when v(mux_1level_tapbuf_size2[64]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[219] trig v(mux_1level_tapbuf_size2[64]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[64]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[219] when v(mux_1level_tapbuf_size2[64]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[219] trig v(mux_1level_tapbuf_size2[64]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[64]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[64]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[64]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[219] param='mux_1level_tapbuf_size2[64]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[64]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[64]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[64]_energy_per_cycle param='mux_1level_tapbuf_size2[64]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[219]  param='mux_1level_tapbuf_size2[64]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[219]  param='dynamic_power_sb_mux[0][0]_rrnode[219]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[219] avg p(Vgvdd_mux_1level_tapbuf_size2[64]) from='start_rise_sb_mux[0][0]_rrnode[219]' to='start_rise_sb_mux[0][0]_rrnode[219]+switch_rise_sb_mux[0][0]_rrnode[219]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[219] avg p(Vgvdd_mux_1level_tapbuf_size2[64]) from='start_fall_sb_mux[0][0]_rrnode[219]' to='start_fall_sb_mux[0][0]_rrnode[219]+switch_fall_sb_mux[0][0]_rrnode[219]'
.meas tran sum_leakage_power_mux[0to64] 
+          param='sum_leakage_power_mux[0to63]+leakage_sb_mux[0][0]_rrnode[219]'
.meas tran sum_energy_per_cycle_mux[0to64] 
+          param='sum_energy_per_cycle_mux[0to63]+energy_per_cycle_sb_mux[0][0]_rrnode[219]'
***** Load for rr_node[219] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=28, type=4 *****
Xchan_mux_1level_tapbuf_size2[64]->out_loadlvl[0]_out mux_1level_tapbuf_size2[64]->out mux_1level_tapbuf_size2[64]->out_loadlvl[0]_out mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[248]_no0 mux_1level_tapbuf_size2[64]->out_loadlvl[0]_out mux_1level_tapbuf_size2[64]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[249]_no0 mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[250]_no0 mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[251]_no0 mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[252]_no0 mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to64] 
+          param='sum_leakage_power_sb_mux[0to63]+leakage_sb_mux[0][0]_rrnode[219]'
.meas tran sum_energy_per_cycle_sb_mux[0to64] 
+          param='sum_energy_per_cycle_sb_mux[0to63]+energy_per_cycle_sb_mux[0][0]_rrnode[219]'
Xmux_1level_tapbuf_size2[65] mux_1level_tapbuf_size2[65]->in[0] mux_1level_tapbuf_size2[65]->in[1] mux_1level_tapbuf_size2[65]->out sram[65]->outb sram[65]->out gvdd_mux_1level_tapbuf_size2[65] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[65], level=1, select_path_id=0. *****
*****1*****
Xsram[65] sram->in sram[65]->out sram[65]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[65]->out) 0
.nodeset V(sram[65]->outb) vsp
***** Signal mux_1level_tapbuf_size2[65]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[65]->in[0] mux_1level_tapbuf_size2[65]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[65]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[65]->in[1] mux_1level_tapbuf_size2[65]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[65] gvdd_mux_1level_tapbuf_size2[65] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[221] trig v(mux_1level_tapbuf_size2[65]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[65]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[221] trig v(mux_1level_tapbuf_size2[65]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[65]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[221] when v(mux_1level_tapbuf_size2[65]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[221] trig v(mux_1level_tapbuf_size2[65]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[65]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[221] when v(mux_1level_tapbuf_size2[65]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[221] trig v(mux_1level_tapbuf_size2[65]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[65]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[65]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[65]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[221] param='mux_1level_tapbuf_size2[65]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[65]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[65]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[65]_energy_per_cycle param='mux_1level_tapbuf_size2[65]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[221]  param='mux_1level_tapbuf_size2[65]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[221]  param='dynamic_power_sb_mux[0][0]_rrnode[221]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[221] avg p(Vgvdd_mux_1level_tapbuf_size2[65]) from='start_rise_sb_mux[0][0]_rrnode[221]' to='start_rise_sb_mux[0][0]_rrnode[221]+switch_rise_sb_mux[0][0]_rrnode[221]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[221] avg p(Vgvdd_mux_1level_tapbuf_size2[65]) from='start_fall_sb_mux[0][0]_rrnode[221]' to='start_fall_sb_mux[0][0]_rrnode[221]+switch_fall_sb_mux[0][0]_rrnode[221]'
.meas tran sum_leakage_power_mux[0to65] 
+          param='sum_leakage_power_mux[0to64]+leakage_sb_mux[0][0]_rrnode[221]'
.meas tran sum_energy_per_cycle_mux[0to65] 
+          param='sum_energy_per_cycle_mux[0to64]+energy_per_cycle_sb_mux[0][0]_rrnode[221]'
***** Load for rr_node[221] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=30, type=4 *****
Xchan_mux_1level_tapbuf_size2[65]->out_loadlvl[0]_out mux_1level_tapbuf_size2[65]->out mux_1level_tapbuf_size2[65]->out_loadlvl[0]_out mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[253]_no0 mux_1level_tapbuf_size2[65]->out_loadlvl[0]_out mux_1level_tapbuf_size2[65]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[254]_no0 mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[255]_no0 mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[256]_no0 mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to65] 
+          param='sum_leakage_power_sb_mux[0to64]+leakage_sb_mux[0][0]_rrnode[221]'
.meas tran sum_energy_per_cycle_sb_mux[0to65] 
+          param='sum_energy_per_cycle_sb_mux[0to64]+energy_per_cycle_sb_mux[0][0]_rrnode[221]'
Xmux_1level_tapbuf_size2[66] mux_1level_tapbuf_size2[66]->in[0] mux_1level_tapbuf_size2[66]->in[1] mux_1level_tapbuf_size2[66]->out sram[66]->outb sram[66]->out gvdd_mux_1level_tapbuf_size2[66] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[66], level=1, select_path_id=0. *****
*****1*****
Xsram[66] sram->in sram[66]->out sram[66]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[66]->out) 0
.nodeset V(sram[66]->outb) vsp
***** Signal mux_1level_tapbuf_size2[66]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[66]->in[0] mux_1level_tapbuf_size2[66]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[66]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[66]->in[1] mux_1level_tapbuf_size2[66]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[66] gvdd_mux_1level_tapbuf_size2[66] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[223] trig v(mux_1level_tapbuf_size2[66]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[66]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[223] trig v(mux_1level_tapbuf_size2[66]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[66]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[223] when v(mux_1level_tapbuf_size2[66]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[223] trig v(mux_1level_tapbuf_size2[66]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[66]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[223] when v(mux_1level_tapbuf_size2[66]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[223] trig v(mux_1level_tapbuf_size2[66]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[66]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[66]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[66]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[223] param='mux_1level_tapbuf_size2[66]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[66]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[66]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[66]_energy_per_cycle param='mux_1level_tapbuf_size2[66]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[223]  param='mux_1level_tapbuf_size2[66]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[223]  param='dynamic_power_sb_mux[0][0]_rrnode[223]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[223] avg p(Vgvdd_mux_1level_tapbuf_size2[66]) from='start_rise_sb_mux[0][0]_rrnode[223]' to='start_rise_sb_mux[0][0]_rrnode[223]+switch_rise_sb_mux[0][0]_rrnode[223]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[223] avg p(Vgvdd_mux_1level_tapbuf_size2[66]) from='start_fall_sb_mux[0][0]_rrnode[223]' to='start_fall_sb_mux[0][0]_rrnode[223]+switch_fall_sb_mux[0][0]_rrnode[223]'
.meas tran sum_leakage_power_mux[0to66] 
+          param='sum_leakage_power_mux[0to65]+leakage_sb_mux[0][0]_rrnode[223]'
.meas tran sum_energy_per_cycle_mux[0to66] 
+          param='sum_energy_per_cycle_mux[0to65]+energy_per_cycle_sb_mux[0][0]_rrnode[223]'
***** Load for rr_node[223] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=32, type=4 *****
Xchan_mux_1level_tapbuf_size2[66]->out_loadlvl[0]_out mux_1level_tapbuf_size2[66]->out mux_1level_tapbuf_size2[66]->out_loadlvl[0]_out mux_1level_tapbuf_size2[66]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[257]_no0 mux_1level_tapbuf_size2[66]->out_loadlvl[0]_out mux_1level_tapbuf_size2[66]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[258]_no0 mux_1level_tapbuf_size2[66]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[66]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[259]_no0 mux_1level_tapbuf_size2[66]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[66]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to66] 
+          param='sum_leakage_power_sb_mux[0to65]+leakage_sb_mux[0][0]_rrnode[223]'
.meas tran sum_energy_per_cycle_sb_mux[0to66] 
+          param='sum_energy_per_cycle_sb_mux[0to65]+energy_per_cycle_sb_mux[0][0]_rrnode[223]'
Xmux_1level_tapbuf_size2[67] mux_1level_tapbuf_size2[67]->in[0] mux_1level_tapbuf_size2[67]->in[1] mux_1level_tapbuf_size2[67]->out sram[67]->outb sram[67]->out gvdd_mux_1level_tapbuf_size2[67] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[67], level=1, select_path_id=0. *****
*****1*****
Xsram[67] sram->in sram[67]->out sram[67]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[67]->out) 0
.nodeset V(sram[67]->outb) vsp
***** Signal mux_1level_tapbuf_size2[67]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[67]->in[0] mux_1level_tapbuf_size2[67]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[67]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[67]->in[1] mux_1level_tapbuf_size2[67]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[67] gvdd_mux_1level_tapbuf_size2[67] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[225] trig v(mux_1level_tapbuf_size2[67]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[67]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[225] trig v(mux_1level_tapbuf_size2[67]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[67]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[225] when v(mux_1level_tapbuf_size2[67]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[225] trig v(mux_1level_tapbuf_size2[67]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[67]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[225] when v(mux_1level_tapbuf_size2[67]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[225] trig v(mux_1level_tapbuf_size2[67]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[67]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[67]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[67]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[225] param='mux_1level_tapbuf_size2[67]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[67]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[67]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[67]_energy_per_cycle param='mux_1level_tapbuf_size2[67]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[225]  param='mux_1level_tapbuf_size2[67]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[225]  param='dynamic_power_sb_mux[0][0]_rrnode[225]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[225] avg p(Vgvdd_mux_1level_tapbuf_size2[67]) from='start_rise_sb_mux[0][0]_rrnode[225]' to='start_rise_sb_mux[0][0]_rrnode[225]+switch_rise_sb_mux[0][0]_rrnode[225]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[225] avg p(Vgvdd_mux_1level_tapbuf_size2[67]) from='start_fall_sb_mux[0][0]_rrnode[225]' to='start_fall_sb_mux[0][0]_rrnode[225]+switch_fall_sb_mux[0][0]_rrnode[225]'
.meas tran sum_leakage_power_mux[0to67] 
+          param='sum_leakage_power_mux[0to66]+leakage_sb_mux[0][0]_rrnode[225]'
.meas tran sum_energy_per_cycle_mux[0to67] 
+          param='sum_energy_per_cycle_mux[0to66]+energy_per_cycle_sb_mux[0][0]_rrnode[225]'
***** Load for rr_node[225] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=34, type=4 *****
Xchan_mux_1level_tapbuf_size2[67]->out_loadlvl[0]_out mux_1level_tapbuf_size2[67]->out mux_1level_tapbuf_size2[67]->out_loadlvl[0]_out mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[260]_no0 mux_1level_tapbuf_size2[67]->out_loadlvl[0]_out mux_1level_tapbuf_size2[67]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[261]_no0 mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[262]_no0 mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[263]_no0 mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[264]_no0 mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to67] 
+          param='sum_leakage_power_sb_mux[0to66]+leakage_sb_mux[0][0]_rrnode[225]'
.meas tran sum_energy_per_cycle_sb_mux[0to67] 
+          param='sum_energy_per_cycle_sb_mux[0to66]+energy_per_cycle_sb_mux[0][0]_rrnode[225]'
Xmux_1level_tapbuf_size2[68] mux_1level_tapbuf_size2[68]->in[0] mux_1level_tapbuf_size2[68]->in[1] mux_1level_tapbuf_size2[68]->out sram[68]->outb sram[68]->out gvdd_mux_1level_tapbuf_size2[68] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[68], level=1, select_path_id=0. *****
*****1*****
Xsram[68] sram->in sram[68]->out sram[68]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[68]->out) 0
.nodeset V(sram[68]->outb) vsp
***** Signal mux_1level_tapbuf_size2[68]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[68]->in[0] mux_1level_tapbuf_size2[68]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[68]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[68]->in[1] mux_1level_tapbuf_size2[68]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[68] gvdd_mux_1level_tapbuf_size2[68] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[227] trig v(mux_1level_tapbuf_size2[68]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[68]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[227] trig v(mux_1level_tapbuf_size2[68]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[68]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[227] when v(mux_1level_tapbuf_size2[68]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[227] trig v(mux_1level_tapbuf_size2[68]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[68]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[227] when v(mux_1level_tapbuf_size2[68]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[227] trig v(mux_1level_tapbuf_size2[68]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[68]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[68]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[68]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[227] param='mux_1level_tapbuf_size2[68]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[68]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[68]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[68]_energy_per_cycle param='mux_1level_tapbuf_size2[68]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[227]  param='mux_1level_tapbuf_size2[68]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[227]  param='dynamic_power_sb_mux[0][0]_rrnode[227]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[227] avg p(Vgvdd_mux_1level_tapbuf_size2[68]) from='start_rise_sb_mux[0][0]_rrnode[227]' to='start_rise_sb_mux[0][0]_rrnode[227]+switch_rise_sb_mux[0][0]_rrnode[227]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[227] avg p(Vgvdd_mux_1level_tapbuf_size2[68]) from='start_fall_sb_mux[0][0]_rrnode[227]' to='start_fall_sb_mux[0][0]_rrnode[227]+switch_fall_sb_mux[0][0]_rrnode[227]'
.meas tran sum_leakage_power_mux[0to68] 
+          param='sum_leakage_power_mux[0to67]+leakage_sb_mux[0][0]_rrnode[227]'
.meas tran sum_energy_per_cycle_mux[0to68] 
+          param='sum_energy_per_cycle_mux[0to67]+energy_per_cycle_sb_mux[0][0]_rrnode[227]'
***** Load for rr_node[227] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=36, type=4 *****
Xchan_mux_1level_tapbuf_size2[68]->out_loadlvl[0]_out mux_1level_tapbuf_size2[68]->out mux_1level_tapbuf_size2[68]->out_loadlvl[0]_out mux_1level_tapbuf_size2[68]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[265]_no0 mux_1level_tapbuf_size2[68]->out_loadlvl[0]_out mux_1level_tapbuf_size2[68]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[266]_no0 mux_1level_tapbuf_size2[68]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[68]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[267]_no0 mux_1level_tapbuf_size2[68]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[68]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to68] 
+          param='sum_leakage_power_sb_mux[0to67]+leakage_sb_mux[0][0]_rrnode[227]'
.meas tran sum_energy_per_cycle_sb_mux[0to68] 
+          param='sum_energy_per_cycle_sb_mux[0to67]+energy_per_cycle_sb_mux[0][0]_rrnode[227]'
Xmux_1level_tapbuf_size2[69] mux_1level_tapbuf_size2[69]->in[0] mux_1level_tapbuf_size2[69]->in[1] mux_1level_tapbuf_size2[69]->out sram[69]->outb sram[69]->out gvdd_mux_1level_tapbuf_size2[69] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[69], level=1, select_path_id=0. *****
*****1*****
Xsram[69] sram->in sram[69]->out sram[69]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[69]->out) 0
.nodeset V(sram[69]->outb) vsp
***** Signal mux_1level_tapbuf_size2[69]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[69]->in[0] mux_1level_tapbuf_size2[69]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[69]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[69]->in[1] mux_1level_tapbuf_size2[69]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[69] gvdd_mux_1level_tapbuf_size2[69] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[229] trig v(mux_1level_tapbuf_size2[69]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[69]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[229] trig v(mux_1level_tapbuf_size2[69]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[69]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[229] when v(mux_1level_tapbuf_size2[69]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[229] trig v(mux_1level_tapbuf_size2[69]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[69]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[229] when v(mux_1level_tapbuf_size2[69]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[229] trig v(mux_1level_tapbuf_size2[69]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[69]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[69]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[69]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[229] param='mux_1level_tapbuf_size2[69]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[69]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[69]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[69]_energy_per_cycle param='mux_1level_tapbuf_size2[69]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[229]  param='mux_1level_tapbuf_size2[69]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[229]  param='dynamic_power_sb_mux[0][0]_rrnode[229]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[229] avg p(Vgvdd_mux_1level_tapbuf_size2[69]) from='start_rise_sb_mux[0][0]_rrnode[229]' to='start_rise_sb_mux[0][0]_rrnode[229]+switch_rise_sb_mux[0][0]_rrnode[229]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[229] avg p(Vgvdd_mux_1level_tapbuf_size2[69]) from='start_fall_sb_mux[0][0]_rrnode[229]' to='start_fall_sb_mux[0][0]_rrnode[229]+switch_fall_sb_mux[0][0]_rrnode[229]'
.meas tran sum_leakage_power_mux[0to69] 
+          param='sum_leakage_power_mux[0to68]+leakage_sb_mux[0][0]_rrnode[229]'
.meas tran sum_energy_per_cycle_mux[0to69] 
+          param='sum_energy_per_cycle_mux[0to68]+energy_per_cycle_sb_mux[0][0]_rrnode[229]'
***** Load for rr_node[229] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=38, type=4 *****
Xchan_mux_1level_tapbuf_size2[69]->out_loadlvl[0]_out mux_1level_tapbuf_size2[69]->out mux_1level_tapbuf_size2[69]->out_loadlvl[0]_out mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[268]_no0 mux_1level_tapbuf_size2[69]->out_loadlvl[0]_out mux_1level_tapbuf_size2[69]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[269]_no0 mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[270]_no0 mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[271]_no0 mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to69] 
+          param='sum_leakage_power_sb_mux[0to68]+leakage_sb_mux[0][0]_rrnode[229]'
.meas tran sum_energy_per_cycle_sb_mux[0to69] 
+          param='sum_energy_per_cycle_sb_mux[0to68]+energy_per_cycle_sb_mux[0][0]_rrnode[229]'
Xmux_1level_tapbuf_size2[70] mux_1level_tapbuf_size2[70]->in[0] mux_1level_tapbuf_size2[70]->in[1] mux_1level_tapbuf_size2[70]->out sram[70]->outb sram[70]->out gvdd_mux_1level_tapbuf_size2[70] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[70], level=1, select_path_id=0. *****
*****1*****
Xsram[70] sram->in sram[70]->out sram[70]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[70]->out) 0
.nodeset V(sram[70]->outb) vsp
***** Signal mux_1level_tapbuf_size2[70]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[70]->in[0] mux_1level_tapbuf_size2[70]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[70]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[70]->in[1] mux_1level_tapbuf_size2[70]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[70] gvdd_mux_1level_tapbuf_size2[70] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[231] trig v(mux_1level_tapbuf_size2[70]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[70]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[231] trig v(mux_1level_tapbuf_size2[70]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[70]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[231] when v(mux_1level_tapbuf_size2[70]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[231] trig v(mux_1level_tapbuf_size2[70]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[70]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[231] when v(mux_1level_tapbuf_size2[70]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[231] trig v(mux_1level_tapbuf_size2[70]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[70]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[70]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[70]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[231] param='mux_1level_tapbuf_size2[70]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[70]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[70]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[70]_energy_per_cycle param='mux_1level_tapbuf_size2[70]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[231]  param='mux_1level_tapbuf_size2[70]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[231]  param='dynamic_power_sb_mux[0][0]_rrnode[231]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[231] avg p(Vgvdd_mux_1level_tapbuf_size2[70]) from='start_rise_sb_mux[0][0]_rrnode[231]' to='start_rise_sb_mux[0][0]_rrnode[231]+switch_rise_sb_mux[0][0]_rrnode[231]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[231] avg p(Vgvdd_mux_1level_tapbuf_size2[70]) from='start_fall_sb_mux[0][0]_rrnode[231]' to='start_fall_sb_mux[0][0]_rrnode[231]+switch_fall_sb_mux[0][0]_rrnode[231]'
.meas tran sum_leakage_power_mux[0to70] 
+          param='sum_leakage_power_mux[0to69]+leakage_sb_mux[0][0]_rrnode[231]'
.meas tran sum_energy_per_cycle_mux[0to70] 
+          param='sum_energy_per_cycle_mux[0to69]+energy_per_cycle_sb_mux[0][0]_rrnode[231]'
***** Load for rr_node[231] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=40, type=4 *****
Xchan_mux_1level_tapbuf_size2[70]->out_loadlvl[0]_out mux_1level_tapbuf_size2[70]->out mux_1level_tapbuf_size2[70]->out_loadlvl[0]_out mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[272]_no0 mux_1level_tapbuf_size2[70]->out_loadlvl[0]_out mux_1level_tapbuf_size2[70]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[273]_no0 mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[274]_no0 mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[275]_no0 mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to70] 
+          param='sum_leakage_power_sb_mux[0to69]+leakage_sb_mux[0][0]_rrnode[231]'
.meas tran sum_energy_per_cycle_sb_mux[0to70] 
+          param='sum_energy_per_cycle_sb_mux[0to69]+energy_per_cycle_sb_mux[0][0]_rrnode[231]'
Xmux_1level_tapbuf_size2[71] mux_1level_tapbuf_size2[71]->in[0] mux_1level_tapbuf_size2[71]->in[1] mux_1level_tapbuf_size2[71]->out sram[71]->outb sram[71]->out gvdd_mux_1level_tapbuf_size2[71] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[71], level=1, select_path_id=0. *****
*****1*****
Xsram[71] sram->in sram[71]->out sram[71]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[71]->out) 0
.nodeset V(sram[71]->outb) vsp
***** Signal mux_1level_tapbuf_size2[71]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[71]->in[0] mux_1level_tapbuf_size2[71]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[71]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[71]->in[1] mux_1level_tapbuf_size2[71]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[71] gvdd_mux_1level_tapbuf_size2[71] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[233] trig v(mux_1level_tapbuf_size2[71]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[71]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[233] trig v(mux_1level_tapbuf_size2[71]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[71]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[233] when v(mux_1level_tapbuf_size2[71]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[233] trig v(mux_1level_tapbuf_size2[71]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[71]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[233] when v(mux_1level_tapbuf_size2[71]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[233] trig v(mux_1level_tapbuf_size2[71]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[71]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[71]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[71]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[233] param='mux_1level_tapbuf_size2[71]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[71]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[71]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[71]_energy_per_cycle param='mux_1level_tapbuf_size2[71]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[233]  param='mux_1level_tapbuf_size2[71]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[233]  param='dynamic_power_sb_mux[0][0]_rrnode[233]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[233] avg p(Vgvdd_mux_1level_tapbuf_size2[71]) from='start_rise_sb_mux[0][0]_rrnode[233]' to='start_rise_sb_mux[0][0]_rrnode[233]+switch_rise_sb_mux[0][0]_rrnode[233]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[233] avg p(Vgvdd_mux_1level_tapbuf_size2[71]) from='start_fall_sb_mux[0][0]_rrnode[233]' to='start_fall_sb_mux[0][0]_rrnode[233]+switch_fall_sb_mux[0][0]_rrnode[233]'
.meas tran sum_leakage_power_mux[0to71] 
+          param='sum_leakage_power_mux[0to70]+leakage_sb_mux[0][0]_rrnode[233]'
.meas tran sum_energy_per_cycle_mux[0to71] 
+          param='sum_energy_per_cycle_mux[0to70]+energy_per_cycle_sb_mux[0][0]_rrnode[233]'
***** Load for rr_node[233] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=42, type=4 *****
Xchan_mux_1level_tapbuf_size2[71]->out_loadlvl[0]_out mux_1level_tapbuf_size2[71]->out mux_1level_tapbuf_size2[71]->out_loadlvl[0]_out mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[276]_no0 mux_1level_tapbuf_size2[71]->out_loadlvl[0]_out mux_1level_tapbuf_size2[71]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[277]_no0 mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[278]_no0 mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[279]_no0 mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to71] 
+          param='sum_leakage_power_sb_mux[0to70]+leakage_sb_mux[0][0]_rrnode[233]'
.meas tran sum_energy_per_cycle_sb_mux[0to71] 
+          param='sum_energy_per_cycle_sb_mux[0to70]+energy_per_cycle_sb_mux[0][0]_rrnode[233]'
Xmux_1level_tapbuf_size2[72] mux_1level_tapbuf_size2[72]->in[0] mux_1level_tapbuf_size2[72]->in[1] mux_1level_tapbuf_size2[72]->out sram[72]->outb sram[72]->out gvdd_mux_1level_tapbuf_size2[72] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[72], level=1, select_path_id=0. *****
*****1*****
Xsram[72] sram->in sram[72]->out sram[72]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[72]->out) 0
.nodeset V(sram[72]->outb) vsp
***** Signal mux_1level_tapbuf_size2[72]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[72]->in[0] mux_1level_tapbuf_size2[72]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[72]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[72]->in[1] mux_1level_tapbuf_size2[72]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[72] gvdd_mux_1level_tapbuf_size2[72] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[235] trig v(mux_1level_tapbuf_size2[72]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[72]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[235] trig v(mux_1level_tapbuf_size2[72]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[72]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[235] when v(mux_1level_tapbuf_size2[72]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[235] trig v(mux_1level_tapbuf_size2[72]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[72]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[235] when v(mux_1level_tapbuf_size2[72]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[235] trig v(mux_1level_tapbuf_size2[72]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[72]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[72]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[72]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[235] param='mux_1level_tapbuf_size2[72]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[72]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[72]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[72]_energy_per_cycle param='mux_1level_tapbuf_size2[72]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[235]  param='mux_1level_tapbuf_size2[72]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[235]  param='dynamic_power_sb_mux[0][0]_rrnode[235]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[235] avg p(Vgvdd_mux_1level_tapbuf_size2[72]) from='start_rise_sb_mux[0][0]_rrnode[235]' to='start_rise_sb_mux[0][0]_rrnode[235]+switch_rise_sb_mux[0][0]_rrnode[235]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[235] avg p(Vgvdd_mux_1level_tapbuf_size2[72]) from='start_fall_sb_mux[0][0]_rrnode[235]' to='start_fall_sb_mux[0][0]_rrnode[235]+switch_fall_sb_mux[0][0]_rrnode[235]'
.meas tran sum_leakage_power_mux[0to72] 
+          param='sum_leakage_power_mux[0to71]+leakage_sb_mux[0][0]_rrnode[235]'
.meas tran sum_energy_per_cycle_mux[0to72] 
+          param='sum_energy_per_cycle_mux[0to71]+energy_per_cycle_sb_mux[0][0]_rrnode[235]'
***** Load for rr_node[235] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=44, type=4 *****
Xchan_mux_1level_tapbuf_size2[72]->out_loadlvl[0]_out mux_1level_tapbuf_size2[72]->out mux_1level_tapbuf_size2[72]->out_loadlvl[0]_out mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[280]_no0 mux_1level_tapbuf_size2[72]->out_loadlvl[0]_out mux_1level_tapbuf_size2[72]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[281]_no0 mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[282]_no0 mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[283]_no0 mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to72] 
+          param='sum_leakage_power_sb_mux[0to71]+leakage_sb_mux[0][0]_rrnode[235]'
.meas tran sum_energy_per_cycle_sb_mux[0to72] 
+          param='sum_energy_per_cycle_sb_mux[0to71]+energy_per_cycle_sb_mux[0][0]_rrnode[235]'
Xmux_1level_tapbuf_size2[73] mux_1level_tapbuf_size2[73]->in[0] mux_1level_tapbuf_size2[73]->in[1] mux_1level_tapbuf_size2[73]->out sram[73]->outb sram[73]->out gvdd_mux_1level_tapbuf_size2[73] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[73], level=1, select_path_id=0. *****
*****1*****
Xsram[73] sram->in sram[73]->out sram[73]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[73]->out) 0
.nodeset V(sram[73]->outb) vsp
***** Signal mux_1level_tapbuf_size2[73]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[73]->in[0] mux_1level_tapbuf_size2[73]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[73]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[73]->in[1] mux_1level_tapbuf_size2[73]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[73] gvdd_mux_1level_tapbuf_size2[73] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[237] trig v(mux_1level_tapbuf_size2[73]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[73]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[237] trig v(mux_1level_tapbuf_size2[73]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[73]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[237] when v(mux_1level_tapbuf_size2[73]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[237] trig v(mux_1level_tapbuf_size2[73]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[73]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[237] when v(mux_1level_tapbuf_size2[73]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[237] trig v(mux_1level_tapbuf_size2[73]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[73]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[73]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[73]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[237] param='mux_1level_tapbuf_size2[73]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[73]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[73]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[73]_energy_per_cycle param='mux_1level_tapbuf_size2[73]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[237]  param='mux_1level_tapbuf_size2[73]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[237]  param='dynamic_power_sb_mux[0][0]_rrnode[237]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[237] avg p(Vgvdd_mux_1level_tapbuf_size2[73]) from='start_rise_sb_mux[0][0]_rrnode[237]' to='start_rise_sb_mux[0][0]_rrnode[237]+switch_rise_sb_mux[0][0]_rrnode[237]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[237] avg p(Vgvdd_mux_1level_tapbuf_size2[73]) from='start_fall_sb_mux[0][0]_rrnode[237]' to='start_fall_sb_mux[0][0]_rrnode[237]+switch_fall_sb_mux[0][0]_rrnode[237]'
.meas tran sum_leakage_power_mux[0to73] 
+          param='sum_leakage_power_mux[0to72]+leakage_sb_mux[0][0]_rrnode[237]'
.meas tran sum_energy_per_cycle_mux[0to73] 
+          param='sum_energy_per_cycle_mux[0to72]+energy_per_cycle_sb_mux[0][0]_rrnode[237]'
***** Load for rr_node[237] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=46, type=4 *****
Xchan_mux_1level_tapbuf_size2[73]->out_loadlvl[0]_out mux_1level_tapbuf_size2[73]->out mux_1level_tapbuf_size2[73]->out_loadlvl[0]_out mux_1level_tapbuf_size2[73]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[284]_no0 mux_1level_tapbuf_size2[73]->out_loadlvl[0]_out mux_1level_tapbuf_size2[73]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[285]_no0 mux_1level_tapbuf_size2[73]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[73]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[286]_no0 mux_1level_tapbuf_size2[73]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[73]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to73] 
+          param='sum_leakage_power_sb_mux[0to72]+leakage_sb_mux[0][0]_rrnode[237]'
.meas tran sum_energy_per_cycle_sb_mux[0to73] 
+          param='sum_energy_per_cycle_sb_mux[0to72]+energy_per_cycle_sb_mux[0][0]_rrnode[237]'
Xmux_1level_tapbuf_size2[74] mux_1level_tapbuf_size2[74]->in[0] mux_1level_tapbuf_size2[74]->in[1] mux_1level_tapbuf_size2[74]->out sram[74]->outb sram[74]->out gvdd_mux_1level_tapbuf_size2[74] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[74], level=1, select_path_id=0. *****
*****1*****
Xsram[74] sram->in sram[74]->out sram[74]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[74]->out) 0
.nodeset V(sram[74]->outb) vsp
***** Signal mux_1level_tapbuf_size2[74]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[74]->in[0] mux_1level_tapbuf_size2[74]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[74]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[74]->in[1] mux_1level_tapbuf_size2[74]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[74] gvdd_mux_1level_tapbuf_size2[74] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[239] trig v(mux_1level_tapbuf_size2[74]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[74]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[239] trig v(mux_1level_tapbuf_size2[74]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[74]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[239] when v(mux_1level_tapbuf_size2[74]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[239] trig v(mux_1level_tapbuf_size2[74]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[74]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[239] when v(mux_1level_tapbuf_size2[74]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[239] trig v(mux_1level_tapbuf_size2[74]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[74]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[74]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[74]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[239] param='mux_1level_tapbuf_size2[74]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[74]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[74]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[74]_energy_per_cycle param='mux_1level_tapbuf_size2[74]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[239]  param='mux_1level_tapbuf_size2[74]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[239]  param='dynamic_power_sb_mux[0][0]_rrnode[239]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[239] avg p(Vgvdd_mux_1level_tapbuf_size2[74]) from='start_rise_sb_mux[0][0]_rrnode[239]' to='start_rise_sb_mux[0][0]_rrnode[239]+switch_rise_sb_mux[0][0]_rrnode[239]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[239] avg p(Vgvdd_mux_1level_tapbuf_size2[74]) from='start_fall_sb_mux[0][0]_rrnode[239]' to='start_fall_sb_mux[0][0]_rrnode[239]+switch_fall_sb_mux[0][0]_rrnode[239]'
.meas tran sum_leakage_power_mux[0to74] 
+          param='sum_leakage_power_mux[0to73]+leakage_sb_mux[0][0]_rrnode[239]'
.meas tran sum_energy_per_cycle_mux[0to74] 
+          param='sum_energy_per_cycle_mux[0to73]+energy_per_cycle_sb_mux[0][0]_rrnode[239]'
***** Load for rr_node[239] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=48, type=4 *****
Xchan_mux_1level_tapbuf_size2[74]->out_loadlvl[0]_out mux_1level_tapbuf_size2[74]->out mux_1level_tapbuf_size2[74]->out_loadlvl[0]_out mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[287]_no0 mux_1level_tapbuf_size2[74]->out_loadlvl[0]_out mux_1level_tapbuf_size2[74]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[288]_no0 mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[289]_no0 mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[290]_no0 mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to74] 
+          param='sum_leakage_power_sb_mux[0to73]+leakage_sb_mux[0][0]_rrnode[239]'
.meas tran sum_energy_per_cycle_sb_mux[0to74] 
+          param='sum_energy_per_cycle_sb_mux[0to73]+energy_per_cycle_sb_mux[0][0]_rrnode[239]'
Xmux_1level_tapbuf_size2[75] mux_1level_tapbuf_size2[75]->in[0] mux_1level_tapbuf_size2[75]->in[1] mux_1level_tapbuf_size2[75]->out sram[75]->outb sram[75]->out gvdd_mux_1level_tapbuf_size2[75] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[75], level=1, select_path_id=0. *****
*****1*****
Xsram[75] sram->in sram[75]->out sram[75]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[75]->out) 0
.nodeset V(sram[75]->outb) vsp
***** Signal mux_1level_tapbuf_size2[75]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[75]->in[0] mux_1level_tapbuf_size2[75]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[75]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[75]->in[1] mux_1level_tapbuf_size2[75]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[75] gvdd_mux_1level_tapbuf_size2[75] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[241] trig v(mux_1level_tapbuf_size2[75]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[75]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[241] trig v(mux_1level_tapbuf_size2[75]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[75]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[241] when v(mux_1level_tapbuf_size2[75]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[241] trig v(mux_1level_tapbuf_size2[75]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[75]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[241] when v(mux_1level_tapbuf_size2[75]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[241] trig v(mux_1level_tapbuf_size2[75]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[75]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[75]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[75]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[241] param='mux_1level_tapbuf_size2[75]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[75]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[75]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[75]_energy_per_cycle param='mux_1level_tapbuf_size2[75]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[241]  param='mux_1level_tapbuf_size2[75]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[241]  param='dynamic_power_sb_mux[0][0]_rrnode[241]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[241] avg p(Vgvdd_mux_1level_tapbuf_size2[75]) from='start_rise_sb_mux[0][0]_rrnode[241]' to='start_rise_sb_mux[0][0]_rrnode[241]+switch_rise_sb_mux[0][0]_rrnode[241]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[241] avg p(Vgvdd_mux_1level_tapbuf_size2[75]) from='start_fall_sb_mux[0][0]_rrnode[241]' to='start_fall_sb_mux[0][0]_rrnode[241]+switch_fall_sb_mux[0][0]_rrnode[241]'
.meas tran sum_leakage_power_mux[0to75] 
+          param='sum_leakage_power_mux[0to74]+leakage_sb_mux[0][0]_rrnode[241]'
.meas tran sum_energy_per_cycle_mux[0to75] 
+          param='sum_energy_per_cycle_mux[0to74]+energy_per_cycle_sb_mux[0][0]_rrnode[241]'
***** Load for rr_node[241] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=50, type=4 *****
Xchan_mux_1level_tapbuf_size2[75]->out_loadlvl[0]_out mux_1level_tapbuf_size2[75]->out mux_1level_tapbuf_size2[75]->out_loadlvl[0]_out mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[291]_no0 mux_1level_tapbuf_size2[75]->out_loadlvl[0]_out mux_1level_tapbuf_size2[75]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[292]_no0 mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[293]_no0 mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[294]_no0 mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[295]_no0 mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to75] 
+          param='sum_leakage_power_sb_mux[0to74]+leakage_sb_mux[0][0]_rrnode[241]'
.meas tran sum_energy_per_cycle_sb_mux[0to75] 
+          param='sum_energy_per_cycle_sb_mux[0to74]+energy_per_cycle_sb_mux[0][0]_rrnode[241]'
Xmux_1level_tapbuf_size2[76] mux_1level_tapbuf_size2[76]->in[0] mux_1level_tapbuf_size2[76]->in[1] mux_1level_tapbuf_size2[76]->out sram[76]->outb sram[76]->out gvdd_mux_1level_tapbuf_size2[76] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[76], level=1, select_path_id=0. *****
*****1*****
Xsram[76] sram->in sram[76]->out sram[76]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[76]->out) 0
.nodeset V(sram[76]->outb) vsp
***** Signal mux_1level_tapbuf_size2[76]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[76]->in[0] mux_1level_tapbuf_size2[76]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[76]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[76]->in[1] mux_1level_tapbuf_size2[76]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[76] gvdd_mux_1level_tapbuf_size2[76] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[243] trig v(mux_1level_tapbuf_size2[76]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[76]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[243] trig v(mux_1level_tapbuf_size2[76]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[76]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[243] when v(mux_1level_tapbuf_size2[76]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[243] trig v(mux_1level_tapbuf_size2[76]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[76]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[243] when v(mux_1level_tapbuf_size2[76]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[243] trig v(mux_1level_tapbuf_size2[76]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[76]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[76]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[76]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[243] param='mux_1level_tapbuf_size2[76]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[76]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[76]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[76]_energy_per_cycle param='mux_1level_tapbuf_size2[76]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[243]  param='mux_1level_tapbuf_size2[76]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[243]  param='dynamic_power_sb_mux[0][0]_rrnode[243]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[243] avg p(Vgvdd_mux_1level_tapbuf_size2[76]) from='start_rise_sb_mux[0][0]_rrnode[243]' to='start_rise_sb_mux[0][0]_rrnode[243]+switch_rise_sb_mux[0][0]_rrnode[243]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[243] avg p(Vgvdd_mux_1level_tapbuf_size2[76]) from='start_fall_sb_mux[0][0]_rrnode[243]' to='start_fall_sb_mux[0][0]_rrnode[243]+switch_fall_sb_mux[0][0]_rrnode[243]'
.meas tran sum_leakage_power_mux[0to76] 
+          param='sum_leakage_power_mux[0to75]+leakage_sb_mux[0][0]_rrnode[243]'
.meas tran sum_energy_per_cycle_mux[0to76] 
+          param='sum_energy_per_cycle_mux[0to75]+energy_per_cycle_sb_mux[0][0]_rrnode[243]'
***** Load for rr_node[243] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=52, type=4 *****
Xchan_mux_1level_tapbuf_size2[76]->out_loadlvl[0]_out mux_1level_tapbuf_size2[76]->out mux_1level_tapbuf_size2[76]->out_loadlvl[0]_out mux_1level_tapbuf_size2[76]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[296]_no0 mux_1level_tapbuf_size2[76]->out_loadlvl[0]_out mux_1level_tapbuf_size2[76]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[297]_no0 mux_1level_tapbuf_size2[76]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[76]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[298]_no0 mux_1level_tapbuf_size2[76]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[76]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to76] 
+          param='sum_leakage_power_sb_mux[0to75]+leakage_sb_mux[0][0]_rrnode[243]'
.meas tran sum_energy_per_cycle_sb_mux[0to76] 
+          param='sum_energy_per_cycle_sb_mux[0to75]+energy_per_cycle_sb_mux[0][0]_rrnode[243]'
Xmux_1level_tapbuf_size2[77] mux_1level_tapbuf_size2[77]->in[0] mux_1level_tapbuf_size2[77]->in[1] mux_1level_tapbuf_size2[77]->out sram[77]->outb sram[77]->out gvdd_mux_1level_tapbuf_size2[77] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[77], level=1, select_path_id=0. *****
*****1*****
Xsram[77] sram->in sram[77]->out sram[77]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[77]->out) 0
.nodeset V(sram[77]->outb) vsp
***** Signal mux_1level_tapbuf_size2[77]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[77]->in[0] mux_1level_tapbuf_size2[77]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[77]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[77]->in[1] mux_1level_tapbuf_size2[77]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[77] gvdd_mux_1level_tapbuf_size2[77] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[245] trig v(mux_1level_tapbuf_size2[77]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[77]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[245] trig v(mux_1level_tapbuf_size2[77]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[77]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[245] when v(mux_1level_tapbuf_size2[77]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[245] trig v(mux_1level_tapbuf_size2[77]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[77]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[245] when v(mux_1level_tapbuf_size2[77]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[245] trig v(mux_1level_tapbuf_size2[77]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[77]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[77]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[77]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[245] param='mux_1level_tapbuf_size2[77]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[77]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[77]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[77]_energy_per_cycle param='mux_1level_tapbuf_size2[77]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[245]  param='mux_1level_tapbuf_size2[77]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[245]  param='dynamic_power_sb_mux[0][0]_rrnode[245]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[245] avg p(Vgvdd_mux_1level_tapbuf_size2[77]) from='start_rise_sb_mux[0][0]_rrnode[245]' to='start_rise_sb_mux[0][0]_rrnode[245]+switch_rise_sb_mux[0][0]_rrnode[245]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[245] avg p(Vgvdd_mux_1level_tapbuf_size2[77]) from='start_fall_sb_mux[0][0]_rrnode[245]' to='start_fall_sb_mux[0][0]_rrnode[245]+switch_fall_sb_mux[0][0]_rrnode[245]'
.meas tran sum_leakage_power_mux[0to77] 
+          param='sum_leakage_power_mux[0to76]+leakage_sb_mux[0][0]_rrnode[245]'
.meas tran sum_energy_per_cycle_mux[0to77] 
+          param='sum_energy_per_cycle_mux[0to76]+energy_per_cycle_sb_mux[0][0]_rrnode[245]'
***** Load for rr_node[245] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=54, type=4 *****
Xchan_mux_1level_tapbuf_size2[77]->out_loadlvl[0]_out mux_1level_tapbuf_size2[77]->out mux_1level_tapbuf_size2[77]->out_loadlvl[0]_out mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[299]_no0 mux_1level_tapbuf_size2[77]->out_loadlvl[0]_out mux_1level_tapbuf_size2[77]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[300]_no0 mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[301]_no0 mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[302]_no0 mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to77] 
+          param='sum_leakage_power_sb_mux[0to76]+leakage_sb_mux[0][0]_rrnode[245]'
.meas tran sum_energy_per_cycle_sb_mux[0to77] 
+          param='sum_energy_per_cycle_sb_mux[0to76]+energy_per_cycle_sb_mux[0][0]_rrnode[245]'
Xmux_1level_tapbuf_size2[78] mux_1level_tapbuf_size2[78]->in[0] mux_1level_tapbuf_size2[78]->in[1] mux_1level_tapbuf_size2[78]->out sram[78]->outb sram[78]->out gvdd_mux_1level_tapbuf_size2[78] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[78], level=1, select_path_id=0. *****
*****1*****
Xsram[78] sram->in sram[78]->out sram[78]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[78]->out) 0
.nodeset V(sram[78]->outb) vsp
***** Signal mux_1level_tapbuf_size2[78]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[78]->in[0] mux_1level_tapbuf_size2[78]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[78]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[78]->in[1] mux_1level_tapbuf_size2[78]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[78] gvdd_mux_1level_tapbuf_size2[78] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[247] trig v(mux_1level_tapbuf_size2[78]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[78]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[247] trig v(mux_1level_tapbuf_size2[78]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[78]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[247] when v(mux_1level_tapbuf_size2[78]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[247] trig v(mux_1level_tapbuf_size2[78]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[78]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[247] when v(mux_1level_tapbuf_size2[78]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[247] trig v(mux_1level_tapbuf_size2[78]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[78]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[78]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[78]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[247] param='mux_1level_tapbuf_size2[78]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[78]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[78]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[78]_energy_per_cycle param='mux_1level_tapbuf_size2[78]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[247]  param='mux_1level_tapbuf_size2[78]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[247]  param='dynamic_power_sb_mux[0][0]_rrnode[247]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[247] avg p(Vgvdd_mux_1level_tapbuf_size2[78]) from='start_rise_sb_mux[0][0]_rrnode[247]' to='start_rise_sb_mux[0][0]_rrnode[247]+switch_rise_sb_mux[0][0]_rrnode[247]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[247] avg p(Vgvdd_mux_1level_tapbuf_size2[78]) from='start_fall_sb_mux[0][0]_rrnode[247]' to='start_fall_sb_mux[0][0]_rrnode[247]+switch_fall_sb_mux[0][0]_rrnode[247]'
.meas tran sum_leakage_power_mux[0to78] 
+          param='sum_leakage_power_mux[0to77]+leakage_sb_mux[0][0]_rrnode[247]'
.meas tran sum_energy_per_cycle_mux[0to78] 
+          param='sum_energy_per_cycle_mux[0to77]+energy_per_cycle_sb_mux[0][0]_rrnode[247]'
***** Load for rr_node[247] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=56, type=4 *****
Xchan_mux_1level_tapbuf_size2[78]->out_loadlvl[0]_out mux_1level_tapbuf_size2[78]->out mux_1level_tapbuf_size2[78]->out_loadlvl[0]_out mux_1level_tapbuf_size2[78]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[303]_no0 mux_1level_tapbuf_size2[78]->out_loadlvl[0]_out mux_1level_tapbuf_size2[78]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[304]_no0 mux_1level_tapbuf_size2[78]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[78]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[305]_no0 mux_1level_tapbuf_size2[78]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[78]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[306]_no0 mux_1level_tapbuf_size2[78]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[78]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to78] 
+          param='sum_leakage_power_sb_mux[0to77]+leakage_sb_mux[0][0]_rrnode[247]'
.meas tran sum_energy_per_cycle_sb_mux[0to78] 
+          param='sum_energy_per_cycle_sb_mux[0to77]+energy_per_cycle_sb_mux[0][0]_rrnode[247]'
Xmux_1level_tapbuf_size2[79] mux_1level_tapbuf_size2[79]->in[0] mux_1level_tapbuf_size2[79]->in[1] mux_1level_tapbuf_size2[79]->out sram[79]->outb sram[79]->out gvdd_mux_1level_tapbuf_size2[79] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[79], level=1, select_path_id=0. *****
*****1*****
Xsram[79] sram->in sram[79]->out sram[79]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[79]->out) 0
.nodeset V(sram[79]->outb) vsp
***** Signal mux_1level_tapbuf_size2[79]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[79]->in[0] mux_1level_tapbuf_size2[79]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[79]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[79]->in[1] mux_1level_tapbuf_size2[79]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[79] gvdd_mux_1level_tapbuf_size2[79] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[249] trig v(mux_1level_tapbuf_size2[79]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[79]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[249] trig v(mux_1level_tapbuf_size2[79]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[79]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[249] when v(mux_1level_tapbuf_size2[79]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[249] trig v(mux_1level_tapbuf_size2[79]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[79]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[249] when v(mux_1level_tapbuf_size2[79]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[249] trig v(mux_1level_tapbuf_size2[79]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[79]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[79]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[79]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[249] param='mux_1level_tapbuf_size2[79]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[79]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[79]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[79]_energy_per_cycle param='mux_1level_tapbuf_size2[79]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[249]  param='mux_1level_tapbuf_size2[79]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[249]  param='dynamic_power_sb_mux[0][0]_rrnode[249]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[249] avg p(Vgvdd_mux_1level_tapbuf_size2[79]) from='start_rise_sb_mux[0][0]_rrnode[249]' to='start_rise_sb_mux[0][0]_rrnode[249]+switch_rise_sb_mux[0][0]_rrnode[249]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[249] avg p(Vgvdd_mux_1level_tapbuf_size2[79]) from='start_fall_sb_mux[0][0]_rrnode[249]' to='start_fall_sb_mux[0][0]_rrnode[249]+switch_fall_sb_mux[0][0]_rrnode[249]'
.meas tran sum_leakage_power_mux[0to79] 
+          param='sum_leakage_power_mux[0to78]+leakage_sb_mux[0][0]_rrnode[249]'
.meas tran sum_energy_per_cycle_mux[0to79] 
+          param='sum_energy_per_cycle_mux[0to78]+energy_per_cycle_sb_mux[0][0]_rrnode[249]'
***** Load for rr_node[249] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=58, type=4 *****
Xchan_mux_1level_tapbuf_size2[79]->out_loadlvl[0]_out mux_1level_tapbuf_size2[79]->out mux_1level_tapbuf_size2[79]->out_loadlvl[0]_out mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[307]_no0 mux_1level_tapbuf_size2[79]->out_loadlvl[0]_out mux_1level_tapbuf_size2[79]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[308]_no0 mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[309]_no0 mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[310]_no0 mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to79] 
+          param='sum_leakage_power_sb_mux[0to78]+leakage_sb_mux[0][0]_rrnode[249]'
.meas tran sum_energy_per_cycle_sb_mux[0to79] 
+          param='sum_energy_per_cycle_sb_mux[0to78]+energy_per_cycle_sb_mux[0][0]_rrnode[249]'
Xmux_1level_tapbuf_size2[80] mux_1level_tapbuf_size2[80]->in[0] mux_1level_tapbuf_size2[80]->in[1] mux_1level_tapbuf_size2[80]->out sram[80]->outb sram[80]->out gvdd_mux_1level_tapbuf_size2[80] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[80], level=1, select_path_id=0. *****
*****1*****
Xsram[80] sram->in sram[80]->out sram[80]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[80]->out) 0
.nodeset V(sram[80]->outb) vsp
***** Signal mux_1level_tapbuf_size2[80]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[80]->in[0] mux_1level_tapbuf_size2[80]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[80]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[80]->in[1] mux_1level_tapbuf_size2[80]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[80] gvdd_mux_1level_tapbuf_size2[80] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[251] trig v(mux_1level_tapbuf_size2[80]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[80]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[251] trig v(mux_1level_tapbuf_size2[80]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[80]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[251] when v(mux_1level_tapbuf_size2[80]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[251] trig v(mux_1level_tapbuf_size2[80]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[80]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[251] when v(mux_1level_tapbuf_size2[80]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[251] trig v(mux_1level_tapbuf_size2[80]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[80]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[80]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[80]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[251] param='mux_1level_tapbuf_size2[80]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[80]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[80]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[80]_energy_per_cycle param='mux_1level_tapbuf_size2[80]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[251]  param='mux_1level_tapbuf_size2[80]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[251]  param='dynamic_power_sb_mux[0][0]_rrnode[251]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[251] avg p(Vgvdd_mux_1level_tapbuf_size2[80]) from='start_rise_sb_mux[0][0]_rrnode[251]' to='start_rise_sb_mux[0][0]_rrnode[251]+switch_rise_sb_mux[0][0]_rrnode[251]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[251] avg p(Vgvdd_mux_1level_tapbuf_size2[80]) from='start_fall_sb_mux[0][0]_rrnode[251]' to='start_fall_sb_mux[0][0]_rrnode[251]+switch_fall_sb_mux[0][0]_rrnode[251]'
.meas tran sum_leakage_power_mux[0to80] 
+          param='sum_leakage_power_mux[0to79]+leakage_sb_mux[0][0]_rrnode[251]'
.meas tran sum_energy_per_cycle_mux[0to80] 
+          param='sum_energy_per_cycle_mux[0to79]+energy_per_cycle_sb_mux[0][0]_rrnode[251]'
***** Load for rr_node[251] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=60, type=4 *****
Xchan_mux_1level_tapbuf_size2[80]->out_loadlvl[0]_out mux_1level_tapbuf_size2[80]->out mux_1level_tapbuf_size2[80]->out_loadlvl[0]_out mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[311]_no0 mux_1level_tapbuf_size2[80]->out_loadlvl[0]_out mux_1level_tapbuf_size2[80]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[312]_no0 mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[313]_no0 mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[314]_no0 mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to80] 
+          param='sum_leakage_power_sb_mux[0to79]+leakage_sb_mux[0][0]_rrnode[251]'
.meas tran sum_energy_per_cycle_sb_mux[0to80] 
+          param='sum_energy_per_cycle_sb_mux[0to79]+energy_per_cycle_sb_mux[0][0]_rrnode[251]'
Xmux_1level_tapbuf_size2[81] mux_1level_tapbuf_size2[81]->in[0] mux_1level_tapbuf_size2[81]->in[1] mux_1level_tapbuf_size2[81]->out sram[81]->outb sram[81]->out gvdd_mux_1level_tapbuf_size2[81] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[81], level=1, select_path_id=0. *****
*****1*****
Xsram[81] sram->in sram[81]->out sram[81]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[81]->out) 0
.nodeset V(sram[81]->outb) vsp
***** Signal mux_1level_tapbuf_size2[81]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[81]->in[0] mux_1level_tapbuf_size2[81]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[81]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[81]->in[1] mux_1level_tapbuf_size2[81]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[81] gvdd_mux_1level_tapbuf_size2[81] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[253] trig v(mux_1level_tapbuf_size2[81]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[81]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[253] trig v(mux_1level_tapbuf_size2[81]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[81]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[253] when v(mux_1level_tapbuf_size2[81]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[253] trig v(mux_1level_tapbuf_size2[81]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[81]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[253] when v(mux_1level_tapbuf_size2[81]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[253] trig v(mux_1level_tapbuf_size2[81]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[81]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[81]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[81]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[253] param='mux_1level_tapbuf_size2[81]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[81]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[81]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[81]_energy_per_cycle param='mux_1level_tapbuf_size2[81]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[253]  param='mux_1level_tapbuf_size2[81]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[253]  param='dynamic_power_sb_mux[0][0]_rrnode[253]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[253] avg p(Vgvdd_mux_1level_tapbuf_size2[81]) from='start_rise_sb_mux[0][0]_rrnode[253]' to='start_rise_sb_mux[0][0]_rrnode[253]+switch_rise_sb_mux[0][0]_rrnode[253]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[253] avg p(Vgvdd_mux_1level_tapbuf_size2[81]) from='start_fall_sb_mux[0][0]_rrnode[253]' to='start_fall_sb_mux[0][0]_rrnode[253]+switch_fall_sb_mux[0][0]_rrnode[253]'
.meas tran sum_leakage_power_mux[0to81] 
+          param='sum_leakage_power_mux[0to80]+leakage_sb_mux[0][0]_rrnode[253]'
.meas tran sum_energy_per_cycle_mux[0to81] 
+          param='sum_energy_per_cycle_mux[0to80]+energy_per_cycle_sb_mux[0][0]_rrnode[253]'
***** Load for rr_node[253] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=62, type=4 *****
Xchan_mux_1level_tapbuf_size2[81]->out_loadlvl[0]_out mux_1level_tapbuf_size2[81]->out mux_1level_tapbuf_size2[81]->out_loadlvl[0]_out mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[315]_no0 mux_1level_tapbuf_size2[81]->out_loadlvl[0]_out mux_1level_tapbuf_size2[81]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[316]_no0 mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[317]_no0 mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to81] 
+          param='sum_leakage_power_sb_mux[0to80]+leakage_sb_mux[0][0]_rrnode[253]'
.meas tran sum_energy_per_cycle_sb_mux[0to81] 
+          param='sum_energy_per_cycle_sb_mux[0to80]+energy_per_cycle_sb_mux[0][0]_rrnode[253]'
Xmux_1level_tapbuf_size2[82] mux_1level_tapbuf_size2[82]->in[0] mux_1level_tapbuf_size2[82]->in[1] mux_1level_tapbuf_size2[82]->out sram[82]->outb sram[82]->out gvdd_mux_1level_tapbuf_size2[82] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[82], level=1, select_path_id=0. *****
*****1*****
Xsram[82] sram->in sram[82]->out sram[82]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[82]->out) 0
.nodeset V(sram[82]->outb) vsp
***** Signal mux_1level_tapbuf_size2[82]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[82]->in[0] mux_1level_tapbuf_size2[82]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[82]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[82]->in[1] mux_1level_tapbuf_size2[82]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[82] gvdd_mux_1level_tapbuf_size2[82] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[255] trig v(mux_1level_tapbuf_size2[82]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[82]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[255] trig v(mux_1level_tapbuf_size2[82]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[82]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[255] when v(mux_1level_tapbuf_size2[82]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[255] trig v(mux_1level_tapbuf_size2[82]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[82]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[255] when v(mux_1level_tapbuf_size2[82]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[255] trig v(mux_1level_tapbuf_size2[82]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[82]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[82]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[82]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[255] param='mux_1level_tapbuf_size2[82]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[82]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[82]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[82]_energy_per_cycle param='mux_1level_tapbuf_size2[82]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[255]  param='mux_1level_tapbuf_size2[82]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[255]  param='dynamic_power_sb_mux[0][0]_rrnode[255]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[255] avg p(Vgvdd_mux_1level_tapbuf_size2[82]) from='start_rise_sb_mux[0][0]_rrnode[255]' to='start_rise_sb_mux[0][0]_rrnode[255]+switch_rise_sb_mux[0][0]_rrnode[255]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[255] avg p(Vgvdd_mux_1level_tapbuf_size2[82]) from='start_fall_sb_mux[0][0]_rrnode[255]' to='start_fall_sb_mux[0][0]_rrnode[255]+switch_fall_sb_mux[0][0]_rrnode[255]'
.meas tran sum_leakage_power_mux[0to82] 
+          param='sum_leakage_power_mux[0to81]+leakage_sb_mux[0][0]_rrnode[255]'
.meas tran sum_energy_per_cycle_mux[0to82] 
+          param='sum_energy_per_cycle_mux[0to81]+energy_per_cycle_sb_mux[0][0]_rrnode[255]'
***** Load for rr_node[255] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=64, type=4 *****
Xchan_mux_1level_tapbuf_size2[82]->out_loadlvl[0]_out mux_1level_tapbuf_size2[82]->out mux_1level_tapbuf_size2[82]->out_loadlvl[0]_out mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[318]_no0 mux_1level_tapbuf_size2[82]->out_loadlvl[0]_out mux_1level_tapbuf_size2[82]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[319]_no0 mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[320]_no0 mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[321]_no0 mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[322]_no0 mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to82] 
+          param='sum_leakage_power_sb_mux[0to81]+leakage_sb_mux[0][0]_rrnode[255]'
.meas tran sum_energy_per_cycle_sb_mux[0to82] 
+          param='sum_energy_per_cycle_sb_mux[0to81]+energy_per_cycle_sb_mux[0][0]_rrnode[255]'
Xmux_1level_tapbuf_size2[83] mux_1level_tapbuf_size2[83]->in[0] mux_1level_tapbuf_size2[83]->in[1] mux_1level_tapbuf_size2[83]->out sram[83]->outb sram[83]->out gvdd_mux_1level_tapbuf_size2[83] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[83], level=1, select_path_id=0. *****
*****1*****
Xsram[83] sram->in sram[83]->out sram[83]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[83]->out) 0
.nodeset V(sram[83]->outb) vsp
***** Signal mux_1level_tapbuf_size2[83]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[83]->in[0] mux_1level_tapbuf_size2[83]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[83]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[83]->in[1] mux_1level_tapbuf_size2[83]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[83] gvdd_mux_1level_tapbuf_size2[83] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[257] trig v(mux_1level_tapbuf_size2[83]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[83]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[257] trig v(mux_1level_tapbuf_size2[83]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[83]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[257] when v(mux_1level_tapbuf_size2[83]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[257] trig v(mux_1level_tapbuf_size2[83]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[83]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[257] when v(mux_1level_tapbuf_size2[83]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[257] trig v(mux_1level_tapbuf_size2[83]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[83]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[83]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[83]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[257] param='mux_1level_tapbuf_size2[83]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[83]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[83]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[83]_energy_per_cycle param='mux_1level_tapbuf_size2[83]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[257]  param='mux_1level_tapbuf_size2[83]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[257]  param='dynamic_power_sb_mux[0][0]_rrnode[257]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[257] avg p(Vgvdd_mux_1level_tapbuf_size2[83]) from='start_rise_sb_mux[0][0]_rrnode[257]' to='start_rise_sb_mux[0][0]_rrnode[257]+switch_rise_sb_mux[0][0]_rrnode[257]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[257] avg p(Vgvdd_mux_1level_tapbuf_size2[83]) from='start_fall_sb_mux[0][0]_rrnode[257]' to='start_fall_sb_mux[0][0]_rrnode[257]+switch_fall_sb_mux[0][0]_rrnode[257]'
.meas tran sum_leakage_power_mux[0to83] 
+          param='sum_leakage_power_mux[0to82]+leakage_sb_mux[0][0]_rrnode[257]'
.meas tran sum_energy_per_cycle_mux[0to83] 
+          param='sum_energy_per_cycle_mux[0to82]+energy_per_cycle_sb_mux[0][0]_rrnode[257]'
***** Load for rr_node[257] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=66, type=4 *****
Xchan_mux_1level_tapbuf_size2[83]->out_loadlvl[0]_out mux_1level_tapbuf_size2[83]->out mux_1level_tapbuf_size2[83]->out_loadlvl[0]_out mux_1level_tapbuf_size2[83]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[323]_no0 mux_1level_tapbuf_size2[83]->out_loadlvl[0]_out mux_1level_tapbuf_size2[83]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[324]_no0 mux_1level_tapbuf_size2[83]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[83]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[325]_no0 mux_1level_tapbuf_size2[83]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[83]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to83] 
+          param='sum_leakage_power_sb_mux[0to82]+leakage_sb_mux[0][0]_rrnode[257]'
.meas tran sum_energy_per_cycle_sb_mux[0to83] 
+          param='sum_energy_per_cycle_sb_mux[0to82]+energy_per_cycle_sb_mux[0][0]_rrnode[257]'
Xmux_1level_tapbuf_size2[84] mux_1level_tapbuf_size2[84]->in[0] mux_1level_tapbuf_size2[84]->in[1] mux_1level_tapbuf_size2[84]->out sram[84]->outb sram[84]->out gvdd_mux_1level_tapbuf_size2[84] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[84], level=1, select_path_id=0. *****
*****1*****
Xsram[84] sram->in sram[84]->out sram[84]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[84]->out) 0
.nodeset V(sram[84]->outb) vsp
***** Signal mux_1level_tapbuf_size2[84]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[84]->in[0] mux_1level_tapbuf_size2[84]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[84]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[84]->in[1] mux_1level_tapbuf_size2[84]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[84] gvdd_mux_1level_tapbuf_size2[84] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[259] trig v(mux_1level_tapbuf_size2[84]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[84]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[259] trig v(mux_1level_tapbuf_size2[84]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[84]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[259] when v(mux_1level_tapbuf_size2[84]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[259] trig v(mux_1level_tapbuf_size2[84]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[84]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[259] when v(mux_1level_tapbuf_size2[84]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[259] trig v(mux_1level_tapbuf_size2[84]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[84]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[84]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[84]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[259] param='mux_1level_tapbuf_size2[84]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[84]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[84]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[84]_energy_per_cycle param='mux_1level_tapbuf_size2[84]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[259]  param='mux_1level_tapbuf_size2[84]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[259]  param='dynamic_power_sb_mux[0][0]_rrnode[259]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[259] avg p(Vgvdd_mux_1level_tapbuf_size2[84]) from='start_rise_sb_mux[0][0]_rrnode[259]' to='start_rise_sb_mux[0][0]_rrnode[259]+switch_rise_sb_mux[0][0]_rrnode[259]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[259] avg p(Vgvdd_mux_1level_tapbuf_size2[84]) from='start_fall_sb_mux[0][0]_rrnode[259]' to='start_fall_sb_mux[0][0]_rrnode[259]+switch_fall_sb_mux[0][0]_rrnode[259]'
.meas tran sum_leakage_power_mux[0to84] 
+          param='sum_leakage_power_mux[0to83]+leakage_sb_mux[0][0]_rrnode[259]'
.meas tran sum_energy_per_cycle_mux[0to84] 
+          param='sum_energy_per_cycle_mux[0to83]+energy_per_cycle_sb_mux[0][0]_rrnode[259]'
***** Load for rr_node[259] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=68, type=4 *****
Xchan_mux_1level_tapbuf_size2[84]->out_loadlvl[0]_out mux_1level_tapbuf_size2[84]->out mux_1level_tapbuf_size2[84]->out_loadlvl[0]_out mux_1level_tapbuf_size2[84]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[326]_no0 mux_1level_tapbuf_size2[84]->out_loadlvl[0]_out mux_1level_tapbuf_size2[84]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[327]_no0 mux_1level_tapbuf_size2[84]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[84]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[328]_no0 mux_1level_tapbuf_size2[84]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[84]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[329]_no0 mux_1level_tapbuf_size2[84]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[84]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to84] 
+          param='sum_leakage_power_sb_mux[0to83]+leakage_sb_mux[0][0]_rrnode[259]'
.meas tran sum_energy_per_cycle_sb_mux[0to84] 
+          param='sum_energy_per_cycle_sb_mux[0to83]+energy_per_cycle_sb_mux[0][0]_rrnode[259]'
Xmux_1level_tapbuf_size2[85] mux_1level_tapbuf_size2[85]->in[0] mux_1level_tapbuf_size2[85]->in[1] mux_1level_tapbuf_size2[85]->out sram[85]->outb sram[85]->out gvdd_mux_1level_tapbuf_size2[85] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[85], level=1, select_path_id=0. *****
*****1*****
Xsram[85] sram->in sram[85]->out sram[85]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[85]->out) 0
.nodeset V(sram[85]->outb) vsp
***** Signal mux_1level_tapbuf_size2[85]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[85]->in[0] mux_1level_tapbuf_size2[85]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[85]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[85]->in[1] mux_1level_tapbuf_size2[85]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[85] gvdd_mux_1level_tapbuf_size2[85] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[261] trig v(mux_1level_tapbuf_size2[85]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[85]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[261] trig v(mux_1level_tapbuf_size2[85]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[85]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[261] when v(mux_1level_tapbuf_size2[85]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[261] trig v(mux_1level_tapbuf_size2[85]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[85]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[261] when v(mux_1level_tapbuf_size2[85]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[261] trig v(mux_1level_tapbuf_size2[85]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[85]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[85]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[85]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[261] param='mux_1level_tapbuf_size2[85]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[85]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[85]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[85]_energy_per_cycle param='mux_1level_tapbuf_size2[85]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[261]  param='mux_1level_tapbuf_size2[85]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[261]  param='dynamic_power_sb_mux[0][0]_rrnode[261]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[261] avg p(Vgvdd_mux_1level_tapbuf_size2[85]) from='start_rise_sb_mux[0][0]_rrnode[261]' to='start_rise_sb_mux[0][0]_rrnode[261]+switch_rise_sb_mux[0][0]_rrnode[261]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[261] avg p(Vgvdd_mux_1level_tapbuf_size2[85]) from='start_fall_sb_mux[0][0]_rrnode[261]' to='start_fall_sb_mux[0][0]_rrnode[261]+switch_fall_sb_mux[0][0]_rrnode[261]'
.meas tran sum_leakage_power_mux[0to85] 
+          param='sum_leakage_power_mux[0to84]+leakage_sb_mux[0][0]_rrnode[261]'
.meas tran sum_energy_per_cycle_mux[0to85] 
+          param='sum_energy_per_cycle_mux[0to84]+energy_per_cycle_sb_mux[0][0]_rrnode[261]'
***** Load for rr_node[261] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=70, type=4 *****
Xchan_mux_1level_tapbuf_size2[85]->out_loadlvl[0]_out mux_1level_tapbuf_size2[85]->out mux_1level_tapbuf_size2[85]->out_loadlvl[0]_out mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[330]_no0 mux_1level_tapbuf_size2[85]->out_loadlvl[0]_out mux_1level_tapbuf_size2[85]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[331]_no0 mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[332]_no0 mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[333]_no0 mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[334]_no0 mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to85] 
+          param='sum_leakage_power_sb_mux[0to84]+leakage_sb_mux[0][0]_rrnode[261]'
.meas tran sum_energy_per_cycle_sb_mux[0to85] 
+          param='sum_energy_per_cycle_sb_mux[0to84]+energy_per_cycle_sb_mux[0][0]_rrnode[261]'
Xmux_1level_tapbuf_size2[86] mux_1level_tapbuf_size2[86]->in[0] mux_1level_tapbuf_size2[86]->in[1] mux_1level_tapbuf_size2[86]->out sram[86]->outb sram[86]->out gvdd_mux_1level_tapbuf_size2[86] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[86], level=1, select_path_id=0. *****
*****1*****
Xsram[86] sram->in sram[86]->out sram[86]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[86]->out) 0
.nodeset V(sram[86]->outb) vsp
***** Signal mux_1level_tapbuf_size2[86]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[86]->in[0] mux_1level_tapbuf_size2[86]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[86]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[86]->in[1] mux_1level_tapbuf_size2[86]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[86] gvdd_mux_1level_tapbuf_size2[86] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[263] trig v(mux_1level_tapbuf_size2[86]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[86]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[263] trig v(mux_1level_tapbuf_size2[86]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[86]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[263] when v(mux_1level_tapbuf_size2[86]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[263] trig v(mux_1level_tapbuf_size2[86]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[86]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[263] when v(mux_1level_tapbuf_size2[86]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[263] trig v(mux_1level_tapbuf_size2[86]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[86]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[86]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[86]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[263] param='mux_1level_tapbuf_size2[86]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[86]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[86]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[86]_energy_per_cycle param='mux_1level_tapbuf_size2[86]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[263]  param='mux_1level_tapbuf_size2[86]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[263]  param='dynamic_power_sb_mux[0][0]_rrnode[263]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[263] avg p(Vgvdd_mux_1level_tapbuf_size2[86]) from='start_rise_sb_mux[0][0]_rrnode[263]' to='start_rise_sb_mux[0][0]_rrnode[263]+switch_rise_sb_mux[0][0]_rrnode[263]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[263] avg p(Vgvdd_mux_1level_tapbuf_size2[86]) from='start_fall_sb_mux[0][0]_rrnode[263]' to='start_fall_sb_mux[0][0]_rrnode[263]+switch_fall_sb_mux[0][0]_rrnode[263]'
.meas tran sum_leakage_power_mux[0to86] 
+          param='sum_leakage_power_mux[0to85]+leakage_sb_mux[0][0]_rrnode[263]'
.meas tran sum_energy_per_cycle_mux[0to86] 
+          param='sum_energy_per_cycle_mux[0to85]+energy_per_cycle_sb_mux[0][0]_rrnode[263]'
***** Load for rr_node[263] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=72, type=4 *****
Xchan_mux_1level_tapbuf_size2[86]->out_loadlvl[0]_out mux_1level_tapbuf_size2[86]->out mux_1level_tapbuf_size2[86]->out_loadlvl[0]_out mux_1level_tapbuf_size2[86]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[335]_no0 mux_1level_tapbuf_size2[86]->out_loadlvl[0]_out mux_1level_tapbuf_size2[86]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[336]_no0 mux_1level_tapbuf_size2[86]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[86]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[337]_no0 mux_1level_tapbuf_size2[86]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[86]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to86] 
+          param='sum_leakage_power_sb_mux[0to85]+leakage_sb_mux[0][0]_rrnode[263]'
.meas tran sum_energy_per_cycle_sb_mux[0to86] 
+          param='sum_energy_per_cycle_sb_mux[0to85]+energy_per_cycle_sb_mux[0][0]_rrnode[263]'
Xmux_1level_tapbuf_size2[87] mux_1level_tapbuf_size2[87]->in[0] mux_1level_tapbuf_size2[87]->in[1] mux_1level_tapbuf_size2[87]->out sram[87]->outb sram[87]->out gvdd_mux_1level_tapbuf_size2[87] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[87], level=1, select_path_id=0. *****
*****1*****
Xsram[87] sram->in sram[87]->out sram[87]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[87]->out) 0
.nodeset V(sram[87]->outb) vsp
***** Signal mux_1level_tapbuf_size2[87]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[87]->in[0] mux_1level_tapbuf_size2[87]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[87]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[87]->in[1] mux_1level_tapbuf_size2[87]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[87] gvdd_mux_1level_tapbuf_size2[87] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[265] trig v(mux_1level_tapbuf_size2[87]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[87]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[265] trig v(mux_1level_tapbuf_size2[87]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[87]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[265] when v(mux_1level_tapbuf_size2[87]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[265] trig v(mux_1level_tapbuf_size2[87]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[87]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[265] when v(mux_1level_tapbuf_size2[87]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[265] trig v(mux_1level_tapbuf_size2[87]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[87]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[87]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[87]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[265] param='mux_1level_tapbuf_size2[87]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[87]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[87]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[87]_energy_per_cycle param='mux_1level_tapbuf_size2[87]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[265]  param='mux_1level_tapbuf_size2[87]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[265]  param='dynamic_power_sb_mux[0][0]_rrnode[265]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[265] avg p(Vgvdd_mux_1level_tapbuf_size2[87]) from='start_rise_sb_mux[0][0]_rrnode[265]' to='start_rise_sb_mux[0][0]_rrnode[265]+switch_rise_sb_mux[0][0]_rrnode[265]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[265] avg p(Vgvdd_mux_1level_tapbuf_size2[87]) from='start_fall_sb_mux[0][0]_rrnode[265]' to='start_fall_sb_mux[0][0]_rrnode[265]+switch_fall_sb_mux[0][0]_rrnode[265]'
.meas tran sum_leakage_power_mux[0to87] 
+          param='sum_leakage_power_mux[0to86]+leakage_sb_mux[0][0]_rrnode[265]'
.meas tran sum_energy_per_cycle_mux[0to87] 
+          param='sum_energy_per_cycle_mux[0to86]+energy_per_cycle_sb_mux[0][0]_rrnode[265]'
***** Load for rr_node[265] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=74, type=4 *****
Xchan_mux_1level_tapbuf_size2[87]->out_loadlvl[0]_out mux_1level_tapbuf_size2[87]->out mux_1level_tapbuf_size2[87]->out_loadlvl[0]_out mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[338]_no0 mux_1level_tapbuf_size2[87]->out_loadlvl[0]_out mux_1level_tapbuf_size2[87]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[339]_no0 mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[340]_no0 mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[341]_no0 mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to87] 
+          param='sum_leakage_power_sb_mux[0to86]+leakage_sb_mux[0][0]_rrnode[265]'
.meas tran sum_energy_per_cycle_sb_mux[0to87] 
+          param='sum_energy_per_cycle_sb_mux[0to86]+energy_per_cycle_sb_mux[0][0]_rrnode[265]'
Xmux_1level_tapbuf_size2[88] mux_1level_tapbuf_size2[88]->in[0] mux_1level_tapbuf_size2[88]->in[1] mux_1level_tapbuf_size2[88]->out sram[88]->outb sram[88]->out gvdd_mux_1level_tapbuf_size2[88] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[88], level=1, select_path_id=0. *****
*****1*****
Xsram[88] sram->in sram[88]->out sram[88]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[88]->out) 0
.nodeset V(sram[88]->outb) vsp
***** Signal mux_1level_tapbuf_size2[88]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[88]->in[0] mux_1level_tapbuf_size2[88]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[88]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[88]->in[1] mux_1level_tapbuf_size2[88]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[88] gvdd_mux_1level_tapbuf_size2[88] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[267] trig v(mux_1level_tapbuf_size2[88]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[88]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[267] trig v(mux_1level_tapbuf_size2[88]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[88]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[267] when v(mux_1level_tapbuf_size2[88]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[267] trig v(mux_1level_tapbuf_size2[88]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[88]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[267] when v(mux_1level_tapbuf_size2[88]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[267] trig v(mux_1level_tapbuf_size2[88]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[88]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[88]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[88]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[267] param='mux_1level_tapbuf_size2[88]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[88]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[88]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[88]_energy_per_cycle param='mux_1level_tapbuf_size2[88]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[267]  param='mux_1level_tapbuf_size2[88]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[267]  param='dynamic_power_sb_mux[0][0]_rrnode[267]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[267] avg p(Vgvdd_mux_1level_tapbuf_size2[88]) from='start_rise_sb_mux[0][0]_rrnode[267]' to='start_rise_sb_mux[0][0]_rrnode[267]+switch_rise_sb_mux[0][0]_rrnode[267]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[267] avg p(Vgvdd_mux_1level_tapbuf_size2[88]) from='start_fall_sb_mux[0][0]_rrnode[267]' to='start_fall_sb_mux[0][0]_rrnode[267]+switch_fall_sb_mux[0][0]_rrnode[267]'
.meas tran sum_leakage_power_mux[0to88] 
+          param='sum_leakage_power_mux[0to87]+leakage_sb_mux[0][0]_rrnode[267]'
.meas tran sum_energy_per_cycle_mux[0to88] 
+          param='sum_energy_per_cycle_mux[0to87]+energy_per_cycle_sb_mux[0][0]_rrnode[267]'
***** Load for rr_node[267] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=76, type=4 *****
Xchan_mux_1level_tapbuf_size2[88]->out_loadlvl[0]_out mux_1level_tapbuf_size2[88]->out mux_1level_tapbuf_size2[88]->out_loadlvl[0]_out mux_1level_tapbuf_size2[88]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[342]_no0 mux_1level_tapbuf_size2[88]->out_loadlvl[0]_out mux_1level_tapbuf_size2[88]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[343]_no0 mux_1level_tapbuf_size2[88]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[88]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[344]_no0 mux_1level_tapbuf_size2[88]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[88]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to88] 
+          param='sum_leakage_power_sb_mux[0to87]+leakage_sb_mux[0][0]_rrnode[267]'
.meas tran sum_energy_per_cycle_sb_mux[0to88] 
+          param='sum_energy_per_cycle_sb_mux[0to87]+energy_per_cycle_sb_mux[0][0]_rrnode[267]'
Xmux_1level_tapbuf_size2[89] mux_1level_tapbuf_size2[89]->in[0] mux_1level_tapbuf_size2[89]->in[1] mux_1level_tapbuf_size2[89]->out sram[89]->outb sram[89]->out gvdd_mux_1level_tapbuf_size2[89] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[89], level=1, select_path_id=0. *****
*****1*****
Xsram[89] sram->in sram[89]->out sram[89]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[89]->out) 0
.nodeset V(sram[89]->outb) vsp
***** Signal mux_1level_tapbuf_size2[89]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[89]->in[0] mux_1level_tapbuf_size2[89]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[89]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[89]->in[1] mux_1level_tapbuf_size2[89]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[89] gvdd_mux_1level_tapbuf_size2[89] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[269] trig v(mux_1level_tapbuf_size2[89]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[89]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[269] trig v(mux_1level_tapbuf_size2[89]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[89]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[269] when v(mux_1level_tapbuf_size2[89]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[269] trig v(mux_1level_tapbuf_size2[89]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[89]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[269] when v(mux_1level_tapbuf_size2[89]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[269] trig v(mux_1level_tapbuf_size2[89]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[89]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[89]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[89]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[269] param='mux_1level_tapbuf_size2[89]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[89]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[89]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[89]_energy_per_cycle param='mux_1level_tapbuf_size2[89]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[269]  param='mux_1level_tapbuf_size2[89]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[269]  param='dynamic_power_sb_mux[0][0]_rrnode[269]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[269] avg p(Vgvdd_mux_1level_tapbuf_size2[89]) from='start_rise_sb_mux[0][0]_rrnode[269]' to='start_rise_sb_mux[0][0]_rrnode[269]+switch_rise_sb_mux[0][0]_rrnode[269]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[269] avg p(Vgvdd_mux_1level_tapbuf_size2[89]) from='start_fall_sb_mux[0][0]_rrnode[269]' to='start_fall_sb_mux[0][0]_rrnode[269]+switch_fall_sb_mux[0][0]_rrnode[269]'
.meas tran sum_leakage_power_mux[0to89] 
+          param='sum_leakage_power_mux[0to88]+leakage_sb_mux[0][0]_rrnode[269]'
.meas tran sum_energy_per_cycle_mux[0to89] 
+          param='sum_energy_per_cycle_mux[0to88]+energy_per_cycle_sb_mux[0][0]_rrnode[269]'
***** Load for rr_node[269] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=78, type=4 *****
Xchan_mux_1level_tapbuf_size2[89]->out_loadlvl[0]_out mux_1level_tapbuf_size2[89]->out mux_1level_tapbuf_size2[89]->out_loadlvl[0]_out mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[345]_no0 mux_1level_tapbuf_size2[89]->out_loadlvl[0]_out mux_1level_tapbuf_size2[89]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[346]_no0 mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[347]_no0 mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[348]_no0 mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[349]_no0 mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to89] 
+          param='sum_leakage_power_sb_mux[0to88]+leakage_sb_mux[0][0]_rrnode[269]'
.meas tran sum_energy_per_cycle_sb_mux[0to89] 
+          param='sum_energy_per_cycle_sb_mux[0to88]+energy_per_cycle_sb_mux[0][0]_rrnode[269]'
Xmux_1level_tapbuf_size2[90] mux_1level_tapbuf_size2[90]->in[0] mux_1level_tapbuf_size2[90]->in[1] mux_1level_tapbuf_size2[90]->out sram[90]->outb sram[90]->out gvdd_mux_1level_tapbuf_size2[90] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[90], level=1, select_path_id=0. *****
*****1*****
Xsram[90] sram->in sram[90]->out sram[90]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[90]->out) 0
.nodeset V(sram[90]->outb) vsp
***** Signal mux_1level_tapbuf_size2[90]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[90]->in[0] mux_1level_tapbuf_size2[90]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[90]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[90]->in[1] mux_1level_tapbuf_size2[90]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[90] gvdd_mux_1level_tapbuf_size2[90] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[271] trig v(mux_1level_tapbuf_size2[90]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[90]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[271] trig v(mux_1level_tapbuf_size2[90]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[90]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[271] when v(mux_1level_tapbuf_size2[90]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[271] trig v(mux_1level_tapbuf_size2[90]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[90]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[271] when v(mux_1level_tapbuf_size2[90]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[271] trig v(mux_1level_tapbuf_size2[90]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[90]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[90]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[90]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[271] param='mux_1level_tapbuf_size2[90]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[90]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[90]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[90]_energy_per_cycle param='mux_1level_tapbuf_size2[90]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[271]  param='mux_1level_tapbuf_size2[90]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[271]  param='dynamic_power_sb_mux[0][0]_rrnode[271]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[271] avg p(Vgvdd_mux_1level_tapbuf_size2[90]) from='start_rise_sb_mux[0][0]_rrnode[271]' to='start_rise_sb_mux[0][0]_rrnode[271]+switch_rise_sb_mux[0][0]_rrnode[271]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[271] avg p(Vgvdd_mux_1level_tapbuf_size2[90]) from='start_fall_sb_mux[0][0]_rrnode[271]' to='start_fall_sb_mux[0][0]_rrnode[271]+switch_fall_sb_mux[0][0]_rrnode[271]'
.meas tran sum_leakage_power_mux[0to90] 
+          param='sum_leakage_power_mux[0to89]+leakage_sb_mux[0][0]_rrnode[271]'
.meas tran sum_energy_per_cycle_mux[0to90] 
+          param='sum_energy_per_cycle_mux[0to89]+energy_per_cycle_sb_mux[0][0]_rrnode[271]'
***** Load for rr_node[271] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=80, type=4 *****
Xchan_mux_1level_tapbuf_size2[90]->out_loadlvl[0]_out mux_1level_tapbuf_size2[90]->out mux_1level_tapbuf_size2[90]->out_loadlvl[0]_out mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[350]_no0 mux_1level_tapbuf_size2[90]->out_loadlvl[0]_out mux_1level_tapbuf_size2[90]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[351]_no0 mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[352]_no0 mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[353]_no0 mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to90] 
+          param='sum_leakage_power_sb_mux[0to89]+leakage_sb_mux[0][0]_rrnode[271]'
.meas tran sum_energy_per_cycle_sb_mux[0to90] 
+          param='sum_energy_per_cycle_sb_mux[0to89]+energy_per_cycle_sb_mux[0][0]_rrnode[271]'
Xmux_1level_tapbuf_size2[91] mux_1level_tapbuf_size2[91]->in[0] mux_1level_tapbuf_size2[91]->in[1] mux_1level_tapbuf_size2[91]->out sram[91]->outb sram[91]->out gvdd_mux_1level_tapbuf_size2[91] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[91], level=1, select_path_id=0. *****
*****1*****
Xsram[91] sram->in sram[91]->out sram[91]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[91]->out) 0
.nodeset V(sram[91]->outb) vsp
***** Signal mux_1level_tapbuf_size2[91]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[91]->in[0] mux_1level_tapbuf_size2[91]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[91]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[91]->in[1] mux_1level_tapbuf_size2[91]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[91] gvdd_mux_1level_tapbuf_size2[91] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[273] trig v(mux_1level_tapbuf_size2[91]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[91]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[273] trig v(mux_1level_tapbuf_size2[91]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[91]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[273] when v(mux_1level_tapbuf_size2[91]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[273] trig v(mux_1level_tapbuf_size2[91]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[91]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[273] when v(mux_1level_tapbuf_size2[91]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[273] trig v(mux_1level_tapbuf_size2[91]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[91]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[91]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[91]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[273] param='mux_1level_tapbuf_size2[91]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[91]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[91]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[91]_energy_per_cycle param='mux_1level_tapbuf_size2[91]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[273]  param='mux_1level_tapbuf_size2[91]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[273]  param='dynamic_power_sb_mux[0][0]_rrnode[273]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[273] avg p(Vgvdd_mux_1level_tapbuf_size2[91]) from='start_rise_sb_mux[0][0]_rrnode[273]' to='start_rise_sb_mux[0][0]_rrnode[273]+switch_rise_sb_mux[0][0]_rrnode[273]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[273] avg p(Vgvdd_mux_1level_tapbuf_size2[91]) from='start_fall_sb_mux[0][0]_rrnode[273]' to='start_fall_sb_mux[0][0]_rrnode[273]+switch_fall_sb_mux[0][0]_rrnode[273]'
.meas tran sum_leakage_power_mux[0to91] 
+          param='sum_leakage_power_mux[0to90]+leakage_sb_mux[0][0]_rrnode[273]'
.meas tran sum_energy_per_cycle_mux[0to91] 
+          param='sum_energy_per_cycle_mux[0to90]+energy_per_cycle_sb_mux[0][0]_rrnode[273]'
***** Load for rr_node[273] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=82, type=4 *****
Xchan_mux_1level_tapbuf_size2[91]->out_loadlvl[0]_out mux_1level_tapbuf_size2[91]->out mux_1level_tapbuf_size2[91]->out_loadlvl[0]_out mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[354]_no0 mux_1level_tapbuf_size2[91]->out_loadlvl[0]_out mux_1level_tapbuf_size2[91]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[355]_no0 mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[356]_no0 mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to91] 
+          param='sum_leakage_power_sb_mux[0to90]+leakage_sb_mux[0][0]_rrnode[273]'
.meas tran sum_energy_per_cycle_sb_mux[0to91] 
+          param='sum_energy_per_cycle_sb_mux[0to90]+energy_per_cycle_sb_mux[0][0]_rrnode[273]'
Xmux_1level_tapbuf_size2[92] mux_1level_tapbuf_size2[92]->in[0] mux_1level_tapbuf_size2[92]->in[1] mux_1level_tapbuf_size2[92]->out sram[92]->outb sram[92]->out gvdd_mux_1level_tapbuf_size2[92] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[92], level=1, select_path_id=0. *****
*****1*****
Xsram[92] sram->in sram[92]->out sram[92]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[92]->out) 0
.nodeset V(sram[92]->outb) vsp
***** Signal mux_1level_tapbuf_size2[92]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[92]->in[0] mux_1level_tapbuf_size2[92]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[92]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[92]->in[1] mux_1level_tapbuf_size2[92]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[92] gvdd_mux_1level_tapbuf_size2[92] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[275] trig v(mux_1level_tapbuf_size2[92]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[92]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[275] trig v(mux_1level_tapbuf_size2[92]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[92]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[275] when v(mux_1level_tapbuf_size2[92]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[275] trig v(mux_1level_tapbuf_size2[92]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[92]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[275] when v(mux_1level_tapbuf_size2[92]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[275] trig v(mux_1level_tapbuf_size2[92]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[92]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[92]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[92]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[275] param='mux_1level_tapbuf_size2[92]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[92]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[92]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[92]_energy_per_cycle param='mux_1level_tapbuf_size2[92]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[275]  param='mux_1level_tapbuf_size2[92]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[275]  param='dynamic_power_sb_mux[0][0]_rrnode[275]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[275] avg p(Vgvdd_mux_1level_tapbuf_size2[92]) from='start_rise_sb_mux[0][0]_rrnode[275]' to='start_rise_sb_mux[0][0]_rrnode[275]+switch_rise_sb_mux[0][0]_rrnode[275]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[275] avg p(Vgvdd_mux_1level_tapbuf_size2[92]) from='start_fall_sb_mux[0][0]_rrnode[275]' to='start_fall_sb_mux[0][0]_rrnode[275]+switch_fall_sb_mux[0][0]_rrnode[275]'
.meas tran sum_leakage_power_mux[0to92] 
+          param='sum_leakage_power_mux[0to91]+leakage_sb_mux[0][0]_rrnode[275]'
.meas tran sum_energy_per_cycle_mux[0to92] 
+          param='sum_energy_per_cycle_mux[0to91]+energy_per_cycle_sb_mux[0][0]_rrnode[275]'
***** Load for rr_node[275] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=84, type=4 *****
Xchan_mux_1level_tapbuf_size2[92]->out_loadlvl[0]_out mux_1level_tapbuf_size2[92]->out mux_1level_tapbuf_size2[92]->out_loadlvl[0]_out mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[357]_no0 mux_1level_tapbuf_size2[92]->out_loadlvl[0]_out mux_1level_tapbuf_size2[92]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[358]_no0 mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[359]_no0 mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[360]_no0 mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[361]_no0 mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to92] 
+          param='sum_leakage_power_sb_mux[0to91]+leakage_sb_mux[0][0]_rrnode[275]'
.meas tran sum_energy_per_cycle_sb_mux[0to92] 
+          param='sum_energy_per_cycle_sb_mux[0to91]+energy_per_cycle_sb_mux[0][0]_rrnode[275]'
Xmux_1level_tapbuf_size2[93] mux_1level_tapbuf_size2[93]->in[0] mux_1level_tapbuf_size2[93]->in[1] mux_1level_tapbuf_size2[93]->out sram[93]->outb sram[93]->out gvdd_mux_1level_tapbuf_size2[93] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[93], level=1, select_path_id=0. *****
*****1*****
Xsram[93] sram->in sram[93]->out sram[93]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[93]->out) 0
.nodeset V(sram[93]->outb) vsp
***** Signal mux_1level_tapbuf_size2[93]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[93]->in[0] mux_1level_tapbuf_size2[93]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[93]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[93]->in[1] mux_1level_tapbuf_size2[93]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[93] gvdd_mux_1level_tapbuf_size2[93] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[277] trig v(mux_1level_tapbuf_size2[93]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[93]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[277] trig v(mux_1level_tapbuf_size2[93]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[93]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[277] when v(mux_1level_tapbuf_size2[93]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[277] trig v(mux_1level_tapbuf_size2[93]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[93]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[277] when v(mux_1level_tapbuf_size2[93]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[277] trig v(mux_1level_tapbuf_size2[93]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[93]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[93]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[93]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[277] param='mux_1level_tapbuf_size2[93]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[93]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[93]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[93]_energy_per_cycle param='mux_1level_tapbuf_size2[93]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[277]  param='mux_1level_tapbuf_size2[93]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[277]  param='dynamic_power_sb_mux[0][0]_rrnode[277]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[277] avg p(Vgvdd_mux_1level_tapbuf_size2[93]) from='start_rise_sb_mux[0][0]_rrnode[277]' to='start_rise_sb_mux[0][0]_rrnode[277]+switch_rise_sb_mux[0][0]_rrnode[277]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[277] avg p(Vgvdd_mux_1level_tapbuf_size2[93]) from='start_fall_sb_mux[0][0]_rrnode[277]' to='start_fall_sb_mux[0][0]_rrnode[277]+switch_fall_sb_mux[0][0]_rrnode[277]'
.meas tran sum_leakage_power_mux[0to93] 
+          param='sum_leakage_power_mux[0to92]+leakage_sb_mux[0][0]_rrnode[277]'
.meas tran sum_energy_per_cycle_mux[0to93] 
+          param='sum_energy_per_cycle_mux[0to92]+energy_per_cycle_sb_mux[0][0]_rrnode[277]'
***** Load for rr_node[277] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=86, type=4 *****
Xchan_mux_1level_tapbuf_size2[93]->out_loadlvl[0]_out mux_1level_tapbuf_size2[93]->out mux_1level_tapbuf_size2[93]->out_loadlvl[0]_out mux_1level_tapbuf_size2[93]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[362]_no0 mux_1level_tapbuf_size2[93]->out_loadlvl[0]_out mux_1level_tapbuf_size2[93]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[363]_no0 mux_1level_tapbuf_size2[93]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[93]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[364]_no0 mux_1level_tapbuf_size2[93]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[93]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to93] 
+          param='sum_leakage_power_sb_mux[0to92]+leakage_sb_mux[0][0]_rrnode[277]'
.meas tran sum_energy_per_cycle_sb_mux[0to93] 
+          param='sum_energy_per_cycle_sb_mux[0to92]+energy_per_cycle_sb_mux[0][0]_rrnode[277]'
Xmux_1level_tapbuf_size2[94] mux_1level_tapbuf_size2[94]->in[0] mux_1level_tapbuf_size2[94]->in[1] mux_1level_tapbuf_size2[94]->out sram[94]->outb sram[94]->out gvdd_mux_1level_tapbuf_size2[94] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[94], level=1, select_path_id=0. *****
*****1*****
Xsram[94] sram->in sram[94]->out sram[94]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[94]->out) 0
.nodeset V(sram[94]->outb) vsp
***** Signal mux_1level_tapbuf_size2[94]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[94]->in[0] mux_1level_tapbuf_size2[94]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[94]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[94]->in[1] mux_1level_tapbuf_size2[94]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[94] gvdd_mux_1level_tapbuf_size2[94] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[279] trig v(mux_1level_tapbuf_size2[94]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[94]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[279] trig v(mux_1level_tapbuf_size2[94]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[94]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[279] when v(mux_1level_tapbuf_size2[94]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[279] trig v(mux_1level_tapbuf_size2[94]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[94]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[279] when v(mux_1level_tapbuf_size2[94]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[279] trig v(mux_1level_tapbuf_size2[94]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[94]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[94]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[94]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[279] param='mux_1level_tapbuf_size2[94]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[94]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[94]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[94]_energy_per_cycle param='mux_1level_tapbuf_size2[94]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[279]  param='mux_1level_tapbuf_size2[94]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[279]  param='dynamic_power_sb_mux[0][0]_rrnode[279]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[279] avg p(Vgvdd_mux_1level_tapbuf_size2[94]) from='start_rise_sb_mux[0][0]_rrnode[279]' to='start_rise_sb_mux[0][0]_rrnode[279]+switch_rise_sb_mux[0][0]_rrnode[279]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[279] avg p(Vgvdd_mux_1level_tapbuf_size2[94]) from='start_fall_sb_mux[0][0]_rrnode[279]' to='start_fall_sb_mux[0][0]_rrnode[279]+switch_fall_sb_mux[0][0]_rrnode[279]'
.meas tran sum_leakage_power_mux[0to94] 
+          param='sum_leakage_power_mux[0to93]+leakage_sb_mux[0][0]_rrnode[279]'
.meas tran sum_energy_per_cycle_mux[0to94] 
+          param='sum_energy_per_cycle_mux[0to93]+energy_per_cycle_sb_mux[0][0]_rrnode[279]'
***** Load for rr_node[279] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=88, type=4 *****
Xchan_mux_1level_tapbuf_size2[94]->out_loadlvl[0]_out mux_1level_tapbuf_size2[94]->out mux_1level_tapbuf_size2[94]->out_loadlvl[0]_out mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[365]_no0 mux_1level_tapbuf_size2[94]->out_loadlvl[0]_out mux_1level_tapbuf_size2[94]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[366]_no0 mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[367]_no0 mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[368]_no0 mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to94] 
+          param='sum_leakage_power_sb_mux[0to93]+leakage_sb_mux[0][0]_rrnode[279]'
.meas tran sum_energy_per_cycle_sb_mux[0to94] 
+          param='sum_energy_per_cycle_sb_mux[0to93]+energy_per_cycle_sb_mux[0][0]_rrnode[279]'
Xmux_1level_tapbuf_size2[95] mux_1level_tapbuf_size2[95]->in[0] mux_1level_tapbuf_size2[95]->in[1] mux_1level_tapbuf_size2[95]->out sram[95]->outb sram[95]->out gvdd_mux_1level_tapbuf_size2[95] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[95], level=1, select_path_id=0. *****
*****1*****
Xsram[95] sram->in sram[95]->out sram[95]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[95]->out) 0
.nodeset V(sram[95]->outb) vsp
***** Signal mux_1level_tapbuf_size2[95]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[95]->in[0] mux_1level_tapbuf_size2[95]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[95]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[95]->in[1] mux_1level_tapbuf_size2[95]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[95] gvdd_mux_1level_tapbuf_size2[95] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[281] trig v(mux_1level_tapbuf_size2[95]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[95]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[281] trig v(mux_1level_tapbuf_size2[95]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[95]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[281] when v(mux_1level_tapbuf_size2[95]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[281] trig v(mux_1level_tapbuf_size2[95]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[95]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[281] when v(mux_1level_tapbuf_size2[95]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[281] trig v(mux_1level_tapbuf_size2[95]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[95]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[95]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[95]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[281] param='mux_1level_tapbuf_size2[95]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[95]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[95]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[95]_energy_per_cycle param='mux_1level_tapbuf_size2[95]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[281]  param='mux_1level_tapbuf_size2[95]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[281]  param='dynamic_power_sb_mux[0][0]_rrnode[281]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[281] avg p(Vgvdd_mux_1level_tapbuf_size2[95]) from='start_rise_sb_mux[0][0]_rrnode[281]' to='start_rise_sb_mux[0][0]_rrnode[281]+switch_rise_sb_mux[0][0]_rrnode[281]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[281] avg p(Vgvdd_mux_1level_tapbuf_size2[95]) from='start_fall_sb_mux[0][0]_rrnode[281]' to='start_fall_sb_mux[0][0]_rrnode[281]+switch_fall_sb_mux[0][0]_rrnode[281]'
.meas tran sum_leakage_power_mux[0to95] 
+          param='sum_leakage_power_mux[0to94]+leakage_sb_mux[0][0]_rrnode[281]'
.meas tran sum_energy_per_cycle_mux[0to95] 
+          param='sum_energy_per_cycle_mux[0to94]+energy_per_cycle_sb_mux[0][0]_rrnode[281]'
***** Load for rr_node[281] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=90, type=4 *****
Xchan_mux_1level_tapbuf_size2[95]->out_loadlvl[0]_out mux_1level_tapbuf_size2[95]->out mux_1level_tapbuf_size2[95]->out_loadlvl[0]_out mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[369]_no0 mux_1level_tapbuf_size2[95]->out_loadlvl[0]_out mux_1level_tapbuf_size2[95]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[370]_no0 mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[371]_no0 mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[372]_no0 mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to95] 
+          param='sum_leakage_power_sb_mux[0to94]+leakage_sb_mux[0][0]_rrnode[281]'
.meas tran sum_energy_per_cycle_sb_mux[0to95] 
+          param='sum_energy_per_cycle_sb_mux[0to94]+energy_per_cycle_sb_mux[0][0]_rrnode[281]'
Xmux_1level_tapbuf_size2[96] mux_1level_tapbuf_size2[96]->in[0] mux_1level_tapbuf_size2[96]->in[1] mux_1level_tapbuf_size2[96]->out sram[96]->outb sram[96]->out gvdd_mux_1level_tapbuf_size2[96] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[96], level=1, select_path_id=0. *****
*****1*****
Xsram[96] sram->in sram[96]->out sram[96]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[96]->out) 0
.nodeset V(sram[96]->outb) vsp
***** Signal mux_1level_tapbuf_size2[96]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[96]->in[0] mux_1level_tapbuf_size2[96]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[96]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[96]->in[1] mux_1level_tapbuf_size2[96]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[96] gvdd_mux_1level_tapbuf_size2[96] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[283] trig v(mux_1level_tapbuf_size2[96]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[96]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[283] trig v(mux_1level_tapbuf_size2[96]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[96]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[283] when v(mux_1level_tapbuf_size2[96]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[283] trig v(mux_1level_tapbuf_size2[96]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[96]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[283] when v(mux_1level_tapbuf_size2[96]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[283] trig v(mux_1level_tapbuf_size2[96]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[96]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[96]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[96]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[283] param='mux_1level_tapbuf_size2[96]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[96]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[96]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[96]_energy_per_cycle param='mux_1level_tapbuf_size2[96]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[283]  param='mux_1level_tapbuf_size2[96]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[283]  param='dynamic_power_sb_mux[0][0]_rrnode[283]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[283] avg p(Vgvdd_mux_1level_tapbuf_size2[96]) from='start_rise_sb_mux[0][0]_rrnode[283]' to='start_rise_sb_mux[0][0]_rrnode[283]+switch_rise_sb_mux[0][0]_rrnode[283]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[283] avg p(Vgvdd_mux_1level_tapbuf_size2[96]) from='start_fall_sb_mux[0][0]_rrnode[283]' to='start_fall_sb_mux[0][0]_rrnode[283]+switch_fall_sb_mux[0][0]_rrnode[283]'
.meas tran sum_leakage_power_mux[0to96] 
+          param='sum_leakage_power_mux[0to95]+leakage_sb_mux[0][0]_rrnode[283]'
.meas tran sum_energy_per_cycle_mux[0to96] 
+          param='sum_energy_per_cycle_mux[0to95]+energy_per_cycle_sb_mux[0][0]_rrnode[283]'
***** Load for rr_node[283] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=92, type=4 *****
Xchan_mux_1level_tapbuf_size2[96]->out_loadlvl[0]_out mux_1level_tapbuf_size2[96]->out mux_1level_tapbuf_size2[96]->out_loadlvl[0]_out mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[373]_no0 mux_1level_tapbuf_size2[96]->out_loadlvl[0]_out mux_1level_tapbuf_size2[96]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[374]_no0 mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[375]_no0 mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[376]_no0 mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to96] 
+          param='sum_leakage_power_sb_mux[0to95]+leakage_sb_mux[0][0]_rrnode[283]'
.meas tran sum_energy_per_cycle_sb_mux[0to96] 
+          param='sum_energy_per_cycle_sb_mux[0to95]+energy_per_cycle_sb_mux[0][0]_rrnode[283]'
Xmux_1level_tapbuf_size2[97] mux_1level_tapbuf_size2[97]->in[0] mux_1level_tapbuf_size2[97]->in[1] mux_1level_tapbuf_size2[97]->out sram[97]->outb sram[97]->out gvdd_mux_1level_tapbuf_size2[97] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[97], level=1, select_path_id=0. *****
*****1*****
Xsram[97] sram->in sram[97]->out sram[97]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[97]->out) 0
.nodeset V(sram[97]->outb) vsp
***** Signal mux_1level_tapbuf_size2[97]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[97]->in[0] mux_1level_tapbuf_size2[97]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[97]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[97]->in[1] mux_1level_tapbuf_size2[97]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[97] gvdd_mux_1level_tapbuf_size2[97] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[285] trig v(mux_1level_tapbuf_size2[97]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[97]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[285] trig v(mux_1level_tapbuf_size2[97]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[97]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[285] when v(mux_1level_tapbuf_size2[97]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[285] trig v(mux_1level_tapbuf_size2[97]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[97]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[285] when v(mux_1level_tapbuf_size2[97]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[285] trig v(mux_1level_tapbuf_size2[97]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[97]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[97]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[97]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[285] param='mux_1level_tapbuf_size2[97]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[97]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[97]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[97]_energy_per_cycle param='mux_1level_tapbuf_size2[97]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[285]  param='mux_1level_tapbuf_size2[97]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[285]  param='dynamic_power_sb_mux[0][0]_rrnode[285]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[285] avg p(Vgvdd_mux_1level_tapbuf_size2[97]) from='start_rise_sb_mux[0][0]_rrnode[285]' to='start_rise_sb_mux[0][0]_rrnode[285]+switch_rise_sb_mux[0][0]_rrnode[285]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[285] avg p(Vgvdd_mux_1level_tapbuf_size2[97]) from='start_fall_sb_mux[0][0]_rrnode[285]' to='start_fall_sb_mux[0][0]_rrnode[285]+switch_fall_sb_mux[0][0]_rrnode[285]'
.meas tran sum_leakage_power_mux[0to97] 
+          param='sum_leakage_power_mux[0to96]+leakage_sb_mux[0][0]_rrnode[285]'
.meas tran sum_energy_per_cycle_mux[0to97] 
+          param='sum_energy_per_cycle_mux[0to96]+energy_per_cycle_sb_mux[0][0]_rrnode[285]'
***** Load for rr_node[285] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=94, type=4 *****
Xchan_mux_1level_tapbuf_size2[97]->out_loadlvl[0]_out mux_1level_tapbuf_size2[97]->out mux_1level_tapbuf_size2[97]->out_loadlvl[0]_out mux_1level_tapbuf_size2[97]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[377]_no0 mux_1level_tapbuf_size2[97]->out_loadlvl[0]_out mux_1level_tapbuf_size2[97]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[378]_no0 mux_1level_tapbuf_size2[97]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[97]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[379]_no0 mux_1level_tapbuf_size2[97]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[97]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[380]_no0 mux_1level_tapbuf_size2[97]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[97]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to97] 
+          param='sum_leakage_power_sb_mux[0to96]+leakage_sb_mux[0][0]_rrnode[285]'
.meas tran sum_energy_per_cycle_sb_mux[0to97] 
+          param='sum_energy_per_cycle_sb_mux[0to96]+energy_per_cycle_sb_mux[0][0]_rrnode[285]'
Xmux_1level_tapbuf_size2[98] mux_1level_tapbuf_size2[98]->in[0] mux_1level_tapbuf_size2[98]->in[1] mux_1level_tapbuf_size2[98]->out sram[98]->outb sram[98]->out gvdd_mux_1level_tapbuf_size2[98] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[98], level=1, select_path_id=0. *****
*****1*****
Xsram[98] sram->in sram[98]->out sram[98]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[98]->out) 0
.nodeset V(sram[98]->outb) vsp
***** Signal mux_1level_tapbuf_size2[98]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[98]->in[0] mux_1level_tapbuf_size2[98]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[98]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[98]->in[1] mux_1level_tapbuf_size2[98]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[98] gvdd_mux_1level_tapbuf_size2[98] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[287] trig v(mux_1level_tapbuf_size2[98]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[98]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[287] trig v(mux_1level_tapbuf_size2[98]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[98]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[287] when v(mux_1level_tapbuf_size2[98]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[287] trig v(mux_1level_tapbuf_size2[98]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[98]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[287] when v(mux_1level_tapbuf_size2[98]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[287] trig v(mux_1level_tapbuf_size2[98]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[98]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[98]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[98]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[287] param='mux_1level_tapbuf_size2[98]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[98]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[98]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[98]_energy_per_cycle param='mux_1level_tapbuf_size2[98]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[287]  param='mux_1level_tapbuf_size2[98]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[287]  param='dynamic_power_sb_mux[0][0]_rrnode[287]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[287] avg p(Vgvdd_mux_1level_tapbuf_size2[98]) from='start_rise_sb_mux[0][0]_rrnode[287]' to='start_rise_sb_mux[0][0]_rrnode[287]+switch_rise_sb_mux[0][0]_rrnode[287]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[287] avg p(Vgvdd_mux_1level_tapbuf_size2[98]) from='start_fall_sb_mux[0][0]_rrnode[287]' to='start_fall_sb_mux[0][0]_rrnode[287]+switch_fall_sb_mux[0][0]_rrnode[287]'
.meas tran sum_leakage_power_mux[0to98] 
+          param='sum_leakage_power_mux[0to97]+leakage_sb_mux[0][0]_rrnode[287]'
.meas tran sum_energy_per_cycle_mux[0to98] 
+          param='sum_energy_per_cycle_mux[0to97]+energy_per_cycle_sb_mux[0][0]_rrnode[287]'
***** Load for rr_node[287] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=96, type=4 *****
Xchan_mux_1level_tapbuf_size2[98]->out_loadlvl[0]_out mux_1level_tapbuf_size2[98]->out mux_1level_tapbuf_size2[98]->out_loadlvl[0]_out mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[381]_no0 mux_1level_tapbuf_size2[98]->out_loadlvl[0]_out mux_1level_tapbuf_size2[98]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[382]_no0 mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[383]_no0 mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to98] 
+          param='sum_leakage_power_sb_mux[0to97]+leakage_sb_mux[0][0]_rrnode[287]'
.meas tran sum_energy_per_cycle_sb_mux[0to98] 
+          param='sum_energy_per_cycle_sb_mux[0to97]+energy_per_cycle_sb_mux[0][0]_rrnode[287]'
Xmux_1level_tapbuf_size2[99] mux_1level_tapbuf_size2[99]->in[0] mux_1level_tapbuf_size2[99]->in[1] mux_1level_tapbuf_size2[99]->out sram[99]->outb sram[99]->out gvdd_mux_1level_tapbuf_size2[99] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[99], level=1, select_path_id=0. *****
*****1*****
Xsram[99] sram->in sram[99]->out sram[99]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[99]->out) 0
.nodeset V(sram[99]->outb) vsp
***** Signal mux_1level_tapbuf_size2[99]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[99]->in[0] mux_1level_tapbuf_size2[99]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[99]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[99]->in[1] mux_1level_tapbuf_size2[99]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[99] gvdd_mux_1level_tapbuf_size2[99] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[0][0]_rrnode[289] trig v(mux_1level_tapbuf_size2[99]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[99]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[0][0]_rrnode[289] trig v(mux_1level_tapbuf_size2[99]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[99]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[0][0]_rrnode[289] when v(mux_1level_tapbuf_size2[99]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[0][0]_rrnode[289] trig v(mux_1level_tapbuf_size2[99]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[99]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[0][0]_rrnode[289] when v(mux_1level_tapbuf_size2[99]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[0][0]_rrnode[289] trig v(mux_1level_tapbuf_size2[99]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[99]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[99]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[99]) from=0 to='clock_period'
.meas tran leakage_sb_mux[0][0]_rrnode[289] param='mux_1level_tapbuf_size2[99]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[99]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[99]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[99]_energy_per_cycle param='mux_1level_tapbuf_size2[99]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[0][0]_rrnode[289]  param='mux_1level_tapbuf_size2[99]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[0][0]_rrnode[289]  param='dynamic_power_sb_mux[0][0]_rrnode[289]*clock_period'
.meas tran dynamic_rise_sb_mux[0][0]_rrnode[289] avg p(Vgvdd_mux_1level_tapbuf_size2[99]) from='start_rise_sb_mux[0][0]_rrnode[289]' to='start_rise_sb_mux[0][0]_rrnode[289]+switch_rise_sb_mux[0][0]_rrnode[289]'
.meas tran dynamic_fall_sb_mux[0][0]_rrnode[289] avg p(Vgvdd_mux_1level_tapbuf_size2[99]) from='start_fall_sb_mux[0][0]_rrnode[289]' to='start_fall_sb_mux[0][0]_rrnode[289]+switch_fall_sb_mux[0][0]_rrnode[289]'
.meas tran sum_leakage_power_mux[0to99] 
+          param='sum_leakage_power_mux[0to98]+leakage_sb_mux[0][0]_rrnode[289]'
.meas tran sum_energy_per_cycle_mux[0to99] 
+          param='sum_energy_per_cycle_mux[0to98]+energy_per_cycle_sb_mux[0][0]_rrnode[289]'
***** Load for rr_node[289] *****
**** Loads for rr_node: xlow=1, ylow=0, xhigh=1, yhigh=0, ptc_num=98, type=4 *****
Xchan_mux_1level_tapbuf_size2[99]->out_loadlvl[0]_out mux_1level_tapbuf_size2[99]->out mux_1level_tapbuf_size2[99]->out_loadlvl[0]_out mux_1level_tapbuf_size2[99]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[384]_no0 mux_1level_tapbuf_size2[99]->out_loadlvl[0]_out mux_1level_tapbuf_size2[99]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[385]_no0 mux_1level_tapbuf_size2[99]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[99]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[386]_no0 mux_1level_tapbuf_size2[99]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[99]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[387]_no0 mux_1level_tapbuf_size2[99]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[99]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to99] 
+          param='sum_leakage_power_sb_mux[0to98]+leakage_sb_mux[0][0]_rrnode[289]'
.meas tran sum_energy_per_cycle_sb_mux[0to99] 
+          param='sum_energy_per_cycle_sb_mux[0to98]+energy_per_cycle_sb_mux[0][0]_rrnode[289]'
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
***** 6 Clock Simulation, accuracy=1e-13 *****
.tran 1e-13  '6*clock_period'
***** Generic Measurements for Circuit Parameters *****
.meas tran total_leakage_srams avg p(Vgvdd_sram) from=0 to='clock_period'
.meas tran total_dynamic_srams avg p(Vgvdd_sram) from='clock_period' to='6*clock_period'
.meas tran total_energy_per_cycle_srams param='total_dynamic_srams*clock_period'
.meas tran total_leakage_power_mux[0to99] 
+          param='sum_leakage_power_mux[0to99]'
.meas tran total_energy_per_cycle_mux[0to99] 
+          param='sum_energy_per_cycle_mux[0to99]'
.meas tran total_leakage_power_sb_mux 
+          param='sum_leakage_power_sb_mux[0to99]'
.meas tran total_energy_per_cycle_sb_mux 
+          param='sum_energy_per_cycle_sb_mux[0to99]'
.end
