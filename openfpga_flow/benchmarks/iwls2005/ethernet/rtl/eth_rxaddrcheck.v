//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_rxaddrcheck.v                                           ////
////                                                              ////
////  This file is part of the Ethernet IP core project           ////
////  http://www.opencores.org/cores/ethmac/                      ////
////                                                              ////
////  Author(s):                                                  ////
////      - Bill Dittenhofer (billditt@aol.com)                   ////
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
// $Log: eth_rxaddrcheck.v,v $
// Revision 1.9  2002/11/22 01:57:06  mohor
// Rx Flow control fixed. CF flag added to the RX buffer descriptor. RxAbort
// synchronized.
//
// Revision 1.8  2002/11/19 17:34:52  mohor
// AddressMiss status is connecting to the Rx BD. AddressMiss is identifying
// that a frame was received because of the promiscous mode.
//
// Revision 1.7  2002/09/04 18:41:06  mohor
// Bug when last byte of destination address was not checked fixed.
//
// Revision 1.6  2002/03/20 15:14:11  mohor
// When in promiscous mode some frames were not received correctly. Fixed.
//
// Revision 1.5  2002/03/02 21:06:32  mohor
// Log info was missing.
//
//
// Revision 1.1  2002/02/08 12:51:54  ditt
// Initial release of the ethernet addresscheck module.
//
//
//
//
//


`include "timescale.v"


module eth_rxaddrcheck(MRxClk,  Reset, RxData, Broadcast ,r_Bro ,r_Pro,
                       ByteCntEq2, ByteCntEq3, ByteCntEq4, ByteCntEq5,
                       ByteCntEq6, ByteCntEq7, HASH0, HASH1, 
                       CrcHash,    CrcHashGood, StateData, RxEndFrm,
                       Multicast, MAC, RxAbort, AddressMiss, PassAll,
                       ControlFrmAddressOK
                      );

parameter Tp = 1;

  input        MRxClk; 
  input        Reset; 
  input [7:0]  RxData; 
  input        Broadcast; 
  input        r_Bro; 
  input        r_Pro; 
  input        ByteCntEq2;
  input        ByteCntEq3;
  input        ByteCntEq4;
  input        ByteCntEq5;
  input        ByteCntEq6;
  input        ByteCntEq7;
  input [31:0] HASH0; 
  input [31:0] HASH1; 
  input [5:0]  CrcHash; 
  input        CrcHashGood; 
  input        Multicast; 
  input [47:0] MAC;
  input [1:0]  StateData;
  input        RxEndFrm;
  input        PassAll;
  input        ControlFrmAddressOK;
  
  output       RxAbort;
  output       AddressMiss;

 wire BroadcastOK;
 wire ByteCntEq2;
 wire ByteCntEq3;
 wire ByteCntEq4; 
 wire ByteCntEq5;
 wire RxAddressInvalid;
 wire RxCheckEn;
 wire HashBit;
 wire [31:0] IntHash;
 reg [7:0]  ByteHash;
 reg MulticastOK;
 reg UnicastOK;
 reg RxAbort;
 reg AddressMiss;
 
assign RxAddressInvalid = ~(UnicastOK | BroadcastOK | MulticastOK | r_Pro);
 
assign BroadcastOK = Broadcast & ~r_Bro;
 
assign RxCheckEn   = | StateData;
 
 // Address Error Reported at end of address cycle
 // RxAbort clears after one cycle
 
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    RxAbort <= #Tp 1'b0;
  else if(RxAddressInvalid & ByteCntEq7 & RxCheckEn)
    RxAbort <= #Tp 1'b1;
  else
    RxAbort <= #Tp 1'b0;
end
 

// This ff holds the "Address Miss" information that is written to the RX BD status.
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    AddressMiss <= #Tp 1'b0;
  else if(ByteCntEq7 & RxCheckEn)
    AddressMiss <= #Tp (~(UnicastOK | BroadcastOK | MulticastOK | (PassAll & ControlFrmAddressOK)));
end


// Hash Address Check, Multicast
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    MulticastOK <= #Tp 1'b0;
  else if(RxEndFrm | RxAbort)
    MulticastOK <= #Tp 1'b0;
  else if(CrcHashGood & Multicast)
    MulticastOK <= #Tp HashBit;
end
 
 
// Address Detection (unicast)
// start with ByteCntEq2 due to delay of addres from RxData
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    UnicastOK <= #Tp 1'b0;
  else
  if(RxCheckEn & ByteCntEq2)
    UnicastOK <= #Tp   RxData[7:0] == MAC[47:40];
  else
  if(RxCheckEn & ByteCntEq3)
    UnicastOK <= #Tp ( RxData[7:0] == MAC[39:32]) & UnicastOK;
  else
  if(RxCheckEn & ByteCntEq4)
    UnicastOK <= #Tp ( RxData[7:0] == MAC[31:24]) & UnicastOK;
  else
  if(RxCheckEn & ByteCntEq5)
    UnicastOK <= #Tp ( RxData[7:0] == MAC[23:16]) & UnicastOK;
  else
  if(RxCheckEn & ByteCntEq6)
    UnicastOK <= #Tp ( RxData[7:0] == MAC[15:8])  & UnicastOK;
  else
  if(RxCheckEn & ByteCntEq7)
    UnicastOK <= #Tp ( RxData[7:0] == MAC[7:0])   & UnicastOK;
  else
  if(RxEndFrm | RxAbort)
    UnicastOK <= #Tp 1'b0;
end
   
assign IntHash = (CrcHash[5])? HASH1 : HASH0;
  
always@(CrcHash or IntHash)
begin
  case(CrcHash[4:3])
    2'b00: ByteHash = IntHash[7:0];
    2'b01: ByteHash = IntHash[15:8];
    2'b10: ByteHash = IntHash[23:16];
    2'b11: ByteHash = IntHash[31:24];
  endcase
end
      
assign HashBit = ByteHash[CrcHash[2:0]];


endmodule
