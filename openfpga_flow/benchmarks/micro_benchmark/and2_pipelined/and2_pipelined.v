/////////////////////////////////////////
//  Functionality: a pipelined 2-input AND 
//                 where inputs and outputs are registered
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module and2_pipelined(
  clk,
  a,
  b,
  c);

input wire clk;
input wire a;
input wire b;
output wire c;

reg a_reg;
reg b_reg;
reg c_reg;

always @(posedge clk) begin
  a_reg <= a;
  b_reg <= b;
end

always @(posedge clk) begin
  c_reg <= a_reg & b_reg;
end

assign c = c_reg;

endmodule
