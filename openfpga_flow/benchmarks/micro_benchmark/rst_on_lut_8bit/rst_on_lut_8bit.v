/////////////////////////////////////////
//  Functionality: 8-bit version of A register driven by a combinational logic with reset signal
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module rst_on_lut_8bit(a, b0, b1, b2, b3, b4, b5, b6, b7, q, out0, out1, out2, out3, out4, out5, out6, out7,  clk, rst);

input wire rst;
input wire clk;
input wire a;
input wire b0;
input wire b1;
input wire b2;
input wire b3;
input wire b4;
input wire b5;
input wire b6;
input wire b7;
output reg q;
output wire out0;
output wire out1;
output wire out2;
output wire out3;
output wire out4;
output wire out5;
output wire out6;
output wire out7;

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
assign out4 = b4 & ~rst;
assign out5 = b5 & ~rst;
assign out6 = b6 & ~rst;
assign out7 = b7 & ~rst;

endmodule
