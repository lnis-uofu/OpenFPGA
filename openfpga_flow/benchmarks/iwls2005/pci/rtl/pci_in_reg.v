//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name: pci_in_reg.v                                     ////
////                                                              ////
////  This file is part of the "PCI bridge" project               ////
////  http://www.opencores.org/cores/pci/                         ////
////                                                              ////
////  Author(s):                                                  ////
////      - Tadej Markovic, tadej@opencores.org                   ////
////                                                              ////
////  All additional information is avaliable in the README.txt   ////
////  file.                                                       ////
////                                                              ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000 Tadej Markovic, tadej@opencores.org       ////
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
// $Log: pci_in_reg.v,v $
// Revision 1.5  2003/12/19 11:11:30  mihad
// Compact PCI Hot Swap support added.
// New testcases added.
// Specification updated.
// Test application changed to support WB B3 cycles.
//
// Revision 1.4  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.3  2002/02/01 15:25:12  mihad
// Repaired a few bugs, updated specification, added test bench files and design document
//
// Revision 1.2  2001/10/05 08:14:29  mihad
// Updated all files with inclusion of timescale file for simulation purposes.
//
// Revision 1.1.1.1  2001/10/02 15:33:46  mihad
// New project directory structure
//
//

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on
`include "pci_constants.v"
// Module is used for registering PCI input signals
// It provides data flip flops with reset
module pci_in_reg
(    
    reset_in,
    clk_in,
    init_complete_in,

    pci_gnt_in,
    pci_frame_in,
    pci_irdy_in,
    pci_trdy_in,
    pci_stop_in,
    pci_devsel_in,
    pci_idsel_in,
    pci_ad_in,
    pci_cbe_in,

    pci_gnt_reg_out,
    pci_frame_reg_out,
    pci_irdy_reg_out,
    pci_trdy_reg_out,
    pci_stop_reg_out,
    pci_devsel_reg_out,
    pci_idsel_reg_out,
    pci_ad_reg_out,
    pci_cbe_reg_out

);

input			reset_in, clk_in, init_complete_in  ;

input           pci_gnt_in ;
input           pci_frame_in ;
input           pci_irdy_in ;
input           pci_trdy_in ;
input           pci_stop_in ;
input           pci_devsel_in ;
input			pci_idsel_in ;
input [31:0]    pci_ad_in ;
input [3:0]     pci_cbe_in ;

output          pci_gnt_reg_out ;
output          pci_frame_reg_out ;
output          pci_irdy_reg_out ;
output          pci_trdy_reg_out ;
output          pci_stop_reg_out ;
output          pci_devsel_reg_out ;
output			pci_idsel_reg_out ;
output [31:0]   pci_ad_reg_out ;
output [3:0]    pci_cbe_reg_out ;


reg             pci_gnt_reg_out ;
reg             pci_frame_reg_out ;
reg             pci_irdy_reg_out ;
reg             pci_trdy_reg_out ;
reg             pci_stop_reg_out ;
reg             pci_devsel_reg_out ;
reg				pci_idsel_reg_out ;
reg    [31:0]   pci_ad_reg_out ;
reg    [3:0]    pci_cbe_reg_out ;

always@(posedge reset_in or posedge clk_in)
begin
    if ( reset_in )
    begin
		pci_gnt_reg_out		<= #`FF_DELAY 1'b1 ;
		pci_frame_reg_out	<= #`FF_DELAY 1'b0 ;
		pci_irdy_reg_out	<= #`FF_DELAY 1'b1 ;
		pci_trdy_reg_out	<= #`FF_DELAY 1'b1 ;
		pci_stop_reg_out	<= #`FF_DELAY 1'b1 ;
		pci_devsel_reg_out	<= #`FF_DELAY 1'b1 ;
		pci_idsel_reg_out	<= #`FF_DELAY 1'b0 ; // active high!
		pci_ad_reg_out      <= #`FF_DELAY 32'h0000_0000 ;
		pci_cbe_reg_out     <= #`FF_DELAY 4'h0 ;
    end
    else if (init_complete_in)
	begin
		pci_gnt_reg_out		<= #`FF_DELAY pci_gnt_in ;
		pci_frame_reg_out	<= #`FF_DELAY pci_frame_in ;
		pci_irdy_reg_out	<= #`FF_DELAY pci_irdy_in ;
		pci_trdy_reg_out	<= #`FF_DELAY pci_trdy_in ;
		pci_stop_reg_out	<= #`FF_DELAY pci_stop_in ;
		pci_devsel_reg_out	<= #`FF_DELAY pci_devsel_in ;
		pci_idsel_reg_out	<= #`FF_DELAY pci_idsel_in ;
		pci_ad_reg_out      <= #`FF_DELAY pci_ad_in ;
		pci_cbe_reg_out     <= #`FF_DELAY pci_cbe_in ;
	end
end

endmodule
