//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Wires 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//-----Wire module, verilog_model_name=direct_interc -----
module direct_interc (
input wire in, output wire out);
		assign out = in;
endmodule
//-----END Wire module, verilog_model_name=direct_interc -----

//----- Wire models for segments in routing -----
//-----Wire module, verilog_model_name=chan_segment -----
module chan_segment_seg0 (
input wire in, output wire out, output wire mid_out);
	assign out = in;
	assign mid_out = in;
endmodule
//-----END Wire module, verilog_model_name=chan_segment -----

//-----Wire module, verilog_model_name=chan_segment -----
module chan_segment_seg1 (
input wire in, output wire out, output wire mid_out);
	assign out = in;
	assign mid_out = in;
endmodule
//-----END Wire module, verilog_model_name=chan_segment -----

//-----Wire module, verilog_model_name=chan_segment -----
module chan_segment_seg2 (
input wire in, output wire out, output wire mid_out);
	assign out = in;
	assign mid_out = in;
endmodule
//-----END Wire module, verilog_model_name=chan_segment -----

