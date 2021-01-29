/////////////////////////////////////////
//  Functionality: 4x1 memory async read
//  Author:        Aurelien Alacchi
////////////////////////////////////////
`timescale 1ns / 1ps

module asyn_spram_4x1(
  clk,
  addr_0,
  addr_1,
  d_in,
  wr_en,
  d_out );

  input wire  clk;
  input wire  addr_0;
  input wire  addr_1;
  input wire  d_in;
  input wire  wr_en;
  output wire d_out;

  wire[1:0]   addr;
  reg [3:0]   mem;

  assign addr = {addr_1, addr_0};
  assign d_out = (addr == 2'd0)? mem[0]:
                 (addr == 2'd1)? mem[1]:
                 (addr == 2'd2)? mem[2]: mem[3];

  always@(posedge clk) begin
    if(wr_en) begin
      mem[addr] <= d_in;
    end
  end

endmodule
