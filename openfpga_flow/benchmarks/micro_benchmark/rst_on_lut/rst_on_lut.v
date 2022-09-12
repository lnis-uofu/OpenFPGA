/////////////////////////////////////////
//  Functionality: A register driven by a combinational logic with reset signal
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module rst_on_lut(a, b, q, out, clk, rst);

input wire rst;
input wire clk;
input wire a;
input wire b;
output reg q;
output wire out;

always @(posedge rst or posedge clk) begin
  if (rst) begin
    q <= 0;
  end else begin
    q <= a;
  end
end

assign out = b & ~rst;

endmodule
