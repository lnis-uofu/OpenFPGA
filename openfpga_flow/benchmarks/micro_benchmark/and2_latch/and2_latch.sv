/////////////////////////////////////////
//  Functionality: 2-input AND with clocked 
//                 and combinational outputs 
//  Author:        Xifan Tang
////////////////////////////////////////

`timescale 1ns / 1ps

module and2_latch(
  input logic a,
  input logic b,
  input logic clk,
  output logic c,
  output logic d);

assign c = a & b;

always_ff @(posedge clk) begin
  d <= c;
end

endmodule
