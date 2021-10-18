//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "wb_addr_mux.v"                                   ////
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
//// Copyright (C) 2000 Miha Dolenc, mihad@opencores.org          ////
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
// $Log: pci_wb_addr_mux.v,v $
// Revision 1.1  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.4  2002/08/19 16:54:25  mihad
// Got rid of undef directives
//
// Revision 1.3  2002/02/01 15:25:13  mihad
// Repaired a few bugs, updated specification, added test bench files and design document
//
// Revision 1.2  2001/10/05 08:14:30  mihad
// Updated all files with inclusion of timescale file for simulation purposes.
//
// Revision 1.1.1.1  2001/10/02 15:33:47  mihad
// New project directory structure
//
//

// module provides instantiation of address decoders and address multiplexer for various number of implemented wishbone images
`include "pci_constants.v"
// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_wb_addr_mux
(
    `ifdef REGISTER_WBS_OUTPUTS
    clk_in,
    reset_in,
    sample_address_in,
    `endif
    address_in,
    bar0_in,
    bar1_in,
    bar2_in,
    bar3_in,
    bar4_in,
    bar5_in,
    am0_in,
    am1_in,
    am2_in,
    am3_in,
    am4_in,
    am5_in,
    ta0_in,
    ta1_in,
    ta2_in,
    ta3_in,
    ta4_in,
    ta5_in,
    at_en_in,
    hit_out,
    address_out
);

input [31:0] address_in ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] bar0_in  ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] bar1_in  ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] bar2_in  ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] bar3_in  ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] bar4_in  ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] bar5_in  ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] am0_in   ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] am1_in   ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] am2_in   ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] am3_in   ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] am4_in   ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] am5_in   ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] ta0_in   ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] ta1_in   ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] ta2_in   ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] ta3_in   ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] ta4_in   ;
input [(`WB_NUM_OF_DEC_ADDR_LINES - 1):0] ta5_in   ;
input [5:0]  at_en_in ;
output [5:0] hit_out  ;
output [31:0] address_out ;
reg    [31:0] address_out ;

wire [31:0] addr0 ;
wire [31:0] addr1 ;
wire [31:0] addr2 ;
wire [31:0] addr3 ;
wire [31:0] addr4 ;
wire [31:0] addr5 ;

wire [5:0] hit ;
assign hit_out = hit ;

`ifdef REGISTER_WBS_OUTPUTS
    input clk_in, reset_in, sample_address_in ;

    reg [31:0] address ;
    always@(posedge clk_in or posedge reset_in)
    begin
       if ( reset_in )
           address <= #`FF_DELAY 0 ;
       else
       if ( sample_address_in )
           address <= #`FF_DELAY address_in ;
    end
`else
    wire [31:0] address = address_in ;
`endif

`ifdef GUEST
    `ifdef NO_CNF_IMAGE
    `else
        `define PCI_WB_ADDR_MUX_DEC0_INCLUDE
    `endif
`else
`ifdef HOST
    `define PCI_WB_ADDR_MUX_DEC0_INCLUDE
`endif
`endif

`ifdef PCI_WB_ADDR_MUX_DEC0_INCLUDE
    pci_wb_decoder #(`WB_NUM_OF_DEC_ADDR_LINES) dec0
    (
     .hit       (hit[0]),
     .addr_out  (addr0),
     .addr_in   (address),
     .base_addr (bar0_in),
     .mask_addr (am0_in),
     .tran_addr (ta0_in),
     .at_en     (1'b0)
    ) ;
`else
    // configuration image not implemented
    assign hit[0] = 1'b0 ;
    assign addr0  = 32'h0000_0000 ;
`endif

// one image is always implemented
pci_wb_decoder #(`WB_NUM_OF_DEC_ADDR_LINES) dec1
(
 .hit       (hit[1]),
 .addr_out  (addr1),
 .addr_in   (address),
 .base_addr (bar1_in),
 .mask_addr (am1_in),
 .tran_addr (ta1_in),
 .at_en     (at_en_in[1])
) ;

`ifdef WB_IMAGE2
    pci_wb_decoder #(`WB_NUM_OF_DEC_ADDR_LINES) dec2
    (
     .hit       (hit[2]),
     .addr_out  (addr2),
     .addr_in   (address),
     .base_addr (bar2_in),
     .mask_addr (am2_in),
     .tran_addr (ta2_in),
     .at_en     (at_en_in[2])
    ) ;

`else
    assign hit[2] = 1'b0 ;
    assign addr2  = 0 ;
`endif

`ifdef WB_IMAGE3
    pci_wb_decoder #(`WB_NUM_OF_DEC_ADDR_LINES) dec3
    (
     .hit       (hit[3]),
     .addr_out  (addr3),
     .addr_in   (address),
     .base_addr (bar3_in),
     .mask_addr (am3_in),
     .tran_addr (ta3_in),
     .at_en     (at_en_in[3])
    ) ;
`else
    assign hit[3] = 1'b0 ;
    assign addr3  = 0 ;
`endif

`ifdef WB_IMAGE4
    pci_wb_decoder #(`WB_NUM_OF_DEC_ADDR_LINES) dec4
    (
     .hit       (hit[4]),
     .addr_out  (addr4),
     .addr_in   (address),
     .base_addr (bar4_in),
     .mask_addr (am4_in),
     .tran_addr (ta4_in),
     .at_en     (at_en_in[4])
    ) ;
`else
    assign hit[4] = 1'b0 ;
    assign addr4  = 0 ;
`endif

`ifdef WB_IMAGE5
    pci_wb_decoder #(`WB_NUM_OF_DEC_ADDR_LINES) dec5
    (
     .hit       (hit[5]),
     .addr_out  (addr5),
     .addr_in   (address),
     .base_addr (bar5_in),
     .mask_addr (am5_in),
     .tran_addr (ta5_in),
     .at_en     (at_en_in[5])
    ) ;
`else
    assign hit[5] = 1'b0 ;
    assign addr5  = 0 ;
`endif

// address multiplexer
always@
(
 hit or
 addr0 or
 addr1 or
 addr2 or
 addr3 or
 addr4 or
 addr5
)
begin
    case ( {hit[5:2], hit[0]} )
        5'b0_0_0_0_1: address_out = addr0 ;
        5'b0_0_0_1_0: address_out = addr2 ;
        5'b0_0_1_0_0: address_out = addr3 ;
        5'b0_1_0_0_0: address_out = addr4 ;
        5'b1_0_0_0_0: address_out = addr5 ;

        // default address is address from decoder 1 - it is always implemented - in case of stripped down core to only one image
        // this multiplexer can be completely removed during synthesys
        default:      address_out = addr1 ;
    endcase
end

endmodule
