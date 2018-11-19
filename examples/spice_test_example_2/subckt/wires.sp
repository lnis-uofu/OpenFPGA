*****************************
*     FPGA SPICE Netlist    *
* Description: Wires *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:08 2018
 *
*****************************
* Wire, spice_model_name=direct_interc
.subckt direct_interc in out svdd sgnd
Rshortcut in out 0
.eom

* Wire models for segments in routing 
* Wire, spice_model_name=chan_segment
.subckt chan_segment_seg0 in out mid_out svdd sgnd
Clvin pie_wire_in0 sgnd 'chan_segment_wire_param_cap_val/2'
Rlv0_idx0 pie_wire_in0 pie_wire_in0_inter 'chan_segment_wire_param_res_val/2'
Rlv0_idx1 pie_wire_in0_inter pie_wire_in1 'chan_segment_wire_param_res_val/2'
Clv0_idx1 pie_wire_in1 sgnd 'chan_segment_wire_param_cap_val/2'
* Connect the output of middle point
Vmid_out_ckt pie_wire_in0_inter mid_out 0
Rin in pie_wire_in0 0
Rout pie_wire_in1 out 0
.eom

* Wire, spice_model_name=chan_segment
.subckt chan_segment_seg1 in out mid_out svdd sgnd
Clvin pie_wire_in0 sgnd 'chan_segment_wire_param_cap_val/2'
Rlv0_idx0 pie_wire_in0 pie_wire_in0_inter 'chan_segment_wire_param_res_val/2'
Rlv0_idx1 pie_wire_in0_inter pie_wire_in1 'chan_segment_wire_param_res_val/2'
Clv0_idx1 pie_wire_in1 sgnd 'chan_segment_wire_param_cap_val/2'
* Connect the output of middle point
Vmid_out_ckt pie_wire_in0_inter mid_out 0
Rin in pie_wire_in0 0
Rout pie_wire_in1 out 0
.eom

* Wire, spice_model_name=chan_segment
.subckt chan_segment_seg2 in out mid_out svdd sgnd
Clvin pie_wire_in0 sgnd 'chan_segment_wire_param_cap_val/2'
Rlv0_idx0 pie_wire_in0 pie_wire_in0_inter 'chan_segment_wire_param_res_val/2'
Rlv0_idx1 pie_wire_in0_inter pie_wire_in1 'chan_segment_wire_param_res_val/2'
Clv0_idx1 pie_wire_in1 sgnd 'chan_segment_wire_param_cap_val/2'
* Connect the output of middle point
Vmid_out_ckt pie_wire_in0_inter mid_out 0
Rin in pie_wire_in0 0
Rout pie_wire_in1 out 0
.eom

