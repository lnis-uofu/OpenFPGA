//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_fifo.v                                                  ////
////                                                              ////
////  This file is part of the Ethernet IP core project           ////
////  http://www.opencores.org/projects/ethmac/                   ////
////                                                              ////
////  Author(s):                                                  ////
////      - Igor Mohor (igorM@opencores.org)                      ////
////                                                              ////
////  All additional information is avaliable in the Readme.txt   ////
////  file.                                                       ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2001 Authors                                   ////
////                                                              ////
//// This source file may be used and distributed without         ////
//// restriction provided that this copyright statement is not    ////
//// removed from the file and that any derivative work contains  ////
//// the original copyright notice and the associated disclaimer. ////
////                                                              ////
//// This source file is free software; you can redistribute it   ////
//// and/or modify it under the terms of the GNU Lesser General   ////
//// Public License as published by the Free Software Foundation; ////
//// either version 2.1 of the License, or (at your option) any   ////
//// later version.                                               ////
////                                                              ////
//// This source is distributed in the hope that it will be       ////
//// useful, but WITHOUT ANY WARRANTY; without even the implied   ////
//// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ////
//// PURPOSE.  See the GNU Lesser General Public License for more ////
//// details.                                                     ////
////                                                              ////
//// You should have received a copy of the GNU Lesser General    ////
//// Public License along with this source; if not, download it   ////
//// from http://www.opencores.org/lgpl.shtml                     ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//
// CVS Revision History
//
// $Log: eth_fifo.v,v $
// Revision 1.4  2005/02/21 12:48:07  igorm
// Warning fixes.
//
// Revision 1.3  2002/04/22 13:45:52  mohor
// Generic ram or Xilinx ram can be used in fifo (selectable by setting
// ETH_FIFO_XILINX in eth_defines.v).
//
// Revision 1.2  2002/03/25 13:33:04  mohor
// When clear and read/write are active at the same time, cnt and pointers are
// set to 1.
//
// Revision 1.1  2002/02/05 16:44:39  mohor
// Both rx and tx part are finished. Tested with wb_clk_i between 10 and 200
// MHz. Statuses, overrun, control frame transmission and reception still  need
// to be fixed.
//
//

`include "eth_defines.v"
`include "timescale.v"

module eth_fifo (data_in, data_out, clk, reset, write, read, clear, almost_full, full, almost_empty, empty, cnt);

parameter DATA_WIDTH    = 32;
parameter DEPTH         = 8;
parameter CNT_WIDTH     = 4;

parameter Tp            = 1;

input                     clk;
input                     reset;
input                     write;
input                     read;
input                     clear;
input   [DATA_WIDTH-1:0]  data_in;

output  [DATA_WIDTH-1:0]  data_out;
output                    almost_full;
output                    full;
output                    almost_empty;
output                    empty;
output  [CNT_WIDTH-1:0]   cnt;

`ifdef ETH_FIFO_XILINX
`else
  `ifdef ETH_ALTERA_ALTSYNCRAM
  `else
    reg     [DATA_WIDTH-1:0]  fifo  [0:DEPTH-1];
    reg     [DATA_WIDTH-1:0]  data_out;
  `endif
`endif

reg     [CNT_WIDTH-1:0]   cnt;
reg     [CNT_WIDTH-2:0]   read_pointer;
reg     [CNT_WIDTH-2:0]   write_pointer;


always @ (posedge clk or posedge reset)
begin
  if(reset)
    cnt <=#Tp 0;
  else
  if(clear)
    cnt <=#Tp { {(CNT_WIDTH-1){1'b0}}, read^write};
  else
  if(read ^ write)
    if(read)
      cnt <=#Tp cnt - 1'b1;
    else
      cnt <=#Tp cnt + 1'b1;
end

always @ (posedge clk or posedge reset)
begin
  if(reset)
    read_pointer <=#Tp 0;
  else
  if(clear)
    read_pointer <=#Tp { {(CNT_WIDTH-2){1'b0}}, read};
  else
  if(read & ~empty)
    read_pointer <=#Tp read_pointer + 1'b1;
end

always @ (posedge clk or posedge reset)
begin
  if(reset)
    write_pointer <=#Tp 0;
  else
  if(clear)
    write_pointer <=#Tp { {(CNT_WIDTH-2){1'b0}}, write};
  else
  if(write & ~full)
    write_pointer <=#Tp write_pointer + 1'b1;
end

assign empty = ~(|cnt);
assign almost_empty = cnt == 1;
assign full  = cnt == DEPTH;
assign almost_full  = &cnt[CNT_WIDTH-2:0];



`ifdef ETH_FIFO_XILINX
  xilinx_dist_ram_16x32 fifo
  ( .data_out(data_out), 
    .we(write & ~full),
    .data_in(data_in),
    .read_address( clear ? {CNT_WIDTH-1{1'b0}} : read_pointer),
    .write_address(clear ? {CNT_WIDTH-1{1'b0}} : write_pointer),
    .wclk(clk)
  );
`else   // !ETH_FIFO_XILINX
`ifdef ETH_ALTERA_ALTSYNCRAM
  altera_dpram_16x32	altera_dpram_16x32_inst
  (
  	.data             (data_in),
  	.wren             (write & ~full),
  	.wraddress        (clear ? {CNT_WIDTH-1{1'b0}} : write_pointer),
  	.rdaddress        (clear ? {CNT_WIDTH-1{1'b0}} : read_pointer ),
  	.clock            (clk),
  	.q                (data_out)
  );  //exemplar attribute altera_dpram_16x32_inst NOOPT TRUE
`else   // !ETH_ALTERA_ALTSYNCRAM
  always @ (posedge clk)
  begin
    if(write & clear)
      fifo[0] <=#Tp data_in;
    else
   if(write & ~full)
      fifo[write_pointer] <=#Tp data_in;
  end
  

  always @ (posedge clk)
  begin
    if(clear)
      data_out <=#Tp fifo[0];
    else
      data_out <=#Tp fifo[read_pointer];
  end
`endif  // !ETH_ALTERA_ALTSYNCRAM
`endif  // !ETH_FIFO_XILINX


endmodule
