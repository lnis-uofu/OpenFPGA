//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_rxstatem.v                                              ////
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
// $Log: eth_rxstatem.v,v $
// Revision 1.6  2002/11/13 22:28:26  tadejm
// StartIdle state changed (not important the size of the packet).
// StartData1 activates only while ByteCnt is smaller than the MaxFrame.
//
// Revision 1.5  2002/01/23 10:28:16  mohor
// Link in the header changed.
//
// Revision 1.4  2001/10/19 08:43:51  mohor
// eth_timescale.v changed to timescale.v This is done because of the
// simulation of the few cores in a one joined project.
//
// Revision 1.3  2001/10/18 12:07:11  mohor
// Status signals changed, Adress decoding changed, interrupt controller
// added.
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
// Revision 1.2  2001/07/03 12:55:41  mohor
// Minor changes because of the synthesys warnings.
//
//
// Revision 1.1  2001/06/27 21:26:19  mohor
// Initial release of the RxEthMAC module.
//
//
//
//


`include "timescale.v"


module eth_rxstatem (MRxClk, Reset, MRxDV, ByteCntEq0, ByteCntGreat2, Transmitting, MRxDEq5, MRxDEqD, 
                     IFGCounterEq24, ByteCntMaxFrame, StateData, StateIdle, StatePreamble, StateSFD, 
                     StateDrop
                    );

parameter Tp = 1;

input         MRxClk;
input         Reset;
input         MRxDV;
input         ByteCntEq0;
input         ByteCntGreat2;
input         MRxDEq5;
input         Transmitting;
input         MRxDEqD;
input         IFGCounterEq24;
input         ByteCntMaxFrame;

output [1:0]  StateData;
output        StateIdle;
output        StateDrop;
output        StatePreamble;
output        StateSFD;

reg           StateData0;
reg           StateData1;
reg           StateIdle;
reg           StateDrop;
reg           StatePreamble;
reg           StateSFD;

wire          StartIdle;
wire          StartDrop;
wire          StartData0;
wire          StartData1;
wire          StartPreamble;
wire          StartSFD;


// Defining the next state
assign StartIdle = ~MRxDV & (StateDrop | StatePreamble | StateSFD | (|StateData));

assign StartPreamble = MRxDV & ~MRxDEq5 & (StateIdle & ~Transmitting);

assign StartSFD = MRxDV & MRxDEq5 & (StateIdle & ~Transmitting | StatePreamble);

assign StartData0 = MRxDV & (StateSFD & MRxDEqD & IFGCounterEq24 | StateData1);

assign StartData1 = MRxDV & StateData0 & (~ByteCntMaxFrame);

assign StartDrop = MRxDV & (StateIdle & Transmitting | StateSFD & ~IFGCounterEq24 &  MRxDEqD 
                         |  StateData0 &  ByteCntMaxFrame
                           );

// Rx State Machine
always @ (posedge MRxClk or posedge Reset)
begin
  if(Reset)
    begin
      StateIdle     <= #Tp 1'b0;
      StateDrop     <= #Tp 1'b1;
      StatePreamble <= #Tp 1'b0;
      StateSFD      <= #Tp 1'b0;
      StateData0    <= #Tp 1'b0;
      StateData1    <= #Tp 1'b0;
    end
  else
    begin
      if(StartPreamble | StartSFD | StartDrop)
        StateIdle <= #Tp 1'b0;
      else
      if(StartIdle)
        StateIdle <= #Tp 1'b1;

      if(StartIdle)
        StateDrop <= #Tp 1'b0;
      else
      if(StartDrop)
        StateDrop <= #Tp 1'b1;

      if(StartSFD | StartIdle | StartDrop)
        StatePreamble <= #Tp 1'b0;
      else
      if(StartPreamble)
        StatePreamble <= #Tp 1'b1;

      if(StartPreamble | StartIdle | StartData0 | StartDrop)
        StateSFD <= #Tp 1'b0;
      else
      if(StartSFD)
        StateSFD <= #Tp 1'b1;

      if(StartIdle | StartData1 | StartDrop)
        StateData0 <= #Tp 1'b0;
      else
      if(StartData0)
        StateData0 <= #Tp 1'b1;

      if(StartIdle | StartData0 | StartDrop)
        StateData1 <= #Tp 1'b0;
      else
      if(StartData1)
        StateData1 <= #Tp 1'b1;
    end
end

assign StateData[1:0] = {StateData1, StateData0};

endmodule
