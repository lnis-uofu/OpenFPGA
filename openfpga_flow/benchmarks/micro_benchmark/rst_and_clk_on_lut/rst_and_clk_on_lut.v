/////////////////////////////////////////
//  Functionality: A register driven by a combinational logic with reset signal
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module rst_and_clk_on_lut(a, b, c, q, out0, out1, clk, rst);

input wire rst;
input wire clk;
input wire a;
input wire b;
input wire c;
output reg q;
output wire out0;
output wire out1;

always @(posedge rst or posedge clk) begin
  if (rst) begin
    q <= 0;
  end else begin
    q <= a;
  end
end

assign out0 = b & ~rst;
assign out1 = c & ~clk;

endmodule
