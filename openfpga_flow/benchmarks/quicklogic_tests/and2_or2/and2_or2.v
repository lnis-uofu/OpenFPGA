/////////////////////////////////////////
//  Functionality: 2-input AND + 2-input OR 
//                 This benchmark is designed to test fracturable LUTs
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module and2_or2(
  a,
  b,
  c,
  d);

input wire a;
input wire b;
output wire c;
output wire d;

assign c = a & b;
assign d = a | b;

endmodule
