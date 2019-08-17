Testbench for D-type Flip-flop with set and reset
*********************************
*     HSPICE Netlist            *  
*     Author:  Xifan TANG       *
*  Organization: EPFL,LSI       *
*********************************
*
* Use Standard CMOS Technology
****** Include Technology Library ******
.lib '/home/xitang/tangxifan-eda-tools/branches/subvt_fpga/process/tsmc40nm/toplevel_crn45gs_2d5_v1d1_shrink0d9_embedded_usage.l' TOP_TT
****** Transistor Parameters ******
.param beta=2
.param nl=4e-08
.param wn=1.4e-07
.param pl=4e-08
.param wp=1.4e-07

****** Include subckt netlists: NMOS and PMOS *****
.include '/home/xitang/tangxifan-eda-tools/branches/vpr7_rram/vpr/spice_test/subckt/nmos_pmos.sp'
****** Include subckt netlists: Inverters, Buffers *****
.include '/home/xitang/tangxifan-eda-tools/branches/vpr7_rram/vpr/spice_test/subckt/inv_buf_trans_gate.sp'

.include '/home/xitang/tangxifan-eda-tools/branches/vpr7_rram/vpr/SpiceNetlists/ff.sp'

.param clk_freq = 1e9
*Temperature
.temp 25
*Global nodes
.global vdd gnd
*Print node capacitance
.option captab
*Print waveforms
.option POST
* Parameters for measurements
.param clk2d=3e-09
.param clk_pwl=3e-09
.param clk_pwh=1.5e-08
.param slew=1e-11
.param thold=3e-09
.param vsp=0.9
* Parameters for Measuring Slew
.param slew_upper_threshold_pct_rise=0.9
.param slew_lower_threshold_pct_rise=0.1
.param slew_upper_threshold_pct_fall=0.1
.param slew_lower_threshold_pct_fall=0.9
* Parameters for Measuring Delay
.param input_threshold_pct_rise=0.5
.param input_threshold_pct_fall=0.5
.param output_threshold_pct_rise=0.5
.param output_threshold_pct_fall=0.5

Xdff[0] set rst clk d q vdd gnd static_dff

Vsupply vdd gnd 'vsp'
*Stimulates
vset set gnd 0
vrst rst gnd 0
vclk_in clk gnd pulse (0 vsp '0.5/clk_freq' '0.025/clk_freq' '0.025/clk_freq' '0.4875/clk_freq' '1/clk_freq') 
* Measuring Clk2Q, Setup Time and Hold Time
vdata D gnd pulse (0 vsp '0.25/clk_freq' '0.025/clk_freq' '0.025/clk_freq' '2*0.4875/clk_freq' '2/clk_freq') 

*Simulation
.tran 1e-15 '10/clk_freq'
.meas tran slew_q trig v(Q) val='slew_lower_threshold_pct_fall*vsp' fall=1 td='2*clk_pwl+clk_pwh+2*slew'
+                 targ v(Q) val='slew_upper_threshold_pct_fall*vsp' fall=1 td='2*clk_pwl+clk_pwh+2*slew'
.meas tran clk2q trig v(CLK) val='input_threshold_pct_fall*vsp' rise=2
+                targ v(Q) val='output_threshold_pct_fall*vsp' fall=1 td='2*clk_pwl+clk_pwh+2*slew'
.end TSPC Flip-flop with set and reset
