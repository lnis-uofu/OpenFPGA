//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name: decoder.v                                        ////
////                                                              ////
////  This file is part of the "PCI bridge" project               ////
////  http://www.opencores.org/cores/pci/                         ////
////                                                              ////
////  Author(s):                                                  ////
////      - Tadej Markovic, tadej@opencores.org                   ////
////      - Tilen Novak, tilen@opencores.org                      ////
////                                                              ////
////  All additional information is avaliable in the README.txt   ////
////  file.                                                       ////
////                                                              ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000 Tadej Markovic, tadej@opencores.org       ////
////                    Tilen Novak, tilen@opencores.org          ////
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
// $Log: pci_wb_decoder.v,v $
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

`include "pci_constants.v"

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_wb_decoder (hit, addr_out, addr_in, base_addr, mask_addr, tran_addr, at_en) ;

// Decoding address size parameter - for FPGAs 1MegByte is recommended
//   MAXIMUM is 20 (4KBytes), length 12 is 1 MByte !!!
parameter		decode_len     = 12 ;

//###########################################################################################################
// ALL COMMENTS are written as there were decode_len 20. This number and 12 (32 - 20) are assigning the
// numbers of decoded and compared bits, etc.
//###########################################################################################################

/*-----------------------------------------------------------------------------------------------------------
DECODER interface decodes input address (ADDR_IN); what means that it validates (HIT), if input address
falls within the defined image space boundaries. Image space boundarie is defined with image base address
register (BASE_ADDR) and address mask register (MASK_ADDR).
Beside that, it also translates (maps) the input address to the output address (ADDR_OUT), regarding the
translation address register (TRAN_ADDR) and the address mask register.
-----------------------------------------------------------------------------------------------------------*/

// output control
output	hit ;
// output address
output	[31:0]	addr_out ;
// input address
input	[31:0]	addr_in ;

// input registers - 12 LSbits are not valid since the smallest possible size is 4KB !
input	[31:(32-decode_len)]	base_addr ;
input	[31:(32-decode_len)]	mask_addr ;
input	[31:(32-decode_len)]	tran_addr ;

// input bit[2] of the Image Control register used to enable the address translation !
input	at_en ;
/*-----------------------------------------------------------------------------------------------------------
Internal signals !
-----------------------------------------------------------------------------------------------------------*/

// bit[31] if address mask register is IMAGE ENABLE bit (img_en)
wire	img_en ;

// addr_in_compare are masked input address bits that are compared with masked base_addr
wire	[31:(32-decode_len)]	addr_in_compare ;
// base_addr_compare are masked base address bits that are compared with masked addr_in
wire	[31:(32-decode_len)]	base_addr_compare ;

/*-----------------------------------------------------------------------------------------------------------
Decoding the input address!
This logic produces the loghest path in this module!

20 MSbits of input addres are as well as base address (20 bits) masked with corrected address mask. Only
masked bits of each vector are actually logically compared.
Bit[31] of address mask register is used to enable the image space !
-----------------------------------------------------------------------------------------------------------*/

assign addr_in_compare = (addr_in[31:(32-decode_len)] & mask_addr) ;

assign base_addr_compare = (base_addr & mask_addr) ;

assign img_en = mask_addr[31] ;

assign hit = { 1'b1, addr_in_compare } == { img_en, base_addr_compare } ;

/*-----------------------------------------------------------------------------------------------------------
Translating the input address!

Translation of input address is not implemented if ADDR_TRAN_IMPL is not defined

20 MSbits of input address are masked with negated value of the corrected address mask in order to get
address bits of the input address which won't be replaced with translation address bits.
Translation address bits (20 bits) are masked with corrected address mask. Only masked bits of vector are
actually valid, all others are zero.
Boath vectors are bit-wise ORed in order to get the valid translation address with an offset of an input
address.
12 LSbits of an input address are assigned to 12 LSbits of an output addres.
-----------------------------------------------------------------------------------------------------------*/

`ifdef ADDR_TRAN_IMPL
    // if Address Translation Enable bit is set, then translation address is used othervise input address is used!
    // addr_in_combine input address bits are not replaced with translation address!
    wire	[31:(32-decode_len)] addr_in_combine ;
    // tran_addr_combine are masked and combined with addr_in_combine!
    reg		[31:(32-decode_len)] tran_addr_combine ;

    assign addr_in_combine = (addr_in[31:(32-decode_len)] & ~mask_addr) ;
    always@(at_en or tran_addr or mask_addr or addr_in)
	begin
	    if (at_en)
			begin
				tran_addr_combine <= (tran_addr & mask_addr) ;
    		end
    	else
			begin
				tran_addr_combine <= (addr_in[31:(32-decode_len)] & mask_addr) ;
			end
	end

    assign addr_out[31:(32-decode_len)] = addr_in_combine | tran_addr_combine ;
    assign addr_out[(31-decode_len):0] = addr_in [(31-decode_len):0] ;
`else
    assign addr_out = addr_in ;
`endif

endmodule

