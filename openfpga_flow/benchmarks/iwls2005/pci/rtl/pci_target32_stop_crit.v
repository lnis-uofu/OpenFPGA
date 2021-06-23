//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name: pci_target32_stop_crit.v                         ////
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
// $Log: pci_target32_stop_crit.v,v $
// Revision 1.4  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.3  2002/02/01 15:25:12  mihad
// Repaired a few bugs, updated specification, added test bench files and design document
//
// Revision 1.2  2001/10/05 08:14:30  mihad
// Updated all files with inclusion of timescale file for simulation purposes.
//
// Revision 1.1.1.1  2001/10/02 15:33:47  mihad
// New project directory structure
//
//

// module is used to separate logic which uses criticaly constrained inputs from slower logic.
// It is used to synthesize critical timing logic separately with faster cells or without optimization

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_target32_stop_crit
(
    stop_w,
    stop_w_frm,
    stop_w_frm_irdy,
    pci_frame_in,
    pci_irdy_in,
    pci_stop_out
);

input       stop_w ;			// stop signal (composed without critical signals) that do not need critical inputs
input       stop_w_frm ;		// stop signal (composed without critical signals) that needs AND with critical FRAME input
input       stop_w_frm_irdy ;	// stop signal (composed without critical signals) that needs AND with critical FRAME and
								// IRDY inputs
input       pci_frame_in ;		// critical constrained input signal
input		pci_irdy_in ;		// critical constrained input signal

output		pci_stop_out ;		// PCI stop output

// PCI stop output with preserved hierarchy for minimum delay!
assign 	pci_stop_out = ~(stop_w || (stop_w_frm && ~pci_frame_in) || (stop_w_frm_irdy && ~pci_frame_in && ~pci_irdy_in)) ;


endmodule
