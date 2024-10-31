/////////////////////////////////////////
//  Functionality: 4-bit version of A register driven by a combinational logic with reset signal
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module rst_on_lut_4bit(a, b0, b1, b2, b3, q, out0, out1, out2, out3, clk, rst);

input wire rst;
input wire clk;
input wire a;
input wire b0;
input wire b1;
input wire b2;
input wire b3;
output reg q;
output wire out0;
output wire out1;
output wire out2;
output wire out3;

always @(posedge rst or posedge clk) begin
  if (rst) begin
    q <= 0;
  end else begin
    q <= a;
  end
end

assign out0 = b0 & ~rst;
assign out1 = b1 & ~rst;
assign out2 = b2 & ~rst;
assign out3 = b3 & ~rst;

endmodule
