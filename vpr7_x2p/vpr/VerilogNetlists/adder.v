//------ Module: sram6T_blwl -----//
//------ Verilog file: sram.v -----//
//------ Author: Xifan TANG -----//
module adder(
input [0:0] a, // Input a
input [0:0] b, // Input b
input [0:0] cin, // Input cin
output [0:0] cout, // Output carry
output [0:0] sumout // Output sum
);
wire[1:0] int_calc;

assign int_calc = a + b + cin;
assign cout = int_calc[1];
assign sumout = int_calc[0];
//  assign sumout = a ^ b ^ cin;
//  assign cout = a & b + a & cin + b & cin; 
endmodule

