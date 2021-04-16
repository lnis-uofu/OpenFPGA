//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_txethmac.v                                              ////
///                                                              ////
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
// $Log: eth_txethmac.v,v $
// Revision 1.9  2005/02/21 11:25:28  igorm
// Delayed CRC fixed.
//
// Revision 1.8  2003/01/30 13:33:24  mohor
// When padding was enabled and crc disabled, frame was not ended correctly.
//
// Revision 1.7  2002/02/26 16:24:01  mohor
// RetryCntLatched was unused and removed from design
//
// Revision 1.6  2002/02/22 12:56:35  mohor
// Retry is not activated when a Tx Underrun occured
//
// Revision 1.5  2002/02/11 09:18:22  mohor
// Tx status is written back to the BD.
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
// Revision 1.2  2001/06/19 10:38:08  mohor
// Minor changes in header.
//
// Revision 1.1  2001/06/19 10:27:58  mohor
// TxEthMAC initial release.
//
//
//

`include "timescale.v"


module eth_txethmac (MTxClk, Reset, TxStartFrm, TxEndFrm, TxUnderRun, TxData, CarrierSense, 
                     Collision, Pad, CrcEn, FullD, HugEn, DlyCrcEn, MinFL, MaxFL, IPGT, 
                     IPGR1, IPGR2, CollValid, MaxRet, NoBckof, ExDfrEn, 
                     MTxD, MTxEn, MTxErr, TxDone, TxRetry, TxAbort, TxUsedData, WillTransmit, 
                     ResetCollision, RetryCnt, StartTxDone, StartTxAbort, MaxCollisionOccured,
                     LateCollision, DeferIndication, StatePreamble, StateData

                    );

parameter Tp = 1;


input MTxClk;                   // Transmit clock (from PHY)
input Reset;                    // Reset
input TxStartFrm;               // Transmit packet start frame
input TxEndFrm;                 // Transmit packet end frame
input TxUnderRun;               // Transmit packet under-run
input [7:0] TxData;             // Transmit packet data byte
input CarrierSense;             // Carrier sense (synchronized)
input Collision;                // Collision (synchronized)
input Pad;                      // Pad enable (from register)
input CrcEn;                    // Crc enable (from register)
input FullD;                    // Full duplex (from register)
input HugEn;                    // Huge packets enable (from register)
input DlyCrcEn;                 // Delayed Crc enabled (from register)
input [15:0] MinFL;             // Minimum frame length (from register)
input [15:0] MaxFL;             // Maximum frame length (from register)
input [6:0] IPGT;               // Back to back transmit inter packet gap parameter (from register)
input [6:0] IPGR1;              // Non back to back transmit inter packet gap parameter IPGR1 (from register)
input [6:0] IPGR2;              // Non back to back transmit inter packet gap parameter IPGR2 (from register)
input [5:0] CollValid;          // Valid collision window (from register)
input [3:0] MaxRet;             // Maximum retry number (from register)
input NoBckof;                  // No backoff (from register)
input ExDfrEn;                  // Excessive defferal enable (from register)

output [3:0] MTxD;              // Transmit nibble (to PHY)
output MTxEn;                   // Transmit enable (to PHY)
output MTxErr;                  // Transmit error (to PHY)
output TxDone;                  // Transmit packet done (to RISC)
output TxRetry;                 // Transmit packet retry (to RISC)
output TxAbort;                 // Transmit packet abort (to RISC)
output TxUsedData;              // Transmit packet used data (to RISC)
output WillTransmit;            // Will transmit (to RxEthMAC)
output ResetCollision;          // Reset Collision (for synchronizing collision)
output [3:0] RetryCnt;          // Latched Retry Counter for tx status purposes
output StartTxDone;
output StartTxAbort;
output MaxCollisionOccured;
output LateCollision;
output DeferIndication;
output StatePreamble;
output [1:0] StateData;

reg [3:0] MTxD;
reg MTxEn;
reg MTxErr;
reg TxDone;
reg TxRetry;
reg TxAbort;
reg TxUsedData;
reg WillTransmit;
reg ColWindow;
reg StopExcessiveDeferOccured;
reg [3:0] RetryCnt;
reg [3:0] MTxD_d;
reg StatusLatch;
reg PacketFinished_q;
reg PacketFinished;


wire ExcessiveDeferOccured;
wire StartIPG;
wire StartPreamble;
wire [1:0] StartData;
wire StartFCS;
wire StartJam;
wire StartDefer;
wire StartBackoff;
wire StateDefer;
wire StateIPG;
wire StateIdle;
wire StatePAD;
wire StateFCS;
wire StateJam;
wire StateJam_q;
wire StateBackOff;
wire StateSFD;
wire StartTxRetry;
wire UnderRun;
wire TooBig;
wire [31:0] Crc;
wire CrcError;
wire [2:0] DlyCrcCnt;
wire [15:0] NibCnt;
wire NibCntEq7;
wire NibCntEq15;
wire NibbleMinFl;
wire ExcessiveDefer;
wire [15:0] ByteCnt;
wire MaxFrame;
wire RetryMax;
wire RandomEq0;
wire RandomEqByteCnt;
wire PacketFinished_d;



assign ResetCollision = ~(StatePreamble | (|StateData) | StatePAD | StateFCS);

assign ExcessiveDeferOccured = TxStartFrm & StateDefer & ExcessiveDefer & ~StopExcessiveDeferOccured;

assign StartTxDone = ~Collision & (StateFCS & NibCntEq7 | StateData[1] & TxEndFrm & (~Pad | Pad & NibbleMinFl) & ~CrcEn);

assign UnderRun = StateData[0] & TxUnderRun & ~Collision;

assign TooBig = ~Collision & MaxFrame & (StateData[0] & ~TxUnderRun | StateFCS);

// assign StartTxRetry = StartJam & (ColWindow & ~RetryMax);
assign StartTxRetry = StartJam & (ColWindow & ~RetryMax) & ~UnderRun;

assign LateCollision = StartJam & ~ColWindow & ~UnderRun;

assign MaxCollisionOccured = StartJam & ColWindow & RetryMax;

assign StateSFD = StatePreamble & NibCntEq15;

assign StartTxAbort = TooBig | UnderRun | ExcessiveDeferOccured | LateCollision | MaxCollisionOccured;


// StopExcessiveDeferOccured
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    StopExcessiveDeferOccured <= #Tp 1'b0;
  else
    begin
      if(~TxStartFrm)
        StopExcessiveDeferOccured <= #Tp 1'b0;
      else
      if(ExcessiveDeferOccured)
        StopExcessiveDeferOccured <= #Tp 1'b1;
    end
end


// Collision Window
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    ColWindow <= #Tp 1'b1;
  else
    begin  
      if(~Collision & ByteCnt[5:0] == CollValid[5:0] & (StateData[1] | StatePAD & NibCnt[0] | StateFCS & NibCnt[0]))
        ColWindow <= #Tp 1'b0;
      else
      if(StateIdle | StateIPG)
        ColWindow <= #Tp 1'b1;
    end
end


// Start Window
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    StatusLatch <= #Tp 1'b0;
  else
    begin
      if(~TxStartFrm)
        StatusLatch <= #Tp 1'b0;
      else
      if(ExcessiveDeferOccured | StateIdle)
        StatusLatch <= #Tp 1'b1;
     end
end


// Transmit packet used data
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    TxUsedData <= #Tp 1'b0;
  else
    TxUsedData <= #Tp |StartData;
end


// Transmit packet done
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    TxDone <= #Tp 1'b0;
  else
    begin
      if(TxStartFrm & ~StatusLatch)
        TxDone <= #Tp 1'b0;
      else
      if(StartTxDone)
        TxDone <= #Tp 1'b1;
    end
end


// Transmit packet retry
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    TxRetry <= #Tp 1'b0;
  else
    begin
      if(TxStartFrm & ~StatusLatch)
        TxRetry <= #Tp 1'b0;
      else
      if(StartTxRetry)
        TxRetry <= #Tp 1'b1;
     end
end                                    


// Transmit packet abort
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    TxAbort <= #Tp 1'b0;
  else
    begin
      if(TxStartFrm & ~StatusLatch & ~ExcessiveDeferOccured)
        TxAbort <= #Tp 1'b0;
      else
      if(StartTxAbort)
        TxAbort <= #Tp 1'b1;
    end
end


// Retry counter
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    RetryCnt[3:0] <= #Tp 4'h0;
  else
    begin
      if(ExcessiveDeferOccured | UnderRun | TooBig | StartTxDone | TxUnderRun 
          | StateJam & NibCntEq7 & (~ColWindow | RetryMax))
        RetryCnt[3:0] <= #Tp 4'h0;
      else
      if(StateJam & NibCntEq7 & ColWindow & (RandomEq0 | NoBckof) | StateBackOff & RandomEqByteCnt)
        RetryCnt[3:0] <= #Tp RetryCnt[3:0] + 1'b1;
    end
end


assign RetryMax = RetryCnt[3:0] == MaxRet[3:0];


// Transmit nibble
always @ (StatePreamble or StateData or StateData or StateFCS or StateJam or StateSFD or TxData or 
          Crc or NibCntEq15)
begin
  if(StateData[0])
    MTxD_d[3:0] = TxData[3:0];                                  // Lower nibble
  else
  if(StateData[1])
    MTxD_d[3:0] = TxData[7:4];                                  // Higher nibble
  else
  if(StateFCS)
    MTxD_d[3:0] = {~Crc[28], ~Crc[29], ~Crc[30], ~Crc[31]};     // Crc
  else
  if(StateJam)
    MTxD_d[3:0] = 4'h9;                                         // Jam pattern
  else
  if(StatePreamble)
    if(NibCntEq15)
      MTxD_d[3:0] = 4'hd;                                       // SFD
    else
      MTxD_d[3:0] = 4'h5;                                       // Preamble
  else
    MTxD_d[3:0] = 4'h0;
end


// Transmit Enable
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    MTxEn <= #Tp 1'b0;
  else
    MTxEn <= #Tp StatePreamble | (|StateData) | StatePAD | StateFCS | StateJam;
end


// Transmit nibble
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    MTxD[3:0] <= #Tp 4'h0;
  else
    MTxD[3:0] <= #Tp MTxD_d[3:0];
end


// Transmit error
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    MTxErr <= #Tp 1'b0;
  else
    MTxErr <= #Tp TooBig | UnderRun;
end


// WillTransmit
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    WillTransmit <= #Tp  1'b0;
  else
    WillTransmit <= #Tp StartPreamble | StatePreamble | (|StateData) | StatePAD | StateFCS | StateJam;
end


assign PacketFinished_d = StartTxDone | TooBig | UnderRun | LateCollision | MaxCollisionOccured | ExcessiveDeferOccured;


// Packet finished
always @ (posedge MTxClk or posedge Reset)
begin
  if(Reset)
    begin
      PacketFinished <= #Tp 1'b0;
      PacketFinished_q  <= #Tp 1'b0;
    end
  else
    begin
      PacketFinished <= #Tp PacketFinished_d;
      PacketFinished_q  <= #Tp PacketFinished;
    end
end


// Connecting module Counters
eth_txcounters txcounters1 (.StatePreamble(StatePreamble), .StateIPG(StateIPG), .StateData(StateData), 
                            .StatePAD(StatePAD), .StateFCS(StateFCS), .StateJam(StateJam), .StateBackOff(StateBackOff), 
                            .StateDefer(StateDefer), .StateIdle(StateIdle), .StartDefer(StartDefer), .StartIPG(StartIPG), 
                            .StartFCS(StartFCS), .StartJam(StartJam), .TxStartFrm(TxStartFrm), .MTxClk(MTxClk), 
                            .Reset(Reset), .MinFL(MinFL), .MaxFL(MaxFL), .HugEn(HugEn), .ExDfrEn(ExDfrEn), 
                            .PacketFinished_q(PacketFinished_q), .DlyCrcEn(DlyCrcEn), .StartBackoff(StartBackoff), 
                            .StateSFD(StateSFD), .ByteCnt(ByteCnt), .NibCnt(NibCnt), .ExcessiveDefer(ExcessiveDefer), 
                            .NibCntEq7(NibCntEq7), .NibCntEq15(NibCntEq15), .MaxFrame(MaxFrame), .NibbleMinFl(NibbleMinFl), 
                            .DlyCrcCnt(DlyCrcCnt)
                           );


// Connecting module StateM
eth_txstatem txstatem1 (.MTxClk(MTxClk), .Reset(Reset), .ExcessiveDefer(ExcessiveDefer), .CarrierSense(CarrierSense), 
                        .NibCnt(NibCnt[6:0]), .IPGT(IPGT), .IPGR1(IPGR1), .IPGR2(IPGR2), .FullD(FullD), 
                        .TxStartFrm(TxStartFrm), .TxEndFrm(TxEndFrm), .TxUnderRun(TxUnderRun), .Collision(Collision), 
                        .UnderRun(UnderRun), .StartTxDone(StartTxDone), .TooBig(TooBig), .NibCntEq7(NibCntEq7), 
                        .NibCntEq15(NibCntEq15), .MaxFrame(MaxFrame), .Pad(Pad), .CrcEn(CrcEn), 
                        .NibbleMinFl(NibbleMinFl), .RandomEq0(RandomEq0), .ColWindow(ColWindow), .RetryMax(RetryMax), 
                        .NoBckof(NoBckof), .RandomEqByteCnt(RandomEqByteCnt), .StateIdle(StateIdle), 
                        .StateIPG(StateIPG), .StatePreamble(StatePreamble), .StateData(StateData), .StatePAD(StatePAD), 
                        .StateFCS(StateFCS), .StateJam(StateJam), .StateJam_q(StateJam_q), .StateBackOff(StateBackOff), 
                        .StateDefer(StateDefer), .StartFCS(StartFCS), .StartJam(StartJam), .StartBackoff(StartBackoff), 
                        .StartDefer(StartDefer), .DeferIndication(DeferIndication), .StartPreamble(StartPreamble), .StartData(StartData), .StartIPG(StartIPG)
                       );


wire Enable_Crc;
wire [3:0] Data_Crc;
wire Initialize_Crc;

assign Enable_Crc = ~StateFCS;

assign Data_Crc[0] = StateData[0]? TxData[3] : StateData[1]? TxData[7] : 1'b0;
assign Data_Crc[1] = StateData[0]? TxData[2] : StateData[1]? TxData[6] : 1'b0;
assign Data_Crc[2] = StateData[0]? TxData[1] : StateData[1]? TxData[5] : 1'b0;
assign Data_Crc[3] = StateData[0]? TxData[0] : StateData[1]? TxData[4] : 1'b0;

assign Initialize_Crc = StateIdle | StatePreamble | (|DlyCrcCnt);


// Connecting module Crc
eth_crc txcrc (.Clk(MTxClk), .Reset(Reset), .Data(Data_Crc), .Enable(Enable_Crc), .Initialize(Initialize_Crc), 
               .Crc(Crc), .CrcError(CrcError)
              );


// Connecting module Random
eth_random random1 (.MTxClk(MTxClk), .Reset(Reset), .StateJam(StateJam), .StateJam_q(StateJam_q), .RetryCnt(RetryCnt), 
                    .NibCnt(NibCnt), .ByteCnt(ByteCnt[9:0]), .RandomEq0(RandomEq0), .RandomEqByteCnt(RandomEqByteCnt));




endmodule
