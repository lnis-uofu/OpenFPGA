/////////////////////////////////////////
//  Functionality: A two-stage clock divider (Frequency is divided by 4)
//                 This is to test the clock generated locally by a LUT/FF
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module clk_divider(clk_i, clk_o);

input wire clk_i;
output reg clk_o;

reg int_clk;

initial begin
  clk_o <= 0;
  int_clk <= 0;
end

always @(posedge clk_i) begin
  int_clk <= ~int_clk;
end

always @(posedge int_clk) begin
  clk_o <= ~clk_o;
end

endmodule
