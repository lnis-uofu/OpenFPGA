/////////////////////////////////////////
//  Functionality: 2-input AND with clocked 
//                 and combinational outputs 
//  Author:        Xifan Tang
////////////////////////////////////////

`timescale 1ns / 1ps

module and2_latch(
  a,
  b,
  clk,
  c,
  d);

input wire clk;

input wire a;
input wire b;
output wire c;
output reg d;

assign c = a & b;

always @(posedge clk) begin
  d <= c;
end

endmodule
