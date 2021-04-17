//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_txstatem.v                                              ////
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
// $Log: eth_txstatem.v,v $
// Revision 1.6  2003/01/30 13:29:08  tadejm
// Defer indication changed.
//
// Revision 1.5  2002/10/30 12:54:50  mohor
// State machine goes from idle to the defer state when CarrierSense is 1. FCS (CRC appending) fixed to check the CrcEn bit also when padding is necessery.
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
// Revision 1.3  2001/06/19 18:16:40  mohor
// TxClk changed to MTxClk (as discribed in the documentation).
// Crc changed so only one file can be used instead of two.
//
// Revision 1.2  2001/06/19 10:38:07  mohor
// Minor changes in header.
//
// Revision 1.1  2001/06/19 10:27:57  mohor
// TxEthMAC initial release.
//
//
//
//


`include "timescale.v"


module eth_txstatem  (MTxClk, Reset, ExcessiveDefer, CarrierSense, NibCnt, IPGT, IPGR1, 
                      IPGR2, FullD, TxStartFrm, TxEndFrm, TxUnderRun, Collision, UnderRun, 
                      StartTxDone, TooBig, NibCntEq7, NibCntEq15, MaxFrame, Pad, CrcEn, 
                      NibbleMinFl, RandomEq0, ColWindow, RetryMax, NoBckof, RandomEqByteCnt,
                      StateIdle, StateIPG, StatePreamble, StateData, StatePAD, StateFCS, 
                      StateJam, StateJam_q, StateBackOff, StateDefer, StartFCS, StartJam, 
                      StartBackoff, StartDefer, DeferIndication, StartPreamble, StartData, StartIPG
                     );

parameter Tp = 1;

input MTxClk;
input Reset;
input ExcessiveDefer;
input CarrierSense;
input [6:0] NibCnt;
input [6:0] IPGT;
input [6:0] IPGR1;
input [6:0] IPGR2;
input FullD;
input TxStartFrm;
input TxEndFrm;
input TxUnderRun;
input Collision;
input UnderRun;
input StartTxDone; 
input TooBig;
input NibCntEq7;
input NibCntEq15;
input MaxFrame;
input Pad;
input CrcEn;
input NibbleMinFl;
input RandomEq0;
input ColWindow;
input RetryMax;
input NoBckof;
input RandomEqByteCnt;


output StateIdle;         // Idle state
output StateIPG;          // IPG state
output StatePreamble;     // Preamble state
output [1:0] StateData;   // Data state
output StatePAD;          // PAD state
output StateFCS;          // FCS state
output StateJam;          // Jam state
output StateJam_q;        // Delayed Jam state
output StateBackOff;      // Backoff state
output StateDefer;        // Defer state

output StartFCS;          // FCS state will be activated in next clock
output StartJam;          // Jam state will be activated in next clock
output StartBackoff;      // Backoff state will be activated in next clock
output StartDefer;        // Defer state will be activated in next clock
output DeferIndication;
output StartPreamble;     // Preamble state will be activated in next clock
output [1:0] StartData;   // Data state will be activated in next clock
output StartIPG;          // IPG state will be activated in next clock

wire StartIdle;           // Idle state will be activated in next clock
wire StartPAD;            // PAD state will be activated in next clock


reg StateIdle;
reg StateIPG;
reg StatePreamble;
reg [1:0] StateData;
reg StatePAD;
reg StateFCS;
reg StateJam;
reg StateJam_q;
reg StateBackOff;
reg StateDefer;
reg Rule1;


// Defining the next state
assign StartIPG = StateDefer & ~ExcessiveDefer & ~CarrierSense;

assign StartIdle = StateIPG & (Rule1 & NibCnt[6:0] >= IPGT | ~Rule1 & NibCnt[6:0] >= IPGR2);

assign StartPreamble = StateIdle & TxStartFrm & ~CarrierSense;

assign StartData[0] = ~Collision & (StatePreamble & NibCntEq15 | StateData[1] & ~TxEndFrm);

assign StartData[1] = ~Collision & StateData[0] & ~TxUnderRun & ~MaxFrame;

assign StartPAD = ~Collision & StateData[1] & TxEndFrm & Pad & ~NibbleMinFl;

assign StartFCS = ~Collision & StateData[1] & TxEndFrm & (~Pad | Pad & NibbleMinFl) & CrcEn
                | ~Collision & StatePAD & NibbleMinFl & CrcEn;

assign StartJam = (Collision | UnderRun) & ((StatePreamble & NibCntEq15) | (|StateData[1:0]) | StatePAD | StateFCS);

assign StartBackoff = StateJam & ~RandomEq0 & ColWindow & ~RetryMax & NibCntEq7 & ~NoBckof;

assign StartDefer = StateIPG & ~Rule1 & CarrierSense & NibCnt[6:0] <= IPGR1 & NibCnt[6:0] != IPGR2
                  | StateIdle & CarrierSense 
                  | StateJam & NibCntEq7 & (NoBckof | RandomEq0 | ~ColWindow | RetryMax)
                  | StateBackOff & (TxUnderRun | RandomEqByteCnt)
                  | StartTxDone | TooBig;

assign DeferIndication = StateIdle & CarrierSense;

// Tx State Machine
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    begin
      StateIPG        <= #Tp 1'b0;
      StateIdle       <= #Tp 1'b0;
      StatePreamble   <= #Tp 1'b0;
      StateData[1:0]  <= #Tp 2'b0;
      StatePAD        <= #Tp 1'b0;
      StateFCS        <= #Tp 1'b0;
      StateJam        <= #Tp 1'b0;
      StateJam_q      <= #Tp 1'b0;
      StateBackOff    <= #Tp 1'b0;
      StateDefer      <= #Tp 1'b1;
    end
  else
    begin
      StateData[1:0] <= #Tp StartData[1:0];
      StateJam_q <= #Tp StateJam;

      if(StartDefer | StartIdle)
        StateIPG <= #Tp 1'b0;
      else
      if(StartIPG)
        StateIPG <= #Tp 1'b1;

      if(StartDefer | StartPreamble)
        StateIdle <= #Tp 1'b0;
      else
      if(StartIdle)
        StateIdle <= #Tp 1'b1;

      if(StartData[0] | StartJam)
        StatePreamble <= #Tp 1'b0;
      else
      if(StartPreamble)
        StatePreamble <= #Tp 1'b1;

      if(StartFCS | StartJam)
        StatePAD <= #Tp 1'b0;
      else
      if(StartPAD)
        StatePAD <= #Tp 1'b1;

      if(StartJam | StartDefer)
        StateFCS <= #Tp 1'b0;
      else
      if(StartFCS)
        StateFCS <= #Tp 1'b1;

      if(StartBackoff | StartDefer)
        StateJam <= #Tp 1'b0;
      else
      if(StartJam)
        StateJam <= #Tp 1'b1;

      if(StartDefer)
        StateBackOff <= #Tp 1'b0;
      else
      if(StartBackoff)
        StateBackOff <= #Tp 1'b1;

      if(StartIPG)
        StateDefer <= #Tp 1'b0;
      else
      if(StartDefer)
        StateDefer <= #Tp 1'b1;
    end
end


// This sections defines which interpack gap rule to use
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    Rule1 <= #Tp 1'b0;
  else
    begin
      if(StateIdle | StateBackOff)
        Rule1 <= #Tp 1'b0;
      else
      if(StatePreamble | FullD)
        Rule1 <= #Tp 1'b1;
    end
end



endmodule
