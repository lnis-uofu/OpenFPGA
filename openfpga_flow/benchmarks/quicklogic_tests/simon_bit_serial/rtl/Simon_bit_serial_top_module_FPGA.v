`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Team: Virginia Tech Secure Embedded Systems (SES) Lab 
// Implementer: Ege Gulcan
// 
// Create Date:    19:14:37 11/13/2013 
// Design Name: 
// Module Name:    top_module 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module top_module(clk,data_in,data_rdy,cipher_out);

input clk,data_in;
input [1:0] data_rdy;
output cipher_out;

wire key;
wire [5:0] bit_counter;
wire round_counter_out;

/*
	data_rdy=0 -> Reset, Idle
	data_rdy=1 -> Load Plaintext
	data_rdy=2 -> Load Key
	data_rdy=3 -> Run (keep at 3 while the block cipher is running)
*/

simon_datapath_shiftreg datapath(.clk(clk), .data_in(data_in), .data_rdy(data_rdy), .key_in(key), 
								 . cipher_out(cipher_out), .round_counter(round_counter_out), .bit_counter(bit_counter));
											
simon_key_expansion_shiftreg key_exp(.clk(clk), .data_in(data_in), .data_rdy(data_rdy), .key_out(key), .bit_counter(bit_counter), 
									 .round_counter_out(round_counter_out));


endmodule
