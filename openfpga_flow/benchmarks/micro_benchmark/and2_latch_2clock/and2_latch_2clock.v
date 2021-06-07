/////////////////////////////////////////
//  Functionality: Two 2-input AND with clocked 
//                 and combinational outputs
//                 Each of which are controlled by different clocks
//  Author:        Xifan Tang
////////////////////////////////////////

`timescale 1ns / 1ps

module and2_latch_2clock(
  a0,
  b0,
  clk0,
  a1,
  b1,
  clk1,
  c0,
  d0,
  c1,
  d1);

input wire clk0;

input wire a0;
input wire b0;
output wire c0;
output reg d0;

input wire clk1;

input wire a1;
input wire b1;
output wire c1;
output reg d1;


assign c0 = a0 & b0;

always @(posedge clk0) begin
  d0 <= c0;
end

assign c1 = a1 & b1;

always @(posedge clk1) begin
  d1 <= c1;
end

endmodule
