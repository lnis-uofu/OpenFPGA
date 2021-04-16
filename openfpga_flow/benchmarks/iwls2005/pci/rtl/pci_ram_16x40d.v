//////////////////////////////////////////////////////////////////////
////                                                              ////
////  Xilinx architecture distributed RAM instantiation           ////
////                                                              ////
////  This file is part of pci bridge project                     ////
////  http://www.opencores.org/cvsweb.shtml/pci/                  ////
////                                                              ////
////  Description                                                 ////
////  Module instantiates 40 16x1D RAMs, to form 40 bit wide,     ////
////  16 locations synchronous RAM to use in PCI Fifo instances   ////
////                                                              ////
////  Author(s):                                                  ////
////      - Miha Dolenc, mihad@opencores.org                      ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000 Authors and OPENCORES.ORG                 ////
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
// $Log: pci_ram_16x40d.v,v $
// Revision 1.1  2002/08/19 16:49:36  mihad
// Extracted distributed RAM module from wb/pci_tpram.v to its own file
//
//
module pci_ram_16x40d (data_out, we, data_in, read_address, write_address, wclk);
    parameter addr_width = 4 ;
    output [39:0] data_out;
    input we, wclk;
    input [39:0] data_in;
    input [addr_width - 1:0] write_address, read_address;

    wire [3:0] waddr = write_address ;
    wire [3:0] raddr = read_address ;

    RAM16X1D ram00 (.DPO(data_out[0]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[0]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram01 (.DPO(data_out[1]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[1]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram02 (.DPO(data_out[2]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[2]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram03 (.DPO(data_out[3]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[3]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram04 (.DPO(data_out[4]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[4]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram05 (.DPO(data_out[5]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[5]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram06 (.DPO(data_out[6]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[6]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram07 (.DPO(data_out[7]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[7]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram08 (.DPO(data_out[8]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[8]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram09 (.DPO(data_out[9]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[9]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram10 (.DPO(data_out[10]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[10]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram11 (.DPO(data_out[11]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[11]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram12 (.DPO(data_out[12]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[12]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram13 (.DPO(data_out[13]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[13]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram14 (.DPO(data_out[14]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[14]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram15 (.DPO(data_out[15]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[15]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram16 (.DPO(data_out[16]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[16]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram17 (.DPO(data_out[17]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[17]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram18 (.DPO(data_out[18]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[18]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram19 (.DPO(data_out[19]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[19]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram20 (.DPO(data_out[20]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[20]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram21 (.DPO(data_out[21]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[21]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram22 (.DPO(data_out[22]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[22]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram23 (.DPO(data_out[23]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[23]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram24 (.DPO(data_out[24]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[24]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram25 (.DPO(data_out[25]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[25]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram26 (.DPO(data_out[26]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[26]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram27 (.DPO(data_out[27]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[27]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram28 (.DPO(data_out[28]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[28]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram29 (.DPO(data_out[29]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[29]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram30 (.DPO(data_out[30]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[30]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram31 (.DPO(data_out[31]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[31]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram32 (.DPO(data_out[32]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[32]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram33 (.DPO(data_out[33]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[33]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram34 (.DPO(data_out[34]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[34]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram35 (.DPO(data_out[35]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[35]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram36 (.DPO(data_out[36]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[36]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram37 (.DPO(data_out[37]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[37]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram38 (.DPO(data_out[38]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[38]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram39 (.DPO(data_out[39]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[39]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
endmodule
