/////////////////////////////////////////
//  Functionality: 4-input AND 
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module and4(
  a,
  b,
  c,
  d,
  e);

input wire a;
input wire b;
input wire c;
input wire d;
output wire e;

assign e = a & b & c & d;

endmodule
