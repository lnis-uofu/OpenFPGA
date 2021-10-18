//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_transmitcontrol.v                                       ////
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
// $Log: eth_transmitcontrol.v,v $
// Revision 1.6  2002/11/21 00:16:14  mohor
// When TxUsedData and CtrlMux occur at the same time, byte counter needs
// to be incremented by 2. Signal IncrementByteCntBy2 added for that reason.
//
// Revision 1.5  2002/11/19 17:37:32  mohor
// When control frame (PAUSE) was sent, status was written in the
// eth_wishbone module and both TXB and TXC interrupts were set. Fixed.
// Only TXC interrupt is set.
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
// Revision 1.1  2001/07/03 12:51:54  mohor
// Initial release of the MAC Control module.
//
//
//
//
//
//


`include "timescale.v"


module eth_transmitcontrol (MTxClk, TxReset, TxUsedDataIn, TxUsedDataOut, TxDoneIn, TxAbortIn, 
                            TxStartFrmIn, TPauseRq, TxUsedDataOutDetected, TxFlow, DlyCrcEn, 
                            TxPauseTV, MAC, TxCtrlStartFrm, TxCtrlEndFrm, SendingCtrlFrm, CtrlMux, 
                            ControlData, WillSendControlFrame, BlockTxDone
                           );

parameter Tp = 1;


input         MTxClk;
input         TxReset;
input         TxUsedDataIn;
input         TxUsedDataOut;
input         TxDoneIn;
input         TxAbortIn;
input         TxStartFrmIn;
input         TPauseRq;
input         TxUsedDataOutDetected;
input         TxFlow;
input         DlyCrcEn;
input  [15:0] TxPauseTV;
input  [47:0] MAC;

output        TxCtrlStartFrm;
output        TxCtrlEndFrm;
output        SendingCtrlFrm;
output        CtrlMux;
output [7:0]  ControlData;
output        WillSendControlFrame;
output        BlockTxDone;

reg           SendingCtrlFrm;
reg           CtrlMux;
reg           WillSendControlFrame;
reg    [3:0]  DlyCrcCnt;
reg    [5:0]  ByteCnt;
reg           ControlEnd_q;
reg    [7:0]  MuxedCtrlData;
reg           TxCtrlStartFrm;
reg           TxCtrlStartFrm_q;
reg           TxCtrlEndFrm;
reg    [7:0]  ControlData;
reg           TxUsedDataIn_q;
reg           BlockTxDone;

wire          IncrementDlyCrcCnt;
wire          ResetByteCnt;
wire          IncrementByteCnt;
wire          ControlEnd;
wire          IncrementByteCntBy2;
wire          EnableCnt;


// A command for Sending the control frame is active (latched)
always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    WillSendControlFrame <= #Tp 1'b0;
  else
  if(TxCtrlEndFrm & CtrlMux)
    WillSendControlFrame <= #Tp 1'b0;
  else
  if(TPauseRq & TxFlow)
    WillSendControlFrame <= #Tp 1'b1;
end


// Generation of the transmit control packet start frame
always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    TxCtrlStartFrm <= #Tp 1'b0;
  else
  if(TxUsedDataIn_q & CtrlMux)
    TxCtrlStartFrm <= #Tp 1'b0;
  else
  if(WillSendControlFrame & ~TxUsedDataOut & (TxDoneIn | TxAbortIn | TxStartFrmIn | (~TxUsedDataOutDetected)))
    TxCtrlStartFrm <= #Tp 1'b1;
end



// Generation of the transmit control packet end frame
always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    TxCtrlEndFrm <= #Tp 1'b0;
  else
  if(ControlEnd | ControlEnd_q)
    TxCtrlEndFrm <= #Tp 1'b1;
  else
    TxCtrlEndFrm <= #Tp 1'b0;
end


// Generation of the multiplexer signal (controls muxes for switching between
// normal and control packets)
always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    CtrlMux <= #Tp 1'b0;
  else
  if(WillSendControlFrame & ~TxUsedDataOut)
    CtrlMux <= #Tp 1'b1;
  else
  if(TxDoneIn)
    CtrlMux <= #Tp 1'b0;
end



// Generation of the Sending Control Frame signal (enables padding and CRC)
always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    SendingCtrlFrm <= #Tp 1'b0;
  else
  if(WillSendControlFrame & TxCtrlStartFrm)
    SendingCtrlFrm <= #Tp 1'b1;
  else
  if(TxDoneIn)
    SendingCtrlFrm <= #Tp 1'b0;
end


always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    TxUsedDataIn_q <= #Tp 1'b0;
  else
    TxUsedDataIn_q <= #Tp TxUsedDataIn;
end



// Generation of the signal that will block sending the Done signal to the eth_wishbone module
// While sending the control frame
always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    BlockTxDone <= #Tp 1'b0;
  else
  if(TxCtrlStartFrm)
    BlockTxDone <= #Tp 1'b1;
  else
  if(TxStartFrmIn)
    BlockTxDone <= #Tp 1'b0;
end


always @ (posedge MTxClk)
begin
  ControlEnd_q     <= #Tp ControlEnd;
  TxCtrlStartFrm_q <= #Tp TxCtrlStartFrm;
end


assign IncrementDlyCrcCnt = CtrlMux & TxUsedDataIn &  ~DlyCrcCnt[2];


// Delayed CRC counter
always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    DlyCrcCnt <= #Tp 4'h0;
  else
  if(ResetByteCnt)
    DlyCrcCnt <= #Tp 4'h0;
  else
  if(IncrementDlyCrcCnt)
    DlyCrcCnt <= #Tp DlyCrcCnt + 1'b1;
end

             
assign ResetByteCnt = TxReset | (~TxCtrlStartFrm & (TxDoneIn | TxAbortIn));
assign IncrementByteCnt = CtrlMux & (TxCtrlStartFrm & ~TxCtrlStartFrm_q & ~TxUsedDataIn | TxUsedDataIn & ~ControlEnd);
assign IncrementByteCntBy2 = CtrlMux & TxCtrlStartFrm & (~TxCtrlStartFrm_q) & TxUsedDataIn;     // When TxUsedDataIn and CtrlMux are set at the same time

assign EnableCnt = (~DlyCrcEn | DlyCrcEn & (&DlyCrcCnt[1:0]));
// Byte counter
always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    ByteCnt <= #Tp 6'h0;
  else
  if(ResetByteCnt)
    ByteCnt <= #Tp 6'h0;
  else
  if(IncrementByteCntBy2 & EnableCnt)
    ByteCnt <= #Tp (ByteCnt[5:0] ) + 2'h2;
  else
  if(IncrementByteCnt & EnableCnt)
    ByteCnt <= #Tp (ByteCnt[5:0] ) + 1'b1;
end


assign ControlEnd = ByteCnt[5:0] == 6'h22;


// Control data generation (goes to the TxEthMAC module)
always @ (ByteCnt or DlyCrcEn or MAC or TxPauseTV or DlyCrcCnt)
begin
  case(ByteCnt)
    6'h0:    if(~DlyCrcEn | DlyCrcEn & (&DlyCrcCnt[1:0]))
               MuxedCtrlData[7:0] = 8'h01;                   // Reserved Multicast Address
             else
						 	 MuxedCtrlData[7:0] = 8'h0;
    6'h2:      MuxedCtrlData[7:0] = 8'h80;
    6'h4:      MuxedCtrlData[7:0] = 8'hC2;
    6'h6:      MuxedCtrlData[7:0] = 8'h00;
    6'h8:      MuxedCtrlData[7:0] = 8'h00;
    6'hA:      MuxedCtrlData[7:0] = 8'h01;
    6'hC:      MuxedCtrlData[7:0] = MAC[47:40];
    6'hE:      MuxedCtrlData[7:0] = MAC[39:32];
    6'h10:     MuxedCtrlData[7:0] = MAC[31:24];
    6'h12:     MuxedCtrlData[7:0] = MAC[23:16];
    6'h14:     MuxedCtrlData[7:0] = MAC[15:8];
    6'h16:     MuxedCtrlData[7:0] = MAC[7:0];
    6'h18:     MuxedCtrlData[7:0] = 8'h88;                   // Type/Length
    6'h1A:     MuxedCtrlData[7:0] = 8'h08;
    6'h1C:     MuxedCtrlData[7:0] = 8'h00;                   // Opcode
    6'h1E:     MuxedCtrlData[7:0] = 8'h01;
    6'h20:     MuxedCtrlData[7:0] = TxPauseTV[15:8];         // Pause timer value
    6'h22:     MuxedCtrlData[7:0] = TxPauseTV[7:0];
    default:   MuxedCtrlData[7:0] = 8'h0;
  endcase
end


// Latched Control data
always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    ControlData[7:0] <= #Tp 8'h0;
  else
  if(~ByteCnt[0])
    ControlData[7:0] <= #Tp MuxedCtrlData[7:0];
end



endmodule
