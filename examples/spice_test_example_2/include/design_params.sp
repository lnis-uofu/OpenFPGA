*****************************
*     FPGA SPICE Netlist    *
* Description: Parameters for Circuit Designs *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:08 2018
 *
*****************************
****** Include Technology Library ******
.lib '/research/ece/lnis/USERS/tang/tangxifan-eda-tools/branches/subvt_fpga/process/tsmc40nm/toplevel.l' TOP_TT
****** Transistor Parameters ******
.param beta=2
.param nl=4e-08
.param wn=1.4e-07
.param pl=4e-08
.param wp=1.4e-07
.param io_nl=2.7e-07
.param io_wn=3.2e-07
.param io_pl=2.7e-07
.param io_wp=3.2e-07
.param vsp=0.9
.param io_vsp=2.5
***** Parameters for Circuits *****
***** Parameters for SPICE MODEL: INVTX1 *****
***** Parameters for SPICE MODEL: buf4 *****
***** Parameters for SPICE MODEL: tap_buf4 *****
***** Parameters for SPICE MODEL: TGATE *****
***** Parameters for SPICE MODEL: chan_segment *****
.param chan_segment_wire_param_res_val=101
.param chan_segment_wire_param_cap_val=2.25e-14
***** Parameters for SPICE MODEL: direct_interc *****
.param direct_interc_wire_param_res_val=0
.param direct_interc_wire_param_cap_val=0
***** Parameters for SPICE MODEL: mux_1level_tapbuf *****
.param mux_1level_tapbuf_input_buf_size=1
.param mux_1level_tapbuf_output_buf_size=1
.param mux_1level_tapbuf_pgl_pmos_size=2
.param mux_1level_tapbuf_pgl_nmos_size=1
***** Parameters for SPICE MODEL: mux_2level *****
.param mux_2level_input_buf_size=1
.param mux_2level_output_buf_size=1
.param mux_2level_pgl_pmos_size=2
.param mux_2level_pgl_nmos_size=1
***** Parameters for SPICE MODEL: mux_2level_tapbuf *****
.param mux_2level_tapbuf_input_buf_size=1
.param mux_2level_tapbuf_output_buf_size=1
.param mux_2level_tapbuf_pgl_pmos_size=2
.param mux_2level_tapbuf_pgl_nmos_size=1
***** Parameters for SPICE MODEL: static_dff *****
.param static_dff_input_buf_size=1
.param static_dff_output_buf_size=1
.param static_dff_pgl_pmos_size=2.22937e-38
.param static_dff_pgl_nmos_size=0
***** Parameters for SPICE MODEL: lut6 *****
.param lut6_input_buf_size=1
.param lut6_output_buf_size=1
.param lut6_pgl_pmos_size=2
.param lut6_pgl_nmos_size=1
***** Parameters for SPICE MODEL: sram6T *****
.param sram6T_input_buf_size=1
.param sram6T_output_buf_size=1
.param sram6T_pgl_pmos_size=2.22995e-38
.param sram6T_pgl_nmos_size=0
***** Parameters for SPICE MODEL: sram6T_blwl *****
.param sram6T_blwl_input_buf_size=1
.param sram6T_blwl_output_buf_size=1
.param sram6T_blwl_pgl_pmos_size=2.23018e-38
.param sram6T_blwl_pgl_nmos_size=0
***** Parameters for SPICE MODEL: iopad *****
.param iopad_input_buf_size=1
.param iopad_output_buf_size=1
.param iopad_pgl_pmos_size=2.23051e-38
.param iopad_pgl_nmos_size=0
