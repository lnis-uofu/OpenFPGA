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
***** Signal mux_1level_tapbuf_size3[0]->in[2] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[0]->in[2] mux_1level_tapbuf_size3[0]->in[2] 0 
+  0
Vgvdd_mux_1level_tapbuf_size3[0] gvdd_mux_1level_tapbuf_size3[0] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[492] trig v(mux_1level_tapbuf_size3[0]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[0]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[492] trig v(mux_1level_tapbuf_size3[0]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[0]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[492] when v(mux_1level_tapbuf_size3[0]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[492] trig v(mux_1level_tapbuf_size3[0]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[0]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[492] when v(mux_1level_tapbuf_size3[0]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[492] trig v(mux_1level_tapbuf_size3[0]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[0]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[0]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[0]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[492] param='mux_1level_tapbuf_size3[0]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[0]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[0]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size3[0]_energy_per_cycle param='mux_1level_tapbuf_size3[0]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[492]  param='mux_1level_tapbuf_size3[0]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[492]  param='dynamic_power_sb_mux[1][1]_rrnode[492]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[492] avg p(Vgvdd_mux_1level_tapbuf_size3[0]) from='start_rise_sb_mux[1][1]_rrnode[492]' to='start_rise_sb_mux[1][1]_rrnode[492]+switch_rise_sb_mux[1][1]_rrnode[492]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[492] avg p(Vgvdd_mux_1level_tapbuf_size3[0]) from='start_fall_sb_mux[1][1]_rrnode[492]' to='start_fall_sb_mux[1][1]_rrnode[492]+switch_fall_sb_mux[1][1]_rrnode[492]'
.meas tran sum_leakage_power_mux[0to0] 
+          param='leakage_sb_mux[1][1]_rrnode[492]'
.meas tran sum_energy_per_cycle_mux[0to0] 
+          param='energy_per_cycle_sb_mux[1][1]_rrnode[492]'
***** Load for rr_node[492] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=1, type=5 *****
Xchan_mux_1level_tapbuf_size3[0]->out_loadlvl[0]_out mux_1level_tapbuf_size3[0]->out mux_1level_tapbuf_size3[0]->out_loadlvl[0]_out mux_1level_tapbuf_size3[0]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[0]_no0 mux_1level_tapbuf_size3[0]->out_loadlvl[0]_out mux_1level_tapbuf_size3[0]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[1]_no0 mux_1level_tapbuf_size3[0]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[0]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to0] 
+          param='leakage_sb_mux[1][1]_rrnode[492]'
.meas tran sum_energy_per_cycle_sb_mux[0to0] 
+          param='energy_per_cycle_sb_mux[1][1]_rrnode[492]'
Xmux_1level_tapbuf_size3[1] mux_1level_tapbuf_size3[1]->in[0] mux_1level_tapbuf_size3[1]->in[1] mux_1level_tapbuf_size3[1]->in[2] mux_1level_tapbuf_size3[1]->out sram[3]->outb sram[3]->out sram[4]->out sram[4]->outb sram[5]->out sram[5]->outb gvdd_mux_1level_tapbuf_size3[1] 0 mux_1level_tapbuf_size3
***** SRAM bits for MUX[1], level=1, select_path_id=0. *****
*****100*****
Xsram[3] sram->in sram[3]->out sram[3]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[3]->out) 0
.nodeset V(sram[3]->outb) vsp
Xsram[4] sram->in sram[4]->out sram[4]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[4]->out) 0
.nodeset V(sram[4]->outb) vsp
Xsram[5] sram->in sram[5]->out sram[5]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[5]->out) 0
.nodeset V(sram[5]->outb) vsp
***** Signal mux_1level_tapbuf_size3[1]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[1]->in[0] mux_1level_tapbuf_size3[1]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size3[1]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[1]->in[1] mux_1level_tapbuf_size3[1]->in[1] 0 
+  0
***** Signal mux_1level_tapbuf_size3[1]->in[2] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[1]->in[2] mux_1level_tapbuf_size3[1]->in[2] 0 
+  0
Vgvdd_mux_1level_tapbuf_size3[1] gvdd_mux_1level_tapbuf_size3[1] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[494] trig v(mux_1level_tapbuf_size3[1]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[1]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[494] trig v(mux_1level_tapbuf_size3[1]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[1]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[494] when v(mux_1level_tapbuf_size3[1]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[494] trig v(mux_1level_tapbuf_size3[1]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[1]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[494] when v(mux_1level_tapbuf_size3[1]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[494] trig v(mux_1level_tapbuf_size3[1]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[1]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[1]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[1]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[494] param='mux_1level_tapbuf_size3[1]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[1]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[1]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size3[1]_energy_per_cycle param='mux_1level_tapbuf_size3[1]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[494]  param='mux_1level_tapbuf_size3[1]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[494]  param='dynamic_power_sb_mux[1][1]_rrnode[494]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[494] avg p(Vgvdd_mux_1level_tapbuf_size3[1]) from='start_rise_sb_mux[1][1]_rrnode[494]' to='start_rise_sb_mux[1][1]_rrnode[494]+switch_rise_sb_mux[1][1]_rrnode[494]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[494] avg p(Vgvdd_mux_1level_tapbuf_size3[1]) from='start_fall_sb_mux[1][1]_rrnode[494]' to='start_fall_sb_mux[1][1]_rrnode[494]+switch_fall_sb_mux[1][1]_rrnode[494]'
.meas tran sum_leakage_power_mux[0to1] 
+          param='sum_leakage_power_mux[0to0]+leakage_sb_mux[1][1]_rrnode[494]'
.meas tran sum_energy_per_cycle_mux[0to1] 
+          param='sum_energy_per_cycle_mux[0to0]+energy_per_cycle_sb_mux[1][1]_rrnode[494]'
***** Load for rr_node[494] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=3, type=5 *****
Xchan_mux_1level_tapbuf_size3[1]->out_loadlvl[0]_out mux_1level_tapbuf_size3[1]->out mux_1level_tapbuf_size3[1]->out_loadlvl[0]_out mux_1level_tapbuf_size3[1]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[2]_no0 mux_1level_tapbuf_size3[1]->out_loadlvl[0]_out mux_1level_tapbuf_size3[1]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[3]_no0 mux_1level_tapbuf_size3[1]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[1]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[4]_no0 mux_1level_tapbuf_size3[1]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[1]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to1] 
+          param='sum_leakage_power_sb_mux[0to0]+leakage_sb_mux[1][1]_rrnode[494]'
.meas tran sum_energy_per_cycle_sb_mux[0to1] 
+          param='sum_energy_per_cycle_sb_mux[0to0]+energy_per_cycle_sb_mux[1][1]_rrnode[494]'
Xmux_1level_tapbuf_size3[2] mux_1level_tapbuf_size3[2]->in[0] mux_1level_tapbuf_size3[2]->in[1] mux_1level_tapbuf_size3[2]->in[2] mux_1level_tapbuf_size3[2]->out sram[6]->outb sram[6]->out sram[7]->out sram[7]->outb sram[8]->out sram[8]->outb gvdd_mux_1level_tapbuf_size3[2] 0 mux_1level_tapbuf_size3
***** SRAM bits for MUX[2], level=1, select_path_id=0. *****
*****100*****
Xsram[6] sram->in sram[6]->out sram[6]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[6]->out) 0
.nodeset V(sram[6]->outb) vsp
Xsram[7] sram->in sram[7]->out sram[7]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[7]->out) 0
.nodeset V(sram[7]->outb) vsp
Xsram[8] sram->in sram[8]->out sram[8]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[8]->out) 0
.nodeset V(sram[8]->outb) vsp
***** Signal mux_1level_tapbuf_size3[2]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[2]->in[0] mux_1level_tapbuf_size3[2]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size3[2]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[2]->in[1] mux_1level_tapbuf_size3[2]->in[1] 0 
+  0
***** Signal mux_1level_tapbuf_size3[2]->in[2] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[2]->in[2] mux_1level_tapbuf_size3[2]->in[2] 0 
+  0
Vgvdd_mux_1level_tapbuf_size3[2] gvdd_mux_1level_tapbuf_size3[2] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[496] trig v(mux_1level_tapbuf_size3[2]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[2]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[496] trig v(mux_1level_tapbuf_size3[2]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[2]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[496] when v(mux_1level_tapbuf_size3[2]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[496] trig v(mux_1level_tapbuf_size3[2]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[2]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[496] when v(mux_1level_tapbuf_size3[2]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[496] trig v(mux_1level_tapbuf_size3[2]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[2]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[2]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[2]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[496] param='mux_1level_tapbuf_size3[2]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[2]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[2]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size3[2]_energy_per_cycle param='mux_1level_tapbuf_size3[2]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[496]  param='mux_1level_tapbuf_size3[2]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[496]  param='dynamic_power_sb_mux[1][1]_rrnode[496]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[496] avg p(Vgvdd_mux_1level_tapbuf_size3[2]) from='start_rise_sb_mux[1][1]_rrnode[496]' to='start_rise_sb_mux[1][1]_rrnode[496]+switch_rise_sb_mux[1][1]_rrnode[496]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[496] avg p(Vgvdd_mux_1level_tapbuf_size3[2]) from='start_fall_sb_mux[1][1]_rrnode[496]' to='start_fall_sb_mux[1][1]_rrnode[496]+switch_fall_sb_mux[1][1]_rrnode[496]'
.meas tran sum_leakage_power_mux[0to2] 
+          param='sum_leakage_power_mux[0to1]+leakage_sb_mux[1][1]_rrnode[496]'
.meas tran sum_energy_per_cycle_mux[0to2] 
+          param='sum_energy_per_cycle_mux[0to1]+energy_per_cycle_sb_mux[1][1]_rrnode[496]'
***** Load for rr_node[496] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=5, type=5 *****
Xchan_mux_1level_tapbuf_size3[2]->out_loadlvl[0]_out mux_1level_tapbuf_size3[2]->out mux_1level_tapbuf_size3[2]->out_loadlvl[0]_out mux_1level_tapbuf_size3[2]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[5]_no0 mux_1level_tapbuf_size3[2]->out_loadlvl[0]_out mux_1level_tapbuf_size3[2]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[6]_no0 mux_1level_tapbuf_size3[2]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[2]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[7]_no0 mux_1level_tapbuf_size3[2]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[2]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to2] 
+          param='sum_leakage_power_sb_mux[0to1]+leakage_sb_mux[1][1]_rrnode[496]'
.meas tran sum_energy_per_cycle_sb_mux[0to2] 
+          param='sum_energy_per_cycle_sb_mux[0to1]+energy_per_cycle_sb_mux[1][1]_rrnode[496]'
Xmux_1level_tapbuf_size3[3] mux_1level_tapbuf_size3[3]->in[0] mux_1level_tapbuf_size3[3]->in[1] mux_1level_tapbuf_size3[3]->in[2] mux_1level_tapbuf_size3[3]->out sram[9]->outb sram[9]->out sram[10]->out sram[10]->outb sram[11]->out sram[11]->outb gvdd_mux_1level_tapbuf_size3[3] 0 mux_1level_tapbuf_size3
***** SRAM bits for MUX[3], level=1, select_path_id=0. *****
*****100*****
Xsram[9] sram->in sram[9]->out sram[9]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[9]->out) 0
.nodeset V(sram[9]->outb) vsp
Xsram[10] sram->in sram[10]->out sram[10]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[10]->out) 0
.nodeset V(sram[10]->outb) vsp
Xsram[11] sram->in sram[11]->out sram[11]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[11]->out) 0
.nodeset V(sram[11]->outb) vsp
***** Signal mux_1level_tapbuf_size3[3]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[3]->in[0] mux_1level_tapbuf_size3[3]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size3[3]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[3]->in[1] mux_1level_tapbuf_size3[3]->in[1] 0 
+  0
***** Signal mux_1level_tapbuf_size3[3]->in[2] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[3]->in[2] mux_1level_tapbuf_size3[3]->in[2] 0 
+  0
Vgvdd_mux_1level_tapbuf_size3[3] gvdd_mux_1level_tapbuf_size3[3] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[498] trig v(mux_1level_tapbuf_size3[3]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[3]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[498] trig v(mux_1level_tapbuf_size3[3]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[3]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[498] when v(mux_1level_tapbuf_size3[3]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[498] trig v(mux_1level_tapbuf_size3[3]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[3]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[498] when v(mux_1level_tapbuf_size3[3]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[498] trig v(mux_1level_tapbuf_size3[3]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[3]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[3]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[3]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[498] param='mux_1level_tapbuf_size3[3]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[3]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[3]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size3[3]_energy_per_cycle param='mux_1level_tapbuf_size3[3]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[498]  param='mux_1level_tapbuf_size3[3]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[498]  param='dynamic_power_sb_mux[1][1]_rrnode[498]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[498] avg p(Vgvdd_mux_1level_tapbuf_size3[3]) from='start_rise_sb_mux[1][1]_rrnode[498]' to='start_rise_sb_mux[1][1]_rrnode[498]+switch_rise_sb_mux[1][1]_rrnode[498]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[498] avg p(Vgvdd_mux_1level_tapbuf_size3[3]) from='start_fall_sb_mux[1][1]_rrnode[498]' to='start_fall_sb_mux[1][1]_rrnode[498]+switch_fall_sb_mux[1][1]_rrnode[498]'
.meas tran sum_leakage_power_mux[0to3] 
+          param='sum_leakage_power_mux[0to2]+leakage_sb_mux[1][1]_rrnode[498]'
.meas tran sum_energy_per_cycle_mux[0to3] 
+          param='sum_energy_per_cycle_mux[0to2]+energy_per_cycle_sb_mux[1][1]_rrnode[498]'
***** Load for rr_node[498] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=7, type=5 *****
Xchan_mux_1level_tapbuf_size3[3]->out_loadlvl[0]_out mux_1level_tapbuf_size3[3]->out mux_1level_tapbuf_size3[3]->out_loadlvl[0]_out mux_1level_tapbuf_size3[3]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[8]_no0 mux_1level_tapbuf_size3[3]->out_loadlvl[0]_out mux_1level_tapbuf_size3[3]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[9]_no0 mux_1level_tapbuf_size3[3]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[3]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[10]_no0 mux_1level_tapbuf_size3[3]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[3]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to3] 
+          param='sum_leakage_power_sb_mux[0to2]+leakage_sb_mux[1][1]_rrnode[498]'
.meas tran sum_energy_per_cycle_sb_mux[0to3] 
+          param='sum_energy_per_cycle_sb_mux[0to2]+energy_per_cycle_sb_mux[1][1]_rrnode[498]'
Xmux_1level_tapbuf_size3[4] mux_1level_tapbuf_size3[4]->in[0] mux_1level_tapbuf_size3[4]->in[1] mux_1level_tapbuf_size3[4]->in[2] mux_1level_tapbuf_size3[4]->out sram[12]->outb sram[12]->out sram[13]->out sram[13]->outb sram[14]->out sram[14]->outb gvdd_mux_1level_tapbuf_size3[4] 0 mux_1level_tapbuf_size3
***** SRAM bits for MUX[4], level=1, select_path_id=0. *****
*****100*****
Xsram[12] sram->in sram[12]->out sram[12]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[12]->out) 0
.nodeset V(sram[12]->outb) vsp
Xsram[13] sram->in sram[13]->out sram[13]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[13]->out) 0
.nodeset V(sram[13]->outb) vsp
Xsram[14] sram->in sram[14]->out sram[14]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[14]->out) 0
.nodeset V(sram[14]->outb) vsp
***** Signal mux_1level_tapbuf_size3[4]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[4]->in[0] mux_1level_tapbuf_size3[4]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size3[4]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[4]->in[1] mux_1level_tapbuf_size3[4]->in[1] 0 
+  0
***** Signal mux_1level_tapbuf_size3[4]->in[2] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[4]->in[2] mux_1level_tapbuf_size3[4]->in[2] 0 
+  0
Vgvdd_mux_1level_tapbuf_size3[4] gvdd_mux_1level_tapbuf_size3[4] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[500] trig v(mux_1level_tapbuf_size3[4]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[4]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[500] trig v(mux_1level_tapbuf_size3[4]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[4]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[500] when v(mux_1level_tapbuf_size3[4]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[500] trig v(mux_1level_tapbuf_size3[4]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[4]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[500] when v(mux_1level_tapbuf_size3[4]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[500] trig v(mux_1level_tapbuf_size3[4]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[4]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[4]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[4]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[500] param='mux_1level_tapbuf_size3[4]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[4]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[4]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size3[4]_energy_per_cycle param='mux_1level_tapbuf_size3[4]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[500]  param='mux_1level_tapbuf_size3[4]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[500]  param='dynamic_power_sb_mux[1][1]_rrnode[500]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[500] avg p(Vgvdd_mux_1level_tapbuf_size3[4]) from='start_rise_sb_mux[1][1]_rrnode[500]' to='start_rise_sb_mux[1][1]_rrnode[500]+switch_rise_sb_mux[1][1]_rrnode[500]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[500] avg p(Vgvdd_mux_1level_tapbuf_size3[4]) from='start_fall_sb_mux[1][1]_rrnode[500]' to='start_fall_sb_mux[1][1]_rrnode[500]+switch_fall_sb_mux[1][1]_rrnode[500]'
.meas tran sum_leakage_power_mux[0to4] 
+          param='sum_leakage_power_mux[0to3]+leakage_sb_mux[1][1]_rrnode[500]'
.meas tran sum_energy_per_cycle_mux[0to4] 
+          param='sum_energy_per_cycle_mux[0to3]+energy_per_cycle_sb_mux[1][1]_rrnode[500]'
***** Load for rr_node[500] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=9, type=5 *****
Xchan_mux_1level_tapbuf_size3[4]->out_loadlvl[0]_out mux_1level_tapbuf_size3[4]->out mux_1level_tapbuf_size3[4]->out_loadlvl[0]_out mux_1level_tapbuf_size3[4]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[11]_no0 mux_1level_tapbuf_size3[4]->out_loadlvl[0]_out mux_1level_tapbuf_size3[4]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[12]_no0 mux_1level_tapbuf_size3[4]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[4]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[13]_no0 mux_1level_tapbuf_size3[4]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[4]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to4] 
+          param='sum_leakage_power_sb_mux[0to3]+leakage_sb_mux[1][1]_rrnode[500]'
.meas tran sum_energy_per_cycle_sb_mux[0to4] 
+          param='sum_energy_per_cycle_sb_mux[0to3]+energy_per_cycle_sb_mux[1][1]_rrnode[500]'
Xmux_1level_tapbuf_size2[5] mux_1level_tapbuf_size2[5]->in[0] mux_1level_tapbuf_size2[5]->in[1] mux_1level_tapbuf_size2[5]->out sram[15]->outb sram[15]->out gvdd_mux_1level_tapbuf_size2[5] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[5], level=1, select_path_id=0. *****
*****1*****
Xsram[15] sram->in sram[15]->out sram[15]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[15]->out) 0
.nodeset V(sram[15]->outb) vsp
***** Signal mux_1level_tapbuf_size2[5]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[5]->in[0] mux_1level_tapbuf_size2[5]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[5]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[5]->in[1] mux_1level_tapbuf_size2[5]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[5] gvdd_mux_1level_tapbuf_size2[5] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[502] trig v(mux_1level_tapbuf_size2[5]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[5]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[502] trig v(mux_1level_tapbuf_size2[5]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[5]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[502] when v(mux_1level_tapbuf_size2[5]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[502] trig v(mux_1level_tapbuf_size2[5]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[5]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[502] when v(mux_1level_tapbuf_size2[5]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[502] trig v(mux_1level_tapbuf_size2[5]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[5]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[5]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[5]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[502] param='mux_1level_tapbuf_size2[5]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[5]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[5]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[5]_energy_per_cycle param='mux_1level_tapbuf_size2[5]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[502]  param='mux_1level_tapbuf_size2[5]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[502]  param='dynamic_power_sb_mux[1][1]_rrnode[502]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[502] avg p(Vgvdd_mux_1level_tapbuf_size2[5]) from='start_rise_sb_mux[1][1]_rrnode[502]' to='start_rise_sb_mux[1][1]_rrnode[502]+switch_rise_sb_mux[1][1]_rrnode[502]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[502] avg p(Vgvdd_mux_1level_tapbuf_size2[5]) from='start_fall_sb_mux[1][1]_rrnode[502]' to='start_fall_sb_mux[1][1]_rrnode[502]+switch_fall_sb_mux[1][1]_rrnode[502]'
.meas tran sum_leakage_power_mux[0to5] 
+          param='sum_leakage_power_mux[0to4]+leakage_sb_mux[1][1]_rrnode[502]'
.meas tran sum_energy_per_cycle_mux[0to5] 
+          param='sum_energy_per_cycle_mux[0to4]+energy_per_cycle_sb_mux[1][1]_rrnode[502]'
***** Load for rr_node[502] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=11, type=5 *****
Xchan_mux_1level_tapbuf_size2[5]->out_loadlvl[0]_out mux_1level_tapbuf_size2[5]->out mux_1level_tapbuf_size2[5]->out_loadlvl[0]_out mux_1level_tapbuf_size2[5]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[14]_no0 mux_1level_tapbuf_size2[5]->out_loadlvl[0]_out mux_1level_tapbuf_size2[5]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[15]_no0 mux_1level_tapbuf_size2[5]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[5]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to5] 
+          param='sum_leakage_power_sb_mux[0to4]+leakage_sb_mux[1][1]_rrnode[502]'
.meas tran sum_energy_per_cycle_sb_mux[0to5] 
+          param='sum_energy_per_cycle_sb_mux[0to4]+energy_per_cycle_sb_mux[1][1]_rrnode[502]'
Xmux_1level_tapbuf_size2[6] mux_1level_tapbuf_size2[6]->in[0] mux_1level_tapbuf_size2[6]->in[1] mux_1level_tapbuf_size2[6]->out sram[16]->outb sram[16]->out gvdd_mux_1level_tapbuf_size2[6] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[6], level=1, select_path_id=0. *****
*****1*****
Xsram[16] sram->in sram[16]->out sram[16]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[16]->out) 0
.nodeset V(sram[16]->outb) vsp
***** Signal mux_1level_tapbuf_size2[6]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[6]->in[0] mux_1level_tapbuf_size2[6]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[6]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[6]->in[1] mux_1level_tapbuf_size2[6]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[6] gvdd_mux_1level_tapbuf_size2[6] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[504] trig v(mux_1level_tapbuf_size2[6]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[6]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[504] trig v(mux_1level_tapbuf_size2[6]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[6]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[504] when v(mux_1level_tapbuf_size2[6]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[504] trig v(mux_1level_tapbuf_size2[6]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[6]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[504] when v(mux_1level_tapbuf_size2[6]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[504] trig v(mux_1level_tapbuf_size2[6]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[6]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[6]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[6]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[504] param='mux_1level_tapbuf_size2[6]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[6]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[6]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[6]_energy_per_cycle param='mux_1level_tapbuf_size2[6]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[504]  param='mux_1level_tapbuf_size2[6]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[504]  param='dynamic_power_sb_mux[1][1]_rrnode[504]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[504] avg p(Vgvdd_mux_1level_tapbuf_size2[6]) from='start_rise_sb_mux[1][1]_rrnode[504]' to='start_rise_sb_mux[1][1]_rrnode[504]+switch_rise_sb_mux[1][1]_rrnode[504]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[504] avg p(Vgvdd_mux_1level_tapbuf_size2[6]) from='start_fall_sb_mux[1][1]_rrnode[504]' to='start_fall_sb_mux[1][1]_rrnode[504]+switch_fall_sb_mux[1][1]_rrnode[504]'
.meas tran sum_leakage_power_mux[0to6] 
+          param='sum_leakage_power_mux[0to5]+leakage_sb_mux[1][1]_rrnode[504]'
.meas tran sum_energy_per_cycle_mux[0to6] 
+          param='sum_energy_per_cycle_mux[0to5]+energy_per_cycle_sb_mux[1][1]_rrnode[504]'
***** Load for rr_node[504] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=13, type=5 *****
Xchan_mux_1level_tapbuf_size2[6]->out_loadlvl[0]_out mux_1level_tapbuf_size2[6]->out mux_1level_tapbuf_size2[6]->out_loadlvl[0]_out mux_1level_tapbuf_size2[6]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[16]_no0 mux_1level_tapbuf_size2[6]->out_loadlvl[0]_out mux_1level_tapbuf_size2[6]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[17]_no0 mux_1level_tapbuf_size2[6]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[6]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[18]_no0 mux_1level_tapbuf_size2[6]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[6]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to6] 
+          param='sum_leakage_power_sb_mux[0to5]+leakage_sb_mux[1][1]_rrnode[504]'
.meas tran sum_energy_per_cycle_sb_mux[0to6] 
+          param='sum_energy_per_cycle_sb_mux[0to5]+energy_per_cycle_sb_mux[1][1]_rrnode[504]'
Xmux_1level_tapbuf_size2[7] mux_1level_tapbuf_size2[7]->in[0] mux_1level_tapbuf_size2[7]->in[1] mux_1level_tapbuf_size2[7]->out sram[17]->outb sram[17]->out gvdd_mux_1level_tapbuf_size2[7] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[7], level=1, select_path_id=0. *****
*****1*****
Xsram[17] sram->in sram[17]->out sram[17]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[17]->out) 0
.nodeset V(sram[17]->outb) vsp
***** Signal mux_1level_tapbuf_size2[7]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[7]->in[0] mux_1level_tapbuf_size2[7]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[7]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[7]->in[1] mux_1level_tapbuf_size2[7]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[7] gvdd_mux_1level_tapbuf_size2[7] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[506] trig v(mux_1level_tapbuf_size2[7]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[7]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[506] trig v(mux_1level_tapbuf_size2[7]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[7]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[506] when v(mux_1level_tapbuf_size2[7]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[506] trig v(mux_1level_tapbuf_size2[7]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[7]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[506] when v(mux_1level_tapbuf_size2[7]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[506] trig v(mux_1level_tapbuf_size2[7]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[7]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[7]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[7]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[506] param='mux_1level_tapbuf_size2[7]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[7]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[7]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[7]_energy_per_cycle param='mux_1level_tapbuf_size2[7]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[506]  param='mux_1level_tapbuf_size2[7]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[506]  param='dynamic_power_sb_mux[1][1]_rrnode[506]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[506] avg p(Vgvdd_mux_1level_tapbuf_size2[7]) from='start_rise_sb_mux[1][1]_rrnode[506]' to='start_rise_sb_mux[1][1]_rrnode[506]+switch_rise_sb_mux[1][1]_rrnode[506]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[506] avg p(Vgvdd_mux_1level_tapbuf_size2[7]) from='start_fall_sb_mux[1][1]_rrnode[506]' to='start_fall_sb_mux[1][1]_rrnode[506]+switch_fall_sb_mux[1][1]_rrnode[506]'
.meas tran sum_leakage_power_mux[0to7] 
+          param='sum_leakage_power_mux[0to6]+leakage_sb_mux[1][1]_rrnode[506]'
.meas tran sum_energy_per_cycle_mux[0to7] 
+          param='sum_energy_per_cycle_mux[0to6]+energy_per_cycle_sb_mux[1][1]_rrnode[506]'
***** Load for rr_node[506] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=15, type=5 *****
Xchan_mux_1level_tapbuf_size2[7]->out_loadlvl[0]_out mux_1level_tapbuf_size2[7]->out mux_1level_tapbuf_size2[7]->out_loadlvl[0]_out mux_1level_tapbuf_size2[7]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[19]_no0 mux_1level_tapbuf_size2[7]->out_loadlvl[0]_out mux_1level_tapbuf_size2[7]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[20]_no0 mux_1level_tapbuf_size2[7]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[7]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to7] 
+          param='sum_leakage_power_sb_mux[0to6]+leakage_sb_mux[1][1]_rrnode[506]'
.meas tran sum_energy_per_cycle_sb_mux[0to7] 
+          param='sum_energy_per_cycle_sb_mux[0to6]+energy_per_cycle_sb_mux[1][1]_rrnode[506]'
Xmux_1level_tapbuf_size2[8] mux_1level_tapbuf_size2[8]->in[0] mux_1level_tapbuf_size2[8]->in[1] mux_1level_tapbuf_size2[8]->out sram[18]->outb sram[18]->out gvdd_mux_1level_tapbuf_size2[8] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[8], level=1, select_path_id=0. *****
*****1*****
Xsram[18] sram->in sram[18]->out sram[18]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[18]->out) 0
.nodeset V(sram[18]->outb) vsp
***** Signal mux_1level_tapbuf_size2[8]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[8]->in[0] mux_1level_tapbuf_size2[8]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[8]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[8]->in[1] mux_1level_tapbuf_size2[8]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[8] gvdd_mux_1level_tapbuf_size2[8] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[508] trig v(mux_1level_tapbuf_size2[8]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[8]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[508] trig v(mux_1level_tapbuf_size2[8]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[8]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[508] when v(mux_1level_tapbuf_size2[8]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[508] trig v(mux_1level_tapbuf_size2[8]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[8]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[508] when v(mux_1level_tapbuf_size2[8]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[508] trig v(mux_1level_tapbuf_size2[8]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[8]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[8]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[8]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[508] param='mux_1level_tapbuf_size2[8]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[8]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[8]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[8]_energy_per_cycle param='mux_1level_tapbuf_size2[8]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[508]  param='mux_1level_tapbuf_size2[8]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[508]  param='dynamic_power_sb_mux[1][1]_rrnode[508]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[508] avg p(Vgvdd_mux_1level_tapbuf_size2[8]) from='start_rise_sb_mux[1][1]_rrnode[508]' to='start_rise_sb_mux[1][1]_rrnode[508]+switch_rise_sb_mux[1][1]_rrnode[508]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[508] avg p(Vgvdd_mux_1level_tapbuf_size2[8]) from='start_fall_sb_mux[1][1]_rrnode[508]' to='start_fall_sb_mux[1][1]_rrnode[508]+switch_fall_sb_mux[1][1]_rrnode[508]'
.meas tran sum_leakage_power_mux[0to8] 
+          param='sum_leakage_power_mux[0to7]+leakage_sb_mux[1][1]_rrnode[508]'
.meas tran sum_energy_per_cycle_mux[0to8] 
+          param='sum_energy_per_cycle_mux[0to7]+energy_per_cycle_sb_mux[1][1]_rrnode[508]'
***** Load for rr_node[508] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=17, type=5 *****
Xchan_mux_1level_tapbuf_size2[8]->out_loadlvl[0]_out mux_1level_tapbuf_size2[8]->out mux_1level_tapbuf_size2[8]->out_loadlvl[0]_out mux_1level_tapbuf_size2[8]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[21]_no0 mux_1level_tapbuf_size2[8]->out_loadlvl[0]_out mux_1level_tapbuf_size2[8]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[22]_no0 mux_1level_tapbuf_size2[8]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[8]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to8] 
+          param='sum_leakage_power_sb_mux[0to7]+leakage_sb_mux[1][1]_rrnode[508]'
.meas tran sum_energy_per_cycle_sb_mux[0to8] 
+          param='sum_energy_per_cycle_sb_mux[0to7]+energy_per_cycle_sb_mux[1][1]_rrnode[508]'
Xmux_1level_tapbuf_size2[9] mux_1level_tapbuf_size2[9]->in[0] mux_1level_tapbuf_size2[9]->in[1] mux_1level_tapbuf_size2[9]->out sram[19]->outb sram[19]->out gvdd_mux_1level_tapbuf_size2[9] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[9], level=1, select_path_id=0. *****
*****1*****
Xsram[19] sram->in sram[19]->out sram[19]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[19]->out) 0
.nodeset V(sram[19]->outb) vsp
***** Signal mux_1level_tapbuf_size2[9]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[9]->in[0] mux_1level_tapbuf_size2[9]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[9]->in[1] density = 0.2026, probability=0.4982.*****
Vmux_1level_tapbuf_size2[9]->in[1] mux_1level_tapbuf_size2[9]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgvdd_mux_1level_tapbuf_size2[9] gvdd_mux_1level_tapbuf_size2[9] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[510] trig v(mux_1level_tapbuf_size2[9]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[9]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[510] trig v(mux_1level_tapbuf_size2[9]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[9]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[510] when v(mux_1level_tapbuf_size2[9]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[510] trig v(mux_1level_tapbuf_size2[9]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[9]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[510] when v(mux_1level_tapbuf_size2[9]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[510] trig v(mux_1level_tapbuf_size2[9]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[9]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[9]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[9]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[510] param='mux_1level_tapbuf_size2[9]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[9]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[9]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[9]_energy_per_cycle param='mux_1level_tapbuf_size2[9]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[510]  param='mux_1level_tapbuf_size2[9]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[510]  param='dynamic_power_sb_mux[1][1]_rrnode[510]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[510] avg p(Vgvdd_mux_1level_tapbuf_size2[9]) from='start_rise_sb_mux[1][1]_rrnode[510]' to='start_rise_sb_mux[1][1]_rrnode[510]+switch_rise_sb_mux[1][1]_rrnode[510]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[510] avg p(Vgvdd_mux_1level_tapbuf_size2[9]) from='start_fall_sb_mux[1][1]_rrnode[510]' to='start_fall_sb_mux[1][1]_rrnode[510]+switch_fall_sb_mux[1][1]_rrnode[510]'
.meas tran sum_leakage_power_mux[0to9] 
+          param='sum_leakage_power_mux[0to8]+leakage_sb_mux[1][1]_rrnode[510]'
.meas tran sum_energy_per_cycle_mux[0to9] 
+          param='sum_energy_per_cycle_mux[0to8]+energy_per_cycle_sb_mux[1][1]_rrnode[510]'
***** Load for rr_node[510] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=19, type=5 *****
Xchan_mux_1level_tapbuf_size2[9]->out_loadlvl[0]_out mux_1level_tapbuf_size2[9]->out mux_1level_tapbuf_size2[9]->out_loadlvl[0]_out mux_1level_tapbuf_size2[9]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[23]_no0 mux_1level_tapbuf_size2[9]->out_loadlvl[0]_out mux_1level_tapbuf_size2[9]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[24]_no0 mux_1level_tapbuf_size2[9]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[9]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[25]_no0 mux_1level_tapbuf_size2[9]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[9]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to9] 
+          param='sum_leakage_power_sb_mux[0to8]+leakage_sb_mux[1][1]_rrnode[510]'
.meas tran sum_energy_per_cycle_sb_mux[0to9] 
+          param='sum_energy_per_cycle_sb_mux[0to8]+energy_per_cycle_sb_mux[1][1]_rrnode[510]'
Xmux_1level_tapbuf_size2[10] mux_1level_tapbuf_size2[10]->in[0] mux_1level_tapbuf_size2[10]->in[1] mux_1level_tapbuf_size2[10]->out sram[20]->outb sram[20]->out gvdd_mux_1level_tapbuf_size2[10] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[10], level=1, select_path_id=0. *****
*****1*****
Xsram[20] sram->in sram[20]->out sram[20]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[20]->out) 0
.nodeset V(sram[20]->outb) vsp
***** Signal mux_1level_tapbuf_size2[10]->in[0] density = 0.2026, probability=0.4982.*****
Vmux_1level_tapbuf_size2[10]->in[0] mux_1level_tapbuf_size2[10]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
***** Signal mux_1level_tapbuf_size2[10]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[10]->in[1] mux_1level_tapbuf_size2[10]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[10] gvdd_mux_1level_tapbuf_size2[10] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[512] trig v(mux_1level_tapbuf_size2[10]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[10]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[512] trig v(mux_1level_tapbuf_size2[10]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[10]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[512] when v(mux_1level_tapbuf_size2[10]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[512] trig v(mux_1level_tapbuf_size2[10]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[10]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[512] when v(mux_1level_tapbuf_size2[10]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[512] trig v(mux_1level_tapbuf_size2[10]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[10]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[10]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[10]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[512] param='mux_1level_tapbuf_size2[10]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[10]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[10]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[10]_energy_per_cycle param='mux_1level_tapbuf_size2[10]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[512]  param='mux_1level_tapbuf_size2[10]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[512]  param='dynamic_power_sb_mux[1][1]_rrnode[512]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[512] avg p(Vgvdd_mux_1level_tapbuf_size2[10]) from='start_rise_sb_mux[1][1]_rrnode[512]' to='start_rise_sb_mux[1][1]_rrnode[512]+switch_rise_sb_mux[1][1]_rrnode[512]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[512] avg p(Vgvdd_mux_1level_tapbuf_size2[10]) from='start_fall_sb_mux[1][1]_rrnode[512]' to='start_fall_sb_mux[1][1]_rrnode[512]+switch_fall_sb_mux[1][1]_rrnode[512]'
.meas tran sum_leakage_power_mux[0to10] 
+          param='sum_leakage_power_mux[0to9]+leakage_sb_mux[1][1]_rrnode[512]'
.meas tran sum_energy_per_cycle_mux[0to10] 
+          param='sum_energy_per_cycle_mux[0to9]+energy_per_cycle_sb_mux[1][1]_rrnode[512]'
***** Load for rr_node[512] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=21, type=5 *****
Xchan_mux_1level_tapbuf_size2[10]->out_loadlvl[0]_out mux_1level_tapbuf_size2[10]->out mux_1level_tapbuf_size2[10]->out_loadlvl[0]_out mux_1level_tapbuf_size2[10]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[26]_no0 mux_1level_tapbuf_size2[10]->out_loadlvl[0]_out mux_1level_tapbuf_size2[10]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[27]_no0 mux_1level_tapbuf_size2[10]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[10]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to10] 
+          param='sum_leakage_power_sb_mux[0to9]+leakage_sb_mux[1][1]_rrnode[512]'
.meas tran sum_energy_per_cycle_sb_mux[0to10] 
+          param='sum_energy_per_cycle_sb_mux[0to9]+energy_per_cycle_sb_mux[1][1]_rrnode[512]'
Xmux_1level_tapbuf_size2[11] mux_1level_tapbuf_size2[11]->in[0] mux_1level_tapbuf_size2[11]->in[1] mux_1level_tapbuf_size2[11]->out sram[21]->outb sram[21]->out gvdd_mux_1level_tapbuf_size2[11] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[11], level=1, select_path_id=0. *****
*****1*****
Xsram[21] sram->in sram[21]->out sram[21]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[21]->out) 0
.nodeset V(sram[21]->outb) vsp
***** Signal mux_1level_tapbuf_size2[11]->in[0] density = 0.2026, probability=0.4982.*****
Vmux_1level_tapbuf_size2[11]->in[0] mux_1level_tapbuf_size2[11]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
***** Signal mux_1level_tapbuf_size2[11]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[11]->in[1] mux_1level_tapbuf_size2[11]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[11] gvdd_mux_1level_tapbuf_size2[11] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[514] trig v(mux_1level_tapbuf_size2[11]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[11]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[514] trig v(mux_1level_tapbuf_size2[11]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[11]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[514] when v(mux_1level_tapbuf_size2[11]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[514] trig v(mux_1level_tapbuf_size2[11]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[11]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[514] when v(mux_1level_tapbuf_size2[11]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[514] trig v(mux_1level_tapbuf_size2[11]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[11]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[11]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[11]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[514] param='mux_1level_tapbuf_size2[11]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[11]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[11]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[11]_energy_per_cycle param='mux_1level_tapbuf_size2[11]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[514]  param='mux_1level_tapbuf_size2[11]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[514]  param='dynamic_power_sb_mux[1][1]_rrnode[514]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[514] avg p(Vgvdd_mux_1level_tapbuf_size2[11]) from='start_rise_sb_mux[1][1]_rrnode[514]' to='start_rise_sb_mux[1][1]_rrnode[514]+switch_rise_sb_mux[1][1]_rrnode[514]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[514] avg p(Vgvdd_mux_1level_tapbuf_size2[11]) from='start_fall_sb_mux[1][1]_rrnode[514]' to='start_fall_sb_mux[1][1]_rrnode[514]+switch_fall_sb_mux[1][1]_rrnode[514]'
.meas tran sum_leakage_power_mux[0to11] 
+          param='sum_leakage_power_mux[0to10]+leakage_sb_mux[1][1]_rrnode[514]'
.meas tran sum_energy_per_cycle_mux[0to11] 
+          param='sum_energy_per_cycle_mux[0to10]+energy_per_cycle_sb_mux[1][1]_rrnode[514]'
***** Load for rr_node[514] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=23, type=5 *****
Xchan_mux_1level_tapbuf_size2[11]->out_loadlvl[0]_out mux_1level_tapbuf_size2[11]->out mux_1level_tapbuf_size2[11]->out_loadlvl[0]_out mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[28]_no0 mux_1level_tapbuf_size2[11]->out_loadlvl[0]_out mux_1level_tapbuf_size2[11]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[29]_no0 mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[30]_no0 mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[11]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to11] 
+          param='sum_leakage_power_sb_mux[0to10]+leakage_sb_mux[1][1]_rrnode[514]'
.meas tran sum_energy_per_cycle_sb_mux[0to11] 
+          param='sum_energy_per_cycle_sb_mux[0to10]+energy_per_cycle_sb_mux[1][1]_rrnode[514]'
Xmux_1level_tapbuf_size2[12] mux_1level_tapbuf_size2[12]->in[0] mux_1level_tapbuf_size2[12]->in[1] mux_1level_tapbuf_size2[12]->out sram[22]->outb sram[22]->out gvdd_mux_1level_tapbuf_size2[12] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[12], level=1, select_path_id=0. *****
*****1*****
Xsram[22] sram->in sram[22]->out sram[22]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[22]->out) 0
.nodeset V(sram[22]->outb) vsp
***** Signal mux_1level_tapbuf_size2[12]->in[0] density = 0.2026, probability=0.4982.*****
Vmux_1level_tapbuf_size2[12]->in[0] mux_1level_tapbuf_size2[12]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
***** Signal mux_1level_tapbuf_size2[12]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[12]->in[1] mux_1level_tapbuf_size2[12]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[12] gvdd_mux_1level_tapbuf_size2[12] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[516] trig v(mux_1level_tapbuf_size2[12]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[12]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[516] trig v(mux_1level_tapbuf_size2[12]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[12]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[516] when v(mux_1level_tapbuf_size2[12]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[516] trig v(mux_1level_tapbuf_size2[12]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[12]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[516] when v(mux_1level_tapbuf_size2[12]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[516] trig v(mux_1level_tapbuf_size2[12]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[12]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[12]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[12]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[516] param='mux_1level_tapbuf_size2[12]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[12]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[12]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[12]_energy_per_cycle param='mux_1level_tapbuf_size2[12]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[516]  param='mux_1level_tapbuf_size2[12]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[516]  param='dynamic_power_sb_mux[1][1]_rrnode[516]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[516] avg p(Vgvdd_mux_1level_tapbuf_size2[12]) from='start_rise_sb_mux[1][1]_rrnode[516]' to='start_rise_sb_mux[1][1]_rrnode[516]+switch_rise_sb_mux[1][1]_rrnode[516]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[516] avg p(Vgvdd_mux_1level_tapbuf_size2[12]) from='start_fall_sb_mux[1][1]_rrnode[516]' to='start_fall_sb_mux[1][1]_rrnode[516]+switch_fall_sb_mux[1][1]_rrnode[516]'
.meas tran sum_leakage_power_mux[0to12] 
+          param='sum_leakage_power_mux[0to11]+leakage_sb_mux[1][1]_rrnode[516]'
.meas tran sum_energy_per_cycle_mux[0to12] 
+          param='sum_energy_per_cycle_mux[0to11]+energy_per_cycle_sb_mux[1][1]_rrnode[516]'
***** Load for rr_node[516] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=25, type=5 *****
Xchan_mux_1level_tapbuf_size2[12]->out_loadlvl[0]_out mux_1level_tapbuf_size2[12]->out mux_1level_tapbuf_size2[12]->out_loadlvl[0]_out mux_1level_tapbuf_size2[12]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[31]_no0 mux_1level_tapbuf_size2[12]->out_loadlvl[0]_out mux_1level_tapbuf_size2[12]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[32]_no0 mux_1level_tapbuf_size2[12]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[12]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[33]_no0 mux_1level_tapbuf_size2[12]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[12]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to12] 
+          param='sum_leakage_power_sb_mux[0to11]+leakage_sb_mux[1][1]_rrnode[516]'
.meas tran sum_energy_per_cycle_sb_mux[0to12] 
+          param='sum_energy_per_cycle_sb_mux[0to11]+energy_per_cycle_sb_mux[1][1]_rrnode[516]'
Xmux_1level_tapbuf_size2[13] mux_1level_tapbuf_size2[13]->in[0] mux_1level_tapbuf_size2[13]->in[1] mux_1level_tapbuf_size2[13]->out sram[23]->outb sram[23]->out gvdd_mux_1level_tapbuf_size2[13] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[13], level=1, select_path_id=0. *****
*****1*****
Xsram[23] sram->in sram[23]->out sram[23]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[23]->out) 0
.nodeset V(sram[23]->outb) vsp
***** Signal mux_1level_tapbuf_size2[13]->in[0] density = 0.2026, probability=0.4982.*****
Vmux_1level_tapbuf_size2[13]->in[0] mux_1level_tapbuf_size2[13]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
***** Signal mux_1level_tapbuf_size2[13]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[13]->in[1] mux_1level_tapbuf_size2[13]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[13] gvdd_mux_1level_tapbuf_size2[13] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[518] trig v(mux_1level_tapbuf_size2[13]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[13]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[518] trig v(mux_1level_tapbuf_size2[13]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[13]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[518] when v(mux_1level_tapbuf_size2[13]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[518] trig v(mux_1level_tapbuf_size2[13]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[13]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[518] when v(mux_1level_tapbuf_size2[13]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[518] trig v(mux_1level_tapbuf_size2[13]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[13]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[13]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[13]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[518] param='mux_1level_tapbuf_size2[13]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[13]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[13]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[13]_energy_per_cycle param='mux_1level_tapbuf_size2[13]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[518]  param='mux_1level_tapbuf_size2[13]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[518]  param='dynamic_power_sb_mux[1][1]_rrnode[518]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[518] avg p(Vgvdd_mux_1level_tapbuf_size2[13]) from='start_rise_sb_mux[1][1]_rrnode[518]' to='start_rise_sb_mux[1][1]_rrnode[518]+switch_rise_sb_mux[1][1]_rrnode[518]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[518] avg p(Vgvdd_mux_1level_tapbuf_size2[13]) from='start_fall_sb_mux[1][1]_rrnode[518]' to='start_fall_sb_mux[1][1]_rrnode[518]+switch_fall_sb_mux[1][1]_rrnode[518]'
.meas tran sum_leakage_power_mux[0to13] 
+          param='sum_leakage_power_mux[0to12]+leakage_sb_mux[1][1]_rrnode[518]'
.meas tran sum_energy_per_cycle_mux[0to13] 
+          param='sum_energy_per_cycle_mux[0to12]+energy_per_cycle_sb_mux[1][1]_rrnode[518]'
***** Load for rr_node[518] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=27, type=5 *****
Xchan_mux_1level_tapbuf_size2[13]->out_loadlvl[0]_out mux_1level_tapbuf_size2[13]->out mux_1level_tapbuf_size2[13]->out_loadlvl[0]_out mux_1level_tapbuf_size2[13]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[34]_no0 mux_1level_tapbuf_size2[13]->out_loadlvl[0]_out mux_1level_tapbuf_size2[13]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[35]_no0 mux_1level_tapbuf_size2[13]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[13]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to13] 
+          param='sum_leakage_power_sb_mux[0to12]+leakage_sb_mux[1][1]_rrnode[518]'
.meas tran sum_energy_per_cycle_sb_mux[0to13] 
+          param='sum_energy_per_cycle_sb_mux[0to12]+energy_per_cycle_sb_mux[1][1]_rrnode[518]'
Xmux_1level_tapbuf_size2[14] mux_1level_tapbuf_size2[14]->in[0] mux_1level_tapbuf_size2[14]->in[1] mux_1level_tapbuf_size2[14]->out sram[24]->outb sram[24]->out gvdd_mux_1level_tapbuf_size2[14] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[14], level=1, select_path_id=0. *****
*****1*****
Xsram[24] sram->in sram[24]->out sram[24]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[24]->out) 0
.nodeset V(sram[24]->outb) vsp
***** Signal mux_1level_tapbuf_size2[14]->in[0] density = 0.2026, probability=0.4982.*****
Vmux_1level_tapbuf_size2[14]->in[0] mux_1level_tapbuf_size2[14]->in[0] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
***** Signal mux_1level_tapbuf_size2[14]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[14]->in[1] mux_1level_tapbuf_size2[14]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[14] gvdd_mux_1level_tapbuf_size2[14] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[520] trig v(mux_1level_tapbuf_size2[14]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[14]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[520] trig v(mux_1level_tapbuf_size2[14]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[14]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[520] when v(mux_1level_tapbuf_size2[14]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[520] trig v(mux_1level_tapbuf_size2[14]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[14]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[520] when v(mux_1level_tapbuf_size2[14]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[520] trig v(mux_1level_tapbuf_size2[14]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[14]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[14]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[14]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[520] param='mux_1level_tapbuf_size2[14]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[14]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[14]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[14]_energy_per_cycle param='mux_1level_tapbuf_size2[14]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[520]  param='mux_1level_tapbuf_size2[14]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[520]  param='dynamic_power_sb_mux[1][1]_rrnode[520]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[520] avg p(Vgvdd_mux_1level_tapbuf_size2[14]) from='start_rise_sb_mux[1][1]_rrnode[520]' to='start_rise_sb_mux[1][1]_rrnode[520]+switch_rise_sb_mux[1][1]_rrnode[520]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[520] avg p(Vgvdd_mux_1level_tapbuf_size2[14]) from='start_fall_sb_mux[1][1]_rrnode[520]' to='start_fall_sb_mux[1][1]_rrnode[520]+switch_fall_sb_mux[1][1]_rrnode[520]'
.meas tran sum_leakage_power_mux[0to14] 
+          param='sum_leakage_power_mux[0to13]+leakage_sb_mux[1][1]_rrnode[520]'
.meas tran sum_energy_per_cycle_mux[0to14] 
+          param='sum_energy_per_cycle_mux[0to13]+energy_per_cycle_sb_mux[1][1]_rrnode[520]'
***** Load for rr_node[520] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=29, type=5 *****
Xchan_mux_1level_tapbuf_size2[14]->out_loadlvl[0]_out mux_1level_tapbuf_size2[14]->out mux_1level_tapbuf_size2[14]->out_loadlvl[0]_out mux_1level_tapbuf_size2[14]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[36]_no0 mux_1level_tapbuf_size2[14]->out_loadlvl[0]_out mux_1level_tapbuf_size2[14]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[37]_no0 mux_1level_tapbuf_size2[14]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[14]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[38]_no0 mux_1level_tapbuf_size2[14]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[14]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to14] 
+          param='sum_leakage_power_sb_mux[0to13]+leakage_sb_mux[1][1]_rrnode[520]'
.meas tran sum_energy_per_cycle_sb_mux[0to14] 
+          param='sum_energy_per_cycle_sb_mux[0to13]+energy_per_cycle_sb_mux[1][1]_rrnode[520]'
Xmux_1level_tapbuf_size2[15] mux_1level_tapbuf_size2[15]->in[0] mux_1level_tapbuf_size2[15]->in[1] mux_1level_tapbuf_size2[15]->out sram[25]->outb sram[25]->out gvdd_mux_1level_tapbuf_size2[15] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[15], level=1, select_path_id=0. *****
*****1*****
Xsram[25] sram->in sram[25]->out sram[25]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[25]->out) 0
.nodeset V(sram[25]->outb) vsp
***** Signal mux_1level_tapbuf_size2[15]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[15]->in[0] mux_1level_tapbuf_size2[15]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[15]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[15]->in[1] mux_1level_tapbuf_size2[15]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[15] gvdd_mux_1level_tapbuf_size2[15] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[522] trig v(mux_1level_tapbuf_size2[15]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[15]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[522] trig v(mux_1level_tapbuf_size2[15]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[15]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[522] when v(mux_1level_tapbuf_size2[15]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[522] trig v(mux_1level_tapbuf_size2[15]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[15]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[522] when v(mux_1level_tapbuf_size2[15]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[522] trig v(mux_1level_tapbuf_size2[15]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[15]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[15]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[15]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[522] param='mux_1level_tapbuf_size2[15]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[15]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[15]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[15]_energy_per_cycle param='mux_1level_tapbuf_size2[15]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[522]  param='mux_1level_tapbuf_size2[15]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[522]  param='dynamic_power_sb_mux[1][1]_rrnode[522]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[522] avg p(Vgvdd_mux_1level_tapbuf_size2[15]) from='start_rise_sb_mux[1][1]_rrnode[522]' to='start_rise_sb_mux[1][1]_rrnode[522]+switch_rise_sb_mux[1][1]_rrnode[522]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[522] avg p(Vgvdd_mux_1level_tapbuf_size2[15]) from='start_fall_sb_mux[1][1]_rrnode[522]' to='start_fall_sb_mux[1][1]_rrnode[522]+switch_fall_sb_mux[1][1]_rrnode[522]'
.meas tran sum_leakage_power_mux[0to15] 
+          param='sum_leakage_power_mux[0to14]+leakage_sb_mux[1][1]_rrnode[522]'
.meas tran sum_energy_per_cycle_mux[0to15] 
+          param='sum_energy_per_cycle_mux[0to14]+energy_per_cycle_sb_mux[1][1]_rrnode[522]'
***** Load for rr_node[522] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=31, type=5 *****
Xchan_mux_1level_tapbuf_size2[15]->out_loadlvl[0]_out mux_1level_tapbuf_size2[15]->out mux_1level_tapbuf_size2[15]->out_loadlvl[0]_out mux_1level_tapbuf_size2[15]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[39]_no0 mux_1level_tapbuf_size2[15]->out_loadlvl[0]_out mux_1level_tapbuf_size2[15]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[40]_no0 mux_1level_tapbuf_size2[15]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[15]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[41]_no0 mux_1level_tapbuf_size2[15]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[15]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to15] 
+          param='sum_leakage_power_sb_mux[0to14]+leakage_sb_mux[1][1]_rrnode[522]'
.meas tran sum_energy_per_cycle_sb_mux[0to15] 
+          param='sum_energy_per_cycle_sb_mux[0to14]+energy_per_cycle_sb_mux[1][1]_rrnode[522]'
Xmux_1level_tapbuf_size2[16] mux_1level_tapbuf_size2[16]->in[0] mux_1level_tapbuf_size2[16]->in[1] mux_1level_tapbuf_size2[16]->out sram[26]->outb sram[26]->out gvdd_mux_1level_tapbuf_size2[16] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[16], level=1, select_path_id=0. *****
*****1*****
Xsram[26] sram->in sram[26]->out sram[26]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[26]->out) 0
.nodeset V(sram[26]->outb) vsp
***** Signal mux_1level_tapbuf_size2[16]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[16]->in[0] mux_1level_tapbuf_size2[16]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[16]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[16]->in[1] mux_1level_tapbuf_size2[16]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[16] gvdd_mux_1level_tapbuf_size2[16] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[524] trig v(mux_1level_tapbuf_size2[16]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[16]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[524] trig v(mux_1level_tapbuf_size2[16]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[16]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[524] when v(mux_1level_tapbuf_size2[16]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[524] trig v(mux_1level_tapbuf_size2[16]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[16]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[524] when v(mux_1level_tapbuf_size2[16]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[524] trig v(mux_1level_tapbuf_size2[16]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[16]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[16]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[16]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[524] param='mux_1level_tapbuf_size2[16]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[16]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[16]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[16]_energy_per_cycle param='mux_1level_tapbuf_size2[16]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[524]  param='mux_1level_tapbuf_size2[16]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[524]  param='dynamic_power_sb_mux[1][1]_rrnode[524]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[524] avg p(Vgvdd_mux_1level_tapbuf_size2[16]) from='start_rise_sb_mux[1][1]_rrnode[524]' to='start_rise_sb_mux[1][1]_rrnode[524]+switch_rise_sb_mux[1][1]_rrnode[524]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[524] avg p(Vgvdd_mux_1level_tapbuf_size2[16]) from='start_fall_sb_mux[1][1]_rrnode[524]' to='start_fall_sb_mux[1][1]_rrnode[524]+switch_fall_sb_mux[1][1]_rrnode[524]'
.meas tran sum_leakage_power_mux[0to16] 
+          param='sum_leakage_power_mux[0to15]+leakage_sb_mux[1][1]_rrnode[524]'
.meas tran sum_energy_per_cycle_mux[0to16] 
+          param='sum_energy_per_cycle_mux[0to15]+energy_per_cycle_sb_mux[1][1]_rrnode[524]'
***** Load for rr_node[524] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=33, type=5 *****
Xchan_mux_1level_tapbuf_size2[16]->out_loadlvl[0]_out mux_1level_tapbuf_size2[16]->out mux_1level_tapbuf_size2[16]->out_loadlvl[0]_out mux_1level_tapbuf_size2[16]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[42]_no0 mux_1level_tapbuf_size2[16]->out_loadlvl[0]_out mux_1level_tapbuf_size2[16]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[43]_no0 mux_1level_tapbuf_size2[16]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[16]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[44]_no0 mux_1level_tapbuf_size2[16]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[16]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to16] 
+          param='sum_leakage_power_sb_mux[0to15]+leakage_sb_mux[1][1]_rrnode[524]'
.meas tran sum_energy_per_cycle_sb_mux[0to16] 
+          param='sum_energy_per_cycle_sb_mux[0to15]+energy_per_cycle_sb_mux[1][1]_rrnode[524]'
Xmux_1level_tapbuf_size2[17] mux_1level_tapbuf_size2[17]->in[0] mux_1level_tapbuf_size2[17]->in[1] mux_1level_tapbuf_size2[17]->out sram[27]->outb sram[27]->out gvdd_mux_1level_tapbuf_size2[17] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[17], level=1, select_path_id=0. *****
*****1*****
Xsram[27] sram->in sram[27]->out sram[27]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[27]->out) 0
.nodeset V(sram[27]->outb) vsp
***** Signal mux_1level_tapbuf_size2[17]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[17]->in[0] mux_1level_tapbuf_size2[17]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[17]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[17]->in[1] mux_1level_tapbuf_size2[17]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[17] gvdd_mux_1level_tapbuf_size2[17] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[526] trig v(mux_1level_tapbuf_size2[17]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[17]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[526] trig v(mux_1level_tapbuf_size2[17]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[17]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[526] when v(mux_1level_tapbuf_size2[17]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[526] trig v(mux_1level_tapbuf_size2[17]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[17]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[526] when v(mux_1level_tapbuf_size2[17]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[526] trig v(mux_1level_tapbuf_size2[17]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[17]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[17]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[17]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[526] param='mux_1level_tapbuf_size2[17]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[17]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[17]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[17]_energy_per_cycle param='mux_1level_tapbuf_size2[17]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[526]  param='mux_1level_tapbuf_size2[17]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[526]  param='dynamic_power_sb_mux[1][1]_rrnode[526]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[526] avg p(Vgvdd_mux_1level_tapbuf_size2[17]) from='start_rise_sb_mux[1][1]_rrnode[526]' to='start_rise_sb_mux[1][1]_rrnode[526]+switch_rise_sb_mux[1][1]_rrnode[526]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[526] avg p(Vgvdd_mux_1level_tapbuf_size2[17]) from='start_fall_sb_mux[1][1]_rrnode[526]' to='start_fall_sb_mux[1][1]_rrnode[526]+switch_fall_sb_mux[1][1]_rrnode[526]'
.meas tran sum_leakage_power_mux[0to17] 
+          param='sum_leakage_power_mux[0to16]+leakage_sb_mux[1][1]_rrnode[526]'
.meas tran sum_energy_per_cycle_mux[0to17] 
+          param='sum_energy_per_cycle_mux[0to16]+energy_per_cycle_sb_mux[1][1]_rrnode[526]'
***** Load for rr_node[526] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=35, type=5 *****
Xchan_mux_1level_tapbuf_size2[17]->out_loadlvl[0]_out mux_1level_tapbuf_size2[17]->out mux_1level_tapbuf_size2[17]->out_loadlvl[0]_out mux_1level_tapbuf_size2[17]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[45]_no0 mux_1level_tapbuf_size2[17]->out_loadlvl[0]_out mux_1level_tapbuf_size2[17]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[46]_no0 mux_1level_tapbuf_size2[17]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[17]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[47]_no0 mux_1level_tapbuf_size2[17]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[17]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to17] 
+          param='sum_leakage_power_sb_mux[0to16]+leakage_sb_mux[1][1]_rrnode[526]'
.meas tran sum_energy_per_cycle_sb_mux[0to17] 
+          param='sum_energy_per_cycle_sb_mux[0to16]+energy_per_cycle_sb_mux[1][1]_rrnode[526]'
Xmux_1level_tapbuf_size2[18] mux_1level_tapbuf_size2[18]->in[0] mux_1level_tapbuf_size2[18]->in[1] mux_1level_tapbuf_size2[18]->out sram[28]->outb sram[28]->out gvdd_mux_1level_tapbuf_size2[18] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[18], level=1, select_path_id=0. *****
*****1*****
Xsram[28] sram->in sram[28]->out sram[28]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[28]->out) 0
.nodeset V(sram[28]->outb) vsp
***** Signal mux_1level_tapbuf_size2[18]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[18]->in[0] mux_1level_tapbuf_size2[18]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[18]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[18]->in[1] mux_1level_tapbuf_size2[18]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[18] gvdd_mux_1level_tapbuf_size2[18] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[528] trig v(mux_1level_tapbuf_size2[18]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[18]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[528] trig v(mux_1level_tapbuf_size2[18]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[18]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[528] when v(mux_1level_tapbuf_size2[18]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[528] trig v(mux_1level_tapbuf_size2[18]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[18]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[528] when v(mux_1level_tapbuf_size2[18]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[528] trig v(mux_1level_tapbuf_size2[18]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[18]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[18]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[18]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[528] param='mux_1level_tapbuf_size2[18]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[18]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[18]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[18]_energy_per_cycle param='mux_1level_tapbuf_size2[18]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[528]  param='mux_1level_tapbuf_size2[18]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[528]  param='dynamic_power_sb_mux[1][1]_rrnode[528]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[528] avg p(Vgvdd_mux_1level_tapbuf_size2[18]) from='start_rise_sb_mux[1][1]_rrnode[528]' to='start_rise_sb_mux[1][1]_rrnode[528]+switch_rise_sb_mux[1][1]_rrnode[528]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[528] avg p(Vgvdd_mux_1level_tapbuf_size2[18]) from='start_fall_sb_mux[1][1]_rrnode[528]' to='start_fall_sb_mux[1][1]_rrnode[528]+switch_fall_sb_mux[1][1]_rrnode[528]'
.meas tran sum_leakage_power_mux[0to18] 
+          param='sum_leakage_power_mux[0to17]+leakage_sb_mux[1][1]_rrnode[528]'
.meas tran sum_energy_per_cycle_mux[0to18] 
+          param='sum_energy_per_cycle_mux[0to17]+energy_per_cycle_sb_mux[1][1]_rrnode[528]'
***** Load for rr_node[528] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=37, type=5 *****
Xchan_mux_1level_tapbuf_size2[18]->out_loadlvl[0]_out mux_1level_tapbuf_size2[18]->out mux_1level_tapbuf_size2[18]->out_loadlvl[0]_out mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[48]_no0 mux_1level_tapbuf_size2[18]->out_loadlvl[0]_out mux_1level_tapbuf_size2[18]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[49]_no0 mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[18]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to18] 
+          param='sum_leakage_power_sb_mux[0to17]+leakage_sb_mux[1][1]_rrnode[528]'
.meas tran sum_energy_per_cycle_sb_mux[0to18] 
+          param='sum_energy_per_cycle_sb_mux[0to17]+energy_per_cycle_sb_mux[1][1]_rrnode[528]'
Xmux_1level_tapbuf_size2[19] mux_1level_tapbuf_size2[19]->in[0] mux_1level_tapbuf_size2[19]->in[1] mux_1level_tapbuf_size2[19]->out sram[29]->outb sram[29]->out gvdd_mux_1level_tapbuf_size2[19] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[19], level=1, select_path_id=0. *****
*****1*****
Xsram[29] sram->in sram[29]->out sram[29]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[29]->out) 0
.nodeset V(sram[29]->outb) vsp
***** Signal mux_1level_tapbuf_size2[19]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[19]->in[0] mux_1level_tapbuf_size2[19]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[19]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[19]->in[1] mux_1level_tapbuf_size2[19]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[19] gvdd_mux_1level_tapbuf_size2[19] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[530] trig v(mux_1level_tapbuf_size2[19]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[19]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[530] trig v(mux_1level_tapbuf_size2[19]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[19]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[530] when v(mux_1level_tapbuf_size2[19]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[530] trig v(mux_1level_tapbuf_size2[19]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[19]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[530] when v(mux_1level_tapbuf_size2[19]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[530] trig v(mux_1level_tapbuf_size2[19]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[19]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[19]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[19]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[530] param='mux_1level_tapbuf_size2[19]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[19]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[19]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[19]_energy_per_cycle param='mux_1level_tapbuf_size2[19]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[530]  param='mux_1level_tapbuf_size2[19]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[530]  param='dynamic_power_sb_mux[1][1]_rrnode[530]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[530] avg p(Vgvdd_mux_1level_tapbuf_size2[19]) from='start_rise_sb_mux[1][1]_rrnode[530]' to='start_rise_sb_mux[1][1]_rrnode[530]+switch_rise_sb_mux[1][1]_rrnode[530]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[530] avg p(Vgvdd_mux_1level_tapbuf_size2[19]) from='start_fall_sb_mux[1][1]_rrnode[530]' to='start_fall_sb_mux[1][1]_rrnode[530]+switch_fall_sb_mux[1][1]_rrnode[530]'
.meas tran sum_leakage_power_mux[0to19] 
+          param='sum_leakage_power_mux[0to18]+leakage_sb_mux[1][1]_rrnode[530]'
.meas tran sum_energy_per_cycle_mux[0to19] 
+          param='sum_energy_per_cycle_mux[0to18]+energy_per_cycle_sb_mux[1][1]_rrnode[530]'
***** Load for rr_node[530] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=39, type=5 *****
Xchan_mux_1level_tapbuf_size2[19]->out_loadlvl[0]_out mux_1level_tapbuf_size2[19]->out mux_1level_tapbuf_size2[19]->out_loadlvl[0]_out mux_1level_tapbuf_size2[19]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[50]_no0 mux_1level_tapbuf_size2[19]->out_loadlvl[0]_out mux_1level_tapbuf_size2[19]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[51]_no0 mux_1level_tapbuf_size2[19]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[19]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to19] 
+          param='sum_leakage_power_sb_mux[0to18]+leakage_sb_mux[1][1]_rrnode[530]'
.meas tran sum_energy_per_cycle_sb_mux[0to19] 
+          param='sum_energy_per_cycle_sb_mux[0to18]+energy_per_cycle_sb_mux[1][1]_rrnode[530]'
Xmux_1level_tapbuf_size2[20] mux_1level_tapbuf_size2[20]->in[0] mux_1level_tapbuf_size2[20]->in[1] mux_1level_tapbuf_size2[20]->out sram[30]->outb sram[30]->out gvdd_mux_1level_tapbuf_size2[20] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[20], level=1, select_path_id=0. *****
*****1*****
Xsram[30] sram->in sram[30]->out sram[30]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[30]->out) 0
.nodeset V(sram[30]->outb) vsp
***** Signal mux_1level_tapbuf_size2[20]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[20]->in[0] mux_1level_tapbuf_size2[20]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[20]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[20]->in[1] mux_1level_tapbuf_size2[20]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[20] gvdd_mux_1level_tapbuf_size2[20] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[532] trig v(mux_1level_tapbuf_size2[20]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[20]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[532] trig v(mux_1level_tapbuf_size2[20]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[20]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[532] when v(mux_1level_tapbuf_size2[20]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[532] trig v(mux_1level_tapbuf_size2[20]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[20]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[532] when v(mux_1level_tapbuf_size2[20]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[532] trig v(mux_1level_tapbuf_size2[20]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[20]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[20]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[20]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[532] param='mux_1level_tapbuf_size2[20]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[20]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[20]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[20]_energy_per_cycle param='mux_1level_tapbuf_size2[20]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[532]  param='mux_1level_tapbuf_size2[20]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[532]  param='dynamic_power_sb_mux[1][1]_rrnode[532]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[532] avg p(Vgvdd_mux_1level_tapbuf_size2[20]) from='start_rise_sb_mux[1][1]_rrnode[532]' to='start_rise_sb_mux[1][1]_rrnode[532]+switch_rise_sb_mux[1][1]_rrnode[532]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[532] avg p(Vgvdd_mux_1level_tapbuf_size2[20]) from='start_fall_sb_mux[1][1]_rrnode[532]' to='start_fall_sb_mux[1][1]_rrnode[532]+switch_fall_sb_mux[1][1]_rrnode[532]'
.meas tran sum_leakage_power_mux[0to20] 
+          param='sum_leakage_power_mux[0to19]+leakage_sb_mux[1][1]_rrnode[532]'
.meas tran sum_energy_per_cycle_mux[0to20] 
+          param='sum_energy_per_cycle_mux[0to19]+energy_per_cycle_sb_mux[1][1]_rrnode[532]'
***** Load for rr_node[532] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=41, type=5 *****
Xchan_mux_1level_tapbuf_size2[20]->out_loadlvl[0]_out mux_1level_tapbuf_size2[20]->out mux_1level_tapbuf_size2[20]->out_loadlvl[0]_out mux_1level_tapbuf_size2[20]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[52]_no0 mux_1level_tapbuf_size2[20]->out_loadlvl[0]_out mux_1level_tapbuf_size2[20]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[53]_no0 mux_1level_tapbuf_size2[20]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[20]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to20] 
+          param='sum_leakage_power_sb_mux[0to19]+leakage_sb_mux[1][1]_rrnode[532]'
.meas tran sum_energy_per_cycle_sb_mux[0to20] 
+          param='sum_energy_per_cycle_sb_mux[0to19]+energy_per_cycle_sb_mux[1][1]_rrnode[532]'
Xmux_1level_tapbuf_size2[21] mux_1level_tapbuf_size2[21]->in[0] mux_1level_tapbuf_size2[21]->in[1] mux_1level_tapbuf_size2[21]->out sram[31]->outb sram[31]->out gvdd_mux_1level_tapbuf_size2[21] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[21], level=1, select_path_id=0. *****
*****1*****
Xsram[31] sram->in sram[31]->out sram[31]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[31]->out) 0
.nodeset V(sram[31]->outb) vsp
***** Signal mux_1level_tapbuf_size2[21]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[21]->in[0] mux_1level_tapbuf_size2[21]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[21]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[21]->in[1] mux_1level_tapbuf_size2[21]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[21] gvdd_mux_1level_tapbuf_size2[21] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[534] trig v(mux_1level_tapbuf_size2[21]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[21]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[534] trig v(mux_1level_tapbuf_size2[21]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[21]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[534] when v(mux_1level_tapbuf_size2[21]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[534] trig v(mux_1level_tapbuf_size2[21]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[21]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[534] when v(mux_1level_tapbuf_size2[21]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[534] trig v(mux_1level_tapbuf_size2[21]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[21]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[21]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[21]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[534] param='mux_1level_tapbuf_size2[21]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[21]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[21]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[21]_energy_per_cycle param='mux_1level_tapbuf_size2[21]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[534]  param='mux_1level_tapbuf_size2[21]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[534]  param='dynamic_power_sb_mux[1][1]_rrnode[534]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[534] avg p(Vgvdd_mux_1level_tapbuf_size2[21]) from='start_rise_sb_mux[1][1]_rrnode[534]' to='start_rise_sb_mux[1][1]_rrnode[534]+switch_rise_sb_mux[1][1]_rrnode[534]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[534] avg p(Vgvdd_mux_1level_tapbuf_size2[21]) from='start_fall_sb_mux[1][1]_rrnode[534]' to='start_fall_sb_mux[1][1]_rrnode[534]+switch_fall_sb_mux[1][1]_rrnode[534]'
.meas tran sum_leakage_power_mux[0to21] 
+          param='sum_leakage_power_mux[0to20]+leakage_sb_mux[1][1]_rrnode[534]'
.meas tran sum_energy_per_cycle_mux[0to21] 
+          param='sum_energy_per_cycle_mux[0to20]+energy_per_cycle_sb_mux[1][1]_rrnode[534]'
***** Load for rr_node[534] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=43, type=5 *****
Xchan_mux_1level_tapbuf_size2[21]->out_loadlvl[0]_out mux_1level_tapbuf_size2[21]->out mux_1level_tapbuf_size2[21]->out_loadlvl[0]_out mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[54]_no0 mux_1level_tapbuf_size2[21]->out_loadlvl[0]_out mux_1level_tapbuf_size2[21]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[55]_no0 mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[56]_no0 mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[21]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to21] 
+          param='sum_leakage_power_sb_mux[0to20]+leakage_sb_mux[1][1]_rrnode[534]'
.meas tran sum_energy_per_cycle_sb_mux[0to21] 
+          param='sum_energy_per_cycle_sb_mux[0to20]+energy_per_cycle_sb_mux[1][1]_rrnode[534]'
Xmux_1level_tapbuf_size2[22] mux_1level_tapbuf_size2[22]->in[0] mux_1level_tapbuf_size2[22]->in[1] mux_1level_tapbuf_size2[22]->out sram[32]->outb sram[32]->out gvdd_mux_1level_tapbuf_size2[22] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[22], level=1, select_path_id=0. *****
*****1*****
Xsram[32] sram->in sram[32]->out sram[32]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[32]->out) 0
.nodeset V(sram[32]->outb) vsp
***** Signal mux_1level_tapbuf_size2[22]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[22]->in[0] mux_1level_tapbuf_size2[22]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[22]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[22]->in[1] mux_1level_tapbuf_size2[22]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[22] gvdd_mux_1level_tapbuf_size2[22] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[536] trig v(mux_1level_tapbuf_size2[22]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[22]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[536] trig v(mux_1level_tapbuf_size2[22]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[22]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[536] when v(mux_1level_tapbuf_size2[22]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[536] trig v(mux_1level_tapbuf_size2[22]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[22]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[536] when v(mux_1level_tapbuf_size2[22]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[536] trig v(mux_1level_tapbuf_size2[22]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[22]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[22]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[22]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[536] param='mux_1level_tapbuf_size2[22]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[22]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[22]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[22]_energy_per_cycle param='mux_1level_tapbuf_size2[22]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[536]  param='mux_1level_tapbuf_size2[22]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[536]  param='dynamic_power_sb_mux[1][1]_rrnode[536]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[536] avg p(Vgvdd_mux_1level_tapbuf_size2[22]) from='start_rise_sb_mux[1][1]_rrnode[536]' to='start_rise_sb_mux[1][1]_rrnode[536]+switch_rise_sb_mux[1][1]_rrnode[536]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[536] avg p(Vgvdd_mux_1level_tapbuf_size2[22]) from='start_fall_sb_mux[1][1]_rrnode[536]' to='start_fall_sb_mux[1][1]_rrnode[536]+switch_fall_sb_mux[1][1]_rrnode[536]'
.meas tran sum_leakage_power_mux[0to22] 
+          param='sum_leakage_power_mux[0to21]+leakage_sb_mux[1][1]_rrnode[536]'
.meas tran sum_energy_per_cycle_mux[0to22] 
+          param='sum_energy_per_cycle_mux[0to21]+energy_per_cycle_sb_mux[1][1]_rrnode[536]'
***** Load for rr_node[536] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=45, type=5 *****
Xchan_mux_1level_tapbuf_size2[22]->out_loadlvl[0]_out mux_1level_tapbuf_size2[22]->out mux_1level_tapbuf_size2[22]->out_loadlvl[0]_out mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[57]_no0 mux_1level_tapbuf_size2[22]->out_loadlvl[0]_out mux_1level_tapbuf_size2[22]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[58]_no0 mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[59]_no0 mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[22]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to22] 
+          param='sum_leakage_power_sb_mux[0to21]+leakage_sb_mux[1][1]_rrnode[536]'
.meas tran sum_energy_per_cycle_sb_mux[0to22] 
+          param='sum_energy_per_cycle_sb_mux[0to21]+energy_per_cycle_sb_mux[1][1]_rrnode[536]'
Xmux_1level_tapbuf_size2[23] mux_1level_tapbuf_size2[23]->in[0] mux_1level_tapbuf_size2[23]->in[1] mux_1level_tapbuf_size2[23]->out sram[33]->outb sram[33]->out gvdd_mux_1level_tapbuf_size2[23] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[23], level=1, select_path_id=0. *****
*****1*****
Xsram[33] sram->in sram[33]->out sram[33]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[33]->out) 0
.nodeset V(sram[33]->outb) vsp
***** Signal mux_1level_tapbuf_size2[23]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[23]->in[0] mux_1level_tapbuf_size2[23]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[23]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[23]->in[1] mux_1level_tapbuf_size2[23]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[23] gvdd_mux_1level_tapbuf_size2[23] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[538] trig v(mux_1level_tapbuf_size2[23]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[23]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[538] trig v(mux_1level_tapbuf_size2[23]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[23]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[538] when v(mux_1level_tapbuf_size2[23]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[538] trig v(mux_1level_tapbuf_size2[23]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[23]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[538] when v(mux_1level_tapbuf_size2[23]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[538] trig v(mux_1level_tapbuf_size2[23]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[23]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[23]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[23]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[538] param='mux_1level_tapbuf_size2[23]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[23]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[23]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[23]_energy_per_cycle param='mux_1level_tapbuf_size2[23]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[538]  param='mux_1level_tapbuf_size2[23]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[538]  param='dynamic_power_sb_mux[1][1]_rrnode[538]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[538] avg p(Vgvdd_mux_1level_tapbuf_size2[23]) from='start_rise_sb_mux[1][1]_rrnode[538]' to='start_rise_sb_mux[1][1]_rrnode[538]+switch_rise_sb_mux[1][1]_rrnode[538]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[538] avg p(Vgvdd_mux_1level_tapbuf_size2[23]) from='start_fall_sb_mux[1][1]_rrnode[538]' to='start_fall_sb_mux[1][1]_rrnode[538]+switch_fall_sb_mux[1][1]_rrnode[538]'
.meas tran sum_leakage_power_mux[0to23] 
+          param='sum_leakage_power_mux[0to22]+leakage_sb_mux[1][1]_rrnode[538]'
.meas tran sum_energy_per_cycle_mux[0to23] 
+          param='sum_energy_per_cycle_mux[0to22]+energy_per_cycle_sb_mux[1][1]_rrnode[538]'
***** Load for rr_node[538] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=47, type=5 *****
Xchan_mux_1level_tapbuf_size2[23]->out_loadlvl[0]_out mux_1level_tapbuf_size2[23]->out mux_1level_tapbuf_size2[23]->out_loadlvl[0]_out mux_1level_tapbuf_size2[23]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[60]_no0 mux_1level_tapbuf_size2[23]->out_loadlvl[0]_out mux_1level_tapbuf_size2[23]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[61]_no0 mux_1level_tapbuf_size2[23]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[23]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[62]_no0 mux_1level_tapbuf_size2[23]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[23]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to23] 
+          param='sum_leakage_power_sb_mux[0to22]+leakage_sb_mux[1][1]_rrnode[538]'
.meas tran sum_energy_per_cycle_sb_mux[0to23] 
+          param='sum_energy_per_cycle_sb_mux[0to22]+energy_per_cycle_sb_mux[1][1]_rrnode[538]'
Xmux_1level_tapbuf_size2[24] mux_1level_tapbuf_size2[24]->in[0] mux_1level_tapbuf_size2[24]->in[1] mux_1level_tapbuf_size2[24]->out sram[34]->outb sram[34]->out gvdd_mux_1level_tapbuf_size2[24] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[24], level=1, select_path_id=0. *****
*****1*****
Xsram[34] sram->in sram[34]->out sram[34]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[34]->out) 0
.nodeset V(sram[34]->outb) vsp
***** Signal mux_1level_tapbuf_size2[24]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[24]->in[0] mux_1level_tapbuf_size2[24]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[24]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[24]->in[1] mux_1level_tapbuf_size2[24]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[24] gvdd_mux_1level_tapbuf_size2[24] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[540] trig v(mux_1level_tapbuf_size2[24]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[24]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[540] trig v(mux_1level_tapbuf_size2[24]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[24]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[540] when v(mux_1level_tapbuf_size2[24]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[540] trig v(mux_1level_tapbuf_size2[24]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[24]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[540] when v(mux_1level_tapbuf_size2[24]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[540] trig v(mux_1level_tapbuf_size2[24]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[24]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[24]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[24]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[540] param='mux_1level_tapbuf_size2[24]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[24]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[24]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[24]_energy_per_cycle param='mux_1level_tapbuf_size2[24]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[540]  param='mux_1level_tapbuf_size2[24]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[540]  param='dynamic_power_sb_mux[1][1]_rrnode[540]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[540] avg p(Vgvdd_mux_1level_tapbuf_size2[24]) from='start_rise_sb_mux[1][1]_rrnode[540]' to='start_rise_sb_mux[1][1]_rrnode[540]+switch_rise_sb_mux[1][1]_rrnode[540]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[540] avg p(Vgvdd_mux_1level_tapbuf_size2[24]) from='start_fall_sb_mux[1][1]_rrnode[540]' to='start_fall_sb_mux[1][1]_rrnode[540]+switch_fall_sb_mux[1][1]_rrnode[540]'
.meas tran sum_leakage_power_mux[0to24] 
+          param='sum_leakage_power_mux[0to23]+leakage_sb_mux[1][1]_rrnode[540]'
.meas tran sum_energy_per_cycle_mux[0to24] 
+          param='sum_energy_per_cycle_mux[0to23]+energy_per_cycle_sb_mux[1][1]_rrnode[540]'
***** Load for rr_node[540] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=49, type=5 *****
Xchan_mux_1level_tapbuf_size2[24]->out_loadlvl[0]_out mux_1level_tapbuf_size2[24]->out mux_1level_tapbuf_size2[24]->out_loadlvl[0]_out mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[63]_no0 mux_1level_tapbuf_size2[24]->out_loadlvl[0]_out mux_1level_tapbuf_size2[24]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[64]_no0 mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[65]_no0 mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[24]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to24] 
+          param='sum_leakage_power_sb_mux[0to23]+leakage_sb_mux[1][1]_rrnode[540]'
.meas tran sum_energy_per_cycle_sb_mux[0to24] 
+          param='sum_energy_per_cycle_sb_mux[0to23]+energy_per_cycle_sb_mux[1][1]_rrnode[540]'
Xmux_1level_tapbuf_size2[25] mux_1level_tapbuf_size2[25]->in[0] mux_1level_tapbuf_size2[25]->in[1] mux_1level_tapbuf_size2[25]->out sram[35]->outb sram[35]->out gvdd_mux_1level_tapbuf_size2[25] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[25], level=1, select_path_id=0. *****
*****1*****
Xsram[35] sram->in sram[35]->out sram[35]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[35]->out) 0
.nodeset V(sram[35]->outb) vsp
***** Signal mux_1level_tapbuf_size2[25]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[25]->in[0] mux_1level_tapbuf_size2[25]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[25]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[25]->in[1] mux_1level_tapbuf_size2[25]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[25] gvdd_mux_1level_tapbuf_size2[25] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[542] trig v(mux_1level_tapbuf_size2[25]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[25]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[542] trig v(mux_1level_tapbuf_size2[25]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[25]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[542] when v(mux_1level_tapbuf_size2[25]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[542] trig v(mux_1level_tapbuf_size2[25]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[25]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[542] when v(mux_1level_tapbuf_size2[25]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[542] trig v(mux_1level_tapbuf_size2[25]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[25]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[25]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[25]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[542] param='mux_1level_tapbuf_size2[25]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[25]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[25]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[25]_energy_per_cycle param='mux_1level_tapbuf_size2[25]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[542]  param='mux_1level_tapbuf_size2[25]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[542]  param='dynamic_power_sb_mux[1][1]_rrnode[542]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[542] avg p(Vgvdd_mux_1level_tapbuf_size2[25]) from='start_rise_sb_mux[1][1]_rrnode[542]' to='start_rise_sb_mux[1][1]_rrnode[542]+switch_rise_sb_mux[1][1]_rrnode[542]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[542] avg p(Vgvdd_mux_1level_tapbuf_size2[25]) from='start_fall_sb_mux[1][1]_rrnode[542]' to='start_fall_sb_mux[1][1]_rrnode[542]+switch_fall_sb_mux[1][1]_rrnode[542]'
.meas tran sum_leakage_power_mux[0to25] 
+          param='sum_leakage_power_mux[0to24]+leakage_sb_mux[1][1]_rrnode[542]'
.meas tran sum_energy_per_cycle_mux[0to25] 
+          param='sum_energy_per_cycle_mux[0to24]+energy_per_cycle_sb_mux[1][1]_rrnode[542]'
***** Load for rr_node[542] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=51, type=5 *****
Xchan_mux_1level_tapbuf_size2[25]->out_loadlvl[0]_out mux_1level_tapbuf_size2[25]->out mux_1level_tapbuf_size2[25]->out_loadlvl[0]_out mux_1level_tapbuf_size2[25]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[66]_no0 mux_1level_tapbuf_size2[25]->out_loadlvl[0]_out mux_1level_tapbuf_size2[25]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[67]_no0 mux_1level_tapbuf_size2[25]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[25]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to25] 
+          param='sum_leakage_power_sb_mux[0to24]+leakage_sb_mux[1][1]_rrnode[542]'
.meas tran sum_energy_per_cycle_sb_mux[0to25] 
+          param='sum_energy_per_cycle_sb_mux[0to24]+energy_per_cycle_sb_mux[1][1]_rrnode[542]'
Xmux_1level_tapbuf_size2[26] mux_1level_tapbuf_size2[26]->in[0] mux_1level_tapbuf_size2[26]->in[1] mux_1level_tapbuf_size2[26]->out sram[36]->outb sram[36]->out gvdd_mux_1level_tapbuf_size2[26] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[26], level=1, select_path_id=0. *****
*****1*****
Xsram[36] sram->in sram[36]->out sram[36]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[36]->out) 0
.nodeset V(sram[36]->outb) vsp
***** Signal mux_1level_tapbuf_size2[26]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[26]->in[0] mux_1level_tapbuf_size2[26]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[26]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[26]->in[1] mux_1level_tapbuf_size2[26]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[26] gvdd_mux_1level_tapbuf_size2[26] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[544] trig v(mux_1level_tapbuf_size2[26]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[26]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[544] trig v(mux_1level_tapbuf_size2[26]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[26]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[544] when v(mux_1level_tapbuf_size2[26]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[544] trig v(mux_1level_tapbuf_size2[26]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[26]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[544] when v(mux_1level_tapbuf_size2[26]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[544] trig v(mux_1level_tapbuf_size2[26]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[26]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[26]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[26]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[544] param='mux_1level_tapbuf_size2[26]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[26]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[26]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[26]_energy_per_cycle param='mux_1level_tapbuf_size2[26]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[544]  param='mux_1level_tapbuf_size2[26]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[544]  param='dynamic_power_sb_mux[1][1]_rrnode[544]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[544] avg p(Vgvdd_mux_1level_tapbuf_size2[26]) from='start_rise_sb_mux[1][1]_rrnode[544]' to='start_rise_sb_mux[1][1]_rrnode[544]+switch_rise_sb_mux[1][1]_rrnode[544]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[544] avg p(Vgvdd_mux_1level_tapbuf_size2[26]) from='start_fall_sb_mux[1][1]_rrnode[544]' to='start_fall_sb_mux[1][1]_rrnode[544]+switch_fall_sb_mux[1][1]_rrnode[544]'
.meas tran sum_leakage_power_mux[0to26] 
+          param='sum_leakage_power_mux[0to25]+leakage_sb_mux[1][1]_rrnode[544]'
.meas tran sum_energy_per_cycle_mux[0to26] 
+          param='sum_energy_per_cycle_mux[0to25]+energy_per_cycle_sb_mux[1][1]_rrnode[544]'
***** Load for rr_node[544] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=53, type=5 *****
Xchan_mux_1level_tapbuf_size2[26]->out_loadlvl[0]_out mux_1level_tapbuf_size2[26]->out mux_1level_tapbuf_size2[26]->out_loadlvl[0]_out mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[68]_no0 mux_1level_tapbuf_size2[26]->out_loadlvl[0]_out mux_1level_tapbuf_size2[26]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[69]_no0 mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[70]_no0 mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[26]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to26] 
+          param='sum_leakage_power_sb_mux[0to25]+leakage_sb_mux[1][1]_rrnode[544]'
.meas tran sum_energy_per_cycle_sb_mux[0to26] 
+          param='sum_energy_per_cycle_sb_mux[0to25]+energy_per_cycle_sb_mux[1][1]_rrnode[544]'
Xmux_1level_tapbuf_size2[27] mux_1level_tapbuf_size2[27]->in[0] mux_1level_tapbuf_size2[27]->in[1] mux_1level_tapbuf_size2[27]->out sram[37]->outb sram[37]->out gvdd_mux_1level_tapbuf_size2[27] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[27], level=1, select_path_id=0. *****
*****1*****
Xsram[37] sram->in sram[37]->out sram[37]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[37]->out) 0
.nodeset V(sram[37]->outb) vsp
***** Signal mux_1level_tapbuf_size2[27]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[27]->in[0] mux_1level_tapbuf_size2[27]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[27]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[27]->in[1] mux_1level_tapbuf_size2[27]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[27] gvdd_mux_1level_tapbuf_size2[27] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[546] trig v(mux_1level_tapbuf_size2[27]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[27]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[546] trig v(mux_1level_tapbuf_size2[27]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[27]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[546] when v(mux_1level_tapbuf_size2[27]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[546] trig v(mux_1level_tapbuf_size2[27]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[27]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[546] when v(mux_1level_tapbuf_size2[27]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[546] trig v(mux_1level_tapbuf_size2[27]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[27]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[27]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[27]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[546] param='mux_1level_tapbuf_size2[27]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[27]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[27]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[27]_energy_per_cycle param='mux_1level_tapbuf_size2[27]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[546]  param='mux_1level_tapbuf_size2[27]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[546]  param='dynamic_power_sb_mux[1][1]_rrnode[546]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[546] avg p(Vgvdd_mux_1level_tapbuf_size2[27]) from='start_rise_sb_mux[1][1]_rrnode[546]' to='start_rise_sb_mux[1][1]_rrnode[546]+switch_rise_sb_mux[1][1]_rrnode[546]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[546] avg p(Vgvdd_mux_1level_tapbuf_size2[27]) from='start_fall_sb_mux[1][1]_rrnode[546]' to='start_fall_sb_mux[1][1]_rrnode[546]+switch_fall_sb_mux[1][1]_rrnode[546]'
.meas tran sum_leakage_power_mux[0to27] 
+          param='sum_leakage_power_mux[0to26]+leakage_sb_mux[1][1]_rrnode[546]'
.meas tran sum_energy_per_cycle_mux[0to27] 
+          param='sum_energy_per_cycle_mux[0to26]+energy_per_cycle_sb_mux[1][1]_rrnode[546]'
***** Load for rr_node[546] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=55, type=5 *****
Xchan_mux_1level_tapbuf_size2[27]->out_loadlvl[0]_out mux_1level_tapbuf_size2[27]->out mux_1level_tapbuf_size2[27]->out_loadlvl[0]_out mux_1level_tapbuf_size2[27]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[71]_no0 mux_1level_tapbuf_size2[27]->out_loadlvl[0]_out mux_1level_tapbuf_size2[27]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[72]_no0 mux_1level_tapbuf_size2[27]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[27]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to27] 
+          param='sum_leakage_power_sb_mux[0to26]+leakage_sb_mux[1][1]_rrnode[546]'
.meas tran sum_energy_per_cycle_sb_mux[0to27] 
+          param='sum_energy_per_cycle_sb_mux[0to26]+energy_per_cycle_sb_mux[1][1]_rrnode[546]'
Xmux_1level_tapbuf_size2[28] mux_1level_tapbuf_size2[28]->in[0] mux_1level_tapbuf_size2[28]->in[1] mux_1level_tapbuf_size2[28]->out sram[38]->outb sram[38]->out gvdd_mux_1level_tapbuf_size2[28] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[28], level=1, select_path_id=0. *****
*****1*****
Xsram[38] sram->in sram[38]->out sram[38]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[38]->out) 0
.nodeset V(sram[38]->outb) vsp
***** Signal mux_1level_tapbuf_size2[28]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[28]->in[0] mux_1level_tapbuf_size2[28]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[28]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[28]->in[1] mux_1level_tapbuf_size2[28]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[28] gvdd_mux_1level_tapbuf_size2[28] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[548] trig v(mux_1level_tapbuf_size2[28]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[28]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[548] trig v(mux_1level_tapbuf_size2[28]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[28]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[548] when v(mux_1level_tapbuf_size2[28]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[548] trig v(mux_1level_tapbuf_size2[28]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[28]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[548] when v(mux_1level_tapbuf_size2[28]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[548] trig v(mux_1level_tapbuf_size2[28]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[28]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[28]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[28]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[548] param='mux_1level_tapbuf_size2[28]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[28]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[28]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[28]_energy_per_cycle param='mux_1level_tapbuf_size2[28]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[548]  param='mux_1level_tapbuf_size2[28]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[548]  param='dynamic_power_sb_mux[1][1]_rrnode[548]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[548] avg p(Vgvdd_mux_1level_tapbuf_size2[28]) from='start_rise_sb_mux[1][1]_rrnode[548]' to='start_rise_sb_mux[1][1]_rrnode[548]+switch_rise_sb_mux[1][1]_rrnode[548]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[548] avg p(Vgvdd_mux_1level_tapbuf_size2[28]) from='start_fall_sb_mux[1][1]_rrnode[548]' to='start_fall_sb_mux[1][1]_rrnode[548]+switch_fall_sb_mux[1][1]_rrnode[548]'
.meas tran sum_leakage_power_mux[0to28] 
+          param='sum_leakage_power_mux[0to27]+leakage_sb_mux[1][1]_rrnode[548]'
.meas tran sum_energy_per_cycle_mux[0to28] 
+          param='sum_energy_per_cycle_mux[0to27]+energy_per_cycle_sb_mux[1][1]_rrnode[548]'
***** Load for rr_node[548] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=57, type=5 *****
Xchan_mux_1level_tapbuf_size2[28]->out_loadlvl[0]_out mux_1level_tapbuf_size2[28]->out mux_1level_tapbuf_size2[28]->out_loadlvl[0]_out mux_1level_tapbuf_size2[28]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[73]_no0 mux_1level_tapbuf_size2[28]->out_loadlvl[0]_out mux_1level_tapbuf_size2[28]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[74]_no0 mux_1level_tapbuf_size2[28]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[28]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to28] 
+          param='sum_leakage_power_sb_mux[0to27]+leakage_sb_mux[1][1]_rrnode[548]'
.meas tran sum_energy_per_cycle_sb_mux[0to28] 
+          param='sum_energy_per_cycle_sb_mux[0to27]+energy_per_cycle_sb_mux[1][1]_rrnode[548]'
Xmux_1level_tapbuf_size2[29] mux_1level_tapbuf_size2[29]->in[0] mux_1level_tapbuf_size2[29]->in[1] mux_1level_tapbuf_size2[29]->out sram[39]->outb sram[39]->out gvdd_mux_1level_tapbuf_size2[29] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[29], level=1, select_path_id=0. *****
*****1*****
Xsram[39] sram->in sram[39]->out sram[39]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[39]->out) 0
.nodeset V(sram[39]->outb) vsp
***** Signal mux_1level_tapbuf_size2[29]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[29]->in[0] mux_1level_tapbuf_size2[29]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[29]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[29]->in[1] mux_1level_tapbuf_size2[29]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[29] gvdd_mux_1level_tapbuf_size2[29] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[550] trig v(mux_1level_tapbuf_size2[29]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[29]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[550] trig v(mux_1level_tapbuf_size2[29]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[29]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[550] when v(mux_1level_tapbuf_size2[29]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[550] trig v(mux_1level_tapbuf_size2[29]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[29]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[550] when v(mux_1level_tapbuf_size2[29]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[550] trig v(mux_1level_tapbuf_size2[29]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[29]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[29]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[29]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[550] param='mux_1level_tapbuf_size2[29]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[29]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[29]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[29]_energy_per_cycle param='mux_1level_tapbuf_size2[29]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[550]  param='mux_1level_tapbuf_size2[29]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[550]  param='dynamic_power_sb_mux[1][1]_rrnode[550]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[550] avg p(Vgvdd_mux_1level_tapbuf_size2[29]) from='start_rise_sb_mux[1][1]_rrnode[550]' to='start_rise_sb_mux[1][1]_rrnode[550]+switch_rise_sb_mux[1][1]_rrnode[550]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[550] avg p(Vgvdd_mux_1level_tapbuf_size2[29]) from='start_fall_sb_mux[1][1]_rrnode[550]' to='start_fall_sb_mux[1][1]_rrnode[550]+switch_fall_sb_mux[1][1]_rrnode[550]'
.meas tran sum_leakage_power_mux[0to29] 
+          param='sum_leakage_power_mux[0to28]+leakage_sb_mux[1][1]_rrnode[550]'
.meas tran sum_energy_per_cycle_mux[0to29] 
+          param='sum_energy_per_cycle_mux[0to28]+energy_per_cycle_sb_mux[1][1]_rrnode[550]'
***** Load for rr_node[550] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=59, type=5 *****
Xchan_mux_1level_tapbuf_size2[29]->out_loadlvl[0]_out mux_1level_tapbuf_size2[29]->out mux_1level_tapbuf_size2[29]->out_loadlvl[0]_out mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[75]_no0 mux_1level_tapbuf_size2[29]->out_loadlvl[0]_out mux_1level_tapbuf_size2[29]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[76]_no0 mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[77]_no0 mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[29]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to29] 
+          param='sum_leakage_power_sb_mux[0to28]+leakage_sb_mux[1][1]_rrnode[550]'
.meas tran sum_energy_per_cycle_sb_mux[0to29] 
+          param='sum_energy_per_cycle_sb_mux[0to28]+energy_per_cycle_sb_mux[1][1]_rrnode[550]'
Xmux_1level_tapbuf_size2[30] mux_1level_tapbuf_size2[30]->in[0] mux_1level_tapbuf_size2[30]->in[1] mux_1level_tapbuf_size2[30]->out sram[40]->outb sram[40]->out gvdd_mux_1level_tapbuf_size2[30] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[30], level=1, select_path_id=0. *****
*****1*****
Xsram[40] sram->in sram[40]->out sram[40]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[40]->out) 0
.nodeset V(sram[40]->outb) vsp
***** Signal mux_1level_tapbuf_size2[30]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[30]->in[0] mux_1level_tapbuf_size2[30]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[30]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[30]->in[1] mux_1level_tapbuf_size2[30]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[30] gvdd_mux_1level_tapbuf_size2[30] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[552] trig v(mux_1level_tapbuf_size2[30]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[30]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[552] trig v(mux_1level_tapbuf_size2[30]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[30]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[552] when v(mux_1level_tapbuf_size2[30]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[552] trig v(mux_1level_tapbuf_size2[30]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[30]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[552] when v(mux_1level_tapbuf_size2[30]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[552] trig v(mux_1level_tapbuf_size2[30]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[30]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[30]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[30]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[552] param='mux_1level_tapbuf_size2[30]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[30]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[30]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[30]_energy_per_cycle param='mux_1level_tapbuf_size2[30]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[552]  param='mux_1level_tapbuf_size2[30]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[552]  param='dynamic_power_sb_mux[1][1]_rrnode[552]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[552] avg p(Vgvdd_mux_1level_tapbuf_size2[30]) from='start_rise_sb_mux[1][1]_rrnode[552]' to='start_rise_sb_mux[1][1]_rrnode[552]+switch_rise_sb_mux[1][1]_rrnode[552]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[552] avg p(Vgvdd_mux_1level_tapbuf_size2[30]) from='start_fall_sb_mux[1][1]_rrnode[552]' to='start_fall_sb_mux[1][1]_rrnode[552]+switch_fall_sb_mux[1][1]_rrnode[552]'
.meas tran sum_leakage_power_mux[0to30] 
+          param='sum_leakage_power_mux[0to29]+leakage_sb_mux[1][1]_rrnode[552]'
.meas tran sum_energy_per_cycle_mux[0to30] 
+          param='sum_energy_per_cycle_mux[0to29]+energy_per_cycle_sb_mux[1][1]_rrnode[552]'
***** Load for rr_node[552] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=61, type=5 *****
Xchan_mux_1level_tapbuf_size2[30]->out_loadlvl[0]_out mux_1level_tapbuf_size2[30]->out mux_1level_tapbuf_size2[30]->out_loadlvl[0]_out mux_1level_tapbuf_size2[30]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[78]_no0 mux_1level_tapbuf_size2[30]->out_loadlvl[0]_out mux_1level_tapbuf_size2[30]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[79]_no0 mux_1level_tapbuf_size2[30]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[30]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to30] 
+          param='sum_leakage_power_sb_mux[0to29]+leakage_sb_mux[1][1]_rrnode[552]'
.meas tran sum_energy_per_cycle_sb_mux[0to30] 
+          param='sum_energy_per_cycle_sb_mux[0to29]+energy_per_cycle_sb_mux[1][1]_rrnode[552]'
Xmux_1level_tapbuf_size2[31] mux_1level_tapbuf_size2[31]->in[0] mux_1level_tapbuf_size2[31]->in[1] mux_1level_tapbuf_size2[31]->out sram[41]->outb sram[41]->out gvdd_mux_1level_tapbuf_size2[31] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[31], level=1, select_path_id=0. *****
*****1*****
Xsram[41] sram->in sram[41]->out sram[41]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[41]->out) 0
.nodeset V(sram[41]->outb) vsp
***** Signal mux_1level_tapbuf_size2[31]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[31]->in[0] mux_1level_tapbuf_size2[31]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[31]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[31]->in[1] mux_1level_tapbuf_size2[31]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[31] gvdd_mux_1level_tapbuf_size2[31] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[554] trig v(mux_1level_tapbuf_size2[31]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[31]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[554] trig v(mux_1level_tapbuf_size2[31]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[31]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[554] when v(mux_1level_tapbuf_size2[31]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[554] trig v(mux_1level_tapbuf_size2[31]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[31]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[554] when v(mux_1level_tapbuf_size2[31]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[554] trig v(mux_1level_tapbuf_size2[31]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[31]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[31]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[31]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[554] param='mux_1level_tapbuf_size2[31]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[31]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[31]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[31]_energy_per_cycle param='mux_1level_tapbuf_size2[31]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[554]  param='mux_1level_tapbuf_size2[31]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[554]  param='dynamic_power_sb_mux[1][1]_rrnode[554]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[554] avg p(Vgvdd_mux_1level_tapbuf_size2[31]) from='start_rise_sb_mux[1][1]_rrnode[554]' to='start_rise_sb_mux[1][1]_rrnode[554]+switch_rise_sb_mux[1][1]_rrnode[554]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[554] avg p(Vgvdd_mux_1level_tapbuf_size2[31]) from='start_fall_sb_mux[1][1]_rrnode[554]' to='start_fall_sb_mux[1][1]_rrnode[554]+switch_fall_sb_mux[1][1]_rrnode[554]'
.meas tran sum_leakage_power_mux[0to31] 
+          param='sum_leakage_power_mux[0to30]+leakage_sb_mux[1][1]_rrnode[554]'
.meas tran sum_energy_per_cycle_mux[0to31] 
+          param='sum_energy_per_cycle_mux[0to30]+energy_per_cycle_sb_mux[1][1]_rrnode[554]'
***** Load for rr_node[554] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=63, type=5 *****
Xchan_mux_1level_tapbuf_size2[31]->out_loadlvl[0]_out mux_1level_tapbuf_size2[31]->out mux_1level_tapbuf_size2[31]->out_loadlvl[0]_out mux_1level_tapbuf_size2[31]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[80]_no0 mux_1level_tapbuf_size2[31]->out_loadlvl[0]_out mux_1level_tapbuf_size2[31]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[81]_no0 mux_1level_tapbuf_size2[31]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[31]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[82]_no0 mux_1level_tapbuf_size2[31]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[31]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to31] 
+          param='sum_leakage_power_sb_mux[0to30]+leakage_sb_mux[1][1]_rrnode[554]'
.meas tran sum_energy_per_cycle_sb_mux[0to31] 
+          param='sum_energy_per_cycle_sb_mux[0to30]+energy_per_cycle_sb_mux[1][1]_rrnode[554]'
Xmux_1level_tapbuf_size2[32] mux_1level_tapbuf_size2[32]->in[0] mux_1level_tapbuf_size2[32]->in[1] mux_1level_tapbuf_size2[32]->out sram[42]->outb sram[42]->out gvdd_mux_1level_tapbuf_size2[32] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[32], level=1, select_path_id=0. *****
*****1*****
Xsram[42] sram->in sram[42]->out sram[42]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[42]->out) 0
.nodeset V(sram[42]->outb) vsp
***** Signal mux_1level_tapbuf_size2[32]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[32]->in[0] mux_1level_tapbuf_size2[32]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[32]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[32]->in[1] mux_1level_tapbuf_size2[32]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[32] gvdd_mux_1level_tapbuf_size2[32] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[556] trig v(mux_1level_tapbuf_size2[32]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[32]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[556] trig v(mux_1level_tapbuf_size2[32]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[32]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[556] when v(mux_1level_tapbuf_size2[32]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[556] trig v(mux_1level_tapbuf_size2[32]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[32]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[556] when v(mux_1level_tapbuf_size2[32]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[556] trig v(mux_1level_tapbuf_size2[32]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[32]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[32]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[32]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[556] param='mux_1level_tapbuf_size2[32]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[32]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[32]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[32]_energy_per_cycle param='mux_1level_tapbuf_size2[32]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[556]  param='mux_1level_tapbuf_size2[32]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[556]  param='dynamic_power_sb_mux[1][1]_rrnode[556]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[556] avg p(Vgvdd_mux_1level_tapbuf_size2[32]) from='start_rise_sb_mux[1][1]_rrnode[556]' to='start_rise_sb_mux[1][1]_rrnode[556]+switch_rise_sb_mux[1][1]_rrnode[556]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[556] avg p(Vgvdd_mux_1level_tapbuf_size2[32]) from='start_fall_sb_mux[1][1]_rrnode[556]' to='start_fall_sb_mux[1][1]_rrnode[556]+switch_fall_sb_mux[1][1]_rrnode[556]'
.meas tran sum_leakage_power_mux[0to32] 
+          param='sum_leakage_power_mux[0to31]+leakage_sb_mux[1][1]_rrnode[556]'
.meas tran sum_energy_per_cycle_mux[0to32] 
+          param='sum_energy_per_cycle_mux[0to31]+energy_per_cycle_sb_mux[1][1]_rrnode[556]'
***** Load for rr_node[556] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=65, type=5 *****
Xchan_mux_1level_tapbuf_size2[32]->out_loadlvl[0]_out mux_1level_tapbuf_size2[32]->out mux_1level_tapbuf_size2[32]->out_loadlvl[0]_out mux_1level_tapbuf_size2[32]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[83]_no0 mux_1level_tapbuf_size2[32]->out_loadlvl[0]_out mux_1level_tapbuf_size2[32]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[84]_no0 mux_1level_tapbuf_size2[32]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[32]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[85]_no0 mux_1level_tapbuf_size2[32]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[32]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to32] 
+          param='sum_leakage_power_sb_mux[0to31]+leakage_sb_mux[1][1]_rrnode[556]'
.meas tran sum_energy_per_cycle_sb_mux[0to32] 
+          param='sum_energy_per_cycle_sb_mux[0to31]+energy_per_cycle_sb_mux[1][1]_rrnode[556]'
Xmux_1level_tapbuf_size2[33] mux_1level_tapbuf_size2[33]->in[0] mux_1level_tapbuf_size2[33]->in[1] mux_1level_tapbuf_size2[33]->out sram[43]->outb sram[43]->out gvdd_mux_1level_tapbuf_size2[33] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[33], level=1, select_path_id=0. *****
*****1*****
Xsram[43] sram->in sram[43]->out sram[43]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[43]->out) 0
.nodeset V(sram[43]->outb) vsp
***** Signal mux_1level_tapbuf_size2[33]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[33]->in[0] mux_1level_tapbuf_size2[33]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[33]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[33]->in[1] mux_1level_tapbuf_size2[33]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[33] gvdd_mux_1level_tapbuf_size2[33] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[558] trig v(mux_1level_tapbuf_size2[33]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[33]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[558] trig v(mux_1level_tapbuf_size2[33]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[33]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[558] when v(mux_1level_tapbuf_size2[33]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[558] trig v(mux_1level_tapbuf_size2[33]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[33]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[558] when v(mux_1level_tapbuf_size2[33]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[558] trig v(mux_1level_tapbuf_size2[33]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[33]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[33]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[33]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[558] param='mux_1level_tapbuf_size2[33]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[33]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[33]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[33]_energy_per_cycle param='mux_1level_tapbuf_size2[33]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[558]  param='mux_1level_tapbuf_size2[33]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[558]  param='dynamic_power_sb_mux[1][1]_rrnode[558]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[558] avg p(Vgvdd_mux_1level_tapbuf_size2[33]) from='start_rise_sb_mux[1][1]_rrnode[558]' to='start_rise_sb_mux[1][1]_rrnode[558]+switch_rise_sb_mux[1][1]_rrnode[558]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[558] avg p(Vgvdd_mux_1level_tapbuf_size2[33]) from='start_fall_sb_mux[1][1]_rrnode[558]' to='start_fall_sb_mux[1][1]_rrnode[558]+switch_fall_sb_mux[1][1]_rrnode[558]'
.meas tran sum_leakage_power_mux[0to33] 
+          param='sum_leakage_power_mux[0to32]+leakage_sb_mux[1][1]_rrnode[558]'
.meas tran sum_energy_per_cycle_mux[0to33] 
+          param='sum_energy_per_cycle_mux[0to32]+energy_per_cycle_sb_mux[1][1]_rrnode[558]'
***** Load for rr_node[558] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=67, type=5 *****
Xchan_mux_1level_tapbuf_size2[33]->out_loadlvl[0]_out mux_1level_tapbuf_size2[33]->out mux_1level_tapbuf_size2[33]->out_loadlvl[0]_out mux_1level_tapbuf_size2[33]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[86]_no0 mux_1level_tapbuf_size2[33]->out_loadlvl[0]_out mux_1level_tapbuf_size2[33]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[87]_no0 mux_1level_tapbuf_size2[33]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[33]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[88]_no0 mux_1level_tapbuf_size2[33]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[33]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to33] 
+          param='sum_leakage_power_sb_mux[0to32]+leakage_sb_mux[1][1]_rrnode[558]'
.meas tran sum_energy_per_cycle_sb_mux[0to33] 
+          param='sum_energy_per_cycle_sb_mux[0to32]+energy_per_cycle_sb_mux[1][1]_rrnode[558]'
Xmux_1level_tapbuf_size2[34] mux_1level_tapbuf_size2[34]->in[0] mux_1level_tapbuf_size2[34]->in[1] mux_1level_tapbuf_size2[34]->out sram[44]->outb sram[44]->out gvdd_mux_1level_tapbuf_size2[34] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[34], level=1, select_path_id=0. *****
*****1*****
Xsram[44] sram->in sram[44]->out sram[44]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[44]->out) 0
.nodeset V(sram[44]->outb) vsp
***** Signal mux_1level_tapbuf_size2[34]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[34]->in[0] mux_1level_tapbuf_size2[34]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[34]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[34]->in[1] mux_1level_tapbuf_size2[34]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[34] gvdd_mux_1level_tapbuf_size2[34] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[560] trig v(mux_1level_tapbuf_size2[34]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[34]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[560] trig v(mux_1level_tapbuf_size2[34]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[34]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[560] when v(mux_1level_tapbuf_size2[34]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[560] trig v(mux_1level_tapbuf_size2[34]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[34]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[560] when v(mux_1level_tapbuf_size2[34]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[560] trig v(mux_1level_tapbuf_size2[34]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[34]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[34]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[34]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[560] param='mux_1level_tapbuf_size2[34]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[34]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[34]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[34]_energy_per_cycle param='mux_1level_tapbuf_size2[34]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[560]  param='mux_1level_tapbuf_size2[34]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[560]  param='dynamic_power_sb_mux[1][1]_rrnode[560]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[560] avg p(Vgvdd_mux_1level_tapbuf_size2[34]) from='start_rise_sb_mux[1][1]_rrnode[560]' to='start_rise_sb_mux[1][1]_rrnode[560]+switch_rise_sb_mux[1][1]_rrnode[560]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[560] avg p(Vgvdd_mux_1level_tapbuf_size2[34]) from='start_fall_sb_mux[1][1]_rrnode[560]' to='start_fall_sb_mux[1][1]_rrnode[560]+switch_fall_sb_mux[1][1]_rrnode[560]'
.meas tran sum_leakage_power_mux[0to34] 
+          param='sum_leakage_power_mux[0to33]+leakage_sb_mux[1][1]_rrnode[560]'
.meas tran sum_energy_per_cycle_mux[0to34] 
+          param='sum_energy_per_cycle_mux[0to33]+energy_per_cycle_sb_mux[1][1]_rrnode[560]'
***** Load for rr_node[560] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=69, type=5 *****
Xchan_mux_1level_tapbuf_size2[34]->out_loadlvl[0]_out mux_1level_tapbuf_size2[34]->out mux_1level_tapbuf_size2[34]->out_loadlvl[0]_out mux_1level_tapbuf_size2[34]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[89]_no0 mux_1level_tapbuf_size2[34]->out_loadlvl[0]_out mux_1level_tapbuf_size2[34]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[90]_no0 mux_1level_tapbuf_size2[34]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[34]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to34] 
+          param='sum_leakage_power_sb_mux[0to33]+leakage_sb_mux[1][1]_rrnode[560]'
.meas tran sum_energy_per_cycle_sb_mux[0to34] 
+          param='sum_energy_per_cycle_sb_mux[0to33]+energy_per_cycle_sb_mux[1][1]_rrnode[560]'
Xmux_1level_tapbuf_size2[35] mux_1level_tapbuf_size2[35]->in[0] mux_1level_tapbuf_size2[35]->in[1] mux_1level_tapbuf_size2[35]->out sram[45]->outb sram[45]->out gvdd_mux_1level_tapbuf_size2[35] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[35], level=1, select_path_id=0. *****
*****1*****
Xsram[45] sram->in sram[45]->out sram[45]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[45]->out) 0
.nodeset V(sram[45]->outb) vsp
***** Signal mux_1level_tapbuf_size2[35]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[35]->in[0] mux_1level_tapbuf_size2[35]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[35]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[35]->in[1] mux_1level_tapbuf_size2[35]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[35] gvdd_mux_1level_tapbuf_size2[35] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[562] trig v(mux_1level_tapbuf_size2[35]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[35]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[562] trig v(mux_1level_tapbuf_size2[35]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[35]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[562] when v(mux_1level_tapbuf_size2[35]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[562] trig v(mux_1level_tapbuf_size2[35]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[35]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[562] when v(mux_1level_tapbuf_size2[35]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[562] trig v(mux_1level_tapbuf_size2[35]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[35]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[35]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[35]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[562] param='mux_1level_tapbuf_size2[35]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[35]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[35]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[35]_energy_per_cycle param='mux_1level_tapbuf_size2[35]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[562]  param='mux_1level_tapbuf_size2[35]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[562]  param='dynamic_power_sb_mux[1][1]_rrnode[562]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[562] avg p(Vgvdd_mux_1level_tapbuf_size2[35]) from='start_rise_sb_mux[1][1]_rrnode[562]' to='start_rise_sb_mux[1][1]_rrnode[562]+switch_rise_sb_mux[1][1]_rrnode[562]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[562] avg p(Vgvdd_mux_1level_tapbuf_size2[35]) from='start_fall_sb_mux[1][1]_rrnode[562]' to='start_fall_sb_mux[1][1]_rrnode[562]+switch_fall_sb_mux[1][1]_rrnode[562]'
.meas tran sum_leakage_power_mux[0to35] 
+          param='sum_leakage_power_mux[0to34]+leakage_sb_mux[1][1]_rrnode[562]'
.meas tran sum_energy_per_cycle_mux[0to35] 
+          param='sum_energy_per_cycle_mux[0to34]+energy_per_cycle_sb_mux[1][1]_rrnode[562]'
***** Load for rr_node[562] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=71, type=5 *****
Xchan_mux_1level_tapbuf_size2[35]->out_loadlvl[0]_out mux_1level_tapbuf_size2[35]->out mux_1level_tapbuf_size2[35]->out_loadlvl[0]_out mux_1level_tapbuf_size2[35]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[91]_no0 mux_1level_tapbuf_size2[35]->out_loadlvl[0]_out mux_1level_tapbuf_size2[35]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[92]_no0 mux_1level_tapbuf_size2[35]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[35]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[93]_no0 mux_1level_tapbuf_size2[35]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[35]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to35] 
+          param='sum_leakage_power_sb_mux[0to34]+leakage_sb_mux[1][1]_rrnode[562]'
.meas tran sum_energy_per_cycle_sb_mux[0to35] 
+          param='sum_energy_per_cycle_sb_mux[0to34]+energy_per_cycle_sb_mux[1][1]_rrnode[562]'
Xmux_1level_tapbuf_size2[36] mux_1level_tapbuf_size2[36]->in[0] mux_1level_tapbuf_size2[36]->in[1] mux_1level_tapbuf_size2[36]->out sram[46]->outb sram[46]->out gvdd_mux_1level_tapbuf_size2[36] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[36], level=1, select_path_id=0. *****
*****1*****
Xsram[46] sram->in sram[46]->out sram[46]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[46]->out) 0
.nodeset V(sram[46]->outb) vsp
***** Signal mux_1level_tapbuf_size2[36]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[36]->in[0] mux_1level_tapbuf_size2[36]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[36]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[36]->in[1] mux_1level_tapbuf_size2[36]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[36] gvdd_mux_1level_tapbuf_size2[36] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[564] trig v(mux_1level_tapbuf_size2[36]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[36]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[564] trig v(mux_1level_tapbuf_size2[36]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[36]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[564] when v(mux_1level_tapbuf_size2[36]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[564] trig v(mux_1level_tapbuf_size2[36]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[36]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[564] when v(mux_1level_tapbuf_size2[36]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[564] trig v(mux_1level_tapbuf_size2[36]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[36]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[36]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[36]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[564] param='mux_1level_tapbuf_size2[36]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[36]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[36]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[36]_energy_per_cycle param='mux_1level_tapbuf_size2[36]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[564]  param='mux_1level_tapbuf_size2[36]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[564]  param='dynamic_power_sb_mux[1][1]_rrnode[564]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[564] avg p(Vgvdd_mux_1level_tapbuf_size2[36]) from='start_rise_sb_mux[1][1]_rrnode[564]' to='start_rise_sb_mux[1][1]_rrnode[564]+switch_rise_sb_mux[1][1]_rrnode[564]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[564] avg p(Vgvdd_mux_1level_tapbuf_size2[36]) from='start_fall_sb_mux[1][1]_rrnode[564]' to='start_fall_sb_mux[1][1]_rrnode[564]+switch_fall_sb_mux[1][1]_rrnode[564]'
.meas tran sum_leakage_power_mux[0to36] 
+          param='sum_leakage_power_mux[0to35]+leakage_sb_mux[1][1]_rrnode[564]'
.meas tran sum_energy_per_cycle_mux[0to36] 
+          param='sum_energy_per_cycle_mux[0to35]+energy_per_cycle_sb_mux[1][1]_rrnode[564]'
***** Load for rr_node[564] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=73, type=5 *****
Xchan_mux_1level_tapbuf_size2[36]->out_loadlvl[0]_out mux_1level_tapbuf_size2[36]->out mux_1level_tapbuf_size2[36]->out_loadlvl[0]_out mux_1level_tapbuf_size2[36]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[94]_no0 mux_1level_tapbuf_size2[36]->out_loadlvl[0]_out mux_1level_tapbuf_size2[36]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[95]_no0 mux_1level_tapbuf_size2[36]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[36]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to36] 
+          param='sum_leakage_power_sb_mux[0to35]+leakage_sb_mux[1][1]_rrnode[564]'
.meas tran sum_energy_per_cycle_sb_mux[0to36] 
+          param='sum_energy_per_cycle_sb_mux[0to35]+energy_per_cycle_sb_mux[1][1]_rrnode[564]'
Xmux_1level_tapbuf_size2[37] mux_1level_tapbuf_size2[37]->in[0] mux_1level_tapbuf_size2[37]->in[1] mux_1level_tapbuf_size2[37]->out sram[47]->outb sram[47]->out gvdd_mux_1level_tapbuf_size2[37] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[37], level=1, select_path_id=0. *****
*****1*****
Xsram[47] sram->in sram[47]->out sram[47]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[47]->out) 0
.nodeset V(sram[47]->outb) vsp
***** Signal mux_1level_tapbuf_size2[37]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[37]->in[0] mux_1level_tapbuf_size2[37]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[37]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[37]->in[1] mux_1level_tapbuf_size2[37]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[37] gvdd_mux_1level_tapbuf_size2[37] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[566] trig v(mux_1level_tapbuf_size2[37]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[37]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[566] trig v(mux_1level_tapbuf_size2[37]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[37]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[566] when v(mux_1level_tapbuf_size2[37]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[566] trig v(mux_1level_tapbuf_size2[37]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[37]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[566] when v(mux_1level_tapbuf_size2[37]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[566] trig v(mux_1level_tapbuf_size2[37]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[37]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[37]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[37]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[566] param='mux_1level_tapbuf_size2[37]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[37]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[37]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[37]_energy_per_cycle param='mux_1level_tapbuf_size2[37]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[566]  param='mux_1level_tapbuf_size2[37]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[566]  param='dynamic_power_sb_mux[1][1]_rrnode[566]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[566] avg p(Vgvdd_mux_1level_tapbuf_size2[37]) from='start_rise_sb_mux[1][1]_rrnode[566]' to='start_rise_sb_mux[1][1]_rrnode[566]+switch_rise_sb_mux[1][1]_rrnode[566]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[566] avg p(Vgvdd_mux_1level_tapbuf_size2[37]) from='start_fall_sb_mux[1][1]_rrnode[566]' to='start_fall_sb_mux[1][1]_rrnode[566]+switch_fall_sb_mux[1][1]_rrnode[566]'
.meas tran sum_leakage_power_mux[0to37] 
+          param='sum_leakage_power_mux[0to36]+leakage_sb_mux[1][1]_rrnode[566]'
.meas tran sum_energy_per_cycle_mux[0to37] 
+          param='sum_energy_per_cycle_mux[0to36]+energy_per_cycle_sb_mux[1][1]_rrnode[566]'
***** Load for rr_node[566] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=75, type=5 *****
Xchan_mux_1level_tapbuf_size2[37]->out_loadlvl[0]_out mux_1level_tapbuf_size2[37]->out mux_1level_tapbuf_size2[37]->out_loadlvl[0]_out mux_1level_tapbuf_size2[37]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[96]_no0 mux_1level_tapbuf_size2[37]->out_loadlvl[0]_out mux_1level_tapbuf_size2[37]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[97]_no0 mux_1level_tapbuf_size2[37]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[37]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[98]_no0 mux_1level_tapbuf_size2[37]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[37]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to37] 
+          param='sum_leakage_power_sb_mux[0to36]+leakage_sb_mux[1][1]_rrnode[566]'
.meas tran sum_energy_per_cycle_sb_mux[0to37] 
+          param='sum_energy_per_cycle_sb_mux[0to36]+energy_per_cycle_sb_mux[1][1]_rrnode[566]'
Xmux_1level_tapbuf_size2[38] mux_1level_tapbuf_size2[38]->in[0] mux_1level_tapbuf_size2[38]->in[1] mux_1level_tapbuf_size2[38]->out sram[48]->outb sram[48]->out gvdd_mux_1level_tapbuf_size2[38] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[38], level=1, select_path_id=0. *****
*****1*****
Xsram[48] sram->in sram[48]->out sram[48]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[48]->out) 0
.nodeset V(sram[48]->outb) vsp
***** Signal mux_1level_tapbuf_size2[38]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[38]->in[0] mux_1level_tapbuf_size2[38]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[38]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[38]->in[1] mux_1level_tapbuf_size2[38]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[38] gvdd_mux_1level_tapbuf_size2[38] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[568] trig v(mux_1level_tapbuf_size2[38]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[38]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[568] trig v(mux_1level_tapbuf_size2[38]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[38]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[568] when v(mux_1level_tapbuf_size2[38]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[568] trig v(mux_1level_tapbuf_size2[38]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[38]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[568] when v(mux_1level_tapbuf_size2[38]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[568] trig v(mux_1level_tapbuf_size2[38]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[38]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[38]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[38]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[568] param='mux_1level_tapbuf_size2[38]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[38]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[38]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[38]_energy_per_cycle param='mux_1level_tapbuf_size2[38]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[568]  param='mux_1level_tapbuf_size2[38]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[568]  param='dynamic_power_sb_mux[1][1]_rrnode[568]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[568] avg p(Vgvdd_mux_1level_tapbuf_size2[38]) from='start_rise_sb_mux[1][1]_rrnode[568]' to='start_rise_sb_mux[1][1]_rrnode[568]+switch_rise_sb_mux[1][1]_rrnode[568]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[568] avg p(Vgvdd_mux_1level_tapbuf_size2[38]) from='start_fall_sb_mux[1][1]_rrnode[568]' to='start_fall_sb_mux[1][1]_rrnode[568]+switch_fall_sb_mux[1][1]_rrnode[568]'
.meas tran sum_leakage_power_mux[0to38] 
+          param='sum_leakage_power_mux[0to37]+leakage_sb_mux[1][1]_rrnode[568]'
.meas tran sum_energy_per_cycle_mux[0to38] 
+          param='sum_energy_per_cycle_mux[0to37]+energy_per_cycle_sb_mux[1][1]_rrnode[568]'
***** Load for rr_node[568] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=77, type=5 *****
Xchan_mux_1level_tapbuf_size2[38]->out_loadlvl[0]_out mux_1level_tapbuf_size2[38]->out mux_1level_tapbuf_size2[38]->out_loadlvl[0]_out mux_1level_tapbuf_size2[38]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[99]_no0 mux_1level_tapbuf_size2[38]->out_loadlvl[0]_out mux_1level_tapbuf_size2[38]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[100]_no0 mux_1level_tapbuf_size2[38]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[38]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[101]_no0 mux_1level_tapbuf_size2[38]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[38]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to38] 
+          param='sum_leakage_power_sb_mux[0to37]+leakage_sb_mux[1][1]_rrnode[568]'
.meas tran sum_energy_per_cycle_sb_mux[0to38] 
+          param='sum_energy_per_cycle_sb_mux[0to37]+energy_per_cycle_sb_mux[1][1]_rrnode[568]'
Xmux_1level_tapbuf_size2[39] mux_1level_tapbuf_size2[39]->in[0] mux_1level_tapbuf_size2[39]->in[1] mux_1level_tapbuf_size2[39]->out sram[49]->outb sram[49]->out gvdd_mux_1level_tapbuf_size2[39] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[39], level=1, select_path_id=0. *****
*****1*****
Xsram[49] sram->in sram[49]->out sram[49]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[49]->out) 0
.nodeset V(sram[49]->outb) vsp
***** Signal mux_1level_tapbuf_size2[39]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[39]->in[0] mux_1level_tapbuf_size2[39]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[39]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[39]->in[1] mux_1level_tapbuf_size2[39]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[39] gvdd_mux_1level_tapbuf_size2[39] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[570] trig v(mux_1level_tapbuf_size2[39]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[39]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[570] trig v(mux_1level_tapbuf_size2[39]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[39]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[570] when v(mux_1level_tapbuf_size2[39]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[570] trig v(mux_1level_tapbuf_size2[39]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[39]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[570] when v(mux_1level_tapbuf_size2[39]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[570] trig v(mux_1level_tapbuf_size2[39]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[39]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[39]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[39]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[570] param='mux_1level_tapbuf_size2[39]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[39]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[39]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[39]_energy_per_cycle param='mux_1level_tapbuf_size2[39]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[570]  param='mux_1level_tapbuf_size2[39]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[570]  param='dynamic_power_sb_mux[1][1]_rrnode[570]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[570] avg p(Vgvdd_mux_1level_tapbuf_size2[39]) from='start_rise_sb_mux[1][1]_rrnode[570]' to='start_rise_sb_mux[1][1]_rrnode[570]+switch_rise_sb_mux[1][1]_rrnode[570]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[570] avg p(Vgvdd_mux_1level_tapbuf_size2[39]) from='start_fall_sb_mux[1][1]_rrnode[570]' to='start_fall_sb_mux[1][1]_rrnode[570]+switch_fall_sb_mux[1][1]_rrnode[570]'
.meas tran sum_leakage_power_mux[0to39] 
+          param='sum_leakage_power_mux[0to38]+leakage_sb_mux[1][1]_rrnode[570]'
.meas tran sum_energy_per_cycle_mux[0to39] 
+          param='sum_energy_per_cycle_mux[0to38]+energy_per_cycle_sb_mux[1][1]_rrnode[570]'
***** Load for rr_node[570] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=79, type=5 *****
Xchan_mux_1level_tapbuf_size2[39]->out_loadlvl[0]_out mux_1level_tapbuf_size2[39]->out mux_1level_tapbuf_size2[39]->out_loadlvl[0]_out mux_1level_tapbuf_size2[39]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[102]_no0 mux_1level_tapbuf_size2[39]->out_loadlvl[0]_out mux_1level_tapbuf_size2[39]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[103]_no0 mux_1level_tapbuf_size2[39]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[39]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to39] 
+          param='sum_leakage_power_sb_mux[0to38]+leakage_sb_mux[1][1]_rrnode[570]'
.meas tran sum_energy_per_cycle_sb_mux[0to39] 
+          param='sum_energy_per_cycle_sb_mux[0to38]+energy_per_cycle_sb_mux[1][1]_rrnode[570]'
Xmux_1level_tapbuf_size2[40] mux_1level_tapbuf_size2[40]->in[0] mux_1level_tapbuf_size2[40]->in[1] mux_1level_tapbuf_size2[40]->out sram[50]->outb sram[50]->out gvdd_mux_1level_tapbuf_size2[40] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[40], level=1, select_path_id=0. *****
*****1*****
Xsram[50] sram->in sram[50]->out sram[50]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[50]->out) 0
.nodeset V(sram[50]->outb) vsp
***** Signal mux_1level_tapbuf_size2[40]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[40]->in[0] mux_1level_tapbuf_size2[40]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[40]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[40]->in[1] mux_1level_tapbuf_size2[40]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[40] gvdd_mux_1level_tapbuf_size2[40] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[572] trig v(mux_1level_tapbuf_size2[40]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[40]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[572] trig v(mux_1level_tapbuf_size2[40]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[40]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[572] when v(mux_1level_tapbuf_size2[40]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[572] trig v(mux_1level_tapbuf_size2[40]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[40]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[572] when v(mux_1level_tapbuf_size2[40]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[572] trig v(mux_1level_tapbuf_size2[40]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[40]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[40]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[40]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[572] param='mux_1level_tapbuf_size2[40]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[40]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[40]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[40]_energy_per_cycle param='mux_1level_tapbuf_size2[40]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[572]  param='mux_1level_tapbuf_size2[40]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[572]  param='dynamic_power_sb_mux[1][1]_rrnode[572]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[572] avg p(Vgvdd_mux_1level_tapbuf_size2[40]) from='start_rise_sb_mux[1][1]_rrnode[572]' to='start_rise_sb_mux[1][1]_rrnode[572]+switch_rise_sb_mux[1][1]_rrnode[572]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[572] avg p(Vgvdd_mux_1level_tapbuf_size2[40]) from='start_fall_sb_mux[1][1]_rrnode[572]' to='start_fall_sb_mux[1][1]_rrnode[572]+switch_fall_sb_mux[1][1]_rrnode[572]'
.meas tran sum_leakage_power_mux[0to40] 
+          param='sum_leakage_power_mux[0to39]+leakage_sb_mux[1][1]_rrnode[572]'
.meas tran sum_energy_per_cycle_mux[0to40] 
+          param='sum_energy_per_cycle_mux[0to39]+energy_per_cycle_sb_mux[1][1]_rrnode[572]'
***** Load for rr_node[572] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=81, type=5 *****
Xchan_mux_1level_tapbuf_size2[40]->out_loadlvl[0]_out mux_1level_tapbuf_size2[40]->out mux_1level_tapbuf_size2[40]->out_loadlvl[0]_out mux_1level_tapbuf_size2[40]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[104]_no0 mux_1level_tapbuf_size2[40]->out_loadlvl[0]_out mux_1level_tapbuf_size2[40]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[105]_no0 mux_1level_tapbuf_size2[40]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[40]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[106]_no0 mux_1level_tapbuf_size2[40]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[40]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to40] 
+          param='sum_leakage_power_sb_mux[0to39]+leakage_sb_mux[1][1]_rrnode[572]'
.meas tran sum_energy_per_cycle_sb_mux[0to40] 
+          param='sum_energy_per_cycle_sb_mux[0to39]+energy_per_cycle_sb_mux[1][1]_rrnode[572]'
Xmux_1level_tapbuf_size2[41] mux_1level_tapbuf_size2[41]->in[0] mux_1level_tapbuf_size2[41]->in[1] mux_1level_tapbuf_size2[41]->out sram[51]->outb sram[51]->out gvdd_mux_1level_tapbuf_size2[41] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[41], level=1, select_path_id=0. *****
*****1*****
Xsram[51] sram->in sram[51]->out sram[51]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[51]->out) 0
.nodeset V(sram[51]->outb) vsp
***** Signal mux_1level_tapbuf_size2[41]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[41]->in[0] mux_1level_tapbuf_size2[41]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[41]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[41]->in[1] mux_1level_tapbuf_size2[41]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[41] gvdd_mux_1level_tapbuf_size2[41] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[574] trig v(mux_1level_tapbuf_size2[41]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[41]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[574] trig v(mux_1level_tapbuf_size2[41]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[41]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[574] when v(mux_1level_tapbuf_size2[41]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[574] trig v(mux_1level_tapbuf_size2[41]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[41]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[574] when v(mux_1level_tapbuf_size2[41]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[574] trig v(mux_1level_tapbuf_size2[41]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[41]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[41]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[41]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[574] param='mux_1level_tapbuf_size2[41]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[41]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[41]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[41]_energy_per_cycle param='mux_1level_tapbuf_size2[41]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[574]  param='mux_1level_tapbuf_size2[41]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[574]  param='dynamic_power_sb_mux[1][1]_rrnode[574]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[574] avg p(Vgvdd_mux_1level_tapbuf_size2[41]) from='start_rise_sb_mux[1][1]_rrnode[574]' to='start_rise_sb_mux[1][1]_rrnode[574]+switch_rise_sb_mux[1][1]_rrnode[574]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[574] avg p(Vgvdd_mux_1level_tapbuf_size2[41]) from='start_fall_sb_mux[1][1]_rrnode[574]' to='start_fall_sb_mux[1][1]_rrnode[574]+switch_fall_sb_mux[1][1]_rrnode[574]'
.meas tran sum_leakage_power_mux[0to41] 
+          param='sum_leakage_power_mux[0to40]+leakage_sb_mux[1][1]_rrnode[574]'
.meas tran sum_energy_per_cycle_mux[0to41] 
+          param='sum_energy_per_cycle_mux[0to40]+energy_per_cycle_sb_mux[1][1]_rrnode[574]'
***** Load for rr_node[574] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=83, type=5 *****
Xchan_mux_1level_tapbuf_size2[41]->out_loadlvl[0]_out mux_1level_tapbuf_size2[41]->out mux_1level_tapbuf_size2[41]->out_loadlvl[0]_out mux_1level_tapbuf_size2[41]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[107]_no0 mux_1level_tapbuf_size2[41]->out_loadlvl[0]_out mux_1level_tapbuf_size2[41]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[108]_no0 mux_1level_tapbuf_size2[41]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[41]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to41] 
+          param='sum_leakage_power_sb_mux[0to40]+leakage_sb_mux[1][1]_rrnode[574]'
.meas tran sum_energy_per_cycle_sb_mux[0to41] 
+          param='sum_energy_per_cycle_sb_mux[0to40]+energy_per_cycle_sb_mux[1][1]_rrnode[574]'
Xmux_1level_tapbuf_size2[42] mux_1level_tapbuf_size2[42]->in[0] mux_1level_tapbuf_size2[42]->in[1] mux_1level_tapbuf_size2[42]->out sram[52]->outb sram[52]->out gvdd_mux_1level_tapbuf_size2[42] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[42], level=1, select_path_id=0. *****
*****1*****
Xsram[52] sram->in sram[52]->out sram[52]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[52]->out) 0
.nodeset V(sram[52]->outb) vsp
***** Signal mux_1level_tapbuf_size2[42]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[42]->in[0] mux_1level_tapbuf_size2[42]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[42]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[42]->in[1] mux_1level_tapbuf_size2[42]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[42] gvdd_mux_1level_tapbuf_size2[42] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[576] trig v(mux_1level_tapbuf_size2[42]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[42]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[576] trig v(mux_1level_tapbuf_size2[42]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[42]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[576] when v(mux_1level_tapbuf_size2[42]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[576] trig v(mux_1level_tapbuf_size2[42]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[42]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[576] when v(mux_1level_tapbuf_size2[42]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[576] trig v(mux_1level_tapbuf_size2[42]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[42]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[42]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[42]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[576] param='mux_1level_tapbuf_size2[42]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[42]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[42]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[42]_energy_per_cycle param='mux_1level_tapbuf_size2[42]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[576]  param='mux_1level_tapbuf_size2[42]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[576]  param='dynamic_power_sb_mux[1][1]_rrnode[576]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[576] avg p(Vgvdd_mux_1level_tapbuf_size2[42]) from='start_rise_sb_mux[1][1]_rrnode[576]' to='start_rise_sb_mux[1][1]_rrnode[576]+switch_rise_sb_mux[1][1]_rrnode[576]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[576] avg p(Vgvdd_mux_1level_tapbuf_size2[42]) from='start_fall_sb_mux[1][1]_rrnode[576]' to='start_fall_sb_mux[1][1]_rrnode[576]+switch_fall_sb_mux[1][1]_rrnode[576]'
.meas tran sum_leakage_power_mux[0to42] 
+          param='sum_leakage_power_mux[0to41]+leakage_sb_mux[1][1]_rrnode[576]'
.meas tran sum_energy_per_cycle_mux[0to42] 
+          param='sum_energy_per_cycle_mux[0to41]+energy_per_cycle_sb_mux[1][1]_rrnode[576]'
***** Load for rr_node[576] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=85, type=5 *****
Xchan_mux_1level_tapbuf_size2[42]->out_loadlvl[0]_out mux_1level_tapbuf_size2[42]->out mux_1level_tapbuf_size2[42]->out_loadlvl[0]_out mux_1level_tapbuf_size2[42]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[109]_no0 mux_1level_tapbuf_size2[42]->out_loadlvl[0]_out mux_1level_tapbuf_size2[42]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[110]_no0 mux_1level_tapbuf_size2[42]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[42]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[111]_no0 mux_1level_tapbuf_size2[42]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[42]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to42] 
+          param='sum_leakage_power_sb_mux[0to41]+leakage_sb_mux[1][1]_rrnode[576]'
.meas tran sum_energy_per_cycle_sb_mux[0to42] 
+          param='sum_energy_per_cycle_sb_mux[0to41]+energy_per_cycle_sb_mux[1][1]_rrnode[576]'
Xmux_1level_tapbuf_size2[43] mux_1level_tapbuf_size2[43]->in[0] mux_1level_tapbuf_size2[43]->in[1] mux_1level_tapbuf_size2[43]->out sram[53]->outb sram[53]->out gvdd_mux_1level_tapbuf_size2[43] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[43], level=1, select_path_id=0. *****
*****1*****
Xsram[53] sram->in sram[53]->out sram[53]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[53]->out) 0
.nodeset V(sram[53]->outb) vsp
***** Signal mux_1level_tapbuf_size2[43]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[43]->in[0] mux_1level_tapbuf_size2[43]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[43]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[43]->in[1] mux_1level_tapbuf_size2[43]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[43] gvdd_mux_1level_tapbuf_size2[43] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[578] trig v(mux_1level_tapbuf_size2[43]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[43]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[578] trig v(mux_1level_tapbuf_size2[43]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[43]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[578] when v(mux_1level_tapbuf_size2[43]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[578] trig v(mux_1level_tapbuf_size2[43]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[43]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[578] when v(mux_1level_tapbuf_size2[43]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[578] trig v(mux_1level_tapbuf_size2[43]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[43]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[43]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[43]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[578] param='mux_1level_tapbuf_size2[43]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[43]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[43]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[43]_energy_per_cycle param='mux_1level_tapbuf_size2[43]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[578]  param='mux_1level_tapbuf_size2[43]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[578]  param='dynamic_power_sb_mux[1][1]_rrnode[578]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[578] avg p(Vgvdd_mux_1level_tapbuf_size2[43]) from='start_rise_sb_mux[1][1]_rrnode[578]' to='start_rise_sb_mux[1][1]_rrnode[578]+switch_rise_sb_mux[1][1]_rrnode[578]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[578] avg p(Vgvdd_mux_1level_tapbuf_size2[43]) from='start_fall_sb_mux[1][1]_rrnode[578]' to='start_fall_sb_mux[1][1]_rrnode[578]+switch_fall_sb_mux[1][1]_rrnode[578]'
.meas tran sum_leakage_power_mux[0to43] 
+          param='sum_leakage_power_mux[0to42]+leakage_sb_mux[1][1]_rrnode[578]'
.meas tran sum_energy_per_cycle_mux[0to43] 
+          param='sum_energy_per_cycle_mux[0to42]+energy_per_cycle_sb_mux[1][1]_rrnode[578]'
***** Load for rr_node[578] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=87, type=5 *****
Xchan_mux_1level_tapbuf_size2[43]->out_loadlvl[0]_out mux_1level_tapbuf_size2[43]->out mux_1level_tapbuf_size2[43]->out_loadlvl[0]_out mux_1level_tapbuf_size2[43]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[112]_no0 mux_1level_tapbuf_size2[43]->out_loadlvl[0]_out mux_1level_tapbuf_size2[43]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[113]_no0 mux_1level_tapbuf_size2[43]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[43]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[114]_no0 mux_1level_tapbuf_size2[43]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[43]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to43] 
+          param='sum_leakage_power_sb_mux[0to42]+leakage_sb_mux[1][1]_rrnode[578]'
.meas tran sum_energy_per_cycle_sb_mux[0to43] 
+          param='sum_energy_per_cycle_sb_mux[0to42]+energy_per_cycle_sb_mux[1][1]_rrnode[578]'
Xmux_1level_tapbuf_size2[44] mux_1level_tapbuf_size2[44]->in[0] mux_1level_tapbuf_size2[44]->in[1] mux_1level_tapbuf_size2[44]->out sram[54]->outb sram[54]->out gvdd_mux_1level_tapbuf_size2[44] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[44], level=1, select_path_id=0. *****
*****1*****
Xsram[54] sram->in sram[54]->out sram[54]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[54]->out) 0
.nodeset V(sram[54]->outb) vsp
***** Signal mux_1level_tapbuf_size2[44]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[44]->in[0] mux_1level_tapbuf_size2[44]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[44]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[44]->in[1] mux_1level_tapbuf_size2[44]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[44] gvdd_mux_1level_tapbuf_size2[44] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[580] trig v(mux_1level_tapbuf_size2[44]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[44]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[580] trig v(mux_1level_tapbuf_size2[44]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[44]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[580] when v(mux_1level_tapbuf_size2[44]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[580] trig v(mux_1level_tapbuf_size2[44]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[44]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[580] when v(mux_1level_tapbuf_size2[44]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[580] trig v(mux_1level_tapbuf_size2[44]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[44]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[44]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[44]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[580] param='mux_1level_tapbuf_size2[44]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[44]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[44]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[44]_energy_per_cycle param='mux_1level_tapbuf_size2[44]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[580]  param='mux_1level_tapbuf_size2[44]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[580]  param='dynamic_power_sb_mux[1][1]_rrnode[580]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[580] avg p(Vgvdd_mux_1level_tapbuf_size2[44]) from='start_rise_sb_mux[1][1]_rrnode[580]' to='start_rise_sb_mux[1][1]_rrnode[580]+switch_rise_sb_mux[1][1]_rrnode[580]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[580] avg p(Vgvdd_mux_1level_tapbuf_size2[44]) from='start_fall_sb_mux[1][1]_rrnode[580]' to='start_fall_sb_mux[1][1]_rrnode[580]+switch_fall_sb_mux[1][1]_rrnode[580]'
.meas tran sum_leakage_power_mux[0to44] 
+          param='sum_leakage_power_mux[0to43]+leakage_sb_mux[1][1]_rrnode[580]'
.meas tran sum_energy_per_cycle_mux[0to44] 
+          param='sum_energy_per_cycle_mux[0to43]+energy_per_cycle_sb_mux[1][1]_rrnode[580]'
***** Load for rr_node[580] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=89, type=5 *****
Xchan_mux_1level_tapbuf_size2[44]->out_loadlvl[0]_out mux_1level_tapbuf_size2[44]->out mux_1level_tapbuf_size2[44]->out_loadlvl[0]_out mux_1level_tapbuf_size2[44]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[115]_no0 mux_1level_tapbuf_size2[44]->out_loadlvl[0]_out mux_1level_tapbuf_size2[44]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[116]_no0 mux_1level_tapbuf_size2[44]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[44]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to44] 
+          param='sum_leakage_power_sb_mux[0to43]+leakage_sb_mux[1][1]_rrnode[580]'
.meas tran sum_energy_per_cycle_sb_mux[0to44] 
+          param='sum_energy_per_cycle_sb_mux[0to43]+energy_per_cycle_sb_mux[1][1]_rrnode[580]'
Xmux_1level_tapbuf_size2[45] mux_1level_tapbuf_size2[45]->in[0] mux_1level_tapbuf_size2[45]->in[1] mux_1level_tapbuf_size2[45]->out sram[55]->outb sram[55]->out gvdd_mux_1level_tapbuf_size2[45] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[45], level=1, select_path_id=0. *****
*****1*****
Xsram[55] sram->in sram[55]->out sram[55]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[55]->out) 0
.nodeset V(sram[55]->outb) vsp
***** Signal mux_1level_tapbuf_size2[45]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[45]->in[0] mux_1level_tapbuf_size2[45]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[45]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[45]->in[1] mux_1level_tapbuf_size2[45]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[45] gvdd_mux_1level_tapbuf_size2[45] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[582] trig v(mux_1level_tapbuf_size2[45]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[45]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[582] trig v(mux_1level_tapbuf_size2[45]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[45]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[582] when v(mux_1level_tapbuf_size2[45]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[582] trig v(mux_1level_tapbuf_size2[45]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[45]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[582] when v(mux_1level_tapbuf_size2[45]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[582] trig v(mux_1level_tapbuf_size2[45]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[45]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[45]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[45]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[582] param='mux_1level_tapbuf_size2[45]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[45]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[45]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[45]_energy_per_cycle param='mux_1level_tapbuf_size2[45]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[582]  param='mux_1level_tapbuf_size2[45]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[582]  param='dynamic_power_sb_mux[1][1]_rrnode[582]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[582] avg p(Vgvdd_mux_1level_tapbuf_size2[45]) from='start_rise_sb_mux[1][1]_rrnode[582]' to='start_rise_sb_mux[1][1]_rrnode[582]+switch_rise_sb_mux[1][1]_rrnode[582]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[582] avg p(Vgvdd_mux_1level_tapbuf_size2[45]) from='start_fall_sb_mux[1][1]_rrnode[582]' to='start_fall_sb_mux[1][1]_rrnode[582]+switch_fall_sb_mux[1][1]_rrnode[582]'
.meas tran sum_leakage_power_mux[0to45] 
+          param='sum_leakage_power_mux[0to44]+leakage_sb_mux[1][1]_rrnode[582]'
.meas tran sum_energy_per_cycle_mux[0to45] 
+          param='sum_energy_per_cycle_mux[0to44]+energy_per_cycle_sb_mux[1][1]_rrnode[582]'
***** Load for rr_node[582] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=91, type=5 *****
Xchan_mux_1level_tapbuf_size2[45]->out_loadlvl[0]_out mux_1level_tapbuf_size2[45]->out mux_1level_tapbuf_size2[45]->out_loadlvl[0]_out mux_1level_tapbuf_size2[45]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[117]_no0 mux_1level_tapbuf_size2[45]->out_loadlvl[0]_out mux_1level_tapbuf_size2[45]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[118]_no0 mux_1level_tapbuf_size2[45]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[45]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[119]_no0 mux_1level_tapbuf_size2[45]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[45]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to45] 
+          param='sum_leakage_power_sb_mux[0to44]+leakage_sb_mux[1][1]_rrnode[582]'
.meas tran sum_energy_per_cycle_sb_mux[0to45] 
+          param='sum_energy_per_cycle_sb_mux[0to44]+energy_per_cycle_sb_mux[1][1]_rrnode[582]'
Xmux_1level_tapbuf_size2[46] mux_1level_tapbuf_size2[46]->in[0] mux_1level_tapbuf_size2[46]->in[1] mux_1level_tapbuf_size2[46]->out sram[56]->outb sram[56]->out gvdd_mux_1level_tapbuf_size2[46] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[46], level=1, select_path_id=0. *****
*****1*****
Xsram[56] sram->in sram[56]->out sram[56]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[56]->out) 0
.nodeset V(sram[56]->outb) vsp
***** Signal mux_1level_tapbuf_size2[46]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[46]->in[0] mux_1level_tapbuf_size2[46]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[46]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[46]->in[1] mux_1level_tapbuf_size2[46]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[46] gvdd_mux_1level_tapbuf_size2[46] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[584] trig v(mux_1level_tapbuf_size2[46]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[46]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[584] trig v(mux_1level_tapbuf_size2[46]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[46]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[584] when v(mux_1level_tapbuf_size2[46]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[584] trig v(mux_1level_tapbuf_size2[46]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[46]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[584] when v(mux_1level_tapbuf_size2[46]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[584] trig v(mux_1level_tapbuf_size2[46]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[46]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[46]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[46]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[584] param='mux_1level_tapbuf_size2[46]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[46]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[46]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[46]_energy_per_cycle param='mux_1level_tapbuf_size2[46]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[584]  param='mux_1level_tapbuf_size2[46]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[584]  param='dynamic_power_sb_mux[1][1]_rrnode[584]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[584] avg p(Vgvdd_mux_1level_tapbuf_size2[46]) from='start_rise_sb_mux[1][1]_rrnode[584]' to='start_rise_sb_mux[1][1]_rrnode[584]+switch_rise_sb_mux[1][1]_rrnode[584]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[584] avg p(Vgvdd_mux_1level_tapbuf_size2[46]) from='start_fall_sb_mux[1][1]_rrnode[584]' to='start_fall_sb_mux[1][1]_rrnode[584]+switch_fall_sb_mux[1][1]_rrnode[584]'
.meas tran sum_leakage_power_mux[0to46] 
+          param='sum_leakage_power_mux[0to45]+leakage_sb_mux[1][1]_rrnode[584]'
.meas tran sum_energy_per_cycle_mux[0to46] 
+          param='sum_energy_per_cycle_mux[0to45]+energy_per_cycle_sb_mux[1][1]_rrnode[584]'
***** Load for rr_node[584] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=93, type=5 *****
Xchan_mux_1level_tapbuf_size2[46]->out_loadlvl[0]_out mux_1level_tapbuf_size2[46]->out mux_1level_tapbuf_size2[46]->out_loadlvl[0]_out mux_1level_tapbuf_size2[46]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[120]_no0 mux_1level_tapbuf_size2[46]->out_loadlvl[0]_out mux_1level_tapbuf_size2[46]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[121]_no0 mux_1level_tapbuf_size2[46]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[46]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to46] 
+          param='sum_leakage_power_sb_mux[0to45]+leakage_sb_mux[1][1]_rrnode[584]'
.meas tran sum_energy_per_cycle_sb_mux[0to46] 
+          param='sum_energy_per_cycle_sb_mux[0to45]+energy_per_cycle_sb_mux[1][1]_rrnode[584]'
Xmux_1level_tapbuf_size2[47] mux_1level_tapbuf_size2[47]->in[0] mux_1level_tapbuf_size2[47]->in[1] mux_1level_tapbuf_size2[47]->out sram[57]->outb sram[57]->out gvdd_mux_1level_tapbuf_size2[47] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[47], level=1, select_path_id=0. *****
*****1*****
Xsram[57] sram->in sram[57]->out sram[57]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[57]->out) 0
.nodeset V(sram[57]->outb) vsp
***** Signal mux_1level_tapbuf_size2[47]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[47]->in[0] mux_1level_tapbuf_size2[47]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[47]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[47]->in[1] mux_1level_tapbuf_size2[47]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[47] gvdd_mux_1level_tapbuf_size2[47] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[586] trig v(mux_1level_tapbuf_size2[47]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[47]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[586] trig v(mux_1level_tapbuf_size2[47]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[47]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[586] when v(mux_1level_tapbuf_size2[47]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[586] trig v(mux_1level_tapbuf_size2[47]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[47]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[586] when v(mux_1level_tapbuf_size2[47]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[586] trig v(mux_1level_tapbuf_size2[47]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[47]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[47]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[47]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[586] param='mux_1level_tapbuf_size2[47]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[47]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[47]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[47]_energy_per_cycle param='mux_1level_tapbuf_size2[47]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[586]  param='mux_1level_tapbuf_size2[47]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[586]  param='dynamic_power_sb_mux[1][1]_rrnode[586]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[586] avg p(Vgvdd_mux_1level_tapbuf_size2[47]) from='start_rise_sb_mux[1][1]_rrnode[586]' to='start_rise_sb_mux[1][1]_rrnode[586]+switch_rise_sb_mux[1][1]_rrnode[586]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[586] avg p(Vgvdd_mux_1level_tapbuf_size2[47]) from='start_fall_sb_mux[1][1]_rrnode[586]' to='start_fall_sb_mux[1][1]_rrnode[586]+switch_fall_sb_mux[1][1]_rrnode[586]'
.meas tran sum_leakage_power_mux[0to47] 
+          param='sum_leakage_power_mux[0to46]+leakage_sb_mux[1][1]_rrnode[586]'
.meas tran sum_energy_per_cycle_mux[0to47] 
+          param='sum_energy_per_cycle_mux[0to46]+energy_per_cycle_sb_mux[1][1]_rrnode[586]'
***** Load for rr_node[586] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=95, type=5 *****
Xchan_mux_1level_tapbuf_size2[47]->out_loadlvl[0]_out mux_1level_tapbuf_size2[47]->out mux_1level_tapbuf_size2[47]->out_loadlvl[0]_out mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[122]_no0 mux_1level_tapbuf_size2[47]->out_loadlvl[0]_out mux_1level_tapbuf_size2[47]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[123]_no0 mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[124]_no0 mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[47]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to47] 
+          param='sum_leakage_power_sb_mux[0to46]+leakage_sb_mux[1][1]_rrnode[586]'
.meas tran sum_energy_per_cycle_sb_mux[0to47] 
+          param='sum_energy_per_cycle_sb_mux[0to46]+energy_per_cycle_sb_mux[1][1]_rrnode[586]'
Xmux_1level_tapbuf_size2[48] mux_1level_tapbuf_size2[48]->in[0] mux_1level_tapbuf_size2[48]->in[1] mux_1level_tapbuf_size2[48]->out sram[58]->outb sram[58]->out gvdd_mux_1level_tapbuf_size2[48] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[48], level=1, select_path_id=0. *****
*****1*****
Xsram[58] sram->in sram[58]->out sram[58]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[58]->out) 0
.nodeset V(sram[58]->outb) vsp
***** Signal mux_1level_tapbuf_size2[48]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[48]->in[0] mux_1level_tapbuf_size2[48]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[48]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[48]->in[1] mux_1level_tapbuf_size2[48]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[48] gvdd_mux_1level_tapbuf_size2[48] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[588] trig v(mux_1level_tapbuf_size2[48]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[48]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[588] trig v(mux_1level_tapbuf_size2[48]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[48]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[588] when v(mux_1level_tapbuf_size2[48]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[588] trig v(mux_1level_tapbuf_size2[48]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[48]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[588] when v(mux_1level_tapbuf_size2[48]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[588] trig v(mux_1level_tapbuf_size2[48]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[48]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[48]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[48]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[588] param='mux_1level_tapbuf_size2[48]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[48]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[48]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[48]_energy_per_cycle param='mux_1level_tapbuf_size2[48]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[588]  param='mux_1level_tapbuf_size2[48]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[588]  param='dynamic_power_sb_mux[1][1]_rrnode[588]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[588] avg p(Vgvdd_mux_1level_tapbuf_size2[48]) from='start_rise_sb_mux[1][1]_rrnode[588]' to='start_rise_sb_mux[1][1]_rrnode[588]+switch_rise_sb_mux[1][1]_rrnode[588]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[588] avg p(Vgvdd_mux_1level_tapbuf_size2[48]) from='start_fall_sb_mux[1][1]_rrnode[588]' to='start_fall_sb_mux[1][1]_rrnode[588]+switch_fall_sb_mux[1][1]_rrnode[588]'
.meas tran sum_leakage_power_mux[0to48] 
+          param='sum_leakage_power_mux[0to47]+leakage_sb_mux[1][1]_rrnode[588]'
.meas tran sum_energy_per_cycle_mux[0to48] 
+          param='sum_energy_per_cycle_mux[0to47]+energy_per_cycle_sb_mux[1][1]_rrnode[588]'
***** Load for rr_node[588] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=97, type=5 *****
Xchan_mux_1level_tapbuf_size2[48]->out_loadlvl[0]_out mux_1level_tapbuf_size2[48]->out mux_1level_tapbuf_size2[48]->out_loadlvl[0]_out mux_1level_tapbuf_size2[48]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[125]_no0 mux_1level_tapbuf_size2[48]->out_loadlvl[0]_out mux_1level_tapbuf_size2[48]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[126]_no0 mux_1level_tapbuf_size2[48]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[48]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[127]_no0 mux_1level_tapbuf_size2[48]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[48]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to48] 
+          param='sum_leakage_power_sb_mux[0to47]+leakage_sb_mux[1][1]_rrnode[588]'
.meas tran sum_energy_per_cycle_sb_mux[0to48] 
+          param='sum_energy_per_cycle_sb_mux[0to47]+energy_per_cycle_sb_mux[1][1]_rrnode[588]'
Xmux_1level_tapbuf_size2[49] mux_1level_tapbuf_size2[49]->in[0] mux_1level_tapbuf_size2[49]->in[1] mux_1level_tapbuf_size2[49]->out sram[59]->outb sram[59]->out gvdd_mux_1level_tapbuf_size2[49] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[49], level=1, select_path_id=0. *****
*****1*****
Xsram[59] sram->in sram[59]->out sram[59]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[59]->out) 0
.nodeset V(sram[59]->outb) vsp
***** Signal mux_1level_tapbuf_size2[49]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[49]->in[0] mux_1level_tapbuf_size2[49]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[49]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[49]->in[1] mux_1level_tapbuf_size2[49]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[49] gvdd_mux_1level_tapbuf_size2[49] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[590] trig v(mux_1level_tapbuf_size2[49]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[49]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[590] trig v(mux_1level_tapbuf_size2[49]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[49]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[590] when v(mux_1level_tapbuf_size2[49]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[590] trig v(mux_1level_tapbuf_size2[49]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[49]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[590] when v(mux_1level_tapbuf_size2[49]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[590] trig v(mux_1level_tapbuf_size2[49]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[49]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[49]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[49]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[590] param='mux_1level_tapbuf_size2[49]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[49]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[49]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[49]_energy_per_cycle param='mux_1level_tapbuf_size2[49]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[590]  param='mux_1level_tapbuf_size2[49]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[590]  param='dynamic_power_sb_mux[1][1]_rrnode[590]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[590] avg p(Vgvdd_mux_1level_tapbuf_size2[49]) from='start_rise_sb_mux[1][1]_rrnode[590]' to='start_rise_sb_mux[1][1]_rrnode[590]+switch_rise_sb_mux[1][1]_rrnode[590]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[590] avg p(Vgvdd_mux_1level_tapbuf_size2[49]) from='start_fall_sb_mux[1][1]_rrnode[590]' to='start_fall_sb_mux[1][1]_rrnode[590]+switch_fall_sb_mux[1][1]_rrnode[590]'
.meas tran sum_leakage_power_mux[0to49] 
+          param='sum_leakage_power_mux[0to48]+leakage_sb_mux[1][1]_rrnode[590]'
.meas tran sum_energy_per_cycle_mux[0to49] 
+          param='sum_energy_per_cycle_mux[0to48]+energy_per_cycle_sb_mux[1][1]_rrnode[590]'
***** Load for rr_node[590] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=99, type=5 *****
Xchan_mux_1level_tapbuf_size2[49]->out_loadlvl[0]_out mux_1level_tapbuf_size2[49]->out mux_1level_tapbuf_size2[49]->out_loadlvl[0]_out mux_1level_tapbuf_size2[49]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[128]_no0 mux_1level_tapbuf_size2[49]->out_loadlvl[0]_out mux_1level_tapbuf_size2[49]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[129]_no0 mux_1level_tapbuf_size2[49]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[49]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to49] 
+          param='sum_leakage_power_sb_mux[0to48]+leakage_sb_mux[1][1]_rrnode[590]'
.meas tran sum_energy_per_cycle_sb_mux[0to49] 
+          param='sum_energy_per_cycle_sb_mux[0to48]+energy_per_cycle_sb_mux[1][1]_rrnode[590]'
Xmux_1level_tapbuf_size3[50] mux_1level_tapbuf_size3[50]->in[0] mux_1level_tapbuf_size3[50]->in[1] mux_1level_tapbuf_size3[50]->in[2] mux_1level_tapbuf_size3[50]->out sram[60]->outb sram[60]->out sram[61]->out sram[61]->outb sram[62]->out sram[62]->outb gvdd_mux_1level_tapbuf_size3[50] 0 mux_1level_tapbuf_size3
***** SRAM bits for MUX[50], level=1, select_path_id=0. *****
*****100*****
Xsram[60] sram->in sram[60]->out sram[60]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[60]->out) 0
.nodeset V(sram[60]->outb) vsp
Xsram[61] sram->in sram[61]->out sram[61]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[61]->out) 0
.nodeset V(sram[61]->outb) vsp
Xsram[62] sram->in sram[62]->out sram[62]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[62]->out) 0
.nodeset V(sram[62]->outb) vsp
***** Signal mux_1level_tapbuf_size3[50]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[50]->in[0] mux_1level_tapbuf_size3[50]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size3[50]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[50]->in[1] mux_1level_tapbuf_size3[50]->in[1] 0 
+  0
***** Signal mux_1level_tapbuf_size3[50]->in[2] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[50]->in[2] mux_1level_tapbuf_size3[50]->in[2] 0 
+  0
Vgvdd_mux_1level_tapbuf_size3[50] gvdd_mux_1level_tapbuf_size3[50] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[292] trig v(mux_1level_tapbuf_size3[50]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[50]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[292] trig v(mux_1level_tapbuf_size3[50]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[50]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[292] when v(mux_1level_tapbuf_size3[50]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[292] trig v(mux_1level_tapbuf_size3[50]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[50]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[292] when v(mux_1level_tapbuf_size3[50]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[292] trig v(mux_1level_tapbuf_size3[50]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[50]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[50]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[50]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[292] param='mux_1level_tapbuf_size3[50]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[50]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[50]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size3[50]_energy_per_cycle param='mux_1level_tapbuf_size3[50]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[292]  param='mux_1level_tapbuf_size3[50]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[292]  param='dynamic_power_sb_mux[1][1]_rrnode[292]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[292] avg p(Vgvdd_mux_1level_tapbuf_size3[50]) from='start_rise_sb_mux[1][1]_rrnode[292]' to='start_rise_sb_mux[1][1]_rrnode[292]+switch_rise_sb_mux[1][1]_rrnode[292]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[292] avg p(Vgvdd_mux_1level_tapbuf_size3[50]) from='start_fall_sb_mux[1][1]_rrnode[292]' to='start_fall_sb_mux[1][1]_rrnode[292]+switch_fall_sb_mux[1][1]_rrnode[292]'
.meas tran sum_leakage_power_mux[0to50] 
+          param='sum_leakage_power_mux[0to49]+leakage_sb_mux[1][1]_rrnode[292]'
.meas tran sum_energy_per_cycle_mux[0to50] 
+          param='sum_energy_per_cycle_mux[0to49]+energy_per_cycle_sb_mux[1][1]_rrnode[292]'
***** Load for rr_node[292] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=1, type=4 *****
Xchan_mux_1level_tapbuf_size3[50]->out_loadlvl[0]_out mux_1level_tapbuf_size3[50]->out mux_1level_tapbuf_size3[50]->out_loadlvl[0]_out mux_1level_tapbuf_size3[50]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[130]_no0 mux_1level_tapbuf_size3[50]->out_loadlvl[0]_out mux_1level_tapbuf_size3[50]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[131]_no0 mux_1level_tapbuf_size3[50]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[50]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[132]_no0 mux_1level_tapbuf_size3[50]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[50]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[133]_no0 mux_1level_tapbuf_size3[50]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[50]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[134]_no0 mux_1level_tapbuf_size3[50]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[50]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to50] 
+          param='sum_leakage_power_sb_mux[0to49]+leakage_sb_mux[1][1]_rrnode[292]'
.meas tran sum_energy_per_cycle_sb_mux[0to50] 
+          param='sum_energy_per_cycle_sb_mux[0to49]+energy_per_cycle_sb_mux[1][1]_rrnode[292]'
Xmux_1level_tapbuf_size3[51] mux_1level_tapbuf_size3[51]->in[0] mux_1level_tapbuf_size3[51]->in[1] mux_1level_tapbuf_size3[51]->in[2] mux_1level_tapbuf_size3[51]->out sram[63]->outb sram[63]->out sram[64]->out sram[64]->outb sram[65]->out sram[65]->outb gvdd_mux_1level_tapbuf_size3[51] 0 mux_1level_tapbuf_size3
***** SRAM bits for MUX[51], level=1, select_path_id=0. *****
*****100*****
Xsram[63] sram->in sram[63]->out sram[63]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[63]->out) 0
.nodeset V(sram[63]->outb) vsp
Xsram[64] sram->in sram[64]->out sram[64]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[64]->out) 0
.nodeset V(sram[64]->outb) vsp
Xsram[65] sram->in sram[65]->out sram[65]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[65]->out) 0
.nodeset V(sram[65]->outb) vsp
***** Signal mux_1level_tapbuf_size3[51]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[51]->in[0] mux_1level_tapbuf_size3[51]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size3[51]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[51]->in[1] mux_1level_tapbuf_size3[51]->in[1] 0 
+  0
***** Signal mux_1level_tapbuf_size3[51]->in[2] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[51]->in[2] mux_1level_tapbuf_size3[51]->in[2] 0 
+  0
Vgvdd_mux_1level_tapbuf_size3[51] gvdd_mux_1level_tapbuf_size3[51] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[294] trig v(mux_1level_tapbuf_size3[51]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[51]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[294] trig v(mux_1level_tapbuf_size3[51]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[51]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[294] when v(mux_1level_tapbuf_size3[51]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[294] trig v(mux_1level_tapbuf_size3[51]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[51]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[294] when v(mux_1level_tapbuf_size3[51]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[294] trig v(mux_1level_tapbuf_size3[51]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[51]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[51]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[51]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[294] param='mux_1level_tapbuf_size3[51]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[51]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[51]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size3[51]_energy_per_cycle param='mux_1level_tapbuf_size3[51]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[294]  param='mux_1level_tapbuf_size3[51]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[294]  param='dynamic_power_sb_mux[1][1]_rrnode[294]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[294] avg p(Vgvdd_mux_1level_tapbuf_size3[51]) from='start_rise_sb_mux[1][1]_rrnode[294]' to='start_rise_sb_mux[1][1]_rrnode[294]+switch_rise_sb_mux[1][1]_rrnode[294]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[294] avg p(Vgvdd_mux_1level_tapbuf_size3[51]) from='start_fall_sb_mux[1][1]_rrnode[294]' to='start_fall_sb_mux[1][1]_rrnode[294]+switch_fall_sb_mux[1][1]_rrnode[294]'
.meas tran sum_leakage_power_mux[0to51] 
+          param='sum_leakage_power_mux[0to50]+leakage_sb_mux[1][1]_rrnode[294]'
.meas tran sum_energy_per_cycle_mux[0to51] 
+          param='sum_energy_per_cycle_mux[0to50]+energy_per_cycle_sb_mux[1][1]_rrnode[294]'
***** Load for rr_node[294] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=3, type=4 *****
Xchan_mux_1level_tapbuf_size3[51]->out_loadlvl[0]_out mux_1level_tapbuf_size3[51]->out mux_1level_tapbuf_size3[51]->out_loadlvl[0]_out mux_1level_tapbuf_size3[51]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[135]_no0 mux_1level_tapbuf_size3[51]->out_loadlvl[0]_out mux_1level_tapbuf_size3[51]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[136]_no0 mux_1level_tapbuf_size3[51]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[51]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[137]_no0 mux_1level_tapbuf_size3[51]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[51]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to51] 
+          param='sum_leakage_power_sb_mux[0to50]+leakage_sb_mux[1][1]_rrnode[294]'
.meas tran sum_energy_per_cycle_sb_mux[0to51] 
+          param='sum_energy_per_cycle_sb_mux[0to50]+energy_per_cycle_sb_mux[1][1]_rrnode[294]'
Xmux_1level_tapbuf_size3[52] mux_1level_tapbuf_size3[52]->in[0] mux_1level_tapbuf_size3[52]->in[1] mux_1level_tapbuf_size3[52]->in[2] mux_1level_tapbuf_size3[52]->out sram[66]->outb sram[66]->out sram[67]->out sram[67]->outb sram[68]->out sram[68]->outb gvdd_mux_1level_tapbuf_size3[52] 0 mux_1level_tapbuf_size3
***** SRAM bits for MUX[52], level=1, select_path_id=0. *****
*****100*****
Xsram[66] sram->in sram[66]->out sram[66]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[66]->out) 0
.nodeset V(sram[66]->outb) vsp
Xsram[67] sram->in sram[67]->out sram[67]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[67]->out) 0
.nodeset V(sram[67]->outb) vsp
Xsram[68] sram->in sram[68]->out sram[68]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[68]->out) 0
.nodeset V(sram[68]->outb) vsp
***** Signal mux_1level_tapbuf_size3[52]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[52]->in[0] mux_1level_tapbuf_size3[52]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size3[52]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[52]->in[1] mux_1level_tapbuf_size3[52]->in[1] 0 
+  0
***** Signal mux_1level_tapbuf_size3[52]->in[2] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[52]->in[2] mux_1level_tapbuf_size3[52]->in[2] 0 
+  0
Vgvdd_mux_1level_tapbuf_size3[52] gvdd_mux_1level_tapbuf_size3[52] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[296] trig v(mux_1level_tapbuf_size3[52]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[52]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[296] trig v(mux_1level_tapbuf_size3[52]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[52]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[296] when v(mux_1level_tapbuf_size3[52]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[296] trig v(mux_1level_tapbuf_size3[52]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[52]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[296] when v(mux_1level_tapbuf_size3[52]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[296] trig v(mux_1level_tapbuf_size3[52]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[52]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[52]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[52]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[296] param='mux_1level_tapbuf_size3[52]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[52]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[52]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size3[52]_energy_per_cycle param='mux_1level_tapbuf_size3[52]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[296]  param='mux_1level_tapbuf_size3[52]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[296]  param='dynamic_power_sb_mux[1][1]_rrnode[296]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[296] avg p(Vgvdd_mux_1level_tapbuf_size3[52]) from='start_rise_sb_mux[1][1]_rrnode[296]' to='start_rise_sb_mux[1][1]_rrnode[296]+switch_rise_sb_mux[1][1]_rrnode[296]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[296] avg p(Vgvdd_mux_1level_tapbuf_size3[52]) from='start_fall_sb_mux[1][1]_rrnode[296]' to='start_fall_sb_mux[1][1]_rrnode[296]+switch_fall_sb_mux[1][1]_rrnode[296]'
.meas tran sum_leakage_power_mux[0to52] 
+          param='sum_leakage_power_mux[0to51]+leakage_sb_mux[1][1]_rrnode[296]'
.meas tran sum_energy_per_cycle_mux[0to52] 
+          param='sum_energy_per_cycle_mux[0to51]+energy_per_cycle_sb_mux[1][1]_rrnode[296]'
***** Load for rr_node[296] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=5, type=4 *****
Xchan_mux_1level_tapbuf_size3[52]->out_loadlvl[0]_out mux_1level_tapbuf_size3[52]->out mux_1level_tapbuf_size3[52]->out_loadlvl[0]_out mux_1level_tapbuf_size3[52]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[138]_no0 mux_1level_tapbuf_size3[52]->out_loadlvl[0]_out mux_1level_tapbuf_size3[52]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[139]_no0 mux_1level_tapbuf_size3[52]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[52]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[140]_no0 mux_1level_tapbuf_size3[52]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[52]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[141]_no0 mux_1level_tapbuf_size3[52]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[52]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to52] 
+          param='sum_leakage_power_sb_mux[0to51]+leakage_sb_mux[1][1]_rrnode[296]'
.meas tran sum_energy_per_cycle_sb_mux[0to52] 
+          param='sum_energy_per_cycle_sb_mux[0to51]+energy_per_cycle_sb_mux[1][1]_rrnode[296]'
Xmux_1level_tapbuf_size3[53] mux_1level_tapbuf_size3[53]->in[0] mux_1level_tapbuf_size3[53]->in[1] mux_1level_tapbuf_size3[53]->in[2] mux_1level_tapbuf_size3[53]->out sram[69]->outb sram[69]->out sram[70]->out sram[70]->outb sram[71]->out sram[71]->outb gvdd_mux_1level_tapbuf_size3[53] 0 mux_1level_tapbuf_size3
***** SRAM bits for MUX[53], level=1, select_path_id=0. *****
*****100*****
Xsram[69] sram->in sram[69]->out sram[69]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[69]->out) 0
.nodeset V(sram[69]->outb) vsp
Xsram[70] sram->in sram[70]->out sram[70]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[70]->out) 0
.nodeset V(sram[70]->outb) vsp
Xsram[71] sram->in sram[71]->out sram[71]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[71]->out) 0
.nodeset V(sram[71]->outb) vsp
***** Signal mux_1level_tapbuf_size3[53]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[53]->in[0] mux_1level_tapbuf_size3[53]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size3[53]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[53]->in[1] mux_1level_tapbuf_size3[53]->in[1] 0 
+  0
***** Signal mux_1level_tapbuf_size3[53]->in[2] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[53]->in[2] mux_1level_tapbuf_size3[53]->in[2] 0 
+  0
Vgvdd_mux_1level_tapbuf_size3[53] gvdd_mux_1level_tapbuf_size3[53] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[298] trig v(mux_1level_tapbuf_size3[53]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[53]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[298] trig v(mux_1level_tapbuf_size3[53]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[53]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[298] when v(mux_1level_tapbuf_size3[53]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[298] trig v(mux_1level_tapbuf_size3[53]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[53]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[298] when v(mux_1level_tapbuf_size3[53]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[298] trig v(mux_1level_tapbuf_size3[53]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[53]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[53]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[53]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[298] param='mux_1level_tapbuf_size3[53]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[53]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[53]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size3[53]_energy_per_cycle param='mux_1level_tapbuf_size3[53]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[298]  param='mux_1level_tapbuf_size3[53]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[298]  param='dynamic_power_sb_mux[1][1]_rrnode[298]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[298] avg p(Vgvdd_mux_1level_tapbuf_size3[53]) from='start_rise_sb_mux[1][1]_rrnode[298]' to='start_rise_sb_mux[1][1]_rrnode[298]+switch_rise_sb_mux[1][1]_rrnode[298]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[298] avg p(Vgvdd_mux_1level_tapbuf_size3[53]) from='start_fall_sb_mux[1][1]_rrnode[298]' to='start_fall_sb_mux[1][1]_rrnode[298]+switch_fall_sb_mux[1][1]_rrnode[298]'
.meas tran sum_leakage_power_mux[0to53] 
+          param='sum_leakage_power_mux[0to52]+leakage_sb_mux[1][1]_rrnode[298]'
.meas tran sum_energy_per_cycle_mux[0to53] 
+          param='sum_energy_per_cycle_mux[0to52]+energy_per_cycle_sb_mux[1][1]_rrnode[298]'
***** Load for rr_node[298] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=7, type=4 *****
Xchan_mux_1level_tapbuf_size3[53]->out_loadlvl[0]_out mux_1level_tapbuf_size3[53]->out mux_1level_tapbuf_size3[53]->out_loadlvl[0]_out mux_1level_tapbuf_size3[53]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[142]_no0 mux_1level_tapbuf_size3[53]->out_loadlvl[0]_out mux_1level_tapbuf_size3[53]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[143]_no0 mux_1level_tapbuf_size3[53]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[53]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[144]_no0 mux_1level_tapbuf_size3[53]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[53]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[145]_no0 mux_1level_tapbuf_size3[53]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[53]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to53] 
+          param='sum_leakage_power_sb_mux[0to52]+leakage_sb_mux[1][1]_rrnode[298]'
.meas tran sum_energy_per_cycle_sb_mux[0to53] 
+          param='sum_energy_per_cycle_sb_mux[0to52]+energy_per_cycle_sb_mux[1][1]_rrnode[298]'
Xmux_1level_tapbuf_size3[54] mux_1level_tapbuf_size3[54]->in[0] mux_1level_tapbuf_size3[54]->in[1] mux_1level_tapbuf_size3[54]->in[2] mux_1level_tapbuf_size3[54]->out sram[72]->outb sram[72]->out sram[73]->out sram[73]->outb sram[74]->out sram[74]->outb gvdd_mux_1level_tapbuf_size3[54] 0 mux_1level_tapbuf_size3
***** SRAM bits for MUX[54], level=1, select_path_id=0. *****
*****100*****
Xsram[72] sram->in sram[72]->out sram[72]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[72]->out) 0
.nodeset V(sram[72]->outb) vsp
Xsram[73] sram->in sram[73]->out sram[73]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[73]->out) 0
.nodeset V(sram[73]->outb) vsp
Xsram[74] sram->in sram[74]->out sram[74]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[74]->out) 0
.nodeset V(sram[74]->outb) vsp
***** Signal mux_1level_tapbuf_size3[54]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[54]->in[0] mux_1level_tapbuf_size3[54]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size3[54]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[54]->in[1] mux_1level_tapbuf_size3[54]->in[1] 0 
+  0
***** Signal mux_1level_tapbuf_size3[54]->in[2] density = 0, probability=0.*****
Vmux_1level_tapbuf_size3[54]->in[2] mux_1level_tapbuf_size3[54]->in[2] 0 
+  0
Vgvdd_mux_1level_tapbuf_size3[54] gvdd_mux_1level_tapbuf_size3[54] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[300] trig v(mux_1level_tapbuf_size3[54]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[54]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[300] trig v(mux_1level_tapbuf_size3[54]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[54]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[300] when v(mux_1level_tapbuf_size3[54]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[300] trig v(mux_1level_tapbuf_size3[54]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[54]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[300] when v(mux_1level_tapbuf_size3[54]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[300] trig v(mux_1level_tapbuf_size3[54]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size3[54]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size3[54]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size3[54]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[300] param='mux_1level_tapbuf_size3[54]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size3[54]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size3[54]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size3[54]_energy_per_cycle param='mux_1level_tapbuf_size3[54]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[300]  param='mux_1level_tapbuf_size3[54]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[300]  param='dynamic_power_sb_mux[1][1]_rrnode[300]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[300] avg p(Vgvdd_mux_1level_tapbuf_size3[54]) from='start_rise_sb_mux[1][1]_rrnode[300]' to='start_rise_sb_mux[1][1]_rrnode[300]+switch_rise_sb_mux[1][1]_rrnode[300]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[300] avg p(Vgvdd_mux_1level_tapbuf_size3[54]) from='start_fall_sb_mux[1][1]_rrnode[300]' to='start_fall_sb_mux[1][1]_rrnode[300]+switch_fall_sb_mux[1][1]_rrnode[300]'
.meas tran sum_leakage_power_mux[0to54] 
+          param='sum_leakage_power_mux[0to53]+leakage_sb_mux[1][1]_rrnode[300]'
.meas tran sum_energy_per_cycle_mux[0to54] 
+          param='sum_energy_per_cycle_mux[0to53]+energy_per_cycle_sb_mux[1][1]_rrnode[300]'
***** Load for rr_node[300] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=9, type=4 *****
Xchan_mux_1level_tapbuf_size3[54]->out_loadlvl[0]_out mux_1level_tapbuf_size3[54]->out mux_1level_tapbuf_size3[54]->out_loadlvl[0]_out mux_1level_tapbuf_size3[54]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[146]_no0 mux_1level_tapbuf_size3[54]->out_loadlvl[0]_out mux_1level_tapbuf_size3[54]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[147]_no0 mux_1level_tapbuf_size3[54]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[54]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[148]_no0 mux_1level_tapbuf_size3[54]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[54]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[149]_no0 mux_1level_tapbuf_size3[54]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[54]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[150]_no0 mux_1level_tapbuf_size3[54]->out_loadlvl[0]_midout mux_1level_tapbuf_size3[54]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to54] 
+          param='sum_leakage_power_sb_mux[0to53]+leakage_sb_mux[1][1]_rrnode[300]'
.meas tran sum_energy_per_cycle_sb_mux[0to54] 
+          param='sum_energy_per_cycle_sb_mux[0to53]+energy_per_cycle_sb_mux[1][1]_rrnode[300]'
Xmux_1level_tapbuf_size2[55] mux_1level_tapbuf_size2[55]->in[0] mux_1level_tapbuf_size2[55]->in[1] mux_1level_tapbuf_size2[55]->out sram[75]->outb sram[75]->out gvdd_mux_1level_tapbuf_size2[55] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[55], level=1, select_path_id=0. *****
*****1*****
Xsram[75] sram->in sram[75]->out sram[75]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[75]->out) 0
.nodeset V(sram[75]->outb) vsp
***** Signal mux_1level_tapbuf_size2[55]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[55]->in[0] mux_1level_tapbuf_size2[55]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[55]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[55]->in[1] mux_1level_tapbuf_size2[55]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[55] gvdd_mux_1level_tapbuf_size2[55] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[302] trig v(mux_1level_tapbuf_size2[55]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[55]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[302] trig v(mux_1level_tapbuf_size2[55]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[55]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[302] when v(mux_1level_tapbuf_size2[55]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[302] trig v(mux_1level_tapbuf_size2[55]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[55]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[302] when v(mux_1level_tapbuf_size2[55]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[302] trig v(mux_1level_tapbuf_size2[55]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[55]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[55]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[55]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[302] param='mux_1level_tapbuf_size2[55]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[55]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[55]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[55]_energy_per_cycle param='mux_1level_tapbuf_size2[55]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[302]  param='mux_1level_tapbuf_size2[55]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[302]  param='dynamic_power_sb_mux[1][1]_rrnode[302]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[302] avg p(Vgvdd_mux_1level_tapbuf_size2[55]) from='start_rise_sb_mux[1][1]_rrnode[302]' to='start_rise_sb_mux[1][1]_rrnode[302]+switch_rise_sb_mux[1][1]_rrnode[302]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[302] avg p(Vgvdd_mux_1level_tapbuf_size2[55]) from='start_fall_sb_mux[1][1]_rrnode[302]' to='start_fall_sb_mux[1][1]_rrnode[302]+switch_fall_sb_mux[1][1]_rrnode[302]'
.meas tran sum_leakage_power_mux[0to55] 
+          param='sum_leakage_power_mux[0to54]+leakage_sb_mux[1][1]_rrnode[302]'
.meas tran sum_energy_per_cycle_mux[0to55] 
+          param='sum_energy_per_cycle_mux[0to54]+energy_per_cycle_sb_mux[1][1]_rrnode[302]'
***** Load for rr_node[302] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=11, type=4 *****
Xchan_mux_1level_tapbuf_size2[55]->out_loadlvl[0]_out mux_1level_tapbuf_size2[55]->out mux_1level_tapbuf_size2[55]->out_loadlvl[0]_out mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[151]_no0 mux_1level_tapbuf_size2[55]->out_loadlvl[0]_out mux_1level_tapbuf_size2[55]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[152]_no0 mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[153]_no0 mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[154]_no0 mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[55]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to55] 
+          param='sum_leakage_power_sb_mux[0to54]+leakage_sb_mux[1][1]_rrnode[302]'
.meas tran sum_energy_per_cycle_sb_mux[0to55] 
+          param='sum_energy_per_cycle_sb_mux[0to54]+energy_per_cycle_sb_mux[1][1]_rrnode[302]'
Xmux_1level_tapbuf_size2[56] mux_1level_tapbuf_size2[56]->in[0] mux_1level_tapbuf_size2[56]->in[1] mux_1level_tapbuf_size2[56]->out sram[76]->outb sram[76]->out gvdd_mux_1level_tapbuf_size2[56] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[56], level=1, select_path_id=0. *****
*****1*****
Xsram[76] sram->in sram[76]->out sram[76]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[76]->out) 0
.nodeset V(sram[76]->outb) vsp
***** Signal mux_1level_tapbuf_size2[56]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[56]->in[0] mux_1level_tapbuf_size2[56]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[56]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[56]->in[1] mux_1level_tapbuf_size2[56]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[56] gvdd_mux_1level_tapbuf_size2[56] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[304] trig v(mux_1level_tapbuf_size2[56]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[56]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[304] trig v(mux_1level_tapbuf_size2[56]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[56]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[304] when v(mux_1level_tapbuf_size2[56]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[304] trig v(mux_1level_tapbuf_size2[56]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[56]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[304] when v(mux_1level_tapbuf_size2[56]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[304] trig v(mux_1level_tapbuf_size2[56]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[56]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[56]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[56]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[304] param='mux_1level_tapbuf_size2[56]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[56]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[56]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[56]_energy_per_cycle param='mux_1level_tapbuf_size2[56]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[304]  param='mux_1level_tapbuf_size2[56]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[304]  param='dynamic_power_sb_mux[1][1]_rrnode[304]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[304] avg p(Vgvdd_mux_1level_tapbuf_size2[56]) from='start_rise_sb_mux[1][1]_rrnode[304]' to='start_rise_sb_mux[1][1]_rrnode[304]+switch_rise_sb_mux[1][1]_rrnode[304]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[304] avg p(Vgvdd_mux_1level_tapbuf_size2[56]) from='start_fall_sb_mux[1][1]_rrnode[304]' to='start_fall_sb_mux[1][1]_rrnode[304]+switch_fall_sb_mux[1][1]_rrnode[304]'
.meas tran sum_leakage_power_mux[0to56] 
+          param='sum_leakage_power_mux[0to55]+leakage_sb_mux[1][1]_rrnode[304]'
.meas tran sum_energy_per_cycle_mux[0to56] 
+          param='sum_energy_per_cycle_mux[0to55]+energy_per_cycle_sb_mux[1][1]_rrnode[304]'
***** Load for rr_node[304] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=13, type=4 *****
Xchan_mux_1level_tapbuf_size2[56]->out_loadlvl[0]_out mux_1level_tapbuf_size2[56]->out mux_1level_tapbuf_size2[56]->out_loadlvl[0]_out mux_1level_tapbuf_size2[56]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[155]_no0 mux_1level_tapbuf_size2[56]->out_loadlvl[0]_out mux_1level_tapbuf_size2[56]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[156]_no0 mux_1level_tapbuf_size2[56]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[56]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[157]_no0 mux_1level_tapbuf_size2[56]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[56]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to56] 
+          param='sum_leakage_power_sb_mux[0to55]+leakage_sb_mux[1][1]_rrnode[304]'
.meas tran sum_energy_per_cycle_sb_mux[0to56] 
+          param='sum_energy_per_cycle_sb_mux[0to55]+energy_per_cycle_sb_mux[1][1]_rrnode[304]'
Xmux_1level_tapbuf_size2[57] mux_1level_tapbuf_size2[57]->in[0] mux_1level_tapbuf_size2[57]->in[1] mux_1level_tapbuf_size2[57]->out sram[77]->outb sram[77]->out gvdd_mux_1level_tapbuf_size2[57] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[57], level=1, select_path_id=0. *****
*****1*****
Xsram[77] sram->in sram[77]->out sram[77]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[77]->out) 0
.nodeset V(sram[77]->outb) vsp
***** Signal mux_1level_tapbuf_size2[57]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[57]->in[0] mux_1level_tapbuf_size2[57]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[57]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[57]->in[1] mux_1level_tapbuf_size2[57]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[57] gvdd_mux_1level_tapbuf_size2[57] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[306] trig v(mux_1level_tapbuf_size2[57]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[57]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[306] trig v(mux_1level_tapbuf_size2[57]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[57]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[306] when v(mux_1level_tapbuf_size2[57]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[306] trig v(mux_1level_tapbuf_size2[57]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[57]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[306] when v(mux_1level_tapbuf_size2[57]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[306] trig v(mux_1level_tapbuf_size2[57]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[57]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[57]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[57]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[306] param='mux_1level_tapbuf_size2[57]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[57]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[57]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[57]_energy_per_cycle param='mux_1level_tapbuf_size2[57]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[306]  param='mux_1level_tapbuf_size2[57]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[306]  param='dynamic_power_sb_mux[1][1]_rrnode[306]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[306] avg p(Vgvdd_mux_1level_tapbuf_size2[57]) from='start_rise_sb_mux[1][1]_rrnode[306]' to='start_rise_sb_mux[1][1]_rrnode[306]+switch_rise_sb_mux[1][1]_rrnode[306]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[306] avg p(Vgvdd_mux_1level_tapbuf_size2[57]) from='start_fall_sb_mux[1][1]_rrnode[306]' to='start_fall_sb_mux[1][1]_rrnode[306]+switch_fall_sb_mux[1][1]_rrnode[306]'
.meas tran sum_leakage_power_mux[0to57] 
+          param='sum_leakage_power_mux[0to56]+leakage_sb_mux[1][1]_rrnode[306]'
.meas tran sum_energy_per_cycle_mux[0to57] 
+          param='sum_energy_per_cycle_mux[0to56]+energy_per_cycle_sb_mux[1][1]_rrnode[306]'
***** Load for rr_node[306] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=15, type=4 *****
Xchan_mux_1level_tapbuf_size2[57]->out_loadlvl[0]_out mux_1level_tapbuf_size2[57]->out mux_1level_tapbuf_size2[57]->out_loadlvl[0]_out mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[158]_no0 mux_1level_tapbuf_size2[57]->out_loadlvl[0]_out mux_1level_tapbuf_size2[57]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[159]_no0 mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[160]_no0 mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[57]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to57] 
+          param='sum_leakage_power_sb_mux[0to56]+leakage_sb_mux[1][1]_rrnode[306]'
.meas tran sum_energy_per_cycle_sb_mux[0to57] 
+          param='sum_energy_per_cycle_sb_mux[0to56]+energy_per_cycle_sb_mux[1][1]_rrnode[306]'
Xmux_1level_tapbuf_size2[58] mux_1level_tapbuf_size2[58]->in[0] mux_1level_tapbuf_size2[58]->in[1] mux_1level_tapbuf_size2[58]->out sram[78]->outb sram[78]->out gvdd_mux_1level_tapbuf_size2[58] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[58], level=1, select_path_id=0. *****
*****1*****
Xsram[78] sram->in sram[78]->out sram[78]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[78]->out) 0
.nodeset V(sram[78]->outb) vsp
***** Signal mux_1level_tapbuf_size2[58]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[58]->in[0] mux_1level_tapbuf_size2[58]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[58]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[58]->in[1] mux_1level_tapbuf_size2[58]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[58] gvdd_mux_1level_tapbuf_size2[58] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[308] trig v(mux_1level_tapbuf_size2[58]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[58]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[308] trig v(mux_1level_tapbuf_size2[58]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[58]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[308] when v(mux_1level_tapbuf_size2[58]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[308] trig v(mux_1level_tapbuf_size2[58]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[58]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[308] when v(mux_1level_tapbuf_size2[58]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[308] trig v(mux_1level_tapbuf_size2[58]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[58]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[58]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[58]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[308] param='mux_1level_tapbuf_size2[58]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[58]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[58]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[58]_energy_per_cycle param='mux_1level_tapbuf_size2[58]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[308]  param='mux_1level_tapbuf_size2[58]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[308]  param='dynamic_power_sb_mux[1][1]_rrnode[308]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[308] avg p(Vgvdd_mux_1level_tapbuf_size2[58]) from='start_rise_sb_mux[1][1]_rrnode[308]' to='start_rise_sb_mux[1][1]_rrnode[308]+switch_rise_sb_mux[1][1]_rrnode[308]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[308] avg p(Vgvdd_mux_1level_tapbuf_size2[58]) from='start_fall_sb_mux[1][1]_rrnode[308]' to='start_fall_sb_mux[1][1]_rrnode[308]+switch_fall_sb_mux[1][1]_rrnode[308]'
.meas tran sum_leakage_power_mux[0to58] 
+          param='sum_leakage_power_mux[0to57]+leakage_sb_mux[1][1]_rrnode[308]'
.meas tran sum_energy_per_cycle_mux[0to58] 
+          param='sum_energy_per_cycle_mux[0to57]+energy_per_cycle_sb_mux[1][1]_rrnode[308]'
***** Load for rr_node[308] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=17, type=4 *****
Xchan_mux_1level_tapbuf_size2[58]->out_loadlvl[0]_out mux_1level_tapbuf_size2[58]->out mux_1level_tapbuf_size2[58]->out_loadlvl[0]_out mux_1level_tapbuf_size2[58]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[161]_no0 mux_1level_tapbuf_size2[58]->out_loadlvl[0]_out mux_1level_tapbuf_size2[58]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[162]_no0 mux_1level_tapbuf_size2[58]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[58]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[163]_no0 mux_1level_tapbuf_size2[58]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[58]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to58] 
+          param='sum_leakage_power_sb_mux[0to57]+leakage_sb_mux[1][1]_rrnode[308]'
.meas tran sum_energy_per_cycle_sb_mux[0to58] 
+          param='sum_energy_per_cycle_sb_mux[0to57]+energy_per_cycle_sb_mux[1][1]_rrnode[308]'
Xmux_1level_tapbuf_size2[59] mux_1level_tapbuf_size2[59]->in[0] mux_1level_tapbuf_size2[59]->in[1] mux_1level_tapbuf_size2[59]->out sram[79]->outb sram[79]->out gvdd_mux_1level_tapbuf_size2[59] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[59], level=1, select_path_id=0. *****
*****1*****
Xsram[79] sram->in sram[79]->out sram[79]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[79]->out) 0
.nodeset V(sram[79]->outb) vsp
***** Signal mux_1level_tapbuf_size2[59]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[59]->in[0] mux_1level_tapbuf_size2[59]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[59]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[59]->in[1] mux_1level_tapbuf_size2[59]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[59] gvdd_mux_1level_tapbuf_size2[59] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[310] trig v(mux_1level_tapbuf_size2[59]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[59]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[310] trig v(mux_1level_tapbuf_size2[59]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[59]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[310] when v(mux_1level_tapbuf_size2[59]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[310] trig v(mux_1level_tapbuf_size2[59]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[59]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[310] when v(mux_1level_tapbuf_size2[59]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[310] trig v(mux_1level_tapbuf_size2[59]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[59]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[59]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[59]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[310] param='mux_1level_tapbuf_size2[59]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[59]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[59]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[59]_energy_per_cycle param='mux_1level_tapbuf_size2[59]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[310]  param='mux_1level_tapbuf_size2[59]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[310]  param='dynamic_power_sb_mux[1][1]_rrnode[310]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[310] avg p(Vgvdd_mux_1level_tapbuf_size2[59]) from='start_rise_sb_mux[1][1]_rrnode[310]' to='start_rise_sb_mux[1][1]_rrnode[310]+switch_rise_sb_mux[1][1]_rrnode[310]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[310] avg p(Vgvdd_mux_1level_tapbuf_size2[59]) from='start_fall_sb_mux[1][1]_rrnode[310]' to='start_fall_sb_mux[1][1]_rrnode[310]+switch_fall_sb_mux[1][1]_rrnode[310]'
.meas tran sum_leakage_power_mux[0to59] 
+          param='sum_leakage_power_mux[0to58]+leakage_sb_mux[1][1]_rrnode[310]'
.meas tran sum_energy_per_cycle_mux[0to59] 
+          param='sum_energy_per_cycle_mux[0to58]+energy_per_cycle_sb_mux[1][1]_rrnode[310]'
***** Load for rr_node[310] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=19, type=4 *****
Xchan_mux_1level_tapbuf_size2[59]->out_loadlvl[0]_out mux_1level_tapbuf_size2[59]->out mux_1level_tapbuf_size2[59]->out_loadlvl[0]_out mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[164]_no0 mux_1level_tapbuf_size2[59]->out_loadlvl[0]_out mux_1level_tapbuf_size2[59]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[165]_no0 mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[166]_no0 mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[167]_no0 mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[59]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to59] 
+          param='sum_leakage_power_sb_mux[0to58]+leakage_sb_mux[1][1]_rrnode[310]'
.meas tran sum_energy_per_cycle_sb_mux[0to59] 
+          param='sum_energy_per_cycle_sb_mux[0to58]+energy_per_cycle_sb_mux[1][1]_rrnode[310]'
Xmux_1level_tapbuf_size2[60] mux_1level_tapbuf_size2[60]->in[0] mux_1level_tapbuf_size2[60]->in[1] mux_1level_tapbuf_size2[60]->out sram[80]->outb sram[80]->out gvdd_mux_1level_tapbuf_size2[60] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[60], level=1, select_path_id=0. *****
*****1*****
Xsram[80] sram->in sram[80]->out sram[80]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[80]->out) 0
.nodeset V(sram[80]->outb) vsp
***** Signal mux_1level_tapbuf_size2[60]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[60]->in[0] mux_1level_tapbuf_size2[60]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[60]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[60]->in[1] mux_1level_tapbuf_size2[60]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[60] gvdd_mux_1level_tapbuf_size2[60] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[312] trig v(mux_1level_tapbuf_size2[60]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[60]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[312] trig v(mux_1level_tapbuf_size2[60]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[60]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[312] when v(mux_1level_tapbuf_size2[60]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[312] trig v(mux_1level_tapbuf_size2[60]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[60]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[312] when v(mux_1level_tapbuf_size2[60]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[312] trig v(mux_1level_tapbuf_size2[60]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[60]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[60]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[60]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[312] param='mux_1level_tapbuf_size2[60]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[60]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[60]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[60]_energy_per_cycle param='mux_1level_tapbuf_size2[60]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[312]  param='mux_1level_tapbuf_size2[60]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[312]  param='dynamic_power_sb_mux[1][1]_rrnode[312]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[312] avg p(Vgvdd_mux_1level_tapbuf_size2[60]) from='start_rise_sb_mux[1][1]_rrnode[312]' to='start_rise_sb_mux[1][1]_rrnode[312]+switch_rise_sb_mux[1][1]_rrnode[312]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[312] avg p(Vgvdd_mux_1level_tapbuf_size2[60]) from='start_fall_sb_mux[1][1]_rrnode[312]' to='start_fall_sb_mux[1][1]_rrnode[312]+switch_fall_sb_mux[1][1]_rrnode[312]'
.meas tran sum_leakage_power_mux[0to60] 
+          param='sum_leakage_power_mux[0to59]+leakage_sb_mux[1][1]_rrnode[312]'
.meas tran sum_energy_per_cycle_mux[0to60] 
+          param='sum_energy_per_cycle_mux[0to59]+energy_per_cycle_sb_mux[1][1]_rrnode[312]'
***** Load for rr_node[312] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=21, type=4 *****
Xchan_mux_1level_tapbuf_size2[60]->out_loadlvl[0]_out mux_1level_tapbuf_size2[60]->out mux_1level_tapbuf_size2[60]->out_loadlvl[0]_out mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[168]_no0 mux_1level_tapbuf_size2[60]->out_loadlvl[0]_out mux_1level_tapbuf_size2[60]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[169]_no0 mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[170]_no0 mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[171]_no0 mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[60]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to60] 
+          param='sum_leakage_power_sb_mux[0to59]+leakage_sb_mux[1][1]_rrnode[312]'
.meas tran sum_energy_per_cycle_sb_mux[0to60] 
+          param='sum_energy_per_cycle_sb_mux[0to59]+energy_per_cycle_sb_mux[1][1]_rrnode[312]'
Xmux_1level_tapbuf_size2[61] mux_1level_tapbuf_size2[61]->in[0] mux_1level_tapbuf_size2[61]->in[1] mux_1level_tapbuf_size2[61]->out sram[81]->outb sram[81]->out gvdd_mux_1level_tapbuf_size2[61] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[61], level=1, select_path_id=0. *****
*****1*****
Xsram[81] sram->in sram[81]->out sram[81]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[81]->out) 0
.nodeset V(sram[81]->outb) vsp
***** Signal mux_1level_tapbuf_size2[61]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[61]->in[0] mux_1level_tapbuf_size2[61]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[61]->in[1] density = 0.2026, probability=0.4982.*****
Vmux_1level_tapbuf_size2[61]->in[1] mux_1level_tapbuf_size2[61]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgvdd_mux_1level_tapbuf_size2[61] gvdd_mux_1level_tapbuf_size2[61] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[314] trig v(mux_1level_tapbuf_size2[61]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[61]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[314] trig v(mux_1level_tapbuf_size2[61]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[61]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[314] when v(mux_1level_tapbuf_size2[61]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[314] trig v(mux_1level_tapbuf_size2[61]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[61]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[314] when v(mux_1level_tapbuf_size2[61]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[314] trig v(mux_1level_tapbuf_size2[61]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[61]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[61]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[61]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[314] param='mux_1level_tapbuf_size2[61]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[61]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[61]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[61]_energy_per_cycle param='mux_1level_tapbuf_size2[61]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[314]  param='mux_1level_tapbuf_size2[61]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[314]  param='dynamic_power_sb_mux[1][1]_rrnode[314]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[314] avg p(Vgvdd_mux_1level_tapbuf_size2[61]) from='start_rise_sb_mux[1][1]_rrnode[314]' to='start_rise_sb_mux[1][1]_rrnode[314]+switch_rise_sb_mux[1][1]_rrnode[314]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[314] avg p(Vgvdd_mux_1level_tapbuf_size2[61]) from='start_fall_sb_mux[1][1]_rrnode[314]' to='start_fall_sb_mux[1][1]_rrnode[314]+switch_fall_sb_mux[1][1]_rrnode[314]'
.meas tran sum_leakage_power_mux[0to61] 
+          param='sum_leakage_power_mux[0to60]+leakage_sb_mux[1][1]_rrnode[314]'
.meas tran sum_energy_per_cycle_mux[0to61] 
+          param='sum_energy_per_cycle_mux[0to60]+energy_per_cycle_sb_mux[1][1]_rrnode[314]'
***** Load for rr_node[314] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=23, type=4 *****
Xchan_mux_1level_tapbuf_size2[61]->out_loadlvl[0]_out mux_1level_tapbuf_size2[61]->out mux_1level_tapbuf_size2[61]->out_loadlvl[0]_out mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[172]_no0 mux_1level_tapbuf_size2[61]->out_loadlvl[0]_out mux_1level_tapbuf_size2[61]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[173]_no0 mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[174]_no0 mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[175]_no0 mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[176]_no0 mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[61]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to61] 
+          param='sum_leakage_power_sb_mux[0to60]+leakage_sb_mux[1][1]_rrnode[314]'
.meas tran sum_energy_per_cycle_sb_mux[0to61] 
+          param='sum_energy_per_cycle_sb_mux[0to60]+energy_per_cycle_sb_mux[1][1]_rrnode[314]'
Xmux_1level_tapbuf_size2[62] mux_1level_tapbuf_size2[62]->in[0] mux_1level_tapbuf_size2[62]->in[1] mux_1level_tapbuf_size2[62]->out sram[82]->outb sram[82]->out gvdd_mux_1level_tapbuf_size2[62] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[62], level=1, select_path_id=0. *****
*****1*****
Xsram[82] sram->in sram[82]->out sram[82]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[82]->out) 0
.nodeset V(sram[82]->outb) vsp
***** Signal mux_1level_tapbuf_size2[62]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[62]->in[0] mux_1level_tapbuf_size2[62]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[62]->in[1] density = 0.2026, probability=0.4982.*****
Vmux_1level_tapbuf_size2[62]->in[1] mux_1level_tapbuf_size2[62]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgvdd_mux_1level_tapbuf_size2[62] gvdd_mux_1level_tapbuf_size2[62] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[316] trig v(mux_1level_tapbuf_size2[62]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[62]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[316] trig v(mux_1level_tapbuf_size2[62]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[62]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[316] when v(mux_1level_tapbuf_size2[62]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[316] trig v(mux_1level_tapbuf_size2[62]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[62]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[316] when v(mux_1level_tapbuf_size2[62]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[316] trig v(mux_1level_tapbuf_size2[62]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[62]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[62]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[62]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[316] param='mux_1level_tapbuf_size2[62]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[62]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[62]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[62]_energy_per_cycle param='mux_1level_tapbuf_size2[62]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[316]  param='mux_1level_tapbuf_size2[62]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[316]  param='dynamic_power_sb_mux[1][1]_rrnode[316]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[316] avg p(Vgvdd_mux_1level_tapbuf_size2[62]) from='start_rise_sb_mux[1][1]_rrnode[316]' to='start_rise_sb_mux[1][1]_rrnode[316]+switch_rise_sb_mux[1][1]_rrnode[316]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[316] avg p(Vgvdd_mux_1level_tapbuf_size2[62]) from='start_fall_sb_mux[1][1]_rrnode[316]' to='start_fall_sb_mux[1][1]_rrnode[316]+switch_fall_sb_mux[1][1]_rrnode[316]'
.meas tran sum_leakage_power_mux[0to62] 
+          param='sum_leakage_power_mux[0to61]+leakage_sb_mux[1][1]_rrnode[316]'
.meas tran sum_energy_per_cycle_mux[0to62] 
+          param='sum_energy_per_cycle_mux[0to61]+energy_per_cycle_sb_mux[1][1]_rrnode[316]'
***** Load for rr_node[316] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=25, type=4 *****
Xchan_mux_1level_tapbuf_size2[62]->out_loadlvl[0]_out mux_1level_tapbuf_size2[62]->out mux_1level_tapbuf_size2[62]->out_loadlvl[0]_out mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[177]_no0 mux_1level_tapbuf_size2[62]->out_loadlvl[0]_out mux_1level_tapbuf_size2[62]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[178]_no0 mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[179]_no0 mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[180]_no0 mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[62]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to62] 
+          param='sum_leakage_power_sb_mux[0to61]+leakage_sb_mux[1][1]_rrnode[316]'
.meas tran sum_energy_per_cycle_sb_mux[0to62] 
+          param='sum_energy_per_cycle_sb_mux[0to61]+energy_per_cycle_sb_mux[1][1]_rrnode[316]'
Xmux_1level_tapbuf_size2[63] mux_1level_tapbuf_size2[63]->in[0] mux_1level_tapbuf_size2[63]->in[1] mux_1level_tapbuf_size2[63]->out sram[83]->outb sram[83]->out gvdd_mux_1level_tapbuf_size2[63] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[63], level=1, select_path_id=0. *****
*****1*****
Xsram[83] sram->in sram[83]->out sram[83]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[83]->out) 0
.nodeset V(sram[83]->outb) vsp
***** Signal mux_1level_tapbuf_size2[63]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[63]->in[0] mux_1level_tapbuf_size2[63]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[63]->in[1] density = 0.2026, probability=0.4982.*****
Vmux_1level_tapbuf_size2[63]->in[1] mux_1level_tapbuf_size2[63]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgvdd_mux_1level_tapbuf_size2[63] gvdd_mux_1level_tapbuf_size2[63] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[318] trig v(mux_1level_tapbuf_size2[63]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[63]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[318] trig v(mux_1level_tapbuf_size2[63]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[63]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[318] when v(mux_1level_tapbuf_size2[63]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[318] trig v(mux_1level_tapbuf_size2[63]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[63]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[318] when v(mux_1level_tapbuf_size2[63]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[318] trig v(mux_1level_tapbuf_size2[63]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[63]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[63]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[63]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[318] param='mux_1level_tapbuf_size2[63]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[63]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[63]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[63]_energy_per_cycle param='mux_1level_tapbuf_size2[63]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[318]  param='mux_1level_tapbuf_size2[63]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[318]  param='dynamic_power_sb_mux[1][1]_rrnode[318]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[318] avg p(Vgvdd_mux_1level_tapbuf_size2[63]) from='start_rise_sb_mux[1][1]_rrnode[318]' to='start_rise_sb_mux[1][1]_rrnode[318]+switch_rise_sb_mux[1][1]_rrnode[318]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[318] avg p(Vgvdd_mux_1level_tapbuf_size2[63]) from='start_fall_sb_mux[1][1]_rrnode[318]' to='start_fall_sb_mux[1][1]_rrnode[318]+switch_fall_sb_mux[1][1]_rrnode[318]'
.meas tran sum_leakage_power_mux[0to63] 
+          param='sum_leakage_power_mux[0to62]+leakage_sb_mux[1][1]_rrnode[318]'
.meas tran sum_energy_per_cycle_mux[0to63] 
+          param='sum_energy_per_cycle_mux[0to62]+energy_per_cycle_sb_mux[1][1]_rrnode[318]'
***** Load for rr_node[318] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=27, type=4 *****
Xchan_mux_1level_tapbuf_size2[63]->out_loadlvl[0]_out mux_1level_tapbuf_size2[63]->out mux_1level_tapbuf_size2[63]->out_loadlvl[0]_out mux_1level_tapbuf_size2[63]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[181]_no0 mux_1level_tapbuf_size2[63]->out_loadlvl[0]_out mux_1level_tapbuf_size2[63]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[182]_no0 mux_1level_tapbuf_size2[63]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[63]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[183]_no0 mux_1level_tapbuf_size2[63]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[63]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to63] 
+          param='sum_leakage_power_sb_mux[0to62]+leakage_sb_mux[1][1]_rrnode[318]'
.meas tran sum_energy_per_cycle_sb_mux[0to63] 
+          param='sum_energy_per_cycle_sb_mux[0to62]+energy_per_cycle_sb_mux[1][1]_rrnode[318]'
Xmux_1level_tapbuf_size2[64] mux_1level_tapbuf_size2[64]->in[0] mux_1level_tapbuf_size2[64]->in[1] mux_1level_tapbuf_size2[64]->out sram[84]->outb sram[84]->out gvdd_mux_1level_tapbuf_size2[64] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[64], level=1, select_path_id=0. *****
*****1*****
Xsram[84] sram->in sram[84]->out sram[84]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[84]->out) 0
.nodeset V(sram[84]->outb) vsp
***** Signal mux_1level_tapbuf_size2[64]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[64]->in[0] mux_1level_tapbuf_size2[64]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[64]->in[1] density = 0.2026, probability=0.4982.*****
Vmux_1level_tapbuf_size2[64]->in[1] mux_1level_tapbuf_size2[64]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgvdd_mux_1level_tapbuf_size2[64] gvdd_mux_1level_tapbuf_size2[64] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[320] trig v(mux_1level_tapbuf_size2[64]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[64]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[320] trig v(mux_1level_tapbuf_size2[64]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[64]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[320] when v(mux_1level_tapbuf_size2[64]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[320] trig v(mux_1level_tapbuf_size2[64]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[64]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[320] when v(mux_1level_tapbuf_size2[64]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[320] trig v(mux_1level_tapbuf_size2[64]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[64]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[64]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[64]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[320] param='mux_1level_tapbuf_size2[64]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[64]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[64]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[64]_energy_per_cycle param='mux_1level_tapbuf_size2[64]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[320]  param='mux_1level_tapbuf_size2[64]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[320]  param='dynamic_power_sb_mux[1][1]_rrnode[320]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[320] avg p(Vgvdd_mux_1level_tapbuf_size2[64]) from='start_rise_sb_mux[1][1]_rrnode[320]' to='start_rise_sb_mux[1][1]_rrnode[320]+switch_rise_sb_mux[1][1]_rrnode[320]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[320] avg p(Vgvdd_mux_1level_tapbuf_size2[64]) from='start_fall_sb_mux[1][1]_rrnode[320]' to='start_fall_sb_mux[1][1]_rrnode[320]+switch_fall_sb_mux[1][1]_rrnode[320]'
.meas tran sum_leakage_power_mux[0to64] 
+          param='sum_leakage_power_mux[0to63]+leakage_sb_mux[1][1]_rrnode[320]'
.meas tran sum_energy_per_cycle_mux[0to64] 
+          param='sum_energy_per_cycle_mux[0to63]+energy_per_cycle_sb_mux[1][1]_rrnode[320]'
***** Load for rr_node[320] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=29, type=4 *****
Xchan_mux_1level_tapbuf_size2[64]->out_loadlvl[0]_out mux_1level_tapbuf_size2[64]->out mux_1level_tapbuf_size2[64]->out_loadlvl[0]_out mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[184]_no0 mux_1level_tapbuf_size2[64]->out_loadlvl[0]_out mux_1level_tapbuf_size2[64]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[185]_no0 mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[186]_no0 mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[187]_no0 mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[64]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to64] 
+          param='sum_leakage_power_sb_mux[0to63]+leakage_sb_mux[1][1]_rrnode[320]'
.meas tran sum_energy_per_cycle_sb_mux[0to64] 
+          param='sum_energy_per_cycle_sb_mux[0to63]+energy_per_cycle_sb_mux[1][1]_rrnode[320]'
Xmux_1level_tapbuf_size2[65] mux_1level_tapbuf_size2[65]->in[0] mux_1level_tapbuf_size2[65]->in[1] mux_1level_tapbuf_size2[65]->out sram[85]->outb sram[85]->out gvdd_mux_1level_tapbuf_size2[65] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[65], level=1, select_path_id=0. *****
*****1*****
Xsram[85] sram->in sram[85]->out sram[85]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[85]->out) 0
.nodeset V(sram[85]->outb) vsp
***** Signal mux_1level_tapbuf_size2[65]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[65]->in[0] mux_1level_tapbuf_size2[65]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[65]->in[1] density = 0.2026, probability=0.4982.*****
Vmux_1level_tapbuf_size2[65]->in[1] mux_1level_tapbuf_size2[65]->in[1] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
Vgvdd_mux_1level_tapbuf_size2[65] gvdd_mux_1level_tapbuf_size2[65] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[322] trig v(mux_1level_tapbuf_size2[65]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[65]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[322] trig v(mux_1level_tapbuf_size2[65]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[65]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[322] when v(mux_1level_tapbuf_size2[65]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[322] trig v(mux_1level_tapbuf_size2[65]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[65]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[322] when v(mux_1level_tapbuf_size2[65]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[322] trig v(mux_1level_tapbuf_size2[65]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[65]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[65]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[65]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[322] param='mux_1level_tapbuf_size2[65]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[65]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[65]) from='clock_period' to='6*clock_period'
.meas tran mux_1level_tapbuf_size2[65]_energy_per_cycle param='mux_1level_tapbuf_size2[65]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[322]  param='mux_1level_tapbuf_size2[65]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[322]  param='dynamic_power_sb_mux[1][1]_rrnode[322]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[322] avg p(Vgvdd_mux_1level_tapbuf_size2[65]) from='start_rise_sb_mux[1][1]_rrnode[322]' to='start_rise_sb_mux[1][1]_rrnode[322]+switch_rise_sb_mux[1][1]_rrnode[322]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[322] avg p(Vgvdd_mux_1level_tapbuf_size2[65]) from='start_fall_sb_mux[1][1]_rrnode[322]' to='start_fall_sb_mux[1][1]_rrnode[322]+switch_fall_sb_mux[1][1]_rrnode[322]'
.meas tran sum_leakage_power_mux[0to65] 
+          param='sum_leakage_power_mux[0to64]+leakage_sb_mux[1][1]_rrnode[322]'
.meas tran sum_energy_per_cycle_mux[0to65] 
+          param='sum_energy_per_cycle_mux[0to64]+energy_per_cycle_sb_mux[1][1]_rrnode[322]'
***** Load for rr_node[322] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=31, type=4 *****
Xchan_mux_1level_tapbuf_size2[65]->out_loadlvl[0]_out mux_1level_tapbuf_size2[65]->out mux_1level_tapbuf_size2[65]->out_loadlvl[0]_out mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[188]_no0 mux_1level_tapbuf_size2[65]->out_loadlvl[0]_out mux_1level_tapbuf_size2[65]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[189]_no0 mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[190]_no0 mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[191]_no0 mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[65]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to65] 
+          param='sum_leakage_power_sb_mux[0to64]+leakage_sb_mux[1][1]_rrnode[322]'
.meas tran sum_energy_per_cycle_sb_mux[0to65] 
+          param='sum_energy_per_cycle_sb_mux[0to64]+energy_per_cycle_sb_mux[1][1]_rrnode[322]'
Xmux_1level_tapbuf_size2[66] mux_1level_tapbuf_size2[66]->in[0] mux_1level_tapbuf_size2[66]->in[1] mux_1level_tapbuf_size2[66]->out sram[86]->outb sram[86]->out gvdd_mux_1level_tapbuf_size2[66] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[66], level=1, select_path_id=0. *****
*****1*****
Xsram[86] sram->in sram[86]->out sram[86]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[86]->out) 0
.nodeset V(sram[86]->outb) vsp
***** Signal mux_1level_tapbuf_size2[66]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[66]->in[0] mux_1level_tapbuf_size2[66]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[66]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[66]->in[1] mux_1level_tapbuf_size2[66]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[66] gvdd_mux_1level_tapbuf_size2[66] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[324] trig v(mux_1level_tapbuf_size2[66]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[66]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[324] trig v(mux_1level_tapbuf_size2[66]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[66]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[324] when v(mux_1level_tapbuf_size2[66]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[324] trig v(mux_1level_tapbuf_size2[66]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[66]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[324] when v(mux_1level_tapbuf_size2[66]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[324] trig v(mux_1level_tapbuf_size2[66]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[66]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[66]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[66]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[324] param='mux_1level_tapbuf_size2[66]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[66]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[66]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[66]_energy_per_cycle param='mux_1level_tapbuf_size2[66]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[324]  param='mux_1level_tapbuf_size2[66]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[324]  param='dynamic_power_sb_mux[1][1]_rrnode[324]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[324] avg p(Vgvdd_mux_1level_tapbuf_size2[66]) from='start_rise_sb_mux[1][1]_rrnode[324]' to='start_rise_sb_mux[1][1]_rrnode[324]+switch_rise_sb_mux[1][1]_rrnode[324]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[324] avg p(Vgvdd_mux_1level_tapbuf_size2[66]) from='start_fall_sb_mux[1][1]_rrnode[324]' to='start_fall_sb_mux[1][1]_rrnode[324]+switch_fall_sb_mux[1][1]_rrnode[324]'
.meas tran sum_leakage_power_mux[0to66] 
+          param='sum_leakage_power_mux[0to65]+leakage_sb_mux[1][1]_rrnode[324]'
.meas tran sum_energy_per_cycle_mux[0to66] 
+          param='sum_energy_per_cycle_mux[0to65]+energy_per_cycle_sb_mux[1][1]_rrnode[324]'
***** Load for rr_node[324] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=33, type=4 *****
Xchan_mux_1level_tapbuf_size2[66]->out_loadlvl[0]_out mux_1level_tapbuf_size2[66]->out mux_1level_tapbuf_size2[66]->out_loadlvl[0]_out mux_1level_tapbuf_size2[66]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[192]_no0 mux_1level_tapbuf_size2[66]->out_loadlvl[0]_out mux_1level_tapbuf_size2[66]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[193]_no0 mux_1level_tapbuf_size2[66]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[66]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[194]_no0 mux_1level_tapbuf_size2[66]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[66]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to66] 
+          param='sum_leakage_power_sb_mux[0to65]+leakage_sb_mux[1][1]_rrnode[324]'
.meas tran sum_energy_per_cycle_sb_mux[0to66] 
+          param='sum_energy_per_cycle_sb_mux[0to65]+energy_per_cycle_sb_mux[1][1]_rrnode[324]'
Xmux_1level_tapbuf_size2[67] mux_1level_tapbuf_size2[67]->in[0] mux_1level_tapbuf_size2[67]->in[1] mux_1level_tapbuf_size2[67]->out sram[87]->outb sram[87]->out gvdd_mux_1level_tapbuf_size2[67] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[67], level=1, select_path_id=0. *****
*****1*****
Xsram[87] sram->in sram[87]->out sram[87]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[87]->out) 0
.nodeset V(sram[87]->outb) vsp
***** Signal mux_1level_tapbuf_size2[67]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[67]->in[0] mux_1level_tapbuf_size2[67]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[67]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[67]->in[1] mux_1level_tapbuf_size2[67]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[67] gvdd_mux_1level_tapbuf_size2[67] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[326] trig v(mux_1level_tapbuf_size2[67]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[67]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[326] trig v(mux_1level_tapbuf_size2[67]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[67]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[326] when v(mux_1level_tapbuf_size2[67]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[326] trig v(mux_1level_tapbuf_size2[67]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[67]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[326] when v(mux_1level_tapbuf_size2[67]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[326] trig v(mux_1level_tapbuf_size2[67]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[67]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[67]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[67]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[326] param='mux_1level_tapbuf_size2[67]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[67]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[67]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[67]_energy_per_cycle param='mux_1level_tapbuf_size2[67]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[326]  param='mux_1level_tapbuf_size2[67]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[326]  param='dynamic_power_sb_mux[1][1]_rrnode[326]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[326] avg p(Vgvdd_mux_1level_tapbuf_size2[67]) from='start_rise_sb_mux[1][1]_rrnode[326]' to='start_rise_sb_mux[1][1]_rrnode[326]+switch_rise_sb_mux[1][1]_rrnode[326]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[326] avg p(Vgvdd_mux_1level_tapbuf_size2[67]) from='start_fall_sb_mux[1][1]_rrnode[326]' to='start_fall_sb_mux[1][1]_rrnode[326]+switch_fall_sb_mux[1][1]_rrnode[326]'
.meas tran sum_leakage_power_mux[0to67] 
+          param='sum_leakage_power_mux[0to66]+leakage_sb_mux[1][1]_rrnode[326]'
.meas tran sum_energy_per_cycle_mux[0to67] 
+          param='sum_energy_per_cycle_mux[0to66]+energy_per_cycle_sb_mux[1][1]_rrnode[326]'
***** Load for rr_node[326] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=35, type=4 *****
Xchan_mux_1level_tapbuf_size2[67]->out_loadlvl[0]_out mux_1level_tapbuf_size2[67]->out mux_1level_tapbuf_size2[67]->out_loadlvl[0]_out mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[195]_no0 mux_1level_tapbuf_size2[67]->out_loadlvl[0]_out mux_1level_tapbuf_size2[67]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[196]_no0 mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[197]_no0 mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[198]_no0 mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[67]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to67] 
+          param='sum_leakage_power_sb_mux[0to66]+leakage_sb_mux[1][1]_rrnode[326]'
.meas tran sum_energy_per_cycle_sb_mux[0to67] 
+          param='sum_energy_per_cycle_sb_mux[0to66]+energy_per_cycle_sb_mux[1][1]_rrnode[326]'
Xmux_1level_tapbuf_size2[68] mux_1level_tapbuf_size2[68]->in[0] mux_1level_tapbuf_size2[68]->in[1] mux_1level_tapbuf_size2[68]->out sram[88]->outb sram[88]->out gvdd_mux_1level_tapbuf_size2[68] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[68], level=1, select_path_id=0. *****
*****1*****
Xsram[88] sram->in sram[88]->out sram[88]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[88]->out) 0
.nodeset V(sram[88]->outb) vsp
***** Signal mux_1level_tapbuf_size2[68]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[68]->in[0] mux_1level_tapbuf_size2[68]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[68]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[68]->in[1] mux_1level_tapbuf_size2[68]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[68] gvdd_mux_1level_tapbuf_size2[68] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[328] trig v(mux_1level_tapbuf_size2[68]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[68]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[328] trig v(mux_1level_tapbuf_size2[68]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[68]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[328] when v(mux_1level_tapbuf_size2[68]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[328] trig v(mux_1level_tapbuf_size2[68]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[68]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[328] when v(mux_1level_tapbuf_size2[68]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[328] trig v(mux_1level_tapbuf_size2[68]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[68]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[68]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[68]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[328] param='mux_1level_tapbuf_size2[68]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[68]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[68]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[68]_energy_per_cycle param='mux_1level_tapbuf_size2[68]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[328]  param='mux_1level_tapbuf_size2[68]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[328]  param='dynamic_power_sb_mux[1][1]_rrnode[328]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[328] avg p(Vgvdd_mux_1level_tapbuf_size2[68]) from='start_rise_sb_mux[1][1]_rrnode[328]' to='start_rise_sb_mux[1][1]_rrnode[328]+switch_rise_sb_mux[1][1]_rrnode[328]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[328] avg p(Vgvdd_mux_1level_tapbuf_size2[68]) from='start_fall_sb_mux[1][1]_rrnode[328]' to='start_fall_sb_mux[1][1]_rrnode[328]+switch_fall_sb_mux[1][1]_rrnode[328]'
.meas tran sum_leakage_power_mux[0to68] 
+          param='sum_leakage_power_mux[0to67]+leakage_sb_mux[1][1]_rrnode[328]'
.meas tran sum_energy_per_cycle_mux[0to68] 
+          param='sum_energy_per_cycle_mux[0to67]+energy_per_cycle_sb_mux[1][1]_rrnode[328]'
***** Load for rr_node[328] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=37, type=4 *****
Xchan_mux_1level_tapbuf_size2[68]->out_loadlvl[0]_out mux_1level_tapbuf_size2[68]->out mux_1level_tapbuf_size2[68]->out_loadlvl[0]_out mux_1level_tapbuf_size2[68]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[199]_no0 mux_1level_tapbuf_size2[68]->out_loadlvl[0]_out mux_1level_tapbuf_size2[68]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[200]_no0 mux_1level_tapbuf_size2[68]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[68]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[201]_no0 mux_1level_tapbuf_size2[68]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[68]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[202]_no0 mux_1level_tapbuf_size2[68]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[68]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to68] 
+          param='sum_leakage_power_sb_mux[0to67]+leakage_sb_mux[1][1]_rrnode[328]'
.meas tran sum_energy_per_cycle_sb_mux[0to68] 
+          param='sum_energy_per_cycle_sb_mux[0to67]+energy_per_cycle_sb_mux[1][1]_rrnode[328]'
Xmux_1level_tapbuf_size2[69] mux_1level_tapbuf_size2[69]->in[0] mux_1level_tapbuf_size2[69]->in[1] mux_1level_tapbuf_size2[69]->out sram[89]->outb sram[89]->out gvdd_mux_1level_tapbuf_size2[69] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[69], level=1, select_path_id=0. *****
*****1*****
Xsram[89] sram->in sram[89]->out sram[89]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[89]->out) 0
.nodeset V(sram[89]->outb) vsp
***** Signal mux_1level_tapbuf_size2[69]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[69]->in[0] mux_1level_tapbuf_size2[69]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[69]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[69]->in[1] mux_1level_tapbuf_size2[69]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[69] gvdd_mux_1level_tapbuf_size2[69] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[330] trig v(mux_1level_tapbuf_size2[69]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[69]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[330] trig v(mux_1level_tapbuf_size2[69]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[69]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[330] when v(mux_1level_tapbuf_size2[69]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[330] trig v(mux_1level_tapbuf_size2[69]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[69]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[330] when v(mux_1level_tapbuf_size2[69]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[330] trig v(mux_1level_tapbuf_size2[69]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[69]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[69]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[69]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[330] param='mux_1level_tapbuf_size2[69]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[69]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[69]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[69]_energy_per_cycle param='mux_1level_tapbuf_size2[69]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[330]  param='mux_1level_tapbuf_size2[69]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[330]  param='dynamic_power_sb_mux[1][1]_rrnode[330]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[330] avg p(Vgvdd_mux_1level_tapbuf_size2[69]) from='start_rise_sb_mux[1][1]_rrnode[330]' to='start_rise_sb_mux[1][1]_rrnode[330]+switch_rise_sb_mux[1][1]_rrnode[330]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[330] avg p(Vgvdd_mux_1level_tapbuf_size2[69]) from='start_fall_sb_mux[1][1]_rrnode[330]' to='start_fall_sb_mux[1][1]_rrnode[330]+switch_fall_sb_mux[1][1]_rrnode[330]'
.meas tran sum_leakage_power_mux[0to69] 
+          param='sum_leakage_power_mux[0to68]+leakage_sb_mux[1][1]_rrnode[330]'
.meas tran sum_energy_per_cycle_mux[0to69] 
+          param='sum_energy_per_cycle_mux[0to68]+energy_per_cycle_sb_mux[1][1]_rrnode[330]'
***** Load for rr_node[330] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=39, type=4 *****
Xchan_mux_1level_tapbuf_size2[69]->out_loadlvl[0]_out mux_1level_tapbuf_size2[69]->out mux_1level_tapbuf_size2[69]->out_loadlvl[0]_out mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg0
Xload_inv[203]_no0 mux_1level_tapbuf_size2[69]->out_loadlvl[0]_out mux_1level_tapbuf_size2[69]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[204]_no0 mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[205]_no0 mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[206]_no0 mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[207]_no0 mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[69]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to69] 
+          param='sum_leakage_power_sb_mux[0to68]+leakage_sb_mux[1][1]_rrnode[330]'
.meas tran sum_energy_per_cycle_sb_mux[0to69] 
+          param='sum_energy_per_cycle_sb_mux[0to68]+energy_per_cycle_sb_mux[1][1]_rrnode[330]'
Xmux_1level_tapbuf_size2[70] mux_1level_tapbuf_size2[70]->in[0] mux_1level_tapbuf_size2[70]->in[1] mux_1level_tapbuf_size2[70]->out sram[90]->outb sram[90]->out gvdd_mux_1level_tapbuf_size2[70] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[70], level=1, select_path_id=0. *****
*****1*****
Xsram[90] sram->in sram[90]->out sram[90]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[90]->out) 0
.nodeset V(sram[90]->outb) vsp
***** Signal mux_1level_tapbuf_size2[70]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[70]->in[0] mux_1level_tapbuf_size2[70]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[70]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[70]->in[1] mux_1level_tapbuf_size2[70]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[70] gvdd_mux_1level_tapbuf_size2[70] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[332] trig v(mux_1level_tapbuf_size2[70]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[70]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[332] trig v(mux_1level_tapbuf_size2[70]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[70]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[332] when v(mux_1level_tapbuf_size2[70]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[332] trig v(mux_1level_tapbuf_size2[70]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[70]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[332] when v(mux_1level_tapbuf_size2[70]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[332] trig v(mux_1level_tapbuf_size2[70]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[70]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[70]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[70]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[332] param='mux_1level_tapbuf_size2[70]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[70]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[70]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[70]_energy_per_cycle param='mux_1level_tapbuf_size2[70]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[332]  param='mux_1level_tapbuf_size2[70]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[332]  param='dynamic_power_sb_mux[1][1]_rrnode[332]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[332] avg p(Vgvdd_mux_1level_tapbuf_size2[70]) from='start_rise_sb_mux[1][1]_rrnode[332]' to='start_rise_sb_mux[1][1]_rrnode[332]+switch_rise_sb_mux[1][1]_rrnode[332]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[332] avg p(Vgvdd_mux_1level_tapbuf_size2[70]) from='start_fall_sb_mux[1][1]_rrnode[332]' to='start_fall_sb_mux[1][1]_rrnode[332]+switch_fall_sb_mux[1][1]_rrnode[332]'
.meas tran sum_leakage_power_mux[0to70] 
+          param='sum_leakage_power_mux[0to69]+leakage_sb_mux[1][1]_rrnode[332]'
.meas tran sum_energy_per_cycle_mux[0to70] 
+          param='sum_energy_per_cycle_mux[0to69]+energy_per_cycle_sb_mux[1][1]_rrnode[332]'
***** Load for rr_node[332] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=41, type=4 *****
Xchan_mux_1level_tapbuf_size2[70]->out_loadlvl[0]_out mux_1level_tapbuf_size2[70]->out mux_1level_tapbuf_size2[70]->out_loadlvl[0]_out mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[208]_no0 mux_1level_tapbuf_size2[70]->out_loadlvl[0]_out mux_1level_tapbuf_size2[70]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[209]_no0 mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[210]_no0 mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[211]_no0 mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[70]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to70] 
+          param='sum_leakage_power_sb_mux[0to69]+leakage_sb_mux[1][1]_rrnode[332]'
.meas tran sum_energy_per_cycle_sb_mux[0to70] 
+          param='sum_energy_per_cycle_sb_mux[0to69]+energy_per_cycle_sb_mux[1][1]_rrnode[332]'
Xmux_1level_tapbuf_size2[71] mux_1level_tapbuf_size2[71]->in[0] mux_1level_tapbuf_size2[71]->in[1] mux_1level_tapbuf_size2[71]->out sram[91]->outb sram[91]->out gvdd_mux_1level_tapbuf_size2[71] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[71], level=1, select_path_id=0. *****
*****1*****
Xsram[91] sram->in sram[91]->out sram[91]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[91]->out) 0
.nodeset V(sram[91]->outb) vsp
***** Signal mux_1level_tapbuf_size2[71]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[71]->in[0] mux_1level_tapbuf_size2[71]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[71]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[71]->in[1] mux_1level_tapbuf_size2[71]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[71] gvdd_mux_1level_tapbuf_size2[71] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[334] trig v(mux_1level_tapbuf_size2[71]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[71]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[334] trig v(mux_1level_tapbuf_size2[71]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[71]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[334] when v(mux_1level_tapbuf_size2[71]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[334] trig v(mux_1level_tapbuf_size2[71]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[71]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[334] when v(mux_1level_tapbuf_size2[71]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[334] trig v(mux_1level_tapbuf_size2[71]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[71]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[71]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[71]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[334] param='mux_1level_tapbuf_size2[71]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[71]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[71]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[71]_energy_per_cycle param='mux_1level_tapbuf_size2[71]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[334]  param='mux_1level_tapbuf_size2[71]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[334]  param='dynamic_power_sb_mux[1][1]_rrnode[334]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[334] avg p(Vgvdd_mux_1level_tapbuf_size2[71]) from='start_rise_sb_mux[1][1]_rrnode[334]' to='start_rise_sb_mux[1][1]_rrnode[334]+switch_rise_sb_mux[1][1]_rrnode[334]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[334] avg p(Vgvdd_mux_1level_tapbuf_size2[71]) from='start_fall_sb_mux[1][1]_rrnode[334]' to='start_fall_sb_mux[1][1]_rrnode[334]+switch_fall_sb_mux[1][1]_rrnode[334]'
.meas tran sum_leakage_power_mux[0to71] 
+          param='sum_leakage_power_mux[0to70]+leakage_sb_mux[1][1]_rrnode[334]'
.meas tran sum_energy_per_cycle_mux[0to71] 
+          param='sum_energy_per_cycle_mux[0to70]+energy_per_cycle_sb_mux[1][1]_rrnode[334]'
***** Load for rr_node[334] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=43, type=4 *****
Xchan_mux_1level_tapbuf_size2[71]->out_loadlvl[0]_out mux_1level_tapbuf_size2[71]->out mux_1level_tapbuf_size2[71]->out_loadlvl[0]_out mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[212]_no0 mux_1level_tapbuf_size2[71]->out_loadlvl[0]_out mux_1level_tapbuf_size2[71]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[213]_no0 mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[214]_no0 mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[215]_no0 mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[71]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to71] 
+          param='sum_leakage_power_sb_mux[0to70]+leakage_sb_mux[1][1]_rrnode[334]'
.meas tran sum_energy_per_cycle_sb_mux[0to71] 
+          param='sum_energy_per_cycle_sb_mux[0to70]+energy_per_cycle_sb_mux[1][1]_rrnode[334]'
Xmux_1level_tapbuf_size2[72] mux_1level_tapbuf_size2[72]->in[0] mux_1level_tapbuf_size2[72]->in[1] mux_1level_tapbuf_size2[72]->out sram[92]->outb sram[92]->out gvdd_mux_1level_tapbuf_size2[72] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[72], level=1, select_path_id=0. *****
*****1*****
Xsram[92] sram->in sram[92]->out sram[92]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[92]->out) 0
.nodeset V(sram[92]->outb) vsp
***** Signal mux_1level_tapbuf_size2[72]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[72]->in[0] mux_1level_tapbuf_size2[72]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[72]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[72]->in[1] mux_1level_tapbuf_size2[72]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[72] gvdd_mux_1level_tapbuf_size2[72] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[336] trig v(mux_1level_tapbuf_size2[72]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[72]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[336] trig v(mux_1level_tapbuf_size2[72]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[72]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[336] when v(mux_1level_tapbuf_size2[72]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[336] trig v(mux_1level_tapbuf_size2[72]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[72]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[336] when v(mux_1level_tapbuf_size2[72]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[336] trig v(mux_1level_tapbuf_size2[72]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[72]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[72]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[72]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[336] param='mux_1level_tapbuf_size2[72]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[72]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[72]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[72]_energy_per_cycle param='mux_1level_tapbuf_size2[72]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[336]  param='mux_1level_tapbuf_size2[72]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[336]  param='dynamic_power_sb_mux[1][1]_rrnode[336]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[336] avg p(Vgvdd_mux_1level_tapbuf_size2[72]) from='start_rise_sb_mux[1][1]_rrnode[336]' to='start_rise_sb_mux[1][1]_rrnode[336]+switch_rise_sb_mux[1][1]_rrnode[336]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[336] avg p(Vgvdd_mux_1level_tapbuf_size2[72]) from='start_fall_sb_mux[1][1]_rrnode[336]' to='start_fall_sb_mux[1][1]_rrnode[336]+switch_fall_sb_mux[1][1]_rrnode[336]'
.meas tran sum_leakage_power_mux[0to72] 
+          param='sum_leakage_power_mux[0to71]+leakage_sb_mux[1][1]_rrnode[336]'
.meas tran sum_energy_per_cycle_mux[0to72] 
+          param='sum_energy_per_cycle_mux[0to71]+energy_per_cycle_sb_mux[1][1]_rrnode[336]'
***** Load for rr_node[336] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=45, type=4 *****
Xchan_mux_1level_tapbuf_size2[72]->out_loadlvl[0]_out mux_1level_tapbuf_size2[72]->out mux_1level_tapbuf_size2[72]->out_loadlvl[0]_out mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[216]_no0 mux_1level_tapbuf_size2[72]->out_loadlvl[0]_out mux_1level_tapbuf_size2[72]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[217]_no0 mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[218]_no0 mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[219]_no0 mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[220]_no0 mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[72]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to72] 
+          param='sum_leakage_power_sb_mux[0to71]+leakage_sb_mux[1][1]_rrnode[336]'
.meas tran sum_energy_per_cycle_sb_mux[0to72] 
+          param='sum_energy_per_cycle_sb_mux[0to71]+energy_per_cycle_sb_mux[1][1]_rrnode[336]'
Xmux_1level_tapbuf_size2[73] mux_1level_tapbuf_size2[73]->in[0] mux_1level_tapbuf_size2[73]->in[1] mux_1level_tapbuf_size2[73]->out sram[93]->outb sram[93]->out gvdd_mux_1level_tapbuf_size2[73] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[73], level=1, select_path_id=0. *****
*****1*****
Xsram[93] sram->in sram[93]->out sram[93]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[93]->out) 0
.nodeset V(sram[93]->outb) vsp
***** Signal mux_1level_tapbuf_size2[73]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[73]->in[0] mux_1level_tapbuf_size2[73]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[73]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[73]->in[1] mux_1level_tapbuf_size2[73]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[73] gvdd_mux_1level_tapbuf_size2[73] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[338] trig v(mux_1level_tapbuf_size2[73]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[73]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[338] trig v(mux_1level_tapbuf_size2[73]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[73]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[338] when v(mux_1level_tapbuf_size2[73]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[338] trig v(mux_1level_tapbuf_size2[73]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[73]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[338] when v(mux_1level_tapbuf_size2[73]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[338] trig v(mux_1level_tapbuf_size2[73]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[73]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[73]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[73]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[338] param='mux_1level_tapbuf_size2[73]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[73]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[73]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[73]_energy_per_cycle param='mux_1level_tapbuf_size2[73]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[338]  param='mux_1level_tapbuf_size2[73]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[338]  param='dynamic_power_sb_mux[1][1]_rrnode[338]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[338] avg p(Vgvdd_mux_1level_tapbuf_size2[73]) from='start_rise_sb_mux[1][1]_rrnode[338]' to='start_rise_sb_mux[1][1]_rrnode[338]+switch_rise_sb_mux[1][1]_rrnode[338]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[338] avg p(Vgvdd_mux_1level_tapbuf_size2[73]) from='start_fall_sb_mux[1][1]_rrnode[338]' to='start_fall_sb_mux[1][1]_rrnode[338]+switch_fall_sb_mux[1][1]_rrnode[338]'
.meas tran sum_leakage_power_mux[0to73] 
+          param='sum_leakage_power_mux[0to72]+leakage_sb_mux[1][1]_rrnode[338]'
.meas tran sum_energy_per_cycle_mux[0to73] 
+          param='sum_energy_per_cycle_mux[0to72]+energy_per_cycle_sb_mux[1][1]_rrnode[338]'
***** Load for rr_node[338] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=47, type=4 *****
Xchan_mux_1level_tapbuf_size2[73]->out_loadlvl[0]_out mux_1level_tapbuf_size2[73]->out mux_1level_tapbuf_size2[73]->out_loadlvl[0]_out mux_1level_tapbuf_size2[73]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[221]_no0 mux_1level_tapbuf_size2[73]->out_loadlvl[0]_out mux_1level_tapbuf_size2[73]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[222]_no0 mux_1level_tapbuf_size2[73]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[73]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[223]_no0 mux_1level_tapbuf_size2[73]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[73]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to73] 
+          param='sum_leakage_power_sb_mux[0to72]+leakage_sb_mux[1][1]_rrnode[338]'
.meas tran sum_energy_per_cycle_sb_mux[0to73] 
+          param='sum_energy_per_cycle_sb_mux[0to72]+energy_per_cycle_sb_mux[1][1]_rrnode[338]'
Xmux_1level_tapbuf_size2[74] mux_1level_tapbuf_size2[74]->in[0] mux_1level_tapbuf_size2[74]->in[1] mux_1level_tapbuf_size2[74]->out sram[94]->outb sram[94]->out gvdd_mux_1level_tapbuf_size2[74] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[74], level=1, select_path_id=0. *****
*****1*****
Xsram[94] sram->in sram[94]->out sram[94]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[94]->out) 0
.nodeset V(sram[94]->outb) vsp
***** Signal mux_1level_tapbuf_size2[74]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[74]->in[0] mux_1level_tapbuf_size2[74]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[74]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[74]->in[1] mux_1level_tapbuf_size2[74]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[74] gvdd_mux_1level_tapbuf_size2[74] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[340] trig v(mux_1level_tapbuf_size2[74]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[74]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[340] trig v(mux_1level_tapbuf_size2[74]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[74]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[340] when v(mux_1level_tapbuf_size2[74]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[340] trig v(mux_1level_tapbuf_size2[74]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[74]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[340] when v(mux_1level_tapbuf_size2[74]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[340] trig v(mux_1level_tapbuf_size2[74]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[74]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[74]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[74]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[340] param='mux_1level_tapbuf_size2[74]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[74]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[74]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[74]_energy_per_cycle param='mux_1level_tapbuf_size2[74]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[340]  param='mux_1level_tapbuf_size2[74]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[340]  param='dynamic_power_sb_mux[1][1]_rrnode[340]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[340] avg p(Vgvdd_mux_1level_tapbuf_size2[74]) from='start_rise_sb_mux[1][1]_rrnode[340]' to='start_rise_sb_mux[1][1]_rrnode[340]+switch_rise_sb_mux[1][1]_rrnode[340]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[340] avg p(Vgvdd_mux_1level_tapbuf_size2[74]) from='start_fall_sb_mux[1][1]_rrnode[340]' to='start_fall_sb_mux[1][1]_rrnode[340]+switch_fall_sb_mux[1][1]_rrnode[340]'
.meas tran sum_leakage_power_mux[0to74] 
+          param='sum_leakage_power_mux[0to73]+leakage_sb_mux[1][1]_rrnode[340]'
.meas tran sum_energy_per_cycle_mux[0to74] 
+          param='sum_energy_per_cycle_mux[0to73]+energy_per_cycle_sb_mux[1][1]_rrnode[340]'
***** Load for rr_node[340] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=49, type=4 *****
Xchan_mux_1level_tapbuf_size2[74]->out_loadlvl[0]_out mux_1level_tapbuf_size2[74]->out mux_1level_tapbuf_size2[74]->out_loadlvl[0]_out mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[224]_no0 mux_1level_tapbuf_size2[74]->out_loadlvl[0]_out mux_1level_tapbuf_size2[74]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[225]_no0 mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[226]_no0 mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[227]_no0 mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[74]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to74] 
+          param='sum_leakage_power_sb_mux[0to73]+leakage_sb_mux[1][1]_rrnode[340]'
.meas tran sum_energy_per_cycle_sb_mux[0to74] 
+          param='sum_energy_per_cycle_sb_mux[0to73]+energy_per_cycle_sb_mux[1][1]_rrnode[340]'
Xmux_1level_tapbuf_size2[75] mux_1level_tapbuf_size2[75]->in[0] mux_1level_tapbuf_size2[75]->in[1] mux_1level_tapbuf_size2[75]->out sram[95]->outb sram[95]->out gvdd_mux_1level_tapbuf_size2[75] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[75], level=1, select_path_id=0. *****
*****1*****
Xsram[95] sram->in sram[95]->out sram[95]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[95]->out) 0
.nodeset V(sram[95]->outb) vsp
***** Signal mux_1level_tapbuf_size2[75]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[75]->in[0] mux_1level_tapbuf_size2[75]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[75]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[75]->in[1] mux_1level_tapbuf_size2[75]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[75] gvdd_mux_1level_tapbuf_size2[75] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[342] trig v(mux_1level_tapbuf_size2[75]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[75]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[342] trig v(mux_1level_tapbuf_size2[75]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[75]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[342] when v(mux_1level_tapbuf_size2[75]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[342] trig v(mux_1level_tapbuf_size2[75]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[75]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[342] when v(mux_1level_tapbuf_size2[75]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[342] trig v(mux_1level_tapbuf_size2[75]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[75]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[75]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[75]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[342] param='mux_1level_tapbuf_size2[75]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[75]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[75]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[75]_energy_per_cycle param='mux_1level_tapbuf_size2[75]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[342]  param='mux_1level_tapbuf_size2[75]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[342]  param='dynamic_power_sb_mux[1][1]_rrnode[342]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[342] avg p(Vgvdd_mux_1level_tapbuf_size2[75]) from='start_rise_sb_mux[1][1]_rrnode[342]' to='start_rise_sb_mux[1][1]_rrnode[342]+switch_rise_sb_mux[1][1]_rrnode[342]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[342] avg p(Vgvdd_mux_1level_tapbuf_size2[75]) from='start_fall_sb_mux[1][1]_rrnode[342]' to='start_fall_sb_mux[1][1]_rrnode[342]+switch_fall_sb_mux[1][1]_rrnode[342]'
.meas tran sum_leakage_power_mux[0to75] 
+          param='sum_leakage_power_mux[0to74]+leakage_sb_mux[1][1]_rrnode[342]'
.meas tran sum_energy_per_cycle_mux[0to75] 
+          param='sum_energy_per_cycle_mux[0to74]+energy_per_cycle_sb_mux[1][1]_rrnode[342]'
***** Load for rr_node[342] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=51, type=4 *****
Xchan_mux_1level_tapbuf_size2[75]->out_loadlvl[0]_out mux_1level_tapbuf_size2[75]->out mux_1level_tapbuf_size2[75]->out_loadlvl[0]_out mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[228]_no0 mux_1level_tapbuf_size2[75]->out_loadlvl[0]_out mux_1level_tapbuf_size2[75]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[229]_no0 mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[230]_no0 mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[75]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to75] 
+          param='sum_leakage_power_sb_mux[0to74]+leakage_sb_mux[1][1]_rrnode[342]'
.meas tran sum_energy_per_cycle_sb_mux[0to75] 
+          param='sum_energy_per_cycle_sb_mux[0to74]+energy_per_cycle_sb_mux[1][1]_rrnode[342]'
Xmux_1level_tapbuf_size2[76] mux_1level_tapbuf_size2[76]->in[0] mux_1level_tapbuf_size2[76]->in[1] mux_1level_tapbuf_size2[76]->out sram[96]->outb sram[96]->out gvdd_mux_1level_tapbuf_size2[76] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[76], level=1, select_path_id=0. *****
*****1*****
Xsram[96] sram->in sram[96]->out sram[96]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[96]->out) 0
.nodeset V(sram[96]->outb) vsp
***** Signal mux_1level_tapbuf_size2[76]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[76]->in[0] mux_1level_tapbuf_size2[76]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[76]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[76]->in[1] mux_1level_tapbuf_size2[76]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[76] gvdd_mux_1level_tapbuf_size2[76] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[344] trig v(mux_1level_tapbuf_size2[76]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[76]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[344] trig v(mux_1level_tapbuf_size2[76]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[76]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[344] when v(mux_1level_tapbuf_size2[76]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[344] trig v(mux_1level_tapbuf_size2[76]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[76]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[344] when v(mux_1level_tapbuf_size2[76]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[344] trig v(mux_1level_tapbuf_size2[76]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[76]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[76]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[76]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[344] param='mux_1level_tapbuf_size2[76]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[76]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[76]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[76]_energy_per_cycle param='mux_1level_tapbuf_size2[76]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[344]  param='mux_1level_tapbuf_size2[76]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[344]  param='dynamic_power_sb_mux[1][1]_rrnode[344]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[344] avg p(Vgvdd_mux_1level_tapbuf_size2[76]) from='start_rise_sb_mux[1][1]_rrnode[344]' to='start_rise_sb_mux[1][1]_rrnode[344]+switch_rise_sb_mux[1][1]_rrnode[344]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[344] avg p(Vgvdd_mux_1level_tapbuf_size2[76]) from='start_fall_sb_mux[1][1]_rrnode[344]' to='start_fall_sb_mux[1][1]_rrnode[344]+switch_fall_sb_mux[1][1]_rrnode[344]'
.meas tran sum_leakage_power_mux[0to76] 
+          param='sum_leakage_power_mux[0to75]+leakage_sb_mux[1][1]_rrnode[344]'
.meas tran sum_energy_per_cycle_mux[0to76] 
+          param='sum_energy_per_cycle_mux[0to75]+energy_per_cycle_sb_mux[1][1]_rrnode[344]'
***** Load for rr_node[344] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=53, type=4 *****
Xchan_mux_1level_tapbuf_size2[76]->out_loadlvl[0]_out mux_1level_tapbuf_size2[76]->out mux_1level_tapbuf_size2[76]->out_loadlvl[0]_out mux_1level_tapbuf_size2[76]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[231]_no0 mux_1level_tapbuf_size2[76]->out_loadlvl[0]_out mux_1level_tapbuf_size2[76]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[232]_no0 mux_1level_tapbuf_size2[76]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[76]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[233]_no0 mux_1level_tapbuf_size2[76]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[76]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to76] 
+          param='sum_leakage_power_sb_mux[0to75]+leakage_sb_mux[1][1]_rrnode[344]'
.meas tran sum_energy_per_cycle_sb_mux[0to76] 
+          param='sum_energy_per_cycle_sb_mux[0to75]+energy_per_cycle_sb_mux[1][1]_rrnode[344]'
Xmux_1level_tapbuf_size2[77] mux_1level_tapbuf_size2[77]->in[0] mux_1level_tapbuf_size2[77]->in[1] mux_1level_tapbuf_size2[77]->out sram[97]->outb sram[97]->out gvdd_mux_1level_tapbuf_size2[77] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[77], level=1, select_path_id=0. *****
*****1*****
Xsram[97] sram->in sram[97]->out sram[97]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[97]->out) 0
.nodeset V(sram[97]->outb) vsp
***** Signal mux_1level_tapbuf_size2[77]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[77]->in[0] mux_1level_tapbuf_size2[77]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[77]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[77]->in[1] mux_1level_tapbuf_size2[77]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[77] gvdd_mux_1level_tapbuf_size2[77] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[346] trig v(mux_1level_tapbuf_size2[77]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[77]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[346] trig v(mux_1level_tapbuf_size2[77]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[77]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[346] when v(mux_1level_tapbuf_size2[77]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[346] trig v(mux_1level_tapbuf_size2[77]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[77]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[346] when v(mux_1level_tapbuf_size2[77]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[346] trig v(mux_1level_tapbuf_size2[77]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[77]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[77]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[77]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[346] param='mux_1level_tapbuf_size2[77]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[77]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[77]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[77]_energy_per_cycle param='mux_1level_tapbuf_size2[77]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[346]  param='mux_1level_tapbuf_size2[77]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[346]  param='dynamic_power_sb_mux[1][1]_rrnode[346]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[346] avg p(Vgvdd_mux_1level_tapbuf_size2[77]) from='start_rise_sb_mux[1][1]_rrnode[346]' to='start_rise_sb_mux[1][1]_rrnode[346]+switch_rise_sb_mux[1][1]_rrnode[346]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[346] avg p(Vgvdd_mux_1level_tapbuf_size2[77]) from='start_fall_sb_mux[1][1]_rrnode[346]' to='start_fall_sb_mux[1][1]_rrnode[346]+switch_fall_sb_mux[1][1]_rrnode[346]'
.meas tran sum_leakage_power_mux[0to77] 
+          param='sum_leakage_power_mux[0to76]+leakage_sb_mux[1][1]_rrnode[346]'
.meas tran sum_energy_per_cycle_mux[0to77] 
+          param='sum_energy_per_cycle_mux[0to76]+energy_per_cycle_sb_mux[1][1]_rrnode[346]'
***** Load for rr_node[346] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=55, type=4 *****
Xchan_mux_1level_tapbuf_size2[77]->out_loadlvl[0]_out mux_1level_tapbuf_size2[77]->out mux_1level_tapbuf_size2[77]->out_loadlvl[0]_out mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[234]_no0 mux_1level_tapbuf_size2[77]->out_loadlvl[0]_out mux_1level_tapbuf_size2[77]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[235]_no0 mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[236]_no0 mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[237]_no0 mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[238]_no0 mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[77]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to77] 
+          param='sum_leakage_power_sb_mux[0to76]+leakage_sb_mux[1][1]_rrnode[346]'
.meas tran sum_energy_per_cycle_sb_mux[0to77] 
+          param='sum_energy_per_cycle_sb_mux[0to76]+energy_per_cycle_sb_mux[1][1]_rrnode[346]'
Xmux_1level_tapbuf_size2[78] mux_1level_tapbuf_size2[78]->in[0] mux_1level_tapbuf_size2[78]->in[1] mux_1level_tapbuf_size2[78]->out sram[98]->outb sram[98]->out gvdd_mux_1level_tapbuf_size2[78] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[78], level=1, select_path_id=0. *****
*****1*****
Xsram[98] sram->in sram[98]->out sram[98]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[98]->out) 0
.nodeset V(sram[98]->outb) vsp
***** Signal mux_1level_tapbuf_size2[78]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[78]->in[0] mux_1level_tapbuf_size2[78]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[78]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[78]->in[1] mux_1level_tapbuf_size2[78]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[78] gvdd_mux_1level_tapbuf_size2[78] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[348] trig v(mux_1level_tapbuf_size2[78]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[78]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[348] trig v(mux_1level_tapbuf_size2[78]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[78]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[348] when v(mux_1level_tapbuf_size2[78]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[348] trig v(mux_1level_tapbuf_size2[78]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[78]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[348] when v(mux_1level_tapbuf_size2[78]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[348] trig v(mux_1level_tapbuf_size2[78]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[78]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[78]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[78]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[348] param='mux_1level_tapbuf_size2[78]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[78]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[78]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[78]_energy_per_cycle param='mux_1level_tapbuf_size2[78]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[348]  param='mux_1level_tapbuf_size2[78]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[348]  param='dynamic_power_sb_mux[1][1]_rrnode[348]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[348] avg p(Vgvdd_mux_1level_tapbuf_size2[78]) from='start_rise_sb_mux[1][1]_rrnode[348]' to='start_rise_sb_mux[1][1]_rrnode[348]+switch_rise_sb_mux[1][1]_rrnode[348]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[348] avg p(Vgvdd_mux_1level_tapbuf_size2[78]) from='start_fall_sb_mux[1][1]_rrnode[348]' to='start_fall_sb_mux[1][1]_rrnode[348]+switch_fall_sb_mux[1][1]_rrnode[348]'
.meas tran sum_leakage_power_mux[0to78] 
+          param='sum_leakage_power_mux[0to77]+leakage_sb_mux[1][1]_rrnode[348]'
.meas tran sum_energy_per_cycle_mux[0to78] 
+          param='sum_energy_per_cycle_mux[0to77]+energy_per_cycle_sb_mux[1][1]_rrnode[348]'
***** Load for rr_node[348] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=57, type=4 *****
Xchan_mux_1level_tapbuf_size2[78]->out_loadlvl[0]_out mux_1level_tapbuf_size2[78]->out mux_1level_tapbuf_size2[78]->out_loadlvl[0]_out mux_1level_tapbuf_size2[78]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[239]_no0 mux_1level_tapbuf_size2[78]->out_loadlvl[0]_out mux_1level_tapbuf_size2[78]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[240]_no0 mux_1level_tapbuf_size2[78]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[78]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[241]_no0 mux_1level_tapbuf_size2[78]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[78]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to78] 
+          param='sum_leakage_power_sb_mux[0to77]+leakage_sb_mux[1][1]_rrnode[348]'
.meas tran sum_energy_per_cycle_sb_mux[0to78] 
+          param='sum_energy_per_cycle_sb_mux[0to77]+energy_per_cycle_sb_mux[1][1]_rrnode[348]'
Xmux_1level_tapbuf_size2[79] mux_1level_tapbuf_size2[79]->in[0] mux_1level_tapbuf_size2[79]->in[1] mux_1level_tapbuf_size2[79]->out sram[99]->outb sram[99]->out gvdd_mux_1level_tapbuf_size2[79] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[79], level=1, select_path_id=0. *****
*****1*****
Xsram[99] sram->in sram[99]->out sram[99]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[99]->out) 0
.nodeset V(sram[99]->outb) vsp
***** Signal mux_1level_tapbuf_size2[79]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[79]->in[0] mux_1level_tapbuf_size2[79]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[79]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[79]->in[1] mux_1level_tapbuf_size2[79]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[79] gvdd_mux_1level_tapbuf_size2[79] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[350] trig v(mux_1level_tapbuf_size2[79]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[79]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[350] trig v(mux_1level_tapbuf_size2[79]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[79]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[350] when v(mux_1level_tapbuf_size2[79]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[350] trig v(mux_1level_tapbuf_size2[79]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[79]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[350] when v(mux_1level_tapbuf_size2[79]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[350] trig v(mux_1level_tapbuf_size2[79]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[79]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[79]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[79]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[350] param='mux_1level_tapbuf_size2[79]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[79]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[79]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[79]_energy_per_cycle param='mux_1level_tapbuf_size2[79]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[350]  param='mux_1level_tapbuf_size2[79]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[350]  param='dynamic_power_sb_mux[1][1]_rrnode[350]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[350] avg p(Vgvdd_mux_1level_tapbuf_size2[79]) from='start_rise_sb_mux[1][1]_rrnode[350]' to='start_rise_sb_mux[1][1]_rrnode[350]+switch_rise_sb_mux[1][1]_rrnode[350]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[350] avg p(Vgvdd_mux_1level_tapbuf_size2[79]) from='start_fall_sb_mux[1][1]_rrnode[350]' to='start_fall_sb_mux[1][1]_rrnode[350]+switch_fall_sb_mux[1][1]_rrnode[350]'
.meas tran sum_leakage_power_mux[0to79] 
+          param='sum_leakage_power_mux[0to78]+leakage_sb_mux[1][1]_rrnode[350]'
.meas tran sum_energy_per_cycle_mux[0to79] 
+          param='sum_energy_per_cycle_mux[0to78]+energy_per_cycle_sb_mux[1][1]_rrnode[350]'
***** Load for rr_node[350] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=59, type=4 *****
Xchan_mux_1level_tapbuf_size2[79]->out_loadlvl[0]_out mux_1level_tapbuf_size2[79]->out mux_1level_tapbuf_size2[79]->out_loadlvl[0]_out mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[242]_no0 mux_1level_tapbuf_size2[79]->out_loadlvl[0]_out mux_1level_tapbuf_size2[79]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[243]_no0 mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[244]_no0 mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[245]_no0 mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[79]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to79] 
+          param='sum_leakage_power_sb_mux[0to78]+leakage_sb_mux[1][1]_rrnode[350]'
.meas tran sum_energy_per_cycle_sb_mux[0to79] 
+          param='sum_energy_per_cycle_sb_mux[0to78]+energy_per_cycle_sb_mux[1][1]_rrnode[350]'
Xmux_1level_tapbuf_size2[80] mux_1level_tapbuf_size2[80]->in[0] mux_1level_tapbuf_size2[80]->in[1] mux_1level_tapbuf_size2[80]->out sram[100]->outb sram[100]->out gvdd_mux_1level_tapbuf_size2[80] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[80], level=1, select_path_id=0. *****
*****1*****
Xsram[100] sram->in sram[100]->out sram[100]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[100]->out) 0
.nodeset V(sram[100]->outb) vsp
***** Signal mux_1level_tapbuf_size2[80]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[80]->in[0] mux_1level_tapbuf_size2[80]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[80]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[80]->in[1] mux_1level_tapbuf_size2[80]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[80] gvdd_mux_1level_tapbuf_size2[80] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[352] trig v(mux_1level_tapbuf_size2[80]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[80]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[352] trig v(mux_1level_tapbuf_size2[80]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[80]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[352] when v(mux_1level_tapbuf_size2[80]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[352] trig v(mux_1level_tapbuf_size2[80]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[80]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[352] when v(mux_1level_tapbuf_size2[80]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[352] trig v(mux_1level_tapbuf_size2[80]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[80]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[80]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[80]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[352] param='mux_1level_tapbuf_size2[80]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[80]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[80]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[80]_energy_per_cycle param='mux_1level_tapbuf_size2[80]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[352]  param='mux_1level_tapbuf_size2[80]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[352]  param='dynamic_power_sb_mux[1][1]_rrnode[352]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[352] avg p(Vgvdd_mux_1level_tapbuf_size2[80]) from='start_rise_sb_mux[1][1]_rrnode[352]' to='start_rise_sb_mux[1][1]_rrnode[352]+switch_rise_sb_mux[1][1]_rrnode[352]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[352] avg p(Vgvdd_mux_1level_tapbuf_size2[80]) from='start_fall_sb_mux[1][1]_rrnode[352]' to='start_fall_sb_mux[1][1]_rrnode[352]+switch_fall_sb_mux[1][1]_rrnode[352]'
.meas tran sum_leakage_power_mux[0to80] 
+          param='sum_leakage_power_mux[0to79]+leakage_sb_mux[1][1]_rrnode[352]'
.meas tran sum_energy_per_cycle_mux[0to80] 
+          param='sum_energy_per_cycle_mux[0to79]+energy_per_cycle_sb_mux[1][1]_rrnode[352]'
***** Load for rr_node[352] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=61, type=4 *****
Xchan_mux_1level_tapbuf_size2[80]->out_loadlvl[0]_out mux_1level_tapbuf_size2[80]->out mux_1level_tapbuf_size2[80]->out_loadlvl[0]_out mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[246]_no0 mux_1level_tapbuf_size2[80]->out_loadlvl[0]_out mux_1level_tapbuf_size2[80]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[247]_no0 mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[248]_no0 mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[249]_no0 mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[80]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to80] 
+          param='sum_leakage_power_sb_mux[0to79]+leakage_sb_mux[1][1]_rrnode[352]'
.meas tran sum_energy_per_cycle_sb_mux[0to80] 
+          param='sum_energy_per_cycle_sb_mux[0to79]+energy_per_cycle_sb_mux[1][1]_rrnode[352]'
Xmux_1level_tapbuf_size2[81] mux_1level_tapbuf_size2[81]->in[0] mux_1level_tapbuf_size2[81]->in[1] mux_1level_tapbuf_size2[81]->out sram[101]->outb sram[101]->out gvdd_mux_1level_tapbuf_size2[81] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[81], level=1, select_path_id=0. *****
*****1*****
Xsram[101] sram->in sram[101]->out sram[101]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[101]->out) 0
.nodeset V(sram[101]->outb) vsp
***** Signal mux_1level_tapbuf_size2[81]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[81]->in[0] mux_1level_tapbuf_size2[81]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[81]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[81]->in[1] mux_1level_tapbuf_size2[81]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[81] gvdd_mux_1level_tapbuf_size2[81] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[354] trig v(mux_1level_tapbuf_size2[81]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[81]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[354] trig v(mux_1level_tapbuf_size2[81]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[81]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[354] when v(mux_1level_tapbuf_size2[81]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[354] trig v(mux_1level_tapbuf_size2[81]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[81]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[354] when v(mux_1level_tapbuf_size2[81]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[354] trig v(mux_1level_tapbuf_size2[81]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[81]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[81]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[81]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[354] param='mux_1level_tapbuf_size2[81]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[81]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[81]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[81]_energy_per_cycle param='mux_1level_tapbuf_size2[81]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[354]  param='mux_1level_tapbuf_size2[81]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[354]  param='dynamic_power_sb_mux[1][1]_rrnode[354]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[354] avg p(Vgvdd_mux_1level_tapbuf_size2[81]) from='start_rise_sb_mux[1][1]_rrnode[354]' to='start_rise_sb_mux[1][1]_rrnode[354]+switch_rise_sb_mux[1][1]_rrnode[354]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[354] avg p(Vgvdd_mux_1level_tapbuf_size2[81]) from='start_fall_sb_mux[1][1]_rrnode[354]' to='start_fall_sb_mux[1][1]_rrnode[354]+switch_fall_sb_mux[1][1]_rrnode[354]'
.meas tran sum_leakage_power_mux[0to81] 
+          param='sum_leakage_power_mux[0to80]+leakage_sb_mux[1][1]_rrnode[354]'
.meas tran sum_energy_per_cycle_mux[0to81] 
+          param='sum_energy_per_cycle_mux[0to80]+energy_per_cycle_sb_mux[1][1]_rrnode[354]'
***** Load for rr_node[354] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=63, type=4 *****
Xchan_mux_1level_tapbuf_size2[81]->out_loadlvl[0]_out mux_1level_tapbuf_size2[81]->out mux_1level_tapbuf_size2[81]->out_loadlvl[0]_out mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[250]_no0 mux_1level_tapbuf_size2[81]->out_loadlvl[0]_out mux_1level_tapbuf_size2[81]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[251]_no0 mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[252]_no0 mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[253]_no0 mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[254]_no0 mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[81]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to81] 
+          param='sum_leakage_power_sb_mux[0to80]+leakage_sb_mux[1][1]_rrnode[354]'
.meas tran sum_energy_per_cycle_sb_mux[0to81] 
+          param='sum_energy_per_cycle_sb_mux[0to80]+energy_per_cycle_sb_mux[1][1]_rrnode[354]'
Xmux_1level_tapbuf_size2[82] mux_1level_tapbuf_size2[82]->in[0] mux_1level_tapbuf_size2[82]->in[1] mux_1level_tapbuf_size2[82]->out sram[102]->outb sram[102]->out gvdd_mux_1level_tapbuf_size2[82] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[82], level=1, select_path_id=0. *****
*****1*****
Xsram[102] sram->in sram[102]->out sram[102]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[102]->out) 0
.nodeset V(sram[102]->outb) vsp
***** Signal mux_1level_tapbuf_size2[82]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[82]->in[0] mux_1level_tapbuf_size2[82]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[82]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[82]->in[1] mux_1level_tapbuf_size2[82]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[82] gvdd_mux_1level_tapbuf_size2[82] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[356] trig v(mux_1level_tapbuf_size2[82]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[82]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[356] trig v(mux_1level_tapbuf_size2[82]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[82]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[356] when v(mux_1level_tapbuf_size2[82]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[356] trig v(mux_1level_tapbuf_size2[82]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[82]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[356] when v(mux_1level_tapbuf_size2[82]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[356] trig v(mux_1level_tapbuf_size2[82]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[82]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[82]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[82]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[356] param='mux_1level_tapbuf_size2[82]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[82]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[82]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[82]_energy_per_cycle param='mux_1level_tapbuf_size2[82]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[356]  param='mux_1level_tapbuf_size2[82]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[356]  param='dynamic_power_sb_mux[1][1]_rrnode[356]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[356] avg p(Vgvdd_mux_1level_tapbuf_size2[82]) from='start_rise_sb_mux[1][1]_rrnode[356]' to='start_rise_sb_mux[1][1]_rrnode[356]+switch_rise_sb_mux[1][1]_rrnode[356]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[356] avg p(Vgvdd_mux_1level_tapbuf_size2[82]) from='start_fall_sb_mux[1][1]_rrnode[356]' to='start_fall_sb_mux[1][1]_rrnode[356]+switch_fall_sb_mux[1][1]_rrnode[356]'
.meas tran sum_leakage_power_mux[0to82] 
+          param='sum_leakage_power_mux[0to81]+leakage_sb_mux[1][1]_rrnode[356]'
.meas tran sum_energy_per_cycle_mux[0to82] 
+          param='sum_energy_per_cycle_mux[0to81]+energy_per_cycle_sb_mux[1][1]_rrnode[356]'
***** Load for rr_node[356] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=65, type=4 *****
Xchan_mux_1level_tapbuf_size2[82]->out_loadlvl[0]_out mux_1level_tapbuf_size2[82]->out mux_1level_tapbuf_size2[82]->out_loadlvl[0]_out mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[255]_no0 mux_1level_tapbuf_size2[82]->out_loadlvl[0]_out mux_1level_tapbuf_size2[82]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[256]_no0 mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[257]_no0 mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[258]_no0 mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[259]_no0 mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[82]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to82] 
+          param='sum_leakage_power_sb_mux[0to81]+leakage_sb_mux[1][1]_rrnode[356]'
.meas tran sum_energy_per_cycle_sb_mux[0to82] 
+          param='sum_energy_per_cycle_sb_mux[0to81]+energy_per_cycle_sb_mux[1][1]_rrnode[356]'
Xmux_1level_tapbuf_size2[83] mux_1level_tapbuf_size2[83]->in[0] mux_1level_tapbuf_size2[83]->in[1] mux_1level_tapbuf_size2[83]->out sram[103]->outb sram[103]->out gvdd_mux_1level_tapbuf_size2[83] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[83], level=1, select_path_id=0. *****
*****1*****
Xsram[103] sram->in sram[103]->out sram[103]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[103]->out) 0
.nodeset V(sram[103]->outb) vsp
***** Signal mux_1level_tapbuf_size2[83]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[83]->in[0] mux_1level_tapbuf_size2[83]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[83]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[83]->in[1] mux_1level_tapbuf_size2[83]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[83] gvdd_mux_1level_tapbuf_size2[83] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[358] trig v(mux_1level_tapbuf_size2[83]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[83]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[358] trig v(mux_1level_tapbuf_size2[83]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[83]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[358] when v(mux_1level_tapbuf_size2[83]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[358] trig v(mux_1level_tapbuf_size2[83]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[83]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[358] when v(mux_1level_tapbuf_size2[83]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[358] trig v(mux_1level_tapbuf_size2[83]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[83]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[83]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[83]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[358] param='mux_1level_tapbuf_size2[83]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[83]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[83]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[83]_energy_per_cycle param='mux_1level_tapbuf_size2[83]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[358]  param='mux_1level_tapbuf_size2[83]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[358]  param='dynamic_power_sb_mux[1][1]_rrnode[358]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[358] avg p(Vgvdd_mux_1level_tapbuf_size2[83]) from='start_rise_sb_mux[1][1]_rrnode[358]' to='start_rise_sb_mux[1][1]_rrnode[358]+switch_rise_sb_mux[1][1]_rrnode[358]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[358] avg p(Vgvdd_mux_1level_tapbuf_size2[83]) from='start_fall_sb_mux[1][1]_rrnode[358]' to='start_fall_sb_mux[1][1]_rrnode[358]+switch_fall_sb_mux[1][1]_rrnode[358]'
.meas tran sum_leakage_power_mux[0to83] 
+          param='sum_leakage_power_mux[0to82]+leakage_sb_mux[1][1]_rrnode[358]'
.meas tran sum_energy_per_cycle_mux[0to83] 
+          param='sum_energy_per_cycle_mux[0to82]+energy_per_cycle_sb_mux[1][1]_rrnode[358]'
***** Load for rr_node[358] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=67, type=4 *****
Xchan_mux_1level_tapbuf_size2[83]->out_loadlvl[0]_out mux_1level_tapbuf_size2[83]->out mux_1level_tapbuf_size2[83]->out_loadlvl[0]_out mux_1level_tapbuf_size2[83]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[260]_no0 mux_1level_tapbuf_size2[83]->out_loadlvl[0]_out mux_1level_tapbuf_size2[83]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[261]_no0 mux_1level_tapbuf_size2[83]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[83]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[262]_no0 mux_1level_tapbuf_size2[83]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[83]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to83] 
+          param='sum_leakage_power_sb_mux[0to82]+leakage_sb_mux[1][1]_rrnode[358]'
.meas tran sum_energy_per_cycle_sb_mux[0to83] 
+          param='sum_energy_per_cycle_sb_mux[0to82]+energy_per_cycle_sb_mux[1][1]_rrnode[358]'
Xmux_1level_tapbuf_size2[84] mux_1level_tapbuf_size2[84]->in[0] mux_1level_tapbuf_size2[84]->in[1] mux_1level_tapbuf_size2[84]->out sram[104]->outb sram[104]->out gvdd_mux_1level_tapbuf_size2[84] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[84], level=1, select_path_id=0. *****
*****1*****
Xsram[104] sram->in sram[104]->out sram[104]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[104]->out) 0
.nodeset V(sram[104]->outb) vsp
***** Signal mux_1level_tapbuf_size2[84]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[84]->in[0] mux_1level_tapbuf_size2[84]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[84]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[84]->in[1] mux_1level_tapbuf_size2[84]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[84] gvdd_mux_1level_tapbuf_size2[84] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[360] trig v(mux_1level_tapbuf_size2[84]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[84]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[360] trig v(mux_1level_tapbuf_size2[84]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[84]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[360] when v(mux_1level_tapbuf_size2[84]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[360] trig v(mux_1level_tapbuf_size2[84]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[84]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[360] when v(mux_1level_tapbuf_size2[84]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[360] trig v(mux_1level_tapbuf_size2[84]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[84]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[84]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[84]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[360] param='mux_1level_tapbuf_size2[84]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[84]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[84]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[84]_energy_per_cycle param='mux_1level_tapbuf_size2[84]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[360]  param='mux_1level_tapbuf_size2[84]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[360]  param='dynamic_power_sb_mux[1][1]_rrnode[360]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[360] avg p(Vgvdd_mux_1level_tapbuf_size2[84]) from='start_rise_sb_mux[1][1]_rrnode[360]' to='start_rise_sb_mux[1][1]_rrnode[360]+switch_rise_sb_mux[1][1]_rrnode[360]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[360] avg p(Vgvdd_mux_1level_tapbuf_size2[84]) from='start_fall_sb_mux[1][1]_rrnode[360]' to='start_fall_sb_mux[1][1]_rrnode[360]+switch_fall_sb_mux[1][1]_rrnode[360]'
.meas tran sum_leakage_power_mux[0to84] 
+          param='sum_leakage_power_mux[0to83]+leakage_sb_mux[1][1]_rrnode[360]'
.meas tran sum_energy_per_cycle_mux[0to84] 
+          param='sum_energy_per_cycle_mux[0to83]+energy_per_cycle_sb_mux[1][1]_rrnode[360]'
***** Load for rr_node[360] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=69, type=4 *****
Xchan_mux_1level_tapbuf_size2[84]->out_loadlvl[0]_out mux_1level_tapbuf_size2[84]->out mux_1level_tapbuf_size2[84]->out_loadlvl[0]_out mux_1level_tapbuf_size2[84]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg1
Xload_inv[263]_no0 mux_1level_tapbuf_size2[84]->out_loadlvl[0]_out mux_1level_tapbuf_size2[84]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[264]_no0 mux_1level_tapbuf_size2[84]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[84]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[265]_no0 mux_1level_tapbuf_size2[84]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[84]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to84] 
+          param='sum_leakage_power_sb_mux[0to83]+leakage_sb_mux[1][1]_rrnode[360]'
.meas tran sum_energy_per_cycle_sb_mux[0to84] 
+          param='sum_energy_per_cycle_sb_mux[0to83]+energy_per_cycle_sb_mux[1][1]_rrnode[360]'
Xmux_1level_tapbuf_size2[85] mux_1level_tapbuf_size2[85]->in[0] mux_1level_tapbuf_size2[85]->in[1] mux_1level_tapbuf_size2[85]->out sram[105]->outb sram[105]->out gvdd_mux_1level_tapbuf_size2[85] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[85], level=1, select_path_id=0. *****
*****1*****
Xsram[105] sram->in sram[105]->out sram[105]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[105]->out) 0
.nodeset V(sram[105]->outb) vsp
***** Signal mux_1level_tapbuf_size2[85]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[85]->in[0] mux_1level_tapbuf_size2[85]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[85]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[85]->in[1] mux_1level_tapbuf_size2[85]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[85] gvdd_mux_1level_tapbuf_size2[85] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[362] trig v(mux_1level_tapbuf_size2[85]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[85]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[362] trig v(mux_1level_tapbuf_size2[85]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[85]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[362] when v(mux_1level_tapbuf_size2[85]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[362] trig v(mux_1level_tapbuf_size2[85]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[85]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[362] when v(mux_1level_tapbuf_size2[85]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[362] trig v(mux_1level_tapbuf_size2[85]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[85]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[85]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[85]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[362] param='mux_1level_tapbuf_size2[85]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[85]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[85]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[85]_energy_per_cycle param='mux_1level_tapbuf_size2[85]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[362]  param='mux_1level_tapbuf_size2[85]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[362]  param='dynamic_power_sb_mux[1][1]_rrnode[362]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[362] avg p(Vgvdd_mux_1level_tapbuf_size2[85]) from='start_rise_sb_mux[1][1]_rrnode[362]' to='start_rise_sb_mux[1][1]_rrnode[362]+switch_rise_sb_mux[1][1]_rrnode[362]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[362] avg p(Vgvdd_mux_1level_tapbuf_size2[85]) from='start_fall_sb_mux[1][1]_rrnode[362]' to='start_fall_sb_mux[1][1]_rrnode[362]+switch_fall_sb_mux[1][1]_rrnode[362]'
.meas tran sum_leakage_power_mux[0to85] 
+          param='sum_leakage_power_mux[0to84]+leakage_sb_mux[1][1]_rrnode[362]'
.meas tran sum_energy_per_cycle_mux[0to85] 
+          param='sum_energy_per_cycle_mux[0to84]+energy_per_cycle_sb_mux[1][1]_rrnode[362]'
***** Load for rr_node[362] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=71, type=4 *****
Xchan_mux_1level_tapbuf_size2[85]->out_loadlvl[0]_out mux_1level_tapbuf_size2[85]->out mux_1level_tapbuf_size2[85]->out_loadlvl[0]_out mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[266]_no0 mux_1level_tapbuf_size2[85]->out_loadlvl[0]_out mux_1level_tapbuf_size2[85]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[267]_no0 mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[268]_no0 mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[269]_no0 mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[85]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to85] 
+          param='sum_leakage_power_sb_mux[0to84]+leakage_sb_mux[1][1]_rrnode[362]'
.meas tran sum_energy_per_cycle_sb_mux[0to85] 
+          param='sum_energy_per_cycle_sb_mux[0to84]+energy_per_cycle_sb_mux[1][1]_rrnode[362]'
Xmux_1level_tapbuf_size2[86] mux_1level_tapbuf_size2[86]->in[0] mux_1level_tapbuf_size2[86]->in[1] mux_1level_tapbuf_size2[86]->out sram[106]->outb sram[106]->out gvdd_mux_1level_tapbuf_size2[86] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[86], level=1, select_path_id=0. *****
*****1*****
Xsram[106] sram->in sram[106]->out sram[106]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[106]->out) 0
.nodeset V(sram[106]->outb) vsp
***** Signal mux_1level_tapbuf_size2[86]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[86]->in[0] mux_1level_tapbuf_size2[86]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[86]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[86]->in[1] mux_1level_tapbuf_size2[86]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[86] gvdd_mux_1level_tapbuf_size2[86] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[364] trig v(mux_1level_tapbuf_size2[86]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[86]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[364] trig v(mux_1level_tapbuf_size2[86]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[86]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[364] when v(mux_1level_tapbuf_size2[86]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[364] trig v(mux_1level_tapbuf_size2[86]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[86]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[364] when v(mux_1level_tapbuf_size2[86]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[364] trig v(mux_1level_tapbuf_size2[86]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[86]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[86]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[86]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[364] param='mux_1level_tapbuf_size2[86]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[86]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[86]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[86]_energy_per_cycle param='mux_1level_tapbuf_size2[86]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[364]  param='mux_1level_tapbuf_size2[86]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[364]  param='dynamic_power_sb_mux[1][1]_rrnode[364]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[364] avg p(Vgvdd_mux_1level_tapbuf_size2[86]) from='start_rise_sb_mux[1][1]_rrnode[364]' to='start_rise_sb_mux[1][1]_rrnode[364]+switch_rise_sb_mux[1][1]_rrnode[364]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[364] avg p(Vgvdd_mux_1level_tapbuf_size2[86]) from='start_fall_sb_mux[1][1]_rrnode[364]' to='start_fall_sb_mux[1][1]_rrnode[364]+switch_fall_sb_mux[1][1]_rrnode[364]'
.meas tran sum_leakage_power_mux[0to86] 
+          param='sum_leakage_power_mux[0to85]+leakage_sb_mux[1][1]_rrnode[364]'
.meas tran sum_energy_per_cycle_mux[0to86] 
+          param='sum_energy_per_cycle_mux[0to85]+energy_per_cycle_sb_mux[1][1]_rrnode[364]'
***** Load for rr_node[364] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=73, type=4 *****
Xchan_mux_1level_tapbuf_size2[86]->out_loadlvl[0]_out mux_1level_tapbuf_size2[86]->out mux_1level_tapbuf_size2[86]->out_loadlvl[0]_out mux_1level_tapbuf_size2[86]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[270]_no0 mux_1level_tapbuf_size2[86]->out_loadlvl[0]_out mux_1level_tapbuf_size2[86]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[271]_no0 mux_1level_tapbuf_size2[86]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[86]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[272]_no0 mux_1level_tapbuf_size2[86]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[86]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[273]_no0 mux_1level_tapbuf_size2[86]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[86]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to86] 
+          param='sum_leakage_power_sb_mux[0to85]+leakage_sb_mux[1][1]_rrnode[364]'
.meas tran sum_energy_per_cycle_sb_mux[0to86] 
+          param='sum_energy_per_cycle_sb_mux[0to85]+energy_per_cycle_sb_mux[1][1]_rrnode[364]'
Xmux_1level_tapbuf_size2[87] mux_1level_tapbuf_size2[87]->in[0] mux_1level_tapbuf_size2[87]->in[1] mux_1level_tapbuf_size2[87]->out sram[107]->outb sram[107]->out gvdd_mux_1level_tapbuf_size2[87] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[87], level=1, select_path_id=0. *****
*****1*****
Xsram[107] sram->in sram[107]->out sram[107]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[107]->out) 0
.nodeset V(sram[107]->outb) vsp
***** Signal mux_1level_tapbuf_size2[87]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[87]->in[0] mux_1level_tapbuf_size2[87]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[87]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[87]->in[1] mux_1level_tapbuf_size2[87]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[87] gvdd_mux_1level_tapbuf_size2[87] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[366] trig v(mux_1level_tapbuf_size2[87]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[87]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[366] trig v(mux_1level_tapbuf_size2[87]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[87]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[366] when v(mux_1level_tapbuf_size2[87]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[366] trig v(mux_1level_tapbuf_size2[87]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[87]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[366] when v(mux_1level_tapbuf_size2[87]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[366] trig v(mux_1level_tapbuf_size2[87]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[87]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[87]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[87]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[366] param='mux_1level_tapbuf_size2[87]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[87]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[87]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[87]_energy_per_cycle param='mux_1level_tapbuf_size2[87]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[366]  param='mux_1level_tapbuf_size2[87]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[366]  param='dynamic_power_sb_mux[1][1]_rrnode[366]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[366] avg p(Vgvdd_mux_1level_tapbuf_size2[87]) from='start_rise_sb_mux[1][1]_rrnode[366]' to='start_rise_sb_mux[1][1]_rrnode[366]+switch_rise_sb_mux[1][1]_rrnode[366]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[366] avg p(Vgvdd_mux_1level_tapbuf_size2[87]) from='start_fall_sb_mux[1][1]_rrnode[366]' to='start_fall_sb_mux[1][1]_rrnode[366]+switch_fall_sb_mux[1][1]_rrnode[366]'
.meas tran sum_leakage_power_mux[0to87] 
+          param='sum_leakage_power_mux[0to86]+leakage_sb_mux[1][1]_rrnode[366]'
.meas tran sum_energy_per_cycle_mux[0to87] 
+          param='sum_energy_per_cycle_mux[0to86]+energy_per_cycle_sb_mux[1][1]_rrnode[366]'
***** Load for rr_node[366] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=75, type=4 *****
Xchan_mux_1level_tapbuf_size2[87]->out_loadlvl[0]_out mux_1level_tapbuf_size2[87]->out mux_1level_tapbuf_size2[87]->out_loadlvl[0]_out mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[274]_no0 mux_1level_tapbuf_size2[87]->out_loadlvl[0]_out mux_1level_tapbuf_size2[87]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[275]_no0 mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[276]_no0 mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[277]_no0 mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[87]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to87] 
+          param='sum_leakage_power_sb_mux[0to86]+leakage_sb_mux[1][1]_rrnode[366]'
.meas tran sum_energy_per_cycle_sb_mux[0to87] 
+          param='sum_energy_per_cycle_sb_mux[0to86]+energy_per_cycle_sb_mux[1][1]_rrnode[366]'
Xmux_1level_tapbuf_size2[88] mux_1level_tapbuf_size2[88]->in[0] mux_1level_tapbuf_size2[88]->in[1] mux_1level_tapbuf_size2[88]->out sram[108]->outb sram[108]->out gvdd_mux_1level_tapbuf_size2[88] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[88], level=1, select_path_id=0. *****
*****1*****
Xsram[108] sram->in sram[108]->out sram[108]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[108]->out) 0
.nodeset V(sram[108]->outb) vsp
***** Signal mux_1level_tapbuf_size2[88]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[88]->in[0] mux_1level_tapbuf_size2[88]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[88]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[88]->in[1] mux_1level_tapbuf_size2[88]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[88] gvdd_mux_1level_tapbuf_size2[88] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[368] trig v(mux_1level_tapbuf_size2[88]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[88]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[368] trig v(mux_1level_tapbuf_size2[88]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[88]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[368] when v(mux_1level_tapbuf_size2[88]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[368] trig v(mux_1level_tapbuf_size2[88]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[88]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[368] when v(mux_1level_tapbuf_size2[88]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[368] trig v(mux_1level_tapbuf_size2[88]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[88]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[88]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[88]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[368] param='mux_1level_tapbuf_size2[88]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[88]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[88]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[88]_energy_per_cycle param='mux_1level_tapbuf_size2[88]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[368]  param='mux_1level_tapbuf_size2[88]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[368]  param='dynamic_power_sb_mux[1][1]_rrnode[368]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[368] avg p(Vgvdd_mux_1level_tapbuf_size2[88]) from='start_rise_sb_mux[1][1]_rrnode[368]' to='start_rise_sb_mux[1][1]_rrnode[368]+switch_rise_sb_mux[1][1]_rrnode[368]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[368] avg p(Vgvdd_mux_1level_tapbuf_size2[88]) from='start_fall_sb_mux[1][1]_rrnode[368]' to='start_fall_sb_mux[1][1]_rrnode[368]+switch_fall_sb_mux[1][1]_rrnode[368]'
.meas tran sum_leakage_power_mux[0to88] 
+          param='sum_leakage_power_mux[0to87]+leakage_sb_mux[1][1]_rrnode[368]'
.meas tran sum_energy_per_cycle_mux[0to88] 
+          param='sum_energy_per_cycle_mux[0to87]+energy_per_cycle_sb_mux[1][1]_rrnode[368]'
***** Load for rr_node[368] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=77, type=4 *****
Xchan_mux_1level_tapbuf_size2[88]->out_loadlvl[0]_out mux_1level_tapbuf_size2[88]->out mux_1level_tapbuf_size2[88]->out_loadlvl[0]_out mux_1level_tapbuf_size2[88]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[278]_no0 mux_1level_tapbuf_size2[88]->out_loadlvl[0]_out mux_1level_tapbuf_size2[88]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[279]_no0 mux_1level_tapbuf_size2[88]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[88]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[280]_no0 mux_1level_tapbuf_size2[88]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[88]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[281]_no0 mux_1level_tapbuf_size2[88]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[88]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to88] 
+          param='sum_leakage_power_sb_mux[0to87]+leakage_sb_mux[1][1]_rrnode[368]'
.meas tran sum_energy_per_cycle_sb_mux[0to88] 
+          param='sum_energy_per_cycle_sb_mux[0to87]+energy_per_cycle_sb_mux[1][1]_rrnode[368]'
Xmux_1level_tapbuf_size2[89] mux_1level_tapbuf_size2[89]->in[0] mux_1level_tapbuf_size2[89]->in[1] mux_1level_tapbuf_size2[89]->out sram[109]->outb sram[109]->out gvdd_mux_1level_tapbuf_size2[89] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[89], level=1, select_path_id=0. *****
*****1*****
Xsram[109] sram->in sram[109]->out sram[109]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[109]->out) 0
.nodeset V(sram[109]->outb) vsp
***** Signal mux_1level_tapbuf_size2[89]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[89]->in[0] mux_1level_tapbuf_size2[89]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[89]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[89]->in[1] mux_1level_tapbuf_size2[89]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[89] gvdd_mux_1level_tapbuf_size2[89] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[370] trig v(mux_1level_tapbuf_size2[89]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[89]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[370] trig v(mux_1level_tapbuf_size2[89]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[89]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[370] when v(mux_1level_tapbuf_size2[89]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[370] trig v(mux_1level_tapbuf_size2[89]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[89]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[370] when v(mux_1level_tapbuf_size2[89]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[370] trig v(mux_1level_tapbuf_size2[89]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[89]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[89]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[89]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[370] param='mux_1level_tapbuf_size2[89]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[89]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[89]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[89]_energy_per_cycle param='mux_1level_tapbuf_size2[89]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[370]  param='mux_1level_tapbuf_size2[89]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[370]  param='dynamic_power_sb_mux[1][1]_rrnode[370]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[370] avg p(Vgvdd_mux_1level_tapbuf_size2[89]) from='start_rise_sb_mux[1][1]_rrnode[370]' to='start_rise_sb_mux[1][1]_rrnode[370]+switch_rise_sb_mux[1][1]_rrnode[370]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[370] avg p(Vgvdd_mux_1level_tapbuf_size2[89]) from='start_fall_sb_mux[1][1]_rrnode[370]' to='start_fall_sb_mux[1][1]_rrnode[370]+switch_fall_sb_mux[1][1]_rrnode[370]'
.meas tran sum_leakage_power_mux[0to89] 
+          param='sum_leakage_power_mux[0to88]+leakage_sb_mux[1][1]_rrnode[370]'
.meas tran sum_energy_per_cycle_mux[0to89] 
+          param='sum_energy_per_cycle_mux[0to88]+energy_per_cycle_sb_mux[1][1]_rrnode[370]'
***** Load for rr_node[370] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=79, type=4 *****
Xchan_mux_1level_tapbuf_size2[89]->out_loadlvl[0]_out mux_1level_tapbuf_size2[89]->out mux_1level_tapbuf_size2[89]->out_loadlvl[0]_out mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[282]_no0 mux_1level_tapbuf_size2[89]->out_loadlvl[0]_out mux_1level_tapbuf_size2[89]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[283]_no0 mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[284]_no0 mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[89]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to89] 
+          param='sum_leakage_power_sb_mux[0to88]+leakage_sb_mux[1][1]_rrnode[370]'
.meas tran sum_energy_per_cycle_sb_mux[0to89] 
+          param='sum_energy_per_cycle_sb_mux[0to88]+energy_per_cycle_sb_mux[1][1]_rrnode[370]'
Xmux_1level_tapbuf_size2[90] mux_1level_tapbuf_size2[90]->in[0] mux_1level_tapbuf_size2[90]->in[1] mux_1level_tapbuf_size2[90]->out sram[110]->outb sram[110]->out gvdd_mux_1level_tapbuf_size2[90] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[90], level=1, select_path_id=0. *****
*****1*****
Xsram[110] sram->in sram[110]->out sram[110]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[110]->out) 0
.nodeset V(sram[110]->outb) vsp
***** Signal mux_1level_tapbuf_size2[90]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[90]->in[0] mux_1level_tapbuf_size2[90]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[90]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[90]->in[1] mux_1level_tapbuf_size2[90]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[90] gvdd_mux_1level_tapbuf_size2[90] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[372] trig v(mux_1level_tapbuf_size2[90]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[90]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[372] trig v(mux_1level_tapbuf_size2[90]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[90]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[372] when v(mux_1level_tapbuf_size2[90]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[372] trig v(mux_1level_tapbuf_size2[90]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[90]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[372] when v(mux_1level_tapbuf_size2[90]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[372] trig v(mux_1level_tapbuf_size2[90]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[90]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[90]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[90]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[372] param='mux_1level_tapbuf_size2[90]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[90]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[90]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[90]_energy_per_cycle param='mux_1level_tapbuf_size2[90]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[372]  param='mux_1level_tapbuf_size2[90]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[372]  param='dynamic_power_sb_mux[1][1]_rrnode[372]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[372] avg p(Vgvdd_mux_1level_tapbuf_size2[90]) from='start_rise_sb_mux[1][1]_rrnode[372]' to='start_rise_sb_mux[1][1]_rrnode[372]+switch_rise_sb_mux[1][1]_rrnode[372]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[372] avg p(Vgvdd_mux_1level_tapbuf_size2[90]) from='start_fall_sb_mux[1][1]_rrnode[372]' to='start_fall_sb_mux[1][1]_rrnode[372]+switch_fall_sb_mux[1][1]_rrnode[372]'
.meas tran sum_leakage_power_mux[0to90] 
+          param='sum_leakage_power_mux[0to89]+leakage_sb_mux[1][1]_rrnode[372]'
.meas tran sum_energy_per_cycle_mux[0to90] 
+          param='sum_energy_per_cycle_mux[0to89]+energy_per_cycle_sb_mux[1][1]_rrnode[372]'
***** Load for rr_node[372] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=81, type=4 *****
Xchan_mux_1level_tapbuf_size2[90]->out_loadlvl[0]_out mux_1level_tapbuf_size2[90]->out mux_1level_tapbuf_size2[90]->out_loadlvl[0]_out mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[285]_no0 mux_1level_tapbuf_size2[90]->out_loadlvl[0]_out mux_1level_tapbuf_size2[90]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[286]_no0 mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[287]_no0 mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[288]_no0 mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[90]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to90] 
+          param='sum_leakage_power_sb_mux[0to89]+leakage_sb_mux[1][1]_rrnode[372]'
.meas tran sum_energy_per_cycle_sb_mux[0to90] 
+          param='sum_energy_per_cycle_sb_mux[0to89]+energy_per_cycle_sb_mux[1][1]_rrnode[372]'
Xmux_1level_tapbuf_size2[91] mux_1level_tapbuf_size2[91]->in[0] mux_1level_tapbuf_size2[91]->in[1] mux_1level_tapbuf_size2[91]->out sram[111]->outb sram[111]->out gvdd_mux_1level_tapbuf_size2[91] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[91], level=1, select_path_id=0. *****
*****1*****
Xsram[111] sram->in sram[111]->out sram[111]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[111]->out) 0
.nodeset V(sram[111]->outb) vsp
***** Signal mux_1level_tapbuf_size2[91]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[91]->in[0] mux_1level_tapbuf_size2[91]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[91]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[91]->in[1] mux_1level_tapbuf_size2[91]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[91] gvdd_mux_1level_tapbuf_size2[91] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[374] trig v(mux_1level_tapbuf_size2[91]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[91]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[374] trig v(mux_1level_tapbuf_size2[91]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[91]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[374] when v(mux_1level_tapbuf_size2[91]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[374] trig v(mux_1level_tapbuf_size2[91]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[91]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[374] when v(mux_1level_tapbuf_size2[91]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[374] trig v(mux_1level_tapbuf_size2[91]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[91]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[91]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[91]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[374] param='mux_1level_tapbuf_size2[91]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[91]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[91]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[91]_energy_per_cycle param='mux_1level_tapbuf_size2[91]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[374]  param='mux_1level_tapbuf_size2[91]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[374]  param='dynamic_power_sb_mux[1][1]_rrnode[374]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[374] avg p(Vgvdd_mux_1level_tapbuf_size2[91]) from='start_rise_sb_mux[1][1]_rrnode[374]' to='start_rise_sb_mux[1][1]_rrnode[374]+switch_rise_sb_mux[1][1]_rrnode[374]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[374] avg p(Vgvdd_mux_1level_tapbuf_size2[91]) from='start_fall_sb_mux[1][1]_rrnode[374]' to='start_fall_sb_mux[1][1]_rrnode[374]+switch_fall_sb_mux[1][1]_rrnode[374]'
.meas tran sum_leakage_power_mux[0to91] 
+          param='sum_leakage_power_mux[0to90]+leakage_sb_mux[1][1]_rrnode[374]'
.meas tran sum_energy_per_cycle_mux[0to91] 
+          param='sum_energy_per_cycle_mux[0to90]+energy_per_cycle_sb_mux[1][1]_rrnode[374]'
***** Load for rr_node[374] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=83, type=4 *****
Xchan_mux_1level_tapbuf_size2[91]->out_loadlvl[0]_out mux_1level_tapbuf_size2[91]->out mux_1level_tapbuf_size2[91]->out_loadlvl[0]_out mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[289]_no0 mux_1level_tapbuf_size2[91]->out_loadlvl[0]_out mux_1level_tapbuf_size2[91]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[290]_no0 mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[291]_no0 mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[292]_no0 mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[293]_no0 mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[91]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to91] 
+          param='sum_leakage_power_sb_mux[0to90]+leakage_sb_mux[1][1]_rrnode[374]'
.meas tran sum_energy_per_cycle_sb_mux[0to91] 
+          param='sum_energy_per_cycle_sb_mux[0to90]+energy_per_cycle_sb_mux[1][1]_rrnode[374]'
Xmux_1level_tapbuf_size2[92] mux_1level_tapbuf_size2[92]->in[0] mux_1level_tapbuf_size2[92]->in[1] mux_1level_tapbuf_size2[92]->out sram[112]->outb sram[112]->out gvdd_mux_1level_tapbuf_size2[92] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[92], level=1, select_path_id=0. *****
*****1*****
Xsram[112] sram->in sram[112]->out sram[112]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[112]->out) 0
.nodeset V(sram[112]->outb) vsp
***** Signal mux_1level_tapbuf_size2[92]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[92]->in[0] mux_1level_tapbuf_size2[92]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[92]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[92]->in[1] mux_1level_tapbuf_size2[92]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[92] gvdd_mux_1level_tapbuf_size2[92] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[376] trig v(mux_1level_tapbuf_size2[92]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[92]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[376] trig v(mux_1level_tapbuf_size2[92]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[92]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[376] when v(mux_1level_tapbuf_size2[92]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[376] trig v(mux_1level_tapbuf_size2[92]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[92]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[376] when v(mux_1level_tapbuf_size2[92]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[376] trig v(mux_1level_tapbuf_size2[92]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[92]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[92]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[92]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[376] param='mux_1level_tapbuf_size2[92]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[92]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[92]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[92]_energy_per_cycle param='mux_1level_tapbuf_size2[92]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[376]  param='mux_1level_tapbuf_size2[92]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[376]  param='dynamic_power_sb_mux[1][1]_rrnode[376]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[376] avg p(Vgvdd_mux_1level_tapbuf_size2[92]) from='start_rise_sb_mux[1][1]_rrnode[376]' to='start_rise_sb_mux[1][1]_rrnode[376]+switch_rise_sb_mux[1][1]_rrnode[376]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[376] avg p(Vgvdd_mux_1level_tapbuf_size2[92]) from='start_fall_sb_mux[1][1]_rrnode[376]' to='start_fall_sb_mux[1][1]_rrnode[376]+switch_fall_sb_mux[1][1]_rrnode[376]'
.meas tran sum_leakage_power_mux[0to92] 
+          param='sum_leakage_power_mux[0to91]+leakage_sb_mux[1][1]_rrnode[376]'
.meas tran sum_energy_per_cycle_mux[0to92] 
+          param='sum_energy_per_cycle_mux[0to91]+energy_per_cycle_sb_mux[1][1]_rrnode[376]'
***** Load for rr_node[376] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=85, type=4 *****
Xchan_mux_1level_tapbuf_size2[92]->out_loadlvl[0]_out mux_1level_tapbuf_size2[92]->out mux_1level_tapbuf_size2[92]->out_loadlvl[0]_out mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[294]_no0 mux_1level_tapbuf_size2[92]->out_loadlvl[0]_out mux_1level_tapbuf_size2[92]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[295]_no0 mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[296]_no0 mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[92]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to92] 
+          param='sum_leakage_power_sb_mux[0to91]+leakage_sb_mux[1][1]_rrnode[376]'
.meas tran sum_energy_per_cycle_sb_mux[0to92] 
+          param='sum_energy_per_cycle_sb_mux[0to91]+energy_per_cycle_sb_mux[1][1]_rrnode[376]'
Xmux_1level_tapbuf_size2[93] mux_1level_tapbuf_size2[93]->in[0] mux_1level_tapbuf_size2[93]->in[1] mux_1level_tapbuf_size2[93]->out sram[113]->outb sram[113]->out gvdd_mux_1level_tapbuf_size2[93] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[93], level=1, select_path_id=0. *****
*****1*****
Xsram[113] sram->in sram[113]->out sram[113]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[113]->out) 0
.nodeset V(sram[113]->outb) vsp
***** Signal mux_1level_tapbuf_size2[93]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[93]->in[0] mux_1level_tapbuf_size2[93]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[93]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[93]->in[1] mux_1level_tapbuf_size2[93]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[93] gvdd_mux_1level_tapbuf_size2[93] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[378] trig v(mux_1level_tapbuf_size2[93]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[93]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[378] trig v(mux_1level_tapbuf_size2[93]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[93]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[378] when v(mux_1level_tapbuf_size2[93]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[378] trig v(mux_1level_tapbuf_size2[93]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[93]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[378] when v(mux_1level_tapbuf_size2[93]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[378] trig v(mux_1level_tapbuf_size2[93]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[93]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[93]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[93]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[378] param='mux_1level_tapbuf_size2[93]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[93]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[93]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[93]_energy_per_cycle param='mux_1level_tapbuf_size2[93]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[378]  param='mux_1level_tapbuf_size2[93]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[378]  param='dynamic_power_sb_mux[1][1]_rrnode[378]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[378] avg p(Vgvdd_mux_1level_tapbuf_size2[93]) from='start_rise_sb_mux[1][1]_rrnode[378]' to='start_rise_sb_mux[1][1]_rrnode[378]+switch_rise_sb_mux[1][1]_rrnode[378]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[378] avg p(Vgvdd_mux_1level_tapbuf_size2[93]) from='start_fall_sb_mux[1][1]_rrnode[378]' to='start_fall_sb_mux[1][1]_rrnode[378]+switch_fall_sb_mux[1][1]_rrnode[378]'
.meas tran sum_leakage_power_mux[0to93] 
+          param='sum_leakage_power_mux[0to92]+leakage_sb_mux[1][1]_rrnode[378]'
.meas tran sum_energy_per_cycle_mux[0to93] 
+          param='sum_energy_per_cycle_mux[0to92]+energy_per_cycle_sb_mux[1][1]_rrnode[378]'
***** Load for rr_node[378] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=87, type=4 *****
Xchan_mux_1level_tapbuf_size2[93]->out_loadlvl[0]_out mux_1level_tapbuf_size2[93]->out mux_1level_tapbuf_size2[93]->out_loadlvl[0]_out mux_1level_tapbuf_size2[93]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[297]_no0 mux_1level_tapbuf_size2[93]->out_loadlvl[0]_out mux_1level_tapbuf_size2[93]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[298]_no0 mux_1level_tapbuf_size2[93]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[93]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[299]_no0 mux_1level_tapbuf_size2[93]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[93]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[300]_no0 mux_1level_tapbuf_size2[93]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[93]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to93] 
+          param='sum_leakage_power_sb_mux[0to92]+leakage_sb_mux[1][1]_rrnode[378]'
.meas tran sum_energy_per_cycle_sb_mux[0to93] 
+          param='sum_energy_per_cycle_sb_mux[0to92]+energy_per_cycle_sb_mux[1][1]_rrnode[378]'
Xmux_1level_tapbuf_size2[94] mux_1level_tapbuf_size2[94]->in[0] mux_1level_tapbuf_size2[94]->in[1] mux_1level_tapbuf_size2[94]->out sram[114]->outb sram[114]->out gvdd_mux_1level_tapbuf_size2[94] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[94], level=1, select_path_id=0. *****
*****1*****
Xsram[114] sram->in sram[114]->out sram[114]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[114]->out) 0
.nodeset V(sram[114]->outb) vsp
***** Signal mux_1level_tapbuf_size2[94]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[94]->in[0] mux_1level_tapbuf_size2[94]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[94]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[94]->in[1] mux_1level_tapbuf_size2[94]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[94] gvdd_mux_1level_tapbuf_size2[94] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[380] trig v(mux_1level_tapbuf_size2[94]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[94]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[380] trig v(mux_1level_tapbuf_size2[94]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[94]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[380] when v(mux_1level_tapbuf_size2[94]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[380] trig v(mux_1level_tapbuf_size2[94]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[94]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[380] when v(mux_1level_tapbuf_size2[94]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[380] trig v(mux_1level_tapbuf_size2[94]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[94]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[94]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[94]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[380] param='mux_1level_tapbuf_size2[94]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[94]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[94]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[94]_energy_per_cycle param='mux_1level_tapbuf_size2[94]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[380]  param='mux_1level_tapbuf_size2[94]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[380]  param='dynamic_power_sb_mux[1][1]_rrnode[380]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[380] avg p(Vgvdd_mux_1level_tapbuf_size2[94]) from='start_rise_sb_mux[1][1]_rrnode[380]' to='start_rise_sb_mux[1][1]_rrnode[380]+switch_rise_sb_mux[1][1]_rrnode[380]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[380] avg p(Vgvdd_mux_1level_tapbuf_size2[94]) from='start_fall_sb_mux[1][1]_rrnode[380]' to='start_fall_sb_mux[1][1]_rrnode[380]+switch_fall_sb_mux[1][1]_rrnode[380]'
.meas tran sum_leakage_power_mux[0to94] 
+          param='sum_leakage_power_mux[0to93]+leakage_sb_mux[1][1]_rrnode[380]'
.meas tran sum_energy_per_cycle_mux[0to94] 
+          param='sum_energy_per_cycle_mux[0to93]+energy_per_cycle_sb_mux[1][1]_rrnode[380]'
***** Load for rr_node[380] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=89, type=4 *****
Xchan_mux_1level_tapbuf_size2[94]->out_loadlvl[0]_out mux_1level_tapbuf_size2[94]->out mux_1level_tapbuf_size2[94]->out_loadlvl[0]_out mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[301]_no0 mux_1level_tapbuf_size2[94]->out_loadlvl[0]_out mux_1level_tapbuf_size2[94]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[302]_no0 mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[303]_no0 mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[304]_no0 mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[94]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to94] 
+          param='sum_leakage_power_sb_mux[0to93]+leakage_sb_mux[1][1]_rrnode[380]'
.meas tran sum_energy_per_cycle_sb_mux[0to94] 
+          param='sum_energy_per_cycle_sb_mux[0to93]+energy_per_cycle_sb_mux[1][1]_rrnode[380]'
Xmux_1level_tapbuf_size2[95] mux_1level_tapbuf_size2[95]->in[0] mux_1level_tapbuf_size2[95]->in[1] mux_1level_tapbuf_size2[95]->out sram[115]->outb sram[115]->out gvdd_mux_1level_tapbuf_size2[95] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[95], level=1, select_path_id=0. *****
*****1*****
Xsram[115] sram->in sram[115]->out sram[115]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[115]->out) 0
.nodeset V(sram[115]->outb) vsp
***** Signal mux_1level_tapbuf_size2[95]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[95]->in[0] mux_1level_tapbuf_size2[95]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[95]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[95]->in[1] mux_1level_tapbuf_size2[95]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[95] gvdd_mux_1level_tapbuf_size2[95] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[382] trig v(mux_1level_tapbuf_size2[95]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[95]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[382] trig v(mux_1level_tapbuf_size2[95]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[95]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[382] when v(mux_1level_tapbuf_size2[95]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[382] trig v(mux_1level_tapbuf_size2[95]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[95]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[382] when v(mux_1level_tapbuf_size2[95]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[382] trig v(mux_1level_tapbuf_size2[95]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[95]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[95]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[95]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[382] param='mux_1level_tapbuf_size2[95]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[95]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[95]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[95]_energy_per_cycle param='mux_1level_tapbuf_size2[95]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[382]  param='mux_1level_tapbuf_size2[95]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[382]  param='dynamic_power_sb_mux[1][1]_rrnode[382]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[382] avg p(Vgvdd_mux_1level_tapbuf_size2[95]) from='start_rise_sb_mux[1][1]_rrnode[382]' to='start_rise_sb_mux[1][1]_rrnode[382]+switch_rise_sb_mux[1][1]_rrnode[382]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[382] avg p(Vgvdd_mux_1level_tapbuf_size2[95]) from='start_fall_sb_mux[1][1]_rrnode[382]' to='start_fall_sb_mux[1][1]_rrnode[382]+switch_fall_sb_mux[1][1]_rrnode[382]'
.meas tran sum_leakage_power_mux[0to95] 
+          param='sum_leakage_power_mux[0to94]+leakage_sb_mux[1][1]_rrnode[382]'
.meas tran sum_energy_per_cycle_mux[0to95] 
+          param='sum_energy_per_cycle_mux[0to94]+energy_per_cycle_sb_mux[1][1]_rrnode[382]'
***** Load for rr_node[382] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=91, type=4 *****
Xchan_mux_1level_tapbuf_size2[95]->out_loadlvl[0]_out mux_1level_tapbuf_size2[95]->out mux_1level_tapbuf_size2[95]->out_loadlvl[0]_out mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[305]_no0 mux_1level_tapbuf_size2[95]->out_loadlvl[0]_out mux_1level_tapbuf_size2[95]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[306]_no0 mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[307]_no0 mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[308]_no0 mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[95]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to95] 
+          param='sum_leakage_power_sb_mux[0to94]+leakage_sb_mux[1][1]_rrnode[382]'
.meas tran sum_energy_per_cycle_sb_mux[0to95] 
+          param='sum_energy_per_cycle_sb_mux[0to94]+energy_per_cycle_sb_mux[1][1]_rrnode[382]'
Xmux_1level_tapbuf_size2[96] mux_1level_tapbuf_size2[96]->in[0] mux_1level_tapbuf_size2[96]->in[1] mux_1level_tapbuf_size2[96]->out sram[116]->outb sram[116]->out gvdd_mux_1level_tapbuf_size2[96] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[96], level=1, select_path_id=0. *****
*****1*****
Xsram[116] sram->in sram[116]->out sram[116]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[116]->out) 0
.nodeset V(sram[116]->outb) vsp
***** Signal mux_1level_tapbuf_size2[96]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[96]->in[0] mux_1level_tapbuf_size2[96]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[96]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[96]->in[1] mux_1level_tapbuf_size2[96]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[96] gvdd_mux_1level_tapbuf_size2[96] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[384] trig v(mux_1level_tapbuf_size2[96]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[96]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[384] trig v(mux_1level_tapbuf_size2[96]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[96]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[384] when v(mux_1level_tapbuf_size2[96]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[384] trig v(mux_1level_tapbuf_size2[96]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[96]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[384] when v(mux_1level_tapbuf_size2[96]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[384] trig v(mux_1level_tapbuf_size2[96]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[96]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[96]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[96]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[384] param='mux_1level_tapbuf_size2[96]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[96]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[96]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[96]_energy_per_cycle param='mux_1level_tapbuf_size2[96]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[384]  param='mux_1level_tapbuf_size2[96]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[384]  param='dynamic_power_sb_mux[1][1]_rrnode[384]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[384] avg p(Vgvdd_mux_1level_tapbuf_size2[96]) from='start_rise_sb_mux[1][1]_rrnode[384]' to='start_rise_sb_mux[1][1]_rrnode[384]+switch_rise_sb_mux[1][1]_rrnode[384]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[384] avg p(Vgvdd_mux_1level_tapbuf_size2[96]) from='start_fall_sb_mux[1][1]_rrnode[384]' to='start_fall_sb_mux[1][1]_rrnode[384]+switch_fall_sb_mux[1][1]_rrnode[384]'
.meas tran sum_leakage_power_mux[0to96] 
+          param='sum_leakage_power_mux[0to95]+leakage_sb_mux[1][1]_rrnode[384]'
.meas tran sum_energy_per_cycle_mux[0to96] 
+          param='sum_energy_per_cycle_mux[0to95]+energy_per_cycle_sb_mux[1][1]_rrnode[384]'
***** Load for rr_node[384] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=93, type=4 *****
Xchan_mux_1level_tapbuf_size2[96]->out_loadlvl[0]_out mux_1level_tapbuf_size2[96]->out mux_1level_tapbuf_size2[96]->out_loadlvl[0]_out mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[309]_no0 mux_1level_tapbuf_size2[96]->out_loadlvl[0]_out mux_1level_tapbuf_size2[96]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[310]_no0 mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[311]_no0 mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[312]_no0 mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[96]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to96] 
+          param='sum_leakage_power_sb_mux[0to95]+leakage_sb_mux[1][1]_rrnode[384]'
.meas tran sum_energy_per_cycle_sb_mux[0to96] 
+          param='sum_energy_per_cycle_sb_mux[0to95]+energy_per_cycle_sb_mux[1][1]_rrnode[384]'
Xmux_1level_tapbuf_size2[97] mux_1level_tapbuf_size2[97]->in[0] mux_1level_tapbuf_size2[97]->in[1] mux_1level_tapbuf_size2[97]->out sram[117]->outb sram[117]->out gvdd_mux_1level_tapbuf_size2[97] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[97], level=1, select_path_id=0. *****
*****1*****
Xsram[117] sram->in sram[117]->out sram[117]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[117]->out) 0
.nodeset V(sram[117]->outb) vsp
***** Signal mux_1level_tapbuf_size2[97]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[97]->in[0] mux_1level_tapbuf_size2[97]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[97]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[97]->in[1] mux_1level_tapbuf_size2[97]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[97] gvdd_mux_1level_tapbuf_size2[97] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[386] trig v(mux_1level_tapbuf_size2[97]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[97]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[386] trig v(mux_1level_tapbuf_size2[97]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[97]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[386] when v(mux_1level_tapbuf_size2[97]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[386] trig v(mux_1level_tapbuf_size2[97]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[97]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[386] when v(mux_1level_tapbuf_size2[97]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[386] trig v(mux_1level_tapbuf_size2[97]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[97]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[97]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[97]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[386] param='mux_1level_tapbuf_size2[97]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[97]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[97]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[97]_energy_per_cycle param='mux_1level_tapbuf_size2[97]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[386]  param='mux_1level_tapbuf_size2[97]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[386]  param='dynamic_power_sb_mux[1][1]_rrnode[386]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[386] avg p(Vgvdd_mux_1level_tapbuf_size2[97]) from='start_rise_sb_mux[1][1]_rrnode[386]' to='start_rise_sb_mux[1][1]_rrnode[386]+switch_rise_sb_mux[1][1]_rrnode[386]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[386] avg p(Vgvdd_mux_1level_tapbuf_size2[97]) from='start_fall_sb_mux[1][1]_rrnode[386]' to='start_fall_sb_mux[1][1]_rrnode[386]+switch_fall_sb_mux[1][1]_rrnode[386]'
.meas tran sum_leakage_power_mux[0to97] 
+          param='sum_leakage_power_mux[0to96]+leakage_sb_mux[1][1]_rrnode[386]'
.meas tran sum_energy_per_cycle_mux[0to97] 
+          param='sum_energy_per_cycle_mux[0to96]+energy_per_cycle_sb_mux[1][1]_rrnode[386]'
***** Load for rr_node[386] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=95, type=4 *****
Xchan_mux_1level_tapbuf_size2[97]->out_loadlvl[0]_out mux_1level_tapbuf_size2[97]->out mux_1level_tapbuf_size2[97]->out_loadlvl[0]_out mux_1level_tapbuf_size2[97]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[313]_no0 mux_1level_tapbuf_size2[97]->out_loadlvl[0]_out mux_1level_tapbuf_size2[97]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[314]_no0 mux_1level_tapbuf_size2[97]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[97]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[315]_no0 mux_1level_tapbuf_size2[97]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[97]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to97] 
+          param='sum_leakage_power_sb_mux[0to96]+leakage_sb_mux[1][1]_rrnode[386]'
.meas tran sum_energy_per_cycle_sb_mux[0to97] 
+          param='sum_energy_per_cycle_sb_mux[0to96]+energy_per_cycle_sb_mux[1][1]_rrnode[386]'
Xmux_1level_tapbuf_size2[98] mux_1level_tapbuf_size2[98]->in[0] mux_1level_tapbuf_size2[98]->in[1] mux_1level_tapbuf_size2[98]->out sram[118]->outb sram[118]->out gvdd_mux_1level_tapbuf_size2[98] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[98], level=1, select_path_id=0. *****
*****1*****
Xsram[118] sram->in sram[118]->out sram[118]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[118]->out) 0
.nodeset V(sram[118]->outb) vsp
***** Signal mux_1level_tapbuf_size2[98]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[98]->in[0] mux_1level_tapbuf_size2[98]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[98]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[98]->in[1] mux_1level_tapbuf_size2[98]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[98] gvdd_mux_1level_tapbuf_size2[98] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[388] trig v(mux_1level_tapbuf_size2[98]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[98]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[388] trig v(mux_1level_tapbuf_size2[98]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[98]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[388] when v(mux_1level_tapbuf_size2[98]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[388] trig v(mux_1level_tapbuf_size2[98]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[98]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[388] when v(mux_1level_tapbuf_size2[98]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[388] trig v(mux_1level_tapbuf_size2[98]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[98]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[98]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[98]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[388] param='mux_1level_tapbuf_size2[98]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[98]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[98]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[98]_energy_per_cycle param='mux_1level_tapbuf_size2[98]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[388]  param='mux_1level_tapbuf_size2[98]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[388]  param='dynamic_power_sb_mux[1][1]_rrnode[388]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[388] avg p(Vgvdd_mux_1level_tapbuf_size2[98]) from='start_rise_sb_mux[1][1]_rrnode[388]' to='start_rise_sb_mux[1][1]_rrnode[388]+switch_rise_sb_mux[1][1]_rrnode[388]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[388] avg p(Vgvdd_mux_1level_tapbuf_size2[98]) from='start_fall_sb_mux[1][1]_rrnode[388]' to='start_fall_sb_mux[1][1]_rrnode[388]+switch_fall_sb_mux[1][1]_rrnode[388]'
.meas tran sum_leakage_power_mux[0to98] 
+          param='sum_leakage_power_mux[0to97]+leakage_sb_mux[1][1]_rrnode[388]'
.meas tran sum_energy_per_cycle_mux[0to98] 
+          param='sum_energy_per_cycle_mux[0to97]+energy_per_cycle_sb_mux[1][1]_rrnode[388]'
***** Load for rr_node[388] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=97, type=4 *****
Xchan_mux_1level_tapbuf_size2[98]->out_loadlvl[0]_out mux_1level_tapbuf_size2[98]->out mux_1level_tapbuf_size2[98]->out_loadlvl[0]_out mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[316]_no0 mux_1level_tapbuf_size2[98]->out_loadlvl[0]_out mux_1level_tapbuf_size2[98]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[317]_no0 mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[318]_no0 mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
Xload_inv[319]_no0 mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout_out[3] gvdd_load 0 inv size=1
Xload_inv[320]_no0 mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[98]->out_loadlvl[0]_midout_out[4] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to98] 
+          param='sum_leakage_power_sb_mux[0to97]+leakage_sb_mux[1][1]_rrnode[388]'
.meas tran sum_energy_per_cycle_sb_mux[0to98] 
+          param='sum_energy_per_cycle_sb_mux[0to97]+energy_per_cycle_sb_mux[1][1]_rrnode[388]'
Xmux_1level_tapbuf_size2[99] mux_1level_tapbuf_size2[99]->in[0] mux_1level_tapbuf_size2[99]->in[1] mux_1level_tapbuf_size2[99]->out sram[119]->outb sram[119]->out gvdd_mux_1level_tapbuf_size2[99] 0 mux_1level_tapbuf_size2
***** SRAM bits for MUX[99], level=1, select_path_id=0. *****
*****1*****
Xsram[119] sram->in sram[119]->out sram[119]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[119]->out) 0
.nodeset V(sram[119]->outb) vsp
***** Signal mux_1level_tapbuf_size2[99]->in[0] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[99]->in[0] mux_1level_tapbuf_size2[99]->in[0] 0 
+  0
***** Signal mux_1level_tapbuf_size2[99]->in[1] density = 0, probability=0.*****
Vmux_1level_tapbuf_size2[99]->in[1] mux_1level_tapbuf_size2[99]->in[1] 0 
+  0
Vgvdd_mux_1level_tapbuf_size2[99] gvdd_mux_1level_tapbuf_size2[99] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_sb_mux[1][1]_rrnode[390] trig v(mux_1level_tapbuf_size2[99]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[99]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_sb_mux[1][1]_rrnode[390] trig v(mux_1level_tapbuf_size2[99]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[99]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_sb_mux[1][1]_rrnode[390] when v(mux_1level_tapbuf_size2[99]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_sb_mux[1][1]_rrnode[390] trig v(mux_1level_tapbuf_size2[99]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[99]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_sb_mux[1][1]_rrnode[390] when v(mux_1level_tapbuf_size2[99]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_sb_mux[1][1]_rrnode[390] trig v(mux_1level_tapbuf_size2[99]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_1level_tapbuf_size2[99]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_1level_tapbuf_size2[99]_leakage_power avg p(Vgvdd_mux_1level_tapbuf_size2[99]) from=0 to='clock_period'
.meas tran leakage_sb_mux[1][1]_rrnode[390] param='mux_1level_tapbuf_size2[99]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_1level_tapbuf_size2[99]_dynamic_power avg p(Vgvdd_mux_1level_tapbuf_size2[99]) from='clock_period' to='2*clock_period'
.meas tran mux_1level_tapbuf_size2[99]_energy_per_cycle param='mux_1level_tapbuf_size2[99]_dynamic_power*clock_period'
.meas tran dynamic_power_sb_mux[1][1]_rrnode[390]  param='mux_1level_tapbuf_size2[99]_dynamic_power'
.meas tran energy_per_cycle_sb_mux[1][1]_rrnode[390]  param='dynamic_power_sb_mux[1][1]_rrnode[390]*clock_period'
.meas tran dynamic_rise_sb_mux[1][1]_rrnode[390] avg p(Vgvdd_mux_1level_tapbuf_size2[99]) from='start_rise_sb_mux[1][1]_rrnode[390]' to='start_rise_sb_mux[1][1]_rrnode[390]+switch_rise_sb_mux[1][1]_rrnode[390]'
.meas tran dynamic_fall_sb_mux[1][1]_rrnode[390] avg p(Vgvdd_mux_1level_tapbuf_size2[99]) from='start_fall_sb_mux[1][1]_rrnode[390]' to='start_fall_sb_mux[1][1]_rrnode[390]+switch_fall_sb_mux[1][1]_rrnode[390]'
.meas tran sum_leakage_power_mux[0to99] 
+          param='sum_leakage_power_mux[0to98]+leakage_sb_mux[1][1]_rrnode[390]'
.meas tran sum_energy_per_cycle_mux[0to99] 
+          param='sum_energy_per_cycle_mux[0to98]+energy_per_cycle_sb_mux[1][1]_rrnode[390]'
***** Load for rr_node[390] *****
**** Loads for rr_node: xlow=1, ylow=1, xhigh=1, yhigh=1, ptc_num=99, type=4 *****
Xchan_mux_1level_tapbuf_size2[99]->out_loadlvl[0]_out mux_1level_tapbuf_size2[99]->out mux_1level_tapbuf_size2[99]->out_loadlvl[0]_out mux_1level_tapbuf_size2[99]->out_loadlvl[0]_midout gvdd_load 0 chan_segment_seg2
Xload_inv[321]_no0 mux_1level_tapbuf_size2[99]->out_loadlvl[0]_out mux_1level_tapbuf_size2[99]->out_loadlvl[0]_out_out[0] gvdd_load 0 inv size=1
Xload_inv[322]_no0 mux_1level_tapbuf_size2[99]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[99]->out_loadlvl[0]_midout_out[1] gvdd_load 0 inv size=1
Xload_inv[323]_no0 mux_1level_tapbuf_size2[99]->out_loadlvl[0]_midout mux_1level_tapbuf_size2[99]->out_loadlvl[0]_midout_out[2] gvdd_load 0 inv size=1
.meas tran sum_leakage_power_sb_mux[0to99] 
+          param='sum_leakage_power_sb_mux[0to98]+leakage_sb_mux[1][1]_rrnode[390]'
.meas tran sum_energy_per_cycle_sb_mux[0to99] 
+          param='sum_energy_per_cycle_sb_mux[0to98]+energy_per_cycle_sb_mux[1][1]_rrnode[390]'
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
