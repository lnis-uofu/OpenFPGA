/////////////////////////////////////////
//  Functionality: A locally generated clock signal which is to test clock network with internal drivers
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module clk_cond(clk_i, clk_cond_i, d_i, q_o);

input wire clk_cond_i;
input wire clk_i;
input wire d_i;
output reg q_o;

wire int_clk;
assign int_clk = clk_cond_i & clk_i;

always @(posedge int_clk) begin
  q_o <= d_i;
end

endmodule
