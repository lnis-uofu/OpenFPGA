//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "pci_io_mux_ad_load_crit.v"                       ////
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
// $Log: pci_io_mux_ad_load_crit.v,v $
// Revision 1.2  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.1  2002/02/01 14:43:31  mihad
// *** empty log message ***
//
//

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

// module is provided for last level of logic for loading AD output flip-flops
// and output backup flip - flops
module pci_io_mux_ad_load_crit
(
    load_in,
    load_on_transfer_in,
    pci_irdy_in,
    pci_trdy_in,
    load_out
);

input  load_in,
       load_on_transfer_in,
       pci_irdy_in,
       pci_trdy_in ;

output load_out ;

assign load_out = load_in || (load_on_transfer_in && ~pci_irdy_in && ~pci_trdy_in) ;

endmodule
