/////////////////////////////////////////
//  Functionality: 2-input OR 
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module or2(
  a,
  b,
  c);

input wire a;
input wire b;
output wire c;

assign c = a | b;

endmodule
