/////////////////////////////////////////
//  Functionality: A register driven by a combinational logic with clk signal
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module clk_on_lut(a, b, q, out, clk);

input wire clk;
input wire a;
input wire b;
output reg q;
output wire out;

always @(posedge clk) begin
  q <= a;
end

assign out = b & clk;

endmodule
