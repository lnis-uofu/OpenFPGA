//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "pci_wbs_wbb3_2_wbb2.v"                           ////
////                                                              ////
////  This file is part of the "PCI bridge" project               ////
////  http://www.opencores.org/cores/pci/                         ////
////                                                              ////
////  Author(s):                                                  ////
////      - Miha Dolenc (mihad@opencores.org)                     ////
////                                                              ////
////                                                              ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2003 Miha Dolenc, mihad@opencores.org          ////
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
// $Log: pci_wbs_wbb3_2_wbb2.v,v $
// Revision 1.5  2004/08/16 09:12:01  mihad
// Removed unsinthesizable !== comparation.
//
// Revision 1.4  2004/01/24 11:54:18  mihad
// Update! SPOCI Implemented!
//
// Revision 1.3  2003/12/19 11:11:30  mihad
// Compact PCI Hot Swap support added.
// New testcases added.
// Specification updated.
// Test application changed to support WB B3 cycles.
//
// Revision 1.2  2003/12/01 16:20:56  simons
// ifdef - endif statements put in separate lines for flint compatibility.
//
// Revision 1.1  2003/08/12 13:58:19  mihad
// Module that converts slave WISHBONE B3 accesses to
// WISHBONE B2 accesses with CAB.
//
//

module pci_wbs_wbb3_2_wbb2
(
    wb_clk_i,
    wb_rst_i,

    wbs_cyc_i,
    wbs_cyc_o,
    wbs_stb_i,
    wbs_stb_o,
    wbs_adr_i,
    wbs_adr_o,
    wbs_dat_i_i,
    wbs_dat_i_o,
    wbs_dat_o_i,
    wbs_dat_o_o,
    wbs_we_i,
    wbs_we_o,
    wbs_sel_i,
    wbs_sel_o,
    wbs_ack_i,
    wbs_ack_o,
    wbs_err_i,
    wbs_err_o,
    wbs_rty_i,
    wbs_rty_o,
    wbs_cti_i,
    wbs_bte_i,
    wbs_cab_o,
    wb_init_complete_i
) ;

input       wb_clk_i    ;
input       wb_rst_i    ;

input           wbs_cyc_i           ;
output          wbs_cyc_o           ;
input           wbs_stb_i           ;
output          wbs_stb_o           ;
input   [31:0]  wbs_adr_i           ;
output  [31:0]  wbs_adr_o           ;
input   [31:0]  wbs_dat_i_i         ;
output  [31:0]  wbs_dat_i_o         ;
input   [31:0]  wbs_dat_o_i         ;
output  [31:0]  wbs_dat_o_o         ;
input           wbs_we_i            ;
output          wbs_we_o            ;
input   [ 3:0]  wbs_sel_i           ;
output  [ 3:0]  wbs_sel_o           ;
input           wbs_ack_i           ;
output          wbs_ack_o           ;
input           wbs_err_i           ;
output          wbs_err_o           ;
input           wbs_rty_i           ;
output          wbs_rty_o           ;
input   [ 2:0]  wbs_cti_i           ;
input   [ 1:0]  wbs_bte_i           ;
output          wbs_cab_o           ;
input           wb_init_complete_i  ;

reg             wbs_cyc_o           ;
reg     [31:0]  wbs_adr_o           ;
reg     [31:0]  wbs_dat_i_o         ;
reg             wbs_dat_i_o_valid   ;
reg     [31:0]  wbs_dat_o_o         ;
reg             wbs_we_o            ;
reg     [ 3:0]  wbs_sel_o           ;
reg             wbs_ack_o           ;
reg             wbs_err_o           ;
reg             wbs_rty_o           ; 
reg             wbs_cab_o           ;

always@(posedge wb_rst_i or posedge wb_clk_i)
begin
    if (wb_rst_i)
    begin
        wbs_cyc_o           <= 1'b0  ;
        wbs_adr_o           <= 32'h0 ;
        wbs_dat_i_o         <= 32'h0 ;
        wbs_dat_o_o         <= 32'h0 ;
        wbs_sel_o           <= 4'h0  ;
        wbs_we_o            <= 1'b0  ;
        wbs_dat_i_o_valid   <= 1'b0  ;
        wbs_cab_o           <= 1'b0  ;
    end
    else
    begin:transfer_and_transfer_adr_ctrl_blk
        reg start_cycle            ;
        reg [3:0] end_cycle        ;
        reg generate_int_adr       ;
        
        start_cycle  = ~wbs_cyc_o & wbs_cyc_i & wbs_stb_i & ~wbs_ack_o & ~wbs_err_o & ~wbs_rty_o & wb_init_complete_i ;
        
        // there is a few conditions when cycle must be terminated
        // I've put them into bit array for better readability of the code

        // 1st condition - pci bridge is signaling an error
        end_cycle[0] = wbs_err_i ;
    
        // 2nd condition - pci bridge is signaling a retry - that can be ignored via the defines
        end_cycle[1] = wbs_rty_i 
            `ifdef PCI_WBS_B3_RTY_DISABLE 
                & 1'b0 
            `endif 
                ;

        // 3rd condition - end non burst cycles as soon as pci bridge response is received
        end_cycle[2] = wbs_cyc_i & wbs_stb_i & wbs_ack_i & ~wbs_cab_o ;

        // 4th condition - end cycle when acknowledge and strobe are both asserted and master is signaling end of burst
        end_cycle[3] = wbs_cyc_i & wbs_stb_i & wbs_ack_o & wbs_cab_o & (wbs_cti_i == 3'b111) ;

        if (wbs_dat_i_o_valid)
        begin
            if (wbs_ack_i | wbs_err_i 
                `ifdef PCI_WBS_B3_RTY_DISABLE 
                `else 
                    | wbs_rty_i 
                `endif
                    )
                wbs_dat_i_o_valid <= 1'b0 ;
        end
        else
        begin
            if (wbs_cyc_i & wbs_stb_i & wbs_we_i & ~wbs_ack_o & ~wbs_err_o & ~wbs_rty_o & wb_init_complete_i)
            begin
                wbs_dat_i_o       <= wbs_dat_i_i ;
                wbs_dat_i_o_valid <= 1'b1 ;
            end
        end

        if (start_cycle)
        begin
            wbs_cyc_o   <= 1'b1         ;
            wbs_sel_o   <= wbs_sel_i    ;
            wbs_we_o    <= wbs_we_i     ;
            
            if (wbs_cti_i == 3'b010)
            begin
                case (wbs_bte_i)
                2'b00:  begin 
                            wbs_cab_o <= 1'b1 ; 
                        end
                2'b01:  begin
                            if (wbs_adr_i[3:2] == 2'b00)
                                wbs_cab_o <= 1'b1 ;
                            else
                                wbs_cab_o <= 1'b0 ;
                        end
                2'b10:  begin
                            if (wbs_adr_i[4:2] == 3'b000)
                                wbs_cab_o <= 1'b1 ;
                            else
                                wbs_cab_o <= 1'b0 ;
                        end
                2'b11:  begin
                            if (wbs_adr_i[5:2] == 4'b0000)
                                wbs_cab_o <= 1'b1 ;
                            else
                                wbs_cab_o <= 1'b0 ;
                        end
                endcase
            end
            else
            begin
                wbs_cab_o <= 1'b0 ;
            end
        end
        else if ( wbs_cyc_o & (|end_cycle) )
        begin
            wbs_cyc_o <= 1'b0 ;
        end        

        if (start_cycle)
            wbs_adr_o <= wbs_adr_i ;
        else if (wbs_ack_i)
            wbs_adr_o[31:2] <= wbs_adr_o[31:2] + 1'b1 ;

        if (~wbs_we_o & wbs_ack_i)
            wbs_dat_o_o <= wbs_dat_o_i ;
    end
end

always@(posedge wb_rst_i or posedge wb_clk_i)
begin
    if (wb_rst_i)
    begin
        wbs_ack_o <= 1'b0 ;
        wbs_err_o <= 1'b0 ;
        wbs_rty_o <= 1'b0 ;
    end
    else
    begin
        if (wbs_ack_o)
            wbs_ack_o <= wbs_ack_i | ~wbs_stb_i ;
        else
            wbs_ack_o <= wbs_ack_i ;
        
        if (wbs_err_o)
            wbs_err_o <= ~wbs_stb_i ;
        else
            wbs_err_o <= wbs_err_i ;

    `ifdef PCI_WBS_B3_RTY_DISABLE
        wbs_rty_o <= 1'b0 ;
    `else
        if (wbs_rty_o)
            wbs_rty_o <= ~wbs_stb_i ;
        else
            wbs_rty_o <= wbs_rty_i ;
    `endif
    end
end

assign wbs_stb_o = (wbs_cyc_o & ~wbs_we_o & ~wbs_ack_o & ~wbs_err_o & ~wbs_rty_o) |
                   (wbs_cyc_o & wbs_stb_i & wbs_cab_o & ~wbs_we_o & wbs_cti_i != 3'b111) |
                   (wbs_cyc_o & wbs_we_o & wbs_dat_i_o_valid) ;

endmodule
