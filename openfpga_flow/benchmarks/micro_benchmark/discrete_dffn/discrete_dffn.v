/////////////////////////////////////////
//  Functionality: A FF with inverted clk. This is useful to test if an FPGA supports clock generation internally or an FPGA supports negative-edged clock
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module discrete_dffn(
  clk_ni,
  d_i,
  d_o);
input wire clk_ni;
input wire d_i;
output reg d_o;

wire int_clk;

assign int_clk = ~clk_ni;

always @(posedge int_clk) begin
  d_o <= d_i;
end

endmodule
