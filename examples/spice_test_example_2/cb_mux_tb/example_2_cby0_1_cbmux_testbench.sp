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
Xmux_2level_tapbuf_size16[0] mux_2level_tapbuf_size16[0]->in[0] mux_2level_tapbuf_size16[0]->in[1] mux_2level_tapbuf_size16[0]->in[2] mux_2level_tapbuf_size16[0]->in[3] mux_2level_tapbuf_size16[0]->in[4] mux_2level_tapbuf_size16[0]->in[5] mux_2level_tapbuf_size16[0]->in[6] mux_2level_tapbuf_size16[0]->in[7] mux_2level_tapbuf_size16[0]->in[8] mux_2level_tapbuf_size16[0]->in[9] mux_2level_tapbuf_size16[0]->in[10] mux_2level_tapbuf_size16[0]->in[11] mux_2level_tapbuf_size16[0]->in[12] mux_2level_tapbuf_size16[0]->in[13] mux_2level_tapbuf_size16[0]->in[14] mux_2level_tapbuf_size16[0]->in[15] mux_2level_tapbuf_size16[0]->out sram[0]->outb sram[0]->out sram[1]->out sram[1]->outb sram[2]->out sram[2]->outb sram[3]->out sram[3]->outb sram[4]->outb sram[4]->out sram[5]->out sram[5]->outb sram[6]->out sram[6]->outb sram[7]->out sram[7]->outb gvdd_mux_2level_tapbuf_size16[0] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[0], level=2, select_path_id=0. *****
*****10001000*****
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
Xsram[6] sram->in sram[6]->out sram[6]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[6]->out) 0
.nodeset V(sram[6]->outb) vsp
Xsram[7] sram->in sram[7]->out sram[7]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[7]->out) 0
.nodeset V(sram[7]->outb) vsp
***** Signal mux_2level_tapbuf_size16[0]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[0] mux_2level_tapbuf_size16[0]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[1] mux_2level_tapbuf_size16[0]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[2] mux_2level_tapbuf_size16[0]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[3] mux_2level_tapbuf_size16[0]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[4] mux_2level_tapbuf_size16[0]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[5] mux_2level_tapbuf_size16[0]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[6] mux_2level_tapbuf_size16[0]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[7] mux_2level_tapbuf_size16[0]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[8] mux_2level_tapbuf_size16[0]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[9] mux_2level_tapbuf_size16[0]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[10] mux_2level_tapbuf_size16[0]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[11] mux_2level_tapbuf_size16[0]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[12] mux_2level_tapbuf_size16[0]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[13] mux_2level_tapbuf_size16[0]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[14] mux_2level_tapbuf_size16[0]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[0]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[0]->in[15] mux_2level_tapbuf_size16[0]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[0] gvdd_mux_2level_tapbuf_size16[0] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[79] trig v(mux_2level_tapbuf_size16[0]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[0]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[79] trig v(mux_2level_tapbuf_size16[0]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[0]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[79] when v(mux_2level_tapbuf_size16[0]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[79] trig v(mux_2level_tapbuf_size16[0]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[0]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[79] when v(mux_2level_tapbuf_size16[0]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[79] trig v(mux_2level_tapbuf_size16[0]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[0]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[0]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[0]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[79] param='mux_2level_tapbuf_size16[0]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[0]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[0]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[0]_energy_per_cycle param='mux_2level_tapbuf_size16[0]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[79]  param='mux_2level_tapbuf_size16[0]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[79]  param='dynamic_power_cb_mux[0][1]_rrnode[79]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[79] avg p(Vgvdd_mux_2level_tapbuf_size16[0]) from='start_rise_cb_mux[0][1]_rrnode[79]' to='start_rise_cb_mux[0][1]_rrnode[79]+switch_rise_cb_mux[0][1]_rrnode[79]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[79] avg p(Vgvdd_mux_2level_tapbuf_size16[0]) from='start_fall_cb_mux[0][1]_rrnode[79]' to='start_fall_cb_mux[0][1]_rrnode[79]+switch_fall_cb_mux[0][1]_rrnode[79]'
.meas tran sum_leakage_power_mux[0to0] 
+          param='leakage_cb_mux[0][1]_rrnode[79]'
.meas tran sum_energy_per_cycle_mux[0to0] 
+          param='energy_per_cycle_cb_mux[0][1]_rrnode[79]'
******* Normal TYPE loads *******
Xload_inv[0]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[1]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[1] gvdd_load 0 inv size=1
Xload_inv[2]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[2] gvdd_load 0 inv size=1
Xload_inv[3]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[3] gvdd_load 0 inv size=1
Xload_inv[4]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[4] gvdd_load 0 inv size=1
Xload_inv[5]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[5] gvdd_load 0 inv size=1
Xload_inv[6]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[6] gvdd_load 0 inv size=1
Xload_inv[7]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[7] gvdd_load 0 inv size=1
Xload_inv[8]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[8] gvdd_load 0 inv size=1
Xload_inv[9]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[9] gvdd_load 0 inv size=1
Xload_inv[10]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[10] gvdd_load 0 inv size=1
Xload_inv[11]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[11] gvdd_load 0 inv size=1
Xload_inv[12]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[12] gvdd_load 0 inv size=1
Xload_inv[13]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[13] gvdd_load 0 inv size=1
Xload_inv[14]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[14] gvdd_load 0 inv size=1
Xload_inv[15]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[15] gvdd_load 0 inv size=1
Xload_inv[16]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[16] gvdd_load 0 inv size=1
Xload_inv[17]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[17] gvdd_load 0 inv size=1
Xload_inv[18]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[18] gvdd_load 0 inv size=1
Xload_inv[19]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[19] gvdd_load 0 inv size=1
Xload_inv[20]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[20] gvdd_load 0 inv size=1
Xload_inv[21]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[21] gvdd_load 0 inv size=1
Xload_inv[22]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[22] gvdd_load 0 inv size=1
Xload_inv[23]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[23] gvdd_load 0 inv size=1
Xload_inv[24]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[24] gvdd_load 0 inv size=1
Xload_inv[25]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[25] gvdd_load 0 inv size=1
Xload_inv[26]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[26] gvdd_load 0 inv size=1
Xload_inv[27]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[27] gvdd_load 0 inv size=1
Xload_inv[28]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[28] gvdd_load 0 inv size=1
Xload_inv[29]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[29] gvdd_load 0 inv size=1
Xload_inv[30]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[30] gvdd_load 0 inv size=1
Xload_inv[31]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[31] gvdd_load 0 inv size=1
Xload_inv[32]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[32] gvdd_load 0 inv size=1
Xload_inv[33]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[33] gvdd_load 0 inv size=1
Xload_inv[34]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[34] gvdd_load 0 inv size=1
Xload_inv[35]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[35] gvdd_load 0 inv size=1
Xload_inv[36]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[36] gvdd_load 0 inv size=1
Xload_inv[37]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[37] gvdd_load 0 inv size=1
Xload_inv[38]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[38] gvdd_load 0 inv size=1
Xload_inv[39]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[39] gvdd_load 0 inv size=1
Xload_inv[40]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[40] gvdd_load 0 inv size=1
Xload_inv[41]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[41] gvdd_load 0 inv size=1
Xload_inv[42]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[42] gvdd_load 0 inv size=1
Xload_inv[43]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[43] gvdd_load 0 inv size=1
Xload_inv[44]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[44] gvdd_load 0 inv size=1
Xload_inv[45]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[45] gvdd_load 0 inv size=1
Xload_inv[46]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[46] gvdd_load 0 inv size=1
Xload_inv[47]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[47] gvdd_load 0 inv size=1
Xload_inv[48]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[48] gvdd_load 0 inv size=1
Xload_inv[49]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[49] gvdd_load 0 inv size=1
Xload_inv[50]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[50] gvdd_load 0 inv size=1
Xload_inv[51]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[51] gvdd_load 0 inv size=1
Xload_inv[52]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[52] gvdd_load 0 inv size=1
Xload_inv[53]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[53] gvdd_load 0 inv size=1
Xload_inv[54]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[54] gvdd_load 0 inv size=1
Xload_inv[55]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[55] gvdd_load 0 inv size=1
Xload_inv[56]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[56] gvdd_load 0 inv size=1
Xload_inv[57]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[57] gvdd_load 0 inv size=1
Xload_inv[58]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[58] gvdd_load 0 inv size=1
Xload_inv[59]_no0 mux_2level_tapbuf_size16[0]->out mux_2level_tapbuf_size16[0]->out_out[59] gvdd_load 0 inv size=1
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to0] 
+          param='leakage_cb_mux[0][1]_rrnode[79]'
.meas tran sum_energy_per_cycle_cb_mux[0to0] 
+          param='energy_per_cycle_cb_mux[0][1]_rrnode[79]'
Xmux_2level_tapbuf_size16[1] mux_2level_tapbuf_size16[1]->in[0] mux_2level_tapbuf_size16[1]->in[1] mux_2level_tapbuf_size16[1]->in[2] mux_2level_tapbuf_size16[1]->in[3] mux_2level_tapbuf_size16[1]->in[4] mux_2level_tapbuf_size16[1]->in[5] mux_2level_tapbuf_size16[1]->in[6] mux_2level_tapbuf_size16[1]->in[7] mux_2level_tapbuf_size16[1]->in[8] mux_2level_tapbuf_size16[1]->in[9] mux_2level_tapbuf_size16[1]->in[10] mux_2level_tapbuf_size16[1]->in[11] mux_2level_tapbuf_size16[1]->in[12] mux_2level_tapbuf_size16[1]->in[13] mux_2level_tapbuf_size16[1]->in[14] mux_2level_tapbuf_size16[1]->in[15] mux_2level_tapbuf_size16[1]->out sram[8]->outb sram[8]->out sram[9]->out sram[9]->outb sram[10]->out sram[10]->outb sram[11]->out sram[11]->outb sram[12]->outb sram[12]->out sram[13]->out sram[13]->outb sram[14]->out sram[14]->outb sram[15]->out sram[15]->outb gvdd_mux_2level_tapbuf_size16[1] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[1], level=2, select_path_id=0. *****
*****10001000*****
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
***** Signal mux_2level_tapbuf_size16[1]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[0] mux_2level_tapbuf_size16[1]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[1] mux_2level_tapbuf_size16[1]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[2] mux_2level_tapbuf_size16[1]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[3] mux_2level_tapbuf_size16[1]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[4] mux_2level_tapbuf_size16[1]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[5] mux_2level_tapbuf_size16[1]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[6] mux_2level_tapbuf_size16[1]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[7] mux_2level_tapbuf_size16[1]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[8] mux_2level_tapbuf_size16[1]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[9] mux_2level_tapbuf_size16[1]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[10] mux_2level_tapbuf_size16[1]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[11] mux_2level_tapbuf_size16[1]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[12] density = 0.2026, probability=0.4982.*****
Vmux_2level_tapbuf_size16[1]->in[12] mux_2level_tapbuf_size16[1]->in[12] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
***** Signal mux_2level_tapbuf_size16[1]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[13] mux_2level_tapbuf_size16[1]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[14] mux_2level_tapbuf_size16[1]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[1]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[1]->in[15] mux_2level_tapbuf_size16[1]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[1] gvdd_mux_2level_tapbuf_size16[1] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[83] trig v(mux_2level_tapbuf_size16[1]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[1]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[83] trig v(mux_2level_tapbuf_size16[1]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[1]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[83] when v(mux_2level_tapbuf_size16[1]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[83] trig v(mux_2level_tapbuf_size16[1]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[1]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[83] when v(mux_2level_tapbuf_size16[1]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[83] trig v(mux_2level_tapbuf_size16[1]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[1]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[1]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[1]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[83] param='mux_2level_tapbuf_size16[1]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[1]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[1]) from='clock_period' to='6*clock_period'
.meas tran mux_2level_tapbuf_size16[1]_energy_per_cycle param='mux_2level_tapbuf_size16[1]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[83]  param='mux_2level_tapbuf_size16[1]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[83]  param='dynamic_power_cb_mux[0][1]_rrnode[83]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[83] avg p(Vgvdd_mux_2level_tapbuf_size16[1]) from='start_rise_cb_mux[0][1]_rrnode[83]' to='start_rise_cb_mux[0][1]_rrnode[83]+switch_rise_cb_mux[0][1]_rrnode[83]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[83] avg p(Vgvdd_mux_2level_tapbuf_size16[1]) from='start_fall_cb_mux[0][1]_rrnode[83]' to='start_fall_cb_mux[0][1]_rrnode[83]+switch_fall_cb_mux[0][1]_rrnode[83]'
.meas tran sum_leakage_power_mux[0to1] 
+          param='sum_leakage_power_mux[0to0]+leakage_cb_mux[0][1]_rrnode[83]'
.meas tran sum_energy_per_cycle_mux[0to1] 
+          param='sum_energy_per_cycle_mux[0to0]+energy_per_cycle_cb_mux[0][1]_rrnode[83]'
******* Normal TYPE loads *******
Xload_inv[60]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[61]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[1] gvdd_load 0 inv size=1
Xload_inv[62]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[2] gvdd_load 0 inv size=1
Xload_inv[63]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[3] gvdd_load 0 inv size=1
Xload_inv[64]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[4] gvdd_load 0 inv size=1
Xload_inv[65]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[5] gvdd_load 0 inv size=1
Xload_inv[66]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[6] gvdd_load 0 inv size=1
Xload_inv[67]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[7] gvdd_load 0 inv size=1
Xload_inv[68]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[8] gvdd_load 0 inv size=1
Xload_inv[69]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[9] gvdd_load 0 inv size=1
Xload_inv[70]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[10] gvdd_load 0 inv size=1
Xload_inv[71]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[11] gvdd_load 0 inv size=1
Xload_inv[72]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[12] gvdd_load 0 inv size=1
Xload_inv[73]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[13] gvdd_load 0 inv size=1
Xload_inv[74]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[14] gvdd_load 0 inv size=1
Xload_inv[75]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[15] gvdd_load 0 inv size=1
Xload_inv[76]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[16] gvdd_load 0 inv size=1
Xload_inv[77]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[17] gvdd_load 0 inv size=1
Xload_inv[78]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[18] gvdd_load 0 inv size=1
Xload_inv[79]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[19] gvdd_load 0 inv size=1
Xload_inv[80]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[20] gvdd_load 0 inv size=1
Xload_inv[81]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[21] gvdd_load 0 inv size=1
Xload_inv[82]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[22] gvdd_load 0 inv size=1
Xload_inv[83]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[23] gvdd_load 0 inv size=1
Xload_inv[84]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[24] gvdd_load 0 inv size=1
Xload_inv[85]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[25] gvdd_load 0 inv size=1
Xload_inv[86]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[26] gvdd_load 0 inv size=1
Xload_inv[87]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[27] gvdd_load 0 inv size=1
Xload_inv[88]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[28] gvdd_load 0 inv size=1
Xload_inv[89]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[29] gvdd_load 0 inv size=1
Xload_inv[90]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[30] gvdd_load 0 inv size=1
Xload_inv[91]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[31] gvdd_load 0 inv size=1
Xload_inv[92]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[32] gvdd_load 0 inv size=1
Xload_inv[93]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[33] gvdd_load 0 inv size=1
Xload_inv[94]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[34] gvdd_load 0 inv size=1
Xload_inv[95]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[35] gvdd_load 0 inv size=1
Xload_inv[96]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[36] gvdd_load 0 inv size=1
Xload_inv[97]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[37] gvdd_load 0 inv size=1
Xload_inv[98]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[38] gvdd_load 0 inv size=1
Xload_inv[99]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[39] gvdd_load 0 inv size=1
Xload_inv[100]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[40] gvdd_load 0 inv size=1
Xload_inv[101]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[41] gvdd_load 0 inv size=1
Xload_inv[102]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[42] gvdd_load 0 inv size=1
Xload_inv[103]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[43] gvdd_load 0 inv size=1
Xload_inv[104]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[44] gvdd_load 0 inv size=1
Xload_inv[105]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[45] gvdd_load 0 inv size=1
Xload_inv[106]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[46] gvdd_load 0 inv size=1
Xload_inv[107]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[47] gvdd_load 0 inv size=1
Xload_inv[108]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[48] gvdd_load 0 inv size=1
Xload_inv[109]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[49] gvdd_load 0 inv size=1
Xload_inv[110]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[50] gvdd_load 0 inv size=1
Xload_inv[111]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[51] gvdd_load 0 inv size=1
Xload_inv[112]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[52] gvdd_load 0 inv size=1
Xload_inv[113]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[53] gvdd_load 0 inv size=1
Xload_inv[114]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[54] gvdd_load 0 inv size=1
Xload_inv[115]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[55] gvdd_load 0 inv size=1
Xload_inv[116]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[56] gvdd_load 0 inv size=1
Xload_inv[117]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[57] gvdd_load 0 inv size=1
Xload_inv[118]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[58] gvdd_load 0 inv size=1
Xload_inv[119]_no0 mux_2level_tapbuf_size16[1]->out mux_2level_tapbuf_size16[1]->out_out[59] gvdd_load 0 inv size=1
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to1] 
+          param='sum_leakage_power_cb_mux[0to0]+leakage_cb_mux[0][1]_rrnode[83]'
.meas tran sum_energy_per_cycle_cb_mux[0to1] 
+          param='sum_energy_per_cycle_cb_mux[0to0]+energy_per_cycle_cb_mux[0][1]_rrnode[83]'
Xmux_2level_tapbuf_size16[2] mux_2level_tapbuf_size16[2]->in[0] mux_2level_tapbuf_size16[2]->in[1] mux_2level_tapbuf_size16[2]->in[2] mux_2level_tapbuf_size16[2]->in[3] mux_2level_tapbuf_size16[2]->in[4] mux_2level_tapbuf_size16[2]->in[5] mux_2level_tapbuf_size16[2]->in[6] mux_2level_tapbuf_size16[2]->in[7] mux_2level_tapbuf_size16[2]->in[8] mux_2level_tapbuf_size16[2]->in[9] mux_2level_tapbuf_size16[2]->in[10] mux_2level_tapbuf_size16[2]->in[11] mux_2level_tapbuf_size16[2]->in[12] mux_2level_tapbuf_size16[2]->in[13] mux_2level_tapbuf_size16[2]->in[14] mux_2level_tapbuf_size16[2]->in[15] mux_2level_tapbuf_size16[2]->out sram[16]->outb sram[16]->out sram[17]->out sram[17]->outb sram[18]->out sram[18]->outb sram[19]->out sram[19]->outb sram[20]->outb sram[20]->out sram[21]->out sram[21]->outb sram[22]->out sram[22]->outb sram[23]->out sram[23]->outb gvdd_mux_2level_tapbuf_size16[2] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[2], level=2, select_path_id=0. *****
*****10001000*****
Xsram[16] sram->in sram[16]->out sram[16]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[16]->out) 0
.nodeset V(sram[16]->outb) vsp
Xsram[17] sram->in sram[17]->out sram[17]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[17]->out) 0
.nodeset V(sram[17]->outb) vsp
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
***** Signal mux_2level_tapbuf_size16[2]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[0] mux_2level_tapbuf_size16[2]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[1] mux_2level_tapbuf_size16[2]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[2] mux_2level_tapbuf_size16[2]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[3] mux_2level_tapbuf_size16[2]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[4] mux_2level_tapbuf_size16[2]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[5] mux_2level_tapbuf_size16[2]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[6] mux_2level_tapbuf_size16[2]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[7] mux_2level_tapbuf_size16[2]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[8] mux_2level_tapbuf_size16[2]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[9] mux_2level_tapbuf_size16[2]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[10] mux_2level_tapbuf_size16[2]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[11] mux_2level_tapbuf_size16[2]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[12] mux_2level_tapbuf_size16[2]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[13] mux_2level_tapbuf_size16[2]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[14] mux_2level_tapbuf_size16[2]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[2]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[2]->in[15] mux_2level_tapbuf_size16[2]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[2] gvdd_mux_2level_tapbuf_size16[2] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[87] trig v(mux_2level_tapbuf_size16[2]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[2]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[87] trig v(mux_2level_tapbuf_size16[2]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[2]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[87] when v(mux_2level_tapbuf_size16[2]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[87] trig v(mux_2level_tapbuf_size16[2]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[2]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[87] when v(mux_2level_tapbuf_size16[2]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[87] trig v(mux_2level_tapbuf_size16[2]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[2]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[2]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[2]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[87] param='mux_2level_tapbuf_size16[2]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[2]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[2]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[2]_energy_per_cycle param='mux_2level_tapbuf_size16[2]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[87]  param='mux_2level_tapbuf_size16[2]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[87]  param='dynamic_power_cb_mux[0][1]_rrnode[87]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[87] avg p(Vgvdd_mux_2level_tapbuf_size16[2]) from='start_rise_cb_mux[0][1]_rrnode[87]' to='start_rise_cb_mux[0][1]_rrnode[87]+switch_rise_cb_mux[0][1]_rrnode[87]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[87] avg p(Vgvdd_mux_2level_tapbuf_size16[2]) from='start_fall_cb_mux[0][1]_rrnode[87]' to='start_fall_cb_mux[0][1]_rrnode[87]+switch_fall_cb_mux[0][1]_rrnode[87]'
.meas tran sum_leakage_power_mux[0to2] 
+          param='sum_leakage_power_mux[0to1]+leakage_cb_mux[0][1]_rrnode[87]'
.meas tran sum_energy_per_cycle_mux[0to2] 
+          param='sum_energy_per_cycle_mux[0to1]+energy_per_cycle_cb_mux[0][1]_rrnode[87]'
******* Normal TYPE loads *******
Xload_inv[120]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[121]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[1] gvdd_load 0 inv size=1
Xload_inv[122]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[2] gvdd_load 0 inv size=1
Xload_inv[123]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[3] gvdd_load 0 inv size=1
Xload_inv[124]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[4] gvdd_load 0 inv size=1
Xload_inv[125]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[5] gvdd_load 0 inv size=1
Xload_inv[126]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[6] gvdd_load 0 inv size=1
Xload_inv[127]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[7] gvdd_load 0 inv size=1
Xload_inv[128]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[8] gvdd_load 0 inv size=1
Xload_inv[129]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[9] gvdd_load 0 inv size=1
Xload_inv[130]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[10] gvdd_load 0 inv size=1
Xload_inv[131]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[11] gvdd_load 0 inv size=1
Xload_inv[132]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[12] gvdd_load 0 inv size=1
Xload_inv[133]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[13] gvdd_load 0 inv size=1
Xload_inv[134]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[14] gvdd_load 0 inv size=1
Xload_inv[135]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[15] gvdd_load 0 inv size=1
Xload_inv[136]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[16] gvdd_load 0 inv size=1
Xload_inv[137]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[17] gvdd_load 0 inv size=1
Xload_inv[138]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[18] gvdd_load 0 inv size=1
Xload_inv[139]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[19] gvdd_load 0 inv size=1
Xload_inv[140]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[20] gvdd_load 0 inv size=1
Xload_inv[141]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[21] gvdd_load 0 inv size=1
Xload_inv[142]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[22] gvdd_load 0 inv size=1
Xload_inv[143]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[23] gvdd_load 0 inv size=1
Xload_inv[144]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[24] gvdd_load 0 inv size=1
Xload_inv[145]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[25] gvdd_load 0 inv size=1
Xload_inv[146]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[26] gvdd_load 0 inv size=1
Xload_inv[147]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[27] gvdd_load 0 inv size=1
Xload_inv[148]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[28] gvdd_load 0 inv size=1
Xload_inv[149]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[29] gvdd_load 0 inv size=1
Xload_inv[150]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[30] gvdd_load 0 inv size=1
Xload_inv[151]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[31] gvdd_load 0 inv size=1
Xload_inv[152]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[32] gvdd_load 0 inv size=1
Xload_inv[153]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[33] gvdd_load 0 inv size=1
Xload_inv[154]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[34] gvdd_load 0 inv size=1
Xload_inv[155]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[35] gvdd_load 0 inv size=1
Xload_inv[156]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[36] gvdd_load 0 inv size=1
Xload_inv[157]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[37] gvdd_load 0 inv size=1
Xload_inv[158]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[38] gvdd_load 0 inv size=1
Xload_inv[159]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[39] gvdd_load 0 inv size=1
Xload_inv[160]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[40] gvdd_load 0 inv size=1
Xload_inv[161]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[41] gvdd_load 0 inv size=1
Xload_inv[162]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[42] gvdd_load 0 inv size=1
Xload_inv[163]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[43] gvdd_load 0 inv size=1
Xload_inv[164]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[44] gvdd_load 0 inv size=1
Xload_inv[165]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[45] gvdd_load 0 inv size=1
Xload_inv[166]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[46] gvdd_load 0 inv size=1
Xload_inv[167]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[47] gvdd_load 0 inv size=1
Xload_inv[168]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[48] gvdd_load 0 inv size=1
Xload_inv[169]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[49] gvdd_load 0 inv size=1
Xload_inv[170]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[50] gvdd_load 0 inv size=1
Xload_inv[171]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[51] gvdd_load 0 inv size=1
Xload_inv[172]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[52] gvdd_load 0 inv size=1
Xload_inv[173]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[53] gvdd_load 0 inv size=1
Xload_inv[174]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[54] gvdd_load 0 inv size=1
Xload_inv[175]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[55] gvdd_load 0 inv size=1
Xload_inv[176]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[56] gvdd_load 0 inv size=1
Xload_inv[177]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[57] gvdd_load 0 inv size=1
Xload_inv[178]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[58] gvdd_load 0 inv size=1
Xload_inv[179]_no0 mux_2level_tapbuf_size16[2]->out mux_2level_tapbuf_size16[2]->out_out[59] gvdd_load 0 inv size=1
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to2] 
+          param='sum_leakage_power_cb_mux[0to1]+leakage_cb_mux[0][1]_rrnode[87]'
.meas tran sum_energy_per_cycle_cb_mux[0to2] 
+          param='sum_energy_per_cycle_cb_mux[0to1]+energy_per_cycle_cb_mux[0][1]_rrnode[87]'
Xmux_2level_tapbuf_size16[3] mux_2level_tapbuf_size16[3]->in[0] mux_2level_tapbuf_size16[3]->in[1] mux_2level_tapbuf_size16[3]->in[2] mux_2level_tapbuf_size16[3]->in[3] mux_2level_tapbuf_size16[3]->in[4] mux_2level_tapbuf_size16[3]->in[5] mux_2level_tapbuf_size16[3]->in[6] mux_2level_tapbuf_size16[3]->in[7] mux_2level_tapbuf_size16[3]->in[8] mux_2level_tapbuf_size16[3]->in[9] mux_2level_tapbuf_size16[3]->in[10] mux_2level_tapbuf_size16[3]->in[11] mux_2level_tapbuf_size16[3]->in[12] mux_2level_tapbuf_size16[3]->in[13] mux_2level_tapbuf_size16[3]->in[14] mux_2level_tapbuf_size16[3]->in[15] mux_2level_tapbuf_size16[3]->out sram[24]->outb sram[24]->out sram[25]->out sram[25]->outb sram[26]->out sram[26]->outb sram[27]->out sram[27]->outb sram[28]->outb sram[28]->out sram[29]->out sram[29]->outb sram[30]->out sram[30]->outb sram[31]->out sram[31]->outb gvdd_mux_2level_tapbuf_size16[3] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[3], level=2, select_path_id=0. *****
*****10001000*****
Xsram[24] sram->in sram[24]->out sram[24]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[24]->out) 0
.nodeset V(sram[24]->outb) vsp
Xsram[25] sram->in sram[25]->out sram[25]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[25]->out) 0
.nodeset V(sram[25]->outb) vsp
Xsram[26] sram->in sram[26]->out sram[26]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[26]->out) 0
.nodeset V(sram[26]->outb) vsp
Xsram[27] sram->in sram[27]->out sram[27]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[27]->out) 0
.nodeset V(sram[27]->outb) vsp
Xsram[28] sram->in sram[28]->out sram[28]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[28]->out) 0
.nodeset V(sram[28]->outb) vsp
Xsram[29] sram->in sram[29]->out sram[29]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[29]->out) 0
.nodeset V(sram[29]->outb) vsp
Xsram[30] sram->in sram[30]->out sram[30]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[30]->out) 0
.nodeset V(sram[30]->outb) vsp
Xsram[31] sram->in sram[31]->out sram[31]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[31]->out) 0
.nodeset V(sram[31]->outb) vsp
***** Signal mux_2level_tapbuf_size16[3]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[0] mux_2level_tapbuf_size16[3]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[1] mux_2level_tapbuf_size16[3]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[2] mux_2level_tapbuf_size16[3]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[3] mux_2level_tapbuf_size16[3]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[4] mux_2level_tapbuf_size16[3]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[5] mux_2level_tapbuf_size16[3]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[6] mux_2level_tapbuf_size16[3]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[7] mux_2level_tapbuf_size16[3]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[8] mux_2level_tapbuf_size16[3]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[9] mux_2level_tapbuf_size16[3]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[10] mux_2level_tapbuf_size16[3]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[11] mux_2level_tapbuf_size16[3]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[12] mux_2level_tapbuf_size16[3]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[13] mux_2level_tapbuf_size16[3]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[14] mux_2level_tapbuf_size16[3]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[3]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[3]->in[15] mux_2level_tapbuf_size16[3]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[3] gvdd_mux_2level_tapbuf_size16[3] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[91] trig v(mux_2level_tapbuf_size16[3]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[3]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[91] trig v(mux_2level_tapbuf_size16[3]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[3]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[91] when v(mux_2level_tapbuf_size16[3]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[91] trig v(mux_2level_tapbuf_size16[3]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[3]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[91] when v(mux_2level_tapbuf_size16[3]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[91] trig v(mux_2level_tapbuf_size16[3]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[3]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[3]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[3]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[91] param='mux_2level_tapbuf_size16[3]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[3]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[3]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[3]_energy_per_cycle param='mux_2level_tapbuf_size16[3]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[91]  param='mux_2level_tapbuf_size16[3]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[91]  param='dynamic_power_cb_mux[0][1]_rrnode[91]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[91] avg p(Vgvdd_mux_2level_tapbuf_size16[3]) from='start_rise_cb_mux[0][1]_rrnode[91]' to='start_rise_cb_mux[0][1]_rrnode[91]+switch_rise_cb_mux[0][1]_rrnode[91]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[91] avg p(Vgvdd_mux_2level_tapbuf_size16[3]) from='start_fall_cb_mux[0][1]_rrnode[91]' to='start_fall_cb_mux[0][1]_rrnode[91]+switch_fall_cb_mux[0][1]_rrnode[91]'
.meas tran sum_leakage_power_mux[0to3] 
+          param='sum_leakage_power_mux[0to2]+leakage_cb_mux[0][1]_rrnode[91]'
.meas tran sum_energy_per_cycle_mux[0to3] 
+          param='sum_energy_per_cycle_mux[0to2]+energy_per_cycle_cb_mux[0][1]_rrnode[91]'
******* Normal TYPE loads *******
Xload_inv[180]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[181]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[1] gvdd_load 0 inv size=1
Xload_inv[182]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[2] gvdd_load 0 inv size=1
Xload_inv[183]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[3] gvdd_load 0 inv size=1
Xload_inv[184]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[4] gvdd_load 0 inv size=1
Xload_inv[185]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[5] gvdd_load 0 inv size=1
Xload_inv[186]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[6] gvdd_load 0 inv size=1
Xload_inv[187]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[7] gvdd_load 0 inv size=1
Xload_inv[188]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[8] gvdd_load 0 inv size=1
Xload_inv[189]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[9] gvdd_load 0 inv size=1
Xload_inv[190]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[10] gvdd_load 0 inv size=1
Xload_inv[191]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[11] gvdd_load 0 inv size=1
Xload_inv[192]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[12] gvdd_load 0 inv size=1
Xload_inv[193]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[13] gvdd_load 0 inv size=1
Xload_inv[194]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[14] gvdd_load 0 inv size=1
Xload_inv[195]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[15] gvdd_load 0 inv size=1
Xload_inv[196]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[16] gvdd_load 0 inv size=1
Xload_inv[197]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[17] gvdd_load 0 inv size=1
Xload_inv[198]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[18] gvdd_load 0 inv size=1
Xload_inv[199]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[19] gvdd_load 0 inv size=1
Xload_inv[200]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[20] gvdd_load 0 inv size=1
Xload_inv[201]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[21] gvdd_load 0 inv size=1
Xload_inv[202]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[22] gvdd_load 0 inv size=1
Xload_inv[203]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[23] gvdd_load 0 inv size=1
Xload_inv[204]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[24] gvdd_load 0 inv size=1
Xload_inv[205]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[25] gvdd_load 0 inv size=1
Xload_inv[206]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[26] gvdd_load 0 inv size=1
Xload_inv[207]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[27] gvdd_load 0 inv size=1
Xload_inv[208]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[28] gvdd_load 0 inv size=1
Xload_inv[209]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[29] gvdd_load 0 inv size=1
Xload_inv[210]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[30] gvdd_load 0 inv size=1
Xload_inv[211]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[31] gvdd_load 0 inv size=1
Xload_inv[212]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[32] gvdd_load 0 inv size=1
Xload_inv[213]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[33] gvdd_load 0 inv size=1
Xload_inv[214]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[34] gvdd_load 0 inv size=1
Xload_inv[215]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[35] gvdd_load 0 inv size=1
Xload_inv[216]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[36] gvdd_load 0 inv size=1
Xload_inv[217]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[37] gvdd_load 0 inv size=1
Xload_inv[218]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[38] gvdd_load 0 inv size=1
Xload_inv[219]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[39] gvdd_load 0 inv size=1
Xload_inv[220]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[40] gvdd_load 0 inv size=1
Xload_inv[221]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[41] gvdd_load 0 inv size=1
Xload_inv[222]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[42] gvdd_load 0 inv size=1
Xload_inv[223]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[43] gvdd_load 0 inv size=1
Xload_inv[224]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[44] gvdd_load 0 inv size=1
Xload_inv[225]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[45] gvdd_load 0 inv size=1
Xload_inv[226]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[46] gvdd_load 0 inv size=1
Xload_inv[227]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[47] gvdd_load 0 inv size=1
Xload_inv[228]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[48] gvdd_load 0 inv size=1
Xload_inv[229]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[49] gvdd_load 0 inv size=1
Xload_inv[230]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[50] gvdd_load 0 inv size=1
Xload_inv[231]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[51] gvdd_load 0 inv size=1
Xload_inv[232]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[52] gvdd_load 0 inv size=1
Xload_inv[233]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[53] gvdd_load 0 inv size=1
Xload_inv[234]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[54] gvdd_load 0 inv size=1
Xload_inv[235]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[55] gvdd_load 0 inv size=1
Xload_inv[236]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[56] gvdd_load 0 inv size=1
Xload_inv[237]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[57] gvdd_load 0 inv size=1
Xload_inv[238]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[58] gvdd_load 0 inv size=1
Xload_inv[239]_no0 mux_2level_tapbuf_size16[3]->out mux_2level_tapbuf_size16[3]->out_out[59] gvdd_load 0 inv size=1
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to3] 
+          param='sum_leakage_power_cb_mux[0to2]+leakage_cb_mux[0][1]_rrnode[91]'
.meas tran sum_energy_per_cycle_cb_mux[0to3] 
+          param='sum_energy_per_cycle_cb_mux[0to2]+energy_per_cycle_cb_mux[0][1]_rrnode[91]'
Xmux_2level_tapbuf_size16[4] mux_2level_tapbuf_size16[4]->in[0] mux_2level_tapbuf_size16[4]->in[1] mux_2level_tapbuf_size16[4]->in[2] mux_2level_tapbuf_size16[4]->in[3] mux_2level_tapbuf_size16[4]->in[4] mux_2level_tapbuf_size16[4]->in[5] mux_2level_tapbuf_size16[4]->in[6] mux_2level_tapbuf_size16[4]->in[7] mux_2level_tapbuf_size16[4]->in[8] mux_2level_tapbuf_size16[4]->in[9] mux_2level_tapbuf_size16[4]->in[10] mux_2level_tapbuf_size16[4]->in[11] mux_2level_tapbuf_size16[4]->in[12] mux_2level_tapbuf_size16[4]->in[13] mux_2level_tapbuf_size16[4]->in[14] mux_2level_tapbuf_size16[4]->in[15] mux_2level_tapbuf_size16[4]->out sram[32]->outb sram[32]->out sram[33]->out sram[33]->outb sram[34]->out sram[34]->outb sram[35]->out sram[35]->outb sram[36]->outb sram[36]->out sram[37]->out sram[37]->outb sram[38]->out sram[38]->outb sram[39]->out sram[39]->outb gvdd_mux_2level_tapbuf_size16[4] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[4], level=2, select_path_id=0. *****
*****10001000*****
Xsram[32] sram->in sram[32]->out sram[32]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[32]->out) 0
.nodeset V(sram[32]->outb) vsp
Xsram[33] sram->in sram[33]->out sram[33]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[33]->out) 0
.nodeset V(sram[33]->outb) vsp
Xsram[34] sram->in sram[34]->out sram[34]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[34]->out) 0
.nodeset V(sram[34]->outb) vsp
Xsram[35] sram->in sram[35]->out sram[35]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[35]->out) 0
.nodeset V(sram[35]->outb) vsp
Xsram[36] sram->in sram[36]->out sram[36]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[36]->out) 0
.nodeset V(sram[36]->outb) vsp
Xsram[37] sram->in sram[37]->out sram[37]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[37]->out) 0
.nodeset V(sram[37]->outb) vsp
Xsram[38] sram->in sram[38]->out sram[38]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[38]->out) 0
.nodeset V(sram[38]->outb) vsp
Xsram[39] sram->in sram[39]->out sram[39]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[39]->out) 0
.nodeset V(sram[39]->outb) vsp
***** Signal mux_2level_tapbuf_size16[4]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[0] mux_2level_tapbuf_size16[4]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[1] mux_2level_tapbuf_size16[4]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[2] mux_2level_tapbuf_size16[4]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[3] mux_2level_tapbuf_size16[4]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[4] mux_2level_tapbuf_size16[4]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[5] mux_2level_tapbuf_size16[4]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[6] mux_2level_tapbuf_size16[4]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[7] mux_2level_tapbuf_size16[4]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[8] mux_2level_tapbuf_size16[4]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[9] mux_2level_tapbuf_size16[4]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[10] mux_2level_tapbuf_size16[4]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[11] mux_2level_tapbuf_size16[4]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[12] mux_2level_tapbuf_size16[4]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[13] mux_2level_tapbuf_size16[4]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[14] mux_2level_tapbuf_size16[4]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[4]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[4]->in[15] mux_2level_tapbuf_size16[4]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[4] gvdd_mux_2level_tapbuf_size16[4] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[95] trig v(mux_2level_tapbuf_size16[4]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[4]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[95] trig v(mux_2level_tapbuf_size16[4]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[4]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[95] when v(mux_2level_tapbuf_size16[4]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[95] trig v(mux_2level_tapbuf_size16[4]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[4]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[95] when v(mux_2level_tapbuf_size16[4]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[95] trig v(mux_2level_tapbuf_size16[4]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[4]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[4]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[4]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[95] param='mux_2level_tapbuf_size16[4]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[4]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[4]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[4]_energy_per_cycle param='mux_2level_tapbuf_size16[4]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[95]  param='mux_2level_tapbuf_size16[4]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[95]  param='dynamic_power_cb_mux[0][1]_rrnode[95]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[95] avg p(Vgvdd_mux_2level_tapbuf_size16[4]) from='start_rise_cb_mux[0][1]_rrnode[95]' to='start_rise_cb_mux[0][1]_rrnode[95]+switch_rise_cb_mux[0][1]_rrnode[95]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[95] avg p(Vgvdd_mux_2level_tapbuf_size16[4]) from='start_fall_cb_mux[0][1]_rrnode[95]' to='start_fall_cb_mux[0][1]_rrnode[95]+switch_fall_cb_mux[0][1]_rrnode[95]'
.meas tran sum_leakage_power_mux[0to4] 
+          param='sum_leakage_power_mux[0to3]+leakage_cb_mux[0][1]_rrnode[95]'
.meas tran sum_energy_per_cycle_mux[0to4] 
+          param='sum_energy_per_cycle_mux[0to3]+energy_per_cycle_cb_mux[0][1]_rrnode[95]'
******* Normal TYPE loads *******
Xload_inv[240]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[241]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[1] gvdd_load 0 inv size=1
Xload_inv[242]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[2] gvdd_load 0 inv size=1
Xload_inv[243]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[3] gvdd_load 0 inv size=1
Xload_inv[244]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[4] gvdd_load 0 inv size=1
Xload_inv[245]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[5] gvdd_load 0 inv size=1
Xload_inv[246]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[6] gvdd_load 0 inv size=1
Xload_inv[247]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[7] gvdd_load 0 inv size=1
Xload_inv[248]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[8] gvdd_load 0 inv size=1
Xload_inv[249]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[9] gvdd_load 0 inv size=1
Xload_inv[250]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[10] gvdd_load 0 inv size=1
Xload_inv[251]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[11] gvdd_load 0 inv size=1
Xload_inv[252]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[12] gvdd_load 0 inv size=1
Xload_inv[253]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[13] gvdd_load 0 inv size=1
Xload_inv[254]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[14] gvdd_load 0 inv size=1
Xload_inv[255]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[15] gvdd_load 0 inv size=1
Xload_inv[256]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[16] gvdd_load 0 inv size=1
Xload_inv[257]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[17] gvdd_load 0 inv size=1
Xload_inv[258]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[18] gvdd_load 0 inv size=1
Xload_inv[259]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[19] gvdd_load 0 inv size=1
Xload_inv[260]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[20] gvdd_load 0 inv size=1
Xload_inv[261]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[21] gvdd_load 0 inv size=1
Xload_inv[262]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[22] gvdd_load 0 inv size=1
Xload_inv[263]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[23] gvdd_load 0 inv size=1
Xload_inv[264]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[24] gvdd_load 0 inv size=1
Xload_inv[265]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[25] gvdd_load 0 inv size=1
Xload_inv[266]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[26] gvdd_load 0 inv size=1
Xload_inv[267]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[27] gvdd_load 0 inv size=1
Xload_inv[268]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[28] gvdd_load 0 inv size=1
Xload_inv[269]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[29] gvdd_load 0 inv size=1
Xload_inv[270]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[30] gvdd_load 0 inv size=1
Xload_inv[271]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[31] gvdd_load 0 inv size=1
Xload_inv[272]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[32] gvdd_load 0 inv size=1
Xload_inv[273]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[33] gvdd_load 0 inv size=1
Xload_inv[274]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[34] gvdd_load 0 inv size=1
Xload_inv[275]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[35] gvdd_load 0 inv size=1
Xload_inv[276]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[36] gvdd_load 0 inv size=1
Xload_inv[277]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[37] gvdd_load 0 inv size=1
Xload_inv[278]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[38] gvdd_load 0 inv size=1
Xload_inv[279]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[39] gvdd_load 0 inv size=1
Xload_inv[280]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[40] gvdd_load 0 inv size=1
Xload_inv[281]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[41] gvdd_load 0 inv size=1
Xload_inv[282]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[42] gvdd_load 0 inv size=1
Xload_inv[283]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[43] gvdd_load 0 inv size=1
Xload_inv[284]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[44] gvdd_load 0 inv size=1
Xload_inv[285]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[45] gvdd_load 0 inv size=1
Xload_inv[286]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[46] gvdd_load 0 inv size=1
Xload_inv[287]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[47] gvdd_load 0 inv size=1
Xload_inv[288]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[48] gvdd_load 0 inv size=1
Xload_inv[289]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[49] gvdd_load 0 inv size=1
Xload_inv[290]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[50] gvdd_load 0 inv size=1
Xload_inv[291]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[51] gvdd_load 0 inv size=1
Xload_inv[292]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[52] gvdd_load 0 inv size=1
Xload_inv[293]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[53] gvdd_load 0 inv size=1
Xload_inv[294]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[54] gvdd_load 0 inv size=1
Xload_inv[295]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[55] gvdd_load 0 inv size=1
Xload_inv[296]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[56] gvdd_load 0 inv size=1
Xload_inv[297]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[57] gvdd_load 0 inv size=1
Xload_inv[298]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[58] gvdd_load 0 inv size=1
Xload_inv[299]_no0 mux_2level_tapbuf_size16[4]->out mux_2level_tapbuf_size16[4]->out_out[59] gvdd_load 0 inv size=1
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to4] 
+          param='sum_leakage_power_cb_mux[0to3]+leakage_cb_mux[0][1]_rrnode[95]'
.meas tran sum_energy_per_cycle_cb_mux[0to4] 
+          param='sum_energy_per_cycle_cb_mux[0to3]+energy_per_cycle_cb_mux[0][1]_rrnode[95]'
Xmux_2level_tapbuf_size16[5] mux_2level_tapbuf_size16[5]->in[0] mux_2level_tapbuf_size16[5]->in[1] mux_2level_tapbuf_size16[5]->in[2] mux_2level_tapbuf_size16[5]->in[3] mux_2level_tapbuf_size16[5]->in[4] mux_2level_tapbuf_size16[5]->in[5] mux_2level_tapbuf_size16[5]->in[6] mux_2level_tapbuf_size16[5]->in[7] mux_2level_tapbuf_size16[5]->in[8] mux_2level_tapbuf_size16[5]->in[9] mux_2level_tapbuf_size16[5]->in[10] mux_2level_tapbuf_size16[5]->in[11] mux_2level_tapbuf_size16[5]->in[12] mux_2level_tapbuf_size16[5]->in[13] mux_2level_tapbuf_size16[5]->in[14] mux_2level_tapbuf_size16[5]->in[15] mux_2level_tapbuf_size16[5]->out sram[40]->outb sram[40]->out sram[41]->out sram[41]->outb sram[42]->out sram[42]->outb sram[43]->out sram[43]->outb sram[44]->outb sram[44]->out sram[45]->out sram[45]->outb sram[46]->out sram[46]->outb sram[47]->out sram[47]->outb gvdd_mux_2level_tapbuf_size16[5] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[5], level=2, select_path_id=0. *****
*****10001000*****
Xsram[40] sram->in sram[40]->out sram[40]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[40]->out) 0
.nodeset V(sram[40]->outb) vsp
Xsram[41] sram->in sram[41]->out sram[41]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[41]->out) 0
.nodeset V(sram[41]->outb) vsp
Xsram[42] sram->in sram[42]->out sram[42]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[42]->out) 0
.nodeset V(sram[42]->outb) vsp
Xsram[43] sram->in sram[43]->out sram[43]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[43]->out) 0
.nodeset V(sram[43]->outb) vsp
Xsram[44] sram->in sram[44]->out sram[44]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[44]->out) 0
.nodeset V(sram[44]->outb) vsp
Xsram[45] sram->in sram[45]->out sram[45]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[45]->out) 0
.nodeset V(sram[45]->outb) vsp
Xsram[46] sram->in sram[46]->out sram[46]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[46]->out) 0
.nodeset V(sram[46]->outb) vsp
Xsram[47] sram->in sram[47]->out sram[47]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[47]->out) 0
.nodeset V(sram[47]->outb) vsp
***** Signal mux_2level_tapbuf_size16[5]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[0] mux_2level_tapbuf_size16[5]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[1] mux_2level_tapbuf_size16[5]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[2] mux_2level_tapbuf_size16[5]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[3] mux_2level_tapbuf_size16[5]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[4] mux_2level_tapbuf_size16[5]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[5] mux_2level_tapbuf_size16[5]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[6] mux_2level_tapbuf_size16[5]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[7] mux_2level_tapbuf_size16[5]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[8] mux_2level_tapbuf_size16[5]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[9] mux_2level_tapbuf_size16[5]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[10] mux_2level_tapbuf_size16[5]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[11] mux_2level_tapbuf_size16[5]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[12] mux_2level_tapbuf_size16[5]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[13] mux_2level_tapbuf_size16[5]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[14] mux_2level_tapbuf_size16[5]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[5]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[5]->in[15] mux_2level_tapbuf_size16[5]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[5] gvdd_mux_2level_tapbuf_size16[5] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[99] trig v(mux_2level_tapbuf_size16[5]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[5]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[99] trig v(mux_2level_tapbuf_size16[5]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[5]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[99] when v(mux_2level_tapbuf_size16[5]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[99] trig v(mux_2level_tapbuf_size16[5]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[5]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[99] when v(mux_2level_tapbuf_size16[5]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[99] trig v(mux_2level_tapbuf_size16[5]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[5]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[5]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[5]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[99] param='mux_2level_tapbuf_size16[5]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[5]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[5]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[5]_energy_per_cycle param='mux_2level_tapbuf_size16[5]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[99]  param='mux_2level_tapbuf_size16[5]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[99]  param='dynamic_power_cb_mux[0][1]_rrnode[99]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[99] avg p(Vgvdd_mux_2level_tapbuf_size16[5]) from='start_rise_cb_mux[0][1]_rrnode[99]' to='start_rise_cb_mux[0][1]_rrnode[99]+switch_rise_cb_mux[0][1]_rrnode[99]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[99] avg p(Vgvdd_mux_2level_tapbuf_size16[5]) from='start_fall_cb_mux[0][1]_rrnode[99]' to='start_fall_cb_mux[0][1]_rrnode[99]+switch_fall_cb_mux[0][1]_rrnode[99]'
.meas tran sum_leakage_power_mux[0to5] 
+          param='sum_leakage_power_mux[0to4]+leakage_cb_mux[0][1]_rrnode[99]'
.meas tran sum_energy_per_cycle_mux[0to5] 
+          param='sum_energy_per_cycle_mux[0to4]+energy_per_cycle_cb_mux[0][1]_rrnode[99]'
******* Normal TYPE loads *******
Xload_inv[300]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[301]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[1] gvdd_load 0 inv size=1
Xload_inv[302]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[2] gvdd_load 0 inv size=1
Xload_inv[303]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[3] gvdd_load 0 inv size=1
Xload_inv[304]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[4] gvdd_load 0 inv size=1
Xload_inv[305]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[5] gvdd_load 0 inv size=1
Xload_inv[306]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[6] gvdd_load 0 inv size=1
Xload_inv[307]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[7] gvdd_load 0 inv size=1
Xload_inv[308]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[8] gvdd_load 0 inv size=1
Xload_inv[309]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[9] gvdd_load 0 inv size=1
Xload_inv[310]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[10] gvdd_load 0 inv size=1
Xload_inv[311]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[11] gvdd_load 0 inv size=1
Xload_inv[312]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[12] gvdd_load 0 inv size=1
Xload_inv[313]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[13] gvdd_load 0 inv size=1
Xload_inv[314]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[14] gvdd_load 0 inv size=1
Xload_inv[315]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[15] gvdd_load 0 inv size=1
Xload_inv[316]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[16] gvdd_load 0 inv size=1
Xload_inv[317]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[17] gvdd_load 0 inv size=1
Xload_inv[318]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[18] gvdd_load 0 inv size=1
Xload_inv[319]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[19] gvdd_load 0 inv size=1
Xload_inv[320]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[20] gvdd_load 0 inv size=1
Xload_inv[321]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[21] gvdd_load 0 inv size=1
Xload_inv[322]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[22] gvdd_load 0 inv size=1
Xload_inv[323]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[23] gvdd_load 0 inv size=1
Xload_inv[324]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[24] gvdd_load 0 inv size=1
Xload_inv[325]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[25] gvdd_load 0 inv size=1
Xload_inv[326]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[26] gvdd_load 0 inv size=1
Xload_inv[327]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[27] gvdd_load 0 inv size=1
Xload_inv[328]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[28] gvdd_load 0 inv size=1
Xload_inv[329]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[29] gvdd_load 0 inv size=1
Xload_inv[330]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[30] gvdd_load 0 inv size=1
Xload_inv[331]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[31] gvdd_load 0 inv size=1
Xload_inv[332]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[32] gvdd_load 0 inv size=1
Xload_inv[333]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[33] gvdd_load 0 inv size=1
Xload_inv[334]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[34] gvdd_load 0 inv size=1
Xload_inv[335]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[35] gvdd_load 0 inv size=1
Xload_inv[336]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[36] gvdd_load 0 inv size=1
Xload_inv[337]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[37] gvdd_load 0 inv size=1
Xload_inv[338]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[38] gvdd_load 0 inv size=1
Xload_inv[339]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[39] gvdd_load 0 inv size=1
Xload_inv[340]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[40] gvdd_load 0 inv size=1
Xload_inv[341]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[41] gvdd_load 0 inv size=1
Xload_inv[342]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[42] gvdd_load 0 inv size=1
Xload_inv[343]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[43] gvdd_load 0 inv size=1
Xload_inv[344]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[44] gvdd_load 0 inv size=1
Xload_inv[345]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[45] gvdd_load 0 inv size=1
Xload_inv[346]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[46] gvdd_load 0 inv size=1
Xload_inv[347]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[47] gvdd_load 0 inv size=1
Xload_inv[348]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[48] gvdd_load 0 inv size=1
Xload_inv[349]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[49] gvdd_load 0 inv size=1
Xload_inv[350]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[50] gvdd_load 0 inv size=1
Xload_inv[351]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[51] gvdd_load 0 inv size=1
Xload_inv[352]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[52] gvdd_load 0 inv size=1
Xload_inv[353]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[53] gvdd_load 0 inv size=1
Xload_inv[354]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[54] gvdd_load 0 inv size=1
Xload_inv[355]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[55] gvdd_load 0 inv size=1
Xload_inv[356]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[56] gvdd_load 0 inv size=1
Xload_inv[357]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[57] gvdd_load 0 inv size=1
Xload_inv[358]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[58] gvdd_load 0 inv size=1
Xload_inv[359]_no0 mux_2level_tapbuf_size16[5]->out mux_2level_tapbuf_size16[5]->out_out[59] gvdd_load 0 inv size=1
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to5] 
+          param='sum_leakage_power_cb_mux[0to4]+leakage_cb_mux[0][1]_rrnode[99]'
.meas tran sum_energy_per_cycle_cb_mux[0to5] 
+          param='sum_energy_per_cycle_cb_mux[0to4]+energy_per_cycle_cb_mux[0][1]_rrnode[99]'
Xmux_2level_tapbuf_size16[6] mux_2level_tapbuf_size16[6]->in[0] mux_2level_tapbuf_size16[6]->in[1] mux_2level_tapbuf_size16[6]->in[2] mux_2level_tapbuf_size16[6]->in[3] mux_2level_tapbuf_size16[6]->in[4] mux_2level_tapbuf_size16[6]->in[5] mux_2level_tapbuf_size16[6]->in[6] mux_2level_tapbuf_size16[6]->in[7] mux_2level_tapbuf_size16[6]->in[8] mux_2level_tapbuf_size16[6]->in[9] mux_2level_tapbuf_size16[6]->in[10] mux_2level_tapbuf_size16[6]->in[11] mux_2level_tapbuf_size16[6]->in[12] mux_2level_tapbuf_size16[6]->in[13] mux_2level_tapbuf_size16[6]->in[14] mux_2level_tapbuf_size16[6]->in[15] mux_2level_tapbuf_size16[6]->out sram[48]->outb sram[48]->out sram[49]->out sram[49]->outb sram[50]->out sram[50]->outb sram[51]->out sram[51]->outb sram[52]->outb sram[52]->out sram[53]->out sram[53]->outb sram[54]->out sram[54]->outb sram[55]->out sram[55]->outb gvdd_mux_2level_tapbuf_size16[6] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[6], level=2, select_path_id=0. *****
*****10001000*****
Xsram[48] sram->in sram[48]->out sram[48]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[48]->out) 0
.nodeset V(sram[48]->outb) vsp
Xsram[49] sram->in sram[49]->out sram[49]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[49]->out) 0
.nodeset V(sram[49]->outb) vsp
Xsram[50] sram->in sram[50]->out sram[50]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[50]->out) 0
.nodeset V(sram[50]->outb) vsp
Xsram[51] sram->in sram[51]->out sram[51]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[51]->out) 0
.nodeset V(sram[51]->outb) vsp
Xsram[52] sram->in sram[52]->out sram[52]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[52]->out) 0
.nodeset V(sram[52]->outb) vsp
Xsram[53] sram->in sram[53]->out sram[53]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[53]->out) 0
.nodeset V(sram[53]->outb) vsp
Xsram[54] sram->in sram[54]->out sram[54]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[54]->out) 0
.nodeset V(sram[54]->outb) vsp
Xsram[55] sram->in sram[55]->out sram[55]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[55]->out) 0
.nodeset V(sram[55]->outb) vsp
***** Signal mux_2level_tapbuf_size16[6]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[0] mux_2level_tapbuf_size16[6]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[1] mux_2level_tapbuf_size16[6]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[2] mux_2level_tapbuf_size16[6]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[3] mux_2level_tapbuf_size16[6]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[4] mux_2level_tapbuf_size16[6]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[5] mux_2level_tapbuf_size16[6]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[6] mux_2level_tapbuf_size16[6]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[7] mux_2level_tapbuf_size16[6]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[8] mux_2level_tapbuf_size16[6]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[9] mux_2level_tapbuf_size16[6]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[10] mux_2level_tapbuf_size16[6]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[11] mux_2level_tapbuf_size16[6]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[12] mux_2level_tapbuf_size16[6]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[13] mux_2level_tapbuf_size16[6]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[14] mux_2level_tapbuf_size16[6]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[6]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[6]->in[15] mux_2level_tapbuf_size16[6]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[6] gvdd_mux_2level_tapbuf_size16[6] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[103] trig v(mux_2level_tapbuf_size16[6]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[6]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[103] trig v(mux_2level_tapbuf_size16[6]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[6]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[103] when v(mux_2level_tapbuf_size16[6]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[103] trig v(mux_2level_tapbuf_size16[6]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[6]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[103] when v(mux_2level_tapbuf_size16[6]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[103] trig v(mux_2level_tapbuf_size16[6]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[6]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[6]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[6]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[103] param='mux_2level_tapbuf_size16[6]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[6]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[6]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[6]_energy_per_cycle param='mux_2level_tapbuf_size16[6]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[103]  param='mux_2level_tapbuf_size16[6]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[103]  param='dynamic_power_cb_mux[0][1]_rrnode[103]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[103] avg p(Vgvdd_mux_2level_tapbuf_size16[6]) from='start_rise_cb_mux[0][1]_rrnode[103]' to='start_rise_cb_mux[0][1]_rrnode[103]+switch_rise_cb_mux[0][1]_rrnode[103]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[103] avg p(Vgvdd_mux_2level_tapbuf_size16[6]) from='start_fall_cb_mux[0][1]_rrnode[103]' to='start_fall_cb_mux[0][1]_rrnode[103]+switch_fall_cb_mux[0][1]_rrnode[103]'
.meas tran sum_leakage_power_mux[0to6] 
+          param='sum_leakage_power_mux[0to5]+leakage_cb_mux[0][1]_rrnode[103]'
.meas tran sum_energy_per_cycle_mux[0to6] 
+          param='sum_energy_per_cycle_mux[0to5]+energy_per_cycle_cb_mux[0][1]_rrnode[103]'
******* Normal TYPE loads *******
Xload_inv[360]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[361]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[1] gvdd_load 0 inv size=1
Xload_inv[362]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[2] gvdd_load 0 inv size=1
Xload_inv[363]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[3] gvdd_load 0 inv size=1
Xload_inv[364]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[4] gvdd_load 0 inv size=1
Xload_inv[365]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[5] gvdd_load 0 inv size=1
Xload_inv[366]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[6] gvdd_load 0 inv size=1
Xload_inv[367]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[7] gvdd_load 0 inv size=1
Xload_inv[368]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[8] gvdd_load 0 inv size=1
Xload_inv[369]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[9] gvdd_load 0 inv size=1
Xload_inv[370]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[10] gvdd_load 0 inv size=1
Xload_inv[371]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[11] gvdd_load 0 inv size=1
Xload_inv[372]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[12] gvdd_load 0 inv size=1
Xload_inv[373]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[13] gvdd_load 0 inv size=1
Xload_inv[374]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[14] gvdd_load 0 inv size=1
Xload_inv[375]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[15] gvdd_load 0 inv size=1
Xload_inv[376]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[16] gvdd_load 0 inv size=1
Xload_inv[377]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[17] gvdd_load 0 inv size=1
Xload_inv[378]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[18] gvdd_load 0 inv size=1
Xload_inv[379]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[19] gvdd_load 0 inv size=1
Xload_inv[380]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[20] gvdd_load 0 inv size=1
Xload_inv[381]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[21] gvdd_load 0 inv size=1
Xload_inv[382]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[22] gvdd_load 0 inv size=1
Xload_inv[383]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[23] gvdd_load 0 inv size=1
Xload_inv[384]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[24] gvdd_load 0 inv size=1
Xload_inv[385]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[25] gvdd_load 0 inv size=1
Xload_inv[386]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[26] gvdd_load 0 inv size=1
Xload_inv[387]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[27] gvdd_load 0 inv size=1
Xload_inv[388]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[28] gvdd_load 0 inv size=1
Xload_inv[389]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[29] gvdd_load 0 inv size=1
Xload_inv[390]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[30] gvdd_load 0 inv size=1
Xload_inv[391]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[31] gvdd_load 0 inv size=1
Xload_inv[392]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[32] gvdd_load 0 inv size=1
Xload_inv[393]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[33] gvdd_load 0 inv size=1
Xload_inv[394]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[34] gvdd_load 0 inv size=1
Xload_inv[395]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[35] gvdd_load 0 inv size=1
Xload_inv[396]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[36] gvdd_load 0 inv size=1
Xload_inv[397]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[37] gvdd_load 0 inv size=1
Xload_inv[398]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[38] gvdd_load 0 inv size=1
Xload_inv[399]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[39] gvdd_load 0 inv size=1
Xload_inv[400]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[40] gvdd_load 0 inv size=1
Xload_inv[401]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[41] gvdd_load 0 inv size=1
Xload_inv[402]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[42] gvdd_load 0 inv size=1
Xload_inv[403]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[43] gvdd_load 0 inv size=1
Xload_inv[404]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[44] gvdd_load 0 inv size=1
Xload_inv[405]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[45] gvdd_load 0 inv size=1
Xload_inv[406]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[46] gvdd_load 0 inv size=1
Xload_inv[407]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[47] gvdd_load 0 inv size=1
Xload_inv[408]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[48] gvdd_load 0 inv size=1
Xload_inv[409]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[49] gvdd_load 0 inv size=1
Xload_inv[410]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[50] gvdd_load 0 inv size=1
Xload_inv[411]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[51] gvdd_load 0 inv size=1
Xload_inv[412]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[52] gvdd_load 0 inv size=1
Xload_inv[413]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[53] gvdd_load 0 inv size=1
Xload_inv[414]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[54] gvdd_load 0 inv size=1
Xload_inv[415]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[55] gvdd_load 0 inv size=1
Xload_inv[416]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[56] gvdd_load 0 inv size=1
Xload_inv[417]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[57] gvdd_load 0 inv size=1
Xload_inv[418]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[58] gvdd_load 0 inv size=1
Xload_inv[419]_no0 mux_2level_tapbuf_size16[6]->out mux_2level_tapbuf_size16[6]->out_out[59] gvdd_load 0 inv size=1
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to6] 
+          param='sum_leakage_power_cb_mux[0to5]+leakage_cb_mux[0][1]_rrnode[103]'
.meas tran sum_energy_per_cycle_cb_mux[0to6] 
+          param='sum_energy_per_cycle_cb_mux[0to5]+energy_per_cycle_cb_mux[0][1]_rrnode[103]'
Xmux_2level_tapbuf_size16[7] mux_2level_tapbuf_size16[7]->in[0] mux_2level_tapbuf_size16[7]->in[1] mux_2level_tapbuf_size16[7]->in[2] mux_2level_tapbuf_size16[7]->in[3] mux_2level_tapbuf_size16[7]->in[4] mux_2level_tapbuf_size16[7]->in[5] mux_2level_tapbuf_size16[7]->in[6] mux_2level_tapbuf_size16[7]->in[7] mux_2level_tapbuf_size16[7]->in[8] mux_2level_tapbuf_size16[7]->in[9] mux_2level_tapbuf_size16[7]->in[10] mux_2level_tapbuf_size16[7]->in[11] mux_2level_tapbuf_size16[7]->in[12] mux_2level_tapbuf_size16[7]->in[13] mux_2level_tapbuf_size16[7]->in[14] mux_2level_tapbuf_size16[7]->in[15] mux_2level_tapbuf_size16[7]->out sram[56]->outb sram[56]->out sram[57]->out sram[57]->outb sram[58]->out sram[58]->outb sram[59]->out sram[59]->outb sram[60]->outb sram[60]->out sram[61]->out sram[61]->outb sram[62]->out sram[62]->outb sram[63]->out sram[63]->outb gvdd_mux_2level_tapbuf_size16[7] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[7], level=2, select_path_id=0. *****
*****10001000*****
Xsram[56] sram->in sram[56]->out sram[56]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[56]->out) 0
.nodeset V(sram[56]->outb) vsp
Xsram[57] sram->in sram[57]->out sram[57]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[57]->out) 0
.nodeset V(sram[57]->outb) vsp
Xsram[58] sram->in sram[58]->out sram[58]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[58]->out) 0
.nodeset V(sram[58]->outb) vsp
Xsram[59] sram->in sram[59]->out sram[59]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[59]->out) 0
.nodeset V(sram[59]->outb) vsp
Xsram[60] sram->in sram[60]->out sram[60]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[60]->out) 0
.nodeset V(sram[60]->outb) vsp
Xsram[61] sram->in sram[61]->out sram[61]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[61]->out) 0
.nodeset V(sram[61]->outb) vsp
Xsram[62] sram->in sram[62]->out sram[62]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[62]->out) 0
.nodeset V(sram[62]->outb) vsp
Xsram[63] sram->in sram[63]->out sram[63]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[63]->out) 0
.nodeset V(sram[63]->outb) vsp
***** Signal mux_2level_tapbuf_size16[7]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[0] mux_2level_tapbuf_size16[7]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[1] mux_2level_tapbuf_size16[7]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[2] mux_2level_tapbuf_size16[7]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[3] mux_2level_tapbuf_size16[7]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[4] mux_2level_tapbuf_size16[7]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[5] mux_2level_tapbuf_size16[7]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[6] mux_2level_tapbuf_size16[7]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[7] mux_2level_tapbuf_size16[7]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[8] mux_2level_tapbuf_size16[7]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[9] mux_2level_tapbuf_size16[7]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[10] mux_2level_tapbuf_size16[7]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[11] mux_2level_tapbuf_size16[7]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[12] mux_2level_tapbuf_size16[7]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[13] mux_2level_tapbuf_size16[7]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[14] mux_2level_tapbuf_size16[7]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[7]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[7]->in[15] mux_2level_tapbuf_size16[7]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[7] gvdd_mux_2level_tapbuf_size16[7] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[107] trig v(mux_2level_tapbuf_size16[7]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[7]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[107] trig v(mux_2level_tapbuf_size16[7]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[7]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[107] when v(mux_2level_tapbuf_size16[7]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[107] trig v(mux_2level_tapbuf_size16[7]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[7]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[107] when v(mux_2level_tapbuf_size16[7]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[107] trig v(mux_2level_tapbuf_size16[7]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[7]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[7]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[7]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[107] param='mux_2level_tapbuf_size16[7]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[7]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[7]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[7]_energy_per_cycle param='mux_2level_tapbuf_size16[7]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[107]  param='mux_2level_tapbuf_size16[7]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[107]  param='dynamic_power_cb_mux[0][1]_rrnode[107]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[107] avg p(Vgvdd_mux_2level_tapbuf_size16[7]) from='start_rise_cb_mux[0][1]_rrnode[107]' to='start_rise_cb_mux[0][1]_rrnode[107]+switch_rise_cb_mux[0][1]_rrnode[107]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[107] avg p(Vgvdd_mux_2level_tapbuf_size16[7]) from='start_fall_cb_mux[0][1]_rrnode[107]' to='start_fall_cb_mux[0][1]_rrnode[107]+switch_fall_cb_mux[0][1]_rrnode[107]'
.meas tran sum_leakage_power_mux[0to7] 
+          param='sum_leakage_power_mux[0to6]+leakage_cb_mux[0][1]_rrnode[107]'
.meas tran sum_energy_per_cycle_mux[0to7] 
+          param='sum_energy_per_cycle_mux[0to6]+energy_per_cycle_cb_mux[0][1]_rrnode[107]'
******* Normal TYPE loads *******
Xload_inv[420]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[421]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[1] gvdd_load 0 inv size=1
Xload_inv[422]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[2] gvdd_load 0 inv size=1
Xload_inv[423]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[3] gvdd_load 0 inv size=1
Xload_inv[424]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[4] gvdd_load 0 inv size=1
Xload_inv[425]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[5] gvdd_load 0 inv size=1
Xload_inv[426]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[6] gvdd_load 0 inv size=1
Xload_inv[427]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[7] gvdd_load 0 inv size=1
Xload_inv[428]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[8] gvdd_load 0 inv size=1
Xload_inv[429]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[9] gvdd_load 0 inv size=1
Xload_inv[430]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[10] gvdd_load 0 inv size=1
Xload_inv[431]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[11] gvdd_load 0 inv size=1
Xload_inv[432]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[12] gvdd_load 0 inv size=1
Xload_inv[433]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[13] gvdd_load 0 inv size=1
Xload_inv[434]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[14] gvdd_load 0 inv size=1
Xload_inv[435]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[15] gvdd_load 0 inv size=1
Xload_inv[436]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[16] gvdd_load 0 inv size=1
Xload_inv[437]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[17] gvdd_load 0 inv size=1
Xload_inv[438]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[18] gvdd_load 0 inv size=1
Xload_inv[439]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[19] gvdd_load 0 inv size=1
Xload_inv[440]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[20] gvdd_load 0 inv size=1
Xload_inv[441]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[21] gvdd_load 0 inv size=1
Xload_inv[442]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[22] gvdd_load 0 inv size=1
Xload_inv[443]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[23] gvdd_load 0 inv size=1
Xload_inv[444]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[24] gvdd_load 0 inv size=1
Xload_inv[445]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[25] gvdd_load 0 inv size=1
Xload_inv[446]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[26] gvdd_load 0 inv size=1
Xload_inv[447]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[27] gvdd_load 0 inv size=1
Xload_inv[448]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[28] gvdd_load 0 inv size=1
Xload_inv[449]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[29] gvdd_load 0 inv size=1
Xload_inv[450]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[30] gvdd_load 0 inv size=1
Xload_inv[451]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[31] gvdd_load 0 inv size=1
Xload_inv[452]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[32] gvdd_load 0 inv size=1
Xload_inv[453]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[33] gvdd_load 0 inv size=1
Xload_inv[454]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[34] gvdd_load 0 inv size=1
Xload_inv[455]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[35] gvdd_load 0 inv size=1
Xload_inv[456]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[36] gvdd_load 0 inv size=1
Xload_inv[457]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[37] gvdd_load 0 inv size=1
Xload_inv[458]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[38] gvdd_load 0 inv size=1
Xload_inv[459]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[39] gvdd_load 0 inv size=1
Xload_inv[460]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[40] gvdd_load 0 inv size=1
Xload_inv[461]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[41] gvdd_load 0 inv size=1
Xload_inv[462]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[42] gvdd_load 0 inv size=1
Xload_inv[463]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[43] gvdd_load 0 inv size=1
Xload_inv[464]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[44] gvdd_load 0 inv size=1
Xload_inv[465]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[45] gvdd_load 0 inv size=1
Xload_inv[466]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[46] gvdd_load 0 inv size=1
Xload_inv[467]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[47] gvdd_load 0 inv size=1
Xload_inv[468]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[48] gvdd_load 0 inv size=1
Xload_inv[469]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[49] gvdd_load 0 inv size=1
Xload_inv[470]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[50] gvdd_load 0 inv size=1
Xload_inv[471]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[51] gvdd_load 0 inv size=1
Xload_inv[472]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[52] gvdd_load 0 inv size=1
Xload_inv[473]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[53] gvdd_load 0 inv size=1
Xload_inv[474]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[54] gvdd_load 0 inv size=1
Xload_inv[475]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[55] gvdd_load 0 inv size=1
Xload_inv[476]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[56] gvdd_load 0 inv size=1
Xload_inv[477]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[57] gvdd_load 0 inv size=1
Xload_inv[478]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[58] gvdd_load 0 inv size=1
Xload_inv[479]_no0 mux_2level_tapbuf_size16[7]->out mux_2level_tapbuf_size16[7]->out_out[59] gvdd_load 0 inv size=1
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to7] 
+          param='sum_leakage_power_cb_mux[0to6]+leakage_cb_mux[0][1]_rrnode[107]'
.meas tran sum_energy_per_cycle_cb_mux[0to7] 
+          param='sum_energy_per_cycle_cb_mux[0to6]+energy_per_cycle_cb_mux[0][1]_rrnode[107]'
Xmux_2level_tapbuf_size16[8] mux_2level_tapbuf_size16[8]->in[0] mux_2level_tapbuf_size16[8]->in[1] mux_2level_tapbuf_size16[8]->in[2] mux_2level_tapbuf_size16[8]->in[3] mux_2level_tapbuf_size16[8]->in[4] mux_2level_tapbuf_size16[8]->in[5] mux_2level_tapbuf_size16[8]->in[6] mux_2level_tapbuf_size16[8]->in[7] mux_2level_tapbuf_size16[8]->in[8] mux_2level_tapbuf_size16[8]->in[9] mux_2level_tapbuf_size16[8]->in[10] mux_2level_tapbuf_size16[8]->in[11] mux_2level_tapbuf_size16[8]->in[12] mux_2level_tapbuf_size16[8]->in[13] mux_2level_tapbuf_size16[8]->in[14] mux_2level_tapbuf_size16[8]->in[15] mux_2level_tapbuf_size16[8]->out sram[64]->outb sram[64]->out sram[65]->out sram[65]->outb sram[66]->out sram[66]->outb sram[67]->out sram[67]->outb sram[68]->outb sram[68]->out sram[69]->out sram[69]->outb sram[70]->out sram[70]->outb sram[71]->out sram[71]->outb gvdd_mux_2level_tapbuf_size16[8] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[8], level=2, select_path_id=0. *****
*****10001000*****
Xsram[64] sram->in sram[64]->out sram[64]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[64]->out) 0
.nodeset V(sram[64]->outb) vsp
Xsram[65] sram->in sram[65]->out sram[65]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[65]->out) 0
.nodeset V(sram[65]->outb) vsp
Xsram[66] sram->in sram[66]->out sram[66]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[66]->out) 0
.nodeset V(sram[66]->outb) vsp
Xsram[67] sram->in sram[67]->out sram[67]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[67]->out) 0
.nodeset V(sram[67]->outb) vsp
Xsram[68] sram->in sram[68]->out sram[68]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[68]->out) 0
.nodeset V(sram[68]->outb) vsp
Xsram[69] sram->in sram[69]->out sram[69]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[69]->out) 0
.nodeset V(sram[69]->outb) vsp
Xsram[70] sram->in sram[70]->out sram[70]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[70]->out) 0
.nodeset V(sram[70]->outb) vsp
Xsram[71] sram->in sram[71]->out sram[71]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[71]->out) 0
.nodeset V(sram[71]->outb) vsp
***** Signal mux_2level_tapbuf_size16[8]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[0] mux_2level_tapbuf_size16[8]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[1] mux_2level_tapbuf_size16[8]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[2] mux_2level_tapbuf_size16[8]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[3] mux_2level_tapbuf_size16[8]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[4] mux_2level_tapbuf_size16[8]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[5] mux_2level_tapbuf_size16[8]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[6] mux_2level_tapbuf_size16[8]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[7] mux_2level_tapbuf_size16[8]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[8] mux_2level_tapbuf_size16[8]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[9] mux_2level_tapbuf_size16[8]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[10] mux_2level_tapbuf_size16[8]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[11] mux_2level_tapbuf_size16[8]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[12] mux_2level_tapbuf_size16[8]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[13] mux_2level_tapbuf_size16[8]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[14] mux_2level_tapbuf_size16[8]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[8]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[8]->in[15] mux_2level_tapbuf_size16[8]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[8] gvdd_mux_2level_tapbuf_size16[8] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[111] trig v(mux_2level_tapbuf_size16[8]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[8]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[111] trig v(mux_2level_tapbuf_size16[8]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[8]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[111] when v(mux_2level_tapbuf_size16[8]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[111] trig v(mux_2level_tapbuf_size16[8]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[8]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[111] when v(mux_2level_tapbuf_size16[8]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[111] trig v(mux_2level_tapbuf_size16[8]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[8]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[8]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[8]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[111] param='mux_2level_tapbuf_size16[8]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[8]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[8]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[8]_energy_per_cycle param='mux_2level_tapbuf_size16[8]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[111]  param='mux_2level_tapbuf_size16[8]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[111]  param='dynamic_power_cb_mux[0][1]_rrnode[111]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[111] avg p(Vgvdd_mux_2level_tapbuf_size16[8]) from='start_rise_cb_mux[0][1]_rrnode[111]' to='start_rise_cb_mux[0][1]_rrnode[111]+switch_rise_cb_mux[0][1]_rrnode[111]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[111] avg p(Vgvdd_mux_2level_tapbuf_size16[8]) from='start_fall_cb_mux[0][1]_rrnode[111]' to='start_fall_cb_mux[0][1]_rrnode[111]+switch_fall_cb_mux[0][1]_rrnode[111]'
.meas tran sum_leakage_power_mux[0to8] 
+          param='sum_leakage_power_mux[0to7]+leakage_cb_mux[0][1]_rrnode[111]'
.meas tran sum_energy_per_cycle_mux[0to8] 
+          param='sum_energy_per_cycle_mux[0to7]+energy_per_cycle_cb_mux[0][1]_rrnode[111]'
******* Normal TYPE loads *******
Xload_inv[480]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[481]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[1] gvdd_load 0 inv size=1
Xload_inv[482]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[2] gvdd_load 0 inv size=1
Xload_inv[483]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[3] gvdd_load 0 inv size=1
Xload_inv[484]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[4] gvdd_load 0 inv size=1
Xload_inv[485]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[5] gvdd_load 0 inv size=1
Xload_inv[486]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[6] gvdd_load 0 inv size=1
Xload_inv[487]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[7] gvdd_load 0 inv size=1
Xload_inv[488]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[8] gvdd_load 0 inv size=1
Xload_inv[489]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[9] gvdd_load 0 inv size=1
Xload_inv[490]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[10] gvdd_load 0 inv size=1
Xload_inv[491]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[11] gvdd_load 0 inv size=1
Xload_inv[492]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[12] gvdd_load 0 inv size=1
Xload_inv[493]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[13] gvdd_load 0 inv size=1
Xload_inv[494]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[14] gvdd_load 0 inv size=1
Xload_inv[495]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[15] gvdd_load 0 inv size=1
Xload_inv[496]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[16] gvdd_load 0 inv size=1
Xload_inv[497]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[17] gvdd_load 0 inv size=1
Xload_inv[498]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[18] gvdd_load 0 inv size=1
Xload_inv[499]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[19] gvdd_load 0 inv size=1
Xload_inv[500]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[20] gvdd_load 0 inv size=1
Xload_inv[501]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[21] gvdd_load 0 inv size=1
Xload_inv[502]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[22] gvdd_load 0 inv size=1
Xload_inv[503]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[23] gvdd_load 0 inv size=1
Xload_inv[504]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[24] gvdd_load 0 inv size=1
Xload_inv[505]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[25] gvdd_load 0 inv size=1
Xload_inv[506]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[26] gvdd_load 0 inv size=1
Xload_inv[507]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[27] gvdd_load 0 inv size=1
Xload_inv[508]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[28] gvdd_load 0 inv size=1
Xload_inv[509]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[29] gvdd_load 0 inv size=1
Xload_inv[510]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[30] gvdd_load 0 inv size=1
Xload_inv[511]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[31] gvdd_load 0 inv size=1
Xload_inv[512]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[32] gvdd_load 0 inv size=1
Xload_inv[513]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[33] gvdd_load 0 inv size=1
Xload_inv[514]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[34] gvdd_load 0 inv size=1
Xload_inv[515]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[35] gvdd_load 0 inv size=1
Xload_inv[516]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[36] gvdd_load 0 inv size=1
Xload_inv[517]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[37] gvdd_load 0 inv size=1
Xload_inv[518]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[38] gvdd_load 0 inv size=1
Xload_inv[519]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[39] gvdd_load 0 inv size=1
Xload_inv[520]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[40] gvdd_load 0 inv size=1
Xload_inv[521]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[41] gvdd_load 0 inv size=1
Xload_inv[522]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[42] gvdd_load 0 inv size=1
Xload_inv[523]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[43] gvdd_load 0 inv size=1
Xload_inv[524]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[44] gvdd_load 0 inv size=1
Xload_inv[525]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[45] gvdd_load 0 inv size=1
Xload_inv[526]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[46] gvdd_load 0 inv size=1
Xload_inv[527]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[47] gvdd_load 0 inv size=1
Xload_inv[528]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[48] gvdd_load 0 inv size=1
Xload_inv[529]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[49] gvdd_load 0 inv size=1
Xload_inv[530]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[50] gvdd_load 0 inv size=1
Xload_inv[531]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[51] gvdd_load 0 inv size=1
Xload_inv[532]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[52] gvdd_load 0 inv size=1
Xload_inv[533]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[53] gvdd_load 0 inv size=1
Xload_inv[534]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[54] gvdd_load 0 inv size=1
Xload_inv[535]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[55] gvdd_load 0 inv size=1
Xload_inv[536]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[56] gvdd_load 0 inv size=1
Xload_inv[537]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[57] gvdd_load 0 inv size=1
Xload_inv[538]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[58] gvdd_load 0 inv size=1
Xload_inv[539]_no0 mux_2level_tapbuf_size16[8]->out mux_2level_tapbuf_size16[8]->out_out[59] gvdd_load 0 inv size=1
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to8] 
+          param='sum_leakage_power_cb_mux[0to7]+leakage_cb_mux[0][1]_rrnode[111]'
.meas tran sum_energy_per_cycle_cb_mux[0to8] 
+          param='sum_energy_per_cycle_cb_mux[0to7]+energy_per_cycle_cb_mux[0][1]_rrnode[111]'
Xmux_2level_tapbuf_size16[9] mux_2level_tapbuf_size16[9]->in[0] mux_2level_tapbuf_size16[9]->in[1] mux_2level_tapbuf_size16[9]->in[2] mux_2level_tapbuf_size16[9]->in[3] mux_2level_tapbuf_size16[9]->in[4] mux_2level_tapbuf_size16[9]->in[5] mux_2level_tapbuf_size16[9]->in[6] mux_2level_tapbuf_size16[9]->in[7] mux_2level_tapbuf_size16[9]->in[8] mux_2level_tapbuf_size16[9]->in[9] mux_2level_tapbuf_size16[9]->in[10] mux_2level_tapbuf_size16[9]->in[11] mux_2level_tapbuf_size16[9]->in[12] mux_2level_tapbuf_size16[9]->in[13] mux_2level_tapbuf_size16[9]->in[14] mux_2level_tapbuf_size16[9]->in[15] mux_2level_tapbuf_size16[9]->out sram[72]->outb sram[72]->out sram[73]->out sram[73]->outb sram[74]->out sram[74]->outb sram[75]->out sram[75]->outb sram[76]->outb sram[76]->out sram[77]->out sram[77]->outb sram[78]->out sram[78]->outb sram[79]->out sram[79]->outb gvdd_mux_2level_tapbuf_size16[9] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[9], level=2, select_path_id=0. *****
*****10001000*****
Xsram[72] sram->in sram[72]->out sram[72]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[72]->out) 0
.nodeset V(sram[72]->outb) vsp
Xsram[73] sram->in sram[73]->out sram[73]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[73]->out) 0
.nodeset V(sram[73]->outb) vsp
Xsram[74] sram->in sram[74]->out sram[74]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[74]->out) 0
.nodeset V(sram[74]->outb) vsp
Xsram[75] sram->in sram[75]->out sram[75]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[75]->out) 0
.nodeset V(sram[75]->outb) vsp
Xsram[76] sram->in sram[76]->out sram[76]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[76]->out) 0
.nodeset V(sram[76]->outb) vsp
Xsram[77] sram->in sram[77]->out sram[77]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[77]->out) 0
.nodeset V(sram[77]->outb) vsp
Xsram[78] sram->in sram[78]->out sram[78]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[78]->out) 0
.nodeset V(sram[78]->outb) vsp
Xsram[79] sram->in sram[79]->out sram[79]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[79]->out) 0
.nodeset V(sram[79]->outb) vsp
***** Signal mux_2level_tapbuf_size16[9]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[0] mux_2level_tapbuf_size16[9]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[1] mux_2level_tapbuf_size16[9]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[2] mux_2level_tapbuf_size16[9]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[3] mux_2level_tapbuf_size16[9]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[4] mux_2level_tapbuf_size16[9]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[5] mux_2level_tapbuf_size16[9]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[6] mux_2level_tapbuf_size16[9]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[7] mux_2level_tapbuf_size16[9]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[8] mux_2level_tapbuf_size16[9]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[9] mux_2level_tapbuf_size16[9]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[10] mux_2level_tapbuf_size16[9]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[11] mux_2level_tapbuf_size16[9]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[12] mux_2level_tapbuf_size16[9]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[13] mux_2level_tapbuf_size16[9]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[14] mux_2level_tapbuf_size16[9]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[9]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[9]->in[15] mux_2level_tapbuf_size16[9]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[9] gvdd_mux_2level_tapbuf_size16[9] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[115] trig v(mux_2level_tapbuf_size16[9]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[9]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[115] trig v(mux_2level_tapbuf_size16[9]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[9]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[115] when v(mux_2level_tapbuf_size16[9]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[115] trig v(mux_2level_tapbuf_size16[9]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[9]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[115] when v(mux_2level_tapbuf_size16[9]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[115] trig v(mux_2level_tapbuf_size16[9]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[9]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[9]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[9]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[115] param='mux_2level_tapbuf_size16[9]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[9]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[9]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[9]_energy_per_cycle param='mux_2level_tapbuf_size16[9]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[115]  param='mux_2level_tapbuf_size16[9]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[115]  param='dynamic_power_cb_mux[0][1]_rrnode[115]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[115] avg p(Vgvdd_mux_2level_tapbuf_size16[9]) from='start_rise_cb_mux[0][1]_rrnode[115]' to='start_rise_cb_mux[0][1]_rrnode[115]+switch_rise_cb_mux[0][1]_rrnode[115]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[115] avg p(Vgvdd_mux_2level_tapbuf_size16[9]) from='start_fall_cb_mux[0][1]_rrnode[115]' to='start_fall_cb_mux[0][1]_rrnode[115]+switch_fall_cb_mux[0][1]_rrnode[115]'
.meas tran sum_leakage_power_mux[0to9] 
+          param='sum_leakage_power_mux[0to8]+leakage_cb_mux[0][1]_rrnode[115]'
.meas tran sum_energy_per_cycle_mux[0to9] 
+          param='sum_energy_per_cycle_mux[0to8]+energy_per_cycle_cb_mux[0][1]_rrnode[115]'
******* Normal TYPE loads *******
Xload_inv[540]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[0] gvdd_load 0 inv size=1
Xload_inv[541]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[1] gvdd_load 0 inv size=1
Xload_inv[542]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[2] gvdd_load 0 inv size=1
Xload_inv[543]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[3] gvdd_load 0 inv size=1
Xload_inv[544]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[4] gvdd_load 0 inv size=1
Xload_inv[545]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[5] gvdd_load 0 inv size=1
Xload_inv[546]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[6] gvdd_load 0 inv size=1
Xload_inv[547]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[7] gvdd_load 0 inv size=1
Xload_inv[548]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[8] gvdd_load 0 inv size=1
Xload_inv[549]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[9] gvdd_load 0 inv size=1
Xload_inv[550]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[10] gvdd_load 0 inv size=1
Xload_inv[551]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[11] gvdd_load 0 inv size=1
Xload_inv[552]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[12] gvdd_load 0 inv size=1
Xload_inv[553]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[13] gvdd_load 0 inv size=1
Xload_inv[554]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[14] gvdd_load 0 inv size=1
Xload_inv[555]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[15] gvdd_load 0 inv size=1
Xload_inv[556]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[16] gvdd_load 0 inv size=1
Xload_inv[557]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[17] gvdd_load 0 inv size=1
Xload_inv[558]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[18] gvdd_load 0 inv size=1
Xload_inv[559]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[19] gvdd_load 0 inv size=1
Xload_inv[560]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[20] gvdd_load 0 inv size=1
Xload_inv[561]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[21] gvdd_load 0 inv size=1
Xload_inv[562]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[22] gvdd_load 0 inv size=1
Xload_inv[563]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[23] gvdd_load 0 inv size=1
Xload_inv[564]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[24] gvdd_load 0 inv size=1
Xload_inv[565]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[25] gvdd_load 0 inv size=1
Xload_inv[566]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[26] gvdd_load 0 inv size=1
Xload_inv[567]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[27] gvdd_load 0 inv size=1
Xload_inv[568]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[28] gvdd_load 0 inv size=1
Xload_inv[569]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[29] gvdd_load 0 inv size=1
Xload_inv[570]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[30] gvdd_load 0 inv size=1
Xload_inv[571]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[31] gvdd_load 0 inv size=1
Xload_inv[572]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[32] gvdd_load 0 inv size=1
Xload_inv[573]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[33] gvdd_load 0 inv size=1
Xload_inv[574]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[34] gvdd_load 0 inv size=1
Xload_inv[575]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[35] gvdd_load 0 inv size=1
Xload_inv[576]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[36] gvdd_load 0 inv size=1
Xload_inv[577]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[37] gvdd_load 0 inv size=1
Xload_inv[578]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[38] gvdd_load 0 inv size=1
Xload_inv[579]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[39] gvdd_load 0 inv size=1
Xload_inv[580]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[40] gvdd_load 0 inv size=1
Xload_inv[581]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[41] gvdd_load 0 inv size=1
Xload_inv[582]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[42] gvdd_load 0 inv size=1
Xload_inv[583]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[43] gvdd_load 0 inv size=1
Xload_inv[584]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[44] gvdd_load 0 inv size=1
Xload_inv[585]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[45] gvdd_load 0 inv size=1
Xload_inv[586]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[46] gvdd_load 0 inv size=1
Xload_inv[587]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[47] gvdd_load 0 inv size=1
Xload_inv[588]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[48] gvdd_load 0 inv size=1
Xload_inv[589]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[49] gvdd_load 0 inv size=1
Xload_inv[590]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[50] gvdd_load 0 inv size=1
Xload_inv[591]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[51] gvdd_load 0 inv size=1
Xload_inv[592]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[52] gvdd_load 0 inv size=1
Xload_inv[593]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[53] gvdd_load 0 inv size=1
Xload_inv[594]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[54] gvdd_load 0 inv size=1
Xload_inv[595]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[55] gvdd_load 0 inv size=1
Xload_inv[596]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[56] gvdd_load 0 inv size=1
Xload_inv[597]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[57] gvdd_load 0 inv size=1
Xload_inv[598]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[58] gvdd_load 0 inv size=1
Xload_inv[599]_no0 mux_2level_tapbuf_size16[9]->out mux_2level_tapbuf_size16[9]->out_out[59] gvdd_load 0 inv size=1
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to9] 
+          param='sum_leakage_power_cb_mux[0to8]+leakage_cb_mux[0][1]_rrnode[115]'
.meas tran sum_energy_per_cycle_cb_mux[0to9] 
+          param='sum_energy_per_cycle_cb_mux[0to8]+energy_per_cycle_cb_mux[0][1]_rrnode[115]'
Xmux_2level_tapbuf_size16[10] mux_2level_tapbuf_size16[10]->in[0] mux_2level_tapbuf_size16[10]->in[1] mux_2level_tapbuf_size16[10]->in[2] mux_2level_tapbuf_size16[10]->in[3] mux_2level_tapbuf_size16[10]->in[4] mux_2level_tapbuf_size16[10]->in[5] mux_2level_tapbuf_size16[10]->in[6] mux_2level_tapbuf_size16[10]->in[7] mux_2level_tapbuf_size16[10]->in[8] mux_2level_tapbuf_size16[10]->in[9] mux_2level_tapbuf_size16[10]->in[10] mux_2level_tapbuf_size16[10]->in[11] mux_2level_tapbuf_size16[10]->in[12] mux_2level_tapbuf_size16[10]->in[13] mux_2level_tapbuf_size16[10]->in[14] mux_2level_tapbuf_size16[10]->in[15] mux_2level_tapbuf_size16[10]->out sram[80]->outb sram[80]->out sram[81]->out sram[81]->outb sram[82]->out sram[82]->outb sram[83]->out sram[83]->outb sram[84]->outb sram[84]->out sram[85]->out sram[85]->outb sram[86]->out sram[86]->outb sram[87]->out sram[87]->outb gvdd_mux_2level_tapbuf_size16[10] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[10], level=2, select_path_id=0. *****
*****10001000*****
Xsram[80] sram->in sram[80]->out sram[80]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[80]->out) 0
.nodeset V(sram[80]->outb) vsp
Xsram[81] sram->in sram[81]->out sram[81]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[81]->out) 0
.nodeset V(sram[81]->outb) vsp
Xsram[82] sram->in sram[82]->out sram[82]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[82]->out) 0
.nodeset V(sram[82]->outb) vsp
Xsram[83] sram->in sram[83]->out sram[83]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[83]->out) 0
.nodeset V(sram[83]->outb) vsp
Xsram[84] sram->in sram[84]->out sram[84]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[84]->out) 0
.nodeset V(sram[84]->outb) vsp
Xsram[85] sram->in sram[85]->out sram[85]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[85]->out) 0
.nodeset V(sram[85]->outb) vsp
Xsram[86] sram->in sram[86]->out sram[86]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[86]->out) 0
.nodeset V(sram[86]->outb) vsp
Xsram[87] sram->in sram[87]->out sram[87]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[87]->out) 0
.nodeset V(sram[87]->outb) vsp
***** Signal mux_2level_tapbuf_size16[10]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[0] mux_2level_tapbuf_size16[10]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[1] mux_2level_tapbuf_size16[10]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[2] mux_2level_tapbuf_size16[10]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[3] mux_2level_tapbuf_size16[10]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[4] mux_2level_tapbuf_size16[10]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[5] mux_2level_tapbuf_size16[10]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[6] mux_2level_tapbuf_size16[10]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[7] mux_2level_tapbuf_size16[10]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[8] mux_2level_tapbuf_size16[10]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[9] mux_2level_tapbuf_size16[10]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[10] mux_2level_tapbuf_size16[10]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[11] mux_2level_tapbuf_size16[10]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[12] mux_2level_tapbuf_size16[10]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[13] mux_2level_tapbuf_size16[10]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[14] mux_2level_tapbuf_size16[10]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[10]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[10]->in[15] mux_2level_tapbuf_size16[10]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[10] gvdd_mux_2level_tapbuf_size16[10] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[16] trig v(mux_2level_tapbuf_size16[10]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[10]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[16] trig v(mux_2level_tapbuf_size16[10]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[10]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[16] when v(mux_2level_tapbuf_size16[10]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[16] trig v(mux_2level_tapbuf_size16[10]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[10]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[16] when v(mux_2level_tapbuf_size16[10]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[16] trig v(mux_2level_tapbuf_size16[10]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[10]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[10]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[10]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[16] param='mux_2level_tapbuf_size16[10]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[10]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[10]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[10]_energy_per_cycle param='mux_2level_tapbuf_size16[10]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[16]  param='mux_2level_tapbuf_size16[10]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[16]  param='dynamic_power_cb_mux[0][1]_rrnode[16]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[16] avg p(Vgvdd_mux_2level_tapbuf_size16[10]) from='start_rise_cb_mux[0][1]_rrnode[16]' to='start_rise_cb_mux[0][1]_rrnode[16]+switch_rise_cb_mux[0][1]_rrnode[16]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[16] avg p(Vgvdd_mux_2level_tapbuf_size16[10]) from='start_fall_cb_mux[0][1]_rrnode[16]' to='start_fall_cb_mux[0][1]_rrnode[16]+switch_fall_cb_mux[0][1]_rrnode[16]'
.meas tran sum_leakage_power_mux[0to10] 
+          param='sum_leakage_power_mux[0to9]+leakage_cb_mux[0][1]_rrnode[16]'
.meas tran sum_energy_per_cycle_mux[0to10] 
+          param='sum_energy_per_cycle_mux[0to9]+energy_per_cycle_cb_mux[0][1]_rrnode[16]'
******* IO_TYPE loads *******
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to10] 
+          param='sum_leakage_power_cb_mux[0to9]+leakage_cb_mux[0][1]_rrnode[16]'
.meas tran sum_energy_per_cycle_cb_mux[0to10] 
+          param='sum_energy_per_cycle_cb_mux[0to9]+energy_per_cycle_cb_mux[0][1]_rrnode[16]'
Xmux_2level_tapbuf_size16[11] mux_2level_tapbuf_size16[11]->in[0] mux_2level_tapbuf_size16[11]->in[1] mux_2level_tapbuf_size16[11]->in[2] mux_2level_tapbuf_size16[11]->in[3] mux_2level_tapbuf_size16[11]->in[4] mux_2level_tapbuf_size16[11]->in[5] mux_2level_tapbuf_size16[11]->in[6] mux_2level_tapbuf_size16[11]->in[7] mux_2level_tapbuf_size16[11]->in[8] mux_2level_tapbuf_size16[11]->in[9] mux_2level_tapbuf_size16[11]->in[10] mux_2level_tapbuf_size16[11]->in[11] mux_2level_tapbuf_size16[11]->in[12] mux_2level_tapbuf_size16[11]->in[13] mux_2level_tapbuf_size16[11]->in[14] mux_2level_tapbuf_size16[11]->in[15] mux_2level_tapbuf_size16[11]->out sram[88]->outb sram[88]->out sram[89]->out sram[89]->outb sram[90]->out sram[90]->outb sram[91]->out sram[91]->outb sram[92]->outb sram[92]->out sram[93]->out sram[93]->outb sram[94]->out sram[94]->outb sram[95]->out sram[95]->outb gvdd_mux_2level_tapbuf_size16[11] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[11], level=2, select_path_id=0. *****
*****10001000*****
Xsram[88] sram->in sram[88]->out sram[88]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[88]->out) 0
.nodeset V(sram[88]->outb) vsp
Xsram[89] sram->in sram[89]->out sram[89]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[89]->out) 0
.nodeset V(sram[89]->outb) vsp
Xsram[90] sram->in sram[90]->out sram[90]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[90]->out) 0
.nodeset V(sram[90]->outb) vsp
Xsram[91] sram->in sram[91]->out sram[91]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[91]->out) 0
.nodeset V(sram[91]->outb) vsp
Xsram[92] sram->in sram[92]->out sram[92]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[92]->out) 0
.nodeset V(sram[92]->outb) vsp
Xsram[93] sram->in sram[93]->out sram[93]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[93]->out) 0
.nodeset V(sram[93]->outb) vsp
Xsram[94] sram->in sram[94]->out sram[94]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[94]->out) 0
.nodeset V(sram[94]->outb) vsp
Xsram[95] sram->in sram[95]->out sram[95]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[95]->out) 0
.nodeset V(sram[95]->outb) vsp
***** Signal mux_2level_tapbuf_size16[11]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[0] mux_2level_tapbuf_size16[11]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[1] mux_2level_tapbuf_size16[11]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[2] mux_2level_tapbuf_size16[11]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[3] mux_2level_tapbuf_size16[11]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[4] mux_2level_tapbuf_size16[11]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[5] mux_2level_tapbuf_size16[11]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[6] mux_2level_tapbuf_size16[11]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[7] mux_2level_tapbuf_size16[11]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[8] mux_2level_tapbuf_size16[11]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[9] mux_2level_tapbuf_size16[11]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[10] mux_2level_tapbuf_size16[11]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[11] mux_2level_tapbuf_size16[11]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[12] density = 0.2026, probability=0.4982.*****
Vmux_2level_tapbuf_size16[11]->in[12] mux_2level_tapbuf_size16[11]->in[12] 0 
+  pulse(0 vsp 'clock_period' 
+  'input_slew_pct_rise*clock_period' 'input_slew_pct_fall*clock_period'
+  '0.4982*9.87167*(1-input_slew_pct_rise-input_slew_pct_fall)*clock_period' '9.87167*clock_period')
***** Signal mux_2level_tapbuf_size16[11]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[13] mux_2level_tapbuf_size16[11]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[14] mux_2level_tapbuf_size16[11]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[11]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[11]->in[15] mux_2level_tapbuf_size16[11]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[11] gvdd_mux_2level_tapbuf_size16[11] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[18] trig v(mux_2level_tapbuf_size16[11]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[11]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[18] trig v(mux_2level_tapbuf_size16[11]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[11]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[18] when v(mux_2level_tapbuf_size16[11]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[18] trig v(mux_2level_tapbuf_size16[11]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[11]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[18] when v(mux_2level_tapbuf_size16[11]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[18] trig v(mux_2level_tapbuf_size16[11]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[11]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[11]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[11]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[18] param='mux_2level_tapbuf_size16[11]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[11]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[11]) from='clock_period' to='6*clock_period'
.meas tran mux_2level_tapbuf_size16[11]_energy_per_cycle param='mux_2level_tapbuf_size16[11]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[18]  param='mux_2level_tapbuf_size16[11]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[18]  param='dynamic_power_cb_mux[0][1]_rrnode[18]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[18] avg p(Vgvdd_mux_2level_tapbuf_size16[11]) from='start_rise_cb_mux[0][1]_rrnode[18]' to='start_rise_cb_mux[0][1]_rrnode[18]+switch_rise_cb_mux[0][1]_rrnode[18]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[18] avg p(Vgvdd_mux_2level_tapbuf_size16[11]) from='start_fall_cb_mux[0][1]_rrnode[18]' to='start_fall_cb_mux[0][1]_rrnode[18]+switch_fall_cb_mux[0][1]_rrnode[18]'
.meas tran sum_leakage_power_mux[0to11] 
+          param='sum_leakage_power_mux[0to10]+leakage_cb_mux[0][1]_rrnode[18]'
.meas tran sum_energy_per_cycle_mux[0to11] 
+          param='sum_energy_per_cycle_mux[0to10]+energy_per_cycle_cb_mux[0][1]_rrnode[18]'
******* IO_TYPE loads *******
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to11] 
+          param='sum_leakage_power_cb_mux[0to10]+leakage_cb_mux[0][1]_rrnode[18]'
.meas tran sum_energy_per_cycle_cb_mux[0to11] 
+          param='sum_energy_per_cycle_cb_mux[0to10]+energy_per_cycle_cb_mux[0][1]_rrnode[18]'
Xmux_2level_tapbuf_size16[12] mux_2level_tapbuf_size16[12]->in[0] mux_2level_tapbuf_size16[12]->in[1] mux_2level_tapbuf_size16[12]->in[2] mux_2level_tapbuf_size16[12]->in[3] mux_2level_tapbuf_size16[12]->in[4] mux_2level_tapbuf_size16[12]->in[5] mux_2level_tapbuf_size16[12]->in[6] mux_2level_tapbuf_size16[12]->in[7] mux_2level_tapbuf_size16[12]->in[8] mux_2level_tapbuf_size16[12]->in[9] mux_2level_tapbuf_size16[12]->in[10] mux_2level_tapbuf_size16[12]->in[11] mux_2level_tapbuf_size16[12]->in[12] mux_2level_tapbuf_size16[12]->in[13] mux_2level_tapbuf_size16[12]->in[14] mux_2level_tapbuf_size16[12]->in[15] mux_2level_tapbuf_size16[12]->out sram[96]->outb sram[96]->out sram[97]->out sram[97]->outb sram[98]->out sram[98]->outb sram[99]->out sram[99]->outb sram[100]->outb sram[100]->out sram[101]->out sram[101]->outb sram[102]->out sram[102]->outb sram[103]->out sram[103]->outb gvdd_mux_2level_tapbuf_size16[12] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[12], level=2, select_path_id=0. *****
*****10001000*****
Xsram[96] sram->in sram[96]->out sram[96]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[96]->out) 0
.nodeset V(sram[96]->outb) vsp
Xsram[97] sram->in sram[97]->out sram[97]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[97]->out) 0
.nodeset V(sram[97]->outb) vsp
Xsram[98] sram->in sram[98]->out sram[98]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[98]->out) 0
.nodeset V(sram[98]->outb) vsp
Xsram[99] sram->in sram[99]->out sram[99]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[99]->out) 0
.nodeset V(sram[99]->outb) vsp
Xsram[100] sram->in sram[100]->out sram[100]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[100]->out) 0
.nodeset V(sram[100]->outb) vsp
Xsram[101] sram->in sram[101]->out sram[101]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[101]->out) 0
.nodeset V(sram[101]->outb) vsp
Xsram[102] sram->in sram[102]->out sram[102]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[102]->out) 0
.nodeset V(sram[102]->outb) vsp
Xsram[103] sram->in sram[103]->out sram[103]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[103]->out) 0
.nodeset V(sram[103]->outb) vsp
***** Signal mux_2level_tapbuf_size16[12]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[0] mux_2level_tapbuf_size16[12]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[1] mux_2level_tapbuf_size16[12]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[2] mux_2level_tapbuf_size16[12]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[3] mux_2level_tapbuf_size16[12]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[4] mux_2level_tapbuf_size16[12]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[5] mux_2level_tapbuf_size16[12]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[6] mux_2level_tapbuf_size16[12]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[7] mux_2level_tapbuf_size16[12]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[8] mux_2level_tapbuf_size16[12]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[9] mux_2level_tapbuf_size16[12]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[10] mux_2level_tapbuf_size16[12]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[11] mux_2level_tapbuf_size16[12]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[12] mux_2level_tapbuf_size16[12]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[13] mux_2level_tapbuf_size16[12]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[14] mux_2level_tapbuf_size16[12]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[12]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[12]->in[15] mux_2level_tapbuf_size16[12]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[12] gvdd_mux_2level_tapbuf_size16[12] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[20] trig v(mux_2level_tapbuf_size16[12]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[12]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[20] trig v(mux_2level_tapbuf_size16[12]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[12]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[20] when v(mux_2level_tapbuf_size16[12]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[20] trig v(mux_2level_tapbuf_size16[12]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[12]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[20] when v(mux_2level_tapbuf_size16[12]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[20] trig v(mux_2level_tapbuf_size16[12]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[12]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[12]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[12]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[20] param='mux_2level_tapbuf_size16[12]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[12]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[12]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[12]_energy_per_cycle param='mux_2level_tapbuf_size16[12]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[20]  param='mux_2level_tapbuf_size16[12]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[20]  param='dynamic_power_cb_mux[0][1]_rrnode[20]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[20] avg p(Vgvdd_mux_2level_tapbuf_size16[12]) from='start_rise_cb_mux[0][1]_rrnode[20]' to='start_rise_cb_mux[0][1]_rrnode[20]+switch_rise_cb_mux[0][1]_rrnode[20]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[20] avg p(Vgvdd_mux_2level_tapbuf_size16[12]) from='start_fall_cb_mux[0][1]_rrnode[20]' to='start_fall_cb_mux[0][1]_rrnode[20]+switch_fall_cb_mux[0][1]_rrnode[20]'
.meas tran sum_leakage_power_mux[0to12] 
+          param='sum_leakage_power_mux[0to11]+leakage_cb_mux[0][1]_rrnode[20]'
.meas tran sum_energy_per_cycle_mux[0to12] 
+          param='sum_energy_per_cycle_mux[0to11]+energy_per_cycle_cb_mux[0][1]_rrnode[20]'
******* IO_TYPE loads *******
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to12] 
+          param='sum_leakage_power_cb_mux[0to11]+leakage_cb_mux[0][1]_rrnode[20]'
.meas tran sum_energy_per_cycle_cb_mux[0to12] 
+          param='sum_energy_per_cycle_cb_mux[0to11]+energy_per_cycle_cb_mux[0][1]_rrnode[20]'
Xmux_2level_tapbuf_size16[13] mux_2level_tapbuf_size16[13]->in[0] mux_2level_tapbuf_size16[13]->in[1] mux_2level_tapbuf_size16[13]->in[2] mux_2level_tapbuf_size16[13]->in[3] mux_2level_tapbuf_size16[13]->in[4] mux_2level_tapbuf_size16[13]->in[5] mux_2level_tapbuf_size16[13]->in[6] mux_2level_tapbuf_size16[13]->in[7] mux_2level_tapbuf_size16[13]->in[8] mux_2level_tapbuf_size16[13]->in[9] mux_2level_tapbuf_size16[13]->in[10] mux_2level_tapbuf_size16[13]->in[11] mux_2level_tapbuf_size16[13]->in[12] mux_2level_tapbuf_size16[13]->in[13] mux_2level_tapbuf_size16[13]->in[14] mux_2level_tapbuf_size16[13]->in[15] mux_2level_tapbuf_size16[13]->out sram[104]->outb sram[104]->out sram[105]->out sram[105]->outb sram[106]->out sram[106]->outb sram[107]->out sram[107]->outb sram[108]->outb sram[108]->out sram[109]->out sram[109]->outb sram[110]->out sram[110]->outb sram[111]->out sram[111]->outb gvdd_mux_2level_tapbuf_size16[13] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[13], level=2, select_path_id=0. *****
*****10001000*****
Xsram[104] sram->in sram[104]->out sram[104]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[104]->out) 0
.nodeset V(sram[104]->outb) vsp
Xsram[105] sram->in sram[105]->out sram[105]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[105]->out) 0
.nodeset V(sram[105]->outb) vsp
Xsram[106] sram->in sram[106]->out sram[106]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[106]->out) 0
.nodeset V(sram[106]->outb) vsp
Xsram[107] sram->in sram[107]->out sram[107]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[107]->out) 0
.nodeset V(sram[107]->outb) vsp
Xsram[108] sram->in sram[108]->out sram[108]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[108]->out) 0
.nodeset V(sram[108]->outb) vsp
Xsram[109] sram->in sram[109]->out sram[109]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[109]->out) 0
.nodeset V(sram[109]->outb) vsp
Xsram[110] sram->in sram[110]->out sram[110]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[110]->out) 0
.nodeset V(sram[110]->outb) vsp
Xsram[111] sram->in sram[111]->out sram[111]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[111]->out) 0
.nodeset V(sram[111]->outb) vsp
***** Signal mux_2level_tapbuf_size16[13]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[0] mux_2level_tapbuf_size16[13]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[1] mux_2level_tapbuf_size16[13]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[2] mux_2level_tapbuf_size16[13]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[3] mux_2level_tapbuf_size16[13]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[4] mux_2level_tapbuf_size16[13]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[5] mux_2level_tapbuf_size16[13]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[6] mux_2level_tapbuf_size16[13]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[7] mux_2level_tapbuf_size16[13]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[8] mux_2level_tapbuf_size16[13]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[9] mux_2level_tapbuf_size16[13]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[10] mux_2level_tapbuf_size16[13]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[11] mux_2level_tapbuf_size16[13]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[12] mux_2level_tapbuf_size16[13]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[13] mux_2level_tapbuf_size16[13]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[14] mux_2level_tapbuf_size16[13]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[13]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[13]->in[15] mux_2level_tapbuf_size16[13]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[13] gvdd_mux_2level_tapbuf_size16[13] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[22] trig v(mux_2level_tapbuf_size16[13]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[13]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[22] trig v(mux_2level_tapbuf_size16[13]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[13]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[22] when v(mux_2level_tapbuf_size16[13]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[22] trig v(mux_2level_tapbuf_size16[13]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[13]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[22] when v(mux_2level_tapbuf_size16[13]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[22] trig v(mux_2level_tapbuf_size16[13]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[13]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[13]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[13]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[22] param='mux_2level_tapbuf_size16[13]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[13]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[13]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[13]_energy_per_cycle param='mux_2level_tapbuf_size16[13]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[22]  param='mux_2level_tapbuf_size16[13]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[22]  param='dynamic_power_cb_mux[0][1]_rrnode[22]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[22] avg p(Vgvdd_mux_2level_tapbuf_size16[13]) from='start_rise_cb_mux[0][1]_rrnode[22]' to='start_rise_cb_mux[0][1]_rrnode[22]+switch_rise_cb_mux[0][1]_rrnode[22]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[22] avg p(Vgvdd_mux_2level_tapbuf_size16[13]) from='start_fall_cb_mux[0][1]_rrnode[22]' to='start_fall_cb_mux[0][1]_rrnode[22]+switch_fall_cb_mux[0][1]_rrnode[22]'
.meas tran sum_leakage_power_mux[0to13] 
+          param='sum_leakage_power_mux[0to12]+leakage_cb_mux[0][1]_rrnode[22]'
.meas tran sum_energy_per_cycle_mux[0to13] 
+          param='sum_energy_per_cycle_mux[0to12]+energy_per_cycle_cb_mux[0][1]_rrnode[22]'
******* IO_TYPE loads *******
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to13] 
+          param='sum_leakage_power_cb_mux[0to12]+leakage_cb_mux[0][1]_rrnode[22]'
.meas tran sum_energy_per_cycle_cb_mux[0to13] 
+          param='sum_energy_per_cycle_cb_mux[0to12]+energy_per_cycle_cb_mux[0][1]_rrnode[22]'
Xmux_2level_tapbuf_size16[14] mux_2level_tapbuf_size16[14]->in[0] mux_2level_tapbuf_size16[14]->in[1] mux_2level_tapbuf_size16[14]->in[2] mux_2level_tapbuf_size16[14]->in[3] mux_2level_tapbuf_size16[14]->in[4] mux_2level_tapbuf_size16[14]->in[5] mux_2level_tapbuf_size16[14]->in[6] mux_2level_tapbuf_size16[14]->in[7] mux_2level_tapbuf_size16[14]->in[8] mux_2level_tapbuf_size16[14]->in[9] mux_2level_tapbuf_size16[14]->in[10] mux_2level_tapbuf_size16[14]->in[11] mux_2level_tapbuf_size16[14]->in[12] mux_2level_tapbuf_size16[14]->in[13] mux_2level_tapbuf_size16[14]->in[14] mux_2level_tapbuf_size16[14]->in[15] mux_2level_tapbuf_size16[14]->out sram[112]->outb sram[112]->out sram[113]->out sram[113]->outb sram[114]->out sram[114]->outb sram[115]->out sram[115]->outb sram[116]->outb sram[116]->out sram[117]->out sram[117]->outb sram[118]->out sram[118]->outb sram[119]->out sram[119]->outb gvdd_mux_2level_tapbuf_size16[14] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[14], level=2, select_path_id=0. *****
*****10001000*****
Xsram[112] sram->in sram[112]->out sram[112]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[112]->out) 0
.nodeset V(sram[112]->outb) vsp
Xsram[113] sram->in sram[113]->out sram[113]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[113]->out) 0
.nodeset V(sram[113]->outb) vsp
Xsram[114] sram->in sram[114]->out sram[114]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[114]->out) 0
.nodeset V(sram[114]->outb) vsp
Xsram[115] sram->in sram[115]->out sram[115]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[115]->out) 0
.nodeset V(sram[115]->outb) vsp
Xsram[116] sram->in sram[116]->out sram[116]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[116]->out) 0
.nodeset V(sram[116]->outb) vsp
Xsram[117] sram->in sram[117]->out sram[117]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[117]->out) 0
.nodeset V(sram[117]->outb) vsp
Xsram[118] sram->in sram[118]->out sram[118]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[118]->out) 0
.nodeset V(sram[118]->outb) vsp
Xsram[119] sram->in sram[119]->out sram[119]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[119]->out) 0
.nodeset V(sram[119]->outb) vsp
***** Signal mux_2level_tapbuf_size16[14]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[0] mux_2level_tapbuf_size16[14]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[1] mux_2level_tapbuf_size16[14]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[2] mux_2level_tapbuf_size16[14]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[3] mux_2level_tapbuf_size16[14]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[4] mux_2level_tapbuf_size16[14]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[5] mux_2level_tapbuf_size16[14]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[6] mux_2level_tapbuf_size16[14]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[7] mux_2level_tapbuf_size16[14]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[8] mux_2level_tapbuf_size16[14]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[9] mux_2level_tapbuf_size16[14]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[10] mux_2level_tapbuf_size16[14]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[11] mux_2level_tapbuf_size16[14]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[12] mux_2level_tapbuf_size16[14]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[13] mux_2level_tapbuf_size16[14]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[14] mux_2level_tapbuf_size16[14]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[14]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[14]->in[15] mux_2level_tapbuf_size16[14]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[14] gvdd_mux_2level_tapbuf_size16[14] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[24] trig v(mux_2level_tapbuf_size16[14]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[14]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[24] trig v(mux_2level_tapbuf_size16[14]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[14]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[24] when v(mux_2level_tapbuf_size16[14]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[24] trig v(mux_2level_tapbuf_size16[14]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[14]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[24] when v(mux_2level_tapbuf_size16[14]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[24] trig v(mux_2level_tapbuf_size16[14]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[14]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[14]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[14]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[24] param='mux_2level_tapbuf_size16[14]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[14]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[14]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[14]_energy_per_cycle param='mux_2level_tapbuf_size16[14]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[24]  param='mux_2level_tapbuf_size16[14]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[24]  param='dynamic_power_cb_mux[0][1]_rrnode[24]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[24] avg p(Vgvdd_mux_2level_tapbuf_size16[14]) from='start_rise_cb_mux[0][1]_rrnode[24]' to='start_rise_cb_mux[0][1]_rrnode[24]+switch_rise_cb_mux[0][1]_rrnode[24]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[24] avg p(Vgvdd_mux_2level_tapbuf_size16[14]) from='start_fall_cb_mux[0][1]_rrnode[24]' to='start_fall_cb_mux[0][1]_rrnode[24]+switch_fall_cb_mux[0][1]_rrnode[24]'
.meas tran sum_leakage_power_mux[0to14] 
+          param='sum_leakage_power_mux[0to13]+leakage_cb_mux[0][1]_rrnode[24]'
.meas tran sum_energy_per_cycle_mux[0to14] 
+          param='sum_energy_per_cycle_mux[0to13]+energy_per_cycle_cb_mux[0][1]_rrnode[24]'
******* IO_TYPE loads *******
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to14] 
+          param='sum_leakage_power_cb_mux[0to13]+leakage_cb_mux[0][1]_rrnode[24]'
.meas tran sum_energy_per_cycle_cb_mux[0to14] 
+          param='sum_energy_per_cycle_cb_mux[0to13]+energy_per_cycle_cb_mux[0][1]_rrnode[24]'
Xmux_2level_tapbuf_size16[15] mux_2level_tapbuf_size16[15]->in[0] mux_2level_tapbuf_size16[15]->in[1] mux_2level_tapbuf_size16[15]->in[2] mux_2level_tapbuf_size16[15]->in[3] mux_2level_tapbuf_size16[15]->in[4] mux_2level_tapbuf_size16[15]->in[5] mux_2level_tapbuf_size16[15]->in[6] mux_2level_tapbuf_size16[15]->in[7] mux_2level_tapbuf_size16[15]->in[8] mux_2level_tapbuf_size16[15]->in[9] mux_2level_tapbuf_size16[15]->in[10] mux_2level_tapbuf_size16[15]->in[11] mux_2level_tapbuf_size16[15]->in[12] mux_2level_tapbuf_size16[15]->in[13] mux_2level_tapbuf_size16[15]->in[14] mux_2level_tapbuf_size16[15]->in[15] mux_2level_tapbuf_size16[15]->out sram[120]->outb sram[120]->out sram[121]->out sram[121]->outb sram[122]->out sram[122]->outb sram[123]->out sram[123]->outb sram[124]->outb sram[124]->out sram[125]->out sram[125]->outb sram[126]->out sram[126]->outb sram[127]->out sram[127]->outb gvdd_mux_2level_tapbuf_size16[15] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[15], level=2, select_path_id=0. *****
*****10001000*****
Xsram[120] sram->in sram[120]->out sram[120]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[120]->out) 0
.nodeset V(sram[120]->outb) vsp
Xsram[121] sram->in sram[121]->out sram[121]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[121]->out) 0
.nodeset V(sram[121]->outb) vsp
Xsram[122] sram->in sram[122]->out sram[122]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[122]->out) 0
.nodeset V(sram[122]->outb) vsp
Xsram[123] sram->in sram[123]->out sram[123]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[123]->out) 0
.nodeset V(sram[123]->outb) vsp
Xsram[124] sram->in sram[124]->out sram[124]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[124]->out) 0
.nodeset V(sram[124]->outb) vsp
Xsram[125] sram->in sram[125]->out sram[125]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[125]->out) 0
.nodeset V(sram[125]->outb) vsp
Xsram[126] sram->in sram[126]->out sram[126]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[126]->out) 0
.nodeset V(sram[126]->outb) vsp
Xsram[127] sram->in sram[127]->out sram[127]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[127]->out) 0
.nodeset V(sram[127]->outb) vsp
***** Signal mux_2level_tapbuf_size16[15]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[0] mux_2level_tapbuf_size16[15]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[1] mux_2level_tapbuf_size16[15]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[2] mux_2level_tapbuf_size16[15]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[3] mux_2level_tapbuf_size16[15]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[4] mux_2level_tapbuf_size16[15]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[5] mux_2level_tapbuf_size16[15]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[6] mux_2level_tapbuf_size16[15]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[7] mux_2level_tapbuf_size16[15]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[8] mux_2level_tapbuf_size16[15]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[9] mux_2level_tapbuf_size16[15]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[10] mux_2level_tapbuf_size16[15]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[11] mux_2level_tapbuf_size16[15]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[12] mux_2level_tapbuf_size16[15]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[13] mux_2level_tapbuf_size16[15]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[14] mux_2level_tapbuf_size16[15]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[15]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[15]->in[15] mux_2level_tapbuf_size16[15]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[15] gvdd_mux_2level_tapbuf_size16[15] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[26] trig v(mux_2level_tapbuf_size16[15]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[15]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[26] trig v(mux_2level_tapbuf_size16[15]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[15]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[26] when v(mux_2level_tapbuf_size16[15]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[26] trig v(mux_2level_tapbuf_size16[15]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[15]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[26] when v(mux_2level_tapbuf_size16[15]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[26] trig v(mux_2level_tapbuf_size16[15]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[15]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[15]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[15]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[26] param='mux_2level_tapbuf_size16[15]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[15]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[15]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[15]_energy_per_cycle param='mux_2level_tapbuf_size16[15]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[26]  param='mux_2level_tapbuf_size16[15]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[26]  param='dynamic_power_cb_mux[0][1]_rrnode[26]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[26] avg p(Vgvdd_mux_2level_tapbuf_size16[15]) from='start_rise_cb_mux[0][1]_rrnode[26]' to='start_rise_cb_mux[0][1]_rrnode[26]+switch_rise_cb_mux[0][1]_rrnode[26]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[26] avg p(Vgvdd_mux_2level_tapbuf_size16[15]) from='start_fall_cb_mux[0][1]_rrnode[26]' to='start_fall_cb_mux[0][1]_rrnode[26]+switch_fall_cb_mux[0][1]_rrnode[26]'
.meas tran sum_leakage_power_mux[0to15] 
+          param='sum_leakage_power_mux[0to14]+leakage_cb_mux[0][1]_rrnode[26]'
.meas tran sum_energy_per_cycle_mux[0to15] 
+          param='sum_energy_per_cycle_mux[0to14]+energy_per_cycle_cb_mux[0][1]_rrnode[26]'
******* IO_TYPE loads *******
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to15] 
+          param='sum_leakage_power_cb_mux[0to14]+leakage_cb_mux[0][1]_rrnode[26]'
.meas tran sum_energy_per_cycle_cb_mux[0to15] 
+          param='sum_energy_per_cycle_cb_mux[0to14]+energy_per_cycle_cb_mux[0][1]_rrnode[26]'
Xmux_2level_tapbuf_size16[16] mux_2level_tapbuf_size16[16]->in[0] mux_2level_tapbuf_size16[16]->in[1] mux_2level_tapbuf_size16[16]->in[2] mux_2level_tapbuf_size16[16]->in[3] mux_2level_tapbuf_size16[16]->in[4] mux_2level_tapbuf_size16[16]->in[5] mux_2level_tapbuf_size16[16]->in[6] mux_2level_tapbuf_size16[16]->in[7] mux_2level_tapbuf_size16[16]->in[8] mux_2level_tapbuf_size16[16]->in[9] mux_2level_tapbuf_size16[16]->in[10] mux_2level_tapbuf_size16[16]->in[11] mux_2level_tapbuf_size16[16]->in[12] mux_2level_tapbuf_size16[16]->in[13] mux_2level_tapbuf_size16[16]->in[14] mux_2level_tapbuf_size16[16]->in[15] mux_2level_tapbuf_size16[16]->out sram[128]->outb sram[128]->out sram[129]->out sram[129]->outb sram[130]->out sram[130]->outb sram[131]->out sram[131]->outb sram[132]->outb sram[132]->out sram[133]->out sram[133]->outb sram[134]->out sram[134]->outb sram[135]->out sram[135]->outb gvdd_mux_2level_tapbuf_size16[16] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[16], level=2, select_path_id=0. *****
*****10001000*****
Xsram[128] sram->in sram[128]->out sram[128]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[128]->out) 0
.nodeset V(sram[128]->outb) vsp
Xsram[129] sram->in sram[129]->out sram[129]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[129]->out) 0
.nodeset V(sram[129]->outb) vsp
Xsram[130] sram->in sram[130]->out sram[130]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[130]->out) 0
.nodeset V(sram[130]->outb) vsp
Xsram[131] sram->in sram[131]->out sram[131]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[131]->out) 0
.nodeset V(sram[131]->outb) vsp
Xsram[132] sram->in sram[132]->out sram[132]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[132]->out) 0
.nodeset V(sram[132]->outb) vsp
Xsram[133] sram->in sram[133]->out sram[133]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[133]->out) 0
.nodeset V(sram[133]->outb) vsp
Xsram[134] sram->in sram[134]->out sram[134]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[134]->out) 0
.nodeset V(sram[134]->outb) vsp
Xsram[135] sram->in sram[135]->out sram[135]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[135]->out) 0
.nodeset V(sram[135]->outb) vsp
***** Signal mux_2level_tapbuf_size16[16]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[0] mux_2level_tapbuf_size16[16]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[1] mux_2level_tapbuf_size16[16]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[2] mux_2level_tapbuf_size16[16]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[3] mux_2level_tapbuf_size16[16]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[4] mux_2level_tapbuf_size16[16]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[5] mux_2level_tapbuf_size16[16]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[6] mux_2level_tapbuf_size16[16]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[7] mux_2level_tapbuf_size16[16]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[8] mux_2level_tapbuf_size16[16]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[9] mux_2level_tapbuf_size16[16]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[10] mux_2level_tapbuf_size16[16]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[11] mux_2level_tapbuf_size16[16]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[12] mux_2level_tapbuf_size16[16]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[13] mux_2level_tapbuf_size16[16]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[14] mux_2level_tapbuf_size16[16]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[16]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[16]->in[15] mux_2level_tapbuf_size16[16]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[16] gvdd_mux_2level_tapbuf_size16[16] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[28] trig v(mux_2level_tapbuf_size16[16]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[16]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[28] trig v(mux_2level_tapbuf_size16[16]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[16]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[28] when v(mux_2level_tapbuf_size16[16]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[28] trig v(mux_2level_tapbuf_size16[16]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[16]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[28] when v(mux_2level_tapbuf_size16[16]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[28] trig v(mux_2level_tapbuf_size16[16]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[16]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[16]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[16]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[28] param='mux_2level_tapbuf_size16[16]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[16]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[16]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[16]_energy_per_cycle param='mux_2level_tapbuf_size16[16]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[28]  param='mux_2level_tapbuf_size16[16]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[28]  param='dynamic_power_cb_mux[0][1]_rrnode[28]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[28] avg p(Vgvdd_mux_2level_tapbuf_size16[16]) from='start_rise_cb_mux[0][1]_rrnode[28]' to='start_rise_cb_mux[0][1]_rrnode[28]+switch_rise_cb_mux[0][1]_rrnode[28]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[28] avg p(Vgvdd_mux_2level_tapbuf_size16[16]) from='start_fall_cb_mux[0][1]_rrnode[28]' to='start_fall_cb_mux[0][1]_rrnode[28]+switch_fall_cb_mux[0][1]_rrnode[28]'
.meas tran sum_leakage_power_mux[0to16] 
+          param='sum_leakage_power_mux[0to15]+leakage_cb_mux[0][1]_rrnode[28]'
.meas tran sum_energy_per_cycle_mux[0to16] 
+          param='sum_energy_per_cycle_mux[0to15]+energy_per_cycle_cb_mux[0][1]_rrnode[28]'
******* IO_TYPE loads *******
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to16] 
+          param='sum_leakage_power_cb_mux[0to15]+leakage_cb_mux[0][1]_rrnode[28]'
.meas tran sum_energy_per_cycle_cb_mux[0to16] 
+          param='sum_energy_per_cycle_cb_mux[0to15]+energy_per_cycle_cb_mux[0][1]_rrnode[28]'
Xmux_2level_tapbuf_size16[17] mux_2level_tapbuf_size16[17]->in[0] mux_2level_tapbuf_size16[17]->in[1] mux_2level_tapbuf_size16[17]->in[2] mux_2level_tapbuf_size16[17]->in[3] mux_2level_tapbuf_size16[17]->in[4] mux_2level_tapbuf_size16[17]->in[5] mux_2level_tapbuf_size16[17]->in[6] mux_2level_tapbuf_size16[17]->in[7] mux_2level_tapbuf_size16[17]->in[8] mux_2level_tapbuf_size16[17]->in[9] mux_2level_tapbuf_size16[17]->in[10] mux_2level_tapbuf_size16[17]->in[11] mux_2level_tapbuf_size16[17]->in[12] mux_2level_tapbuf_size16[17]->in[13] mux_2level_tapbuf_size16[17]->in[14] mux_2level_tapbuf_size16[17]->in[15] mux_2level_tapbuf_size16[17]->out sram[136]->outb sram[136]->out sram[137]->out sram[137]->outb sram[138]->out sram[138]->outb sram[139]->out sram[139]->outb sram[140]->outb sram[140]->out sram[141]->out sram[141]->outb sram[142]->out sram[142]->outb sram[143]->out sram[143]->outb gvdd_mux_2level_tapbuf_size16[17] 0 mux_2level_tapbuf_size16
***** SRAM bits for MUX[17], level=2, select_path_id=0. *****
*****10001000*****
Xsram[136] sram->in sram[136]->out sram[136]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[136]->out) 0
.nodeset V(sram[136]->outb) vsp
Xsram[137] sram->in sram[137]->out sram[137]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[137]->out) 0
.nodeset V(sram[137]->outb) vsp
Xsram[138] sram->in sram[138]->out sram[138]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[138]->out) 0
.nodeset V(sram[138]->outb) vsp
Xsram[139] sram->in sram[139]->out sram[139]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[139]->out) 0
.nodeset V(sram[139]->outb) vsp
Xsram[140] sram->in sram[140]->out sram[140]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[140]->out) 0
.nodeset V(sram[140]->outb) vsp
Xsram[141] sram->in sram[141]->out sram[141]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[141]->out) 0
.nodeset V(sram[141]->outb) vsp
Xsram[142] sram->in sram[142]->out sram[142]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[142]->out) 0
.nodeset V(sram[142]->outb) vsp
Xsram[143] sram->in sram[143]->out sram[143]->outb gvdd_sram sgnd  sram6T
.nodeset V(sram[143]->out) 0
.nodeset V(sram[143]->outb) vsp
***** Signal mux_2level_tapbuf_size16[17]->in[0] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[0] mux_2level_tapbuf_size16[17]->in[0] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[1] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[1] mux_2level_tapbuf_size16[17]->in[1] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[2] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[2] mux_2level_tapbuf_size16[17]->in[2] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[3] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[3] mux_2level_tapbuf_size16[17]->in[3] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[4] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[4] mux_2level_tapbuf_size16[17]->in[4] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[5] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[5] mux_2level_tapbuf_size16[17]->in[5] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[6] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[6] mux_2level_tapbuf_size16[17]->in[6] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[7] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[7] mux_2level_tapbuf_size16[17]->in[7] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[8] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[8] mux_2level_tapbuf_size16[17]->in[8] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[9] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[9] mux_2level_tapbuf_size16[17]->in[9] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[10] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[10] mux_2level_tapbuf_size16[17]->in[10] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[11] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[11] mux_2level_tapbuf_size16[17]->in[11] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[12] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[12] mux_2level_tapbuf_size16[17]->in[12] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[13] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[13] mux_2level_tapbuf_size16[17]->in[13] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[14] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[14] mux_2level_tapbuf_size16[17]->in[14] 0 
+  0
***** Signal mux_2level_tapbuf_size16[17]->in[15] density = 0, probability=0.*****
Vmux_2level_tapbuf_size16[17]->in[15] mux_2level_tapbuf_size16[17]->in[15] 0 
+  0
Vgvdd_mux_2level_tapbuf_size16[17] gvdd_mux_2level_tapbuf_size16[17] 0 vsp
***** Measurements *****
***** Rise delay *****
.meas tran delay_rise_cb_mux[0][1]_rrnode[30] trig v(mux_2level_tapbuf_size16[17]->in[0]) val='input_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[17]->out) val='output_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall delay *****
.meas tran delay_fall_cb_mux[0][1]_rrnode[30] trig v(mux_2level_tapbuf_size16[17]->in[0]) val='input_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[17]->out) val='output_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Rise timing period *****
.meas start_rise_cb_mux[0][1]_rrnode[30] when v(mux_2level_tapbuf_size16[17]->in[0])='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
.meas tran switch_rise_cb_mux[0][1]_rrnode[30] trig v(mux_2level_tapbuf_size16[17]->in[0]) val='slew_lower_thres_pct_rise*vsp' rise=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[17]->out) val='slew_upper_thres_pct_rise*vsp' rise=1 td='clock_period'
***** Fall timing period *****
.meas start_fall_cb_mux[0][1]_rrnode[30] when v(mux_2level_tapbuf_size16[17]->in[0])='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
.meas tran switch_fall_cb_mux[0][1]_rrnode[30] trig v(mux_2level_tapbuf_size16[17]->in[0]) val='slew_lower_thres_pct_fall*vsp' fall=1 td='clock_period'
+          targ v(mux_2level_tapbuf_size16[17]->out) val='slew_upper_thres_pct_fall*vsp' fall=1 td='clock_period'
***** Leakage Power Measurement *****
.meas tran mux_2level_tapbuf_size16[17]_leakage_power avg p(Vgvdd_mux_2level_tapbuf_size16[17]) from=0 to='clock_period'
.meas tran leakage_cb_mux[0][1]_rrnode[30] param='mux_2level_tapbuf_size16[17]_leakage_power'
***** Dynamic Power Measurement *****
.meas tran mux_2level_tapbuf_size16[17]_dynamic_power avg p(Vgvdd_mux_2level_tapbuf_size16[17]) from='clock_period' to='2*clock_period'
.meas tran mux_2level_tapbuf_size16[17]_energy_per_cycle param='mux_2level_tapbuf_size16[17]_dynamic_power*clock_period'
.meas tran dynamic_power_cb_mux[0][1]_rrnode[30]  param='mux_2level_tapbuf_size16[17]_dynamic_power'
.meas tran energy_per_cycle_cb_mux[0][1]_rrnode[30]  param='dynamic_power_cb_mux[0][1]_rrnode[30]*clock_period'
.meas tran dynamic_rise_cb_mux[0][1]_rrnode[30] avg p(Vgvdd_mux_2level_tapbuf_size16[17]) from='start_rise_cb_mux[0][1]_rrnode[30]' to='start_rise_cb_mux[0][1]_rrnode[30]+switch_rise_cb_mux[0][1]_rrnode[30]'
.meas tran dynamic_fall_cb_mux[0][1]_rrnode[30] avg p(Vgvdd_mux_2level_tapbuf_size16[17]) from='start_fall_cb_mux[0][1]_rrnode[30]' to='start_fall_cb_mux[0][1]_rrnode[30]+switch_fall_cb_mux[0][1]_rrnode[30]'
.meas tran sum_leakage_power_mux[0to17] 
+          param='sum_leakage_power_mux[0to16]+leakage_cb_mux[0][1]_rrnode[30]'
.meas tran sum_energy_per_cycle_mux[0to17] 
+          param='sum_energy_per_cycle_mux[0to16]+energy_per_cycle_cb_mux[0][1]_rrnode[30]'
******* IO_TYPE loads *******
******* END loads *******
.meas tran sum_leakage_power_cb_mux[0to17] 
+          param='sum_leakage_power_cb_mux[0to16]+leakage_cb_mux[0][1]_rrnode[30]'
.meas tran sum_energy_per_cycle_cb_mux[0to17] 
+          param='sum_energy_per_cycle_cb_mux[0to16]+energy_per_cycle_cb_mux[0][1]_rrnode[30]'
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
.meas tran total_leakage_power_mux[0to17] 
+          param='sum_leakage_power_mux[0to17]'
.meas tran total_energy_per_cycle_mux[0to17] 
+          param='sum_energy_per_cycle_mux[0to17]'
.meas tran total_leakage_power_cb_mux 
+          param='sum_leakage_power_cb_mux[0to17]'
.meas tran total_energy_per_cycle_cb_mux 
+          param='sum_energy_per_cycle_cb_mux[0to17]'
.end
