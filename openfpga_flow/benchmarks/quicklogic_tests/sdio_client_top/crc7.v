///////////////////////////////////////////////////////////////////////////////
//
//
// Copyright (C) 2007, Licensed customers of QuickLogic may copy or modify
// this file for use in designing QuickLogic devices only.
//
// Module Name:  crc7
// File Name:    crc7.v
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ns/1ns

module crc7(
  rst,
  clk,
  gen_en,
  out_en,
  din,
  dout,
  crc_reg
);

input  rst;
input  clk;
input  gen_en;
input  out_en;
input  din;
output dout;
output [6:0] crc_reg;

wire   rst;
wire   clk;
wire   gen_en;
wire   out_en;
wire   din;
wire   dout;
wire   crc_din;

reg [6:0] crc_reg;

  assign dout = crc_reg[6];
  assign crc_din = din;

  always @(posedge rst or posedge clk) 
    begin
      if( rst ) 
	  begin
        crc_reg          <= 7'b0;
      end
      else
      begin
        if( gen_en ) 
		begin
          crc_reg[6:4]   <= crc_reg[5:3] ;
          crc_reg[3]     <= crc_din ^ crc_reg[6] ^ crc_reg[2];
          crc_reg[2:1]   <= crc_reg[1:0] ;
          crc_reg[0]     <= crc_din ^ crc_reg[6];
        end
        else if( out_en ) 
		begin
          crc_reg[6:1]   <= crc_reg[5:0] ;
          crc_reg[0]     <= 1'b 0;
        end
      end
    end


endmodule
