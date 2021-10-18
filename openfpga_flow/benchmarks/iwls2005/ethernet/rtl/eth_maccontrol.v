//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_maccontrol.v                                            ////
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
// $Log: eth_maccontrol.v,v $
// Revision 1.7  2003/01/22 13:49:26  tadejm
// When control packets were received, they were ignored in some cases.
//
// Revision 1.6  2002/11/22 01:57:06  mohor
// Rx Flow control fixed. CF flag added to the RX buffer descriptor. RxAbort
// synchronized.
//
// Revision 1.5  2002/11/21 00:14:39  mohor
// TxDone and TxAbort changed so they're not propagated to the wishbone
// module when control frame is transmitted.
//
// Revision 1.4  2002/11/19 17:37:32  mohor
// When control frame (PAUSE) was sent, status was written in the
// eth_wishbone module and both TXB and TXC interrupts were set. Fixed.
// Only TXC interrupt is set.
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
// Revision 1.1  2001/07/03 12:51:54  mohor
// Initial release of the MAC Control module.
//
//
//
//


`include "timescale.v"


module eth_maccontrol (MTxClk, MRxClk, TxReset, RxReset, TPauseRq, TxDataIn, TxStartFrmIn, TxUsedDataIn, 
                       TxEndFrmIn, TxDoneIn, TxAbortIn, RxData, RxValid, RxStartFrm, RxEndFrm, ReceiveEnd, 
                       ReceivedPacketGood, ReceivedLengthOK, TxFlow, RxFlow, DlyCrcEn, TxPauseTV, 
                       MAC, PadIn, PadOut, CrcEnIn, CrcEnOut, TxDataOut, TxStartFrmOut, TxEndFrmOut, 
                       TxDoneOut, TxAbortOut, TxUsedDataOut, WillSendControlFrame, TxCtrlEndFrm, 
                       ReceivedPauseFrm, ControlFrmAddressOK, SetPauseTimer, r_PassAll, RxStatusWriteLatched_sync2
                      );


parameter   Tp = 1;


input         MTxClk;                   // Transmit clock (from PHY)
input         MRxClk;                   // Receive clock (from PHY)
input         TxReset;                  // Transmit reset
input         RxReset;                  // Receive reset
input         TPauseRq;                 // Transmit control frame (from host)
input   [7:0] TxDataIn;                 // Transmit packet data byte (from host)
input         TxStartFrmIn;             // Transmit packet start frame input (from host)
input         TxUsedDataIn;             // Transmit packet used data (from TxEthMAC)
input         TxEndFrmIn;               // Transmit packet end frame input (from host)
input         TxDoneIn;                 // Transmit packet done (from TxEthMAC)
input         TxAbortIn;                // Transmit packet abort (input from TxEthMAC)
input         PadIn;                    // Padding (input from registers)
input         CrcEnIn;                  // Crc append (input from registers)
input   [7:0] RxData;                   // Receive Packet Data (from RxEthMAC)
input         RxValid;                  // Received a valid packet
input         RxStartFrm;               // Receive packet start frame (input from RxEthMAC)
input         RxEndFrm;                 // Receive packet end frame (input from RxEthMAC)
input         ReceiveEnd;               // End of receiving of the current packet (input from RxEthMAC)
input         ReceivedPacketGood;       // Received packet is good
input         ReceivedLengthOK;         // Length of the received packet is OK
input         TxFlow;                   // Tx flow control (from registers)
input         RxFlow;                   // Rx flow control (from registers)
input         DlyCrcEn;                 // Delayed CRC enabled (from registers)
input  [15:0] TxPauseTV;                // Transmit Pause Timer Value (from registers)
input  [47:0] MAC;                      // MAC address (from registers)
input         RxStatusWriteLatched_sync2;
input         r_PassAll;

output  [7:0] TxDataOut;                // Transmit Packet Data (to TxEthMAC)
output        TxStartFrmOut;            // Transmit packet start frame (output to TxEthMAC)
output        TxEndFrmOut;              // Transmit packet end frame (output to TxEthMAC)
output        TxDoneOut;                // Transmit packet done (to host)
output        TxAbortOut;               // Transmit packet aborted (to host)
output        TxUsedDataOut;            // Transmit packet used data (to host)
output        PadOut;                   // Padding (output to TxEthMAC)
output        CrcEnOut;                 // Crc append (output to TxEthMAC)
output        WillSendControlFrame;
output        TxCtrlEndFrm;
output        ReceivedPauseFrm;
output        ControlFrmAddressOK;
output        SetPauseTimer;

reg           TxUsedDataOutDetected;    
reg           TxAbortInLatched;         
reg           TxDoneInLatched;          
reg           MuxedDone;                
reg           MuxedAbort;               

wire          Pause;                    
wire          TxCtrlStartFrm;
wire    [7:0] ControlData;              
wire          CtrlMux;                  
wire          SendingCtrlFrm;           // Sending Control Frame (enables padding and CRC)
wire          BlockTxDone;


// Signal TxUsedDataOut was detected (a transfer is already in progress)
always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    TxUsedDataOutDetected <= #Tp 1'b0;
  else
  if(TxDoneIn | TxAbortIn)
    TxUsedDataOutDetected <= #Tp 1'b0;
  else
  if(TxUsedDataOut)
    TxUsedDataOutDetected <= #Tp 1'b1;
end    


// Latching variables
always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    begin
      TxAbortInLatched <= #Tp 1'b0;
      TxDoneInLatched  <= #Tp 1'b0;
    end
  else
    begin
      TxAbortInLatched <= #Tp TxAbortIn;
      TxDoneInLatched  <= #Tp TxDoneIn;
    end
end



// Generating muxed abort signal
always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    MuxedAbort <= #Tp 1'b0;
  else
  if(TxStartFrmIn)
    MuxedAbort <= #Tp 1'b0;
  else
  if(TxAbortIn & ~TxAbortInLatched & TxUsedDataOutDetected)
    MuxedAbort <= #Tp 1'b1;
end


// Generating muxed done signal
always @ (posedge MTxClk or posedge TxReset)
begin
  if(TxReset)
    MuxedDone <= #Tp 1'b0;
  else
  if(TxStartFrmIn)
    MuxedDone <= #Tp 1'b0;
  else
  if(TxDoneIn & (~TxDoneInLatched) & TxUsedDataOutDetected)
    MuxedDone <= #Tp 1'b1;
end


// TxDoneOut
assign TxDoneOut  = CtrlMux? ((~TxStartFrmIn) & (~BlockTxDone) & MuxedDone) : 
                             ((~TxStartFrmIn) & (~BlockTxDone) & TxDoneIn);

// TxAbortOut
assign TxAbortOut  = CtrlMux? ((~TxStartFrmIn) & (~BlockTxDone) & MuxedAbort) :
                              ((~TxStartFrmIn) & (~BlockTxDone) & TxAbortIn);

// TxUsedDataOut
assign TxUsedDataOut  = ~CtrlMux & TxUsedDataIn;

// TxStartFrmOut
assign TxStartFrmOut = CtrlMux? TxCtrlStartFrm : (TxStartFrmIn & ~Pause);


// TxEndFrmOut
assign TxEndFrmOut = CtrlMux? TxCtrlEndFrm : TxEndFrmIn;


// TxDataOut[7:0]
assign TxDataOut[7:0] = CtrlMux? ControlData[7:0] : TxDataIn[7:0];


// PadOut
assign PadOut = PadIn | SendingCtrlFrm;


// CrcEnOut
assign CrcEnOut = CrcEnIn | SendingCtrlFrm;



// Connecting receivecontrol module
eth_receivecontrol receivecontrol1 
(
 .MTxClk(MTxClk), .MRxClk(MRxClk), .TxReset(TxReset), .RxReset(RxReset), .RxData(RxData), 
 .RxValid(RxValid), .RxStartFrm(RxStartFrm), .RxEndFrm(RxEndFrm), .RxFlow(RxFlow), 
 .ReceiveEnd(ReceiveEnd), .MAC(MAC), .DlyCrcEn(DlyCrcEn), .TxDoneIn(TxDoneIn), 
 .TxAbortIn(TxAbortIn), .TxStartFrmOut(TxStartFrmOut), .ReceivedLengthOK(ReceivedLengthOK), 
 .ReceivedPacketGood(ReceivedPacketGood), .TxUsedDataOutDetected(TxUsedDataOutDetected), 
 .Pause(Pause), .ReceivedPauseFrm(ReceivedPauseFrm), .AddressOK(ControlFrmAddressOK), 
 .r_PassAll(r_PassAll), .RxStatusWriteLatched_sync2(RxStatusWriteLatched_sync2), .SetPauseTimer(SetPauseTimer)
);


eth_transmitcontrol transmitcontrol1
(
 .MTxClk(MTxClk), .TxReset(TxReset), .TxUsedDataIn(TxUsedDataIn), .TxUsedDataOut(TxUsedDataOut), 
 .TxDoneIn(TxDoneIn), .TxAbortIn(TxAbortIn), .TxStartFrmIn(TxStartFrmIn), .TPauseRq(TPauseRq), 
 .TxUsedDataOutDetected(TxUsedDataOutDetected), .TxFlow(TxFlow), .DlyCrcEn(DlyCrcEn), .TxPauseTV(TxPauseTV), 
 .MAC(MAC), .TxCtrlStartFrm(TxCtrlStartFrm), .TxCtrlEndFrm(TxCtrlEndFrm), .SendingCtrlFrm(SendingCtrlFrm), 
 .CtrlMux(CtrlMux), .ControlData(ControlData), .WillSendControlFrame(WillSendControlFrame), .BlockTxDone(BlockTxDone)
);



endmodule
