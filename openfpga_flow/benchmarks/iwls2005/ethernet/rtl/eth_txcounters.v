//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_txcounters.v                                            ////
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
// $Log: eth_txcounters.v,v $
// Revision 1.6  2005/02/21 11:25:27  igorm
// Delayed CRC fixed.
//
// Revision 1.5  2002/04/22 14:54:14  mohor
// FCS should not be included in NibbleMinFl.
//
// Revision 1.4  2002/01/23 10:28:16  mohor
// Link in the header changed.
//
// Revision 1.3  2001/10/19 08:43:51  mohor
// eth_timescale.v changed to timescale.v This is done because of the
// simulation of the few cores in a one joined project.
//
// Revision 1.2  2001/09/11 14:17:00  mohor
// Few little NCSIM warnings fixed.
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
// Revision 1.4  2001/06/27 21:27:45  mohor
// Few typos fixed.
//
// Revision 1.2  2001/06/19 10:38:07  mohor
// Minor changes in header.
//
// Revision 1.1  2001/06/19 10:27:57  mohor
// TxEthMAC initial release.
//
//
//


`include "timescale.v"


module eth_txcounters (StatePreamble, StateIPG, StateData, StatePAD, StateFCS, StateJam, 
                       StateBackOff, StateDefer, StateIdle, StartDefer, StartIPG, StartFCS, 
                       StartJam, StartBackoff, TxStartFrm, MTxClk, Reset, MinFL, MaxFL, HugEn, 
                       ExDfrEn, PacketFinished_q, DlyCrcEn, StateSFD, ByteCnt, NibCnt, 
                       ExcessiveDefer, NibCntEq7, NibCntEq15, MaxFrame, NibbleMinFl, DlyCrcCnt
                      );

parameter Tp = 1;

input MTxClk;             // Tx clock
input Reset;              // Reset
input StatePreamble;      // Preamble state
input StateIPG;           // IPG state
input [1:0] StateData;    // Data state
input StatePAD;           // PAD state
input StateFCS;           // FCS state
input StateJam;           // Jam state
input StateBackOff;       // Backoff state
input StateDefer;         // Defer state
input StateIdle;          // Idle state
input StateSFD;           // SFD state
input StartDefer;         // Defer state will be activated in next clock
input StartIPG;           // IPG state will be activated in next clock
input StartFCS;           // FCS state will be activated in next clock
input StartJam;           // Jam state will be activated in next clock
input StartBackoff;       // Backoff state will be activated in next clock
input TxStartFrm;         // Tx start frame
input [15:0] MinFL;       // Minimum frame length (in bytes)
input [15:0] MaxFL;       // Miximum frame length (in bytes)
input HugEn;              // Pakets bigger then MaxFL enabled
input ExDfrEn;            // Excessive deferral enabled
input PacketFinished_q;             
input DlyCrcEn;           // Delayed CRC enabled

output [15:0] ByteCnt;    // Byte counter
output [15:0] NibCnt;     // Nibble counter
output ExcessiveDefer;    // Excessive Deferral occuring
output NibCntEq7;         // Nibble counter is equal to 7
output NibCntEq15;        // Nibble counter is equal to 15
output MaxFrame;          // Maximum frame occured
output NibbleMinFl;       // Nibble counter is greater than the minimum frame length
output [2:0] DlyCrcCnt;   // Delayed CRC Count

wire ExcessiveDeferCnt;
wire ResetNibCnt;
wire IncrementNibCnt;
wire ResetByteCnt;
wire IncrementByteCnt;
wire ByteCntMax;

reg [15:0] NibCnt;
reg [15:0] ByteCnt;
reg  [2:0] DlyCrcCnt;



assign IncrementNibCnt = StateIPG | StatePreamble | (|StateData) | StatePAD 
                       | StateFCS | StateJam | StateBackOff | StateDefer & ~ExcessiveDefer & TxStartFrm;


assign ResetNibCnt = StateDefer & ExcessiveDefer & ~TxStartFrm | StatePreamble & NibCntEq15 
                   | StateJam & NibCntEq7 | StateIdle | StartDefer | StartIPG | StartFCS | StartJam;

// Nibble Counter
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    NibCnt <= #Tp 16'h0;
  else
    begin
      if(ResetNibCnt)
        NibCnt <= #Tp 16'h0;
      else
      if(IncrementNibCnt)
        NibCnt <= #Tp NibCnt + 1'b1;
     end
end


assign NibCntEq7   = &NibCnt[2:0];
assign NibCntEq15  = &NibCnt[3:0];

assign NibbleMinFl = NibCnt >= (((MinFL-3'h4)<<1) -1);  // FCS should not be included in NibbleMinFl

assign ExcessiveDeferCnt = NibCnt[13:0] == 16'h17b7;

assign ExcessiveDefer  = NibCnt[13:0] == 16'h17b7 & ~ExDfrEn;   // 6071 nibbles

assign IncrementByteCnt = StateData[1] & ~ByteCntMax
                        | StateBackOff & (&NibCnt[6:0])
                        | (StatePAD | StateFCS) & NibCnt[0] & ~ByteCntMax;

assign ResetByteCnt = StartBackoff | StateIdle & TxStartFrm | PacketFinished_q;


// Transmit Byte Counter
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    ByteCnt[15:0] <= #Tp 16'h0;
  else
    begin
      if(ResetByteCnt)
        ByteCnt[15:0] <= #Tp 16'h0;
      else
      if(IncrementByteCnt)
        ByteCnt[15:0] <= #Tp ByteCnt[15:0] + 1'b1;
    end
end


assign MaxFrame = ByteCnt[15:0] == MaxFL[15:0] & ~HugEn;

assign ByteCntMax = &ByteCnt[15:0];


// Delayed CRC counter
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    DlyCrcCnt <= #Tp 3'h0;
  else
    begin        
      if(StateData[1] & DlyCrcCnt == 3'h4 | StartJam | PacketFinished_q)
        DlyCrcCnt <= #Tp 3'h0;
      else
      if(DlyCrcEn & (StateSFD | StateData[1] & (|DlyCrcCnt[2:0])))
        DlyCrcCnt <= #Tp DlyCrcCnt + 1'b1;
    end
end



endmodule
