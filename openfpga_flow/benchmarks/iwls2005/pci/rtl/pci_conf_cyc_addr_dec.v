//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "conf_cyc_addr_dec.v"                             ////
////                                                              ////
////  This file is part of the "PCI bridge" project               ////
////  http://www.opencores.org/cores/pci/                         ////
////                                                              ////
////  Author(s):                                                  ////
////      - Miha Dolenc (mihad@opencores.org)                     ////
////                                                              ////
////  All additional information is avaliable in the README       ////
////  file.                                                       ////
////                                                              ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2001 Miha Dolenc, mihad@opencores.org          ////
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
// $Log: pci_conf_cyc_addr_dec.v,v $
// Revision 1.1  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.3  2002/02/01 15:25:12  mihad
// Repaired a few bugs, updated specification, added test bench files and design document
//
// Revision 1.2  2001/10/05 08:14:28  mihad
// Updated all files with inclusion of timescale file for simulation purposes.
//
// Revision 1.1.1.1  2001/10/02 15:33:46  mihad
// New project directory structure
//
//


// module is a simple decoder which decodes device num field of configuration address
// for type0 configuration cycles. If type 1 configuration cycle is
// initiated then address goes through unchanged

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_conf_cyc_addr_dec
(
    ccyc_addr_in,
    ccyc_addr_out
) ;

input   [31:0]  ccyc_addr_in ;
output  [31:0]  ccyc_addr_out ;
reg     [31:11]  ccyc_addr_31_11 ;

// lower 11 address lines are alweys going through unchanged
assign ccyc_addr_out = {ccyc_addr_31_11, ccyc_addr_in[10:0]} ;

// configuration cycle type indicator
wire ccyc_type = ccyc_addr_in[0] ;

always@(ccyc_addr_in or ccyc_type)
begin
    if (ccyc_type)
        // type 1 cycle - address goes through unchanged
        ccyc_addr_31_11 = ccyc_addr_in[31:11] ;
    else
    begin
        // type 0 conf. cycle - decode device number field to appropriate value
        case (ccyc_addr_in[15:11])
            5'h00:ccyc_addr_31_11 = 21'h00_0001 ;
            5'h01:ccyc_addr_31_11 = 21'h00_0002 ;
            5'h02:ccyc_addr_31_11 = 21'h00_0004 ;
            5'h03:ccyc_addr_31_11 = 21'h00_0008 ;
            5'h04:ccyc_addr_31_11 = 21'h00_0010 ;
            5'h05:ccyc_addr_31_11 = 21'h00_0020 ;
            5'h06:ccyc_addr_31_11 = 21'h00_0040 ;
            5'h07:ccyc_addr_31_11 = 21'h00_0080 ;
            5'h08:ccyc_addr_31_11 = 21'h00_0100 ;
            5'h09:ccyc_addr_31_11 = 21'h00_0200 ;
            5'h0A:ccyc_addr_31_11 = 21'h00_0400 ;
            5'h0B:ccyc_addr_31_11 = 21'h00_0800 ;
            5'h0C:ccyc_addr_31_11 = 21'h00_1000 ;
            5'h0D:ccyc_addr_31_11 = 21'h00_2000 ;
            5'h0E:ccyc_addr_31_11 = 21'h00_4000 ;
            5'h0F:ccyc_addr_31_11 = 21'h00_8000 ;
            5'h10:ccyc_addr_31_11 = 21'h01_0000 ;
            5'h11:ccyc_addr_31_11 = 21'h02_0000 ;
            5'h12:ccyc_addr_31_11 = 21'h04_0000 ;
            5'h13:ccyc_addr_31_11 = 21'h08_0000 ;
            5'h14:ccyc_addr_31_11 = 21'h10_0000 ;
            default: ccyc_addr_31_11 = 21'h00_0000 ;
        endcase
    end
end

endmodule
