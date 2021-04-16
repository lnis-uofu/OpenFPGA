//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_rxcounters.v                                            ////
////                                                              ////
////  This file is part of the Ethernet IP core project           ////
////  http://www.opencores.org/projects/ethmac/                   ////
////                                                              ////
////  Author(s):                                                  ////
////      - Igor Mohor (igorM@opencores.org)                      ////
////      - Novan Hartadi (novan@vlsi.itb.ac.id)                  ////
////      - Mahmud Galela (mgalela@vlsi.itb.ac.id)                ////
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
// $Log: eth_rxcounters.v,v $
// Revision 1.6  2005/02/21 11:00:57  igorm
// Delayed CRC fixed.
//
// Revision 1.5  2002/02/15 11:13:29  mohor
// Format of the file changed a bit.
//
// Revision 1.4  2002/02/14 20:19:41  billditt
// Modified for Address Checking,
// addition of eth_addrcheck.v
//
// Revision 1.3  2002/01/23 10:28:16  mohor
// Link in the header changed.
//
// Revision 1.2  2001/10/19 08:43:51  mohor
// eth_timescale.v changed to timescale.v This is done because of the
// simulation of the few cores in a one joined project.
//
// Revision 1.1  2001/08/06 14:44:29  mohor
// A define FPGA added to select between Artisan RAM (for ASIC) and Block Ram (For Virtex).
// Include files fixed to contain no path.
// File names and module names changed ta have a eth_ prologue in the name.
// File eth_timescale.v is used to define timescale
// All pin names on the top module are changed to contain _I, _O or _OE at the end.
// Bidirectional signal MDIO is changed to three signals (Mdc_O, Mdi_I, Mdo_O
// and Mdo_OE. The bidirectional signal must be created on the top level. This
// is done due to the ASIC tools.
//
// Revision 1.1  2001/07/30 21:23:42  mohor
// Directory structure changed. Files checked and joind together.
//
// Revision 1.1  2001/06/27 21:26:19  mohor
// Initial release of the RxEthMAC module.
//
//
//
//
//
//


`include "timescale.v"


module eth_rxcounters (MRxClk, Reset, MRxDV, StateIdle, StateSFD, StateData, StateDrop, StatePreamble, 
                       MRxDEqD, DlyCrcEn, DlyCrcCnt, Transmitting, MaxFL, r_IFG, HugEn, IFGCounterEq24, 
                       ByteCntEq0, ByteCntEq1, ByteCntEq2,ByteCntEq3,ByteCntEq4,ByteCntEq5, ByteCntEq6,
                       ByteCntEq7, ByteCntGreat2, ByteCntSmall7, ByteCntMaxFrame, ByteCntOut
                      );

parameter Tp = 1;

input         MRxClk;
input         Reset;
input         MRxDV;
input         StateSFD;
input [1:0]   StateData;
input         MRxDEqD;
input         StateIdle;
input         StateDrop;
input         DlyCrcEn;
input         StatePreamble;
input         Transmitting;
input         HugEn;
input [15:0]  MaxFL;
input         r_IFG;

output        IFGCounterEq24;           // IFG counter reaches 9600 ns (960 ns)
output [3:0]  DlyCrcCnt;                // Delayed CRC counter
output        ByteCntEq0;               // Byte counter = 0
output        ByteCntEq1;               // Byte counter = 1
output        ByteCntEq2;               // Byte counter = 2  
output        ByteCntEq3;               // Byte counter = 3  
output        ByteCntEq4;               // Byte counter = 4  
output        ByteCntEq5;               // Byte counter = 5  
output        ByteCntEq6;               // Byte counter = 6
output        ByteCntEq7;               // Byte counter = 7
output        ByteCntGreat2;            // Byte counter > 2
output        ByteCntSmall7;            // Byte counter < 7
output        ByteCntMaxFrame;          // Byte counter = MaxFL
output [15:0] ByteCntOut;               // Byte counter

wire          ResetByteCounter;
wire          IncrementByteCounter;
wire          ResetIFGCounter;
wire          IncrementIFGCounter;
wire          ByteCntMax;

reg   [15:0]  ByteCnt;
reg   [3:0]   DlyCrcCnt;
reg   [4:0]   IFGCounter;

wire  [15:0]  ByteCntDelayed;



assign ResetByteCounter = MRxDV & (StateSFD & MRxDEqD | StateData[0] & ByteCntMaxFrame);

assign IncrementByteCounter = ~ResetByteCounter & MRxDV & 
                              (StatePreamble | StateSFD | StateIdle & ~Transmitting |
                               StateData[1] & ~ByteCntMax & ~(DlyCrcEn & |DlyCrcCnt)
                              );


always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    ByteCnt[15:0] <= #Tp 16'h0;
  else
    begin
      if(ResetByteCounter)
        ByteCnt[15:0] <= #Tp 16'h0;
      else
      if(IncrementByteCounter)
        ByteCnt[15:0] <= #Tp ByteCnt[15:0] + 1'b1;
     end
end

assign ByteCntDelayed = ByteCnt + 3'h4;
assign ByteCntOut = DlyCrcEn? ByteCntDelayed : ByteCnt;

assign ByteCntEq0       = ByteCnt == 16'h0;
assign ByteCntEq1       = ByteCnt == 16'h1;
assign ByteCntEq2       = ByteCnt == 16'h2; 
assign ByteCntEq3       = ByteCnt == 16'h3; 
assign ByteCntEq4       = ByteCnt == 16'h4; 
assign ByteCntEq5       = ByteCnt == 16'h5; 
assign ByteCntEq6       = ByteCnt == 16'h6;
assign ByteCntEq7       = ByteCnt == 16'h7;
assign ByteCntGreat2    = ByteCnt >  16'h2;
assign ByteCntSmall7    = ByteCnt <  16'h7;
assign ByteCntMax       = ByteCnt == 16'hffff;
assign ByteCntMaxFrame  = ByteCnt == MaxFL[15:0] & ~HugEn;


assign ResetIFGCounter = StateSFD  &  MRxDV & MRxDEqD | StateDrop;

assign IncrementIFGCounter = ~ResetIFGCounter & (StateDrop | StateIdle | StatePreamble | StateSFD) & ~IFGCounterEq24;

always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    IFGCounter[4:0] <= #Tp 5'h0;
  else
    begin
      if(ResetIFGCounter)
        IFGCounter[4:0] <= #Tp 5'h0;
      else
      if(IncrementIFGCounter)
        IFGCounter[4:0] <= #Tp IFGCounter[4:0] + 1'b1; 
    end
end



assign IFGCounterEq24 = (IFGCounter[4:0] == 5'h18) | r_IFG; // 24*400 = 9600 ns or r_IFG is set to 1


always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    DlyCrcCnt[3:0] <= #Tp 4'h0;
  else
    begin
      if(DlyCrcCnt[3:0] == 4'h9)
        DlyCrcCnt[3:0] <= #Tp 4'h0;
      else
      if(DlyCrcEn & StateSFD)
        DlyCrcCnt[3:0] <= #Tp 4'h1;
      else
      if(DlyCrcEn & (|DlyCrcCnt[3:0]))
        DlyCrcCnt[3:0] <= #Tp DlyCrcCnt[3:0] + 1'b1;
    end
end


endmodule
