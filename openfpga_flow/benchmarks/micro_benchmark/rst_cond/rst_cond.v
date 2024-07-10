/////////////////////////////////////////
//  Functionality: A locally generated reset signal which is to test clock network with internal drivers
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module rst_cond(rst_i, rst_cond_i, clk_i, d_i, q_o);

input wire rst_cond_i;
input wire rst_i;
input wire clk_i;
input wire d_i;
output reg q_o;

wire int_rst;
assign int_rst = rst_cond_i & rst_i;

always @(posedge int_rst or posedge clk_i) begin
  if (int_rst) begin
    q_o <= 0;
  end else begin
    q_o <= d_i;
  end
end

endmodule
