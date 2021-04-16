//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "cur_out_reg.v"                                   ////
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
// $Log: pci_cur_out_reg.v,v $
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

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on
`include "pci_constants.v"

// module is only a backup copy of relevant output registers
// used in some arhitectures that support IOB registers, which have to have a
// fanout of 1
// Otherwise nothing special in this module
module pci_cur_out_reg
(
    reset_in,
    clk_in,
    frame_in,
    frame_load_in,
    irdy_in,
    devsel_in,
    trdy_in,
    trdy_en_in,
    stop_in,
    ad_load_in,
    cbe_in,
    cbe_en_in,
    mas_ad_in,
    tar_ad_in,
    frame_en_in,
    irdy_en_in,

    mas_ad_en_in,
    tar_ad_en_in,
    ad_en_unregistered_in,

    par_in,
    par_en_in,
    perr_in,
    perr_en_in,
    serr_in,
    serr_en_in,

    frame_out,
    irdy_out,
    devsel_out,
    trdy_out,
    stop_out,
    cbe_out,
    cbe_en_out,
    ad_out,
    frame_en_out,
    irdy_en_out,
    ad_en_out,
    mas_ad_en_out,
    tar_ad_en_out,
    trdy_en_out,

    par_out,
    par_en_out,
    perr_out,
    perr_en_out,
    serr_out,
    serr_en_out
) ;

input reset_in, clk_in ;

input           frame_in ;
input           frame_load_in ;
input           irdy_in ;
input           devsel_in ;
input           trdy_in ;
input           stop_in ;
input           ad_load_in ;

input [3:0]     cbe_in ;
input           cbe_en_in ;
input [31:0]    mas_ad_in ;
input [31:0]    tar_ad_in ;

input           mas_ad_en_in ;
input           tar_ad_en_in ;
input           ad_en_unregistered_in ;

input           frame_en_in,
                irdy_en_in ;

input           trdy_en_in ;

input par_in ;
input par_en_in ;
input perr_in ;
input perr_en_in ;
input serr_in ;
input serr_en_in ;

output          frame_out ;
reg             frame_out ;
output          irdy_out ;
reg             irdy_out ;
output          devsel_out ;
reg             devsel_out ;
output          trdy_out ;
reg             trdy_out ;
output          stop_out ;
reg             stop_out ;
output [3:0]    cbe_out ;
reg    [3:0]    cbe_out ;
output [31:0]   ad_out ;
reg    [31:0]   ad_out ;

output          frame_en_out,
                irdy_en_out,
                ad_en_out,
                cbe_en_out,
                mas_ad_en_out,
                tar_ad_en_out,
                trdy_en_out ;

reg             frame_en_out,
                irdy_en_out,
                cbe_en_out,
                mas_ad_en_out,
                tar_ad_en_out,
                trdy_en_out;

output          par_out ;
output          par_en_out ;
output          perr_out ;
output          perr_en_out ;
output          serr_out ;
output          serr_en_out ;

reg             par_out ;
reg             par_en_out ;
reg             perr_out ;
reg             perr_en_out ;
reg             serr_out ;
reg             serr_en_out ;

assign ad_en_out = mas_ad_en_out || tar_ad_en_out ;

always@(posedge reset_in or posedge clk_in)
begin
    if ( reset_in )
    begin
        irdy_out     <= #`FF_DELAY 1'b1 ;
        devsel_out   <= #`FF_DELAY 1'b1 ;
        trdy_out     <= #`FF_DELAY 1'b1 ;
        stop_out     <= #`FF_DELAY 1'b1 ;
        frame_en_out <= #`FF_DELAY 1'b0 ;
        irdy_en_out  <= #`FF_DELAY 1'b0 ;
        mas_ad_en_out<= #`FF_DELAY 1'b0 ;
        tar_ad_en_out<= #`FF_DELAY 1'b0 ;
        trdy_en_out  <= #`FF_DELAY 1'b0 ;
        par_out      <= #`FF_DELAY 1'b0 ;
        par_en_out   <= #`FF_DELAY 1'b0 ;
        perr_out     <= #`FF_DELAY 1'b1 ;
        perr_en_out  <= #`FF_DELAY 1'b0 ;
        serr_out     <= #`FF_DELAY 1'b1 ;
        serr_en_out  <= #`FF_DELAY 1'b0 ;
        cbe_en_out   <= #`FF_DELAY 1'b0 ;

    end
    else
    begin
        irdy_out     <= #`FF_DELAY irdy_in ;
        devsel_out   <= #`FF_DELAY devsel_in ;
        trdy_out     <= #`FF_DELAY trdy_in ;
        stop_out     <= #`FF_DELAY stop_in ;
        frame_en_out <= #`FF_DELAY frame_en_in ;
        irdy_en_out  <= #`FF_DELAY irdy_en_in ;
        mas_ad_en_out<= #`FF_DELAY mas_ad_en_in && ad_en_unregistered_in ;
        tar_ad_en_out<= #`FF_DELAY tar_ad_en_in && ad_en_unregistered_in ;
        trdy_en_out  <= #`FF_DELAY trdy_en_in ;

        par_out      <= #`FF_DELAY par_in ;
        par_en_out   <= #`FF_DELAY par_en_in ;
        perr_out     <= #`FF_DELAY perr_in ;
        perr_en_out  <= #`FF_DELAY perr_en_in ;
        serr_out     <= #`FF_DELAY serr_in ;
        serr_en_out  <= #`FF_DELAY serr_en_in ;
        cbe_en_out   <= #`FF_DELAY cbe_en_in ;
    end
end

always@(posedge reset_in or posedge clk_in)
begin
    if ( reset_in )
        cbe_out <= #`FF_DELAY 4'hF ;
    else if ( ad_load_in )
        cbe_out <= #`FF_DELAY cbe_in ;

end

wire [31:0] ad_source = tar_ad_en_out ? tar_ad_in : mas_ad_in ;

always@(posedge reset_in or posedge clk_in)
begin
    if ( reset_in )
        ad_out <= #`FF_DELAY 32'h0000_0000 ;
    else if ( ad_load_in )
        ad_out <= #`FF_DELAY ad_source ;

end

always@(posedge reset_in or posedge clk_in)
begin
    if ( reset_in )
        frame_out <= #`FF_DELAY 1'b1 ;
    else if ( frame_load_in )
        frame_out <= #`FF_DELAY frame_in ;

end

endmodule
