//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name "bus_commands.v"                                  ////
////                                                              ////
////  This file is part of the "PCI bridge" project               ////
////  http://www.opencores.org/cores/pci/                         ////
////                                                              ////
////  Author(s):                                                  ////
////      - Miha Dolenc (mihad@opencores.org)                     ////
////                                                              ////
////  All additional information is avaliable in the README.pdf   ////
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
// $Log: bus_commands.v,v $
// Revision 1.4  2002/08/22 13:28:05  mihad
// Updated for synthesis purposes. Gate level simulation was failing in some configurations
//
// Revision 1.3  2002/02/01 15:25:12  mihad
// Repaired a few bugs, updated specification, added test bench files and design document
//
// Revision 1.2  2001/10/05 08:14:28  mihad
// Updated all files with inclusion of timescale file for simulation purposes.
//
// Revision 1.1.1.1  2001/10/02 15:33:47  mihad
// New project directory structure
//
//

// definitions of PCI bus commands | used by PCI Master | used by PCI Target
`define BC_IACK             4'h0  //				yes					no
`define BC_SPECIAL          4'h1  //				no					no
`define BC_IO_READ          4'h2  //				yes					yes
`define BC_IO_WRITE         4'h3  //				yes					yes
`define BC_RESERVED0        4'h4  //				no					no
`define BC_RESERVED1        4'h5  //				no					no
`define BC_MEM_READ         4'h6  //				yes					yes
`define BC_MEM_WRITE        4'h7  //				yes					yes
`define BC_RESERVED2        4'h8  //				no					no
`define BC_RESERVED3        4'h9  //				no					no
`define BC_CONF_READ        4'hA  //				yes					yes
`define BC_CONF_WRITE       4'hB  //				yes					yes
`define BC_MEM_READ_MUL     4'hC  //				yes					yes
`define BC_DUAL_ADDR_CYC    4'hD  //				no					no
`define BC_MEM_READ_LN      4'hE  //				yes					yes
`define BC_MEM_WRITE_INVAL  4'hF  //				no					yes

// common bits for configuration cycle commands
`define BC_CONF_RW 3'b101 
// common bits for io cycle commands
`define BC_IO_RW 3'b001
