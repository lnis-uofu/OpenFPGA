/////////////////////////////////////////
//  Functionality: A register driven by a combinational logic with reset signal
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module rst_on_lut(a, b, out, clk, rst);

input wire rst;
input wire clk;
input wire a;
input wire b;
output reg out;

always @(rst or posedge clk) begin
  if (rst) begin
    out <= 0;
  end else begin
    out <= a & b & rst;
  end
end

endmodule
