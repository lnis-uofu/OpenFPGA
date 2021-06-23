//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "sync_module.v"                                   ////
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
// $Log: pci_sync_module.v,v $
// Revision 1.3  2003/08/14 13:06:03  simons
// synchronizer_flop replaced with pci_synchronizer_flop, artisan ram instance updated.
//
// Revision 1.2  2003/03/26 13:16:18  mihad
// Added the reset value parameter to the synchronizer flop module.
// Added resets to all synchronizer flop instances.
// Repaired initial sync value in fifos.
//
// Revision 1.1  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.1  2002/02/01 14:43:31  mihad
// *** empty log message ***
//
//
//

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_sync_module
(
					set_clk_in,
					delete_clk_in,
					reset_in,
					delete_set_out,
					block_set_out,
					delete_in
);

// system inputs from two clock domains
input	set_clk_in;
input	delete_clk_in;
input	reset_in;
// control outputs
output	delete_set_out;
output	block_set_out;
// control input
input	delete_in;

// internal signals
reg		del_bit;
wire	meta_del_bit;
reg		sync_del_bit;
reg		delayed_del_bit;
wire	meta_bckp_bit;
reg		sync_bckp_bit;
reg		delayed_bckp_bit;


// DELETE_IN input FF - when set must be active, until it is sinchronously cleared
always@(posedge delete_clk_in or posedge reset_in)
begin
	if (reset_in)
		del_bit <= 1'b0;
	else
	begin
		if (!delayed_bckp_bit && sync_bckp_bit)
			del_bit <= 1'b0;
		else if (delete_in)
			del_bit <= 1'b1;
	end
end
assign	block_set_out = del_bit;

// interemediate stage to clk synchronization flip - flops - this ones are prone to metastability
pci_synchronizer_flop	#(1, 0) delete_sync
(
    .data_in        (del_bit),
    .clk_out        (set_clk_in),
    .sync_data_out  (meta_del_bit),
    .async_reset    (reset_in)
) ;

// Final synchronization of del_bit signal to the set clock domain
always@(posedge set_clk_in or posedge reset_in)
begin
	if (reset_in)
		sync_del_bit <= 1'b0;
	else
		sync_del_bit <= meta_del_bit;
end

// Delayed sync_del_bit signal for one clock period pulse generation
always@(posedge set_clk_in or posedge reset_in)
begin
	if (reset_in)
		delayed_del_bit <= 1'b0;
	else
		delayed_del_bit <= sync_del_bit;
end

assign	delete_set_out = !delayed_del_bit && sync_del_bit;

// interemediate stage to clk synchronization flip - flops - this ones are prone to metastability
pci_synchronizer_flop	#(1, 0) clear_delete_sync
(
    .data_in        (sync_del_bit),
    .clk_out        (delete_clk_in),
    .sync_data_out  (meta_bckp_bit),
    .async_reset    (reset_in)
) ;

// Final synchronization of sync_del_bit signal to the delete clock domain
always@(posedge delete_clk_in or posedge reset_in)
begin
	if (reset_in)
		sync_bckp_bit <= 1'b0;
	else
		sync_bckp_bit <= meta_bckp_bit;
end

// Delayed sync_bckp_bit signal for one clock period pulse generation
always@(posedge delete_clk_in or posedge reset_in)
begin
	if (reset_in)
		delayed_bckp_bit <= 1'b0;
	else
		delayed_bckp_bit <= sync_bckp_bit;
end

endmodule
